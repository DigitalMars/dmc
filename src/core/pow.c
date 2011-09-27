
/* pow.c   written by: Phil Hinger
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

/*********************************************
 * Determine if y is odd.
 */

static int __near __pascal yodd(long double y)
{
    int oldStatus;
    int yIsOddInteger;
    long double yTemp;

    oldStatus = fetestexcept(FE_ALL_EXCEPT);
    yTemp = truncl(y);
    yIsOddInteger = ((yTemp == y) && (fabsl(remainderl(yTemp,2.0)) == 1.0));
    feclearexcept(FE_ALL_EXCEPT);
    feexcept(oldStatus);
    return yIsOddInteger;
}

/**************************************
 * Compute x ** y.
 * Error if:
 *	x == 0 and y <= 0
 *	x < 0 and y is not integral
 */

#if __INLINE_8087

float powf(float x,float y)
{
  return (float)pow((double)x,(double)y);
}

double pow(double x,double y)
{
  return (double)powl((long double)x,(long double)y);
}


long double powl(long double x,long double y)
{
    int sgn;
    int yIsOddInteger;
    struct exception e;
    static const char name[] = "pow";

    _asm
    {
	fld	tbyte ptr y
	fxam
	fstsw	AX
	mov	DL,AH
	fstp	ST(0)

	fld	tbyte ptr x
	fxam
	fstsw	AX
	mov	AL,DL
	fstp	ST(0)
    }

    #define X(x,y)	((x) | ((y) >> 8))

    switch (_EAX & 0x4545)
    {
	case X(_CCC_NORMAL,   _CCC_NORMAL):
	case X(_CCC_NORMAL,   _CCC_DENORMAL):
	case X(_CCC_DENORMAL, _CCC_NORMAL):
	case X(_CCC_DENORMAL, _CCC_DENORMAL):
	    break;

	case X(_CCC_NORMAL,   _CCC_ZERO):
	case X(_CCC_DENORMAL, _CCC_ZERO):
	case X(_CCC_ZERO,     _CCC_ZERO):
	case X(_CCC_INFINITY, _CCC_ZERO):
	case X(_CCC_NAN,      _CCC_ZERO):
	    return 1.0;

	case X(_CCC_NORMAL,   _CCC_INFINITY):
	case X(_CCC_DENORMAL, _CCC_INFINITY):
	    if (fabsl(x) == 1.0)
		return __matherr(DOMAIN,NAN,x,y,name);
	    if (y > 0.0)
		return (fabsl(x) > 1.0) ? INFINITY : 0.0;
	    else
		return (fabsl(x) < 1.0) ? INFINITY : 0.0;

	case X(_CCC_NORMAL,   _CCC_NAN):
	case X(_CCC_DENORMAL, _CCC_NAN):
	case X(_CCC_ZERO,     _CCC_NAN):
	case X(_CCC_INFINITY, _CCC_NAN):
	    if (isqnanl(y))
		return  y;
	    return __matherr(DOMAIN,y,x,y,name);

	case X(_CCC_ZERO,   _CCC_NORMAL):
	case X(_CCC_ZERO,   _CCC_DENORMAL):
	    yIsOddInteger = yodd(y);
	    if (y > 0.0)
		return (yIsOddInteger) ? x : 0.0;
	    else
		return __matherr(DIVIDE_BY_ZERO,(yIsOddInteger) ? copysign(INFINITY,x): INFINITY,x,y,name);

	case X(_CCC_ZERO,   _CCC_INFINITY):
	    return (y > 0.0) ? 0.0 :__matherr(DIVIDE_BY_ZERO,INFINITY,x,y,name);

	case X(_CCC_INFINITY, _CCC_NORMAL):
	case X(_CCC_INFINITY, _CCC_DENORMAL):
	    yIsOddInteger = yodd(y);
	    if (x > 0.0)
		return (y > 0) ? INFINITY : 0.0;
	    else if (y > 0)
		return (yIsOddInteger) ? -INFINITY : INFINITY ;
	    else
		return (yIsOddInteger) ? -0.0  : 0.0 ;

	case X(_CCC_INFINITY, _CCC_INFINITY):
	    return (y > 0.0) ? INFINITY : 0.0 ;

	case X(_CCC_NAN, _CCC_NORMAL):
	case X(_CCC_NAN, _CCC_DENORMAL):
	case X(_CCC_NAN, _CCC_INFINITY):
	    if (isqnanl(x))
		return  x;
	    return __matherr(DOMAIN,x,x,y,name);

	case X(_CCC_NAN, _CCC_NAN):
	    return _largestNaN(x,y);

	default:
	    *(char*)0 = 0;
    }
    if (y == 1) return x;
    sgn = 0;
    if (x < 0)
    {
	/* see if y is integral	*/
#if __INTSIZE == 4
	if (y >= (long long) 0x8000000000000000LL &&
	    y <= (long long) 0x7FFFFFFFFFFFFFFFLL)
	{
	    if ((long double)(long long)y == y)
	    {	x = -x;
		sgn = (long long)y & 1;
	    }
	    else /* error */
		goto errdom;
	}
	else
	    x = -x;
#else
	if (truncl(y) == y)
	{
	    x = -x;
	    if (fabsl(remainderl(y,2.0)) == 1.0)
		sgn = (long)y & 1;
	}
	else
	    goto errdom;
#endif
    }
    return _pow87(x,y,sgn);

errdom:
  e.type = DOMAIN;
  e.retval = NAN;
  goto err;

err0:
  e.retval = 0.0;
err:
  e.name = (char *)name;
  e.arg1 = x;
  e.arg2 = y;
  matherr(&e);
  return e.retval;
}

