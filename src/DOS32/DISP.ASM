;_ disp.asm
; Phar Lap support added by G. Eric Engstrom Oct 1990
; Overlapping window support added by G. Eric Engstrom Oct 1989
; Copyright (C) 1986-1990 by Northwest Software
; All Rights Reserved
; Written by Walter Bright
; Modified by Richard S. Sadowsky July, 1993
;
; Fast screen display package
;
; $Revision: 1.1.1.1 $

include macros.asm

ifndef M_UNIX
ifdef M_XENIX
M_UNIX	equ	M_XENIX		;XENIX is handled the same as UNIX.
endif
endif

;ASCII values
DEL	equ	07Fh		;DEL character

TIMEOUT equ		6
RETRACE equ		250

	begdata

ifdef M_UNIX
;;;;;;;;;;;;;;;;;;;;;;;;;
; Converts the byte arguement (a number less than 100) to an ascii number in
; AH,AL.
; January 23, 1991
;;;;;;;;;;;;;;;;;;;;;;;;;
ascii	macro	b
	ifnb	<b>
	  mov	  AL,b
	endif
	aam
	add	AX,'00'
	endm

video_io macro	cmd
	push	size_&cmd		;;Size of the command.
	push	offset DGROUP: cmd	;;The text of the command.
	push	1			;;stdout
	call	_write
	add	ESP,12
	endm
else	;M_UNIX
video_io macro
	int	10h
	endm
endif	;M_UNIX

	public	_disp_state
_disp_state	label	byte

	public	_disp_numrows,_disp_numcols
_disp_numrows	dd	25	;# of rows in display
_disp_numcols	dd	00	;# of columns in display

 	public	_disp_cursorrow,_disp_cursorcol
_disp_cursorrow dd	0	;row,col of where we want the cursor to be
_disp_cursorcol dd	0

ifdef M_UNIX
	public	_disp_scomode, _disp_vt_number, _disp_offset, _disp_captured

_disp_scomode	db	?
_disp_captured	db	1
_disp_vt_number dd	-1
endif

_disp_mono	db	1	;0 for color, 1 for monochrome
_disp_snowycga	db	1	;if 1, then the display is an IBM snowy CGA
_disp_mode	db	?	;what is the current display mode?
_disp_inited	db	0	;1 if display package is opened
_disp_ega	db	0	;1 if IBM EGA
_disp_reserved	db	?,?,?

   public _disp_nowrap
_disp_nowrap    dw      0       ;set to 1 to avoid wrap


_disp_offset	dd	0	;offset portion of far ptr into display
_disp_base	dw	0b000h	;monochrome is default

scrnrowcol	label	word	;row,col of actual screen cursor
scrncol db	?
scrnrow db	?

	public	_disp_cursortype
_disp_cursortype  dd	0	;start/end scan line of cursor
_disp_cursorcount dd	0	;if 0, cursor is shown

normatt equ	07h	;white chars on black background
stndatt equ	070h	;inverse video
att	db	normatt ;current attribute

	public	_disp_mono,_disp_base,_disp_snowycga,_disp_mode,_disp_inited
	public	_disp_ega,_disp_activepage,_disp_base
_disp_activepage  db	0	;which page to read/write
_disp_displaypage db	0	;which page is displayed
cursortype	dw	?	;where we save the cursor type

	enddata

	begcode disp

extrn	__get_video_selector:near

	c_public	disp_open
func	disp_open
	_ifs	_disp_inited e 0, Q4	;if not already initialized
	  ret

Q4:	mov	_disp_inited,1
	push	EBP
	uses	<EAX,EBX,EDX,EDI>

	;Determine number of rows, also if we have an EGA
	clr	EDX		;default value
	clr	EBX
	mov	AX,01130h	;inquire info from EGA BIOS
	video_io
	tst	DL		;Check if DL is still zero
	jz	L3		;if so, no EGA

	  mov	  _disp_ega,1	  ;flag that we have an EGA
	  mov	  _disp_snowycga,0	  ;EGAs don't snow
	  inc	  DL
	  mov	  word ptr _disp_numrows,DX	  ;number of rows

L3:	mov	AH,15
	video_io
	mov	byte ptr _disp_numcols,AH	;set number of columns
	mov	_disp_mode,AL		;save display mode
	_ifs	AL ne 7, Q2		;if not mono mode

	  mov	  _disp_snowycga,0		  ;mono displays don't snow

	  ;Do an equipment check to see if this is really a mono card.
	  ;Note that the COMPAQ can be in mode 7, but still be a color card!
	  int	  11h			  ;Equipment Determination BIOS call
	  and	  AX,  00110000b	  ;isolate initial video mode bits
	  _ifs	  AX e 00110000b, L1	  ;yes, it is a real mono card

	    mov     AL,7		    ;mode 7
	    jmps    L51

