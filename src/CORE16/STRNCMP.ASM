;_ strncmp.asm
;Copyright (C) 1985-1995 Digital Mars
;All Rights Reserved

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compare a string of at most n chars (unsigned).
; Use:
;	int strncmp(s1,s2,n)
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
	c_public	strncmp
 	if LPTR and LCODE
	public _fstrncmp
_fstrncmp:
	endif
func	strncmp
	push    BP
	mov     BP,SP
	.save	<SI,DI>

	mov	cx,P+2*SIZEPTR[bp]

    if SPTR
	mov     DI,P+2[BP]		;get source pointer (s2)
	mov     SI,P[BP]		;get destination pointer (s1)
    else
	mov	BX,DS
	les     DI,P+4[BP]		;get source pointer (s2)
	lds	SI,P[BP]		;get destination pointer (s1)
    endif

	shr	cx,1

	shr	cx,1
	jz	TryOdd
QuadLoop:				;groups of 4
	mov	dl,eso[di]
	mov	al,[si]

	cmp	al,dl
	jnz	Unequal

	test	al,al
	jz	Equal

	mov	dl,eso[di+1]
	mov	al,[si+1]

	cmp	al,dl
	jnz	Unequal

	test	al,al
	jz	Equal

	mov	dl,eso[di+2]
	mov	al,[si+2]

	cmp	al,dl
	jnz	Unequal

	test	al,al
	jz	Equal

	mov	dl,eso[di+3]
	mov	al,[si+3]

	cmp	al,dl
	jnz	Unequal

	test	al,al
	jz	Equal

	add	si,4		;Update pointers
	add	di,4

	dec	cx		;Count by fours
	jnz	QuadLoop
TryOdd:
	mov	cx,P+2*SIZEPTR[bp]	;restore n

	and	cx,3
	jz	Equal
DoOdd:
	mov	dl,eso[di]
	mov	al,[si]

	cmp	al,dl
	jnz	Unequal

	test	al,al
	jz	Equal

        inc si    ; Update pointers
        inc di  
    
	dec	cx
	jnz	DoOdd
Equal:
	xor	ax,ax
	jmp	L3

Unequal:
	sbb	AX,AX

	or	AL,1
L3:
    if LPTR
	mov	DS,BX
    endif
	.restore <DI,SI>
	pop     BP
	ret
c_endp	strncmp
	endcode	strings
	end


