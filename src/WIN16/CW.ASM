;_ winstart.asm
; Copyright (C) 1991 by Walter Bright
; All Rights Reserved
; Written by G. Eric Engstrom
; Modified by Richard S. Sadowsky August 1, 1993


STARTUP EQU 1
include macros.asm
includelib libw.lib

WF_PMODE       equ     0001h   ; from WINDOWS.H

       DOSSEG          ;have linker fix ordering of segments

       extrn   INITTASK    : far
       extrn   WAITEVENT   : far
       extrn   INITAPP     : far
        extrn   GETWINFLAGS : far
       extrn   LOCKSEGMENT : far
    if LCODE
       extrn   WINMAIN     : far
       extrn   __cinit     : far
       extrn   __setargv   : far
     ifdef _MBCS
       extrn   ___initmbctype : far
     endif
       extrn   __exit      : far
       extrn   _exit       : far
    else
       extrn   WINMAIN     : near
       extrn   __cinit     : near
       extrn   __setargv   : near
     ifdef _MBCS
       extrn   ___initmbctype : far
     endif
       extrn   __exit      : near
       extrn   _exit       : near
    endif
 
       extrn __osmode      : byte
       extrn __cpumode     : byte

        extrn  _aenvseg        : word
        extrn  __datapar       : word
        extrn  __heapbottom    : word
        extrn  __pastdata      : word

       public  __acrtused
       
__acrtused     equ     09876h
       
NULL   segment para  public 'BEGDATA'
       dd      0               ; windows 3.0 seems to need these 0 bytes
       dw      5,0,0,0,0,0     ; more data areas for Windows to scribble on
NULL   ends

;Regular initialized data goes in _DATA
_DATA  segment para  public 'DATA'

       extrn   __psp       : word

prevInstance   dw      ?       ; ushort
hInstance      dw      ?       ; ushort
commandLine    dw      ?,?     ; uchar *
showWindow     dw      ?       ; ushort
__BASE         dw      ?       ; ushort bottom of stack

_DATA  ends

CDATA  segment word  common 'DATA'
CDATA  ends

;Constant data, such as switch tables, go here.
CONST  segment word  public 'CONST'
CONST  ends

;Segment for uninitialized data. This is set to 0 by the startup code,
;so it does not consume room in the executable file.
_BSS   segment para  public 'BSS'
_BSS   ends

;Segment group Windows needs
XOB    segment word  public 'BSS'
XOB    ends
XO     segment word  public 'BSS'
XO     ends
XOE    segment word  public 'BSS'
XOE    ends

;Segment group Windows needs
XPB    segment word  public 'DATA'
XPB    ends
XP     segment word  public 'DATA'
XP     ends
XPE    segment word  public 'DATA'
XPE    ends

;Function pointers to near constructors
XIB    segment word  public 'DATA'
XIB    ends
XI     segment word  public 'DATA'
XI     ends
XIE    segment word  public 'DATA'
XIE    ends

;Function pointers to far constructors
XIFB   segment word  public 'DATA'
XIFB   ends
XIF    segment word  public 'DATA'
XIF    ends
XIFE   segment word  public 'DATA'
XIFE   ends

;Function pointers to near destructors
XCB    segment word  public 'DATA'
XCB    ends
XC     segment word  public 'DATA'
XC     ends
XCE    segment word  public 'DATA'
XCE    ends

;Function pointers to far destructors
XCFB   segment word  public 'DATA'
XCFB   ends
XCF    segment word  public 'DATA'
XCF    ends
XCFE   segment word  public 'DATA'
XCFE   ends

DBDATA segment word  common 'DATA'
DBDATA ends

DGROUP group   NULL,_DATA,CDATA,CONST,_BSS,XIFB,XIF,XIFE,XIB,XI,XIE,XOB,XO,XOE,XPB,XP,XPE,XCB,XC,XCE,XCFB,XCF,XCFE


_TEXT  segment para public 'CODE'
       assume  cs:_TEXT

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; When Windows calls the startup code, __astart, the registers are:
;      AX      0
;      BX      size in bytes of the stack
;      CX      size in bytes of the heap
;      DI      handle of new application instance
;      SI      handle of previous application instance
;      BP      0
;      ES      segment of PSP
;      DS      set to DGROUP
;      SS      == DS
;      SP      offset of first byte of stack

       assume DS:DGROUP
       
__astart:
       cld                             ;no direction flag bugs
       xor     BP,BP
       push    BP                      ;end of BP chain for stack walkers

       call    INITTASK                ;initialize TASK
       or      AX,AX
       je      st1

         mov     __psp,ES
         mov     AX,ES:2Ch             ;environment segment
         mov     _aenvseg,AX
         add     CX,0100h              ;leave some bytes for stack overflow
         jc      st1                   ;if out of memory

           dec     CX

           ;Compute number of paragraphs in data segment
           mov     AX,CX
            add     AX,0Fh
            mov     CL,4
            shr     AX,CL
    if SPTR
           mov     __datapar,AX
           mov     __heapbottom,AX     ; in case there is no heap
           mov     __pastdata,AX       ; this makes our malloc fail gracefully
    endif
           ;Save away parameters for later
           mov     prevInstance,SI
           mov     hInstance,DI
           mov     commandLine,BX
           mov     commandLine+2,ES
           mov     showWindow,DX

           mov     AX,0FFFFh
           push    AX
           call    LOCKSEGMENT         ;lock the data segment
           
           mov     __osmode,2          ;indicates windows

           ;test for protected mode
            call    GETWINFLAGS
            test    AX,WF_PMODE
            jz      st3                        ;WF_PMODE is always set if STANDARD or ENHANCED mode
           mov     __cpumode,1         ;we're in protected mode

st3:
           ;Clear the event that started the task
           xor     AX,AX
           push    AX
           call    WAITEVENT

           ;Initialize the queue and support routines for the application
           push    hInstance
           call    INITAPP
           or      AX,AX
           jne     st4                 ;ok

       ;The application failed to initialize, so abort it
st1:   mov     AL,0FFh
       push    AX
       call    __exit

st4:
 ifdef _MBCS
           call    ___initmbctype
 endif
           call    __setargv
           call    __cinit             ;initialize C program

       ;Call the application's entry point
       push    hInstance
       push    prevInstance
       push    commandLine+2
       push    commandLine
       push    showWindow
       call    WINMAIN
       push    AX
       call    _exit

_TEXT  ends

       end     __astart
