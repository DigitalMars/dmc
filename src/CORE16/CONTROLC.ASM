;_ controlc.asm
; Windows support added by G. Eric Engstrom Jan 1991	-D_WINDOWS
; OS2 support added by G. Eric Engstrom Jan 1991	-D__OS2__
; Copyright (C) 1990 by Concentric Software Designs, Inc.
; All Rights Reserved
; Written by G. Eric Engstrom
;

;controlc_open			turn control C trapping on
;controlc_close			turn control C trapping off

include	macros.asm

if LCODE
	comm	__controlc_handler:dword	;address of handler
else
        comm	__controlc_handler:word
endif        	

ifdef __OS2__

	begdata

dummyControlC1	dd	0
dummyCBreak1	dd	0
dummyControlC2	dw	0
dummyCBreak2	dw	0

SIGA_KILL		equ 0		; from bsedos.h included with os2.h
SIGA_ACCEPT		equ 2		; from bsedos.h included with os2.h
SIGA_ACKNOWLEDGE	equ 4		; from bsedos.h included with os2.h

SIG_CTRLC		equ 1		; from bsedos.h included with os2.h
SIG_CTRLBREAK		equ 4		; from bsedos.h included with os2.h
	enddata

	extrn	DOSSETSIGHANDLER:far

	begcode	controlc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Use same handler for both CTRLC and CTRLBREAK
;  void pascal far controlCHandler(unsigned short SigArg,unsigned short SigNum);

	public controlCHandler
controlCHandler proc far
	push	DS
	push	BP
	mov	BP,SP
;	sub	SP,6		; make room for locals
	mov	AX,seg DGROUP
	mov	DS,AX

	push	CS
	mov	BX,offset controlCHandler
	push	BX			;address of new handler

	clr	AX
	push	AX
	push	AX
	push	AX
	push	AX

;	push	DS
;	lea	BX,[BP]-4
;	push	BX			;dword* receives addr of prev handler

;	push	DS
;	sub	BX,2
;	push	BX			;word* receives previous sig handler action

	mov	BX,SIGA_ACKNOWLEDGE	;reset the signal
	push	BX

	push	8[BP]			;signal number
	call	DOSSETSIGHANDLER

	;call user-defined handler
    if LCODE
	cmp	word ptr __controlc_handler+2,0
	je	nohandler
	  call	  dword ptr __controlc_handler
    else
	cmp	word ptr __controlc_handler,0
	je	nohandler
	  call	  word ptr __controlc_handler
    endif
nohandler:
;	mov	SP,BP
	pop	BP
	pop	DS
	ret	4
controlCHandler endp

	c_public controlc_open
func	controlc_open
	mov	CX,SIGA_ACCEPT
	jmp	short _setup
c_endp	controlc_open

	c_public controlc_close
func	controlc_close
	mov	CX,SIGA_KILL
_setup: push	CX

	mov	BX,offset controlCHandler
	push	CS
	push	BX

	push	DS
	mov	BX,offset DGROUP:dummyControlC1
	push	BX				;addr of prev sig handler

	push	DS
	mov	BX,offset DGROUP:dummyControlC2	;prev sig handler action
	push	BX

	push	CX				;new signal handler action

	mov	BX,SIG_CTRLC
	push	BX				;signal number of interest
	call	DOSSETSIGHANDLER
	pop	CX

	mov	BX,offset controlCHandler
	push	CS
	push	BX

	push	DS
	mov	BX,offset DGROUP:dummyCBreak1
	push	BX

	push	DS
	mov	BX,offset DGROUP:dummyCBreak2
	push	BX

	push	CX

	mov	BX,SIG_CTRLBREAK
	push	BX

	call	DOSSETSIGHANDLER

	ret
c_endp	controlc_close

	endcode controlc


else ; not __OS2__
ifdef _WINDOWS

	begcode	controlc
	c_public controlc_open
func	controlc_open
c_endp	controlc_open

	c_public controlc_close
func	controlc_close
	ret
c_endp	controlc_close

	endcode controlc

else ; not _WINDOWS

	begdata

cBreak1BVector	dd ?
cBreak23Vector	dd ?
cBreakStatus	db ?

	enddata

	begcode	controlc

    ifdef I8086S
dsvalue	dw	?		;where saved value of DS goes
    endif

cBreakHandler proc far
	sti
	push	AX
	push	BX
	push	CX
	push	DX
	push	BP
	push	SI
	push	DI
	push	ES
	push	DS

	;Load DS with the value of DGROUP
    ifdef I8086S
	mov	DS,CS:dsvalue	;.COM files can't have seg fixups in them
    else
	mov	AX,seg DGROUP
	mov	DS,AX
    endif

    if LCODE
	cmp	word ptr __controlc_handler+2,0
	je	nohandler
	  call	  dword ptr __controlc_handler
    else
	cmp	word ptr __controlc_handler,0
	je	nohandler
	  call	  word ptr __controlc_handler
    endif
nohandler:

	pop	DS
	pop	ES
	pop	DI
	pop	SI
	pop	BP
	pop	DX
	pop	CX
	pop	BX
	pop	AX
	iret
cBreakHandler endp


	c_public controlc_open
func	controlc_open
	push	BP
	push	DS

    ifdef I8086S
	mov	CS:dsvalue,DS		;save so handler can reload DS
    endif

	; get current ^C BREAK STATUS
	mov	AX,03300h
	int	21h
	mov	byte ptr DGROUP:cBreakStatus,DL
	; set current ^C BREAK STATUS TO OFF
	mov	AX,03301h
	mov	DL,1
	int	21h
	; trap int 1b and 23
	push	DI
	push	DS
	mov	AX,0351Bh
	mov	DI,offset DGROUP:cBreak1BVector
	call	installVector
	pop	DS
	mov	AX,03523h
	mov	DI,offset DGROUP:cBreak23Vector
	call	installVector
	pop	DI
	pop	DS
	pop	BP
	ret
c_endp	controlc_open

	c_public controlc_close
func	controlc_close
	push	BP
	push	DS
	push	DS
	push	DS
	lds	DX,dword ptr DGROUP:cBreak23Vector
	mov	AX,02523h
	int	21h
	pop	DS
	lds	DX,dword ptr DGROUP:cBreak1BVector
	mov	AX,0251Bh
	int	21h
	pop	DS
	mov	AX,03301h
	mov	DL,byte ptr DGROUP:cBreakStatus
	int	21h
	pop	DS
	pop	BP
	ret
c_endp	controlc_close

installVector proc near
	int	21h
	mov	[DI],BX
	mov	[DI+2],ES
	push	CS
	pop	DS
	mov	DX,offset CS:cBreakHandler
	mov	AH,025h
	int	21h
	ret
installVector endp

	endcode controlc

endif ; not _WINDOWS
endif ; not __OS2__


	end

