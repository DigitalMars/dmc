;_float.asm    Mar 11 1991   Modified by: Phil Hinger */
;$Header: z:/core/crtl/src/dos32/float.asm,v 1.1.1.1 1997/01/02 19:17:57 smarx Exp $
;Copyright (C) 1984-1988 by Walter Bright
;All Rights Reserved, written by Walter Bright
;Floating point package

include macros.asm
include flthead.asm

	.287

    ifdef _MT
	c_extrn	_FEEXCEPT,near
	c_extrn	_FEROUND,near
    endif
	extrn	__FDIVP:near

	begdata
	extrn	__8087:word		;8087 flag word
	extrn	_fdiv_chk_flag:byte

    ifndef _MT
	extrn	__fe_cur_env:word
    endif

	enddata

	begcode	double

	;Note:	0=int	2=unsigned	3=long	4=float	5=double

	public		__FSUB@
	c_public	_FADD@,_FMUL@,_FDIV@,_FNEG@,_FTST@
	c_public	_FTST0@,_FTST0EXC@
	c_public	_FCMP@,_FCMPEXC@
	c_public	_INTFLT@,_UNSFLT@,_LNGFLT@,_FLTINT@,_FLTUNS@,_FLTLNG@,_FLTUNLNG@

    if LCODE
	c_extrn		_DBLINT@,far,_INTDBL@,far,_DBLUNS@,far,_UNSDBL@,far
	c_extrn		_DBLLNG@,far,_LNGDBL@,far,_DBLULNG@,far,_ULNGDBL@,far
	c_extrn		_DBLFLT@,far
    else
	c_extrn		_DBLINT@,near,_INTDBL@,near,_DBLUNS@,near,_UNSDBL@,near
	c_extrn		_DBLLNG@,near,_LNGDBL@,near,_DBLULNG@,near,_ULNGDBL@,near
	c_extrn		_DBLFLT@,near
    endif

	extrn		fget_dtype:near, fround:near, fright_justify:near
	extrn		fleft_justify:near, fnorm:near, funnorm:near
	extrn		fget_dtype:near
	extrn		fget_dtype_pair:near


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Short real:
;	s | exponent| significand|
;	31|30	  23|22		0|

CCeq	equ	40h
CClt	equ	81h
CCgt	equ	00h
CCinv	equ	45h


;;;;;;;;;;;;;;;;;;;;;;;;;;
; Test and see if [EAX] is 0

	_align
func	_FTST@
	push	EAX
	shl	EAX,1		 ;dump sign bit
	tst	EAX
	pop	EAX
	ret
c_endp	_FTST@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Float compare against 0, setting sign bits appropriately
;	a = b ? 0
; Input:
;	[EAX] = b
; Output:
;	[EAX] = b
; _FTST0EXC@ is same as _FTST0@, but set invalid exception flag if
; the operand is a NAN.

	_align
func	_FTST0EXC@
	stc
	jmp	short DT9
c_endp	_FTST0EXC@

	_align
func	_FTST0@
if 1
	clc
DT9:	push	ESI
	sbb	ESI,ESI			  ;SI==-1 if raise exception
	push	EDI
	push	EAX

	;Test if b is a special value
	mov	EDI,EAX
	and	EDI,shortexp
	jz	DT1			   ;c is 0 or subnormal
	_ifs	EDI e shortexp, DT2	    ;c is nan or infinity

	;Do a straightforward comparison

DT3:	test	EAX,sgn_mask
	jz	DTeq
	cmp	EAX,0
	mov	AH,CCgt
	jg	DTret
	mov	AH,CClt
DTret:	;or	AX,AX			;OF (overflow flag) is already clear
	sahf
	_pop	<EAX,EDI,ESI>
	ret


DTeq:	mov	AH,CCeq
	jmp	DTret


DT1:	test	EAX,7fffffh
	jnz	DT3			;subnormal
	jmp	DTeq			;b is 0

DT2:	test	EAX,7fffffh
	jz	DT3			;b is infinity

	;b is a NAN
	mov	AH,CCinv
	and	ESI,FE_INVALID
	feexcept SI
	jmp	DTret


else
	push	EAX
	tst	EAX
	js	TST01
	neg	EAX
	sbb	EAX,EAX
	neg	EAX
	pop	EAX
	ret

