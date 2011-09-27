/*_round.c   Sat Apr 15 1989   Modified by: Phil Hinger */
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

#undef remquol

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

/***********************************
 * Returns i, which is the smallest integer
 * such that i >= x.
 */

double ceil(double x)
{
#if __INLINE_8087
    return _ceil87(x);
#else
    double result;
    int old_mode;

    if( _8087)
      return _ceil87(x);

    old_mode = fegetround();
    fesetround(FE_UPWARD);
    result = rint(x);
    fesetround(old_mode);
    return result;
#endif
}

float ceilf(float x)
{
#if __INLINE_8087
    return _ceil87(x);
#else
    float result;
    int old_mode;

    old_mode = fegetround();
    fesetround(FE_UPWARD);
    result = rintf(x);
    fesetround(old_mode);
    return result;
#endif
}

long double ceill(long double x)
{
#if _WIN32
    return _ceil87(x);
#else
    return ceil(x);
#endif
}

/***********************************
 * Returns i, which is the largest integer
 * such that i <= x.
 */

double floor(double x)
{
#if __INLINE_8087
    return _floor87(x);
#else
    double result;
    int old_mode;

    if (_8087)
      return _floor87(x);

    old_mode = fegetround();
    fesetround(FE_DOWNWARD);
    result = rint(x);
    fesetround(old_mode);
    return result;
#endif
}

float floorf(float x)
{
#if __INLINE_8087
    return _floor87(x);
#else
    float result;
    int old_mode;

    old_mode = fegetround();
    fesetround(FE_DOWNWARD);
    result = rintf(x);
    fesetround(old_mode);

    return result;
#endif
}

long double floorl(long double x)
{
#if _WIN32
    return _floor87(x);
#else
    return floor(x);
#endif
}

/**************************
 * Return the larger of the two NaNs, converting it to a quiet NaN.
 * Only the signficand bits are considered.
 * The sign and QNAN bits are ignored.
 */

double _largestNaN(double x1 ,double x2)
{
#define MSLSIG	0x7FFFF			/* most significant dword significand */

    toqnan(x1);
    toqnan(x2);
    if ((((unsigned long *)&x1)[1] & MSLSIG) != (((unsigned long *)&x2)[1] & MSLSIG))
    {
	if ((((unsigned long *)&x1)[1] & MSLSIG) >= (((unsigned long *)&x2)[1] & MSLSIG))
	    goto retx1;
	else
	    goto retx2;
    }
    else
    {
	if (((unsigned long *)&x1)[0] >= ((unsigned long *)&x2)[0])
	    goto retx1;
	else
	    goto retx2;
    }

retx1:
    return x1;

retx2:
    return x2;
}

/**********************
 * Convert from signalling NaN to quiet NaN.
 */

double _AsQNaN(double x)
{
  P(x)[3] |= 8;
  return x;
}

/**********************
 */

double round(double x)
{
  int oldRound;

  oldRound = fegetround();
  fesetround(FE_TOWARDZERO);
  x = rint((x >= 0) ? x + 0.5 : x - 0.5);
  fesetround(oldRound);
  return x;
}

float roundf(float x)
{
    return round(x);
}

#if _WIN32
long double roundl(long double x)
{
    int oldRound;

    oldRound = fegetround();
    fesetround(FE_TOWARDZERO);
    x = rintl((x >= 0) ? x + 0.5 : x - 0.5);
    fesetround(oldRound);
    return x;
}
#endif

/********************************
 * Same as rint() except that inexact exception is not raised.
 */

double nearbyint(double x)
{
  fenv_t fe;
  int oldStatus;

  fegetenv(&fe);
  oldStatus = fe.status & FE_INEXACT;
  x = rint(x);
  fegetenv(&fe);
  fe.status = (fe.status & ~FE_INEXACT) | oldStatus;
  fesetenv(&fe);
  return x;
}

float nearbyintf(float x)
{
  fenv_t fe;
  int oldStatus;

  fegetenv(&fe);
  oldStatus = fe.status & FE_INEXACT;
  x = rintf(x);
  fegetenv(&fe);
  fe.status = (fe.status & ~FE_INEXACT) | oldStatus;
  fesetenv(&fe);
  return x;
}

#if _WIN32
long double nearbyintl(long double x)
{
  fenv_t fe;
  int oldStatus;

  fegetenv(&fe);
  oldStatus = fe.status & FE_INEXACT;
  x = rintl(x);
  fegetenv(&fe);
  fe.status = (fe.status & ~FE_INEXACT) | oldStatus;
  fesetenv(&fe);
  return x;
}
#endif

/****************************
 */

