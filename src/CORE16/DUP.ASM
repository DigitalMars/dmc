;_ dup.asm   Fri Jun  1 1990   Modified by: Walter Bright
; Windows supported added by G. Eric Engstrom Jan 1991	-D_WINDOWS
; Modified by Bob Stout May 1990
; Copyright (C) 1985-1991 Walter Bright
; All Rights Reserved
; Written by Walter Bright

include	macros.asm
include stdio.asm

if LCODE
extrn __SET_DOS_ERRNO:far
else
extrn __SET_DOS_ERRNO:near
endif

    ifdef __OS2__
    	extrn	DOSDUPHANDLE:far
    endif

	begcode	dup

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	int dup(int fd);
; Returns:
;	handle	success
;	-1	error (errno is set)

	public	_dup
    if LCODE
_dup	proc	far
    else
_dup	proc	near
    endif
    ifdef __OS2__
	push	BP
	mov	BP,SP
    	mov	AX,-1			;set handle = -1
	push	AX			;space for handle on stack
	mov	AX,SP			;offset of dup handle
	push	P[BP]			;handle to dup
    	push	SS			;push dup handle segment
	push	AX
	call	DOSDUPHANDLE
	tst	AX			;error?
	pop	AX			;return new handle
D10:	jz	D11			;no, continue
	errno_set			;yes, save in errno...
	mov	AX,-1			; and return -1
D11:	pop	BP
    else ;if DOS
    	WINENTER
	mov	BX,P[BP]
	bdos	045h
	jnc	D1			;no error
D2:	push    AX			;save DOS error number
        call    __SET_DOS_ERRNO
	mov     AX,-1
D1:	WINLEAVE
    endif
	ret
_dup	endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	int dup2(int fd1,int fd2);
; Returns:
;	0	success
;	-1	error (errno is set)

	c_public	dup2
func	dup2
    ifdef __OS2__
	push	BP
	mov	BP,SP
	push	P[BP]			;handle to dup
    	push	SS			;push dup handle segment
	lea	AX,P+2[BP]
	push	AX			;push dup handle offset
	call	DOSDUPHANDLE
	tst	AX			;error?
	jmp	D10
    else ;if DOS
    	WINENTER
	mov	BX,P[BP]
	mov	CX,P+2[BP]
	bdos	046h
	jc	D2			;error
	mov     AX,0
	jmp	D1			;success
    endif
c_endp	dup2

	endcode	dup

	end
