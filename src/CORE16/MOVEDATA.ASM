;_ movedata.asm   Sat July 28, 1990  
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	void movedata(unsigned short srcseg,unsigned srcoff,
;		unsigned short destseg,unsigned destoff,size_t numbytes);

	c_public	movedata
	begcode		strings
func	movedata
	push	BP
	mov	BP,SP
	_push	<SI,DI,DS>
	mov	DS,P[BP]
	mov	SI,P+2[BP]
	mov	ES,P+4[BP]
	mov	DI,P+6[BP]
	mov	CX,P+8[BP]	;get numbytes
	shr	CX,1		;convert to word count
;	cld
	rep 	movsw
	rcl	CX,1
	rep	movsb		;move odd byte
   	_pop	<DS,DI,SI>
   	pop	BP
	ret
c_endp	movedata
	endcode	strings
	end


