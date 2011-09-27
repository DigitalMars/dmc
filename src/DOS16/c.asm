;_ c.asm
; OS2 support added by Nikki Locke May 1989
; Copyright (C) 1985-2001 by Digital Mars
; All rights reserved.
; www.digitalmars.com
; Written by Walter Bright
; C startup file

;I8086T   equ  1       ;defined to create a .COM version (CT.OBJ)

       DOSSEG          ;have linker fix ordering of segments

ifdef DOS16RM
       .286P           ;Disable automatic FWAIT generation

       ;Kick out any model but large as an error.
       ifndef I8086L
       %out    Only L model supported with RATIONAL DOS Extender.
       end
       endif

else
       .286C           ;disable automatic FWAIT generation

;*********************************************

; Determine which memory model we are assembling for. For .COM files,
; force S model.
    ifdef I8086T
I8086S equ     1
    else
    ifndef I8086S
    ifndef I8086M
    ifndef I8086C
    ifndef I8086L              ;if none of the memory models are defined
    ifndef I8086V
I8086S equ     1               ;default to S model
    endif
    endif
    endif
    endif
    endif
    endif
endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; These global values are used for huge pointer manipulation.
; For OS/2 and Windows, they are pulled out of DLL's (an undocumented
; feature).

    ifndef __OS2__             ;Windows has separate startup code
       public  __AHSHIFT,__AHINCR
    ifdef DOS16RM
__AHSHIFT      equ     3
__AHINCR       equ     8
    else ;DOS in real mode
__AHSHIFT      equ     12
__AHINCR       equ     1000h
    endif
    endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Macros specific to each memory model in an attempt to make it easier
; to write memory model independent code.
;      P                       Offset on BP to first argument on stack
;      SPTR                    1 if small data model
;      LPTR                    1 if large pointers (large data)
;      LCODE                   1 if large code model
;      SSeqDS                  1 if SS == DS at all times
;      SIZEPTR                 # of bytes in a pointer
;      func                    Declare a function as NEAR or FAR

;;;;;;;;;;;;;; SMALL MEMORY MODEL ;;;;;;;;;;;;;;;;;

ifdef I8086S

P      equ     4       ; Offset of start of parameters on the stack frame
SPTR   equ     1
LPTR   equ     0
LCODE  equ     0
SSeqDS equ     1
SIZEPTR equ    2       ; Size of a pointer

func   macro   name
name   proc    near
       endm

endif

;;;;;;;;;;;;;;;;; MEDIUM MEMORY MODEL ;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ifdef I8086M

P      equ     6       ; Offset of start of parameters on the stack frame
SPTR   equ     1
LPTR   equ     0
LCODE  equ     1
SSeqDS equ     1
SIZEPTR equ    2

func   macro   name
name   proc    far
       endm

endif

;;;;;;;;;;;;;;;;; COMPACT MEMORY MODEL ;;;;;;;;;;;;;;

ifdef I8086C

P      equ     4       ; Offset of start of parameters on the stack frame
SPTR   equ     0
LPTR   equ     1
LCODE  equ     0
SSeqDS equ     0
SIZEPTR equ    4

func   macro   name
name   proc    near
       endm

endif

;;;;;;;;;;;;;;;; LARGE MEMORY MODEL ;;;;;;;;;;;;;;;;;;;

ifdef I8086L

P      equ     6       ; Offset of start of parameters on the stack frame
SPTR   equ     0
LPTR   equ     1
LCODE  equ     1
SSeqDS equ     0
SIZEPTR equ    4

func   macro   name
name   proc    far
       endm

endif

;;;;;;;;;;;;;;;; OVERLAY MEMORY MODEL ;;;;;;;;;;;;;;;;;;;

ifdef I8086V

P      equ     6       ; Offset of start of parameters on the stack frame
SPTR   equ     0
LPTR   equ     1
LCODE  equ     1
SSeqDS equ     0
SIZEPTR equ    4

func   macro   name
name   proc    far
       endm

endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Other more or less useful macros

get_seg_size macro reg,segnum  ;Put size of segnum in reg.
       lsl     reg, segnum
       inc     reg
       endm

