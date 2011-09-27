/*
 * Cephes code Copyright 1994 by Stephen L. Moshier
 * Converted to D by Don Clugston.
 * Converted back to C by Walter Bright.
 */

/**
Macros:
 GAMMA =  &#915;
 INTEGRAL = &#8747;
*/

#include <stdio.h>
#include <math.h>
#include <float.h>

typedef long double real;

double tgamma(double x) { return (double) tgammal(x); }
float tgammaf(float x)  { return (float) tgammal(x); }

double lgamma(double x) { return (double) lgammal(x); }
float lgammaf(float x)  { return (float) lgammal(x); }


long double _polyl(long double x, int deg, long double *coeff);
#define poly(x, A) _polyl(x, sizeof(A)/sizeof((A)[0]) - 1, A)

//------------------------------------------------------------------
const real SQRT2PI = 2.50662827463100050242E0L; // sqrt(2pi)
const real MAXGAMMA = 1755.455L;	// exp(tgamma(x)) == inf if x>MAXGAMMA

// Polynomial approximations for gamma and loggamma.

static real GammaNumeratorCoeffs[] = {
   0x1p+0L,			// 1
   0x1.acf42d903366539ep-1L,	// 0.83780043015731267283
   0x1.73a991c8475f1aeap-2L,	// 0.36295154366402391688
   0x1.c7e918751d6b2a92p-4L,	// 0.1113062816019361559
   0x1.86d162cca32cfe86p-6L,	// 0.023853632434611082525
   0x1.0c378e2e6eaf7cd8p-8L,	// 0.0040926668283940355009
   0x1.dc5c66b7d05feb54p-12L,	// 0.00045429319606080091555
   0x1.616457b47e448694p-15L	// 4.2127604874716220134e-05
};

static real GammaDenominatorCoeffs[] = {
    0x1p+0L,			//  1
    0x1.a8f9faae5d8fc8b0p-2L,	//  0.41501609505884554346
   -0x1.cb7895a6756eebdep-3L,	// -0.22435109056703291645
   -0x1.7b9bab006d30652ap-5L,	// -0.046338876712445342138
    0x1.c671af78f312082ep-6L,	//  0.027737065658400729792
   -0x1.a11ebbfaf96252dcp-11L,	// -0.00079559336824947383209
   -0x1.447b4d2230a77ddap-10L,	// -0.0012377992466531522311
    0x1.ec1d45bb85e06696p-13L,	//  0.00023465840591606352443
   -0x1.d4ce24d05bd0a8e6p-17L	// -1.3971485174761704409e-05
};

static real GammaSmallCoeffs[] = {
    0x1p+0L,			//  1
    0x1.2788cfc6fb618f52p-1L,	//  0.57721566490153286082
   -0x1.4fcf4026afa2f7ecp-1L,	// -0.65587807152025406846
   -0x1.5815e8fa24d7e306p-5L,	// -0.042002635034033440541
    0x1.5512320aea2ad71ap-3L,	//  0.16653861137208052067
   -0x1.59af0fb9d82e2160p-5L,	// -0.042197733607059154702
   -0x1.3b4b61d3bfdf244ap-7L,	// -0.0096220233604062716456
    0x1.d9358e9d9d69fd34p-8L,	//  0.0072205994780369096722
   -0x1.38fc4bcbada775d6p-10L 	// -0.0011939450513815100956
};

static real GammaSmallNegCoeffs[] = {
   -0x1p+0L,			// -1
    0x1.2788cfc6fb618f54p-1L,	//  0.57721566490153286086
    0x1.4fcf4026afa2bc4cp-1L,	//  0.65587807152025365473
   -0x1.5815e8fa2468fec8p-5L,	// -0.042002635034021129105
   -0x1.5512320baedaf4b6p-3L,	// -0.16653861139444135193
   -0x1.59af0fa283baf07ep-5L,	// -0.042197733437311917216
    0x1.3b4a70de31e05942p-7L,	//  0.0096219111550359767339
    0x1.d9398be3bad13136p-8L,	//  0.0072208372618931703258
    0x1.291b73ee05bcbba2p-10L 	//  0.001133374167243894382
};

