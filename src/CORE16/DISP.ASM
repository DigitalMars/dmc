;_ disp.asm   Thu Jul 13 1989	Modified by: Walter Bright
; Windows support added by G. Eric Engstrom Jan 1991	-D_WINDOWS
; NOTE: Win3 support for disp makes it work on the 2nd monitor,
;	it is not a graphics patch.
; OS2 support added by Nikki Locke May 1989		-D__OS2__
; Copyright (C) 1986-1991 by Walter Bright
; All Rights Reserved
; Written by Walter Bright

; Fast screen display package

include macros.asm

video_io macro
    ifndef _WINDOWS
	int	10h
    endif
	endm

;ASCII values
DEL	equ	07Fh		;DEL character

TIMEOUT equ		6
RETRACE equ		250

	begdata

	public	_disp_state
_disp_state	label	byte

	public	_disp_numrows,_disp_numcols
_disp_numrows	dw	25	;# of rows in display
_disp_numcols	dw	0	;# of columns in display


	public	_disp_cursorrow,_disp_cursorcol
_disp_cursorrow dw	?	;row,col of where we want the cursor to be
_disp_cursorcol dw	?

_disp_mono	db	1	;0 for color, 1 for monochrome
_disp_snowycga	db	1	;if 1, then the display is an IBM snowy CGA
_disp_mode	db	?	;what is the current display mode?
_disp_inited	db	0	;1 if display package is opened
_disp_ega	db	0	;1 if IBM EGA
_disp_reserved	db	?,?,?

        public _disp_nowrap
_disp_nowrap    dw      0       ;set to 1 to avoid wrap

_disp_offset	dw	0	;offset portion of far ptr into display
    ifdef __OS2__
_disp_base	dw	0B000h	;segment of base of video screen
				; (default is mono)
_disp_length	dw	0	;length of display buffer
    else
_disp_base	dw	0B000h	;segment of base of video screen
_bios_base	dw	040h
				;if 0, then not available (use BIOS)
    endif

scrnrowcol	label	word	;row,col of actual screen cursor
scrncol db	?
scrnrow db	?

	public	_disp_cursortype
_disp_cursortype dw	?	;start/end scan line of cursor
_disp_cursorcount dw	0	;if 0, cursor is shown

normatt equ	07h	;white chars on black background
stndatt equ	070h	;inverse video
att	db	normatt ;current attribute

	public	_disp_mono,_disp_base,_disp_snowycga,_disp_mode,_disp_inited
	public	_disp_ega,_disp_activepage,_disp_base
_disp_activepage db	0	;which page to read/write
_disp_displaypage db	0	;which page is displayed
cursortype	dw	?	;where we save the cursor type

    ifdef __OS2__
;************** OS/2 stuff ****************
modedata	label	word
m_length	dw	14
m_type	db	0
m_color db	0
m_col	dw	0
m_row	dw	0
m_hres	dw	0
m_vres	dw	0
m_fmt_ID	db	0
m_attrib	db	0

configdata	label	word	;data returned by VIOGETCONFIG
c_length	dw	10	;length of this buffer
c_adapter_type	dw	0
c_display_type	dw	0
c_memory	dd	0	;bytes of memory on video adapter

cursordata	label	byte
cs_row		dw	0
cs_col		dw	0

csrtype		label	byte
ct_start	dw	0	;start line of cursor
ct_end		dw	0	;end line
ct_width	dw	0	;0 = default, 1 = max value in text modes
ct_attribute	dw	0	;0 = visible, -1 = hidden

csrblank	label	byte
bct_start	dw	0
bct_end		dw	0
bct_width	dw	0
bct_attribute	dw	0

old_attribute	dw	0ffffh	;fixed for OS/2 1.3

    endif
	enddata

    ifdef __OS2__
extrn	DOSBEEP:far
extrn	VIOGETBUF:far
extrn	VIOGETMODE:far
extrn	VIOSETMODE:far
extrn	VIOSETCURPOS:far
extrn	VIOGETCURPOS:far
extrn	VIOGETCURTYPE:far
extrn	VIOSETCURTYPE:far
extrn	VIOSHOWBUF:far
extrn	VIOGETCONFIG:far
extrn	VIOWRTNCELL:far
extrn	VIOREADCELLSTR:far
extrn	VIOWRTCELLSTR:far
extrn	VIOSCROLLUP:far
extrn	VIOSCROLLDN:far
    else
    ifdef DOS16RM
extrn	_D16SegAbsolute:far
extrn	_D16SegCancel:far
    endif ;DOS16RM
    endif ;__OS2__

    ifdef _WINDOWS
extrn	MESSAGEBEEP:far
extrn	__cpumode:byte
    endif

	begcode disp


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set things up. That is, determine what display we've got.

	c_public	disp_open
func	disp_open
	_if	_disp_inited e 0, Q4	;if not already initialized
	ret

Q4:	WINENTER
	mov	_disp_inited,1
	.save	DI

    ifdef __OS2__
	xor	ax,ax
	push	ax				;reserved 0
	push	ds
	pushi	BX,<offset DGROUP:configdata>	;address of data
	push	ax				;vio handle
	call	VIOGETCONFIG
	push	ds
	pushi	AX,<offset DGROUP:modedata>
	clr	AX
	push	AX
	call	VIOGETMODE
	cmp	c_adapter_type,2
	jb	L3
	mov	_disp_ega,1	;flag that we have an EGA or better
L3:	mov	_disp_mode,3	;??
	mov	ax,m_col
	mov	_disp_numcols,ax
	mov	ax,m_row
	mov	_disp_numrows,ax;set rows & columns
	mov	al,m_type
	and	al,1		;bit 0 of type = color
	xor	al,1		;reverse the bit
	mov	_disp_mono,al	;so it means mono
	mov	_disp_snowycga,0
	push	ds
	pushi	AX,<offset DGROUP:_disp_offset>
	push	ds
	pushi	AX,<offset DGROUP:_disp_length>
	pushi	AX,0		;vio handle
	call	VIOGETBUF	;get logical display buffer
	push	ds
	pushi	AX,<offset DGROUP:cs_row>
	push	ds
	pushi	AX,<offset DGROUP:cs_col>
	pushi	AX,0		;vio handle
	call	VIOGETCURPOS
	mov	ax,cs_row
	mov	_disp_cursorrow,ax
	mov	scrnrow,al
	mov	ax,cs_col
	mov	_disp_cursorcol,ax
	mov	scrncol,al
	push	ds
	pushi	AX,<offset DGROUP:csrtype>
	pushi	AX,0		;vio handle
	call	VIOGETCURTYPE
	mov	ax,ct_end
	mov	bx,ct_start
	mov	ah,bl
	mov	cursortype,ax
	mov	ax,ct_attribute
	mov	old_attribute,ax

    else
    ifdef _WINDOWS
	mov	BX,0B000h
	mov	_disp_base,BX
	cmp	__cpumode,1
	jne	L21
	  ; DPMI call to get selector to monochrome screen, if in P mode
	  dpmi	  2
	  sbb	  _disp_inited,0
	  mov	  _disp_base,AX
