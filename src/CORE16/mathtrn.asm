;_ mathtrn.asm   Modified by: Phil Hinger */
; Written by Walter Bright
; Copyright (C) 1984-2001 by Walter Bright
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
;	c_extrn	_8087,word

    ifndef _MT
	extrn	__fe_cur_env:word
    endif

	enddata




	begcode double

	extrn	dunnorm:near, dround:near, dget_dtype:near
	extrn	dnorm:near,   dleft_justify:near, dget_dtype_pair:near

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	#include <math.h>
;	double frexp(double value,int *eptr)
; Returns:
;	x such that value=x*2**n, .5 <= |x| < 1.0
;	x has same sign as value.
;	*eptr = n
;
;	Special cases:
;		value	  x	*eptr
;		+-0.0	+-0.0	  0
;		+-inf	+-inf	  *
;		+-NaN	+-NaN	  *
;		+-NaNs	+-NaN	  *
;
;	* Indeterminate
;	No exceptions are raised, even if value is a signaling NaN.

	c_public frexp

func	frexp
	WINENTER
  if 0	;Using the x87 is a problem because FXTRACT returns a value
	;between 1 and 2 instead of .5 to 1, also the different x87's
	;have different behavior on special values, and set the
	;exception flags strangely.

	_if	_8087 e 0, FR4		;if no 8087
	fld	qword ptr P[BP]
	fxtract
	fstp	qword ptr P[BP]
    if SPTR
	mov	BX,P+8[BP]
	fistp	word ptr [BX]		;store *eptr
    else ;LPTR
	les	BX,P+8[BP]
	fistp	word ptr ES:[BX]
    endif
	mov	AX,P+6[BP]
	mov	CX,P+2[BP]
	mov	DX,P+0[BP]
	fwait
    if SPTR
	_if	<word ptr [BX]> e 08000h, FR5		;check for -infinity
    else
	_if	<word ptr ES:[BX]> e 08000h, FR5	;check for -infinity
    endif
FR6:	mov	BX,P+4[BP]
	jmp	short FR0

	;The 80387 gives -infinity as a result if value is 0.0. To
	;conform to ANSI, set *eptr to 0. The 8087 and 80287 already yield 0.
FR5:
    if SPTR
	shl	word ptr [BX],1		;make it 0
    else
	shl	word ptr ES:[BX],1	;make it 0
    endif
	jmp	FR6
FR4:
  endif

	.save	<DI>
	mov	AX,P+6[BP]
	mov	BX,P+4[BP]
	mov	CX,P+2[BP]
	mov	DX,P+0[BP]

	mov	DI,AX
	and	DI,longexp		;isolate exponent
	jz	FRzeroexp		;zero exponent
	_if	DI e longexp, FR7	;if infinity or NaN
	shr	DI,1
	shr	DI,1
	shr	DI,1
	shr	DI,1			;right justify it
	sub	DI,longbias		;un-bias it
FR8:	and	AX,800Fh		;scrap exponent bits
;	push	AX
;	cbw				;clear sign bit
;	or	AX,BX
;	or	AX,CX
;	or	AX,DX			;see if 1
;	pop	AX
;	jz	FR3			;yes
	inc	DI
	or	AX,03FE0h		;to get .5 < |x| <= 1
;	jmps	FR9

;FR3:	or	AX,03FF0h		;to get .5 < |x| <= 1
FR9:	push	AX
	mov	AX,DI
    if LPTR
	les	DI,P+8[BP]		;ES:DI = eptr
	mov	ES:[DI],AX
    else
	mov	DI,P+8[BP]		;DI = eptr
	mov	[DI],AX
    endif
	pop	AX

FR2:	.restore <DI>
FR0:	WINLEAVE
	ret

FRzeroexp:
	;Determine if 0 or subnormal
	mov	DI,AX
	shl	DI,1
	or	DI,BX
	or	DI,CX
	or	DI,DX			;is value 0?
	jz	FR9			;yes (DI is 0)

	;It's subnormal
	;Left justify subnormal significand, decrementing exponent as we go
	mov	DI,1-longbias
FRS1:	dec	DI
	shl	DX,1
	rcl	CX,1
	rcl	BX,1
	rcl	AL,1
	test	AL,longhid
	je	FRS1
	jmp	FR8

	;value is +-INFINITY, or a NAN
