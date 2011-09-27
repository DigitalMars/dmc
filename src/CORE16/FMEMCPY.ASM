;_ fmemcpy.asm   Sat July 28, 1990   
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copy n bytes from p2 to p1.
;	void __far * __far _fmemcpy(void __far *p1,const void __far *p2,size_t n)
; Returns:
;	p1

    ife LPTR and LCODE
	public	_fmemmove		;alternate entry point
        public  _fmemcpy
	public	__fmemcpy
	begcode	strings
_fmemmove:
_fmemcpy:
__fmemcpy proc	far
	push	BP
	mov	BP,SP
	.save	<SI,DI>

	mov	BX,DS			;Save DS.
	les	DI,6 + [BP]		;p1
	lds	SI,6 + 4[BP]		;p2
	mov	AX,DI			;Return value.
	mov	CX,6 + 2 * 4[BP]	;n
	cmp	SI,DI
	ja	memcpy_forward		;Jmp if forward copy.

	mov	DX,CX
	and	CX,1			;do odd remaining byte first
	add	SI,DX
	dec	SI
	add	DI,DX
	dec	DI

	std				;Reverse direction copy
	rep	movsb			;do 0 or 1 bytes.
	mov	CX,DX
	shr	CX,1
	dec	SI
	dec	DI
	rep	movsw			;do remaining words.
	cld
	jmp	short memcpy_done

memcpy_forward:
;	cld
	shr	CX,1
	rep	movsw
	rcl	CX,1			;Compute odd remainder byte
	rep	movsb

memcpy_done:
	mov	DS,BX			;Restore DS
	mov	DX,ES			;Segment of return value.
	.restore <DI,SI>
	pop	bp
	ret
__fmemcpy endp
	endcode	strings
    endif
	end


