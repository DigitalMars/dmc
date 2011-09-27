;_ clock.asm   Thu Jul 13 1989   Modified by: Walter Bright
; Windows support added by G. Eric Engstrom Jan 1991    -D_WINDOWS
; Optimizations made to OS2 G. Eric Engstrom Jan 1991
; OS2 support added by Nikki Locke May 1989             -D__OS2__
; Copyright (C) 1986-1989 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

include MACROS.ASM

	begdata
starttime       dw      0,0     ;program starting time
	enddata

    ifdef __OS2__
extrn   DOSGETDATETIME:far

; structure returned by DOSGETDATETIME - offsets into stack frame
hour            equ     -12
minute          equ     -11
second          equ     -10
hundr           equ     -9
day             equ     -8
month           equ     -7
year            equ     -6
timez           equ     -4
wday            equ     -2
structsize      equ     12
    endif

	begcode clock

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Return the time in 1/100ths of a second since program start.
; Useful for timing various things.
; Use:
;       clock_t clock(void);

	c_public clock
func    clock
    ifdef __OS2__
	push    BP
	mov     BP,SP
	sub     SP,structsize   ;reserve space for structure
	push    SS              ;segment of structure
	lea     AX,hour[BP]     ;offset of structure
	push    AX
	call    DOSGETDATETIME
	mov     DX,second[BP]
	xchg    DL,DH
	mov     CX,hour[BP]
	xchg    CL,CH
	add     SP,structsize   ;tidy up stack
	pop     BP              ;and restore BP
    else
	WINENTER
	bdos    2Ch             ;get system time
	WINLEAVE
	; compute ticks + 100L * (secs + 60L * (mins + 60 * hours))
	;          DL              DH            CL          CH
    endif
	push    DX
	push    DX
	mov     BX,60
	mov     AL,BL           ;60 min/hr
	mul     CH              ;AX = 60 * hours
	clr     CH              ;CX = minutes
	add     AX,CX           ;AX = mins + 60 * hours
	mul     BX              ;DXAX = 60 * (mins + 60 * hours)

	pop     BX
	mov     BL,BH
	clr     BH              ;BX = seconds
	add     AX,BX
	adc     DX,0            ;DXAX = secs + 60L * (mins + 60 * hours)

	mov     BX,AX
	mov     AX,DX

	mov     CX,1000
	mul     CX
	xchg    AX,BX
	mul     CX
	add     DX,BX           ;DXAX = 1000L * (secs + 60L * (mins + 60 * hours))

	pop     BX
	clr     BH
	sal     BX,1 
	add     AX,BX
	adc     DX,0
	sal     BX,1
	sal     BX,1
	add     AX,BX
	adc     DX,0            ;DXAX += 1000th of a second


	sub     AX,starttime
	sbb     DX,starttime[2] ;subtract off time that program started
	jc      L1              ;rolled past midnight
	ret

L1:     ;Add in one day's worth of CLK_TCKs (1000 * 60 * 60 * 24)
	add     AX,05C00h
	adc     DX,00526h       ;I don't trust MASM and math
	ret
c_endp  clock

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Initializer for clock(), it sets starttime to time that the program
; was started.

func    clock_ctor
	callm   clock
	mov     starttime,AX
	mov     starttime[2],DX
	ret
c_endp  clock_ctor

; Define clock_ctor as a static constructor, so it is called at
; program startup.

static_ctor     clock_ctor

	endcode clock

	end
