;_ fstrcat.asm   Sat July 28, 1990  
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Cat s2 to s1 till a zero byte.
; Use:
;	char __far *fstrcat(void __far *s1,const void __far *s2)
; Returns:
;	s1
;
	ife LPTR and LCODE
	begcode	strings
	public	_fstrcat

_fstrcat	proc far
	push    BP                                 
	mov     BP,SP                              
	.save	<SI,DI>
	les	DI,6+4[BP]		;s2
	clr	AX			;scan for 0
	mov	CX,-1			;largest possible string
	repne	scasb			;find end of s1
	not	CX			;CX = strlen(s2) + 1 (for EOS)
	sub	DI,CX			;back up DI to start of s2

	mov	DX,CX
	mov	CX,-1
	mov	SI,DI
	les	DI,6+[BP]		;ES:DI = s1
	mov	BX,DI
	repne	scasb
	dec	DI			;ES:DI -> EOS of s1
	mov	CX,DX
	shr	CX,1
	push	DS
	mov	DS,6+4+2[BP]		;DS:SI = s2
	rep	movsw
	rcl	CX,1
	rep	movsb			;transfer bytes (including EOS)
	mov	AX,BX
	mov	DX,ES			;DX,AX points to s1
	pop	DS
	.restore <DI,SI>
	pop	BP
	ret                                        
_fstrcat	endp
	endcode	strings
	endif
	end