_if    macro   arg1,cond,arg2,lbl
       cmp     arg1,arg2
       j&cond  lbl
       endm

bdos   macro   func
       ifnb    <func>
        mov    AH,func
       endif
       int     21h
       endm

.retf  macro   val             ;force assembler to build a far return
       ifnb    <val>
        db     0CAh
        dw     val
       else
        db     0CBh
       endif
       endm

ifndef DOS16RM
       public  __BASE
       public  __progpar
    if LPTR
       public  __totalpar
    endif
       extrn   __datapar:word
       extrn   __heapbottom:word
       extrn   __pastdata:word
endif
       extrn   _aenvseg:word
ifdef __OS2__
       public  __ginfoseg,__linfoseg,__threadid
endif
       public  __exe_type

       extrn   __psp:word
       extrn   __osmajor:byte,__osminor:byte,__cpumode:byte,__dos:word
       extrn   __argc:word,__argv:word
       extrn   __environ:word
       extrn   __stack:word            ;default stack size
       extrn   __okbigbuf:word
    ifdef _MBCS
       extrn   __mbctype:byte
    endif
    if LCODE
       extrn   _exit:far, _sbrk:far, _free:far, _main:far;, __entry:far
       extrn   __exit:far, __cinit:far, ___faterr:far
     ifdef _MBCS
       extrn   ___initmbctype:far
     endif
    else
       extrn   _exit:near, _sbrk:near, _free:near, _main:near;, __entry:near
       extrn   __exit:near, __cinit:near, ___faterr:near
     ifdef _MBCS
       extrn   ___initmbctype:near
     endif
    endif

    ifdef __OS2__
       includelib OS2.LIB
       extrn   DOSWRITE:far
       extrn   DOSGETVERSION:far
       extrn   DOSEXIT:far
       extrn   DOSGETMACHINEMODE:far
       extrn   DOSREALLOCSEG:far
       extrn   DOSGETINFOSEG:far
    endif

    ifdef DOS16RM
;      includelib zrl.lib              ;DOS Extender library
    else
       public  __acrtused
__acrtused equ 1234                    ;cause linker to pull in this module
    endif

       public  __aDBdoswp
__aDBdoswp equ 0D6D6h

ARGMAX =       64                      ;max # of command line args
CR     =       13
LF     =       10

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;The code segment must be lower in memory than the data segment
;(so we can add to the top of the data segment).

    ifdef DOS16RM
NULLSEG segment para public 'CODE'
NULLSEG ends
    endif

    if LCODE
C_TEXT segment word public 'CODE'
       ifdef @Version
       if @Version lt 600
       assume CS:C_TEXT
       endif
       else
       assume CS:C_TEXT
       endif
    else
_TEXT  segment word public 'CODE'
       ifdef @Version
       if @Version lt 600
       assume CS:_TEXT
       endif
       else
       assume CS:_TEXT
       endif
    endif

    ifdef      I8086T
       ;Note that this is not 100h! This is because the DOSSEG directive
       ;causes LINK to move everything up 10h bytes! Why, I dunno.
       org     0F0h



start:
       jmp     begin
    else
       mov     DX,offset DGROUP:nullfp         ;NULL function pointer
       jmp     fatmsg
    endif

    if LCODE
C_TEXT ends
    else
_TEXT  ends
    endif

;Define a segment so we can find the end of the code
C_ETEXT segment word public 'ENDCODE'
C_ETEXT ends

    ifdef I8086T
CGROUP group   _TEXT,C_ETEXT           ;code must all fit in 1 segment
    endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set up segments for data

;Segment so we can find the start of DGROUP

NULL   segment para public 'BEGDATA'           ;note PARAGRAPH alignment
NULL   ends

;Regular initialized data goes in _DATA

_DATA  segment word public 'DATA'
_DATA  ends

;Function pointers to far constructors
XIFB   segment word public 'DATA'
XIFB   ends
XIF    segment word public 'DATA'
XIF    ends
XIFE   segment word public 'DATA'
XIFE   ends

    ife LCODE
