//_ strtoull.c
// Copyright (C) 1994 by Digital Mars
// All Rights Reserved
// Written by Walter Bright

#include	<errno.h>
#include	<string.h>

/******************************
 * Same as strtoul(), but for unsigned long long's.
 */

unsigned long long strtoull(const char *p,char **pend,int base)
{
    _asm
    {
	mov	ESI,p[EBP]	;get p
A1:	lodsb
	cmp	AL,' '
	je	A1
	cmp	AL,9
	jb	A5
	cmp	AL,0Dh
	jbe	A1		;skip white space

A3:	cmp	AL,'+'
	jne	A5
	lodsb

A5:	mov	EBX,base
	test	EBX,EBX
	jz	BASE0
	cmp	AL,'0'
	jne	A6
	lodsb
	cmp	EBX,2
	je	BASE2
	cmp	EBX,16
	je	BASE16
	jmp	A6


BASE2:	;Skip over '0b' and '0B'
	cmp	AL,'b'
	je	B16a
	cmp	AL,'B'
	je	B16a
	jmp	A6

BASE16:	;Skip over '0x' and '0X'
	cmp	AL,'x'
	je	B16a
	cmp	AL,'X'
	jne	A6
B16a:	lodsb
	jmp	A6

BASE0:	;Determine which base to use
	mov	base[EBP],10		;default to base 10
	cmp	AL,'0'
	jne	A6			;it is decimal
	lodsb
	mov	base[EBP],8		;could be octal
	cmp	AL,'b'
	je	BIN
	cmp	AL,'B'
	je	BIN
	cmp	AL,'x'
	je	HEX
	cmp	AL,'X'
	je	HEX
	jmp	A6

HEX:	mov	base[EBP],16+6
BIN:	sub	base[EBP],6	;convert to base 2 or base 16
	lodsb

A6:	xor	EDI,EDI		;EDI accumulates error indication
	mov	ECX,EDI
	mov	EDX,ECX		;EDX,ECX = 0: accumulate result in EDX,ECX
	jmp	A8

ALPHA:	and	AL,0DFh		;to upper case
	sub	AL,'A'-10	;to 10..35
	jmp	A9

	even
A4:	lodsb
A8:	cbw			;AH = 0 if AL >= 0
	cwde
	test	AL,AL
	jle	A2
	cmp	AL,'A'
	jae	ALPHA
	sub	AL,'0'		;to binary

A9:	cmp	EAX,base
	jae	A2

	;EDX,ECX = EDX,ECX * base + AL
	mov	EBX,EAX
	mov	EAX,EDX
	mul	base[EBP]		;EAX = EDX * base
	adc	EDI,0
	xchg	EAX,ECX
	mul	base[EBP]		;EDX,EAX = ECX * base
	add	EDX,ECX
	adc	EDI,0

	mov	ECX,EBX
	add	ECX,EAX
	adc	EDX,0
	adc	EDI,0

	jmp	A4
    }

overflow:
	// Do this in C to handle errno macro
	errno = ERANGE;
    _asm
    {
	mov	EAX,-1
	cdq				;return ULLONG_MAX
	jmp	A7

A2:	mov	EAX,ECX
	test	EDI,EDI			;any overflow?
	jne	overflow		;yes

A7:	mov	EBX,pend[EBP]
	dec	ESI			;back up to point to last char read
	test	EBX,EBX
	jz	A11
	mov	[EBX],ESI		;*pend = ESI
    }
A11: ;
}
