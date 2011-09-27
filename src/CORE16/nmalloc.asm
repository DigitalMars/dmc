;_ nmalloc.asm   Tue Aug 10, 1993
; Windows support added by G. Eric Engstrom Dec 1990,	-D_WINDOWS
; OS2 support added by Nikki Locke May 1989,		-D__OS2__
; Copyright (C) 1985-1991 by Walter Bright
; All rights reserved
; Written by Walter Bright
; $Revision: 1.1.1.1 $

include	macros.asm

    if LCODE
	extrn	___faterr:far
    else
        extrn   ___faterr:near
    endif
        	
ifdef _WINDOWS
	LMEM_FIXED      =   0000h
	LMEM_MOVEABLE   =   0002h
	LMEM_NOCOMPACT  =   0010H
	LMEM_NODISCARD  =   0020H
	LMEM_ZEROINIT   =   0040h
	LMEM_MODIFY     =   0080H
	LMEM_DISCARDABLE=   0F00h
	extrn LOCKSEGMENT:far
	extrn LOCALALLOC:far
	extrn UNLOCKSEGMENT:far
	extrn LOCALFREE:far
else
  if SPTR
        c_extrn _pastdata,word,_heapbottom,word
    if LCODE
	c_extrn	sbrk,far
    else
	c_extrn	sbrk,near
    endif

    ifdef _MT
    if LCODE
	extrn	__semerr:far
    else
	extrn	__semerr:near
    endif
    endif
  endif  
endif 
  
	begcode	nmalloc
        c_public _nmalloc
        c_public _nfree

; Storage allocator

	begdata
heapmsg         db      'Heap is corrupted'
ifndef _WINDOWS
  if SPTR
	extrn __baslnk:word
__allocp	dw	offset DGROUP:__baslnk	;roving pointer for allocator
_allocp	equ	__allocp
_baslnk equ	__baslnk
  endif	
endif	
	enddata

; A block in the free list consists of:
;	dw	pointer to next block in list
;	dw	size of block in bytes (must be even) (including both words)

; When it's allocated,
;	dw	# of bytes in this block including this word
;	db...	the bytes allocated

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Handle locking/unlocking for multithreaded programs

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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Something went wrong with the semaphores.
; Print an error message and abort the program.

    ifdef _MT
    if LCODE
semerr	proc	near
	jmp	__semerr
semerr	endp
    endif
    endif

	even
    ifdef _WINDOWS
;-------------------------------------------------
; Windows _nmalloc (both near and far data models)
;-------------------------------------------------
func	_nmalloc
	push	BP
	mov	BP,SP
;	
	mov	AX,0FFFFh		; lock local segment
	push	AX
	call	far ptr LOCKSEGMENT
;	
	mov	AX,P[BP]		; # bytes to allocate
	or	AX,AX			; if zero, fake LocalAlloc into returning a valid PTR, for MS Compat.
;;	jz	A7
        jnz  A9
        mov AX,1            ; For Microsoft Compatability, return valid PTR on Zero Bytes
A9:
	mov	CX,LMEM_NODISCARD+LMEM_FIXED	; allocate fixed memory
	push	CX
	push	AX
	call	far ptr LOCALALLOC
;	
A7:	push	AX			; save return value
	mov	AX,0FFFFh		
	push	AX
	call	far ptr UNLOCKSEGMENT
	pop	AX
;
	pop	BP
	ret
c_endp	_nmalloc	
    else
      if LPTR
;----------------------------------------------------------------
; DOS/OS2/DOS16RM _nmalloc for far data models is not implemented
;----------------------------------------------------------------
func	_nmalloc
	mov	AX,0
	ret
c_endp	_nmalloc      
      else
;--------------------------------------
; DOS/OS2 _nmalloc for near data models
;--------------------------------------
func	_nmalloc
	talloc_lock
	push	BP
A4:	mov	BP,SP
	.save	<SI,DI>
	mov	AX,P[BP]	;get nbytes
	add	AX,3		;need another word for length info
	and	AL,0FEh		;round up to nearest word
	_if	AX ae 4, A8	;if number of bytes is 4 or greater, start allocation
        mov     AX,4            ;Allow zero byte allocation for MS Compatability
A8:
	mov	BP,2		;save some bytes
;	mov	SI,_allocp	;last item
	mov	SI,_baslnk	;last item
	mov	CX,SI		;CX to save bytes
	jmps	A2

allocerr:
	clr	AX		;NULL
	jmp	A6

	even
A1:	mov	SI,DI
	_if	DI e CX, trysbrk	;wrapped around, didn't find any
A2:	mov	DI,[SI]			;next item in list
	_if	AX a [DI+BP], A1	;not big enough

	je	A3		;exactly big enough
	add	AX,BP		;we'll need another 2 bytes
	_if	AX e [DI+BP],A3	;have to allocate an entire block
	sub	AX,BP

