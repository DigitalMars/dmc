;_ llmath.asm
; Copyright (C) 1993-2008 by Digital Mars
; All Rights Reserved
; http://www.digitalmars.com
; Written by Walter Bright

include	macros.asm

; 64 bit negate
neg64	macro	reg1,reg2
	 neg	reg1
	 neg	reg2
	 sbb	reg1,0
	endm


	begcode	llmath

	public	__LMUL@,__LDIV@,__ULDIV@,__LCMP@

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Multiply:
;	EDX,EAX = EDX,EAX * ECX,EBX
; ECX is destroyed

__LMUL@	proc	near
    if 0
	push	ESI
	mov	ESI,EDX		;ESI = d
	xchg	EAX,ECX		;EAX = c, ECX = a
	tst	EAX
	jz	short M1
	mul	ECX		;EAX = acl
M1:	xchg	EAX,ESI		;ESI = acl, EAX = d
	tst	EAX
	jz	short M2
	mul	EBX		;EAX = bdl
	add	ESI,EAX		;ESI = acl + bdl
M2:	mov	EAX,ECX		;EAX = a
	mul	EBX		;EAX = abl, EDX = abh
	add	EDX,ESI		;EAX = abl, EDX = abh + acl + bdl
	pop	ESI
	ret
    else
	tst	ECX
	jnz	short M1
	tst	EDX
	jnz	short M2
	mul	EBX
	ret

	even
M1:	tst	EDX
	jnz	short M3
	;EDX is 0
	xchg	EAX,ECX
	mul	ECX
	xchg	EAX,ECX
	mul	EBX
	add	EDX,ECX
	ret

	even
M2:	;ECX is 0
	mov	ECX,EAX
	mov	EAX,EDX
	mul	EBX
	xchg	EAX,ECX
	mul	EBX
	add	EDX,ECX
	ret

	even
M3:	push	ESI
	mov	ESI,EDX		;ESI = d
	xchg	EAX,ECX		;EAX = c, ECX = a
	mul	ECX		;EAX = acl
	xchg	EAX,ESI		;ESI = acl, EAX = d
	mul	EBX		;EAX = bdl
	add	ESI,EAX		;ESI = acl + bdl
	mov	EAX,ECX		;EAX = a
	mul	EBX		;EAX = abl, EDX = abh
	add	EDX,ESI		;EAX = abl, EDX = abh + acl + bdl
	pop	ESI
	ret
    endif
__LMUL@	endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Unsigned long divide.
; Input:
;	[EDX,EAX],[ECX,EBX]
; Output:
;	[EDX,EAX] = [EDX,EAX] / [ECX,EBX]
;	[ECX,EBX] = [EDX,EAX] % [ECX,EBX]
;	ESI,EDI	destroyed

	even

