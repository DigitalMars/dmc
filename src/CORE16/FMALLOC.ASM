;_ fmalloc.asm   
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

  if LPTR
    ifdef __OS2__
	extrn	DOSWRITE:far
	extrn	DOSEXIT:far
    endif

    if LCODE
	c_extrn	sbrk,far
	extrn	___faterr:far
    else
	c_extrn	sbrk,near
	extrn	___faterr:near
    endif

    ifdef _MT
      if LCODE
	extrn	__semerr:far
      else
	extrn	__semerr:near
      endif
    endif
  else
    ifdef _WINDOWS
      if LCODE
	extrn	_dos_alloc:far
	extrn	_dos_free:far
      else
        extrn	_dos_alloc:near
        extrn	_dos_free:near
      endif
    else  
      if LCODE
	extrn	_farmalloc:far
	extrn   _farfree:far
      else
        extrn	_farmalloc:near
        extrn	_farfree:near
      endif
    endif
  endif  

	begcode	fmalloc
	c_public _fmalloc,_ffree
	
; Storage allocator	

  if LPTR	
	begdata
	c_public _allocp
	extrn __baslnk:word
    ifdef DOS16RM
	c_public _almblksiz
    endif

__allocp	dw	-1,?
    ifdef DOS16RM
__almblksiz	dw	1000h		; Minimum block allocation size.
    endif
_allocp	equ	__allocp
_baslnk equ	__baslnk

    ifdef _WINDOWS
heapmsg		db	'Heap is corrupted',0
    else
heapmsg		db	0Dh,0Ah,'Heap is corrupted',0Dh,0Ah,0
    endif

	enddata
  endif

; A block in the free list consists of:
;	dw	pointer to next block in list
;	dw	segment of next block in list (for LPTR)
;	dw	size of block in bytes (must be even) (including both words)

; When it's allocated,
;	dw	# of bytes in this block including this word
;	db...	the bytes allocated

_if32	macro	r1H,r1L,b,r2H,r2L,lbl
	local	L1
	_if	r1H ne r2H, L1
	cmp	r1L,r2L
L1:	j&b	lbl
	endm

mov32	macro	ah,al,bh,bl
	mov	ah,bh
	mov	al,bl
	endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Handle locking/unlocking for multithreaded programs

  if LPTR
    ifdef _MT			;if multi-threaded library support

;extrn	DOSENTERCRITSEC:far
;extrn	DOSEXITCRITSEC:far
extrn	DOSSEMREQUEST:far
extrn	DOSSEMCLEAR:far

talloc_lock macro	RX
	local	L1
    ifnb <RX>
	push	RX		;save register
    endif
    if 0
	call	DOSENTERCRITSEC
    else
;	mov	AX,DGROUP
;	push	AX
	push	DS
	mov	AX,offset DGROUP:alloc_sem
	push	AX
	mov	AX,-1
	push	AX
	push	AX		;wait until semaphore is unowned
	call	DOSSEMREQUEST
	tst	AX
	jz	L1
    if LCODE
	jmp	semerr
    else
	jmp	__semerr
    endif
L1:
    endif
    ifnb <RX>
	pop	RX
    endif
	endm

talloc_unlock macro	RX
	local	L1
    ifnb <RX>
	push	RX		;save register
    endif
    if 0
	call	DOSEXITCRITSEC
    else
;	mov	AX,DGROUP
;	push	AX
	push	DS
	mov	AX,offset DGROUP:alloc_sem
	push	AX
	call	DOSSEMCLEAR
	tst	AX
	jz	L1
    if LCODE
	jmp	semerr
    else
	jmp	__semerr
    endif
L1:
    endif
    ifnb <RX>
	pop	RX
    endif
	endm

	begdata
alloc_sem	dd	0	;ram semaphore
	enddata

    else
talloc_lock macro
	endm

talloc_unlock macro RX
	endm
    endif
  endif  

  if LPTR
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; When out of memory, this function pointer is called in an attempt to get more.
; The function pointer can be set by the application to point at a function
; that can, say, free up some disk buffers.
; Returns:
;	0	unsuccessful at finding more memory to free
;	!=0	found some, try allocating it

	ifndef __OS2__

func	default_malloc_handler
	clr	AX		;didn't find any
	ret
c_endp	default_malloc_handler

	begdata

	public	__malloc_handler

    if LCODE
__malloc_handler	dd	default_malloc_handler
    else
__malloc_handler	dw	offset default_malloc_handler
    endif

	enddata
	endif
  endif	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Something went wrong with the semaphores.
