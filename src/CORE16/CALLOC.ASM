;_ calloc.asm   Mon Aug 9 1993
; Copyright (C) 1985-1991 by Digital Mars
; All rights reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	public _calloc
	
if LPTR
  if LCODE
	extrn __fcalloc:far
  else
  	extrn __fcalloc:near
  endif
else
  if LCODE
	extrn __ncalloc:far
  else
  	extrn __ncalloc:near
  endif	
endif

	begcode	calloc
  	p_func  _calloc
if LPTR
  	jmp	__fcalloc
else
  	jmp	__ncalloc
endif
  	p_endp	_calloc

	endcode calloc
	end

