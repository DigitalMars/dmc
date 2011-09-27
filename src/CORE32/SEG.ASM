;;;;;;;;;;;;;;;;;;;;;;;;;
; C compiler system calls for Phar Lap DOS386.
;
; Copyright Walter Bright

include macros.asm

	begcode seg
;;;;;;;;;;;;;;;;;;;;;;;;;
; Return data segment.

public	_getDS

_getDS	proc	near
	clr	EAX
	mov	AX,DS
	ret
_getDS	endp

;;;;;;;;;;;;;;;;;;;;;;;;;
; Make a far pointer from its components.
; Use: MK_FP(unsigned short seg, unsigned offset);
; Returns: DX:EAX

public	      _MK_FP

_MK_FP	proc	near
	movzx	edx,word ptr PS[ESP]
	mov	eax,PS + 4 [ESP]
	_ret	8
_MK_FP	endp

;;;;;;;;;;;;;;;;;;;;;;;;;
; Get the segment of a far pointer.
; Use: FG_SEG (fp)
; Returns: segment in AX.

public	_FP_SEG
_FP_SEG proc	near
	movzx	EAX,word ptr PS+4[ESP]
	_ret	8
_FP_SEG endp

	endcode seg

	end
