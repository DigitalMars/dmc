;_ double.asm   Tue Dec 13 1994   Modified by: Walter Bright */
;$Header: z:/core/crtl/src/core16/double.asm,v 1.1.1.1 1997/01/02 19:17:34 smarx Exp $
;Copyright (C) 1984-1991 by Walter Bright
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

;These are for backwards compatibility with pre-3.0 libraries.
;They will be dropped in the future.

	public	_DBL_MAX,_DBL_MIN,_FLT_MAX,_FLT_MIN

_DBL_MAX	dw	0,0,0,longexp	;maximum double value
_DBL_MIN	dw	0,0,0,longhid	;minimum
_FLT_MAX	dw	0,shortexp	;maximum float value
_FLT_MIN	dw	0,shorthid


	enddata

	begcode	double

	;Note:	0=int	2=unsigned	3=long	4=float	5=double

	public		dunnorm, dround, dget_dtype
	public		dleft_justify,dnorm, dget_dtype_pair
	public		__DSUB@
	c_public	_FLTDBL@,_DBLFLT@
	c_public	_DADD@,_DMUL@,_DDIV@,_DNEG@,_DTST@
	c_public	_DTST0@,_DTST0EXC@
	c_public	_DCMP@,_DCMPEXC@
	c_public	_DBLINT@,_INTDBL@,_DBLUNS@,_UNSDBL@
	c_public	_DBLLNG@,_LNGDBL@,_DBLULNG@,_ULNGDBL@
	c_public	__dtype

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Short real:
;	s | exponent| significand|
;	31|30	  23|22		0|
; Long real:
;	s | exponent| significand|
;	63|62	  52|51		0|

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Unpack a double. The double must not be 0.
; Input:
;	[AX,BX,CX,DX] = the double (with sign = 0)
; Output:
;	[AX,BX,CX,DX] = significand (with hidden bit in AX bit 15)
;	SI	exponent
;	DI	sign (in bit 15)

	even
dunnorm proc	near
	mov	DI,AX		;save sign
	mov	SI,AX
	and	SI,longexp	;mask off exponent bits
	jne	dunnorm1	;special case when exponent is zero
	call	dleft_justify
	jmps	dunnorm2

dunnorm1:
	shr	SI,1
	shr	SI,1
	shr	SI,1
	shr	SI,1		;right justify exponent
	or	AL,longhid	;or in hidden bit

dunnorm2:
	; AX,BX,CX,DX <<= 11
	sh64LBy8
	shl64	AX,BX,CX,DX
	shl64	AX,BX,CX,DX
	shl64	AX,BX,CX,DX
	ret
dunnorm endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Left justify mantissa when
; exponent is zero
;
;  Input:
; [AX,BX,CX,DX] = double
; SI		= Exponent
;
dleft_justify proc near
	mov	SI,1

dleft_justify1:
	dec	SI			;Adjust exponent
	shl64	AL,BX,CX,DX		;shift mantissa left
	test	AL,longhid		;is it shifted enough
	je	dleft_justify1		;no
	ret
dleft_justify endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Round and normalize and add
;Input:
;	[AX,BX,CX,DH] = significand
;	assume bit 7 or 8 of AH is set
;	if bit 8 of AH is set then
;		 bits 1 and 2 of DH are sticky bits and bit
;		3 in DH is the guard bit.
;	else
;		bits 1 of DH is a sticky bits and bit
;	2 in DH is the guard bit.
;	DL = other sticky bits
;	SI = exponent (biased)
;	DI (sign bit)
;
	even
dround	proc	near
	dec	SI		;Make sure exponent is correct if not shifted
	test	AH,80h		;if bit 8 of AH is set then
	jz	dround1		;adjust shift register and
	shr64	AX,BX,CX,DX
	adc	DL,0		;make sure sticky bit did not drop off
	inc	SI		;adjust exponent

dround1:
	_if	SI l 07FFh, dround11
	jmp	dpackOverflow


dround11:
	_if SI g 0, dround7
	dec	SI		;adjustment so it will shift ok
	call	dright_justify	;shift right until SI is zero

dround7:
	test	DH,11b
	jne	dround6
	or	DL,DL
	je	dround5

dround6:
	tst	SI
	jne	dround10
	feexcept FE_UNDERFLOW
dround10:
	feexcept FE_INEXACT		;no longer exact
    ifdef _MT
	push	AX
	feround				;get rounding mode in AX
	_if	AX e  FE_TONEAREST,  dround3
	_if	AX e  FE_UPWARD,     dround2
	cmp	AX,FE_TOWARDZERO
	pop	AX
	je	dround5
    else
	_if	__fe_cur_env.round e  FE_TONEAREST,  dround3
	_if	__fe_cur_env.round e  FE_TOWARDZERO, dround5
	_if	__fe_cur_env.round e  FE_UPWARD,     dround2
    endif

	;FE_DOWNWARD
	test	DI,DI
	jns	dround5
	jmps	dround4


dround2:;FE_UPWARD
    ifdef _MT
	pop	AX
    endif
	test	DI,DI
	js	dround5
	jmps	dround4


dround3:;FE_TONEAREST
    ifdef _MT
	pop	AX
    endif
	test	DH,10b		;If guard bit is not set then
	jz	dround5		;no rounding is necessary
	test	DH,101b		;Test if odd or sticky bits are set
	jne	dround4
	tst	DL		;Test other sticky bits
	jz	dround5


dround4:;round up
	add	DH,100b
	adc	CX,0
	adc	BX,0
	adc	AX,0
	tst	SI		;is exponent zero
	je	dround9		;yes

	test	AH,80h		;is msb still where is should be
	jz	dround5
	inc	SI
	shr64	AX,BX,CX,DX

dround5:
	sh64RBy8
	shr64	AX,BX,CX,DX
	shr64	AX,BX,CX,DX
	jmp	dpack

dround9:			;when exponent is zero
	test	AH,40h		;is msb still where is should be
	jz	dround5		;yes
	inc	SI		;exponent is now 1
	jmps	dround5
dround	endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Normalize and pack a double.
; Input:
;	[AX,BX,CX,DX] = significand
;	SI = exponent (biased)
;	DI bit 15 = sign of result
;
dnorm	proc	near
	sub	SI,11		;offset

dnorm2:	test	AX,0FFE0h	;do we need to shift right?
	jz	dnorm3		;no

dnorm1:
	shr64	AX,BX,CX,DX
	inc	SI		;exponent
	jnc	dnorm2
	test	AX,0FFE0h	;done shifting yet?
	jnz	dnorm1


;see if our significand is 0
dnorm3:	tst	AL
	jnz	dnorm4
	tst	BX
	jnz	dnorm5
	xchg	BX,DX
	xchg	BX,CX		;BX,CX,DX <<= 16
	sub	SI,16		;shift left by 16
	tst	BX
	jnz	dnorm5
	xchg	BX,CX		;BX,CX <<= 16
	sub	SI,16
	tst	BX
	jz	dpack2		;result is 0
	tst	BH
	jnz	dnorm5
	xchg	BH,BL		;BX <<= 8
	sub	SI,8

dnorm4:	test	AL,longhid	;hidden bit in right spot?
	jnz	dnorm6		;no
dnorm5:
	shl64	AL,BX,CX,DX	;shift left till it is
	dec	SI
	jmp	dnorm4

dnorm6:
	_if SI ge 0, dpack
	sh64LBy8
	shl64	AX,BX,CX,DX
	shl64	AX,BX,CX,DX
	dec	SI		;adjustment so it will shift ok
	call	dright_justify	;shift right until SI is zero
	jmp	dround7
dnorm	endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pack a double.
; Input:
;	[AX,BX,CX,DX] = significand
;	SI = exponent (biased)
;	DI bit 15 = sign of result
;

dpack	proc	near
	shl	SI,1
	shl	SI,1
	shl	SI,1
	shl	SI,1
	_if	SI ae 07FF0h, dpackOverflow

	and	AL,longhid - 1	;dump hidden bit
	or	AX,SI		;install exponent
	and	DI,sgn		;mask sign bit
	or	AX,DI		;install sign
