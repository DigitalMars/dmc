;_ fstrncmp.asm   Sat July 28, 1990   
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compare a string of at most n chars (unsigned).
; Use:
;	int __far fstrncmp(s1,s2,n)
; Returns:
;	> 0	if s1 > s2
;	= 0	if s1 == s2
;	< 0	if s1 < s2
;
 	ife LPTR and LCODE
	begcode	strings
	public	_fstrncmp
_fstrncmp proc far
	push    BP                                 
	mov     BP,SP                              
	.save	<SI,DI>
	mov	BX,DS
	les     DI,6+4[BP]		;get source pointer (s2)
	lds	SI,6[BP]		;get destination pointer (s1)
	clr	AX			;scan for 0
	mov	CX,-1			;largest possible string
;	cld
	repne	scasb
	not	CX			;CX = string length of s2
	sub	DI,CX			;point DI back to beginning
	mov	dx,6+2*4[bp]
	_if	cx b dx,L5
	mov	cx,dx			;CX = min(CX,n)
L5:	tst	ax			;Set ZF.
	repe	cmpsb			;compare string
	je	L3			;strings are equal
	sbb	AX,AX
	cmc
	adc	AX,0
L3:	mov	DS,BX
	.restore <DI,SI>
	pop     BP                                 
	ret                                        
_fstrncmp endp
	endcode	strings
	endif
	end


