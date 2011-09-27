
/*_ frexpl.c
 * Copyright (C) 2003 by Digital Mars
 * All rights reserved.
 * www.digitalmars.com
 * Written by Walter Bright
 */


#include <stdio.h>
#include <math.h>
#include <limits.h>

/******************************************
 * Returns:
 *	x such that value=x*2**n, .5 <= |x| < 1.0
 *	x has same sign as value.
 *	*eptr = n
 *
 *	Special cases:
 *		value	  x	*eptr
 *		+-0.0	+-0.0	  0
 *		+-inf	+-inf	  INT_MAX/INT_MIN
 *		+-NaN	+-NaN	  LONG_MIN
 *		+-NaNs	+-NaN	  LONG_MIN
 */


long double frexpl(long double value,int *eptr)
{
    unsigned short *vu = (unsigned short *)&value;
    long long *vl = (long long *)&value;
    unsigned exp;

    // If exponent is non-zero
    exp = vu[4] & 0x7FFF;
    if (exp)
    {
	if (exp == 0x7FFF)
	{   // infinity or NaN
	    if (*vl &  0x7FFFFFFFFFFFFFFF)	// if NaN
	    {	*vl |= 0xC000000000000000;	// convert NANS to NANQ
		*eptr = LONG_MIN;
	    }
	    else if (vu[4] & 0x8000)
	    {	// negative infinity
		*eptr = INT_MIN;
	    }
	    else
	    {	// positive infinity
		*eptr = INT_MAX;
	    }
	}
	else
	{
	    *eptr = exp - 0x3FFE;
	    vu[4] = (0x8000 & vu[4]) | 0x3FFE;
	}
    }
    else if (!*vl)
    {
	// value is +-0.0
	*eptr = 0;
    }
    else
    {	// denormal
	int i = -0x3FFD;

	do
	{
	    i--;
	    *vl <<= 1;
	} while (*vl > 0);
	*eptr = i;
        vu[4] = (0x8000 & vu[4]) | 0x3FFE;
    }
    return value;
}
