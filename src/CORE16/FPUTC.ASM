;_ FPUTC.ASM   Mon Dec 18 1989   Modified by: Walter Bright */
; Copyright (C) 1985-1989 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

	include	macros.asm
	include	stdio.asm

    if LCODE
	c_extrn	_flushbu,far
	c_extrn	_fillbuf,far
	c_extrn	fflush,far
    else
	c_extrn	_flushbu,near
	c_extrn	_fillbuf,near
	c_extrn	fflush,near
    endif

    ifdef _MT
    if LCODE
	extrn	___fp_lock:far,___fp_unlock:far
    else
	extrn	___fp_lock:near,___fp_unlock:near
    endif
    endif

	begcode	fputc

	c_public	fputc

;;;;;;;;;;;;;;;;;;;;;;;;;
;	int fputc(c,fp);

func	fputc

    ifdef _MT
	;The idea with the multi-threaded version is to lock the
	;stream, call the locked version of fputc, and then
	;unlock the stream.

	WINENTER
	sub	SP,2
    if LPTR
	push	P+4[BP]
    endif
	push	P+2[BP]
	call	___fp_lock
	push	P+0[BP]
    if LCODE
	push	CS
    endif
	call	near ptr __fputc_nlock
	add	SP,2			;clean off c, but not fp
	mov	-2[BP],AX		;save return value
	call	___fp_unlock
	mov	AX,-2[BP]
	mov	SP,BP
	WINLEAVE
	ret
    endif

	public	__fputc_nlock
__fputc_nlock:

	WINENTER
	mov	AL,P[BP]	;get c
    if SPTR
	mov	BX,P+2[BP]	;get fp
        cmp     BX,0
        je      L7
    else
	mov	CX,DS		;save original DS
	lds	BX,P+2[BP]	;get fp
        mov     DX,DS
        or      DX,BX
        je      L8
    endif
	_if	AL e 10, L4	;if LF

; Input:
;	AL = c
;	DS:BX = fp
;	CX = original value of DS
; Output:
;	AX = c & 0xFF
;	DS is restored

L1:	WINLEAVE

_putit:
	dec	word ptr _cnt[BX]
	js	L2
L5:	mov	DX,DI
    if SPTR
	mov	DI,_ptr[BX]
     ifdef BIGBUF		;can't have LPTR with BIGBUF
	mov	ES,_seg[BX]	;segment of buffer
	stosb
     else
	mov	[DI],AL
	inc	DI
     endif
    else ;LPTR
	les	DI,_ptr[BX]
	stosb
    endif
	mov	_ptr[BX],DI
    if LPTR
	mov	DS,CX		;restore original DS
    endif
	mov	DI,DX		;restore DI
	clr	AH		;can't let c sign-extend to produce wrong EOF
	ret

L4:	test	byte ptr _flag+1[BX],_IOTRAN/256
	jz	L3		;if not translated mode
	mov	AL,13		;CR first
	callm	putit
	mov	AL,P[BP]
    if SPTR
	mov	BX,P+2[BP]
    else
	mov	CX,DS		;save original DS
	lds	BX,P+2[BP]
    endif

L3:	test	byte ptr _flag[BX],_IOLBF	;line buffered?
	jz	L1				;no
	callm	putit
	push	AX
    if LPTR
	push	P+4[BP]		;segment of fp
    endif
	push	P+2[BP]		;offset of fp
	callm	fflush
	add	SP,SIZEPTR
	tst	AX		;did fflush succeed?
	pop	AX		;AX = c & 0xFF
	jz	L6		;it succeeded
L7:
	mov	AX,-1		;fflush failed, return EOF
L6:	WINLEAVE
	ret
L8:
       mov    DS,CX
       jmp    L7
L2:
	_if	<word ptr _cnt[BX]> ne 0FFFFh, L5
        WINENTER_NF
    if LPTR
	push	DS		;segment of fp
	mov	DS,CX		;restore original DS
    endif
	push	BX		;push fp
	push	AX		;push c
	callm	_flushbu
	add	SP,2+SIZEPTR
        WINLEAVE_NF
	ret
c_endp	fputc

;;;;;;;;;;;;;;;;;;;;;;;;;;
;	int fgetc(fp);

	c_public fgetc

	even
func	fgetc

    ifdef _MT
	;The idea with the multi-threaded version is to lock the
	;stream, call the locked version of fgetc, and then
	;unlock the stream.

	WINENTER
	sub	SP,2
    if LPTR
	push	P+2[BP]
    endif
	push	P+0[BP]
	call	___fp_lock
    if LCODE
	push	CS
    endif
	call	near ptr __fgetc_nlock
	mov	-2[BP],AX		;save return value
	call	___fp_unlock
	mov	AX,-2[BP]
	mov	SP,BP
	WINLEAVE
	ret
    endif

	public	__fgetc_nlock
__fgetc_nlock:
 
	WINENTER
G4:
    if SPTR
	mov	BX,P[BP]
        cmp     BX,0
        je      G7
    else
	mov	CX,DS
	lds	BX,P[BP]
        mov     DX,DS
        or      DX,BX
        je      G7
    endif
G1:	dec	word ptr _cnt[BX]
	js	G2			;if out of chars in buffer

G6:	mov	DX,SI
    if SPTR
	mov	SI,_ptr[BX]
      ifdef BIGBUF			;can't have both LPTR and BIGBUF
	mov	ES,_seg[BX]		;load segment of disk buffer
	mov	AL,ES:[SI]
	inc	SI
      else
	lodsb				;get char from buffer
      endif
    else
	les	SI,_ptr[BX]
	mov	AL,ES:[SI]
	inc	SI
    endif
	clr	AH
	mov	_ptr[BX],SI			;updated pointer
	mov	SI,DX
	test	byte ptr _flag+1[BX],_IOTRAN/256	;translated mode?
	je	G3				;no
	_if	AL e 13, G1			;ignore CRs
	_if	AL e 26, G5			;^Z marks end of file
G3:
    if LPTR
	mov	DS,CX
    endif
	WINLEAVE
	ret

G2:
	_if	<word ptr _cnt[BX]> ne 0FFFFh, G6
    if LPTR
	push	DS
	mov	DS,CX
    endif
	push	BX
	callm	_fillbuf		;fill buffer (_fillbuf(fp))
	mov	SP,BP
	tst	AX			;EOF?
	jz	G4			;no
	WINLEAVE
	ret

G7:
        mov     AX,EOF
        jmp     G8

G5:	mov	AX,EOF
	or	byte ptr _flag[BX],_IOEOF	;set EOF flag bit
	mov	word ptr _cnt[BX],0		;0 chars left in buffer
G8:
    if LPTR
	mov	DS,CX
    endif
	WINLEAVE
	ret
c_endp	fgetc



	endcode	fputc
	end
