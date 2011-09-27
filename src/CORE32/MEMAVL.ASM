;_ memavl.asm
; Copyright (C) 1985-1992 by Digital Mars
; All rights reserved
; $Revision: 1.1.1.1 $

include macros.asm

	extrn _coreleft:near
	extrn __baslnk:dword
	
    ifdef _MT
	extrn	__semerr:near
	extrn	___alloc_sem:dword
    endif

	public __memavl,__nmemavl

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

	begcode memavl

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Find total available memory in heap
; Use:
;       size_t __cdecl _memavl(void);
; Returns:
;       unsigned (32 bit) total memory available for allocation
        _align
__nmemavl:        
__memavl proc	near
	push	ebp
	mov	ebp,esp
	uses	<EBX,ECX,ESI,EDI>
	talloc_lock
	callm	coreleft        ;EAX gets size of unallocated memory
	mov	esi,__baslnk	;last item
	mov	ecx,esi		;ECX to save bytes
	mov	ebp,4
	jmps	V1
V3:	mov	esi,edi
	_ifs	ESI e ECX, V2	;wrapped around, didn't find any
V1:	mov	edi,[esi]
	mov	ebx,[edi+ebp]
	_ifs	ebx be ebp,V3
	sub	ebx,ebp
	add	eax,ebx
   	jmps	V3	
V2:     talloc_unlock EAX
	unuse	<EDI,ESI,ECX,EBX>
	pop	ebp
	_ret
__memavl endp

	endcode memavl
	end