static real logGammaStirlingCoeffs[] = {
    0x1.5555555555553f98p-4L,	//  0.083333333333333314473
   -0x1.6c16c16c07509b10p-9L,	// -0.0027777777777503496034
    0x1.a01a012461cbf1e4p-11L,	//  0.00079365077958550707556
   -0x1.3813089d3f9d1640p-11L,	// -0.00059523458517656885149
    0x1.b911a92555a277b8p-11L,	//  0.00084127232973224980805
   -0x1.ed0a7b4206087b22p-10L,	// -0.0018808019381193769072
    0x1.402523859811b308p-8L 	//  0.0048850261424322707812
};

static real logGammaNumerator[] = {
   -0x1.0edd25913aaa40a2p+23L,	// -8875666.7836507038022
   -0x1.31c6ce2e58842d1ep+24L,	// -20039374.181038151756
   -0x1.f015814039477c3p+23L,	// -16255680.62543700591
   -0x1.74ffe40c4b184b34p+22L,	// -6111225.0120052143001
   -0x1.0d9c6d08f9eab55p+20L,	// -1104326.8146914642612
   -0x1.54c6b71935f1fc88p+16L,	// -87238.715228435114593
   -0x1.0e761b42932b2aaep+11L 	// -2163.6908276438128575
};

static real logGammaDenominator[] = {
   -0x1.4055572d75d08c56p+24L,	// -20993367.177578958762
   -0x1.deeb6013998e4d76p+24L,	// -31386464.076561826621
   -0x1.106f7cded5dcc79ep+24L,	// -17854332.870450781569
   -0x1.25e17184848c66d2p+22L,	// -4814940.3794118821866
   -0x1.301303b99a614a0ap+19L,	// -622744.11640662195015
   -0x1.09e76ab41ae965p+15L,	// -34035.708405343046707
   -0x1.00f95ced9e5f54eep+9L,	// -513.94814844353701437
    0x1p+0L 			// 1
};

/* ****************************************************
 * Helper function: Gamma function computed by Stirling's formula.
 *
 * Stirling's formula for the gamma function is:
 *
 * $(GAMMA)(x) = sqrt(2 &pi;) x<sup>x-0.5</sup> exp(-x) (1 + 1/x P(1/x))
 *
 */
static real gammaStirling(real x)
{
    static real SmallStirlingCoeffs[] = {
        0x1.55555555555543aap-4,   //  0.083333333333333318004
        0x1.c71c71c720dd8792p-9,   //  0.0034722222222300753277
       -0x1.5f7268f0b5907438p-9,   // -0.0026813271618763044182
       -0x1.e13cd410e0477de6p-13,  // -0.00022947197478731854057
        0x1.9b0f31643442616ep-11,  //  0.00078403348427447530038
        0x1.2527623a3472ae08p-14,  //  6.9893322606231931717e-05
       -0x1.37f6bc8ef8b374dep-11,  // -0.00059502375540563301557
       -0x1.8c968886052b872ap-16,  // -2.3638488095017590616e-05
        0x1.76baa9c6d3eeddbcp-11   //  0.0007147391378143610789
    };

    static real LargeStirlingCoeffs[] = {
	 1.0L,
	 8.33333333333333333333E-2L,
	 3.47222222222222222222E-3L,
	-2.68132716049382716049E-3L,
	-2.29472093621399176955E-4L,
	 7.84039221720066627474E-4L,
	 6.97281375836585777429E-5L
    };

    real w = 1.0L / x;
    real y = expl(x);
    if (x > 1024.0L)
    {
	// For large x, use rational coefficients from the analytical expansion.
	w = poly(w, LargeStirlingCoeffs);
	// Avoid overflow in pow()
	real v = powl( x, 0.5L * x - 0.25L );
	y = v * (v / y);
    }
    else
    {
	w = 1.0L + w * poly(w, SmallStirlingCoeffs);
	y = powl( x, x - 0.5L ) / y;
    }
    y = SQRT2PI * y * w;
    return  y;
}

