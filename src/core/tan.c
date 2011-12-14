/*_tan.c   Sat Apr 15 1989   Modified by: Phil Hinger */
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

/******************
 * Compute tan(x).
 */

float tanf(float x)
{
  return (float)tan((double)x);
}

double tan(double x)
{
    return tanl(x);
}

long double tanl(long double x)
{
#if _WIN32
    _asm
    {
	fld	tbyte ptr x
	fxam
	fstsw	AX
	fstp	ST(0)
    }
    switch (_EAX & _CCC_MASK)
    {
	case _CCC_NORMAL:
	case _CCC_DENORMAL:
	    return _sincostan87(x,2);

	case _CCC_ZERO:
	    return x;

	case _CCC_NAN:
	    if (!isqnanl(x))
	        return __trigerr(DOMAIN,toqnanl(x),x,2);
	    return x;

	case _CCC_INFINITY:
	    return __trigerr(DOMAIN,NAN,x,2);

	default:
	    return __nan;
    }

#else
  switch(__dtype((double)x))
  {
     case DTYPE_ZERO	    :
     case DTYPE_QNAN	    :
			      return x;
     case DTYPE_INFINITE    :

			      return __trigerr(DOMAIN,NAN,x,2);
     case DTYPE_SNAN	    :
			      return __trigerr(DOMAIN,_AsQNaN(x),x,2);
  }
#if __INLINE_8087
  return _sincostan87(x,2);
#else
  {
  double y,xn,f,g,xnum,xden,result;
  int sgn = 0;
  long n;
#undef C1
#undef C2
  static double C1 = 1.57080078125;		/* octal 1.4442		*/
  static double C2 = -4.454455103380768678308e-6;
  static double p[3] =
  {	-0.13338350006421960681e+0,
	 0.34248878235890589960e-2,
	-0.17861707342254426711e-4
  };
  static double q[5] =
  {	 0.10000000000000000000e+1,
	-0.46671683339755294240e+0,
	 0.25663832289440112864e-1,
	-0.31181531907010027307e-3,
	 0.49819433993786512270e-6
  };

  if (_8087)
	return _sincostan87(x,2);
  sgn = 0;
  y = x;
  if (x < 0)
  {	y = -x;
	sgn++;
  }
  if (y > SQRTMAXPOW2 * PIOVER2)
	return __trigerr(TLOSS,0.0,x,2);
  n = x * TWOOVERPI + .5;
  xn = n;
  f = (x - xn * C1) - xn * C2;
  if (fabs(f) < EPS)
  {	xnum = f;
	xden = 1.0;
  }
  else
  {	g = f * f;
	xnum = poly(g,2,p) * g * f + f;
	xden = poly(g,4,q);
  }
  if (n & 1)				/* if n is odd			*/
	result = -xden / xnum;
  else
	result = xnum / xden;
  feexcept(FE_INEXACT);
  return result;
  }
#endif
#endif
}