dpack2:	ret

dpackOverflow:
;	jmp	exception	;raise overflow exception
dpack	endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Raise underflow/overflow exception
; Input:
;	DI	bit 15 is sign bit of result
;	AX	1 = underflow
;		2 = overflow
; Returns:
;	AX,BX,CX,DX	adjusted result

	public	exception
exception proc near
	feexcept <FE_OVERFLOW or FE_INEXACT>
	clr	BX
	mov	CX,BX
	mov	DX,BX
	dec	AX
	jz	FPV3		;0 is result for underflow

	;Adjust infinity based on rounding mode
	;NEAREST	infinity with sign
	;DOWN		+ overflows to + largest finite, - overflows to -inf
	;UP		- overflows to - largest finite, + overflows to +inf
	;TOZERO		to signed largest finite

	feround			;get rounding mode in AX
	_if	AX e FE_TONEAREST, FPV4
	_if	AX e FE_TOWARDZERO, FPV1
	_if	AX e FE_UPWARD, FPV2

	tst	DI
	js	FPV4
	jmps	FPV1

FPV2:
	tst	DI
	jns	FPV4

	;Generate largest finite
FPV1:
	mov	AX,07FEFh
	dec	BX
	dec	CX
	dec	DX		;to 7FEF FFFF FFFF FFFF
	jmp	short FPV3

FPV4:	mov	AX,longexp

FPV3:	and	DI,sgn
	or	AX,DI		;install sign bit
	ret
exception endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; get index of a pair of double float
;	index = (case(b)*dtype_base + case(c))*2
; input:
;	[BP] = b
;	[AX,BX,CX,DX] = c
; Output:
;	SI = index

dget_dtype_pair proc near
	clr	SI				;set to default
	mov	DI,6[BP]
	test	DI,07fffh
	jz	dget_dtype_pair1

	and	DI,longexp				;mask exponent
	_if	DI ne long_infinity, dget_dtype_pair2	;b is not NaN or infinite
	mov	DI,6[BP]
	test	DI,dqnan_bit				;bit must be set to be
	jz	dget_dtype_pair3			;a quiet NaN
	mov	SI,dtype_qnan*2				;b is a quiet NaN
	jmps	dget_dtype_pair2

dget_dtype_pair3:
	and	DI,0Fh
	or	DI,4[BP]
	or	DI,2[BP]				;Mantissa must be zero
	or	DI,[BP]					;to be infinite
	jz	dget_dtype_pair4			;otherwise
	mov	SI,dtype_snan*2				;b is a signaling NaN
	jmps	dget_dtype_pair2

dget_dtype_pair4:					;b is infinite
	mov	SI,dtype_infinite*2
	jmps	dget_dtype_pair2

dget_dtype_pair1:
	mov	DI,4[BP]
	or	DI,2[BP]
	or	DI,[BP]
	jnz	dget_dtype_pair2
	mov	SI,dtype_zero*2				;b is zero

dget_dtype_pair2:
	test	AX,07fffh
	jz	dget_dtype_pair5

	mov	DI,AX
	and	DI,longexp				;mask exponent
	_if	DI ne long_infinity, dget_dtype_pair6	;c is not NaN or infinite
	mov	DI,AX
	test	DI,dqnan_bit				;bit must be set to be
	jz	dget_dtype_pair7			;a quiet NaN
	add	SI,dtype_qnan * dtype_base*2		;c is a quiet NaN
	ret

dget_dtype_pair7:
	and	DI,0Fh
	or	DI,BX
	or	DI,CX					;Mantissa must be zero
	or	DI,DX					;to be infinite
	jz	dget_dtype_pair8			;otherwise
	add	SI,dtype_snan * dtype_base*2		;c is a signaling NaN
	ret

dget_dtype_pair8:					;c is infinite
	add	SI,dtype_infinite * dtype_base*2
	ret

dget_dtype_pair5:
	mov	DI,BX
	or	DI,CX
	or	DI,DX
	jnz	dget_dtype_pair6
	add	SI,dtype_zero * dtype_base*2		;c is zero

dget_dtype_pair6:
	ret
dget_dtype_pair endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; get specal case index of double float
;	index = case(a)
; input:
;	[AX,BX,CX,DX] = a
; Output:
;	SI = index
;
dget_dtype proc near
	clr	SI				;set to default
	test	AX,07fffh
	jz	dget_dtype1

	mov	DI,AX
	and	DI,longexp				;mask exponent
	_if	DI ne long_infinity, dget_dtype2	;c is not NaN or infinite
	mov	DI,AX
	test	DI,dqnan_bit				;bit must be set to be
	jz	dget_dtype3				;a quiet NaN
	mov	SI,dtype_qnan				;c is a quiet NaN
	ret

dget_dtype3:
	and	DI,0Fh
	or	DI,BX
	or	DI,CX					;Mantissa must be zero
	or	DI,DX					;to be infinite
	jz	dget_dtype4				;otherwise
	mov	SI,dtype_snan				;c is a signaling NaN
	ret

dget_dtype4:						;c is infinite
	mov	SI,dtype_infinite
	ret

dget_dtype1:
	mov	DI,BX
	or	DI,CX
	or	DI,DX
	jnz	dget_dtype2
		mov	SI,dtype_zero			;c is zero

dget_dtype2:
	ret
dget_dtype endp
;Condition code values
CCeq	equ	40h
CClt	equ	81h
CCgt	equ	00h
CCinv	equ	45h

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Put into DI the OR of all the significand bits

orsigc	proc	near
	mov	DI,AX
	and	DI,0Fh
	or	DI,BX
	or	DI,CX
	or	DI,DX
	ret
orsigc	endp

orsigb	proc	near
	mov	DI,P+6[BP]
	and	DI,0Fh
	or	DI,P+4[BP]
	or	DI,P+2[BP]
	or	DI,P+0[BP]
	ret
orsigb	endp

;;;;;;;;;;;;;;;;;;;;;;;;;;
; Test and see if [AX,BX,CX,DX] is 0

	even
func	_DTST@
	push	AX
	shl	AX,1		;dump sign bit
	or	AX,DX
	or	AX,CX
	or	AX,BX
	pop	AX
	ret
c_endp	_DTST@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Double compare against 0, setting sign bits appropriately
;	a = b ? 0
; Input:
;	[AX,BX,CX,DX] = b
; Output:
;	[AX,BX,CX,DX] = b
; _DTST0EXC@ is same as _DTST0@, but set invalid exception flag if
; the operand is a NAN.

	even
func	_DTST0EXC@
	stc
	jmp	short DT9
c_endp	_DTST0EXC@

	even
func	_DTST0@
if 1
	clc
DT9:	push	SI
	sbb	SI,SI			;SI==-1 if raise exception
	push	DI
	push	AX

	;Test if b is a special value
	mov	DI,AX
	and	DI,7FF0h
	jz	DT1			;c is 0 or subnormal
	_if	DI e 7FF0h, DT2		;c is nan or infinity

	;Do a straightforward comparison
DT3:	tst	AX
	jz	DT8
	mov	AH,CCgt
	jg	DTret
	mov	AH,CClt
DTret:	;or	AX,AX			;OF (overflow flag) is already clear
	sahf
	pop	AX
	pop	DI
	pop	SI
	ret

DT8:	or	AX,BX
	or	AX,CX
	or	AX,DX
	jne	DT7

DTeq:	mov	AH,CCeq
	jmp	DTret

DT7:	mov	AH,CCgt
	ja	DTret
	mov	AH,CClt
	jmp	DTret

DT1:	call	orsigc
	jnz	DT3			;subnormal
	jmp	DTeq			;b is 0

DT2:	call	orsigc
	jz	DT3			;b is infinity

	;b is a NAN
	mov	AH,CCinv
	and	SI,FE_INVALID
	feexcept SI
	jmp	DTret