;Function pointers to near constructors
XIB    segment word public 'DATA'
XIB    ends
XI     segment word public 'DATA'
XI     ends
XIE    segment word public 'DATA'
XIE    ends

;Function pointers to near destructors
XCB    segment word public 'DATA'
XCB    ends
XC     segment word public 'DATA'
XC     ends
XCE    segment word public 'DATA'
XCE    ends
    endif

;Function pointers to far destructors
XCFB   segment word public 'DATA'
XCFB   ends
XCF    segment word public 'DATA'
XCF    ends
XCFE   segment word public 'DATA'
XCFE   ends

;Constant data, such as switch tables, go here.

CONST  segment word public 'CONST'
CONST  ends

;Segment for uninitialized data. This is set to 0 by the startup code,
;so it does not consume room in the executable file.

_BSS   segment word public 'BSS'
_BSS      ends

;Segment to provide an initial stack so DOS can fire up the program.
;We'll set up our own stack later. Use the space later for the
;program arguments.
;Our stack in large data models is in its own segment, not part of
;DGROUP. This is different than MSC.

    ifdef      I8086T
STACK  segment word public 'STACK'             ;no stack for .COM files
bssend equ     $
    else
STACK  segment para stack 'STACK'
    endif

    ifdef DOS16RM
       org     $+128           ;a 256 byte stack is necessary
    else
cmdline   equ  $               ;where the command line will be placed.
    endif
    ifndef I8086T
       org     $+128           ;a 128 byte stack is necessary to fire
                               ;up .EXE programs, even if a CLI is the
                               ;first instruction!
    ifdef __OS2__
       org     $+130           ;OS/2 stack until _stack processed
    endif
dummy  equ     $
    endif
STACK  ends


;Stuff all these segments into one group so they can all be accessed by DS
; DOSSEG should do this, but who trusts it?
    ife LCODE
DGROUP group   NULL,XIFB,XIF,XIFE,XIB,XI,XIE,XCB,XC,XCE,XCFB,XCF,XCFE,_DATA,CONST,_BSS,STACK
    else
DGROUP group   NULL,XIFB,XIF,XIFE,XCFB,XCF,XCFE,_DATA,CONST,_BSS,STACK
    endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Form the start of DGROUP

NULL   segment

       ;for programs that dereference NULL string pointers
       db      'ERROR: NULL pointer', 0

       ;Put baggage here where it can be safely stomped by NULL pointer
       ;assignments. Should do a checksum of this to detect these.
       db      'Digital Mars C 4.0 library, Copyright (C) 1988-2001 '
    ifdef I8086S
       db      'S'
    endif
    ifdef I8086M
       db      'M'
    endif
    ifdef I8086C
       db      'C'
    endif
    ifdef I8086L
       db      'L'
    endif
    ifdef I8086V
       db      'V'
    endif
       db      ', written by Walter Bright',0

NULL   ends

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Initialized data globals

ifdef DOS16RM
       extrn   d16_seginfo_struct:byte
endif

_DATA  segment

ifndef NODOSSEG
       ;These symbols are created by the linker (DOSSEG switch)
       extrn   _edata:byte             ;first location in first BSS segment
    ifndef I8086T
       extrn   _end:byte               ;first location in first STACK segment
    endif
endif

prgnam db      0                       ;dummy program name (null string)
argv   dw      offset DGROUP:prgnam    ;which is the program name
    if LPTR
       dw      seg DGROUP
    endif
       db      (ARGMAX+1)*SIZEPTR dup (0)      ;the rest of the arguments
                                       ;(this area doubles as the
                                       ;initial stack, so no initialized
                                       ;data here!)
                                       ;The +1 is so argv[] is always
                                       ;followed by a NULL.
ifdef DOS16RM
       dd      d16_seginfo_struct
else
__BASE         dw      ?       ;offset to stack overflow check word
                               ;(if this word changes, we have a stack
                               ;overflow)
__progpar      dw      ?       ;# of paragraphs in PSP + code segment
    if LPTR
__totalpar     dw      ?       ;total # of paragraphs in program
    endif
