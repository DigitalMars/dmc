;_ mathtrnf.asm  May 8 1991   Modified by: Phil Hinger */
; Written by Walter Bright
; Copyright (C) 1984-1991 by Walter Bright
; All rights reserved

	include	macros.asm
	include flthead.asm

	.287

    ifdef _MT
	c_extrn	_FEEXCEPT,near
	c_extrn	_FEROUND,near
    endif

	begdata

    ifndef _MT
	extrn	__fe_cur_env:word
    endif

	enddata


	begcode double


	extrn	fget_dtype:near, fget_dtype_pair:near, fnorm:near

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; rintf function returns the rounded integal value
;
;
	b	 =	P
c_public rintf

func	rintf
	push	EBP
	mov	EBP,ESP
	_push	<EBX,ESI,EDI>
	mov	EAX,b+[EBP]		;mov b into registers
	call	fget_dtype

	_ifs	ESI e dtype_other, rintf10
	_ifs	ESI ne dtype_snan, rintfDone

	or	EAX,fqnan_bit
	feexcept FE_INVALID

rintfDone:
    if _RETST0
	;Return result in ST0
	push	EAX
	fld	dword ptr [ESP]
	fwait
	add	ESP,4
    endif
	_pop	<EDI,ESI,EBX>
	pop	EBP
	ret
rintf10:
	clr	EBX		 ;clear for guard and stick bits
	mov	ESI,EAX
	and	ESI,shortexp
	cmp	ESI,(shortbias + 24) * 800000h
	jae	rintfDone
	mov	EDI,EAX
	and	EDI,sgn
	cmp	ESI,(shortbias -1) * 800000h
	jb	rintf3

	mov	ECX,EAX
	and	ECX,sgn_mask
	shr	ECX,7+16	    ;right justify exponent


	sub	CX,shortbias + 24
	not	CX
	and	EAX,7fffffh
	or	EAX,shorthid

	_align
rintf11:
	_ifs	CX l 8, rintf12
	rcr	BX,1		;make room for guard bit
	adc	BL,0
	or	BH,AL		;capture sticky and guard bits
	shr	EAX,8
	sub	CX,8
	jnz	rintf11

rintf12:
	tst	CX
	je	rintf4

	_align
rintf1:
	shr	EAX,1
	rcr	BH,1		;save sticky and guard bits
	adc	BL,0		;save stick bits
	loop	rintf1
	jmps	rintf4
rintf3:
	clr	EAX
	mov	BL,1
rintf4:
	tst	BX
	je	rintf5

	feexcept FE_INEXACT	;no longer exact
    ifdef _MT
	push	EAX
	feround			;get rounding mode in EAX
	_ifs	EAX e FE_TONEAREST, rintf6
	_ifs	EAX e FE_TOWARDZERO,rintf13
	cmp	EAX,FE_UPWARD
	pop	EAX
	je	short rintf7
    else
	_ifs	__fe_cur_env.round e FE_TONEAREST, rintf6
	_ifs	__fe_cur_env.round e FE_TOWARDZERO,rintf5
	_ifs	__fe_cur_env.round e FE_UPWARD,  rintf7
    endif

	;FE_DOWNWARD
	test	EDI,sgn
	jns	rintf5
	jmps	rintf8


rintf7:;FE_UPWARD
	test	EDI,sgn
	js	rintf5
	jmps	rintf8


rintf6:;FE_TONEAREST
    ifdef _MT
	pop	EAX
    endif
	test	BH,80h		;If guard bit is not set then
	jz	rintf5		 ;no rounding is necessary
	test	BX,7fffh
	jne	rintf8		 ;Sticky bits found
	test	AL,1		;Integer is Odd
	je	rintf5

rintf8:;round up
	add	EAX,1

    ifdef _MT
	jmp	short rintf5
rintf13:
	pop	EAX
    endif

rintf5:
	mov	ESI, 24 + 7 + shortbias ;reset exponent
	call	fnorm
	jmp	rintfDone
c_endp	rintf


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; floating Nextafterf.
; Input:
;	x,y both on stack
; Output:
;	a = EDX
;	ESI,EDI = preserved
; Stack offsets

c_public nextafterf

func	nextafterf
	push	EBP
	mov	EBP,ESP
	_push	 <EBX,EDI,ESI>

	mov	EDX,P+4[EBP]		 ;mov b into registers
	mov	EAX,P[EBP]		 ;mov b into registers

	call	fget_dtype_pair
	jmp	dword ptr cs:fnxtindex[ESI]

