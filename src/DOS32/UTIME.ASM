;_ time2.asm   Sun Jun  4 1989   Modified by: Walter Bright */
; Copyright (C) 1987-1991 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

include	macros.asm
include stdio.asm

extrn __SET_DOS_ERRNO:near

;Offsets into struct tm (must match time.h values)
tm_sec	=	0
tm_min	=	4
tm_hour	=	8
tm_mday	=	12
tm_mon	=	16
tm_year	=	20
tm_wday	=	24
tm_yday	=	28
tm_isdst =	32

; variables used by utime
t		equ	-4
varsize		equ	4

    if LCODE
	extrn	_time:far, _localtime:far
    else
	extrn	_time:near, _localtime:near
    endif

	begcode	utime

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set the time stamp on a file.
;	int utime(char *filespec,time_t timep[2]);
; Input:
;	filespec ->	ascii string giving the path and filename
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
	push	EBP
	mov	EBP,ESP
	sub	ESP,varsize
	uses	<EBX>

    if LPTR
	movzx	EBX,word ptr P+SIZEPTR+4[EBP]
	or	EBX,P+SIZEPTR[EBP]
	jz	L2
	les	EBX,P+SIZEPTR[EBP]
	jz	L2
	mov	EAX,ES:4[EBX]	;t = timep[1]; get 'last modified' time
    else
	mov	EBX,P+SIZEPTR[EBP]
	or	EBX,EBX
	jz	L2
	mov	EAX,4[EBX]	;t = timep[1]; get 'last modified' time
    endif
	jmps	L3
L2:
    if LPTR
	push	EBX
    endif
	push	EBX		;push NULL
	call	_time
	add	ESP,SIZEPTR
L3:
	mov	t[EBP],EAX

	;bd = localtime(&t);
    if LPTR
	push	SS
    endif
	lea	EBX,t[EBP]
	push	EBX
	call	_localtime	;localtime(&t)
	add	ESP,SIZEPTR

	mov	EBX,EAX
    if LPTR
	push	DS
	mov	DS,EDX
    endif

	;date = ((((bd->tm_year - 80) << 4) + bd->tm_mon + 1) << 5) +
	;	bd->tm_mday;

	mov	EAX,tm_year[EBX]
	sub	EAX,80
	shl	EAX,4
	add	EAX,tm_mon[EBX]
	inc	EAX
	shl	EAX,5
	add	EAX,tm_mday[EBX]
	mov	EDX,EAX

	;tday = (((bd->tm_hour << 6) + bd->tm_min) << 5) + (bd->tm_sec >> 1);
	mov	EAX,tm_hour[EBX]
	shl	EAX,6
	add	EAX,tm_min[EBX]
	shl	EAX,5
	mov	ECX,tm_sec[EBX]
	shr	ECX,1
	add	ECX,EAX
	push	EDX
	mov	EDX,P[EBP]
	mov	EAX,03D00h		;open file for reading
    if LPTR
	mov	DS,P+4[EBP]
    endif
	bdos
	pop	EDX
    if LPTR
	pop	DS
    endif
	jc	short err

	mov	EBX,EAX		;handle
;	mov	ECX,tday
;	mov	EDX,date
	mov	EAX,05701h	;set date/time
	bdos
	jc	short err

	bdos	3Eh		;close file
	jc	short err

	clr	EAX
L1:
	unuse	<EBX>
	mov	ESP,EBP
	pop	EBP
	ret

err:	movzx	EAX,AX
	push    EAX
        call    __SET_DOS_ERRNO
	bdos	3Eh		;close file
	mov	EAX,-1
	jmp	L1
c_endp	utime

	endcode	utime

	end