else
	push	AX
	tst	AX
	js	TST01
	or	AX,BX
	or	AX,CX
	or	AX,DX
	neg	AX
	sbb	AX,AX
	neg	AX
	pop	AX
	ret

TST01:
	shl	AX,1
	or	AX,BX
	or	AX,CX
	or	AX,DX
	neg	AX			;C=1 if b!=0
	sbb	AX,AX			;if (b==0) AX=0 else AX=-1
	pop	AX
	ret
endif
c_endp	_DTST0@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Double compare
;	a = b ? c
; Input:
;	stack = b
;	[AX,BX,CX,DX] = c
; Output:
;	no registers changed
;	Condition codes set same as 8087 would
;	(but also set SF and OF so that pre-3.0 code will link which
;	used signed jmps after DCMP)
;
; _DCMPEXC@ is same as _DCMP@, but set invalid exception flag if
; either of the operands are NAN.

if 1
	even
func	_DCMPEXC@
	stc
	jmp	short DC9
c_endp	_DCMPEXC@

	even
func	_DCMP@
	clc
DC9:	push	BP
	mov	BP,SP
	push	SI
	sbb	SI,SI			;SI==-1 if raise exception
	push	DI
	push	AX

	;Test if c is a special value
	mov	DI,AX
	and	DI,7FF0h
	jz	DC1			;c is 0 or subnormal
	_if	DI e 7FF0h, DC2		;c is nan or infinity

	;Test if b is a special value
DC3:	mov	DI,P+6[BP]
	and	DI,7FF0h
	jz	DC4			;b is 0 or subnormal
	_if	DI e 7FF0h, DC5		;b is nan or infinity

	;Do a straightforward comparison
DC6:	mov	DI,AX
	xor	DI,P+6[BP]
	js	DC8			;signs are different
	_if	P+6[BP] ne AX, DC7
	_if	P+4[BP] ne BX, DC7
	_if	P+2[BP] ne CX, DC7
	_if	P+0[BP] ne DX, DC7

DCeq:	mov	AH,CCeq
DCret:	or	AX,AX			;clear OF (overflow flag)
	sahf
	pop	AX
	pop	DI
	pop	SI
	pop	BP
	ret	8

DC7:	mov	AL,AH
	mov	AH,CCgt
	ja	DC10
	mov	AH,CClt
DC10:	tst	AL
	jns	DCret
	xor	AH,CCgt XOR CClt
	jmp	DCret

DC8:	tst	AX
	mov	AH,CClt
	jns	DCret
	mov	AH,CCgt
	jmp	DCret

DC1:	call	orsigc
	jnz	DC3			;subnormal
	;c is +0 or -0
	and	AH,7Fh			;no -0 bugs
	jmp	DC3			;c is 0

DC2:	call	orsigc
	jz	DC3			;c is infinity
	jmp	short DCinv		;c is a nan

DC4:	call	orsigb
	jnz	DC6			;b is subnormal
	;c is +0 or -0
	and	byte ptr P+7[BP],7Fh	;no -0 bugs
	jmp	DC6			;b is 0

DC5:	call	orsigb
	jz	DC6			;b is infinity
;	jmp	DCinv			;b is a nan

DCinv:	mov	AH,CCinv
	and	SI,FE_INVALID
	feexcept SI
	jmp	DCret
c_endp	_DCMP@

else
	even
func	_DCMP@
	push	BP
	mov	BP,SP
	push	DI

	push	AX

	;test if c is 0
	mov	DI,AX
	shl	DI,1			;dump sign bit
	or		DI,DX
	or		DI,CX
	or		DI,BX
	jnz	C3			;no
	and	AH,7Fh			;no -0 bugs
C3:
	mov	DI,6+P[BP]
	shl	DI,1			;dump sign bit
	or	DI,4+P[BP]
	or	DI,2+P[BP]
	or	DI,P[BP]
	jnz	C2			;no
	and	byte ptr 7+P[BP],7Fh	;convert -0 to 0
C2:
	mov	DI,AX
	xor	DI,word ptr 6+P[BP]
	js	C52			;signs are different
	mov	DI,1			;1 for positive compares
	tst	AX
	jns	C51
	neg	DI			;-1 for negative compares
C51:	_if	6+P[BP] ne AX, C6	;compare MSW
	_if	4+P[BP] ne BX, C6
	_if	2+P[BP] ne CX, C6
	_if	P[BP] e DX, L21
C6:	ja	C7
	neg	DI
C7:	tst	DI
L21:	pop	AX
	pop	DI
	pop	BP
	ret	8

C52:	cmp	6+P[BP],AX
	jmp	L21
c_endp	_DCMP@
endif


;;;;;;;;;;;;;;;;;;;
; Negate a double (or a float)
; Input:
;	[AX,BX,CX,DX]
; Output:
;	[AX,BX,CX,DX] = - [AX,BX,CX,DX]

func	_DNEG@
	xor	AH,80h		;toggle sign bit
	ret
c_endp	_DNEG@


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Right justifty mantissa when
; exponent is less then zero
;
;  Input:
;  [AX,BX,CX,DX] = double
;  SI		 = Exponent
;
dright_justify proc near
	_if SI l -56, dright_justify5

dright_justify1:
	_if	SI g -8, dright_justify2
	sh64StyRBy8
	add	SI,8
	jnz	dright_justify1

dright_justify2:
	xchg	CX,SI
	jcxz	dright_justify3	;no shifting need be done
	neg	CX
	or	DL,DL		;If any sticky bits are set then
	je	dright_justify4	;make sure they don't all rotate out
	or	DL,40h

dright_justify4:
	shr64	AX,BX,SI,DX
	loop	dright_justify4

dright_justify3:
	xchg	CX,SI		;restore CX, SI = 0
	ret

dright_justify5:
	or	DX,AX
	or	DX,BX
	or	DX,CX
	or	DL,DH		;save sticky bits
	clr	AX
	mov	BX,AX
	mov	CX,AX
	mov	DH,AH		;clear mantissa
	mov	SI,AX
	ret
dright_justify endp


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
	mov	DX,b+[BP]	   	;mov b into registers
	_push	 <DI,SI>
	call	dget_dtype_pair
	jmp	word ptr CS:dnxtindex[SI]

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
	xor	DI,AX			    ;always subtract if signs are
	js	NxtSub			    ;different
	_if	6[BP] ne AX, Nxt2
	_if	4[BP] ne BX, Nxt2
	_if	2[BP] ne CX, Nxt2
	_if	0[BP] ne DX, Nxt2
	jmp	NxtDone			    ;return x if equal

Nxt2:
	jb	NxtSub

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
	jmps	Nxt3

Nxt9:
	test	AX,longhid		    ;test if it is still subnormal
	jz	Nxt3
	jmps	Nxt10
NxtSub:
	mov	DI,AX			    ;save sign
	and	AX,longhid-1		    ;clear exponent and sign
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
NxtDone1: jmps	NxtDone
	
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
	or	AX,dqnan_bit
	or	word ptr 6[BP],dqnan_bit
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




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Double floating add/subtract.
;	a = b +(-) c
; Input:
;	b is on stack
;	[AX,BX,CX,DX] = c
; Output:
;	a = [AX,BX,CX,DX]
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

__DSUB@:
	push	BP
	sub	SP,nn			;make room for nn variables
	mov	BP,SP
	_if	__8087 e 0, A6		;if no 8087
	fld	qword ptr b[BP]		;load b
	_push	<AX,BX,CX,DX>		;push c
	fsub	qword ptr -8[BP]	;sub c
	jmps	fltret
A6:
	mov	orgsign[BP],8000h	;need to fip sign back on  NaNs
	xor	AH,80h			;flip sign for subtraction
	jmps	A1

	even
func	_DADD@
	push	BP
	sub	SP,nn			;make room for nn variables
	mov	BP,SP
	_if	__8087 e 0, A7		;if no 8087
	fld	qword ptr b[BP]		;load b
	_push	<AX,BX,CX,DX>		;push c
	fadd	qword ptr -8[BP]	;add c