L21:	mov	_disp_numrows,25
	mov	_disp_numcols,80
	mov	_disp_mode,7
	mov	_disp_mono,1
	mov	AX,7
	video_io
	mov	AH,3
	video_io
	mov	cursortype,CX		;save original cursor type
	mov	_disp_cursortype,CX	;current cursor type
	mov	scrnrow,DH
	mov	scrncol,DL
	mov	AL,DH
	cbw
	mov	_disp_cursorrow,AX
	clr	DH
	mov	_disp_cursorcol,DX
    else
	;Determine number of rows, also if we have an EGA
	clr	DX		;default value
	clr	BH
	mov	AX,01130h	;inquire info from EGA BIOS
	video_io
	tst	DL		;Check if DL is still zero
	jz	L3		;if so, no EGA
	mov	_disp_ega,1	;flag that we have an EGA
	mov	_disp_snowycga,0	;EGAs don't snow
	inc	DL
	mov	_disp_numrows,DX	;number of rows
L3:

	mov	AH,15
	video_io
	mov	byte ptr _disp_numcols,AH	;set number of columns
	mov	_disp_mode,AL		;save display mode
	_if	AL ne 7, Q2		;if not mono mode
	mov	_disp_snowycga,0		;mono displays don't snow

	;Do an equipment check to see if this is really a mono card.
	;Note that the COMPAQ can be in mode 7, but still be a color card!
	int	11h			;Equipment Determination BIOS call
	and	AX,  00110000b		;isolate initial video mode bits
	_if	AX e 00110000b, L1	;yes, it is a real mono card
	mov	AL,7			;mode 7
	jmps	L51

Q2:	_if	AL e 15, L51		;if EGA monochrome graphics mode
	mov	_disp_mono,0		;else color display
L51:	mov	_disp_base,0B800h	;base of color adapter
	_if	AL be 3, L1	;if color text mode
	_if	AL e 7, L1	; or mono text mode
	mov	_disp_base,0	;use BIOS instead of direct access to video ram
				; for all graphics and other unrecognized modes
	jmps	L50
L1:
	;Find out if we are running under DESQview.
	;This section provided by Erik Lindberg.
    if 0 ;Not needed
	mov	AX,2B01h	; Get DESQview version (set date)
	mov	CX,'DE'		;     if CX,DX = 'DESQ', is invalid
	mov	DX,'SQ'		;     setdate call.
	int	21h		; DOS interrupt.
	cmp	AL,0FFh		; check for invalid return.
	jz	L2		;   then DESQview is not running.
    endif

	;Determine base address of screen.
	;This code is in compliance with V-Communications' RSIS
	;(Relocated Screen Interface Specification)
	les	DI,dword ptr _disp_offset	; get pre-determined buffer address
	mov	AH,0FEh		; load function code into AH
	video_io		; to request alternate buffer address.
	mov	_disp_base,ES	; and save it.
	mov	_disp_offset,DI
L2:

	;determine where cursor is
L50:	mov	AH,3
	video_io
	mov	cursortype,CX		;save original cursor type
	mov	_disp_cursortype,CX	;current cursor type
	mov	scrnrow,DH
	mov	scrncol,DL
	mov	AL,DH
	cbw
	mov	_disp_cursorrow,AX
	clr	DH
	mov	_disp_cursorcol,DX
    ifdef DOS16RM
	cmp	_disp_base,0
	je	D16_1
	  pushi   AX,0			; create 64K segment
	  clr	  DX
	  mov	  AX,_disp_base
	  mov	  CX,4
	  even				; convert video segment to linear address
D16_:	    shl     AX,1
	    rcl     DX,1
	    loop    D16_
	  push	  DX
	  push	  AX
	  call	  _D16SegAbsolute	; create PM selector for linear address
	  add	  SP,6
	  or	  AX,DX
	  jnz	  D16_0
	    mov     _disp_inited,0	; if no selector then disp not initialized
	    jmp     short D16_1
D16_0:	  mov	  _disp_base,DX		; selector is new video segment
D16_1:
    endif ;DOS16RM
    endif ;_WINDOWS
    endif ;__OS2__

	.restore DI
	WINLEAVE
	ret
c_endp	disp_open

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Close things up

	c_public	disp_close
func	disp_close
	_if	_disp_inited e 0, Q3	;quit if never opened
	push	BP
	callm	disp_flush	;flush all output
    ifdef __OS2__
	mov	AX,cursortype
	mov	BL,AH
	xor	BH,BH
	mov	AH,BH
	mov	ct_end,AX
	mov	ct_start,BX
	mov	AX,old_attribute
	mov	ct_attribute,AX
	push	DS
	pushi	AX,<offset DGROUP:csrtype>
	pushi	AX,0		;vio handle
	call	VIOSETCURTYPE
    else
    ifdef _WINDOWS
	mov	BX,_disp_base
	dpmi	1
    else
	mov	AH,1
	mov	CX,cursortype	;restore original cursor type
	video_io
    ifdef DOS16RM
	cmp	_disp_base,0
	je	D16_2			; if selector was created
	  push	  _disp_base
	  push	  0
	  call	  _D16SegCancel		; return it to the free selector pool
	  add	  SP,4
D16_2:
    endif ;DOS16RM
    endif ;_WINDOWS
    endif ;__OS2__
	mov	_disp_inited,0
	pop	BP
Q3:	ret
c_endp	disp_close

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Be BIOS compatible, instead of poking directly into the screen
; memory.

    ifndef _WINDOWS
	c_public	disp_usebios