; Print an error message and abort the program.

  if LPTR
    ifdef _MT
    if LCODE
semerr	proc	near
	jmp	__semerr
semerr	endp
    endif
    endif
  endif  

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; The heap has been corrupted.
; Print an error message and abort the program.

  if LPTR
heaperr	proc	near
	mov	AX,offset DGROUP:heapmsg
	push	AX
	jmp	___faterr
heaperr	endp
  endif

  if LPTR
;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Allocate a block of data.
; Use:
;	char *malloc();
;	p = malloc(nbytes);
; Returns:
;	pointer to allocated data else NULL

	even

A17:	mov	AX,6			;minimum allocation size
	jmp	short A5

A16:	;Initialize
	mov	_baslnk+2,DS
	mov32	AX,BX _baslnk+2,_baslnk
    ifndef PROTECTED
	normptr	AX,BX, CX		;normalize _baslnk
	mov32	_baslnk+2,_baslnk AX,BX
    endif
	mov32	_allocp+2,_allocp AX,BX
	talloc_unlock
	even
    
func	_fmalloc
	talloc_lock
	_if	_allocp e -1, A16	;if not initialized
	WINENTER
	.save	<SI,DI>
	push	DS
;	nbytes = (nbytes + 3) / 4 * 4
A4:	mov	AX,P[BP]
	add	AX,3
	and	AL,0FEh
;	if (nbytes < 4)
;		return 0
;	_if	AX b 4, mallocerr   ;; For Microsoft Compatability, allow 0 length allocations
;	if (nbytes < 6)
;		nbytes = 6
	_if	AX b 6, A17
A5:
	lds	DI,dword ptr _baslnk		;pstart = _baslnk
	mov	DX,DS
	mov32	CX,SI DS,DI			;p = pstart

	even
	; We've unrolled this loop 3 times
A7:
	lds	BX,[SI]			;pnext = p->next
	_if	AX be 4[BX], A6		;if (nbytes <= pnext->size)
	mov	SI,BX			;p = pnext
	mov	CX,DS
	_if	SI e DI, M1

	lds	BX,[SI]			;pnext = p->next
	_if	AX be 4[BX], A6		;if (nbytes <= pnext->size)
	mov	SI,BX			;p = pnext
	mov	CX,DS
	_if	SI e DI, M1

	lds	BX,[SI]			;pnext = p->next
	_if	AX be 4[BX], A6		;if (nbytes <= pnext->size)
	mov	SI,BX			;p = pnext
	mov	CX,DS
	_if	SI ne DI, A7		;if (p != pstart)
M1:	_if	CX ne DX, A7

	;We've wrapped around, meaning no block is big enough
;			p = wsbrk(nbytes)
	mov	DS,-6[BP]
    ifdef __OS2__
	_if	AH ae 10h, A14
	mov	AX,4096		;4096 minimum growth size
    else ;__OS2__
    ifdef DOS16RM
	_if	AX a _almblksiz, A14	;allocate a new allocation
					;block from the system. Clamp min
	mov	AX, _almblksiz		;allocation size to 4K bytes.
	add	AX, 2		;for our overhead
	jc	mallocerr	;64K allocations are illegal
    else ;DOS16RM
    ifdef _WINDOWS
	_if	AH ae 80h, A14
	mov	AX,32768	;32768 minimum growth size SCC need to try successively smaller chunks if this doesn't work
    else ;_WINDOWS
	_if	AH ae 2, A14
	mov	AX,512		;512 minimum growth size
    endif ;_WINDOWS
    endif ;DOS16RM
    endif ;__OS2__
A14:	push	AX
	callm	sbrk		;extend program segment
	pop	BX		;fix stack
;			if (p == -1)
;				return 0
	inc	AX
	jz	A15		;error (p == -1)
	inc	AX
	push	DX
	push	AX
	callm	free_nlock	;add allocated memory into free list
	talloc_lock
	pop	AX
	pop	AX
	jmp	A4
A15:
    ifndef __OS2__
	;Try calling __malloc_handler to see if it can
	;find any memory

	talloc_unlock
	call	__malloc_handler	; SCC HERE WE PUT RETRY FOR JUST THE AMOUNT HE NEEDS
	talloc_lock AX
	tst	AX
	jnz	A4		;found some, try again
    endif
mallocerr:
	clr	AX
	cwd
	pop	DS
	jmp	A11

A6:	; We have:
	;	pnext -> block to alloc
	;	pnext = DS:BX

