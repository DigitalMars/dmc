;_ dosfil.asm   Tue Oct 10 1989   Modified by: Walter Bright */
; $Header: z:/core/crtl/src/dos32/dosfil.asm,v 1.1.1.1 1997/01/02 19:17:56 smarx Exp $
;Copyright (C) 1989-1991 by Walter Bright
;All rights reserved.
;Written by Dave Mansell

include	macros.asm
include stdio.asm

extrn __SET_DOS_ERRNO:near

FILEINFO STRUC
	dcreate		dw	?
 	tcreate		dw	?
	daccess		dw	?
	taccess		dw	?
	dwrite		dw	?
	twrite		dw	?
	fsize		dd	?
	falloc		dd	?
	fattr		dw	?
FILEINFO ENDS

FILEINFOSIZE EQU 22

	begcode	dosfil

	c_public dos_setdrive,dos_getdrive
	c_public dos_setftime,dos_getftime
	c_public dos_setfileattr,dos_getfileattr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; get default disk drive 
; 1 = A, 2 = B etc.

func	dos_getdrive
	bdos	19h		;get default disk drive no
	movzx	EAX,AL
	inc	EAX
    if SPTR
	mov	ECX,PS[ESP]	;where to store the drive no
	mov	[ECX],EAX
    else
	les	ECX,PS[ESP]	;where to store the drive no
	mov	ES:[ECX],EAX
    endif
	ret
c_endp	dos_getdrive

;;;;;;;;;;;;;;;;;;;;;;;;;
; set default disk drive
;	void dos_setdrive(unsigned drive, unsigned *no_of_drives);

func	dos_setdrive
	push	EBP
	mov	EBP,ESP
	mov	EDX,P[EBP]	;disk drive number (1 = A)
	dec	EDX
	bdos	0Eh		;set default drive to DL
	movzx	EAX,AL		;AL = total no of logical drives
    if SPTR
	mov	ECX,P+4[EBP]	;where to store the no of drives
	mov	[ECX],EAX
    else
	les	ECX,P+4[EBP]	;where to store the no of drives
	mov	ES:[ECX],EAX
    endif
	pop	EBP
	ret
c_endp	dos_setdrive

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; get the file date/time.
;	int dos_getftime(fd,date,time)

func	dos_getftime
	push	EBP
	mov	EBP,ESP
	uses	EBX
	mov	BX,P[EBP]	;file descriptor
	mov	AX,5700h
	bdos			;get file time
	jc	A2		;if error
	movzx	ECX,CX
	movzx	EDX,DX
    if SPTR
	mov	EBX,P+4[EBP]	;where to store the date
	mov	[EBX],EDX
	mov	EBX,P+4+SIZEPTR[EBP]	;where to store the time
	mov	[EBX],ECX
    else
	les	EBX,P+4[EBP]	;where to store the date
	mov	ES:[EBX],EDX
	les	EBX,P+4+SIZEPTR[EBP]	;where to store the time
	mov	ES:[EBX],ECX
    endif
A4:	clr	EAX
	unuse	EBX
	pop	EBP
	ret
c_endp	dos_getftime

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; set the file date/time.
;	int dos_setftime(int fd,unsigned date,unsigned time)

func	dos_setftime
	push	EBP
	mov	EBP,ESP
	uses	EBX
	mov	BX,P[EBP]	;file descriptor
	mov	AX,5701h
	mov	EDX,P+4[EBP]	;the date
	mov	ECX,P+8[EBP]	;the time
	bdos 			;set file time
A3:	jnc	A4
A2:	movzx   EAX,AX
        push    EAX
        call    __SET_DOS_ERRNO
	unuse	EBX
	pop	EBP
	ret
c_endp	dos_setftime

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; get the file attribute.
;	int dos_getfileattr(char *path,unsigned *attribute);

func	dos_getfileattr
	push	EBP
	mov	EBP,ESP
	uses	EBX
	mov	AX,4300h
    if SPTR
	mov	EDX,P[EBP]	;path
	bdos			;get file attribute
	jc	A2
	mov	EBX,P+SIZEPTR[EBP]	;where to store the attribute
	movzx	ECX,CX
	mov	[EBX],ECX
    else
	push	DS
	lds	EDX,P[EBP]	;path
	bdos			;get file attribute
	pop	DS
	jc	A2
	les	EBX,P+SIZEPTR[EBP]	;where to store the attribute
	movzx	ECX,CX
	mov	ES:[EBX],ECX
    endif
	jmp	A4
c_endp	dos_getfileattr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; set the file attribute.
;	unsigned dos_setfileattr(char *path,unsigned attribute)

func	dos_setfileattr
	push	EBP
	mov	EBP,ESP
	uses	EBX
	mov	AX,4301h
    if SPTR
	mov	EDX,P[EBP]		;path
	mov	ECX,P+SIZEPTR[EBP]	;attribute
	bdos				;get file attribute
    else
	push	DS
	lds	EDX,P[EBP]		;path
	mov	ECX,P+SIZEPTR[EBP]	;attribute
	bdos				;get file attribute
	pop	DS
    endif
	jmp	A3
c_endp	dos_setfileattr

	endcode	dosfil

	end

