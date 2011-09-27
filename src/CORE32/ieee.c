/*_ieee.c   Sat Apr 15 1989   Modified by: Phil Hinger */
// Copyright (C) 1985-2001 by Digital Mars
// All Rights Reserved
// www.digitalmars.com

#include	<fltpnt.h>

#define SIGNMASK	0x80		// mask for sign bit

// Treat real as array of bytes
#define P(d)	((unsigned char *)(&d))

#define MSB_F	(sizeof(float) - 1)
#define MSB_D	(sizeof(double) - 1)
#define MSB_LD	(sizeof(long double) - 1)

/********************************
 * Return x with the sign of y.
 */

double copysign(double x, double y)
{
	P(x)[MSB_D] &= ~SIGNMASK;
	P(x)[MSB_D] |= P(y)[MSB_D] & SIGNMASK;
	return x;
}

float copysignf(float x, float y)
{
	P(x)[MSB_F] &= ~SIGNMASK;
	P(x)[MSB_F] |= P(y)[MSB_F] & SIGNMASK;
	return x;
}

#undef copysignl

long double copysignl(long double x, long double y)
{
	P(x)[MSB_LD] &= ~SIGNMASK;
	P(x)[MSB_LD] |= P(y)[MSB_LD] & SIGNMASK;
	return x;
}

/*******************************
 * Absolute value.
 */

#undef fabs			/* disable macro version	*/
#undef fabsf
#undef fabsl

double fabs(double x)
{
	P(x)[MSB_D] &= ~SIGNMASK;
	return x;
}


float fabsf(float x)
{
	P(x)[MSB_F] &= ~SIGNMASK;
	return x;
}

long double fabsl(long double x)
{
	P(x)[MSB_LD] &= ~SIGNMASK;
	return x;
}




/*******************************
 * Generate various types of NaN's.
 */

double nan(const char *tagp)	{ return NAN; }

float nanf(const char *tagp)	{ return NAN; }

double nans(const char *tagp)	{ return NANS; }

float nansf(const char *tagp)   { return NANS; }
