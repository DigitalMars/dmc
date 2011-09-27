;_ realmode.asm   Wed Jan  2 1991
; Copyright (C) 1991 by Walter Bright
; All Rights Reserved

; Pharlap requires that any real mode code be linked to the
; beginning of the .EXP file.  If you need your own
; real mode code and use FG or msm_signal() then you will need REALMODE.OBJ.
; It is best if it is the first object module specified to the linker.
; These requirements can be ignored with the X386 extender.

include macros.asm
include x386mac.asm

begcode_16
;NOTE!!!!!!!!!!!!!!!!!!!!!!!
;In the case of Pharlap this MUST be before all real mode data.
	public	__realmode_start
__realmode_start   label   byte

assume cs:__X386_CODESEG_16,ds:__X386_CODESEG_16
;cs and ds both have the code segment in all of the following code

;;;;;;;;;;;;;;;;;;;;;;;;;
; The real mode side of the mouse interrupt server.
; December 31, 1990
;;;;;;;;;;;;;;;;;;;;;;;;;

msm_isr_wrapper_ptr	dd	?	;Pointer to the protected mode wrapper
					;function for the mouse signal routine.
__msm_dos386_call_prot	dd	?

msm_rserver   proc    far
	push	DS		;Save DS.
	push	CS		;Setup DS so dos386_call_prot pointer is valid.
	pop	DS

	push	EAX		;Save general registers.  High order words
	push	EBX		;may not be saved by real mode mouse driver.
	push	ECX
	push	EDX

	push	dword ptr 0		;No seg reg parameter block.
	push	word ptr 0ch
;CS of protected mode server for pharlap, this is ignored in the X386 
;extender, the default 32 bit code seg is used independant of this parameter.

	push	msm_isr_wrapper_ptr	;The protected mode server offset.
	movzx	EAX,AX			;Get the value of condition mask.
	movzx	EBX,BX			;The button state.
	movsx	ECX,CX			;Cursor horizontal location.
	movsx	EDX,DX			;Cursor vertical location.
	call	[__msm_dos386_call_prot]
	add	SP,10 

	pop	EDX
	pop	ECX
	pop	EBX
	pop	EAX

	pop	DS
	ret
msm_rserver	endp

;;;;;;;;;;;;;;;;;;;;;;;;;
; Initialize the mouse interrupt handler.
; DX has the call mask to use.
; EDI has protected mode address of interrupt server wrapper function.
; ESI has real mode call back address
; December 31, 1990
;;;;;;;;;;;;;;;;;;;;;;;;;

	public	__msm_dos386_init
__msm_dos386_init proc	  far
	;Set up the pointer to the wrapper for the isr.
	mov	msm_isr_wrapper_ptr,EDI
	;Set up pointer to real mode call back
	mov	__msm_dos386_call_prot,ESI
	mov	CX,DX		;Move the call mask into proper register.
	mov	AX,CS
	mov	ES,AX		;ES:DX -> _fg_dos386_msm_rserver
	lea	DX,msm_rserver
	mov	AX,12
	int	33h
	ret
__msm_dos386_init endp

;NOTE!!!!!!!!!!!!!!!!!!!!!!!
;In the case of Pharlap, this MUST be after all real code.
	public	__realmode_end
__realmode_end	   label   byte

endcode_16

	end

