;_ memchr.asm	
;Copyright (C) 1985-1995 Digital Mars
;All Rights Reserved

include macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Search for char c in first n bytes pointed to by p.
;	void *memchr(const void *p,int c,size_t n)
; Returns:
;	pointer to char found, else NULL.

	begcode memchr
	public	_memchr

_memchr proc	near

	mov	ecx,12[esp]		;ECX = n
	mov	edx,4[esp]

	mov	al,8[esp]
	push	ebx

	cmp	ecx,4
	jb	DoBytes			;return NULL
DoAlign:
	test	dl,3

	jz	Aligned

	mov	ah,[edx]
	inc	edx

	cmp	ah,al
	jz	Found

	dec	ecx
	jnz	DoAlign
NotFound:
	pop	ebx
	xor	eax,eax

	ret

Aligned:
	push	ecx

	shr	ecx,2
	jz	TryBytes
QuadLoop:
	mov	ebx,[edx]
	add	edx,4

	cmp	bl,al
	jz	Found1

	cmp	bh,al
	jz	Found2

	shr	ebx,16

	cmp	bl,al
	jz	Found3

	cmp	bh,al
	jz	Found4

	dec	ecx
	jnz	QuadLoop
TryBytes:
	pop	ecx
DoBytes:
	and	ecx,3
	jz	NotFound
BytesLoop:
	mov	ah,[edx]
	inc	edx

	cmp	al,ah
	jz	Found

	dec	ecx
	jnz	BytesLoop

	pop	ebx
	xor	eax,eax

	ret

Found4:
	pop	ecx
Found:
	pop	ebx
	lea	eax,[edx-1]

	ret

Found1:
	pop	ecx
	pop	ebx

	lea	eax,[edx-4]

	ret

Found2:
	pop	ecx
	pop	ebx

	lea	eax,[edx-3]

	ret

Found3:
	pop	ecx
	pop	ebx

	lea	eax,[edx-2]

	ret


_memchr endp

	endcode memchr

	end

