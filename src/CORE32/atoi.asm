;_ atoi.asm  
; Copyright (C) 1985-2001 by Digital Mars
; All Rights Reserved
; www.digitalmars.com
; Written by Walter Bright

	include	macros.asm

; 64 bit negate
neg64	macro	reg1,reg2
	 neg	reg1
	 neg	reg2
	 sbb	reg1,0
	endm

	begcode	atoi

;;;;;;;;;;;;;;;;;;;;;;;;;
; Take absolute value of integer.

	public  _abs
;	c_public abs   ;Phar Lap's 386asm doesn't like this for some reason.

func	abs
	;Fall through to labs
c_endp	abs

;;;;;;;;;;;;;;;;;;;;;;;;;
; Take absolute value of long.

	c_public labs
func	labs
	mov	EAX,PS[ESP]
	cdq
	xor	EAX,EDX
	sub	EAX,EDX
	_ret	4
c_endp	labs

	c_public llabs
func	llabs
	mov	EAX,PS[ESP]
	mov	EDX,PS+4[ESP]
	test	EDX,EDX
	jns	L2
	neg64	EDX,EAX
L2:	_ret	8
c_endp	llabs

;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert ascii string to long/integer.
; Use:
;	i = atoi(p);
;	i = atol(p);

	_align

	c_public atoi
func	atoi
	uses	<EBX,ECX,ESI>
    if SPTR
	mov	ESI,PS+8[ESP]	;get p
    else
	push	DS
	lds	ESI,PS+12[ESP]	;DS:ESI -> string
    endif
	cld
A1:	lodsb
	_ifs	AL e ' ', A1
	_ifs	AL b 9, A3
	_ifs	AL be 0Dh, A1	;skip white space

A3:	clr	ECX		;accumulated result
	mov	EBX,ECX		;assume positive
	movzx	EAX,AL
	_if	AL e '+', A4
	_if	AL ne '-', A5
	dec	EBX		;neg flag (BX = -1)
	_align
A4:	lodsb
A5:	sub	AL,'0'		;to binary
	jb	A2		;not a digit
	_ifs	AL ae 10, A2	;not a digit

	;ECX = ECX * 10 + EAX
	lea	ECX,[ECX+ECX*4]
	add	ECX,ECX
	add	ECX,EAX

	jmp	A4

A2:	mov	EAX,ECX
	xor	EAX,EBX
	sub	EAX,EBX		;if (BX == -1), negate AX
    if LPTR
	pop	DS
    endif
	unuse   <ESI,ECX,EBX>
	_ret	SIZEPTR
c_endp	atoi

	endcode	atoi

	end
