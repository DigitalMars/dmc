;_ malloc.asm   Mon Aug 9 1993
; Copyright (C) 1985-1991 by Digital Mars
; All rights reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	public _free
	
	extrn __nfree:near

	begcode	free
_free	proc	near
  	jmp	__nfree
_free	endp

	endcode free
	end

