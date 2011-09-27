;_ memint.asm	
;Copyright (C) 1985-1995 Digital Mars
;All Rights Reserved

include macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; int *_memintset(int *, int value, size_t num)
; Set an array of ints to a value.  Return the pointer to the array.
;

	begcode _memint
	public	__memintset

extrn	__opti_stosd:near

__memintset	proc	near

	mov	edx,edi
	mov	edi,PS[esp]

	mov	eax,PS + SIZEPTR[esp]	    ;value
	mov	ecx,PS + SIZEPTR + 4[esp]   ;num

	call	__opti_stosd

	mov	edi,edx			;Restore edi.
	mov	eax,PS[esp]		;Return value.

	_ret	12

__memintset	endp
	endcode _memint
	end


