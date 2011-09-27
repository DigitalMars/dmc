;_ atoi.asm  
; Copyright (C) 1985-1991 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

	include	macros.asm
	
	c_extrn atoi,near
	
	begcode atol
	_align
	c_public atol
func	atol
	jmp 	_atoi
c_endp	atol
	endcode	atol

	end
