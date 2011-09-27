//_ strtof.c
// Copyright (C) 1985-2001 by Digital Mars
// All Rights Reserved

#include	<stdlib.h>
#include	<ctype.h>
#include	<float.h>
#include	<string.h>
#include	<math.h>
#include	<fltpnt.h>
#include        <scrtl.h>

/*************************
 * Convert string to float.
 * Terminates on first unrecognized character.
 */

float strtof(const char *p,char **endp)
{   int errnosave = errno;
    long double d;
    float f;

    errno = 0;
#if _WIN32
    d = strtold(p,endp);
#else
    d = strtod(p,endp);
#endif
    f = d;
    if (d != 0 && f == 0)		// if underflow converting to float
	__set_errno (ERANGE);
    else if (d != HUGE_VAL && f == HUGE_VAL)	// if infinity converting to float
	__set_errno (ERANGE);
    else if (errno != ERANGE)
    {
	errno = errnosave;
    }
    return d;
}
