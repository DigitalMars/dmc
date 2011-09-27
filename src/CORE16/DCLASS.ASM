;_double.asm   Mar 3 1991   Modified by: Phil Hinger */
;$Header: z:/core/crtl/src/core16/dclass.asm,v 1.1.1.1 1997/01/02 19:17:33 smarx Exp $
;Copyright (C) 1984-1991 by Walter Bright
;All Rights Reserved, written by Walter Bright
;Floating point package

include macros.asm
include flthead.asm

	.8087

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; classify double float
; input:
;	[P] = a
; Output:
;	AX = classification
;
	b	 =	P

c_public __fpclassify_d

	begcode dclass

func	__fpclassify_d
	push	BP
	mov	BP,SP
	_push	<SI,DI>
	mov	AX,b+6[BP]
	mov	BX,b+4[BP]
	mov	CX,b+2[BP]
	mov	DX,b+[BP]		;mov a into registers
	mov	SI,FP_NORMAL		;set to default
	mov	DI,AX
	and	DI,longexp	       ;mask exponent
	je	fpclassify_d1	       ;set if exponent is zero

	_if	DI e long_infinity, fpclassify_d2 ;test for NaN or infinite

fpclassify_dDone:
	mov	AX,SI			;return classification
	_pop	<DI,SI>
	pop	BP
	ret

fpclassify_d2:
	mov	SI,FP_NANQ		;assumes quiet NaN
	test	AX,dqnan_bit		;bit must be set to be
	jnz	fpclassify_dDone	;a quiet NaN

	mov	SI,FP_INFINITE		;assume Infinity
	mov	DI,AX
	and	DI,longhid-1
	or	DI,BX
	or	DI,CX			;Mantissa must be zero
	or	DI,DX			;to be infinite
	jz	fpclassify_dDone	;otherwize

	mov	SI,FP_NANS		;a is a signaling NaN
	jmps	fpclassify_dDone


fpclassify_d1:
	mov	SI,FP_ZERO		;assume Zero
	mov	DI,AX
	and	DI,longhid-1		;drop sign and exponent
	or	DI,BX
	or	DI,CX
	or	DI,DX			;are any other bit set
	jz	fpclassify_dDone
					;if not then it must be subnormal
	mov	SI,FP_SUBNORMAL
	jmps	fpclassify_dDone
c_endp	__fpclassify_d
	endcode	dclass

	end