Q2:	    _ifs    AL e 15, L51	    ;if EGA monochrome graphics mode
	      mov     _disp_mono,0	    ;else color display

L51:
	  mov	  _disp_base,0b800h	;base of color adapter
	  _ifs	  AL be 3, L1	  ;if color text mode
	  _ifs	  AL e 7, L1	  ; or mono text mode
	  mov	  _disp_base,0	  ;use BIOS instead of direct access to video ram
				  ; for all graphics and other unrecognized modes
	  jmps	  L50
L1:
	;Find out if we are running under DESQview.
	;This section provided by Erik Lindberg.
	mov	AX,2B01h	; Get DESQview version (set date)
	mov	CX,'DE'		;     if CX,DX = 'DESQ', is invalid
	mov	DX,'SQ'		;     setdate call.
	int	21h		; DOS interrupt.
	cmp	AL,0FFh		; check for invalid return.
	jz	L2		;   then DESQview is not running.
	  mov	  _disp_base,0	  ;use BIOS when running under DESQview
L2:
	;determine where cursor is
L50:	mov	AH,3
	video_io
	mov	cursortype,CX		;save original cursor type
	mov	word ptr _disp_cursortype,CX	;current cursor type
	mov	scrnrow,DH
	mov	scrncol,DL
	mov	AL,DH
	cbw
	mov	word ptr _disp_cursorrow,AX
	clr	DH
	mov	word ptr _disp_cursorcol,DX
	mov	ax,_disp_base			;real mode segment
	call	__get_video_selector		;convert segment to selector
	mov	_disp_base,ax			;protected mode selector
	unuse	<EDI,EDX,EBX,EAX>
	pop	EBP
	ret
c_endp	disp_open

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Close things up

	c_public	disp_close
func	disp_close
	_ifs	_disp_inited e 0, Q3	;quit if never opened
	  push	  EBP
	  uses	  <ECX>
	  callm   disp_flush
	  mov	  AH,1
	  mov	  CX,cursortype   ;restore original cursor type
	  video_io
	  mov	  _disp_inited,0
	  unuse   <ECX>
	  pop	  EBP
Q3:	ret
c_endp	disp_close

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Be BIOS compatible, instead of poking directly into the screen
; memory.

	c_public	disp_usebios
func	disp_usebios
	mov	_disp_base,0
	ret
c_endp	disp_usebios

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set cursor type.
; If the cursor is hidden (via calls to disp_hidecursor()), the
; new cursor type will show up when the cursor is redisplayed.
; Use:
;	disp_setcursortype(startline*256 + endline);

	c_public	disp_setcursortype
func	disp_setcursortype
	push	EBP
	mov	EBP,ESP
	uses	<ECX>
	mov	CX,P[EBP]
	mov	word ptr _disp_cursortype,CX
	_ifs	_disp_cursorcount ne 0, SCT1	;if cursor is not displayed
	  mov	  AH,1
	  video_io
SCT1:	unuse	<ECX>
	pop	EBP
	ret
c_endp	disp_setcursortype

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Hide/show hardware cursor on screen. Calls can be nested.

	c_public	disp_hidecursor,disp_showcursor

func	disp_hidecursor
	inc	_disp_cursorcount
	_ifs	_disp_cursorcount ne 1, HC1
	  mov	  CX,02000h		  ;non-displaying combination
HC2:	  push	  EBP
	  mov	  AH,1
	  video_io
	  pop	  EBP
HC1:	ret
c_endp	disp_hidecursor

func	disp_showcursor
	dec	_disp_cursorcount
	jnz	HC1			;nested, not ready to show yet
	  mov	  CX,word ptr _disp_cursortype
	  jmp	  HC2
c_endp	disp_showcursor

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Put a character into the display.
; Behave as a dumb terminal.
; Returns character passed.

	c_public	disp_putc
func	disp_putc
	push	EBP
	mov	EBP,ESP
	uses	<EBX,EDX>
	clr	EAX
	mov	AL,P[EBP]
	_ifs	AL b ' ',docntl		;control char
	_ifs	AL z DEL,zip1		;ignore rubouts
doout:	call	near ptr outchr		;display character
	mov	EAX,_disp_cursorcol
	inc	EAX			;next column
	_ifs	EAX ae _disp_numcols, L19	;if on or past right edge
L20:	mov	_disp_cursorcol,EAX
zip1:	unuse	<EDX,EBX>
	clr	EAX
	mov	AL,P[EBP]
	pop	EBP
	ret

