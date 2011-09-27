;_ memavl.asm Wed Aug 11 1993
; Copyright (C) 1993 by Digital Mars
; All rights reserved
; $Revision: 1.1.1.1 $

include	macros.asm

    ifdef _WINDOWS
        extrn	LOCALCOMPACT:far
    endif

    ifdef _MT
    if LCODE
	extrn	__semerr:far
    else
	extrn	__semerr:near
    endif
    endif
    
    if LCODE
        extrn _coreleft:far
    else
        extrn _coreleft:near
    endif

	begcode	memavl
  ifndef _WINDOWS
    if SPTR
	extrn	__baslnk:word
    endif
  endif  	
  
  	public	__memavl
	
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; _memavl()

  ifdef _WINDOWS
func	_memavl
	xor	AX,AX
	push	AX
	call	LOCALCOMPACT
	ret
c_endp	_memavl       
  else
    if LPTR 
func	_memavl
	mov 	AX,0		; not yet implemented
	ret
c_endp	_memavl       
    else
func	_memavl
	WINENTER
        .save   <SI,DI>
	talloc_lock
	callm	coreleft        ;AX gets size of unallocated memory
	mov	si,__baslnk	;last item
	mov	cx,si		;CX to save bytes
	mov	bp,2
	jmp	V1
V3:	mov	si,di
	_if	SI e CX, V2	;wrapped around, didn't find any
V1:	mov	di,[si]
	mov	bx,[di+bp]
	_if	bx be bp,V3
	sub	bx,bp
	add	ax,bx
   	jmp	V3	
V2:     talloc_unlock AX
   	.restore <DI,SI>
	WINLEAVE
	ret
c_endp	_memavl
    endif
  endif  
	endcode	memavl
	end

