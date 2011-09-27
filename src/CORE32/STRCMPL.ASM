;_ strcmpl.asm	 
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

;CAUTION!  This code is highly optimized.  It does not fully utilize the
;macros 'uses' and 'unuse' for register preservation.  If registers other than
;EBX, ESI, and EDI (and of course EBP,DS, and ES) must be preserved examine 
;this module carefully.

include macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compare strings just like strcmp(), except that case is ignored.

	begcode strcmpl

	public	_stricmp
_stricmp:

	public	_strcmpl
_strcmpl	proc	near
	push	ebp
	mov	ebp,esp
	mov	edx,esi
	uses	<ECX,EDI>
	mov	esi,P[ebp]
	mov	edi,P+SIZEPTR[ebp]

ife ESeqDS
	mov	ax,ds
	mov	es,ax
endif

	_align
L1:	lodsb				;AL = *p1++
	mov	cl,[edi]
	inc	edi			;CL = *p2++
	_if	AL ne CL, L2		;strings are different at this char
	tst	AL			;end of string?
	jnz	short L1		;no
	jmps	L3

L2:	;Perhaps when converted to lower case, they will be the same
	_ifs	al b 'A', L3
	_ifs	al a 'Z', L4		;Already lower case?
	add	al,'a' - 'A'		;convert AL to lower case
L4:	_ifs	cl b 'A', L3
	_ifs	cl a 'Z', L5		;Already lower case?
	add	cl,'a' - 'A'		;convert CL to lower case
L5:	_ifs	al e cl, L1		;same now, so continue looping

L3:	clr	ah
	mov	ch,ah
	movzx	eax,ax
	movzx	ecx,cx
	sub	eax,ecx

	unuse	<edi,ecx>
	mov	esi,edx
	pop	ebp
	_ret	8
_strcmpl	endp
	endcode strcmpl
	end

