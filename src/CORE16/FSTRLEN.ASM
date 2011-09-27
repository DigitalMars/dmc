;_ fstrlen.asm   Sat July 28, 1990   
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Calculate length of string and return it.
;	int __far fstrlen(s)

 	ife LPTR and LCODE
	begcode	strings
	public	__fstrlen
	public	_fstrlen
__fstrlen:	
_fstrlen	proc far
	mov	BX,SP
	mov	DX,DI			;save DI
	les	DI,SS:6-2[BX]
	clr	AX			;scan for 0
	mov	CX,-1			;largest possible string
;	cld
	repne	scasb
	mov	AX,CX
	not	AX			;AX = string length
	dec	AX
	mov	DI,DX
	ret                                        
_fstrlen	endp
	endcode	strings
	endif
	end


