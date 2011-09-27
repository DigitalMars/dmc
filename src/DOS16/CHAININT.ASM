;_ chainint.asm
; Copyright 1993 by Digital Mars
; All Rights Reserved
; $Revision: 1.1.1.1 $

	include macros.asm
	include stdio.asm
	
    ifndef _WINDOWS
   
	begcode chainint
	c_public _chain_intr

;****************************************************************************
; void __CLIB _chain_intr (void (__far * __interrupt funcptr)());
;****************************************************************************

;----------------------------------------------------------------------------
;------	stack frame variables -----------------------------------------------
;----------------------------------------------------------------------------

old_ES = 0
old_DS = 2
old_DI = 4
old_SI = 6
old_BP = 8
old_SP = 10
old_BX = 12
old_DX = 14
old_CX = 16
old_AX = 18

new_IP = old_CX
new_CS = old_AX

;----------------------------------------------------------------------------
;------ No new stack frame is necessary, so we don't set one up. ------------
;----------------------------------------------------------------------------

func	_chain_intr

;----------------------------------------------------------------------------
;------ Toss the return address.  We will not be returning. -----------------
;----------------------------------------------------------------------------

    if LCODE
	add	SP,4
    else
	pop	AX
    endif

;----------------------------------------------------------------------------
;------ Load the parameter funcptr (the location of the routine we will -----
;-------- jump to) into DX:BX by popping it off of the stack. ---------------
;----------------------------------------------------------------------------

	pop	BX			; offset comes off the stack first
	pop	DX
	
;----------------------------------------------------------------------------
;------ Restore AX and CX to the values they had at the time of the ---------
;-------- original interrupt.  We will make use below of the space this -----
;-------- frees up.  Note that BP still points to the stack frame -----------
;-------- originally set up on entry to the interrupt routine. --------------
;----------------------------------------------------------------------------

	mov	AX,[BP].old_AX
	mov	CX,[BP].old_CX
	
;----------------------------------------------------------------------------
;------ Save the address of the new interrupt routine on the stack, in ------
;-------- place of the saved AX and CX values.  Nowhere else will do, since -
;-------- the stack segment is the only writable segment which will be ------
;-------- addressable just before the jump. ---------------------------------
;----------------------------------------------------------------------------

	mov	[BP].new_IP,BX
	mov	[BP].new_CS,DX
	
;----------------------------------------------------------------------------
;------ Toss the caller's stack frame.  The registers saved at the time of --
;-------- the interrupt are now on the top of the stack. --------------------
;----------------------------------------------------------------------------

	mov	SP,BP
	
;----------------------------------------------------------------------------
;------ Now restore the rest of the registers. ------------------------------
;----------------------------------------------------------------------------
	
	pop	ES
	pop	DS
	pop	DI
	pop	SI
	pop	BP
	pop	BX	; toss the saved SP value
	pop	BX
	pop	DX
	
;----------------------------------------------------------------------------
;------ The new handler will expect interrupts to be disabled. --------------
;----------------------------------------------------------------------------

	cli
	
;----------------------------------------------------------------------------
;------ Since we have placed the address of the new handler on the stack, ---
;-------- the retf below will take us there. --------------------------------
;----------------------------------------------------------------------------

	retf
c_endp	_chain_intr
	endcode chainint
    endif	
	end
