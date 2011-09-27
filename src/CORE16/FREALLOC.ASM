;_ alloc.asm   Sun Apr	8 1990	 Modified by: Walter Bright
; Windows support added by G. Eric Engstrom Dec 1990,	-D_WINDOWS
; RATIONAL support added by P. Murray April 1990,	-DDOS16RM
; OS2 support added by Nikki Locke May 1989,		-D__OS2__
; Copyright (C) 1985-1991 by Walter Bright
; All rights reserved
; Written by Walter Bright
; $Revision: 1.1.1.1 $

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	PROTECTED	This is a protected mode executable
;	DATAMIXED	Defined if data segment is not guaranteed to
;			be below heap

ifdef DOS16RM
    PROTECTED EQU 1
endif
ifdef _WINDOWS
    PROTECTED EQU 1
    DATAMIXED EQU 1
endif
ifdef __OS2__
    PROTECTED EQU 1
    DATAMIXED EQU 1
endif


;;;;;;;;;;;;;;;;;;;;;;;;;;
; Do far pointer normalization
; SCRATCH is a scratch register we can destroy

    ifdef PROTECTED
normptr macro	MSREG,LSREG,SCRATCH
	endm
    else
normptr	macro	MSREG,LSREG,SCRATCH
	mov	SCRATCH,LSREG
	and	LSREG,0Fh
	shr	SCRATCH,1
	shr	SCRATCH,1
	shr	SCRATCH,1
	shr	SCRATCH,1
	add	MSREG,SCRATCH
	endm
    endif


    ifdef __OS2__
extrn	DOSWRITE:far
extrn	DOSEXIT:far
    endif

    if LCODE
        c_extrn _fmalloc,far,_ffree,far,farrealloc,far,dos_setblock,far
    else
        c_extrn _fmalloc,near,_ffree,near,farrealloc,near,dos_setblock,far
    endif

	begcode	frealloc
	c_public _frealloc

  if SPTR
    ifdef _WINDOWS
func	_frealloc
	WINENTER
	mov	AX,P+2[BP]
	or	AX,P[BP]
	jne	R1
	push	word ptr P+4[BP]
	call	__fmalloc
	pop	CX
	WINLEAVE
	ret
R1:	cmp	word ptr P+4[BP],0
	jne	R2
	push	word ptr P+2[BP]
	push	word ptr P[BP]
	call	__ffree
	add	SP,4
	xor	AX,AX
	cwd
	WINLEAVE
	ret
;	
R2:	push	word ptr P+2[BP]
	push	word ptr P+4[BP]
	call	_dos_setblock
	add	SP,4
;	
	mov	DX,AX
	xor	AX,AX
	WINLEAVE
	ret
c_endp	_frealloc    
    else
func	_frealloc
	WINENTER
	mov	AX,P+4[BP]
	xor	CX,CX
	push	CX
	push	AX
	push	word ptr P+2[BP]
	push	word ptr P[BP]
	call	_farrealloc
	add	SP,8
	WINLEAVE
	ret
c_endp	_frealloc    
    endif
  else
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Reallocate memory that was allocated by _fmalloc()
; Use:
;	char *realloc(char *p, unsigned nbytes)
; Returns:
;	0 error
;	else pointer to reallocated memory

R6:	WINLEAVE
	jmp	_ffree		;free(p)
	
func	_frealloc
	WINENTER

	mov	AX,P+SIZEPTR[BP]	;AX = nbytes
	tst	AX			;trying to realloc() to 0 size?
	jz	R6			;yes

	;if realloced size is smaller, attempt to just shrink current block
	mov	BX,P[BP]	;ES:BX = p
	mov	CX,P+2[BP]
	or	CX,BX		;is p NULL?
	jnz	R5		;no
	;function just like malloc(nbytes)
	push	AX
	callm	_fmalloc
	mov	SP,BP
	WINLEAVE
	ret

R8:	jmp	rallocerr

R5:	mov	ES,P+2[BP]
	dec	BX
	dec	BX
	mov	CX,ES:[BX]
	add	AX,3
	jc	R8		;overflow
	and	AL,0FEh		;AX = real new size

	;Make sure that we don't create an allocated unit that's less than
	;6 bytes.
	_if	AX ae <SIZEPTR+2>, R7
	mov	AL,SIZEPTR+2
R7:
	sub	CX,AX
	jb	R3			;if allocating more bytes
	_if	CX b <SIZEPTR+2>, R4	;size of free list entry
	.save	<DI>
	mov	DI,BX
	add	DI,AX
	mov	ES:[DI],CX	;size of new fragment
	mov	ES:[BX],AX	;realloced size of p
	mov	BX,DI
	mov	AX,ES		;AX:BX is pointer to new fragment
	normptr	AX,BX, CX	;normalize it
	inc	BX		;point past size of fragment
	inc	BX
	push	AX
	push	BX
	callm	_ffree
	add	SP,4
	.restore <DI>
R4:
	mov	DX,P+2[BP]
	mov	AX,P[BP]
	jmps	R1		;no change, return p

	;we'll have to allocate a new block, and free the old one
R3:
	push	P+SIZEPTR[BP]
	callm	_fmalloc	;malloc(nbytes)
	mov	SP,BP
	tst	DX
	jz	rallocerr	;error
	push	AX		;save pointer to new memory
	.save	<SI,DI>
	push	DX
	push	DS
	lds	SI,P[BP]	;DS:SI -> original
	mov	ES,DX
	mov	DI,AX		;ES:DI -> new item
	mov	CX,-2[SI]
	_if	CX be ES:-2[DI], R2
	mov	CX,ES:-2[DI]	;CX = smaller of two sizes
R2:	shr	CX,1		;# of words
	dec	CX		;compensate for extra word in beginning
	rep	movsw		;transfer the words
	pop	DS
	push	P+2[BP]
	push	P[BP]
	callm	_ffree		;free the old one
	add	SP,SIZEPTR
	pop	DX
	.restore <DI,SI>
	pop	AX		;restore pointer to new memory
	jmp	short R1

rallocerr:
	clr	AX
	cwd
R1:	WINLEAVE
	ret
c_endp	_frealloc
  endif
    
	endcode	frealloc
	end

