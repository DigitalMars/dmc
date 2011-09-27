;_ ehasm.asm
; Copyright (C) 1994 by Digital Mars
; All rights reserved.
; Written by Walter Bright
; Exception handling

include	macros.asm

_DATA	segment dword use32 public 'DATA'
_DATA	ends
if OMF
FIB	segment	dword use32 public 'DATA'
FIB	ends
FI	segment	byte use32 public 'DATA'
FI	ends
FIE	segment	byte use32 public 'DATA'
FIE	ends


;Stuff all these segments into one group so they can all be accessed by DS
DGROUP	group	_DATA,FIB,FI,FIE
else
DGROUP	group	_DATA
endif

if COFF OR ELF
	begdata
	extrn __xeb:dword
	extrn __xee:dword
	enddata
endif

	begcode	ehasm

SIZEFP	equ	4

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Find and return pointer to static data for address in function.
;	struct Ehfunc *__eh_finddata(fp_t fp);
; Returns:
;	pointer to data if found
;	NULL if not found

	c_public	__eh_finddata
func	__eh_finddata
	push	ebp
	mov	ebp,esp
	push	ecx
	push	edx
	mov	ECX,P[EBP]		;get address

if COFF OR ELF
	mov	EAX,offset DGROUP:__xeb
else
	mov	EAX,offset DGROUP:FIB
endif
	jmp	short EH1
	
	clr	EAX
	pop	edx
	pop	ecx
	pop	ebp
	ret

EH4:	add	EAX,SIZEFP+SIZEPTR+4	;next entry in table
if COFF OR ELF
EH1:	_if	EAX ae <offset DGROUP:__xee>,EH2
else
EH1:	_if	EAX ae <offset DGROUP:FIE>,EH2
endif
	mov	EDX,[EAX]			;offset of start of function
	_if	ECX b EDX, EH4
	add	EDX,SIZEFP+SIZEPTR[EAX]	;EDX is now offset of end of function
	_if	ECX ae EDX, EH4
	pop	edx
	pop	ecx
	pop	ebp
	ret

EH2:	clr	EAX			;not found, return NULL
	pop	edx
	pop	ecx
	pop	ebp
	ret
c_endp	__eh_finddata


	endcode	ehasm

	end
