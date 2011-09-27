
// Copyright (C) 2001 by Digital Mars
// All Rights Reserved
// www.digitalmars.com

#include <math.h>
#include <complex.h>
#include <assert.h>
#include <fp.h>

long double complex __cdecl polarl(long double r, long double theta )
{
  return r * (cos(theta) + sin(theta) * I);
}


double complex      __cdecl cacos(double complex z)
{
    return cacosl(z);
}

float complex       __cdecl cacosf(float complex z)
{
    return cacosl(z);
}

long double complex __cdecl cacosl(long double complex z)
{
    return -I * clogl(z + I * csqrtl(1 - z * z));
}

double complex      __cdecl casin(double complex z)
{
    return casinl(z);
}

float complex       __cdecl casinf(float complex z)
{
    return casinl(z);
}

long double complex __cdecl casinl(long double complex z)
{
    return -I * clogl(I * z + csqrtl(1 - z * z));
}


double complex      __cdecl catan(double complex z)
{
    return catanl(z);
}

float complex       __cdecl catanf(float complex z)
{
    return catanl(z);
}

long double complex __cdecl catanl(long double complex z)
{
    return -clogl((1 + z * I)/(1 - z * I)) / 2 * I;
}


double complex      __cdecl ccos(double complex z)
{
    return ccosl(z);
}

float complex       __cdecl ccosf(float complex z)
{
    return ccosl(z);
}

long double complex __cdecl ccosl(long double complex z)
{   long double z_re = creall(z);
    long double z_im = cimagl(z);

    return cos(z_re) * cosh(z_im) - sin(z_re) * sinh(z_im) * I;
}

double complex      __cdecl csin(double complex z)
{
    return csinl(z);
}

float complex       __cdecl csinf(float complex z)
{
    return csinl(z);
}

long double complex __cdecl csinl(long double complex z)
{   long double z_re = creall(z);
    long double z_im = cimagl(z);

    return sin(z_re) * cosh(z_im) + cos(z_re) * sinh(z_im) * I;
}

double complex      __cdecl ctan(double complex z)
{
    return ctanl(z);
}

float complex       __cdecl ctanf(float complex z)
{
    return ctanl(z);
}

long double complex __cdecl ctanl(long double complex z)
{
    long double x = 2 * creall(z);
    long double y = 2 * cimagl(z);
    long double t = 1.0 / (cos(x) + cosh(y));

    return t * (sin(x) + sinh(y) * I);
}

double complex      __cdecl cacosh(double complex z)
{
    return cacoshl(z);
}

float complex       __cdecl cacoshf(float complex z)
{
    return cacoshl(z);
}

long double complex __cdecl cacoshl(long double complex z)
{
    return cacosl(z) * I;
}

double complex      __cdecl casinh(double complex z)
{
    return casinhl(z);
}

float complex       __cdecl casinhf(float complex z)
{
    return casinhl(z);
}

long double complex __cdecl casinhl(long double complex z)
{
    return -casinl(z * I) * I;
}

double complex      __cdecl catanh(double complex z)
{
    return catanhl(z);
}

float complex       __cdecl catanhf(float complex z)
{
    return catanhl(z);
}

long double complex __cdecl catanhl(long double complex z)
{
    return -catanl(z * I) * I;
}

double complex      __cdecl ccosh(double complex z)
{
    return ccoshl(z);
}

float complex       __cdecl ccoshf(float complex z)
{
    return ccoshl(z);
}

long double complex __cdecl ccoshl(long double complex z)
{   long double z_re = creall(z);
    long double z_im = cimagl(z);

    return cosh(z_re) * cos(z_im) + sinh(z_re) * sin(z_im) * I;
}

double complex      __cdecl csinh(double complex z)
{
    return csinhl(z);
}

float complex       __cdecl csinhf(float complex z)
{
    return csinhl(z);
}

long double complex __cdecl csinhl(long double complex z)
{   long double z_re = creall(z);
    long double z_im = cimagl(z);

    return sinh(z_re) * cos(z_im) + cosh(z_re) * sin(z_im) * I;
}

double complex      __cdecl ctanh(double complex z)
{
    return ctanhl(z);
}

float complex       __cdecl ctanhf(float complex z)
{
    return ctanhl(z);
}

long double complex __cdecl ctanhl(long double complex z)
{   long double z_re = creall(z);
    long double z_im = cimagl(z);

    long double x = 2 * z_re;
    long double y = 2 * z_im;
    long double t = 1.0 / (cosh(x) + cos(y));

    return t * (sinh(x) + sin(y) * I);
}

double complex      __cdecl cexp(double complex z)
{
    return cexpl(z);
}

float complex       __cdecl cexpf(float complex z)
{
    return cexpl(z);
}

