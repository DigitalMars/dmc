;Copyright (C) 1985-1993 by Digital Mars
;All Rights Reserved
;Modified by Richard S. Sadowsky July, 1993
; $Revision: 1.1.1.1 $

include macros.asm
include stdio.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Returns country dependent information.
; Use:
;	struct COUNTRY *country(int code, struct country *cc)
; Returns:
;		if successful, pointer cc
;		if an error, NULL

	begcode country
	c_public	country
func	country
	push	EBP
	mov	EBP,ESP
	uses	<EBX,EDX>
	mov	EDX,P+4[EBP]
	clr	EAX
	mov	AX,WORD PTR P[EBP]
	cmp	EAX,0FFh
	jb	L1
	clr	EBX,EBX
	mov	BX,AX
	mov	AL,0FFh
L1:
	bdos	38h
	mov	EAX,EDX
	jnc	done
	xor	EAX,EAX
done:	unuse	<EDX,EBX>
	pop	EBP
	ret

c_endp	country
	endcode country

	end
