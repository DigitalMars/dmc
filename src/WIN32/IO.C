/*_ io.c	Modified by: Walter Bright
 * Copyright (C) 1991 by Digital Mars
 * All Rights Reserved
 * Written by Walter Bright
 */

#include <tchar.h>
#include <sctchar.h>
#include <stdio.h>
#include <io.h>
#include <errno.h>
#include <share.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <stdarg.h>
#include "mt.h"
#include "semlock.h"
#include <scrtl.h>
#include <scio.h>

#ifdef __OS2__
#include <os2lib.h>
#endif
#ifdef __NT__
#include <windows.h>
#include <lcapi32.h>
#endif

int __cdecl _fmode;

#ifndef _UNICODE
unsigned char __fhnd_info[_NFILE] =
{
  FHND_DEVICE | FHND_TEXT,
  FHND_DEVICE | FHND_TEXT,
  FHND_DEVICE | FHND_TEXT
};
#endif

#if __NT__

#ifndef _UNICODE

/**********************
 * Initialize the translation table between
 * NT file handles and unix-style file handles for stdin, stdout, & stderr
 */

void *_osfhnd[_NFILE] = 
{
    0
};

#if _NOCTOR
void _sti_init_std_files()
#else
void _STI_init_std_files()
#endif
{
    _osfhnd[0] = GetStdHandle(STD_INPUT_HANDLE);
    _osfhnd[1] = GetStdHandle(STD_OUTPUT_HANDLE);
    _osfhnd[2] = GetStdHandle(STD_ERROR_HANDLE);
}


void _STD_init_std_files()
{
}

#endif

#endif

/****************************
 * Open a file
 * Returns:
 *	-1	error
 *	fd	file handle
 */

int _topen(const _TCHAR *name,int rwmode,... /* pmode */ )
{
    va_list marker;
    
    va_start (marker, rwmode);
    return _tsopen(name,rwmode,SH_DENYNO,va_arg (marker, int));
}

/****************************
 */

int _tsopen(const _TCHAR *name, int rwmode, int shmode, ... /* pmode */)
{
#if __NT__
    int pmode;
    int open_flag;
    unsigned fd;
    DWORD access, share;
    HANDLE h;

    rwmode = (rwmode & 0xFF8F) | (shmode & 0x70);

    if (rwmode & O_RDWR)
	access = GENERIC_READ | GENERIC_WRITE;
    else if (rwmode & O_WRONLY)
	access = GENERIC_WRITE;
    else
	access = GENERIC_READ;

    switch (rwmode & 0x00f0)
    {
	case SH_COMPAT:
	case SH_DENYRW:
	    share = 0;
	    break;
	case SH_DENYWR:
	    share = FILE_SHARE_READ;
	    break;
	case SH_DENYRD:
	    share = FILE_SHARE_WRITE;
	    break;
	case SH_DENYNO:
	    share = FILE_SHARE_READ | FILE_SHARE_WRITE;
	    break;
    }

    pmode = *((int __ss *)&shmode + 1) & ~__umask_pmode;
    if (pmode & S_IWRITE)		/* any WRITE permissions set?	*/
	pmode = FILE_ATTRIBUTE_NORMAL;		/* normal attribute		*/
    else
	pmode = FILE_ATTRIBUTE_READONLY;	/* read only attribute		*/

    if (rwmode & O_CREAT)
    {
	if ((rwmode & O_EXCL))
	{
     #if 0  /* VC does not treat this as an error */
	    if (rwmode & O_TRUNC)
	    {
		/* O_EXCL and O_TRUNC is an error	*/
	 	__set_errno (EINVAL);
	 	return -1;
	    }
     #endif
	    open_flag = CREATE_NEW;		/* fail is exist, create if not */
	}
	else if (rwmode & O_TRUNC)
	     open_flag = CREATE_ALWAYS;		/* replace if exist, create if not exist */
	else
	    open_flag = OPEN_ALWAYS;		/* open if exist, create if not exist	*/
    }
    else if (rwmode & O_TRUNC)
	open_flag = TRUNCATE_EXISTING;		/* replace if exists, fail if not */
    else
	open_flag = OPEN_EXISTING;

    /* Find an unused file handle to use */
	 LockSemaphore(_semIO)
    for (fd = 3; 1; fd++)
    {
	if (fd == _NFILE)		/* out of file handles	*/
	{
	    __set_errno (EMFILE);
	    fd = -1;
	    goto done;
	}
	if (_osfhnd[fd] == 0)
	    break;
    }

    h = __tCreateFile((LPTSTR)name,access,share,NULL,open_flag,pmode,0);
    __fhnd_info[fd] = 0;
    if (h == (HANDLE)-1)
    {   __set_nt_errno ();
        fd = -1;
    }
    else
    {
	_osfhnd[fd] = h;
	if (rwmode & O_APPEND)		/* if append to file		*/
	    __fhnd_info[fd] |= FHND_APPEND;
	if (isatty(fd))
	   __fhnd_info[fd] |= FHND_DEVICE;
	if (rwmode & O_TEXT)
	   __fhnd_info[fd] |= FHND_TEXT;
	else if ((rwmode & O_BINARY) == 0 &&
                 (_fmode & O_BINARY) == 0)
	   __fhnd_info[fd] |= FHND_TEXT;
    }
done:
    UnlockSemaphore(_semIO)
    return fd;

#else
    int pmode;
    int open_flag;
    int attribute;			/* file attribute		*/
    unsigned fd;
    unsigned action;
    int result;

    rwmode = (rwmode & 0xFF8F) | (shmode & 0x70);

    if (rwmode & O_CREAT)
    {
	open_flag = 0b10001;		/* open if exist, create if not exist	*/
	if (!(rwmode & O_EXCL))
	{
	    if (rwmode & O_TRUNC)
		open_flag++;		/* replace if exist, create if not exist */
	}
	else if (rwmode & O_TRUNC)
	{
	    /* O_EXCL and O_TRUNC is an error	*/
	    __set_errno (EINVAL);
	    return -1;
	}
	else
	    open_flag--;		/* fail if exist, create if not exist	*/

	pmode = *((int __ss *)&shmode + 1);
#if 1
	if (pmode & S_IWRITE)		/* any WRITE permissions set?	*/
	    attribute = 0;		/* normal attribute		*/
	else
	    attribute = 1;		/* read only attribute		*/
#else
	attribute = 0;			/* normal attribute		*/
#endif
    }
    else
    {
	attribute = 0;
	open_flag = 1;			/* open if exists, fail if not	*/
	if (rwmode & O_TRUNC)
	    open_flag++;		/* replace if exists, fail if not */
    }
    result = DosOpen(name,&fd,&action,0L,attribute,open_flag,rwmode & 0xF3,0L);
    if (result)
    {   __set_errno (result);
	fd = -1;
    }
    else
    {
	if (rwmode & O_APPEND)		/* if append to file		*/
	    lseek(fd,0L,SEEK_END);	/* position at end of file	*/
    }
    return fd;
#endif
}

