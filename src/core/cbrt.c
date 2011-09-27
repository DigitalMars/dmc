/*_ cbrt.c
 * Copyright (C) 2003 by Digital Mars, www.digitalmars.com
 * All Rights Reserved
 * Written by Walter Bright
 */

#include <math.h>

#if __INLINE_8087

#include "8087.h"

long double _exp2_87();

__declspec(naked) long double __cbrtx()
{
    static long double onethird = 1.0L / 3.0L;

    _asm
    {
	fxam
	fstsw	AX
    }
    switch (_EAX & (C3 | C2 | C1 | C0))
    {
	case _CCC_NORMAL | _CCC_NEGATIVE:
	case _CCC_DENORMAL | _CCC_NEGATIVE:
	    _asm
	    {
		fchs		// make x positive
		mov	DH,1	// flag _exp2_87 that result should be negative
		jmp	Lcbrt
	    }
	case _CCC_NORMAL:
	case _CCC_DENORMAL:
	    _asm
	    {
		mov	DH,0
	    Lcbrt:
		push	EBP
		mov	EBP,ESP
		fld	tbyte ptr onethird
		fxch	ST(1)
		fyl2x		// ST = 1/3 * log2(x)
		jmp	_exp2_87
	    }
	default:
	    _asm
	    {
		ret
	    }
    }
}

__declspec(naked) float cbrtf(float x)
{
    _asm
    {
	fld	float ptr 4[ESP]
	jmp	__cbrtx
    }
}

__declspec(naked) double cbrt(double x)
{
    _asm
    {
	fld	double ptr 4[ESP]
	jmp	__cbrtx
    }
}

__declspec(naked) long double cbrtl(long double x)
{
    _asm
    {
	fld	tbyte ptr 4[ESP]
	jmp	__cbrtx
    }
}

/*********************************************/

__declspec(naked) long double __exp2x()
{
    _asm
    {
	fxam
	fstsw	AX
    }
    switch (_EAX & (C3 | C2 | C1 | C0))
    {
	case _CCC_NORMAL:
	case _CCC_DENORMAL:
	case _CCC_NORMAL | _CCC_NEGATIVE:
	case _CCC_DENORMAL | _CCC_NEGATIVE:
	    _asm
	    {
		push	EBP
		mov	EBP,ESP
		xor	DH,DH
		jmp	_exp2_87
	    }

	case _CCC_INFINITY | _CCC_NEGATIVE:
	    _asm
	    {
		fstp	ST(0)
		fldz
		ret
	    }

	case _CCC_ZERO:
	case _CCC_ZERO | _CCC_NEGATIVE:
	    _asm
	    {
		fstp	ST(0)
		fld1
		ret
	    }

	default:
	    _asm
	    {
		ret
	    }
    }
}

__declspec(naked) float exp2f(float x)
{
    _asm
    {
	fld	float ptr 4[ESP]
	jmp	__exp2x
    }
}

__declspec(naked) double exp2(double x)
{
    _asm
    {
	fld	double ptr 4[ESP]
	jmp	__exp2x
    }
}

__declspec(naked) long double exp2l(long double x)
{
    _asm
    {
	fld	tbyte ptr 4[ESP]
	jmp	__exp2x
    }
}
#endif

