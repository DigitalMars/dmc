
/* Based on code from:
 * Cephes Math Library Release 2.9:  June, 2000
 * Copyright 2000 by Stephen L. Moshier
 */

#include	<stdio.h>
#include	<math.h>
#include	<fenv.h>
#include	<assert.h>

#include	"8087.h"
#include	"math87.h"

#if _MT
extern void _pascal __FEEXCEPT(int e);
#define feexcept(e)	(__FEEXCEPT(e))
#else
extern fenv_t _fe_cur_env;
#define feexcept(e)	(_fe_cur_env.status |= (e))
#endif

double __matherr(int type, double retval, double arg1, double arg2,const char * name);

double erf(double x) { return (double) erfl(x); }
float erff(float x) { return (float) erfl(x); }

double erfc(double x) { return (double) erfcl(x); }
float erfcf(float x) { return (float) erfcl(x); }


static long double expx2l(long double x, int sign);

#define MAXLOGL	(M_LN2_L * (1 << (15 - 1)))

static long double P[10] =
{
    -0x1.30dfa809b3cc6676p-17L,
    0x1.38637cd0913c0288p+18L,
    0x1.2f015e047b4476bp+22L,
    0x1.24726f46aa9ab08p+25L,
    0x1.64b13c6395dc9c26p+27L,
    0x1.294c93046ad55b5p+29L,
    0x1.5962a82f92576dap+30L,
    0x1.11a709299faba04ap+31L,
    0x1.11028065b087be46p+31L,
    0x1.0d8ef40735b097ep+30L,
};

static long double Q[11] =
{
    0x1.14d8e2a72dec49f4p+19L,
    0x1.0c880ff467626e1p+23L,
    0x1.04417ef060b58996p+26L,
    0x1.404e61ba86df4ebap+28L,
    0x1.0f81887bc82b873ap+30L,
    0x1.4552a5e39fb49322p+31L,
    0x1.11779a0ceb2a01cep+32L,
    0x1.3544dd691b5b1d5cp+32L,
    0x1.a91781f12251f02ep+31L,
    0x1.0d8ef3da605a1c86p+30L,
    0x1p+0L,
};

static long double R[5] =
{
    0x1.b9f6d8b78e22459ep-6L,
    0x1.1b84686b0a4ea43ap-1L,
    0x1.b8f6aebe96000c2ap+1L,
    0x1.cb1dbedac27c8ec2p+2L,
    0x1.cf885f8f572a4c14p+1L,
};

static long double S[6] =
{
    0x1.87ae3cae5f65eb5ep-5L,
    0x1.01616f266f306d08p+0L,
    0x1.a4abe0411eed6c22p+2L,
    0x1.eac9ce3da600abaap+3L,
    0x1.5752a9ac2faebbccp+3L,
    0x1p+0L,
};

static long double T[7] =
{
    0x1.0da01654d757888cp+20L,
    0x1.2eb7497bc8b4f4acp+17L,
    0x1.79078c19530f72a8p+15L,
    0x1.4eaf2126c0b2c23p+11L,
    0x1.1f2ea81c9d272a2ep+8L,
    0x1.59ca6e2d866e625p+2L,
    0x1.c188e0b67435faf4p-4L,
};

static long double U[7] =
{
    0x1.dde6025c395ae34ep+19L,
    0x1.c4bc8b6235df35aap+18L,
    0x1.8465900e88b6903ap+16L,
    0x1.855877093959ffdp+13L,
    0x1.e5c44395625ee358p+9L,
    0x1.6a0fed103f1c68a6p+5L,
    0x1p+0L,
};