endif

nomem  db      CR,LF,'Not enough memory',CR,LF,0
badstk db      CR,LF,'Bad stack size parameter',CR,LF,0
baddos db      CR,LF,'DOS 1.xx not supported',CR,LF,0
badcmd db      CR,LF,'Max of 32 args allowed',CR,LF,0
nullfp db      CR,LF,'NULL function pointer',CR,LF,0

ifdef DOS16RM
cmdline db     129 dup (0)
endif

       even

public  _acmdln, _atopsp

_acmdln                dw      0       ; Offset of command line string in env
_atopsp                dw      0       ; top of stack (heap bottom)


    ifndef __OS2__
public ___aDBswpflg,___aDBswpchk
___aDBswpflg   dw      0
___aDBswpchk   dw      offset retaddr
    endif

;Set exe file type
ifdef __OS2__
__exe_type     dw      40h     ;EXE_OS2
else
ifdef SEGEXE
__exe_type     dw      4       ;EXE_ZPM
else
ifdef DOS16RM
__exe_type     dw      2       ;EXE_DOS16RM
else
ifdef I8086T                   ;.COM file
__exe_type     dw      1       ;EXE_DOS
else
__exe_type     dw      1       ;EXE_DOS
endif
endif
endif
endif

;OS/2 only globals
ifdef __OS2__
__ginfoseg     dw      ?       ;selector for global information segment
__threadid     dw      6       ;far pointer to current thread ID
__linfoseg     dw      ?       ;selector for local information segment
endif

_DATA  ends

    if LCODE
C_TEXT segment word public 'CODE'
       ifdef @Version
       if @Version lt 600
       assume CS:C_TEXT
       endif
       else
       assume CS:C_TEXT
       endif
    else
_TEXT  segment word public 'CODE'
       ifdef @Version
       if @Version lt 600
       assume CS:_TEXT
       endif
       else
       assume CS:_TEXT
       endif
    endif

ifndef DOS16RM
       assume DS:DGROUP
endif


    ifdef __OS2__
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;      OS/2 Start-Up Conditions:
;
;      DS              Automatic Data Segment
;      SS:SP           Stack Segment and Initial Stack Pointer
;      ES              0000 ?
;      AX              Selector of Environment Segment
;      BX              Offset of Command Line in Environment Segment
;      CX              Size of Automatic Data Segment (CX=0 means 65536 bytes)
;      BP              0000 ?

start:                         ;program entry point
       mov     _atopsp,sp      ;top of Stack
       dec     cx
       mov     __pastdata,cx
       mov     __heapbottom,cx
       mov     _aenvseg,ax     ; Handle of Environment Segment
       mov     __psp,ax
       mov     _acmdln,bx      ; Offset of Command Line String
       mov     bx,cx
       mov     cl,4
       shr     bx,cl
       mov     __datapar,bx    ;# of paragraphs in data segment

       ;Figure out what version of OS/2 we're running under.

       push    DS
       mov     AX,offset DGROUP:__dos
       push    AX
       call    DOSGETVERSION
       mov     AX,__dos
       xchg    AH,AL
       mov     __dos,AX        ;swap halves

       ;Find out if we are in real mode

       push    DS
       mov     AX,offset DGROUP:__cpumode
       push    AX
       call    DOSGETMACHINEMODE

       ;Get info about current GDT and LDT

       push    DS
       mov     AX,offset DGROUP:__ginfoseg
       push    AX
       push    DS
       mov     AX,offset DGROUP:__linfoseg
       push    AX
       call    DOSGETINFOSEG

       mov     DI,_acmdln
       mov     ES,_aenvseg     ;segment of command line
       mov     si,di           ;save
       mov     cx,0ffffh
       xor     al,al
       cld
       repne   scasb           ;argv[0]
       repne   scasb           ;rest of command line
       inc     cx
       neg     cx              ;cx = length of command line
       mov     bx,es
       mov     ax,ds
       mov     es,ax           ;es points to data segment
       mov     ds,bx           ;ds points to command line segment

    else
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Program segment prefix:
;      00h             int 20h instruction
;      02h             top of memory in paragraphs
;      80h             number of bytes in command line
;      81h             the command line
; Input (for .EXE files):
;      DS,ES           segment of program segment prefix
;      CS              segment of prog (CS = DS + 10h)
;      IP              offset of $start from prog
;      SS              right after end of stack segment
;      SP              100h (the stack grows negative)

    ifdef      I8086T
