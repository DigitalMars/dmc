;_ dosfil.asm   Tue Oct 10 1989   Modified by: Walter Bright */
; Windows support added by G. Eric Engstrom Dec 1990,	-D_WINDOWS
;Copyright (C) 1989 by Walter Bright
;All rights reserved.
;Written by Dave Mansell

include	macros.asm
include stdio.asm

if LCODE
extrn __SET_DOS_ERRNO:far
else
extrn __SET_DOS_ERRNO:near
endif

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

    ifdef __OS2__
	extrn	DOSQCURDISK:far
	extrn	DOSSELECTDISK:far
	extrn	DOSQFILEINFO:far
	extrn	DOSSETFILEINFO:far
	extrn	DOSQFILEMODE:far
	extrn	DOSSETFILEMODE:far
    endif

	begcode	dosfil

	c_public dos_setdrive,dos_getdrive
	c_public dos_setftime,dos_getftime
	c_public dos_setfileattr,dos_getfileattr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; get default disk drive 
; 1 = A, 2 = B etc.

func	dos_getdrive
	WINENTER
IFDEF __OS2__
	sub	SP,6		;local variables
	mov	BX,SP		;points at locals
	push	SS
	push	BX		;no of drives
	inc	BX
	inc	BX
	push	SS
	push	BX		;drive map
	call	DOSQCURDISK
	mov	AX,SS:[BX-2]
	add	SP,6		;remove local variable
ELSE
	bdos	19h		;get default disk drive no
	clr	AH
	inc	AX		;add 1 (to match MSC)
ENDIF	;__OS2__
    if SPTR
	mov	BX,P[BP]	;where to store the drive no
D0:	mov	[BX],AX
    else
	les	BX,P[BP]	;where to store the drive no
D0:	mov	ES:[BX],AX
    endif	;SPTR
	WINLEAVE
	ret
c_endp	dos_getdrive

;;;;;;;;;;;;;;;;;;;;;;;;;
; set default disk drive
; dos_setdrive(unsigned drive, unsigned * no_of_drives)

func	dos_setdrive
	WINENTER
IFDEF __OS2__
	sub	SP,6		;unsigned for drive no
	mov	BX,SP		;long for drive map
	push	SS
	push	BX		;no of drives
	inc	BX
	inc	BX
	push	SS
	push	BX		;drive map
	call	DOSQCURDISK
	mov	CX,16		;count logical drives
	clr	DX
	mov	AX,SS:[BX]	;get the low word
L1:	test	AL,1		;every one set means a logical drive
	jz	L2
	inc	DX
L2:	shr	AX,1
	loop	L1
	mov	CL,10
	mov	AX,SS:[BX+2]	;get the high word
L3:	test	AL,1
	jz	L4
	inc	DX
L4:	shr	AX,1
	loop	L3
	add	SP,6		;remove local variables
	push	DX		;save no of logical drives
	push	P[BP]		;drive to set
	call	DOSSELECTDISK
	pop	AX		;get no of logical drives
ELSE
	mov	DX,P[BP]	;disk drive number (1 = A)
	dec	DX		;subtract 1 for dos call
	bdos	0Eh		;set default drive to DL
	clr	AH		;AL = total no of logical drives
ENDIF	;__OS2__
    if SPTR
	mov	BX,P+2[BP]	;where to store the no of drives
    else
	les	BX,P+2[BP]	;where to store the no of drives
    endif	 ;SPTR
	jmp	short D0
c_endp	dos_setdrive

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; get the file date/time.
; int dos_getftime(int fd,unsigned *date,unsigned *time)

func	dos_getftime
	WINENTER
IFDEF __OS2__
	sub	SP,FILEINFOSIZE
	mov	BX,SP
	push	P[BP]		;file descriptor
	pushi	AX,00001h	;level of info required
	push	SS		;far pointer to FILEINFO
	push	BX		;structure
	pushi	AX,FILEINFOSIZE	;size of FILEINFO structure
	call	DOSQFILEINFO
	mov	CX,SS:twrite[BX]
	mov	DX,SS:dwrite[BX]
	or	AX,AX
	jnz	A2
ELSE
	mov	BX,P[BP]	;file descriptor
	mov	AX,5700h
	bdos			;get file time
	jc	A2		;if error
