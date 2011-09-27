;_ math87.asm	Mon Dec 18 1989   Modified by: Walter Bright */
; Copyright (C) 1987-1994 by Digital Mars
; All Rights Reserved
; Written by Walter Bright

;Define this if we wish to support machines with an 80287 on a 386
SUPPORT287 equ	1

	include	macros.asm
	include flthead.asm

    ifdef _MT
	c_extrn	_FEEXCEPT,near
    endif

fncompp	macro
	db	0D8h+6,0D9h		;MASM doesn't recognize FNCOMPP
	endm

fntst	macro
	db	0D8h+1,0E4h		;MASM doesn't recognize FNTST
	endm

fnabs	macro
	db	0D8h+1,0E1h		;MASM doesn't recognize FNABS
	endm

_fsin	macro				;bugs in MASM 5.0
	db	0D8h+1,0FEh
	endm

_fcos	macro				;bugs in MASM 5.0
	db	0D8h+1,0FFh
	endm

_fprem1	macro				;bugs in MASM 5.0
	local	FP1,FP2
	cmp	_fdiv_chk_flag,1
	je	short FP1
	call	fprem1_chk		;;work around Pentium bug
	jmp	short FP2
FP1:	db	0D8h+1,0F5h		;;MASM 5.0 won't do FPREM1
FP2:
	endm

_fpbug	macro	fpop,fpop_chk
	local	FP1,FP2
	cmp	_fdiv_chk_flag,1
	je	short FP1
	call	fpop_chk		;;work around Pentium bug
	jmp	short FP2
FP1:	fpop
FP2:
	endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Values for exception.type (MUST match values in math.h)

DOMAIN		equ	1
SING		equ	2
OVERFLOW	equ	3
UNDERFLOW	equ	4
TLOSS		equ	5
PLOSS		equ	6

;Bits for condition code flags in 8087 status word
C0	equ	00000001B
C1	equ	00000010B
C2	equ	00000100B
C3	equ	01000000B

	begdata

	extrn	__8087:word
	extrn	_fdiv_chk_flag:dword

PIOVER4	dt	3FFEC90FDAA22168C235R
SQRT2	dt	3FFFB504F333F9DE6484R
 if 0			; masm386.exe Version 5.10A.15
ONEHALF	dd	0.5
 else			; masm386.exe after Version 5.NT.02
ONEHALF	dd	3F000000R
TWO	dd	40000000R
 endif

    ifndef _MT
	extrn	__fe_cur_env:word
    endif

	enddata

	extrn	__arcerr:near, ___trigerr:near
	extrn	_fdiv_r:near
	extrn	_fptan_chk:near
	extrn	_fatan_chk:near
	extrn	fprem_chk:near
	extrn	fprem1_chk:near

	begcode	math87

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Square root
; Errors aren't checked for, should be checked by caller.
;	double _sqrt87(double);

	c_public	_sqrt87
func	_sqrt87
    if LONGDOUBLE
	fld	tbyte ptr PS[ESP]
    else
	fld	qword ptr PS[ESP]
    endif
	fsqrt
    ife _RETST0
	fstp	qword ptr PS[ESP]
	fwait
	mov	EAX,PS[ESP]
	mov	EDX,PS+4[ESP]
    endif
	ret
c_endp	_sqrt87

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 8087 equivalent to poly().
;	double _poly87(double x, int deg, double coeff[]);

	c_public	_poly87
func	_poly87
	push	EBP
	mov	EBP,ESP
	mov	ECX,P+8[EBP]		;ECX = deg
	mov	EDX,ECX
	shl	EDX,3			;* 8 to get offset into doubles
	add	EDX,P+8+4[EBP]
	fld	qword ptr [EDX]		;ST0 = coeff[deg]
	sub	ESP,8			;make room for result at -8[EBP]
	jecxz	return_ST
	fld	qword ptr P[EBP]	;ST0 = x
	fxch	ST(1)			;ST1 = x, ST0 = r
	_align
L2:	fmul	ST(0),ST(1)		;r *= x
	sub	EDX,8			;deg--
	fadd	qword ptr [EDX]
	loop	L2
	fxch	ST(1)			;ST1 = r, ST0 = x
	fstp	ST(0)			;dump x

	_align
return_ST:
    if 0
	fstsw	AX		    ; store 8087 status word in AX
	fwait
	and	AX,FE_ALL_EXCEPT
	feexcept EAX
    endif

    if _RETST0
	mov	ESP,EBP
    else
	fstp	qword ptr -8[EBP]
	fwait
	_pop	<EAX,EDX>
    endif
	pop	EBP
	ret
