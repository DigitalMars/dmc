;_ _ptrchk.asm   Wed Jun 22 1994
; Copyright (C) 1994 by Digital Mars
; All rights reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	public __ptrchk, __current_stackptr,__check_stack_top, __check_stack_bottom
	begdata

    ifndef _WINDOWS
	extrn	__BASE:word
	extrn	__heapbottom:word
    endif

__current_stackptr  dd ?   ;; Filled with current seg:off of ss:sp for checking
__check_stack_top dd ?   ;; Filled with current top of stack
__check_stack_bottom dd ? ;; Filled with current bottom of stack

if LCODE
OVERHEAD equ 0ah      ;; amount to adjust sp by to get current sp
else
OVERHEAD equ 08h      ;; amount to adjust sp by go get current sp
endif

STACKTOP equ 0ah      ;; Offset off of ss to top of stack in Windows
STACKBOT equ 0eh      ;; Offset off of ss to bottom of stack for windows
        enddata

	if LCODE
	extrn PTRCHK:far     ;; Routine to do actual pointer checking
	else
	extrn PTRCHK:near    ;; Routine to do actual pointer checking
	endif

	begcode	_ptrchk

p_func __ptrchk
  	push bp
  	mov bp,sp   ; save sp to access arg
	;; Save all registers
  	push ES
       	push AX
       	push CX
       	push DX
       	push BX
       	push SI
       	push DI
	;; Setup up stack pointer variables used in PTRCHK

     ifdef _WINDOWS
	mov ax, ss:STACKTOP  ;; In windows top of stack is stored off of ss
     else
        mov ax, __BASE	     ;; In DOS, __BASE contains top of stack
     endif
	lea SI, __check_stack_top  ;; SI contains address of global __check_stack_top
	mov word ptr 2[SI],ss ;; ___stack_top is a far pointer with seg = ss
	mov [SI],ax	      ;; offset contains ss:STACKTOP
	lea SI,__check_stack_bottom  ;; SI contains address of global __check_stack_bottom
	mov word ptr 2[SI],  ss	 ;; ___stack_bottom is a far pointer with seg = ss
     ifdef _WINDOWS
	mov ax, word ptr ss:STACKBOT  ;; bottom of stack stored off of ss for Win.
     else
        mov ax, __heapbottom	      ;; for DOS, stored in __heapbottom
     endif
	mov [SI],ax		      ;; offset contains current heapbottom
	mov ax,bp		      ;; grab current frame pointer
	add ax,OVERHEAD		      ;; Get value of SP at time of ptrchk
	lea SI, __current_stackptr   ;; SI contains address of global ___current_stackptr
	mov word ptr 2[SI],ss	     ;; seg of current_stackptr is always ss
	mov [SI],ax		    ;; offset is current value of sp
	;; Grab address passed in and pass onto PTRCHK
  	mov DX, P+2[bp]
  	mov AX, P[bp]
  	push DX
  	push AX
  	call PTRCHK
	;; Restore all registers before returning
  	pop  DI
	pop  SI
	pop  BX
	pop  DX
	pop  CX
	pop  AX
	pop  ES
  	mov sp,bp   ; restore sp
  	pop bp
  	ret 4
p_endp __ptrchk 

      endcode _ptrchk

      end