ENDIF	;__OS2__
    if SPTR
	mov	BX,P+2[BP]	;where to store the date
	mov	[BX],DX
	mov	BX,P+4[BP]	;where to store the time
	mov	[BX],CX
    else
	les	BX,P+2[BP]	;where to store the date
	mov	ES:[BX],DX
	les	BX,P+6[BP]	;where to store the time
	mov	ES:[BX],CX
    endif	;SPTR
A4:	clr	AX
IFDEF __OS2__
	add	SP,FILEINFOSIZE
ENDIF
	WINLEAVE
	ret
c_endp	dos_getftime

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; set the file date/time.
; int dos_setftime(int fd,unsigned date,unsigned time)

func	dos_setftime
	WINENTER
IFDEF __OS2__
	sub	SP,FILEINFOSIZE
	mov	BX,SP
	push	p[BP]		;file descriptor
	pushi	AX,1		;level of info required
	push	SS		;far pointer to FILEINFO
	push	BX		;structure
	pushi	AX,FILEINFOSIZE	;size of FILEINFO structure
	call	DOSQFILEINFO	;get file info
	or	AX,AX
	jnz	A2		;if error
	mov	DX,P+2[BP]
	mov	CX,P+4[BP]
	mov	SS:dwrite[BX],DX	;set date
	mov	SS:twrite[BX],CX	;set time
	push	P[BP]		;file descriptor
	pushi	AX,1		;level of info required
	push	SS		;far pointer to FILEINFO
	push	BX		;structure
	pushi	AX,FILEINFOSIZE	;size of FILEINFO structure
	call	DOSSETFILEINFO	;set new values
	or	AX,AX
	jz	A4
ELSE
	mov	DX,P+2[BP]	;the date
	mov	CX,P+4[BP]	;the time
	mov	BX,P[BP]	;file descriptor
	mov	AX,5701h
	bdos 			;set file time
A3:	jnc	A4
ENDIF	;__OS2__
A2:	push    AX
        call    __SET_DOS_ERRNO
IFDEF __OS2__
	add	SP,FILEINFOSIZE
ENDIF
	WINLEAVE
	ret
c_endp	dos_setftime

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; get the file attribute.
; unsigned dos_getfileattr(char *path,unsigned *attribute)

func	dos_getfileattr
	WINENTER
IFDEF __OS2__
    if SPTR
	push	DS
	push	P[BP]		;path
	push	DS
	push	P+2[BP]		;where to store the attribute
    else
	les	BX,P[BP]	;far pointer to path
	push	ES
	push	BX
	les	BX,P+4[BP]	;where to store the attribute
	push	ES
	push	BX
    endif	;SPTR
	clr	AX
	push	AX		;reserved unsigned long
	push	AX
	call	DOSQFILEMODE
	or	AX,AX
	jnz	A5
ELSE	;__OS2__
	mov	AX,4300h
    if SPTR
	mov	DX,P[BP]	;path
	bdos			;get file attribute
	jc	A5
	clr	AX
	mov	BX,P+2[BP]	;where to store the attribute
	mov	[BX],CX
    else
	push	DS
	lds	DX,P[BP]	;path
	bdos			;get file attribute
	pop	DS
	jc	A5
	clr	AX
	les	BX,P+4[BP]	;where to store the attribute
	mov	ES:[BX],CX
    endif
ENDIF	;__OS2__
	jmps	A7
A5:	push    AX
        call    __SET_DOS_ERRNO
A7:	WINLEAVE
	ret
c_endp	dos_getfileattr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; set the file attribute.
; unsigned dos_setfileattr(char *path,unsigned attribute)

func	dos_setfileattr
	WINENTER
IFDEF __OS2__
IF SPTR
	push	DS
	push	P[BP]		;path
ELSE
	les	BX,P[BP]	;far pointer to path
	push	ES
	push	BX
ENDIF	;SPTR
	push	P+SIZEPTR[BP]	;the attribute
	clr	AX
	push	AX		;reserved unsigned long
	push	AX
	call	DOSSETFILEMODE
	or	AX,AX
	jnz	A5
ELSE	;__OS2__
	mov	AX,4301h
    if SPTR
	mov	DX,P[BP]	;path
	mov	CX,P+2[BP]	;attribute
	bdos			;set file attribute
    else
	push	DS
	lds	DX,P[BP]	;path
	mov	CX,P+4[BP]	;attribute
	bdos			;set file attribute
	pop	DS
    endif
	jc	A5
	clr	AX
ENDIF
	jmps	A7
c_endp	dos_setfileattr
	endcode	dosfil

	end