FR7:	;(don't worry about *exp in this case)
	mov	DI,AX
	and	DI,0Fh			;isolate significand bits
	or	DI,BX
	or	DI,CX
	or	DI,DX
	jz	FR2			;it's +-INFINITY
	or	AL,8			;convert NANS to NANQ
	jmp	FR2

c_endp	frexp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; logb
	b	 =	P
c_public logb

func	logb
	push	BP
	mov	BP,SP
	_push	<SI,DI>
	mov	AX,b+6[BP]
	mov	BX,b+4[BP]
	mov	CX,b+2[BP]
	mov	DX,b+[BP]		;mov b into registers
	call	dget_dtype
	shl	SI,1
	jmp	word ptr cs:logbIndex[SI]

logbIndex label       word
	dw	logbNormal	      ;other
	dw	logbZero	      ;zero
	dw	logbInfinity	      ;infinite
	dw	logbSNaN	      ;SNaN
	dw	logbQNaN	      ;QNaN

logbNormal:
	mov	SI,AX
	and	SI,longexp		;mask off exponent bits
	jne	logb1			;special case when exponent is zero
	call	dleft_justify		;will figure out what exponent is
	jmps	logb2

logb1:
	shr	SI,1
	shr	SI,1
	shr	SI,1
	shr	SI,1			;right justify exponent

logb2:
	mov	AX,SI
	sub	AX,longbias
	or	AX,AX			;negative? (also clear C)
	jns	logb3			;no
	neg	AX			;abs value (also set C)

logb3:
	rcr	DI,1			;bit 15 becomes sign of result
	clr	DX
	mov	CX,DX
	mov	BX,DX			;rest of significand is 0
	tst	AX
	je	logbDone

	mov	SI,15+longbias		;2^15
	call	dnorm			;pack result into a double
	jmps	logbDone

logbZero:
	mov	AX,longexp OR sgn
	clr	BX
	mov	CX,BX
	mov	DX,BX
	feexcept FE_DIVBYZERO
	jmps	logbDone

logbInfinity:
	and	AX,sgn-1
	jmps	logbDone

logbSNaN:
	or	AX,dqnan_bit
	feexcept FE_INVALID
logbQNaN:
logbDone:
	_pop	<DI,SI>
	pop	BP
	ret
c_endp	logb
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; rint function returns the rounded integral value
;
;
	b	 =	P
c_public rint

func	rint
	push	BP
	mov	BP,SP
	_push	<SI,DI>
	mov	AX,b+6[BP]
	mov	BX,b+4[BP]
	mov	CX,b+2[BP]
	mov	DX,b+[BP]		;mov b into registers
	call	dget_dtype
	cmp	SI,dtype_other
	je	rint10

	cmp	SI,dtype_snan
	jne	rintdone

	or	AX,dqnan_bit
	feexcept FE_INVALID

rintdone:
	_pop	<DI,SI>
	pop	BP
	ret
rint10:
	mov	SI,AX
	and	SI,longexp
	cmp	SI,(longbias + 53) *16
	jae	rintDone
	mov	DI,AX
	and	DI,sgn
	cmp	SI,(longbias -1) *16
	jb	rint3

	shr	SI,1
	shr	SI,1
	shr	SI,1
	shr	SI,1		;right justify exponent

	sub	SI,longbias + 53
	not	SI
	and	AX,longhid-1
	or	AL,longhid

rint11:
	_if SI l 8, rint12
	rcr	AH,1		;make room for guard bit
	jnc	rint14		;make we didn't lose sticky
	or	AH,1
rint14:
	or	AH,DL		;capture stick and guard bits
	mov	DL,DH
	mov	DH,CL
	mov	CL,CH
	mov	CH,BL
	mov	BL,BH
	mov	BH,AL
	xor	AL,AL
	sub	SI,8
	jnz	rint11

rint12:
	xchg	SI,CX
	tst	CX
	je	rint13
rint1:
	shr64	AL,BX,SI,DX
	rcr	AH,1
	jnc	rint2
	or	AH,1
rint2:
	loop	rint1
rint13:
	mov	CX,SI
	jmps	rint4
rint3:
	clr	AX
	mov	BX,AX
	mov	CX,AX
	mov	DX,AX
	mov	AH,1
rint4:
	tst	AH
	je	rint5

	feexcept FE_INEXACT	;no longer exact
    ifdef _MT
	push	AX
	feround
	_if	AX e FE_TONEAREST, rint6
	_if	AX e FE_TOWARDZERO,rint16
	cmp	AX,FE_UPWARD
	pop	AX
	je	rint7
    else
	_if	__fe_cur_env.round e  FE_TONEAREST, rint6
	_if	__fe_cur_env.round e  FE_TOWARDZERO,rint9
	_if	__fe_cur_env.round e  FE_UPWARD,  rint7
    endif

	;FE_DOWNWARD
	test	DI,sgn
	jns	rint9
	jmps	rint8


rint7:;FE_UPWARD
	test	DI,sgn
	js	rint9
	jmps	rint8


rint6:;FE_TONEAREST
    ifdef _MT
	pop	AX
    endif
	test	AH,80h		;If guard bit is not set then
	jz	rint9		;no rounding is necessary
	test	AH,7fh
	jne	rint8		;Sticky bits found
	test	DL,1		;Integer is Odd
	je	rint9

rint8:;round up
	add	DX,1
	adc	CX,0
	adc	BX,0
	adc	AL,0
    ifdef _MT
	jmp	short rint9
rint16:	pop	AX
    endif

rint9:
	clr	AH

rint5:
	mov	SI,AX
	or	SI,BX
	or	SI,CX
	or	SI,DX
	je	rint15
	mov	SI,52 + 11 + longbias	    ;reset exponent
	call	dnorm
	jmp	rintdone
rint15:
	and	DI,sgn
	mov	AX,DI
	jmp	rintdone
c_endp	rint


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Return current rounding mode.
;	-1	indeterminate
;	0	toward 0
;	1	to nearest
;	2	toward +infinity
;	3	toward -infinity

frndtbl	db	1,3,2,0		;convert to ANSI C value

	c_public __flt_rounds

func	__flt_rounds
    ifdef _MT
	feround			;get rounding mode in AX
	xchg	AH,AL
	mov	BX,AX
	shr	BL,1
	shr	BL,1
    else
	mov	BL,byte ptr __fe_cur_env.control + 1
	shr	BL,1
	shr	BL,1
	and	BX,3
    endif
	mov	AL,CS:frndtbl[BX]
	cbw
	ret
c_endp	__flt_rounds

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Double floating Nextafter.
; Input:
;	x,y both on stack
; Output:
;	a = [AX,BX,CX,DX]
;	SI,DI = preserved
; Stack offsets
	floatBuffer =  8
	nxt	    =  floatBuffer
	b	    =	P + nxt
c_public nextafter

func	nextafter
	push	BP
	sub	SP,nxt
	mov	BP,SP
	mov	AX,b+14[BP]
	mov	BX,b+12[BP]
	mov	CX,b+10[BP]
	mov	DX,b+8[BP]		;mov b into registers
	regStk64			;transfer sig(b) to 0[BP]

	mov	AX,b+6[BP]
	mov	BX,b+4[BP]
	mov	CX,b+2[BP]
	mov	DX,b+[BP]	   ;mov b into registers
	_push	 <DI,SI>
	call	dget_dtype_pair
	jmp	word ptr cs:dnxtindex[SI]

Dnxtindex	label	word
	dw	NxtNormal		  ;other + other
	dw	NxtNormal		  ;other + zero
	dw	NxtNormal		  ;other + infinite
	dw	NxtSecondAsQNaN		  ;other + SNaN
	dw	NxtSecondQNaN		  ;other + QNaN

	dw	NxtFirstZero		  ;zero + other
	dw	Nxtzeros		  ;zero + zero
	dw	NxtFirstZero		  ;zero + infinite
	dw	NxtSecondAsQNaN		  ;zero + SNaN
	dw	NxtSecondQNaN		  ;zero + QNaN

	dw	NxtNormal		  ;infinite + other
	dw	NxtNormal		  ;infinite + zero
	dw	NxtNormal		  ;infinite + infinite
	dw	NxtSecondAsQNaN		  ;infinite + SNaN
	dw	NxtSecondQNaN		  ;infinite + QNaN

	dw	NxtFirstAsQnan		  ;SNaN + other
	dw	NxtFirstAsQnan		  ;SNaN + zero
	dw	NxtFirstAsQnan		  ;SNaN + infinite
	dw	NxtLargestSNaNasQNaN	  ;SNaN + SNaN
	dw	NxtSecondAsQNaN		  ;SNaN + QNaN

	dw	NxtFirstQNaN		  ;QNaN + other
	dw	NxtFirstQNaN		  ;QNaN + zero
	dw	NxtFirstQNaN		  ;QNaN + infinite
	dw	NxtFirstAsQNaN		  ;QNaN + SNaN
	dw	NxtLargestQNaN		  ;QNaN + QNaN

NxtNormal:
	mov	DI,6[BP]
	mov	SI,AX
	and	SI,longexp		    ;save exponent
	xor	DI,AX			    ;alwasy subtract if signs are
	js	NxtSub			    ;different
	_if	6[BP] ne AX, Nxt2
	_if	4[BP] ne BX, Nxt2
	_if	2[BP] ne CX, Nxt2
	_if	0[BP] ne DX, Nxt2
	jmp	NxtDone			    ;return x if equal

Nxt2:
	jb	NxtSub			    ;

NxtAdd:
	mov	DI,AX			    ;save sign
	and	AX,longhid-1		    ;clear exponent and sign
	tst	SI
	je	Nxt8
	or	AL,longhid		    ;no hidden bit on subnormals
Nxt8:
	add	DX,1
	adc	CX,0
	adc	BX,0
	adc	AL,0
	tst	SI			    ;is it subnormal
	je	Nxt9

	test	AX,longhid * 2		    ;did it add past hidden
	jz	Nxt3
Nxt10:
	add	SI,10h			    ;increment exponent (never will be infinte here)
	_if	SI l  7FF0h, Nxt3
	feexcept <FE_OVERFLOW or FE_INEXACT>
;	jmp	Nxt3
Nxt4:
	mov	AX,longhid		    ; same as shr64   AL,BX,CX,DX
	jmp	Nxt3

Nxt9:
	test	AX,longhid		    ;test if it is still subnormal
	jz	Nxt3
	jmps	Nxt10
NxtSub:
	mov	DI,AX			    ;save sign
	and	AX,longhid-1		    ;clear expont and sign
	tst	SI
	je	Nxt6			    ;subnormals don't need hidden bit
	or	AL,longhid
Nxt6:
	sub	DX,1
	sbb	CX,0
	sbb	BX,0
	sbb	AL,0			    ;did we lose hidden bit
	test	AL,longhid
	jne	Nxt3
	tst	SI
	je	Nxt3			    ;don't bother with subnormal

	sub	SI,10h
	shl64	AL,BX,CX,DX
	or	DL,1			    ;don't lose the last bit
Nxt3:
	and	AL,longhid - 1		    ;dump hidden bit
	or	AX,SI			    ;install exponent
	and	DI,sgn			    ;mask sign bit
	or	AX,DI			    ;install sign
Nxt7:
	test	AX,7ff0h		    ;check for subnormals
	jnz	NxtDone1

	feexcept <FE_UNDERFLOW or FE_INEXACT>
NxtDone1:
	jmps	NxtDone
	
NxtFirstZero:
	mov	AX,6[BP]
	and	AX,sgn
	clr	BX			    ;nextafter zero is the
	mov	CX,BX			    ;smallest subnormal with the
	mov	DX,1			    ;the sign of the direction
	feexcept <FE_UNDERFLOW or FE_INEXACT>
	jmps	NxtDone


NxtSecondAsQNaN:
	or	word ptr 6[BP],dqnan_bit
	feexcept FE_INVALID
	jmps	NxtSecondQNaN


NxtLargestSNaNasQNaN:
	or	 AX,dqnan_bit
	or	 word ptr 6[BP],dqnan_bit
	feexcept FE_INVALID

NxtLargestQNaN:
	_if	AL a 6[BP], NxtFirstIsAnswer
	jb	NxtSecondQNaN
	_if	BX a 4[BP], NxtFirstIsAnswer
	jb	NxtSecondQNaN
	_if	CX a 2[BP], NxtFirstIsAnswer
	jb	NxtSecondQNaN
	_if	DX ae [BP],   NxtFirstIsAnswer

NxtSecondQNaN:
	

NxtSecondIsAnswer:
	mov	AX,6[BP]
	mov	BX,4[BP]
	mov	CX,2[BP]
	mov	DX,[BP]
	jmps	NxtDone

NxtFirstAsQNAN:
	or	AX,dqnan_bit
	feexcept FE_INVALID

Nxtzeros:
NxtFirstQNaN:
NxtFirstIsAnswer:

Nxtdone:
	pop	SI
	pop	DI
	add	SP,nxt
	pop	BP
	ret
c_endp	nextAfter





	endcode double


	end
