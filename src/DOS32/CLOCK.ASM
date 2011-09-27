;_ clock.asm   Thu Jul 13 1989   Modified by: Walter Bright
; Phar lap support added by G. Eric Engstrom
; Copyright (C) 1986-1992 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

include MACROS.ASM

	begdata
starttime       dw      0,0     ;program starting time
	enddata

	begcode clock

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Return the time in 1/1000ths of a second since midnight.
; Useful for timing various things.
; Use:
;       clock_t clock(void);

	c_public clock
func    clock
	uses    <EAX,EBX,ECX,EDX>
	clr     EDX
	clr     ECX
	bdos    2Ch             ;get system time
	; compute ticks*10 + 1000L * (secs + 60L * (mins + 60 * hours))
	;          DL              DH            CL          CH
	movzx   ECX,CX
	movzx   EDX,DX
	push    EDX
	push    EDX
	mov     EBX,60
	mov     EAX,EBX         ;60 min/hr
	mul     CH              ;EAX = 60 * hours
	clr     CH              ;ECX = minutes, i.e. remove hours from ECX
	add     EAX,ECX         ;EAX = mins + (60 * hours)
	mul     EBX             ;EAX = 60 * (mins + (60 * hours)) i.e. convert to seconds

	pop     EBX
	mov     BL,BH
	clr     BH              ;EBX = seconds
	add     EAX,EBX         ;EAX = secs + (60 * (mins + 60 * hours))
	mov     ECX,1000
	mul     ECX             ;EAX = 1000 * (secs + 60 * (mins + 60 * hours))

	pop     EBX
	clr     BH
	imul    EBX,10
	add     EAX,EBX         ;EAX = ticks*10 + (everything else converted to 1000th's of a secound)

	sub     EAX,DWORD PTR starttime ;subtract off time that program started
	jnc     short L1        ;if rolled past midnight

	add     EAX,05265C00h  ;Add in one day's worth of CLK_TCKs (1000 * 60 * 60 * 24)

L1:     unuse   <EDX,ECX,EBX,EAX>
	ret
c_endp  clock

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Initializer for clock(), it sets starttime to time that the program
; was started.

	c_public clock_ctor
func    clock_ctor
	callm   clock
	mov     DWORD PTR starttime,EAX
	ret
c_endp  clock_ctor

; Define clock_ctor as a static constructor, so it is called at
; program startup.

static_ctor     clock_ctor

	endcode clock

	end
