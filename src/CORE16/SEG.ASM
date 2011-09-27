;_ seg.asm   Tue Dec 19 1989
; Written by Walter Bright
; $Revision: 1.1.1.1 $

	include macros.asm

	c_public getDS,getES,getSS
	
	begcode seg
	
;;;;;;;;;;;;;;;;;;;;;;;;;
; Return data segment.

func	getDS
	mov	AX,DS
	ret
c_endp	getDS

;;;;;;;;;;;;;;;;;;;;;;;;;
; Return extra segment.

func	getES
	mov	AX,ES
	ret
c_endp	getES

;;;;;;;;;;;;;;;;;;;;;;;;
; Return stack segment

func	getSS
	mov	AX,SS
	ret
c_endp	getSS

	endcode seg

	end
