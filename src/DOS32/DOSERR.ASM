;_ doserr.asm   Tue Sep  5 1989   Modified by: Walter Bright */
; $Header: z:/core/crtl/src/dos32/doserr.asm,v 1.1.1.1 1997/01/02 19:17:56 smarx Exp $
;Copyright (C) 1989-1990 by Walter Bright
;All rights reserved.
;Written by Dave Mansell

include	macros.asm


DOSERROR STRUC
	exterr	dd	?
	eclass	db	?
	action	db	?
	locus	db	?
DOSERROR ENDS

	begcode	doserr

	public _dosexterr
_dosexterr:

	c_public dos_exterr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; get dos extended error information

func	dos_exterr
	push	EBP
	mov	EBP,ESP
	uses	<EBX,EDI>
	clr	EBX
	bdos	59h
    if SPTR
	mov	EDI,P[EBP]	;where to store the information
	tst	EDI		;passed a null pointer?
	jz	a1
    else
	push	DS
	lds	EDI,P[EBP]	;where to store the information
	xor	EDX,EDX
	mov	DX,DS
	or	EDX,EDI
	je	a1		;passed a null pointer?
    endif
	movzx	EAX,AX
	mov	exterr[EDI],EAX
	mov	eclass[EDI],BH
	mov	action[EDI],BL
	mov	locus[EDI],CH
a1:
    if LPTR
	pop	DS
    endif
	unuse	<EDI,EBX>
	pop	EBP
	ret
c_endp	dos_exterr

	endcode	doserr

	end

