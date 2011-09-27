;_ msmouse.asm	 Fri Dec 22 1989   Modified by: Walter Bright */
; Copyright (C) 1987-1989 by Walter Bright
; All Rights Reserved
; Written by Walter Bright
; Interface to Microsoft Mouse
; $Revision: 1.1.1.1 $

; The mouse coordinate system is left-handed for both text and graphics modes,
; with 0,0 being the upper left corner. Note that the display package
; uses a left-handed coordinate system, but Flash Graphics uses a
; right-handed system.
; Also note that the mouse coordinates in text mode are not in character
; coordinates!(?)
;
; To convert from fg coordinates to mouse coords:
;	mouse_x = fg_x;
;	mouse_y = fg_displaybox[FG_Y2] - fg_y;
; To convert from display (character) coordinates to mouse coords:
;	if (40 column mode)
;	    mouse_x = display_x * 16;
;	else
;	    mouse_x = display_x * 8;
;	mouse_y = display_y * 8;
;
; The Microsoft mouse sometimes gets the number of screen rows wrong in text
; mode, so the recommended method of opening the mouse if the display package
; is also used is:
;	disp_open();			/* initialize display		*/
;	msm_init();			/* initialize mouse		*/
;
;	/* Mouse driver sometimes gets the number of screen rows wrong, */
;	/* so here we force it to whatever disp_open() discovered.	*/
;	msm_setareay(0,(disp_numrows - 1) * 8);
;
;	msm_showcursor();		/* turn mouse cursor on		*/
;
; For more information refer to the Microsoft Mouse User's Guide.

include macros.asm
include x386mac.asm	 ;This is required for callr macro

	begcode_16
	endcode_16

	begdata

msm_inited	dw	0	;set to !=0 if there is a mouse

extrn	__msm_dos386_init:near,__x386_zero_base_selector:word
extrn	__x386_version:word	   ;required for callr macro
ptrtoisr	dd	?	;pointer to C interrupt service routine
stacksp		dd	?	;value of ESP to use.
stacksave	dd	?	;value of stack of msm_isr
stacksaveSS  dw ?   ;stack segment of msm_isr
	enddata

	c_public msm_init,msm_term,msm_showcursor,msm_hidecursor
	c_public msm_getstatus,msm_setcurpos,msm_getpress,msm_getrelease
	c_public msm_setareax,msm_setareay,msm_setgraphcur,msm_settextcur
	c_public msm_readcounters,msm_signal,msm_lightpenon,msm_lightpenoff
	c_public msm_setratio,msm_condoff,msm_setthreshhold

mouse	macro	func
	ifnb	<func>
	 mov	AX,func
	endif
	int	33h
	endm

	begcode msmouse

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	int msm_init(void);
; Initialize mouse driver:
;	o Turn off all mouse interrupts
;	o Turn on light pen emulation
;	o If in graphics mode, set cursor to be an arrow, if in text mode,
;	  set cursor to be inverse video
;	o Set cursor to middle of screen
;	o Turn off cursor display
;	o Set min/max cursor position to full screen
;	o Set mickey/pixel to 1/1 in the x direction and 2/1 in the y
; Returns:
;	0	failed
;	-1	success

func	msm_init
L1:
    ;Some versions of DOS have 0 in the vector table for interrupt 33h
	uses	<EBX,ES>
	mov	ES,__x386_zero_base_selector
	clr	AX
	_ifs	<word ptr ES:0CCh> ne AX, L3
	_ifs	<word ptr ES:0CEh> e AX, L2
L3:	mouse
L2:	mov	msm_inited,AX
	cwde				;convert 16 bit int to 32 bits
	unuse	<ES,EBX>
	ret
c_endp	msm_init

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	void msm_term(void);
; Terminate mouse driver. This should be called before the program is
; exited if the mouse was used.

func	msm_term
	jmp	L1		;the init routine will fix things up
c_endp	msm_term

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	void msm_showcursor(void);
; Show cursor. That is, increment the cursor flag. If the cursor flag is
; 0, then the cursor is displayed. Since msm_init() sets the value of
; the cursor flag to -1, msm_showcursor() must be called after msm_init()
; in order for the cursor to appear.
; Note that showcursor and hidecursor can be nested. That is, if n
; hidecursors were done, then n showcursors must be done in order to
; show the cursor.
; Generally, the point is to remove the cursor before any screen I/O
; is done, and then restore the cursor.

