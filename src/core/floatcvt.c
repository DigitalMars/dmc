/*_ floatcvt.c					*/
/* Copyright (C) 1986-2001 by Digital Mars	*/
/* All Rights Reserved				*/
/* www.digitalmars.com				*/

#include	<stdlib.h>
#include	<string.h>
#include	<float.h>
#include	<stdio.h>
#include	<fp.h>
#include	<ctype.h>

#if _MT
#include	"mt.h"
#endif

#define DIGMAX	(LDBL_DIG*2)	/* max # of digits in string		*/
				/* (*2 is a good fudge factor)		*/

#define ECVT	0
#define FCVT	1


#if _WIN32
static long double negtab[] =
	{
#if _WIN32
	 1e-4096L,1e-2048L,1e-1024L,1e-512L,
#endif
	 1e-256L,1e-128L,1e-64L,1e-32L,1e-16L,1e-8L,1e-4L,1e-2L,1e-1L,1.0};
static long double postab[] =
	{
#if _WIN32
	 1e+4096L,1e+2048L,1e+1024L,1e+512L,
#endif
	 1e+256L,1e+128L,1e+64L,1e+32L,1e+16L,1e+8L,1e+4L,1e+2L,1e+1};
#else

static unsigned long hnegtab[][2] =
{
    { 0x64ac6f43, 0x0ac80628 },
    { 0xcf8c979d, 0x255bba08 },
    { 0x44f4a73d, 0x32a50ffd },
    { 0xd5a8a733, 0x3949f623 },
    { 0x97d889bc, 0x3c9cd2b2 },
    { 0xe2308c3a, 0x3e45798e },
    { 0xeb1c432d, 0x3f1a36e2 },
    { 0x47ae147b, 0x3f847ae1 },
    { 0x9999999a, 0x3fb99999 },
    { 0x00000000, 0x3ff00000 },
};

static unsigned long hpostab[][2] =
{
    { 0x7f73bf3c, 0x75154fdd },
    { 0xf9301d32, 0x5a827748 },
    { 0xe93ff9f5, 0x4d384f03 },
    { 0xb5056e17, 0x4693b8b5 },
    { 0x37e08000, 0x4341c379 },
    { 0x00000000, 0x4197d784 },
    { 0x00000000, 0x40c38800 },
    { 0x00000000, 0x40590000 },
    { 0x00000000, 0x40240000 },
};

#define negtab ((double *)hnegtab)
#define postab ((double *)hpostab)
#endif

/*************************
 * Convert double val to a string of
 * decimal digits.
 *	if (cnvflag == ECVT)
 *		ndig = # of digits in resulting string past the decimal point
 *	else
 *		ndig = # of digits in resulting string
 * 	digstr[LDBL_DIG * 2 + 2]
 * Returns:
 *	*pdecpt = position of decimal point from left of first digit
 *	*psign  = nonzero if value was negative
 * BUGS:
 *	This routine will hang if it is passed a NAN or INFINITY.
 */

char * __pascal __floatcvt(int cnvflag,char *digstr,long double val,int ndig,
	int *pdecpt,int *psign)
{
	int decpt,pow,i;
	int nsig;
	int sig;
	char c;

	if (signbit(val))
	{
	    *psign = 1;
	    val = -val;
	}
	else
	    *psign = 0;
	ndig = (ndig < 0) ? 0
			  : (ndig < DIGMAX) ? ndig
					    : DIGMAX;
	if (val == 0)
	{
		memset(digstr,'0',ndig);
		decpt = 0;
	}
	else
	{	/* Adjust things so that 1 <= val < 10	*/
		decpt = 1;
#if _WIN32
		pow = 4096;
#else
		pow = 256;
#endif
		i = 0;
		while (val < 1)
		{	while (val < negtab[i + 1])
			{	val *= postab[i];
				decpt -= pow;
			}
			pow >>= 1;
			i++;
		}
#if _WIN32
		pow = 4096;
#else
		pow = 256;
#endif
		i = 0;
		while (val >= 10)
		{	while (val >= postab[i])
			{	val *= negtab[i];
				decpt += pow;
			}
			pow >>= 1;
			i++;
		}

		if (cnvflag == FCVT && decpt > 0)
		{	ndig += decpt;
			if (ndig > DIGMAX)
			    ndig = DIGMAX;
		}

		/* Pick off digits 1 by 1 and stuff into digstr[]	*/
		/* Do 1 extra digit for rounding purposes		*/
		nsig = 0;
		sig = 0;
		for (i = 0; i <= ndig; i++)
		{	int n;

			if (nsig > LDBL_DIG+1)
			    c = '0';
			else
			{
			    n = val;
			    c = n + '0';
			    val = (val - n) * 10;	/* get next digit */
			    if (n)
				sig = 1;
			    nsig += sig;
			}
			digstr[i] = c;
		}
		if (c >= '5')		/* if we need to round		*/
		{	--i;
			while (1)
			{
				c = '0';
				if (i == 0)		/* if at start	*/
				{	ndig += cnvflag;
					decpt++;	/* shift dec pnt */
							/* "100000..."	*/
					break;
				}
				digstr[i] = '0';
				--i;
				c = digstr[i];
				if (c != '9')
					break;
			}
			digstr[i] = c + 1;
		} /* if */
	} /* else */
	*pdecpt = decpt;
	digstr[ndig] = 0;		/* terminate string		*/
	return digstr;
}

