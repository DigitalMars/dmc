//_ sin.c
// Copyright (C) 1985-2001 by Digital Mars
// All Rights Reserved
// www.digitalmars.com

/* Algorithms from Cody and Waite, "Software Manual For The Elementary	*/
/* Functions".								*/


#include	<stdio.h>
#include	<stdlib.h>
#include	<fltpnt.h>
#include	<errno.h>
#include	<math.h>
#include	<float.h>
#include	<fenv.h>


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

/***********************
 * Do sine and cosine.
 * Input:
 *	cosine = 0:	sine
 *		 1:	cosine
 */

static long double near pascal _sincos(long double,int);

#undef sinf
float sinf(float x)
{
	return (float)_sincos((long double)x,0);
}

#undef sin
double sin(double x)
{
	return (double)_sincos((long double)x,0);
}

#undef sinl
long double sinl(long double x)
{
	return _sincos(x, 0);
}

#undef cosf
float cosf(float x)
{

	return (float)_sincos((long double)x,1);
}

#undef cos
double cos(double x)
{

	return (double)_sincos((long double)x,1);
}

#undef cosl
long double cosl(long double x)
{

	return _sincos(x, 1);
}

static long double near pascal _sincos(long double x,int cosine)
{
  switch(__dtype(x))
  {
     case DTYPE_ZERO	    :
			      if (cosine)
				return 1.0;
     case DTYPE_QNAN	    :
			      return x;
     case DTYPE_INFINITE    :

			      return __trigerr(DOMAIN,NAN,x,cosine);
     case DTYPE_SNAN	    :
			      return __trigerr(DOMAIN,_AsQNaN(x),x,cosine);
  }
#if __INLINE_8087
    return _sincostan87(x,cosine);
#else
  {
  double y,xn,f;
  int sgn;
  long n;
  static double r[8] =
  {	-0.16666666666666665052e+0,
	 0.83333333333331650314e-2,
	-0.19841269841201840457e-3,
	 0.27557319210152756119e-5,
	-0.25052106798274584544e-7,
	 0.16058936490371589114e-9,
	-0.76429178068910467734e-12,
	 0.27204790957888846175e-14	/* pg. 138		*/
  };
  static double C1 = 3.1416015625;
  static double C2 = -8.908910206761537356617e-6;	/* pg. 136	*/

  if (_8087)
	return _sincostan87(x,cosine);
  sgn = 1;
  if (x < 0)
  {	y = -x;
	if (!cosine)
		sgn = -1;
  }
  else
	y = x;
  if (cosine)
	y += PIOVER2;

#define YMAX	(M_PI * MAXPOW2)	// pi*B**t

  if (y >= YMAX)			/* if total loss of significance */
	return __trigerr(TLOSS,0.0,x,cosine);
  n = y * ONEOVERPI + .5;		/* round and take integer part	*/
  xn = n;
  if (n & 1)				/* if n is odd			*/
	sgn = -sgn;
  if (cosine)
	xn -= .5;
  f = (fabs(x) - xn * C1) - xn * C2;

  if (fabs(f) >= EPS)
  {	double g;

	g = f * f;
	g = g * poly(g,7,r);
	f = f + f * g;
  }
  if (sgn < 0)
	f = -f;
  if (y >= M_PI * SQRTMAXPOW2)		// partial loss of significance
	f = __trigerr(PLOSS,f,x,cosine);
  feexcept(FE_INEXACT);
  return f;
  }
#endif
}
