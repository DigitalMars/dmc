/*_ mktemp.c	*/
/* Copyright (C) 1991 by Digital Mars	*/
/* All Rights Reserved			*/
/* Written by Walter Bright		*/

#include <tchar.h>
#include <sctchar.h>
#include <stddef.h>
#include <io.h>
#include <errno.h>
#include <process.h>
#if M_UNIX || M_XENIX
#include <sys/types.h>
#include <unistd.h>
#else
#include <dos.h>
#endif
#include <stdlib.h>

/***************************
 * Generate a unique filename.
 * Input:
 *	template	Pointer to a string that ends with six X's.
 *			The X's are replaced with the process's PID
 *			and a letter, until a unique filename is
 *			generated. 26 filenames are tried.
 *			The non-path portion of template should be
 *			no longer than 8 characters, including the
 *			6 X's.
 *			The original string is modified.
 * Returns:
 *	template if successful.
 *	NULL if unsuccessful (a unique filename was not found or
 *	template did not end in 6 X's).
 */

_TCHAR *_tmktemp(_TCHAR *template)
{   int templen;
    _TCHAR *p;

    templen = 0;
#ifdef _MBCS
    for (p = template; *p != '\0'; p++) {
        templen = (*p == _T('X')) ? templen + 1 : 0;
        if (_istlead (*p)) {
            p++;
            if (*p == '\0') {
                break;
            }
        }
    }
#else
    for (p = template; *p; p++)
	templen = (*p == _T('X')) ? templen + 1 : 0;
#endif
    if (templen < 6)			/* must be at least 6 'X's	*/
	template = NULL;		/* invalid format		*/
    else
    {	unsigned pid;
	int i;
	_TCHAR c;
	int errnosave, doserrnosave;

#if MSDOS
	pid = _psp;
#else
	pid = getpid();			/* get unique starting point	*/
#endif
	for (i = 0; i < 6; i++)
	{   *--p = (pid % 10) + _T('0');
	    pid /= 10;
	}

	c = _T('a') - 1;
	errnosave = errno;
	doserrnosave = _doserrno;
	while (_taccess(template,0) == 0 || errno == EACCES)
	{
	    *p = ++c;
	    if (c > _T('z'))
	    {	template = NULL;
		break;
	    }
	}
	errno = errnosave;
	_doserrno = doserrnosave;
    }
    return template;
}

