;_ dup.asm   Fri Jun  1 1990   Modified by: Walter Bright */
; Modified 28 May 90 by Bob Stout

include	macros.asm
include stdio.asm

extrn __SET_DOS_ERRNO:near

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
	push	EBP
	mov	EBP,ESP
	push	EBX
	mov	EBX,P[EBP]
	bdos	045h
	jnc	short D1		;no error
D2:	push    EAX
        call    __SET_DOS_ERRNO
        mov     EAX,-1
        jmp     D1
D3:	mov	EAX,0
D1:	pop	EBX
	pop	EBP
	ret
c_endp	dup

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	int dup2(int fd1,int fd2);
; Returns:
;	0	success
;	-1	error (errno is set)

	c_public	dup2
func	dup2
	push	EBP
	mov	EBP,ESP
	push	EBX
	mov	EBX,P[EBP]
	mov	ECX,P+4[EBP]
	bdos	046h
	jc	D2			;error
	jmp	D3			;success
c_endp	dup2

	endcode	dup

	end
