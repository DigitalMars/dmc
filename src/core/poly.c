//_poly.c
// Copyright (C) 1985-2005 by Digital Mars
// All Rights Reserved
// www.digitalmars.com

/* Algorithms from Cody and Waite, "Software Manual For The Elementary	*/
/* Functions".								*/

#include	<stdio.h>
#include	<float.h>

/* Functions from math87.asm	*/
#include "math87.h"

/***********************************
 * Evaluate polynomial of the form:
 *	(coeff[deg] * x + coeff[deg - 1]) * x + ... + coeff[0]
 */

#ifndef __INLINE_8087

double poly(double x,int deg,double *coeff)
{	double r;

	if (_8087)
		return _poly87(x,deg,coeff);
	r = coeff[deg];
	while (deg--)
		r = x * r + coeff[deg];
	return r;
}

#endif

#if _WIN32

long double _polyl(long double x, int deg, long double *coeff)
{	long double r;

	r = coeff[deg];
	while (deg--)
	{
		r = x * r + coeff[deg];
	}
	return r;
}

#endif
