/*_ strerror.c   Fri May 12 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars  */
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* Modified for Unix by David Bustin                    */

#include	<stdio.h>
#include	<errno.h>
#include	<assert.h>
#include	<string.h>
#include	<stdlib.h>
#ifdef _WINDOWS
#include	<stdarg.h>
#include	<windows.h>
#endif

/*******************************
 * Map errnum into an error string.
 */

char *strerror(int errnum)
{
	static char buf[8 + sizeof(errnum) * 3 + 1],*p;
	int i;

    if (errnum < sys_nerr)
		p = sys_errlist[errnum];
    else
	    {   
#if M_UNIX || M_XENIX
		sprintf(buf,"Error %d",errnum);			/* default string       */
#else
		sprintf(buf,"errnum = %d",errnum);		/* default string       */
#endif
		p = buf;
	    }
    return p;
}

