/*_ locking.c   Modified by: Walter Bright
 * Copyright (C) 1991 by Digital Mars
 * All rights reserved.
 * Written by Walter Bright
 */

#include	<stddef.h>
#include	<io.h>
#include	<sys\locking.h>
#include	<errno.h>
#include	<scrtl.h>


#ifdef __OS2__
#include "os2lib.h"
#endif
#ifdef __NT__
#include <windows.h>
extern void *_osfhnd[];
#endif


/*************************************
 * Locking function
 * Input:
 *	fd	file handle
 *	mode	LK_XXXX from sys\locking.h
 *	size	length in bytes
 */

int locking(int fd, int mode, long size)
{
#if __NT__
    unsigned count = 10;
    DWORD where;

    where = SetFilePointer (_osfhnd[fd], 0, 0, FILE_CURRENT);
    if (where != -1)
    {
	if (mode == LK_UNLCK)
	{
	    if (UnlockFile (_osfhnd[fd], where, 0, size, 0))
		return 0;
	}
	else
	{
	    while (1)
	    {
		/* Lock the area	*/
		if (LockFile (_osfhnd[fd], where, 0, size, 0))
		    return 0;
		if (mode != LK_LOCK && mode != LK_RLCK)
		    break;

		if (!--count)			/* if out of retries		*/
		{
		    __set_errno (EDEADLOCK);
		    return -1;
		}
		Sleep(1000);			/* wait a second and try again	*/
	    }
	}
    }

    __set_nt_errno ();
    return -1;

#else
    unsigned count = 10;
    FILELOCK filelock;
    int result;
#if __I86__ >= 3
    static FILELOCK unused;
#endif

    /* Need to do an LSEEK to find out where we are	*/
#if __I86__ >= 3
    result = DosSetFilePtr(fd,0,1,&filelock.offset);
#else
    result = DosChgFilePtr(fd,0,1,&filelock.offset);
#endif
    if (result)
	goto err;

    filelock.range = size;
    if (mode == LK_UNLCK)
    {
	/* Unlock the area	*/
#if __I86__ >= 3
	result = DosSetFileLocks(fd,&filelock,&unused,0,0);
#else
	result = DosFileLocks(fd,&filelock,NULL);
#endif
    }
    else
    {
#if __I86__ >= 3
	unsigned timeout;

	timeout = (mode == LK_LOCK || mode == LK_RLCK)
		? 1000 : 0;
	result = DosSetFileLocks(fd,&unused,&filelock,timeout,0);
#else
	while (1)
	{
	    /* Lock the area	*/
	    result = DosFileLocks(fd,NULL,&filelock);
	    if (result == 0)
		break;
	    if (mode != LK_LOCK && mode != LK_RLCK)
		break;

	    if (!--count)			/* if out of retries		*/
	    {   result = EDEADLOCK;
		goto err;
	    }
	    DosSleep(1000);			/* wait a second and try again	*/
	}
#endif
    }

    if (result)
    {
     err:
	__set_errno (result);
	result = -1;
    }
    return result;
#endif
}
