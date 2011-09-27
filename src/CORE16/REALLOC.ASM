;_ realloc.asm   Mon Aug 9 1993
; Copyright (C) 1985-1991 by Digital Mars
; All rights reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	public _realloc
	
if LPTR
  if LCODE
	extrn __frealloc:far
  else
  	extrn __frealloc:near
  endif
else
  if LCODE
	extrn __nrealloc:far
  else
  	extrn __nrealloc:near
  endif	
endif

	begcode	realloc
  	p_func  _realloc
if LPTR
  	jmp	__frealloc
else
  	jmp	__nrealloc
endif
  	p_endp	_realloc

	endcode realloc
	end

