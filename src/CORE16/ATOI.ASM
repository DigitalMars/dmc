;_ atoi.asm   Thu Jun 29 1989   Modified by: Walter Bright */
; Windows support added by G. Eric Engstrom Jan 1991	-D_WINDOWS
; Copyright (C) 1985-1989 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

	include	macros.asm

	begcode	atoi

;;;;;;;;;;;;;;;;;;;;;;;;;
; Take absolute value of integer.

	c_public abs

func	abs
	mov	BX,SP
	mov	AX,SS:P-2[BX]
	cwd
	xor	AX,DX
	sub	AX,DX
	ret
c_endp	abs

;;;;;;;;;;;;;;;;;;;;;;;;;
; Take absolute value of long.

	c_public labs
	even
func	labs
	mov	BX,SP
	mov	AX,SS:P-2[BX]
	mov	DX,SS:P+2-2[BX]
	tst	DX
	jns	L2
	neg32	DX,AX
L2:	ret
c_endp	labs

;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert ascii string to integer.
; Use:
;	i = atoi(p);

	c_public atoi
	even
func	atoi
	push	BP
	mov	BP,SP
	.save	<SI>
    if SPTR
	mov	SI,P[BP]	;get p
    else
	push	DS
	lds	SI,P[BP]	;DS:SI -> string
    endif
A1:	lodsb
	_if	AL e ' ', A1
	_if	AL b 9, A3
	_if	AL be 0Dh, A1	;skip white space

A3:	clr	CX		;accumulated result
	mov	BX,CX		;assume positive
	mov	AH,CL
	_if	AL e '+', A4
	_if	AL ne '-', A5
	dec	BX		;neg flag (BX = -1)
	even
A4:	lodsb
A5:	sub	AL,'0'		;to binary
	jb	A2		;not a digit
	_if	AL ae 10, A2	;not a digit

	;CX = CX * 10 + AX
	shl	CX,1
	mov	DX,CX
	shl	CX,1
	shl	CX,1
	add	CX,DX
	add	CX,AX

	jmp	A4

A2:	mov	AX,CX
	xor	AX,BX
	sub	AX,BX		;if (BX == -1), negate AX
    if LPTR
	pop	DS
    endif
	.restore <SI>
	pop	BP
	ret
c_endp	atoi

	endcode	atoi

	end