fltret:
	;Check for floating point error
;	fstsw	word ptr -2[BP]
;	fwait
;	mov	AX,-2[BP]
;	and	AX,FE_ALL_EXCEPT
;	jnz	fltret5			;jmp if error
;fltret4:
	fstp	qword ptr -8[BP]
	fwait				;wait for it to finish
	_pop	<DX,CX,BX,AX>		;pop result
	add	SP,nn
	pop	BP
	ret	8

;fltret5:
;	feexcept AX
;	jmp	fltret4


A7:	mov	word ptr orgsign[BP],0	;need for NaN

A1:
	push	DI
	push	SI
	add	BP,b
	call	dget_dtype_pair
	sub	BP,b
	jmp	word ptr CS:daddindex[SI]

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

	dw	AFirstQNaN		;QNaN + other
	dw	AFirstQNaN		;QNaN + zero
	dw	AFirstQNaN		;QNaN + infinite
	dw	AFirstAsQNaN		;QNaN + SNaN
	dw	ALargestQNaN		;QNaN + QNaN

ANormalAdd:

	mov	signc[BP],AX	;Save sign
	mov	DI,b+6[BP]
	mov	sign[BP],DI
	xor	DI,AX		;if sign(b) != sign(c), then subtraction
	mov	subtract[BP],DI	;flag for subtraction

	call	dunnorm		;unpack second operand (c)
	mov	exp[BP],SI	;save exponent of c
	regStk64		;move registers to stack
	mov	AX,b+6[BP]
	mov	BX,b+4[BP]
	mov	CX,b+2[BP]
	mov	DX,b[BP]
	call	dunnorm		;unpack first operand (b)
	sub	SI,exp[BP]	;SI = exp(b) - exp(c)
	jle	A2		;exp(b) > exp(c)
	add	exp[BP],SI	;exponent is b
	xChgStkReg64
	neg	SI
	mov	DI,signc[BP]
	mov	sign[BP],DI		;reset sign
A2:
	call	dright_justify
	test	byte ptr subtract+1[BP],80h	;subtracting? (test bit 15)
	je	A3				;no

	sub	DX,[BP]
	sbb	CX,2[BP]
	sbb	BX,4[BP]
	sbb	AX,6[BP]

	jnc	A4				;no borrow
	xor	byte ptr sign+1[BP],80h		;toggle sign of result
	neg64					;SI must be 0 for this to work

A4:
	test	AH,0C0h				;if bit 8 or 7 are set then
	jnz	A5				;do normal round
	mov	DI,AX
	or	DI,BX
	or	DI,CX
	or	DI,DX			;is result zero
	je	Azeros1			;yes

	mov	DI,sign[BP]
	mov	SI,exp[BP]		;exponent of result
	call	dnorm			;normalize and pack
	jmp	Adone

A3:
	add	DX,[BP]
	adc	CX,2[BP]
	adc	BX,4[BP]
	adc	AX,6[BP]
	jnc	A5
	rcr	AX,1
	rcr	BX,1
	rcr	CX,1
	rcr	DX,1
	inc	word ptr exp[BP]	;bump exponent

A5:
	mov	DI,sign[BP]
	mov	SI,exp[BP]		;exponent of result
	call	dround			;round and normalize
	jmp	Adone


Azeros:
	mov	DI,AX
	xor	DI,b+6[BP]
	test	DI,8000h		;are signs the same
	jne	Azeros1
	jmp	ADone			;yes

Azeros1:
    ifdef _MT
	feround
	sub	AX,FE_DOWNWARD
	je	Azeros2
	clr	AX
    else
	clr	AX
	_if	__fe_cur_env.round e FE_DOWNWARD,Azeros2
    endif
	jmp	AFirstIsAnswer
Azeros2:
	mov	AH,80h			;set sign to -
	jmp	AFirstIsAnswer
	
AInfiniteInfinite:
	mov	DI,AX
	xor	DI,b+6[BP]
	test	DI,8000h		;are signs the same
	je	AFirstIsAnswer		;yes

					;Default invalid operation
	mov	AX,dbl_qnan
	mov	BX,02000H
	clr	CX
	mov	DX,CX
	feexcept FE_INVALID
	jmps	ADone


ASecondAsQNaN:
	or	word ptr B+6[BP],dqnan_bit
	feexcept FE_INVALID
	jmps	ASecondQNaN


ALargestSNaNasQNaN:
	or	AX,dqnan_bit
	or	word ptr b[BP],dqnan_bit
	feexcept FE_INVALID

ALargestQNaN:
	xor	AX,orgsign[BP]		;reset original sign
	_if	AL a b+6[BP], AFirstIsAnswer
	jb	ASecondQNaN
	_if	BX a b+4[BP], AFirstIsAnswer
	jb	ASecondQNaN
	_if	CX a b+2[BP], AFirstIsAnswer
	jb	ASecondQNaN
	_if	DX ae b[BP],   AFirstIsAnswer

ASecondQNaN:
	

ASecondIsAnswer:
	mov	AX,b+6[BP]
	mov	BX,b+4[BP]
	mov	CX,b+2[BP]
	mov	DX,b[BP]
	jmps	ADone

AFirstAsQNAN:
	or	AX,dqnan_bit
	feexcept FE_INVALID

AFirstQNaN:
	xor	AX,orgsign[BP]		;reset original sign
AFirstIsAnswer:

Adone:
	pop	SI
	pop	DI
	add	SP,nn
	pop	BP
	ret	8
c_endp	_DADD@


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Double floating divide.
;	a = b / c
; Input:
;	[BP] = b
;	[AX,BX,CX,DX] = c
; Output:
;	a = [AX,BX,CX,DX]
;	SI,DI preserved

; Stack offsets:

	paddd	=	16		;so nn == cxdd == mm == 40
	resp	=	paddd+16	;pointer to result
	sign	=	resp+2		;sign of result
	exp	=	sign+2		;exponent of result
	count	=	exp+2		;loop counter
	cxdd	=	count+2		;amount of local variables

func	_DDIV@
	push	BP
	sub	SP,cxdd
	mov	BP,SP

	_if	__8087 e 0, D7		;if no 8087
	fld	qword ptr cxdd+P[BP]	;load b
	_push	<AX,BX,CX,DX>		;push c

	; FDIV bug workaround
	_if	_fdiv_chk_flag e 1, D20	;if FDIV is ok
	fld	qword ptr -8[BP]
	call	__FDIVP			;workaround for FDIV bug
	jmp	fltret
D20:

	fdiv	qword ptr -8[BP]	;div c
	jmp	fltret

D7:	push	SI
	push	DI

	mov	sign[BP],AX
	regStk64			  ;transfer sig(b) to 0[BP]

	mov	AX,cxdd+P+6[BP]
	mov	BX,cxdd+P+4[BP]
	mov	CX,cxdd+P+2[BP]
	mov	DX,cxdd+P+[BP]		;mov b into registers
	xor	sign[BP],AX
	and	sign[BP],sgn
	call	dget_dtype_pair
	jmp	word ptr CS:Dindex[SI]

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


	even
DNormalDivide:
	;unpack c
	mov	DI,6[BP]
	and	DI,longexp		;mask off exponent bits
	xor	6[BP],DI		;remove exponent from mantissa
	tst	DI			;is exponent zero
	jnz	D12
					;special case when exponent is zero
	xChgStkReg64			;may need to do a lot of shifting
	call	dleft_justify		;msb must be block to left
	xChgStkReg64
	mov	DI,SI			;save exponent
	jmps	D13
D12:
	or	byte ptr 6[BP],longhid
	shr	DI,1
	shr	DI,1
	shr	DI,1
	shr	DI,1
D13:
	
	;unpack b
	mov	SI,AX
	and	SI,longexp		;mask off exponent bits
	xor	AX,SI			;turn off exponent bits
	tst	SI			;is exponent zero
	jnz	D14
					;special case when exponent is zero
	call	dleft_justify		;msb must be block to left
	jmps	D15