begin:
       ;Monkey around because the assembler won't give us
       ;the data segment start. We have to compute it.
       cli
       mov     AX,offset CGROUP:C_ETEXT
       add     AX,15           ;round up to next paragraph
       mov     CX,4
       shr     AX,CL           ;convert offset to paragraphs
       mov     CX,CS
       add     AX,CX           ;now AX = segment of DGROUP
    else
    ifdef DOS16RM
       public  start
    endif
start:                         ;program entry point
       cli                     ;turn off till we get stack fixed up
       cld
       mov     AX,seg DGROUP   ;start of data segment (in paragraphs)
    endif

       mov     DS,AX           ;DS = DGROUP
ifdef DOS16RM
       assume  ds:DGROUP
       mov     __cpumode,1     ;DOS Extender is in protected mode
endif
       mov     __psp,ES        ;save segment of PSP
       mov     BX,ES:2Ch       ;environment segment
       mov     _aenvseg,BX
ifndef DOS16RM
       mov     BX,AX
       sub     BX,__psp        ;# of paragraphs from PSP to DGROUP
       mov     __progpar,BX    ;and save that

       mov     BX,ES:2         ;top of memory (in paragraphs)
       sub     BX,AX           ;BX = # of paragraphs we have in data seg
       _if     BX b 1000h, L10 ;if less than 64k
       mov     BX,0FFFh        ;round down
L10:   mov     DX,AX           ;DX = DGROUP
       add     DX,BX
       inc     DX              ;DX = DGROUP + data seg size + round
       mov     CL,4
       shl     BX,CL
       mov     SS,AX           ;SS = DGROUP
       mov     SP,BX           ;set SP to end of DGROUP

       ;See if we have enough stack space before we turn interrupts back on.
       sub     BX,offset DGROUP:cmdline ;max of allocated addresses
       _if     BH ae 2, L11             ;need at least 512 for now
       mov     DX,offset DGROUP:nomem
       jmp     fatmsg
endif
L11:   ;We've got a temporary stack now, so we can allow interrupts
       sti

       ;Figure out what version of MS-DOS we're running under.
       bdos    30h
       mov     word ptr __dos,AX
       _if     AL ae 2, L7     ;if 2.00 or later
       mov     DX,offset DGROUP:baddos
       jmp     fatmsg

L7:

       ;Transfer the command line to cmdline, as the PSP is not
       ;necessarilly in the data segment.
       ;DS = DGROUP
       ;ES = PSP

ifdef DOS16RM
       mov     AX,DS
       push    ES
       pop     DS              ;DS = PSP
       mov     ES,AX           ;ES = DGROUP
else
       push    ES
       pop     DS              ;DS = PSP
       push    SS
       pop     ES              ;ES = DGROUP
endif
       mov     SI,80h
       cld
       lodsb                   ;AL = # of bytes in command line
       xor     AH,AH
       mov     CX,AX           ;# of bytes to copy
    endif
       ; ES = DGROUP
       ; DS:SI -> command line

       mov     DI,offset DGROUP:cmdline        ;ES:DI -> cmdline
       rep     movsb           ;transfer to stack
       xor     AL,AL
       stosb                   ;and a terminating 0

       ;Now set DS to be the same as ES and SS (DGROUP)

       mov     AX,ES
       mov     DS,AX

ifndef DOS16RM
       ;Round up DI, which will form the bottom of the stack
       inc     DI
       and     DI,0FFFEh
       mov     __BASE,DI
    if SPTR
       mov     word ptr [DI],55AAh     ;stack overflow check word
    endif
endif

ifdef _MBCS
       ;Set up MBCS environment
       call    ___initmbctype
