;_ wincio.asm   Modified by: Walter Bright
; Windows support added by G. Eric Engstrom Dec 1990	-D_WINDOWS
; Copyright (C) 1990-1991 by Walter Bright
; All Rights Reserved
; Written by Eric Engstrom
; $Revision: 1.1.1.1 $

    ifdef _WINDOWS

	include	macros.asm
	include stdio.asm

;Windows 3 winc support constants
	HSTDERR			equ	2	; dos file handles
	HSTDOUT			equ	1
	HSTDIN			equ	0

	SYSTEM_FIXED_FONT	equ	16	; from windows.h

extrn	CREATESOLIDBRUSH:far
extrn	DELETEOBJECT:far
extrn	DPTOLP:far
extrn	FILLRECT:far
extrn	GETACTIVEWINDOW:far
extrn	GETBKCOLOR:far
extrn	GETCLIENTRECT:far
extrn	GETCURRENTPOSITION:far
extrn	GETDC:far
extrn	GETSTOCKOBJECT:far
extrn	GETTEXTEXTENT:far
extrn	GETTEXTMETRICS:far
extrn	LPTODP:far
extrn	MOVETO:far
extrn	RELEASEDC:far
extrn	SCROLLWINDOW:far
extrn	SELECTOBJECT:far
extrn	TEXTOUT:far
extrn	VALIDATERGN:far

    if LCODE	
	extrn	_getche		:far
	extrn	_printf		:far
    else
    	extrn	_getche		:near
    	extrn   _printf		:near
    endif 		
    	extrn   ___winc_bdos	:dword
    
	begdata

	extrn	__tab_size	:word		; size of tabs used by this system
	

	; entry points to stdout & stderr control structures
	_std_X		equ	0
	_std_Y		equ	2
	_std_hWnd	equ	4
	_std_PrevHWnd	equ	6
	_std_hDC	equ	8
	_std_XTextSize	equ	10
	_std_YTextSize	equ	12
	_std_YSize	equ	16
	_std_YScroll	equ	20
	_std_SolidBrush	equ	22
	_std_Font	equ	24
	_std_ClientRect	equ	26
	_std_BLineRect	equ	34
	_std_Hook	equ	42
	_std__SizeOf	equ	46
 
	; entry points to ClientRect control struct
	rectLeft	equ	0
	rectTop		equ	2
	rectRight	equ	4
	rectBottom	equ	6

	CR			dw	0

	_StdErrW		db	_std__Sizeof dup (0)
	_StdOutW		db	_std__Sizeof dup (0)
	TextMetricBuffer	db	32 dup (0)
	NEWLINE			db	0ah,0
	enddata

	begcode	io

	c_public StdOutW,StdErrW
	public STDIOFONTSET,STDIOSET,STDIOGET,STDIORELEASE,STDIOHOOKSET,STDIOHOOKGET,STDIOCLEAR
	public ___winc_hook_bdos

callptr macro func
   if LCODE
	call  dword ptr func
   else
	call  word ptr func
   endif
	endm

ThisLoad macro off
    if LPTR
    ifnb <off>
	les	BX,P+off[BP]
    else
	les	BX,P[BP]
    endif
    else
	mov	BX,DS
	mov	ES,BX
    ifnb <off>
	mov	BX,P+off[BP]
    else
	mov	BX,P[BP]
    endif
    endif
	endm

ThisPush macro
	push	ES
	push	BX
	endm

ThisPop macro
	pop	BX
	pop	ES
	endm

DSResetPushCXSI macro
	pop	DS
	push	DS
	push	CX
	push	SI
	endm

DSSetPopSICX macro
	mov	DS,6[BP]
	pop	SI
	pop	CX
	endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; StdIOFontSet(StdIOC *, HFONT hFont);
; Selects a new font to use when displaying
; through this stream

