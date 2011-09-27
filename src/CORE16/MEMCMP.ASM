;_ memcmp.asm
;Copyright (C) 1985-1995 Digital Mars
;All Rights Reserved

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compare an array of n bytes (unsigned).
; Use:
;	int memcmp(s1,s2,n)
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
	c_public	memcmp
 	if LPTR and LCODE
 		public __fmemcmp
		public _fmemcmp
__fmemcmp:		
_fmemcmp:
	endif

func	memcmp

	push    BP
	mov     BP,SP

	.save	<SI,DI>

	mov	CX,P+SIZEPTR+SIZEPTR[BP]		;CX = n
    if SPTR
      ife ESeqDS
	mov	AX,DS
	mov	ES,AX
      endif
	mov	DI,P+2[BP]		;get source pointer (s2)
	mov     SI,P[BP]		;get destination pointer (s1)
    else
	push	ds
	les     DI,P+SIZEPTR[BP]	;get source pointer (s2)
	lds	SI,P[BP]		;get destination pointer (s1)
    endif
	mov	ax,si			;align if both ptrs odd
	mov	dx,cx

	and	ax,di

	test	al,1
	jz	Aligned

	cmpsb

	jnz	Unequal

	dec	cx
	dec	dx
Aligned:
	shr	cx,1			;do blocks of 2 bytes
	jz	DoByte
QuadLoop:
	repe	cmpsw

	jnz	UnequalWord
DoByte:
	and	dl,1
	jz	Equal

	mov	bl,eso[di]
	mov	al,[si]

	cmp	al,bl
	jnz	Unequal
Equal:
	xor	ax,ax
	jmp	L3

UnequalWord:
	mov	bx,eso[di-2]
	mov	ax,[si-2]

	cmp	al,bl
	jnz	Unequal

	cmp	ah,bh
Unequal:
	sbb	AX,AX

	or	al,1
L3:
    if LPTR
	pop	DS
    endif
	.restore <DI,SI>
	pop     BP
	ret
c_endp	memcmp
	endcode	strings
	end