long double complex __cdecl cexpl(long double complex z)
{   long double z_re = creall(z);
    long double z_im = cimagl(z);

    long double x = exp(z_re);

    return x * (cos(z_im) + sin(z_im) * I);
}

double complex      __cdecl clog(double complex z)
{
    return clogl(z);
}

float complex       __cdecl clogf(float complex z)
{
    return clogl(z);
}

long double complex __cdecl clogl(long double complex z)
{
    return log(cabsl(z)) + cargl(z) * I;
}

double complex      __cdecl clog10(double complex z)
{
    return clog10l(z);
}

float complex       __cdecl clog10f(float complex z)
{
    return clog10l(z);
}

long double complex __cdecl clog10l(long double complex z)
{   long double z_re = creall(z);
    long double z_im = cimagl(z);

    long double norm = z_re * z_re + z_im * z_im;

    return 0.2171472409516259 * log(norm) + cargl(z) * I;
}

double complex      __cdecl cpow(double complex x, double complex y)
{
    return cpowl(x, y);
}
float complex       __cdecl cpowf(float complex x, float complex y)
{
    return cpowl(x, y);
}
long double complex __cdecl cpowl(long double complex x, long double complex y)
{
    long double result;
    long double y_re;
    long double y_im;

    long double r;
    long double theta;

    if (y == 0)
	result = 1.0;
    else
    {	y_re = (long double) y;
	y_im = cimagl(y);

	// Convert x to polar form (r,theta)
	r = cabsl(x);
	theta = cargl(x);

	if (y_im == 0)
	{
	    r = pow(r, y_re);
	    theta *= y_re;
	    result = r * cos(theta) + r * sin(theta) * I;
	}
	else
	{
	    long double logf_re, logf_im;
	    long double phase_re, phase_im;
	    long double x_re;
	    long double x_im;

	    x_re = (long double) x;
	    x_im = cimagl(x);

	    logf_re = log(r);
	    logf_im = theta;
	    phase_re = exp(logf_re * y_re - logf_im * y_im);
	    phase_im = logf_re * y_im + logf_im * y_re;
	    result = phase_re * cos(phase_im) + phase_re * sin(phase_im) * I;
	}
    }
    return result;
}

double complex      __cdecl csqrt(double complex z)
{
    return csqrtl(z);
}

float complex       __cdecl csqrtf(float complex z)
{
    return csqrtl(z);
}

long double complex __cdecl csqrtl(long double complex z)
{
    long double complex c;
    long double x,y,w,r;

    if (z == 0)
    {
	c = 0;
    }
    else
    {	long double z_re = (long double) z;
	long double z_im = cimagl(z);

	x = fabs(z_re);
	y = fabs(z_im);
	if (x >= y)
	{
	    r = y / x;
	    w = sqrt(x) * sqrt(0.5 * (1 + sqrt(1 + r * r)));
	}
	else
	{
	    r = x / y;
	    w = sqrt(y) * sqrt(0.5 * (r + sqrt(1 + r * r)));
	}

	if (z_re >= 0)
	{
	    c = w + (z_im / (w + w)) * I;
	}
	else
	{
	    if (z_im < 0)
		w = -w;
	    c = z_im / (w + w) + w * I;
	}
    }
    return c;
}

double complex      __cdecl carg(double complex z)
{
    return atan2(cimag(z), (double)z);
}

 float complex       __cdecl cargf(float complex z)
{
    return atan2(cimagf(z), (float)z);
}

long double complex __cdecl cargl(long double complex z)
{
    return atan2(cimagl(z), (long double)z);
}

double complex      __cdecl cconj(double complex z)
{
    return (double)z - (_Imaginary double)(z);
}

float complex       __cdecl cconjf(float complex z)
{
    return (float)z - (_Imaginary float)(z);
}

long double complex __cdecl cconjl(long double complex z)
{
    return (long double)z - (_Imaginary long double)(z);
}

double complex      __cdecl cproj(double complex z)
{
    return cprojl(z);
}
float complex       __cdecl cprojf(float complex z)
{
    return cprojl(z);
}

long double complex __cdecl cprojl(long double complex z)
{
    if (isinf(creal(z)) || isinf(cimag(z)))
	return INFINITY + I * copysignl(0.0, cimag(z));
    else
	return z;
}

double      __cdecl cabs(double complex z)
{
    return cabsl(z);
}

float       __cdecl cabsf(float complex z)
{
    return cabsl(z);
}

long double __cdecl cabsl(long double complex z)
{
    long double x;
    long double y;
    long double result;

    x = fabs((long double)z);
    y = fabs(cimagl(z));
    if (x == 0)
	result = y;
    else if (y == 0)
	result = x;
    else if (x > y)
    {	result = y / x;
	result = x * sqrt(1 + result * result);
    }
    else
    {	result = x / y;
	result = y * sqrt(1 + result * result);
    }
    return result;
}


