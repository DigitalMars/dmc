/*_ dosfil.c   Modified by: Walter Bright
 * Copyright (C) 1991 by Digital Mars
 * All rights reserved.
 * Written by Walter Bright
 */

#include	<dos.h>
#include	<errno.h>
#include	<scrtl.h>


#ifdef __OS2__
#include "os2lib.h"
#endif
#ifdef __NT__
#include <windows.h>
extern void *_osfhnd[];
#endif


/************************
 * Get default disk drive
 * 1 = A, 2 = B etc.
 */

void dos_getdrive(unsigned *drive)
{
#if __OS2__
	unsigned long map;		/* drive bitmap		*/

#if __I86__ >= 3
	DosQueryCurrentDisk(drive,&map);
#else
	DosQCurDisk(drive,&map);
#endif
#endif

#if __NT__
	char *currentdir = NULL;
	DWORD dir_len;
	DWORD result;

    currentdir = malloc(1);  
    if (!currentdir) {
        *drive = 0;
        goto end;
    }
    dir_len = GetCurrentDirectory(0,currentdir);
    currentdir = realloc(currentdir,dir_len+1);
    if (!currentdir) {
        *drive = 0;
        goto end;
    }
	result = GetCurrentDirectory(dir_len, currentdir);
	if (result < 2 || currentdir[1] != ':')
	{				/* not DOS drive letter - could be network but no way to map it */
	*drive = 0;
	goto end;
	}
	if (currentdir[0] <= 'Z')	/* handle upper or lower case */
	*drive = currentdir[0] - ('A' - 1);
	else
	*drive = currentdir[0] - ('a' - 1);
	end:
	   if (currentdir)
	     free(currentdir);
	   return;
#endif
}

/************************
 * Set default disk drive
 */

void dos_setdrive(unsigned drive, unsigned *no_of_drives)
{
#ifdef __OS2__
	unsigned long map;		/* drive bitmap		*/
	unsigned u;
	unsigned count;

#if __I86__ >= 3
	DosSetDefaultDisk(drive);
	DosQueryCurrentDisk(&drive,&map);
#else
	DosSelectDisk(drive);
	DosQCurDisk(&drive,&map);
#endif

	count = 0;
	for (u = 0; u < 26; u++)
	{	count += (unsigned)map & 1;
	map >>= 1;
	}
	*no_of_drives = count;
#endif
#ifdef __NT__
	char dir[4];
	char i;
	unsigned count;

	dir[0] = (char)drive + 'A' - 1;
	dir[1] = ':';
	dir[2] = 0;
	SetCurrentDirectory(dir);
	if (no_of_drives)
	{
	    count = 0;
	    dir[2] = '\\';
	    dir[3] = 0;
	    for (i = 'A'; i <= 'Z'; i++)
	    {   dir[0] = i;
		if ((unsigned)GetDriveType(dir) > 1)
		    count++;
	    }
	    *no_of_drives = count;
	}
#endif
}

/**************************
 * Get the file date/time.
 */

unsigned dos_getftime(int fd, unsigned *pdate, unsigned *ptime)
{
#ifdef __OS2__
	int result;
	FileInfoBuf fileinfo;

	/* get file info	*/
#if __I86__ >= 3
	result = DosQueryFileInfo(fd,1,&fileinfo,sizeof(FileInfoBuf));
#else
	result = DosQFileInfo(fd,1,&fileinfo,sizeof(FileInfoBuf));
#endif
	if (result)
	errno = result;
	else
	{
	*pdate = fileinfo.wdate;
	*ptime = fileinfo.wtime;
	}
	return result;
#endif
#ifdef __NT__
	FILETIME lastwrite;
	int result;

	result = GetFileTime (_osfhnd[fd], 0, 0, &lastwrite);
	if (result)
	{
	result = FileTimeToDosDateTime (&lastwrite, (LPWORD)pdate, (LPWORD)ptime);
	if (result)
		return 0;
	}
	return __set_nt_errno ();
#endif
}

/**************************
 * Set the file date/time.
 */

unsigned dos_setftime(int fd, unsigned date, unsigned time)
{
#ifdef __OS2__
	int result;
	FileInfoBuf fileinfo;

	/* get file info	*/
#if __I86__ >= 3
	result = DosQueryFileInfo(fd,1,&fileinfo,sizeof(FileInfoBuf));
#else
	result = DosQFileInfo(fd,1,&fileinfo,sizeof(FileInfoBuf));
#endif
	if (result)
	goto err;

	/* Set file info	*/
	fileinfo.wdate = date;
	fileinfo.wtime = time;
	result = DosSetFileInfo(fd,1,&fileinfo,sizeof(FileInfoBuf));
	if (result)
	{
	 err:
	errno = result;
	}

	return result;
#endif
#ifdef __NT__
	FILETIME ft;
	int result;

	result = DosDateTimeToFileTime ((WORD)date, (WORD)time, &ft);
	if (result)
	{
	result = SetFileTime (_osfhnd[fd], 0, 0, &ft);
	if (result)
		return 0;
	}
	return __set_nt_errno ();
#endif
}

/************************
 * Get the file attribute.
 */

unsigned dos_getfileattr(const char *path, unsigned *attribute)
{
	int result;
#ifdef __OS2__

#if __I86__ >= 3
	FileInfoBuf fileinfo;

	/* get file info	*/
	result = DosQueryPathInfo(path,1,&fileinfo,sizeof(FileInfoBuf));
	if (result)
	errno = result;
	else
	*attribute = fileinfo.attribute;
#else
	result = DosQFileMode(path,attribute,0L);
	if (result)
	errno = result;
#endif
	return result;
#endif
#ifdef __NT__
	result = GetFileAttributes ((LPTSTR)path);
	if (result != -1)
	{
	*attribute = result & (FA_RDONLY | FA_HIDDEN | FA_SYSTEM | FA_ARCH | FA_DIREC);
	return 0;
	}
	return __set_nt_errno ();
#endif
}

/************************
 * Set the file attribute.
 */

unsigned dos_setfileattr(const char *filepath, unsigned attribute)
{	int result;

#ifdef __OS2__

#if __I86__ >= 3
	FileInfoBuf fileinfo;

	/* get file info	*/
	result = DosQueryPathInfo(filepath,1,&fileinfo,sizeof(FileInfoBuf));
	if (result)
	goto err;

	/* Set file info	*/
	fileinfo.attribute = attribute;
	result = DosSetPathInfo(filepath,1,&fileinfo,sizeof(FileInfoBuf),0x10);
	if (result)
	{
	 err:
	errno = result;
	}
#else
	result = DosSetFileMode(filepath,attribute,0L);
	if (result)
	errno = result;
#endif
	return result;
#endif

#ifdef __NT__
	result = SetFileAttributes ((LPTSTR)filepath, attribute);
	if (result)
	return 0;
	return __set_nt_errno ();
#endif
}