p_func	STDIOFONTSET
	WINENTER
	push	ES
	ThisLoad 2
	mov	AX, P[BP]
	mov	ES:[BX]._std_Font, AX
	mov	AX, ES:[BX]._std_HWnd
	call    ___getdc
	mov	ES:[BX]._std_HDC,AX
	ThisPush
	push    ES:[BX]._std_HDC
	push    DS
	mov	DX,offset DGROUP:TextMetricBuffer
	push    DX
	call    GETTEXTMETRICS
	ThisPop
	mov	AX,word ptr TextMetricBuffer+10
	mov	ES:[BX]._std_XTextSize,AX
	mov	AX,word ptr TextMetricBuffer
	mov	ES:[BX]._std_YTextSize,AX
	mov	ES:[BX]._std_YScroll,AX
	clr	DX
	mov	AX,word ptr ES:[BX]._std_BLineRect+rectBottom
	mov	CX,AX
	div	word ptr ES:[BX]._std_YTextSize
	sub	CX,DX
	mov	ES:[BX]._std_YSize,CX
	sub	CX,ES:[BX]._std_YTextSize
	mov	word ptr ES:[BX]._std_BLineRect+rectTop,CX
	ThisPush
	push    ES:[BX]._std_HDC
	push    ES
	lea	BX,ES:[BX]._std_YScroll-2
	push    BX
	mov	AX,1
	push    AX
	call    LPTODP
	ThisPop
	neg	word ptr ES:[BX]._std_YScroll
	push    ES:[BX]._std_HWnd
	push    ES:[BX]._std_HDC
	call    RELEASEDC
	pop	ES
	WINLEAVE
	ret	2+SIZEPTR
p_endp	STDIOFONTSET

p_func	STDIOCLEAR
	WINENTER
	push	ES
	ThisLoad
	ThisPush
	push	ES:[BX]._std_HWnd
	call	GETDC
	ThisPop
	mov	ES:[BX]._std_HDC,AX
	clr	CX
	mov	ES:[BX]._std_X,CX			; home cursor
	mov	ES:[BX]._std_Y,CX
	ThisPush
	push	ES:[BX]._std_HWnd		      ;VALIDATERGN
	push	CX				      ;VALIDATERGN
	push	AX				      ;FILLRECT
	push	ES				      ;FILLRECT
	lea	DX,ES:[BX]._std_ClientRect
	push	DX				      ;FILLRECT
	push	ES:[BX]._std_SolidBrush		      ;FILLRECT
	call	FILLRECT
	call	VALIDATERGN
	ThisPop
	push	ES:[BX]._std_HWnd
	push	ES:[BX]._std_HDC
	call	RELEASEDC
	pop	ES
	WINLEAVE
	ret	SIZEPTR
p_endp	STDIOCLEAR

p_func	STDIOSET
	WINENTER
	push	ES
	push	SI	; save SI & DI (DAM - 6/18/91)
	push	DI
	mov	AX, SYSTEM_FIXED_FONT	; Added by DAM
	push	AX			; sets default font
	call    GETSTOCKOBJECT		; for this stream
	ThisLoad 2
	mov	ES:[BX]._std_Font,AX	; set default font (DAM)
	mov	AX,P[BP]
	mov	ES:[BX]._std_HWnd,AX
	call	___getdc
	mov	ES:[BX]._std_HDC,AX
	mov	CX,ES:[BX]._std_HWnd
	cmp	CX,ES:[BX]._std_PrevHWnd
	jne	out2
	mov	word ptr ES:[BX]._std_PrevHWnd,-1
	jmp	short out3
out2:	ThisPush
	push	AX
	call	GETCURRENTPOSITION
	ThisPop
	mov	ES:[BX]._std_X,AX
	mov	ES:[BX]._std_Y,DX

