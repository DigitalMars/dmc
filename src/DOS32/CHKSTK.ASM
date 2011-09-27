;_ read.asm
; Copyright 1993 by Digital Mars
; All Rights Reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	
	extrn	__exit:near
	public	___stk_ovfl

	begdata	
ovrflw	db	0Dh,0Ah,'Stack Overflow',0Dh,0Ah,'$'
	enddata
	
	begcode	chkstk
___stk_ovfl proc near
	mov	edx,offset DGROUP:ovrflw
	bdos	9		;Output the message.
	push	1		;error exit code
	call	__exit		;abort
	ret
___stk_ovfl endp
	endcode
	end