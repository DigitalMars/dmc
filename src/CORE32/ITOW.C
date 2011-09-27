//_ itow.c
// Copyright (C) 1995-2002 by Digital Mars
// www.digitalmars.com
// All Rights Reserved

#ifdef __NT__

#include <stdio.h>
#include <stdlib.h>

wchar_t * __CLIB _itow(int value, wchar_t *s, int radix)
{
    int i, neg;
    wchar_t *p;
    wchar_t *q;

    /* a few early bailouts */
    if (!s)
    {
	return (wchar_t*)0;
    }

    if ((radix < 2) || (radix > 36))
    {
	*s = 0;
	return s;
    }

    if (value == 0)
    {
	s[0] = '0';
	s[1] = '\0';
	return s;
    }

    neg = 0;
    if (value < 0 && radix == 10)	// only provides a negative value for base 10
    {
	neg = 1;
	value = -value;
    }

    /*
     * Write the digits from high to low initially, we'll reverse the string at
     * the end.
     */
    p = s;
    while (value)
    {
	i = (unsigned)value % radix;
	if(i > 9)
	{
	    i += 7;
	}
	*p++ = '0' + i;
	value = (unsigned)value / radix;
    }

    if (neg)
    {
	*p++ = '-';
    }

    *p = '\0';

    // Reverse
    q = s;
    while (--p > q)
    {	wchar_t c;

	c = *q;
	*q = *p;
	*p = c;
	q++;
    }

    return s;
}

#endif

/**/