D14:
	or	AL,longhid		;or in hidden bit
	shr	SI,1
	shr	SI,1
	shr	SI,1
	shr	SI,1			;right justify exponent
D15:
	sub	SI,DI			;exp(result) = exp(b) - exp(c)
	add	SI,longbias		;so bias is retained after subtraction
	mov	exp[BP],SI		;exponent of result

	mov	SI,CX			;free up CX for loop counter
	mov	AH,6[BP]
;;;;;;;
	;if (b >= c) goto D31 else D41
	mov	CX,55			;16 bits per word
	mov	DI,1			;used to count to 16
	
	even
D51:	_if	AL a AH, D31
	jb	D41
	_if	BX a 4[BP], D31
	jb	D41
	_if	SI a 2[BP], D31
	jb	D41
	_if	DX b 0[BP], D41

	;b -= c

D31:	sub	DX,0[BP]
	sbb	SI,2[BP]
	sbb	BX,4[BP]
	sbb	AL,AH			;since b > c, C == 0

D41:	rcl	DI,1			;0 if we subtracted, 1 if not
	jc	D71			;push value every 16 loops

D61:	shl64	AL,BX,SI,DX		;b <<= 1
	loop	D51


	or	DX,SI			;set DL to sticky bits
	or	DX,BX
	or	DL,AL
	or	DL,DH
	mov	AX,DI
	not	AL
	shl	AL,1			;shift out counting bit
	mov	DH,AL			;we shift in the complement

;;;;;;;

	pop	CX
	pop	BX
	pop	AX			;load sig(result)
	mov	SI,exp[BP]
	mov	DI,sign[BP]
	call	dround			;round and normalize result
	jmp	DDone


D71:	not	DI			;push next mantissa on stack
	push	DI
	mov	DI,1			;reset counter
	jmps	D61

DDivideByZero:
	feexcept FE_DIVBYZERO

DSignedInfinite:
	mov	AX,sign[BP]
	or	AX,long_infinity
	clr	BX
	mov	CX,BX
	mov	DX,BX
	jmp	DDone

	even
DSignedZero:
	mov	AX,sign[BP]
	clr	BX
	mov	CX,BX
	mov	DX,BX
	jmps	DDone

	even
DSecondAsQNaN:
	or	word ptr 6[BP],dqnan_bit
	feexcept FE_INVALID

DSecondQNaN:
	stkReg64
	jmps	DDone

	even
DDefaultQNaN:
	mov	AX,dbl_qnan
	or	AX,sign[BP]
	mov	BX,02000H
	clr	CX
	mov	DX,CX
	feexcept FE_INVALID
	jmps	DDone


	even
DLargestSNaNasQNaN:
	or	AX,dqnan_bit
	or	word ptr [BP],dqnan_bit
	feexcept FE_INVALID

DLargestQNaN:
	_if	AL a 6[BP], DFirstQNaN
	jb	DSecondQNaN
	_if	BX a 4[BP], DFirstQNaN
	jb	DSecondQNaN
	_if	CX a 2[BP], DFirstQNaN
	jb	DSecondQNaN
	_if	DX ae [BP], DFirstQNaN
	jmps	DSecondQNaN

DFirstAsQNAN:
	or	AX,dqnan_bit
	feexcept FE_INVALID
DFirstQNaN:

DDone:
	pop	DI
	pop	SI
	add	SP,cxdd
	pop	BP
	ret	8
c_endp	_DDIV@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Double floating multiply.
;	a = b * c
; Input:
;	[BP] = b
;	[AX,BX,CX,DX] = c
; Output:
;	a = [AX,BX,CX,DX]
;	SI,DI preserved

; Stack offsets:

	padmm	=	26		;so nn == dd == mm == 40
	sign	=	padmm+8		;sig(b) + sig(c) + sig(result)
	exp	=	sign+2
	count	=	exp+2
	mm	=	count+2
	 b	=	mm + P

	even
func	_DMUL@
	push	BP
	sub	SP,mm
	mov	BP,SP
	_if	__8087 e 0, M7		;if no 8087

	fld	qword ptr mm+P[BP]	;load b
	_push	<AX,BX,CX,DX>		;push c
	fmul	qword ptr -8[BP]	;mul c
	jmp	fltret

M7:	push	SI
	push	DI

	mov	sign[BP],AX
	regStk64			;transfer sig(b) to 0[BP]
	mov	AX,b+6[BP]
	mov	BX,b+4[BP]
	mov	CX,b+2[BP]
	mov	DX,b+[BP]		;mov b into registers
	xor	sign[BP],AX
	and	sign[BP],sgn
	call	dget_dtype_pair
	jmp	word ptr CS:Mindex[SI]

Mindex	label	word
	dw	MNormalMultiply		;other * other
	dw	MSignedZero		;other * zero
	dw	MSignedInfinite		;other * infinite
	dw	MSecondAsQNaN		;other * SNaN
	dw	MSecondQNaN		;other * QNaN

	dw	MSignedZero		;zero * other
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

	dw	MFirstQNaN		;QNaN * other
	dw	MFirstQNaN		;QNaN * zero
	dw	MFirstQNaN		;QNaN * infinite
	dw	MFirstAsQNaN		;QNaN * SNaN
	dw	MLargestQNaN		;QNaN * QNaN



MNormalMultiply:

	call	dunnorm			;unpack second operand (c)
	mov	exp[BP],SI		;save exponent of c
	xChgStkReg64
	call	dunnorm			;unpack second operand (b)
	sub	SI,longbias - 1 	;so bias is retained after add
	add	exp[BP],SI		;exponent of result


	push	DS			;assume SS != DS
	mov	SI,ES			;save ES
	mov	DI,SS
	mov	ES,DI
	mov	DS,DI

	lea	DI,14[BP]		;ES:DI = &14[BP]
	mov	8[BP],DX
	mov	10[BP],CX
	mov	12[BP],BX
	stosw				;transfer c to 8[BP]
	clr	AX
	mov	CX,8			;8 words of 0 to product
	rep	stosw
	mov	ES,SI			;restore ES

	;Compute the 128 bit result of sig(b)*sig(c), and use the high 64 bits.
	;	  C6,C4,C2,C0
	;	* B6,B4,B2,B0

	;BP -> b
	sub	DI,16+8			;DI -> c (BP + 8)
	clr	BX
	mov	CX,6[DI]		;CX = C6
	mov	SI,4[DI]		;SI = C4

	;BP    -> b
	;BP+16 -> result

	mov	AX,[BP]			;AX = B0
	mul	word ptr [DI]		;* C0
