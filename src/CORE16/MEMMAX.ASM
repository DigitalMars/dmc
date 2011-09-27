;_ memmax.asm Wed Aug 11 1993
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

	begcode	memmax
  ifndef _WINDOWS
    if SPTR
	extrn	__baslnk:word
    endif
  endif  	
  
  	public	__memmax
	
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
; _memmax()

  ifdef _WINDOWS
func	_memmax
	xor	AX,AX
	push	AX
	call	LOCALCOMPACT
	ret
c_endp	_memmax       
  else
    if LPTR 
func	_memmax
	mov 	AX,0		; not yet implemented
	ret
c_endp	_memmax       
    else
func    _memmax
	WINENTER
	.save	<SI,DI>
	talloc_lock
	callm	coreleft        ;AX has size of unallocated memory
	mov	si,__baslnk	;last item
	mov	cx,si		;CX to save bytes
	mov	bp,2
	jmp	X1
X3:	mov	si,di
	_if	SI e CX, X2	;wrapped around, didn't find any
X1:	mov	di,[si]
	mov	bx,[di+bp]
	_if	ax ae bx, X3
	mov	ax,bx
   	jmp	X3	
X2:	sub	ax,bp
	jnc	short X4
	mov	ax,0
X4:     talloc_unlock AX
	.restore <DI,SI>
	WINLEAVE
	ret
c_endp  _memmax
    endif
  endif  
	endcode	memmax
	end

