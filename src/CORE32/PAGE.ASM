;_ page.asm   Wed May 24 1989   Modified by: Walter Bright */
; Copyright (C) 1989-1993 by Digital Mars
; All rights reserved
; Written by Walter Bright

include	macros.asm

; Storage allocator
; The page layout is set up to be binary compatible with the
; 16 bit version.

	begcode	page

	c_public page_malloc,page_calloc,page_realloc,page_free

; The start of each page (set by page_initialize()) looks like:
pageheader struc
	pagesize	dw	?	;total size of this page
	maxsize		dw	?	;max size of a free block in free list
	allocp		dw	?	;roving pointer for allocator
	bassize		dw	?	;size of first block (0 so it
					; is never allocated)
	baslnk		dw	?	;offset of next free block
pageheader ends
pageoverhead	equ	10	;# of bytes of bookkeeping
__PGOFF		equ	2	;size of overhead

; A block in the free list consists of:
;	dw	size of block in bytes (must be even) (including both words)
;	dw	pointer to next block in list

; When it's allocated,
;	dw	# of bytes in this block including this word
;	db...	the bytes allocated


;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Allocate a block of data and clear it.
; Use:
;	unsigned page_calloc(void *baseptr,unsigned size);
; Returns:
;	offset of allocated data else 0

	_align
func	page_calloc
	push	EDI
	mov	EDI,4+PS[ESP]
	push	4+PS+4[ESP]
	push	EDI
	callm	page_malloc	;allocate a chunk
	add	ESP,8
	tst	EAX		;out of memory?
	jz	short C2	;yes
	mov	EDX,EAX		;save offset of result
	add	EDI,EAX		;offset to start of allocated memory
	movzx	ECX,word ptr -__PGOFF[EDI]	;# of bytes
	clr	EAX
	shr	ECX,1		;# of words (including byte count)
    ife ESeqDS
	push	DS
	dec	ECX		;skip byte count
	pop	ES
    else
	dec	ECX		;skip byte count
    endif
	shr	ECX,1		;to dword count
	rep	stosd
	rcl	ECX,1
	rep	stosw		;clear the memory
	mov	EAX,EDX		;restore offset of result
C2:
	pop	EDI
	ret
c_endp	page_calloc


;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Allocate a block of data.
;	unsigned page_malloc(void *baseptr,unsigned size);
; Returns:
;	offset of allocated data else 0

	_align
func	page_malloc
	uses	<EBX,ESI,EDI>
	mov	EAX,12+PS+4[ESP]	;get size
	mov	EBX,12+PS[ESP]		;DS:EBX = baseptr
	add	EAX,3			;need another word for length info
	and	AL,0FEh			;round up to nearest word
	_ifs	EAX ae 10000h-__PGOFF, allocerr	;can't allocate > 64k
	_ifs	EAX b 4, allocerr		;can't allocate 0 bytes

;	movzx	ESI,allocp[EBX]	;last item
	movzx	ESI,baslnk[EBX]	;last item
	clr	EDI		;zero high word
	mov	ECX,ESI		;ECX to save bytes
	jmps	A2

	_align
A1:	mov	ESI,EDI
	_ifs	ESI e ECX, allocerr	;wrapped around, didn't find any
A2:	mov	DI,__PGOFF[EBX][ESI]	;next item in list
	_ifs	AX be [EBX+EDI], A4	;big enough

	mov	ESI,EDI
	_ifs	ESI e ECX, allocerr	;wrapped around, didn't find any
	mov	DI,__PGOFF[EBX][ESI]	;next item in list
	_ifs	AX a [EBX+EDI], A1	;not big enough

A4:	je	short A3		;exactly big enough
	add	EAX,__PGOFF		;we'll need another __PGOFF bytes
	_ifs	AX e [EBX+EDI],A3	;have to allocate an entire block
	sub	EAX,__PGOFF

;Allocate from bottom of free block.
; EDI -> free block
; ESI -> previous free block
; EAX =  # of bytes in allocated block
	add	__PGOFF[EBX+ESI],AX	;link to new free block
	mov	CX,[EBX+EDI]		;number of bytes in block we're splitting
	mov	SI,__PGOFF[EBX+ESI]	;pointer to new free block
	sub	ECX,EAX			;ECX = remaining bytes
	mov	[EBX+ESI],CX		;# of bytes in this block
	mov	[EBX+EDI],AX		;[EDI] = # of bytes

