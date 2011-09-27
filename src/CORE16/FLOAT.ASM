;_float.asm    Mar 11 1991   Modified by: Phil Hinger */
;$Header: z:/core/crtl/src/core16/float.asm,v 1.1.1.1 1997/01/02 19:17:35 smarx Exp $
;Copyright (C) 1984-1988 by Walter Bright
;All Rights Reserved, written by Walter Bright
;Floating point package

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

    if LCODE
	extrn		__FDIVP:far
    else
	extrn		__FDIVP:near
    endif

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
	c_public	_INTFLT@,_UNSFLT@,_LNGFLT@,_FLTINT@,_FLTUNS@,_FLTLNG@

      if LCODE
	c_extrn		_DBLINT@,far,_INTDBL@,far,_DBLUNS@,far,_UNSDBL@,far,_DBLFLT@,far
	c_extrn		_DBLLNG@,far,_LNGDBL@,far,_DBLULNG@,far,_ULNGDBL@,far
      else
	c_extrn		_DBLINT@,near,_INTDBL@,near,_DBLUNS@,near,_UNSDBL@,near,_DBLFLT@,near
	c_extrn		_DBLLNG@,near,_LNGDBL@,near,_DBLULNG@,near,_ULNGDBL@,near
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Put into DI the OR of all the significand bits

orsigc	proc	near
	mov	DI,CX
	and	DI,7fh
	or	DI,BX
	ret
orsigc	endp

orsigb	proc	near
	mov	DI,DX
	and	DI,7fh
	or	DI,AX
	ret
orsigb	endp

;;;;;;;;;;;;;;;;;;;;;;;;;;
; Test and see if [DX,AX] is 0

	even
func	_FTST@
	push	DX
	shl	DX,1		;dump sign bit
	or	DX,AX
	pop	DX
	ret
c_endp	_FTST@

;;;;;;;;;;;;;;;;;;;;;;;;;;
; Test and see if [DX,AX] is 0
;	a = b ? 0
; Input:
;	[DX,AX] = b
; Output:
;	[DX,AX] = b
; _FTST0EXC@ is same as _FTST0@, but set invalid exception flag if
; the operand is a NAN.

func	_FTST0EXC@
	stc
	jmp	short DT9
c_endp	_FTST0EXC@

	even
func	_FTST0@
if 1
	clc
DT9:	push	SI
	sbb	SI,SI			;SI==-1 if raise exception
	push	DI
	push	AX
	push	DX

	;Test if b is a special value
	mov	DI,DX
	and	DI,shortexp
	jz	DT1			   ;c is 0 or subnormal
	_if	DI e shortexp, DT2	   ;c is nan or infinity

	;Do a straightforward comparison
DT3:	tst	DX
	jz	DT8
	mov	AH,CCgt
	jg	DTret
	mov	AH,CClt
DTret:	;or	AX,AX			;OF (overflow flag) is already clear
	sahf
	_pop	<DX,AX,DI,SI>
	ret

DT8:	or	DX,AX
	jne	DT7

DTeq:	mov	AH,CCeq
	jmp	DTret

DT7:	mov	AH,CCgt
	ja	DTret
	mov	AH,CClt
	jmp	DTret

DT1:	call	orsigb
	jnz	DT3			;subnormal
	jmp	DTeq			;b is 0

DT2:	call	orsigb
	jz	DT3			;b is infinity

	;b is a NAN
	mov	AH,CCinv
	and	SI,FE_INVALID
	feexcept SI
	jmp	DTret
else
	push	DX
	tst	DX
	js	TST01
	or	DX,AX
	neg	DX
	sbb	DX,DX
	neg	DX
	pop	DX
	ret

TST01:
	shl	DX,1
	or	DX,AX
	neg	DX			;C=1 if b!=0
	sbb	DX,DX			;if (b==0) DX=0 else DX=-1
	pop	DX
	ret

endif
c_endp	_FTST0@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Float compare against 0, setting sign bits appropriately
;	a = b ? c
; Input:
;	DX,AX = b
;	CX,BX = c
; Output:
;	no registers changed
;	Condition codes set same as 8087 would
;	(but also set SF and OF so that pre-3.0 code will link which
;	used signed jmps after FCMP)
;
; _FCMPEXC@ is same as _FCMP@, but set invalid exception flag if
; either of the operands are NAN.

if 1
	even
func	_FCMPEXC@
	stc
	jmp	short DC9