double trunc(double x)
{
  int oldRound;

  oldRound = fegetround();
  fesetround(FE_TOWARDZERO);
  x = rint(x);
  fesetround(oldRound);
  return(x);
}

float truncf(float x)
{
  int oldRound;

  oldRound = fegetround();
  fesetround(FE_TOWARDZERO);
  x = rintf(x);
  fesetround(oldRound);
  return(x);
}

#if _WIN32
long double truncl(long double x)
{
  int oldRound;

  oldRound = fegetround();
  fesetround(FE_TOWARDZERO);
  x = rint(x);
  fesetround(oldRound);
  return x;
}
#endif

/****************************
 */

#undef rndtol
long int rndtol(long double x)
{
  return ((long int) rint(x));
}

long int rndtonl(long double x)
{
  int oldRound;
  long int y;

  oldRound = fegetround();
  fesetround(FE_TOWARDZERO);
  x += 0.5;
  y = rint(x);
  fesetround(oldRound);
  return(y);
}


/************************************
 * IEEE remainder function.
 * If y != 0 then the remainder r = x REM y is defined as:
 *	r = x - y * n
 */

float remainderf(float y, float x)
{
    int quo;

    return remquof(y, x, &quo);
}

double remainder(double y, double x)
{
    int quo;

    return remquo(y, x, &quo);
}

long double remainderl(long double y, long double x)
{
    int quo;

    return remquol(y, x, &quo);
}

float remquof(float y, float x, int *quo)
{
  float n,rem;
  int oldRound,oldStatus,classifyX,classifyY;

  *quo = 0;
  classifyX = fpclassify(x);
  if(classifyX == FP_SUBNORMAL)
    classifyX = FP_NORMAL;
  classifyY = fpclassify(y);
  switch(classifyY + 6 * classifyX)
  {
    case  FP_NANS      + 6 * FP_NANS	  : feexcept(FE_INVALID); return _AsQNaN(x);
    case  FP_NANQ      + 6 * FP_NANS	  : feexcept(FE_INVALID); return y;
    case  FP_INFINITE  + 6 * FP_NANS	  :
    case  FP_SUBNORMAL + 6 * FP_NANS	  :
    case  FP_NORMAL    + 6 * FP_NANS	  :
    case  FP_ZERO      + 6 * FP_NANS	  :
					    feexcept(FE_INVALID); return _AsQNaN(x);

    case  FP_NANS      + 6 * FP_NANQ	  : feexcept(FE_INVALID); return x;
    case  FP_NANQ      + 6 * FP_NANQ	  :
    case  FP_INFINITE  + 6 * FP_NANQ	  :
    case  FP_SUBNORMAL + 6 * FP_NANQ	  :
    case  FP_NORMAL    + 6 * FP_NANQ	  :
    case  FP_ZERO      + 6 * FP_NANQ	  :
					    return x;

    case  FP_NANS      + 6 * FP_INFINITE  : feexcept(FE_INVALID); return _AsQNaN(y);
    case  FP_NANQ      + 6 * FP_INFINITE  : return y;
    case  FP_INFINITE  + 6 * FP_INFINITE  : feexcept(FE_INVALID); return NAN;
    case  FP_SUBNORMAL + 6 * FP_INFINITE  :
    case  FP_NORMAL    + 6 * FP_INFINITE  :
    case  FP_ZERO      + 6 * FP_INFINITE  : return y;

    case  FP_NANS      + 6 * FP_NORMAL	  : feexcept(FE_INVALID); return _AsQNaN(y);
    case  FP_NANQ      + 6 * FP_NORMAL	  : return y;
    case  FP_INFINITE  + 6 * FP_NORMAL	  : feexcept(FE_INVALID); return NAN;
    case  FP_SUBNORMAL + 6 * FP_NORMAL	  :
    case  FP_NORMAL    + 6 * FP_NORMAL	  :
	   oldStatus = fetestexcept(FE_ALL_EXCEPT);
	   feclearexcept(FE_ALL_EXCEPT);
	   oldRound  = fegetround();
	   fesetround(FE_TONEAREST);
	   n = rint(y / x);
	   if (fetestexcept(FE_OVERFLOW))
	     rem = (y < 0.0 ) ? -0.0 : 0.0;
	   else
	   {
	     int q; // remember fmod can cause recursive error

	     feclearexcept(FE_ALL_EXCEPT);
	     q = n;
	     if (!fetestexcept(FE_INVALID))
	       *quo = q;
	     feclearexcept(FE_ALL_EXCEPT);
	     rem = y - n * x;
	     if (fetestexcept(FE_OVERFLOW))
	     {
	       float temp;

	       temp = x;
	       y -= copysign(temp,y);
	       n -= copysign(1.0,n);
	       rem = y - n * x;
	     }
	   }
	   if (rem == 0.0)
	     rem = (y < 0.0) ? -0.0 : 0.0;
	   feclearexcept(FE_ALL_EXCEPT);
	   feexcept(oldStatus);
	   fesetround(oldRound);
	   return rem;

    case  FP_ZERO      + 6 * FP_NORMAL	  :  return y;

    case  FP_NANS      + 6 * FP_ZERO	  :  feexcept(FE_INVALID); return _AsQNaN(y);
    case  FP_NANQ      + 6 * FP_ZERO	  :  return y;
    case  FP_INFINITE  + 6 * FP_ZERO	  :
    case  FP_SUBNORMAL + 6 * FP_ZERO	  :
    case  FP_NORMAL    + 6 * FP_ZERO	  :
    case  FP_ZERO      + 6 * FP_ZERO	  :
					  feexcept(FE_INVALID);
					  return (float)NAN;

  }
}

