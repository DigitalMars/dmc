
/* Copyright (C) 1986-2003 by Digital Mars.
 * All Rights Reserved
 * www.digitalmars.com
 */

#if __DMC__ || __RCC__
#pragma once
#endif

#ifndef __MATH_H
#define __MATH_H 1

#if __cplusplus
extern "C" {
#endif

/* Define _CRTAPI1 (for compatibility with the NT SDK) */
#ifndef _CRTAPI1
#define _CRTAPI1 __cdecl
#endif

/* Define _CRTAPI2 (for compatibility with the NT SDK) */
#ifndef _CRTAPI2
#define _CRTAPI2 __cdecl
#endif

/* Define CRTIMP */
#ifndef _CRTIMP
#if defined(_WIN32) && defined(_DLL)
#define _CRTIMP  __declspec(dllimport)
#else
#define _CRTIMP
#endif
#endif

typedef long double float_t;
typedef long double double_t;

#define HUGE_VAL	__inf
#define HUGE_VALF	((float)__inf)
#define HUGE_VALL	((long double)__inf)

#define INFINITY	__inf
#define	NAN		__nan

#ifndef __STDC__
#define NANS		__nans
#endif

#define	FP_NANS		0
#define FP_NANQ		1
#define	FP_INFINITE	2
#define	FP_NORMAL	3
#define	FP_SUBNORMAL	4
#define	FP_ZERO		5

#define FP_NAN		FP_NANQ
#define FP_EMPTY	6
#define FP_UNSUPPORTED	7

#if __INTSIZE == 2
#define FP_ILOGB0	(-32768)
#define FP_ILOGBNAN	(-32768)
#else
#define FP_ILOGB0	(-2147483647L - 1)
#define FP_ILOGBNAN	(-2147483647L - 1)
#endif

#define MATH_ERRNO	1
#define MATH_ERREXCEPT	2
#define math_errhandling (MATH_ERRNO | MATH_ERREXCEPT)

unsigned __cdecl __fpclassify_f(float);
unsigned __cdecl __fpclassify_d(double);
unsigned __cdecl __fpclassify_ld(long double);

unsigned __cdecl __fpxam_ld(long double);

#if __cplusplus
extern "C++" {
inline int fpclassify(float fe)	{ return __fpclassify_f(fe); }
inline int fpclassify(double fe){ return __fpclassify_d(fe); }
inline int fpclassify(long double fe)
	{ return (sizeof(long double) == sizeof(double))
		? __fpclassify_d(fe)
		: __fpclassify_ld(fe);
	}

inline int signbit(float fe)	{ return ((short *)&(fe))[1] & 0x8000; }
inline int signbit(double fe)	{ return ((short *)&(fe))[3] & 0x8000; }
inline int signbit(long double fe)
	{ return (sizeof(long double) == sizeof(double))
		? ((short *)&(fe))[3] & 0x8000
		: ((short *)&(fe))[4] & 0x8000;
	}

inline int isfinite(float fe)	{ return fpclassify(fe) >= FP_NORMAL; }
inline int isfinite(double fe)	{ return fpclassify(fe) >= FP_NORMAL; }
inline int isfinite(long double fe) { return fpclassify(fe) >= FP_NORMAL; }

inline int isnormal(float fe)	{ return fpclassify(fe) == FP_NORMAL; }
inline int isnormal(double fe)	{ return fpclassify(fe) == FP_NORMAL; }
inline int isnormal(long double fe) { return fpclassify(fe) == FP_NORMAL; }

inline int isnan(float fe)	{ return fpclassify(fe) <= FP_NANQ;   }
inline int isnan(double fe)	{ return fpclassify(fe) <= FP_NANQ;   }
inline int isnan(long double fe) { return fpclassify(fe) <= FP_NANQ;   }

inline int isinf(float fe)	{ return fpclassify(fe) == FP_INFINITE; }
inline int isinf(double fe)	{ return fpclassify(fe) == FP_INFINITE; }
inline int isinf(long double fe) { return fpclassify(fe) == FP_INFINITE; }

}
#else
#define fpclassify(fe)	(sizeof(fe) == sizeof(float) ?			\
			    __fpclassify_f(fe) :			\
			    (sizeof(fe) == sizeof(double) ? __fpclassify_d(fe) : __fpclassify_ld(fe)) \
			)
#if 1
#define signbit __signbit
__inline int __signbit(long double fe)
	{ return (sizeof(long double) == sizeof(double))
		? ((short *)&(fe))[3] & 0x8000
		: ((short *)&(fe))[4] & 0x8000;
	}
#else
#define signbit(fe)	(sizeof(fe) == sizeof(float) ?			\
			    (int)(((short *)&(fe))[1] & 0x8000) :	\
			    (sizeof(fe) == sizeof(double) ? (int)(((short *)&(fe))[3] & 0x8000)  \
							  : (int)(((short *)&(fe))[4] & 0x8000)) \
			)