L19:
    clr	EAX
    _if EAX b _disp_nowrap, L185   ; if not wrapping
	mov	EBX,_disp_cursorrow		;get row and column
	inc	EBX			;start of next row
	_ifs	EBX b _disp_numrows, L18	;if not past bottom
	call	near ptr scrollup	;scroll up 1 line
	clr	EAX
	mov	EBX,_disp_numrows		;lower left corner
	dec	EBX
L18:
    mov	_disp_cursorrow,EBX
	jmp	L20
L185: 
    mov EAX,_disp_cursorcol
    jmp L20

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Handle control characters

docntl: movzx	EAX,AL
	mov	EBX,EAX
	shl	EBX,2
	jmp	dword ptr CS:(offset cntltab)[EBX]

cntltab:
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout
	dd	offset _TEXT:beep,offset _TEXT:dobs	;BEL,BS
	dd	offset _TEXT:dotab,offset _TEXT:donl	;TAB,LF
	dd	offset _TEXT:donl,offset _TEXT:donl	;VT,FF
	dd	offset _TEXT:docr			;CR
	dd	offset _TEXT:doout,offset _TEXT:doout
	dd	offset _TEXT:doout,offset _TEXT:doout		;P,XON,R,XOFF
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout				;escape
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout
	dd	offset _TEXT:doout

;;;;;;;;;;;;;;;;;;;;;;
; Bell

beep:
	mov	DL,7
	bdos	2			;send a bell to DOS
	clr	EAX
	mov	AL,7
	jmp	short L140

;;;;;;;;;;;;;;;;;;;;
; Backspace (non-destructive)

dobs:	mov	EBX,_disp_cursorcol
	dec	EBX			;backup 1 column
	js	L140			;oops! already in column 0
	  mov	  _disp_cursorcol,EBX
L140:	unuse	<EDX,EBX>
	pop	EBP
	ret

;;;;;;;;;;;;;;;;;;;;
; Carriage return

docr:	mov	_disp_cursorcol,0		;reset column
	jmp	short L140

;;;;;;;;;;;;;;;;;;;
; Line feed

donl:	mov	_disp_cursorcol,0		;reset column to start of line

dolf:	mov	EDX,_disp_cursorrow
	inc	EDX
	_ifs	EDX b _disp_numrows, L101
	  call	  near ptr scrollup
	  clr	  EAX
	  mov	  AL,0Ah
	  jmp	  short L140
L101:	mov	_disp_cursorrow,EDX
	jmp	short L140

;;;;;;;;;;;;;;;;;;;
; Tab (non-destructive)

dotab:
	mov	AX,word ptr _disp_cursorcol
	or	AX,7
	inc	EAX		;advance to next tab stop
	_ifs	AX b <word ptr _disp_numcols>, D1
	mov	AX,word ptr _disp_numcols
	dec	EAX
D1:	mov	word ptr _disp_cursorcol,AX
	clr	EAX
	mov	AL,9
	jmp	short L140
c_endp	disp_putc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Scroll a region of the display.
;	void disp_scroll(lines,ulrow,ulcol,lrrow,lrcol,att);
;	lines > 0:	scroll up
;	lines == 0:	blank window
;	lines < 0:	scroll down

PSC_LINECOUNT	EQU 0
PSC_LINE	EQU PSC_LINECOUNT+4
PSC_COLUMN	EQU PSC_LINE+4
PSC_BOTTOMLINE	EQU PSC_COLUMN+4
PSC_RIGHTCOLUMN EQU PSC_BOTTOMLINE+4
PSC_ATTRIBUTE	EQU PSC_RIGHTCOLUMN+4

	c_public	disp_scroll
func	disp_scroll
	push	EBP
	mov	EBP,ESP
	uses	<EAX,EBX,ECX,EDX>
	mov	AL,P+PSC_LINECOUNT[EBP]
	mov	CH,P+PSC_LINE[EBP]
	mov	CL,P+PSC_COLUMN[EBP]
	mov	DH,P+PSC_BOTTOMLINE[EBP]
	mov	DL,P+PSC_RIGHTCOLUMN[EBP]
	mov	BH,P+PSC_ATTRIBUTE[EBP]
	mov	AH,6
	tst	AL
	jns	scroll1
	  neg	  AL
	  inc	  AH
scroll1:
	video_io
	unuse	<EDX,ECX,EBX,EAX>
	pop	EBP
	ret
c_endp	disp_scroll

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Poke att/char into display at row,column.
;	void disp_pokew(row,col,attchr);

POKE_ROW	equ	0
POKE_COL	equ	POKE_ROW+4
POKE_ATTCHR	equ	POKE_COL+4

	c_public	disp_pokew