/**************************
 * Create a file
 * Returns:
 *	-1	error
 *	fd	file handle
 */

int _tcreat(const _TCHAR *name,int pmode)
{
    return _topen(name,O_CREAT | O_TRUNC | O_RDWR,pmode);
}

#ifndef _UNICODE

/********************
 * Close a file.
 * Returns:
 *	-1	error
 *	0	successful
 */

int close(int fd)
{
#if __NT__

    /* NOTE:  io_semaphore is not used */
    HANDLE h = _osfhnd[fd];

   /* validate fd to avoid obvious problems... */
   if (fd < 0 || fd >= _NFILE)
   {
       __set_errno (EBADF);
       return -1;
   }

    __fhnd_info[fd] = 0;
    _osfhnd[fd] = 0;
    if (CloseHandle(h))
	return 0;
    __set_nt_errno ();
    return -1;
#else
    int result;
    
    __fhnd_info[fd] = 0;
    result = DosClose(fd);
    if (result)
    {	__set_errno (result);
	result = -1;
    }
    return result;
#endif
}

#endif

/*****************************
 * Rename a file.
 * Returns:
 *	-1	error
 *	0	successful
 */

int _trename(const _TCHAR *from,const _TCHAR *to)
{
#if __NT__
    if (__tMoveFile ((LPTSTR)from, (LPTSTR)to))
	return 0;
    __set_nt_errno ();
    return -1;

#else
    int result;

#if __I86__ >= 3
    result = DosMove(from,to);
#else
    result = DosMove(from,to,0L);
#endif
    if (result)
    {	__set_errno (result);
	result = -1;
    }
    return result;
#endif
}


/*****************************
 * Delete a file.
 * Returns:
 *	-1	error
 *	0	successful
 */

int _tremove(const _TCHAR *name)
{
#if __NT__
    if (__tDeleteFile ((LPTSTR)name))
	return 0;
    __set_nt_errno ();
    return -1;

#else
    int result;

#if __I86__ >= 3
    result = DosDelete(name);
#else
    result = DosDelete(name,0L);
#endif
    if (result)
    {	__set_errno (result);
	result = -1;
    }
    return result;
#endif
}

