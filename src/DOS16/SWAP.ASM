;_ swap.asm
; Windows support added by G. Eric Engstrom Jan 1991	-D_WINDOWS
; Copyright (C) 1990, 1991 by Concentric Software Designs, Inc.
; All Rights Reserved
; Written by G. Eric Engstrom
;
; includes   exec2.asm written by Walter Bright
;            See file for pertinent copyright information
;

include	macros.asm

if LPTR

ifndef _WINDOWS

switchStack macro segm,offm
	mov     AX,segm
	cli                   ; for bug in old 8088's
	mov     SS,AX
	mov     SP,offm
	sti                   ; for bug in old 8088's
	endm

    if LCODE
assume_DS_swap_codeseg macro
	assume	DS:exec2_text
	endm
    else
assume_DS_swap_codeseg macro
	assume	DS:_text
        endm
    endif

;swap_windowon			turn output windowing on
;swap_windowoff			turn output windowing off
;swap_iswindowon		returns the status of the swap window control flag > 0 if swap window is on
;swap_window			set window for output to be clipped to

;swap_pipeon			turn piping on
;swap_pipeoff			turn piping off
;swap_ispipeon          	returns the status of the swap pipe control flag > 0 if swap piping is on
;swap_pipe			set file to pipe to

;swap_on			turn swapping on
;swap_off			turn swapping off
;swap_ison			returns the status of the swap control flag > 0 if swapping is on

;swap_tempcheckon		turn the TMP/TEMP checking on
;swap_tempcheckoff		turn the TMP/TEMP checking off
;swap_istempcheckon		returns the 1 if swap will use TMP/TEMP for location of .TMP file

;swap_freeparagraphson		turn the free a specific amount of memory feature on
;swap_freeparagraphsoff		turn the free a specific amount of memory feature off
;swap_isfreeparagraphson	returns the status of the swap free a specific amount of memory control flag >0 is specific size freeing on
;swap_freeparagraphs		set number of paragraphs to free, SWAP normally frees everything it can

;swap_trapcbreakon		turns control C/control Break trapping on
;swap_trapcbreakoff		turns control C/control Break trapping off (default)
;swap_istrapcbreakon		returns the status of the swap trap flag >0 is trap ^C/^Break on

;swap_clearkeyboardon		turns keyboard buffer clearing on
;swap_clearkeyboardoff		turns keyboard buffer clearing off
;swap_isclearkeyboardon		returns the status of the swap clear keyboard flag >0 is clear keyboard buffer on

_SWAP EQU 1

SWAP_FREEMEMERROR	equ	-2
SWAP_NOVMSPACE		equ	-3

    if LCODE
extrn          _getenv : far
extrn          _getcwd : far
extrn          _strlen : far
    else
extrn          _getenv : near
extrn          _getcwd : near
extrn          _strlen : near
    endif

	begdata

        c_extrn _tab_size,word

    if SPTR

        c_extrn _progpar,word
        c_extrn _datapar,word
    else
        c_extrn _totalpar,word
    endif

	c_extrn	errno,word, _psp,word

    if SPTR
swapTMP_DS      db  "TMP",0
swapTEMP_DS     db  "TEMP",0
swapCWD_DS      db  146 dup (0)		; CWD where .TMP file landes here, then moved to CS:swapCWD
    endif

header	equ	$			; .EXE file header
fcb1	db	37 dup (?)
fcb2	db	37 dup (?)		; a sub-process may want these

param_block	equ $
env	dw	?			; segment address of environment
comml	dw	?			; DWORD ptr to command line
	dw	?			; segment of command line
pfcb1	dw	offset DGROUP:fcb1	; DWORD points to first FCB
	dw	?			; segment of first FCB
pfcb2	dw	offset DGROUP:fcb2	; DWORD ptr to second FCB
	dw	?			; segment of second FCB

	enddata

	begcode	exec2

                db  512 dup (0)
newStack        equ $

sheader	equ	$			; .EXE file header
sfcb1	db	37 dup (?)
sfcb2	db	37 dup (?)		; a sub-process may want these
sheader_end equ $

sparam_block	equ $
senv	dw	?			; segment address of environment
scomml	dw	?                       ; DWORD ptr to command line
scomseg dw	?			; segment of command line
spfcb1	dw	?                       ; DWORD points to first FCB
spseg1  dw	?			; segment of first FCB
spfcb2	dw	?           	        ; DWORD ptr to second FCB
spseg2	dw	?			; segment of second FCB

