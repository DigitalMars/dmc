;_ hdiff.asm
; Copyright (C) 1990-1992 by Digital Mars
; All Rights Reserved
; Written by Walter Bright

include macros.asm

ifdef DOS16RM
    PROTECTED EQU 1
endif
ifdef _WINDOWS
    PROTECTED EQU 1
endif
ifdef __OS2__
    PROTECTED EQU 1
endif

	extrn	__AHSHIFT:near

	begcode	hdiff

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compute difference between two huge pointers (h1 - h2).

h1off	=	4
h1seg	=	6
h2off	=	8
h2seg	=	10

	public	__aNahdiff
__aNahdiff proc	near
	push	BP
	mov	BP,SP

    ifdef PROTECTED
	mov	AX,h1off[BP]
	mov	DX,h1seg[BP]
	mov	CX,offset __AHSHIFT
	shr	DX,CL
	mov	BX,h2seg[BP]
	shr	BX,CL
	sub	AX,h2off[BP]
	sbb	DX,BX
    else
	mov	AX,h1seg[BP]
	sub	AX,h2seg[BP]	;difference between segments
	sbb	DX,DX

	;DX,AX *= 16
	shl	AX,1
	rcl	DX,1
	shl	AX,1
	rcl	DX,1
	shl	AX,1
	rcl	DX,1
	shl	AX,1
	rcl	DX,1

	add	AX,h1off[BP]
	adc	DX,0
	sub	AX,h2off[BP]
	sbb	DX,0
    endif

	pop	BP
	ret	8
__aNahdiff endp

h1off	=	6
h1seg	=	8
h2off	=	10
h2seg	=	12

	public	__aFahdiff
__aFahdiff proc	far
	push	BP
	mov	BP,SP

    ifdef PROTECTED
	mov	AX,h1off[BP]
	mov	DX,h1seg[BP]
	mov	CX,offset __AHSHIFT
	shr	DX,CL
	mov	BX,h2seg[BP]
	shr	BX,CL
	sub	AX,h2off[BP]
	sbb	DX,BX
    else
	mov	AX,h1seg[BP]
	sub	AX,h2seg[BP]	;difference between segments
	sbb	DX,DX

	;DX,AX *= 16
	shl	AX,1
	rcl	DX,1
	shl	AX,1
	rcl	DX,1
	shl	AX,1
	rcl	DX,1
	shl	AX,1
	rcl	DX,1

	add	AX,h1off[BP]
	adc	DX,0
	sub	AX,h2off[BP]
	sbb	DX,0
    endif

	pop	BP
	ret	8
__aFahdiff endp

	endcode	hdiff

	end
