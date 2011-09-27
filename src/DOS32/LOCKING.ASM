;_ locking.asm   Fri Dec 15 1989   Modified by: Walter Bright */
;Copyright (C) 1989-1990 by Walter Bright
;All rights reserved.
;Written by Dave Mansell 

include macros.asm
include stdio.asm

extrn __SET_DOS_ERRNO:near
extrn __SET_ERRNO:near

	begcode

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	int locking(int fd, int mode, long size);

count	equ	byte ptr -1[EBP]
mins	equ	byte ptr -2[EBP]
hundredths equ	word ptr -4[EBP]

	c_public locking
func	locking
	push	EBP
	mov	EBP,ESP
	sub	ESP,4
	uses	<EBX,ESI,EDI>
	mov	AX,4201h
	mov	EBX,P[EBP]		;get file handle
	clr	ECX
	mov	EDX,ECX
	bdos				;seek to current position in file
	jc	Lerr
	mov	ECX,EDX			;high word of region offset
	mov	EDX,EAX			;low word of region offset
	mov	AX,5C00h
	mov	DI,P+4+4[EBP]		;low word of region length
	mov	SI,P+4+4+2[EBP]		;high word of region length
	_ifs	<byte ptr P+4[EBP]> ne AL, L1
	inc	AL
L1:	bdos
	jnc	short Lok
	_ifs	AX e 1, Lerr
	test	byte ptr P+4[EBP], 1
	je	Lerr

	mov	count,11
L2:	dec	count
	je	short Ldeadlock
	push	ECX
	push	EDX
	bdos	2Ch			;get system time
	movzx	EAX,DH			;EAX = seconds (0-59)
	imul	EAX,100
	movzx	EDX,DL
	add	EAX,EDX
	mov	mins,CL
	mov	hundredths,AX
L4:	bdos	2Ch
	movzx	EAX,DH
	imul	EAX,100
	movzx	EDX,DL
	add	EAX,EDX
	cmp	CL,mins
	je	short L5
	add	AX,6000
L5:	sub	AX,hundredths
	_ifs	AX l 100, L4
	pop	EDX
	pop	ECX
	mov	AX,5C00h
	mov	EBX,P[EBP]		;fd
	mov	DI,P+4+4[EBP]
	mov	SI,P+4+4+2[EBP]
	bdos
	jc	short L2
Lok:	clr	EAX
Lx:	unuse	<EDI,ESI,EBX>
	mov	ESP,EBP
	pop	EBP
	ret

Ldeadlock:
	push    dword ptr EDEADLOCK
        call    __SET_ERRNO
        jmp     Lx
Lerr:	push    EAX
        call    __SET_DOS_ERRNO
	mov	EAX,-1
	jmp	Lx
c_endp	locking

	endcode
	end
