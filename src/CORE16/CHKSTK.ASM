;_ chkstk.asm   Fri Aug 18 1989   Modified by: Walter Bright */
; Copyright (C) 1985-1991 by Walter Bright
; All rights reserved.
; Written by Walter Bright

include	macros.asm

    ifdef _WINDOWS
;Offsets into stack segment maintained by Windows
STACKTOP	equ	0ah
STACKMIN	equ	0ch
STACKBOT	equ	0eh
    endif

    ifdef __OS2__
extrn	DOSWRITE:far
extrn	DOSEXIT:far
    endif

    if LCODE
	extrn	___faterr:far
    else
	extrn	___faterr:near
    endif

	begdata

    ifndef _WINDOWS
    ifndef DOS16RM
	extrn	__BASE:word
	extrn	__heapbottom:word
    endif
    endif

    ifdef _WINDOWS
ovrflw		db	'Stack Overflow',0
    else
ovrflw		db	0Dh,0Ah,'Stack Overflow',0Dh,0Ah,0
    endif

	enddata

	begcode	chkstk

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Check and see if stack overflowed.
; This can be called at any time by the application code. It is useful
; to place a call to this at selected points when stack overflow checking
; is turned off.
; Returns:
;	# of bytes left in stack

	public	_stackavail
	public	__stackavail
__stackavail:				;alternate entry point
_stackavail:				;alternate entry point

	public	__chkstack
func	_chkstack
    ifdef _WINDOWS
	mov	AX,SP
	sub	AX,SS:STACKTOP
	jbe	XCOVF			;stack has overflowed
    else
    ifndef DOS16RM			;  hardware protected
	mov	BX,__BASE
      if SSeqDS
	_if	<word ptr [BX]> ne 55AAh, XCOVF
      else
	_if	<SS:word ptr [BX]> ne 55AAh, XCOVF
      endif
	mov	AX,SP
	sub	AX,BX
	jbe XCOVF				;SP is off bottom of stack
	_if	SP a __heapbottom, XCOVF	;SP wrapped around segment
    endif
    endif
	ret
c_endp	_chkstack

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Stack frame generator.
; Called at entry to each function when stack overflow checking
; is turned on.

	public	__chkstk
func	_chkstk
	pop	DX		;get offset of return addr
    if LCODE
	pop	CX		;get segment
    endif
	sub	SP,AX		;create space for local variables
	jbe	XCOVF		;overflowed

    ifdef _WINDOWS
	_if	SS:STACKTOP a SP, XCOVF
	_if	SS:STACKMIN be SP, C1
	mov	SS:STACKMIN,SP
C1:
    else

    ifndef DOS16RM
	mov	BX,__BASE
      if SSeqDS
	_if	<word ptr [BX]> ne 55AAh, XCOVF
      else
	_if	<SS:word ptr [BX]> ne 55AAh, XCOVF
      endif
    endif

    if SPTR
	_if	SP be BX, XCOVF			;is SP off bottom?
	_if	SP ae __heapbottom, XCOVF	;has SP wrapped around?
    endif
    endif

    if LCODE
	push	CX
	push	DX
	ret
    else
	jmp	DX		;return to caller
    endif
c_endp	_chkstk

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Stack overflow jumps here.

XCOVF	proc	near
	mov	DX,offset DGROUP:ovrflw
	push	DX
	call	___faterr
XCOVF	endp

	endcode	chkstk

	end
