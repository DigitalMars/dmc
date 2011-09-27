;_ utime.asm   Sun Jun	4 1989	 Modified by: Walter Bright */
; Windows support added by G. Eric Engstrom Jan 1991	-D_WINDOWS
; OS2 support added by Nikki Locke May 1989		-D__OS2__
; WIN32 support added by Richard S. Sadowsky Nov 1993 -D__NT__
; Copyright (C) 1987-1989 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

include macros.asm
include stdio.asm

if LCODE
extrn __SET_DOS_ERRNO:far
else
extrn __SET_DOS_ERRNO:near
endif

;Offsets into struct tm (must match time.h values)
tm_sec	=	0
tm_min	=	2
tm_hour =	4
tm_mday =	6
tm_mon	=	8
tm_year =	10
tm_wday =	12
tm_yday =	14
tm_isdst =	16

; variables used by utime
t		equ -4
	ifdef __OS2__

extrn	DOSSETFILEINFO:far

; structure used by DOSSETFILEINFO - offsets into stack frame
cdate		equ -16
ctime		equ -14
adate		equ -12
atime		equ -10
wdate		equ -8
wtime		equ -6

structsize	equ 12
varsize		equ 16

	  if LCODE
	c_extrn open,far, close,far
	  else
	c_extrn open,near, close,near
	  endif
	else ;__OS2__
varsize		equ 4
	endif ;__OS2__

	if LCODE
	c_extrn time,far, localtime,far
	else
	c_extrn time,near, localtime,near
	endif

	begcode utime

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set the time stamp on a file.
;	int utime(char *filespec,time_t timep[2]);
; Input:
;	filespec -> ascii string giving the path and filename
;	timep ->	a 'last accessed' time and 'last modified' time,
;			respectively. MS-DOS has no concept of 'last
;			accessed' time, so that field is ignored, and the
;			time stamp is set to the 'last modified' time.
;			If timep is NULL, the current time is used.
; Returns:
;	0	success
;	-1	failure, errno will have a clue

	c_public utime
	public	 __utime
__utime:
func	utime
	WINENTER
	sub SP,varsize

	if LPTR
	les BX,P+SIZEPTR[BP]
	mov AX,ES
	or	AX,BX
	jz	L2
	mov AX,ES:4[BX]
	mov DX,ES:6[BX] ;t = timep[1]; get 'last modified' time
	else
	mov BX,P+SIZEPTR[BP]
	or	BX,BX
	jz	L2
	mov AX,4[BX]
	mov DX,6[BX]	;t = timep[1]; get 'last modified' time
	endif
	jmps	L3
L2:
	if LPTR
	push	BX
	endif
	push	BX		;push NULL
	callm	time
	add SP,SIZEPTR
L3:
	mov t[BP],AX
	mov t+2[BP],DX

	;bd = localtime(&t);
	if LPTR
	push	SS
	endif
	lea BX,t[BP]
	push	BX
	callm	localtime	;localtime(&t)
	add SP,SIZEPTR

	mov BX,AX
	if LPTR
	push	DS
	mov DS,DX
	endif

	;date = ((((bd->tm_year - 80) << 4) + bd->tm_mon + 1) << 5) +
	;	bd->tm_mday;

	mov AX,tm_year[BX]
	sub AX,80
	mov CL,4
	shl AX,CL
	add AX,tm_mon[BX]
	inc AX
	inc CL
	shl AX,CL
	add AX,tm_mday[BX]
	ifdef __OS2__
	mov t[BP],AX		;date
	else
	mov DX,AX
	endif

	;tday = (((bd->tm_hour << 6) + bd->tm_min) << 5) + (bd->tm_sec >> 1);
	mov AX,tm_hour[BX]
	inc CL
	shl AX,CL
	add AX,tm_min[BX]
	dec CL
	shl AX,CL
	mov CX,tm_sec[BX]
	shr CX,1
	add CX,AX
	ifdef __OS2__
	  if LPTR
	pop DS
	  endif
	mov t+2[BP],CX		;time

	;fd = open(filename,O_RDWR);
	mov AX,8002h		;mode = O_RDWR|O_BINARY
	push	AX
	  if LPTR
	push	P+2[BP]			;segment of file name
	  endif
	push	P[BP]			;offset of file name
	callm	open
	add SP,SIZEPTR+2		;fix stack
	_if AX e -1, L1		;-1 = error (errno already set)

	mov BX,AX			;handle
	mov CX,t+2[BP]		;time
	mov DX,t[BP]		;date

	mov wdate[bp],dx
	mov wtime[bp],cx
	clr CX
	; OS2 1.1 demands that the rest of the fields be 0!
	mov cdate[bp],CX
	mov ctime[bp],CX
	mov adate[bp],CX
	mov atime[bp],CX
	push	ax			;handle
	pushi	AX,1			;info level 1
	push	ss			;segment of info
	lea ax,cdate[bp]
	push	ax			;offset of info to set
	pushi	AX,structsize		;size of info buffer
	call	DOSSETFILEINFO
	test	ax,ax
	jnz err


	push	BX			;file handle
	callm	close
	inc SP
	inc SP			;fix stack
	_if AX e -1, L1		;-1 = error (errno already set)

	else ;__OS2__
	push	DX
	mov DX,P[BP]
	mov AX,03D00h		;open file for reading
	if LPTR
	mov DS,P+2[BP]
	endif
	bdos
	pop DX
	if LPTR
	pop DS
	endif
	jc	err

	mov BX,AX		;handle
;	mov CX,tday
;	mov DX,date
	mov AX,05701h	;set date/time
	bdos
	jc	err

	bdos	3Eh		;close file
	jc	err
	endif ;__OS2__

	clr AX
L1:
	mov SP,BP
		WINLEAVE
	ret

err:	push    AX
        call    __SET_DOS_ERRNO
	ifdef __OS2__
	push	BX			;file handle
	callm	close
	inc SP
	inc SP			;fix stack
	else
	bdos	3Eh		;close file
	endif
	mov AX,-1
	jmp L1
c_endp	utime

	endcode utime
	end