func	disp_pokew
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ECX,EDX,ES>

	clr	EAX
	movzx	ECX,word ptr P+POKE_ATTCHR[EBP] ;save att/char
	movzx	EDX,byte ptr P+POKE_COL[EBP]	;column
	mov	DH,P+POKE_ROW[EBP]	;row
	_ifs	_disp_base e 0, W7

	  movzx   EAX,word ptr _disp_numcols
	  mul	  DH
	  clr	  DH
	  add	  EAX,EDX	  ;add in column
	  shl	  EAX,1		  ;compute offset into screen
	  mov	  EBX,EAX
	  mov	  ES,_disp_base
	  _ifs	  _disp_snowycga ne 0, W1A	  ;if dont skip snow check

W1:	    mov     ES:[EBX],CX		  ;poke it
	    sti				  ;enable interrupts
W8:	    unuse   <ES,EDX,ECX,EBX>
	    pop     EBP
	    ret

W1A:	  mov	  DX,03DAh		  ;color card status port
	  mov	  AH,1

W5:	  in	  AL,DX			  ;wait for retrace low
	  test	  AL,AH
	  jnz	  W5
	  cli				  ;turn off interrupts
W6:	  in	  AL,DX			  ;wait for retrace high
	  test	  AL,AH
	  jz	  W6
	  jmp	  short W1

W7:	mov	scrnrowcol,DX
	mov	BH,_disp_activepage	;page number
	mov	AH,2
	video_io		;set cursor position (BP is destroyed)
	mov	AL,CL		;char
	mov	BL,CH		;attribute to use
	mov	AH,9		;write att/char at current cursor position
	mov	CX,1		;write one character
	video_io
	jmp	short W8
c_endp	disp_pokew

LBP_COLUMN	EQU	0
LBP_ROW		EQU	LBP_COLUMN+4
LBP_X		EQU	LBP_ROW+4
LBP_Y		EQU	LBP_X+4
LBP_LEVEL	EQU	LBP_Y+4
LBP_LEVELMAP	EQU	LBP_LEVEL+4
LBP_LEVELMAPX	EQU	LBP_LEVELMAP+SIZEPTR
LBP_VIEWARRAY	EQU	LBP_LEVELMAPX+4
LBP_VIEWARRAYX	EQU	LBP_VIEWARRAY+SIZEPTR

	c_public	disp_levelblockpoke
func	disp_levelblockpoke
	_ifs	_disp_inited ne 0,lbp0
	  ret
lbp0:	push	EBP
	mov	EBP,ESP
	uses	<EBX,ECX,EDX,ESI,EDI,ES>

	movzx	EDX,byte ptr P+LBP_ROW[EBP]	;row
	mov	EAX,_disp_numcols
	mul	DX
	movzx	EDX,byte ptr P+LBP_COLUMN[EBP]	;column
	add	EAX,EDX		;add in column
	shl	EAX,1		;xlat from bytes to words
	mov	EDI,EAX
	mov	ES,_disp_base	;es:edi is screen ram

	mov	EAX,_disp_numcols
	sub	AX,P+LBP_X[EBP]
	shl	EAX,17			;compute video ptr offset to next line

	mov	ESI,P+LBP_LEVELMAP[EBP]		;esi points at level map
	mov	EBX,P+LBP_VIEWARRAY[EBP]	;ebx points to data being transferred
	mov	CX,P+LBP_Y[EBP]			;lines in block being transferred
	mov	DX,P+LBP_LEVEL[EBP]		;dx = level being transferred
	cld

	mov	AX,word ptr P+LBP_X[EBP]		;columns in block being transferred
	push	EAX		;create local variable used by this loop
E_1:	  mov	  EAX,[ESP]
	  shl	  ECX,16
	  mov	  CX,DX
	  mov	  EDX,ECX	;save line count in upper 16bits of EDX
	  movzx   ECX,AX	;restore loop counter for columns
E_2:	    lodsw
	    cmp     AX,DX	  ;if levelMap <= level being transferred
	    ja	    E_3
	      mov     AX,[EBX]	    ;get char & attr to be displayed
	      stosw		    ;update video
	      mov     -2[ESI],DX    ;update level map
	      inc     EBX
	      inc     EBX	    ;advance data ptr to next char
	      loop    E_2
	      jmp     short E_5
E_3:	    inc     EBX
	    inc     EBX
	    inc     EDI
	    inc     EDI
	    loop    E_2
E_5:	  mov	  EAX,[ESP]
	  shr	  EAX,16

	  add	  EDI,EAX
	  movzx   EAX,word ptr P+LBP_VIEWARRAYX[EBP]	;advance data ptr to next data line
	  add	  EBX,EAX
	  movzx   EAX,word ptr P+LBP_LEVELMAPX[EBP]	;advance level map ptr to next levelmap line
	  add	  ESI,EAX

	  mov	  ECX,EDX
	  shr	  ECX,16	;restore loop ctr for lines in block
	  loop	  E_1
	pop	EAX		;destroy local variable
	unuse	<ES,EDI,ESI,EDX,ECX,EBX>
	pop	EBP
	ret
