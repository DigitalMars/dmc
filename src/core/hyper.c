/* hyper.c   Sat Apr 15 1989   Modified by: Phil Hinger
 * Copyright (C) 1985-2003 by Digital Mars, www.digitalmars.com
 * All Rights Reserved
 */

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

#define LN3OVER2_L	0x1.193ea7aad030a976p-1L

static long infinite[2]  = {0x00000000, 0x7ff00000};
#define INFINITE    (*(double *)infinite)


/********** Parameters of floating point stuff (see float.h) **************/

#define DSIG		DBL_MANT_DIG	/* bits in significand		*/
#define DEXP		11		/* bits in exponent		*/
#define DMAXEXP		(1 << (DEXP - 1))	/* maximum exponent	*/
#define DMINEXP		(-DMAXEXP + 1)		/* minimum exponent	*/

			/* maximum power of 2 exactly representable	*/
			/* which is B**t				*/
#define	MAXPOW2		((double)(1L << 30) * (1L << (DSIG - 1 - 30)))

// The following is equivalent exp2(DSIG * .5) when DSIG is odd
#define SQRTMAXPOW2	((1L << DSIG/2) * SQRT2)	/* B**(t/2)	*/

#define EPS		(1.0 / SQRTMAXPOW2)		/* B**(-t/2)	*/
#define BIGX		(M_LN2 * (1 << (DEXP - 1)))
#define SMALLX		(M_LN2 * -(1 << (DEXP - 1)))


#define DBLEXPMASK	0x7FF0		/* mask for long exponent	*/
#define SIGNMASK	0x8000		/* mask for sign bit		*/

#define EPS_L		0x1p-32L			// B**(-t/2)
#define BIGX_L		(M_LN2_L * (1 << (15 - 1)))

/* Return pointer to the 4 unsigned shorts in a double	*/
#define P(d)	((unsigned short *)(&d))

/* Return biased exponent	*/
#define DOUBLE_EXP(d)  (P(d)[3] & DBLEXPMASK)

/* Return OR'd together bits of significand	*/
#define DOUBLE_SIG(d)  (P(d)[3] & ~(SIGNMASK | DBLEXPMASK) | P(d)[2] | P(d)[1] | P(d)[0])

#define signOfDouble(x)  ((P(x)[3] & SIGNMASK) ? 1 : 0)

/* Turn a signalling NaN into a quiet NaN in place	*/
#define toqnan(x)	(P(x)[3] |= 8)

/************************
 * sinh(x) and cosh(x).
 *	sinh = 1:	sinh()
 *	       0:	cosh()
 */

static long double _near _pascal _sinhcosh(int sinh, long double x);

float sinhf(float x)
{
	return (float)_sinhcosh(1,(double)x);
}

double sinh(double x)
{
	return _sinhcosh(1,x);
}

long double sinhl(long double x)
{
	return _sinhcosh(1,x);
}

float coshf(float x)
{
	return (float)_sinhcosh(0,(double)x);
}

double cosh(double x)
{
	return _sinhcosh(0,x);
}

long double coshl(long double x)
{
	return _sinhcosh(0,x);
}