func	disp_usebios
	mov	_disp_base,0
	ret
c_endp	disp_usebios
    endif ;_WINDOWS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set cursor type.
; If the cursor is hidden (via calls to disp_hidecursor()), the
; new cursor type will show up when the cursor is redisplayed.
; Use:
;	disp_setcursortype(startline*256 + endline);

	c_public	disp_setcursortype
func	disp_setcursortype
	WINENTER
    ifdef __OS2__
	mov	ax,P[BP]
	mov	_disp_cursortype,AX
	_if	_disp_cursorcount ne 0, SCT1	;if cursor is not displayed
	mov	bl,ah
	xor	bh,bh
	mov	ah,bh
	mov	ct_end,ax
	mov	ct_start,bx
	push	ds
	pushi	AX,<offset DGROUP:csrtype>
	pushi	AX,0				;vio handle
	call	VIOSETCURTYPE
    else
	mov	CX,P[BP]
	mov	_disp_cursortype,CX
	_if	_disp_cursorcount ne 0, SCT1	;if cursor is not displayed
	mov	AH,1
	video_io
    endif ;__OS2__
SCT1:	WINLEAVE
	ret
c_endp	disp_setcursortype

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Hide/show hardware cursor on screen. Calls can be nested.

	c_public	disp_hidecursor,disp_showcursor

func	disp_hidecursor
	inc	_disp_cursorcount
	_if	_disp_cursorcount ne 1, HC1
    ifdef __OS2__
	mov	ax,offset DGROUP:csrblank
HC2:	push	ds
	push	ax
	pushi	AX,0			;vio handle
	call	VIOSETCURTYPE
    else
	mov	CX,02000h		;non-displaying combination
HC2:	WINENTER
    ifndef _WINDOWS
	mov	AH,1
	video_io
    else
    endif
	WINLEAVE
    endif
HC1:	ret
c_endp	disp_hidecursor

func	disp_showcursor
	dec	_disp_cursorcount
	js	_disp_hidecursor	;don't allow it to go negative
	jnz	HC1			;nested, not ready to show yet
    ifdef __OS2__
	mov	ax,offset DGROUP:csrtype
    else
	mov	CX,_disp_cursortype	;original type
    endif
	jmp	HC2
c_endp	disp_showcursor

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Put a character into the display.
; Behave as a dumb terminal.
; Returns character passed.

	c_public	disp_putc
	even
func	disp_putc
	WINENTER
	mov	AL,P[BP]		;char
	_if	AL b ' ', docntl	;control char
	_if	AL z DEL, zip1		;ignore rubouts
doout:	call	near ptr outchr		;display character
	mov	AX,_disp_cursorcol
	inc	AX			;next column
	_if	AX ae _disp_numcols, L19	;if on or past right edge
L20:	mov	_disp_cursorcol,AX
zip1:	clr	AH
	mov	AL,P[BP]
	WINLEAVE
	ret


L19:	
    clr	AX
    _if AX b _disp_nowrap, L185     ;if not wrapping
    mov	BX,_disp_cursorrow		;get row and column
	inc	BX			;start of next row
	_if	BX b _disp_numrows, L18 ;if not past bottom
	call	near ptr scrollup	;scroll up 1 line
	clr	AX
	mov	BX,_disp_numrows		;lower left corner
	dec	BX
L18:	mov	_disp_cursorrow,BX
	jmp	L20
L185:   mov     AX,_disp_cursorcol
        jmp     L20

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Handle control characters

docntl: clr	AH
	mov	BX,AX
	add	BX,AX
	jmp	CS:word ptr (offset cntltab)[BX]

cntltab:
    if LCODE
	dw	doout
	dw	doout
	dw	doout
	dw	doout
	dw	doout
	dw	doout
	dw	doout
	dw	beep,dobs,dotab,donl		;BEL,BS,TAB,LF
	dw	donl,donl			;VT,FF
	dw	docr				;CR
	dw	doout,doout,doout,doout		;P,XON,R,XOFF
	dw	doout,doout,doout,doout,doout,doout,doout
	dw	doout				;escape
	dw	doout,doout,doout,doout,doout,doout
    else
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
	dw	offset _TEXT:beep,offset _TEXT:dobs	;BEL,BS
	dw	offset _TEXT:dotab,offset _TEXT:donl	;TAB,LF
	dw	offset _TEXT:donl,offset _TEXT:donl	;VT,FF
	dw	offset _TEXT:docr			;CR
	dw	offset _TEXT:doout,offset _TEXT:doout
	dw	offset _TEXT:doout,offset _TEXT:doout		;P,XON,R,XOFF
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout				;escape
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
	dw	offset _TEXT:doout
    endif

;;;;;;;;;;;;;;;;;;;;;;
; Bell

beep:
    ifdef __OS2__
	mov	ax,400
	push	ax
	mov	ax,250
	push	ax
	call	DOSBEEP
    else
    ifdef _WINDOWS
		clr	DX
	push	DX
	call	MESSAGEBEEP
    else
	mov	DL,7
	bdos	2			;send a bell to DOS
    endif
    endif
	mov	AX,7
	WINLEAVE
	ret

;;;;;;;;;;;;;;;;;;;;
; Backspace (non-destructive)

dobs:	mov	BX,_disp_cursorcol
	dec	BX			;backup 1 column
	js	L140			;oops! already in column 0
	mov	_disp_cursorcol,BX
L140:	WINLEAVE
	ret

;;;;;;;;;;;;;;;;;;;;
; Carriage return

docr:	mov	_disp_cursorcol,0		;reset column
	WINLEAVE
	ret

;;;;;;;;;;;;;;;;;;;
; Line feed

donl:	mov	_disp_cursorcol,0		;reset column to start of line

dolf:	mov	DX,_disp_cursorrow
	inc	DX
	_if	DX b _disp_numrows, L101
	call	near ptr scrollup
	mov	AX,0Ah
	WINLEAVE
	ret

L101:	mov	_disp_cursorrow,DX
	WINLEAVE
	ret

;;;;;;;;;;;;;;;;;;;
; Tab (non-destructive)