#endif
#define isfinite(fe)	(fpclassify(fe) >= FP_NORMAL)
#define isnormal(fe)	(fpclassify(fe) == FP_NORMAL)
#define isnan(fe)	(fpclassify(fe) <= FP_NANQ)
#define isinf(fe)	(fpclassify(fe) == FP_INFINITE)
#endif


#if __OS2__ && __INTSIZE == 4
#define __CLIB	__stdcall
#else
#define __CLIB	__cdecl
#endif

double	__CLIB acos(double);
float	__CLIB acosf(float);
long double __CLIB acosl(long double);

double	__CLIB asin(double);
float	__CLIB asinf(float);
long double __CLIB asinl(long double);

double	__CLIB atan(double);
float	__CLIB atanf(float);
long double __CLIB atanl(long double);

double	__CLIB atan2(double,double);
float	__CLIB atan2f(float,float);
long double __CLIB atan2l(long double, long double);

float		__CLIB cosf(float);
#if !(__INLINE_8087 && __I86__ >= 3)
double		__CLIB cos(double);
#endif
long double	__CLIB cosl(long double);
float		__CLIB _inline_cosf(float);
double		__CLIB _inline_cos(double);
long double	__CLIB _inline_cosl(long double);
#if __INLINE_8087 && __I86__ >= 3
#if __cplusplus
extern "C++" {
inline double      cos(double x)      { return _inline_cos(x);  }
}
#else
#define cos(d)  _inline_cos(d)
#endif
#define cosf(f) _inline_cosf(f)
#define cosl(d) _inline_cosl(d)
#elif !_WIN32
#define cosl(d) cos(d)
#endif
//#define _cosl	cosl

float		__CLIB sinf(float);
#if !(__INLINE_8087 && __I86__ >= 3)
double		__CLIB sin(double);
#endif
long double	__CLIB sinl(long double);
float		__CLIB _inline_sinf(float);
double		__CLIB _inline_sin(double);
long double	__CLIB _inline_sinl(long double);
#if __INLINE_8087 && __I86__ >= 3
#if __cplusplus
extern "C++" {
inline double      sin(double x)      { return _inline_sin(x);  }
}
#else
#define sin(d)  _inline_sin(d)
#endif
#define sinf(f) _inline_sinf(f)
#define sinl(d) _inline_sinl(d)
#elif !_WIN32
#define sinl(d) sin(d)
#endif

double	__CLIB tan(double);
float	__CLIB tanf(float);
long double __CLIB tanl(long double);

double __CLIB acosh(double x);
float __CLIB acoshf(float x);
long double __CLIB acoshl(long double x);

double __CLIB asinh(double x);
float __CLIB asinhf(float x);
long double __CLIB asinhl(long double x);

double __CLIB atanh(double x);
float __CLIB atanhf(float x);
long double __CLIB atanhl(long double x);


double	__CLIB cosh(double);
float	__CLIB coshf(float);
long double __CLIB coshl(long double);

double	__CLIB sinh(double);
float	__CLIB sinhf(float);
long double __CLIB sinhl(long double);

double	__CLIB tanh(double);
float	__CLIB tanhf(float);
long double __CLIB tanhl(long double);

double	__CLIB exp(double);
float	__CLIB expf(float);
long double __CLIB expl(long double);

double	__CLIB exp2(double);
float	__CLIB exp2f(float);
long double __CLIB exp2l(long double);

double	__CLIB expm1(double);
float	__CLIB expm1f(float);
long double __CLIB expm1l(long double);

double	__CLIB frexp(double,int *);
float	__CLIB frexpf(float,int *);
#if _WIN32
long double __CLIB frexpl(long double,int *);
#else
#define frexpl frexp
#endif

int __CLIB ilogb(double);
int __CLIB ilogbf(float);
int __CLIB ilogbl(long double);

double	__CLIB ldexp(double,int);
float	__CLIB ldexpf(float,int);
double	__CLIB _inline_ldexp(double,int);
float	__CLIB _inline_ldexpf(float,int);
#if _WIN32
long double	__CLIB ldexpl(long double, int);
long double	__CLIB _inline_ldexpl(long double, int);
#else
#define ldexpl	ldexp
#endif
#if __INLINE_8087 && __I86__ >= 3
#define ldexp(d,i)  _inline_ldexp(d,i)
#define ldexpf(f,i) _inline_ldexpf(f,i)
#define ldexpl(d,i) _inline_ldexpl(d,i)
#endif

#if __INLINE_8087
long double __cdecl _inline_yl2x(long double x, long double y);
long double __cdecl _inline_yl2xp1(long double x, long double y);
#endif

double	__CLIB log(double);
float	__CLIB logf(float);
long double __CLIB logl(long double);

