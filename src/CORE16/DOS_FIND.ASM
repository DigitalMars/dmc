;_ dos_find.asm   29 July 1993 Modified by: Richard S. Sadowsky
;_ dos_find.asm   22 May 1990	Modified by: Bob Stout
; Windows support added by G. Eric Engstrom Dec 1990,	-D_WINDOWS
; Copyright (C) 1985-1990 by Walter Bright
; All Rights Reserved
; Written by Walter Bright, Bob Stout, & Nikki Locke

include macros.asm
include stdio.asm

if LCODE
extrn __SET_DOS_ERRNO:far
else
extrn __SET_DOS_ERRNO:near
endif

ifndef __OS2__
        
	begdata

FIND		STRUC	;struct used by _dos_findfirst() and _dos_findnext()
reserved	db	21 dup(0)	;reserved by DOS
attribute	db	0		;attribute found (FA_XXXX)
time		dw	0
date		dw	0		;file's time and date
sizel		dw	0		;file's size
sizeh		dw	0		;file's size
fname		db	13 dup(0)	;filename followed by 0 byte
FIND		ENDS
	enddata

	begcode dosfind

;;;;;;;;;;;;;;;;;;;;;;
; Find first matching file.
; Input:
;	name	Drive, path and filename of file to be found. May
;		include global filename characters.
;	attribute	Attribute of file to search for. Attributes
;		are described in the MS-DOS manual. The search strategy
;		is described under DOS call 0x11.
;	ffblk	Pointer to a FIND structure.
; Returns:
;	Zero if successful, else errno.
;

	c_public _dos_findfirst
func	_dos_findfirst

	WINENTER
	push	DS
	bdos	2Fh			;EX:BX = DTA
	push	BX
	push	ES
    if LPTR
	lds	DX,P+SIZEPTR+2[BP]	;DS:DX = &ffblk
	bdos	1Ah			;set DTA
	lds	DX,P[BP]		;DS:DX = name
    else
	mov	DX,P+SIZEPTR+2[BP]
	bdos	1Ah			;set DTA
	mov	DX,P[BP]		;DS:DX = name
    endif
	mov	CX,P+SIZEPTR[BP]	;CX = attribute
	bdos	4Eh			;find first
	jmps	FN2
c_endp	_dos_findfirst

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Find next matching file.
; Input:
;	ffblk	Pointer to a FIND structure.
; Returns:
;	Zero if successful, else errno.

	c_public _dos_findnext
func	_dos_findnext
	WINENTER
	push	DS
	bdos	2Fh			;EX:BX = DTA
	push	BX
	push	ES
    if LPTR
	lds	DX,P[BP]
    else
	mov	DX,P[BP]
    endif
	bdos	1Ah			;set DTA
	bdos	4Fh			;findnext
FN2:	pop	DS			;DS:DX = old DTA
	pop	DX
	pushf				;save carry flag
	push	AX
	bdos	1Ah			;restore DTA
	pop	AX
	popf				;restore carry flag
	pop	DS			;restore original DS
	jc	FN1			;if error
	clr	AX			;return zero
	jmp	short FN4

FN1:	push    Ax
        call    __SET_DOS_ERRNO
FN4:	WINLEAVE
	ret
c_endp	_dos_findnext

	endcode dosfind

endif ;__OS2__

	end
