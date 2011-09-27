;_ ncalloc.asm
; Copyright (C) 1985-1992 by Digital Mars
; All rights reserved
; Written by Walter Bright
; $Revision: 1.1.1.1 $

include macros.asm

	public __ncalloc
	extrn __nmalloc:near

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Allocate a block of data and clear it.
; Use:
;	p = _ncalloc(numelems,sizeof(elem));
; Returns:
;	pointer to allocated data else NULL

	begcode ncalloc
	_align
	
__ncalloc proc	near
	mov	eax,PS[esp]	;get numelems
	mov	edx,PS+4[esp]	;get sizeof(elem)
	_ifs	edx e 1, C1	;no need to multiply
	mul	edx
	jc	short C3	;if overflow

C1:	push	eax		;nbytes
	call	__nmalloc
    ife _STDCALL
	add	esp,4
    endif

	tst	eax		;fail?
	jz	short C2	;yes

	uses	<ECX,EDX,EDI>
	mov	edi,eax
	mov	edx,eax		;save pointer to result
	mov	ecx,-4[edi]	;# of bytes
	shr	ecx,2		;# of dwords (including byte count)
	dec	ecx		;skip byte count

    ife ESeqDS
	mov	ax,ds
	mov	es,ax
    endif

	clr	eax
	rep	stosd		;clear the memory
	mov	eax,edx		;restore pointer to result
	unuse	<edi,edx,ecx>
C2:
	_ret	8

C3:	clr	eax
	_ret	8

__ncalloc endp

	endcode ncalloc
	end
