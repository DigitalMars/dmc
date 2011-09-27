/*_ ilogb.c
 * Copyright (C) 2003 by Digital Mars, www.digitalmars.com
 * All Rights Reserved
 * Written by Walter Bright
 */

#include <math.h>

#if __INLINE_8087

#define C0	0x0100
#define C1	0x0200		// sign of ST
#define C2	0x0400
#define C3	0x4000

/*
 *	C3,C2,C0
 *	00 0	unsupported
 *	00 1	nan
 *	01 0	normal
 *	01 1	infinity
 *	10 0	zero
 *	10 1	empty
 *	11 0	denormal
 */

__declspec(naked) int __ilogbx(long double x)
{
    _asm
    {
	fxam
	fstsw	AX
	and	AH,0x45
	cmp	AH,0x01
	jz	Lnan
	cmp	AH,0x40
	jz	Lzero
	cmp	AH,0x05
	jz	Linfinity

	fxtract
	push	EAX
	fstp	ST(0)
	fistp	dword ptr [ESP]
	fwait
	pop	EAX
	ret

      Lzero:
      Lnan:
	mov	EAX,80000000h	// INT_MIN
	fstp	ST(0)
	ret

      Linfinity:
	mov	EAX,7FFFFFFFh	// INT_MAX
	fstp	ST(0)
	ret
    }
}

__declspec(naked) int ilogbf(float x)
{
    _asm
    {
	fld	float ptr 4[ESP]
	jmp	__ilogbx
    }
}

__declspec(naked) int ilogb(double x)
{
    _asm
    {
	fld	double ptr 4[ESP]
	jmp	__ilogbx
    }
}

__declspec(naked) int ilogbl(long double x)
{
    _asm
    {
	fld	tbyte ptr 4[ESP]
	jmp	__ilogbx
    }
}
#endif

