;_ hugeptr.asm
; Copyright (C) 1990-1994 by Walter Bright
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

	begcode	hugeptr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compute difference between two huge pointers.

	public HUGEPTR_DIFF
    if LCODE
HUGEPTR_DIFF proc far
    else
HUGEPTR_DIFF proc near
    endif
	push	BP
	mov	BP,SP

    ifdef PROTECTED
	mov	AX,P+4[BP]
	mov	DX,P+6[BP]
	mov	CX,offset __AHSHIFT
	shr	DX,CL
	mov	BX,P+2[BP]
	shr	BX,CL
	sub	AX,P+0[BP]
	sbb	DX,BX
    else
	mov	AX,P+6[BP]
	sub	AX,P+2[BP]	;difference between segments
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

	add	AX,P+4[BP]
	adc	DX,0
	sub	AX,P+0[BP]
	sbb	DX,0
    endif

	pop	BP
	ret	8
HUGEPTR_DIFF endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	void __far * __pascal hugeptr_add(void __far *h, long offset);

	public HUGEPTR_ADD
    if LCODE
HUGEPTR_ADD proc far
    else
HUGEPTR_ADD proc near
    endif
	push	BP
	mov	BP,SP

	mov	AX,P[BP]
	mov	DX,P+2[BP]		;DX,AX = offset
	add	AX,P+4[BP]
	adc	DX,0			;DX,AX += FP_OFF(h)
	mov	CX,offset __AHSHIFT
	shl	DX,CL			;adjust DX for stride
	add	DX,P+6[BP]		;add in FP_SEG(h)

	pop	BP
	ret	8
HUGEPTR_ADD endp

	endcode	hugeptr

	end
