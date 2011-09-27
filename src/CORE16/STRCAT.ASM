;_ strcat.asm   Sat July 28, 1990  
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Cat s2 to s1 till a zero byte.
; Use:
;	char *strcat(s1,s2)
; Returns:
;	s1
;
	begcode	strings
	c_public	strcat
	
	if LPTR and LCODE
	public	_fstrcat
_fstrcat:
	endif

func	strcat
	push    BP                                 
	mov     BP,SP                              
	.save	<SI,DI>

    if SPTR
	mov	AX,DS
	mov	ES,AX
	mov     DI,P+SIZEPTR[BP]	;s2
    else
	les	DI,P+SIZEPTR[BP]	;s2
    endif
	clr	AX			;scan for 0
	mov	CX,-1			;largest possible string
;	cld
	repne	scasb			;find end of s1
	not	CX			;CX = strlen(s2) + 1 (for EOS)
	sub	DI,CX			;back up DI to start of s2

	mov	DX,CX
	mov	CX,-1
	mov	SI,DI
    if SPTR
	mov	DI,P[BP]
    else
	les	DI,P[BP]		;ES:DI = s1
    endif
	mov	BX,DI
	repne	scasb
	dec	DI			;ES:DI -> EOS of s1
	mov	CX,DX
	shr	CX,1
    if SPTR
	rep	movsw
	rcl	CX,1
	rep	movsb			;transfer bytes (including EOS)
	mov	AX,BX			;return pointer to s1
    else
	push	DS
	mov	DS,P+SIZEPTR+2[BP]	;DS:SI = s2
	rep	movsw
	rcl	CX,1
	rep	movsb			;transfer bytes (including EOS)
	mov	AX,BX
	mov	DX,ES			;DX,AX points to s1
	pop	DS
    endif
	.restore <DI,SI>
	pop	BP
	ret                                        
c_endp	strcat
	endcode	strings
	end


