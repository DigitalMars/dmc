;_ malloc.asm   Mon Aug 9 1993
; Copyright (C) 1985-1991 by Digital Mars
; All rights reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	public _free
	
if LPTR
  if LCODE
	extrn __ffree:far
  else
  	extrn __ffree:near
  endif
else
  if LCODE
	extrn __nfree:far
  else
  	extrn __nfree:near
  endif	
endif

	begcode	free
  	p_func  _free
if LPTR
  	jmp	__ffree
else
  	jmp	__nfree
endif
  	p_endp	_free

	endcode free
	end