c_endp	_poly87


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 8087 versions of floor() and ceil().
; Also we have _chop87(), which rounds towards 0.

	c_public _chop87,_floor87,_ceil87

func	_chop87
	mov	AH,3 SHL 2	;round towards 0 (chop)
	jmps	FC1
c_endp	_chop87

func	_floor87
	mov	AH,1 SHL 2	;round towards - infinity
	jmps	FC1
c_endp	_floor87

func	_ceil87
	mov	AH,2 SHL 2	;round towards + infinity
FC1:	push	EBP
	mov	EBP,ESP
    if LONGDOUBLE
	fld	tbyte ptr P[EBP]	;get parameter
    else
	fld	qword ptr P[EBP]	;get parameter
    endif
	sub	ESP,8		;allocate space for return value and temps
	fstcw	-2[EBP]		;need to manipulate control word
	fwait
	mov	AL,-1[EBP]	;get high byte of control word
	mov	DL,AL		;and save original
	and	AL,11110011b	;clear old rounding control bits
	or	AL,AH		;set new rounding control
	mov	-1[EBP],AL
	;fnldcw	-2[EBP]		;load new rounding control
	db	0D8h+1,06Dh,-2
	frndint			;and round
	mov	-1[EBP],DL
	fldcw	-2[EBP]		;restore original control word
	jmp	return_ST	;return
c_endp	_ceil87

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 8087 version of fmod(x,y).

	c_public	_fmod87
func	_fmod87
	push	EBP
	mov	EBP,ESP
	sub	ESP,8			;scratch and return value
    if LONGDOUBLE
	fld	tbyte ptr P+12[EBP]
    else
	fld	qword ptr P+8[EBP]
    endif
;	ftst
;	fstsw	-2[EBP]
;	fwait
;	mov	AH,-1[EBP]		;get msb of status word in AH
;	sahf				;transfer to flags
;	je	FM2			;y is 0, return 0
    if LONGDOUBLE
	fld	tbyte ptr P[EBP]	;ST = x, ST1 = y
    else
	fld	qword ptr P[EBP]	;ST = x, ST1 = y
    endif
FM1:	_fpbug	fprem,fprem_chk		;ST = ST - ST1
	fstsw	-2[EBP]
	fwait
	mov	AH,-1[EBP]		;get msb of status word in AH
	sahf				;transfer to flags
	jp	FM1			;continue till ST < ST1
	fstp	ST(1)			;leave remainder on stack
FM2:	jmp	return_ST
c_endp	_fmod87

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compute arc tan of y/x.
; Cases of y/0 and 0/0 are already handled.

	c_public	_atan287
func	_atan287
	push	EBP
	mov	EBP,ESP

	;Must adjust things so that (0 < y < x < infinity).
    if LONGDOUBLE
	fld	tbyte ptr P[EBP]		;get y
    else
	fld	qword ptr P[EBP]		;get y
    endif
	sub	ESP,8			;room for return value
	ftst
	fstsw	-2[EBP]			;save sign of y
    if LONGDOUBLE
	fld	tbyte ptr P+12[EBP]	;get x
    else
	fld	qword ptr P+8[EBP]	;get x
    endif
AT4:	ftst
	fstsw	-4[EBP]			;save sign of x

	_ifs	_fdiv_chk_flag e 1, AT5
	mov	EAX,0Dh
	call	_fdiv_r			;work around Pentium FDIV bug
	jmp	short AT6

AT5:	fdivp	ST(1),ST		;compute z=y/x

AT6:	fabs
	fld1
	fcom				;see if 0th octant or 1st octant
	fstsw	-6[EBP]
	fwait
	mov	AH,-5[EBP]
	sahf
	ja	oct0			;0 <= angle < 45
	jb	oct1			;45 < angle < 90

	;Angle is 45 degrees (because z == 1)
	fncompp				;clear stack
	fld	PIOVER4			;return PI/4
	jmps	AT1

oct1:	;Must use identity atan(z) == PI/2 - atan(1/z)
	fxch	ST(1)
	_fpbug	fpatan,_fatan_chk	;ST(0) == atan(1/z)
	fld	PIOVER4
	fadd	ST,ST(0)		;compute PI/2
	fsubrp	ST(1),ST		;compute atan(z)
	jmps	AT1

oct0:	_fpbug	fpatan,_fatan_chk	;tangent of z/1