/*****************************************************
 *  The Gamma function, $(GAMMA)(x)
 *
 *  Generalizes the factorial function to real and complex numbers.
 *  Like x!, $(GAMMA)(x+1) = x*$(GAMMA)(x).
 *
 *  Mathematically, if z.re > 0 then
 *   $(GAMMA)(z) =<big>$(INTEGRAL)<sub>0</sub><sup>&infin</sup></big>t<sup>z-1</sup>e<sup>-t</sup>dt
 *
 *  This function is equivalent to tgamma() in the C programming language.
 *
 *	<table border=1 cellpadding=4 cellspacing=0>
 *	<caption>Special Values</caption>
 *	<tr> <th> x               <th> $(GAMMA)(x)   <th>invalid?
 *	<tr> <td> NAN             <td> NAN           <td> yes
 *	<tr> <td> &plusmn;0.0     <td> &plusmn;&infin;      <td> yes
 *	<tr> <td> integer > 0     <td> (x-1)!        <td> no
 *	<tr> <td> integer < 0     <td> NAN           <td> yes
 *	<tr> <td> +&infin;        <td> +&infin;      <td> no
 *	<tr> <td> -&infin;        <td> NAN           <td> yes
 *	</table>
 *
 *  References:
 *     cephes, http://en.wikipedia.org/wiki/Gamma_function
 *
 */
real tgammal(real x)
{
    /* Author: Don Clugston. Based on code from the CEPHES library.
     *
     * Arguments |x| <= 13 are reduced by recurrence and the function
     * approximated by a rational function of degree 7/8 in the
     * interval (2,3).  Large arguments are handled by Stirling's
     * formula. Large negative arguments are made positive using
     * a reflection formula. 
     */ 

    real q, z;

    if (isnan(x))
	return x;
    if (x == -INFINITY)
	return NAN;
    if ( fabsl(x) > MAXGAMMA )
	return INFINITY;
    if (x == 0)
	return 1.0 / x; // +- infinity depending on sign of x, create an exception.
    
    q = fabsl(x);
	    
    if ( q > 13.0L )
    {
	// Large arguments are handled by Stirling's
	// formula. Large negative arguments are made positive using
	// the reflection formula.  

	if ( x < 0.0L )
	{
	    int sgngam = 1;	// sign of gamma.
	    real p  = floorl(q);
	    if ( p == q )
		return NAN; // poles for all integers <0.
	    int intpart = (int)(p);
	    if ( (intpart & 1) == 0 )
		    sgngam = -1;
	    z = q - p;
	    if ( z > 0.5L )
	    {
		p += 1.0L;
		z = q - p;
	    }
	    z = q * sinl( M_PI * z );
	    z = fabsl(z) * gammaStirling(q);
	    if ( z <= M_PI / LDBL_MAX )
		return sgngam * INFINITY;
	    return sgngam * M_PI / z;
	}
	else
	    return gammaStirling(x);
    }
    
    // Arguments |x| <= 13 are reduced by recurrence and the function
    // approximated by a rational function of degree 7/8 in the
    // interval (2,3).

    z = 1.0L;
    while ( x >= 3.0L )
    {
	x -= 1.0L;
	z *= x;
    }
    
    while ( x < -0.03125L )
    {
	z /= x;
	x += 1.0L;
    }
    
    if ( x <= 0.03125L )
    {
	if ( x == 0.0L )
	    return NAN;
	else if ( x < 0.0L )
	{
	    x = -x;
	    return z / (x * poly( x, GammaSmallNegCoeffs ));
	}
	else
	{
	    return z / (x * poly( x, GammaSmallCoeffs ));
	}
    }
    
    while ( x < 2.0L )
    {
	z /= x;
	x += 1.0L;
    }
    if ( x == 2.0L )
	return z;
    
    x -= 2.0L;
    return z * poly( x, GammaNumeratorCoeffs ) / poly( x, GammaDenominatorCoeffs );
}

