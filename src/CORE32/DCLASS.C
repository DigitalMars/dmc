//_ dclass.c
// $Header: z:/core/crtl/src/core32/dclass.c,v 1.1.1.1 1997/01/02 19:17:46 smarx Exp $
// Copyright (C) 1994 by Digital Mars
// All Rights Reserved
// Written by Walter Bright

#include	<fltpnt.h>

#define longexp		0x7FF00000	// mask for long exponent
#define longhid 	0x00100000	// mask for hidden bit
#define mantisa_mask	0x000FFFFF	// mask out Mantisa
#define longbias	0x000003FF	// exponent bias
#define long_qnan	0x7FF80000	// quiet NaN
#define dqnan_bit	0x00080000	// Quite NaN bit for double
#define long_infinity	0x7FF00000

/************************
 * classify double float
 * Input:
 *	[P] = a
 * Returns:
 *	FP_xxxxxx
 */

__declspec(naked) unsigned __cdecl __fpclassify_d(double d)
{
    _asm
    {
	mov	EDX,4+4[ESP]
	mov	EAX,EDX
	and	EAX,longexp		;mask exponent
	je	d1			;exponent is zero

	cmp	EAX,long_infinity	;test for NaN or infinite
	je	d2
	mov	EAX,FP_NORMAL		;set to default
	ret

d2:	mov	EAX,FP_NANQ		;assumes quiet NaN
	test	EDX,dqnan_bit		;bit must be set to be
	jnz	d3			;a quiet NaN

	mov	EAX,FP_INFINITE		;assume Infinity
	and	EDX,mantisa_mask	;clear sign and exponent
	or	EDX,4[ESP]		;all other bit must be zero to be inifite
	jz	d3			;otherwise

	mov	EAX,FP_NANS		;a is a signaling NaN
	ret


d1:
	mov	EAX,FP_ZERO		;assume Zero
	and	EDX,mantisa_mask	;drop sign and exponent
	or	EDX,4[ESP]		;are any other bits set
	jz	d3
					;if not then it must be subnormal
	mov	EAX,FP_SUBNORMAL
d3:	ret
    }
}