TST01:
	shl	EAX,1
	neg	EAX			 ;C=1 if b!=0
	sbb	EAX,EAX			  ;if (b==0) DX=0 else DX=-1
	pop	EAX
	ret
endif
c_endp	_FTST0@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Float compare
;	a = b ? c
; Input:
;	EAX = b
;	EDX = c
; Output:
;	no registers changed
;	Condition codes set same as 8087 would
;	(but also set SF and OF so that pre-3.0 code will link which
;	used signed jmps after FCMP)
;
; _FCMPEXC@ is same as _FCMP@, but set invalid exception flag if
; either of the operands are NAN.

if 1
	_align
func	_FCMPEXC@
	stc
	jmp	short DC9
c_endp	_FCMPEXC@

	_align
func	_FCMP@
	clc
DC9:
	push	ESI
	sbb	ESI,ESI			  ;SI==-1 if raise exception
	push	EDI
	push	EDX
	push	EAX


	;Test if c is a special value
	mov	EDI,EDX
	and	EDI,shortexp
	jz	DC1			;c is 0 or subnormal
	_ifs	 EDI e shortexp, DC2	;c is nan or infinity

	;Test if b is a special value
DC3:	mov	EDI,EAX
	and	EDI,shortexp
	jz	DC4			;b is 0 or subnormal
	_ifs	EDI e shortexp, DC5	;b is nan or infinity

	;Do a straightforward comparison
DC6:	mov	EDI,EDX
	xor	EDI,EAX
	js	DC8			;signs are different
	_ifs	EAX ne EDX, DC7


DCeq:	mov	AH,CCeq
DCret:	or	AX,AX			;clear OF (overflow flag)
	sahf
	pop	EAX
	pop	EDX
	pop	EDI
	pop	ESI
	ret


DC7:
	mov	AH,CCgt
	ja	DC10
	mov	AH,CClt
DC10:	test	EDX,sgn
	jns	DCret
	xor	AH,CCgt XOR CClt
	jmp	DCret

DC8:	test	EDX,sgn
	mov	AH,CClt
	jns	DCret
	mov	AH,CCgt
	jmp	DCret

DC1:	test	EDX,7fffffh
	jnz	DC3			;subnormal
	;c is +0 or -0
	and	EDX,sgn_mask		;no -0 bugs
	jmp	DC3			;c is 0

DC2:	test	EDX,7fffffh
	jz	DC3			;c is infinity
	jmp	short DCinv		;c is a nan

DC4:	test	EAX,7fffffh
	jnz	DC6			;b is subnormal
	;c is +0 or -0
	and	EAX,sgn_mask		 ;no -0 bugs
	jmp	DC6			;b is 0

DC5:	test	EAX,7fffffh
	jz	DC6			;b is infinity
;	jmp	DCinv			;b is a nan

DCinv:	mov	AH,CCinv
	and	ESI,FE_INVALID
	feexcept SI
	jmp	DCret
c_endp	_FCMP@


else
func	_FCMP@
	push	EDI
	push	EDX
	push	ECX

	;test if c is 0
	mov	EDI,EDX
	shl	EDI,1			;dump sign bit
	tst	EDI
	jnz	C3			;no
	and	EDX,7fffffffh;		;no -0 bugs
C3:
	mov	EDI,EAX
	shl	EDI,1			;dump sign bit
	tst	EDI
	jnz	C2			;no
	and	EAX,7fffffffh		;convert -0 to 0
C2:
	mov	EDI,EAX
	xor	EDI,EDX
	js	C52			;signs are different
	mov	EDI,1			;1 for positive compares
	tst	ECX
	jns	C51
	neg	EDI			;-1 for negative compares
C51:	_ifs	 EAX a EDX, C7		  ;compare MSW
	neg	EDI
C7:	tst	EDI
L21:	pop	ECX
	pop	EDX
	pop	EDI
	ret

C52:	cmp	EAX,EDX
	jmp	L21
c_endp	_FCMP@
endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  floating add/subtract.
;	a = b +(-) c
; Input:
;	[EDX] = b
;	[EAX] = c
; Output:
;	a = [EAX]
;	SI,DI = preserved
; Stack offsets


	padnn	 =	22	;so nn == dd == mm == 50
	sign	 =	padnn+8
	signc	 =	sign+4
	subtract =	signc+4
	exp	 =	subtract+4
	orgsign	 =	exp+4
	nn	 =	orgsign+4
	b	 =	nn+P

