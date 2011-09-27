;_ port.asm   Fri Dec 15 1989	Modified by: Walter Bright */
; Copyright (C) 1985-1990 by Walter Bright
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
;	byte read from I/O port with high bytes cleared

	c_public inp
func	inp
	mov	DX,PS[ESP]	; The port number.
	in	AL,DX
	movzx	EAX,AL		; Clear the upper bytes.
	_ret	4
c_endp	inp

;;;;;;;;;;;;;;;;;;;;;;;;;
; Read from I/O port.
; Use:
;	int inpw(unsigned portnumber);
; Returns:
;	word read from I/O port

	c_public inpw
func	inpw
	mov	DX,PS[ESP]	; The port number.
	in	AX,DX		; Done.
	movzx	EAX,AX
	_ret	4
c_endp	inpw

;;;;;;;;;;;;;;;;;;;;;;;;;
; Read from I/O port.
; Use:
;	long inpw(unsigned portnumber);
; Returns:
;	long read from I/O port

	c_public inpl
func	inpl
	mov	DX,PS[ESP]	; The port number.
	in	EAX,DX		; Done.
	_ret	4
c_endp	inpl

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Output byte to I/O port.
; Use:
;	int outp(portnumber,byte)
;	unsigned portnumber,byte;
; Returns:
;	byte

	c_public outp
func	outp
	mov	EAX,PS+4[ESP]
	mov	DX,PS[ESP]
	out	DX,AL
	_ret	8
c_endp	outp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Output word to I/O port.
; Use:
;	int outpw(unsigned portnumber,unsigned word);
; Returns:
;	word

	c_public outpw
func	outpw
	mov	EAX,PS+4[ESP]
	mov	DX,PS[ESP]
	out	DX,AX
	_ret	8
c_endp	outpw

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Output long to I/O port.
; Use:
;	long outpl(unsigned portnumber,unsigned long x);
; Returns:
;	word

	c_public outpl
func	outpl
	mov	EAX,PS+4[ESP]
	mov	DX,PS[ESP]
	out	DX,EAX
	_ret	8
c_endp	outpl

	endcode	port

	end