;	mov	[BP+16],AX		;always zero
	mov	2[BP+16],DX

	mov	AX,[BP]			;B0
	mul	word ptr 2[DI]		;* C2
	add	2[BP+16],AX
	adc	4[BP+16],DX

	mov	AX,2[BP]		;AX = B2
	mul	word ptr [DI]		;* C0
	add	2[BP+16],AX
	adc	4[BP+16],DX
	adc	6[BP+16],BX

	mov	AX,[BP]			;B0
	mul	SI			;* C4
	add	4[BP+16],AX
	adc	6[BP+16],DX

	mov	AX,2[BP]		;B2
	mul	word ptr 2[DI]		;* C2
	add	4[BP+16],AX
	adc	6[BP+16],DX
	adc	8[BP+16],BX

	mov	AX,4[BP]		;B4
	mul	word ptr [DI]		;* C0
	add	4[BP+16],AX
	adc	6[BP+16],DX
	adc	8[BP+16],BX

	mov	AX,[BP]			;B0
	mul	CX			;* C6
	add	6[BP+16],AX
	adc	8[BP+16],DX

	mov	AX,2[BP]		;B2
	mul	SI			;* C4
	add	6[BP+16],AX
	adc	8[BP+16],DX
	adc	10[BP+16],BX

	mov	AX,4[BP]		;B4
	mul	word ptr 2[DI]		;* C2
	add	6[BP+16],AX
	adc	8[BP+16],DX
	adc	10[BP+16],BX

	mov	AX,6[BP]		;B6
	mul	word ptr [DI]		;* C0
	add	6[BP+16],AX
	adc	8[BP+16],DX
	adc	10[BP+16],BX

	mov	AX,2[BP]		;B2
	mul	CX			;* C6
	add	8[BP+16],AX
	adc	10[BP+16],DX
	adc	12[BP+16],BX

	mov	AX,4[BP]		;B4
	mul	SI			;* C0
	add	8[BP+16],AX
	adc	10[BP+16],DX
	adc	12[BP+16],BX

	mov	AX,6[BP]		;B6
	mul	word ptr 2[DI]		;* C2
	add	8[BP+16],AX
	adc	10[BP+16],DX
	adc	12[BP+16],BX

	mov	AX,4[BP]		;B4
	mul	CX			;* C6
	add	10[BP+16],AX
	adc	12[BP+16],DX
	adc	14[BP+16],BX

	mov	AX,6[BP]		;B6
	mul	SI			;* C4
	add	10[BP+16],AX
	adc	12[BP+16],DX
	adc	14[BP+16],BX

	mov	AX,6[BP]		;B6
	mul	CX			;* C6
	mov	BX,AX
	mov	AX,DX			;12[BP+16]

	pop	DS

	;Gather result, pack it and return it.
	;Low 16 bits are guaranteed to be 0.
	;One of the first 2 bits are guaranteed to be the most sig bit

	mov	DX,2[BP+16]
	or	DX,4[BP+16]
	or	DX,6[BP+16]
	or	DL,DH			;get sticky bits
	clr	DH
	or	DX,8[BP+16]
	mov	CX,10[BP+16]
	add	BX,12[BP+16]
	adc	AX,14[BP+16]		;get high 64 bits of result

	mov	SI,exp[BP]
	mov	DI,sign[BP]
	call	dround			;round and normalize result
	jmp	MDone

MSignedInfinite:
	mov	AX,sign[BP]
	or	AX,long_infinity
	clr	BX
	mov	CX,BX
	mov	DX,BX
	jmp	MDone

	even
MSignedZero:
	mov	AX,sign[BP]
	clr	BX
	mov	CX,BX
	mov	DX,BX
	jmps	MDone

	even
MSecondAsQNaN:
	or	word ptr 6[BP],dqnan_bit
	feexcept FE_INVALID

MSecondQNaN:
	stkReg64
	jmps	MDone

	even
MDefaultQNaN:
	mov	AX,dbl_qnan
	or	AX,sign[BP]
	clr	BX
	mov	CX,BX
	mov	DX,BX
	feexcept FE_INVALID
	jmps	MDone


	even
MLargestSNaNasQNaN:
	or	AX,dqnan_bit
	or	word ptr [BP],dqnan_bit
	feexcept FE_INVALID

MLargestQNaN:
	_if	AL a 6[BP], MFirstQNaN
	jb	MSecondQNaN
	_if	BX a 4[BP], MFirstQNaN
	jb	MSecondQNaN
	_if	CX a 2[BP], MFirstQNaN
	jb	MSecondQNaN
	_if	DX ae 0[BP],MFirstQNaN
	jmps	MSecondQNaN

MFirstAsQNAN:
	or	AX,dqnan_bit
	feexcept FE_INVALID
MFirstQNaN:

MDone:
	pop	DI
	pop	SI
	add	SP,mm
	pop	BP
	ret	8
c_endp	_DMUL@


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert from float to double.
; Input:
;	[DX,AX] = float
; Output:
;	[AX,BX,CX,DX] = double

func	_FLTDBL@
	_if	__8087 e 0, fltDbl1		;if no 8087
	_push	<DX,AX>			;push b
	sub	SP,4			;2 extra words
	push	BP
	mov	BP,SP
	fld	dword ptr 6[BP]		;load float into 8087
fltret2:
	fstp	qword ptr 2[BP]
	pop	BP
	fwait				;wait for it to finish
	_pop	<DX,CX,BX,AX>		;pop result
	ret
fltDbl1:
	_push	<SI,DI>
	call	fget_dtype
	shl	SI,1
	jmp	word ptr CS:fltDblIndex[SI]

fltDblIndex label	word
	dw	fltDblNormal		;other
	dw	fltDblZero		;zero
	dw	fltDblInfinite		;infinite
	dw	fltDblSNaN		;SNaN
	dw	fltDblQNaN		;QNaN

fltDblNormal:
		call	funnorm		;unpack the float
	mov	BX,AX
	mov	AX,DX
	clr	CX
	mov	DX,CX
	add	SI,longbias-shortbias	;fix the bias on the exponent
	call	dround			;pack a double
	jmps	fltDblDone

fltDblZero:
	clr	BX
	jmps	fltDblSign

fltDblInfinite:
	mov	AX,long_infinity
	clr	BX
	jmps	fltDblSign
	
fltDblSNaN:
	mov	AX,long_infinity
	mov	BX,2000H
	jmps	fltDblSign

fltDblQNaN:
	mov	AX,dbl_qnan OR 04h
	clr	BX
	test	DL,1
	je	fltDblSign
	mov	BH,20h


fltDblSign:
	and	DH,80h
	or	AH,DH
	clr	DX
	mov	CX,DX

fltDblDone:
	_pop	<DI,SI>
	ret
c_endp	_FLTDBL@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert from double to float.
; Input:
;	[AX,BX,CX,DX]
; Output:
;	[DX,AX]
;	CX,BX	destroyed
;	SI,DI	preserved

func	_DBLFLT@
	_if	__8087 e 0, dblFlt1	;if no 8087
	_push	<AX,BX,CX,DX>		;push double
	push	BP
	mov	BP,SP
	fld	qword ptr 2[BP]		;load b into 8087
	fstp	dword ptr 6[BP]		;store float result
	pop	BP
	add	SP,4
	fwait				;wait for it to finish
	_pop	<AX,DX>			;pop result
	ret

dblFlt1:
	_push	<SI,DI>
	call	dget_dtype
	shl	SI,1
	jmp	word ptr CS:dblFltIndex[SI]

dblFltIndex label	word
	dw	dblFltNormal		;other
	dw	dblFltSign		;zero
	dw	dblFltInfinite		;infinite
	dw	dblFltSNaN		;SNaN
	dw	dblFltQNaN		;QNaN

dblFltNormal:
	call	dunnorm			;unpack double
	sub	SI,longbias-shortbias	;fix exponent bias
	or	CX,DX
	jz	dblFlt4
	or	BX,2			;save sticky bit
dblFlt4:
	mov	DX,AX
	mov	AX,BX
	call	fround			;pack float
	jmps	dblFltDone



dblFltInfinite:
	mov	DX,short_infinity
	jmps	dblFltSign
	
dblFltSNaN:
	mov	DX,short_infinity OR 1
	jmps	dblFltSign

dblFltQNaN:
	mov	DX,short_qnan
	cmp	BH,20h
	jne	dblFltSign
	or	DL,1

dblFltSign:
	and	AH,80h
	or	DH,AH
	clr	AX

dblFltDone:
	_pop	<DI,SI>
	ret
c_endp	_DBLFLT@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert unsigned int to double.
; SI,DI preserved.

func	_UNSDBL@
	clc
	jmps	INTDBL2

c_endp	_UNSDBL@

; Convert int to double

func	_INTDBL@
	_if	__8087 e 0, INTDBL1	;if no 8087
	push	AX
	sub	SP,6			;3 extra words
	push	BP
	mov	BP,SP
	fild	word ptr 6+2[BP]	;load integer into 8087
INTDBL3:
	fstp	qword ptr 2[BP]
	pop	BP
	fwait				;wait for it to finish
	_pop	<DX,CX,BX,AX>		;pop result
	ret


