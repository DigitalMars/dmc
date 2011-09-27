/*_ mbyte.c   Mon Mar 11 1991   Modified by: Walter Bright */
/* Copyright (C) 1991 by Walter Bright		*/
/* All Rights Reserved				*/
/* Written by Walter Bright			*/

#include <stdlib.h>

#if __INTSIZE == 2
#define __32BIT __far
#else
#define __32BIT
#endif

/*******************************
 */

int __32BIT _fmblen(const char __far *s, size_t n)
{   int result = 0;

    if (s && *s)
    {
	result = n ? 1 : -1;
    }
    return result;
}

/*******************************
 */

int __32BIT _fmbtowc(wchar_t __far *pwc, const char __far *s, size_t n)
{   int result = 0;

    if (s)
    {
/*	if (*pwc && n)
*/
	if (pwc)
	    *pwc = *s;
	if (*s)
	    result = n ? 1 : -1;
    }
    return result;
}

/*******************************
 */

int __32BIT _fwctomb(char __far *s, wchar_t wchar)
{
    if (s)
    {
	*s = wchar;
	return 1;
    }
    else
	return 0;
}

/*******************************
 */

size_t __32BIT _fmbstowcs(wchar_t __far *pwcs, const char __far *s, size_t n)
{   size_t i;

    for (i = 0; i < n; i++)
    {
	if ((*pwcs = *s) == 0)
	    break;
	pwcs++;
	s++;
    }
    return i;
}

/*******************************
 */

size_t __32BIT _fwcstombs(char __far *s, const wchar_t __far *pwcs, size_t n)
{   size_t i;

    for (i = 0; i < n; i++)
    {
	if ((*s = *pwcs) == 0)
	    break;
	pwcs++;
	s++;
    }
    return i;
}