dotab:
	mov	AX,_disp_cursorcol
	or	AL,7
	inc	AX		;advance to next tab stop
	_if	AX b _disp_numcols, D1
	mov	AX,_disp_numcols
	dec	AX
D1:	mov	_disp_cursorcol,AX
	mov	AX,9
	WINLEAVE
	ret

c_endp	disp_putc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Scroll a region of the display.
;	void disp_scroll(lines,ulrow,ulcol,lrrow,lrcol,att);
;	lines > 0:	scroll up
;	lines == 0:	blank window
;	lines < 0:	scroll down

	c_public	disp_scroll
func	disp_scroll
	WINENTER
    ifdef __OS2__
	mov	AX,P[BP]
	mov	dx,ax
	tst	ax
	jns	notneg
	neg	ax
notneg: push	P+2[bp]		;top row
	push	P+4[bp]		;left col
	push	P+6[bp]		;bottom row
	push	P+8[bp]		;right col
	push	ax		;lines
	mov	bh,P+10[bp]	;attribute
	mov	bl,' '		;character
	mov	P+10[bp],bx	;save as a 'cell' pair
	push	ss
	lea	ax,p+10[BP]
	push	ax		;and push the address of the cell
	pushi	AX,0		;vio handle
	tst	dx		;did we want to scroll up or down ?
	jns	vscrollup
	call	VIOSCROLLDN
	jmp short scroll0
vscrollup:
	call	VIOSCROLLUP
scroll0:

    else
    ifdef _WINDOWS
    ;SCC
    else
	mov	AL,P[BP]
	mov	CH,P+2[BP]
	mov	CL,P+4[BP]
	mov	DH,P+6[BP]
	mov	DL,P+8[BP]
	mov	BH,P+10[BP]
	mov	AH,6
	tst	AL
	jns	scroll1
	neg	AL
	inc	AH
scroll1:
	video_io
    endif
    endif ;__OS2__
	WINLEAVE
	ret
c_endp	disp_scroll

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Poke att/char into display at row,column.
;	void disp_pokew(row,col,attchr);

	c_public	disp_pokew
	even
func	disp_pokew
    ifdef _WINDOWS
	_if	_disp_inited ne 0,W4
	  ret
W4:
    endif
	push	BP
	mov	BP,SP

	mov	CX,P+4[BP]	;save att/char
	mov	DH,P[BP]	;row
	mov	DL,P+2[BP]	;column
    ifndef _WINDOWS
	_if	_disp_base e 0, W7
    endif ;_WINDOWS
	pop	BP

	mov	AX,_disp_numcols
	mul	DH
	clr	DH
	add	AX,DX		;add in column
	shl	AX,1		;compute offset into screen
	mov	BX,AX
	mov	ES,_disp_base
    ifdef __OS2__
	add	BX,_disp_offset
	mov	ES:[BX],CX		;poke it
	ret
    else
    ifndef _WINDOWS
	_if	_disp_snowycga ne 0, W1 ;if need snow check
    endif ;_WINDOWS
	mov	ES:[BX],CX		;poke it
	ret
    ifndef _WINDOWS
W1:	mov	DX,03DAh		;color card status port
	mov	AH,1

W5:	in	AL,DX			;wait for retrace low
	test	AL,AH
	jnz	W5
	cli				;turn off interrupts
W6:	in	AL,DX			;wait for retrace high
	test	AL,AH
	jz	W6

	mov	ES:[BX],CX		;poke it
	sti				;enable interrupts
	ret
    endif ;_WINDOWS
    endif ;__OS2__

W7: ifndef _WINDOWS
    ifdef __OS2__
	push	ss
	lea	ax,P+4[bp]
	push	ax		;address of cell
	mov	AX,1
	push	AX		;1 character
	push	P[BP]		;row
	push	P+2[BP]		;column
	dec	AX
	push	AX		;vio handle (0)
	call	VIOWRTNCELL
    else
	mov	scrnrowcol,DX
	mov	BH,_disp_activepage	;page number
	mov	AH,2
	video_io		;set cursor position (BP is destroyed)
	mov	AL,CL		;char
	mov	BL,CH		;attribute to use
	mov	AH,9		;write att/char at current cursor position
	mov	CX,1		;write one character
	video_io
    endif
	pop	BP
	ret
    endif ;_WINDOWS
c_endp	disp_pokew

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Read att/char from display at row,column.
;	unsigned disp_peekw(row,col);

	c_public	disp_peekw
func	disp_peekw
    ifdef _WINDOWS
	_if	_disp_inited ne 0,_W4
	  ret
_W4:
    endif
	push	BP
	mov	BP,SP

	mov	DH,P[BP]	;row
	mov	DL,P+2[BP]	;column
    ifndef _WINDOWS
	_if	_disp_base e 0, W8
    endif ;_WINDOWS
	pop	BP

	mov	AX,_disp_numcols
	mul	DH
	clr	DH
	add	AX,DX		;add in column
	shl	AX,1		;compute offset into screen
	mov	BX,AX
	mov	ES,_disp_base
    ifdef __OS2__
	add	BX,_disp_offset
	mov	AX,ES:[BX]		;read it
	ret
    else
    ifndef _WINDOWS
	_if	_disp_snowycga ne 0, W9 ;if need snow check
    endif ;_WINDOWS
	mov	AX,ES:[BX]		;read it
	ret
    ifndef _WINDOWS
W9:	mov	DX,03DAh		;color card status port
	mov	AH,1

W10:	in	AL,DX			;wait for retrace low
	test	AL,AH
	jnz	W10
	cli				;turn off interrupts
W11:	in	AL,DX			;wait for retrace high
	test	AL,AH
	jz	W11

	mov	AX,ES:[BX]		;read it
	sti				;enable interrupts
	ret
    endif ;_WINDOWS
    endif

W8:
    ifndef _WINDOWS
    ifdef __OS2__
	push	ss
	lea	ax,P[bp]
	push	ax		;address of cell
	mov	word ptr P+2[bp],1	;length = 1
	inc	ax
	inc	ax		;ax points to length
	push	ss
	push	ax		;address of length word
	push	P[BP]		;row
	push	P+2[BP]		;column
	pushi	AX,0		;vio handle
	call	VIOREADCELLSTR
	mov	ax,P+[bp]	;get cell contents
    else
	mov	scrnrowcol,DX
	mov	BH,_disp_activepage	;page number
	mov	AH,2
	video_io		;set cursor position (BP is destroyed)
	mov	AH,8		;read att/char at current cursor position
	video_io
    endif
	pop	BP
	ret
    endif ;_WINDOWS