c_endp	_FCMPEXC@

	even
func	_FCMP@
	clc
DC9:	push	BP
	mov	BP,SP
	push	SI
	sbb	SI,SI			;SI==-1 if raise exception
	push	DI
	push	DX
	push	CX
	push	AX

	;Test if c is a special value
	mov	DI,CX
	and	DI,shortexp
	jz	DC1			 ;c is 0 or subnormal
	_if	DI e shortexp, DC2     ;c is nan or infinity

	;Test if b is a special value
DC3:	mov	DI,DX
	and	DI,shortexp
	jz	DC4			;b is 0 or subnormal
	_if	DI e shortexp, DC5     ;b is nan or infinity

	;Do a straightforward comparison
DC6:	mov	DI,CX
	xor	DI,DX
	js	DC8			;signs are different
	_if	 DX ne CX, DC7
	_if	 AX ne BX, DC7

DCeq:	mov	AH,CCeq
DCret:	or	AX,AX			;clear OF (overflow flag)
	sahf
	pop	AX
	pop	CX
	pop	DX
	pop	DI
	pop	SI
	pop	BP
	ret    ; 8

DC7:
	mov	AH,CCgt
	ja	DC10
	mov	AH,CClt
DC10:	test	CX,sgn
	jns	DCret
	xor	AH,CCgt XOR CClt
	jmp	DCret

DC8:	test	CX,sgn
	mov	AH,CClt
	jns	DCret
	mov	AH,CCgt
	jmp	DCret

DC1:	call	orsigc
	jnz	DC3			;subnormal
	;c is +0 or -0
	and	CX,sgn-1		;no -0 bugs
	jmp	DC3			;c is 0

DC2:	call	orsigc
	jz	DC3			;c is infinity
	jmp	short DCinv		;c is a nan

DC4:	call	orsigb
	jnz	DC6			;b is subnormal
	;c is +0 or -0
	and	DX,sgn-1		;no -0 bugs
	jmp	DC6			;b is 0

DC5:	call	orsigb
	jz	DC6			;b is infinity
;	jmp	DCinv			;b is a nan

DCinv:	mov	AH,CCinv
	and	SI,FE_INVALID
	feexcept SI
	jmp	DCret
c_endp	_FCMP@

else

func	_FCMP@
	push	DI
	push	DX
	push	CX

	;test if c is 0
	mov	DI,CX
	shl	DI,1			;dump sign bit
	or		DI,BX
	jnz	C3			;no
	and	CH,7Fh			;no -0 bugs
C3:
	mov	DI,DX
	shl	DI,1			;dump sign bit
	or	DI,AX
	jnz	C2			;no
	and	DH,7Fh			;convert -0 to 0
C2:
	mov	DI,DX
	xor	DI,CX
	js	C52			;signs are different
	mov	DI,1			;1 for positive compares
	tst	CX
	jns	C51
	neg	DI			;-1 for negative compares
C51:	_if	DX ne CX, C6		;compare MSW
	_if	AX e BX, L21
C6:	ja	C7
	neg	DI
C7:	tst	DI
L21:	pop	CX
	pop	DX
	pop	DI
	ret

C52:	cmp	DX,CX
	jmp	L21
c_endp	_FCMP@

endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  floating add/subtract.
;	a = b +(-) c
; Input:
;	[CX,BX] = b
;	[DX,AX] = c
; Output:
;	a = [DX,AX]
;	SI,DI = preserved
; Stack offsets


	padnn	 =	22	;so nn == dd == mm == 40
	sign	 =	padnn+8
	signc	 =	sign+2
	subtract =	signc+2
	exp	 =	subtract+2
	orgsign	 =	exp+2
	nn	 =	orgsign+2
	b	 =	nn+P

__FSUB@:
	push	BP
	sub	SP,nn		;make room for nn variables
	mov	BP,SP
	_if	__8087 e 0, A6		;if no 8087
	_push	<DX,AX>			;push b
	fld	dword ptr -4[BP]	;load b into 8087
	_push	<CX,BX>			;push c
	fsub	dword ptr -8[BP]	;sub c
	jmps	fltret

A6:
	mov	orgsign[BP],8000h	;need to flip sign back on  NaNs
	xor	CH,80h			;flip sign for subtraction
	jmps	A1

	even
