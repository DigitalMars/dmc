/*_log.c   Sat Apr 15 1989   Modified by: Phil Hinger */
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

#define SQRT2		0x1.6a09e667f3bcc908p+0
#define SQRT2L		0x1.6a09e667f3bcc908p+0L
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

/**************************
 * Compute natural log plus 1
 */

float log1pf(float x)
{
    return log1pl(x);
}

#if __INLINE_8087 && _WIN32

double log1p(double x)
{
    return log1pl(x);
}

long double log1pl(long double x)
{
    unsigned cc;
    static char name[] = "log1p";
    static long double SQRT2M1 = SQRT2L - 1.0L;
    static long double NOneMSQRT2D2 = -(1.0L - SQRT2L / 2.0L);

    //printf("log1pl(%Lg)\n", x);

    _asm
    {
	fld	tbyte ptr x
	fxam
	fstsw	AX
	mov	cc, EAX
    }

#define C0	0x100
#define C1	0x200		// sign of ST
#define C2	0x400
#define C3	0x4000

/*
 *	C3,C2,C0
 *	000	unsupported
 *	001	nan
 *	010	normal
 *	011	infinity
 *	100	zero
 *	101	empty
 *	110	denormal
 */

    switch (cc & (C3 | C2 | C1 | C0))
    {
	case C2:		// normal
	    __asm
	    {
		fld tbyte ptr SQRT2M1
		fcomp
		fstsw	AX
		sahf
		ja	Ldenormal

		fldln2
		fxch	ST(1)
		fld1
		faddp ST(1),ST
		fyl2x
	    }
	    break;		// return ST

	case C3|C2:		// denormal
	case C3|C2|C1:		// -denormal
	Ldenormal:
	    __asm
	    {
		fldln2
		fxch	ST(1)
		fyl2xp1
	    }
	    break;		// return ST

	case C2|C1:		// -normal
	    __asm
	    {
		fld1
		fchs
		fcomp
		fstsw	AX
		sahf
		ja	Lnan		// x < -1
		je	Ldivbyzero	// x == -1

		fld	tbyte ptr NOneMSQRT2D2
		fcomp
		fstsw	AX
		sahf
		jb	Ldenormal

		fldln2
		fxch	ST(1)
		fld1
		faddp ST(1),ST
		fyl2x
	    }
	    break;		// return ST

	case C0:		// nan
	case C1|C0:		// -nan
	case C3:		// zero
	case C3|C1:		// -zero
	case C2|C0:		// +infinity
	    break;		// return ST

	default:	
	Lnan:
	    __asm
	    {
		fstp	ST(0)
	    }
	    return __matherr(DOMAIN, NAN, x, 0.0, name);

	Ldivbyzero:
	    __asm
	    {
		fstp	ST(0)
	    }
	    return __matherr(DIVIDE_BY_ZERO, -INFINITY, x, 0.0, name);
    }
}
#else
double log1p(double x)
{
    return log(x + 1);
}
#endif


/**************************
 * Compute logarithm of x.
 * Input:
 *	log10 = 0	compute natural log
 *		1	compute log base 10
 */

static long double _near _pascal _loglog(int log10, long double x);


float logf(float x)
{
#if __INLINE_8087
	return _inline_yl2x(x, M_LN2_L);
#else
	return (float)_loglog(0,(long double)x);
#endif
}

double log(double x)
{
#if __INLINE_8087
	return _inline_yl2x(x, M_LN2_L);
#else
	return (double)_loglog(0,(long double)x);
#endif
}

long double logl(long double x)
{
#if __INLINE_8087
	return _inline_yl2x(x, M_LN2_L);
#else
	return _loglog(0,x);
#endif
}

float log10f(float x)
{
#if __INLINE_8087
	return _inline_yl2x(x, M_LOG2_L);
#else
	return (float)_loglog(1,(long double)x);
#endif
}

double log10(double x)
{
#if __INLINE_8087
	return _inline_yl2x(x, M_LOG2_L);
#else
	return (double)_loglog(1,(long double)x);
#endif
}

long double log10l(long double x)
{
#if __INLINE_8087
	return _inline_yl2x(x, M_LOG2_L);
#else
	return _loglog(1,x);
#endif
}

#if !__INLINE_8087

static long double _near _pascal _loglog(int log10, long double x)
{
  static char __near *name[] = {"log","log10"};

  switch(__dtype((double)x))
  {
     case DTYPE_ZERO	    :
			      return __matherr(DIVIDE_BY_ZERO, -INFINITY, x, 0.0, name[log10]);
     case DTYPE_INFINITE    :
			      return (x > 0 ) ? x : __matherr(DOMAIN, NAN, x, 0.0, name[log10]);
     case DTYPE_SNAN	    :
			      return __matherr(DOMAIN, x, x, 0.0, name[log10]);
     case DTYPE_QNAN	    :
			      return x;
  }
  if (x < 0 )
    return __matherr(DOMAIN, NAN, x, 0.0, name[log10]);
  if (x == 1.0)
    return 0.0;
#if __INLINE_8087
    return (log10) ? _log1087(x) : _log87(x);
#else
  {
  double z;
  int n;
  static double C1 = 355./512.;		/* octal .543			*/
  static double C2 = -2.121944400546905827679e-4;
  static double a[3] =
  {	-0.64124943423745581147e+2,
	 0.16383943563021534222e+2,
	-0.78956112887491257267e+0
  };
  static double b[4] =
  {	-0.76949932108494879777e+3,
	 0.31203222091924532844e+3,
	-0.35667977739034646171e+2,
	 0.10000000000000000000e+1
  };

  if (_8087)
    return (log10) ? _log1087(x) : _log87(x);
  z = frexp(x,&n);			/* x = z * (2 ** n)		*/
  x = z - .5;
  if (z > SQRTPNT5)
	x -= .5;
  else
  {	z = x;
	n--;
  }
  z = x / (ldexp(z,-1) + .5);
  x = z * z;
  z += z * x * poly(x,2,a) / poly(x,3,b);
  x = n;
  x = (x * C2 + z) + x * C1;
  feexcept(FE_INEXACT);
  return (log10) ? x * LOG10E : x;
  }
#endif
}

#endif

/******************************************************/

#if __INLINE_8087

float log2f(float x)
{
    return _inline_yl2x(x, 1);
}

double log2(double x)
{
    return _inline_yl2x(x, 1);
}

long double log2l(long double x)
{
    return _inline_yl2x(x, 1);
}

#endif

