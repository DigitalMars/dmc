;_ nmalloc.asm
; Copyright (C) 1985-1992 by Digital Mars
; All rights reserved
; Written by Walter Bright
; $Revision: 1.1.1.1 $

include macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	DATAMIXED	Defined if data segment is not guaranteed to
;			be below heap

ifdef __NT__
    DATAMIXED EQU 1
endif
ifdef __OS2__
    DATAMIXED EQU 1
endif


	extrn _sbrk:near, _write:near, __exit:near

	public __nmalloc,__nfree

; Storage allocator

	begdata
	public	__baslnk
	public	__allocp

    if _FLAT
__baslnk	dd	offset FLAT:__baslnk	;starting link for
						; storage allocator
		dd	0	;give it a size of 0 so it is never allocated
__allocp	dd	offset FLAT:__baslnk	;roving pointer for allocator
    else

	extrn	__pastdata:dword
	extrn	__heapbottom:dword

__baslnk	dd	offset DGROUP:__baslnk	;starting link for
						; storage allocator
		dd	0	;give it a size of 0 so it is never allocated
__allocp	dd	offset DGROUP:__baslnk	;roving pointer for allocator
    endif

heapmsg		db	0Ah,'Heap is corrupted',0Ah
heapmsglen	equ	$-heapmsg

; A block in the free list consists of:
;free_block	struc
;next	dd	?	;pointer to next block in list
;f_size	dd	?	;size of block in bytes including free_block struc
;free_block	ends	;(must be multiple of 4)


; When it's allocated,
;used_block	struc
;u_size	dd	?	;# of bytes in this block including this struc
;used_block	ends

	public	__malloc_handler

    if _FLAT
__malloc_handler	dd	offset FLAT: default_malloc_handler
    else
__malloc_handler	dd	offset _TEXT: default_malloc_handler
    endif

	enddata

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Handle locking/unlocking for multithreaded programs

    ifdef _MT			;if multi-threaded library support

  ifdef __OS2__

extrn	DOSCREATEMUTEXSEM:near
extrn	DOSRELEASEMUTEXSEM:near
extrn	DOSREQUESTMUTEXSEM:near

talloc_lock macro	RX
	local	L1
    ifnb <RX>
	push	RX		;save register
    endif
	push	0FFFFFFFFh	;wait until semaphore is unowned
	push	___alloc_sem
	call	DOSREQUESTMUTEXSEM
	add	ESP,2*4
	tst	EAX
	jz	L1
	jmp	__semerr
L1:
    ifnb <RX>
	pop	RX
    endif
	endm

talloc_unlock macro	RX
	local	L1
    ifnb <RX>
	push	RX		;save register
    endif
	push	___alloc_sem
	call	DOSRELEASEMUTEXSEM
	add	ESP,4
	tst	EAX
	jz	L1
	jmp	__semerr
L1:
    ifnb <RX>
	pop	RX
    endif
	endm

	begdata

___alloc_sem	dd	0	;ram semaphore
	enddata

  else
  ifdef __NT__

extrn	__LockSemaphore   : near
extrn	__UnlockSemaphore : near

talloc_lock macro	RX
    ifnb <RX>
	push	RX		;save register
    endif

	push	1					; !const: _semAlloc
	call	__LockSemaphore
	add		esp, 4

    ifnb <RX>
	pop	RX
    endif
	endm

talloc_unlock macro	RX
    ifnb <RX>
	push	RX		;save register
    endif

	push	1					; !const: _semAlloc
	call	__UnlockSemaphore
	add		esp, 4

    ifnb <RX>
	pop	RX
    endif
	endm

    endif	; __NT__
    endif	; __OS2__

    else
talloc_lock macro
	endm

talloc_unlock macro RX
	endm
    endif	; !_MT

	begcode nmalloc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Initialize storage allocator

	public	__allocinit
__allocinit proc	near
    ifdef _MT
    ifdef __OS2__
	clr	EAX
	push	EAX			;semaphore as reset
	push	EAX			;attr of semaphore

	push	offset FLAT:___alloc_sem ;address of handle
	push	EAX			;create anonymous semaphore
	call	DOSCREATEMUTEXSEM
	add	ESP,4*4
	tst	EAX
	jz	H1
	jmp	__semerr
H1:	ret

    else
    ifdef __NT__
	ret

    endif
    endif
    else
	ret
    endif

__allocinit endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Wrap up storage allocator

	public	__allocwrap