endif

       ;Set up __argc and argv

ifdef DOS16RM
       assume  es:DGROUP,ds:DGROUP
endif
       mov     SI,offset DGROUP:cmdline        ;DS:SI -> start of command line
    ifdef __OS2__
       xor     BX,BX           ;OS/2 includes argv[0] on command line
    else
    ifdef SEGEXE
       xor     BX,BX           ;ZPM includes argv[0] on command line
    else
       mov     BX,SIZEPTR      ;number of args * SIZEPTR
    endif
    endif

L1:    lodsb                   ;get char from command line
       _if     AL ne '=', L5
       _if     __stack e 0, L5 ;if ignore '=nnnn' command
       call    set_stack       ;set _stack
L5:    or      AL,AL           ;end of command line?
       jz      L2              ;yes, done
       mov     DX,9*256 + ' '
       _if     AL e DL, L1
       _if     AL e DH, L1     ;eat spaces and tabs preceding argument
       _if     AL e '"', L22
       _if     AL ne "'", L21
L22:   mov     DL,AL
       mov     DH,AL
       inc     SI              ;point past the ' or "

L21:   _if     BX be ARGMAX*SIZEPTR, L20
       mov     DX,offset DGROUP:badcmd
       jmp     fatmsg

L20:   dec     SI              ;address of start of parameter
       mov     argv[BX],SI     ;store in argv
    if LPTR
       mov     argv+2[BX],DS
    endif
       mov     DI,SI
       add     BX,SIZEPTR      ;next slot in argv
       xor     AL,AL           ;last char read
L4:    mov     AH,AL           ;remember last char read
       lodsb                   ;get char of parameter
       stosb                   ;and store it
       or      AL,AL           ;done?
       jz      L2a             ;yes
 ifdef _MBCS
       push    BX
       mov     BL,AL
       xor     BH,BH
       test    __mbctype[BX+1],04h   ;check MBCS 1st byte (_M1)
       pop     BX
       jz      L4a
       lodsb                         ;get 2nd char of parameter
       stosb                         ;and store it
       or      AL,AL                 ;done?
       jz      L2a                   ;yes
       xor     AL,AL                 ;disable last char
L4a:
 endif
       ;Remove any \  that are immediately followed by " 
       _if     AX ne <'\'*256 + '"'>, L3a
       dec     DI              ;remove '\'
       dec     DI
       stosb
       jmp     L4

L3a:   _if     AL e DL, L3     ;end of parameter
       _if     AL ne DH, L4    ;not end of parameter
L3:    xor     AL,AL
       dec     DI
       stosb                   ;terminate parameter with a 0
       jmp     L1              ;look for next parameter
L2a:
    ifdef __OS2__
       cmp     BX,SIZEPTR      ;is this argv[0]
       jz      L3              ;if so, then go for the next argument
    endif

L2:
       shr     BX,1            ;get arg count
    if LPTR
       shr     BX,1
    endif
       mov     __argc,BX
       mov     __argv,offset DGROUP:argv

       ;Set up stack boundaries
       ;__stack = min(_stack,512)

       mov     BX,__stack
       or      BX,BX           ;if __stack was special value of 0
       jnz     L14
       mov     BX,02000h       ;then use this as the stack size
L14:   _if     BX ae 512, L8   ;make sure at least 512 bytes
       mov     BX,512
L8:    mov     __stack,BX

ifdef DOS16RM
if 1
       shr     BX,4            ;allocate stack segment to size in paragraphs
       bdos    48h             ;allocate memory for stack
       jc      outofmemory
       dec     BX              ;16 bytes less, give some pad at end of seg
       shl     BX,4            ;and back to offset
       mov     SS,AX
       mov     SP,BX
       jmp     short L9
else
       assume  es:nothing
       cli                             ;SP could point to unallocated memory
       mov     ax,ss
       mov     es,ax           ;  resize stack segment
       shr     BX,4            ;  shrink/expand segment to this size
       bdos    4Ah             ;Resize memory
       mov     ax, ss
       lsl     sp, ax
       dec     sp              ;was inc sp but this put sp outside selector limit
       sti
       push    ds
       pop     es
       assume  es:DGROUP
       jnc     L9
