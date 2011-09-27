/*_ ltoa.c	 Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1989 by Walter Bright	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* $Revision: 1.1.1.1 $ */

#include	<stdlib.h>

char *ltoa(long i, char *a, int base)
{
	if (base == 10 && i < 0)
	{
	    a[0] = '-';
	    return ultoa (-i, a+1, base) - 1;
	}
	else
	    return ultoa (i, a, base);
}
