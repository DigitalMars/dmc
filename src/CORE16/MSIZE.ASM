;_ msize.asm   Mon Aug 9 1993
; Copyright (C) 1985-1991 by Digital Mars
; All rights reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	public __msize
	
if LPTR
  if LCODE
	extrn __fmsize:far
  else
  	extrn __fmsize:near
  endif
else
  if LCODE
	extrn __nmsize:far
  else
  	extrn __nmsize:near
  endif	
endif

	begcode	msize
  	p_func  __msize
if LPTR
  	jmp	__fmsize
else
  	jmp	__nmsize
endif
  	p_endp	__msize

	endcode msize
	end