func	msm_showcursor
	_ifs	msm_inited e 0, S1
	mouse	1
S1:	ret
c_endp	msm_showcursor

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	void msm_hidecursor(void);
; Hide cursor. Decrement the cursor flag. Complement to msm_showcursor().

func	msm_hidecursor
	_ifs	msm_inited e 0, H1
	mouse	2
H1:	ret
c_endp	msm_hidecursor

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	int msm_getstatus(unsigned *curposx,unsigned *curposy);
; Get status.
; Output:
;	*curposx,*curposy =	mouse position
; Returns:
;	Bit 0:	left button (1 == down, 0 == up)
;	Bit 1:	right button
;	Bit 2:	middle button (for some mice)
;	Ignore other bits.

func	msm_getstatus
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ECX,EDX>
	clr	ECX
	clr	EDX
	_ifs	msm_inited e 0, G1
	mouse	3
	mov	EAX,EBX
	jmps	msm1

G1:	clr	EAX
	cwd
	mov	ECX,EAX

msm1:	mov	EBX,P[EBP]
	movsx	ECX, CX   ;DJB 23/2/93
	mov	[EBX],ECX
	mov	EBX,P+SIZEPTR[EBP]
	movsx	EDX, DX   ;DJB 23/2/93
	mov	[EBX],EDX
	unuse	<EDX,ECX,EBX>
	pop	EBP
	ret
c_endp	msm_getstatus

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	void msm_setcurpos(unsigned curposx,unsigned curposy);
; Set cursor position.
; The upper left corner of the screen is 0,0. The values for curposx
; and curposy must be within the screen.

func	msm_setcurpos
	mov	AX,4
msm3:	_ifs	msm_inited e 0, S4
	  push	  EBP
	  mov	  EBP,ESP
	  uses	  <ECX,EDX>
	  mov	  ECX,P[EBP]	;DJB 23/2/93
	  mov	  EDX,P+4[EBP]	;DJB 23/2/93
	  mouse
	  unuse   <EDX,ECX>
	  pop	  EBP
S4:	ret
c_endp	msm_setcurpos

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	int msm_getpress(unsigned *count, unsigned *curposx,unsigned *curposy);
; Get button press information.
; Input:
;	*count =		which button are we refering to
;				(0 = left button, 1 = right button, 2 = middle)
; Output:
;	*count =		count of button presses since last call
;				to msm_getpress. Values can be 0..32767.
;	*curposx,*curposy =	mouse position at last press
; Returns:
;	Bit 0:	left button (1 == down, 0 == up)
;	Bit 1:	right button
;	Bit 2:	middle button
;	Ignore other bits

func	msm_getpress
	mov	AX,5		;mouse function
msm2:	push	EBP
	mov	EBP,ESP
	uses	<EBX,ECX,EDX,ESI>
	clr	ECX
	clr	EDX
	mov	EBX,P[EBP]
	movzx	EBX,word ptr [EBX]
	shr	BX,1
	_ifs	msm_inited e 0, G2
	mouse
	jmps	G3

G2:	clr	EAX
	cwd
	mov	EBX,EAX
	mov	ECX,EAX
G3:
	mov	ESI,P[EBP]
	mov	[ESI],EBX		;*count
	mov	ESI,P+SIZEPTR[EBP]
	mov	[ESI],ECX		;*curposx
	mov	ESI,P+(2*SIZEPTR)[EBP]
	mov	[ESI],EDX		;*curposy
	unuse	<ESI,EDX,ECX,EBX>
	pop	EBP
	ret
c_endp	msm_getpress

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	int msm_getrelease(unsigned *count,
;		unsigned *curposx,unsigned *curposy);
; Get button release information.
; Input:
;	*count =		which button are we refering to
;				(0 = left button, 1 = right button, 2 = middle)
; Output:
;	*count =		count of button releases since last call
;				to msm_getpress. Values can be 0..32767.
;	*curposx,*curposy =	mouse position at last release
; Returns:
;	Bit 0:	left button (1 == down, 0 == up)
;	Bit 1:	right button
;	Bit 2:	middle button
;	Ignore other bits

func	msm_getrelease
	mov	AX,6		;mouse function
	jmp	msm2
c_endp	msm_getrelease

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	void msm_setareax(unsigned minx,unsigned maxx);
; Set minimum and maximum horizontal position. If maxx < minx, the values
; are exchanged. The mouse horizontal motion will be restricted to be within
; these values.

func	msm_setareax
	mov	AX,7
	jmp	msm3