c_endp	disp_peekw

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Display character in AL.

	even
outchr	proc	near
    ifndef _WINDOWS
	_if	_disp_base e 0, P7
    endif ;_WINDOWS
	mov	CL,AL
	mov	CH,att		;attribute to use
	mov	AX,_disp_numcols
	mul	_disp_cursorrow
	add	AX,_disp_cursorcol
	shl	AX,1		;compute offset into screen
	mov	BX,AX
	mov	ES,_disp_base
    ifdef __OS2__
	mov	ES:[BX],CX		;poke it
	ret
    else
    ifndef _WINDOWS
	_if	_disp_snowycga ne 0, P1 ;if need snow check
    endif ;_WINDOWS
	mov	ES:[BX],CX		;poke it
	ret
    ifndef _WINDOWS
P1:	mov	DX,03DAh		;color card status port
	mov	AH,1
P5:	in	AL,DX			;wait for retrace low
	test	AL,AH
	jnz	P5
	cli				;turn off interrupts
P6:	in	AL,DX			;wait for retrace high
	test	AL,AH
	jz	P6

	mov	ES:[BX],CX		;poke it
	sti				;enable interrupts
	ret
    endif ;_WINDOWS
    endif
    ifndef _WINDOWS
P7:
    ifdef __OS2__
	mov	ah,att		;attribute
	push	ax		;push on to stack
	mov	ax,sp		;address in ax
	push	ss
	push	ax		;address of cell
	mov	AX,1
	push	AX		;1 character
	push	_disp_cursorrow ;row
	push	_disp_cursorcol ;column
	dec	AX
	push	AX		;vio handle (0)
	call	VIOWRTNCELL
	pop	ax		;tidy cell off stack
    else
	push	AX
	callm	disp_flush	;bring cursor up to date
	pop	AX
	mov	AH,9		;write att/char at current cursor position
	mov	BH,_disp_activepage	;page number
	mov	BL,att		;attribute to use
	mov	CX,1		;write one character
	video_io
    endif
	ret
    endif ;_WINDOWS
outchr	endp
comment |
monoInit    proc near
	clr	AX
	mov	BX,offset monoTable
	mov	CX,offset monoTableSize
M10:	mov	AL,AH
	out	DX,AL
	inc	DX
	inc	AH
	mov	AL,[BX]
	out	DX,AL
	inc	BX
	dec	DX
	loop	M10
	ret
monoInit    endp


writeTo6845 proc near
	mov	DX,03B4h
	out	DX,AL
	inc	DX
	mov	AL,CH
	out	DX,AL
	dec	DX
	mov	AL,14
	out	DX,AL
	inc	DX
	mov	AL,CL
	out	DX,AL
	ret
writeTo6845 endp
	|
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set cursor position
;	void disp_move(unsigned row,unsigned col);

	c_public	disp_move
func	disp_move
    ifndef _WINDOWS
	push	BP
	mov	BP,SP
	mov	AX,P[BP]
	mov	_disp_cursorrow,AX
	mov	AX,P+2[BP]
	mov	_disp_cursorcol,AX
	pop	BP
	ret
    else
	push	BP
	mov	BP,SP
	mov	AX,P[BP]	; AX = line
	mov	_disp_cursorrow,AX
  ;	 mov	CL,80
  ;	 mul	CL		; multiply by characters in row
	mov	CX,P+2[BP]	; add columns to result
  ;	 add	AX,CX
	mov	_disp_cursorcol,CX
  ;	 mov	CX,AX		; move total to CX
  ;	 mov	AH,14		; 6845 register number for cursor
  ;	 call	writeTo6845
	pop	BP
	ret
    endif
c_endp	disp_move

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Flush output.
; What this does is set the hardware cursor (scrnxxx) to be where
; the software cursor (disp_cursorxxx) is. This is only done when
; disp_flush() is called because it is a relatively slow operation.

	c_public	disp_flush
func	disp_flush
	mov	DH,byte ptr _disp_cursorrow
	mov	DL,byte ptr _disp_cursorcol
	_if	DX e scrnrowcol, F1
	mov	scrnrowcol,DX
    ifdef __OS2__
	push	_disp_cursorrow
	push	_disp_cursorcol
	pushi	AX,0		;video handle
	call	VIOSETCURPOS
F1:
	xor	ax,ax
	push	ax		;start update at offset 0
	push	_disp_length	;do whole screen
	push	ax		;vio handle
	call	VIOSHOWBUF
    else
	WINENTER
	mov	BH,_disp_activepage
	mov	AH,2		;set cursor function
	video_io
	WINLEAVE
F1:
    endif
	ret
c_endp	disp_flush


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Delete to end of line (including cursor position)

	c_public	disp_eeol
func	disp_eeol
	mov	CX,_disp_numcols
	sub	CX,_disp_cursorcol	;CX = # of spaces left in line
	mov	AX,_disp_numcols
	mul	_disp_cursorrow
	add	AX,_disp_cursorcol
	jmps	clear1
c_endp	disp_eeol

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Delete to end of screen (including cursor position)

	c_public	disp_eeop
func	disp_eeop
	mov	AX,_disp_numcols
	mul	_disp_numrows
	mov	CX,AX		;CX = # of chars on screen
	mov	AX,_disp_numcols
	mul	_disp_cursorrow
	add	AX,_disp_cursorcol
	sub	CX,AX
clear1:
	.save	DI
	shl	AX,1
	mov	DI,AX
;	jmps	clear
c_endp	disp_eeop

;;;;;;;;;;;;;;;;;;;;;;;;
; Send CX spaces to screen starting at DI.

func	clear
    ifndef _WINDOWS
	_if	_disp_base e 0, C7
    endif
	mov	ES,_disp_base
	cld
	mov	AL,' '
	mov	AH,att
    ifndef __OS2__
    ifndef _WINDOWS
	_if	_disp_snowycga ne 0, C1
    endif
    endif
	rep	stosw
	.restore DI
	ret

    ifndef _WINDOWS
    ifndef __OS2__
C1:	mov	DX,03DAh		;color card status port
	mov	BX,AX

