;_ malloc.asm   Mon Aug 9 1993
; Copyright (C) 1985-1991 by Digital Mars
; All rights reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	public _malloc
	
if LPTR
  if LCODE
	extrn __fmalloc:far
  else
  	extrn __fmalloc:near
  endif
else
  if LCODE
	extrn __nmalloc:far
  else
  	extrn __nmalloc:near
  endif	
endif

	begcode	malloc
  	p_func  _malloc
if LPTR
  	jmp	__fmalloc
else
  	jmp	__nmalloc
endif
  	p_endp	_malloc

	endcode malloc
	end

