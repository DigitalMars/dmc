;_ ftol.asm   Sun Mar 5 1995
; Copyright (C) 1995 by Digital Mars Corporation
; All rights reserved
; Compatability entry point for Float to long
; conversions with VC 2.0
; $Revision: 1.1.1.1 $

	include macros.asm
	public __ftol
	
  	extrn __DBLLNG87@:near
  	
	begcode	ftol
__ftol proc    near
  	jmp	__DBLLNG87@
__ftol endp

	endcode ftol
	end