long double erfcl(long double a)
{
    long double p,q,x,y,z;

    _asm
    {
	fld	tbyte ptr a
	fxam
	fstsw	AX
	fstp	ST(0)
    }
    switch (_EAX & 0x4700)
    {
	case _CCC_NORMAL:
	case _CCC_DENORMAL:
	case _CCC_ZERO:
	case _CCC_ZERO | _CCC_NEGATIVE:
	    x = a;
	    break;

	case _CCC_NORMAL | _CCC_NEGATIVE:
	case _CCC_DENORMAL | _CCC_NEGATIVE:
	    x = -a;
	    break;

	case _CCC_INFINITY:
	    return 0;

	case _CCC_INFINITY | _CCC_NEGATIVE:
	    return 2.0;

	case _CCC_NAN:
	    if (isqnanl(x))
		return x;
	    return __matherr(DOMAIN,x,x,0.0,"erfc");

	default:
	    return __nan;
    }

    if (x < 1)
	return 1 - erfl(a);

    z = -a * a;

    if (z < -MAXLOGL)
    {
under:
	return __matherr(UNDERFLOW,(a < 0 ? 2.0L : 0.0L),x,0.0,"erfc");
    }

    /* Compute z = expl(z).  */
    z = expx2l(a, -1);
    y = 1 / x;

    if (x < 8.0L)
    {
	p = _polyl(y, 9, P);
	q = _polyl(y, 10, Q);
    }
    else
    {
	q = y * y;
	p = y * _polyl(q, 4, R);
	q = _polyl(q, 5, S);
    }
    y = (z * p)/q;

    if (a < 0)
	y = 2 - y;

    if (y == 0)
	goto under;

    return y;
}


long double erfl(long double x)
{
    long double y, z;

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
	    return x;

	case _CCC_INFINITY:
	    if (signbit(x))
		return -1.0;
	    else
		return 1.0;

	case _CCC_NAN:
	    if (isqnanl(x))
		return x;
	    return __matherr(DOMAIN,x,x,0.0,"erf");

	default:
	    return __nan;
    }

    if (fabsl(x) > 1)
	return 1.0L - erfcl(x);

    z = x * x;
    y = x * _polyl(z, 6, T) / _polyl(z, 6, U);
    return y;
}


/**************************************************************
 * Computes y = exp(x*x) while suppressing error amplification
 * that would ordinarily arise from the inexactness of the
 * exponential argument x*x.
 *
 * If sign < 0, the result is inverted; i.e., y = exp(-x*x) .
 */

static long double expx2l(long double x, int sign)
{
    long double u, u1, m, f;

    x = fabsl(x);
    if (sign < 0)
	x = -x;

    /* Represent x as an exact multiple of M plus a residual.
     * M is a power of 2 chosen so that exp(m * m) does not overflow
     * or underflow and so that |x - m| is small.
     */
    #define M 32768.0
    #define MINV 3.0517578125e-5
    m = MINV * floorl(M * x + 0.5L);
    f = x - m;

    /* x^2 = m^2 + 2mf + f^2 */
    u = m * m;
    u1 = 2 * m * f  +  f * f;

    if (sign < 0)
    {
	u = -u;
	u1 = -u1;
    }

    if ((u+u1) > MAXLOGL)
	return __inf;

    /* u is exact, u1 is small.  */
    u = expl(u) * expl(u1);
    return u;
}

#if 0
void main()
{
    printf("\nP\n");
    for (int i = 10; i-- > 0;)
	printf("%LaL,\n", ((long double *)&P)[i]);
    printf("\n");
    for (int i = 0; i < 10; i++)
    {	printf("%LaL,\n", ((long double *)&Px)[i]);
	assert(((long double *)&P)[9 - i] == ((long double *)&Px)[i]);
    }
    printf("\nQ\n");
    for (int i = 11; i-- > 0;)
	printf("%LaL,\n", ((long double *)&Q)[i]);
    printf("\n");
    for (int i = 0; i < 11; i++)
    {	printf("%LaL,\n", ((long double *)&Qx)[i]);
	assert(((long double *)&Q)[10 - i] == ((long double *)&Qx)[i]);
    }
    printf("\n");
}
#endif



