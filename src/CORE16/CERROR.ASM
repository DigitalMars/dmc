;_ cerror.asm
; Windows support added by G. Eric Engstrom Jan 1991	-D_WINDOWS
; OS2 support added by G. Eric Engstrom May 1989	-D__OS2__
; Copyright (C) 1990 by Concentric Software Designs, Inc.
; All Rights Reserved
; Written by G. Eric Engstrom

;cerror_open			turn critical error trapping on
;cerror_close			turn critical error trapping off

include	macros.asm

    if LCODE
	extrn	__cerror_handler:far
    else
    	extrn	__cerror_handler:near
    endif		

ifdef __OS2__

	begdata

dummyInt24a		dd 0
dummyInt24b		dw 0

SIGA_KILL		equ 0		; from bsedos.h included with os2.h
SIGA_ACCEPT		equ 2		; from bsedos.h included with os2.h
SIGA_ACKNOWLEDGE	equ 4		; from bsedos.h included with os2.h

SIG_KILLPROCESS		equ 3       

	enddata

	extrn	DOSSETSIGHANDLER:far

	begcode	cerror

	public cerrorIntTrap
cerrorIntTrap 	proc far
	push	DS
	push	BP
	mov	BP,SP
	mov	AX,seg DGROUP
	mov	DS,AX

	push	CS
	mov	BX,offset cerrorIntTrap
	push	BX			;address of new handler

	clr	AX
	push	AX
	push	AX
	push	AX
	push	AX

	mov	BX,SIGA_ACKNOWLEDGE	;reset the signal
	push	BX

	push	8[BP]			;signal number
	call	DOSSETSIGHANDLER

	callm	_cerror_handler
#endif
nohandler:
	pop	BP
	pop	DS
	ret	4
cerrorIntTrap endp

	c_public cerror_open
func	cerror_open
	mov	CX,SIGA_ACCEPT
	jmp	short _setup
c_endp	cerror_open

	c_public cerror_close
func	cerror_close
	mov	CX,SIGA_KILL
_setup: push	CX

	mov	BX,offset cerrorIntTrap
	push	CS
	push	BX

	push	DS
	mov	BX,offset DGROUP:dummyInt24a
	push	BX				;addr of prev sig handler

	push	DS
	mov	BX,offset DGROUP:dummyInt24b	;prev sig handler action
	push	BX

	push	CX				;new signal handler action

	mov	BX,SIG_KILLPROCESS
	push	BX				;signal number of interest
	call	DOSSETSIGHANDLER
	pop	CX

	mov	BX,offset cerrorIntTrap
	push	CS
	push	BX

	ret
c_endp	cerror_close

	endcode cerror


else ; not __OS2__
ifdef _WINDOWS

	begcode	cerror
	c_public cerror_open
func	cerror_open
c_endp	cerror_open

	c_public cerror_close
func	cerror_close
	ret
c_endp	cerror_close

	endcode cerror

else ; not _WINDOWS

	begdata
        db 512 dup (?)
cerrorStack equ $
cerrorStackSeg			dw ?
cerrorStackPtr			dw ?
cerrorInt24Vector		dd ?
cerrorAX			dw ?
cerrorDI			dw ?
	enddata

	begcode	cerror
dsvalue	dw	?		;where saved value of DS goes

cerrorIntTrap proc far
	push	DS
	mov	DS,CS:dsvalue
        mov     cerrorStackSeg,SS
        mov     cerrorStackPtr,SP
        mov     cerrorAX,AX
        mov     cerrorDI,DI
        cli
	push	DS
	pop	SS
	mov	SP,offset DGROUP:cerrorStack-SIZEPTR
	and	SP,0FFFCh
	sti
	push	BX
	push	CX
	push	DX
	push	BP
	push	SI
	push	DI
	push	ES

    if LPTR
    	push	DS
    endif 	
        mov	BX,offset DGROUP:cerrorDI
        push	BX
    if LPTR
     	push	DS
    endif 	    
        mov	BX,offset DGROUP:cerrorAX
        push	BX
	call	__cerror_handler
    if LPTR
    	add	SP,8
    else
        add 	SP,4
    endif    
    	
	pop	ES
	pop	DI
	pop	SI
	pop	BP
	pop	DX
	pop	CX
	pop	BX
	
	mov	AX,cerrorAX
	
        cli
        mov     SS,cerrorStackSeg
        mov     SP,cerrorStackPtr
nohandler:        
        pop	DS
        iret
cerrorIntTrap endp

	c_public cerror_open
func	cerror_open
	push	BP
	push	DS
	push	ES
	
	mov	CS:dsvalue,DS		;save so handler can reload DS
	mov	AX,03524h
	int	21h
	mov	word ptr DGROUP:cerrorInt24Vector,BX
	mov	word ptr DGROUP:cerrorInt24Vector+2,ES
	push	CS
	pop	DS
	mov	DX,offset CS:cerrorIntTrap
	mov	AX,02524h
	int	21h
	pop	ES
	pop	DS
	pop	BP
	ret
c_endp	cerror_open

	c_public cerror_close
func	cerror_close
	push	BP
	push	DS
	lds	DX,dword ptr DGROUP:cerrorInt24Vector
	mov	AX,02524h
	int	21h
	pop	DS
	pop	BP
	ret
c_endp	cerror_close

	endcode cerror

    endif ; NOT_WINDOWS
    endif ; NOT__OS2__

	end

