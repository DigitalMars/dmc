;_ ltoa.asm   Modified by: Walter Bright */
; Copyright (C) 1991 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

include	macros.asm

	c_public	ltoa
	c_extrn		itoa,near

	begcode	ltoa

;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convert integer i to ascii, and store in string pointed to by a.
;	char *ltoa(long i,char *a,int base)
; Input:
;	a	pointer to string of sufficient length,
;		sizeof(int)*8+1 is always sufficient
;	base	radix, between 2 and 36 inclusive
; Returns:
;	a

func	ltoa			;same code as for itoa()
	jmp 	_itoa
c_endp	ltoa
	endcode	ltoa

	end
