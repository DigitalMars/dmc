;_ _fmemicm.asm   Wed Jun 22 1994
; Copyright (C) 1994 by Digital Mars
; All rights reserved


include macros.asm

	extrn _fmemicmp:far


	public __fmemicmp


	begcode _FMEMICM


__fmemicmp proc	far
	jmp	_fmemicmp
__fmemicmp endp

	endcode _FMEMICM
	end