AT1:	;Determine, based on the original signs of x and y, which
	;quadrant the angle should be in.
	;The 4 cases are:
	; 1:	x > 0 && y >= 0:	return ST
	; 2:	x > 0 && y <  0:	return -ST
	; 3:	x < 0 && y >= 0:	return PI - ST
	; 4:	x < 0 && y <  0:	return -(PI - ST)

	mov	AH,-3[EBP]
	sahf
	ja	AT2			;x > 0, so cases 1 and 2
	fldpi
	fsubrp	ST(1),ST
AT2:	mov	AH,-1[EBP]
	sahf
	jae	AT3			;y >= 0
	fchs
AT3:	jmp	return_ST
c_endp	_atan287

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Do arcsin's and arccos's.
; Input:
;	double _asincos87(int flag,double x);
;
;	flag = 0: asin
;	       1: acos

	c_public	_asincos87
func	_asincos87
	push	EBP
	mov	EBP,ESP

	;Use the identities:
	;	asin(x) = atan2(x,sqrt(1-x*x))
	;	acos(x) = atan2(sqrt(1-x*x),x)

    if LONGDOUBLE
	fld	tbyte ptr P+4[EBP]	;ST = x
    else
	fld	qword ptr P+4[EBP]	;ST = x
    endif
	sub	ESP,8
	ftst
	fstsw	-2[EBP]			;remember flags for atan2()
	;Compute (1-x*x)
	fld	ST			;copy x
	fmul	ST,ST			;ST = x * x
	fld1
	fsubrp	ST(1),ST		;ST = 1 - x * x
	;If (1-x*x) < 0, we have a domain error
	ftst
	fstsw	-6[EBP]
	fwait
	mov	AH,-5[EBP]
	sahf
	jae	ASC1			;no domain error
	fncompp				;clean 8087 stack
	mov	ESP,EBP
	pop	EBP
	jmp	__arcerr		;_arcerr(flag,x) handles the exception

ASC1:	fsqrt				;sqrt(1 - x * x)
	;If acos, we need to reverse the arguments to atan2()
	_ifs	<word ptr P[EBP]> e 0, ASC2	;if asin
	ftst
	fstsw	-2[EBP]			;flags of y
	fxch	ST(1)
ASC2:	jmp	AT4			;let atan2(y,x) do the rest
c_endp	_asincos87

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Do sine, cosine and tangent of x.
; Use:
;	double _sincostan87(double x,int flag);
; Input:
;	flag:	0 = sine
;		1 = cosine
;		2 = tan

    if LCODE
trigerr	proc	far
    else
trigerr	proc	near
    endif
	;Total loss of precision
TR2:	fstp	ST
	fstp	ST			;clean stack
	sub	ESP,4+8+8+4-8		;already 8 on stack
    if LONGDOUBLE
	fld	tbyte ptr P[EBP]
    else
	fld	qword ptr P[EBP]
    endif
	mov	-4[EBP],EDX		;flag
	fstp	qword ptr [EBP-4-8]	;x
	fldz
	fstp	qword ptr [EBP-4-8-8]		;return value of 0.0
	mov	dword ptr [EBP-4-8-8-4],TLOSS	;total loss of significance
	fwait
	call	___trigerr
	mov	ESP,EBP
	pop	EBP
	ret
trigerr	endp

	c_public	_sincostan87
func	_sincostan87
	push	EBP
	mov	EBP,ESP
	sub	ESP,8
    if LONGDOUBLE
	mov	EDX,P+12[EBP]		;DL = flag
    else
	mov	EDX,P+8[EBP]		;DL = flag
    endif

    if SUPPORT287
	_ifs	__8087 b 3, SC11	;if 80287
    endif

	;Take advantage of special 80387 instructions
    if LONGDOUBLE
	fld	tbyte ptr P[EBP]	;load theta
    else
	fld	qword ptr P[EBP]	;load theta
    endif
	fxam				;test for oddball values
	fstsw	AX
	sahf
	jc	trigerr			;x is NAN, infinity, or empty
					;387's can handle denormals
SC18:	tst	DL			;sine?
	jnz	SC12			;no
	_fsin
	jmps	SC13
SC12:	_ifs DL ne 2, SC15
	_fpbug	fptan,_fptan_chk
	fstp	ST			;dump X, which is always 1
	jmps	SC13
SC15:	_fcos
SC13:	fstsw	AX
	sahf
	jp	SC16			;C2 = 1 (x is out of range)
	jmp	return_ST

	;Need to do argument reduction
SC16:	fldpi
	fxch
SC17:	_fprem1
	fstsw	AX
	sahf
	jp	SC17
	fstp	ST(1)			;remove pi from stack
	jmp	SC18

    if SUPPORT287
