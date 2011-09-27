;_ nrealloc.asm
; Copyright (C) 1985-1992 by Digital Mars
; All rights reserved
; Written by Walter Bright
; $Revision: 1.1.1.1 $

include macros.asm

	public __nrealloc
	extrn __nmalloc:near, __nfree:near

	begcode nrealloc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Reallocate memory that was allocated by malloc().
; Use:
;	char *_nrealloc(char *p, unsigned nbytes)
; Returns:
;	0 error
;	else pointer to reallocated memory

R6:
    if _STDCALL
	push	P[EBP]
	call	near ptr __nfree
	pop	EBP
	_ret	8
    else
	pop	ebp
	jmp	near ptr __nfree			;free(p)
    endif

R5:	;function just like malloc(nbytes)
	push	eax
	call	__nmalloc
    ife _STDCALL
	add	esp,4
    endif
	unuse	<edx>
	pop	ebp
	_ret	8

__nrealloc proc	near
	push	ebp
	mov	ebp,esp

	mov	eax,P+SIZEPTR[ebp]	;EAX = nbytes
	tst	eax			;trying to realloc() to 0 size?
	jz	R6			;yes

	;if realloced size is smaller, attempt to just shrink current block
	uses	<EDX>

	mov	edx,P[ebp]	;EDX = p
	tst	edx		;is p NULL?
	jz	R5		;yes

	uses	<ECX,ESI,EDI>
	sub	edx,4
	mov	ecx,[edx]	;ECX = # of bytes in this block
	add	eax,7
	and	al,NOT 3	;EAX = real new size

	sub	ecx,eax

	jb	short R3		;if allocating more bytes
	_ifs	ecx b <SIZEPTR+4>, R4	;size of free list entry

	mov	[edx],eax	;realloced size of p
	add	edx,eax
	mov	[edx],ecx	;size of new fragment
	add	edx,4
	push	edx
	call	near ptr __nfree
    ife _STDCALL
	add	esp,4
    endif
R4:
	mov	eax,P[ebp]	;reload original pointer p
	jmps	R1		;no change, return p

	;we'll have to allocate a new block, and free the old one
R3:
	push	P+SIZEPTR[ebp]
	call	__nmalloc		;malloc(nbytes)
    ife _STDCALL
	add	esp,4
    endif
	tst	eax

	jz	rallocerr	;error
	push	eax		;save pointer to new memory

	mov	esi,P[ebp]	;DS:ESI -> original
	mov	edi,eax		;ES:EDI -> new item
	mov	ecx,-4[esi]
	_ifs	ecx be -4[edi], R2
	mov	ecx,-4[edi]	;ECX = smaller of two size
R2:	shr	ecx,2		;# of dwords
	dec	ecx		;compensate for extra dword in beginning

ife ESeqDS
	mov	ax,ds
	mov	es,ax
endif

	rep	movsd		;transfer the dwords
	push	P[ebp]
	call	__nfree		;free the old one
    ife _STDCALL
	add	esp,SIZEPTR
    endif

	tst	eax
	pop	eax		;restore pointer to new memory
	jnz	rallocerr
R1:	unuse	<edi,esi,ecx,edx>
	pop	ebp
	_ret	8

rallocerr:
	clr	eax
	jmp	R1
__nrealloc endp

	endcode nrealloc
	end
