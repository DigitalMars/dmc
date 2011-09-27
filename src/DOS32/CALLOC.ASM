;_ calloc.asm   Mon Aug 9 1993
; Copyright (C) 1985-1991 by Digital Mars
; All rights reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	public _calloc
	
  	extrn __ncalloc:near

	begcode	calloc
_calloc proc	near
  	jmp	__ncalloc
_calloc endp

	endcode calloc
	end