func	_FADD@
	push	BP
	sub	SP,nn			;make room for nn variables
	mov	BP,SP
	mov	word ptr orgsign[BP],0	;need for NaN
	_if	__8087 e 0, A1		;if no 8087
	_push	<DX,AX>			;push b
	fld	dword ptr -4[BP]	;load b into 8087
	_push	<CX,BX>			;push c
	fadd	dword ptr -8[BP]	;add c

fltret:
	;Check for floating point error
	fstsw	-2[BP]
	fwait
	mov	AX,-2[BP]
	and	AX,FE_ALL_EXCEPT
	jnz	fltret5			;jmp if error
fltret4:
	
	fstp	dword ptr -8[BP]
	fwait
	_pop	<AX,DX>
	_pop	<BX,CX>			;pop result
	add	SP,nn			;subtract c off also
	pop	BP
	ret
;	 ret	 8

fltret5:
	feexcept AX
	jmp	fltret4


A1:
	push	DI
	push	SI
	call	fget_dtype_pair
	jmp	word ptr cs:daddindex[SI]

	even
Daddindex	label	word
	dw	ANormalAdd		;other + other
	dw	AFirstIsAnswer		;other + zero
	dw	ASecondIsAnswer		;other + infinite
	dw	ASecondAsQNaN		;other + SNaN
	dw	ASecondQNaN		;other + QNaN

	dw	ASecondIsAnswer		;zero + other
	dw	Azeros			;zero + zero
	dw	ASecondIsAnswer		;zero + infinite
	dw	ASecondAsQNaN		;zero + SNaN
	dw	ASecondQNaN		;zero + QNaN

	dw	AFirstIsAnswer		;infinite + other
	dw	AFirstIsAnswer		;infinite + zero
	dw	AInfiniteInfinite	;infinite + infinite
	dw	ASecondAsQNaN		;infinite + SNaN
	dw	ASecondQNaN		;infinite + QNaN

	dw	AFirstAsQnan		;SNaN + other
	dw	AFirstAsQnan		;SNaN + zero
	dw	AFirstAsQnan		;SNaN + infinite
	dw	ALargestSNaNasQNaN	;SNaN + SNaN
	dw	ASecondAsQNaN		;SNaN + QNaN

	dw	AFirstQNaN			;QNaN + other
	dw	AFirstQNaN			;QNaN + zero
	dw	AFirstQNaN		;QNaN + infinite
	dw	AFirstAsQNaN		;QNaN + SNaN
	dw	ALargestQNaN		;QNaN + QNaN

ANormalAdd:

	mov	signc[BP],DX	;Save sign
	mov	DI,CX
	mov	sign[BP],DI
	xor	DI,DX		;if sign(b) != sign(c), then subtraction
	mov	subtract[BP],DI	;flag for subtraction

	call	funnorm		;unpack second operand (c)
	mov	exp[BP],SI	;save exponent of c
	xchg32reg		;exchange regs
	call	funnorm		;unpack first operand (b)
	sub	SI,exp[BP]	;SI = exp(b) - exp(c)
	jle	A2		;exp(b) > exp(c)
	add	exp[BP],SI	;exponent is b
	xchg32reg
	neg	SI
	mov	DI,signc[BP]
	mov	sign[BP],DI		;reset sign
A2:
	call	fright_justify
	test	byte ptr subtract+1[BP],80h	;subtracting? (test bit 15)
	je	A3			;no

	sub	AX,BX
	sbb	DX,CX

	jnc	A4				;no borrow
	xor	byte ptr sign+1[BP],80h		;toggle sign of result
	neg32	DX,AX				;SI must be 0 for this to work

A4:
	test	DH,0C0h			;if bit 8 or 7 are set then
	jnz	A5			;do normal round
	mov	DI,DX
	or	DI,AX			;is result zero
	je	Azeros1			;yes

	mov	DI,sign[BP]
	mov	SI,exp[BP]		;exponent of result
	call	fnorm			;normalize and pack
	jmp	Adone

A3:
	add	AX,BX
	adc	DX,CX
	jnc	A5
	rcr	DX,1
	rcr	AX,1
	inc	word ptr exp[BP]	;bump exponent

A5:
	mov	DI,sign[BP]
	mov	SI,exp[BP]		;exponent of result
	call	fround			;round and normalize
	jmp	Adone


Azeros:
	mov	DI,DX
	xor	DI,CX
	test	DI,8000h		;are signs the same
	jne	Azeros1
	jmp	ADone			;yes

