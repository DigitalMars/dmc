/*_ dup.c	 Modified by: Walter Bright */
/*
 * Copyright (C) 1991 by Digital Mars
 * All rights reserved.
 * Written by Walter Bright
 */

#include	<io.h>
#include	<errno.h>
#include	"mt.h"
#include	"semlock.h"
#include	<scrtl.h>

#ifdef __OS2__
#include "os2lib.h"
#endif
#ifdef __NT__
#include <windows.h>
extern void *_osfhnd[];
#endif


/************************************
 * Returns:
 *	handle	success
 *	-1	error (errno is set)
 */

int dup(int fd)
{    int result;
#ifdef __OS2__
    int fd2 = -1;

    result = DosDupHandle(fd,&fd2);
    if (result)				/* if error		*/
    {	__set_errno (result);
	result = -1;
    }
    else
	result = fd2;
    return result;
#endif
#ifdef __NT__
    HANDLE proc;
    HANDLE h2;
    int fd2;

    proc = GetCurrentProcess ();

    /* Find an unused file handle to use */
	 LockSemaphore(_semIO)
    for (fd2 = 0; 1; fd2++)
    {
	if (fd2 == _NFILE)		/* out of file handles	*/
	{
	    __set_errno (EMFILE);
	    fd2 = -1;
	    goto done;
	}
	if (_osfhnd[fd2] == 0)
	    break;
    }

    result = DuplicateHandle (proc, _osfhnd[fd], proc, &h2, 0, TRUE, DUPLICATE_SAME_ACCESS);
    if (!result)
    {	__set_nt_errno ();
	fd2 = -1;
    }
    else
	_osfhnd[fd2] = h2;
done:
    UnlockSemaphore(_semIO)
    return fd2;
#endif
}

/************************************
 * Returns:
 *	0	success
 *	-1	error (errno is set)
 */

int dup2(int fd1, int fd2)
{   int result;

#ifdef __OS2__
    result = DosDupHandle(fd1,&fd2);
    if (result)				/* if error		*/
    {	__set_errno (result);
	result = -1;
    }
    return result;
#endif
#ifdef __NT__
    HANDLE proc;

    proc = GetCurrentProcess ();
    result = DuplicateHandle (proc, _osfhnd[fd1], proc, &_osfhnd[fd2], 0, 
				TRUE, DUPLICATE_SAME_ACCESS);
    if (result)
	return 0;
    __set_nt_errno ();
    return -1;
#endif
}