static long double _near _pascal _sinhcosh(int sinh, long double x)
{
#if _WIN32
    long double y,z,w,f;
    static char _near *name[] = {"cosh","sinh"};
    int sgn;
    static long lnv[2] =		/* 0.69316 10107 42187 50000e+0	*/
    {   0L,(0x3FDL << (52 - 32)) + (0542714L << 3)};	/* 0.542714 octal */
    static long double vmin2 =  0.24999308500451499336e+0L;
    static long double v2min1 = 0.13830277879601902638e-4L;

    /* Coefficients p[] and q[] from sinhl in:
     *	"Cephes Math Library Release 2.7:  January, 1998
     *	 Copyright 1984, 1991, 1998 by Stephen L. Moshier"
     * Algorithm from Cody & Waite
     */
    static long double p[4] =
    {
	0x1p+0L,
	0x1.fbcc4c2403ed112p-6L,
	0x1.b50dc86f66146cbcp-12L,
	0x1.d71ff767b285d8d4p-20L,
    };
    static long double q[5] =
    {
	 0x1.8p+2L,
	-0x1.d31a5a96c6e93864p-4L,
	 0x1.15da60d2ba43915ap-10L,
	-0x1.8cb9ac0b36e086ecp-18L,
	 0x1.2bdb5a1298429dbap-26L,
    };

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
	    return sinh ? x : 1.0;

	case _CCC_NAN:
	    if (!isqnanl(x))
		return __matherr(DOMAIN,x,x,0.0,name[sinh]);
	    return x;

	case _CCC_INFINITY:
	    return (sinh) ? x : INFINITE;

	default:
	    return __nan;
    }
    y = fabsl(x);
    sgn = (sinh) ? (x < 0.0) : 0;
    if (!sinh || y > 1.0)
    {   if (y > BIGX_L)	/* largest value exp() can handle	*/
	{   w = y - *(double *)&lnv[0];
	    if (w > BIGX_L + 0.69 - *(double *)&lnv[0])
	    {
		z = __inf;
		if (sinh && sgn)
		    z = -z;
		return __matherr(DOMAIN,z,x,0.0,name[sinh]);
	    }
	    z = expl(w);
	    x = z + v2min1 * z;
	}
	else
	{   x = expl(y);
	    x = ldexpl((x + ((sinh) ? -(1 / x) : (1 / x))),-1);
	}
	if (sgn)
	    x = -x;
    }
    else
    {   if (y >= EPS_L)		/* EPS = B ** (-t/2)		*/
	{   f = x * x;
	    f *= _polyl(f,3,p) / _polyl(f,4,q);
	    x += x * f;
	}
    }
    feexcept(FE_INEXACT);
    return x;
#else
    /* Algorithm and coefficients from Cody & Waite */
    double y,z,w,f;
    static char _near *name[] = {"cosh","sinh"};
    int sgn;
    static long lnv[2] =		/* 0.69316 10107 42187 50000e+0	*/
    {   0L,(0x3FDL << (52 - 32)) + (0542714L << 3)};	/* 0.542714 octal */
    static double vmin2 =  0.24999308500451499336e+0;
    static double v2min1 = 0.13830277879601902638e-4;
    static double p[4] =
    {   -0.35181283430177117881e+6,
	-0.11563521196851768270e+5,
	-0.16375798202630751372e+3,
	-0.78966127417357099479e+0
    };
    static double q[4] =
    {   -0.21108770058106271242e+7,
	 0.36162723109421836460e+5,
	-0.27773523119650701667e+3,
	 0.10000000000000000000e+1
    };

    switch(__dtype((double)x))
    {
	case DTYPE_ZERO:
	    if (!sinh)
		return 1.0;
	case DTYPE_QNAN:
	    return x;
	case DTYPE_INFINITE:
	    return (sinh) ? x : INFINITE;
	case DTYPE_SNAN:
	    return __matherr(DOMAIN,x,x,0.0,name[sinh]);
    }
    y = fabs(x);
    sgn = (sinh) ? (x < 0.0) : 0;
    if (!sinh || y > 1.0)
    {   if (y > BIGX)	/* largest value exp() can handle	*/
	    {   w = y - *(double *)&lnv[0];
		if (w > BIGX + 0.69 - *(double *)&lnv[0])
		{
		    z = INFINITY;
		    if (sinh && sgn)
			z = -z;
		    return __matherr(DOMAIN,z,x,0.0,name[sinh]);
		}
		z = exp(w);
		x = z + v2min1 * z;
	    }
	    else
	    {   x = exp(y);
		x = ldexpl((x + ((sinh) ? -(1 / x) : (1 / x))),-1);
	    }
	    if (sgn)
		x = -x;
    }
    else
    {   if (y >= EPS)		/* EPS = B ** (-t/2)		*/
	{   f = x * x;
	    f *= (poly(f,3,p) / poly(f,3,q));
	    x += x * f;
	}
    }
    feexcept(FE_INEXACT);
    return x;
#endif
}

/*************************
 * Compute tanh(x).
 * No error returns.
 */

