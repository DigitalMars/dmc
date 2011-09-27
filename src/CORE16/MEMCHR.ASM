;_ memchr.asm
;Copyright (C) 1985-1995
;All Rights Reserved

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Search for char c in first n bytes pointed to by p.
;	void *memchr(const void *p,int c,size_t n)
; Returns:
;	pointer to char found, else NULL.

	c_public memchr
	begcode	strings

    if LPTR and LCODE
	public _fmemchr
_fmemchr:
    endif

func	memchr

	push    BP
	mov     BP,SP

	mov	CX,P+SIZEPTR+2[BP]	;CX = n
	mov	AL,P+SIZEPTR[BP]	;AL = c

	test	cx,cx
	jz	NotFoundA		;return NULL

    if SPTR
	mov	bx,P[BP]		;bx = s
    else
	push	ds
	lds	bx,P[BP]		;ds:bx = s
    endif
	test	bl,1

	jz	Aligned

	mov	ah,[bx]
	inc	bx

	cmp	ah,al
	jz	Found

	dec	cx
	jz	NotFound
Aligned:
	push	cx

	shr	cx,1

	shr	cx,1
	jz	TryBytes
QuadLoop:
	mov	dx,[bx]
	add	bx,2

	cmp	dl,al
	jz	Found1

	cmp	dh,al
	jz	Found2

	mov	dx,[bx]
	add	bx,2

	cmp	dl,al
	jz	Found1

	cmp	dh,al
	jz	Found2

	dec	cx
	jnz	QuadLoop
TryBytes:
	pop	cx

	and	cx,3
	jz	NotFound
BytesLoop:
	mov	ah,[bx]
	inc	bx

	cmp	ah,al
	jz	Found

	dec	cx
	jnz	BytesLoop
NotFound:
    if LPTR
	pop	ds
    endif
NotFoundA:
	xor	ax,ax
    if LPTR
	xor	dx,dx
    endif
	pop	bp

	ret

Found1:
	pop	cx
	lea	ax,[bx-2]

	jmp	Done

Found2:
	pop	cx
Found:
	lea	ax,[bx-1]
Done:
    if LPTR
	mov	dx,ds
	pop	ds
    endif
	pop	BP
	ret

c_endp	memchr
	endcode	strings
	end