SC11:	fld	PIOVER4			;pi/4
    if LONGDOUBLE
	fld	tbyte ptr P[EBP]	;load theta
    else
	fld	qword ptr P[EBP]	;load theta
    endif
	fxam				;test for oddball values
	fstsw	-6[EBP]
	fwait
	mov	AL,-5[EBP]
	mov	AH,AL			;C1 gives sign of x, save in AL
	sahf				;C3 == Z, C2 == P, C0 == C
	jc	SC14			;x is NAN, infinity, or empty
	jp	SC3			;x is normal or denormal
	jnz	SC3			;x is unnormal
	;x is 0
	;fnstp	ST(1)			;get rid of pi/4
	db	0D8h+5,0D9h		;I thought assemblers were supposed
					;to eliminate machine code
	_if	DL ne 1, SC2		;if sin or tan, return 0
	fstp	ST
	fld1
	jmp	SC2			;cos(0) is 1

SC14:	jmp	TR2

SC3:	;Attempt to reduce x so that |r| < PI/4
	fnabs
	fprem				;also normalize ST if unnormal
	fstsw	-4[EBP]
	fwait
	mov	AH,-3[EBP]
	sahf
	jp	SC14 ;(trigerr)		;|x| > 2**62, so total loss of precision
					;BUG: what about partial loss?
	_ifs	DL e 2, TAN		;do tangent
	;skip section if sin
	tst	DL
	jz	SC4			;if sin

	;Since cos(-x) == cox(x), ignore sign of angle
	and	AL,not C1

	;Since cos(x) == sin(x + pi/2), add 2 octants to the
	;octant formed by C0,C3,C1
	xor	AH,C3
	test	AH,C3
	jnz	SC4
	xor	AH,C0			;carry

SC4:	;If argument was in octants 1,3,5,7 then replace x with pi/4-x
	test	AH,C1
	jz	SC5			;in an even octant
	;fnsubp	ST(1),ST		;pi/4 is also removed
	db	0D8h+6,0E9h		;maybe someday MASM will support the 8087
	jmps	SC6

SC5:	;fptan will not work if x == 0, so if x == 0 then result is 1
	fntst
	fstsw	-2[EBP]
	fstp	ST(1)			;remove pi/4 from stack
	test	byte ptr -1[EBP],C3	;C3 is Z bit
	je	SC6
	fld1				;y/x is 0/1
	jmps	SC7

SC6:	fptan

SC7:	;If in octants 1,2,5,6
	;then use relation cos(angle) = x/sqrt(x*x + y*y)
	;else use relation sin(angle) = y/sqrt(x*x + y*y)

	test	AH,C3 + C1
	jp	SC8			;in octants 0,3,4,8
	fxch	ST(1)
SC8:
	;Compute ST1/sqrt(ST1*ST1 + ST0*ST0)
	fmul	ST,ST
	fld	ST(1)
	fmul	ST,ST
	faddp	ST(1),ST
	fsqrt
	fdivp	ST(1),ST

	;Figure out sign of result while fdiv is running
	and	AX,C0*256 + C1		;isolate sign of fprem and fxam
SC10:	or	AL,AH			;set parity = !(C0 ^ C1)
	jp	SC2
	fchs

SC2:	jmp	return_ST

TAN:	test	AH,C1			;test C1, if set then octants 1,3,5,7
	jz	TAN3			;octants 0,2,4,6
	fsubp	ST(1),ST		;ST == PI/4 - |r|
	jmps	TAN4

TAN3:	fstp	ST(1)			;get rid of pi/4
	;See if ST is 0
	ftst
	fstsw	-2[EBP]
	fwait
	test	byte ptr -1[EBP],C3	;check C3 (Z bit)
	jz	TAN4			;ST is not 0

	;fptan of 0 won't work, we fake it with 1/0
	fld1
	fxch	ST(1)
	jmps	TAN5

TAN4:	fptan
TAN5:	;Determine if we should take reciprocal (octants 1,2,5,6)
	and	AH,C3 or C1		;isolate bits C3 and C1
	jp	TAN7			;jmp if octants 0,3,4,7
	fdivrp	ST(1),ST
	jmps	TAN6

TAN7:	fdivp	ST(1),ST
TAN6:	;Determine if the sign should be switched
	and	AX,0100000000000010B	;isolate C3 in AH: octants 2,3,6,7
					;and C1 in AL: the sign bit of x
	jmp	SC10
    endif

c_endp	_sincostan87

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Do natural log of x, and log base 10 of x.
; The case of x <= 0 is assumed to be handled already.
; Use relations:
;	log(x) = loge(2) * log2(x)
;	log10(x) = log10(2) * log2(x)

	c_public	_log87,_log1087
