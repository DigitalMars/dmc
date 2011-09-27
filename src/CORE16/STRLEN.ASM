;_ strlen.asm
;Copyright (C) 1985-1995 Digital Mars
;All Rights Reserved

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Calculate length of string and return it.
;	int strlen(s)

	begcode	strings
	c_public strlen
 	if LPTR and LCODE
 	public __fstrlen
	public _fstrlen
__fstrlen:	
_fstrlen:
	endif
func	strlen
	mov	BX,SP
    if SPTR
	mov	bx,SS:P-2[BX]
    else
	mov	cx,ds
	lds	bx,SS:P-2[BX]
    endif
	mov	dx,bx
DoLoop:
	mov	al,[bx]

	test	al,al
	jz	Done0

	mov	al,[bx+1]

	test	al,al
	jz	Done1

	mov	al,[bx+2]

	test	al,al
	jz	Done2

	mov	al,[bx+3]
	add	bx,4

	test	al,al
	jnz	DoLoop
Done3:
	lea	ax,[bx-1]
Done:
	sub	ax,dx
    if SPTR
    else
	mov	ds,cx
    endif
	ret

Done2:
	lea	ax,[bx+2]
	jmp	Done

Done1:
	lea	ax,[bx+1]
	jmp	Done

Done0:
	mov	ax,bx
	jmp	Done

c_endp	strlen
	endcode	strings
	end