double remquo(double y, double x, int *quo)
{
  double n,rem;
  int oldRound,oldStatus,classifyX,classifyY;

  *quo = 0;
  classifyX = fpclassify(x);
  if(classifyX == FP_SUBNORMAL)
    classifyX = FP_NORMAL;
  classifyY = fpclassify(y);
  switch(classifyY + 6 * classifyX)
  {
    case  FP_NANS      + 6 * FP_NANS	  : feexcept(FE_INVALID); return _AsQNaN(x);
    case  FP_NANQ      + 6 * FP_NANS	  : feexcept(FE_INVALID); return y;
    case  FP_INFINITE  + 6 * FP_NANS	  :
    case  FP_SUBNORMAL + 6 * FP_NANS	  :
    case  FP_NORMAL    + 6 * FP_NANS	  :
    case  FP_ZERO      + 6 * FP_NANS	  :
					    feexcept(FE_INVALID); return _AsQNaN(x);

    case  FP_NANS      + 6 * FP_NANQ	  : feexcept(FE_INVALID); return x;
    case  FP_NANQ      + 6 * FP_NANQ	  :
    case  FP_INFINITE  + 6 * FP_NANQ	  :
    case  FP_SUBNORMAL + 6 * FP_NANQ	  :
    case  FP_NORMAL    + 6 * FP_NANQ	  :
    case  FP_ZERO      + 6 * FP_NANQ	  :
					    return x;

    case  FP_NANS      + 6 * FP_INFINITE  : feexcept(FE_INVALID); return _AsQNaN(y);
    case  FP_NANQ      + 6 * FP_INFINITE  : return y;
    case  FP_INFINITE  + 6 * FP_INFINITE  : feexcept(FE_INVALID); return NAN;
    case  FP_SUBNORMAL + 6 * FP_INFINITE  :
    case  FP_NORMAL    + 6 * FP_INFINITE  :
    case  FP_ZERO      + 6 * FP_INFINITE  : return y;

    case  FP_NANS      + 6 * FP_NORMAL	  : feexcept(FE_INVALID); return _AsQNaN(y);
    case  FP_NANQ      + 6 * FP_NORMAL	  : return y;
    case  FP_INFINITE  + 6 * FP_NORMAL	  : feexcept(FE_INVALID); return NAN;
    case  FP_SUBNORMAL + 6 * FP_NORMAL	  :
    case  FP_NORMAL    + 6 * FP_NORMAL	  :
	   oldStatus = fetestexcept(FE_ALL_EXCEPT);
	   feclearexcept(FE_ALL_EXCEPT);
	   oldRound  = fegetround();
	   fesetround(FE_TONEAREST);
	   n = rint(y / x);
	   if (fetestexcept(FE_OVERFLOW))
	     rem = (y < 0.0) ? -0.0 : 0.0;
	   else
	   {
	     int q; // remember fmod can cause recursive error

	     feclearexcept(FE_ALL_EXCEPT);
	     q = n;
	     if (!fetestexcept(FE_INVALID))
	       *quo = q ;
	     feclearexcept(FE_ALL_EXCEPT);
	     rem =   y - n * x;
	     if (fetestexcept(FE_OVERFLOW))
	     {
	       double temp;

	       temp = x;
	       y -= copysign(temp,y);
	       n -= copysign(1.0,n);
	       rem = y - n * x;
	     }
	     if (rem == 0.0)
		rem = (y < 0.0) ? -0.0 : 0.0;
	   }
	   feclearexcept(FE_ALL_EXCEPT);
	   feexcept(oldStatus);
	   fesetround(oldRound);
	   return rem;

    case  FP_ZERO      + 6 * FP_NORMAL	  :  return y;

    case  FP_NANS      + 6 * FP_ZERO	  :  feexcept(FE_INVALID); return _AsQNaN(y);
    case  FP_NANQ      + 6 * FP_ZERO	  :  return y;
    case  FP_INFINITE  + 6 * FP_ZERO	  :
    case  FP_SUBNORMAL + 6 * FP_ZERO	  :
    case  FP_NORMAL    + 6 * FP_ZERO	  :
    case  FP_ZERO      + 6 * FP_ZERO	  :
					  feexcept(FE_INVALID);
					  return NAN;

  }
}


