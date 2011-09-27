;_ strcmp.asm
;Copyright (C) 1985-1995 Digital Mars
;All Rights Reserved

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compare a string.
; Use:
;	int strcmp(s1,s2)
; Returns:
;	> 0	if s1 > s2
;	= 0	if s1 == s2
;	< 0	if s1 < s2
;

if	SPTR
eso	equ	<>
else
eso	equ	<es:>
endif

	begcode	strings
	c_public	strcmp
 	if LPTR and LCODE
 	public __fstrcmp
	public _fstrcmp
__fstrcmp:	
_fstrcmp:
	endif
func	strcmp
	push	bp
	mov	bp,SP

	.save	<SI>

    if SPTR
      ife ESeqDS
	mov	AX,DS
	mov	ES,AX
      endif
	mov     BX,P+2[BP]		;get source pointer (s2)
	mov     SI,P[BP]		;get destination pointer (s1)
    else
	mov	CX,DS
	les     BX,P+4[BP]		;get source pointer (s2)
	lds	SI,P[BP]		;get destination pointer (s1)
    endif

QuadLoop:
	mov	dl,eso[bx]
	mov	al,[si]

	cmp	al,dl
	jnz	Unequal

	test	al,al
	jz	Equal

	mov	dl,eso[bx+1]	;Repeat 3 more times
	mov	al,[si+1]

	cmp	al,dl
	jnz	Unequal

	test	al,al
	jz	Equal

	mov	dl,eso[bx+2]
	mov	al,[si+2]

	cmp	al,dl
	jnz	Unequal

	test	al,al
	jz	Equal

	mov	dl,eso[bx+3]
	mov	al,[si+3]

	cmp	al,dl
	jnz	Unequal

	add	si,4		;Update pointers
	add	bx,4

	test	al,al
	jnz	QuadLoop
Equal:
	xor	ax,ax
	jmp	L3

Unequal:
	sbb	AX,AX

	or	AL,1
L3:
	.restore SI

    if SPTR
    else
	mov	DS,CX
    endif
	mov	sp,bp

	pop	bp

	ret
c_endp	strcmp
	endcode	strings
        end
