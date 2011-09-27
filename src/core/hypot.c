/*
Copyright (c) 2001-2003 by Digital Mars
www.digitalmars.com
This is based on code from:
Cephes Math Library Release 2.1:  January, 1989
Copyright 1984, 1987, 1989 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

#include <math.h>
#include <float.h>

#define PRECD 27
#define MAXEXPD 1024
#define MINEXPD -1024

#define PRECF 12
#define MAXEXPF 128
#define MINEXPF -128

#define PRECL 32
#define MAXEXPL 16384
#define MINEXPL -16384

double __matherr(int type, double retval, double arg1, double arg2,const char * name);

#if _WIN32

float hypotf(float zre, float zim)
{
    return hypotl(zre, zim);
}

double hypot(double zre, double zim)
{
    return hypotl(zre, zim);
}

long double hypotl(long double zre, long double zim)
{
    long double x, y, b, re, im;
    int ex, ey, e;

    // Note, hypot(INFINITY,NAN) = INFINITY.
    if (isinf(zre) || isinf(zim))
	return INFINITY;

    if (isnan(zre))
	return zre;
    if (isnan(zim))
	return zim;

    re = fabsl(zre);
    im = fabsl(zim);

    if (re == 0.0)
	return im;
    if (im == 0.0)
	return re;

    // Get the exponents of the numbers
    x = frexpl(re, &ex);
    y = frexpl(im, &ey);

    // Check if one number is tiny compared to the other
    e = ex - ey;
    if (e > PRECL)
	return re;
    if (e < -PRECL)
	return im;

    // Find approximate exponent e of the geometric mean.
    e = (ex + ey) >> 1;

    // Rescale so mean is about 1
    x = ldexpl(re, -e);
    y = ldexpl(im, -e);

    // Hypotenuse of the right triangle
    b = sqrtl(x * x  +  y * y);

    // Compute the exponent of the answer.
    y = frexpl(b, &ey);
    ey = e + ey;

    // Check it for overflow and underflow.
    if (ey > MAXEXPL)
    {
	return __matherr(_OVERFLOW, INFINITY, x, y, "hypotl");
    }
    if (ey < MINEXPL)
	return 0.0;

    // Undo the scaling
    b = ldexpl(b, e);
    return b;
}

#else

float hypotf(float zre, float zim)
{
    return hypot(zre, zim);
}

long double hypotl(long double zre, long double zim)
{
    return hypot(zre, zim);
}

double hypot(double zre, double zim)
{
    double x, y, b, re, im;
    int ex, ey, e;

    // Note, hypot(INFINITY,NAN) = INFINITY.
    if (isinf(zre) || isinf(zim))
	return INFINITY;

    if (isnan(zre))
	return zre;
    if (isnan(zim))
	return zim;

    re = fabs(zre);
    im = fabs(zim);

    if (re == 0.0)
	return im;
    if (im == 0.0)
	return re;

    // Get the exponents of the numbers
    x = frexp(re, &ex);
    y = frexp(im, &ey);

    // Check if one number is tiny compared to the other
    e = ex - ey;
    if (e > PRECD)
	return re;
    if (e < -PRECD)
	return im;

    // Find approximate exponent e of the geometric mean.
    e = (ex + ey) >> 1;

    // Rescale so mean is about 1
    x = ldexp(re, -e);
    y = ldexp(im, -e);

    // Hypotenuse of the right triangle
    b = sqrt(x * x  +  y * y);

    // Compute the exponent of the answer.
    y = frexp(b, &ey);
    ey = e + ey;

    // Check it for overflow and underflow.
    if (ey > MAXEXPD)
    {
	return __matherr(_OVERFLOW, INFINITY, x, y, "hypot");
    }
    if (ey < MINEXPD)
	return 0.0;

    // Undo the scaling
    b = ldexp(b, e);
    return b;
}

#endif
