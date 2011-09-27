/*_atan.c   Sat Apr 15 1989   Modified by: Phil Hinger */
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
#if 0
#define PIOVER2		(M_PI_L / 2)
#define PIOVER3		1.04719755119659774615
#define PIOVER4		(M_PI_L / 4)
#else
#define PIOVER2		1.57079632679489661923
#define PIOVER3		1.04719755119659774615
#define PIOVER4		0.78539816339744830962
#endif
#define PIOVER6		0.52359877559829887308
#define THREEPIOVER4	(PIOVER4 * 3.0)
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

/****************
 * Compute arctan(V / U).
 */

float atanf(float x)
{
    return atan2l(x,1.0);
}

double atan(double x)
{
    return atan2l(x,1.0);
}

long double atanl(long double x)
{
    return atan2l(x,1.0);
}

float atan2f(float V,float U)
{
    return atan2l(V,U);
}

double atan2(double V, double U)
{
    return atan2l(V,U);
}

long double atan2l(long double V, long double U)
{   double result;

#if _WIN32
    //printf("atan2l(%Lg, %Lg)\n", V, U);
    _asm
    {
	fld	tbyte ptr U
	fxam
	fstsw	AX
	mov	DL,AH
	fstp	ST(0)

	fld	tbyte ptr V
	fxam
	fstsw	AX
	mov	AL,DL
	fstp	ST(0)
    }

    #define X(x,y)	((x) | ((y) >> 8))

    switch (_EAX & 0x4747)
    {
	case X(_CCC_NORMAL,   _CCC_NORMAL):
	case X(_CCC_DENORMAL, _CCC_NORMAL):
	case X(_CCC_NORMAL,   _CCC_DENORMAL):
	case X(_CCC_DENORMAL, _CCC_DENORMAL):
	case X(_CCC_NORMAL,   _CCC_NORMAL   | _CCC_NEGATIVE):
	case X(_CCC_DENORMAL, _CCC_NORMAL   | _CCC_NEGATIVE):
	case X(_CCC_NORMAL,   _CCC_DENORMAL | _CCC_NEGATIVE):
	case X(_CCC_DENORMAL, _CCC_DENORMAL | _CCC_NEGATIVE):
	case X(_CCC_NORMAL   | _CCC_NEGATIVE, _CCC_NORMAL):
	case X(_CCC_DENORMAL | _CCC_NEGATIVE, _CCC_NORMAL):
	case X(_CCC_NORMAL   | _CCC_NEGATIVE, _CCC_DENORMAL):
	case X(_CCC_DENORMAL | _CCC_NEGATIVE, _CCC_DENORMAL):
	case X(_CCC_NORMAL   | _CCC_NEGATIVE, _CCC_NORMAL   | _CCC_NEGATIVE):
	case X(_CCC_DENORMAL | _CCC_NEGATIVE, _CCC_NORMAL   | _CCC_NEGATIVE):
	case X(_CCC_NORMAL   | _CCC_NEGATIVE, _CCC_DENORMAL | _CCC_NEGATIVE):
	case X(_CCC_DENORMAL | _CCC_NEGATIVE, _CCC_DENORMAL | _CCC_NEGATIVE):
	    return _atan287(V,U);

	case X(_CCC_NORMAL,   _CCC_ZERO):
	case X(_CCC_DENORMAL, _CCC_ZERO):
	case X(_CCC_INFINITY, _CCC_NORMAL):
	case X(_CCC_INFINITY, _CCC_DENORMAL):
	case X(_CCC_INFINITY, _CCC_ZERO):
	    feexcept(FE_INEXACT);
	    return PIOVER2;

	case X(_CCC_NORMAL,   _CCC_INFINITY):
	case X(_CCC_DENORMAL, _CCC_INFINITY):
	case X(_CCC_ZERO, _CCC_NORMAL):
	case X(_CCC_ZERO, _CCC_DENORMAL):
	case X(_CCC_ZERO, _CCC_ZERO):
	case X(_CCC_ZERO, _CCC_INFINITY):
	    return 0.0;

	case X(_CCC_INFINITY, _CCC_INFINITY):
	    feexcept(FE_INEXACT);
	    return PIOVER4;

	case X(_CCC_NORMAL,   _CCC_ZERO     | _CCC_NEGATIVE):
	case X(_CCC_DENORMAL, _CCC_ZERO     | _CCC_NEGATIVE):
	case X(_CCC_INFINITY, _CCC_NORMAL   | _CCC_NEGATIVE):
	case X(_CCC_INFINITY, _CCC_DENORMAL | _CCC_NEGATIVE):
	case X(_CCC_INFINITY, _CCC_ZERO     | _CCC_NEGATIVE):
	    feexcept(FE_INEXACT);
	    return PIOVER2;


	case X(_CCC_NORMAL,   _CCC_INFINITY | _CCC_NEGATIVE):
	case X(_CCC_DENORMAL, _CCC_INFINITY | _CCC_NEGATIVE):
	case X(_CCC_ZERO,     _CCC_NORMAL   | _CCC_NEGATIVE):
	case X(_CCC_ZERO,     _CCC_DENORMAL | _CCC_NEGATIVE):
	case X(_CCC_ZERO,     _CCC_ZERO     | _CCC_NEGATIVE):
	case X(_CCC_ZERO,     _CCC_INFINITY | _CCC_NEGATIVE):
	    feexcept(FE_INEXACT);
	    return M_PI_L;

	case X(_CCC_INFINITY, _CCC_INFINITY | _CCC_NEGATIVE):
	    feexcept(FE_INEXACT);
	    return THREEPIOVER4;

	case X(_CCC_NORMAL   | _CCC_NEGATIVE, _CCC_ZERO):
	case X(_CCC_DENORMAL | _CCC_NEGATIVE, _CCC_ZERO):
	case X(_CCC_INFINITY | _CCC_NEGATIVE, _CCC_NORMAL):
	case X(_CCC_INFINITY | _CCC_NEGATIVE, _CCC_DENORMAL):
	case X(_CCC_INFINITY | _CCC_NEGATIVE, _CCC_ZERO):
	    feexcept(FE_INEXACT);
	    return -PIOVER2;

	case X(_CCC_NORMAL   | _CCC_NEGATIVE, _CCC_INFINITY):
	case X(_CCC_DENORMAL | _CCC_NEGATIVE, _CCC_INFINITY):
	case X(_CCC_ZERO   | _CCC_NEGATIVE, _CCC_NORMAL):
	case X(_CCC_ZERO   | _CCC_NEGATIVE, _CCC_DENORMAL):
	case X(_CCC_ZERO   | _CCC_NEGATIVE, _CCC_ZERO):
	case X(_CCC_ZERO   | _CCC_NEGATIVE, _CCC_INFINITY):
	    return -0.0;

	case X(_CCC_INFINITY | _CCC_NEGATIVE, _CCC_INFINITY):
	   feexcept(FE_INEXACT);
	   return -PIOVER4;

	case X(_CCC_NORMAL   | _CCC_NEGATIVE, _CCC_ZERO | _CCC_NEGATIVE):
	case X(_CCC_DENORMAL | _CCC_NEGATIVE, _CCC_ZERO | _CCC_NEGATIVE):
	case X(_CCC_INFINITY | _CCC_NEGATIVE, _CCC_NORMAL   | _CCC_NEGATIVE):
	case X(_CCC_INFINITY | _CCC_NEGATIVE, _CCC_DENORMAL | _CCC_NEGATIVE):
	case X(_CCC_INFINITY | _CCC_NEGATIVE, _CCC_ZERO     | _CCC_NEGATIVE):
	   feexcept(FE_INEXACT);
	   return -PIOVER2;

	case X(_CCC_NORMAL   | _CCC_NEGATIVE, _CCC_INFINITY | _CCC_NEGATIVE):
	case X(_CCC_DENORMAL | _CCC_NEGATIVE, _CCC_INFINITY | _CCC_NEGATIVE):
	case X(_CCC_ZERO     | _CCC_NEGATIVE, _CCC_NORMAL   | _CCC_NEGATIVE):
	case X(_CCC_ZERO     | _CCC_NEGATIVE, _CCC_DENORMAL | _CCC_NEGATIVE):
	case X(_CCC_ZERO     | _CCC_NEGATIVE, _CCC_ZERO     | _CCC_NEGATIVE):
	case X(_CCC_ZERO     | _CCC_NEGATIVE, _CCC_INFINITY | _CCC_NEGATIVE):
	    feexcept(FE_INEXACT);
	    return -M_PI_L;

	case X(_CCC_INFINITY | _CCC_NEGATIVE, _CCC_INFINITY | _CCC_NEGATIVE):
	   feexcept(FE_INEXACT);
	   return -THREEPIOVER4;

	case X(_CCC_NORMAL,   _CCC_NAN):
	case X(_CCC_DENORMAL, _CCC_NAN):
	case X(_CCC_ZERO,     _CCC_NAN):
	case X(_CCC_INFINITY, _CCC_NAN):
	case X(_CCC_NORMAL,   _CCC_NAN | _CCC_NEGATIVE):
	case X(_CCC_DENORMAL, _CCC_NAN | _CCC_NEGATIVE):
	case X(_CCC_ZERO,     _CCC_NAN | _CCC_NEGATIVE):
	case X(_CCC_INFINITY, _CCC_NAN | _CCC_NEGATIVE):
	case X(_CCC_NORMAL   | _CCC_NEGATIVE, _CCC_NAN):
	case X(_CCC_DENORMAL | _CCC_NEGATIVE, _CCC_NAN):
	case X(_CCC_ZERO     | _CCC_NEGATIVE, _CCC_NAN):
	case X(_CCC_INFINITY | _CCC_NEGATIVE, _CCC_NAN):
	case X(_CCC_NORMAL   | _CCC_NEGATIVE, _CCC_NAN | _CCC_NEGATIVE):
	case X(_CCC_DENORMAL | _CCC_NEGATIVE, _CCC_NAN | _CCC_NEGATIVE):
	case X(_CCC_ZERO     | _CCC_NEGATIVE, _CCC_NAN | _CCC_NEGATIVE):
	case X(_CCC_INFINITY | _CCC_NEGATIVE, _CCC_NAN | _CCC_NEGATIVE):
	    if (isqnanl(U))
		return U;
	    result = U;
	    goto err;

	case X(_CCC_NAN,                 _CCC_NORMAL):
	case X(_CCC_NAN,                 _CCC_DENORMAL):
	case X(_CCC_NAN,                 _CCC_ZERO):
	case X(_CCC_NAN,                 _CCC_INFINITY):
	case X(_CCC_NAN | _CCC_NEGATIVE, _CCC_NORMAL):
	case X(_CCC_NAN | _CCC_NEGATIVE, _CCC_DENORMAL):
	case X(_CCC_NAN | _CCC_NEGATIVE, _CCC_ZERO):
	case X(_CCC_NAN | _CCC_NEGATIVE, _CCC_INFINITY):
	case X(_CCC_NAN,                 _CCC_NORMAL   | _CCC_NEGATIVE):
	case X(_CCC_NAN,                 _CCC_DENORMAL | _CCC_NEGATIVE):
	case X(_CCC_NAN,                 _CCC_ZERO     | _CCC_NEGATIVE):
	case X(_CCC_NAN,                 _CCC_INFINITY | _CCC_NEGATIVE):
	case X(_CCC_NAN | _CCC_NEGATIVE, _CCC_NORMAL   | _CCC_NEGATIVE):
	case X(_CCC_NAN | _CCC_NEGATIVE, _CCC_DENORMAL | _CCC_NEGATIVE):
	case X(_CCC_NAN | _CCC_NEGATIVE, _CCC_ZERO     | _CCC_NEGATIVE):
	case X(_CCC_NAN | _CCC_NEGATIVE, _CCC_INFINITY | _CCC_NEGATIVE):
	    if (isqnanl(V))
		return V;
	    result = V;
	    goto err;

	case X(_CCC_NAN,                 _CCC_NAN):
	case X(_CCC_NAN,                 _CCC_NAN | _CCC_NEGATIVE):
	case X(_CCC_NAN | _CCC_NEGATIVE, _CCC_NAN):
	case X(_CCC_NAN | _CCC_NEGATIVE, _CCC_NAN | _CCC_NEGATIVE):
	    if (isqnanl(V))
	    {
		if (isqnanl(U))
		    goto Llargestnan;
		result = U;
	    }
	    else
	    {
		if (!isqnanl(U))
		    goto Llargestnan;
		result = V;
	    }
	    goto err;

	Llargestnan:
	    return _largestNaN(V,U);

	default:
	    result = NAN;
	    goto err;
    }
#else

  switch(50 * signbit(V) + 25 * signbit(U) + __dtype((double)V)* 5 + __dtype((double)U))
  {
    /* +V and + U */

   /* Other and */

/* Other    */ case 0 :    break;
/* Zero     */ case 1 :    feexcept(FE_INEXACT); return PIOVER2;
/* Infinite */ case 2 :    return 0.0;
/* SNaN     */ case 3 :    result = U; goto err;
/* QNaN     */ case 4 :    return  U;

   /* Zero and */
/* Other    */ case 5 :
/* Zero     */ case 6 :
/* Infinite */ case 7 :    return 0.0;
/* SNaN     */ case 8 :    result = U; goto err;
/* QNaN     */ case 9 :    return  U;

   /* Infinite and */
/* Other    */ case 10:
/* Zero     */ case 11:    feexcept(FE_INEXACT);return PIOVER2;
/* Infinite */ case 12:    feexcept(FE_INEXACT);return PIOVER4;
/* SNaN     */ case 13:    result = U; goto err;
/* QNaN     */ case 14:    return  U;

   /* SNaN and */
/* Other    */ case 15:
/* Zero     */ case 16:
/* Infinite */ case 17:    result = V; goto err;
/* SNaN     */ case 18:    result = _largestNaN(V,U); goto err;
/* QNaN     */ case 19:    result = V; goto err;

   /* QNaN and */
/* Other    */ case 20:
/* Zero     */ case 21:
/* Infinite */ case 22:    return V;
/* SNaN     */ case 23:    result = V; goto err;
/* QNaN     */ case 24:    return _largestNaN(V,U);

    /* +V and -U */

   /* Other and */

/* Other    */ case 25:     break;
/* Zero     */ case 26:     feexcept(FE_INEXACT);return PIOVER2;
/* Infinite */ case 27:     feexcept(FE_INEXACT);return M_PI;
/* SNaN     */ case 28:     result = U; goto err;
/* QNaN     */ case 29:     return  U;

   /* Zero and */
/* Other    */ case 30:
/* Zero     */ case 31:
/* Infinite */ case 32:     feexcept(FE_INEXACT); return M_PI;
/* SNaN     */ case 33:     result = U; goto err;
/* QNaN     */ case 34:     return  U;

   /* Infinite and */
/* Other    */ case 35:
/* Zero     */ case 36:     feexcept(FE_INEXACT); return PIOVER2;
/* Infinite */ case 37:     feexcept(FE_INEXACT); return THREEPIOVER4;
/* SNaN     */ case 38:     result = U; goto err;
/* QNaN     */ case 39:     return  U;

   /* SNaN and */
/* Other    */ case 40:
/* Zero     */ case 41:
/* Infinite */ case 42:     result = V; goto err;
/* SNaN     */ case 43:     result = _largestNaN(V,U); goto err;
/* QNaN     */ case 44:     result = U; goto err;

   /* QNaN and */
/* Other    */ case 45:
/* Zero     */ case 46:
/* Infinite */ case 47:    return  V;
/* SNaN     */ case 48:    result = U; goto err;
/* QNaN     */ case 49:    return _largestNaN(V,U);

    /* -V and + U */

   /* Other and */

/* Other    */ case 50:    break;
/* Zero     */ case 51:    feexcept(FE_INEXACT);return -PIOVER2;
/* Infinite */ case 52:    return -0.0;
/* SNaN     */ case 53:    result = U; goto err;
/* QNaN     */ case 54:    return U;

   /* Zero and */
/* Other    */ case 55:
/* Zero     */ case 56:
/* Infinite */ case 57:    return -0.0;
/* SNaN     */ case 58:    result = U; goto err;
/* QNaN     */ case 59:    return  U;

   /* Infinite and */
/* Other    */ case 60:
/* Zero     */ case 61:    feexcept(FE_INEXACT);return -PIOVER2;
/* Infinite */ case 62:    feexcept(FE_INEXACT);return -PIOVER4;
/* SNaN     */ case 63:    result = U; goto err;
/* QNaN     */ case 64:    return U;

   /* SNaN and */
/* Other    */ case 65:
/* Zero     */ case 66:
/* Infinite */ case 67:    result = V; goto err;
/* SNaN     */ case 68:    result = _largestNaN(V,U); goto err;
/* QNaN     */ case 69:    result = U; goto err;

   /* QNaN and */
/* Other    */ case 70:
/* Zero     */ case 71:
/* Infinite */ case 72:    return  V;
/* SNaN     */ case 73:    result = V; goto err;
/* QNaN     */ case 74:    return _largestNaN(V,U);

    /* -V and -U */

   /* Other and */

/* Other    */ case 75:    break;
/* Zero     */ case 76:    feexcept(FE_INEXACT);return -PIOVER2;
/* Infinite */ case 77:    feexcept(FE_INEXACT);return -M_PI;
/* SNaN     */ case 78:    result = U; goto err;
/* QNaN     */ case 79:    return U;

   /* Zero and */
/* Other    */ case 80:
/* Zero     */ case 81:
/* Infinite */ case 82:    feexcept(FE_INEXACT);return -M_PI;
/* SNaN     */ case 83:    result = U; goto err;
/* QNaN     */ case 84:    return  U;

   /* Infinite and */
/* Other    */ case 85:
/* Zero     */ case 86:    feexcept(FE_INEXACT);return -PIOVER2;
/* Infinite */ case 87:    feexcept(FE_INEXACT);return -THREEPIOVER4;
/* SNaN     */ case 88:    result = U; goto err;
/* QNaN     */ case 89:    return  U;

   /* SNaN and */
/* Other    */ case 90:
/* Zero     */ case 91:
/* Infinite */ case 92:     result = V;			goto err;
/* SNaN     */ case 93:     result = _largestNaN(V,U);	goto err;
/* QNaN     */ case 94:     result = U;			goto err;

   /* QNaN and */
/* Other    */ case 95:
/* Zero     */ case 96:
/* Infinite */ case 97:    return  V;
/* SNaN     */ case 98:    result = V; goto err;
/* QNaN     */ case 99:    return _largestNaN(V,U);
  }

#if __INLINE_8087
    return _atan287(V,U);
#else
    {
	int oldStatus,newStatus;
	int n;
	double g,f;
	static double p[4] =
	{	-0.13688768894191926929e+2,
		-0.20505855195861651981e+2,
		-0.84946240351320683534e+1,
		-0.83758299368150059274e+0
	};
	static double q[5] =
	{	 0.41066306682575781263e+2,
		 0.86157349597130242515e+2,
		 0.59578436142597344465e+2,
		 0.15024001160028576121e+2,
		 0.10000000000000000000e+1
	};
	static double a[4] =
	{	0.0, PIOVER6, PIOVER2, PIOVER3	};
#if 0
#define TWOMINUSSQRT3	0.26794919243112270647
#else
/* From Cody+Waite pg. 204: octal 0.21114 12136 47546 52614	*/
static long twominussqrt3[2] = {0x5e9ecd56,0x3fd12614};
#define TWOMINUSSQRT3	(*(double *)twominussqrt3)
#endif

	if (_8087)
		return _atan287(V,U);

	oldStatus = fetestexcept(FE_ALL_EXCEPT);
	feclearexcept(FE_ALL_EXCEPT);
	f = fabs(V / U);
	newStatus = fetestexcept(FE_ALL_EXCEPT);
	feexcept(oldStatus);

	if (newStatus & FE_OVERFLOW )	      /* then overflow on V/U */
	{
		result = PIOVER2;
		goto C;
	}
	if (newStatus & FE_UNDERFLOW)	     /* then underflow on V/U */
	{
		result = 0.0;
		goto B;
	}
	if (f > 1.0)
	{	f = 1 / f;
		n = 2;
	}
	else
		n = 0;
	if (f > TWOMINUSSQRT3)
	{	double tmp;	/* to make sure of order of evaluation	*/

		tmp = (SQRT3 - 1) * f - 0.5;
		tmp -= 0.5;
		f = (tmp + f) / (SQRT3 + f);
		n++;
	}
	if (fabs(f) < EPS)
		result = f;
	else
	{	g = f * f;
		result = (g * poly(g,3,p)) / poly(g,4,q);
		result = f + f * result;
	}
	if (n > 1)
		result = -result;
	result += a[n];
    B:	if (U < 0.0)
		result = M_PI - result;
    C:	if (V < 0.0)
		result = -result;
	feexcept(FE_INEXACT);
	return result;
    }
#endif
#endif
    err:
	return __matherr(DOMAIN,result,V,U,"atan2");
}

#if 0
void main()
{
    long double r;

    r = atanl(__inf);
    printf("r = %Lg\n", r);
}
#endif