long double remquol(long double y, long double x, int *quo)
{
#if _WIN32
  long double n,rem;
  int oldRound,oldStatus,classifyX,classifyY;

  *quo = 0;
  classifyX = fpclassify(x);
  if(classifyX == FP_SUBNORMAL)
    classifyX = FP_NORMAL;
  classifyY = fpclassify(y);
  switch(classifyY + 6 * classifyX)
  {
    case  FP_NANS      + 6 * FP_NANS	  : feexcept(FE_INVALID); return _AsQNaN(x);
    case  FP_NANQ      + 6 * FP_NANS	  : feexcept(FE_INVALID); return y;
    case  FP_INFINITE  + 6 * FP_NANS	  :
    case  FP_SUBNORMAL + 6 * FP_NANS	  :
    case  FP_NORMAL    + 6 * FP_NANS	  :
    case  FP_ZERO      + 6 * FP_NANS	  :
					    feexcept(FE_INVALID); return _AsQNaN(x);

    case  FP_NANS      + 6 * FP_NANQ	  : feexcept(FE_INVALID); return x;
    case  FP_NANQ      + 6 * FP_NANQ	  :
    case  FP_INFINITE  + 6 * FP_NANQ	  :
    case  FP_SUBNORMAL + 6 * FP_NANQ	  :
    case  FP_NORMAL    + 6 * FP_NANQ	  :
    case  FP_ZERO      + 6 * FP_NANQ	  :
					    return x;

    case  FP_NANS      + 6 * FP_INFINITE  : feexcept(FE_INVALID); return _AsQNaN(y);
    case  FP_NANQ      + 6 * FP_INFINITE  : return y;
    case  FP_INFINITE  + 6 * FP_INFINITE  : feexcept(FE_INVALID); return NAN;
    case  FP_SUBNORMAL + 6 * FP_INFINITE  :
    case  FP_NORMAL    + 6 * FP_INFINITE  :
    case  FP_ZERO      + 6 * FP_INFINITE  : return y;

    case  FP_NANS      + 6 * FP_NORMAL	  : feexcept(FE_INVALID); return _AsQNaN(y);
    case  FP_NANQ      + 6 * FP_NORMAL	  : return y;
    case  FP_INFINITE  + 6 * FP_NORMAL	  : feexcept(FE_INVALID); return NAN;
    case  FP_SUBNORMAL + 6 * FP_NORMAL	  :
    case  FP_NORMAL    + 6 * FP_NORMAL	  :
	   oldStatus = fetestexcept(FE_ALL_EXCEPT);
	   feclearexcept(FE_ALL_EXCEPT);
	   oldRound  = fegetround();
	   fesetround(FE_TONEAREST);
	   n = rintl(y / x);
	   if (fetestexcept(FE_OVERFLOW))
	     rem = (y < 0.0) ? -0.0 : 0.0;
	   else
	   {
	     int q; // remember fmod can cause recursive error

	     feclearexcept(FE_ALL_EXCEPT);
	     q = n;
	     if (!fetestexcept(FE_INVALID))
	       *quo = q ;
	     feclearexcept(FE_ALL_EXCEPT);
	     rem =   y - n * x;
	     if (fetestexcept(FE_OVERFLOW))
	     {
	       double temp;

	       temp = x;
	       y -= copysignl(temp,y);
	       n -= copysignl(1.0,n);
	       rem = y - n * x;
	     }
	     if (rem == 0.0)
		rem = (y < 0.0) ? -0.0 : 0.0;
	   }
	   feclearexcept(FE_ALL_EXCEPT);
	   feexcept(oldStatus);
	   fesetround(oldRound);
	   return rem;

    case  FP_ZERO      + 6 * FP_NORMAL	  :  return y;

    case  FP_NANS      + 6 * FP_ZERO	  :  feexcept(FE_INVALID); return _AsQNaN(y);
    case  FP_NANQ      + 6 * FP_ZERO	  :  return y;
    case  FP_INFINITE  + 6 * FP_ZERO	  :
    case  FP_SUBNORMAL + 6 * FP_ZERO	  :
    case  FP_NORMAL    + 6 * FP_ZERO	  :
    case  FP_ZERO      + 6 * FP_ZERO	  :
					  feexcept(FE_INVALID);
					  return NAN;

  }
#else
  return remquo(y, x, quo);
#endif
}


