/*_ stat.c   Modified by: Walter Bright */
/* OS2 support added by Nikki Locke May 1989 */
/* Copyright (C) 1987-1995 by Digital Mars	*/
/* All Rights Reserved			*/
/* Written by Walter Bright		*/

#include	<tchar.h>
#include	<sys/stat.h>
#include	<dos.h>
#include	<string.h>
#include	<errno.h>
#include	<time.h>
#include	<io.h>
#include	<direct.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<scrtl.h>
#include	<sctchar.h>

#ifdef _WIN32
#include <scio.h>
#endif

#define OFFSET_68 378691200
#define SECSPERHOUR (60*60L)
#define SECSPERDAY      (SECSPERHOUR*24L)

/* OFFSET_70 is OFFSET_68 - Seconds in 69 and 68, taking into account that 68 was a leap year */
#define OFFSET_70 (((OFFSET_68) - (SECSPERDAY)) - ((SECSPERDAY * 365L) * 2))


#if __OS2__
#include "os2lib.h"
#endif
#if _WIN32
#include <windows.h>
#include <lcapi32.h>
#endif


#if _WIN32
#if _UNICODE
static time_t near __pascal _filetime(FILETIME *st);
#else
time_t near __pascal _filetime(FILETIME *st);
#endif
#else
static time_t near pascal _filetime(unsigned date,unsigned time);
#endif

#ifndef _UNICODE

/**************************
 * Determine information about an open file, given its file handle.
 * Returns:
 *	0	success (*pstat is filled in)
 *	-1	failure (errno is set, *pstat is garbage)
 */

int fstat(int fd,struct stat *pstat)
{
#if defined(_WIN32)
        int mode;
        int result;
        FILETIME ft;
        DWORD attr;

        memset(pstat,0,sizeof(*pstat));	/* zero out all fields		*/
        pstat->st_nlink = 1;		/* NT doesn't support links	*/
        if (GetFileType (_osfhnd[fd]) != FILE_TYPE_DISK)
        {					/* character device or pipe 	*/
                mode = S_IFCHR | S_IREAD | S_IWRITE;
                pstat->st_rdev = pstat->st_dev = fd;
        }
        else				/* else regular file		*/
        {
                if (!GetFileTime (_osfhnd[fd], NULL, NULL, &ft))
                {
                        __set_nt_errno ();
                        return -1;
                }
                pstat->st_mtime = pstat->st_ctime = pstat->st_atime =
                _filetime(&ft);
                if (!pstat->st_mtime)		/* if error converting time	*/
                        return -1;

                pstat->st_rdev = pstat->st_dev = 0;
                mode = (S_IFREG | S_IREAD | S_IWRITE);		/* cannot get sttributes of file without its name */
                pstat->st_size = GetFileSize (_osfhnd[fd], NULL);
        }

#elif defined(__OS2__)
        unsigned HandType, FlagWord;
        int mode;
        int result;
        FileInfoBuf fileinfo;

#if __INTSIZE == 4
        if ((result = DosQueryHType(fd,&HandType,&FlagWord)) != 0)
#else
        if ((result = DosQHandType(fd,&HandType,&FlagWord)) != 0)
#endif
        {
    err:
	        __set_errno (result);
	        return -1;
        }
        memset(pstat,0,sizeof(*pstat));	/* zero out all fields		*/
        pstat->st_nlink = 1;		/* OS/2 doesn't support links	*/
        if (HandType & 0xFF)		/* if character device or pipe	*/
        {
                mode = S_IFCHR | S_IREAD | S_IWRITE;
                pstat->st_rdev = pstat->st_dev = fd;
        }
        else				/* else regular file		*/
        {
#if __INTSIZE == 4
	        result = DosQueryFileInfo(fd,1,&fileinfo,sizeof(fileinfo));
#else
        	result = DosQFileInfo(fd,1,&fileinfo,sizeof(fileinfo));
#endif
	        if (result)
        	    goto err;
	        pstat->st_rdev = pstat->st_dev = 0;
        	mode = (fileinfo.attribute & 1)?
		        (S_IFREG | S_IREAD):(S_IFREG | S_IREAD | S_IWRITE);
        	pstat->st_size = fileinfo.fsize;
	        pstat->st_mtime = pstat->st_ctime = pstat->st_atime =
	                _filetime(fileinfo.wdate,fileinfo.wtime);
        }
#else
        union REGS regs;
        int mode;

        /* Do an IOCTL on the handle	*/
        regs.x.ax = 0x4400;			/* get device information	*/
        regs.x.bx = fd;
        intdos(&regs,&regs);
        if (regs.x.cflag)			/* if carry is set		*/
        {
                __set_dos_errno (regs.x.ax);
        	return -1;
        }
        memset(pstat,0,sizeof(*pstat));	/* zero out all fields		*/
        pstat->st_nlink = 1;		/* MS-Dos doesn't support links	*/
        if (regs.x.dx & 0x80)		/* if device			*/
        {
	        mode = S_IFCHR | S_IREAD | S_IWRITE;
        	pstat->st_rdev = pstat->st_dev = fd;
        }
        else				/* else regular file		*/
        {
                long curpos;

                pstat->st_rdev = pstat->st_dev = regs.x.dx & 0x3F;
                mode = S_IFREG | S_IREAD;

                /* Determine file size using lseek() */
                curpos = lseek(fd,0L,1);
                pstat->st_size = lseek(fd,0L,2);
                lseek(fd,curpos,0);

                regs.x.ax = 0x5700;		/* get file's date and time	*/
                intdos(&regs,&regs);
                pstat->st_mtime = pstat->st_ctime = pstat->st_atime =
                _filetime(regs.x.dx,regs.x.cx);
        }
#endif
        /* Fake group and world bits to be same as user bits	*/
        pstat->st_mode = mode | ((mode & 0700) >> 3) | ((mode & 0700) >> 6);
        return 0;
}

