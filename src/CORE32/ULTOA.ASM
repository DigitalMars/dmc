;_ ultoa.asm   Modified by: Walter Bright */
; Copyright (C) 1991 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

include	macros.asm

	c_public	ultoa
	extrn		___itoa:near

	begcode	ultoa

;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert integer i to ascii, and store in string pointed to by a.
;	char *ultoa(long i,char *a,int base)
; Input:
;	a	pointer to string of sufficient length,
;		sizeof(int)*8+1 is always sufficient
;	base	radix, between 2 and 36 inclusive
; Returns:
;	a

func	ultoa
	push	EBP
	mov	EBP,ESP
	sub	ESP,36			;char buf[33] (rounded to dword)
	uses	<EBX,ESI,EDI>

    if SPTR
	push	DS
	pop	ES
    endif

    ife SSeqDS
	push	DS
	push	SS
	pop	DS
    endif

	mov	EDI,P+4+SIZEPTR[EBP]	;EDI = base

	mov	EBX,1			;assume negative
	mov	ECX,P[EBP]		;ECX = i
	lea	ESI,-1[EBP]		;ESI = &buf[sizeof(buf)-1] ;last char pos
	mov	[ESI],BH		;*ESI = 0
	jmp     ___itoa			;continue processing w/itoa
c_endp	ultoa

	endcode	ultoa

	end
