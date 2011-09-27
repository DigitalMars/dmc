;Copyright (C) 1985-1993 by Digital Mars Corp.
;All Rights Reserved
;Modified by Richard S. Sadowsky July, 1993
;$Revision: 1.1.1.1 $

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

     ifdef __OS2__
; not yet supported
     else
	push	BP
	mov	BP,SP
    if SPTR
	mov	AX,P[BP]
	cmp	AX,255			;use 16 bit country code if >= 255
	jb	L1
	mov	BX,AX
L1:
	mov	DX,P+2[BP]
	bdos	38h
    else
	push	DS
	mov	AX,P[BP]
	cmp	AX,0FFh			;use 16 bit country code if >= 255
	jb	L2
	mov	AL,0FFh
	mov	BX,AX
L2:
    if LPTR
	cmp	word ptr P+2[BP],-1	;don't set seg if ofs=FFFF
	jne	L3
	
	xor	DX,DX
	push	DX				;save 0 for segment for return
	mov	DX,P+2[BP]
	jmp	short L4
L3:
	lds	DX,P+2[BP]
	push	DS				;save segment for return
L4:
    else
	mov	DX,P+2[BP]
    
    endif
next:	bdos	38h
 
    if LPTR
	pop	DX
    endif
	
	pop	DS
    endif
	mov	AX,DX
	jnc	done
	xor	AX,AX
done:
	pop	BP
	ret
     endif

c_endp	country
	endcode country
	end
