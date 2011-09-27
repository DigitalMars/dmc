;_ realloc.asm   Mon Aug 9 1993
; Copyright (C) 1985-1991 by Digital Mars
; All rights reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	public _realloc
	
  	extrn __nrealloc:near

	begcode	realloc
_realloc proc	near
  	jmp	__nrealloc
_realloc endp

	endcode realloc
	end

