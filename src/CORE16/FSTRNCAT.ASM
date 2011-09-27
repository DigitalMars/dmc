;_ fstrncat.asm   Sat July 28, 1990   
;Copyright (C) 1985-1989 by Walter Bright
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Cat s2 to s1 till a zero byte or n bytes are copied.
; Use:
;	char __far *fstrncat(char __far *s1,char __far *s2,unsigned n)
; Returns:
;	s1
;

 	ife LPTR and LCODE
	begcode	strings
	public	_fstrncat
_fstrncat proc far
	push    BP                                 
	mov     BP,SP                              
	.save	<SI,DI>
	push	DS
	les	DI,6[BP]		;get destination pointer (s1)
	mov	BX,DI			;set up offset of return value (s1)
	clr	AX			;scan for 0
	mov	CX,-1			;largest possible string
;	cld
	repne	scasb			;find end of s1
	dec	DI			;point at terminating 0
	mov	DX,DI			;save
	les	DI,6+4[BP]		;ES:DI -> s2
	mov	CX,-1
	repne	scasb
	not	CX
	dec	CX			;CX = strlen(s2)

	mov	DI,DX			;DI -> end of s1
	mov	DX,6+4+4[BP]		;DX -> n
	_if	CX b DX, L6
	mov	CX,DX			;CX = min(CX,n)
L6:
	mov	ES,6+2[BP]		;ES = segment of s1
	lds	SI,6+4[BP]		;DS:SI -> s2
	shr	CX,1
	rep	movsw
	rcl	CX,1
	rep	movsb			;transfer bytes
	stosb				;terminate with a 0
	mov	AX,BX
	mov	DX,ES			;DX,AX = s1
	pop	DS
	.restore <DI,SI>
	pop	BP
	ret                                        
_fstrncat endp
	endcode	strings
	endif
	end