#if 0
unittest
{
    // gamma(n) = factorial(n-1) if n is an integer.
    double fact = 1.0L;
    for (int i = 1; fact < LDBL_MAX; ++i)
    {
	// Require exact equality for small factorials
	if (i < 14)
	    assert(tgammal(i * 1.0L)==fact);
	assert(feqrel(tgammal(i * 1.0L), fact) > LDBL_MANT_DIG - 15);
	//writefln(i, " %a ---> %a   %a ", i*1.0L, tgammal(i * 1.0L), fact, feqrel(tgammal(i*1.0L), fact));
	fact *= (i * 1.0L);
    }
    assert(tgammal(0.0) == INFINITY);
    assert(tgammal(-0.0) == -INFINITY);
    assert(isnan(tgammal(-1.0)));
    assert(isnan(tgammal(NAN)));
    assert(tgammal(INFINITY) == INFINITY);
    assert(isnan(tgammal(-INFINITY)));
    assert(tgammal(LDBL_MIN * LDBL_EPSILON) == INFINITY);

    assert(feqrel(tgammal(0.5), sqrtl(M_PI)) > LDBL_MANT_DIG - 3);
}
#endif

/*****************************************************
 * Natural logarithm of gamma function.
 *
 * Returns the base e (2.718...) logarithm of the absolute
 * value of the gamma function of the argument.
 *
 * For reals, lgamma is equivalent to log(fabs(tgamma(x)).
 *
 *	<table border=1 cellpadding=4 cellspacing=0>
 *	<caption>Special Values</caption>
 *	<tr> <th> x               <th> log$(GAMMA)(x) <th>invalid?
 *	<tr> <td> NaN             <td> NaN           <td> yes
 *	<tr> <td> integer <= 0    <td> +&infin;      <td> yes
 *	<tr> <td> 1, 2            <td> +0.0          <td> no
 *	<tr> <td> &plusmn;&infin;  <td> +&infin;      <td> no
 *	</table>
 * 
 */
real lgammal(real x)
{
    /* Author: Don Clugston. Based on code from the CEPHES library.
     *
     * For arguments greater than 33, the logarithm of the gamma
     * function is approximated by the logarithmic version of
     * Stirling's formula using a polynomial approximation of
     * degree 4. Arguments between -33 and +33 are reduced by
     * recurrence to the interval [2,3] of a rational approximation.
     * The cosecant reflection formula is employed for arguments
     * less than -33.
     */
    real p, q, w, z, f, nx;
    
    if (isnan(x))
	return x;
    if (fabsl(x) == INFINITY)
	return INFINITY;
    
    if ( x < -34.0L )
    {
	q = -x;
	w = lgammal(q);
	real p = floorl(q);
	if ( p == q )
	    return INFINITY;
	int intpart = (int)(p);
	real sgngam = 1;
	if ( (intpart & 1) == 0 )
	    sgngam = -1;
	z = q - p;
	if ( z > 0.5L )
	{
	    p += 1.0L;
	    z = p - q;
	}
	z = q * sinl( M_PI * z );
	if ( z == 0.0L )
	    return sgngam * INFINITY;
	/* z = LOGPI - logl( z ) - w; */
	z = logl( M_PI / z ) - w;
	return z;
    }

    if ( x < 13.0L )
    {
	z = 1.0L;
	nx = floorl( x +  0.5L );
	f = x - nx;
	while ( x >= 3.0L )
	{
	    nx -= 1.0L;
	    x = nx + f;
	    z *= x;
	}
	while ( x < 2.0L )
	{
	    if( fabsl(x) <= 0.03125 )
	    {
		if ( x == 0.0L )
		    return INFINITY;
		if ( x < 0.0L )
		{
		    x = -x;
		    q = z / (x * poly( x, GammaSmallNegCoeffs));
		}
		else
		    q = z / (x * poly( x, GammaSmallCoeffs));
		return logl( fabsl(q) );
	    }			
	    z /= nx +  f;
	    nx += 1.0L;
	    x = nx + f;
	}
	z = fabsl(z);
	if ( x == 2.0L )
	    return logl(z);
	x = (nx - 2.0L) + f;
	p = x * poly( x, logGammaNumerator ) / poly( x, logGammaDenominator);
	return ( logl(z) + p );
    }
    
    //const real MAXLGM = 1.04848146839019521116e+4928L;
    //if ( x > MAXLGM ) return sgngaml * INFINITY;

    /* log( sqrt( 2*pi ) ) */
    const real LOGSQRT2PI  =  0.91893853320467274178L;
    
    q = ( x - 0.5L ) * logl(x) - x + LOGSQRT2PI;
    if (x > 1.0e10L)
	return q;
    p = 1.0L/(x*x);
    q += poly( p, logGammaStirlingCoeffs ) / x;
    return q ;
}

