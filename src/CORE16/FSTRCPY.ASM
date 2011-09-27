;_ fstrcpy.asm   Sat July 28, 1990   
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copy s2 to s1 till a zero byte.
; Use:
;	char __far  *fstrcpy(s1,s2)
;
 	ife LPTR and LCODE
	begcode	strings
	public	_fstrcpy
	public	__fstrcpy
__fstrcpy:	
_fstrcpy	proc far
	mov	BX,SP                              
	.save	<SI,DI>
	les	DI,SS:6-2+4[BX]		;ES:DI = s2
	clr	AX			;scan for 0
	mov	CX,-1			;largest possible string
;	cld
	repne	scasb			;find end of s2
	not	CX			;CX = strlen(s2) + 1 (for EOS)
	mov	DX,DS
	les	DI,SS:6-2[BX]
	lds	SI,SS:6-2+4[BX]
	mov	AX,DI			;return value is s1
	rep	movsb
	mov	DS,DX
	.restore <DI,SI>
	mov	DX,ES
	ret                                        
_fstrcpy	endp
	endcode	strings
	endif
	end


