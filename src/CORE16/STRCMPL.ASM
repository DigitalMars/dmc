;_ strcmpl.asm   Sat July 28, 1990  
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compare strings just like strcmp(), except that case is ignored.

	begcode	strings

	public	_stricmp
_stricmp:

	c_public	strcmpl
 	if LPTR and LCODE
		public _fstricmp
_fstricmp:
	endif
func	strcmpl
	push	BP
	mov	BP,SP
	mov	DX,SI
    if SPTR
	mov	SI,P[BP]
	mov	BX,P+SIZEPTR[BP]
    else
	push	DS
	lds	SI,P[BP]		;DS:SI -> p1
	les	BX,P+SIZEPTR[BP]	;ES:BX -> p2
    endif
	even
L1:	lodsb				;AL = *p1++
    if SPTR
	mov	CL,[BX]
    else
	mov	CL,ES:[BX]
    endif
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
    if LPTR
	pop	DS
    endif
	mov	SI,DX
	pop	BP
	ret
c_endp	strcmpl
	endcode	strings
	end