;	if (nbytes + sizeof(*pnext) > pnext->size)
	mov	DX,AX
	add	DX,6
	jc	A10
	_if	DX be 4[BX], A9

;		Allocate entire block that pnext points to.
A10:	mov32	DX,DI 2[BX],[BX]
	mov	ES,CX
	mov32	ES:2[SI],ES:[SI] DX,DI	;p->next = pnext->next

	mov	DX,4[BX]
	mov	[BX],DX			;*pnext = pnext->size

	jmp	A8

A9:
; Create new block pnew that consists of the remainder of pnext.
; At this point, we have:
;	AX = nbytes
;	CX:SI = p
;	DS:BX = pnext

;	pnew = pnext + nbytes

	mov	DI,BX
	mov	DX,DS
	add	DI,AX			;no overflow possible here
	normptr	DX,DI, BP		;DX:DI = pnew

	push	AX
	neg	AX
	add	AX,4[BX]
	mov	ES,DX
	mov	ES:4[DI],AX		;pnew->size = pnext->size - nbytes

	mov	AX,[BX]
	mov	ES:[DI],AX
	mov	AX,2[BX]
	mov	ES:2[DI],AX		;pnew->next = pnext->next

	mov	ES,CX
	mov32	ES:2[SI],ES:[SI] DX,DI	;p->next = pnew

	pop	[BX]			;*pnext = nbytes

A8:	;return pnext + 2
	; DS:BX = pnext
	mov	DX,DS
	mov	AX,BX
	inc	AX
	inc	AX

	pop	DS
	mov32	_allocp+2,_allocp CX,SI			;_allocp = p
A11:	.restore <DI,SI>
	WINLEAVE
	talloc_unlock AX
	ret
c_endp	_fmalloc
  endif    
  
  if SPTR
    ifdef _WINDOWS
func	_fmalloc
	push	BP
	mov	BP,SP
	mov	AX,P[BP]	; bytes to allocate
	add	AX,0Fh		; convert to paragraphs
	shr	AX,1
	shr	AX,1
	shr	AX,1
	shr	AX,1
	jz	L53		; if zero, return NULL (allocation failed)
	push	AX
	callp	_dos_alloc	; allocate paragraphs from OS
	pop	CX		
L53:	mov	DX,AX		; make selector into a far pointer
	xor	AX,AX
	pop	BP		
	ret
c_endp	_fmalloc	
    else
func 	_fmalloc    
	push	BP
	mov	BP,SP
	mov	AX,P[BP]	; bytes to allocate
	xor	CX,CX		; convert to long, to match farmalloc
	push	CX		;   parameter
	push	AX
	callp	_farmalloc	; let farmalloc do the work
	add	SP,4
	pop	BP
	ret
c_endp	_fmalloc	
    endif
  endif	
  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; free() for large data models

    if LPTR
func	_ffree
	talloc_lock

_free_nlock:
	WINENTER
	.save	<SI,DI>
	push	DS

	mov32	DX,DI P+2[BP],P[BP]	;DX:DI = pfree
	mov	AX,DX
	or	AX,DI			;if (pfree == NULL)
	jz	F9			;return 0

	test	DI,1			;odd pointers are errors
	jne	Ferr

	dec	DI
	dec	DI			;pfree -= 2

    ifndef DATAMIXED
;	p = (_allocp < pfree) ? _allocp : &_baslnk
	mov32	CX,SI _allocp+2,_allocp		;CX:SI = _allocp
	_if32	CX,SI b DX,DI, F1
	mov32	CX,SI DS,<offset DGROUP:_baslnk>
    endif
	jmp	short F1

F9:	jmp	F4
Ferr:	jmp	freeerr

	even

    ifdef DATAMIXED
F1:		mov32	AX,BX DS,<offset DGROUP:_baslnk>
F6:		mov32	CX,SI AX,BX		;p = pnext
		mov	ES,CX			;ES:SI = p
		mov32	AX,BX ES:2[SI],ES:[SI]	;pnext = p->next

		_if32	CX,SI ae DX,DI, F7	;if (p >= pfree) goto F7
		_if32	DX,DI b  AX,BX, F2	;if (pfree < pnext) break;
F7:		_if32	AX,BX a  CX,SI, F6	;if (pnext > p) goto F8
		_if32	CX,SI b  DX,DI, F2	;if (p < pfree) break;
		_if32	DX,DI b  AX,BX, F2	;if (pfree < pnext) break;
