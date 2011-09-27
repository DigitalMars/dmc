;_ filelen.asm   Tue Sep  5 1989   Modified by: Walter Bright */
; $Header: z:/core/crtl/src/dos32/filelen.asm,v 1.1.1.1 1997/01/02 19:17:57 smarx Exp $
;Copyright (C) 1989-1990 by Walter Bright
;All rights reserved.

include	macros.asm

SEEK_SET	equ	0
SEEK_CUR	equ	1
SEEK_END	equ	2

	if LCODE
	extrn	_lseek:far
	else
	extrn	_lseek:near
	endif

	begcode filelen

	c_public filelength

func	filelength
	push	EBP
	mov	EBP,ESP
	add	ESP,-4		;make room for local variables
	push	ESI		;save ESI
	mov	ESI,P[EBP]	;file handle
	push	SEEK_CUR
	push	0
	push	ESI
	call	_lseek		;find out where we are	
	add	ESP,12
	mov	-4[EBP],EAX	;save current position
	cmp	EAX,-1		;error?
	je	FLX		;yes, so return -1L (lseek has already
				;set errno)
	push	SEEK_END
	push	0
	push	ESI
	call	_lseek		;find the end of the file
	add	ESP,12
	push	EAX		;save the length
	push	SEEK_SET
	push	-4[EBP]
	push	ESI
	call	_lseek		;restore position in the file
	add	ESP,12
	pop	EAX
FLX:
	pop	ESI
	mov	ESP,EBP
	pop	EBP
	ret
c_endp	filelength

	endcode filelen

	end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; The C version is a bit bigger.
;/* _ filelen.c   Sat Aug 19 1989   Modified by: Walter Bright */
;/* Copyright (C) 1989 by Walter Bright		*/
;/* All Rights Reserved					*/
;/* Written by Walter Bright				*/
;
;#include <io.h>
;
;long filelength(int fd)
;{
;    long curpos;
;    long length;
;
;    /* Determine file size using lseek() */
;    curpos = lseek(fd,0L,SEEK_CUR);
;    if (curpos == -1L)
;	length = -1L;			/* errno was set by lseek()	*/
;    else
;    {	length = lseek(fd,0L,SEEK_END);
;	lseek(fd,curpos,SEEK_SET);
;    }
;    return length;
;}