c_endp	disp_levelblockpoke

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Read att/char from display at row,column.
;	unsigned disp_peekw(row,col);

PK_LINE		EQU 0
PK_COLUMN	EQU PK_LINE+4

	c_public	disp_peekw
func	disp_peekw
	push	EBP
	mov	EBP,ESP
	uses	<EBX,EDX,ES>

	mov	DH,P+PK_LINE[EBP]		;row
	mov	DL,P+PK_COLUMN[EBP]		;column
	_ifs	_disp_base e 0, W8A

	clr	EBX
	mov	AX,word ptr _disp_numcols
	mul	DH
	clr	DH
	add	AX,DX		;add in column
	shl	AX,1		;compute offset into screen
	mov	BX,AX
	mov	ES,_disp_base
	_ifs	_disp_snowycga ne 0, W9A ;if dont skip snow check

W9:	  mov	  AX,ES:[EBX]		;read it
	  sti				;enable interrupts
	  unuse   <ES,EDX,EBX>
	  pop	  EBP
	  ret

W9A:	mov	DX,03DAh		;color card status port
	mov	AH,1

W10:	in	AL,DX			;wait for retrace low
	test	AL,AH
	jnz	W10
	cli				;turn off interrupts
W11:	in	AL,DX			;wait for retrace high
	test	AL,AH
	jz	W11
	jmp	short W9

W8A:	mov	scrnrowcol,DX
	mov	BH,_disp_activepage	;page number
	mov	AH,2
	video_io		;set cursor position (BP is destroyed)
	mov	AH,8		;read att/char at current cursor position
	video_io
	unuse	<ES,EDX,EBX>
	pop	EBP
	ret
c_endp	disp_peekw

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Display character in AL.

outchr	proc	near
	_ifs	_disp_base e 0, P7

	mov	CL,AL
	mov	CH,att		;attribute to use
	mov	EAX,_disp_numcols
	mul	_disp_cursorrow
	add	AX,word ptr _disp_cursorcol
	shl	EAX,1		;compute offset into screen
	mov	EBX,EAX
	mov	ES,_disp_base
	_ifs	_disp_snowycga ne 0, P1A;if dont skip snow check

P1:	  mov	  ES:[EBX],CX		;poke it
	  sti				;enable interrupts
	  ret

P1A:	mov	DX,03DAh		;color card status port
	mov	AH,1
P5:	in	AL,DX			;wait for retrace low
	test	AL,AH
	jnz	P5
	cli				;turn off interrupts
P6:	in	AL,DX			;wait for retrace high
	test	AL,AH
	jz	P6
	jmp	short P1

P7:	push	EAX
	callm	disp_flush	;bring cursor up to date
	pop	EAX
	mov	AH,9		;write att/char at current cursor position
	mov	BH,_disp_activepage	;page number
	mov	BL,att		;attribute to use
	mov	CX,1		;write one character
	video_io
	ret
outchr	endp


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set cursor position
;	void disp_move(unsigned row,unsigned col);

	c_public	disp_move
func	disp_move
	mov	EAX,PS[ESP]
	mov	_disp_cursorrow,EAX
	mov	EAX,PS+4[ESP]
	mov	_disp_cursorcol,EAX
	ret
c_endp	disp_move

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Flush output.
; What this does is set the hardware cursor (scrnxxx) to be where
; the software cursor (disp_cursorxxx) is. This is only done when
; disp_flush() is called because it is a relatively slow operation.

	c_public	disp_flush
func	disp_flush
	uses	<EBX,EDX>
	mov	DH,byte ptr _disp_cursorrow
	mov	DL,byte ptr _disp_cursorcol
	_ifs	DX e scrnrowcol, F1

	  mov	  scrnrowcol,DX
	  mov	  BH,_disp_activepage
	  mov	  AH,2		  ;set cursor function
	  push	  EBP
	  video_io
	  pop	  EBP

F1:	unuse	<EDX,EBX>
	ret
c_endp	disp_flush


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Delete to end of line (including cursor position)

	c_public	disp_eeol
func	disp_eeol
	uses	<ECX>
	mov	ECX,_disp_numcols
	sub	ECX,_disp_cursorcol	;CX = # of spaces left in line
	mov	EAX,_disp_numcols
	mul	_disp_cursorrow
	add	EAX,_disp_cursorcol
	jmps	clear1
c_endp	disp_eeol

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Delete to end of screen (including cursor position)

	c_public	disp_eeop