__allocwrap proc	near

    ifdef _MT
    ifdef __NT__
    
    endif
    endif
    
        ret
__allocwrap endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; When out of memory, this function pointer is called in an attempt to get more.
; The function pointer can be set by the application to point at a function
; that can, say, free up some disk buffers.
; Returns:
;	0	unsuccessful at finding more memory to free
;	!=0	found some, try allocating it

default_malloc_handler proc    near
	clr	EAX		;didn't find any
	ret
default_malloc_handler endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Allocate a block of data.
; Use:
;	char *_nmalloc();
;	p = _nmalloc(nbytes);
; Returns:
;	pointer to allocated data else NULL

	_align

__nmalloc proc	near
	talloc_lock
	push	ebp
A4:
	mov	ebp,esp
	uses	<EBX,ECX,EDX,ESI,EDI>

	mov	eax,P[ebp]	;get nbytes
	add	eax,7		;need another dword for length info

	and	AL,NOT 3		;round up to nearest dword
	_if	eax ge 8, allocnonzero	;There are > 0 bytes, no more rounding 
	mov     eax,8  ;; allow allocating of Zero Bytes

allocnonzero:	

	mov	ebp,4		;save some bytes
	mov	esi,__baslnk	;last item
	
	
	mov	ecx,esi		;ECX to save bytes

    ifdef __OS2__
	test	EAX,0FFFF0000h		;is allocation request > 64Kb?
	jnz	A2			;yes, skip alignment checks
	jmps	A22

	_align
A21:	mov	ESI,EDI
	_if	ESI e ECX, trysbrk	;wrapped around, didn't find any
A22:	mov	EDI,[ESI]		;next item in list
	mov	EDX,[EDI+EBP]		;EDX = size of free block
	sub	EDX,EAX
	jb	short A21		;not big enough
	je	short A7		;exactly big enough
	_ifs	EDX be EBP, A7		;if EDX is 0 or 4

	;Try to allocate from the bottom
	mov	EBX,EDI
	add	BX,AX
	jnc	short A9		;does not cross a 64K boundary

	;Try to allocate from the top
	mov	EBX,EDI
	add	EBX,[EDI+EBP]		;EBX points past end of free block
	sub	BX,AX
	jc	short A21		;crosses a 64K boundary

	;Allocate from top of free block
	sub	[EDI+EBP],EAX		;reduce size of free block
	mov	[EBX],EAX		;set size of allocated block
	mov	__allocp,EDI
	lea	EAX,[EBX+EBP]
	jmp	short A6
A7:
	mov	EBX,EDI
	add	BX,AX
	jc	short A21		;if crossed a 64K boundary
	add	EAX,EDX
	jmp	short A3
    else
	jmps	A2
    endif

	_align
A1:	mov	esi,edi
	_ifs	esi e ecx, trysbrk	;wrapped around, didn't find any
A2:	mov	edi,[esi]		;next item in list
	_ifs	eax a [edi+ebp], A1	;not big enough

	je	short A3		;exactly big enough
	add	eax,ebp			;we'll need another 4 bytes
	_ifs	eax e [edi+ebp],A3	;have to allocate an entire block
	sub	eax,ebp

;Allocate from bottom of free block.  Desirable in order to delay
;stack overflow as long as possible.
; edi -> free block
; esi -> previous free block
; eax =  # of bytes in allocated block
A9:	add	[esi],eax	;link to new free block
	mov	esi,[esi]	;pointer to new free block
	mov	ecx,[edi+ebp]	;number of bytes in block we're splitting
	sub	ecx,eax		;ECX = remaining bytes
	mov	[esi+ebp],ecx	;# of bytes in this block

	;Current block becomes the allocated block
	; EDI -> block to allocate
	; ESI -> previous free block
	; EAX =  number of bytes in EDI
A3:	xchg	eax,[edi]	;[EDI] = # of bytes, EAX = next free block
	mov	[esi],eax	;skip the DI entry in list
	mov	__allocp,esi
	lea	eax,[edi+ebp]	;pointer to area allocated (EDI + 4)

A6:	talloc_unlock EAX
	unuse	<edi,esi,edx,ecx,ebx>
	pop	ebp
	_ret	4

