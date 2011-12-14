/*_exp.c   Sat Apr 15 1989   Modified by: Phil Hinger */
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

#include	"8087.h"

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

extern long sqrtpnt5[2];
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
 * Compute e**x -1.
 */

float expm1f(float x)
{
    return expm1(x);
}

double expm1(double x)
{
    return expm1l(x);
}

long double expm1l(long double x)
{
#if __INLINE_8087 && _WIN32

#if 1
    _asm
    {
	fld	tbyte ptr x
	fxam
	fstsw	AX
	fstp	ST(0)
    }
    switch (_EAX & _CCC_MASK)
    {
	case _CCC_ZERO:
	    return 0;

	case _CCC_NAN:
	    return x;

	case _CCC_INFINITY:
	    return (x > 0) ? x : -1;

	case _CCC_NORMAL:
	case _CCC_DENORMAL:
	    break;

	default:
	    return __nan;
    }
#else
    switch (__dtype((double)x))
    {
	case DTYPE_ZERO:      return 0.0;
	case DTYPE_QNAN:      return x;
	case DTYPE_INFINITE:  return (x > 0) ? x : -1.0;
	case DTYPE_SNAN:      return __trigerr(DOMAIN, _AsQNaN(x), x, 4);
    }
#endif

    if ((_8087 >= 3 && -1.0L / M_LOG2E < x && x < 1.0L / M_LOG2E) ||
	0 < x && x < 0.5L / M_LOG2E)
    {	// The range for f2xm1 is 0<=x<=0.5 for the 80287 and less,
	// and is -1<x<1 for the 387 and up.
	__asm
	{
		fldl2e
		fld	x
		fmulp	ST(1),ST		//ST = y * log2(e)
		f2xm1				//ST = (2 ** ST) - 1
	}
    }
    else
	return _exp87(x) - 1.0;
#else
    x = exp(x);
    return x - 1.0;
#endif
}


/*****************************
 * Compute e**x.
 */

float expf(float x)
{
  return (float)expl((long double)x);
}

double exp(double x)
{
  return (double)expl((long double)x);
}

long double expl(long double x)
{
#if 1
    _asm
    {
	fld	tbyte ptr x
	fxam
	fstsw	AX
	fstp	ST(0)
    }
    switch (_AX & _CCC_MASK)
    {
	case _CCC_ZERO:
	    return 1;

	case _CCC_NAN:
	    return x;

	case _CCC_INFINITY:
	    return (x > 0) ? x : 0;

	case _CCC_NORMAL:
	case _CCC_DENORMAL:
	    return _exp87(x);

	default:
	    return __nan;
    }
#else
  switch(__dtype((double)x))
  {
     case DTYPE_ZERO	    :
			      return 1.0;
     case DTYPE_QNAN	    :
			      return x;
     case DTYPE_INFINITE    :
			      return (x > 0) ? x : 0.0;
     case DTYPE_SNAN	    :
			      return __trigerr(DOMAIN, _AsQNaN(x), x, 4);
  }

#if __INLINE_8087
     return _exp87(x);
#else
  if (_8087)
     return _exp87(x);

  {
  int n;
  double z;
  static double C1 = 355./512.;			/* .543 octal		*/
  static double C2 = -2.121944400546905827679e-4;
  static double p[3] =
  {	0.25000000000000000000e+0,
	0.75753180159422776666e-2,
	0.31555192765684646356e-4
  };
  static double q[4] =
  {	0.50000000000000000000e+0,
	0.56817302698551221787e-1,
	0.63121894374398503557e-3,
	0.75104028399870046114e-6
  };



#define EPS1	(1.0 / (MAXPOW2 * 2))		/* 2 ** (-b - 1)	*/
  if (fabs(x) < EPS1)
	return 1.0;
  n = x * M_LOG2E + .5;
  z = n;


  x = (x - z * C1) - z * C2;
  z = x * x;
  x *= poly(z,2,p);
  feexcept(FE_INEXACT);
  return ldexp(.5 + x / (poly(z,3,q) - x),n + 1);
  }
#endif
#endif
}