scommlt equ     $
        db      130 dup (0)
scommltl equ     $
        db      140 dup (0)

swapInsideWrite db  0
swapHexStr      db  "0123456789ABCDEF"
    if LPTR
swapTMP         db  "TMP",0
swapTEMP	db  "TEMP",0
    endif
swapCWD         db  146 dup (0)       ; CWD where .TMP file was written is kept here
swapVMFile      db  "SC00000S.TMP",0  ; 0's are overlayed by _psp to avoid conflicting file names
swapFatalError  db  7,"Fatal Error: Unable to return to parent process. Returning to OS.",13,10,"$"
swapAttribute   db  ?
swapVidPage     db  ?
swapCtr         dw  1
swapPipeCtr     dw  0
swapWindowCtr   dw  0
swapClearBufferCtr    dw 1
swapTrapCBreakCtr     dw 1
swapTempCheckCtr      dw 1
swapFreeParagraphsCtr dw 0
swapFreeParagraphs    dw 0
swapPipeFile    dd  ?
swapPipeHandle  dw  ?
swapHandle      dw  ?
swapPar         dw  ?
swapRemainder   dw  ?
swapBlocks      dw  ?
swapFrom        dw  ?
swapErrno	dw  ?
swapErrret	dw  ?
swapPipeCnt     dw  ?                 ;this area is used to buffer the output that goes to the screen
swapPipeBufSize dw  ?

  ;swapPagePar,swapPSP must maintain their position relationship with each other
swapPagePar    dw  ?
swapPSP        dw  ?

  ;swapColumn,swapLine,swapX,swapY maintain their position relationship with each other
swapColumn     dw  0  ;default window size
swapLine       dw  0
swapX          dw  80
swapY          dw  25 

swapVidOff     dw  ?
swapCurCol     dw  ?
swapCurLin     dw  ?
swapTabSize    dw  ?

cBreak1BVector dd  ?
cBreak23Vector dd  ?
int21Vector    dd  ?

swapAX         dw  ?
swapBX         dw  ?
swapCX         dw  ?
swapDX         dw  ?
swapDS         dw  ?
swapES         dw  ?

oldsp	       dw  ?  ;save SP
oldss          dw  ?  ;save SS

	assume	DS:nothing
cBreakStubHandler proc far
	sti
	iret
cBreakStubHandler endp

        assume  DS:nothing
int21Handler proc far
        cmp     AH,02h
        je      d1
        cmp     AH,06h
        je      d1
        cmp     AH,09h
        je      d1
        cmp     AH,40h
        jne     d0

          ; pipe file write is only done during safe calls to OS
          ; as is I/O redirection and window clipping

	  sti
	  cmp	  CS:swapClearBufferCtr,0
          jle	  d1
	    push    AX
d00:	      mov     AH,1
	      int     16h		; check for character waiting
	      jz      d01
              mov     AH,0
              int     16h
	      jmp     short d00
d01:  	    pop	    AX
d1:       cmp     AH,02h       ; direct console Output
          je      d10
          cmp     AH,06h       ; direct console I/O
          jne     d2
            cmp     DL,0FFh
            je      d8
d10:          push    AX
              mov     AL,DL
              call    vWrite
              pop     AX
              jmp     short d41
d2:       cmp     AH,40h       ; file write
          jne     d5
            cmp     BX,1       ; see if stdout
            je      d3
            cmp     BX,2       ; see if stderr
            jne     d8         ; if !(stdout||stderr) then not interested
d3:         jcxz    d8	       ; skip 0 bytes writes, (this happens! why?)
              push    SI
              mov     SI,DX
              push    AX
              push    CX
d4:             push    CX
                mov     AL,[SI]
                inc     SI
                call    vWrite
                pop     CX
                loop    d4
              pop     CX
              pop     AX
              cmp     CS:swapWindowCtr,0
              jle     d40
                mov     AX,CX        ; return count of chars written
d40:          pop     SI
d41:          cmp     CS:swapWindowCtr,0
              jle     d0             ; if no window then do normal DOS write
              iret
d5:       push    SI
          push    AX
          mov     SI,DX
d6:         mov     AL,[SI]
            inc     SI
            cmp     AL,'$'
            jne     d7
              pop     AX
              jmp     short d40
d7:         call    vWrite
            jmp     short d6

d8:     call    dWrite
d0:     jmp     dword ptr CS:int21Vector
int21Handler endp

        assume  DS:nothing
