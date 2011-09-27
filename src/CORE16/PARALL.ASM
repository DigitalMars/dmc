;_ parall.asm	Mon Feb 26 1990   Modified by: Walter Bright
; Windows support added by G. Eric Engstrom Jan 1991
; OS2 support added by Nikki Locke May 1989
; Copyright (C) 1985-1991 by Walter Bright
; All rights reserved
; Written by Walter Bright
; $Revision: 1.1.1.1 $

include	macros.asm
include stdio.asm

if LCODE
extrn __SET_DOS_ERRNO:far
extrn __SET_ERRNO:far
else
extrn __SET_DOS_ERRNO:near
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

	begcode	parall

	c_public dos_alloc,dos_calloc,dos_free,dos_avail,_dos_blocksize

; Storage allocator


;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Allocate a block of data.
; Use:
;	p = dos_alloc(unsigned numpara);
; Returns:
;	segment of allocated data else 0
;
; WIN3: It may not be possible to get a full 64K in one segment from
;	Windows 3.0. For example, Windows has a 17 byte overhead for
;	GLOBALALLOC segments in standard mode. Hi order word of return
;	value is offset from segment where valid addresses begin.

func	dos_alloc
    ifdef _WINDOWS
	mov	BX,GMEM_MOVEABLE
    endif
_near_dos_alloc label near	; used as entry point by dos_calloc

	WINENTER
    ifdef _WINDOWS
	call	paraToByte
	push	BX		; wFlags = GMEM_MOVEABLE
	push	DX
	push	AX		; DX:AX number of bytes to allocate
	call	GLOBALALLOC	; make a new segment
	or	AX,AX
	jz	AE0
	  mov	  P[BP],AX
	  push	  AX
	  call	  GLOBALLOCK	; lock segment & convert handle to pointer
	  or	  DX,DX
	  jz	  AE1		; segment/selector can not be zero unless error has occurred
	    xchg    AX,DX
	    jmp     short A1	; swap segment/offset, for compatibility
AE1:	  push	  P[BP]
	  call	  GLOBALFREE	; free segment if lock failed
AE0:
	mov     AX,ENOMEM	; set errno to NO MEMORY if any failure occurs
	push    AX
	call    __SET_ERRNO
	clr	AX		; return 0 on failure
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
	test	AX,AX
	mov	BX,AX
	mov	AX,[BP-2]	;get segment address
	jz	A0
	mov	AX,BX
AE1:	errno_set
	clr	AX
A0:	mov	SP,BP
    else
	mov	BX,P[BP]	;get numpara
	bdos	48h		;allocate memory
	jnc	A1		;no error
	push    AX
        call    __SET_DOS_ERRNO
	clr	AX
    endif ;__OS2__
    endif ;_WINDOWS
A1:	WINLEAVE
	ret
c_endp	dos_alloc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Allocate a block of data and clear it.
; Use:
;	p = dos_calloc(numpar);
; Returns:
;	segment of allocated data else NULL
;
; WIN3: Multiple segment copies are valid under WIN 3, but it is
;	definately something to do carefully.
; WIN3: It may not be possible to get a full 64K in one segment from
;	Windows 3.0. For example, Windows has a 17 byte overhead for
;	GLOBALALLOC segments in standard mode. Hi order word of return
;	value is offset from segment where valid addresses begin.

func	dos_calloc
	WINENTER
	push	P[BP]
   if LCODE
	push	CS		; far return in dos_alloc expects this
   endif ;LCODE
   ifdef _WINDOWS
	mov	BX,GMEM_MOVEABLE + GMEM_ZEROINIT	; let windows 0 the memory for us
	call	_near_dos_alloc
	mov	SP,BP
   else ;_WINDOWS
	call	_near_dos_alloc
	mov	SP,BP
	tst	AX		;error?
	jz	CA2		;yes
	.save	DI
	mov	BX,AX		;save segment of result
CA4:	mov	CX,P[BP]	;# of paragraphs
	jcxz	CA1
	mov	ES,AX
	test	CH,0F0h
	jz	CA3		;jmp if less than 64k left
	mov	CX,1000h	;clear 64k
CA3:	sub	P[BP],CX
	shl	CX,1
	shl	CX,1
	shl	CX,1		;# of words
	clr	DI		;start at ES:0
	mov	AX,DI		;AX = 0
	rep	stosw		;clear the memory
	mov	AX,ES
	add	AX,offset __AHINCR	;next segment
	jmp	CA4

