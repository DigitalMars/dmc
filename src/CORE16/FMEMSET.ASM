;_ fmemset.asm   Sat July 28, 1990   
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set n bytes in s to c.
;	char __far *fmemset(char __far *p,int c,int n)
; Returns:
;	p

	ife LPTR and LCODE
	begcode	strings
	public	_fmemset

_fmemset	proc far
	push	BP
	mov	BP,SP
	.save	<DI>
	les	DI,6+[BP]	;p
	mov	BX,DI		;Return value.
	mov	CX,6+4+2[BP]	;n
	mov	AL,6+4[BP]	;c
	mov	AH,AL 		;Set up a 16 bit pattern.
	shr	CX,1
	rep	stosw
	rcl	CX,1
	rep	stosb

	mov	DX,ES		;return original value of p in DX,AX
	mov	AX,BX		;Return value.
	.restore <DI>
	pop	BP
	ret
_fmemset	endp
	endcode	strings
	endif
	end