c_endp	msm_setareax

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	void msm_setareay(unsigned miny,unsigned maxy);
; Set minimum and maximum vertical position. If maxy < miny, the values
; are exchanged. The mouse vertical motion will be restricted to be within
; these values.

func	msm_setareay
	mov	AX,8
	jmp	msm3
c_endp	msm_setareay

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	void msm_setgraphcur(int hotx,int hoty,int *pmasks);
; Set graphics cursor block.
; Input:
;	hotx,hoty	Location of 'hot spot' of cursor. Values
;			must be -16..16. Location 0,0 is the upper
;			left corner of the cursor, positive values
;			extend right and down.
;	pmasks		Points to 32 words which contain bit masks
;			defining the cursor.
;			The first 16 words define the mask, that is,
;			which bits of the background 'shine' through
;			the cursor. A 1 means shine through, a 0 means not.
;			The second 16 words define the bitmap of the cursor,
;			1 being on and 0 being off.
;			The cursor is 16*16, the first word forms
;			the top row, bit 15 forms the leftmost column.

GC_HOTX EQU	0
GC_HOTY EQU	GC_HOTX+4
GC_MASK EQU	GC_HOTY+4

func	msm_setgraphcur
	_ifs	msm_inited e 0, S2
	  push	  EBP
	  mov	  EBP,ESP
	  uses	  <EBX,ECX,ES>
	  mov	  EBX,P+GC_HOTX[EBP]
	  mov	  ECX,P+GC_HOTY[EBP]
	  push	  DS
	  pop	  ES
	  mov	  EDX,P+GC_MASK[EBP]	;put pointer in ES:EDX
	  mouse   9
	  unuse   <ES,ECX,EBX>
	  pop	  EBP
S2:	ret
c_endp	msm_setgraphcur

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	void msm_settextcur(int select,int scanstart,int scanstop);
; Set text cursor.
; Input:
;	select		If 1, then hardware text cursor. If 0, then
;			attribute cursor.
;	scanstart,
;	scanstop	If select is 1, then these values form the
;			starting and ending scan lines of the hardware
;			text cursor.
;			If select is 0, then these values form the
;			screen mask and cursor mask, respectively, for
;			the attribute cursor.

TC_SELECT	EQU 0
TC_SCANSTART	EQU TC_SELECT+4
TC_SCANSTOP	EQU TC_SCANSTART+4

func	msm_settextcur
	_ifs	msm_inited e 0, S3
	  push	  EBP
	  mov	  EBP,ESP
	  uses	  <EBX,ECX,EDX>
	  mov	  EBX,P+TC_SELECT[EBP]
	  mov	  ECX,P+TC_SCANSTART[EBP]
	  mov	  EDX,P+TC_SCANSTOP[EBP]
	  mouse   10
	  unuse   <EDX,ECX,EBX>
	  pop	  EBP
S3:	ret
c_endp	msm_settextcur

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	void msm_readcounters(int *countx,int *county);
; Read mouse motion counters in mickeys. A mickey is 1/200 of an inch.
; Output:
;	*countx,*county =	mickey count since last call, values
;				range from -32768 to 32767.

func	msm_readcounters
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ECX,EDX>	;DJB 3/2/93
	_ifs	msm_inited e 0, R1
	mouse	11
	jmp	msm1

R1:	jmp	G1
c_endp	msm_readcounters

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	void msm_signal(unsigned mask,
;		void (*func)(unsigned mask,unsigned state,
;			unsigned curposx,unsigned curposy),
;		void *stack);
; Set user-defined subroutine input mask. Used to set a function
; to be called at interrupt level whenever there is input available
; from the mouse. All the caveats apply to the interrupt service routine.
; Using this function is not for the faint-hearted.
; Input:
;	mask	Mask defining when to call func (1 means yes):
;		Bit 0:	mouse moved
;		Bit 1:	left button is pressed
;		Bit 2:	left button is released
;		Bit 3:	right button is pressed
;		Bit 4:	right button is released
;		Bit 5:	middle button is pressed
;		Bit 6:	middle button is released
;		Other bits are not used.
;	func	Pointer to application-defined interrupt service routine
;		to call whenever a mouse button is
;		pressed or released, or the mouse moves, according to the
;		bits in mask.
;	stack	Value to set stack pointer to when func is called. Should
;		point just past end of area that is at least 256 bytes
;		long.
; When func is called, it is passed the following:
;	mask	Event that occured is indicated with the bit set as defined
;		above.
;	state	If button event, this is the button number of the button that
;		changed (0 = left, 1 = right, 2 = middle).
;	curposx,curposy Current mouse position.

