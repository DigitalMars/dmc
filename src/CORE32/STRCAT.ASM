;_ strcat.asm	
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

;CAUTION!  This code is highly optimized.  It does not fully utilize the
;macros 'uses' and 'unuse' for register preservation.  If registers other than
;EBX, ESI, and EDI (and of course EBP,DS, and ES) must be preserved examine 
;this module carefully.

include macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Cat s2 to s1 till a zero byte.
; Use:
;	char *strcat(s1,s2)
; Returns:
;	s1
;
	begcode strcat
	public	_strcat
_strcat proc	near
	push	ebp
	mov	ebp,esp
	uses	<ECX,EBX,EDX,ESI,EDI>

	mov	edi,P+SIZEPTR[ebp]	;s2

ife ESeqDS
	mov	ax,ds
	mov	es,ax
endif

	clr	eax			;scan for 0
	mov	ecx,eax
	dec	ecx			;largest possible string (ECX = -1)
	cld
	repne	scasb			;find end of s1
	not	ecx			;ECX = strlen(s2) + 1 (for EOS)
	sub	edi,ecx			;back up EDI to start of s2

	mov	edx,ecx			;EDX = strlen(s2)
	mov	ecx,eax
	mov	esi,edi
	mov	edi,P[ebp]		;EDI = s1
	dec	ecx			;ECX = -1
	mov	ebx,edi			;EBX = s1
	repne	scasb
	dec	edi			;EDI -> EOS of s1
	mov	ecx,edx			;ECX = strlen(s2)
	shr	ecx,2
	rep	movsd
	mov	ecx,3
	and	ecx,edx
	rep	movsb			;transfer bytes (including EOS)
	mov	eax,ebx			;return pointer to s1

	unuse	<edi,esi,edx,ebx,ecx>
	pop	ebp
	_ret	8
_strcat endp
	endcode strcat
	end