#else

float powf(float x,float y)
{
  return (float)pow((double)x,(double)y);
}

#undef powl
long double powl(long double x,long double y)
{
  return (long double)pow((double)x,(double)y);
}

double pow(double x,double y)
{
    int sgn;
    int yIsOddInteger;
    struct exception e;
    static const char name[] = "pow";

  switch( __dtype(x)* 5 + __dtype(y))
  {
   /* Other and */

/* Other    */ case 0 :    break;
/* Zero     */ case 1 :    return 1.0;
/* Infinite */ case 2 :
			   if( fabs(x) == 1.0)
			     return __matherr(DOMAIN,NAN,x,y,name);
			   if(y > 0.0)
			     return (fabs(x) > 1.0) ? INFINITY : 0.0;
			   else
			     return (fabs(x) < 1.0) ? INFINITY : 0.0;
/* SNaN     */ case 3 :    return __matherr(DOMAIN,y,x,y,name);
/* QNaN     */ case 4 :    return  y;

   /* Zero and */
/* Other    */ case 5 :
			   yIsOddInteger = yodd(y);
			   if(y > 0.0)
			     return (yIsOddInteger) ? x : 0.0;
			   else
			     return __matherr(DIVIDE_BY_ZERO,(yIsOddInteger) ? copysign(INFINITY,x): INFINITY,x,y,name);

/* Zero     */ case 6 :    return 1.0;
/* Infinite */ case 7 :    return (y > 0.0) ? 0.0 :__matherr(DIVIDE_BY_ZERO,INFINITY,x,y,name);
/* SNaN     */ case 8 :    return __matherr(DOMAIN,y,x,y,name);
/* QNaN     */ case 9 :    return  y;

   /* Infinite and */
/* Other    */ case 10:
			   yIsOddInteger = yodd(y);
			   if(x > 0.0)
			     return (y > 0) ? INFINITY : 0.0;
			   else
			     if (y > 0)
			       return (yIsOddInteger) ? -INFINITY : INFINITY ;
			     else
			       return (yIsOddInteger) ? -0.0  : 0.0 ;

/* Zero     */ case 11:     return 1.0;
/* Infinite */ case 12:     return (y > 0.0) ? INFINITY : 0.0 ;
/* SNaN     */ case 13:     return __matherr(DOMAIN,y,x,y,name);
/* QNaN     */ case 14:     return  y;

   /* SNaN and */
/* Other    */ case 15:     return __matherr(DOMAIN,x,x,y,name);
/* Zero     */ case 16:     return 1.0;
/* Infinite */ case 17:     return __matherr(DOMAIN,x,x,y,name);
/* SNaN     */ case 18:     return __matherr(DOMAIN,_largestNaN(x,y),x,y, name);
/* QNaN     */ case 19:     return __matherr(DOMAIN,y,x,y,name);

   /* QNaN and */
/* Other    */ case 20:    return  x;
/* Zero     */ case 21:    return  1.0;
/* Infinite */ case 22:    return  x;
/* SNaN     */ case 23:    return __matherr(DOMAIN,x,x,y,name);
/* QNaN     */ case 24:    return _largestNaN(x,y);
  }
  if (y == 1) return x;
  sgn = 0;
  if (x <= 0)
  {	if (x == 0)
	{	if (y <= 0)
			goto errdom;
		return 0.0;
	}
	/* see if y is integral	*/
#if __INTSIZE == 4
	if (y >= (long long) 0x8000000000000000LL &&
	    y <= (long long) 0x7FFFFFFFFFFFFFFFLL)
	{
	    if ((double)(long long)y == y)
	    {	x = -x;
		sgn = (long long)y & 1;
	    }
	    else /* error */
		goto errdom;
	}
	else
	    x = -x;
#else
	if (trunc(y) == y)
	{
	    x = -x;
	    if (fabs(remainder(y,2.0)) == 1.0)
		sgn = (long)y & 1;
	}
	else
	    goto errdom;
#endif
  }
#if __INLINE_8087
    return _pow87(x,y,sgn);
#else
  {
  int pp,p,iw1,mp,m;
  double v,z,W,W1,W2,Y1,Y2,g,R,U1,U2;
  static double K = 0.44269504088896340736e+0;
  static double P[4] =
  {	0.83333333333333211405e-1,
	0.12500000000503799174e-1,
	0.22321421285924258967e-2,
	0.43445775672163119635e-3
  };
  static double q[7] =
  {	0.69314718055994529629e+0,
	0.24022650695905937056e+0,
	0.55504108664085595326e-1,
	0.96181290595172416964e-2,
	0.13333541313585784703e-2,
	0.15400290440989764601e-3,
	0.14928852680595608186e-4
  };
  static long a[17][2] =		/* assume 2 longs make a double	*/
  {	/* These coefficients are from the octal ones in C+W. The	*/
	/* program makea.c created the hex equivalents.			*/
	0x00000000,0x3ff00000,	/* 1.	*/
	0xa2a490da,0x3feea4af,	/* 0.95760328069857365	*/
	0xdcfba487,0x3fed5818,	/* 0.91700404320467114	*/
	0xdd85529c,0x3fec199b,	/* 0.87812608018664963	*/
	0x995ad3ad,0x3feae89f,	/* 0.84089641525371447	*/
	0x82a3f090,0x3fe9c491,	/* 0.80524516597462714	*/
	0x422aa0db,0x3fe8ace5,	/* 0.7711054127039703	*/
	0x73eb0187,0x3fe7a114,	/* 0.7384130729697496	*/
	0x667f3bcd,0x3fe6a09e,	/* 0.70710678118654755	*/
	0xdd485429,0x3fe5ab07,	/* 0.6771277734684463	*/
	0xd5362a27,0x3fe4bfda,	/* 0.64841977732550475	*/
	0x4c123422,0x3fe3dea6,	/* 0.62092890603674195	*/
	0x0a31b715,0x3fe306fe,	/* 0.59460355750136049	*/
	0x6e756238,0x3fe2387a,	/* 0.56939431737834578	*/
	0x3c7d517b,0x3fe172b8,	/* 0.54525386633262877	*/
	0x6cf9890f,0x3fe0b558,	/* 0.52213689121370681	*/
	0x00000000,0x3fe00000	/* 0.5	*/
  };

#define A(i)	(*(double *)&a[i][0])
#define A1(j)	A(-(1 - (j)))
#define A2(j)	(A(-(1 - 2 * (j))) - A1(2 * (j)))

  if (_8087)
	return _pow87(x,y,sgn);
  g = frexp(x,&m);
  p = 1;
  if (g <= A1(9)) p = 9;
  if (g <= A1(p + 4)) p += 4;
  if (g <= A1(p + 2)) p += 2;
  z = ((g - A1(p + 1)) - A2((p + 1) >> 1)) / (g + A1(p + 1));
  z += z;
  v = z * z;
  R = z * v * poly(v,3,P);
  R += K * R;
  U2 = R + z * K;
  U2 += z;
#define REDUCE(V) ldexp((double)(long)(16.0 * (V)),-4)
  U1 = ldexp((double)(m * 16 - p),-4);
  Y1 = REDUCE(y);
  Y2 = y - Y1;
  W = U2 * y + U1 * Y2;
  W1 = REDUCE(W);
  W2 = W - W1;
  W = W1 + U1 * Y1;
  W1 = REDUCE(W);
  W2 += W - W1;
  W = REDUCE(W2);
  iw1 = 16 * (W1 + W);
  W2 -= W;
  if (iw1 >= 16 * DMAXEXP)
  {	e.type = OVERFLOW;
	e.retval = sgn ? -DBL_MAX : DBL_MAX;
	goto err;
  }
  if (iw1 <= 16 * DMINEXP)
  {	e.type = UNDERFLOW;
	goto err0;
  }
  if (W2 > 0)				/* next step requires W2 <= 0	*/
  {	iw1++;
	W2 -= .0625;
  }
  mp = iw1 / 16 + ((iw1 < 0) ? 0 : 1);
  pp = 16 * mp - iw1;
  z = W2 * poly(W2,6,q);
  z = A1(pp + 1) + A1(pp + 1) * z;
  z = ldexp(z,mp);
  feexcept(FE_INEXACT);
  return sgn ? -z : z;
  }
#endif

errdom:
  e.type = DOMAIN;
  e.retval = NAN;
  goto err;

err0:
  e.retval = 0.0;
err:
  e.name = (char *)name;
  e.arg1 = x;
  e.arg2 = y;
  matherr(&e);
  return e.retval;
}


#endif

#if 0
#include <string.h>
#include <assert.h>

void main()
{
    char buffer[10];
    int i;
    static double d = NANS;

    feclearexcept(FE_ALL_EXCEPT);
    printf("%g\n", pow(d, 0));
    printf("%08x%08x\n", *(unsigned *)&d, ((unsigned *)&d)[1]);
    printf("%d\n", fetestexcept(FE_ALL_EXCEPT));
}
#endif
