
// Copyright (C) 2001 by Digital Mars
// All Rights Reserved
// www.digitalmars.com

// Runtime support for complex arithmetic code generation

#include <stdio.h>
#include <math.h>

#define	fpop	fstp ST(0)

/****************************
 * Multiply two complex floating point numbers, x and y.
 * Input:
 *	x.re	ST3
 *	x.im	ST2
 *	y.re	ST1
 *	y.im	ST0
 * Output:
 *	ST1	real part
 *	ST0	imaginary part
 */

__declspec(naked) void _Cmul()
{
    // p.re = x.re * y.re - x.im * y.im;
    // p.im = x.im * y.re + x.re * y.im;
    __asm
    {
	fld	ST(1)		; x.re
	fmul	ST,ST(4)	; ST0 = x.re * y.re

	fld	ST(1)		; y.im
	fmul	ST,ST(4)	; ST0 = x.im * y.im

	fsubp	ST(1),ST	; ST0 = x.re * y.re - x.im * y.im

	fld	ST(3)		; x.im
	fmul	ST,ST(3)	; ST0 = x.im * y.re

	fld	ST(5)		; x.re
	fmul	ST,ST(3)	; ST0 = x.re * y.im

	faddp	ST(1),ST	; ST0 = x.im * y.re + x.re * y.im

	fxch	ST(4),ST
	fpop
	fxch	ST(4),ST
	fpop
	fpop
	fpop

	ret
    }
#if 0
    if (isnan(x) && isnan(y))
    {
	// Recover infinities that computed as NaN+ iNaN ...
	int recalc = 0;
	if ( isinf( a) || isinf( b) )
	{   // z is infinite
	    // "Box" the infinity and change NaNs in the other factor to 0
	    a = copysignl( isinf( a) ? 1.0 : 0.0, a);
	    b = copysignl( isinf( b) ? 1.0 : 0.0, b);
	    if (isnan( c)) c = copysignl( 0.0, c);
	    if (isnan( d)) d = copysignl( 0.0, d);
	    recalc = 1;
	}
	if (isinf(c) || isinf(d))
	{   // w is infinite
	    // "Box" the infinity and change NaNs in the other factor to 0
	    c = copysignl( isinf( c) ? 1.0 : 0.0, c);
	    d = copysignl( isinf( d) ? 1.0 : 0.0, d);
	    if (isnan( a)) a = copysignl( 0.0, a);
	    if (isnan( b)) b = copysignl( 0.0, b);
	    recalc = 1;
	}
	if (!recalc && (isinf(ac) || isinf(bd) ||
	    isinf(ad) || isinf(bc)))
	{
	    // Recover infinities from overflow by changing NaNs to 0 ...
	    if (isnan( a)) a = copysignl( 0.0, a);
	    if (isnan( b)) b = copysignl( 0.0, b);
	    if (isnan( c)) c = copysignl( 0.0, c);
	    if (isnan( d)) d = copysignl( 0.0, d);
	    recalc = 1;
	}
	if (recalc)
	{
	    x = INFINITY * (a * c - b * d);
	    y = INFINITY * (a * d + b * c);
	}
    }
#endif
}

/****************************
 * Divide two complex floating point numbers, x / y.
 * Input:
 *	x.re	ST3
 *	x.im	ST2
 *	y.re	ST1
 *	y.im	ST0
 * Output:
 *	ST1	real part
 *	ST0	imaginary part
 */

_Complex long double _Cdiv()
{
    long double x_re, x_im;
    long double y_re, y_im;
    long double q_re, q_im;
    long double r;
    long double den;

    __asm
    {
	fstp	y_im
	fstp	y_re
	fstp	x_im
	fstp	x_re
    }

    if (fabs(y_re) < fabs(y_im))
    {
	r = y_re / y_im;
	den = y_im + r * y_re;
	q_re = (x_re * r + x_im) / den;
	q_im = (x_im * r - x_re) / den;
    }
    else
    {
	r = y_im / y_re;
	den = y_re + r * y_im;
	q_re = (x_re + r * x_im) / den;
	q_im = (x_im - r * x_re) / den;
    }
//printf("q.re = %g, q.im = %g\n", (double)q_re, (double)q_im);
#if 0
    if (isnan(q_re) && isnan(q_im))
    {
	long double denom = y_re * y_re + y_im * y_im;

	// non-zero / zero
	if (denom == 0.0 && (!isnan(x_re) || !isnan(x_im)))
	{
	    q_re = copysignl(INFINITY, y_re) * x_re;
	    q_im = copysignl(INFINITY, y_re) * x_im;
	}
	// infinite / finite
	else if ((isinf(x_re) || isinf(x_im)) && isfinite(y_re) && isfinite(y_im))
	{
	    x_re = copysignl(isinf(x_re) ? 1.0 : 0.0, x_re); 
	    x_im = copysignl(isinf(x_im) ? 1.0 : 0.0, x_im); 
	    q_re = INFINITY * (x_re * y_re + x_im * y_im); 
	    q_im = INFINITY * (x_im * y_re - x_re * y_im); 
	}
	// finite / infinite
	else if (isinf(logbw) && isfinite(x_re) && isfinite(x_im))
	{
	    y_re = copysignl(isinf(y_re) ? 1.0 : 0.0, y_re); 
	    y_im = copysignl(isinf(y_im) ? 1.0 : 0.0, y_im); 
	    q_re = 0.0 * (x_re * y_re + x_im * y_im); 
	    q_im = 0.0 * (x_im * y_re - x_re * y_im); 
	}
    }
#endif
    return q_re + q_im * __imaginary;
}

/****************************
 * Compare two complex floating point numbers, x and y.
 * Input:
 *	x.re	ST3
 *	x.im	ST2
 *	y.re	ST1
 *	y.im	ST0
 * Output:
 *	8087 stack is cleared
 *	flags set
 */

__declspec(naked) void _Ccmp()
{
    __asm
    {
	fucomp	ST(2)		; compare x.im and y.im
	fstsw	AX
	sahf
	jne	L1
	jp	L1		; jmp if NAN
	fucomp	ST(2)		; compare x.re and y.re
	fstsw	AX
	sahf
	fstp	ST(0)		; pop
	fstp	ST(0)		; pop
	ret

      L1:
	fstp	ST(0)		; pop
	fstp	ST(0)		; pop
	fstp	ST(0)		; pop
	ret
    }
}
