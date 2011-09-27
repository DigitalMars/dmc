;_ strncat.asm   Sat July 28, 1990  
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Cat s2 to s1 till a zero byte or n bytes are copied.
; Use:
;	char *strncat(char *s1,char *s2,unsigned n)
; Returns:
;	s1
;
	begcode	strings
	c_public	strncat
 	if LPTR and LCODE
	public _fstrncat
_fstrncat:
	endif
func	strncat
	push    BP                                 
	mov     BP,SP                              
	.save	<SI,DI>
    if SPTR
      ife ESeqDS
	mov	AX,DS
	mov	ES,AX
      endif
	mov     SI,P+2[BP]		;get source pointer (s2)
	mov     DI,P[BP]		;get destination pointer (s1)
    else
	push	DS
	les	DI,P[BP]		;get destination pointer (s1)
	mov	BX,DI			;set up offset of return value (s1)
    endif
	clr	AX			;scan for 0
	mov	CX,-1			;largest possible string
;	cld
	repne	scasb			;find end of s1
	dec	DI			;point at terminating 0
	mov	DX,DI			;save
    if SPTR
	mov	DI,SI			;DI -> s2
    else
	les	DI,P+SIZEPTR[BP]	;ES:DI -> s2
    endif
	mov	CX,-1
	repne	scasb
	not	CX
	dec	CX			;CX = strlen(s2)

	mov	DI,DX			;DI -> end of s1
	mov	DX,P+SIZEPTR+SIZEPTR[BP];DX -> n
	_if	CX b DX, L6
	mov	CX,DX			;CX = min(CX,n)
L6:
    if LPTR
	mov	ES,P+2[BP]		;ES = segment of s1
	lds	SI,P+SIZEPTR[BP]	;DS:SI -> s2
    endif
	shr	CX,1
	rep	movsw
	rcl	CX,1
	rep	movsb			;transfer bytes
	stosb				;terminate with a 0
    if SPTR
	mov	AX,P[BP]		;return pointer to s1
    else
	mov	AX,BX
	mov	DX,ES			;DX,AX = s1
	pop	DS
    endif
	.restore <DI,SI>
	pop	BP
	ret                                        
c_endp	strncat
	endcode	strings
	end