F8:		jmp	F6
		;_if32	AX,BX ne ecx, F6

    else
F10:
		_if32	AX,BX be CX,SI, F2	;if (pnext <= p) break
		mov32	CX,SI AX,BX		;p = pnext

F1:		mov	ES,CX			;ES:SI = p
		mov32	AX,BX ES:2[SI],ES:[SI]	;pnext = p->next

;		_if32 CX,SI ae DX,DI, Ferr	;if (p >= pfree) then error

		_if	AX b DX, F10
		ja	F2
		_if	BX be DI, F10		;if (pnext > pfree) break
    endif
F2:
	mov32	_allocp+2,_allocp CX,SI		;_allocp = p

	mov	DS,DX
	mov	BP,[DI]
	mov	4[DI],BP		;pfree->size = *pfree

	mov32	2[DI],[DI] AX,BX	;pfree->next = pnext

	mov	DS,CX
	mov32	2[SI],[SI] DX,DI	;p->next = pfree

	mov32	AX,BX CX,SI
	add	BX,4[SI]
   ifndef PROTECTED
	normptr	AX,BX, CX		;AX,BX = p + p->size
	mov	CX,DS			;restore CX trashed by normptr
   endif
	_if32	AX,BX e DX,DI, F3	;if (p + p->size != pfree)
		mov32	CX,SI DX,DI	;p = pfree
F3:
;	while (p + p->size == p->next)
	mov	DS,CX
	mov32	AX,BX CX,SI
	add	BX,4[SI]
ifdef PROTECTED
	jc	F4
endif
	normptr	AX,BX, DX		;normalize AX,BX

	mov32	DX,DI 2[SI],[SI]	;DX:DI = p->pnext
	mov	ES,DX
	_if32	AX,BX ne DX,DI, F4

;		if (p->size + p->next->size < 64k)
		mov	BP,4[SI]
		add	BP,ES:4[DI]
    ifdef DOS16RM
	jc	F4			;Yes - then we can't combine it.
	push	BX
	dec	BP
	mov	BX, DS			;Get the segment length
	lsl	BX, BX
	cmp	BP, BX			;Have we exceeded the segment length?
	pop	BX
	jae	F5			;Yes -	overflowed a segment, all done.
	inc	BP
	mov	4[SI],BP		;No - Save the new combined size
	mov	AX,ES:[DI]		; and move next link to previous block.
	mov	[SI],AX
	mov	AX,ES:2[DI]
	mov	2[SI],AX		;Keep looping until all consecutive
	jmp	F3			; blocks have been combined.

					;We get here if the consecutive blocks
					; exceed 64K.
F5:	jne	freeerr			;If not exactly 64K, it's an error.
    else
		jc	F5
			mov	4[SI],BP	;p->size += p->next->size

			mov	AX,ES:[DI]
			mov	[SI],AX
			mov	AX,ES:2[DI]
			mov	2[SI],AX	;p->next = p->next->next
			jmp	F3
;		else
F5:
;			pnew = p + 64k - 16		;CX:SI = p
			mov	AX,CX
			add	AX,0FFFh		;AX:SI = pnew
			mov	DS,AX			;DS:SI = pnew

;			pnew->next = p->next->next	;ES:DI = p->next
			mov32	AX,BX ES:2[DI],ES:[DI]
			mov32	2[SI],[SI] AX,BX

;			pnew->size = p->size + p->next->size - (64k - 16)
			sub	BP,0FFF0h
			mov	4[SI],BP
;			p->next = pnew
			mov	AX,DS
			mov	DS,CX
			mov32	2[SI],[SI] AX,SI
;			p->size = (64k - 16)
			mov	4[SI],0FFF0h
;			p = pnew
			mov	CX,AX	;only the segments are different
			jmp	F3
    endif
F4:
	clr	AX
	cwd			;return 0L on success (for backwards compatibility)
	pop	DS
	.restore <DI,SI>
	WINLEAVE
	talloc_unlock
	ret
;
freeerr:
	pop	DS
	jmp	heaperr
c_endp	_ffree
    endif
    
  if SPTR
    ifdef _WINDOWS
func	_ffree
	push	BP
	mov	BP,SP
	mov	AX,P+2[BP]
	or	AX,P[BP]
	je	F10
	push	word ptr P+2[BP]
	callp   _dos_free
	pop	CX
F10:	pop	BP
	ret
c_endp	_ffree    
    else
func	_ffree
	jmp	_farfree
c_endp  _ffree
    endif
  endif

	endcode	fmalloc
	end
