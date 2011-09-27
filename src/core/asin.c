/*_asin.c   Sat Apr 15 1989   Modified by: Phil Hinger */
/* Copyright (C) 1985-1991 by Walter Bright	*/
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

//#define PIOVER2	1.57079632679489661923
#define PIOVER2		(M_PI_L / 2)

#define PIOVER3		1.04719755119659774615

//#define PIOVER4	0.78539816339744830962
#define PIOVER4		(M_PI_L / 4)


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

/*************************
 * Compute asin(x) (flag == 0) or acos(x) (flag == 1).
 */

static long double _near _pascal _asincos(int,long double);

float asinf(float x)
{
	return _asincos(0,x);
}

double asin(double x)
{
	return _asincos(0,x);
}

long double asinl(long double x)
{
	return _asincos(0,x);
}

float acosf(float x)
{
   return _asincos(1,x);
}

double acos(double x)
{
	return _asincos(1,x);
}

long double acosl(long double x)
{
	return _asincos(1,x);
}

static long double _near _pascal _asincos(int flag, long double x)
{
#if _WIN32 && __INLINE_8087
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
	    break;

	case _CCC_ZERO:
	    if (flag)
	    {
		feexcept(FE_INEXACT);
		return PIOVER2;
	    }
	    else
		return x;

	case _CCC_INFINITY:
	    return _arcerr(flag, x);

	case _CCC_NAN:
	    if (isqnanl(x))
		return x;
	    x = toqnanl(x);
	    return _arcerr(flag,x);

	default:
	    return __nan;
    }
#else
    double d;

    switch( __dtype((double)x))
    {
	case DTYPE_QNAN:
			return x;
	case DTYPE_SNAN:
			d = x;
			toqnan(d);
			return _arcerr(flag,d);
	case DTYPE_ZERO:
			if (flag)
			{
			    feexcept(FE_INEXACT);
			    return PIOVER2;
			}
			else
			    return x;
    }
#endif
    if (x == 1.0)
    {
	if (flag)
	    return 0.0;
	else
	{
	    feexcept(FE_INEXACT);
	    return PIOVER2;
	}
    }
    else if (x == -1.0)
    {
	feexcept(FE_INEXACT);
	return (flag) ? M_PI : -PIOVER2;
    }
#if __INLINE_8087
    return _asincos87(flag,x);
#else
    {
	double y,r,g,result;
	int i,dtype;
	static double p[5] =
	{	-0.27368494524164255994e+2,
		 0.57208227877891731407e+2,
		-0.39688862997504877339e+2,
		 0.10152522233806463645e+2,
		-0.69674573447350646411e+0
	};
	static double q[6] =
	{	-0.16421096714498560795e+3,
		 0.41714430248260412556e+3,
		-0.38186303361750149284e+3,
		 0.15095270841030604719e+3,
		-0.23823859153670238830e+2,
		 0.10000000000000000000e+1
	};
	static double a[2] = {	0.0,PIOVER4	};
	       static double b[2] = {  PIOVER2,PIOVER4 };

	if (_8087)
		return _asincos87(flag,x);
	y = fabs(x);
	if (y > .5)
	{
		if (y > 1.0)
		{
			return _arcerr(flag,x);
		}
		i = 1 - flag;
		g = ldexp(1. - y,-1);		/* g = (1-y)/2		*/
		y = -ldexp(sqrt(g),1);		/* y = -2 * g ** .5	*/
		goto L1;
	}
	else
	{	i = flag;
		if (y < EPS)
			result = y;
		else
		{	g = y * y;
		    L1:	r = g * poly(g,4,p) / poly(g,5,q);
			result = y + y * r;
		}
	}
	if (flag == 0)				/* if asin(x)		*/
	{	result += a[i];
		result += a[i];
		if (x < 0)
			result = -result;
	}
	else
	{	if (x < 0)
		{	result += b[i];
			result += b[i];
		}
		else
		{	result = a[i] - result;
			result += a[i];
		}
	}
	feexcept(FE_INEXACT);
	return result;
    }
#endif
}
