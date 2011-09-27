;_ strncpy.asm	 
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

;CAUTION!  This code is highly optimized.  It does not fully utilize the
;macros 'uses' and 'unuse' for register preservation.  If registers other than
;EBX, ESI, and EDI (and of course EBP,DS, and ES) must be preserved examine 
;this module carefully.

include macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copy exactly n chars from s2 to s1, padding with nulls if necessary.
; Use:
;	#include <string.h>
;	char *strncpy(char *s1,const char *s2,size_t n);

	begcode strncpy
	public	_strncpy
_strncpy	proc	near
	push	ebp
	mov	ebp,esp
	uses	<EBX,ECX,EDX,ESI,EDI>

	mov	edi,P+SIZEPTR[ebp]	;get source pointer

ife ESeqDS
	mov	ax,ds
	mov	es,ax
endif

	clr	eax			;scan for 0
	mov	ecx,eax
	dec	ecx			;largest possible string (ECX = -1)
	mov	ebx,edi			;save it
	cld
	repne	scasb			;find end of s2
	mov	edx,P+(2*SIZEPTR)[ebp]	;EDX = n
	not	ecx			;ECX = strlen(s2) + 1 (for EOS)
	_if	edx a ecx, L7		;if n > strlen(s2) + 1
	mov	ecx,edx			;ECX = min(ECX,n)
L7:
	mov	eax,ecx
	sub	edx,ecx			;EDX = # of nulls to pad
	mov	esi,ebx			;ESI -> s2
	mov	edi,P[ebp]		;EDI -> s1
	shr	ecx,2
	mov	ebx,edi			;Save s1 for return value.
	rep	movsd
	mov	cl,3
	and	ecx,eax
	rep	movsb			;transfer bytes (including EOS)
	mov	ecx,edx			;# of nulls to pad
	clr	al
	shr	ecx,2
	rep	stosd
	mov	cl,3
	and	ecx,edx
	rep	stosb
	mov	eax,ebx			;return value
	unuse	<edi,esi,edx,ecx,ebx>
	pop	ebp
	_ret	3*4
_strncpy	endp
	endcode strncpy
	end

