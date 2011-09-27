;_ excptlst.asm 
; Copyright (C) 1994 by Digital Mars
; All rights reserved.
; Contains definition for global used in Structured Exception Handling

include macros.asm

 	begdata

	public	__except_list
__except_list	equ	0h
	enddata

	end
