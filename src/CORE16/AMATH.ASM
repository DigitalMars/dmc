;_ amath.asm
; Copyright (C) 1993 by Digital Mars
; All Rights Reserved
; Written by Walter Bright
; Compatibility helper functions

include	macros.asm

    if LCODE
	extrn	__LMUL@:far,__LDIV@:far,__ULDIV@:far
    else
	extrn	__LMUL@:near,__LDIV@:near,__ULDIV@:near
    endif

	begcode	lmath

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; DX,AX <<= CL

	public	__aFlshl
__aFlshl proc	far
	xor	CH,CH
	jcxz	J2
J1:	shl	AX,1
   	rcl	DX,1
	dec	CX
	jnz	J1
J2:	ret
__aFlshl endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Alternate long multiply
;	DX,AX = 8[BP],6[BP] * 0Ch[BP],0AH[BP]
; CX is destroyed

	public	__aFlmul,__aFulmul
__aFlmul proc	far
__aFulmul:
	push	BP
	mov	BP,SP
	push	BX

	mov	DX,8[BP]
	mov	AX,6[BP]
	mov	CX,0Ch[BP]
	mov	BX,0Ah[BP]
	call	__LMUL@

	pop	BX
	pop	BP
	ret	8
__aFlmul endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Alternate signed long divide.
;	DX,AX = 8[BP],6[BP] / 0Ch[BP],0Ah[BP]
; CX is destroyed

	public	__aFldiv
__aFldiv proc	far
	push	BP
	mov	BP,SP
	_push	<BX,SI,DI>

	mov	DX,8[BP]
	mov	AX,6[BP]
	mov	CX,0Ch[BP]
	mov	BX,0Ah[BP]
	call	__LDIV@

J3:	_pop	<DI,SI,BX>
	pop	BP
	ret	8
__aFldiv endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Alternate unsigned long divide.
;	DX,AX = 8[BP],6[BP] / 0Ch[BP],0Ah[BP]
; CX is destroyed

	public	__aFuldiv
__aFuldiv proc	far
	push	BP
	mov	BP,SP
	_push	<BX,SI,DI>

	mov	DX,8[BP]
	mov	AX,6[BP]
	mov	CX,0Ch[BP]
	mov	BX,0Ah[BP]
	call	__ULDIV@

	jmp	J3
__aFuldiv endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Do a long multiply.
;	long _aNNalmul(long *pda,long cb);	// C++ calling sequence
; Input:
;	*pda	first factor (da)
;	cb	second factor
; Output:
;	*pda = da * cb
; Returns:
;	da * cb

    if LCODE
		public __aFNalmul,__aFNaulmul
__aFNalmul	proc far
__aFNaulmul:
    else
		public __aNNalmul,__aNNaulmul
__aNNalmul	proc near
__aNNaulmul:
    endif
	push	BP
	mov	BP,SP
	mov	BX,P[BP]	;BX -> second factor
	mov	AX,P+2[BP]
	mul	word ptr 2[BX]
	mov	CX,AX		;CX = acl
	mov	AX,P+4[BP]
	mul	word ptr [BX]
	add	CX,AX		;CX = acl + bdl
	mov	AX,P+2[BP]
	mul	word ptr [BX]	;DX,AX = abh,abl
	add	DX,CX		;DX = acl + bdl + abh
	mov	[BX],AX
	mov	2[BX],DX	;*BX gets result
	mov	SP,BP
	pop	BP
	ret	6

    if LCODE
__aFNalmul	endp
    else
__aNNalmul	endp
    endif

	endcode	lmath

	end
