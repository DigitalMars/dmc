;_ fmemchr.asm   Sat July 28, 1990   
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Search for char c in first n bytes pointed to by p.
;	void __far *fmemchr(const void __far *p,int c,size_t n)
; Returns:
;	pointer to char found, else NULL.

    ife LPTR and LCODE
	begcode	strings

	public _fmemchr

_fmemchr	proc far
	push    BP                                 
	mov     BP,SP                              
	mov	CX,6+4+2[BP]	;CX = n
	jcxz	L9			;return NULL
	.save	<DI>
	les	DI,6+[BP]		;ES:DI = s
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

L9:	mov	AX,CX
	cwd
	pop	BP
	ret
_fmemchr	endp
	endcode	strings
    endif
	end


