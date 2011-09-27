;_ fstrcmp.asm   Sat July 28, 1990  
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compare a string.
; Use:
;	int __far fstrcmp(s1,s2)
; Returns:
;	> 0	if s1 > s2
;	= 0	if s1 == s2
;	< 0	if s1 < s2
;

 	ife LPTR and LCODE
	begcode	strings
	public  __fstrcmp
	public	_fstrcmp
__fstrcmp:	
_fstrcmp proc far
	mov	BX,SP
	.save	<SI,DI>
	mov	DX,DS
	les     DI,SS:6-2+4[BX]		;get source pointer (s2)
	lds	SI,SS:6-2[BX]		;get destination pointer (s1)
	clr	AX			;scan for 0
	mov	CX,-1			;largest possible string
;	cld
	repne	scasb
	not	CX			;CX = string length of s2
	sub	DI,CX			;point DI back to beginning
	repe	cmpsb			;compare string
	je	L1			;strings are equal
	sbb	AX,AX
	cmc
	adc	AX,0
L1:	mov	DS,DX
	.restore <DI,SI>
	ret                                        
_fstrcmp endp
	endcode	strings
	endif
	end