out3:	ThisPush
	push	ES:[BX]._std_HDC
	push	DS
	mov	DX,offset DGROUP:TextMetricBuffer
	push	DX
	call	GETTEXTMETRICS
	ThisPop
	mov	AX,word ptr TextMetricBuffer+10
	mov	ES:[BX]._std_XTextSize,AX
	mov	AX,word ptr TextMetricBuffer
	mov	ES:[BX]._std_YTextSize,AX
	mov	ES:[BX]._std_YScroll,AX
	ThisPush
	push	ES:[BX]._std_HWnd
	push	DS
	lea	DX,ES:[BX]._std_ClientRect
	push	DX
	call	GETCLIENTRECT
	ThisPop
	ThisPush
	push	ES
	pop	DS
	lea	SI,[BX]._std_ClientRect
	lea	DI,[BX]._std_BLineRect
	mov	CX,4
	rep	movsw
	ThisPop
	clr	DX
	mov	AX,word ptr ES:[BX]._std_BLineRect+rectBottom
	mov	CX,AX
	div	word ptr ES:[BX]._std_YTextSize
	sub	CX,DX
	mov	ES:[BX]._std_YSize,CX
	sub	CX,ES:[BX]._std_YTextSize
	mov	word ptr ES:[BX]._std_BLineRect+rectTop,CX
	ThisPush
	push	ES:[BX]._std_HDC
	push	ES
	lea	BX,ES:[BX]._std_YScroll-2
	push	BX
	mov	AX,1
	push	AX
	call	LPTODP
	ThisPop
	neg	word ptr ES:[BX]._std_YScroll

	ThisPush
	push	ES:[BX]._std_HDC
	call	GETBKCOLOR
	push	DX
	push	AX
	call	CREATESOLIDBRUSH
	ThisPop
	mov	ES:[BX]._std_SolidBrush,AX
	mov	word ptr ES:[BX]._std_Hook,0
	mov	word ptr ES:[BX]._std_Hook+2,0

	push	ES:[BX]._std_HWnd
	push	ES:[BX]._std_HDC
	call	RELEASEDC

	pop	DI	; restore SI & DI (DAM - 16/18/91)
	pop	SI
	pop	ES
	WINLEAVE
	ret	2+SIZEPTR
p_endp	STDIOSET

p_func	STDIOGET
	WINENTER
	push	ES
	ThisLoad
	mov	AX,ES:[BX]._std_HWND
	pop	ES
	WINLEAVE
	ret	SIZEPTR
p_endp	STDIOGET

p_func	STDIOHOOKSET
	WINENTER
	push	ES
    if LCODE
	ThisLoad 4
	mov	DX,P+2[BP]
	mov	ES:[BX]._std_Hook+2,DX
    else
	ThisLoad 2
    endif
	mov	AX,P[BP]
	mov	ES:[BX]._std_Hook,AX
	pop	ES
	WINLEAVE
    if LCODE
	ret	SIZEPTR+4
    else
	ret	SIZEPTR+2
    endif
p_endp	STDIOHOOKSET

p_func	STDIOHOOKGET
	WINENTER
	push	ES
	ThisLoad
    if LCODE
	mov	DX,ES:[BX]._std_Hook+2
    endif
	mov	AX,ES:[BX]._std_Hook
	pop	ES
	WINLEAVE
	ret	SIZEPTR
p_endp	STDIOHOOKGET

p_func	STDIORELEASE
	WINENTER
	push	ES
	ThisLoad
	ThisPush
	push	ES:[BX]._std_SolidBrush
	call	DELETEOBJECT
	ThisPop
	mov	AX,ES:[BX]._std_HWnd
	mov	ES:[BX]._std_PrevHWnd,AX
	mov	word ptr ES:[BX]._std_HWnd,-1
	mov	word ptr ES:[BX]._std_Hook,0
	mov	word ptr ES:[BX]._std_Hook+2,0
	pop	ES
	WINLEAVE
	ret	SIZEPTR
p_endp	STDIORELEASE

___doit proc	near
	ThisPush
	mov	DX,ES:[BX]._std_YSize
	cmp	DX,ES:[BX]._std_Y
	ja	___do0
 
	  ThisPush
	  clr	  AX
	  push	  ES:[BX]._std_HWnd			;VALIDATERGN window handle
	  push	  AX					;VALIDATERGN whole window flag
	  push	  ES:[BX]._std_HDC			;FILLRECT device context
	  push	  DS					;FILLRECT seg
	  lea	  DX,ES:[BX]._std_BLineRect
	  push	  DX					;FILLRECT offset of fill rectangle for
									; for bottom line
	  push	  ES:[BX]._std_SolidBrush		;FILLRECT brush to fill with
	  push	  ES:[BX]._std_HWnd			;SCROLLWINDOW window handle
	  push	  AX					;SCROLLWINDOW 0 x dimension, no scroll
	  mov	  DX,ES:[BX]._std_YScroll
	  push	  DX					;SCROLLWINDOW y dimension to scroll up
	  push	  AX					;SCROLLWINDOW 0 scroll rect
	  push	  AX					;SCROLLWINDOW (means whole window)
	  push	  AX					;SCROLLWINDOW 0 clip rect
	  push	  AX					;SCROLLWINDOW (means whole window)
	  call	  SCROLLWINDOW
	  call	  FILLRECT
	  call	  VALIDATERGN
	  ThisPop
	  mov	  DX,ES:[BX]._std_YTextSize
	  sub	  ES:[BX]._std_Y,DX