;Allocate from bottom of free block. Desirable in order to delay
;stack overflow as long as possible.
; DI -> free block
; SI -> previous free block
; AX =	# of bytes in allocated block
	add	[SI],AX		;link to new free block
	mov	CX,[DI+BP]	;number of bytes in block we're splitting
	mov	SI,[SI]		;pointer to new free block
	sub	CX,AX		;CX = remaining bytes
	mov	[SI+BP],CX	;# of bytes in this block

A3:	xchg	AX,[DI]		;[DI] = # of bytes, AX = next free block
	mov	[SI],AX		;skip the DI entry in list
	mov	_allocp,SI
	lea	AX,[DI+BP]	;pointer to area allocated (DI + 2)
A6:	talloc_unlock AX
	.restore <DI,SI>
	WINLEAVE
	ret

trysbrk:			;try sbrk() to grow our data segment
    ifdef __OS2__
	_if	AX ae 1024, A5
	mov	AX,1024		;1024 byte chunk minimum size
    else ;__OS2__
	_if	AX ae 256, A5
	mov	AX,256		; 256 byte chunk minimum size
    endif ;__OS2__
A5:
	push	AX
	callm	sbrk
	pop	BX
	inc	AX		;point past # of bytes allocated
	jz	A6		;failed, sbrk returned -1, so return NULL
	inc	AX
	push	AX
	callm	free_nlock	;add allocated memory into free list
	talloc_lock
	pop	BX
	.restore <DI,SI>
	jmp	A4		;try again
c_endp	_nmalloc
      endif
    endif

; The heap has been corrupted.
; Print an error message and abort the program.

func	heaperr
	mov	AX,offset DGROUP:heapmsg
    if LPTR
        push 	DS
    endif        	
	push	AX
	jmp	___faterr
c_endp	heaperr	

    ifdef _WINDOWS
;-----------------------------------------------
; Windows _nfree (both near and far data models)
;-----------------------------------------------
func	_nfree
	push	BP
	mov	BP,SP
	mov	AX,P[BP]
	or	AX,AX
	jz	F7
	push	AX
	call	far ptr LOCALFREE
	or	AX,AX
	jnz	_heaperr
F7:	pop	BP
	ret
c_endp	_nfree    
    else
      if LPTR
;--------------------------------------------------------------
; DOS/OS2/DOS16RM _nmalloc for near data models not implemented
;--------------------------------------------------------------      
func	_nfree
	ret
c_endp	_nfree    
      else
;------------------------------------
; DOS/OS2 _nfree for near data models
;------------------------------------
func	_nfree
	talloc_lock

_free_nlock:
	WINENTER
	.save	<SI,DI>
	mov	BX,P[BP]		;get p
	tst	BX			;pass a NULL pointer?
	jz	F5			;yes, return 0
	mov	BP,2			;to save some bytes
					;check if below bottom of pool
	_if	BX be _heapbottom, _heaperr	;if below bottom of pool
	_if	BX ae _pastdata, _heaperr	;if above top of pool
	test	BL,1			;odd?
	jne	_heaperr
	sub	BX,BP			;point to start of block
	mov	AX,[BX]			;# of bytes in block to be freed

; Try to find SI and DI such that SI < BX < DI

	mov	SI,_allocp		;try our roving pointer
	_if	SI b BX, F1		;a good starting point
	mov	SI, offset DGROUP:_baslnk
	jmps	F1

	even
F6:	mov	SI,DI
F1:	mov	DI,[SI]			;the next in the list
	_if	SI ae BX, _heaperr
	_if	DI a BX, F2		;got it
	_if	DI a SI, F6		;no wrap around (SI < DI < BX)

; We have SI < BX < DI (relative position in list)

F2:	mov	CX,[SI+BP]		;# of bytes in previous block
	add	CX,SI			;+ link
	_if	CX ne BX, F3		;if can't collapse with prev block
	add	[SI+BP],AX
	jmps	F4

F3:	mov	2[BX],AX		;store # of bytes in freed block
	mov	[BX],DI			;link to block after BX
	mov	[SI],BX			;link to BX
	mov	SI,BX

; See if we can collapse SI with DI
; SI -> block just before DI
; DI -> block just after SI
; BP = 2

F4:	mov	_allocp,SI		;for next time
	mov	AX,[SI+BP]
	add	AX,SI
	_if	AX ne DI, F5		;nope
	mov	AX,[DI]			;link after DI
	mov	[SI],AX			;becomes link after SI
	mov	AX,[DI+BP]		;# of bytes in DI
	add	[SI+BP],AX		;add to # of bytes in SI

F5:	talloc_unlock
	clr	AX			;success
	.restore <DI,SI>
	WINLEAVE
	ret
c_endp	_nfree

      endif
    endif
	endcode	nmalloc
	end

