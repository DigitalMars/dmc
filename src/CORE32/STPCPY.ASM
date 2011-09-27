;_ stpcpy.asm	
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

;CAUTION!  This code is highly optimized.  It does not fully utilize the
;macros 'uses' and 'unuse' for register preservation.  If registers other than
;EBX, ESI, and EDI (and of course EBP,DS, and ES) must be preserved examine 
;this module carefully.

include macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copy s2 to s1 till a zero byte.
; Use:
;	char *stpcpy(char *s1,const char *s2)
; Returns:
;	pointer to terminating NUL
; Written by Bob Stout and Walter Bright

	begcode stpcpy
	public	_stpcpy
_stpcpy proc	near
	push	esi
	push	edi
	mov	edi,8+PS+SIZEPTR[esp]	;get source pointer (s2)

ife ESeqDS
	mov	ax,ds
	mov	es,ax
endif

	clr	eax			;scan for 0
	mov	ecx,eax
	dec	ecx			;largest possible string (ECX = -1)
	mov	esi,edi			;save it
	repne	scasb			;find end of s2
	not	ecx			;ECX = strlen(s2) + 1 (for EOS)
	mov	edi,8+PS[esp]		;EDI -> s1
	mov	eax,ecx			;return value
	add	eax,edi
	mov	edx,ecx
	shr	ecx,2
	rep	movsd
	mov	cl,3
	and	ecx,edx
	rep	movsb			;transfer bytes (including EOS)
	mov	eax,edi
	dec	eax			;EAX = s1 + strlen(s2)
	pop	edi
	pop	esi
	_ret	8
_stpcpy endp
	endcode stpcpy
	end

