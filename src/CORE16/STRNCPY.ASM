;_ strncpy.asm   Sat July 28, 1990   
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copy exactly n chars from s2 to s1, padding with nulls if necessary.
; Use:
;	char *strncpy(s1,s2,n)
;
	begcode	strings
	c_public	strncpy
 	if LPTR and LCODE
		public _fstrncpy
_fstrncpy:
	endif
func	strncpy
	push    BP                                 
	mov     BP,SP                              
	.save	<SI,DI>
    if SPTR
      ife ESeqDS
	mov	AX,DS
	mov	ES,AX
      endif
	mov     DI,P+2[BP]		;get source pointer
	mov	BX,DI			;save it
    else
	les	DI,P+4[BP]
    endif
	clr	AX			;scan for 0
	mov	CX,-1			;largest possible string
;	cld
	repne	scasb			;find end of s2
	not	CX			;CX = strlen(s2) + 1 (for EOS)
    if SPTR
	mov	DX,P+4[BP]		;DX = n
    else
	mov	DX,P+8[BP]
    endif
	_if	DX ae CX, L7		;if n >= strlen(s2) + 1
	mov	CX,DX			;CX = min(CX,n)
L7:	sub	DX,CX			;DX = # of nulls to pad
    if SPTR
	mov	DI,P[BP]		;DI -> s1
	mov	SI,BX			;SI -> s2
    else
	push	DS
	les	DI,P[BP]
	lds	SI,P+4[BP]
    endif
	mov	BX,DI			;for return value
	shr	CX,1
	rep	movsw
	rcl	CX,1
	rep	movsb			;transfer bytes (including EOS)
	mov	AX,CX			;AX = 0
	mov	CX,DX			;# of nulls to pad
	shr	CX,1
	rep	stosw
	rcl	CX,1
	rep	stosb
	mov	AX,BX			;Return Value
    if LPTR
	mov	DX,ES
	pop	DS
    endif
	.restore <DI,SI>
	pop	BP
	ret                                        
c_endp	strncpy
	endcode	strings
	end


