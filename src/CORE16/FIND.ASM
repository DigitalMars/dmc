;_ find.asm   Thu Jan 11 1990	Modified by: Walter Bright
; Windows support added by G. Eric Engstrom Dec 1990	-D_WINDOWS
; OS2 support added by Nikki Locke May 1989		-D__OS2__
; Modified by Richard S. Sadowsky July 29, 1993
; Copyright (C) 1985-1990 by Walter Bright
; All Rights Reserved
; Written by Walter Bright
; $Revision: 1.1.1.1 $

include macros.asm
include stdio.asm

if LCODE
extrn __SET_DOS_ERRNO:far
else
extrn __SET_DOS_ERRNO:near
endif

	begdata

    ifdef __OS2__

	extrn	DOSFINDFIRST:far
	extrn	DOSFINDNEXT:far
	extrn	DOSFINDCLOSE:far

FindHandle	dw	-1		;file find handle
noentries	dw	1		;number of entries to find

FileFindBuf	label	byte
create_date	dw	0		;date of file creation
create_time	dw	0		;time of file creation
access_date	dw	0		;date of last access
access_time	dw	0		;time of last access
write_date	dw	0		;date of last write
write_time	dw	0		;time of last write
file_sizel	dw	0		;file size (end of data)
file_sizeh	dw	0		;file size (end of data)
falloc_size	dd	0		;file allocated size
attributes	dw	0		;attributes of the file
string_len	db	0		;returned length of ascii name str.
file_name	db	13 dup(0)	;name string

    endif ;__OS2__

FIND	label	byte		;struct used by findfirst() and findnext()
reserved	db	21 dup(0)	;reserved by DOS
attribute	db	0		;attribute found (FA_XXXX)
time		dw	0
date		dw	0		;file's time and date
sizel		dw	0		;file's size
sizeh		dw	0		;file's size
fname		db	13 dup(0)	;filename followed by 0 byte
		db	?
	enddata

	begcode find

;;;;;;;;;;;;;;;;;;;;;;
; Find first matching file.
; Input:
;	name	Drive, path and filename of file to be found. May
;		include global filename characters.
;	attribute	Attribute of file to search for. Attributes
;		are described in the MS-DOS manual. The search strategy
;		is described under DOS call 0x11.
; Returns:
;	Pointer to a static struct FIND.
;	If error, NULL is returned and errno is set to the error #.
;

	c_public findfirst
func	findfirst
	WINENTER
    ifdef __OS2__
	mov	AX,FindHandle
	cmp	AX,-1			;is handle active ?
	jz	ff1
	push	AX			;yes - so close it
	call	DOSFINDCLOSE
	mov	FindHandle,-1
ff1:
	mov	noentries,1
    if SPTR
	push	DS			;name segment
    else
	push	P+2[BP]			;name segment
    endif
	push	P[BP]			;name offset
	push	DS
	pushi	AX,<offset DGROUP:FindHandle>	;address of handle
	push	P+SIZEPTR[BP]			;attribute
	push	DS
	pushi	AX,<offset DGROUP:FileFindBuf>	;address of buffer
	pushi	AX,<offset FIND - offset FileFindBuf>	;size of buffer
	push	DS
	pushi	AX,<offset DGROUP:noentries>	;address of number of entries
	xor	AX,AX
	push	AX
	push	AX			;reserved 0l
	call	DOSFINDFIRST
found:
	test	AX,AX
	jnz	FN1

	_if	noentries ne 1, FN3	;no matches

	mov	AX,attributes
	mov	attribute,AL
	mov	AX,write_time
	mov	time,AX
	mov	AX,write_date
	mov	date,AX
	mov	AX,file_sizel
	mov	sizel,AX
	mov	AX,file_sizeh
	mov	sizeh,AX
	.save	<SI,DI>
	push	DS
	pop	ES
	mov	SI,offset DGROUP:file_name
	mov	DI,offset DGROUP:fname
	xor	CH,CH
	mov	CL,string_len
	rep	movsb
	xor	AL,AL
	stosb				;0 terminate
	.restore <DI,SI>
	jmps	FNX
    else ;__OS2__
	push	DS
	bdos	2Fh			;ES:BX = DTA
	push	BX
	push	ES
	mov	DX,offset DGROUP:FIND
	bdos	1Ah			;DTA = FIND
    if LPTR
	mov	DS,P+2[BP]
    endif
	mov	DX,P[BP]		;DS:DX = name
	mov	CX,P+SIZEPTR[BP]	;CX = attribute
	bdos	4Eh			;find first
	jmps	FN2
    endif ;__OS2__
c_endp	findfirst

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Find next matching file.
; Returns:
;	pointer to static struct FIND
;	NULL if no more matching files found

	c_public findnext
func	findnext
	WINENTER
    ifdef __OS2__
	mov	noentries,1
	push	FindHandle			;handle
	push	DS
	pushi	AX,<offset DGROUP:FileFindBuf>		;address of buffer
	pushi	AX,<offset FIND - offset FileFindBuf>	;size of buffer
	push	DS
	pushi	AX,<offset DGROUP:noentries>	;address of number of entries
	call	DOSFINDNEXT
	jmp	found
    else
	push	DS
	bdos	2Fh			;ES:BX = DTA
	push	BX
	push	ES
	mov	DX,offset DGROUP:FIND
	bdos	1Ah			;DTA = FIND
	bdos	4Fh			;findnext

FN2:	pop	DS			;DS:DX = old DTA
	pop	DX
	pushf				;save carry flag
	push	AX
	bdos	1Ah			;restore DTA
	pop	AX
	popf				;restore carry flag
	pop	DS			;restore original DS
	jc	FN1
    endif
FNX:
	mov	AX,offset DGROUP:FIND	;return &FIND
    if LPTR
	mov	DX,DS
    endif
	jmps	FN4
FN1:	push    AX
	call    __SET_DOS_ERRNO
FN3:	clr	AX
    if LPTR
	cwd
    endif
FN4:	WINLEAVE
	ret
c_endp	findnext

	endcode find

	end