__FSUB@:
	push	EBP
	sub	ESP,nn	     ;make room for nn variables
	mov	EBP,ESP
	_ifs	 __8087 e 0, A6		 ;if no 8087
	push	EAX			;push b
	fld	dword ptr -4[EBP]	;load b into 8087
	_push	EDX			;push c
	fsub	dword ptr -8[EBP]	;sub c
	jmps	fltret

A6:
	mov	dword ptr orgsign[EBP],sgn ;need to flip sign back on  NaNs
	xor	EDX,sgn			;flip sign for subtraction
	jmps	A1

	_align
func	_FADD@
	push	EBP
	sub	ESP,nn			;make room for nn variables
	mov	EBP,ESP
	mov	dword ptr orgsign[EBP],0  ;need for NaN
	_ifs	 __8087 e 0, A1		  ;if no 8087
	_push	EAX			;push b
	fld	dword ptr -4[EBP]	 ;load b into 8087
	_push	EDX			;push c
	fadd	dword ptr -8[EBP]	 ;add c

fltret:
	;Check for floating point error
	fstsw	-2[EBP]
	fwait
	mov	AX,-2[EBP]
	and	AX,FE_ALL_EXCEPT
	jnz	fltret5			;jmp if error
fltret4:
	
	fstp	dword ptr -8[EBP]
	fwait
	pop	EAX
	pop	EDX			;pop result
	add	ESP,nn			 ;subtract c off also
	pop	EBP
	ret

fltret5:
	feexcept AX
	jmp	fltret4


A1:
	push	EDI
	push	ESI
	call	fget_dtype_pair
	jmp	dword ptr cs:Daddindex[ESI]

	_align
Daddindex	label	word
	dd	ANormalAdd			;other + other
	dd	AFirstIsAnswer		;other + zero
	dd	ASecondIsAnswer		;other + infinite
	dd	ASecondAsQNaN		;other + SNaN
	dd	ASecondQNaN		;other + QNaN

	dd	ASecondIsAnswer		;zero + other
	dd	Azeros			;zero + zero
	dd	ASecondIsAnswer		;zero + infinite
	dd	ASecondAsQNaN		;zero + SNaN
	dd	ASecondQNaN		;zero + QNaN

	dd	AFirstIsAnswer		;infinite + other
	dd	AFirstIsAnswer		;infinite + zero
	dd	AInfiniteInfinite	;infinite + infinite
	dd	ASecondAsQNaN		;infinite + SNaN
	dd	ASecondQNaN		;infinite + QNaN

	dd	AFirstAsQNaN		;SNaN + other
	dd	AFirstAsQNaN		;SNaN + zero
	dd	AFirstAsQNaN		;SNaN + infinite
	dd	ALargestSNaNasQNaN	;SNaN + SNaN
	dd	ASecondAsQNaN		;SNaN + QNaN

	dd	AFirstQNaN		;QNaN + other
	dd	AFirstQNaN		;QNaN + zero
	dd	AFirstQNaN		;QNaN + infinite
	dd	AFirstAsQNaN		;QNaN + SNaN
	dd	ALargestQNaN		;QNaN + QNaN

ANormalAdd:

	mov	signc[EBP],EAX		;Save sign
	mov	EDI,EDX
	mov	sign[EBP],EDI
	xor	EDI,EAX			;if sign(b) != sign(c), then subtraction
	mov	subtract[EBP],EDI	;flag for subtraction

	call	funnorm			;unpack second operand (c)
	mov	exp[EBP],ESI		;save exponent of c
	xchg	EAX,EDX			;exchange regs
	call	funnorm			;unpack first operand (b)
	sub	ESI,exp[EBP]		;SI = exp(b) - exp(c)
	jle	A2			;exp(b) > exp(c)
	add	exp[EBP],ESI		;exponent is b
	xchg	EAX,EDX
	neg	ESI
	mov	EDI,signc[EBP]
	mov	sign[EBP],EDI		;reset sign
