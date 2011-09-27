;_ allocmem.asm	Fri Jul 23 1993 Copyright (C) 1985-1991 by Digital Mars
; All rights reserved
; $Revision: 1.1.1.1 $

include	macros.asm
include stdio.asm

if LCODE
extrn __SET_ERRNO:far
else
extrn __SET_ERRNO:near
endif

;	extrn	__AHSHIFT:near
	extrn	__AHINCR:near

    ifdef __OS2__
extrn	DOSALLOCSEG:far
extrn	DOSREALLOCSEG:far
extrn	DOSFREESEG:far
extrn	DOSMEMAVAIL:far
    endif ;__OS2__

    ifdef _WINDOWS
extrn	GLOBALHANDLE	:far
extrn	GLOBALLOCK	:far
extrn	GLOBALUNLOCK	:far
extrn	GLOBALALLOC	:far
extrn	GLOBALFREE	:far
extrn	GLOBALREALLOC	:far
extrn	GLOBALCOMPACT	:far
extrn   GLOBALSIZE      :far
	SEG_DISTANCE	equ	4h	;distance between one selector and the next

;	/* Global Memory Flags */
GMEM_FIXED          equ 0000h
GMEM_MOVEABLE       equ 0002h
GMEM_NOCOMPACT      equ 0010h
GMEM_NODISCARD      equ 0020h
GMEM_ZEROINIT       equ 0040h
GMEM_MODIFY         equ 0080h
GMEM_DISCARDABLE    equ 0100h
GMEM_NOT_BANKED     equ 1000h
GMEM_SHARE          equ 2000h
GMEM_DDESHARE       equ 2000h
GMEM_NOTIFY         equ 4000h
    endif ;_WINDOWS

	begcode	allocmem

	c_public _dos_allocmem,_dos_freemem

; Storage allocator


;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Allocate a block of data.
; Use:
;	p = _dos_allocmem (unsigned size, unsigned *segp);
; Returns:
;       0 if successful; DOS error code otherwise
;
; WIN3: It may not be possible to get a full 64K in one segment from
;	Windows 3.0. For example, Windows has a 17 byte overhead for
;	GLOBALALLOC segments in standard mode. Hi order word of return
;	value is offset from segment where valid addresses begin.

func	_dos_allocmem
    ifdef _WINDOWS
;	xor	BX,BX
	mov	BX,GMEM_MOVEABLE
    endif
	WINENTER
    ifdef _WINDOWS
	call	paraToByte
	push	BX		; wFlags
	push	DX
	push	AX		; DX:AX number of bytes to allocate
	call	GLOBALALLOC	; make a new segment
	or	AX,AX
	jz	AE0
	mov	P[BP],AX
	push	AX
	call	GLOBALLOCK	; lock segment & convert handle to pointer
	or	DX,DX
	jz	AE1		; segment/selector can not be zero unless error has occurred
	xchg    AX,DX
	call 	setSegp
	mov	ax,0		; to indicate success
	jmp     short A1	; swap segment/offset, for compatibility
AE1:	push	P[BP]
	call	GLOBALFREE	; free segment if lock failed
AE0:	
	clr	AX
	push	AX
	push	AX
	call	GLOBALCOMPACT
	call	bytetoPara
	or	DX,DX
	jz	AE2
	mov	AX,0FFFFh
AE2:    call	setSegp
	mov	AX,ENOMEM
	push    AX
	call    __SET_ERRNO
	mov     AX,-1
    else ;_WINDOWS
    ifdef __OS2__
	sub	SP,2
	mov	AX,ENOMEM	;ENOMEM error
	mov	BX,P[BP]
	test	BX,0F000h
	jnz	AE1		;asking for too much memory
	mov	CL,4
	shl	BX,CL		;convert paras to bytes
	push	BX
	push	SS
	lea	AX,[BP-2]
	push	AX
	pushi	AX,0		;zero share flags
	call	DOSALLOCSEG
	mov	BX,AX
	mov	AX,[BP-2]	;get segment address
	call	setSegp
	mov	AX,0
	or	bx,bx
	jz	A0
AE1:	mov	AX,0		;this *should* be the maximum paragraphs
	call	setSegp		;  available
	mov	AX,ENOMEM
	push    AX
	call    __SET_ERRNO
	mov     AX,-1
A0:	mov	SP,BP
    else
	mov	BX,P[BP]	;get numpara
	bdos	48h		;allocate memory
	jc	A3		;no error
	call	setSegp
	mov	ax,0
	jmp	short A1
A3:	mov	ax,bx
	call	setSegp
	mov	AX,ENOMEM
	push    AX
	call    __SET_ERRNO
	mov     AX,-1
    endif ;__OS2__
    endif ;_WINDOWS
A1:	WINLEAVE
	ret
c_endp	_dos_allocmem

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Free memory that was allocated by _dos_allocmem().
; Use:
;	unsigned _dos_freemem(unsigned seg);
; Returns:
;	0	if successful, DOS error code otherwise

func	_dos_freemem
	WINENTER
    ifdef _WINDOWS
	push	P[BP]
	call	GLOBALHANDLE
	or	DX,DX
	jz	F1A
	push	AX		; handle for GLOBALFREE
push	AX		; handle for GLOBALUNLOCK
	call	GLOBALUNLOCK
	call	GLOBALFREE
	or	AX,AX
	jz	F2
F1A:	mov	AX,09h	; DOS error code for 'memory blk addr invalid'	
    else ;_WINDOWS
    ifdef __OS2__
	push	P[BP]			;segment
	call	DOSFREESEG
	test	AX,AX
	jz	F2			;no error
	mov	AX,09h  ; DOS error code for 'memory blk addr invalid'
    else  ;__OS2__
	mov	ES,P[BP]		;get segment
	bdos	49h			;free allocated memory
	jc	F1			;no error
	mov	AX,0
	jmp	short F2
    endif ;__OS2__
    endif ;_WINDOWS
F1:	mov     AX,ENOMEM
	push    AX
	call    __SET_ERRNO
	mov     AX,-1
F2:	WINLEAVE
	ret
c_endp	_dos_freemem


; presumes P+2[BP] is pointer to unsigned where
; segment value or max avail paragraphs is to be stored
setSegp proc near
    if	SPTR
    	mov	BX,P+2[BP]
    	mov	[BX],AX
    else
    	les	BX,P+2[BP]
    	mov	ES:[BX],AX
    endif
        ret
setSegp endp        
    
    ifdef _WINDOWS
; presumes P[BP] is paragraph count word
; which will be converted to DX:AX byte count long
paraToByte proc near
	clr	DX
	mov	AX,P[BP]
	mov	CX,4
	even			; speed up loop
P0:	shl	AX,1
	rcl	DX,1
	loop	P0		; convert from paragraphs to bytes
	ret
paraToByte endp
bytetoPara proc near
	mov	CX,4
	even			; speed up loop
P1:	shr	DX,1
	rcr	AX,1
	loop	P1		; convert from bytes to paragraphs
	ret
bytetoPara endp
    endif
    
	endcode	allocmem

	end
