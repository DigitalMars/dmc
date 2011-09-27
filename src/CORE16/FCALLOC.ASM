;_ fcalloc.asm Aug 10 1993
; Copyright (C) 1985-1991 by Walter Bright
; All rights reserved
; Written by Walter Bright
; $Revision: 1.1.1.1 $

include	macros.asm

    if LCODE
	c_extrn	_fmalloc,far,dos_calloc,far,farcalloc,far
    else
	c_extrn	_fmalloc,near,dos_calloc,near,farcalloc,near
    endif

	begcode	fcalloc
	c_public _fcalloc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Allocate a block of data and clear it.
; Use:
;	p = calloc(numelems,sizeof(elem));
; Returns:
;	pointer to allocated data else NULL

	even
  if SPTR
    ifdef _WINDOWS
func	_fcalloc
	WINENTER	
	mov	AX,P[BP]
	mul	word ptr P+2[BP]
	add	AX,0Fh
	adc	DX,0
	sar	DX,1
	rcr	AX,1
	sar	DX,1
	rcr	AX,1
	sar	DX,1
	rcr	AX,1
	sar	DX,1
	rcr	AX,1
	mov	CX,DX
	or	CX,AX
	je	C1
	cmp	DX,0
	je	C2
C1:	xor	AX,AX
	cwd
	mov	SP,BP
	WINLEAVE
	ret
C2:	push	AX
	call	_dos_calloc
	pop	CX
	mov	DX,AX
	xor	AX,AX
	WINLEAVE
	ret
c_endp	_fcalloc    
    else
func	_fcalloc
	WINENTER
	mov	AX,P+2[BP]
	xor	CX,CX
	push	CX
	push	AX
	mov	AX,P[BP]
	xor	CX,CX
	push	CX
	push	AX
	call	_farcalloc
	add	SP,8
	WINLEAVE
	ret
c_endp	_fcalloc    
    endif
  else
func	_fcalloc
	WINENTER
	mov	BX,P+2[BP]	;get sizeof(elem)
	mov	AX,P[BP]	;get numelems
	_if	BX e 1, C1	;no need to multiply
	mul	BX
	jc	C3		;if overflow
C1:	push	AX		;nbytes
	push	AX
	callm	_fmalloc
	pop     BX		;retrieve nbytes
	mov	SP,BP
	tst	DX
	jz	C2		;yes
	.save	<DI>
	mov	ES,DX
	mov	DI,AX
	mov	CX,BX		;get nbytes
	mov	BX,AX
    ifdef DOS16RM
	jne	pp1		;if not 64k bytes
	mov	CH,080h		;32k words
pp1:
    endif
;	dec	CX		;skip # of bytes
	clr	AX
	shr	CX,1		;get number of words
	cld
	rep	stosw		;clear the memory
	jnc	C4
	stosb	
C4:
	mov	AX,BX		;DX:AX is pointer to result
	.restore <DI>
C2:	WINLEAVE
	ret

C3:	clr	AX
	cwd
	WINLEAVE
	ret
c_endp	_fcalloc
  endif
	endcode	fcalloc
	end