A2:
	call	fright_justify
	test	dword ptr subtract[EBP],sgn ;subtracting? (test bit 15)
	je	A3			;no

	sub	EAX,EDX

	jnc	A4			 ;no borrow
	xor	dword ptr sign[EBP],sgn  ;toggle sign of result
	neg	EAX			 ;SI must be 0 for this to work

A4:
	test	EAX,0c0000000h		 ;if bit 8 or 7 are set then
	jnz	A5			 ;do normal round
	tst	EAX			 ;is result zero
	je	Azeros1			 ;yes

	mov	EDI,sign[EBP]
	mov	ESI,exp[EBP]		 ;exponent of result
	call	fnorm			 ;normalize and pack
	jmp	ADone

A3:
	add	EAX,EDX
	jnc	A5
	rcr	EAX,1
	inc	word ptr exp[EBP]	 ;bump exponent

A5:
	mov	EDI,sign[EBP]
	mov	ESI,exp[EBP]		 ;exponent of result
	call	fround			 ;round and normalize
	jmp	ADone


Azeros:
	mov	EDI,EAX
	xor	EDI,EDX
	test	EDI,sgn			 ;are signs the same
	jne	Azeros1
	jmp	ADone			 ;yes

Azeros1:
    ifdef _MT
	feround
	cmp	EAX,FE_DOWNWARD
	mov	EAX,0
	je	Azeros2
    else
	clr	EAX
	_ifs	 __fe_cur_env.round e FE_DOWNWARD,Azeros2
    endif
	jmp	AFirstIsAnswer
Azeros2:
	mov	EAX,sgn			 ;set sign to -
	jmps	AFirstIsAnswer
	
AInfiniteInfinite:
	mov	EDI,EAX
	xor	EDI,EDX
	test	EDI,sgn			;are signs the same
	je	AFirstIsAnswer		;yes

					;Default invalid operation
	mov	EAX,short_qnan OR 10000h
	feexcept FE_INVALID
	jmps	ADone


ASecondAsQNaN:
	or	EDX,fqnan_bit
	feexcept FE_INVALID
	jmps	ASecondQNaN


ALargestSNaNasQNaN:
	or	EAX,fqnan_bit
	or	EDX,fqnan_bit
	feexcept FE_INVALID

ALargestQNaN:
	mov	ESI,EAX
	and	ESI,sgn_mask
	mov	EDI,EDX
	and	EDI,sgn_mask
	_ifs	ESI ae EDI, AFirstIsAnswer

ASecondQNaN:
	
	xor	EDX,orgsign[EBP]	 ;reset orginal sign
ASecondIsAnswer:
	mov	EAX,EDX
	jmps	ADone

AFirstAsQNaN:
	or	EAX,fqnan_bit
	feexcept FE_INVALID

AFirstQNaN:

AFirstIsAnswer:

ADone:
	pop	ESI
	pop	EDI
	add	ESP,nn
	pop	EBP
	ret
c_endp	_FADD@
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Floating divide.
;	a = b / c
; Input:
;	 [EDX] = b
;	 [EDX] = c
; Output:
;	a = [EAX]
;	ESI,EDI preserved
; Stack offsets:

	paddd	=	18		;so nn == cxdd == mm == 50
	resp	=	paddd+16	;pointer to result
	sign	=	resp+4		;sign of result
	exp	=	sign+4		;exponent of result
	count	=	exp+4		;loop counter
	cxdd	=	count+4		;amount of local variables

func	_FDIV@
	push	EBP
	sub	ESP,cxdd
	mov	EBP,ESP

	_ifs	 __8087 e 0, D7		  ;if no 8087
	push	EAX			;push b
	fld	dword ptr -4[EBP]	;load b into 8087
	push	EDX			;push c

	; FDIV bug workaround
	_if	_fdiv_chk_flag e 1, D20	;if FDIV is ok
	fld	dword ptr -8[EBP]
	call	__FDIVP			;workaround for FDIV bug
	jmp	fltret
D20:
	fdiv	dword ptr -8[EBP]	 ;div c
	jmp	fltret

D7:	push	ESI
	push	EDI

	mov	sign[EBP],EAX		;transfer sig(b) to 0[EBP]
	xor	sign[EBP],EDX
	and	dword ptr sign[EBP],sgn
	call	fget_dtype_pair
	jmp	dword ptr cs:Dindex[ESI]

	_align
