;_ stkovfl.asm
; Copyright (C) 1985-1991 by Digital Mars
; All rights reserved.

include	macros.asm

    if LCODE
	extrn	___faterr:far
    else
	extrn	___faterr:near
    endif

	begdata

    ifdef _WINDOWS
ovrflw		db	'Stack Overflow',0
    else
ovrflw		db	0Dh,0Ah,'Stack Overflow',0Dh,0Ah,0
    endif

	enddata

	begcode	stkovfl

	public ___stk_ovfl
func	__stk_ovfl
    if LPTR
	mov	AX,DGROUP
	mov	DS,AX
    endif	
	mov	DX,offset DGROUP:ovrflw
	push	DX
	jmp	___faterr
c_endp  __stk_ovfl

	endcode	stkovfl

	end
