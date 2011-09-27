;_ controlc.asm  written by G. Eric Engstrom

;controlc_open			turn control C trapping on
;controlc_close			turn control C trapping off

include	macros.asm

	comm	__controlc_handler:dword	;address of handler

	begdata
        db 512 dup (?)
controlCStack equ $

controlCStackPtr	dd ?
controlCStackSeg	dw ?

cBreak1BVector		dd ?
cBreak1BVectorProtected	dd ?
			dw ?
cBreak23Vector		dd ?
cBreak23VectorProtected dd ?
			dw ?
cBreakStatus   db ?
	enddata

	begcode	controlc
        extrn __x386_memlock:near
        extrn __x386_memunlock:near

	c_public cBreakHandler
func	cBreakHandler
        mov	controlCStackSeg,SS
        mov	controlCStackPtr,ESP
        push	DS
        pop	SS
        mov	ESP,offset DGROUP:controlCStack-SIZEPTR
        and	ESP,0FFFFFFFCh			;dword align stack
        sti

	push    EAX
	push    EBX
	push    ECX
	push    EDX
	push    EBP
	push    ESI
	push    EDI
	push    ES
	push    DS

	cmp	dword ptr __controlc_handler,0
	je	short nohandler
	  call	  dword ptr __controlc_handler
nohandler:
 
	pop     DS
	pop     ES
	pop     EDI
	pop     ESI
	pop     EBP
	pop     EDX
	pop     ECX
	pop     EBX
	pop     EAX
        mov	SS,controlCStackSeg
        mov	ESP,controlCStackPtr
cBreakHandler_ret:
	iretd
c_endp	cBreakHandler

	c_public controlc_open
func	controlc_open
    ifdef __OS2__
    else
	push	EBP
	push	EBX
	push	DS
	push	ES
	; get current ^C BREAK STATUS
	mov	AX,03300h
	int	21h
	mov	cBreakStatus,DL
	; set current ^C BREAK STATUS TO ON
	mov	AX,03301h
	mov	DL,1
	int	21h

;	trap int 1b and 23
	push	ESI
	push	EDI

	mov	CL,01Bh
	mov	ESI,offset DGROUP:cBreak1BVector
	call	getVector
	mov	CL,023h
	mov	ESI,offset DGROUP:cBreak23Vector
	call	getVector

        ;; Lock code and data used in cBreakHandler
    push EAX
    lea EAX,__x386_memlock
    push EAX
        call ctrlc_mem
    pop EAX
       
	mov	CL,01Bh
	mov	EDX,offset cBreakHandler
	call	installVector
	mov	CL,023h
	mov	EDX,offset cBreakHandler
	call	installVector
	
        pop	EDI
	pop	ESI

	pop	ES
	pop	DS
	pop	EBX
	pop	EBP
    endif
	ret
c_endp	controlc_open

	c_public controlc_close
func	controlc_close
    ifdef __OS2__
    else
	push	EBP
	push	EBX
	push	DS
        push	ES

        mov	CL,01Bh
        mov	ESI,offset DGROUP:cBreak1BVector
        call	restoreVector
        mov	CL,023h
	mov	ESI,offset DGROUP:cBreak23Vector
        call	restoreVector

	mov	AX,03301h
	mov	DL,cBreakStatus
	int	21h

	;; Unlock code and data used in ControlBreakHandler
    push EAX
    lea EAX,__x386_memunlock
    push EAX
        call    ctrlc_mem
    pop EAX
        
	pop	ES
	pop	DS
	pop	EBX
	pop	EBP
    endif
	ret
c_endp	controlc_close

getVector proc near
	push	ESI
	push	ECX
	mov	AX,02502h	;get protected mode interrupt vector
	int	21h
	pop	ECX
	pop	ESI

	mov	4[ESI],EBX
	mov	8[ESI],ES

	push	ESI
	mov	AX,02503h	;get real mode interrupt vector
	int	21h
	pop	ESI

	mov	[ESI],EBX
	ret
getVector endp

restoreVector proc near
	push	DS
        mov	EBX,[ESI]
	lds	EDX,4[ESI]
	mov	AX,02507h	;set real and protected mode interrupt vectors
L1:	int	21h
        pop	DS
        ret
restoreVector endp

installVector proc near
	push	DS
        push	CS
        pop	DS
	mov	AX,02506h
	jmp	short L1
installVector endp

ctrlc_mem  proc near
       push ESI
       mov ESI,8[ESP]  ;; unlock or lock
       mov  EAX, offset cBreakHandler_ret[1] ;offset of last instruction in isr
       sub  EAX, offset cBreakHandler ;calculate length of isr
       push EAX  
       push CS
       push offset cBreakHandler
       call ESI
       lea  ESP,[ESP+12]    ;pop parameters off the stack
       mov EAX,offset DGROUP:cBreak1BVector ;End of data to unlock
       sub EAX,offset DGROUP:controlCStack-512  ;calculate length of data, take into account bytes of stack
       push EAX
       push DS
       push offset DGROUP:controlCStack-512
       call ESI 
       lea ESP,[ESP+12]   ;pop parameters off the stack
       push 4
       push DS
       push offset DGROUP:__controlc_handler
       call ESI
       lea ESP,[ESP+12]   ; pop parameters off the stack
       pop ESI
       ret 4
ctrlc_mem  endp
	endcode controlc

	end

