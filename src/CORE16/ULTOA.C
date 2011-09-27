/*_ ultoa.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<string.h>

char *ultoa(unsigned long u, char *a, int base)
{
	char buf[sizeof(long) * 8 + 1];
	char __ss *p;
	char *q = a;

	if ((base < 2) || (base > 36))
	{
		a[0] = 0;
		return a;
	}

	buf[sizeof(buf) - 1] = 0;
	p = &buf[sizeof(buf) - 2];	    /* last character position	    */
	while (1)
	{
	    int n;

	    n	= u % base;
	    *p	= n + ((9 < n) ? ('A' - 10) : '0');
	    if ((u /= base) == 0)
		break;
	    p--;
	}
	memcpy(q, p, &buf[sizeof(buf)] - p);
	return a;
}