fnxtindex	label	word
    if _FLAT
	dd	offset FLAT:NxtNormal		  ;other + other
	dd	offset FLAT:NxtNormal		  ;other + zero
	dd	offset FLAT:NxtNormal		  ;other + infinite
	dd	offset FLAT:NxtSecondAsQNaN		  ;other + SNaN
	dd	offset FLAT:NxtSecondQNaN		  ;other + QNaN

	dd	offset FLAT:NxtFirstZero		  ;zero + other
	dd	offset FLAT:Nxtzeros		  ;zero + zero
	dd	offset FLAT:NxtFirstZero		  ;zero + infinite
	dd	offset FLAT:NxtSecondAsQNaN		  ;zero + SNaN
	dd	offset FLAT:NxtSecondQNaN		  ;zero + QNaN

	dd	offset FLAT:NxtNormal		  ;infinite + other
	dd	offset FLAT:NxtNormal		  ;infinite + zero
	dd	offset FLAT:NxtNormal		  ;infinite + infinite
	dd	offset FLAT:NxtSecondAsQNaN		  ;infinite + SNaN
	dd	offset FLAT:NxtSecondQNaN		  ;infinite + QNaN

	dd	offset FLAT:NxtFirstAsQNaN		  ;SNaN + other
	dd	offset FLAT:NxtFirstAsQNaN		  ;SNaN + zero
	dd	offset FLAT:NxtFirstAsQNaN		  ;SNaN + infinite
	dd	offset FLAT:NxtLargestSNaNasQNaN	  ;SNaN + SNaN
	dd	offset FLAT:NxtSecondAsQNaN		  ;SNaN + QNaN

	dd	offset FLAT:NxtFirstQNaN		  ;QNaN + other
	dd	offset FLAT:NxtFirstQNaN		  ;QNaN + zero
	dd	offset FLAT:NxtFirstQNaN		  ;QNaN + infinite
	dd	offset FLAT:NxtFirstAsQNaN		  ;QNaN + SNaN
	dd	offset FLAT:NxtLargestQNaN		  ;QNaN + QNaN
    else
	dd	NxtNormal		  ;other + other
	dd	NxtNormal		  ;other + zero
	dd	NxtNormal		  ;other + infinite
	dd	NxtSecondAsQNaN		  ;other + SNaN
	dd	NxtSecondQNaN		  ;other + QNaN

	dd	NxtFirstZero		  ;zero + other
	dd	Nxtzeros		  ;zero + zero
	dd	NxtFirstZero		  ;zero + infinite
	dd	NxtSecondAsQNaN		  ;zero + SNaN
	dd	NxtSecondQNaN		  ;zero + QNaN

	dd	NxtNormal		  ;infinite + other
	dd	NxtNormal		  ;infinite + zero
	dd	NxtNormal		  ;infinite + infinite
	dd	NxtSecondAsQNaN		  ;infinite + SNaN
	dd	NxtSecondQNaN		  ;infinite + QNaN

	dd	NxtFirstAsQNaN		  ;SNaN + other
	dd	NxtFirstAsQNaN		  ;SNaN + zero
	dd	NxtFirstAsQNaN		  ;SNaN + infinite
	dd	NxtLargestSNaNasQNaN	  ;SNaN + SNaN
	dd	NxtSecondAsQNaN		  ;SNaN + QNaN

	dd	NxtFirstQNaN		  ;QNaN + other
	dd	NxtFirstQNaN		  ;QNaN + zero
	dd	NxtFirstQNaN		  ;QNaN + infinite
	dd	NxtFirstAsQNaN		  ;QNaN + SNaN
	dd	NxtLargestQNaN		  ;QNaN + QNaN
    endif

NxtNormal:
	mov	EDI,EDX
	mov	ESI,EAX
	and	ESI,shortexp		  ;save exponent
	xor	EDI,EAX			  ;alwasy subtract if signs are
	js	NxtSub			  ;differnent
	_ifs	EDX ne EAX, Nxt2	  ;return x if equal
	jmp	NxtDone
Nxt2:
	jb	NxtSub

NxtAdd:
	mov	EDI,EAX			  ;save sign
	and	EAX,007fffffh		  ;clear exponent and sign
	tst	ESI
	je	Nxt8
	or	EAX,shorthid		  ;no hidden bit on subnormals
