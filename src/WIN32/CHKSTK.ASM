;_ chkstk.asm	Modified by: Walter Bright */
;Copyright (C) 1991 by Digital Mars
;All Rights Reserved
;Written by Walter Bright

include	macros.asm

	begcode	chkstk

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Check and see if stack overflowed.
; This can be called at any time by the application code. It was originally
; useful to place a call to this at selected points when stack overflow
; checking is turned off.  Obsolete for UNIX because of hardware protection
; and virtual memory.
; Returns:
;	# of bytes left in stack

	public	__chkstack
__chkstack	proc	near
    mov	EAX,ESP
;BUG: This is not implemented right
;	sub    EAX,__pastdata	;Remaining stack left.
	ret
__chkstack	endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Allocate local variables on stack.
; Let the operating system worry about stack overflow.
; Input:
;	EAX	aggregate aligned size of local variables
; Output:
;	EAX,EDX destroyed

	public	__chkstk, __alloca_probe

__alloca_probe:
__chkstk proc	near
        
    pop    EDX
    push   ECX         ;Save ECX for 'this' pointer
    mov    ECX,ESP
    add    ECX,4
    sub    ECX,EAX     ;new stack bottom
L1:	
    test   [ECX+EAX],EAX    ;bring in page
    sub    EAX,01000h	    ;next 4K page down
    jae	   L1		    ;if more pages
    test   [ECX],EAX	    ;bring in last page
    mov    EAX,ESP          ;EAX contains location of saved ECX above
    mov    ESP,ECX
    mov    ECX,[EAX]        ;Restore ECX
    jmp    EDX

__chkstk endp

	endcode	chkstk

	end