C5:	in	AL,DX			;wait for retrace low
	test	AL,1
	jnz	C5
	cli				;turn off interrupts
C6:	in	AL,DX			;wait for retrace high
	test	AL,1
	jz	C6

	mov	AX,BX
	stosw				;poke it
	sti				;enable interrupts
	loop	C5

	.restore DI
	ret
    endif
C7:
    ifdef __OS2__
	mov	ax,' '		;att/char = cell to write
	push	ax
	mov	ax,sp		;address of where we pushed ax
	push	ss
	push	ax		;address of cell
	push	cx		;no of cells to write
	push	_disp_cursorrow
	push	_disp_cursorcol
	pushi	AX,0		;vio handle
	call	VIOWRTNCELL
	pop	ax		;tidy stack
    else
	callm	disp_flush	;bring cursor up to date
	mov	AX,9*256+' '	;write att/char at current cursor position
	mov	BH,_disp_activepage	;page number
	mov	BL,att		;attribute to use
	push	BP
	video_io
	pop	BP
    endif
	.restore DI
	ret
    endif ;_WINDOWS
c_endp	clear

;;;;;;;;;;;;;;;;;;;;;;;;
; Start/end standout mode.
; Set attribute for subsequent writes.

	c_public	disp_startstand,disp_endstand
	c_public	disp_setattr,disp_getattr

func	disp_startstand
	mov	att,stndatt
	ret
c_endp	disp_startstand

func	disp_endstand
	mov	att,normatt
	ret
c_endp	disp_endstand

func	disp_setattr
	push	BP
	mov	BP,SP
	mov	AL,P[BP]
	mov	att,AL
	pop	BP
	ret
c_endp	disp_setattr

func	disp_getattr
	mov	AL,att
	xor	AH,AH
	ret
c_endp	disp_getattr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Scroll up 1 line

scrollup proc	near
    ifndef _WINDOWS
    ifndef __OS2__
	_if	_disp_snowycga ne 0, biosway	;give up and use bios
    endif
	_if	_disp_base e 0, biosway
    else
	_if	_disp_inited ne 0,_SOK
	  ret
_SOK:
    endif ;_WINDOWS
	push	DS
	.save	<SI,DI>
	mov	AX,_disp_numcols
	mov	SI,AX
	mov	BX,AX
	mul	_disp_numrows
	sub	AX,SI
	mov	CX,AX			;CX = # of words to scroll
	shl	SI,1			;source is 1 row in
    ifdef __OS2__
	mov	DI,_disp_offset
	add	SI,DI
    else
	clr	DI
    endif
	mov	AL,' '			;char for blank row
	mov	AH,att			;attribute for blank row
	mov	ES,_disp_base		;point to video segment
	mov	DS,_disp_base
	cld				;increment
	rep movsw			;do the scroll
	mov	CX,BX			;# of words in bottom row
	rep stosw			;blank bottom row
	.restore <DI,SI>
	pop	DS
	ret
    ifndef _WINDOWS
biosway:
    ifdef __OS2__
	mov	bh,att		;attribute
	mov	bl,' '		;character
	push	bx
	mov	bx,sp		;address of cell to write at bottom
	xor	ax,ax
	push	ax		;top row
	push	ax		;left col
	mov	ax,_disp_numrows
	push	ax		;bottom row
	mov	ax,_disp_numcols
	dec	ax
	push	ax		;right col
	mov	AX,1
	push	AX		;lines
	push	ss
	push	bx		;and push the address of the cell
	dec	AX
	push	AX		;vio handle (0)
	call	VIOSCROLLUP
	pop	bx		;tidy up stack
    else
	clr	CX			;row,col of upper left corner
	mov	DX,_disp_numcols	;col of lower right corner
	dec	DX
	mov	DH,24
	mov	BH,att			;att for blank
	mov	AX,0601h		;scroll up 1 line
	video_io
    endif
	ret
    endif ;_WINDOWS
scrollup endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get and return the current video mode.

	c_public	disp_getmode
func	disp_getmode
	push	BP
    ifdef __OS2__
	mov	al,_disp_mode
    else
	mov	AH,15
	video_io
    endif
	clr	AH
	pop	BP
	ret
c_endp	disp_getmode

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set the video mode.
; Do not use while disp package is open.

	c_public	disp_setmode
func	disp_setmode
	push	BP
	mov	BP,SP
    ifndef __OS2__
	clr	AH
	mov	AL,P[BP]
	video_io
    endif
	pop	BP
	ret
c_endp	disp_setmode

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set into 43 line mode.
; Do not use while disp package is open.

    ifndef _WINDOWS
	c_public	disp_set43
func	disp_set43
	callm	disp_open		;set global variables
	callm	disp_close
	_if	_disp_ega ne 1, S1
	_if	_disp_numrows ne 25, S1
    ifdef __OS2__
	mov	m_row,43	;to 43 lines
S5:	push	DS
	pushi	AX,<offset DGROUP:modedata>
	pushi	AX,0
	call	VIOSETMODE
	mov	DX,m_row
	dec	DX
    else
	push	BP		;some BIOS's trash BP
	mov	AX,0003h	;set color mode, 80x43
	_if	_disp_mono e 0, S2
	mov	AL,07h		;set monochrome mode, 80x43
S2:	mov	_disp_mode,AL		;save display mode
	video_io
	mov	AX,01112h	;character generator BIOS routine
	mov	BL,0		;8x8 double dot character font
	video_io
	mov	AX,01200h	;alternate screen routine
	mov	BL,020h		;alternate print screen routine
	video_io
	;Must set cursor to be a block else it will disappear
	mov	AX,0100h
	mov	CX,0007h	;block cursor
	video_io		;set cursor type

S3:	mov	AX,01130h	;inquire info from EGA BIOS
	video_io
	pop	BP
	clr	DH
    endif
	_if	DX ae _disp_cursorrow, S4
	mov	_disp_cursorrow,DX	;reset cursor so it's on the screen
S4:	inc	DX
	mov	_disp_numrows,DX	;set new # of rows
S1:	ret
c_endp	disp_set43
    endif ;_WINDOWS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Switch from 43 line mode back to 25 line mode.

    ifndef _WINDOWS
	c_public	disp_reset43
