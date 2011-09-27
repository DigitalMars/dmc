;_ ldexp.asm   Mon Dec 18 1989	 Modified by: Phil Hinger */
; Written by Walter Bright
; Copyright (C) 1984-1991 by Walter Bright
; All rights reserved

include macros.asm
include flthead.asm

	.8087

    ifdef _MT
      if LCODE
	extrn	__FEEXCEPT:far
      else
	extrn	__FEEXCEPT:near
      endif
    endif

	begdata
	c_extrn	_8087,word

    ifndef _MT
	extrn	__fe_cur_env:word
    endif

	enddata




	begcode double

	extrn	dunnorm:near, dround:near, dget_dtype:near, exception:near

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	double ldexp(value,exp)
;	double value;
;	int exp;
; Returns:
;	value*(2**exp)

	c_public ldexp


func	ldexp
	WINENTER
	_if	_8087 e 0, Ld2		;if 8087 not installed
	fild	word ptr P+8[BP]	;load exp
	fld	qword ptr P[BP]		;load value
	fscale				;ST(0) = ST(0) * (2**ST(1))
	fstp	qword ptr P[BP]
	fstp	ST			;leave stack as we found it
					;(also doing an fwait, MASM doesn't
					; recognize fnstp !@#$%^&)
	mov	AX,P+6[BP]
	mov	BX,P+4[BP]
	mov	CX,P+2[BP]
	mov	DX,P+0[BP]		;transfer result to AX..DX
	WINLEAVE
	ret

Ld2:	_push	<SI,DI>
	mov	AX,P+6[BP]
	mov	BX,P+4[BP]
	mov	CX,P+2[BP]
	mov	DX,P+0[BP]		;transfer double to AX..DX

	call	dget_dtype
	shl	SI,1
	jmp	word ptr cs:LdIndex[SI]

LdIndex label	    word
	dw	LdNormal	    ;other
	dw	LdZero		    ;zero
	dw	LdInfinite	    ;infinite
	dw	LdSNaN		    ;SNaN
	dw	LdQNaN		    ;QNaN

LdNormal:

	mov	SI,AX
	and	SI,longexp	    ;mask off exponent bits
	je	LdSubNormal	    ;special case for subnormal

	xor	AX,SI		    ;clear exponent bits in AX

	shr	SI,1
	shr	SI,1
	shr	SI,1
	shr	SI,1		    ;right justify exponent

	add	SI,P+8[BP]	    ;add exp
	jle	Ld7		    ;test for underflow

	cmp	SI,7ffh		    ;test for  overflow
	jge	Ld6		    ;yes

	shl	SI,1
	shl	SI,1
	shl	SI,1
	shl	SI,1
;	and	SI,longexp	;dump extraneous bits (not necessary)
	or	AX,SI		;install exponent
;	jmps	LdDone

LdZero:
LdInfinite:
LdQNaN:

LdDone:
       _pop	<DI,SI>
	WINLEAVE
	ret

Ld6:	;overflow
	mov	DI,AX
	feexcept <FE_OVERFLOW or FE_INEXACT>
	call	exception	;raise overflow exception
	jmps	LdDone

Ld7:	;underflow

	; AX,BX,CX,DX <<= 11	 normalize so dround will work
	mov	DI,AX		 ;save sign bit
	or	AX,longhid
	sh64LBy8
	shl64	AX,BX,CX,DX
	shl64	AX,BX,CX,DX
	shl64	AX,BX,CX,DX
	call	dround
	jmps	LdDone


LdSNaN:
	or	AX,dqnan_bit
	feexcept FE_INVALID
	jmps	LdDone


LdSubNormal: ; do it the slow but safe way
	call	dunnorm			;unpack
	add	SI,P+8[BP]		;add exp
	call	dround
	jmps	LdDone
c_endp	ldexp


	endcode double


	end