trysbrk:			;try sbrk() to grow our data segment

    ifdef __NT__
	add	eax,03FFFFh	;round allocation request up to 256K
	and     eax,0FFFC0000h
    else	
	add	eax,0FFFFh	;round allocation request up to 64K
	xor	ax,ax
    endif	
	push	eax
	callm	sbrk
	pop	ebx
	_ifs	eax e -1, allocerr	 ;failed

	mov	[eax],ebx	;Number of bytes allocated.
	add	eax,4		;point past # of bytes allocated
	push	eax
	callm	free_nlock	;add allocated memory into free list
	talloc_lock
    ife _STDCALL
	add	esp,4
    endif
	unuse	<edi,esi,edx,ecx,ebx>
	jmp	A4		;try again

allocerr:
	clr	EAX		;NULL
	jmp	A6
__nmalloc endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Free memory that was allocated by _nmalloc().
; Use:
;	_nfree(p);

__nfree	proc	near
	talloc_lock

_free_nlock:
	push	ebp
	mov	ebp,esp
	uses	<EBX,ECX,ESI,EDI>
	mov	ebx,P[ebp]		;get p
	tst	ebx			;pass a NULL pointer?
	jz	F5			;yes, return 0
	mov	ebp,SIZEPTR		;to save some bytes

    ifndef DATAMIXED
	_ifs	ebx be __heapbottom, heaperr	;if below bottom of pool
	_ifs	ebx ae __pastdata, heaperr	;if above top of pool
    endif
	test	bl,3			;Not multiple of 4?
	jne	heaperr
	sub	ebx,ebp			;point to start of block
	mov	eax,[ebx]		;# of bytes in block to be freed

; Try to find ESI and EDI such that ESI < EBX < EDI

  ifdef DATAMIXED
	;Try to find logically esi < p < edi
	;This is different in that __baslnk is
	;not guaranteed to lie below the heap.

	mov	ecx, offset FLAT:__baslnk
	mov	edi,ecx
	_align
F6:	mov	esi,edi
	mov	edi,[esi]
	_ifs	esi ae ebx, F7
	_ifs	ebx b edi, F2
F7:	_ifs	edi a esi, F8
	_ifs	esi b ebx, F2
	_ifs	ebx b edi, F2
F8:	_ifs edi ne ecx, F6
  else
	; __baslnk is guaranteed to lie below the heap
	mov	esi,__allocp		;try our roving pointer
	_ifs	esi b ebx, F1		;a good starting point
    if _FLAT
	mov	esi, offset FLAT:__baslnk
    else
	mov	esi, offset DGROUP:__baslnk
    endif
	mov	edi,esi

	_align
F6:	mov	esi,edi
F1:	mov	edi,[esi]		;the next in the list
	_ifs	esi ae ebx, heaperr	;could happen in OS/2 DLLs
	_ifs	edi a ebx, F2		;got it
	_ifs	edi a esi, F6		;no wrap around (ESI < EDI < EBX)
  endif

; We have ESI < EBX < EDI (relative position in list)

F2:	mov	ecx,[esi+ebp]		;# of bytes in previous block
	add	ecx,esi			;+ link
	_ifs	ecx ne ebx, F3		;if can't collapse with prev block
	add	[esi+ebp],eax		;Number of bytes in block to be freed.
	jmps	F4

F3:	mov	[ebx+ebp],eax		;store # of bytes in freed block
	mov	[ebx],edi		;link to block after EBX
	mov	[esi],ebx		;link to EBX
	mov	esi,ebx

; See if we can collapse SI with DI
; SI -> block just before DI
; DI -> block just after SI
; BP = SIZEPTR

F4:	mov	__allocp,esi		;for next time
	mov	eax,[esi+ebp]
	add	eax,esi
	_ifs	eax ne edi, F5		;nope
	mov	eax,[edi]		;link after EDI
	mov	[esi],eax		;becomes link after ESI
	mov	eax,[edi+ebp]		;# of bytes in EDI
	add	[esi+ebp],eax		;add to # of bytes in ESI

F5:	talloc_unlock
	clr	eax			;success
	unuse	<edi,esi,ecx,ebx>
	pop	ebp
	_ret	4

__nfree	endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; The heap has been corrupted. Print an error message and abort the program.

heaperr proc	near
	push	heapmsglen
    if _FLAT
	push	offset FLAT:heapmsg
    else
	push	offset DGROUP:heapmsg
    endif
	push	2		;Stderr
	call	_write

	push	1		;error exit code
	call	__exit
	hlt			;Should never reach here.
heaperr endp
	endcode nmalloc
	end
	
