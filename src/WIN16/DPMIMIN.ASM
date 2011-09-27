;_ dpmimin.asm
; Copyright (C) 1991 Walter Bright
; All Rights Reserved
; Written by G. Eric Engstrom

include macros.asm

	begdata
	enddata
 
	begcode dpmimin

	c_public dpmi_GetBaseAddress
func	dpmi_GetBaseAddress
	WINENTER
	mov	BX,P[BP]
	dpmi	6
	jnc	gba0
	  clr	  CX
	  mov	  DX,CX
gba0:	mov	AX,DX
	mov	DX,CX
	WINLEAVE
	ret
c_endp	dpmi_GetBaseAddress

	c_public dpmi_FreeDescriptor
func	dpmi_FreeDescriptor
	WINENTER
	mov	AX,1
	jmp	short dpmi_do_bp
c_endp	dpmi_FreeDescriptor

	c_public dpmi_SetSegLimit
func	dpmi_SetSegLimit
	WINENTER
	mov	AX,8
        jmp	short dpmi_addr
c_endp	dpmi_SetSegLimit

	c_public dpmi_SetBaseAddress
func	dpmi_SetBaseAddress
	WINENTER
	mov	AX,7
dpmi_addr:
	mov	DX,P+2[BP]
	mov	CX,P+4[BP]
dpmi_do_bp:
	mov	BX,P[BP]
	dpmi
	jnc	sba0
	  clr	  AX		; on error return -1
	  dec	  AX
sba0:	WINLEAVE
	ret
c_endp	dpmi_SetBaseAddress

	c_public dpmi_IsRealMode
func	dpmi_IsRealMode
	WINENTER
	mov	AX,1686h
	int	02Fh
	WINLEAVE
	ret
c_endp	dpmi_IsRealMode

	endcode	dpmimin
        end
        end