Nxt8:
	add	EAX,1
	tst	ESI			  ;is it subnormal
	je	Nxt9

	test	EAX,01000000h		  ;did it add past hidden
	jz	Nxt3
Nxt10:
	add	ESI,shorthid		  ;increment exponent (never will be infinte here)
	_ifs	ESI l  7F800000h, Nxt3
	feexcept <FE_OVERFLOW or FE_INEXACT>
;	jmp	Nxt3
Nxt4:
	mov	EAX,shorthid		  ; same as shr
	jmp	Nxt3

Nxt9:
	test	EAX,shorthid		  ;test if it is still subnormal
	jz	Nxt3
	jmps	Nxt10
NxtSub:
	mov	EDI,EAX			  ;save sign
	and	EAX,007fffffh		  ;clear expont and sign
	tst	ESI
	je	Nxt6			  ;subnormals don't need hidden bit
	or	EAX,shorthid
Nxt6:
	sub	EAX,1			  ;did we lose hidden bit
	test	EAX,shorthid
	jne	Nxt3
	tst	ESI
	je	Nxt3			  ;don't bother with subnormal

	sub	ESI,longhid
	shl	EAX,1
	or	AL,1			  ;don't lose the last bit
Nxt3:
	and	EAX,007fffffh		  ;dump hidden bit
	or	EAX,ESI			  ;install exponent
	and	EDI,sgn			  ;mask sign bit
	or	EAX,EDI			  ;install sign
Nxt7:
	test	EAX,7f800000h		  ;check for subnormals
	jnz	NxtDone

	feexcept <FE_UNDERFLOW or FE_INEXACT>
	jmps	NxtDone
	
NxtFirstZero:
	mov	EAX,EDX			  ;nextafter zero is the
	and	EAX,sgn			  ;smalled subnorm with the
	or	EAX,1			  ;the sign of the direction
	feexcept <FE_UNDERFLOW or FE_INEXACT>
	jmps	NxtDone


NxtSecondAsQNaN:
	or	EDX ,dqnan_bit
	feexcept FE_INVALID
	jmps	NxtSecondQNaN


NxtLargestSNaNasQNaN:
	or	EAX,dqnan_bit
	or	EDX,dqnan_bit
	feexcept FE_INVALID

NxtLargestQNaN:
	_ifs	EAX ae EDX,   NxtFirstIsAnswer

NxtSecondQNaN:
	

NxtSecondIsAnswer:
	mov	EAX,EDX
	jmps	NxtDone

NxtFirstAsQNaN:
	or	EAX,dqnan_bit
	feexcept FE_INVALID

Nxtzeros:
NxtFirstQNaN:
NxtFirstIsAnswer:

NxtDone:
    if _RETST0
	;Return result in ST0
	push	EAX
	fld	dword ptr [ESP]
	fwait
	add	ESP,4
    endif
	_pop	<ESI,EDI,EBX>
	pop	EBP
	ret
c_endp	nextafterf

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; classify float
; input:
;	[P] = a
; Output:
;	EAX = classification
;

c_public __fpclassify_f

func	__fpclassify_f
	push	EBP
	mov	EBP,ESP
	_push	<ESI,EDI>
	mov	EAX,b+[EBP]		;mov a into registers
	mov	ESI,FP_NORMAL		;set to default to normal
	mov	EDI,EAX
	and	EDI,shortexp		;mask exponent
	je	fpclassify_f1

	_ifs	EDI e short_infinity, fpclassify_f2 ;test for NaN or infinite

fpclassify_fDone:
	mov	EAX,ESI
	_pop	<EDI,ESI>
	pop	EBP
	ret

fpclassify_f2:
	mov	ESI,FP_NANQ		;assume quiet NaN
	test	EAX,fqnan_bit		;bit must be set to be
	jnz	fpclassify_fDone	;a quiet NaN

	mov	ESI,FP_INFINITE		;assume infinity
	test	EAX,7fffffh		;make sure there are not any other bits
	jz	fpclassify_fDone	;otherwize

	mov	ESI,FP_NANS		;a is a signaling NaN
	jmps	fpclassify_fDone


fpclassify_f1:
	mov	ESI,FP_ZERO		;assume zero
	and	EAX,7fffffh		;Test for mantiza bits
	jz	fpclassify_fDone
					;otherwise it is a subnormal
	mov	ESI,FP_SUBNORMAL
	jmps	fpclassify_fDone
c_endp	__fpclassify_f


	endcode double

	end
