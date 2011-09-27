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
        c_extrn _nmalloc,far
    else
    	c_extrn _nmalloc,near
    endif
    
    ifdef _WINDOWS
LMEM_FIXED      =   0000h
LMEM_MOVEABLE   =   0002h
LMEM_NOCOMPACT  =   0010H
LMEM_NODISCARD  =   0020H
LMEM_ZEROINIT   =   0040h
LMEM_MODIFY     =   0080H
LMEM_DISCARDABLE=   0F00h
    	extrn	LOCKSEGMENT:far
    	extrn	LOCALALLOC:far
    	extrn	UNLOCKSEGMENT:far
    endif

	begcode	ncalloc
	c_public _ncalloc

	even
  ifdef _WINDOWS
func	_ncalloc
	WINENTER
	mov	AX,P+2[BP]
	mul	word ptr P[BP]
;	
	or	AX,AX		; if size to allocate == 0, return NULL
	je	C1
;	
	push	AX		; save size to allocate
	mov	CX,0FFFFh	; lock data segment
	push	CX
	call	LOCKSEGMENT
	pop	AX		; recover size to allocate
;	
	mov	CX,LMEM_NODISCARD+LMEM_ZEROINIT+LMEM_FIXED
	push	CX
	push	AX
	call	LOCALALLOC
;	
	push	AX		; save handle == near pointer
	mov	AX,0FFFFh	; unlock data segment
	push	AX
	call	UNLOCKSEGMENT
	pop	AX		; recover handle == near pointer
;
C1:	WINLEAVE
	ret
c_endp	_ncalloc  
  else
    if LPTR
func	_ncalloc
	mov	AX,0	; not implemented
	ret
c_endp	_ncalloc
    else
func	_ncalloc
	WINENTER
	mov	BX,P+2[BP]	;get sizeof(elem)
	mov	AX,P[BP]	;get numelems
	_if	BX e 1, C1	;no need to multiply
	mul	BX
	jc	C3		;if overflow
C1:	push	AX		;nbytes
	callm	_nmalloc
	mov	SP,BP
	tst	AX
	jz	C2		;yes
	.save	<DI>
      ife ESeqDS
	mov	DX,DS
	mov	ES,DX
      endif
	mov	DI,AX
	mov	DX,AX		;save pointer to result
	mov	CX,-2[DI]	;# of bytes
	shr	CX,1		;# of words (including byte count)
	dec	CX		;skip # of bytes
	clr	AX
	rep	stosw		;clear the memory
	mov	AX,DX		;restore pointer to result
	.restore <DI>
C2:	WINLEAVE
	ret

C3:	clr	AX
	WINLEAVE
	ret
c_endp	_ncalloc
    endif
  endif
	endcode	ncalloc
	end