dWrite  proc    near
        push    AX
        mov     AX,CS:swapPipeBufSize
        cmp     CS:swapPipeCnt,AX
        jb      dW0
          cmp     CS:swapInsideWrite,0
          jne     dW0
            inc     CS:swapInsideWrite
            call    iWrite
            dec     CS:swapInsideWrite
dW0:    pop     AX
        ret
dWrite  endp

        assume  DS:nothing
iWrite  proc    near
        ; save registers
        mov     CS:swapDS,DS
        push    CS
        pop     DS
        mov     swapAX,AX
        mov     swapBX,BX
        mov     swapCX,CX
        mov     swapDX,DX
        mov     swapES,ES

        ; write out pipe buffer
        mov     CX,swapPipeCnt
        mov     swapPipeCnt,0
        mov     BX,swapPipeHandle
        mov     DX,offset CS:swapPipeBuf
        mov     AH,40h
        int     21h

        ; restore registers
        mov     AX,swapAX
        mov     BX,swapBX
        mov     CX,swapCX
        mov     DX,swapDX
        mov     ES,swapES
        mov     DS,swapDS
        ret
iWrite  endp

;
; move the curcol position to to meet the tab requirements
; curcol + (TABSIZE-(curcol%TABSZIE)
;
; returns nothing, modifies data structure
;
tabcalc proc    near
	clr	DX
        mov	AX,swapCurCol
        mov	CX,swapTabSize
        div	CX
        sub	CX,DX
        jcxz	tab1
tab0:	  cmp     swapVidOff,0
	  jne	  tab1
            mov     AX,3584+' '	; (14*256) == 3584
            push    CX
            int     10h
            pop	    CX
            inc     swapCurCol
            call    swap_clip
          loop    tab0
tab1:   ret
tabcalc endp

vWrite  proc    near
        push    DS
        push    CS
        pop     DS
        cmp     swapPipeCtr,0
        jle     i1
        cmp     swapPipeHandle,0
        je      i1
          push    DI
          mov     DI,offset CS:swapPipeBuf
          add     DI,swapPipeCnt
          mov     [DI],AL
          inc     swapPipeCnt
          pop     DI
i1:     cmp     swapWindowCtr,0
        jle     i7
          push    ES
          push    BX
          cmp     AL,10
          jne     i2
            inc     swapCurLin
            jmp     short i25
i2:       cmp     AL,13
          jne     i3
            mov     swapCurCol,0
i25:        call    swap_recalc
            jmp     short i5
i3:       cmp     swapVidOff,0
          jne     i6
            cmp     AL,9
            jne     i4
              call    tabcalc
              jmp     short i6
i4:         cmp     AL,7
            jne     i45
              mov     AL,' '
i45:        mov     AH,14
            int     10h
            inc     swapCurCol
i5:       call    swap_clip
i6:       pop     BX
          pop     ES
i7:     pop     DS
        jmp     dWrite
vWrite  endp

;swap_clip
;
;  CS = DS
swap_clip proc near
        push   AX
        mov    swapVidOff,0
        mov    AX,swapX
        cmp    AX,swapCurCol
        ja     c0
          mov    swapVidOff,1
c0:     mov    AX,swapY
        cmp    AX,swapCurLin
        ja     c1
          push   CX
          push   DX
          push   BX
          mov    CX,swapColumn
          mov    CH,byte ptr swapLine
          mov    DX,CX
          add    DH,byte ptr swapY
          add    DL,byte ptr swapX
          dec    DX
          dec    DH
          mov    BH,swapAttribute
          mov    AX,0601h
          int    10h
          dec    swapCurLin
          call   swap_recalc
          pop    BX
          pop    DX
          pop    CX
c1:     pop    AX
        ret
swap_clip endp

        assume  DS:nothing
swap_recalc proc near
        push   AX
        push   BX
        push   DX
        mov    AX,CS:swapLine
        add    AX,CS:swapCurLin
        mov    DX,CS:swapColumn
        add    DX,CS:swapCurCol
        mov    DH,AL
        mov    BH,CS:swapVidPage
        mov    AH,2
        int    10h
        pop    DX
        pop    BX
        pop    AX
        ret
swap_recalc endp

	assume_DS_swap_codeseg
swap_preamble proc near
	cmp     swapWindowCtr,0
        jle     pre0

          call    swap_recalc
          push    BX
          mov     BH, swapVidPage  ; Page Number for Int 10, AH=8 function
          mov     AH,8
          int     10h
          mov     swapAttribute,AH
          pop     BX

pre0:   call    swap_calc       ; compute all the sizes of things to be swapped

        ; swapBlocks = 16K blocks to write
        ; swapFrom   = seg start address of swappable stuff

        mov     AH,03Ch
        mov     BP,04000h
        call    swap_file_io    ; write program to disk file
        jnc     pre1            ; jump to jump to exit

	  mov	  AX,SWAP_NOVMSPACE
          jmp	  short pre5

pre1:   ; free dos memory
        les     BX,dword ptr swapPagePar
        cmp     swapPipeCtr,0
        jle     pre4
        cmp     swapFreeParagraphsCtr,0
        jg      pre2
          mov     swapPipeBufSize,1024
          mov     AX,BX
          add     AX,65 ; add room for pipe file buffer, 1 extra for insurance
          cmp     AX,swapPar
          jbe     pre3
            mov     AX,swapPar
            sub     AX,BX
            cmp     AX,1
            jle     pre2
              dec     AX          ; 1 extra for insurance
              add     BX,AX
              mov     AX,BX
              mov     CL,4
              sal     AX,CL       ; convert paragraphs to bytes
              mov     swapPipeBufSize,AX
              jmp     short pre4
pre2:       mov     swapPipeBufSize,16
            jmp     short pre4
pre3:     mov     BX,AX
pre4:   mov     AX,04A00h
	bdos
        push	CS
        pop	ES
        jnc	pre6

	  mov	  AX,SWAP_FREEMEMERROR
pre5:     mov     swapErrno,AX
          mov     swapErrret,AX
				; failed, carry set, going to exit
pre6:	ret
swap_preamble endp

	assume_DS_swap_codeseg
swap_postamble proc near
        cmp     swapPipeCtr,0
        je      post1
        cmp     swapPipeCnt,0
        je      post1

          call    iWrite          ; flush last of pipe buffer to disk

post1:  ; malloc dos memory
        mov     BX,swapPar
        mov     ES,swapPSP
        mov     AX,04A00h
        bdos
        jc      post2

          mov     AX,03D02h
          mov     BP,03F00h
          call    swap_file_io    ; read program from disk file
          jnc     post3

	    ; fatal exit, everything is stopping

post2:      call    swap_kill_file  ; kill .tmp file
            call    swap_close      ; unhook any interrupts
            push    CS              ; display critical error message and then stop
            pop     DS
            mov     DX,offset CS:swapFatalError
            mov     AH,09h
            bdos
            mov     DS,swapPSP
            push    DS
            pop     ES
            mov     AX,04C01h       ; terminate process
            bdos

post3:	ret
swap_postamble endp

	assume_DS_swap_codeseg
swap proc near
	call	swap_prolog

        mov     oldsp,SP                ; only thing preserved by exec is
        mov     oldss,SS                ; CS and IP, so we must save these
                                        ; in the code segment
        switchStack	CS,<offset CS:newStack-2>

        cmp     swapCtr,0
        jle     sw0

	  call    swap_preamble
          jc	  exit

sw0:    mov     DX,offset CS:scommltl
        mov     BX,offset CS:sparam_block

	mov	AX,4B00h	; load/execute program
	bdos			; perform exec
	cld			; no direction flag bugs

        mov	BX,CS
        mov	ES,BX
        mov	DS,BX	

        jnc	sw1

	  mov	  swapErrno,AX
          mov     swapErrret,-1

sw1:	cmp	swapCtr,0
	jle	sw3

	  switchStack	CS,<offset CS:newStack-2>

	  call    swap_postamble

exit:     call    swap_kill_file

sw3:    ; restore stack
	switchStack	oldss,oldsp

	mov	AX,swapErrret
        cmp     AX,0
        je	sw4

          stc
          ret

sw4: 	clc
        ret
swap endp

;;swap_block_io
        assume  DS:nothing
swap_block_io proc near
sio0:     push    CX
          mov     CX,DX
          xor     DX,DX           ;perfrom DOS function for 16K chunk, except for 1st block
          mov     AX,BP           ;perform DOS function with file handle
          int     21h
          mov     DX,CX
          pop     CX
          jc      sio1
          cmp     AX,DX
          jne     sio1

            sar     DX,1            ;DX has size in bytes, convert to PARAGRAPHS
            sar     DX,1            ;NOTE: must be an even paragraph number of bytes
            sar     DX,1            ;      or we will loose bytes to round off error
            sar     DX,1
            mov     AX,DS
            add     AX,DX           ;advance ES to next chunk to write to disk
            mov     DS,AX
            mov     DX,04000h       ;read&write 16k chunks except for 1st block

          loop    sio0
          clc
          ret
sio1:   stc
        ret
swap_block_io endp

;;swap_file_io
        assume_DS_swap_codeseg
swap_file_io proc near
        xor     CX,CX
        mov     DX,offset CS:swapCWD
        int     21h
        jc      io3

        mov     swapHandle,AX

        mov     CX,swapBlocks
        dec     CX
        mov     DS,swapFrom
        
        assume  DS:nothing

        mov     BX,CS:swapHandle
        mov     DX,04000h
        jcxz    io0
          call    swap_block_io       ;write out all 16k chunks first
          jc      io1
io0:    mov     CX,1
        mov     DX,swapRemainder      ;write out odd ball size now
        call    swap_block_io         ;this should minimize I/O time

        mov     AX,03E00h             ;close file handle
        int     21h
        clc
        jmp	short io3

io1:      mov     AX,03E00h
          int     21h
          stc

io3:    push	CS
        pop	DS
        ret
swap_file_io endp

;;swap_kill_file
        assume  DS:nothing
swap_kill_file proc near
        push    CS
        pop     DS
        mov     DX,offset CS:swapCWD
        mov     AX,04100h
        bdos
        ret
swap_kill_file endp

;;swap_close
        assume  DS:nothing
swap_close proc near
        push    AX
        push    DX
        push    DS

        cmp     CS:swapPipeHandle,0
        je      cl0

          mov     AH,3Eh
          mov     BX,CS:swapPipeHandle
          int     21h

cl0:    cmp     CS:swapWindowCtr,0
        jg      cl1
        cmp     CS:swapPipeCtr,0
        jle     cl2

cl1:      lds     DX,CS:int21Vector
          mov     AX,02521h
          int     21h

cl2:	call    swapRestore1B		; restore original 1B interrupt
	pop     DS
        pop     DX
        pop     AX
        ret
swap_close endp

setIntHandler	proc	near
        push	CS
        pop	DS
        mov	AH,25h
        int	21h
        ret
setIntHandler	endp

swapGrab1B	proc	near
        cmp     CS:swapTrapCBreakCtr,0
	jle	op0
          push    DS
          mov     AX,0351Bh
          int     21h
          mov     word ptr CS:cBreak1BVector,BX   ; save old cBreak 1B Vector
          mov     word ptr CS:cBreak1BVector+2,ES
          mov     DX,offset CS:cBreakStubHandler
          call    setIntHandler
          mov     AX,03523h
          int     21h
          mov     word ptr CS:cBreak23Vector,BX   ; save old cBreak 23 Vector
          mov     word ptr CS:cBreak23Vector+2,ES
          mov     DX,offset CS:cBreakStubHandler
          call    setIntHandler
          pop     DS
op0:	ret
swapGrab1B	endp

;swapRestore1B
;
;restores Control-C & Control Break
;
;WARNING: trashes all registers
swapRestore1B	proc	near
	cmp	CS:swapTrapCBreakCtr,0
        jle	op1
          lds     DX,CS:cBreak1BVector
          mov     AX,0251Bh
          int     21h
          lds     DX,CS:cBreak23Vector
          mov     AX,02523h
          int     21h
op1:	ret
swapRestore1B	endp

;SWAP OUT BOUNDARY
	db 16 dup(0)
swappee     label near
swapPipeBuf db  32 dup (0)              ;before it is piped to the pipe file


;;swap_calc
        assume_DS_swap_codeseg
swap_calc proc near
        ; compute paragraphs that will remain resident
        mov     AX,CS
        sub     AX,swapPSP
        mov     DX,offset CS:swappee
        mov     CL,4
        sar     DX,CL              ; convert offset to seg addr
        add     AX,DX
        mov     swapPagePar,AX  ; save paragraphs that will remain resident

        cmp     swapFreeParagraphsCtr,0
        je      calc0

          mov     DX,swapPar
          sub     DX,swapFreeParagraphs
          cmp     DX,AX
          jbe     calc0            ; if this remainder is <= everything we can swap then do old thing

            mov     swapPagePar,DX
            mov     AX,DX

calc0:  ; compute # of blocks to swap out
        mov     DX,swapPar      ; total paragraphs in program
        sub     DX,AX
        mov     AX,DX
        xor     DX,DX              ; DX:AX == paragraphs that will be swapped out

        mov     CX,1024            ; to get 16K block count
        div     CX
        mov     swapRemainder,04000h  ; default remainder is 16k i.e. no remainder
        or      DX,DX
        jz      calc1

          mov     CL,4
          sal     DX,CL            ; convert from paragraphs to bytes
          mov     swapRemainder,DX
          inc     AX               ; add one to block count for remainder

calc1:  mov     swapBlocks,AX

        mov     AX,swapPagePar
        add     AX,SwapPSP
        mov     swapFrom,AX     ; make swapFrom a real seg addr rather than number of paragraphs to swap
        ret
swap_calc endp

        assume  DS:DGROUP
swap_prolog proc near
        push    ES
        push    DS
        
        push    ES
        push    DS
        
        push    CS
        pop     ES
        
        mov     SI,DX
        mov     DI,offset CS:scommltl
        cld
pro0:     lodsb
          stosb
          or      AL,AL
          jnz     pro0
        mov     AX,offset CS:sfcb1
        mov     CS:spfcb1,offset CS:sfcb1
        mov     AX,offset CS:sfcb2
        mov     CS:spfcb2,AX
        mov     AX,offset CS:scommlt
        mov     CS:scomml,AX

        pop     DS
        pop     ES
        
        push    ES
        pop     DS
        
        mov     AX,env
        mov     word ptr CS:senv,AX
        mov     AX,CS
        mov     CS:scomseg,AX
        mov     CS:spseg1,AX
        mov     CS:spseg2,AX
        mov     CX,offset sheader_end-sheader
        mov     ES,AX
        cld
        mov     SI,offset DGROUP:header
        mov     DI,offset CS:sheader
    rep movsb
        xor     CX,CX
        mov     DI,offset CS:scommlt
        mov     AX,comml+2
        mov     SI,comml
        mov     DS,AX
        mov     CL,[SI]
        inc	CX
        inc	CX			; move extra 0 at end
    rep movsb

        pop     DS
        pop     ES

        mov     AX,CS
        mov     ES,AX
        mov     DS,AX
	ret
swap_prolog endp

;;swap_open
        assume  DS:DGROUP
swap_open proc near
        push    BP
        push    DS
        push    ES

	mov	AX,DGROUP:__tab_size	;transfer tab size to swap control
        mov	CS:swapTabSize,AX	;area

        mov     CS:swapErrno,0
        mov     CS:swapErrret,0
        mov     CS:swapInsideWrite,0
        mov     CS:swapPipeCnt,0
        mov     CS:swapVidOff,0
        mov     CS:swapCurCol,0 ; initialize display window cursor
        mov     CS:swapCurLin,0

	call    swapGrab1B

        mov     AX,_psp
        mov     CS:swapPSP,AX
    if SPTR
        mov     AX,_progpar
        add	AX,_datapar
    else
        mov     AX,_totalpar
    endif
        mov     CS:swapPar,AX

        ; build unique VM file name by using program _PSP in name
        ; this allows swap exec to be recursive across programs
        ; calling maketemp would be preferable, but would require
        ; additional library support, which is very undesirable
        ; this should work perfectly in any single tasking environment
        ; it won't work reliably under WINDOWS because you can have multiple
        ; DOS sessions so I'll have to add some arbitration later

        mov     DI,offset CS:swapVMFile+6
        mov     CX,4
        mov     AX,CS:swapPSP
doit:     push    CX
          xor     DX,DX
          mov     CX,16
          div     CX
          mov     BX,DX
          mov     DL,byte ptr CS:swapHexStr[BX]
          mov     byte ptr CS:[DI],DL
          dec     DI
          pop     CX
          loop    doit

	call	swap_whereami
	call	swap_outputtrap

        pop     ES
        pop     DS
        pop     BP
        ret
swap_open endp

; trap output, and send it to piped file and/or window, if required
swap_outputtrap proc near
        mov     CS:swapPipeHandle,0

        cmp     CS:swapPipeCtr,0
        jg      trap0
        cmp     CS:swapWindowCtr,0
        jg      trap1
        jmp     short trap2
trap0:    lds     DX,CS:swapPipeFile
          xor     CX,CX
          mov     AH,3Ch
          int     21h
          jc      trap1
            mov     CS:swapPipeHandle,AX
trap1:    mov     AX,03521h
          int     21h
          mov     word ptr CS:int21Vector,BX
          mov     word ptr CS:int21Vector+2,ES  ; save old interrupt handler
          mov     DX,offset CS:int21Handler     ; new interrupt handler
	  call    setIntHandler
          mov     AH,15
          int     10h
          mov     CS:swapVidPage,BH             ; get current display page
trap2:	ret
swap_outputtrap endp

; determines where the program is at currently
; and remembers where the SWAP.TMP file is being placed
; so if the user changes directories, we can still get back to it
;
    if LPTR
swap_whereami proc near
	push	ES
	cmp	CS:swapTempCheckCtr,0
        jle	open5
          mov     AX,offset CS:swapTMP           ; check for definition of TMP
	  mov	  CX,2
open00:     push    CX
            push    CS
            push    AX
            call    _getenv
            add     SP,4
            or      DX,DX
            jz      open0
              push    DS
              
              push    DX
              push    AX

              push    DX
              push    AX
              call    _strlen
              add     SP,4
              mov     CX,AX

              pop     SI
              pop     DS

              push    CS
              pop     ES
              mov     DI,offset CS:swapCWD
              inc     CX          ; move trailing zero also
              cld
          rep movsb

              pop     DS
              pop     CX
              jmp     short open1 ; jump into file maker as if this was CWD
open0:      mov     AX,offset CS:swapTEMP           ; check for definition of TEMP
	    pop	    CX
            loop    open00        ; circle back if we haven't checked for TEMP yet

open5:  mov     AX,132        ; max buffer size for getcwd
        push    AX
        mov     AX,offset CS:swapCWD
        push    CS
        push    AX
        call    _getcwd
        add     SP,6
        mov     AX,offset CS:swapCWD
        push    CS
        push    AX
        call    _strlen
        add     SP,4

open1:  mov     DI,offset CS:swapCWD
        add     DI,AX
open2:    cmp     byte ptr CS:[DI-1],'\'
          je      open4
          cmp     byte ptr CS:[DI-1],';'
          jne     open3
            dec     DI          ; get rid of ';' at end
            jmp     short open2 ; and go back and test for '\'
open3:    mov     byte ptr CS:[DI],'\'
          inc     DI
open4:  mov     SI,offset CS:swapVMFile
        push    DS
        mov     AX,CS
        mov     DS,AX
        mov     ES,AX
        mov     CX,6
        cld
    rep movsw
        movsb          ; move zero delimiter in addition to file name
        pop     DS
        pop	ES
        ret
swap_whereami endp
    else
swap_whereami proc near
	cmp	byte ptr CS:swapTempCheckCtr,0
        jle	open5
          mov     AX,offset DGROUP:swapTMP_DS	; check for definition of TMP
	  mov	  CX,2
open0:	    push    CX
            push    AX
            call    _getenv
            add     SP,2
            pop     CX
            or      AX,AX
            jnz     open1
              mov     AX,offset DGROUP:swapTEMP_DS	; check for definition of TEMP
            loop    open0			  	; circle back if we haven't checked for TEMP yet

open5:  mov     AX,132				; max buffer size for getcwd
        push    AX
        mov     AX,offset DGROUP:swapCWD_DS
        push    AX
        call    _getcwd
        add     SP,4

        mov     AX,offset DGROUP:swapCWD_DS

open1:  push    ES
        push    AX
        push    AX
        call    _strlen
        add     SP,2
        mov     CX,AX

        ; move from DS landing zone to CS area that swap uses
        pop     SI
        push    CS
        pop     ES
        inc     CX				;move trailing zero also
        mov     DI,offset CS:swapCWD
        cld
    rep movsb
	pop     ES

	; make sure path to swap file has a backslash at the end,
        ; and nothing else

        mov     DI,offset CS:swapCWD
        add     DI,AX
open2:    cmp     byte ptr CS:[DI-1],'\'
          je      open4
          cmp     byte ptr CS:[DI-1],';'
          jne     open3
            dec     DI			; get rid of ';' at end
            jmp     short open2		; and go back and test for '\'
open3:    mov     byte ptr CS:[DI],'\'	;add '\' to end, if not present
          inc     DI
open4:  mov     SI,offset CS:swapVMFile

        push    ES
        push    DS

        mov     AX,CS
        mov     DS,AX
        mov     ES,AX
        mov     CX,6
        cld
    rep movsw
        movsb          ; move zero delimiter in addition to file name

        pop     DS
        pop     ES

        ret
swap_whereami endp
    endif

; C FUNCTIONAL INTERFACE

        c_public swap_window
func    swap_window
	mov	BX,SP
        mov     SI,offset CS:swapColumn
        push    DS
        lds     DX,SS:P-2[BX]
        mov     CS:[SI],DX
        mov     CS:[SI+2],DS
        lds     DX,SS:P+4-2[BX]
        mov     CS:[SI+4],DX
        mov     CS:[SI+6],DS
        pop     DS
        ret
c_endp  swap_window

        c_public swap_pipe
func    swap_pipe
	mov	BX,SP
     if SPTR
        mov     AX,SS:P-2[BX]
        mov     word ptr CS:swapPipeFile,AX
        mov     word ptr CS:swapPipeFile+2,DS
     else
        push    DS
        lds     AX,SS:P-2[BX]
        mov	word ptr CS:swapPipeFile,AX
        mov     word ptr CS:swapPipeFIle+2,DS
        pop	DS
     endif
        ret
c_endp  swap_pipe

        c_public swap_freeparagraphs
func    swap_freeparagraphs
        mov     BX,SP
        mov     AX,SS:P-2[BX]
        mov     CS:swapFreeParagraphs,AX
        ret
c_endp  swap_freeparagraphs

        c_public swap_on
func    swap_on
        inc     CS:swapCtr
        ret
c_endp  swap_on

        c_public swap_off
func    swap_off
        dec     CS:swapCtr
        ret
c_endp  swap_off

        c_public swap_tempcheckon
func    swap_tempcheckon
        inc     CS:swapTempCheckCtr
        ret
c_endp  swap_tempcheckon

        c_public swap_tempcheckoff
func    swap_tempcheckoff
        dec     CS:swapTempCheckCtr
        ret
c_endp  swap_tempcheckoff

        c_public swap_windowon
func    swap_windowon
        inc     CS:swapWindowCtr
        ret
c_endp  swap_windowon

        c_public swap_windowoff
func    swap_windowoff
        dec     CS:swapWindowCtr
        ret
c_endp  swap_windowoff

        c_public swap_pipeon
func    swap_pipeon
        inc     CS:swapPipeCtr
        ret
c_endp  swap_pipeon

        c_public swap_pipeoff
func    swap_pipeoff
        dec     CS:swapPipeCtr
        ret
c_endp  swap_pipeoff

        c_public swap_freeparagraphson
func    swap_freeparagraphson
        inc     CS:swapFreeParagraphsCtr
        ret
c_endp  swap_freeparagraphson

        c_public swap_freeparagraphsoff
func    swap_freeparagraphsoff
        dec     CS:swapFreeParagraphsCtr
        ret
c_endp  swap_freeparagraphsoff

        c_public swap_trapcbreakon
func    swap_trapcbreakon
        inc     CS:swapTrapCBreakCtr
        ret
c_endp  swap_trapcbreakon

        c_public swap_trapcbreakoff
func    swap_trapcbreakoff
        dec     CS:swapTrapCBreakCtr
        ret
c_endp  swap_trapcbreakoff

	c_public swap_clearkeyboardon
func	swap_clearkeyboardon
	inc	CS:swapClearBufferCtr
        ret
c_endp	swap_clearkeyboardon

	c_public swap_clearkeyboardoff
func	swap_clearkeyboardoff
	dec	CS:swapClearBufferCtr
        ret
c_endp	swap_clearkeyboardoff

        c_public swap_ison
func    swap_ison
        mov     AX,CS:swapCtr
tm1:    xor     DX,DX
        ret
c_endp  swap_ison

        c_public swap_iswindowon
func    swap_iswindowon
        mov     AX,CS:swapWindowCtr
        jmp     short tm1
c_endp  swap_iswindowon

        c_public swap_ispipeon
func    swap_ispipeon
        mov     AX,CS:swapPipeCtr
        jmp     short tm1
c_endp  swap_ispipeon

        c_public swap_isfreeparagraphson
func    swap_isfreeparagraphson
        mov     AX,CS:swapFreeParagraphsCtr
        jmp     short tm1
c_endp  swap_isfreeparagraphson

	c_public swap_istrapcbreakon
func	swap_istrapcbreakon
	mov	AX,CS:swapTrapCBreakCtr
	jmp	short tm1
c_endp	swap_istrapcbreakon

	c_public swap_isclearkeyboardon
func	swap_isclearkeyboardon
	mov	AX,CS:swapClearBufferCtr
        jmp	short tm1
c_endp	swap_isclearkeyboardon

	c_public swap_istempcheckon
func	swap_istempcheckon
	mov	AX,CS:swapTempCheckCtr
        jmp	short tm1
c_endp	swap_istempcheckon

endif ;_WINDOWS

endif ;SPTR

include exec2.asm