endif
else
    if SPTR
       add     BX,__BASE       ;add base of stack to stack size
       jc      outofmemory
    else
       mov     BX,__BASE
    endif
       add     BX,2 + 15       ;base word + round up to paragraph
       jc      outofmemory
       and     BL,0F0h
       mov     __pastdata,BX
       mov     __heapbottom,BX
       mov     CL,4
       shr     BX,CL
       mov     __datapar,BX    ;# of paragraphs in data segment
       add     BX,__progpar    ;total size of program in paragraphs
    if LPTR
       mov     __totalpar,BX
    endif
    ifdef __OS2__
      if LPTR
       jmp short L9            ;Don't shrink stack if LARGE model
      else
       push    __pastdata      ;new segment size requested
       push    ds              ;selector of segment to be resized
       call    DOSREALLOCSEG
       test    ax,ax
       jz      L9
      endif
    else
       mov     ES,__psp        ;segment of start of program
       cli                     ;SP could point to unallocated memory
       bdos    4Ah             ;resize memory
       push    DS
       pop     ES              ;restore ES
       jnc     L9
       sti
    endif
endif
outofmemory:
       mov     DX,offset DGROUP:nomem
       jmp     fatmsg

L9:
    if SPTR
       mov     SP,__heapbottom ;new top of stack
    ifndef __OS2__
       sti
    endif
       _if     __okbigbuf e 0, L12
       ;Attempt to grow our data segment to 64k
       mov     AX,0FFF0h
       sub     AX,__pastdata
       push    AX
       call    _sbrk
       pop     BX              ;dump argument off stack
       _if     AX e -1, L13    ;error
       inc     AX              ;skip over byte count
       inc     AX
       push    AX
       call    _free           ;add to memory pool
       pop     BX              ;clean stack
       jmp     short L12
L13:   mov     __okbigbuf,0    ;big buffers are not ok, we're short on memory
L12:
    else
    ifdef __OS2__
       mov     __okbigbuf,0    ;no big buffers for large data model
       mov     ax,__stack
       add     ax,16           ;add 16 bytes for overflow checks
       jc      outofmemory
       push    ax
       call    _sbrk           ;grow data segment by size of stack wanted
       pop     CX              ;fix stack
       _if     AX e -1, outofmemory

       mov     ES,DX
       mov     BX,AX           ;ES:BX -> first word of allocated block
       add     AX,ES:[BX]      ;DX:AX -> past last word of allocated block
       mov     SS,DX
       mov     SP,AX           ;set just past end of allocated block

       mov     __heapbottom,SP ;set top of stack
       mov     __BASE,BX       ;set address of check word
       mov     word ptr ES:[BX],55AAh  ;stack check word
    else
       mov     __okbigbuf,0    ;no big buffers for large data model
ifndef DOS16RM
       push    __stack
       call    _sbrk           ;grow data segment by size of stack wanted
       pop     CX              ;fix stack
       _if     AX e -1, outofmemory

       cli                     ;while we fiddle with the stack
       mov     ES,DX
       mov     BX,AX
       mov     SP,ES:[BX]      ;set just past end of allocated block
       add     SP,16           ;one paragraph more
       dec     DX              ;but compensate by decrementing segment
       mov     SS,DX           ;this is so stack overflow checks work better
       sti

       mov     __heapbottom,SP ;set top of stack
       mov     __BASE,16       ;set address of check word
       mov     word ptr SS:16,55AAh    ;stack check word
endif
    endif
    endif

    ifndef __OS2__
    ifndef SEGEXE
       call    getargv0        ;determine argv[0] if possible
    endif
    endif

       ;Clear uninitialized data segment (UDATA)