func	disp_eeop
	uses	<ECX>
	mov	EAX,_disp_numcols
	mul	_disp_numrows
	mov	ECX,EAX		;CX = # of chars on screen
	mov	EAX,_disp_numcols
	mul	_disp_cursorrow
	add	EAX,_disp_cursorcol
	sub	ECX,EAX
clear1:
	uses	<EDI>
	shl	AX,1
	movzx	EDI,AX
	jmps	aclear
c_endp	disp_eeop

;;;;;;;;;;;;;;;;;;;;;;;;
; Send CX spaces to screen starting at DI.
; expect EDI,ECX <USED> on stack
; i.e. this routine will unuse them in that order

	c_public aclear
func	aclear
	uses	<EBX,EDX,ES>
	_ifs	_disp_base e 0, C7
	  mov	  ES,_disp_base
	  cld
	  mov	  AL,' '
	  mov	  AH,att

	  _ifs	  _disp_snowycga ne 0, C1
	    rep     stosw
	    jmp     short C8		;exit tail merge

C1:	_ifs	_disp_base e 0, C7
	  mov	  DX,03DAh		  ;color card status port
	  mov	  BX,AX

C5:	      in      AL,DX		    ;wait for retrace low
	      test    AL,1
	      jnz     C5

	    cli				  ;turn off interrupts

C6:	      in      AL,DX		    ;wait for retrace high
	      test    AL,1
	      jz      C6

	    mov     AX,BX
	    stosw				  ;poke it
	    sti				  ;enable interrupts
	    loop    C5
	  jmp	  short C8		;exit tail merge

C7:	callm	disp_flush	;bring cursor up to date
	mov	AX,9*256+' '	;write att/char at current cursor position
	mov	BH,_disp_activepage	;page number
	mov	BL,att		;attribute to use
	push	EBP
	video_io
	pop	EBP

C8:	unuse	<ES,EDX,EBX,EDI,ECX>
	ret
c_endp	aclear

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
	mov	AL,PS[ESP]
	mov	att,AL
	ret
c_endp	disp_setattr

func	disp_getattr
	clr	EAX
	mov	AL,att
	ret
c_endp	disp_getattr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Scroll up 1 line

scrollup proc	near
	_ifs	_disp_snowycga ne 0, biosway	;give up and use bios
	_ifs	_disp_base e 0, biosway

	  uses	  <EBX,ECX,EDX,ESI,EDI>
	  mov	  EAX,_disp_numcols
	  mov	  ESI,EAX
	  mov	  EBX,EAX
	  mul	  _disp_numrows
	  sub	  EAX,ESI
	  mov	  ECX,EAX		  ;CX = # of words to scroll
	  shl	  ESI,1			  ;source is 1 row in
	  clr	  EDI
	  push	  ES
	  push	  DS
	  mov	  AL,' '		  ;char for blank row
	  mov	  AH,att		  ;attribute for blank row
	  mov	  ES,_disp_base		  ;point to video segment
	  mov	  DS,_disp_base
	  cld				  ;increment
	  rep movsw			  ;do the scroll
	  mov	  ECX,EBX		  ;# of words in bottom row
	  rep stosw			  ;blank bottom row
	  pop	  DS
	  pop	  ES
	  unuse   <EDI,ESI,EDX,ECX,EBX>
	  ret

biosway:
	clr	ECX			;row,col of upper left corner
	mov	EDX,_disp_numcols	;col of lower right corner
	dec	EDX
	mov	DH,24
	mov	BH,att			;att for blank
	mov	AX,0601h		;scroll up 1 line
	video_io
	ret
scrollup endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get and return the current video mode.

	c_public	disp_getmode
func	disp_getmode
	push	EBP
	push	EBX
	mov	AH,15
	video_io
	movzx	EAX,AL
	pop	EBX
	pop	EBP
	ret
c_endp	disp_getmode

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set the video mode.
; Do not use while disp package is open.

	c_public	disp_setmode
func	disp_setmode
	movzx	AX,byte ptr PS[ESP]
	push	EBP
	video_io
	pop	EBP
	ret
c_endp	disp_setmode

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set into 43 line mode.
; Do not use while disp package is open.

	c_public	disp_set43
func	disp_set43
	call	disp_open
	call    disp_close
	_ifs	_disp_ega ne 1, S1
	_ifs	_disp_numrows ne 25, S1
	uses	<EBX,ECX,EDX>
	push	EBP		;some BIOS's trash BP
	mov	AX,0003h	;set color mode, 80x43
	_ifs	_disp_mono e 0, S2
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
	mov	BH,2
	video_io
	pop	EBP
	movzx	EDX,DL
	_ifs	EDX ae _disp_cursorrow, S4
	mov	_disp_cursorrow,EDX	;reset cursor so it's on the screen
