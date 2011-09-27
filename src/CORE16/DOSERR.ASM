;_ doserr.asm   Tue Sep  5 1989   Modified by: Walter Bright */
; Windows support added by G. Eric Engstrom Dec 1990	-D_WINDOWS
; Modified by G. Eric Engstrom (Optimization) Jan 1991
;Copyright (C) 1989 by Walter Bright
;All rights reserved.
;Written by Dave Mansell

include	macros.asm

IFDEF __OS2__
	extrn DOSERRCLASS:far

    ifdef _MT
    if LCODE
	extrn	__errno:far
    else
	extrn	__errno:near
    endif
    else
	begdata
	extrn	_errno:word
	enddata
    endif

ENDIF

DOSERROR STRUC
	exterr	dw	?
	eclass	db	?
	action	db	?
	locus	db	?
DOSERROR ENDS

	begcode	doserr

	public _dosexterr
_dosexterr:

	c_public dos_exterr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get dos extended error information
; OS/2 version assumes _errno contains
; the OS/2 error code

func	dos_exterr
	WINENTER
	push	DI
ifdef __OS2__
    ifdef _MT
	call	__errno
	mov	BX,AX
      if SPTR
	mov	AX,[BX]
      else
	mov	ES,DX
	mov	AX,ES:[BX]
      endif
    else
	mov	AX,errno
    endif
	push	AX
	sub	SP,6
	mov	BX,SP
	mov	CX,BX
	add	CX,2
	mov	DX,CX
	add	CX,2
	push	AX		;errno
	push	SS
	push	BX		;receives error class
	push	SS
	push	CX		;receives recommended action
	push	SS
	push	DX		;receives error locus
	call	DOSERRCLASS
	pop	BX		;class
	pop	DX		;locus
	pop	CX		;action
	pop	AX		;_errno
ELSE	
	clr	BX
	bdos	59h
ENDIF
    if SPTR
	mov	DI,P[BP]	;where to store the information
	tst	DI		;passed a null pointer?
	jz	a1
    else
	push	DS
	lds	DI,P[BP]	;where to store the information
	mov	DX,DS
	or	DX,DI
	je	a1		;passed a null pointer?
    endif
IFDEF __OS2__
	mov	exterr[DI],AX
	mov	eclass[DI],BL
	mov	action[DI],CL
	mov	locus[DI],DL
ELSE
	mov	exterr[DI],AX
        xchg	BH,BL			;swap cause order is wrong to
        mov	word ptr eclass[DI],BX	; save as word
	mov	locus[DI],CH
ENDIF
a1:
    if LPTR
	pop	DS
    endif
	pop	DI
        WINLEAVE
	ret
c_endp	dos_exterr

	endcode	doserr

	end