CA1:	.restore DI
	mov	AX,BX		;restore segment of result
    endif ;_WINDOWS
CA2:	WINLEAVE
	ret
c_endp	dos_calloc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Free memory that was allocated by dos_alloc() or dos_calloc().
; Use:
;	int dos_free(unsigned segment);
; Returns:
;	0	success
;	-1	error

func	dos_free
	WINENTER
    ifdef _WINDOWS
	push	P[BP]
	call	GLOBALHANDLE
	or	DX,DX
	jz	F1
	  push	  AX		; handle for GLOBALFREE
	  push	  AX		; handle for GLOBALUNLOCK
	  call	  GLOBALUNLOCK
	  call	  GLOBALFREE
	  or	  AX,AX
	  jz	  F2
F1:       mov     AX,ENOMEM
	  push    AX
          call    __SET_ERRNO
	  mov	    AX,-1
    else ;_WINDOWS
    ifdef __OS2__
	push	P[BP]			;segment
	call	DOSFREESEG
	test	AX,AX
	jz	F1			;no error
    else  ;__OS2__
	mov	ES,P[BP]		;get segment
	bdos	49h			;free allocated memory
	jnc	F1			;no error
    endif ;__OS2__
	push    AX
        call    __SET_DOS_ERRNO
        mov     AX,-1
        jmp     Fz
F1:	mov     AX,0
Fz:
    endif ;_WINDOWS
F2:	WINLEAVE
	ret
c_endp	dos_free

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Modify allocated memory.
;	unsigned dos_setblock(unsigned newsize,unsigned segment);
; Returns:
;       if successful, the segment address of the reallocated memory block
;	if errno is set, it returns the max possible size
; WIN3: does not return the max possible size, because GLOBALCOMPACT is
;	not a fast function, use dos_avail if max memory available is
;	required.
; WIN3: It may not be possible to get a full 64K in one segment from
;	Windows 3.0. For example, Windows has a 17 byte overhead for
;	GLOBALALLOC segments in standard mode. Hi order word of return
;	value is offset from segment where valid addresses begin.

	c_public dos_setblock
func	dos_setblock
    ifdef _WINDOWS
	WINENTER
	push	P+2[BP]		;segment/selector
	call	GLOBALHANDLE
	or	DX,DX
	jz	S3		;(DX == 0) ? error condition : success
	push	AX		; AX == handle for selector/segment, used by GLOBALLOCK in case of error
	push	AX		; AX == handle for selector/segment, used by GLOBALREALLOC
	push	AX		; AX == handle for selector/segment, used by GLOBALUNLOCK
	call	GLOBALUNLOCK
	call	paraToByte
	push	DX
	push	AX		; DX:AX new size of segment
	mov	AX,GMEM_MOVEABLE 
	push	AX		; FLAGS == GMEM_MOVEABLE
	call	GLOBALREALLOC
	or	AX,AX
	jz	S1
	add     SP,2	; get rid of handle pushed in case of error
	push    AX		; push handle for selector/segment of resized block
	call	GLOBALLOCK  ; lock new resized segment in memory and return a pointer
	xchg	DX,AX
	jmp	S2
S1:	call	GLOBALLOCK	; re-lock the original block	
S3:	mov     AX,ENOMEM
        push    AX
        call    __SET_ERRNO
	mov	ax,0
S2:	WINLEAVE
	ret
    else ;_WINDOWS
	push	BP
	mov	BP,SP
    ifdef __OS2__
	push	P[BP]		;newsize
	push	P+2[BP]		;segment to resize
	call	DOSREALLOCSEG
	test	AX,AX
	jnz	SE0
	mov	AX,P+2[BP]	;get segment address
	jmps	S0

SE0:	errno_set ENOMEM
	lea	AX,P[BP]
	push	SS
	push	AX		;address of 4 bytes of parameter
	call	DOSMEMAVAIL
	test	AX,AX
	mov	AX,P[BP]
	mov	DX,P+2[BP]	;load no of bytes left
	jz	S2		;No error
	clr	AX		;signal error
	jmps	S0
S2:	mov	CX,4

	even
S3:	shr	DX,1
	rcr	AX,1
	loop	S3
	or	DX,DX
	jz	S0
	mov	AX,0ffffh	;so set to 0xffff
S0:
	pop	BP
	ret
    else ;__OS2__
	les	BX,P[BP]
	bdos	4Ah		;modify allocated memory
	jc	S1		;no error, return size of allocated block
	mov	AX,P+2[BP]	;get segment address for success return
	jmps	S0	