A3:	mov	AX,__PGOFF[EBX+EDI]	;EAX = next free block
	mov	__PGOFF[EBX+ESI],AX	;skip the EDI entry in list
	mov	allocp[EBX],SI
	lea	EAX,__PGOFF[EDI]	;pointer to area allocated (EDI + __PGOFF)
	mov	maxsize[EBX],0		;recalculate size of largest available block
A6:	unuse	<EDI,ESI,EBX>
	ret

allocerr:
	clr	EAX		;NULL
	jmp	A6
c_endp	page_malloc


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Reallocate memory that was allocated by page_malloc() or page_calloc().
; Use:
;	unsigned page_realloc(void *baseptr,unsigned p, unsigned nbytes)
; Returns:
;	0 error
;	else offset of reallocated memory

	_align
func	page_realloc
	mov	EAX,PS+4+4[ESP]		;EAX = nbytes
	tst	EAX			;trying to realloc() to 0 size?
	jnz	short R6		;no
	jmp	near ptr page_free	;page_free(baseptr,p)

R6:
	;If p is 0, this is just a page_malloc()
	mov	EDX,PS+4[ESP]		;EDX = p
	tst	EDX			;is p NULL?
	jnz	short R5		;no
	;function just like page_malloc(baseptr,nbytes)
	mov	EDX,PS[ESP]		;baseptr
	push	EAX
	push	EDX
	callm	page_malloc
	add	ESP,8
	ret

	;if realloced size is smaller, attempt to just shrink current block
R5:
	push	EBP
	mov	EBP,ESP

	push	ESI
	mov	ESI,P[EBP]		;ESI = baseptr
	sub	EDX,__PGOFF
	clr	ECX
	add	EAX,3
	mov	CX,word ptr [ESI+EDX]	;ECX = # of bytes in this block
	and	AL,0FEh			;EAX = real new size
	sub	ECX,EAX
	jb	short R3		;if allocating more bytes
	_ifs	ECX b 4, R4		;size of free list entry
	mov	[ESI+EDX],AX		;realloced size of p
	add	EDX,EAX			;EDX -> new fragment
	mov	[ESI+EDX],CX		;size of new fragment
	add	EDX,__PGOFF
	push	EDX
	push	P[EBP]
	callm	page_free
	add	ESP,8
R4:
	mov	EAX,P+4[EBP]
	jmps	R1			;no change, return p

	;we'll have to allocate a new block, copy the data over,
	;and free the old one
R3:
	push	P+8[EBP]
	push	P[EBP]
	callm	page_malloc	;page_malloc(baseptr,nbytes)
	add	ESP,8
	tst	EAX
	jz	short rallocerr	;error
	push	EAX		;save pointer to new memory
	uses	<EDI>
    ife ESeqDS
	push	DS
	pop	ES
    endif
	mov	EDI,EAX
	add	EDI,ESI		;ES:EDI -> new item
	add	ESI,P+4[EBP]	;DS:ESI -> original
	movzx	ECX,word ptr -__PGOFF[ESI]
	_ifs	CX be -__PGOFF[EDI], R2
	mov	CX,-__PGOFF[EDI]	;ECX = smaller of two sizes
R2:	shr	ECX,1		;# of words
	dec	ECX		;compensate for extra word in beginning
	shr	ECX,1		;to dword count
	rep	movsd
	rcl	ECX,1
	rep	movsw		;transfer the words
	push	P+4[EBP]
	push	P[EBP]
	callm	page_free	;free the old one
	add	ESP,8
	unuse	<EDI>
	tst	EAX
	pop	EAX		;restore pointer to new memory
	jz	short R1
rallocerr:
	clr	EAX
R1:	pop	ESI
	pop	EBP
	ret
c_endp	page_realloc


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Free memory that was allocated by page_malloc() or page_calloc().
; Use:
;	int page_free(void *baseptr,unsigned p);
; Returns:
;	0	success
;	-1	error

	_align