Azeros1:
	clr	DX
    ifdef _MT
	push	AX
	feround
	cmp	AX,FE_DOWNWARD
	pop	AX
	je	Azeros2
    else
	_if	__fe_cur_env.round e FE_DOWNWARD,Azeros2
    endif
	jmp	AFirstIsAnswer
Azeros2:
	mov	DH,80h			;set sign to -
	jmps	AFirstIsAnswer
	
AInfiniteInfinite:
	mov	DI,DX
	xor	DI,CX
	test	DI,8000h		;are signs the same
	je	AFirstIsAnswer		;yes

					;Default invalid operation
	mov	DX,short_qnan OR 1
	clr	AX
	feexcept FE_INVALID
	jmps	ADone


ASecondAsQNaN:
	or	CX,fqnan_bit
	feexcept FE_INVALID
	jmps	ASecondQNaN


ALargestSNaNasQNaN:
	or	DX,fqnan_bit
	or	CX,fqnan_bit
	feexcept FE_INVALID

ALargestQNaN:
	_if	DL a CL, AFirstIsAnswer
	jb	ASecondQNaN
	_if	AX ae BX,   AFirstIsAnswer

ASecondQNaN:
	
	xor	CX,orgsign[BP]		;reset original sign
ASecondIsAnswer:
	mov	DX,CX
	mov	AX,BX
	jmps	ADone

AFirstAsQNAN:
	or	DX,fqnan_bit
	feexcept FE_INVALID

AFirstQNaN:

AFirstIsAnswer:

Adone:
	pop	SI
	pop	DI
	add	SP,nn
	pop	BP
	ret
;	ret	8
c_endp	_FADD@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Floating divide.
;	a = b / c
; Input:
;	[BX,CX] = b
;	[AX,DX] = c
; Output:
;	a = [AX,DX]
;	SI,DI preserved
; Stack offsets:

	paddd	=	16		;so nn == cxdd == mm == 38
	resp	=	paddd+16	;pointer to result
	sign	=	resp+2		;sign of result
	exp	=	sign+2		;exponent of result
	count	=	exp+2		;loop counter
	cxdd	=	count+2		;amount of local variables

func	_FDIV@
	push	BP
	sub	SP,cxdd
	mov	BP,SP

	_if	__8087 e 0, D7		;if no 8087
	_push	<DX,AX>			;push b
	fld	dword ptr -4[BP]	;load b into 8087
	_push	<CX,BX>			;push c

	; FDIV bug workaround
	_if	_fdiv_chk_flag e 1, D20	;if FDIV is ok
	fld	dword ptr -8[BP]
	call	__FDIVP			;workaround for FDIV bug
	jmp	fltret
D20:
	fdiv	dword ptr -8[BP]	;div c
	jmp	fltret

D7:	push	SI
	push	DI

	mov	sign[BP],DX		;transfer sig(b) to 0[BP]
	xor	sign[BP],CX
	and	sign[BP],sgn
	call	fget_dtype_pair
	jmp	word ptr cs:Dindex[SI]

	even
Dindex	label	word
	dw	DNormalDivide		;other / other
	dw	DDivideByZero		;other / zero
	dw	DSignedZero		;other / infinite
	dw	DSecondAsQNaN		;other / SNaN
	dw	DSecondQNaN		;other / QNaN

	dw	DSignedZero		;zero / other
	dw	DDefaultQNaN		;zero / zero
	dw	DSignedZero		;zero / infinite
	dw	DSecondAsQNaN		;zero / SNaN
	dw	DSecondQNaN		;zero / QNaN

	dw	DSignedInfinite		;infinite / other
	dw	DSignedInfinite		;infinite / zero
	dw	DDefaultQNaN		;infinite / infinite
	dw	DSecondAsQNaN		;infinite / SNaN
	dw	DSecondQNaN		;infinite / QNaN

	dw	DFirstAsQnan		;SNaN / other
	dw	DFirstAsQnan		;SNaN / zero
	dw	DFirstAsQnan		;SNaN / infinite
	dw	DLargestSNaNasQNaN	;SNaN / SNaN
	dw	DSecondAsQNaN		;SNaN / QNaN

	dw	DFirstQNaN		;QNaN / other
	dw	DFirstQNaN		;QNaN / zero
	dw	DFirstQNaN		;QNaN / infinite
	dw	DFirstAsQNaN		;QNaN / SNaN
	dw	DLargestQNaN		;QNaN / QNaN


