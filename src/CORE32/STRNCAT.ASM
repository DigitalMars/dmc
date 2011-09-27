;_ strncat.asm	 
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

;CAUTION!  This code is highly optimized.  It does not fully utilize the
;macros 'uses' and 'unuse' for register preservation.  If registers other than
;EBX, ESI, and EDI (and of course EBP,DS, and ES) must be preserved examine 
;this module carefully.

include macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Cat s2 to s1 till a zero byte or n bytes are copied.
; Use:
;	char *strncat(char *s1,char *s2,unsigned n)
; Returns:
;	s1
;
	begcode strncat
	public	_strncat
_strncat	proc	near
	push	ebp
	mov	ebp,esp
	uses	<ECX,EDX,ESI,EDI>

	mov	esi,P+SIZEPTR[ebp]	;get source pointer (s2)
	mov	edi,P[ebp]		;get destination pointer (s1)

ife ESeqDS
	mov	ax,ds
	mov	es,ax
endif

	clr	eax			;scan for 0
	mov	ecx,eax
	dec	ecx			;largest possible string (ECX = -1)
	cld
	repne	scasb			;find end of s1
	dec	edi			;point at terminating 0
	mov	edx,edi			;save end of s1
	mov	edi,esi			;EDI -> s2
	mov	ecx,eax
	dec	ecx			;ECX = -1
	repne	scasb
	not	ecx

	mov	edi,edx			;EDI -> end of s1

	mov	edx,P+SIZEPTR+SIZEPTR[ebp]
	dec	ecx			;ECX = strlen(s2)
	_ifs	ecx b edx, L6
	mov	ecx,edx			;ECX = min(ECX,n)
L6:	mov	edx,ecx
	shr	ecx,2
	rep	movsd
	mov	cl,3
	and	ecx,edx
	rep	movsb			;transfer bytes
	stosb				;terminate with a 0
	mov	eax,P[ebp]		;return pointer to s1

	unuse	<edi,esi,ecx,edx>
	pop	ebp
	_ret	12

_strncat	endp
	endcode strncat
	end