__ULDIV@ proc	near
	test	ECX,ECX		;
	jz	uldiv		;

	; if ECX > EDX, then quotient is 0 and remainder is [EDX,EAX]
	cmp	ECX,EDX		;
	ja	quo0		;

	test	ECX,ECX		;
	js	Lleft		;

	; We have n>d, and know that n/d will fit in 32 bits.
	; d will be left justified if we shift it left s bits.
	; [d1,d0] <<= s
	; [n2,n1,n0] = [n1,n0] << s
	;
	; Use one divide, by this reasoning:
	; ([n2,n1]<<32 + n0)/(d1<<32 + d0)
	; becomes:
	; ([n2,n1]<<32)/(d1<<32 + d0) + n0/(d1<<32 + d0)
	; The second divide is always 0.
	; Ignore the d0 in the first divide, which will yield a quotient
	; that might be too high by 1 (because d1 is left justified).
	; We can tell if it's too big if:
	;  q*[d1,d0] > [n2,n1,n0]
	; which is:
	;  q*[d1,d0] > [[q*[d1,0]+q%[d1,0],n1,n0]
	; If we subtract q*[d1,0] from both sides, we get:
	;  q*d0 > [[n2,n1]%d1,n0]
	; So if it is too big by one, reduce q by one to q'=q-one.
	; Compute remainder as:
	;  r = ([n1,n0] - q'*[d1,d0]) >> s
	; Again, we can subtract q*[d1,0]:
	;  r = ([n1,n0] - q*[d1,0] - (q'*[d1,d0] - q*[d1,0])) >> s
	;  r = ([[n2,n1]%d1,n0] + (q*[d1,0] - (q - one)*[d1,d0])) >> s
	;  r = ([[n2,n1]%d1,n0] + (q*[d1,0] - [d1 *(q-one),d0*(1-q)])) >> s
	;  r = ([[n2,n1]%d1,n0] + [d1 *one,d0*(one-q)])) >> s

	push	EBP		;
	push	ESI		;
	push	EDI		;

	mov	ESI,EDX		;
	mov	EDI,EAX		;
	mov	EBP,ECX		;

	bsr	EAX,ECX		;	; EAX is now 30..0
	xor	EAX,01Fh	;	; EAX is now 1..31
	mov	CH,AL		;
	neg	EAX		;
	add	EAX,32		;
	mov	CL,AL		;

	mov	EAX,EBX		;
	shr	EAX,CL		;
	xchg	CH,CL		;
	shl	EBP,CL		;
	or	EBP,EAX		;
	shl	EBX,CL		;

	mov	EDX,ESI		;
	xchg	CH,CL		;
	shr	EDX,CL		;

	mov	EAX,EDI		;
	shr	EAX,CL		;
	xchg	CH,CL		;
	shl	EDI,CL		;
	shl	ESI,CL		;
	or	EAX,ESI		;

	div	EBP		;
	push	EBP		;
	mov	EBP,EAX		;
	mov	ESI,EDX		;

	mul	EBX		;
	cmp	EDX,ESI		;
	ja	L1		;
	jb	L2		;
	cmp	EAX,EDI		;
	jbe	L2		;
L1:	dec	EBP		;
	sub	EAX,EBX		;
	sbb	EDX,0[ESP]	;
L2:
	add	ESP,4		;
	sub	EDI,EAX		;
	sbb	ESI,EDX		;
	mov	EAX,ESI		;
	xchg	CH,CL		;
	shl	EAX,CL		;
	xchg	CH,CL		;
	shr	EDI,CL		;
	or	EDI,EAX		;
	shr	ESI,CL		;
	mov	EBX,EDI		;
	mov	ECX,ESI		;
	mov	EAX,EBP		;
	xor	EDX,EDX		;

	pop	EDI		;
	pop	ESI		;
	pop	EBP		;
	ret			;

uldiv:	test	EDX,EDX		;
	jnz	D3		;
	; Both high words are 0, we can use the DIV instruction
	div	EBX		;
	mov	EBX,EDX		;
	mov	EDX,ECX		;	; EDX = ECX = 0
	ret			;

	even			;
D3:	; Divide [EDX,EAX] by EBX
	mov	ECX,EAX		;
	mov	EAX,EDX		;
	xor	EDX,EDX		;
	div	EBX		;
	xchg	ECX,EAX		;
	div	EBX		;
	; ECX,EAX = result
	; EDX = remainder
	mov	EBX,EDX		;
	mov	EDX,ECX		;
	xor	ECX,ECX		;
	ret			;

quo0:	; Quotient is 0
	; Remainder is [EDX,EAX]
	mov	EBX,EAX		;
	mov	ECX,EDX		;
	xor	EAX,EAX		;
	xor	EDX,EDX		;
	ret			;

Lleft:	; The quotient is 0 or 1 and EDX >= ECX
	cmp	EDX,ECX		;
	ja	quo1		;	; [EDX,EAX] > [ECX,EBX]
	; EDX == ECX
	cmp	EAX,EBX		;
	jb	quo0		;

quo1:	; Quotient is 1
	; Remainder is [EDX,EAX] - [ECX,EBX]
	sub	EAX,EBX		;
	sbb	EDX,ECX		;
	mov	EBX,EAX		;
	mov	ECX,EDX		;
	mov	EAX,1		;
	xor	EDX,EDX		;
	ret			;
__ULDIV@ endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Signed long divide.
; Input:
;	[EDX,EAX],[ECX,EBX]
; Output:
;	[EDX,EAX] = [EDX,EAX] / [ECX,EBX]
;	[ECX,EBX] = [EDX,EAX] % [ECX,EBX]
;	ESI,EDI	destroyed

	even

__LDIV@	proc	near
	tst	EDX			;[EDX,EAX] negative?
	jns	short L10		;no
	neg64	EDX,EAX			;[EDX,EAX] = -[EDX,EAX]
	tst	ECX			;[ECX,EBX] negative?
	jns	short L11		;no
	neg64	ECX,EBX
	call	__ULDIV@
	neg64	ECX,EBX			;remainder same sign as dividend
	ret

L11:	call	__ULDIV@
	neg64	ECX,EBX			;remainder same sign as dividend
	neg64	EDX,EAX			;quotient is negative
	ret

L10:	tst	ECX			;[ECX,EBX] negative?
	jns	L12			;no (all is positive)
	neg64	ECX,EBX
	call	__ULDIV@
	neg64	EDX,EAX			;quotient is negative
	ret

L12:	jmp	__ULDIV@
__LDIV@	endp


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compare [EDX,EAX] with [ECX,EBX]
; Signed

	even
__LCMP@	proc	near
	cmp	EDX,ECX
	jne	short C1
	push	EDX
	clr	EDX
	cmp	EAX,EBX
	jz	short C2
	ja	short C3
	dec	EDX
	pop	EDX
	ret

C3:	inc	EDX
C2:	pop	EDX
C1:	ret
__LCMP@	endp

	endcode	lmath

	end