func	page_free
	mov	EAX,PS+4[ESP]		;get p
	tst	EAX			;pass a NULL pointer?
	jz	F8			;yes, return 0
	mov	EDX,EAX
	uses	<EBX,ESI,EDI>
	mov	EBX,4+4+4+PS[ESP]		;DS:EBX = baseptr
	_if	EDX be baslnk+1, freeerr	;if below bottom of pool
	_if	EDX ae 10000h, freeerr		;if high 16 bits is not clear
	_if	DX ae pagesize[EBX], freeerr	;if above top of pool
	test	DL,1			;odd?
	jne	freeerr
	sub	EDX,__PGOFF		;point to start of block
	mov	ESI,EDX
	movzx	EAX,word ptr [EBX+ESI]	;# of bytes in block to be freed

; Try to find ESI and EDI such that ESI < EDX < EDI

	clr	EDI			;zero high word
	mov	SI,allocp[EBX]		;try our roving pointer
	_ifs	ESI b EDX, F1		;a good starting point
	mov	ESI, bassize
	jmp	short F1

	_align
F6:	mov	ESI,EDI
F1:	mov	DI,__PGOFF[EBX+ESI]	;the next in the list
	_ifs	ESI ae EDX, freeerr
	_ifs	EDI a EDX, F2		;got it
	_ifs	EDI a ESI, F6		;no wrap around (ESI < EDI < EDX)

; We have ESI < EDX < EDI (relative position in list)

F2:	movzx	ECX,word ptr [EBX+ESI]	;# of bytes in previous block
	add	ECX,ESI			;+ link
	_ifs	ECX ne EDX, F3		;if can't collapse with prev block
	add	[EBX+ESI],AX
	jmps	F4

F3:	mov	__PGOFF[EBX+ESI],DX	;link to EDX
	mov	ESI,EDX
	mov	__PGOFF[EBX+ESI],DI	;link to block after EDX

; See if we can collapse ESI with EDI
; ESI -> block just before EDI
; EDI -> block just after ESI

F4:	mov	allocp[EBX],SI		;for next time
	mov	AX,[EBX+ESI]
	add	EAX,ESI
	_ifs	EAX ne EDI, F5		;nope
	mov	AX,__PGOFF[EDI+EBX]	;link after EDI
	mov	__PGOFF[ESI+EBX],AX	;becomes link after ESI
	mov	AX,[EBX+EDI]		;# of bytes in EDI
	add	[EBX+ESI],AX		;add to # of bytes in ESI

F5:	clr	EAX			;success
F7:	mov	maxsize[EBX],0		;recalc max size
	unuse	<EDI,ESI,EBX>
F8:	ret

freeerr:
	mov	EAX,-1			;error
	jmp	F7
c_endp	page_free


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Determine size of largest free block in page.
;	unsigned page_maxfree(void far *baseptr);

	c_public page_maxfree
	_align
func	page_maxfree
	push	ESI
	mov	ESI,PS+4[ESP]		;DS:ESI = baseptr
	movzx	EAX,maxsize[ESI]
	tst	EAX
	jnz	short M3		;does not need recalculation
	movzx	EDX,baslnk[ESI]		;offset to first free block
M1:	movzx	ECX,word ptr [ESI+EDX]	;size of free block
	_ifs	ECX b EAX, M2
	mov	EAX,ECX
M2:	mov	CX,__PGOFF[ESI+EDX]	;offset to next link
	xchg	ECX,EDX
	_ifs	EDX a ECX, M1
	sub	EAX,__PGOFF
	jc	short M4
	mov	maxsize[ESI],AX
M3:	pop	ESI
	ret

M4:	clr	EAX
	pop	ESI
	ret
c_endp	page_maxfree


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Initialize memory allocation system in a page.
; Page size must be < 64k.
;	unsigned page_initialize(void *baseptr,unsigned pagesize);
; Returns:
;	size of largest allocatable block

	c_public page_initialize
	_align
func	page_initialize
	mov	EDX,PS[ESP]		;DS:EDX = baseptr
	movzx	EAX,word ptr PS+4[ESP]	;EAX = pagesize
	mov	pagesize[EDX],AX
	sub	EAX,pageoverhead
	mov	allocp[EDX],bassize
	mov	bassize[EDX],0
	mov	baslnk[EDX],pageoverhead

	;Construct one big free block of the remainder
	mov	pageoverhead[EDX],AX	;size of that block
	mov	word ptr pageoverhead+__PGOFF[EDX],bassize

	sub	EAX,__PGOFF		;overhead per allocated block
	mov	maxsize[EDX],AX

	ret
c_endp	page_initialize

	endcode	page
	end