Dindex	label	word
	dd	DNormalDivide		;other / other
	dd	DDivideByZero		;other / zero
	dd	DSignedZero		;other / infinite
	dd	DSecondAsQNaN		;other / SNaN
	dd	DSecondQNaN		;other / QNaN

	dd	DSignedZero		;zero / other
	dd	DDefaultQNaN		;zero / zero
	dd	DSignedZero		;zero / infinite
	dd	DSecondAsQNaN		;zero / SNaN
	dd	DSecondQNaN		;zero / QNaN

	dd	DSignedInfinite		;infinite / other
	dd	DSignedInfinite		;infinite / zero
	dd	DDefaultQNaN		;infinite / infinite
	dd	DSecondAsQNaN		;infinite / SNaN
	dd	DSecondQNaN		;infinite / QNaN

	dd	DFirstAsQNaN		;SNaN / other
	dd	DFirstAsQNaN		;SNaN / zero
	dd	DFirstAsQNaN		;SNaN / infinite
	dd	DLargestSNaNasQNaN	;SNaN / SNaN
	dd	DSecondAsQNaN		;SNaN / QNaN

	dd	DFirstQNaN		;QNaN / other
	dd	DFirstQNaN		;QNaN / zero
	dd	DFirstQNaN		;QNaN / infinite
	dd	DFirstAsQNaN		;QNaN / SNaN
	dd	DLargestQNaN		;QNaN / QNaN


DNormalDivide:
	;unpack c
	mov	EDI,EDX
	and	EDI,shortexp		;mask off exponent bits
	and	EDX,shorthid-1		;remove exponent from mantissa
	tst	EDI			;is exponent zero
	jnz	D12
					;special case when exponent is zero
	xchg	EAX,EDX			;may need to do a lot of shifting
	call	fleft_justify		;msb must be block to left
	xchg	EAX,EDX
	mov	EDI,ESI			;save exponent
	jmps	D13
D12:
	or	EDX,shorthid
	shr	EDI,16+7		;right justify exponent
D13:
	
	;unpack b
	mov	ESI,EAX
	and	ESI,shortexp		;mask off exponent bits
	and	EAX,shorthid-1
	tst	ESI			;is exponent zero
	jnz	D14
					;special case when exponent is zero
	call	fleft_justify		;msb must be block to left
	jmps	D15
D14:
	or	EAX,shorthid		;or in hidden bit
	shr	ESI,16+7		;right justify exponent
D15:
	sub	ESI,EDI			;exp(result) = exp(b) - exp(c)
	add	ESI,shortbias		;so bias is retained after subtraction
	mov	exp[EBP],ESI		;exponent of result

	shl	EDX,1			;Make sure there is not an overflow
	dec	ESI
	mov	ECX,EDX
	mov	EDX,EAX
	clr	EAX			;arange regestors for divide
	div	ECX
	tst	EDX
	je	D16
	or	EAX,1			;set Sticky bit if remainder
D16:
	mov	ESI,exp[EBP]
	mov	EDI,sign[EBP]
	call	fround			;round and normalize result
	jmp	DDone



DDivideByZero:
	feexcept FE_DIVBYZERO

DSignedInfinite:
	mov	EAX,sign[EBP]
	or	EAX,short_infinity
	jmps	DDone

DSignedZero:
	mov	EAX,sign[EBP]
	jmps	DDone

DSecondAsQNaN:
	or	EDX,fqnan_bit
	feexcept FE_INVALID

DSecondQNaN:
	xchg	EAX,EDX
	jmps	DDone

	_align
DDefaultQNaN:
	mov	EAX,short_qnan OR 10000h
	or	EAX,sign[EBP]
	feexcept FE_INVALID
	jmps	DDone


DLargestSNaNasQNaN:
	or	EAX,fqnan_bit
	or	EDX,fqnan_bit
	feexcept FE_INVALID

DLargestQNaN:
	mov	ESI,EAX
	and	ESI,sgn_mask
	mov	EDI,EDX
	and	EDI,sgn_mask
	_ifs	ESI ae EDI, DFirstQNaN
	jmps	DSecondQNaN