DNormalDivide:
	;unpack c
	mov	DI,CX
	and	DI,shortexp		;mask off exponent bits
	clr	CH			;remove exponent from mantissa
	tst	DI			;is exponent zero
	jnz	D12
					;special case when exponent is zero
	xchg32reg			;may need to do a lot of shifting
	call	fleft_justify		;msb must be block to left
	xchg32reg
	mov	DI,SI			;save exponent
	jmps	D13
D12:
	or	CL,shorthid
	shr	DI,1
	shr	DI,1
	shr	DI,1
	shr	DI,1
	shr	DI,1
	shr	DI,1
	shr	DI,1
D13:
	
	;unpack b
	mov	SI,DX
	and	SI,shortexp		;mask off exponent bits
	clr	DH
	tst	SI			;is exponent zero
	jnz	D14
					;special case when exponent is zero
	call	fleft_justify		;msb must be block to left
	jmps	D15
D14:
	or	DL,shorthid		;or in hidden bit
	shr	SI,1
	shr	SI,1
	shr	SI,1
	shr	SI,1
	shr	SI,1
	shr	SI,1
	shr	SI,1			;right justify exponent
D15:
	sub	SI,DI			;exp(result) = exp(b) - exp(c)
	add	SI,shortbias		;so bias is retained after subtraction
	mov	exp[BP],SI		;exponent of result

	mov	SI,CX			;free up CX for loop counter
;;;;;;;
	;if (b >= c) goto D31 else D41
	mov	CX,26			;16 bits per word
	mov	DI,1			;used to count to 16
	
	even
D51:	_if	DX a SI, D31
	jb	D41
	_if	AX b BX, D41

	;b -= c

D31:	sub	AX,BX
	sbb	DX,SI			;since b > c, C == 0

D41:	rcl	DI,1			;0 if we subtracted, 1 if not
	jc	D71			;push value every 16 loops

D61:	shl32	DX,AX			;b <<= 1
	loop	D51

	or	AX,DX
	or	AL,AH			;set AL to sticky bits
	je	D63
	mov	AL,1
D63:
	and	AX,1
	mov	DX,DI
	not	DX
	shl	DX,1			;shift out count
	shl	DX,1
	shl	DX,1
	shl	DX,1
	shl	DX,1
	shl	DX,1
	or	AX,DX
;;;;;;;

	pop	DX			;load sig(result)
	mov	SI,exp[BP]
	mov	DI,sign[BP]
	call	fround			;round and normalize result
	jmp	DDone


D71:	not	DI			;push next mantissa on stack
	push	DI
	clr	DI
	jmps	D61

DDivideByZero:
	feexcept FE_DIVBYZERO

DSignedInfinite:
	mov	DX,sign[BP]
	or	DX,short_infinity
	clr	AX
	jmps	DDone

DSignedZero:
	mov	DX,sign[BP]
	clr	AX
	jmps	DDone

DSecondAsQNaN:
	or	CX,fqnan_bit
	feexcept FE_INVALID

DSecondQNaN:
	xchg32reg
	jmps	DDone

	even
DDefaultQNaN:
	mov	DX,short_qnan OR 1
	or	DX,sign[BP]
	feexcept FE_INVALID
	jmps	DDone


DLargestSNaNasQNaN:
	or	DX,fqnan_bit
	or	CX,fqnan_bit
	feexcept FE_INVALID

DLargestQNaN:
	_if	DL a CL, DFirstQNaN
	jb	DSecondQNaN
	_if	AX ae BX, DFirstQNaN
	jmps	DSecondQNaN

DFirstAsQNAN:
	or	DX,fqnan_bit
	feexcept FE_INVALID
DFirstQNaN:

DDone:
	pop	DI
	pop	SI
	add	SP,cxdd
	pop	BP
;	ret	8
	ret
c_endp	_FDIV@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Floating Multiply
;	a = b * c
; Input:
;	[CX,BX] = b
;	[DX,AX] = c
; Output:
;	a = [DX,AX]
;	SI,DI preserved
; Stack offsets:

	padmm	=	24		;so nn == cxdd == mm == 40
	resp	=	padmm+8		;pointer to result
	sign	=	resp+2		;sign of result
	exp	=	sign+2		;exponent of result
	count	=	exp+2		;loop counter
	mm	=	count+2		;amount of local variables