INTDBL1:
	or	AX,AX			;negative? (also clear C)
	jns	INTDBL2			;no
	neg	AX			;abs value (also set C)
INTDBL2:
	_push	<SI,DI>
	rcr	DI,1			;bit 15 becomes sign of result
	clr	DX
	mov	CX,DX
	mov	BX,DX			;rest of significand is 0
	mov	SI,15+longbias		;2^15
	call	dnorm			;pack result into a double
	_pop	<DI,SI>
	ret

c_endp	_INTDBL@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert unsigned long to double.
; SI,DI preserved.

func	_ULNGDBL@
	clc
	jmps	A8
c_endp	_ULNGDBL@

; Convert long to double.

func	_LNGDBL@
	_if	__8087 e 0, A12		;if no 8087
	_push	<DX,AX>
	sub	SP,4			;2 extra words
	push	BP
	mov	BP,SP
	fild	dword ptr 6[BP]		;load long into 8087
	jmp	INTDBL3

A12:	or	DX,DX			;negative? (also clear C)
	jns	A8			;no
	neg32	DX,AX			;abs value
	stc				;indicate negative result
A8:	_push	<SI,DI>
	rcr	DI,1			;bit 15 becomes sign of result
	clr	CX
	mov	BX,CX			;rest of significand is 0
	xchg	AX,BX
	xchg	AX,DX
	mov	SI,31+longbias		;2^15
	call	dnorm			;pack result into a double
	_pop	<DI,SI>
	ret
c_endp	_LNGDBL@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert double to unsigned.
      MinBitShift =  0
       stackSize  =  MinBitShift + 2

	even
func	_DBLUNS@
	push	BP
	sub	SP,stackSize
	mov	BP,SP
	mov	word ptr MinBitShift[BP],15
	test	AH,80h
	jz	dblIntgl

	feexcept FE_INVALID
	add	SP,stackSize
	pop	BP
	ret
c_endp	_DBLUNS@

; Convert double to int.

func	_DBLINT@
	push	BP
	sub	SP,stackSize
	mov	BP,SP
	mov	word ptr MinBitShift[BP],14
	test	AH,80h
	je	dblIntgl

	inc	word ptr MinBitShift[BP]
	jmps	dblIntgl

c_endp	_DBLINT@


; Convert double to unsigned long.

func	_DBLULNG@
	push	BP
	sub	SP,stackSize
	mov	BP,SP
	mov	word ptr MinBitShift[BP],31
	test	AH,80h
	jz	dblIntgl

	feexcept FE_INVALID
	clr	AX
	cwd				;return 0 for invalid operation
	add	SP,stackSize
	pop	BP
	ret
c_endp	_DBLULNG@

; Convert double to long

func	_DBLLNG@
	push	BP
	sub	SP,stackSize
	mov	BP,SP
	mov	word ptr MinBitShift[BP],30
	test	AH,80h
	jz	dblIntgl

	inc	word ptr MinBitShift[BP]

dblIntgl:
	_push	<SI,DI>
	call	dget_dtype
	shl	SI,1
	jmp	word ptr CS:dblIntglIndex[SI]

dblIntglIndex label	  word
	dw	dblIntglNormal		  ;other
	dw	dblIntglZero		  ;zero
	dw	dblIntglInvalid		  ;infinite
	dw	dblIntglInvalid		  ;SNaN
	dw	dblIntglInvalid		  ;QNaN

dblIntglNormal:
	call	dunnorm		;unpack double
	xchg	CX,BX		;save guard bit
	or	DL,DH
	or	BL,DL
	je	dblIntgl1
	mov	BL,1		;save stick bits
dblIntgl1:
	mov	DX,AX
	mov	AX,CX
	sub	SI,longbias	;un-bias the exponent
	js	dblIntgl4	;for neg exponents, the result is 0
	_if	SI a MinBitShift[BP], dblIntglInvalid
	mov	CX,31
	sub	CX,SI
	jcxz	dblIntgl2

dblIntgl3:
	shr	DX,1
	rcr	AX,1
	rcr	BH,1		;keep sticky bit
	adc	BL,0		;keep guard bit
	loop	dblIntgl3

dblIntgl2:
	tst	BX
	je	dblIntgl6
	feexcept FE_INEXACT	;no longer exact
dblIntgl6:
	test	DI,8000h	;is result negative?
	jns	dblIntglDone	;no
	mov	SI,MinBitShift[BP]
	_if	SI e 31 dblIntgl7
	cmp	AX,8000h	;special case at most negative
	ja	dblIntglInvalid
	je	dblIntglDone

	neg	AX	     ;yes
	jmps	dblIntglDone

dblIntgl7:
	test	DX,8000h	;special case at most negative
	jne	dblIntgl8

	neg32	DX,AX		;yes
	jmps	dblIntglDone

dblIntgl4:
	mov	BL,1
	cmp	SI,-1		;is guard bit needed
	je	dblIntgl5
	or	BH,80h		;make guard bit into sticky bit
dblIntgl5:
	clr	AX
	mov	DX,AX
	jmps	dblIntgl2
dblIntglInexact:
	feexcept FE_INEXACT	;no longer exact
dblIntglZero:
	clr	AX		;result is 0
	cwd
	jmps   dblIntglDone

dblIntglInvalid:
	feexcept FE_INVALID

dblIntgldone:
	_pop	<DI,SI>
	add	SP,stackSize
	pop	BP
	ret
dblIntgl8:
	or	DX,AX		;special case when integer is max neg
	cmp	DX,8000h
	je	dblIntgldone
	jmp	dblIntglInvalid
c_endp	_DBLLNG@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
	b	 =	P

func	__dtype
	push	BP
	mov	BP,SP
	_push	<SI,DI>
	mov	AX,b+6[BP]
	mov	BX,b+4[BP]
	mov	CX,b+2[BP]
	mov	DX,b+[BP]		;mov b into registers
	call	dget_dtype
	mov	AX,SI
	_pop	<DI,SI>
	pop	BP
	ret
c_endp	 __dtype

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; float
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; get special case index of  float
;	index = case(a)
; input:
;	[DX,AX] = a
; Output:
;	SI = index
;
public	    fget_dtype
fget_dtype proc near
	clr	SI				;set to default
	test	DX,07fffh
	jz	fget_dtype1

	mov	DI,DX
	and	DI,shortexp				;mask exponent
	_if	DI ne short_infinity, fget_dtype2	;c is not NaN or infinite
	mov	DI,DX
	test	DI,fqnan_bit				;bit must be set to be
	jz	fget_dtype3				;a quiet NaN
	mov	SI,dtype_qnan				;c is a quiet NaN
	ret

fget_dtype3:
	and	DI,07fh					;Mantissa must be zero
	or	DI,AX					;to be infinite
	jz	fget_dtype4				;otherwise
	mov	SI,dtype_snan				;c is a signaling NaN
	ret

fget_dtype4:						;c is infinite
	mov	SI,dtype_infinite
	ret

fget_dtype1:
	or	AX,AX
	jnz	fget_dtype2
	mov	SI,dtype_zero				;c is zero

fget_dtype2:
	ret
fget_dtype endp


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Unpack a float. The float must not be 0.
; Input:
;	[DX,AX] = the float (with sign = 0)
; Output:
;	[DX,AX] = significand (with hidden bit in DX bit 15)
;	SI	exponent
;	DI	sign (in bit 15)

public funnorm
funnorm proc	near
	mov	DI,DX		;save sign
	test	DI,shortexp
	jne	funnorm1
	clr	SI
	call	fleft_justify
	jmps	funnorm2

funnorm1:
	shl	DX,1
	mov	DL,DH
	clr	DH
	mov	SI,DX		;SI = exponent (AX >> 7)
	mov	DX,DI
	or	DL,shorthid	;or in hidden bit (80h)

funnorm2:
	mov	DH,DL
	mov	DL,AH
	mov	AH,AL
	clr	AL

	ret