DFirstAsQNaN:
	or	EAX,fqnan_bit
	feexcept FE_INVALID
DFirstQNaN:

DDone:
	pop	EDI
	pop	ESI
	add	ESP,cxdd
	pop	EBP
	ret
c_endp	_FDIV@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Floating Multiply
;	a = b * c
; Input:
;	[EAX] = b
;	[EDX] = c
; Output:
;	a = [EDX]
;	ESI,EDI preserved
; Stack offsets:

	padmm	=	26		;so nn == cxdd == mm == 50
	resp	=	padmm+8		;pointer to result
	sign	=	resp+4		;sign of result
	exp	=	sign+4		;exponent of result
	count	=	exp+4		;loop counter
	mm	=	count+4		;amount of local variables

func	_FMUL@
	push	EBP
	sub	ESP,mm
	mov	EBP,ESP

	_ifs	 __8087 e 0, M1		;if no 8087
	push	EAX			;push b
	fld	dword ptr -4[EBP]	;load b into 8087
	push	EDX		;push c
	fmul	dword ptr -8[EBP]	;mul c
	jmp	fltret

M1:	push	ESI
	push	EDI

	mov	sign[EBP],EAX		;transfer sig(b) to 0[EBP]
	xor	sign[EBP],EDX
	and	dword ptr sign[EBP],sgn
	call	fget_dtype_pair
	jmp	dword ptr cs:Mindex[ESI]

Mindex	label	dword
	dd	MNormalMultiply		;other * other
	dd	MSignedZero		;other * zero
	dd	MSignedInfinite		;other * infinite
	dd	MSecondAsQNaN		;other * SNaN
	dd	MSecondQNaN		;other * QNaN

	dd	MSignedZero		;zero * other
	dd	MSignedZero		;zero * zero
	dd	MDefaultQNaN		;zero * infinite
	dd	MSecondAsQNaN		;zero * SNaN
	dd	MSecondQNaN		;zero * QNaN

	dd	MSignedInfinite		;infinite * other
	dd	MDefaultQNaN		;infinite * zero
	dd	MSignedInfinite		;infinite * infinite
	dd	MSecondAsQNaN		;infinite * SNaN
	dd	MSecondQNaN		;infinite * QNaN

	dd	MFirstAsQNaN		;SNaN * other
	dd	MFirstAsQNaN		;SNaN * zero
	dd	MFirstAsQNaN		;SNaN * infinite
	dd	MLargestSNaNasQNaN	;SNaN * SNaN
	dd	MSecondAsQNaN		;SNaN * QNaN

	dd	MFirstQNaN		;QNaN * other
	dd	MFirstQNaN		;QNaN * zero
	dd	MFirstQNaN		;QNaN * infinite
	dd	MFirstAsQNaN		;QNaN * SNaN
	dd	MLargestQNaN		;QNaN * QNaN


MNormalMultiply:
	call	funnorm		;unpack second operand (c)
	mov	exp[EBP],ESI	;save exponent of c
	xchg	EAX,EDX		;may need to do a lot of shifting
	call	funnorm		;unpack second operand (b)
	sub	ESI,shortbias - 1;so bias is retained after add
	add	ESI,exp[EBP]

	shr	EDX,1		;Make sure there is not an overflow
	inc	ESI
	MUL	EDX
	test	EDX,0c0000000h	 ;Make sure one of the two high order
	jne	M4		;bits are set
	shld	EDX,EAX,1	;in this case the third will alway be set
	shl	EAX,1
	dec	ESI
M4:
	tst	EAX		;check for sticky bits
	je	M5
	or	EDX,1
M5:
	mov	EAX,EDX
	mov	EDI,sign[EBP]
	call	fround		;round and normalize result
	jmp	MDone

MSignedInfinite:
	mov	EAX,sign[EBP]
	or	EAX,short_infinity
	jmps	MDone

MSignedZero:
	mov	EAX,sign[EBP]
	jmps	MDone

MSecondAsQNaN:
	or	EDX,fqnan_bit
	feexcept FE_INVALID

MSecondQNaN:
	xchg	EAX,EDX
	jmps	MDone

MDefaultQNaN:
	mov	EAX,short_qnan
	or	EAX,sign[EBP]
	feexcept FE_INVALID
	jmps	MDone


