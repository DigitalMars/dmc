;_ stpcpy.asm   Sat July 28, 1990  
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copy s2 to s1 till a zero byte.
; Use:
;	char *stpcpy(char *s1,const char *s2)
; Returns:
;	pointer to terminating NUL
; Written by Bob Stout and Walter Bright

	begcode	strings
	c_public stpcpy
func	stpcpy
    if SPTR
	mov     BX,SP                              
	.save	SI
	mov	DX,DI
      ife ESeqDS
	mov	AX,DS
	mov	ES,AX
      endif
	mov     DI,SS:P-2+2[BX]		;get source pointer (s2)
	mov	SI,DI			;save it
	clr	AX			;scan for 0
	mov	CX,-1			;largest possible string
;	cld
	repne	scasb			;find end of s2
	not	CX			;CX = strlen(s2) + 1 (for EOS)
	mov	DI,SS:P-2[BX]		;DI -> s1
	mov	AX,DI			;return value
	add	AX,CX
	shr	CX,1
	rep	movsw			;Transfer words.
	rcl	CX,1
	rep	movsb			;transfer bytes (including EOS)
	mov	AX,DI
	dec	AX			;AX = s1 + strlen(s2)
	mov	DI,DX
	.restore SI
    else
	mov	BX,SP                              
	.save	<SI,DI>
	les	DI,SS:P-2+4[BX]		;ES:DI = s2
	clr	AX			;scan for 0
	mov	CX,-1			;largest possible string
	repne	scasb			;find end of s2
	not	CX			;CX = strlen(s2) + 1 (for EOS)
	mov	DX,DS
	les	DI,SS:P-2[BX]
	lds	SI,SS:P-2+4[BX]
	shr	CX,1
	rep	movsw
	rcl	CX,1
	rep	movsb
	mov	AX,DI
	dec	AX			;AX = s1 + strlen(s2)
	mov	DS,DX
	.restore <DI,SI>
	mov	DX,ES
    endif
	ret                                        
c_endp	stpcpy
	endcode	strings
	end