func	_FMUL@
	push	BP
	sub	SP,mm
	mov	BP,SP

	_if	__8087 e 0, M1		;if no 8087
	_push	<DX,AX>			;push b
	fld	dword ptr -4[BP]	;load b into 8087
	_push	<CX,BX>			;push c
	fmul	dword ptr -8[BP]	;mul c
	jmp	fltret

M1:	push	SI
	push	DI

	mov	sign[BP],DX			;transfer sig(b) to 0[BP]
	xor	sign[BP],CX
	and	sign[BP],sgn
	call	fget_dtype_pair
	jmp	word ptr cs:Mindex[SI]

	even
Mindex	label	word
	dw	MNormalMultiply		;other * other
	dw	MSignedZero		;other * zero
	dw	MSignedInfinite		;other * infinite
	dw	MSecondAsQNaN		;other * SNaN
	dw	MSecondQNaN		;other * QNaN

	dw	MSignedZero			;zero * other
	dw	MSignedZero		;zero * zero
	dw	MDefaultQNaN		;zero * infinite
	dw	MSecondAsQNaN		;zero * SNaN
	dw	MSecondQNaN		;zero * QNaN

	dw	MSignedInfinite		;infinite * other
	dw	MDefaultQNaN		;infinite * zero
	dw	MSignedInfinite		;infinite * infinite
	dw	MSecondAsQNaN		;infinite * SNaN
	dw	MSecondQNaN		;infinite * QNaN

	dw	MFirstAsQnan		;SNaN * other
	dw	MFirstAsQnan		;SNaN * zero
	dw	MFirstAsQnan		;SNaN * infinite
	dw	MLargestSNaNasQNaN	;SNaN * SNaN
	dw	MSecondAsQNaN		;SNaN * QNaN

	dw	MFirstQNaN			;QNaN * other
	dw	MFirstQNaN			;QNaN * zero
	dw	MFirstQNaN		;QNaN * infinite
	dw	MFirstAsQNaN		;QNaN * SNaN
	dw	MLargestQNaN		;QNaN * QNaN


MNormalMultiply:
	call	funnorm		;unpack second operand (c)
	mov	exp[BP],SI	;save exponent of c
	xchg32reg		;may need to do a lot of shifting
	call	funnorm		;unpack second operand (b)
	sub	SI,shortbias - 1;so bias is retained after add
	add	exp[BP],SI	;exponent of result

	mov	SI,DX		; save b is [SI,DI]
	mov	DI,AX

				;b_low * c_low
	mul	BX
	mov	[BP],AX
	mov	2[BP],DX

				;b_high * c_low
	mov	AX,SI
	mul	BX
	add	2[BP],AX
	adc	DX,0
	mov	BX,DX		;BX now free
	
				;b_low * c_high
	mov	AX,DI
	mul	CX
	clr	DI		;DI now free
	add	2[BP],AX
	adc	BX,DX
	adc	DI,0		;save overflow bit

				;b_high * c_high
	mov	AX,SI
	mul	CX
	add	AX,BX
	adc	DX,DI

	mov	CX,[BP]
	or	CX,2[BP]
	je	M2
	or	AX,1		;save sticky bits
M2:
	mov	SI,exp[BP]
	mov	DI,sign[BP]
	call	fround		;round and normalize result
	jmp	MDone

MSignedInfinite:
	mov	DX,sign[BP]
	or	DX,short_infinity
	clr	AX
	jmps	MDone

MSignedZero:
	mov	DX,sign[BP]
	clr	AX
	jmps	MDone

MSecondAsQNaN:
	or	CX,fqnan_bit
	feexcept FE_INVALID

MSecondQNaN:
	xchg32reg
	jmps	MDone

	even
MDefaultQNaN:
	mov	DX,short_qnan
	or	DX,sign[BP]
	feexcept FE_INVALID
	jmps	MDone


MLargestSNaNasQNaN:
	or	DX,fqnan_bit
	or	CX,fqnan_bit
	feexcept FE_INVALID

MLargestQNaN:
	_if	DL a CL, MFirstQNaN
	jb	MSecondQNaN
	_if	AX ae BX, MFirstQNaN
	jmps	MSecondQNaN

MFirstAsQNAN:
	or	DX,fqnan_bit
	feexcept FE_INVALID
MFirstQNaN:

MDone:
	pop	DI
	pop	SI
	add	SP,mm
	pop	BP
;	ret	8
	ret
c_endp	_FMUL@
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
func	_FNEG@
	xor	DH,80h
	ret
c_endp	_FNEG@
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert int to float

