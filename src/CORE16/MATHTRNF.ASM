;_ mathtrnf.asm   Mon Dec 18 1989	 Modified by: Phil Hinger */
; Written by Phil Hinger
; Copyright (C) 1991 by Walter Bright
; All rights reserved

include macros.asm
include flthead.asm

	.8087

    ifdef _MT
    if LCODE
	c_extrn	_FEEXCEPT,far
	c_extrn	_FEROUND,far
    else
	c_extrn	_FEEXCEPT,near
	c_extrn	_FEROUND,near
    endif
    endif

	begdata
	c_extrn	_8087,word

    ifndef _MT
	extrn	__fe_cur_env:word
    endif

	enddata


	begcode double

	extrn	fget_dtype:near, fget_dtype_pair:near, fnorm:near

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; IEEE round to integral value using the current rounding direction.
;	#include <fltpnt.h>
;	float rintf(float x)
;
	b	 =	P
c_public rintf

func	rintf
	push	BP
	mov	BP,SP
	_push	<SI,DI>
	mov	DX,b+2[BP]
	mov	AX,b+[BP]		;mov b into registers
	call	fget_dtype
	cmp	SI,dtype_other
	je	rintf10

	cmp	SI,dtype_snan
	jne	rintfdone

	or	DX,fqnan_bit
	feexcept FE_INVALID

rintfdone:
	_pop	<DI,SI>
	pop	BP
	ret
rintf10:
	clr	BX		;clear for guard and sticky bits
	mov	SI,DX
	and	SI,shortexp
	cmp	SI,(shortbias + 24) * 128
	jae	rintfDone
	mov	DI,DX
	and	DI,sgn
	cmp	SI,(shortbias -1) * 128
	jb	rintf3

	mov	CX,DX
	shl	CX,1
	mov	CL,CH
	clr	CH		;right justify exponent


	sub	CX,shortbias + 24
	not	CX
	and	DX,shorthid-1
	or	DL,shorthid

rintf11:
	_if CX l 8, rintf12
	rcr	BH,1		;make room for guard bit
	adc	BL,0
	or	BH,AL		;capture sticky and guard bits
	mov	AL,AH
	mov	AH,DL
	mov	DL,DH
;	xor	DH,DH		;already clear
	sub	CX,8
	jnz	rintf11

rintf12:
	tst	CX
	je	rintf4
rintf1:
	shr32	DL,AX
	rcr	BH,1		;save sticky and guard bits
	adc	BL,0		;save sticky bits
	loop	rintf1
	jmps	rintf4
rintf3:
	clr	AX
	mov	DX,AX
	mov	BL,1
rintf4:
	tst	BX
	je	rintf5

	feexcept FE_INEXACT	;no longer exact
    ifdef _MT
	push	AX
	feround			;get rounding mode in AX
	_if	AX e FE_TONEAREST, rintf6
	_if	AX e FE_TOWARDZERO,rintf13
	cmp	AX,FE_UPWARD
	pop	AX
	je	rintf7
    else
	_if	__fe_cur_env.round e FE_TONEAREST, rintf6
	_if	__fe_cur_env.round e FE_TOWARDZERO,rintf5
	_if	__fe_cur_env.round e FE_UPWARD,  rintf7
    endif

	;FE_DOWNWARD
	test	DI,sgn
	jns	rintf5
	jmps	rintf8


rintf7:;FE_UPWARD
	test	DI,sgn
	js	rintf5
	jmps	rintf8


rintf6:;FE_TONEAREST
    ifdef _MT
	pop	AX
    endif
	test	BH,80h		;If guard bit is not set then
	jz	rintf5		;no rounding is necessary
	test	BX,7fffh
	jne	rintf8		;Sticky bits found
	test	AL,1		;Integer is Odd
	je	rintf5

rintf8:;round up
	add	AX,1
	adc	DX,0

    ifdef _MT
	jmp	short rintf5
rintf13:
	pop	AX
    endif