func	disp_reset43
	callm	disp_open		;set global variables
	callm	disp_close
	_if	_disp_ega ne 1, S1
	_if	_disp_numrows be 25, S1
    ifdef __OS2__
	mov	m_row,25	;to 25 lines
	jmp	S5
    else
	push	BP
	mov	AX,0003		;color, 80x25
	_if	_disp_mono e 0, R2
	mov	AX,0007		;mono, 80x25
R2:	mov	_disp_mode,AL	;save display mode
	video_io
	mov	AX,01101h	;character generator BIOS routine
	mov	BL,0
	video_io
	mov	AX,01200h
	mov	BL,020h
	video_io
	clr	AX
	mov	AL,_disp_mode	;reset video mode to clean up odd characters
	video_io		;gee 5-20-90
	jmp	S3		;determine # of rows
    endif ;__OS2__
	ret
c_endp	disp_reset43
    endif ;_WINDOWS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	disp_peekbox		Assembly language routine by Dave Mansell
;				February, 18th 1989
;  Synopsis:
;
;	void disp_peekbox(unsigned *buffer,
;		unsigned trow,unsigned lcol,unsigned brow,unsigned rcol);
;
;	Moves char/attr pairs from screen memory to buf avoiding screen snow if a
;	cga is present, uses both horizontal and vertical retrace intervals.
;  Where:
;	buffer	Pointer to array of unsigneds. Into each unsigned
;		will be placed the corresponding attr/char word.
;		The words are stored sequentially from left to right
;		and top to bottom. The number of unsigneds required
;		is ((brow - trow + 1) * (rcol - lcol + 1)).
;	trow,lcol	Top left corner
;	brow,rcol	Bottom right corner

	c_public disp_peekbox
func  disp_peekbox
	push	BP
	mov	BP,SP
	push	DS
	push	ES
	push	DI
	push	SI
IF LPTR
	mov	DH,P+4[BP]			;top row
	mov	DL,P+6[BP]			;left column
	mov	BX,P+8[BP]			;bottom row
	sub	BL,DH				;BX = no of rows
	inc	Bx
	mov	CX,P+10[BP]			;right column
	sub	CL,DL				;CX = no of columns
	inc	CX
	les	DI,P[BP]			;pointer to buf in ES:DI
ELSE
	mov	DH,P+2[BP]			;top row
	mov	DL,P+4[BP]			;left column
	mov	BX,P+6[BP]			;bottom row
	sub	BL,DH				;BX = no of rows
	inc	BX
	mov	CX,P+8[BP]			;right column
	sub	CL,DL				;CX = no of columns
	inc	CX
	push	DS
	pop	ES
	mov	DI,P[BP]			;buf (source offset)
ENDIF
    ifndef _WINDOWS
	_if _disp_base e 0,pkk			;use bios
    endif ;_WINDOWS
	mov	AX,_disp_numcols
	push	AX
	mul	DH
	xor	DH,DH
	add	AX,DX
	shl	AX,1
	mov	SI,AX
    ifdef __OS2__
	add	SI,_disp_offset
    endif
	pop	DX
	sub	DX,CX
	shl	DX,1
	mov	DH,_disp_snowycga
	mov	AX,_disp_base			;screen segment
	mov	DS,AX

pkb0:	push	BX				;no of rows
	push	DX				;offset to next row (and snow flag)
	push	CX				;no of cols
    ifndef __OS2__
	_if DH e 1,pkb1				;if snow is a problem jump
    endif
	rep	movsw				;otherwise just do copy
	jmp	short pkb99
    ifndef _WINDOWS
pkk:	jmp	peekbios
    endif
    ifndef __OS2__
pkb1:	mov	DX,3DAh				;CGA status port
	even
pkb2:	mov	BX,CX
	mov	CX,TIMEOUT
	cli					;clear interrupts
pkb3:	in	AL,DX				;read the port
	test	AL,1				;loop while display enable
	loopnz	pkb3				;is not active
	jz	pkb4				;jump if not timed out
	movsw					;copy one word (char & attr)
	sti					;re-enable interrupts
	mov	CX,BX				;restore CX = no of bytes
	loop	pkb2				;no more bytes so exit
	jmp	short pkb99

pkb4:	sti
	mov	CX,BX				;restore CX = no of bytes
pkb5:	push	CX				;save loop counter
	mov	CX,TIMEOUT			;timeout loop limit
	cli					;clear interrupts
pkb6:	in	AL,DX
	test	AL,1
	loopnz	pkb6
	jnz	pkb8
pkb7:	in	AL,DX
	test	AL,1
	jz	pkb7
	lodsw					;AL = char   AH = attr
	sti
	stosw
	pop	CX
	loop	pkb5
	jmp short pkb99
pkb8:	pop	BX
	mov	CX,RETRACE
	cmp	BX,CX
	jnb	pkb9
	mov	CX,BX
	xor	BX,BX
	jmp	short pkb10
pkb9:	sub	BX,CX
pkb10:	rep	movsw
	mov	CX,BX
	test	CX,CX
	jnz	pkb2
    endif

pkb99:	pop	CX
	pop	DX
	mov	BX,DX
	xor	DH,DH
	add	SI,DX
	mov	DX,BX
	pop	BX
	dec	BX
	jnz	pkb0
	cld
	pop	SI
	pop	DI
	pop	ES
	pop	DS
	pop	BP
	ret
    ifndef _WINDOWS
peekbios:
	mov	BH,_disp_displaypage
peeklp1:
	push	CX				;save the # of columns
	push	DX
    ifdef __OS2__
	push	di
	push	bx				;save no of rows
	push	cx				;length byte
	mov	ax,sp				;pointer to it
	push	es
	push	di				;CellStr
	push	ss
	push	ax				;@Length
	push	P+SIZEPTR[BP]			;Row
	push	P+SIZEPTR+2[BP]			;Column
	pushi	AX,0				;VioHandle
	call	VIOREADCELLSTR
	inc	word ptr P+SIZEPTR[BP]		;Row
	pop	ax				;actual length read
	pop	bx				;restore no of rows
	pop	di				;and buffer position
	pop	dx
	pop	cx
	add	di,cx				;add in length
	add	di,cx				;in words
	dec	bx				;more rows left ?
	jnz	peeklp1
    else