func	_INTFLT@
	_push	<BX,CX>
	callm	_INTDBL@
CX1:	callm	_DBLFLT@
CX2:	_pop	<CX,BX>
	ret
c_endp	_INTFLT@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert unsigned to float

func	_UNSFLT@
	_push	<BX,CX>
	callm	_UNSDBL@
	jmp	CX1
c_endp	_UNSFLT@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert long to float

__LNGFLT@:
	_push	<BX,CX>
	callm	_LNGDBL@
	jmp	CX1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert fload to unsigned.
      MinBitShift =  0
       stackSize  =  MinBitShift + 2

	even
func	_FLTUNS@
	push	BP
	sub	SP,stackSize
	mov	BP,SP
	mov	word ptr MinBitShift[BP],15
	test	DH,80h
	jz	fltIntgl

	feexcept FE_INVALID
	add	SP,stackSize
	pop	BP
	ret
c_endp	_FLTUNS@

; Convert float to int.

func	_FLTINT@
	push	BP
	sub	SP,stackSize
	mov	BP,SP
	mov	word ptr MinBitShift[BP],14
	test	DH,80h
	je	fltIntgl

	inc	word ptr MinBitShift[BP]
	jmps	fltIntgl

c_endp	_FLTINT@


; Convert float to unsigned long.

func	_FLTULNG@
	push	BP
	sub	SP,stackSize
	mov	BP,SP
	mov	word ptr MinBitShift[BP],31
	test	DH,80h
	jz	fltIntgl

	feexcept FE_INVALID
	add	SP,stackSize
	pop	BP
	ret
c_endp	_FLTULNG@

; Convert float to long

func	_FLTLNG@
	push	BP
	sub	SP,stackSize
	mov	BP,SP
	mov	word ptr MinBitShift[BP],30
	test	DH,80h
	jz	fltIntgl

	inc	word ptr MinBitShift[BP]

fltIntgl:
	_push	<SI,DI>
	call	fget_dtype
	shl	SI,1
	jmp	word ptr cs:fltIntglIndex[SI]

fltIntglIndex label	  word
	dw	fltIntglNormal		  ;other
	dw	fltIntglZero		  ;zero
	dw	fltIntglInvalid		  ;infinite
	dw	fltIntglInvalid		  ;SNaN
	dw	fltIntglInvalid		  ;QNaN

fltIntglNormal:
	call	funnorm		;unpack double
	clr	BX		;clear guard and sticky bits
	sub	SI,shortbias	;un-bias the exponent
	js	fltIntgl4	;for neg exponents, the result is 0
	_if	SI a MinBitShift[BP] fltIntglInvalid
	mov	CX,31
	sub	CX,SI
	jcxz	fltIntgl2

fltIntgl3:
	shr	DX,1
	rcr	AX,1
	rcr	BH,1		;keep sticky bit
	adc	BL,0		;keep guard bit
	loop	fltIntgl3

fltIntgl2:
	tst	BX
	je	fltIntgl6
	feexcept FE_INEXACT	;no longer exact
fltIntgl6:
	test	DI,8000h	;is result negative?
	jns	fltIntglDone	;no
	mov	SI,MinBitShift[BP]
	_if	SI e 31 fltIntgl7
	cmp	AX,8000h	;special case at most negative
	ja	fltIntglInvalid
	je	fltIntglDone

	neg	AX	     ;yes
	jmps	fltIntglDone

fltIntgl7:
	test	DX,8000h	;special case at mast negative
	jne	fltIntgl8

	neg32	DX,AX		;yes
	jmps	fltIntglDone

fltIntgl4:
	mov	BL,1
	cmp	SI,-1		;is guard bit needed
	je	fltIntgl5
	mov	BH,80h		;make guard bit into sticky bit
fltIntgl5:
	clr	AX
	mov	DX,AX
	jmps	fltIntgl2
fltIntglInexact:
	feexcept FE_INEXACT	;no longer exact
fltIntglZero:
	clr	AX		;result is 0
	cwd
	jmps   fltIntglDone

fltIntglInvalid:
	feexcept FE_INVALID

fltIntgldone:
	_pop	<DI,SI>
	add	SP,stackSize
	pop	BP
	ret
fltIntgl8:
	or	DX,AX		;special case when integer in max neg
	cmp	DX,8000h
	je	fltIntgldone
	jmp	fltIntglInvalid
c_endp	_FLTLNG@




	endcode	double

	end

