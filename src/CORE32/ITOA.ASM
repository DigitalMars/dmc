;_ itoa.asm   Modified by: Walter Bright */
; Copyright (C) 1991 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

include	macros.asm

	c_public	itoa
	public		___itoa

	begcode	itoa

;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert integer i to ascii, and store in string pointed to by a.
;	char *itoa(int i,char *a,int base)
; Input:
;	a	pointer to string of sufficient length,
;		sizeof(int)*8+1 is always sufficient
;	base	radix, between 2 and 36 inclusive
; Returns:
;	a

func	itoa
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

	;if (base == 10 && (int) u < 0)
	_ifs	EDI ne 10, L1
	tst	ECX
	jns	short L1
	dec	EBX
	neg	ECX

___itoa:
L1:	dec	ESI
	mov	EAX,ECX
	xor	EDX,EDX
	div	EDI
	mov	ECX,EAX			;u /= base
	add	DL,'0'			;DL = (u % base) + '0';

	_ifs	DL le '9', L50
	add	DL,7			;DL += 'A'-'0'-10;
L50:	mov	[ESI],DL
	or	EAX,EAX
	jne	short L1

	dec	ESI
	mov	byte ptr [ESI],'-'
	add	ESI,EBX			;p += sign;

	;return memcpy(a,p,&buf[sizeof(buf)] - p);
	mov	ECX,EBP
	sub	ECX,ESI
    if SPTR
	mov	EDI,P+4[EBP]		;ES already points to DS
	mov	EAX,EDI			;return a
    else
	les	EDI,P+4[EBP]
	mov	EAX,EDI
	mov	DX,ES
    endif
	rep	movsb			;DS:ESI -> ES:EDI

    ife SSeqDS
	pop	DS
    endif
	unuse	<EDI,ESI,EBX>
	mov	ESP,EBP
	pop	EBP
	_ret	12
c_endp	itoa
	endcode	itoa
	end
