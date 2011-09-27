;_ movedata.asm	
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

;CAUTION!  This code is highly optimized.  It does not fully utilize the
;macros 'uses' and 'unuse' for register preservation.  If registers other than
;EBX, ESI, and EDI (and of course EBP,DS, and ES) must be preserved examine 
;this module carefully.

include macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	void movedata(unsigned short srcseg,unsigned srcoff,
;		unsigned short destseg,unsigned destoff,size_t numbytes);

	begcode		movedata

	public	_movedata
_movedata	proc	near
	push	ebp
	mov	ebp,esp
	push	ds
	push	es
	uses	<ESI,EDI>

	mov	ds,P[ebp]
	mov	esi,P+4[ebp]
	mov	es,P+8[ebp]
	mov	eax,P+16[ebp]	;get numbytes
	mov	edi,P+12[ebp]
	mov	ecx,eax
	shr	ecx,2		;convert to dword count
	repz	movsd

	mov	cl,3
	and	ecx,eax		;Compute remainder.
	rep	movsb		;move remainder bytes

	unuse	<edi,esi>
	pop	es
	pop	ds
	pop	ebp
	_ret	5*4
_movedata	endp
	endcode movedata
	end