ifdef DOS16RM
       mov     ax, seg DGROUP
       mov     es, ax
       assume  es:DGROUP
       mov     di, offset DGROUP:_BSS  ;Compute the starting address of the
       sub     di, offset _BSS ; BSS segment.  (Get its end address and then
                               ; subtract its length.
       get_seg_size cx, ax     ; Get the overall size of the segment then
                               ; subtract the start address of BSS
else
       push    DS
       pop     ES
    ifdef I8086T
       mov     CX,offset DGROUP:bssend
    else
       mov     CX,offset DGROUP:_end
    endif
       mov     DI,offset DGROUP:_edata         ;start of uninitialized data
endif
       sub     CX,DI           ;CX = number of bytes to clear
       jcxz    L23             ;no uninitialized data
       xor     AL,AL
       rep     stosb           ;clear it out
L23:


       xor     BP,BP           ;so debuggers can find last stack frame
       call    __cinit         ;perform static constructors

       ;Call main(__argc,__argv,__environ)

    if LPTR
       push    __environ[2]
    endif
       push    __environ
    if LPTR
       push    __argv[2]
    endif
       push    __argv
       push    __argc
       call    _main           ;call C main(__argc,__argv,__environ)
       push    AX              ;save exit status for _exit
;      call    __chkstack      ;see if stack overflowed
       call    _exit           ;return to MS-DOS or OS/2

    ifndef __OS2__
    ifndef SEGEXE
;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Determine argv[0] if possible.

getargv0 proc  near
       _if     __osmajor b 3, GA1      ;not possible for old versions of DOS

       ;Determine start of argv0
       mov     ES,_aenvseg             ;get segment of environment string
       xor     DI,DI
       xor     AL,AL                   ;looking for terminating 0
       mov     CX,0FFFFh
       cld
GA2:   repne   scasb
       scasb                           ;2 bytes of 0?
       jnz     GA2                     ;no, more environment
       inc     DI                      ;ES:DI -> argv0
       inc     DI
    if LPTR
       mov     argv,DI
       mov     argv+2,ES
    else ;SPTR
       ;Need to copy to a location accessible by DS. Use the stack.
       ;Determine length needed in CX.
       mov     SI,DI                   ;save offset
       mov     CX,0FFFFh
       repne   scasb                   ;look for terminating 0
       neg     CX
       and     CX,0FFFEh               ;CX is count rounded up to next word
       pop     BX                      ;BX = return address from this function
       sub     SP,CX                   ;allocate room for argv0
       mov     DI,SP
       push    ES
       pop     DS                      ;SI:DS -> argv0
       push    SS
       pop     ES                      ;ES:DI -> stack buffer
       rep     movsb                   ;transfer to stack
       push    SS
       pop     DS                      ;restore DS
       mov     argv,SP                 ;set pointer to it
       ;Note at this point, ES==DS
       jmp     BX
    endif
GA1:   ret
getargv0 endp
    endif
    endif

;;;;;;;;;;;;;;
; Set __stack.
; Input:
;      DS:SI -> start of parameter
; Returns:
;      AL = char past end of number
;      SI -> past AL
;      Do not destroy BX

set_stack proc near
       mov     DI,10
       xor     CX,CX           ;accumulate result in CX
S1:    lodsb                   ;get next char of paramter
       or      AL,AL           ;end of command line?
       jz      S2              ;yes
       _if     AL e ' ', S2
       _if     AL e 9, S2      ;if end of parameter
       sub     AL,'0'
       js      err
       cbw                     ;AH = 0
       _if     AX ae DI, err   ;AL is not a digit
       xchg    AX,CX
       mul     DI
       jc      err             ;integer overflow
       add     CX,AX           ;CX = CX*10 + AX
       jnc     S1              ;no error

err:   mov     DX,offset DGROUP:badstk
       jmp     short fatmsg

S2:    mov     __stack,CX      ;store result in __stack
retaddr:
       ret
set_stack endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Print out fatal error message and abort.
; Input:
;      DS:DX -> message

fatmsg: push   DX
       jmp     ___faterr

    if LCODE
C_TEXT ends
    else
_TEXT  ends
    endif

ifdef SEGEXE
endgo  macro   l
       end             l
endm
else
ifdef DOS16RM
endgo  macro   l
       end
endm
else
endgo  macro   l
       end             l
endm
endif
endif
       endgo   start
