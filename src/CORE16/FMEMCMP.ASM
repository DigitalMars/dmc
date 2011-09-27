;_ fmemcmp.asm   Sat July 28, 1990  
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compare an array of n bytes (unsigned).
; Use:
;	int __far _fmemcmp(const void __far *s1,const void __far *s2,n)
; Returns:
;	> 0	if s1 > s2
;	= 0	if s1 == s2
;	< 0	if s1 < s2
;

 	ife LPTR and LCODE
	begcode	strings
	public	__fmemcmp
	public	_fmemcmp
__fmemcmp:
_fmemcmp	proc far
	push    BP                                 
	mov     BP,SP                              
	.save	<SI,DI>
	mov	BX,DS
	les     DI,6+4[BP]		;get source pointer (s2)
	lds	SI,6+[BP]		;get destination pointer (s1)
	mov	CX,6+4+4[BP]		;CX = n
	clr	AX
	repe	cmpsb			;compare string
	je	L3			;strings are equal
	sbb	AX,AX
	cmc
	adc	AX,0
L3:
	mov	DS,BX
	.restore <DI,SI>
	pop     BP                                 
	ret                                        
_fmemcmp	endp
	endcode	strings
	endif
	end


