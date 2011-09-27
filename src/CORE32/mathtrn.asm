;_ mathtrn.asm	May 8 1991   Modified by: Phil Hinger */
; Written by Walter Bright
; Copyright (C) 1984-2001 by Walter Bright
; All rights reserved

	include	macros.asm
	include flthead.asm

	.287

    ifdef _MT
	c_extrn	_FEEXCEPT,near
	c_extrn	_FEROUND,near
    endif

	begdata
;	extrn	__8087:word

    ifndef _MT
	extrn	__fe_cur_env:word
    endif

	enddata


	begcode double

	extrn	dunnorm:near, dround:near, dget_dtype:near, exception:near
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
	push	EBP
	mov	EBP,ESP
  if 0	;Using the x87 is a problem because FXTRACT returns a value
	;between 1 and 2 instead of .5 to 1, also the different x87's
	;have different behavior on special values, and set the
	;exception flags strangely.

	_ifs	__8087 e 0, FR4		;if no 8087
	fld	qword ptr P[EBP]
	fxtract
	fstp	qword ptr P[EBP]
    if SPTR
	mov	EDX,P+8[EBP]
	fistp	dword ptr [EDX]		;store *eptr
    else ;LPTR
	les	EDX,P+8[EBP]
	fistp	dword ptr ES:[EDX]
    endif
	mov	EAX,P+0[EBP]
	fwait
    if SPTR
	_ifs	<dword ptr [EDX]> e 080000000h, FR5	;check for -infinity
    else
	_ifs	<dword ptr ES:[EDX]> e 080000000h, FR5	;check for -infinity
    endif
FR6:	mov	EDX,P+4[EBP]
	pop	EBP
	_ret	8+4

	;The 80387 gives -infinity as a result if value is 0.0. To
	;conform to ANSI, set *eptr to 0. The 8087 and 80287 already yield 0.
FR5:
    if SPTR
	shl	dword ptr [EDX],1	;make it 0
    else
	shl	dword ptr ES:[EDX],1	;make it 0
    endif
	jmp	FR6
FR4:
  endif

	uses	<ECX,EDI>
	mov	EAX,P+0[EBP]
	mov	EDX,P+4[EBP]

	mov	EDI,EDX
	and	EDI,longexp
	jz	short FRzeroexp		;zero exponent
	_ifs	EDI e longexp, FR7	;if infinity or NaN
	shr	EDI,20			;right justify it
	sub	EDI,longbias		;un-bias it
FR8:	and	EDX,800FFFFFh		;scrap exponent bits
;	mov	ECX,EDX
;	and	ECX,000FFFFFh		;clear sign bit
;	or	ECX,EAX			;see if 1
;	mov	ECX,03FF00000h
;	jz	short FR3		;yes
	inc	EDI
	mov	ECX,03FE00000h		;to get .5 < |x| <= 1
FR3:	or	EDX,ECX			;to get .5 < |x| <= 1
FR9:
	mov	ECX,P+8[EBP]		;ECX = eptr
	mov	[ECX],EDI

FR2:	unuse	<EDI,ECX>
FR0:	pop	EBP
    if _RETST0
	;Return result in ST0
	push	EDX
	push	EAX
	fld	qword ptr [ESP]
	fwait
	add	ESP,8
    endif
	_ret	8+4

FRzeroexp:
	;Determine if 0 or subnormal
	lea	EDI,[EDX*2]
	or	EDI,EAX			;is value 0?
	jz	short FR9		;yes (DI is 0)

	;It's subnormal
	;Left justify subnormal significand, decrementing exponent as we go
	mov	EDI,1-longbias
	mov	ECX,EDX
	and	ECX,80000000h		;save original sign
FRS1:	dec	EDI
	shl	EAX,1
	rcl	EDX,1
	test	EDX,longhid
	je	short FRS1
	or	EDX,ECX			;restore sign bit
	jmp	FR8

	;value is +-INFINITY, or a NAN