double	__CLIB log10(double);
float	__CLIB log10f(float);
long double __CLIB log10l(long double);

double	__CLIB log1p(double);
float	__CLIB log1pf(float);
#if _WIN32
long double	__CLIB log1pl(long double);
#else
#define log1pl	log1p
#endif

double	__CLIB log2(double);
float	__CLIB log2f(float);
long double __CLIB log2l(long double);

double	__CLIB logb(double);
float	__CLIB logbf(float);
#if _WIN32
long double __CLIB logbl(long double);
#else
#define logbl		logb
#endif

double	__CLIB modf(double,double *);
float	__CLIB modff(float,float *);
long double __CLIB modfl(long double, long double *);

double	__CLIB scalbn(double, int);
float	__CLIB scalbnf(float, int);
long double __CLIB scalbnl(long double, int);

double	__CLIB scalbln(double, long);
float	__CLIB scalblnf(float, long);
long double __CLIB scalblnl(long double, long);

double	__CLIB cbrt(double);
float	__CLIB cbrtf(float);
long double __CLIB cbrtl(long double);

float		__CLIB fabsf(float);
long double	__CLIB fabsl(long double);
float		__CLIB _inline_fabsf(float);
double		__CLIB _inline_fabs(double);
long double	__CLIB _inline_fabsl(long double);
#if __cplusplus
extern "C++" {
inline double      fabs(double x)      { return _inline_fabs(x);  }
}
#else
double		__CLIB fabs(double);
#define fabs(d)  _inline_fabs(d)
#endif
#define fabsf(f) _inline_fabsf(f)
#define fabsl(d) _inline_fabsl(d)

float	__CLIB hypotf(float, float);
double	__CLIB hypot(double,double);
long double __CLIB hypotl(long double, long double);

double	__CLIB pow(double,double);
float	__CLIB powf(float,float);
long double __CLIB powl(long double, long double);

float		__CLIB sqrtf(float);
#if !__INLINE_8087 && !__cplusplus
double		__CLIB sqrt(double);
#endif
long double	__CLIB sqrtl(long double);
float		__CLIB _inline_sqrtf(float);
double		__CLIB _inline_sqrt(double);
long double	__CLIB _inline_sqrtl(long double);
#if __INLINE_8087
#if __cplusplus
extern "C++" {
inline double      sqrt(double x)      { return _inline_sqrt(x);  }
}
#else
#define sqrt(d)  _inline_sqrt(d)
#endif
#define sqrtf(f) _inline_sqrtf(f)
#define sqrtl(d) _inline_sqrtl(d)
#elif !_WIN32
#define sqrtl(d) sqrt(d)
#endif


double __CLIB erf(double x);
float __CLIB erff(float x);
long double __CLIB erfl(long double x);

double __CLIB erfc(double x);
float __CLIB erfcf(float x);
long double __CLIB erfcl(long double x);

double __CLIB lgamma(double x);
float __CLIB lgammaf(float x);
long double __CLIB lgammal(long double x);

double __CLIB tgamma(double x);
float __CLIB tgammaf(float x);
long double __CLIB tgammal(long double x);

float		__CLIB ceilf(float);
double		__CLIB ceil(double);
long double	__CLIB ceill(long double);

float		__CLIB floorf(float);
double		__CLIB floor(double);
long double	__CLIB floorl(long double);

double __cdecl nearbyint(double);
float __cdecl nearbyintf(float);
long double __cdecl nearbyintl(long double);
#if !_WIN32
#define nearbyintl	nearbyint
#endif

float		__cdecl rintf(float);
double		__cdecl rint(double);
long double	__cdecl rintl(long double);
float		__cdecl _inline_rintf(float);
double		__cdecl _inline_rint(double);
long double	__cdecl _inline_rintl(long double);
#if __INLINE_8087
#define rint(d)  _inline_rint(d)
#define rintf(f) _inline_rintf(f)
#define rintl(d) _inline_rintl(d)
#elif !_WIN32
#define rintl(d) rint(d)
#endif

long int __cdecl lrint(double x);
long int __cdecl lrintf(float x);
long int __cdecl lrintl(long double x);

#if _WIN32
long long int __cdecl llrint(double x);
long long int __cdecl llrintf(float x);
long long int __cdecl llrintl(long double x);
#endif

double __cdecl round(double );
float __cdecl roundf(float );
long double __cdecl roundl(long double );
#if !_WIN32
#define roundl		round
#endif

long int __cdecl lround(double x);
long int __cdecl lroundf(float x);
long int __cdecl lroundl(long double x);

#if _WIN32
long long int __cdecl llround(double x);
long long int __cdecl llroundf(float x);
long long int __cdecl llroundl(long double x);
#endif

double __cdecl trunc(double);
float __cdecl truncf(float);
long double __cdecl truncl(long double);
#if !_WIN32
#define truncl		trunc
#endif

