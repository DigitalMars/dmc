;_ strtoul.asm   Modified by: Walter Bright */
; Copyright (C) 1991 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

	include	macros.asm
	include stdio.asm

extrn __SET_ERRNO:near

	begcode	strtoul

;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert ascii string to long.

    if SPTR
pend	equ	dword ptr P+SIZEPTR[EBP]
    else
pend	equ	fword ptr P+SIZEPTR[EBP]
    endif
base	equ	dword ptr P+SIZEPTR+SIZEPTR[EBP]

	c_public	strtoul

func	strtoul
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ESI,EDI>
    if SPTR
	mov	ESI,P[EBP]	;get p
    else
	push	DS
	lds	ESI,P[EBP]	;get p
    endif
A1:	lodsb
	_if	AL e ' ', A1
	_if	AL b 9, A5
	_if	AL be 0Dh, A1	;skip white space

A3:	_if	AL ne '+', A5
	lodsb

A5:	mov	EBX,base
	tst	EBX
	jz	BASE0
	_if	AL ne '0', A6
	lodsb
	_if	EBX e 2, BASE2
	_if	EBX e 16, BASE16
	jmp	A6


BASE2:	;Skip over '0b' and '0B'
	_if	AL e 'b', B16a
	_if	AL e 'B', B16a
	jmp	A6

BASE16:	;Skip over '0x' and '0X'
	_if	AL e 'x', B16a
	_if	AL ne 'X', A6
B16a:	lodsb
	jmp	A6

BASE0:	;Determine which base to use
	mov	base,10			;default to base 10
	_if	AL ne '0', A6		;it is decimal
	lodsb
	mov	base,8			;could be octal
	_if	AL e 'b', BIN
	_if	AL e 'B', BIN
	_if	AL e 'x', HEX
	_if	AL e 'X', HEX
	jmp	A6

HEX:	mov	base,16+6
BIN:	sub	base,6		;convert to base 2 or base 16
	lodsb

A6:	clr	EDI		;EDI accumulates error indication
	mov	ECX,EDI		;ECX = 0: accumulate result in ECX
	jmp	A8

ALPHA:	and	AL,0DFh		;to upper case
	sub	AL,'A'-10	;to 10..35
	jmp	A9

	even
A4:	lodsb
A8:	movsx	EAX,AL		;AH = 0 if AL >= 0
	tst	AL
	jle	A2
	_if	AL ae 'A', ALPHA
	sub	AL,'0'		;to binary

A9:	_if	EAX ae base, A2

	;ECX = ECX * base + AL
	xchg	EAX,ECX
	mul	base
	adc	EDI,0		;accumulate error
	add	ECX,EAX
	adc	EDI,0		;accumulate error

	jmp	A4

overflow:
	push    dword ptr ERANGE
	call    __SET_ERRNO
	mov	EAX,-1			;return ULONG_MAX
	jmp	A7

A2:	mov	EAX,ECX
	tst	EDI			;any overflow?
	jne	overflow		;yes
A7:
    if SPTR
	mov	EBX,pend
    else
	lds	EBX,pend
    endif
	tst	EBX
	jz	A11
	dec	ESI			;back up to point to last char read
	mov	[EBX],ESI		;*pend = ESI

A11:
    if LPTR
	pop	DS
    endif
	unuse	<EDI,ESI,EBX>
	pop	EBP
	_ret	12
c_endp	strtoul

	endcode	strtoul

	end
