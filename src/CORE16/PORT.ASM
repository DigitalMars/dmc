;_ port.asm   Fri Dec 15 1989	Modified by: Walter Bright */
; Windows support added by G. Eric Engstrom Dec 1990	-D_WINDOWS
; Modified by G. Eric Engstrom Jan 1991
; Copyright (C) 1985-1989 by Walter Bright
; All Rights Reserved
; I/O port functions

	include macros.asm

	begcode	port

;;;;;;;;;;;;;;;;;;;;;;;;;
; Read from I/O port.
; Use:
;	int inp(portnumber)
;	unsigned portnumber;
; Returns:
;	byte read from I/O port with high byte cleared

	c_public inp
func	inp
	mov	BX,SP
	mov	DX,SS:P-2[BX]	; The port number.
	in	AL,DX
	xor	AH,AH		; Clear the upper byte.
	ret
c_endp	inp

;;;;;;;;;;;;;;;;;;;;;;;;;
; Read from I/O port.
; Use:
;	int inpw(portnumber)
;	unsigned portnumber;
; Returns:
;	word read from I/O port

	c_public inpw
func	inpw
	mov	BX,SP
	mov	DX,SS:P-2[BX]	; The port number.
	in	AX,DX		; Done.
	ret
c_endp	inpw

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Output byte to I/O port.
; Use:
;	int outp(portnumber,byte)
;	unsigned portnumber,byte;
; Returns:
;	byte

	c_public outp
func	outp
	mov	BX,SP
	mov	AX,SS:P-2+2[BX]
	mov	DX,SS:P-2[BX]
	out	DX,AL
	ret
c_endp	outp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Output word to I/O port.
; Use:
;	int outpw(portnumber,word)
;	unsigned portnumber,word;
; Returns:
;	byte

	c_public outpw
func	outpw
	mov	BX,SP
	mov	AX,SS:P-2+2[BX]
	mov	DX,SS:P-2[BX]
	out	DX,AX
	ret
c_endp	outpw

	endcode	port

	end

