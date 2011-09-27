;_ _anlshl.asm   Wed Jun 22 1994
; Copyright (C) 1994 by Digital Mars
; All rights reserved


include macros.asm

	public __aNlshl

	begcode _ANLSHL


__aNlshl	proc	near
	and	CX,00FFh
	jz	exit1
loop1:	shl	AX,1
	rcl	DX,1
	dec	CX
	jne	loop1
exit1:	ret	
__aNlshl endp

        endcode _ANLSHL

	end


