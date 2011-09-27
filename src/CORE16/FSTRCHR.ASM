;_ fstrchr.asm   Sat July 28, 1990   
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Return pointer to first occurrence of char c in string s.
;	char *strchr(const char *s, int c);

 	ife LPTR and LCODE
	begcode	strings
	public	_fstrchr
_fstrchr proc far
	push    BP                                 
	mov     BP,SP                              
	.save	<DI>
	les	DI,6[BP]		;ES:DI = s
	clr	AX
	mov	CX,-1
	repne	scasb
	not	CX			;CX = length of s (including NULL)
	sub	DI,CX			;DI = s
	mov	AL,6+4[BP]		;AL = c
	repne	scasb			;scan for c
	mov	AX,CX			;assume we didn't find it
	cwd
	jnz	L8
	mov	DX,ES
	mov	AX,DI			;yes, found it
	dec	AX
L8:	.restore <DI>
	pop	BP
	ret     
_fstrchr endp
	endcode	strings
	endif
	end


