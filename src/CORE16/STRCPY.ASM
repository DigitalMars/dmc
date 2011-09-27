;_ strcpy.asm
;Copyright (C) 1985-1995 Digital Mars
;All Rights Reserved

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copy s2 to s1 till a zero byte.
; Use:
;	char *strcpy(s1,s2)
;
if	SPTR
eso	equ	<>
else
eso	equ	<es:>
endif

	begcode	strings
	c_public	strcpy
 	if LPTR and LCODE
	public _fstrcpy
	public __fstrcpy
_fstrcpy:
__fstrcpy:
	endif
func	strcpy

	push	bp
	mov	bp,sp

	.save	SI

    if SPTR
      ife ESeqDS
	mov	AX,DS
	mov	ES,AX
      endif
	mov	BX,P[BP]		;DI -> s1
	mov     SI,P+2[BP]		;get source pointer (s2)
    else
	mov	dx,ds
	les	bx,P[BP]		;s1
	lds	si,P+4[BP]		;DS:SI = s2
    endif
	;
	;align for word store
	;
	test	bl,1

	mov	cx,bx
	jz	aligned

	mov	al,[si]
	inc	si

	mov	eso[bx],al
	inc	bx

	test	al,al
	jz	CpyDone
aligned:
	;
	;do 2 bytes at a time
	;
	mov	al,[si]			;can only load a byte since we
	add	si,2			;don't know where end of segment is...

	test	al,al
	jz	CpyDoneLo

	mov	ah,[si-1]

	mov	eso[bx],ax
	add	bx,2

	test	ah,ah
	jnz	aligned
CpyDone:
	.restore	si
	mov	sp,bp

	mov	AX,CX			;return value is s1
if	SPTR
else
	mov	DS,DX
	mov	DX,ES
endif
	pop	bp

	ret

CpyDoneLo:
	mov	eso[bx],al
	jmp	CpyDone

c_endp	strcpy
	endcode	strings
	end


