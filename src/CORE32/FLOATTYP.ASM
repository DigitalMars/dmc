;_floattyp.asm    Modified by: Phil Hinger */
;$Header: z:/core/crtl/src/core32/floattyp.asm,v 1.1.1.1 1997/01/02 19:17:47 smarx Exp $
;Copyright (C) 1991 by Walter Bright
;All Rights Reserved, written by Phil Hinger
;Floating point package

include macros.asm
include flthead.asm

	begcode	double

	;Note:	0=int	2=unsigned	3=long	4=float	5=double

	public	fget_dtype_pair


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Short real:
;	s | exponent| significand|
;	31|30	  23|22		0|

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; get index of a pair of float
;	index = (case(b)*dtype_base + case(c)) * 2
; input:
;	[EDX] = b
;	[EAX] = c
; Output:
;	SI = index
;
fget_dtype_pair proc near
	clr	ESI				;set to default
	mov	EDI,EDX
	test	EDI,shortexp
	jz	short fget_dtype_pair1

	and	EDI,shortexp			       ;mask exponent
	_ifs	 EDI ne short_infinity, fget_dtype_pair2  ;b is not NaN or infinite
	mov	EDI,EDX
	test	EDI,fqnan_bit				;bit must be set to be
	jz	short fget_dtype_pair3			;a quit NaN
	mov	ESI,dtype_qnan*4			;b is a quite NaN
	jmps	fget_dtype_pair2

fget_dtype_pair3:
	and	EDI,shorthid-1				;Mantissa must be zero
	jz	short fget_dtype_pair4			;otherwise
	mov	ESI,dtype_snan*4			;b is a signaling NaN
	jmps	fget_dtype_pair2

fget_dtype_pair4:					;b is infinite
	mov	ESI,dtype_infinite*4
	jmps	fget_dtype_pair2

fget_dtype_pair1:
	test	EDX,shorthid-1
	jnz	short fget_dtype_pair2
	mov	ESI,dtype_zero*4			;b is zero

fget_dtype_pair2:
	test	EAX,shortexp
	jz	short fget_dtype_pair5

	mov	EDI,EAX
	and	EDI,shortexp			       ;mask exponent
	_ifs	EDI ne short_infinity, fget_dtype_pair6  ;c is not NaN or infinite
	mov	EDI,EAX
	test	EDI,fqnan_bit				;bit must be set to be
	jz	short fget_dtype_pair7			;a quit NaN
	add	ESI,dtype_qnan * dtype_base*4		;c is a quite NaN
	ret

fget_dtype_pair7:
	and	EDI,shorthid-1				 ;Mantissa must be zero to be infinite
	jz	short fget_dtype_pair8			;otherwise
	add	ESI,dtype_snan * dtype_base*4		;c is a signaling NaN
	ret

fget_dtype_pair8:					;c is infinite
	add	ESI,dtype_infinite * dtype_base*4
	ret

fget_dtype_pair5:
	test	EAX,shorthid-1
	jnz	short fget_dtype_pair6
	add	ESI,dtype_zero * dtype_base*4		;c is zero

fget_dtype_pair6:
	ret
fget_dtype_pair endp

	endcode double

	end

