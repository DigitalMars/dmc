;_ memset.asm
;Copyright (C) 1985-1995 Digital Mars
;All Rights Reserved

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set n bytes in s to c.
;	char *memset(char *p,int c,int n)
; Returns:
;	p

	begcode	strings
	c_public	memset

	if LPTR and LCODE
	public _fmemset
_fmemset:
	endif	

func	memset
	push	BP
	mov	BP,SP

	.save	<DI>
	mov	CX,P+SIZEPTR+2[BP]	;n

    if SPTR
      ife ESeqDS
	mov	DX,DS
	mov	ES,DX
      endif
	mov	DI,P[BP]	;p
    else
	les	DI,P[BP]	;p
    endif

	mov	AL,P+SIZEPTR[BP]	;c
	mov	BX,DI		;Return value.

	cmp	cx,10		;handle small number faster
	jb	Small

	test	bl,1		;make sure word aligned

	jz	Aligned		;this overhead worthwhile >=10 bytes (on Pentium)

	stosb

	dec	cx
Aligned:
	shr	CX,1
	mov	AH,AL		;Set up a 16 bit pattern.

	rep	stosw

	jnc	Done

	stosb
Done:
    if LPTR
	mov	DX,ES		;return original value of p in DX,AX
    endif
	mov	AX,BX		;Return value.
	.restore <DI>
	pop	BP
	ret

Small:
	rep	stosb

	jmp	Done

c_endp	memset
	endcode	strings
	end


