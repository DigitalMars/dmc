;_ _afulshr.asm   Wed Jun 22 1994
; Copyright (C) 1994 by Digital Mars
; All rights reserved


include macros.asm

	public __aFulshr



	begcode _AFULSHR

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;  _aFulshr is an emulation of the Microsoft
;  intrinsic to do a shift right of an unsigned long.
;  Upon Entry, CX is the number of bits to shift
;  AX is the low word of the long and DX is the
;  high word.


__aFulshr proc	far
	and	CX,00FFh
	jz	exit4
loop4:	shr	DX,1
	rcr	AX,1
	dec	CX
	jne	loop4
exit4:	ret	
__aFulshr endp

	
	endcode _AFULSHR

	end
