;_ kbhit.asm	Mon Jun 6 1994
; Copyright (C) 1994 Digital Mars
; All rights reserved.
; $Revision: 1.1.1.1 $

include	macros.asm
include stdio.asm

     ifdef __OS2__
	extrn	KBDPEEK:far
	extrn   extchar:byte
	extrn   CharData:byte
	extrn   Status:byte
    endif

        extrn   ch_is_waiting:byte
	extrn   kb_callback:dword



	begcode	kbhit
	c_public kbhit

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Return 1 if key is available from console, else 0

func	kbhit
    ifdef __OS2__
        cmp	ch_is_waiting,0
        jnz	kbhok
	cmp	extchar,0
	jnz	kbhok
	push	DS
	pushi	AX,<offset DGROUP:CharData>
	pushi	AX,0		;KbdHandle
	call	KBDPEEK
	test	AX,AX
	mov	AX,0
	jnz	kbhret		;error peeking keyboard
	mov	BL,Status
	and	BL,011000000b	;bits 6 or 7 set if character returned
	jz	kbhret
kbhok:
	mov	AX,1
kbhret:
	ret
    else
	WINENTER_NF
        cmp	ch_is_waiting,0
        jnz	kbhok
	mov	ax,WORD PTR kb_callback
	or	ax,WORD PTR kb_callback+2
	jz	kbh1
	mov	ax,0	;to indicate kbhit
	push	ax
	call	dword ptr [kb_callback]
	pop	cx
	jmp	kbhret
kbh1:	bdos	0Bh	;check standard input status
	cbw		;clear AX is now 0 or -1
	neg	AX	;0 or 1
	jmp	kbhret
kbhok:
	mov	AX,1
kbhret:
	WINLEAVE_NF
	ret
    endif
c_endp	kbhit

	endcode kbhit

	end