___do0: ThisPush
	push	ES:[BX]._std_HDC	; GETTEXTEXTENT device context
	push	6[BP]			; GETTEXTEXTENT seg
	push	4[BP]			; GETTEXTEXTENT offset of text string
	push	8[BP]			; GETTEXTEXTENT length
	push	ES:[BX]._std_HDC	; TEXTOUT device context
	push	ES:[BX]._std_X		; TEXTOUT x coordinate
	push	ES:[BX]._std_Y		; TEXTOUT y coordinate
	push	6[BP]			; TEXTOUT seg
	push	4[BP]			; TEXTOUT offset of text string
	push	8[BP]			; TEXTOUT length
	call	TEXTOUT
	call	GETTEXTEXTENT
	ThisPop
	ThisPush
	add	ES:[BX]._std_X,AX	; update x coordinate with text extent width
	push	ES:[BX]._std_HDC	; MOVETO device context
	push	AX			; MOVETO x coordinate
	push	DX			; MOVETO y coordinate
	call	MOVETO
	ThisPop
	cmp	word ptr ES:[BX]._std_Hook,0
	je	___do1
	  push	  6[BP]			; user HOOK seg
	  push	  4[BP]			; user HOOK offset of text string
	  push	  8[BP]			; user HOOK length
	  push	  CR			; user HOOK carriage return \ tab flag
	  callptr ES:[BX]._std_Hook
___do1:	ThisPop
	ret
___doit endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Computes the x position to start drawing on after a tab
; Input:
;	ES:BX -> this
;
;	move the x position out to meet the tab requirements
;	xPosition+((TABSIZE-((xPosition/xCharSize)%TABSIZE))*xCharSize)
;
; Returns:
;	nothing, modifies data structure

__tabcalc proc near
	clr	DX
	mov	AX,ES:[BX]._std_X
	div	word ptr ES:[BX]._std_XTextSize	; xpos/xcharsize
	clr	DX
	mov	CX,DGROUP:__tab_size
	div	CX				; % TABSIZE
	mov	AX,CX
	sub	AX,DX				; TABSIZE - remainder
	mul	word ptr ES:[BX]._std_XTextSize ; * xcharsize
	add	ES:[BX]._std_X,AX
	ret
__tabcalc endp

; ___getdc	gets a DC, sets the font to SYSTEM_FONT_FIXED
;
; ES:BX -> this
; returns DC in AX
; Modified 6/12/91 by DAM to set
; font from StdIOC structure
___getdc proc near
	ThisPush
	push	AX			; GETDC window handle
	call	GETDC
	ThisPop
	ThisPush
	push	AX			; PUSH device context
	push	AX			; SELECTOBJECT device context
	mov	AX,ES:[BX]._std_Font
	push	AX			; SELECTOBJECT object
	call	SELECTOBJECT
	pop	AX			; POP device context
	ThisPop
	ret
___getdc endp

;__textout handles formatting of text streams to stdout & stderr
;	   under Windows 3.0
;
; ES:BX == <this>

__textout proc near
	push	BP
	mov	BP,SP
	ThisPush
	push	DI
;	
	mov	AX,ES:[BX]._std_HWnd
	push	AX			; RELEASEDC window handle
	call	___getdc
	mov	ES:[BX]._std_HDC,AX

; scan string for carriage returns, line feeds
; and where to position cursor next

	push	SI
	push	DS

	mov	DS,6[BP]
	mov	SI,4[BP]
	mov	CX,8[BP]

__txt0:   lodsb
	  cmp	  AL,13		; is it a carriage return
	  jne	  __txt4
	    DSResetPushCXSI

	    sub     8[BP],CX
	    call    ___doit

	    mov     word ptr ES:[BX]._std_X,0
	    jmp     short __txt5
__txt4:   cmp	  AL,9		; is it a tab
	  jne	  __txt1
	    DSResetPushCXSI

	    mov     CR,2	; set TAB flag
	    sub     8[BP],CX
	    call    ___doit
	    mov     CR,0	; reset TAB flag

	    call    __tabcalc

__txt5:     mov     AX,8[BP]
	    inc     AX

	    DSSetPopSICX

	    add     4[BP],AX
	    dec     CX
	    mov     8[BP],CX
	    inc     CX
	    jmp     short __txt2