S4:	inc	EDX
	mov	_disp_numrows,EDX	;set new # of rows
	unuse	<EDX,ECX,EBX>
S1:	ret
c_endp	disp_set43

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Switch from 43 line mode back to 25 line mode.

	c_public	disp_reset43
func	disp_reset43
	_ifs	_disp_ega ne 1, S1
	_ifs	_disp_numrows be 25, S1
	uses	<EBX,EDX>
	push	EBP
	mov	AX,0003		;color, 80x25
	_ifs	_disp_mono e 0, R2
	mov	AX,0007		;mono, 80x25
R2:	mov	_disp_mode,AL	;save display mode
	video_io
	mov	AX,01101h	;character generator BIOS routine
	mov	BL,0
	video_io
	mov	AX,01200h
	mov	BL,020h
	video_io
	mov	BH,2
	mov	AX,01130h	;inquire info from EGA BIOS
	video_io
	movzx	EDX,DL
	_ifs	EDX ae _disp_cursorrow, R3
	mov	_disp_cursorrow,EDX	;reset cursor so it's on the screen
R3:	inc	EDX
	mov	_disp_numrows,EDX	;set new # of rows
	mov	AH,0
	mov	AL,_disp_mode	;reset video mode to clean up odd characters
	video_io		;gee 5-20-90
	pop	EBP
	unuse	<EDX,EBX>
	ret
c_endp	disp_reset43

;parameter list for disp_peekbox & disp_pokebox
;

PBX_BUFFER	EQU	0
PBX_LINE	EQU	PBX_BUFFER+SIZEPTR
PBX_COLUMN	EQU	PBX_LINE+4
PBX_BOTTOMLINE	EQU	PBX_COLUMN+4
PBX_RIGHTCOLUMN EQU	PBX_BOTTOMLINE+4

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
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ECX,EDX,ESI,EDI,ES,DS>
	clr	EDX
	mov	DH,P+PBX_LINE[EBP]			;top row
	mov	DL,P+PBX_COLUMN[EBP]			;left column
	mov	EBX,P+PBX_BOTTOMLINE[EBP]		;bottom row
	sub	BL,DH					;BX = no of rows
	inc	EBX
	mov	ECX,P+PBX_RIGHTCOLUMN[EBP]		;right column
	sub	CL,DL				;CX = no of columns
	inc	ECX
	mov	EDI,P+PBX_BUFFER[EBP]		;pointer to buf in EDI
	_ifs _disp_base e 0,pkk			;use bios
	mov	EAX,_disp_numcols
	push	EAX
	mul	DH
	xor	DH,DH
	add	EAX,EDX
	shl	EAX,1
	mov	ESI,EAX
	pop	EDX
	sub	EDX,ECX
	shl	EDX,1
	mov	DH,_disp_snowycga
ife ESeqDS
	push	DS
	pop	ES
endif
	mov	DS,_disp_base			;screen segment

pkb0:	push	EBX				;no of rows
	push	EDX				;offset to next row (and snow flag)
	push	ECX				;no of cols
	_ifs DH e 1,pkb1				;if snow is a problem jump
	rep	movsw				;otherwise just do copy
	jmp	short pkb99
pkk:	jmp	peekbios
pkb1:	mov	DX,3DAh				;CGA status port
pkb2:	mov	EBX,ECX
	mov	ECX,TIMEOUT
	cli	;clear interrupts
pkb3:	in	AL,DX				;read the port
	test	AL,1				;loop while display enable
	loopnz	pkb3				;is not active
	jz	pkb4				;jump if not timed out
	movsw					;copy one word (char & attr)
	sti					;re-enable interrupts
	mov	ECX,EBX				;restore CX = no of bytes
	loop	pkb2				;no more bytes so exit
	jmp	short pkb99
pkb4:	sti
	mov	ECX,EBX				;restore CX = no of bytes
pkb5:	push	ECX				;save loop counter
	mov	ECX,TIMEOUT			;timeout loop limit
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
	pop	ECX
	loop	pkb5
	jmp short pkb99
pkb8:	pop	EBX
	mov	ECX,RETRACE
	cmp	EBX,ECX
	jnb	pkb9
	mov	ECX,EBX
	xor	EBX,EBX
	jmp	short pkb10
pkb9:	sub	EBX,ECX
pkb10:	rep	movsw
	mov	ECX,EBX
	test	ECX,ECX
	jnz	pkb2

pkb99:	pop	ECX
	pop	EDX
	mov	EBX,EDX
	xor	DH,DH
	add	ESI,EDX
	mov	EDX,EBX
	pop	EBX
	dec	EBX
	jnz	pkb0
	cld
	jmp	short peekend