func	_log87
	fldln2
	jmps	LOG1
c_endp	_log87

func	_log1087
	fldlg2
LOG1:
    if _RETST0
      if LONGDOUBLE
	fld	tbyte ptr PS[ESP]		;load x
      else
	fld	qword ptr PS[ESP]		;load x
      endif
	fyl2x
	ret
    else
	push	EBP
	mov	EBP,ESP
	fld	qword ptr P[EBP]		;load x
	fyl2x
	sub	ESP,8
	jmp	return_ST
    endif
c_endp	_log1087

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compute:
;	_exp87(y):		e ** y
;	_pow87(x,y,sign):	(sign) ? -(x ** y) : x ** y
; Note:
;	for _exp87(), values of y that will cause underflow or overflow
;	are handled previously.
;	for _pow87(), cases where x<=0 are already handled. Values of
;	y that will cause overflow or underflow will work correctly, but
;	will not trigger a call to matherr() as they should (BUG).

	c_public	_exp87,_pow87

func	_exp87
	;Use the formula exp(y) = 2 ** (y * log2(e))
	fldl2e
	push	EBP
	mov	EBP,ESP
    if LONGDOUBLE
	fld	tbyte ptr P[EBP]
    else
	fld	qword ptr P[EBP]
    endif
	xor	DH,DH			;want positive result
	fmulp	ST(1),ST		;ST = y * log2(e)
	jmps	__exp2_87
c_endp	_exp87

func	_pow87
	;Use the formula x ** y = 2 ** (y * log2(x))
	push	EBP
	mov	EBP,ESP
    if LONGDOUBLE
	fld	tbyte ptr P+12[EBP]	;push y
	fld	tbyte ptr P[EBP]	;push x	(0 < x < infinity)
	mov	DH,P+12+12[EBP]		;if DH != 0, then want negative result
    else
	fld	qword ptr P+8[EBP]	;push y
	fld	qword ptr P[EBP]	;push x	(0 < x < infinity)
	mov	DH,P+8+8[EBP]		;if DH != 0, then want negative result
    endif
	fyl2x				;ST = y * log2(x)

	public	__exp2_87
__exp2_87:
	;The common part computes (2 ** ST)
	;Use the formula:
	;	(2 ** ST) = (2 ** fraction(ST)) * (2 ** floor(ST))
	sub	ESP,8			;make room for result
	fld	ST			;duplicate ST

	;Compute ST = floor(ST)
	fstcw	-2[EBP]			;need to manipulate control word
	fwait
	mov	AL,-1[EBP]		;get high byte of control word
	mov	DL,AL			;and save original
	and	AL,11110011b		;clear old rounding control bits
	or	AL,00000100b		;round towards -infinity
	mov	-1[EBP],AL
	;fnldcw	-2[EBP]
	db	0D8h+1,06Dh,-2
	frndint
	mov	-1[EBP],DL
	fldcw	-2[EBP]			;restore original control word

	fsub	ST(1),ST		;ST(1) = fraction (0 <= ST < 1)
	fxch	ST(1)			;ST(1) = floor, ST = fraction

	;Compute ST = 2 ** ST
	;Note that 0 <= ST < 1
	;Use the relation:
	;	(2 ** ST) = (ST < .5) ? (2 ** ST) : (2 ** (ST-.5) * SQRT2)
	fld	ONEHALF
	fxch	ST(1)
	_fpbug	fprem,fprem_chk		;0 <= ST < .5
	fstsw	-2[EBP]			;remember if (.5 <= ST < 1)

	fstp	ST(1)			;dump ONEHALF
	f2xm1				;ST = (2 ** ST) - 1
	mov	AH,-1[EBP]
	test	AH,C1			;was (.5 <= ST < 1)?
	fld1
	faddp	ST(1),ST		;add in that 1
	jz	EXP1			;no
	fld	SQRT2			;yes
	fmulp	ST(1),ST
	jmp	short EXP4

EXP1:	test	AH,C3			; was ST == 1?
	jz	EXP4			; no
	fld	TWO
	fmulp	ST(1),ST

EXP4:	fscale				;ST *= 2 ** floor
	test	DH,DH			;do we want a negative result?
	fstp	ST(1)			;get rid of floor
	jz	EXP3			;result is positive
	fchs				;form negative result
EXP3:
    if _RETST0
	mov	ESP,EBP
	pop	EBP
	ret
    else
	jmp	return_ST
    endif

c_endp	_pow87

	endcode	math87

	end