#endif

/**************************
 * Determine information about a file, given its name.
 * Returns:
 *	0	success (*pstat is filled in)
 *	-1	failure (errno is set, *pstat is garbage)
 */

int _tstat(const _TCHAR *filespec,struct stat *pstat)
{
#if defined(_WIN32)
        HANDLE FindHandle;
        WIN32_FIND_DATA find;
#else
        struct FIND *find;
#endif
        unsigned char attr;
        int mode;
        _TCHAR *ext;

        if (_tcspbrk(filespec,_T("*?")))
        {
                __set_errno (ENOENT);
                goto err;
        }
#if defined(_WIN32)
        FindHandle = __tFindFirstFile ((LPTSTR)filespec, &find);
        if (FindHandle == INVALID_HANDLE_VALUE)
#else
        find = findfirst(filespec,FA_DIREC | FA_SYSTEM | FA_HIDDEN);
        if (!find)
#endif
        {
                /* Could be the root directory. Easy to find out, just see	*/
                /* if it can be switched to.					*/
#if defined(__OS2__) || defined(_WIN32)
                _TCHAR *cwd;

                /* Dynamically allocate cwd so we're prepared for longer file names */
                cwd = _tgetcwd(NULL,0);		/* determine if drive exists	*/
                if (!cwd)
                        goto err;
                if (_tchdir(filespec) == -1)	/* the proof is if we can switch to it */
                {
                        free(cwd);
                        goto err;
                }
                _tchdir(cwd);			/* restore original directory	*/
                free(cwd);
#else
                _TCHAR cwd[3+64];			/* max possible directory size	*/

                if (!_tgetcwd(cwd,sizeof(cwd)))	/* determine if drive exists	*/
                        goto err;
                if (_tchdir(filespec) == -1)	/* the proof is if we can switch to it */
                        goto err;
                _tchdir(cwd);			/* restore original directory	*/
#endif

                attr = FA_DIREC;
                memset(pstat,0,sizeof(*pstat));	/* zero most fields	*/
                pstat->st_atime = pstat->st_ctime = pstat->st_mtime =
#if defined(_WIN32)
                    -1;
#else
                _filetime(0x21,0);          /* Jan 1, 1980, midnight */
#endif
        }
        else
        {
                memset(pstat,0,sizeof(*pstat));	/* zero most fields		*/
#if defined(_WIN32)
                pstat->st_size = find.nFileSizeLow;
                /* BUG: no check for if _filetime() fails */
                pstat->st_atime = _filetime(&find.ftLastAccessTime);
                pstat->st_ctime = _filetime(&find.ftCreationTime);
                pstat->st_mtime = _filetime(&find.ftLastWriteTime);
                attr = find.dwFileAttributes;
                FindClose (FindHandle);
#else
                pstat->st_size = find->size;
                attr = find->attribute;
                pstat->st_atime = pstat->st_ctime = pstat->st_mtime =
                _filetime(find->date,find->time);
#endif
        }

        /* Determine mode bits	*/
        if (attr & FA_DIREC)	/* if directory			*/
                mode = S_IFDIR | S_IEXEC | S_IREAD;
        else
        {
                mode = S_IFREG | S_IREAD;
                /* Determine if file is executable */
                ext = _tcsrchr(filespec,_T('.'));
                if (ext)
                {
                        if (_tcsicmp(ext,_T(".exe")) == 0 ||
#if defined(__OS2__) || defined(_WIN32)
                            _tcsicmp(ext,_T(".cmd")) == 0 ||
#endif
#if !defined(__OS2__)
                            _tcsicmp(ext,_T(".bat")) == 0 ||
#endif
                            _tcsicmp(ext,_T(".com")) == 0)
                                mode |= S_IEXEC;
                }
        }
        if (!(attr & FA_RDONLY))		/* if not a read-only file	*/
                mode |= S_IWRITE;		/* then it's writable		*/
        /* Fake group and world bits to be same as user bits	*/
        pstat->st_mode = mode | ((mode & 0700) >> 3) | ((mode & 0700) >> 6);

        pstat->st_nlink = 1;

        /* Determine drive number	*/
#if defined(_WIN32)
        if(filespec[1] == _T(':'))		/* if explicit drive		*/
                pstat->st_dev = pstat->st_rdev = _totupper(filespec[0]) - _T('A');
        else
        {
                pstat->st_dev = pstat->st_rdev = _getdrive (); /* default drive */
        }

#elif defined(__OS2__)
        if(filespec[1] == _T(':'))		/* if explicit drive		*/
                pstat->st_dev = pstat->st_rdev = _totupper(filespec[0]) - _T('A');
        else
        {
                unsigned long LogicalDriveMap;
                unsigned DriveNumber;
                int result;

#if __INTSIZE == 4
                result = DosQueryCurrentDisk(&DriveNumber,&LogicalDriveMap);
#else
                result = DosQCurDisk(&DriveNumber,&LogicalDriveMap);
#endif
                if (result)
                {
                        __set_errno (result);
                        goto err;
                }
                pstat->st_dev = pstat->st_rdev = DriveNumber;/* default drive */
        }
#else
        pstat->st_dev = pstat->st_rdev =
                (filespec[1] == _T(':'))		/* if explicit drive		*/
                ? toupper(filespec[0]) - _T('A')
                : (_TCHAR)bdos(0x19,0,0);		/* else default drive		*/
#endif

        return 0;

err:
        return -1;
}