__txt1:   cmp	  AL,10		; is it a line feed
	  jne	  __txt2
	    mov     DX,ES:[BX]._std_YTextSize
	    add     ES:[BX]._std_Y,DX
	    inc     word ptr 4[BP]
	    dec     word ptr 8[BP]
	    cmp     word ptr ES:[BX]._std_Hook,0	; user HOOK valid ?
	    je	    __txt2
	      DSResetPushCXSI
	      ThisPush          ; DAM

	      push    6[BP]	; user HOOK seg
	      push    4[BP]	; user HOOK offset of text stream
	      clr     AX
	      push    AX	; user HOOK length
	      inc     AX	; set flag to Carriage Return
	      push    AX	; user HOOK carriage return flag
	      callptr ES:[BX]._std_Hook

	      ThisPop		; DAM
	      DSSetPopSICX
__txt2:   loop	  __txt0

	pop	DS
	pop	SI

	cmp	word ptr 8[BP],0
	je	__txt3

	  call	  ___doit

__txt3: 
	push	ES:[BX]._std_HDC
	call	RELEASEDC
;
	pop	DI
	ThisPop
	pop	BP
	ret
__textout endp

;;;;;;;;;;;;;;;;;;;;;;;;;
; BDOS filter to allow WINC to trap & emulate console I/O --
; Parameters and return values exactly as for DOS INT 21
;

buf_sel	equ word ptr -2[BP]
buf_off equ word ptr -4[BP]
buf_len equ word ptr -6[BP]
localsize equ 6

wincio_bdos proc far

;----------------------------------------------------------------------------
; 	we currently emulate AH = 3F (read) and AH = 40 (write)
;----------------------------------------------------------------------------

	cmp	AH,03FH
	je	wb1
	cmp	AH,040H
	jne	wbx

;----------------------------------------------------------------------------
;	we emulate write only to STDOUT and STDERR
;----------------------------------------------------------------------------
	
wb2:	cmp	BX,HSTDOUT
	je	wb3
	cmp	BX,HSTDERR
	je	wb3
	jmp	wbx

;----------------------------------------------------------------------------
;	we emulate read only from STDIN
;----------------------------------------------------------------------------
	
wb1:	cmp	BX,HSTDIN
	je	wb3

;----------------------------------------------------------------------------
;	pass the call to DOS and return
;----------------------------------------------------------------------------
	
wbx:	call	DOS3CALL
	ret

;----------------------------------------------------------------------------
;	common initialization for all emulation
;----------------------------------------------------------------------------

wb3:	WINENTER
	sub	SP,localsize
	push	BX
	push	CX
	push	DX
	push	SI
	push	DI
	push	ES
	push	DS
	
;----------------------------------------------------------------------------
;	save parameter values
;----------------------------------------------------------------------------

	mov	buf_sel,DS
	mov	buf_off,DX
	mov	buf_len,CX
	
;----------------------------------------------------------------------------
;	establish DGROUP addressability
;----------------------------------------------------------------------------

	mov	SI,DGROUP
	mov	DS,SI
	
;----------------------------------------------------------------------------
;	for read emulation, jump to proper code
;----------------------------------------------------------------------------
	
	cmp	AH,3FH
	je	wb4
	
;----------------------------------------------------------------------------
;	check for write to stdout vs stderr
;----------------------------------------------------------------------------

	cmp	BX,HSTDOUT
	je	wb5

;----------------------------------------------------------------------------
;	initialization for STDERR output -- if there is no STDERR window,
;         do the write to STDOUT
;----------------------------------------------------------------------------

	mov	SI,word ptr _StdErrW+_std_hWnd
	cmp     SI,-1
	je	wb5
	
;----------------------------------------------------------------------------
;	STDERR processing:  ES:BX points to the STDERR control block -- DI
;         points to the other stream (STDOUT in this case) for common check
;         performed below -- note that SI != -1 here -- this will be
;         important below
;----------------------------------------------------------------------------

	push	DS
	pop	ES
	mov     BX,offset DGROUP:_StdErrW
	mov     DI,offset DGROUP:_StdOutW
	jmp     short wb6
	
;----------------------------------------------------------------------------
;	STDOUT processing:  ES:BX points to the STDOUT control block -- DI
;	  points to the other stream (STDERR in this case) for common check
;         performed below
;----------------------------------------------------------------------------