rintf5:
	mov	SI,DX
	or	SI,AX
	je	rintf15			    ;don't try to normalize zero

	mov	SI,24 + 7 + shortbias	    ;reset exponent
	call	fnorm
	jmp	rintfdone
rintf15:
	and	DI,sgn			    ;recover sign
	mov	DX,DI
	jmp	rintfdone
c_endp	rintf
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; classify float
; input:
;	[P] = a
; Output:
;	AX = classification
;

c_public __fpclassify_f

func	__fpclassify_f
	push	BP
	mov	BP,SP
	_push	<SI,DI>
	mov	DX,b+2[BP]
	mov	AX,b+[BP]		;mov a into registers
	mov	SI,FP_NORMAL		;set to default to normal
	mov	DI,DX
	and	DI,shortexp		;mask exponent
	je	fpclassify_f1

	_if	DI e short_infinity, fpclassify_f2 ;test for NaN or infinite

fpclassify_fDone:
	mov	AX,SI
	_pop	<DI,SI>
	pop	BP
	ret

fpclassify_f2:
	mov	SI,FP_NANQ		;assume quiet NaN
	test	DX,fqnan_bit		;bit must be set to be
	jnz	fpclassify_fDone	;a quiet NaN

	mov	SI,FP_INFINITE		;assume infinity
	mov	DI,DX
	and	DI,shorthid-1		;mask out sign and exponent
	or	DI,AX			;all bit must be clear
	jz	fpclassify_fDone	;otherwise

	mov	SI,FP_NANS		;a is a signaling NaN
	jmps	fpclassify_fDone


fpclassify_f1:
	mov	SI,FP_ZERO		;assume zero
	mov	DI,DX
	and	DI,shorthid-1		;mask out sign and exponent
	or	DI,AX			;all other bits mus be zero
	jz	fpclassify_fDone
					;otherwise it is a subnormal
	mov	SI,FP_SUBNORMAL
	jmps	fpclassify_fDone
c_endp	__fpclassify_f
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; floating Nextafterf.
; Input:
;	x,y both on stack
; Output:
;	a = [DX,AX]
;	SI,DI = preserved
; Stack offsets

c_public nextafterf

func	nextafterf
	push	BP
	mov	BP,SP
	mov	CX,P+6[BP]
	mov	BX,P+4[BP]	       ;mov y into registers CX,BX
	mov	DX,P+2[BP]
	mov	AX,P+[BP]	       ;mov x into registers DX AX
	_push	 <DI,SI>
	call	fget_dtype_pair
	jmp	word ptr cs:FNxtIndex[SI]

FNxtIndex	label	word
	dw	FNxtNormal		   ;other + other
	dw	FNxtNormal		   ;other + zero
	dw	FNxtNormal		   ;other + infinite
	dw	FNxtSecondAsQNaN	   ;other + SNaN
	dw	FNxtSecondQNaN		   ;other + QNaN

	dw	FNxtFirstZero		   ;zero + other
	dw	FNxtzeros		   ;zero + zero
	dw	FNxtFirstZero		   ;zero + infinite
	dw	FNxtSecondAsQNaN	   ;zero + SNaN
	dw	FNxtSecondQNaN		   ;zero + QNaN

	dw	FNxtNormal		   ;infinite + other
	dw	FNxtNormal		   ;infinite + zero
	dw	FNxtNormal		   ;infinite + infinite
	dw	FNxtSecondAsQNaN	   ;infinite + SNaN
	dw	FNxtSecondQNaN		   ;infinite + QNaN

	dw	FNxtFirstAsQnan		   ;SNaN + other
	dw	FNxtFirstAsQnan		   ;SNaN + zero
	dw	FNxtFirstAsQnan		   ;SNaN + infinite
	dw	FNxtLargestSNaNasQNaN	   ;SNaN + SNaN
	dw	FNxtSecondAsQNaN	   ;SNaN + QNaN

	dw	FNxtFirstQNaN		   ;QNaN + other
	dw	FNxtFirstQNaN		   ;QNaN + zero
	dw	FNxtFirstQNaN		   ;QNaN + infinite
	dw	FNxtFirstAsQNaN		   ;QNaN + SNaN
	dw	FNxtLargestQNaN		   ;QNaN + QNaN