/*****************************
 * Remainder of x / y.
 * If y == 0, return 0.
 * Else return f such that x==i*y+f.
 */

float fmodf(float x,float y)
{
#if __INLINE_8087
    return _fmod87(x, y);
#else
    if (_8087)
	return (float)_fmod87((double)x,(double)y);
    else
    {
	float result;

	result = remainderf(fabsf(x), (y = fabsf(y)));
	if(signbit(result))
	   result += y;
	return copysignf(result,x);
    }
#endif
}

double fmod(double x,double y)
{
#if __INLINE_8087
    return _fmod87(x,y);
#else
    if (_8087)
	return _fmod87(x,y);
    else
    {
#if 1
	double result;

	result = remainder(fabs(x), (y = fabs(y)));
	if(signbit(result))
	   result += y;
	return copysign(result,x);
#else
	y = fabs(y);
	return (x + y == x) ? 0.0 : modf(x / y,&x) * y;
#endif
    }
#endif
}

long double fmodl(long double x, long double y)
{
#if __INLINE_8087
    return _fmod87(x,y);
#else
    return fmod(x, y);
#endif
}

/***********************************
 * Split x into an integer and fractional part (f).
 * Returns:
 *	f (with same sign as f)
 *	*pi = integer part (with same sign as f)
 * Note that the integer part is stored into a double!
 */

float modff(float value, float *iptr)
{
    if (_8087)
       *iptr = (float)_chop87(value);
    else
    {
	int save_rnd;

	save_rnd = fegetround();
	fesetround(FE_TOWARDZERO);
	*iptr = nearbyintf(value);
	fesetround(save_rnd);
    }
    return copysign(
		    (fabsf(value) == INFINITY) ? 0.0 : value - (*iptr), value
		   );
}
double modf(double value,double *iptr)
{
#if __INLINE_8087
    *iptr = _chop87(value);
    return copysign(
		    (fabs(value) == INFINITY) ? 0.0 : value - (*iptr), value
		   );
#else
    int sgn = 0;

#if 1
    if (_8087)
      *iptr = _chop87(value);
    else
    {
	int save_rnd;

	save_rnd = fegetround();
	fesetround(FE_TOWARDZERO);
	*iptr = nearbyint(value);
	fesetround(save_rnd);
    }
    return copysign((fabs(value) == INFINITY) ? 0.0 : value - (*iptr), value);
#else
    if (_8087)
    {
       *iptr = _chop87(value);
	return value - *iptr;
    }
    else
    {
	*iptr = x;
	if (x < 0)
	{	if (*iptr > -MAXPOW2)
		{	*iptr -= MAXPOW2;
			*iptr += MAXPOW2;
			if (*iptr < x)
				(*iptr)++;
		}
	}
	else
	{	if (*iptr < MAXPOW2)
		{	*iptr += MAXPOW2;
			*iptr -= MAXPOW2;
			if (*iptr > x)
				(*iptr)--;
		}
	}
	return x - *iptr;
    }
#endif
#endif
}

long double modfl(long double value, long double *iptr)
{
#if __INLINE_8087
   *iptr = _chop87(value);
    return copysignl(
		    (fabsl(value) == INFINITY) ? 0.0 : value - (*iptr), value
		   );
#else
    return modf(value, (double *)iptr);
#endif
}


float frexpf(float value, int *exp)
{
  return  (float)frexp((double)value,exp);
}

float ldexpf(float value,int exp)
{
  return (float)ldexp((double)value, exp);
}

double scalb(double x, int n)
{
  if (abs(n) < 32768)
    return ldexpl(x,(int)n);
  else
    return (n > 0) ? ldexpl(x,32768) : ldexpl(x,-32768);
}

float scalbf(float x, int n)
{
  if (abs(n) < 32768)
    return (float)ldexp((double)x,(int)n);
  else
    return (n > 0) ? (float)ldexp((double)x,32768) : (float)ldexp((double)x,-32768);
}

#undef rintl
#if _WIN32
long double rintl(long double x)
{
    return _inline_rintl(x);
}
#else
long double rintl(long double x)
{
    return rint((double)x);
}
#endif

#if 0 && _WIN32
#undef ldexpl
long double ldexpl(long double value, int exp)
{
  return _inline_ldexpl(value, exp);
}
#endif