float tanhf(float x)
{
    return (float)tanh((double)x);
}

double tanh(double x)
{
    return tanhl(x);
}

long double tanhl(long double x)
{
#if _WIN32
    long double g;
    int sgn;

    /* Coefficients p[] and q[] from tanhl in:
     *   "Cephes Math Library Release 2.7:  May, 1998
     *    Copyright 1984, 1987, 1989, 1998 by Stephen L. Moshier"
     * Algorithm from Cody & Waite
     */
    static long double p[] =
    {	-0x1.4702b9797f7bc77cp+10L,
	-0x1.5036daafdebd6aecp+6L,
	-0x1.e9c5398f2222b2b2p-1L,
	-0x1.1f331e2a3619a548p-14L,
    };
    static long double q[] =
    {
	0x1.ea8416363f39ab44p+11L,
	0x1.c773f45f92be6f26p+10L,
	0x1.8109bad99c48d0fep+6L,
	0x1p+0L,
    };

    sgn = 0;
    _asm
    {
	fld	tbyte ptr x
	fxam
	fstsw	AX
	fstp	ST(0)
    }
    switch (_EAX & (_CCC_MASK | _CCC_NEGATIVE))
    {
	case _CCC_NORMAL:
	case _CCC_DENORMAL:
	    break;

	case _CCC_NORMAL | _CCC_NEGATIVE:
	case _CCC_DENORMAL | _CCC_NEGATIVE:
	    sgn = 1;
	    x = -x;
	    break;

	case _CCC_ZERO:
	case _CCC_ZERO | _CCC_NEGATIVE:
	    return x;

	case _CCC_NAN:
	case _CCC_NAN | _CCC_NEGATIVE:
	    if (!isqnanl(x))
		return __trigerr(DOMAIN,toqnanl(x),x,5);
	    return x;

	case _CCC_INFINITY:
	    return 1.0;

	case _CCC_INFINITY | _CCC_NEGATIVE:
	    return -1.0;

	default:
	    return __nan;
    }

    if (x > (M_LN2_L + (LDBL_MANT_DIG + 1) * M_LN2_L) / 2)
	x = 1.0;
    else if (x > LN3OVER2_L)
    {   x = 0.5 - 1.0 / (expl(x + x) + 1.0);
	x += x;
    }
    else if (x < EPS_L)
	/* x = x */ ;
    else
    {   g = x * x;
	g = (g * _polyl(g,3,p)) / _polyl(g,3,q);
	x += x * g;
    }
    feexcept(FE_INEXACT);
    return (sgn) ? -x : x;
#else
    long double g;
    int sgn;
    /* Algorithm & coefficients p[] and q[] from Cody & Waite */
    static double p[3] =
    {   -0.16134119023996228053e+4,
	-0.99225929672236083313e+2,
	-0.96437492777225469787e+0
    };
    static double q[4] =
    {   0.48402357071988688686e+4,
	0.22337720718962312926e+4,
	0.11274474380534949335e+3,
	0.10000000000000000000e+1
    };

    switch(__dtype((double)x))
    {
      case DTYPE_ZERO:
      case DTYPE_QNAN:
		       return x;
      case DTYPE_INFINITE:
		       return (x > 0) ? 1.0 : -1.0;
      case DTYPE_SNAN:
		       return __trigerr(DOMAIN,_AsQNaN(x),x,5);
    }
    sgn = 0;
    if (x < 0)
    {   sgn++;
	x = -x;
    }
    if (x > (M_LN2 + (DSIG + 1) * M_LN2) / 2)
	x = 1.0;
    else if (x > LN3OVER2)
    {   x = 0.5 - 1.0 / (expl(x + x) + 1.0);
	x += x;
    }
    else if (x < EPS)
	/* x = x */ ;
    else
    {   g = x * x;
	g = (g * poly(g,2,p)) / poly(g,3,q);
	x += x * g;
    }
    feexcept(FE_INEXACT);
    return (sgn) ? -x : x;
#endif
}

#if 0
#include <stdio.h>

void main()
{
    printf("sinhl = %Lg\n", sinhl(.25));
}
#endif
