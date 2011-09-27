;_ memicmp.asm	
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

;CAUTION!  This code is highly optimized.  It does not fully utilize the
;macros 'uses' and 'unuse' for register preservation.  If registers other than
;EBX, ESI, and EDI (and of course EBP,DS, and ES) must be preserved examine 
;this module carefully.

include macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compare strings just like memcmp(), except that case is ignored.

	begcode memicmp
	public	_memicmp
_memicmp	proc	near
	push	ebp
	mov	ebp,esp
	uses	<ECX,EDX,ESI,EDI>
	mov	esi,P[ebp]
	mov	edi,P+SIZEPTR[ebp]
	mov	ecx,P+SIZEPTR+SIZEPTR[ebp] ;ECX = numbytes
	jecxz	short match

ife ESeqDS
	mov	ax,ds
	mov	es,ax
endif

	mov	ah,'A'
	mov	dh,'Z'

	_align
L1:	lodsb				;AL = *p1++
	mov	dl,[edi]
	inc	edi			;DL = *p2++
	_if	al ne dl, L2		;strings are different at this char
	loop	L1			;try next char
	jmps	match

L2:	;Perhaps when converted to lower case, they will be the same
	_ifs	al b ah, nomatch
	_ifs	al a dh, L4
	add	al,'a' - 'A'		;convert AL to lower case
L4:	_ifs	dl b ah, nomatch
	_ifs	dl a dh, L5
	add	dl,'a' - 'A'		;convert DL to lower case
L5:	_ifs	al ne dl, nomatch	;different, we're done
	loop	L1
match:	mov	eax,ecx			;return 0 in eax
	unuse	<edi,esi,edx,ecx>
	pop	ebp
	_ret	12

nomatch:
	movzx	edx,dl
	movzx	eax,al
	sub	eax,edx
	unuse	<edi,esi,edx,ecx>
	pop	ebp
	_ret	12
_memicmp	endp
	endcode memicmp
	end

