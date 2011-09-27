/*_ mbyte.c   Mon Mar 11 1991   Modified by: Walter Bright */
/* Copyright (C) 1991 by Walter Bright		*/
/* All Rights Reserved				*/
/* Written by Walter Bright			*/

#ifndef __NT__

#include <stdlib.h>

/*******************************
 */

int mblen(const char *s, size_t n)
{   int result = 0;

    if (s && *s)
    {
	result = n ? 1 : -1;
    }
    return result;
}

/*******************************
 */

int mbtowc(wchar_t *pwc, const char *s, size_t n)
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

int wctomb(char *s, wchar_t wchar)
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

size_t wcstombs(char *s, const wchar_t *pwcs, size_t n)
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

#endif

