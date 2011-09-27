;_ _8087.asm   Modified by: Walter Bright */
; Copyright (C) 1991-2002 by Digital Mars
; All rights reserved.
; www.digitalmars.com
; Written by Walter Bright

; 8087 detection and initialization code for 32-bit OS/2 and NT
; This code is linked in if _8087 is referenced.

	.386		;disable automatic FWAIT generation


include	macros.asm

    ifdef __OS2__
	extrn	DosDevConfig:near
    endif

	public	__80x87,__8087

	begdata
__80x87	label	dword			;alias for __8087
__8087	dd	0			;2 means 287, 3 means 387
	enddata

	begcode	_8087

DEVINFO_COPROCESSOR	equ	3

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Determine if we have and which version of 8087 we have installed.

	public	__8087_init
__8087_init proc near

    ifdef __OS2__

	; test for presence of 80x87
	push	DEVINFO_COPROCESSOR		;get coprocessor info
	push	offset FLAT:__8087		;where to put returned 8087 info
	call	DosDevConfig
	add	ESP,8
	test	EAX,EAX				;error?

	jnz	short L10			;yes, use our own test for 8087
	_if	<byte ptr __8087> e 0, L15	;coprocessor is not present, return
    endif

L10:
	fninit					;initialize 8087 (no WAIT)
	mov	ECX,30
L1:	loop	L1				;wait for it to complete
	fnstcw	word ptr __8087			;store control word
	mov	ECX,30
L2:	loop	L2				;wait for it to complete

	;Determine if we have an 8087, 80287, or 80387
	xor	EDX,EDX			;assume no NDP
	mov	EAX,__8087
	and	AH,0Fh			;look at rounding control & precision
	_ifs	AH ne 3, L6		;if no 80x87
	fstcw	word ptr __8087
	inc	EDX			;could be 8087, 80287 or 80387
	fwait
	and	byte ptr __8087,07Fh	;turn off interrupt mask bit
	fldcw	word ptr __8087
	fdisi				;disable interrupts (works on 8087 only)
	fstcw	word ptr __8087
	fwait
	test	byte ptr __8087,80h	;see if bit is back on
	jnz	short L6		;yes, then 8087
	inc	EDX			;287 or 387
	or	byte ptr __8087,0BFh	;disable interrupts, mask exceptions
	and	word ptr __8087,0EFFFh	;turn off infinity bit
	fldcw	word ptr __8087
	fld1
	fldz
	fdivp	ST(1),ST	;divide by 0 to get infinity
	fld	ST
	fchs			;create +infinity and -infinity
	fcompp			;and see if they're the same
	fstsw	AX
	or	__8087,01000h	;turn on infinity bit
	fldcw	word ptr __8087
	sahf
	jz	short L3	;equal, so it's a 287
	inc	EDX		;EDX = 3 for 80387
L3:	fclex			;clear error bits caused by divide-by-0
L6:	mov	__8087,EDX	;set flag
L15:
	ret
__8087_init endp

	endcode	_8087

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Setup function pointers for initialization as
; static constructors.

	static_ctor	__8087_init


	end
