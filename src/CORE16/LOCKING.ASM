;_ locking.asm	 Fri Dec 15 1989   Modified by: Walter Bright */
; Copyright (C) 1989-1991 by Walter Bright
; All rights reserved.
; Written by Dave Mansell
; Windows support added by G. Eric Engstrom Jan 1991,	-D_WINDOWS

include macros.asm
include stdio.asm

if LCODE
extrn __SET_DOS_ERRNO:far
extrn __SET_ERRNO:far
else
extrn __SET_DOS_ERRNO:near
extrn __SET_ERRNO:near
endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Locking function
;	int locking(int fd, int mode, long size);
; P = file handle
; P+2 = mode
; P+4 = length in bytes

; Values for mode
LK_UNLCK	equ	0
LK_LOCK		equ	1
LK_NBLCK	equ	2
LK_RLCK		equ	3
LK_NBRLCK	equ	4


IFDEF __OS2__

FILELOCK STRUC
	lOffset		dd	?
 	lRange		dw	?
	lRange1		dw	?
FILELOCK ENDS

FILELOCKSIZE equ 8

	extrn	DOSCHGFILEPTR:far
	extrn	DOSFILELOCKS:far
	extrn	DOSSLEEP:far

	begcode	locking

	c_public locking
func locking
	push	BP
	mov	BP,SP
	.save	<SI,DI>
	sub	SP,FILELOCKSIZE
	mov	BX,SP		;BX points to FILELOCK structure
	mov	CX,10
	push	P[BP]		;file handle
	clr	AX
	push	AX
	push	AX
	inc	AX
	push	AX
	push	SS
	push	BX
	call	DOSCHGFILEPTR
	;need to do an LSEEK to find out where we are!
	or	AX,AX
	jnz	Lout
	mov	DI,P+4[BP]
	mov	SI,P+6[BP]
	mov	SS:lrange[BX],DI
	mov	SS:lrange1[BX],SI
	_if 	<byte ptr P+2[BP]> ne LK_UNLCK, Lkon
	push	P[BP]		;file handle
	push	SS
	push	BX		;FILELOCK structure
	clr	AX
	push	AX		;null for lock range
	push	AX
	call	DOSFILELOCKS	; unlock the area
	or	AX,AX
	jz	Lok
Lkon:	push	P[BP]		;file handle
	clr	AX
	push	AX		;null for unlock range
	push	AX
	push	SS
	push	BX		;FILELOCK structure
	call	DOSFILELOCKS	; lock the area
	or	AX,AX
	jz	Lok
	_if	<byte ptr P+2[BP]> e LK_LOCK, Lout
	_if	<byte ptr P+2[BP]> e LK_RLCK, Lout
	loop	L3
	mov	AX,EDEADLOCK
	jmps	Lout
L3:	pushi	AX,0
	pushi	AX,1000
	call	DOSSLEEP
	jmps	Lkon

Lok:	clr	AX
Lout:   tst	AX
	jz	Lx
	errno_set
	mov	AX,-1
Lx:	add	SP,FILELOCKSIZE
	.restore <DI,SI>
	pop	BP
	ret
c_endp	locking

ELSE	; MSDOS version

	begcode	locking

count	equ	byte ptr -1[BP]		;retry count
mins	equ	byte ptr -2[BP]
hundredths equ	word ptr -4[BP]

;local support function
bdos_2C proc	near
	bdos	2Ch
	mov	AL,DH
	cbw
	mov	BX,100
	imul	BX
	clr	DH
	add	AX,DX
	ret
bdos_2C endp

bdos_5C proc	near
	mov	DI,P+4[BP]	;lsw(size)
	mov	SI,P+6[BP]	;msw(size)
	bdos	5Ch
	ret
bdos_5C endp

	c_public locking
func locking
	WINENTER
	sub	SP,4
	.save	<SI,DI>
	mov	count,11		;retry 10 times
	mov	AX,4201h
	mov	BX,P[BP]		;get fd
	cwd
	mov	CX,DX			;CX:DX = 0
	bdos				;get current file position
	jc	Lerr
	mov	CX,DX
	mov	DX,AX			;which becomes region offset
	clr	AX
	_if	<byte ptr P+2[BP]> ne AL, L1	;if not LK_UNLCK
	inc	AX				;unlock region
L1:	call	bdos_5C			;lock/unlock region
	jnc	Lok			;no errors
	_if	AX e 1, Lerr		;if invalid function
	test	byte ptr P+2[BP], 1	;if not (LK_LOCK or LK_RLCK)
	je	Lerr

L2:	dec	count
	je	Ldeadlock
	push	CX
	push	DX

	;Delay 1 second
	call	bdos_2C
	mov	mins,CL
	mov	hundredths,AX
L4:	call	bdos_2C
	_if	CL e mins, L5
	add	AX,6000
L5:	sub	AX,hundredths
	_if	AX l 100, L4

	pop	DX
	pop	CX
	clr	AX
	mov	BX,P[BP]		;get file handle
	call	bdos_5C			;try locking again
	jc	L2			;failed again

Lok:	clr	AX
Lx:	.restore <DI,SI>
	mov	SP,BP
	WINLEAVE
	ret

Ldeadlock:
	mov	AX,EDEADLOCK
	push    AX
        call    __SET_ERRNO
        mov     AX,-1
        jmp     Lx
Lerr:	push    AX
        call    __SET_DOS_ERRNO
	mov	AX,-1
	jmp	Lx
c_endp	locking

ENDIF

	endcode	locking

	end
