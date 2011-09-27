;_ memicmp.asm   Sat July 28, 1990   
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compare strings just like memcmp(), except that case is ignored.

	begcode	strings

    if LPTR and LCODE
	public	_fmemicmp
_fmemicmp:
    endif

	c_public	memicmp
func	memicmp
	push	BP
	mov	BP,SP
	.save	SI
    if SPTR
	mov	SI,P[BP]
	mov	BX,P+SIZEPTR[BP]
    else
	push	DS
	lds	SI,P[BP]		;DS:SI -> p1
	les	BX,P+SIZEPTR[BP]	;ES:BX -> p2
    endif
	mov	CX,P+SIZEPTR+SIZEPTR[BP] ;CX = numbytes
	jcxz	match
	mov	AH,'A'
	mov	DH,'Z'
	even
L1:	lodsb				;AL = *p1++
    if SPTR
	mov	DL,[BX]
    else
	mov	DL,ES:[BX]
    endif
	inc	BX			;DL = *p2++
	_if	AL ne DL, L2		;strings are different at this char
	loop	L1			;try next char
	jmps	match

	even
L2:	;Perhaps when converted to lower case, they will be the same
	_if	AL b AH, nomatch
	_if	AL a DH, L4
	add	AL,'a' - 'A'		;convert AL to lower case
L4:	_if	DL b AH, nomatch
	_if	DL a DH, L5
	add	DL,'a' - 'A'		;convert DL to lower case
L5:	_if	AL ne DL, nomatch	;different, we're done
	loop	L1
match:	mov	AX,CX			;return 0 in AX
    if LPTR
	pop	DS
    endif
	.restore SI
	pop	BP
	ret

nomatch:
	clr	AH
	mov	DH,AH
	sub	AX,DX
    if LPTR
	pop	DS
    endif
	.restore SI
	pop	BP
	ret
c_endp	memicmp
	endcode	strings
	end