funnorm endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Normalize and pack a float.
; Input:
;	[DX,AX] = significand
;	SI = exponent (biased)
;	DI bit 15 = sign of result
;
public	fnorm
fnorm	proc	near
	sub	SI,8		;offset
	even
fnorm1:
	tst	DH		;do we need to shift right?
	jz	fnorm3		;no
fnorm2:
	shr	DX,1
	rcr	AX,1
	inc	SI		;exponent
	jnc	fnorm1		;no rounding
	tst	DH		;done shifting?
	jnz	fnorm2		;no
	inc	AX		;round up by 1
	jnz	fnorm3		;no carry (also, we're done shifting)
	inc	DX
	jmp	fnorm1

				;see if our significand is 0
fnorm3:
	tst	DX
	jnz	fnorm4
	tst	AX
	jnz	fnorm4
	mov	DX,shorthid
	clr	SI		;trick fnorm4 into giving us a 0 result

fnorm4:
	test	DL,shorthid	;hidden bit in right spot?
	jnz	fnorm5		;yes
	shl	AX,1
	rcl	DX,1		;shift left till it is
	dec	SI
	jmp	fnorm4

fnorm5:
	_if SI ge 0, fpack
	shl32by8
	shr32	DX,AX
	dec	SI
	call	fright_justify
	jmp	fround7
fnorm	endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pack a float.
; Input:
;	[AX,DX] = significand
;	SI = exponent (biased)
;	DI bit 15 = sign of result
;
;	CX is lost
fpack	proc	near
	mov	CX,SI
	mov	CH,CL
	clr	CL
	mov	SI,CX
	shr	SI,1
	_if	SI ae short_infinity, fpackOverflow

	and	DL,shorthid - 1	;dump hidden bit
	or	DX,SI		;install exponent
	and	DI,sgn		;mask sign bit
	or	DX,DI		;install sign
fpack2:	ret

fpackOverflow:
	jmp	fexception	;raise overflow exception
fpack	endp


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Left justify mantissa when
; exponent is zero
;
;  Input:
; [DX,AX] = float
; SI		= Exponent
;
public fleft_justify
fleft_justify proc near
	mov	SI,1

fleft_justify1:
	dec	SI			;Adjust exponent
	shl32	DL,AX			;shift mantissa left
	test	DL,shorthid		;is it shifted enough
	je	fleft_justify1		;no
	ret
fleft_justify endp
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Right justifty mantissa when
; exponent is less then zero
;
;  Input:
;  [DX,AX]  = float
;  SI	    = Exponent
;
public fright_justify

fright_justify proc near
	_if SI l -24, fright_justify5

fright_justify1:
	_if	SI g -8, fright_justify2
	sh32StyRBy8
	add	SI,8
	jnz	fright_justify1

fright_justify2:
	xchg	CX,SI
	jcxz	fright_justify3	;no shifting need be done
	neg	CX
	or	AL,AL		;If any sticky bits are set then
	je	fright_justify4	;make sure they don't all rotate out
	or	AL,40h

fright_justify4:
	shr32	DX,AX
	loop	fright_justify4

fright_justify3:
	xchg	CX,SI		;restore CX, SI = 0
	ret

fright_justify5:
	or	AX,DX
	clr	DX
	or	AL,AH		;save sticky bits
	je	fright_justify6
	mov	AL,1
	jmps	fright_justify7

fright_justify6:
	mov	AL,DL
	
fright_justify7:
	mov	AH,DH		;clear mantissa
	mov	SI,DX
	ret
fright_justify endp


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Round and normalize short float
;Input:
;	[DX,AH] = significand
;	assume bit 7 or 8 of DH is set
;	if bit 8 of DH is set then
;		 all of AL make up the sticky bits and bit
;		1 in AH is the guard bit.
;	else
;		bit 7 of AL is the guard bit, the rest of the
;		bits int AL make up the sticky bits and bit
;	SI = exponent (biased)
;	DI (sign bit)
;
public	 fround
fround	proc	near
	dec	SI		;Make sure exponent is correct if not shifted
	test	DH,80h		;if bit 8 of AH is set then
	jz	fround1		;adjust shift register and
	shr32	DX,AX
	jnc	fround12
	or	AL,1		;make sure sticky bit dit not drop off

fround12:
	inc	SI		;adjust exponent

fround1:
	_if	SI l 0ffh, fround11
	jmp	fpackOverflow

fround11:
	_if SI g 0, fround7
	dec	SI		;adjustment so it will shift ok
	call	fright_justify	;shift right until SI is zero

fround7:
	test	AL,01000000b
	jne	fround6
	test	AL,00111111b
	je	fround5

fround6:
	tst	SI
	jne	fround10
	feexcept FE_UNDERFLOW
fround10:
	feexcept FE_INEXACT		;no longer exact
    ifdef _MT
	push	AX
	feround				;get rounding mode in AX
	_if	AX e FE_TONEAREST, fround3
	_if	AX e FE_UPWARD,    fround2
	cmp	AX,FE_TOWARDZERO
	pop	AX
	je	fround5
    else
	_if	__fe_cur_env.round e FE_TONEAREST, fround3
	_if	__fe_cur_env.round e FE_TOWARDZERO,fround5
	_if	__fe_cur_env.round e FE_UPWARD,    fround2
    endif

	;FE_DOWNWARD
	test	DI,DI
	jns	fround5
	jmps	fround4


fround2:;FE_UPWORD
    ifdef _MT
	pop	AX
    endif
	test	DI,DI
	js	fround5
	jmps	fround4


fround3:;FE_TONEAREST
    ifdef _MT
	pop	AX
    endif
	test	AL,01000000b	;If guard bit is not set then
	jz	fround5		;no rounding is necessary
	test	AL,10111111b	;Test if odd or sticky bits are set
	jz	fround5


fround4:;round up
	add	AX,80h
	adc	DX,0
	tst	SI		;is exponent zero
	je	fround9		;yes

	test	DH,80h		;is msb still where is should be
	jz	fround5
	inc	SI
	shr32	DX,AX

fround5:
	shl32	DX,AX
	sh32RBy8
	jmp	fpack

fround9:			;when exponent is zero
	test	DH,40h		;is msb still where is should be
	jz	fround5		;yes
	inc	SI		;exponent is now 1
	jmps	fround5
fround	endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Raise overflow exception
; Input:
;	DI	bit 15 is sign bit of result
; Returns:
;	AX,DX	adjusted result

fexception proc near
	feexcept <FE_OVERFLOW or FE_INEXACT>
	mov	DX,shortexp

	;Adjust infinity based on rounding mode
	;NEAREST	infinity with sign
	;DOWN		+ overflows to + largest finite, - overflows to -inf
	;UP		- overflows to - largest finite, + overflows to +inf
	;TOZERO		to signed largest finite

    ifdef _MT
	feround				;get rounding mode in AX
	_if	AX e FE_TONEAREST,  fexcpToNearest
	_if	AX e FE_TOWARDZERO, fexcpTowardZero
	cmp	AX,FE_UPWARD
	mov	AX,0
	je	fexcpUpward
    else
	clr	AX
	_if	__fe_cur_env.round e FE_TONEAREST, fexcpToNearest
	_if	__fe_cur_env.round e FE_TOWARDZERO, fexcpTowardZero
	_if	__fe_cur_env.round e FE_UPWARD, fexcpUpward
    endif

	tst	DI
	js	fexcp1
	jmps	fexcpTowardZero

fexcpUpward:
	tst	DI
	jns	fexcp1

	;Generate largest finite
fexcpTowardZero:
	mov	DX,07F7Fh
    ifdef _MT
	mov	AX,0FFFFh	;to 7F7F FFFF
	jmp	short fexcp1

fexcpToNearest:
	clr	AX
    else
	dec	AX		;to 7F7F FFFF

fexcpToNearest:
    endif
fexcp1:
	and	DI,sgn
	or	DX,DI		;install sign bit
	ret
fexception endp


	endcode	double

	end
