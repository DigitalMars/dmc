;_ atol.asm   Sat Jan 23 1988   Modified by: Walter Bright */
; Copyright (C) 1985-1988 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

	include	macros.asm

	begcode	atol

;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert ascii string to long.
; Use:
;	l = atol(p);

	c_public	atol

func	atol
	push	BP
	mov	BP,SP
	.save	<SI,DI>
    if SPTR
	mov	SI,P[BP]	;get p
A1:	mov	CL,[SI]
    else
	les	SI,P[BP]	;get p
A1:	mov	CL,ES:[SI]
    endif
	inc	SI
	_if	CL e ' ', A1
	_if	CL b 9, A3
	_if	CL be 0Dh, A1	;skip white space

A3:
	clr	AX
	cwd			;DX,AX = 0: accumulate result in DX,AX
	mov	BP,AX		;assume positive
	mov	CH,AL		;CH = 0
	_if	CL e '+', A4
	_if	CL ne '-', A5
	inc	BP		;nope, it's negative
	even
    if SPTR
A4:	mov	CL,[SI]
    else
A4:	mov	CL,ES:[SI]
    endif
	inc	SI
A5:	tst	CL
	jz	A2
	sub	CL,'0'		;to binary
	jb	A2
	_if	CL a 9, A2	;not a digit

	;DX,AX = DX,AX * 10 - CX
	shl	AX,1
	rcl	DX,1
	mov	BX,AX
	mov	DI,DX
	shl	AX,1
	rcl	DX,1
	shl	AX,1
	rcl	DX,1
	add	AX,BX
	adc	DX,DI
	sub	AX,CX
	sbb	DX,0

	jmp	A4

A2:	;if (BP == 0), negate the result
	tst	BP
	jnz	A6
	neg	DX
	neg	AX
	sbb	DX,BP
A6:	.restore <DI,SI>
	pop	BP
	ret
c_endp	atol

	endcode	atol

	end