#if 0
unittest
{
    assert(isnan(lgammal(NAN)));
    assert(lgammal(INFINITY) == INFINITY);
    assert(lgammal(-1.0) == INFINITY);
    assert(lgammal(0.0) == INFINITY);
    assert(isPosZero(lgammal(1.0L)));
    assert(isPosZero(lgammal(2.0L)));
 
    // x, correct loggamma(x), correct d/dx loggamma(x).
    static real[] testpoints =
    { 
	 8.0L,                    8.525146484375L      + 1.48766904143001655310E-5,   2.01564147795560999654E0L,
	 8.99993896484375e-1L,    6.6375732421875e-2L  + 5.11505711292524166220E-6L, -7.54938684259372234258E-1,
	 7.31597900390625e-1L,    2.2369384765625e-1   + 5.21506341809849792422E-6L, -1.13355566660398608343E0L,
	 2.31639862060546875e-1L, 1.3686676025390625L  + 1.12609441752996145670E-5L, -4.56670961813812679012E0,
	 1.73162841796875L,      -8.88214111328125e-2L + 3.36207740803753034508E-6L, 2.33339034686200586920E-1L,
	 1.23162841796875L,      -9.3902587890625e-2L  + 1.28765089229009648104E-5L, -2.49677345775751390414E-1L,
	 7.3786976294838206464e19L,   3.301798506038663053312e21L - 1.656137564136932662487046269677E5L,
                          4.57477139169563904215E1L,
	 1.08420217248550443401E-19L, 4.36682586669921875e1L + 1.37082843669932230418E-5L,
                         -9.22337203685477580858E18L,
//	 1.0L, 0.0L, -5.77215664901532860607E-1L,
//	 2.0L, 0.0L, 4.22784335098467139393E-1L,
	-0.5L,  1.2655029296875L    + 9.19379714539648894580E-6L, 3.64899739785765205590E-2L,
	-1.5L,  8.6004638671875e-1L + 6.28657731014510932682E-7L, 7.03156640645243187226E-1L,
	-2.5L, -5.6243896484375E-2L + 1.79986700949327405470E-7,  1.10315664064524318723E0L,
	-3.5L,  -1.30902099609375L  + 1.43111007079536392848E-5L, 1.38887092635952890151E0L
    };

    // TODO: test derivatives as well.

    for (int i=0; i<testpoints.length; i+=3)
    {
	//writefln("%a  %a  ", lgammal(testpoints[i]), testpoints[i+1], feqrel(lgammal(testpoints[i]), testpoints[i+1]));
	 assert( feqrel(lgammal(testpoints[i]), testpoints[i+1]) > LDBL_MANT_DIG - 5);
    }

    static real logabsgamma(real x)
    {
	// For poles, tgamma(x) returns nan, but lgamma() returns infinity.
	if (x < 0 && x == floorl(x))
	    return INFINITY;
	return log(fabsl(tgammal(x)));
    }

    static real exploggamma(real x)
    {
	return exp(lgammal(x));
    }

    static real absgamma(real x)
    {
	if (x < 0 && x == floorl(x))
	    return INFINITY;
	return fabsl(tgammal(x));
    }

    // Check that loggamma(x) = log(gamma(x)) provided x is not -1, -2, -3, ...
    assert(consistencyTwoFuncs(&lgammal, &logabsgamma, -1000, 1700) > LDBL_MANT_DIG - 7);
    assert(consistencyTwoFuncs(&exploggamma, &absgamma, -2000, INFINITY) > LDBL_MANT_DIG - 16);
}
#endif
  