func	msm_signal
sig_mask	equ	P[EBP]
sig_func	equ	P + 4[EBP]
sig_stack	equ	P + 8[EBP]
	push	EBP
	mov	EBP,ESP
	uses	<EAX,EBX,ECX,EDX,EDI,ESI>

	mov	EAX,sig_func
	mov	ptrtoisr,EAX
	mov	EAX,sig_stack
	mov	stacksp,EAX

	mov	AX,250Dh	;Get the pointer to the real mode call
	int	21h		;protected mode from real mode routine.
	mov	ESI,EAX		;Pass pointer to real mode call back in ESI.
	lea	EDI,msm_isr	;Pass pointer to isr wrapper in EDI.
	mov	DX,sig_mask	;Pass signal mask in DX
	callr	__msm_dos386_init
	unuse	<ESI,EDI,EDX,ECX,EBX,EAX>
	pop	EBP
	ret
c_endp	msm_signal

msm_isr proc	far
;Under Pharlap DS and ES are already set up to DGROUP.
;We do have to set up a different stack though.
	push	ESI
	mov	stacksave,ESP
	mov	word ptr stacksaveSS,SS
	mov	SI,DS
	mov	SS,SI
	mov	ESP,stacksp
	cld			;no direction flag bugs
	push	EDX		;vertical cursor coordinate
	push	ECX		;horizontal cursor coordinate
	push	EBX		;button state
	push	EAX		;condition mask
	call	ptrtoisr
	mov	SS,word ptr stacksaveSS
	mov	ESP,stacksave
	pop	ESI
	ret
msm_isr endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	void msm_lightpenon(void);
; Light pen emulation mode on. The mouse emulates a light pen, that is,
; the 'pen' is off the screen when both buttons are up, and the 'pen' is
; down when both buttons are down.

func	msm_lightpenon
	_ifs	msm_inited e 0, P1
	mouse	13
P1:	ret
c_endp	msm_lightpenon

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	void msm_lightpenoff(void);
; Light pen emulation mode off.

func	msm_lightpenoff
	_ifs	msm_inited e 0, P1
	mouse	14
	ret
c_endp	msm_lightpenoff

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	void msm_setratio(unsigned ratiox,unsigned ratioy);
; Set mickey/pixel ratio (the sensitivity of the mouse). Higher values
; mean less cursor movement for corresponding mouse movement.
; The default values are 8,16. The values for
; ratiox and ratioy must be 1..32767.

func	msm_setratio
	mov	AX,15
	jmp	msm3
c_endp	msm_setratio

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	void msm_condoff(unsigned upperx, unsigned uppery,
;			 unsigned lowerx, unsigned lowery);
; Conditional off.
; The parameters define a rectangular region on the screen. When the
; mouse is in that region, the mouse cursor is hidden. This is useful
; if a portion of the screen is to be updated. A call to msm_showcursor()
; will undo this and turn the mouse cursor back on.

CO_UPPERX	EQU 0
CO_UPPERY	EQU CO_UPPERX+4
CO_LOWERX	EQU CO_UPPERY+4
CO_LOWERY	EQU CO_LOWERX+4

func	msm_condoff
	_ifs	msm_inited e 0, P1
	push	EBP
	mov	EBP,ESP
	uses	<ECX,EDX,ESI,EDI>
	mov	CX,P+CO_UPPERX[EBP]
	mov	DX,P+CO_UPPERY[EBP]
	mov	SI,P+CO_LOWERX[EBP]
	mov	DI,P+CO_LOWERY[EBP]
	mouse	16
	unuse	<EDI,ESI,EDX,ECX>
	pop	EBP
	ret
c_endp	msm_condoff

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Syntax:
;	void msm_setthreshhold(unsigned speed);
; Set double speed threshhold, i.e. the speed at which the mickey/pixel
; ratio is temporarilly halved so the mouse apparently moves faster.
; Speed is in mickeys/second. The default is 64.

func	msm_setthreshhold
	_ifs	msm_inited e 0, P1
	push	EBP
	mov	EBP,ESP
	uses	<EDX>
	mov	DX,P[EBP]
	mouse	19
	unuse	<EDX>
	pop	EBP
	ret
c_endp	msm_setthreshhold

	endcode msmouse

	end
