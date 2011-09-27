;_ fstrncpy.asm   Sat July 28, 1990  
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copy exactly n chars from s2 to s1, padding with nulls if necessary.
; Use:
;	char *__far fstrncpy(s1,s2,n)
;
 	ife LPTR and LCODE
	begcode	strings
	public	_fstrncpy
_fstrncpy proc far
	push    BP                                 
	mov     BP,SP                              
	.save	<SI,DI>
	les	DI,6+4[BP]
	clr	AX			;scan for 0
	mov	CX,-1			;largest possible string
;	cld
	repne	scasb			;find end of s2
	not	CX			;CX = strlen(s2) + 1 (for EOS)
	mov	DX,6+8[BP]
	_if	DX ae CX, L7		;if n >= strlen(s2) + 1
	mov	CX,DX			;CX = min(CX,n)
L7:	sub	DX,CX			;DX = # of nulls to pad
	push	DS
	les	DI,6[BP]
	lds	SI,6+4[BP]
	mov	BX,DI			;for return value
	shr	CX,1
	rep	movsw
	rcl	CX,1
	rep	movsb			;transfer bytes (including EOS)
	mov	AX,CX			;AX = 0
	mov	CX,DX			;# of nulls to pad
	shr	CX,1
	rep	stosw
	rcl	CX,1
	rep	stosb
	mov	AX,BX			;Return Value
	mov	DX,ES
	pop	DS
	.restore <DI,SI>
	pop	BP
	ret                                        
_fstrncpy endp
	endcode	strings
	endif
	end