peekbios:
	mov	BH,_disp_displaypage
peeklp1:
	push	ECX				;save the # of columns
	push	EDX
peeklp2:
	mov	AX,0200h
	video_io				;position the cursor
	mov	AX,0800h
	video_io				;read one word (char & attr)
	stosw					;store in ES:DI
	inc	DL				;next column
	loop	peeklp2				;loop until row finished
	pop	EDX
	pop	ECX
	inc	DH				;next row
	dec	BL				;all rows done?
	jnz	peeklp1				;go and do the next row
peekend:
	unuse	<DS,ES,EDI,ESI,EDX,ECX,EBX>
	pop	EBP
	ret
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
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ECX,EDX,ESI,EDI,ES>
	clr	EDX
	mov	DH,P+PBX_LINE[EBP]			;top row
	mov	DL,P+PBX_COLUMN[EBP]			;left column
	movzx	EBX,word ptr P+PBX_BOTTOMLINE[EBP]	;bottom row
	sub	BL,DH					;BX = no of rows
	inc	EBX
	movzx	ECX,word ptr P+PBX_RIGHTCOLUMN[EBP]	;right column
	sub	CL,DL					;CX = no of columns
	inc	ECX
	mov	ESI,P+PBX_BUFFER[EBP]			;pointer to buf in ESI
	_ifs _disp_base e 0,pbk				;use bios
	mov	EAX,_disp_numcols
	push	EAX
	mul	DH
	xor	DH,DH
	add	EAX,EDX
	shl	EAX,1
	mov	EDI,EAX
	pop	EDX
	sub	EDX,ECX
	shl	EDX,1
	mov	DH,_disp_snowycga
	mov	ES,_disp_base			;screen segment
pb0:	push	EBX
	push	EDX
	push	ECX
	_ifs DH e 1,pb1				;if snow is a problem jump
	rep	movsw				;otherwise just do copy
	jmp short pb99
pbk:	jmp	pokebios
pb1:	mov	DX,3DAh				;CGA status port
pb2:	mov	EBX,ECX
	mov	ECX,TIMEOUT
	cli					;clear interrupts
pb3:	in	AL,DX				;read the port
	test	AL,1				;loop while display enable
	loopnz	pb3				;is not active
	jz	pb4				;jump if not timed out
	movsw					;copy one word (char & attr)
	sti					;re-enable interrupts
	mov	ECX,EBX				;restore CX = no of bytes
	loop	pb2				;no more bytes so exit
	jmp short pb99
pb4:	sti
	mov	ECX,EBX				;restore CX = no of bytes
pb5:	lodsw					;AL = char   AH = attr
	mov	EBX,EAX				;get into BX
	push	ECX				;save loop counter
	mov	ECX,TIMEOUT			;timeout loop limit
	cli					;clear interrupts
pb6:	in	AL,DX
	test	AL,1
	loopnz	pb6
	jnz	pb8
pb7:	in	AL,DX
	test	AL,1
	jz	pb7
	xchg	EAX,EBX
	stosw
	sti
	pop	ECX
	loop	pb5
	jmp short pb99
pb8:	pop	EBX
	dec	ESI
	dec	ESI
	mov	ECX,RETRACE
	cmp	EBX,ECX
	jnb	pb9
	mov	ECX,EBX
	xor	EBX,EBX
	jmp short pb10
pb9:	sub	EBX,ECX
pb10:	rep	movsw
	mov	ECX,EBX
	test	ECX,ECX
	jnz	pb2

pb99:	pop	ECX
	pop	EDX
	mov	EBX,EDX
	xor	DH,DH
	add	EDI,EDX
	mov	EDX,EBX
	pop	EBX
	dec	EBX
	jnz	pb0
	cld
	jmp	short pokeend
pokebios:
	mov	BH,_disp_displaypage	;display page
pokelp1:
	push	ECX			;save the # of columns
	push	EBX
	push	EDX
pokelp2:push	ECX
	mov	AX,0200h
	video_io			;position the cursor
	mov	AX,[ESI]		;get character and attribute
	mov	BL,AH			;attribute
	mov	ECX,1
	mov	AH,09h
	video_io			;write one word (char & attr)
	inc	DL			;next column
	inc	ESI			;next char/attr
	inc	ESI
	pop	ECX
	loop	pokelp2			;loop until row finished
	pop	EDX
	pop	EBX
	pop	ECX
	inc	DH			;next row
	dec	BL			;all rows done?
	jnz	pokelp1			;go and do the next row
pokeend:
	unuse	<ES,EDI,ESI,EDX,ECX,EBX>
	pop	EBP
	ret
c_endp disp_pokebox

	endcode disp

	end
