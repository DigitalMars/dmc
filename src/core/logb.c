/*_ logb.c
 * Copyright (C) 2003 by Digital Mars, www.digitalmars.com
 * All Rights Reserved
 * Written by Walter Bright
 */

#include <math.h>

#if __INLINE_8087

__declspec(naked) float logbf(float x)
{
    _asm
    {
	fld	float ptr 4[ESP]
	fxtract
	fstp	ST(0)
	ret
    }
}

__declspec(naked) double logb(double x)
{
    _asm
    {
	fld	double ptr 4[ESP]
	fxtract
	fstp	ST(0)
	ret
    }
}

__declspec(naked) long double logbl(long double x)
{
    _asm
    {
	fld	tbyte ptr 4[ESP]
	fxtract
	fstp	ST(0)
	ret
    }
}
#endif

