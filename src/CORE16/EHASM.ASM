;_ ehasm.asm
; Copyright (C) 1994 by Digital Mars
; All rights reserved.
; Written by Walter Bright
; Exception handling

_DATA	segment dword public 'DATA'
_DATA	ends
FIB	segment	word public 'DATA'
FIB	ends
FI	segment	word public 'DATA'
FI	ends
FIE	segment	word public 'DATA'
FIE	ends


;Stuff all these segments into one group so they can all be accessed by DS
DGROUP	group	_DATA,FIB,FI,FIE

include	macros.asm

	begcode	ehasm

if LCODE
SIZEFP	equ	4
else
SIZEFP	equ	2
endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Find and return pointer to static data for address in function.
;	struct Ehfunc *__eh_finddata(fp_t fp);
; Returns:
;	pointer to data if found
;	NULL if not found

	c_public	__eh_finddata
func	__eh_finddata
	WINENTER

	mov	AX,P[BP]		;get IP of address
    if LCODE
	mov	DX,P+2[BP]		;get CS of address
    endif

	mov	DI,offset DGROUP:FIB
	jmp	short EH1

EH4:	add	DI,SIZEFP+SIZEPTR+2	;next entry in table
EH1:	_if	DI ae <offset DGROUP:FIE>,EH2
    if LCODE
	_if	DX ne 2[DI], EH4	;if segments don't match
    endif
	mov	BX,[DI]			;offset of start of function
	_if	AX b BX, EH4
	add	BX,SIZEFP+SIZEPTR[DI]	;BX is now offset of end of function
	_if	AX ae BX, EH4
	mov	AX,DI
	mov	DX,DS
	jmp	short EH3

EH2:	clr	AX			;not found, return NULL
	cwd

EH3:	WINLEAVE
	ret
c_endp	__eh_finddata


	endcode	ehasm

	end
