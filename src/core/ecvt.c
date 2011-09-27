/*_ ecvt.c				*/
/* Copyright (C) 1986-1994 by Digital Mars	*/
/* All Rights Reserved			*/
/* $Revision: 1.1.1.1 $ 			*/

#include	<stdlib.h>
#include	<string.h>
#include	<float.h>
#include	<stdio.h>
#include	<fltpnt.h>
#include	<ctype.h>

#if _MT
#include	"mt.h"
#endif

#define DIGMAX	(LDBL_DIG*2)	/* max # of digits in string		*/
				/* (*2 is a good fudge factor)		*/
#define ECVT	0

#if !_MT
static char digstr[DIGMAX + 1 + 1];	/* +1 for end of string		*/
					/* +1 in case rounding adds	*/
					/* another digit		*/
#endif


char * __pascal __floatcvt(int cnvflag,char *digstr,long double val,int ndig,
	int *pdecpt,int *psign);

/*************************
 * Convert double val to a string of
 * decimal digits.
 *	ndig = # of digits in resulting string
 * Returns:
 *	*pdecpt = position of decimal point from left of first digit
 *	*psign  = nonzero if value was negative
 */

char *ecvt(double val,int ndig,int *pdecpt,int *psign)
{
#if _MT
    return __floatcvt(ECVT,_getthreaddata()->t_digstr,val,ndig,pdecpt,psign);
#else
    return __floatcvt(ECVT,digstr,val,ndig,pdecpt,psign);
#endif
}

