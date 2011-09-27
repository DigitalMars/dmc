;_ rot.asm   
; Copyright (C) 1988 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

	include macros.asm

	begcode rot

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	#include <stdlib.h>
;	unsigned short _rotl(unsigned i,unsigned count);
;	unsigned long _lrotl(unsigned long i,unsigned count);
;
;	i is rotated left by count bits. 0 <= count < 32
;	The rotated result is returned.

	c_public  _lrotl
	c_public  _rotl

func	_lrotl
	mov	EAX,PS[ESP]
	mov	CL,PS+4[ESP]
	rol	EAX,CL
	_ret	8
c_endp	_lrotl

func	_rotl
	movzx	EAX,word ptr PS[ESP]
	mov	CL,PS+4[ESP]
	rol	AX,CL
	_ret	8
c_endp	_rotl

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	#include <stdlib.h>
;	unsigned short _rotr(unsigned i,unsigned count);
;	unsigned long _lrotr(unsigned long i,unsigned count);
;
;	i is rotated right by count bits. 0 <= count < 32
;	The rotated result is returned.

	c_public  _lrotr

	_align
func	_lrotr
	mov	EAX,PS[ESP]
	mov	CL,PS+4[ESP]
	ror	EAX,CL
	_ret	8
c_endp	_lrotr

	c_public  _rotr
func	_rotr
	movzx	EAX,word ptr PS[ESP]
	mov	CL,PS+4[ESP]
	ror	AX,CL
	_ret	8
c_endp	_rotr

	endcode rot

	end