#ifndef _UNICODE

/*****************************
 * Set handle count.
 * Returns:
 *	-1	error
 *	0	successful
 */

int _dos_sethandlecount(unsigned nhandles)
{
#if __NT__
    if (SetHandleCount (nhandles) == nhandles)
	return 0;
    __set_errno (ENOMEM);
    return -1;

#else
    int result;

    result = DosSetMaxFH(nhandles);
    if (result)
    {	__set_errno (result);
	result = -1;
    }
    return result;
#endif
}

#endif

#ifndef _UNICODE

/*****************************
 * Seek to specified file position.
 * Input:
 *	mode =	SEEK_SET or SEEK_CUR or SEEK_END
 * Returns:
 *	-1L	error
 *	n	new file position
 */

long lseek(int fd,long offset,int mode)
{
    unsigned long newpos;

#if __NT__
    newpos = SetFilePointer (_osfhnd[fd], offset, NULL, mode);
    if (newpos == -1)
	__set_nt_errno ();
   return newpos;

#else

    int result;

#if __I86__ >= 3
    result = DosSetFilePtr(fd,offset,mode,&newpos);
#else
    result = DosChgFilePtr(fd,offset,mode,&newpos);
#endif
    if (result)
    {	__set_errno (result);
	newpos = -1;
    }
    return newpos;
#endif
}

#endif

/*****************************
 * Get and return the size of a file.
 * Use:
 *	long filesize(filename)
 *	char *filename *
 * Returns:
 *	-1L	error
 */

long __tfilesize(const _TCHAR *name)
{
#if __NT__

    WIN32_FIND_DATA filefindbuf;
    HANDLE findhndl;

    findhndl = __tFindFirstFile ((LPTSTR)name, &filefindbuf);
    if (findhndl == (HANDLE)-1)
    {
	__set_nt_errno ();
	return -1;
    }
    FindClose (findhndl);
    return filefindbuf.nFileSizeLow;

#else
    unsigned hdir;
    unsigned count;
    FILEFINDBUF3 filefindbuf;
    int result;

    hdir = -1;			/* create a new handle		*/
    count = 1;			/* number of entries to find	*/
    result = DosFindFirst(name,&hdir,6,&filefindbuf,sizeof(filefindbuf),&count,(__I86__ >= 3) ? 1L : 0L);
    if (result)
    {	__set_errno (result);
	filefindbuf.fsize = -1;
    }
    else
	DosFindClose(hdir);
    return filefindbuf.fsize;
#endif
}

/********************************
 * Synopsis:
 *	Test file for existence and its access bits. The access bits are:
 *	 F_OK	0	does file exist?
 *	 X_OK	1	execute permission?
 *	 W_OK	2	write permission?
 *	 R_OK	4	read permission?
 *	OR these values together to test a combination.
 * Bugs:
 *	Since MS-DOS and OS/2 only record a write permission bit, the X_OK
 *	and R_OK access permissions always succeed. They are included here
 *	for unix compatibility.
 * Returns:
 *	0	file exists and access mode is granted
 *	-1	otherwise and errno is set
 */

int _taccess(const _TCHAR *name,int mode)
{
#if __NT__
    DWORD result;

    result = __tGetFileAttributes ((LPTSTR)name);
    if (result == -1)
	__set_nt_errno ();
    else if ((mode & W_OK) && (result & FILE_ATTRIBUTE_READONLY))
    {
	__set_errno (EACCES);
	result = -1;
    }
    else
        result = 0;
    return result;

#else
    int result;

#if __I86__ >= 3
    unsigned hdir;
    FILEFINDBUF3 filefindbuf;
    unsigned count;

    hdir = -1;
    count = 1;
    result = DosFindFirst(name,&hdir,6,&filefindbuf,sizeof(filefindbuf),&count,(__I86__ >= 3) ? 1L : 0L);
    if (result)
    {	__set_errno (result);
	result = -1;
    }
    else
    {	DosFindClose(hdir);
	if (mode & W_OK && filefindbuf.attribute & 1)
	{   __set_errno (EACCES);
	    result = -1;
	}
    }
#else
    unsigned attribute;

    result = DosQFileMode(name,&attribute,0L);
    if (result)
    {	__set_errno (result);
	result = -1;
    }
    else if (mode & W_OK && attribute & 1)
    {	__set_errno (EACCES);
	result = -1;
    }
#endif
    return result;
#endif
}

