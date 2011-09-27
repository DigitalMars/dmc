;_ alloc.asm   Sun Apr	8 1990	 Modified by: Walter Bright
; Windows support added by G. Eric Engstrom Dec 1990,	-D_WINDOWS
; RATIONAL support added by P. Murray April 1990,	-DDOS16RM
; OS2 support added by Nikki Locke May 1989,		-D__OS2__
; Copyright (C) 1985-1991 by Walter Bright
; All rights reserved
; Written by Walter Bright
; $Revision: 1.1.1.1 $

include	macros.asm

    if LCODE
        extrn   __nmalloc:far
        extrn	__nfree:far
    else
        extrn   __nmalloc:near
        extrn   __nfree:near
    endif
    
    ifdef _WINDOWS
        extrn	LOCKSEGMENT:far
        extrn	LOCALREALLOC:far
        extrn	UNLOCKSEGMENT:far
LMEM_FIXED      =   0000h
LMEM_MOVEABLE   =   0002h
LMEM_NOCOMPACT  =   0010H
LMEM_NODISCARD  =   0020H
LMEM_ZEROINIT   =   0040h
LMEM_MODIFY     =   0080H
LMEM_DISCARDABLE=   0F00h
    endif

	begcode	nrealloc
        c_public _nrealloc
        
  ifdef _WINDOWS
func	_nrealloc
	WINENTER
	mov	AX,P[BP]
	or	AX,AX
	jne	R1
;	
	push	word ptr P+2[BP]
	call	__nmalloc
	pop	CX
	jmps	R3
;	
R1:	cmp	word ptr P+2[BP],0
	jne	R2
;	
	push	AX
	call	__nfree
	pop	CX
	xor	AX,AX
	jmps	R3
;	
R2:	push	AX		;for call to LOCALREALLOC
;
	mov	CX,0FFFFh
	push	CX
	call	LOCKSEGMENT
;	
	push	word ptr P+2[BP]	;handle on stack -- push new size
	mov	CX,LMEM_NODISCARD+LMEM_ZEROINIT+LMEM_MOVEABLE
	push	CX
	call	LOCALREALLOC
;	
	push	AX
	mov	CX,0FFFFh
	push	CX
	call	UNLOCKSEGMENT
;	
	pop	AX
R3:	WINLEAVE
	ret
c_endp	_nrealloc	
  else
    if LPTR
func	_nrealloc    
        mov	AX,0
        ret
c_endp	_nrealloc        
    else
R6:	WINLEAVE
	jmp	__nfree
	
func	_nrealloc
	WINENTER

	mov	AX,P+SIZEPTR[BP]	;AX = nbytes
	tst	AX			;trying to realloc() to 0 size?
	jz	R6			;yes

	;if realloced size is smaller, attempt to just shrink current block
	mov	BX,P[BP]	;BX = p
	tst	BX		;is p NULL?
	jnz	R5		;no
	;function just like malloc(nbytes)
	push	AX
	callm	_nmalloc
        mov	SP,BP
        WINLEAVE
	ret

R5:	dec	BX
	dec	BX
	mov	CX,[BX]		;CX = # of bytes in this block
	add	AX,3
	jc	rallocerr	;overflow
	and	AL,0FEh		;AX = real new size
	sub	CX,AX
	jb	R3			;if allocating more bytes
	_if	CX b <SIZEPTR+2>, R4	;size of free list entry
	mov	[BX],AX		;realloced size of p
	add	BX,AX
	mov	[BX],CX		;size of new fragment
	inc	BX
	inc	BX
	push	BX
	callm	_nfree
	pop	BX
R4:
	mov	AX,P[BP]
	jmps	R1		;no change, return p

	;we'll have to allocate a new block, and free the old one
R3:
	push	P+SIZEPTR[BP]
	callm	_nmalloc		;malloc(nbytes)
	mov	SP,BP
	tst	AX
	jz	rallocerr	;error
	push	AX		;save pointer to new memory
	.save	<SI,DI>
	mov	SI,P[BP]	;DS:SI -> original
      ife ESeqDS
	mov	CX,DS
	mov	ES,CX
      endif
	mov	DI,AX		;ES:DI -> new item
	mov	CX,-2[SI]
	_if	CX be -2[DI], R2
	mov	CX,-2[DI]	;CX = smaller of two size
R2:	shr	CX,1		;# of words
	dec	CX		;compensate for extra word in beginning
	rep	movsw		;transfer the words
	push	P[BP]
	callm	_nfree		;free the old one
	add	SP,SIZEPTR
	.restore <DI,SI>
	pop	AX		;restore pointer to new memory
	jmp	short R1

rallocerr:
	clr	AX
R1:	WINLEAVE
	ret
c_endp	_nrealloc
    endif
  endif
	endcode nrealloc
	end
