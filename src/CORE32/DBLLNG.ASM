;_dbllng.asm
;$Header: z:/core/crtl/src/core32/dbllng.asm,v 1.1.1.1 1997/01/02 19:17:46 smarx Exp $
;Copyright (C) 1994 by Digital Mars
;All Rights Reserved, Written by Walter Bright and Phil Hinger
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



	begdata
	extrn	__8087:word		;8087 flag word

    ifndef _MT
	extrn	__fe_cur_env:word
    endif

	enddata

	begcode	double

    ifndef _INLINE_8087
	extrn	__ULNGDBL@:near
    endif

	extrn	dnorm:near
	extrn	dget_dtype:near
	extrn	dunnorm:near

      MinBitShift =  0
       stackSize  =  MinBitShift + 4


	public	__LLNGDBL@
__LLNGDBL@ proc	near
	test	EDX,EDX
	jz	L2
	jns	L1
	neg64
	stc
L1:	_push	<ESI,EDI>
	rcr	EDI,1
	mov	ESI,63+longbias
	call	dnorm
	_pop	<EDI,ESI>
	ret
__LLNGDBL@ endp

	public	__ULLNGDBL@
__ULLNGDBL@ proc near
	test	EDX,EDX
	jnz	L1
L2:
    ifdef _INLINE_8087
	jmp	L1
    else
	jmp	__ULNGDBL@
    endif
__ULLNGDBL@ endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	public	__DBLULLNG@
__DBLULLNG@ proc near
	push	EBP
	sub	ESP,stackSize
	mov	EBP,ESP
	mov	dword ptr MinBitShift[EBP],63
	test	EDX,sgn
	jz	dblIntgl

	feexcept FE_INVALID
	add	ESP,stackSize
	pop	EBP
	ret
__DBLULLNG@ endp

	public	__DBLLLNG@
__DBLLLNG@ proc	near
	push	EBP
	sub	ESP,stackSize
	mov	EBP,ESP
	mov	dword ptr MinBitShift[EBP],62
	test	EDX,sgn
	jz	short dblIntgl

	inc	dword ptr MinBitShift[EBP]

dblIntgl:
	_push	<ESI,EDI,EBX,ECX>
	call	dget_dtype
	jmp	dword ptr cs:dblIntglIndex[ESI*4]

dblIntglIndex label	dword
    if _FLAT
	dd	offset FLAT:dblIntglNormal		  ;other
	dd	offset FLAT:dblIntglZero		  ;zero
	dd	offset FLAT:dblIntglInvalid		  ;infinity
	dd	offset FLAT:dblIntglInvalid		  ;SNaN
	dd	offset FLAT:dblIntglInvalid		  ;QNaN
    else
	dd	dblIntglNormal		  ;other
	dd	dblIntglZero		  ;zero
	dd	dblIntglInvalid		  ;infinity
	dd	dblIntglInvalid		  ;SNaN
	dd	dblIntglInvalid		  ;QNaN
    endif

dblIntglNormal:
	call	dunnorm		    ;unpack double
	clr	EBX
	sub	ESI,longbias	    ;un-bias the exponent
	js	dblIntgl4	    ;for neg exponents, the result is 0
	_ifs	ESI a MinBitShift[EBP], dblIntglInvalid
	mov	ECX,63
	sub	ECX,ESI
	jcxz	dblIntgl2

	_align
dblIntgl3:
	shr	EDX,1
	rcr	EAX,1
	rcr	BH,1		;keep sticky bit
	adc	BL,0		;keep guard bit
	loop	dblIntgl3

dblIntgl2:
	mov	ESI,MinBitShift[EBP]
	tst	EBX		;is guard or sticky bit set?
	je	dblIntgl6	;no
	feexcept FE_INEXACT	;no longer exact
dblIntgl6:
	test	EDI,sgn		;is result negative?
	js	dblIntgl8
	jmps	dblIntglDone	;no


dblIntgl4:
	mov	BL,1		;set guard bit
	cmp	ESI,-1		;is guard bit needed
	je	dblIntgl5
	or	BH,80h		;make guard bit into sticky bit
dblIntgl5:
	clr	EAX
	mov	EDX,EAX
	jmps	dblIntgl2

dblIntglInexact:
	feexcept FE_INEXACT	;no longer exact
dblIntglZero:
	clr	EAX		 ;result is 0
	mov	EDX,EAX
	jmps   dblIntglDone

dblIntglInvalid:
	feexcept FE_INVALID

dblIntglDone:
	_pop	<ECX,EBX,EDI,ESI>
	add	ESP,stackSize
	pop	EBP
	ret

dblIntgl8:
	cmp	EDX,080000000h
	ja	dblIntglInvalid
	jb	L9
	test	EAX,EAX
	ja	dblIntglInvalid
	je	dblIntglDone

L9:	neg64
	jmp	dblIntglDone
__DBLLLNG@ endp

	endcode	double

	end
