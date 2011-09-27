;_ criterr.asm  written by G. Eric Engstrom
;            Copyright (C) 1990 by Concentric Software Designs, Inc.
;            All Rights Reserved
; This function has not been ported to OS/2 because the operating system
; provides these features.

;cerror_open			turn critical error trapping on
;cerror_close			turn critical error trapping off

include	macros.asm

	c_extrn  _cerror_handler,near
        extrn  __x386_memunlock:near
        extrn  __x386_memlock:near

	begdata

cerrorBeg   dd 256 dup (?)
cerrorStack equ $

cerrorStackSeg			dw ?
cerrorStackPtr			dd ?
cerrorInt24Vector		dd ?
cerrorProtectedInt24Vector	dd ?
				dw ?
cerrorEAX			dd ?
cerrorEDI			dd ?
	enddata

	begcode	cerror

	c_public cerrorIntTrap
func    cerrorIntTrap
	sti
        mov     cerrorStackSeg,SS
        mov     cerrorStackPtr,ESP
        mov     cerrorEAX,EAX
        mov     cerrorEDI,EDI
	cli
	push	DS
	pop	SS
	mov	ESP,offset DGROUP:cerrorStack-SIZEPTR
	and	ESP,0FFFFFFFCh
	sti
	
        push	EBX
        push	ECX
        push	EDX
        push	EBP
        push	ESI
        push	EDI
        push	ES

        mov	EBX,offset DGROUP:cerrorEDI
        push	EBX
        mov	EBX,offset DGROUP:cerrorEAX
        push	EBX
        callm	cerror_handler
        add	ESP,SIZEPTR+SIZEPTR
	clr	EAX
        mov	EAX,cerrorEAX

        pop	ES
        pop	EDI
        pop	ESI
        pop	EBP
        pop	EDX
        pop	ECX
        pop	EBX

        cli
        mov     SS,cerrorStackSeg
        mov     ESP,cerrorStackPtr
        sti
cerrorIntTrap_ret:
        iretd
c_endp	cerrorIntTrap

        c_public cerror_open
func    cerror_open
        push	EBP
	uses	<ES,DS,EBX>
         
        ;; Lock mem used in interrupt handler
        push EAX
       lea EAX,__x386_memlock
        push EAX
        call cerror_mem
        pop EAX
      

        mov	AX,02502h
	mov	CL,024h
        int	21h
        mov     dword ptr cerrorProtectedInt24Vector,EBX
        mov     word ptr cerrorProtectedInt24Vector+4,ES
	mov	AX,02503h
	mov	CL,024h
	int	21h
        mov     word ptr cerrorInt24Vector,BX
        mov     word ptr cerrorInt24Vector+2,ES
        mov	EDX,offset _TEXT:_cerrorIntTrap
        push	CS
        pop	DS
        mov	AX,02506h
	mov	CL,024h
        int	21h
	unuse	<EBX,DS,ES>
        pop	EBP
        ret
c_endp  cerror_open

        c_public cerror_close
func    cerror_close
        push	EBP
	uses	<EBX,EDX,DS,ES>
	mov	EDX,dword ptr cerrorProtectedInt24Vector
        mov	EBX,dword ptr cerrorInt24Vector
	mov	DS,word ptr cerrorProtectedInt24Vector+4
        mov	AX,02507h
	mov	CL,024h
        int	21h

        ;; Unlock used memory
        push EAX
	    lea EAX, __x386_memunlock
        push EAX
        call cerror_mem
		pop EAX

	unuse	<ES,DS,EDX,EBX>
        pop	EBP
        ret
c_endp  cerror_close

cerror_mem  proc near
       push ESI
       mov ESI,8[ESP]  ;; unlock or lock
       
       mov  EAX, offset cerrorIntTrap_ret[1] ;offset of last instruction in isr
       sub  EAX, offset cerrorIntTrap ;calculate length of isr
       push EAX  
       push CS
       push offset cerrorIntTrap
       call ESI
       lea  ESP,[ESP+12]    ;pop parameters off the stack

       mov EAX,offset DGROUP:cerrorInt24Vector ;End of data
       sub EAX,offset DGROUP:cerrorBeg  ;calculate length of data, take into account bytes of stack
       push EAX
       push DS
       push offset DGROUP:cerrorBeg
       call ESI 
       lea ESP,[ESP+12]   ;pop parameters off the stack

       ;; Don't forget cerrorEAX and cerrorEDI
       push 8
       push DS
       push offset DGROUP:cerrorEAX
       call ESI
       lea ESP,[ESP+12]   ; pop parameters off the stack

       pop ESI
       ret 4
cerror_mem  endp

        endcode cerror

        end


