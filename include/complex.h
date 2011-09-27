
// Copyright (C) 2001 by Digital Mars
// All Rights Reserved
// www.digitalmars.com

#pragma once

#if __cplusplus
extern "C" {
#endif

#define complex		_Complex
#define _Complex_I	((const float _Complex)__imaginary)

#define imaginary	_Imaginary
#define _Imaginary_I	((const float _Imaginary)__imaginary)

#define I		_Imaginary_I

double complex      __cdecl cacos(double complex z);
float complex       __cdecl cacosf(float complex z);
long double complex __cdecl cacosl(long double complex z);

double complex      __cdecl casin(double complex z);
float complex       __cdecl casinf(float complex z);
long double complex __cdecl casinl(long double complex z);

double complex      __cdecl catan(double complex z);
float complex       __cdecl catanf(float complex z);
long double complex __cdecl catanl(long double complex z);

double complex      __cdecl ccos(double complex z);
float complex       __cdecl ccosf(float complex z);
long double complex __cdecl ccosl(long double complex z);

double complex      __cdecl csin(double complex z);
float complex       __cdecl csinf(float complex z);
long double complex __cdecl csinl(long double complex z);

double complex      __cdecl ctan(double complex z);
float complex       __cdecl ctanf(float complex z);
long double complex __cdecl ctanl(long double complex z);

double complex      __cdecl cacosh(double complex z);
float complex       __cdecl cacoshf(float complex z);
long double complex __cdecl cacoshl(long double complex z);

double complex      __cdecl casinh(double complex z);
float complex       __cdecl casinhf(float complex z);
long double complex __cdecl casinhl(long double complex z);

double complex      __cdecl catanh(double complex z);
float complex       __cdecl catanhf(float complex z);
long double complex __cdecl catanhl(long double complex z);

double complex      __cdecl ccosh(double complex z);
float complex       __cdecl ccoshf(float complex z);
long double complex __cdecl ccoshl(long double complex z);

double complex      __cdecl csinh(double complex z);
float complex       __cdecl csinhf(float complex z);
long double complex __cdecl csinhl(long double complex z);

double complex      __cdecl ctanh(double complex z);
float complex       __cdecl ctanhf(float complex z);
long double complex __cdecl ctanhl(long double complex z);

double complex      __cdecl cexp(double complex z);
float complex       __cdecl cexpf(float complex z);
long double complex __cdecl cexpl(long double complex z);

double complex      __cdecl clog(double complex z);
float complex       __cdecl clogf(float complex z);
long double complex __cdecl clogl(long double complex z);

double complex      __cdecl clog10(double complex z);
float complex       __cdecl clog10f(float complex z);
long double complex __cdecl clog10l(long double complex z);

double complex      __cdecl cpow(double complex x, double complex y);
float complex       __cdecl cpowf(float complex x, float complex y);
long double complex __cdecl cpowl(long double complex x, long double complex y);

double complex      __cdecl csqrt(double complex z);
float complex       __cdecl csqrtf(float complex z);
long double complex __cdecl csqrtl(long double complex z);

double complex      __cdecl carg(double complex z);
float complex       __cdecl cargf(float complex z);
long double complex __cdecl cargl(long double complex z);

double complex      __cdecl cconj(double complex z);
float complex       __cdecl cconjf(float complex z);
long double complex __cdecl cconjl(long double complex z);

double complex      __cdecl cproj(double complex z);
float complex       __cdecl cprojf(float complex z);
long double complex __cdecl cprojl(long double complex z);

double      __cdecl cabs(double complex z);
float       __cdecl cabsf(float complex z);
long double __cdecl cabsl(long double complex z);

__inline double      __cdecl creal(double complex z)
{
    return (double) z;
}

__inline float       __cdecl crealf(float complex z)
{
    return (float) z;
}

__inline long double __cdecl creall(long double complex z)
{
    return (long double) z;
}

__inline double      __cdecl cimag(double complex z)
{
    return ((double *)&z)[1];
    //return (_Imaginary double) z * -I;
}

__inline float       __cdecl cimagf(float complex z)
{
    return ((float *)&z)[1];
    //return (_Imaginary float) z * -I;
}

__inline long double __cdecl cimagl(long double complex z)
{
    return ((long double *)&z)[1];
    //return (_Imaginary long double) z * -I;
}

#if __cplusplus
}
#endif