FNxtNormal:
	mov	DI,CX
	mov	SI,DX
	and	SI,shortexp		    ;save exponent
	xor	DI,DX			    ;always subtract if signs are
	js	FNxtSub			    ;different
	_if	CX ne DX, FNxt2
	_if	BX ne AX, FNxt2
	jmp	FNxtDone		    ;return x if equal

FNxt2:
	jb	FNxtSub

FNxtAdd:
	mov	DI,DX			    ;save sign
	and	DX,shorthid-1		    ;clear exponent and sign
	tst	SI
	je	FNxt8
	or	DL,shorthid		    ;no hidden bit on subnormals
FNxt8:
	add	AX,1
	adc	DX,0
	tst	SI			    ;is it subnormal
	je	FNxt9

	test	DH,1			    ;did it add past hidden
	jz	FNxt3
FNxt10:
	add	SI,80h			    ;increment exponent (never will be infinte here)
	_if	SI l  7F80h, FNxt3
	feexcept <FE_OVERFLOW or FE_INEXACT>
;	jmp	FNxt3
FNxt4:
	mov	DX,shorthid		    ; same as shr64   AL,BX,CX,DX
	jmp	FNxt3

FNxt9:
	test	DL,shorthid		    ;test if it is still subnormal
	jz	FNxt3
	jmps	FNxt10
FNxtSub:
	mov	DI,DX			    ;save sign
	and	dX,shorthid-1		    ;clear exponent and sign
	tst	SI
	je	FNxt6			    ;subnormals don't need hidden bit
	or	DL,shorthid
FNxt6:
	sub	AX,1
	sbb	DX,0			    ;did we lose hidden bit
	test	DX,shorthid
	jne	FNxt3
	tst	SI
	je	FNxt3			    ;don't bother with subnormal

	sub	SI,80h
	shl32	DX,AX
	or	AL,1			    ;don't lose the last bit
FNxt3:
	and	DL,shorthid - 1		    ;dump hidden bit
	or	DX,SI			    ;install exponent
	and	DI,sgn			    ;mask sign bit
	or	DX,DI			    ;install sign
FNxt7:
	test	DX,7f80h		    ;check for subnormals
	jnz	FNxtDone

	feexcept <FE_UNDERFLOW or FE_INEXACT>
	jmps	FNxtDone
	
FNxtFirstZero:
	mov	DX,CX			    ;nextafter zero is the
	and	DX,sgn			    ;smallest subnorm with
	mov	AX,1			    ;the sign of the direction
	feexcept <FE_UNDERFLOW or FE_INEXACT>
	jmps	FNxtDone


FNxtSecondAsQNaN:
	or	CX ,fqnan_bit
	feexcept FE_INVALID
	jmps	FNxtSecondQNaN


FNxtLargestSNaNasQNaN:
	or	DX,dqnan_bit
	or	CX,fqnan_bit
	feexcept FE_INVALID

FNxtLargestQNaN:
	_if	DL a CL, FNxtFirstIsAnswer
	jb	FNxtSecondQNaN
	_if	AX ae BX, FNxtFirstIsAnswer

FNxtSecondQNaN:
	

FNxtSecondIsAnswer:
	mov	DX,CX
	mov	AX,BX
	jmps	FNxtDone

FNxtFirstAsQNAN:
	or	DX,fqnan_bit
	feexcept FE_INVALID

FNxtzeros:
FNxtFirstQNaN:
FNxtFirstIsAnswer:

FNxtdone:
	pop	SI
	pop	DI
	pop	BP
	ret
c_endp	nextAfterf



	endcode double


	end