wb5:	push	DS
	pop	ES
	mov     BX,offset DGROUP:_StdOutW
	mov     DI,offset DGROUP:_StdErrW
	
;----------------------------------------------------------------------------
;	check to see if STDOUT has a window assigned, jump if so
;----------------------------------------------------------------------------

	mov	SI,word ptr _StdOutW+_std_hWnd
	cmp	SI,-1
	jne	wb6
	
;----------------------------------------------------------------------------
;       if not, find the active window (handle -> AX)
;----------------------------------------------------------------------------

	call	GETACTIVEWINDOW
	
;----------------------------------------------------------------------------
;       and set it up to receive STDOUT
;----------------------------------------------------------------------------

    if LPTR
    	push	ES
    endif
    	push	BX
    	push	AX
    	call	STDIOSET
    	
;----------------------------------------------------------------------------
;	common processing for all cases:  output the text to the selected
;         stream
;----------------------------------------------------------------------------

wb6:	push	buf_len
	push	buf_sel
	push	buf_off
	call	__textout
	add	SP,6
	
;----------------------------------------------------------------------------
;	see whether we are temporarily using the active window for output
;         -- if so, SI will be -1
;----------------------------------------------------------------------------
	
	cmp	SI,-1
	jne	wb7
	
;----------------------------------------------------------------------------
;	if so, release the temporary STDOUT association
;----------------------------------------------------------------------------
	
    if LPTR
	push	ES
    endif
	push	word ptr _StdOutW+_std_hWnd
	call	STDIORELEASE
wb7:	
;----------------------------------------------------------------------------
;	find out if the current stream is sharing a window with the other
;         stream
;----------------------------------------------------------------------------

	mov	AX,[DI]._std_hWnd
	cmp	AX,-1
	je	wb12
	cmp	AX,[BX]._std_hWnd
	jne	wb12
	
;----------------------------------------------------------------------------
;	if so, copy the coordinates for the current stream to the other
;         stream, so that they don't write over each other 
;----------------------------------------------------------------------------

	mov	AX,[BX]._std_X
	mov	[DI]._std_X,AX
	mov	AX,[BX]._std_Y
	mov	[DI]._std_Y,AX
wb12:	
;----------------------------------------------------------------------------
;	return the number of characters written (== the buffer length)
;----------------------------------------------------------------------------

	mov	AX,buf_len	; get number of bytes read/written
	jmps	wb8
	
;----------------------------------------------------------------------------
; 	code to emulate DOS read from STDIN
;----------------------------------------------------------------------------
	
wb4:	mov	es,buf_sel
	mov	bx,buf_off
	jcxz	wb9			;done when CX = 0
wb10:
	push	es			;save buffer pointer and count
	push	bx			
	push	cx
	call	_getche			;get a character and echo
	pop	cx			;restore buffer pointer and count
	pop	bx	
	pop	es
	cmp	al,0dh
	jz	wb11
	mov	es:[bx],al		;store the char in the buffer
	inc	bx			;buffer ptr ++
	loop	wb10			;do it again till the buffer is full
wb11:	
	jcxz	wb9
	mov	ax,0d0ah
	mov	es:[bx],ah
	inc	bx
	dec	cx
	jcxz	wb9
	mov	es:[bx],al
	dec	cx
	push	cx
    	mov	ax,offset DGROUP:NEWLINE
    if LPTR	
	push	DS
    endif	
	push	ax
	call	_printf
    	add	sp,SIZEPTR	
	pop	cx
wb9:	mov	ax,buf_len		; size of buffer
    	sub	ax,cx			; minus chars left = chars avail
    	
;----------------------------------------------------------------------------
;	restore registers and return
;----------------------------------------------------------------------------

wb8:	pop	DS
	pop	ES
	pop	DI
	pop	SI
	pop	DX
	pop	CX
	pop	BX
	mov	SP,BP
	WINLEAVE
	ret
wincio_bdos endp
     
p_func ___winc_hook_bdos
	mov	AX,offset wincio_bdos
	mov	DX,seg wincio_bdos
	mov	word ptr ___winc_bdos,AX
	mov	word ptr ___winc_bdos+2,DX
	ret
p_endp ___winc_hook_bdos

	endcode	io
    endif
    

	end
	