S1:	mov     AX,ENOMEM
        push    BX
        push    AX
        call    __SET_ERRNO
        pop     AX              ;return max possible
S0:   	pop	BP
	ret
    endif ;__OS2__
    endif ;_WINDOWS
c_endp	dos_setblock

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copy paragraphs of memory.
;	void _copy(unsigned newseg,unsigned oldseg,unsigned numpar);

; WIN3 NOTE: Multiple segment copies are valid under WIN 3, but it is
;	     definately something to do carefully.

	c_public _copy
func	_copy
	WINENTER
	.save	<SI,DI>
	push	DS
	mov	ES,P[BP]	;newseg
	mov	DS,P+2[BP]	;oldseg
	mov	BX,P+4[BP]	;numpar
COP4:	mov	CX,BX		;# of paragraphs
	jcxz	COP1
	and	CX,0FFFh
	jnz	COP3
	mov	CX,1000h	;clear 64k
COP3:	sub	BX,CX
	shl	CX,1
	shl	CX,1
	shl	CX,1		;# of words
	clr	DI		;start at ES:0
	clr	SI		;start at DS:0
	rep	movsw

	;Modify segment registers to next segment
	mov	AX,ES
	add	AX,offset __AHINCR	;next segment
	mov	ES,AX
	mov	AX,DS
	add	AX,offset __AHINCR	;next segment
	mov	DS,AX

	jmp	COP4

COP1:	pop	DS
	.restore <DI,SI>
	WINLEAVE
	ret
c_endp	_copy

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; How many bytes of memory available
;	unsigned long dos_avail()

func	dos_avail
	WINENTER
    ifdef _WINDOWS
	clr	AX
	push	AX
	push	AX
	call	GLOBALCOMPACT	; when called with 0 returns max contiguous space
    else ;_WINDOWS
    ifdef __OS2__
	sub	SP,4		;allow space for long return value
	lea	AX,[BP-4]
	push	SS
	push	AX		;address of return value
	call	DOSMEMAVAIL
	test	AX,AX
	mov	BX,AX		;save possible errno
	mov	AX,P[BP]
	mov	DX,P+2[BP]	;load no of bytes left
	jz	AV0
	errno_set BX
	clr	AX
	cwd
AV0:	mov	SP,BP
    else ;__OS2__
	mov	BX,0ffffh	;try to allocate all the memory
	bdos	48h		;allocate memory
	jc	AV1		;no error
	mov	ES,AX
	bdos	49h		;free the allocated memory
    if SPTR
	push	DS
	pop	ES
    endif ;SPTR
	mov	BX,0ffffh	;that was how much we got
	jmp short AV2
AV1:	cmp	AX,8		;not enough memory ?
	jz	AV2
	push    AX
        call    __SET_DOS_ERRNO
	clr	BX
AV2:	mov	AX,BX		;return max possible
	clr	DX
	mov	CX,4		;shift dx:ax left 4 times

	even
AV3:	shl	AX,1		;to convert from paras to bytes
	rcl	DX,1
	loop	AV3
    endif ;__OS2__
    endif ;_WINDOWS
	WINLEAVE
	ret
c_endp	dos_avail

func	_dos_blocksize
	WINENTER
	.save <SI,DI>
    ifdef _WINDOWS
	push	P[BP]
	call	GLOBALHANDLE
	or	DX,DX
	jz	B1
	push	AX
	call	GLOBALSIZE
	mov	CX,4
	even
B2:	shr	  DX,1
	rcr	  AX,1
	loop	  B2		; convert from bytes to paragraphs
	jmp	  B3
B1:	xor	AX,AX	
B3:	
    else
    ifdef __OS2__
        mov	ax,0		;***STILL NEEDS TO BE IMPLEMENTED FOR OS2***
    else
        mov	ax,P[BP]
        dec	ax
        mov	es,ax		;es addresses arena header
        mov	ax,es:3		;size of memory block
    endif
    endif
        .restore <DI,SI>
	WINLEAVE AX
	ret
c_endp	_dos_blocksize

    ifdef _WINDOWS
; presumes P[BP] is paragraph count word
; which will be converted to DX:AX byte count long
paraToByte proc near
	clr	DX
	mov	AX,P[BP]
	mov	CX,4
	even			; speed up loop
A0:	  shl	  AX,1
	  rcl	  DX,1
	  loop	  A0		; convert from paragraphs to bytes
	ret
paraToByte endp
    endif

	endcode	parall

	end