#if defined(_WIN32)
#if _UNICODE
static time_t near __pascal _filetime(FILETIME *pft)
#else
time_t near __pascal _filetime(FILETIME *pft)
#endif
{
	SYSTEMTIME st;
	FILETIME ft;
	struct tm dt;

	if (!FileTimeToLocalFileTime (pft, &ft) ||
	    !FileTimeToSystemTime (&ft, &st))
	{
		__set_nt_errno ();
		return 0;
	}

        dt.tm_sec = st.wSecond;
        dt.tm_min = st.wMinute;
        dt.tm_hour = st.wHour;
        dt.tm_mday = st.wDay;
        dt.tm_mon = st.wMonth;
        dt.tm_year = st.wYear;

        return __localstructtmtotime_t(&dt);
}

#else

static time_t near pascal _filetime(unsigned date,unsigned time)
{
        struct tm dt;

        dt.tm_sec = (time & 0x1F) * 2;  /* time is in 2 second increments */
        dt.tm_min = (time >> 5) & 0x3F;
        dt.tm_hour = (time >> 11) & 0x1F;
        dt.tm_mday = date & 0x1F;
        dt.tm_mon = ((date >> 5) & 0x0F);
        dt.tm_year = ((date >> 9) & 0x7f) + 1980;  /* localstructtmtotime_t wants year in terms of 1900 */

        return __localstructtmtotime_t(&dt);
}

#endif

