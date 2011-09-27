
#include <math.h>
#include <float.h>

float acoshf(float x)
{
    return (float)acoshl(x);
}

double acosh(double x)
{
    return (double)acoshl(x);
}

long double acoshl(long double x)
{
    if (x > 1.0L / LDBL_EPSILON)
	return M_LN2_L + logl(x);
    else
	return logl(x + sqrtl(x * x - 1));
}

float asinhf(float x)
{
    return (float)asinhl(x);
}

double asinh(double x)
{
    return (double)asinhl(x);
}

long double asinhl(long double x)
{
    if (fabsl(x) > 1.0L / LDBL_EPSILON)
	return copysignl(M_LN2_L + logl(fabsl(x)), x);
    else
	return copysignl(log1pl(fabsl(x) + x * x / (1 + sqrtl(x * x + 1)) ), x);
}

float atanhf(float x)
{
    return (float)atanhl(x);
}

double atanh(double x)
{
    return (double)atanhl(x);
}

long double atanhl(long double x)
{
    return  0.5L * log1pl( 2 * x / (1 - x) );
}

/*
ldexpf
scalbn
scalbln
lrint
lround
llrint
llround
nextafterl
nexttoward
fdim
fmax
fmin
fma
*/