peeklp2:
	mov	AX,0200h
	video_io				;position the cursor
	mov	AX,0800h
	video_io				;read one word (char & attr)
	stosw					;store in ES:DI
	inc	DL				;next column
	loop	peeklp2				;loop until row finished
	pop	DX
	pop	CX
	inc	DH				;next row
	dec	BL				;all rows done?
	jnz	peeklp1				;go and do the next row
    endif
	pop	SI				;tidy up and exit
	pop	DI
	pop	ES
	pop	DS
	pop	BP
	ret
    endif
c_endp disp_peekbox

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  Name:
;	disp_pokebox		Assembly language routine by Dave Mansell
;				February, 18th 1989
;  Synopsis:
;
;	void disp_pokebox(unsigned *buffer,
;		unsigned trow,unsigned lcol,unsigned brow,unsigned rcol);
;
;  Description
;	Moves char/attr pairs from buf to screen memory avoiding screen snow if a
;	cga is present, uses both horizontal and vertical retrace intervals.

	c_public disp_pokebox
func disp_pokebox
	push	BP
	mov	BP,SP
	push	DS
	push	ES
	push	DI
	push	SI
IF LPTR
	mov	DH,P+4[BP]			;top row
	mov	DL,P+6[BP]			;left column
	mov	BX,P+8[BP]			;bottom row
	sub	BL,DH				;BX = no of rows
	inc	BX
	mov	CX,P+10[BP]			;right column
	sub	CL,DL				;CX = no of columns
	inc	CX
	les	SI,P[BP]			;pointer to buf in ES:SI
ELSE
	mov	DH,P+2[BP]			;top row
	mov	DL,P+4[BP]			;left column
	mov	BX,P+6[BP]			;bottom row
	sub	BL,DH				;BX = no of rows
	inc	BX
	mov	CX,P+8[BP]			;right column
	sub	CL,DL				;CX = no of columns
	inc	CX
	push	DS
	pop	ES				;temporary storage for DS
	mov	SI,P[BP]			;buf (source offset)
ENDIF
    ifndef _WINDOWS
	_if _disp_base e 0,pbk			;use bios
    endif
	mov	AX,_disp_numcols
	push	AX
	mul	DH
	xor	DH,DH
	add	AX,DX
	shl	AX,1
	mov	DI,AX
    ifdef __OS2__
	add	di,_disp_offset
    endif
	pop	DX
	sub	DX,CX
	shl	DX,1
	mov	DH,_disp_snowycga
	mov	AX,_disp_base			;screen segment
	push	ES
	pop	DS
	mov	ES,AX
pb0:	push	BX
	push	DX
	push	CX
    ifndef __OS2__
	_if DH e 1,pb1				;if snow is a problem jump
    endif
	rep	movsw				;otherwise just do copy
	jmp short pb99
    ifndef _WINDOWS
pbk:	jmp	pokebios
    endif
    ifndef __OS2__
pb1:	mov	DX,3DAh				;CGA status port
pb2:	mov	BX,CX
	mov	CX,TIMEOUT
	cli					;clear interrupts
pb3:	in	AL,DX				;read the port
	test	AL,1				;loop while display enable
	loopnz	pb3				;is not active
	jz	pb4				;jump if not timed out
	movsw					;copy one word (char & attr)
	sti					;re-enable interrupts
	mov	CX,BX				;restore CX = no of bytes
	loop	pb2				;no more bytes so exit
	jmp short pb99
pb4:	sti
	mov	CX,BX				;restore CX = no of bytes
pb5:	lodsw					;AL = char   AH = attr
	mov	BX,AX				;get into BX
	push	CX				;save loop counter
	mov	CX,TIMEOUT			;timeout loop limit
	cli					;clear interrupts
pb6:	in	AL,DX
	test	AL,1
	loopnz	pb6
	jnz	pb8
pb7:	in	AL,DX
	test	AL,1
	jz	pb7
	xchg	AX,BX
	stosw
	sti
	pop	CX
	loop	pb5
	jmp short pb99
pb8:	pop	BX
	dec	SI
	dec	SI
	mov	CX,RETRACE
	cmp	BX,CX
	jnb	pb9
	mov	CX,BX
	xor	BX,BX
	jmp short pb10
pb9:	sub	BX,CX
pb10:	rep	movsw
	mov	CX,BX
	test	CX,CX
	jnz	pb2
    endif

pb99:	pop	CX
	pop	DX
	mov	BX,DX
	xor	DH,DH
	add	DI,DX
	mov	DX,BX
	pop	BX
	dec	BX
	jnz	pb0
	cld
	pop	SI
	pop	DI
	pop	ES
	pop	DS
	pop	BP
	ret
    ifndef _WINDOWS
pokebios:
	mov	BH,_disp_displaypage	;display page
pokelp1:
	push	CX			;save the # of columns
	push	BX
	push	DX
    ifdef __OS2__
	push	bx				;save no of rows
	push	es
	push	di				;CellStr
	push	cx				;Length
	push	P+SIZEPTR[BP]			;Row
	push	P+SIZEPTR+2[BP]			;Column
	pushi	AX,0				;VioHandle
	call	VIOWRTCELLSTR
	inc	word ptr P+SIZEPTR[BP]		;Row
	pop	ax				;actual length read
	pop	bx				;restore no of rows
	pop	di				;and buffer position
	pop	dx
	pop	cx
	add	di,cx				;add in length
	add	di,cx				;in words
	dec	bx				;more rows left ?
	jnz	pokelp1
    else
pokelp2:push	CX
	mov	AX,0200h
	video_io			;position the cursor
	mov	AX,ES:[SI]		;get character and attribute
	mov	BL,AH			;attribute
	mov	CX,1
	mov	AH,09h
	video_io			;write one word (char & attr)
	inc	DL			;next column
	inc	SI			;next char/attr
	inc	SI
	pop	CX
	loop	pokelp2			;loop until row finished
	pop	DX
	pop	BX
	pop	CX
	inc	DH			;next row
	dec	BL			;all rows done?
	jnz	pokelp1			;go and do the next row
    endif
	pop	SI			;tidy up and exit
	pop	DI
	pop	ES
	pop	DS
	pop	BP
	ret
    endif ;_WINDOWS
c_endp disp_pokebox

	endcode disp

	end
