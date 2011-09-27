;_ msize.asm   Mon Aug 9 1993
; Copyright (C) 1985-1991 by Digital Mars
; All rights reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	public __msize
	
  	extrn __nmsize:near

	begcode	msize
__msize proc	near
  	jmp	__nmsize
__msize endp

	endcode msize
	end

