;_ _anulshr.asm   Wed Jun 22 1994
; Copyright (C) 1994 by Digital Mars
; All rights reserved


include macros.asm

	public __aNulshr

	begcode _ANULSHR

__aNulshr proc	near
	and	CX,00FFh
	jz	exit3
loop3:	shr	DX,1
	rcr	AX,1
	dec	CX
	jne	loop3
exit3:	ret	
__aNulshr endp

	endcode _ANULSHR

	end




