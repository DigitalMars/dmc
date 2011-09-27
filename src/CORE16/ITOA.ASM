;_ itoa.asm	Modified by: Walter Bright */
; Copyright (C) 1991 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

include	macros.asm

	c_public	itoa

	begcode	itoa

;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert integer i to ascii, and store in string pointed to by a.
;	char *itoa(int i,char *a,int base)
; Input:
;	a	pointer to string of sufficient length,
;		17 is always sufficient
;	base	radix, between 2 and 36 inclusive
; Returns:
;	a

func	itoa
	push	BP
	mov	BP,SP
	sub	SP,18			;char buf[18]
	push	SI
	push	DI

    if SPTR
	push	DS
	pop	ES
    endif

    ife SSeqDS
	push	DS
	push	SS
	pop	DS
    endif

	mov	DI,P+2+SIZEPTR[BP]	;DI = base

	mov	BX,1			;assume negative
	mov	CX,P[BP]		;CX = i
	lea	SI,-1[BP]		;SI = &buf[sizeof(buf)-1] ;last char pos
	mov	[SI],BH			;*SI = 0

	;if (base == 10 && (int) u < 0)
	cmp	DI,10
	jne	L1
	tst	CX
	jns	L1
	dec	BX
	neg	CX

L1:	dec	SI
	mov	AX,CX
	xor	DX,DX
	div	DI
	mov	CX,AX			;u /= base
	add	DL,'0'			;DL = (u % base) + '0';

	_if	DL le '9', L50
	add	DL,7			;DL += 'A'-'0'-10;
L50:	mov	[SI],DL
	or	AX,AX
	jne	L1

	dec	SI
	mov	byte ptr [SI],'-'
	add	SI,BX			;p += sign;

	;return memcpy(a,p,&buf[sizeof(buf)] - p);
	mov	CX,BP
	sub	CX,SI
    if SPTR
	mov	DI,P+2[BP]		;ES already points to DS
	mov	AX,DI			;return a
    else
	les	DI,P+2[BP]
	mov	AX,DI
	mov	DX,ES
    endif
	rep	movsb			;DS:SI -> ES:DI

    ife SSeqDS
	pop	DS
    endif
	pop	DI
	pop	SI
	mov	SP,BP
	pop	BP
	ret
c_endp	itoa

	endcode	itoa

	end
