;_ strtoul.asm   Modified by: Walter Bright */
; Copyright (C) 1991 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

	include	macros.asm
	include stdio.asm

if LCODE
extrn __SET_ERRNO:far
else
extrn __SET_ERRNO:near
endif

	begcode	strtoul

;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert ascii string to long.
; Use:
;	unsigned long strtoul(const char *p,char **pend,int base);

    if SPTR
pend	equ	word ptr P+SIZEPTR[BP]
    else
pend	equ	dword ptr P+SIZEPTR[BP]
    endif
base	equ	word ptr P+SIZEPTR+SIZEPTR[BP]

	c_public	strtoul

func	strtoul
	push	BP
	mov	BP,SP
	.save	<SI,DI>
    if SPTR
	mov	SI,P[BP]	;get p
    else
	push	DS
	lds	SI,P[BP]	;get p
    endif
A1:	lodsb
	_if	AL e ' ', A1
	_if	AL b 9, A5
	_if	AL be 0Dh, A1	;skip white space

A3:	_if	AL ne '+', A5
	lodsb

A5:	mov	BX,base
	tst	BX
	jz	BASE0
	_if	AL ne '0', A6
	lodsb
	_if	BX e 2, BASE2
	_if	BX e 16, BASE16
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

A6:	clr	DI		;DI accumulates error indication
	mov	CX,DI
	mov	DX,CX		;DX,CX = 0: accumulate result in DX,CX
	jmp	A8

ALPHA:	and	AL,0DFh		;to upper case
	sub	AL,'A'-10	;to 10..35
	jmp	A9

	even
A4:	lodsb
A8:	cbw			;AH = 0 if AL >= 0
	tst	AL
	jle	A2
	_if	AL ae 'A', ALPHA
	sub	AL,'0'		;to binary

A9:	_if	AX ae base, A2

	;DX,CX = DX,CX * base + AL
	mov	BX,AX
	mov	AX,DX
	mul	base			;AX = DX * base
	adc	DI,0
	xchg	AX,CX
	mul	base			;DX,AX = CX * base
	add	DX,CX
	adc	DI,0

	mov	CX,BX
	add	CX,AX
	adc	DX,0
	adc	DI,0

	jmp	A4

overflow:
        mov     AX,ERANGE
    if LPTR
	pop	DS
	push    AX
	call    __SET_ERRNO
	push	DS
    else
	push    AX
	call    __SET_ERRNO
    endif
	mov	AX,-1
	cwd				;return ULONG_MAX
	jmp	A7

A2:	mov	AX,CX
	tst	DI			;any overflow?
	jne	overflow		;yes
A7:
    if SPTR
	mov	BX,pend
    else
	mov	CX,DS
	lds	BX,pend
    endif
	tst	BX
	jz	A11
	dec	SI			;back up to point to last char read
	mov	[BX],SI			;*pend = SI
    if LPTR
	mov	2[BX],CX		;*pend = CX:SI
    endif

A11:
    if LPTR
	pop	DS
    endif
	.restore <DI,SI>
	pop	BP
	ret
c_endp	strtoul

	endcode	strtoul

	end