FR7:	;(don't worry about *exp in this case)
	mov	EDI,EDX
	and	EDI,0FFFFFh		;isolate significand bits
	or	EDI,EAX
	jz	short FR2		;it's +-INFINITY
	or	EDX,80000h		;convert NANS to NANQ
	jmp	short FR2

c_endp	frexp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Double floating Nextafter.
; Input:
;	x,y both on stack
; Output:
;	a = [EDX,EAX]
;	ESI,EDI = preserved
; Stack offsets
       b = P

c_public nextafter

func	nextafter
	push	EBP
	mov	EBP,ESP
	_push	 <EBX,EDI,ESI>

	mov	ECX,P+12[EBP]
	mov	EBX,P+8[EBP]		  ;mov b into registers

	mov	EDX,P+4[EBP]
	mov	EAX,P[EBP]		   ;mov b into registers

	call	dget_dtype_pair
	jmp	dword ptr cs:dnxtindex[ESI]

dnxtindex	label	word
    if _FLAT
	dd	offset FLAT:NxtNormal		  ;other + other
	dd	offset FLAT:NxtNormal		  ;other + zero
	dd	offset FLAT:NxtNormal		  ;other + infinite
	dd	offset FLAT:NxtSecondAsQNaN	  ;other + SNaN
	dd	offset FLAT:NxtSecondQNaN	  ;other + QNaN

	dd	offset FLAT:NxtFirstZero	  ;zero + other
	dd	offset FLAT:Nxtzeros		  ;zero + zero
	dd	offset FLAT:NxtFirstZero	  ;zero + infinite
	dd	offset FLAT:NxtSecondAsQNaN	  ;zero + SNaN
	dd	offset FLAT:NxtSecondQNaN	  ;zero + QNaN

	dd	offset FLAT:NxtNormal		  ;infinite + other
	dd	offset FLAT:NxtNormal		  ;infinite + zero
	dd	offset FLAT:NxtNormal		  ;infinite + infinite
	dd	offset FLAT:NxtSecondAsQNaN	  ;infinite + SNaN
	dd	offset FLAT:NxtSecondQNaN	  ;infinite + QNaN

	dd	offset FLAT:NxtFirstAsQNaN	  ;SNaN + other
	dd	offset FLAT:NxtFirstAsQNaN	  ;SNaN + zero
	dd	offset FLAT:NxtFirstAsQNaN	  ;SNaN + infinite
	dd	offset FLAT:NxtLargestSNaNasQNaN  ;SNaN + SNaN
	dd	offset FLAT:NxtSecondAsQNaN	  ;SNaN + QNaN

	dd	offset FLAT:NxtFirstQNaN	  ;QNaN + other
	dd	offset FLAT:NxtFirstQNaN	  ;QNaN + zero
	dd	offset FLAT:NxtFirstQNaN	  ;QNaN + infinite
	dd	offset FLAT:NxtFirstAsQNaN	  ;QNaN + SNaN
	dd	offset FLAT:NxtLargestQNaN	  ;QNaN + QNaN
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
	mov	EDI,ECX
	mov	ESI,EDX
	and	ESI,longexp		  ;save exponent
	xor	EDI,EDX			  ;always subtract if signs are
	js	NxtSub			  ;different
	_ifs	ECX ne EDX, Nxt2
	_ifs	EBX ne EAX, Nxt2
	jmp	NxtDone			    ;return x if equal

Nxt2:
	jb	NxtSub			    ;

NxtAdd:
	mov	EDI,EDX			  ;save sign
	and	EDX,mantisa_mask	  ;clear exponent and sign
	tst	ESI
	je	Nxt8
	or	EDX,longhid		  ;no hidden bit on subnormals
Nxt8:
	add	EAX,1
	adc	EDX,0
	tst	ESI			 ;is it subnormal
	je	Nxt9

	test	EDX,00200000h		;did it add past hidden
	jz	Nxt3
Nxt10:
	add	ESI,longhid		 ;increment exponent (never will be infinte here)
	_ifs	ESI l  7FF00000h, Nxt3
	feexcept <FE_OVERFLOW or FE_INEXACT>
;	jmp	Nxt3
Nxt4:
	mov	EDX,longhid		     ; same as shr64   AL,BX,CX,DX
	jmp	Nxt3

Nxt9:
	test	EDX,longhid		     ;test if it is still subnormal
	jz	Nxt3
	jmps	Nxt10
NxtSub:
	mov	EDI,EDX			      ;save sign
	and	EDX,mantisa_mask	      ;clear exponent and sign
	tst	ESI
	je	Nxt6			    ;subnormals don't need hidden bit
	or	EDX,longhid
Nxt6:
	sub	EAX,1
	sbb	EDX,0			     ;did we lose hidden bit
	test	EDX,longhid
	jne	Nxt3
	tst	ESI
	je	Nxt3			    ;don't bother with subnormal

	sub	ESI,longhid
	shl64	EDX,EAX
	or	AL,1			    ;don't lose the last bit
Nxt3:
	and	EDX,mantisa_mask	      ;dump hidden bit
	or	EDX,ESI			      ;install exponent
	and	EDI,sgn			     ;mask sign bit
	or	EDX,EDI			      ;install sign
Nxt7:
	test	EDX,7ff00000h			 ;check for subnormals
	jnz	NxtDone

	feexcept <FE_UNDERFLOW or FE_INEXACT>
	jmps	NxtDone
	
NxtFirstZero:
	mov	EDX,ECX			    ;nextafter zero is the
	and	EDX,sgn			    ;smaller subnormal with the
	mov	EAX,1			    ;sign of the direction
	feexcept <FE_UNDERFLOW or FE_INEXACT>
	jmps	NxtDone


NxtSecondAsQNaN:
	or	ECX,dqnan_bit
	feexcept FE_INVALID
	jmps	NxtSecondQNaN


NxtLargestSNaNasQNaN:
	or	EDX,dqnan_bit
	or	ECX,dqnan_bit
	feexcept FE_INVALID

NxtLargestQNaN:
	_ifs	EDX a ECX, NxtFirstIsAnswer
	jb	NxtSecondQNaN
	_ifs	EAX ae EBX,   NxtFirstIsAnswer

NxtSecondQNaN:
	

NxtSecondIsAnswer:
	mov	EDX,ECX
	mov	EAX,EBX
	jmps	NxtDone

NxtFirstAsQNaN:
	or	EDX,dqnan_bit
	feexcept FE_INVALID

Nxtzeros:
NxtFirstQNaN:
NxtFirstIsAnswer:

NxtDone:
    if _RETST0
	;Return result in ST0
	push	EDX
	push	EAX
	fld	qword ptr [ESP]
	fwait
	add	ESP,8
    endif
	_pop	<ESI,EDI,EBX>
	pop	EBP
	ret
c_endp	nextafter

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; logb

	b	 =	P

if _FLAT
else

c_public logb

func	logb
	push	EBP
	mov	EBP,ESP
	_push	<ESI,EDI,EBX>
	mov	EDX,b+4[EBP]
	mov	EAX,b[EBP]		 ;mov b into registers
	call	dget_dtype
	jmp	dword ptr cs:logbIndex[ESI*4]

logbIndex label       word
    if _FLAT
	dd	offset FLAT:logbNormal		;other
	dd	offset FLAT:logbZero		;zero
	dd	offset FLAT:logbInfinity	;infinite
	dd	offset FLAT:logbSNaN		;SNaN
	dd	offset FLAT:logbQNaN		;QNaN
    else
	dd	logbNormal		;other
	dd	logbZero		;zero
	dd	logbInfinity		;infinite
	dd	logbSNaN		;SNaN
	dd	logbQNaN		;QNaN
    endif

logbNormal:
	mov	ESI,EDX
	and	ESI,longexp		;mask off exponent bits
	jne	logb1			;special case when exponent is zero
	call	dleft_justify		;will figure out what exponent is
	jmps	logb2

logb1:
	shr	ESI,4+16		;right-justify exponent

logb2:
	mov	EDX,ESI
	sub	EDX,longbias
	test	EDX,EDX			;negative? (also clear C)
	jns	logb3			;no
	neg	EDX			;abs value (also set C)

logb3:
	rcr	EDI,1			;bit 15 becomes sign of result
	clr	EAX			;rest of significand is 0
	tst	EDX
	je	logbDone

	mov	ESI,31+longbias        ;2^15
	call	dnorm			;pack result into a double
	jmps	logbDone

logbZero:
	mov	EDX,longexp OR sgn
	clr	EAX
	feexcept FE_DIVBYZERO
	jmps	logbDone

logbInfinity:
	and	EDX,NOT sgn
	jmps	logbDone

logbSNaN:
	or	EDX,dqnan_bit
	feexcept FE_INVALID
logbQNaN:
logbDone:
    if _RETST0
	;Return result in ST0
	push	EDX
	push	EAX
	fld	qword ptr [ESP]
	fwait
	add	ESP,8
    endif
	_pop	<EBX,EDI,ESI>
	pop	EBP
	ret
c_endp	logb
endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; rint function returns the rounded integral value
;
;
	b	 =	P
c_public rint

func	rint
	push	EBP
	mov	EBP,ESP
	_push	<EBX,ESI,EDI>
	mov	EDX,b+4[EBP]
	mov	EAX,b+[EBP]		;mov b into registers
	call	dget_dtype
	cmp	ESI,dtype_other
	je	rint10

	cmp	ESI,dtype_snan
	jne	rintDone

	or	EDX,dqnan_bit
	feexcept FE_INVALID

rintDone:
    if _RETST0
	;Return result in ST0
	push	EDX
	push	EAX
	fld	qword ptr [ESP]
	fwait
	add	ESP,8
    endif
	_pop	<EDI,ESI,EBX>
	pop	EBP
	ret
rint10:
	clr	EBX		 ;clear for guard and sticky bits
	mov	ESI,EDX
	and	ESI,longexp
	cmp	ESI,(longbias + 53) * 100000h
	jae	rintDone
	mov	EDI,EDX
	and	EDI,sgn
	cmp	ESI,(longbias -1) * 100000h
	jb	rint3

	mov	ECX,EDX
	and	ECX,sgn_mask
	shr	ECX,4+16	    ;right justify exponent
	sub	ECX,longbias + 53
	not	ECX
	and	EDX,mantisa_mask
	or	EDX,longhid

	_align
rint11:
	_ifs CX l 8, rint12
	rcr	BX,1		;make room for guard bit
	adc	BL,0
	or	BH,AL		;capture sticky and guard bits
	shrd	EAX,EDX,8
	shr	EDX,8
	sub	CX,8
	jnz	rint11

rint12:
	tst	CX
	je	rint4

	_align
rint1:
	shr64	EDX,EAX
	rcr	BH,1		;save sticky and guard bits
	adc	BL,0		;save sticky bits
	loop	rint1
	jmps	rint4
rint3:
	clr	EAX
	mov	EDX,EAX
	mov	BL,1
rint4:
	tst	BX
	je	rint5

	feexcept FE_INEXACT	;no longer exact
    ifdef _MT
	push	EAX
	feround
	_ifs	EAX e FE_TONEAREST, rint6
	_ifs	EAX e FE_TOWARDZERO,rint16
	cmp	EAX,FE_UPWARD
	pop	EAX
	je	short rint7
    else
	_ifs	__fe_cur_env.round e FE_TONEAREST, rint6
	_ifs	__fe_cur_env.round e FE_TOWARDZERO,rint5
	_ifs	__fe_cur_env.round e FE_UPWARD,  rint7
    endif

	;FE_DOWNWARD
	test	EDI,sgn
	jns	rint5
	jmps	rint8


rint7:;FE_UPWARD
	test	EDI,sgn
	js	rint5
	jmps	rint8


rint6:;FE_TONEAREST
    ifdef _MT
	pop	EAX
    endif
	test	BH,80h		;If guard bit is not set then
	jz	rint5		;no rounding is necessary
	test	BX,7fffh
	jne	rint8		;Sticky bits found
	test	AL,1		;Integer is Odd
	je	rint5

rint8:;round up
	add	EAX,1
	adc	EDX,0
    ifdef _MT
	jmp	short rint5
rint16:	pop	EAX
    endif

rint5:
	mov	ESI,EDX
	or	ESI,EAX
	je	rint15

	mov	ESI, 52 + 11 + longbias ;reset exponent
	call	dnorm
	jmp	rintDone

rint15:
	and	EDI,sgn
	mov	EDX,EDI
	jmp	rintDone
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
	feround			;get rounding mode in EAX
	shr	EAX,10
    else
	mov	AL,byte ptr __fe_cur_env.control + 1
	shr	AL,2
	and	EAX,3
    endif
	movzx	EAX,byte ptr CS:frndtbl[EAX]
	ret
c_endp	__flt_rounds



	endcode double

	end