double	__CLIB fmod(double,double);
float	__CLIB fmodf(float,float);
long double __CLIB fmodl(long double, long double);

double __cdecl remainder(double, double);
float __cdecl remainderf(float, float);
long double __cdecl remainderl(long double, long double);

double __cdecl remquo(double, double, int *);
float __cdecl remquof(float, float, int *);
long double __cdecl remquol(long double, long double, int *);
#define remquol		remquo

double		__cdecl copysign(double, double);
float		__cdecl copysignf(float, float);
long double	__cdecl copysignl(long double, long double);
#if !_WIN32
#define copysignl	copysign
#endif

double __cdecl nan(const char *);
float __cdecl nanf(const char *);
long double __cdecl nanl(const char *);
#if !_WIN32
#define nanl		(long double)nan
#endif

double __cdecl nextafter(double, double);
float __cdecl nextafterf(float, float);
long double __cdecl nextafterl(long double, long double);
#define nextafterl	(long double)nextafter

double __cdecl nexttoward(double, double);
float __cdecl nexttowardf(float, float);
long double __cdecl nexttowardl(long double, long double);

double __cdecl fdim(double, double);
float __cdecl fdimf(float, float);
long double __cdecl fdiml(long double, long double);

double __cdecl fmax(double, double);
float __cdecl fmaxf(float, float);
long double __cdecl fmaxl(long double, long double);

double __cdecl fmin(double, double);
float __cdecl fminf(float, float);
long double __cdecl fminl(long double, long double);

double __cdecl fma(double, double, double);
float __cdecl fmaf(float, float, float);
long double __cdecl fmal(long double, long double, long double);


#define isgreater(x,y)		!((x) !>  (y))
#define isgreaterequal(x,y)	!((x) !>= (y))
#define isless(x,y)		!((x) !<  (y))
#define islessequal(x,y)	!((x) !<= (y))
#define islessgreater(x,y)	!((x) !<> (y))
#define isunordered(x,y)	((x) !<>= (y))

#ifndef __STDC__

#define _matherrl _matherr

#if 0 // obsolete
#define PI		3.14159265358979323846
#define LOG2		0.30102999566398119521
#define LN2		0.6931471805599453094172321
#define LOG2T		3.32192809488736234787
#define LOG2E		1.4426950408889634074
#endif

#define M_LOG2T		3.32192809488736234787
#define M_LOG2		0.30102999566398119521
#define	M_LOG2E		1.4426950408889634074
#define	M_LN2		0.6931471805599453094172321
#define	M_PI		3.14159265358979323846
#define	M_E		2.7182818284590452354
#define	M_LOG10E	0.43429448190325182765
#define	M_LN10		2.30258509299404568402
#define	M_PI_2		1.57079632679489661923
#define	M_PI_4		0.78539816339744830962
#define	M_1_PI		0.31830988618379067154
#define	M_2_PI		0.63661977236758134308
#define	M_2_SQRTPI	1.12837916709551257390
#define	M_SQRT2		1.41421356237309504880
#define	M_SQRT1_2	0.70710678118654752440

// 80 bit
#define	M_PI_L		0x1.921fb54442d1846ap+1L	// 3.14159 fldpi
#define	M_LOG2T_L	0x1.a934f0979a3715fcp+1L	// 3.32193 fldl2t
#define	M_LOG2E_L	0x1.71547652b82fe178p+0L	// 1.4427 fldl2e
#define	M_LOG2_L	0x1.34413509f79fef32p-2L	// 0.30103 fldlg2
#define	M_LN2_L		0x1.62e42fefa39ef358p-1L	// 0.693147 fldln2

#ifndef _EXCEPTION_DEFINED

struct _exception
{	int type;
	char *name;
	double arg1;
	double arg2;
	double retval;
};

#if !__STDC__
#define exception _exception
#endif

#define _exceptionl _exception

#define _EXCEPTION_DEFINED
#endif 


#define _DOMAIN		1
#define _SING		2
#define	_OVERFLOW	3
#define _UNDERFLOW	4
#define _TLOSS		5
#define _PLOSS		6

#define EDOM            33
#define ERANGE          34

#ifndef __STDC__
#define DOMAIN		_DOMAIN
#define SING		_SING
#define	OVERFLOW	_OVERFLOW
#define UNDERFLOW	_UNDERFLOW
#define TLOSS		_TLOSS
#define PLOSS		_PLOSS
#define DIVIDE_BY_ZERO  7
#endif

int	__CLIB matherr(struct exception *);
double	__CLIB atof(const char *);
#define _atold atof
#define _matherr matherr
double	__CLIB poly(double,int,double []);
#define polyl poly
double _poly87(double, int, double []);

#if __INLINE_8087
#define poly _poly87
#endif


#endif

#if __cplusplus
}
#endif

#endif
