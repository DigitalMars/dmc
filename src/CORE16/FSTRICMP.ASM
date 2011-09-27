;_ fstricmp.asm   Sat July 28, 1990  
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compare strings just like strcmp(), except that case is ignored.

 	ife LPTR and LCODE
	begcode	strings
	public	_fstricmp
_fstricmp proc far
	push	BP
	mov	BP,SP
	mov	DX,SI
	push	DS
	lds	SI,6[BP]		;DS:SI -> p1
	les	BX,6+4[BP]		;ES:BX -> p2
	even
L1:	lodsb				;AL = *p1++
	mov	CL,ES:[BX]
	inc	BX			;CL = *p2++
	_if	AL ne CL, L2		;strings are different at this char
	tst	AL			;end of string?
	jnz	L1			;no
	jmps	L3

L2:	;Perhaps when converted to lower case, they will be the same
	_if	AL b 'A', L3
	_if	AL a 'Z', L4
	add	AL,'a' - 'A'		;convert AL to lower case
L4:	_if	CL b 'A', L3
	_if	CL a 'Z', L5
	add	CL,'a' - 'A'		;convert CL to lower case
L5:	_if	AL e CL, L1		;same now, so continue looping

L3:	clr	AH
	mov	CH,AH
	sub	AX,CX
	pop	DS
	mov	SI,DX
	pop	BP
	ret
_fstricmp endp
	endcode	strings
	endif
	end


