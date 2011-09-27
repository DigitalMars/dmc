;_ strchr.asm
;Copyright (C) 1985-1995 Digital Mars
;All Rights Reserved

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Return pointer to first occurrence of char c in string s.
;	char *strchr(const char *s, int c);

	begcode	strings

	c_public strchr
 	if LPTR and LCODE
	public _fstrchr
_fstrchr:
	endif
func	strchr

	mov     BX,SP

	mov	dl,ss:P+SIZEPTR-2[bx]
    if SPTR
	mov	bx,ss:P-2[BX]		;bx = s
    else
	mov	cx,ds
	lds	bx,ss:P-2[BX]		;ds:bx = s
    endif

DoLoop:
	mov	al,[bx]

	cmp	al,dl
	jz	FoundLo

	test	al,al
	jz	DoneLo

	mov	al,[bx+1]
	add	bx,2

	cmp	al,dl
	jz	FoundHi

	test	al,al
	jnz	DoLoop
DoneHi:
DoneLo:
	xor	ax,ax
    if SPTR
    else
	xor	dx,dx
    endif
Done:
    if SPTR
    else
	mov	ds,cx
    endif
	ret

FoundLo:
	mov	ax,bx
    if SPTR
    else
	mov	dx,ds
	mov	ds,cx
    endif

	ret

FoundHi:
	lea	ax,[bx-1]
    if SPTR
    else
	mov	dx,ds
	mov	ds,cx
    endif

	ret

c_endp	strchr
	endcode	strings
	end


