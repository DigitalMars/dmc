;_ memcpy.asm
;Copyright (C) 1985-1995 Digital Mars
;All Rights Reserved

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copy n bytes from p2 to p1.
;	void *memcpy(void *p1,void *p2,n)
; Returns:
;	p1

	c_public	memcpy
	begcode	strings

	c_public	memmove		;alternate entry point
func	memmove
c_endp	memmove

    if LPTR and LCODE
	public	__fmemcpy,_fmemcpy
	public	_fmemmove
__fmemcpy:
_fmemcpy:	
_fmemmove:
    endif

func	memcpy
	push	BP
	mov	BP,SP

	.save	<SI,DI>

	mov	CX,P + 2 * SIZEPTR[BP]	;n

    if SPTR
      ife ESeqDS
	mov	AX,DS
	mov	ES,AX
      endif
	mov	DI,P[BP]		;p1
	mov	SI,P + SIZEPTR[BP]	;p2
	mov	ax,di			;return value
    else
	push	DS			;Save DS.
	les	DI,P + [BP]		;p1
	lds	SI,P + SIZEPTR[BP]	;p2
	mov	ax,di
    endif

	test	cx,cx
	jz	memcpy_done
    if LPTR
	mov	bx,ds
	mov	dx,es

	cmp	bx,dx
	jnz	memcpy_forward		;use forward if segments different
    endif
	cmp	SI,DI
	jz	memcpy_done

	mov	bx,si
	ja	memcpy_forward		;Jmp if forward copy.

	add	bx,cx

	cmp	bx,di
	ja	memcpy_backward
memcpy_forward:
	mov	bx,si		;align if BOTH are odd

	and	bx,di

	and	bl,1
	jz	Aligned

	movsb

	dec	cx
	jz	memcpy_done
Aligned:
	shr	CX,1
	rep	movsw

	jnc	memcpy_done

	movsb

memcpy_done:
    if LPTR
	pop	DS			;Restore DS
	mov	DX,ES			;Segment of return value.
      if ESeqDS
	mov	bx,ds
	mov	ES,BX
      endif
    endif
	.restore <DI,SI>
	pop	bp
	ret

memcpy_backward:
	add	si,cx
	add	di,cx

	sub	si,2
	sub	di,2

	mov	bx,si		;align if BOTH are odd

	and	bx,di

	and	bl,1
	jz	BackAligned

	mov	bl,[si+1]
	dec	si

	mov	es:[di+1],bl
	dec	di

	dec	cx
	jz	memcpy_done
BackAligned:
	shr	CX,1
	jz	DoBackByte

	std

	rep	movsw

	cld

	jnc	memcpy_done
DoBackByte:
	mov	bl,[si+1]

	mov	es:[di+1],bl
	jmp	memcpy_done

c_endp	memcpy
	endcode	strings
	end



