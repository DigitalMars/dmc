;_floattyp.asm    Mar 11 1991   Modified by: Phil Hinger */
;$Header: z:/core/crtl/src/core16/floattyp.asm,v 1.1.1.1 1997/01/02 19:17:35 smarx Exp $
;Copyright (C) 1984-1988 by Walter Bright
;All Rights Reserved, written by Walter Bright
;Floating point package

include macros.asm
include flthead.asm

	.8087
	
	begcode double
	public		fget_dtype_pair
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; get index of a pair of float
;	index = (case(b)*dtype_base + case(c)) * 2
; input:
;	[CX,BX] = b
;	[DX,AX] = c
; Output:
;	SI = index
;
fget_dtype_pair proc near
	clr	SI				;set to default
	mov	DI,CX
	test	DI,07fffh
	jz	fget_dtype_pair1

	and	DI,shortexp				;mask exponent
	_if	DI ne short_infinity, fget_dtype_pair2	;b is not NaN or infinite
	mov	DI,CX
	test	DI,fqnan_bit				;bit must be set to be
	jz	fget_dtype_pair3			;a quit NaN
	mov	SI,dtype_qnan*2				;b is a quite NaN
	jmps	fget_dtype_pair2

fget_dtype_pair3:
	and	DI,07fh					;Mantissa must be zero
	or	DI,BX					;to be infinite
	jz	fget_dtype_pair4			;otherwise
	mov	SI,dtype_snan*2				;b is a signaling NaN
	jmps	fget_dtype_pair2

fget_dtype_pair4:					;b is infinite
	mov	SI,dtype_infinite*2
	jmps	fget_dtype_pair2

fget_dtype_pair1:
	or	BX,BX
	jnz	fget_dtype_pair2
	mov	SI,dtype_zero*2				;b is zero

fget_dtype_pair2:
	test	DX,07fffh
	jz	fget_dtype_pair5

	mov	DI,DX
	and	DI,shortexp				;mask exponent
	_if	DI ne short_infinity, fget_dtype_pair6	;c is not NaN or infinite
	mov	DI,DX
	test	DI,fqnan_bit				;bit must be set to be
	jz	fget_dtype_pair7			;a quit NaN
	add	SI,dtype_qnan * dtype_base*2		;c is a quite NaN
	ret

fget_dtype_pair7:
	and	DI,07fh					;Mantissa must be zero
	or	DI,AX					;to be infinite
	jz	fget_dtype_pair8			;otherwise
	add	SI,dtype_snan * dtype_base*2		;c is a signaling NaN
	ret

fget_dtype_pair8:					;c is infinite
	add	SI,dtype_infinite * dtype_base*2
	ret

fget_dtype_pair5:
	or	AX,AX
	jnz	fget_dtype_pair6
	add	SI,dtype_zero * dtype_base*2		;c is zero

fget_dtype_pair6:
	ret
fget_dtype_pair endp
	endcode	double

	end

