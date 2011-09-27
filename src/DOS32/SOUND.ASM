;_ sound.asm   Fri Oct  2 1987   Modified by: Walter Bright */
; Copyright (C) 1985-1990 by Walter Bright
; All rights reserved
; Written by Walter Bright

include	macros.asm

	begcode	sound

	c_public	sound_beep,sound_click

ppi_port	equ	61h	;8255A-5 PPI port
timer		equ	40h	;timer 8253-5

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Generate a tone on the speaker
; Use:
;	void sound_tone(int cycles,int uptime,int dntime);
;	cycles =	# of cycles
;	uptime =	one half-cycle time
;	dntime =	other half-cycle time
; BUGS:
;	dependent on clock speed of CPU - should be rewritten to use
;	timer chip.

c_public	sound_tone
func	sound_tone
	push	EBP
	mov	EBP,ESP

;	cli
	in	AL,ppi_port
	push	EAX		;save original configuration of port
	mov	EDX,P[EBP]

L3:	and	AL,0FCh
	out	ppi_port,AL	;turn off speaker
	mov	ECX,P+4[EBP]
L1:	loop	L1
	or	AL,2
	out	ppi_port,AL	;turn on speaker
	mov	ECX,P+8[EBP]
L2:	loop	L2
	dec	EDX		;for cycles times
	jne	short L3
	pop	EAX
	out	ppi_port,AL	;restore port to original configuration
;	sti

	pop	EBP
	ret
c_endp	sound_tone


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Beep the speaker.
; Use:
;	void sound_beep(int freq);
;	freq = frequency (1331 = 1000 Hz)
; BUGS:
;	The duration of the beep is dependent on the CPU clock speed.

func	sound_beep
	mov	AL,10110110b	;sel tim 2,lsb,msb,binary
	out	timer+3,AL	;write the timer mode reg
	mov	EAX,PS[ESP]	;get freq
	out	timer+2,AL	;write timer 2 count, lsb
	mov	AL,AH
	out	timer+2,AL	;write timer 2 count, msb
	in	AL,ppi_port	;get current setting of port
	mov	AH,AL		;save it
	or	AL,3		;turn speaker on
	out	ppi_port,AL
	mov	ECX,20000h	;set count to wait ~1 S
B1:	loop	B1
	mov	AL,AH		;original value of port
	out	ppi_port,AL
	ret
c_endp	sound_beep


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Generate a click on the speaker. This is a degenerate version of sound_tone().
; Use:
;	void sound_click();
;

func	sound_click
	in	AL,ppi_port
	push	EAX

	and	AL,0FCh
	out	ppi_port,AL
	mov	ECX,80
C1:	loop	C1
	or	AL,2
	out	ppi_port,AL
	mov	ECX,80
C2:	loop	C2
	pop	EAX
	out	ppi_port,AL
	ret
c_endp	sound_click

	endcode	sound

	end

