/*_ieee.c   Sat Apr 15 1989   Modified by: Phil Hinger */
/* Copyright (C) 1985-1991 by Walter Bright	*/
/* All Rights Reserved				*/

/* Algorithms from Cody and Waite, "Software Manual For The Elementary	*/
/* Functions".								*/

#include	<fltpnt.h>

#define SIGNMASK	0x8000		/* mask for sign bit		*/

/* Return pointer to the 4 unsigned shorts in a double	*/
#define P(d)	((unsigned short *)(&d))

/********************************
 * Return x with the sign of y.
 */

double copysign(double x, double y)
{
	P(x)[3] &= ~ SIGNMASK;
	P(x)[3] |= P(y)[3] & SIGNMASK;
	return x;
}

float copysignf(float x, float y)
{
	P(x)[1] &= ~ SIGNMASK;
	P(x)[1] |= P(y)[1] & SIGNMASK;
	return x;
}

#if _WIN32
long double copysignl(long double x, long double y)
{
	P(x)[4] &= ~ SIGNMASK;
	P(x)[4] |= P(y)[4] & SIGNMASK;
	return x;
}
#endif

/*******************************
 * Absolute value.
 */

#undef fabs			/* disable macro versions	*/
#undef fabsf
#undef fabsl

double fabs(double x)
{
	P(x)[3] &= ~ SIGNMASK;
	return x;
}


float fabsf(float x)
{
	P(x)[1] &= ~ SIGNMASK;
	return x;
}

#if _WIN32
long double fabsl(long double x)
{
	P(x)[4] &= ~ SIGNMASK;
	return x;
}
#endif



/*******************************
 * Generate various types of NaN's.
 */

double nan(const char *tagp)	{ return NAN; }

float nanf(const char *tagp)	{ return NAN; }

double nans(const char *tagp)	{ return NANS; }

float nansf(const char *tagp)   { return NANS; }

#if _WIN32
long double nanl(const char *tagp)	{ return NAN; }
long double nansl(const char *tagp)	{ return NANS; }
#endif
