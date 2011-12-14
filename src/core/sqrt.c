/*_sqrt.c   Sat Apr 15 1989   Modified by: Phil Hinger */
/* Copyright (C) 1985-1994 by Digital Mars */
/* All Rights Reserved				*/

/* Algorithms from Cody and Waite, "Software Manual For The Elementary	*/
/* Functions".								*/


#include	<stdio.h>
#include	<stdlib.h>
#include	<fltpnt.h>
#include	<errno.h>
#include	<math.h>
#include	<float.h>
#include	<fenv.h>

/*****************************************/

#define DTYPE_OTHER	0
#define DTYPE_ZERO	1
#define DTYPE_INFINITE	2
#define DTYPE_SNAN	3
#define DTYPE_QNAN	4


extern int __dtype(double);

#if _MT
extern void _pascal __FEEXCEPT(int e);
#define feexcept(e)	(__FEEXCEPT(e))
#else
extern fenv_t _fe_cur_env;
#define feexcept(e)	(_fe_cur_env.status |= (e))
#endif

double __trigerr(int type,double retval,double x,int flag);
double _arcerr(int flag,double x);
double __matherr(int type, double retval, double arg1, double arg2,const char * name);
double _largestNaN(double ,double);
double _AsQNaN(double);

/* Functions from math87.asm	*/
#include "math87.h"

#if __INLINE_8087
#define poly _poly87
#endif

/********* Various math constants **********/

#if 0
#define SQRTPNT5	 .70710678118654752440
#else
/* From Cody+Waite pg. 24: octal .55202 36314 77473 63110 */
long sqrtpnt5[2] = {0x667f3bcc,0x3fe6a09e};
#endif

#define SQRTPNT5	(*(double *)sqrtpnt5)

#define SQRT2		1.41421356227309504880
#define SQRT3		1.73205080756887729353
#define LOG10E		0.43429448190325182765
#define LN3OVER2	0.54930614433405484570
#define PIOVER2		1.57079632679489661923
#define PIOVER3		1.04719755119659774615
#define PIOVER4		0.78539816339744830962
#define PIOVER6		0.52359877559829887308
#define THREEPIOVER4	PIOVER4 * 3.0
#define ONEOVERPI	0.31830988618379067154
#define TWOOVERPI	0.63661977236758134308



static long infinite[2]  = {0x00000000, 0x7ff00000};
#define INFINITE    (*(double *)infinite)


/********** Parameters of floating point stuff **************/

#define DSIG		53		/* bits in significand		*/
#define DEXP		11		/* bits in exponent		*/
#define DMAXEXP		(1 << (DEXP - 1))	/* maximum exponent	*/
#define DMINEXP		(-DMAXEXP + 1)		/* minimum exponent	*/

			/* maximum power of 2 exactly representable	*/
			/* which is B**t				*/
#define	MAXPOW2		((double)(1L << 30) * (1L << (DSIG - 1 - 30)))
#define SQRTMAXPOW2	((1L << DSIG/2) * SQRT2)	/* B**(t/2)	*/
#define EPS		(1.0 / SQRTMAXPOW2)		/* B**(-t/2)	*/
#define BIGX		(LN2 * (1 << (DEXP - 1)))
#define SMALLX		(LN2 * -(1 << (DEXP - 1)))


#define DBLEXPMASK	0x7FF0		/* mask for long exponent	*/
#define SIGNMASK	0x8000		/* mask for sign bit		*/

/* Return pointer to the 4 unsigned shorts in a double	*/
#define P(d)	((unsigned short *)(&d))

/* Return biased exponent	*/
#define DOUBLE_EXP(d)  (P(d)[3] & DBLEXPMASK)

/* Return OR'd together bits of significand	*/
#define DOUBLE_SIG(d)  (P(d)[3] & ~(SIGNMASK | DBLEXPMASK) | P(d)[2] | P(d)[1] | P(d)[0])

#define signOfDouble(x)  ((P(x)[3] & SIGNMASK) ? 1 : 0)

/* Turn a signalling NaN into a quiet NaN in place	*/
#define toqnan(x)	(P(x)[3] |= 8)

/*****************************
 * Compute square root of double.
 * Returns:
 *	square root of |x|
 */

#undef sqrtf				/* disable macro versions	*/
#undef sqrt
#undef sqrtl

double sqrt(double x);

float sqrtf(float x)
{
  return (float)sqrt((double)x);
}

double sqrt(double x)
{
  switch(__dtype(x))
  {
     case DTYPE_ZERO	    :
     case DTYPE_QNAN	    : return x;
     case DTYPE_INFINITE    :
			      if (x > 0.0)
				return x;
			      x = NAN;
     case DTYPE_SNAN	    :
			      return __trigerr(DOMAIN, _AsQNaN(x), x, 3);
  }
  if (x < 0)				/* sqrt of negative number?	*/
    return __trigerr(DOMAIN, NAN, x, 3);
#if __INLINE_8087
  return _sqrt87(x);			/* use the 8087			*/
#else
  {
  double f,y;
  int exp;				/* exponent			*/

  if (_8087)				/* if 8087 is available		*/
	return _sqrt87(x);		/* use the 8087			*/
  f = frexp(x,&exp);			/* split x into f and exp	*/
  y = .41731 + .59016 * f;		/* first guess (y0)	*/
  y = y + f / y;
  y = ldexp(y,-2) + f / y;		/* now we have y2		*/
  y = ldexp(y + f / y,-1);		/* and now y3			*/
  if (exp & 1)				/* if odd exponent		*/
  {
	y *= SQRTPNT5;
  }
  y = ldexp(y,(exp + 1) >> 1);
  feexcept(FE_INEXACT);
  return y;
  }
#endif
}

#if _WIN32
long double sqrtl(long double x)
{
    return _inline_sqrtl(x);
}
#else
long double sqrtl(long double x)
{
    return sqrt((double)x);
}
#endif
