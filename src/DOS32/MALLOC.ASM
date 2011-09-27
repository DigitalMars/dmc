;_ malloc.asm   Mon Aug 9 1993
; Copyright (C) 1985-1991 by Digital Mars
; All rights reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	public _malloc
	
  	extrn __nmalloc:near
  	
	begcode	malloc
_malloc proc    near
  	jmp	__nmalloc
_malloc endp

	endcode malloc
	end