MLargestSNaNasQNaN:
	or	EAX,fqnan_bit
	or	EDX,fqnan_bit
	feexcept FE_INVALID

MLargestQNaN:
	mov	ESI,EAX
	and	ESI,sgn_mask
	mov	EDI,EDX
	and	EDI,sgn_mask
	_ifs	ESI a EDI,   MFirstQNaN
	jmps	MSecondQNaN

MFirstAsQNaN:
	or	EAX,fqnan_bit
	feexcept FE_INVALID
MFirstQNaN:

MDone:
	pop	EDI
	pop	ESI
	add	ESP,mm
	pop	EBP
	ret
c_endp	_FMUL@
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
func	_FNEG@
	xor	EAX,sgn
	ret
c_endp	_FNEG@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert int to float

func	_INTFLT@
	_push	<EBX,ECX,EDX>
	callm	_INTDBL@
CX1:	callm	_DBLFLT@
CX2:	_pop	<EDX,ECX,EBX>
	ret
c_endp	_INTFLT@
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert float to unsigned.


	_align
func	_FLTUNS@
	push	EDX
	mov	EDX,15
	test	EAX,sgn
	jz	fltIntgl

	feexcept FE_INVALID
	ret
c_endp	_FLTUNS@

; Convert float to int.

func	_FLTINT@
	push	EDX
	mov	EDX,14
	test	EAX,sgn
	je	fltIntgl

	inc	EDX
	jmps	fltIntgl
c_endp	_FLTINT@


; Convert float to unsigned long.

func	_FLTUNLNG@
	push	EDX
	mov	EDX,31
	test	EAX,sgn
	jz	fltIntgl

	feexcept FE_INVALID
	ret
c_endp	_FLTUNLNG@

; Convert float to long

func	_FLTLNG@
	push	EDX
	mov	EDX,30
	test	EAX,sgn
	jz	fltIntgl

	inc	EDX

fltIntgl:
	_push	<EBX,ECX,ESI,EDI>
	call	fget_dtype
	jmp	dword ptr cs:fltIntglIndex[ESI*4]

fltIntglIndex label	  word
	dd	fltIntglNormal		  ;other
	dd	fltIntglZero		  ;zero
	dd	fltIntglInvalid		  ;infinite
	dd	fltIntglInvalid		  ;SNaN
	dd	fltIntglInvalid		  ;QNaN

fltIntglNormal:
	call	funnorm		    ;unpack double
	clr	EBX
	sub	ESI,shortbias	    ;un-bias the exponent
	js	fltIntgl4	    ;for neg exponents, the result is 0
	_ifs	ESI a EDX, fltIntglInvalid
	mov	ECX,31
	sub	ECX,ESI
	jcxz	fltIntgl2

	_align
fltIntgl3:
	shr	EAX,1
	rcr	BH,1		    ;keep stick bit
	adc	BL,0		    ;keep gaurd bit
	loop	fltIntgl3

fltIntgl2:
	tst	BX
	je	fltIntgl6
	feexcept FE_INEXACT	;no longer exact
fltIntgl6:
	test	EDI,sgn		    ;is result negative?
	jns	fltIntglDone	    ;no
	neg	EAX		    ;yes
	jmps	fltIntglDone

fltIntgl4:
	mov	BL,1		;save stick bits
	cmp	ESI,-1		;is guard bit needed
	je	fltIntgl5
	or	BH,80h		;make guard bit into sticky bit
fltIntgl5:
	clr	EAX
	jmps	fltIntgl2
fltIntglInexact:
	feexcept FE_INEXACT	;no longer exact
fltIntglZero:
	clr	EAX		 ;result is 0
	jmps   fltIntglDone

fltIntglInvalid:
	feexcept FE_INVALID

fltIntglDone:
	_pop	<EDI,ESI,ECX,EBX,EDX>
	ret
c_endp	_FLTLNG@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert unsigned to float

func	_UNSFLT@
	_push	<EBX,ECX,EDX>
	callm	_UNSDBL@
	jmp	CX1
c_endp	_UNSFLT@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert long to float

__LNGFLT@:
	_push	<EBX,ECX,EDX>
	callm	_LNGDBL@
	jmp	CX1
	endcode double

	end

