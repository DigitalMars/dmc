; Copyright (C) 1985-1994 by Digital Mars
; All rights reserved.
; Written by Joe Huffman
; Modified by Joe Huffman September 17, 1990
; C startup code for UNIX System V and Phar Lap
; DOS extender

include macros.asm
include stdio.asm

ifndef M_UNIX
ifdef M_XENIX
M_UNIX equ     1       ;No differences in this module between XENIX and UNIX.
endif
endif

ifdef DOS386
include x386mac.asm
       extrn __dos:word, __8087:dword, __stack:dword
       extrn __DOS386_version:dword,__DOS386_environ:dword
       extrn   _calloc:near, _realloc:near, _malloc:near
endif

ifdef X386
includelib zlx.lod             ;stub loader
include x386mac.asm
       extrn   _calloc:near, _realloc:near, _malloc:near, __stack:dword, __psp:dword
       extrn   __osmajor:byte, __osminor:byte,__x386_coproc_init:near
       extrn   __x386_start:near
endif

ifdef M_UNIX
       begdata
if COFF OR ELF
ifdef COHERENT
       extrn ___end:dword
else
       extrn _end:dword
endif
endif
       extrn _environ:dword, __8087:dword
       enddata

       extrn _signal:near, _sysi86:near
endif
       extrn   _exit:near, __exit:near
       extrn __argc:dword,__argv:dword,__envptr:dword,__environ:dword
    ifdef _MBCS
       extrn __mbctype:byte
       extrn ___initmbctype:near
    endif
       extrn __cinit:near, _main:near

       public  __acrtused, __heapbottom, __pastdata
       public  __x386_break, __x386_zero_base_selector, __x386_zero_base_ptr
       public  __chkstack, __chkstk
       public  __exe_type

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Form the start of DGROUP.
; Modified by Joe Huffman June 19, 1990
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Initialized data globals
CR     =       13
LF     =       10

ifdef X386  ;Define order of segments
begdata_16
;16 bit data segment for real mode and priv level 0 activities
        dw      _TEXT           ;first word in extender code, used by loader
        extrn __x386_esp3:dword,__x386_esp0:dword
        extrn __x386_stack_32_des:word
enddata_16

begcode_16
__start:
        jmp     __x386_start    ;jump to 16 bit initialization
endcode_16

_TEXT  segment para public 'CODE' ;must be paragraph aligned here
endcode

       begdata

extrn __x386_mk_protected_ptr:dword

        db      'Digital Mars C 6.0 library, Copyright (C) 1988-1994 ',CR,LF
        db      'X386',CR,LF
        db      'written by Walter Bright, Joe Huffman and Doug Huffman',0

public  __data_bottom,__x386_version
                ;Used for NULL pointer detection.  No one has any business
                ;accessing below this point.
__data_bottom   label byte
null_code_ptr           db     CR,LF,'NULL code pointer called',CR,LF,'$'
__x386_disp_base        dw      0               ;storage for _dispbase
__stack_bottom  label byte                      ;for pharlap compatibility
__x386_version          dw      300h            ;extender vesion #

DGROUP  group   _TEXT,HUGE_BSS

        extrn __x386_data_16_alias:word,__stack:dword
        public __x386_set_ss_lim
        __x386_set_ss_lim       dd      offset no_dpmi_set_ss_lim

else    ;X386

ifdef DOS386
       begcode_16
       endcode_16
endif ;DOS386

       begdata
endif   ;X386

       extrn   _errno:dword

ifdef DOS386
       db      'Digital Mars C 6.0 DOS386 library, Copyright (C) 1988-1994 ',CR,LF
else
       db      'Digital Mars C 6.0 UNIX library, Copyright (C) 1988-1994 ',LF
endif
       db      'written by Walter Bright and Joe Huffman',0

       align   4

__pastdata     dd      ?       ;address of 1 past data segment
__heapbottom   dd      ?       ;lowest address in heap (used to detect
                               ;free() errors and stack wrap)

__acrtused     equ     1234    ;cause linker to pull in this module.

__x386_break   dd      offset DGROUP:__stack_bottom
;used in alloca()

;Set exe file type
ifdef X386
__exe_type     dw      EXE_DOSX
else
ifdef DOS386
__exe_type     dw      EXE_PHAR386
else
__exe_type     dw      EXE_SCOUNIX
endif
endif

__x386_zero_base_selector       dw      34h     ;writable segment, base = 0
__x386_zero_base_ptr           dd      0
ifdef DOS386
public __x386_version
__x386_version                  dw      0       ;zero for pharlap
endif

ifndef M_UNIX
ovrflw db      CR,LF,'Stack Overflow',CR,LF,'$'
nomem  db      CR,LF,'Not enough memory',CR,LF,'$'
badcmd db      CR,LF,'Max of 128 args allowed',CR,LF,'$'
badstk db      CR,LF,'Bad stack size parameter',CR,LF,'$'
ARGMAX =       128     ;max # of command line args
endif

       enddata

ifdef DOS386

STACK_MIN_SIZE equ     1024
STACK  segment

       public  __stack_bottom
__stack_bottom dd      66BB55AAh       ;Check value.
               db      (STACK_MIN_SIZE-4) dup(?) ;Start up with minimum size.
STACK  ends

endif

_TEXT  segment para public 'CODE'
       assume DS:DGROUP, ES:DGROUP, CS:_TEXT

ifdef X386
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;This is the very beginning of the 32 bit code segment.
;It must be paragraph aligned so that the offset of the first instruction
;will be zero.
;
;Exit with an error message if this is ever exectuted.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
       mov     edx,offset DGROUP:null_code_ptr
       jmp     fatmsg
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;This is the location where 16 bit code transfers control to when 
;first entering 32 bit code.  SS and ESP are set up when arriving here.  DS 
;will = ES = __x386_data_32, GS = __x386_seg_env, FS = __x386_seg_psp.  
;__pastdata and __heapbottom are intialized prior to this code obtaining 
;control.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

public  __x386_init
__x386_init:
       mov     EDI,offset DGROUP:_BSS  ;Initialize BSS to zero. 
       mov     ECX,offset DGROUP:EEND 
       sub     ECX,EDI 
       clr     EAX 
       shr     ECX,2 
       rep     stosd 

       mov     __psp,FS
       call    get_stack       ;FS has selector for PSP.
       mov     EAX,ESP         ;Check for enough room for stack.
       sub     EAX,__stack

       mov     EDX,offset DGROUP:badstk
       jc      fatmsg          ;Is the stack value too big?
       cmp     EAX,__pastdata
       jl      fatmsg

       call    args_setup
       push    GS
       pop     ES
       call    env_malloc      ;ES:0 points to the start of the environment

       clr     EAX
       mov     FS,AX           ;Makes switches to/from real mode faster.
       mov     GS,AX

       push    ds
       pop     es

       mov     AX,3000h
       bdos
       mov     __osmajor,AL
       mov     __osminor,AH

       call    __x386_coproc_init
else ;X386

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Program entry point.  Under UNIX it must be named __start for the
; linker to make this the entry point.  The end 'label'
; directive is ignored.  If __start is not found then _main is
; used.
; Modified by Joe Huffman January 13, 1991
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

       public  __start
__start:
endif ;X386

       clr     EBP             ;so debuggers can find last stack frame
       push    EBP
       mov     EBP,ESP

ifdef DOS386
       call    dos386_setup
       mov     EBP,ESP         ;stack was changed in dos386_set_ds.
endif


ifdef M_UNIX
if OMF
       mov     ECX,offset DGROUP:EEND
else
ifdef COHERENT
       lea     ECX,___end      ;This symbol is defined by the linker.
else
       lea     ECX,_end        ;This symbol is defined by the linker.
endif
                               ;Next available address after .bss
endif
       mov     __heapbottom,ECX
       mov     __pastdata,ECX

;;;;;;;;;;;;;;;;;;;;;;;;;
;Set up __argc and argv
;;;;;;;;;;;;;;;;;;;;;;;;;
       mov     EAX,4[ebp]      ;argc
       mov     __argc,EAX
       lea     ECX,[EBP + SIZEPTR * EAX + 12]        ;Start of environment.
       mov     _environ,ECX
       push    ECX             ;environ in preparation for calling main().
       mov     ECX,[ECX]
       mov     __envptr,ECX    ;_envptr for comp with DOS.
       lea     ECX,[EBP + 8]   ;argv
       mov     __argv,ECX
;;;;;;;;;;;;;;;;;;;;;;;;;;
;Find out what type of coprocessor we have (if any).
;;;;;;;;;;;;;;;;;;;;;;;;;;
       push    offset _8087_helper
       push    12              ;SIGSYS
       call    _signal

       mov     EBX,EAX         ;Save previous handler.
       push    offset __8087
       push    40
       call    _sysi86

       push    EBX             ;Restore handler.
       push    12
       call    _signal

       mov     ESP,EBP         ;Clean up stack from past 3 function calls.
       and     __8087,3
if 0
;Not required
       jz      short no_fp
;Initialise fp
       finit                   ;initialize 80x87
       fstcw   [ESP]
       fwait
       or      word ptr [ESP],01000h   ;turn on affine infinity bit
       fldcw   [ESP]
no_fp:
endif
endif ;M_UNIX
       call    __cinit         ;perform static constructors
ifdef DOS386
       push    __environ
endif
       push    __argv
       push    __argc
       call    _main           ;call C main(__argc,__argv, _environ)
       push    EAX             ;save exit status for exit
       call    _exit           ;return to OS
       hlt                     ;Should never reach here.

ifdef M_UNIX
_8087_helper    proc    near
       mov     __8087,0
        ret
_8087_helper   endp
endif

ifdef DOS386

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Determine if we have and which version of 8087 we have installed.
dos386_setup   proc    near
       push    EBP
       mov     EBP,ESP

       sub     ESP,512
       mov     EBX,ESP         ;Pointers to buffer areas.
       sub     ESP,256
       mov     ECX,ESP

       mov     EAX,02526h      ;Get configuration information.
       bdos

       mov     EAX,20h[ebx]    ;Get coprocesser flag.

       sub     EAX,4           ;4 == None, 6 = 287, 7 = 387
       mov     __8087,EAX
       jz      short no_87
       finit                   ;initialize 80x87
       fstcw   [ESP]
       fwait
       or      word ptr [ESP],01000h   ;turn on affine infinity bit
       fldcw   [ESP]
no_87:
       mov     ESP,EBP
       mov     EBX,'PHAR'      ;To get Phar Lap version too.
       bdos    30h
       mov     __dos,AX
       mov     __DOS386_version,EBX
       mov     __DOS386_environ,ECX

       call    dos386_set_bss
       mov     AX,SEG_PSP
       mov     FS,AX
ifdef _MBCS
       call    ___initmbctype  ;Set up MBCS environment
endif
       call    get_stack       ;Requires FS == PSP selector.
       call    dos386_set_ds
       call    args_setup      ;Requires FG == PSP selector.

if ESeqDS
       push    ES
endif
       mov     EAX,SEG_ENV
       mov     ES,AX
       call    env_malloc
if ESeqDS
       pop      ES
endif
       pop     EBP
       ret
dos386_setup   endp

;;;;;;;;;;;;;;;;;;;;;;;;;
; Set BSS to 0.  Destroys EDI.
; September 12, 1990
;;;;;;;;;;;;;;;;;;;;;;;;;
dos386_set_bss proc    near
       mov     EDI,offset DGROUP:_BSS
       mov     ECX,offset DGROUP:EEND
       sub     ECX,EDI         ;Number of bytes to clear.
       clr     EAX
       shr     ECX,2           ;Number of dwords
       rep     stosd           ;Always on dword boundaries.
       ret
dos386_set_bss endp

;;;;;;;;;;;;;;;;;;;;;;;;;
; Shrink the data segment to the minimum required.  Set Stack to proper size.
; Set up the pointers to the heapbottom and pastdata.
; September 11, 1990
;;;;;;;;;;;;;;;;;;;;;;;;;
dos386_set_ds  proc    near
       push    EBP
       mov     EBP,ESP
       mov     eax,STACK_MIN_SIZE
       cmp     eax,__stack
       jge     short set_seg1  ;eax = max(STACK_MIN_SIZE,__stack)
       mov     eax,__stack

set_seg1:
       mov     __stack,eax     ;Actual stack size.
       add     eax,offset DGROUP:__stack_bottom + SIZEPTR
       and     eax,NOT 3
       mov     __heapbottom,eax ;Next dword past top of stack.
       push    eax
       call    _brk            ;Sets __pastdata
       add     esp,SIZEPTR
       inc     eax             ;Test eax for -1.
       jz      short alloc_fail
       mov     eax,__heapbottom
       sub     eax,SIZEPTR     ;New top of stack.

set_seg2:
       mov     EBX,[EBP]       ;Trace stack frame back to find top of stack.
       tst     EBX             
       jz      set_seg3
       mov     EBP,[EBX]
       jmps    set_seg2

set_seg3:
       push    DS
       pop     ES

       mov     ECX,EBP         ;Prepare to copy old stack to new stack.
       sub     ECX,ESP
       mov     ESI,ESP
       mov     ESP,EAX         ;Set up new stack top.
       sub     ESP,ECX
       mov     EDI,ESP         ;Point at destination for new stack.
       rep     movsb           ;Move the stack data.

       pop     EBP
       ret
dos386_set_ds endp

endif  ;DOS386

ifndef M_UNIX
alloc_fail:
       mov     EDX,offset DGROUP:nomem
;      jmp     short fatmsg

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Print out fatal error message and abort.
; Input:
;      DS:EDX -> message
; Modified by Joe Huffman September 10, 1990

fatmsg:
       bdos    9               ;Output the message.
       push    1               ;error exit code
       call    __exit          ;abort
       hlt                     ;never reached
endif ;M_UNIX



ifndef M_UNIX
;;;;;;;;;;;;;;;;;;
;Stuff common to both the PharLap and X386 code.
;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;
; Get =XXXX from command line and set __stack.  Rounds it up to the next
; 4096 boundary.
; Input: FS == selector for PSP.
;      
; Returns:
;      __stack = value from command line rounded up.
; Destroys: All general registers.  Saves segment registers.

get_stack      proc    near
       mov     EBX,__stack     ;Ignore if __stack == 0
       tst     EBX
       jz      GS10

       push    DS
       mov     AX,FS
       mov     DS,AX
       mov     ESI,80h
       lodsb
       mov     EDI,ESI
       movzx   ECX,AL          ;Length of the command line.

GS1:   jecxz   GS9
       lodsb
       dec     ECX

GS2:   mov     DX,256 * 9 + ' '        ;Search for tabs, spaces,
       _ifs    AL e "'",GS3            ;If found a ' skip past it.
       _ifs    AL e '"',GS3            ;If found a " skip it.
       _ifs    AL e DL,GS1             ;If found a ' ' eat whitespace
       _ifs    AL e DH,GS1             ;If found a '\t' eat white
       _ifs    AL e '=',GS6            ;found =XXXX stack size parameter
       jmps    GS4

GS3:   mov     DL,AL           ;Search for this character -- ' or "
       mov     DH,AL

GS4:   jecxz   GS9
 ifdef _MBCS
       push    EBX
       xor     EBX,EBX
       mov     BL,AL
       test    __mbctype[EBX+1],04h  ;check MBCS 1st byte (_M1)
       pop     EBX
       jz      GS4a
       lodsb                         ;get 2nd char of parameter
       dec     ECX
       jecxz   GS9
GS4a:
 endif
       lodsb
       dec     ECX

       _ifs    AL e DL,GS5     ;Found it, search for another
       _ifs    AL ne DL,GS4

GS5:   _ifs    DL e DH,GS1     ;Load next char and continue.
       jmps    GS2             ;Already loaded next char.


GS6:   clr     EBX             ;accumulate result in EBX

GS7:   jecxz   GS9                     ;if end of command line
       lodsb                           ;get next char of paramter
       dec     ECX

       _ifs    AL e ' ', GS9
       _ifs    AL e 9, GS9             ;if end of parameter
       sub     AL,'0'
       js      short GSerr
       movzx   EAX,AL
       _ifs    AL ae 10, GSerr         ;AL is not a digit
       imul    EBX,10
       jc      short GSerr             ;integer overflow
       add     EBX,EAX                 ;EBX = EBX * 10 + EAX
       jnc     short GS7               ;no error

GSerr: pop     DS
       mov     EDX,offset DGROUP:badstk
       jmp     fatmsg

GS9:   pop     DS
       neg     EBX                     ;Round up to nearest 4096 boundary.
       and     BX,0f000h
       neg     EBX
       mov     __stack,EBX             ;store result in __stack
GS10:  ret
get_stack      endp

;;;;;;;;;;;;;;;;;;
;This expects ES:0 to point to the start of the environment.
;It malloc's space for the envirnment and copies the environment there.
;All general registers are trashed.  Segment registers are preserved.
;argv is expected to be setup with room for argv[0] so that the program
;name is put there.
;;;;;;;;;;;;;;;;;;
env_malloc     proc    near

;Need to find the end of the environment.  This is two consecutive 0 bytes.
       clr     EAX             ;Search for a 0.
       mov     EDI,EAX         ;Environment starts at offset 0.
       mov     ECX,EAX
       dec     ECX             ;Allow it to be very long.

env_len_loop:
         repne   scasb           ;Find the first 0.
         scasb                   ;Is the next a 0 also?
         jz      short env_len_done      ;Jmp if found two 0's in a row.
         loop    env_len_loop

env_len_done:
       push    ES              ;Save a copy of ES.
if ESeqDS
       mov     AX,DS
       mov     ES,AX
endif

       push    EDI             ;EDI has the length of the entire enviroment.
       call    _malloc
       pop     EBX             ;Clean up the stack.

       tst     EAX
       jz      alloc_fail

       mov     __envptr,EAX
       mov     ECX,EDI         ;Still has length of the environment.
       mov     EDI,EAX         ;Destination of environment.
       clr     ESI             ;Source offset of environment.

       push    DS
       pop     ES              ;Get DGROUP in ES.
       pop     DS              ;Get environment selector in DS.

       rep     movsb           ;Environment is copied.

       add     ESI,2           ;Get to program name.
       mov     EDI,ESI         ;Place to start scan.

       push    ES              ;DGROUP.
       push    DS              ;Environment selector.
       pop     ES              ;Set ES to enviroment sel.
       pop     DS              ;Set DS to DGROUP.

       clr     AL
       mov     CL,0FFh         ;Something longer than the real length.
       repne   scasb           ;find the end.
       sub     EDI,ESI         ;Length of program name.
       inc     EDI             ;Leave room for 0.

       push    ES              ;Save environment selector.
if ESeqDS
       mov     AX,DS
       mov     ES,AX
endif
       push    EDI             ;Length of program name plus '\0'.
       call    _malloc         ;Get some memory for the program name.
       pop     EBX             ;Clean up the stack.
       tst     EAX
       jz      alloc_fail

       mov     EBX,__argv
       mov     [EBX],EAX       ;Set up pointer argv[0].
       mov     ECX,EDI         ;Length of prog name plus a 0.
       mov     EDI,EAX         ;ptr to allocated memory for the program name.

       push    DS              ;DGROUP
       pop     ES
       pop     DS              ;Environment selector.
       rep     movsb           ;Copy program name.

       push    DS              ;Environment selector.
       push    ES              ;DGROUP
       pop     DS              ;DGROUP
       pop     ES              ;Envirnment selector.
       ret
env_malloc     endp

;;;;;;;;;;;;;;;;;;;;;;;;;
; Set up argc and argv for DOS386.
; Because the data is in another segment the data space is malloced and copied
; to the new area.  On entry FS must be the selector for the PSP.
; Memory is allocated for argv[].
; Uses/destroys EAX, EBX, ECX, EDX, ESI, EDI, and ES.
; January 15, 1991
;;;;;;;;;;;;;;;;;;;;;;;;;
args_setup     proc    near
       push    1
       push    SIZEPTR * (ARGMAX + 2)  ;+1 for argv[0], +1 for argv[argc]

if ESeqDS
       push    DS
       pop     ES
endif
       call    _calloc
       add     ESP,8
       mov     __argv,EAX
       jz      alloc_fail
       tst     EAX

       movzx   EBX,byte ptr FS:[80h]     ;Length of the command line.
       tst     EBX
       jz      arg_L7          ;no command line
       inc     EBX             ;Room for terminating 0.
       push    EBX             ;Size allocated.
       dec     EBX             ;Actual length.
       call    _malloc
       add     ESP,4
       tst     EAX
       jz      alloc_fail

       mov     EDI,EAX         ;Dest of copy of command line.
       mov     EDX,EAX         ;Save this pointer.  Note that it is
ife ESeqDS                     ;eventually 'lost'.  It will never be freed.
       push    DS              ;If it is desired to free this later (like
       pop     ES              ;after the program is run) then this is the
endif                          ;place to save a copy away somewhere.
       mov     ECX,EBX         ;Length of the command line
       mov     ESI,081h

       push    DS
       push    FS
       pop     DS              ;source is DS:ESI
       rep     movsb           ;Entire command line is now in our data seg.
       clr     AL
       stosb                   ;0 terminate the command line
       pop     DS

       ;;;;;;;;;;;;;
       ; EDX has pointer to our copy of command line (in DGROUP).
       ; EBX has length (not including terminating 0).
       ; ES == DS == DGROUP

       mov     ESI,EDX
       mov     EDI,EDX
       mov     EBX,SIZEPTR     ;&argv[1] - argv

arg_loop_top:
       mov     DX,9*256 + ' '          ;DH = tab, DL = space
       lodsb                           ;get char from command line
       tst     AL                      ;done?
       jz      set_argv_done           ;yes

       _ifs    AL ne '=', arg_ignore_eq
       _ifs    __stack e 0,arg_ignore_eq  ;if ignore '=nnnn' command

arg_eat__stack:                ;__stack already been taken care of, ignore it now.
       lodsb
       tst     AL
       jz      set_argv_done
       _ifs    AL e DL, arg_loop_top
       _ifs    AL e DH, arg_loop_top   ;eat spaces and tabs
       jmp     arg_eat__stack

arg_ignore_eq:
       _ifs    AL e DL, arg_loop_top
       _ifs    AL e DH, arg_loop_top   ;eat spaces and tabs
       _ifs    AL e '"', arg_L1
       _ifs    AL ne "'", arg_L2

arg_L1: mov    DL,AL
       mov     DH,AL
       inc     ESI             ;past ' or "

arg_L2: _ifs   EBX be ARGMAX*SIZEPTR,arg_L3
       mov     EDX,offset DGROUP:badcmd
       jmp     fatmsg

arg_L3: dec    ESI
       mov     EAX,__argv      ;set the pointer to this arg.
       mov     [EAX + EBX],ESI ;EBX has offset of pointer to this arg.
       mov     EDI,ESI
       add     EBX,SIZEPTR
       xor     AL,AL           ;last char read
arg_L4:        mov     AH,AL           ;remember last char read
       lodsb                   ;get next char of parameter
       stosb                   ;and store it
       tst     AL              ;done
       jz      short set_argv_done

 ifdef _MBCS
       push    EBX
       xor     EBX,EBX
       mov     BL,AL
       test    __mbctype[EBX+1],04h  ;check MBCS 1st byte (_M1)
       pop     EBX
       jz      arg_L4a
       lodsb                         ;get 2nd char of parameter
       stosb                         ;and store it
       or      AL,AL                 ;done?
       jz      set_argv_done         ;yes
       xor     AL,AL                 ;disable last char
arg_L4a:
 endif

       ;Remove any \ that are immediately followed by "
       _ifs    AX ne <'\'*256 + '"'>, L3a
       dec     EDI             ;remove '\'
       dec     EDI
       stosb
       jmp     arg_L4

L3a:   _ifs    AL e  DL,arg_L5 ;end of parameter
       _ifs    AL ne DH,arg_L4 ;not end of parameter
arg_L5: clr    AL
       dec     EDI
       stosb                   ;terminate parameter with a 0
       jmp     arg_loop_top    ;look for next parameter

arg_L7:        mov     BL,SIZEPTR      ;Length required for _argv.
                               ;argv[0] will be set by dos386_set_envp()
set_argv_done:
       add     EBX,SIZEPTR     ;allow room for terminating NULL
       push    EBX             ;Size of array used for pointers to args.
                               ;Parameter to realloc().
       shr     EBX,2           ;get arg count
       dec     EBX             ;remove terminating NULL
       mov     __argc,EBX
       push    __argv
       call    _realloc        ;shrink argv[] down to actual size
       add     ESP,8
       tst     EAX
       jz      alloc_fail
       mov     __argv,EAX
       ret

args_setup     endp

endif  ;M_UNIX

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Check and see if stack overflowed.
; This can be called at any time by the application code. It was originally
; useful to place a call to this at selected points when stack overflow
; checking is turned off.  Obsolete for UNIX because of hardware protection
; and virtual memory.
; Returns:
;      # of bytes left in stack
; Modified by Joe Huffman September 13, 1990
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__chkstack     proc    near
ifdef DOS386
       mov     EAX,ESP
       _ifs    __stack_bottom ne 66BB55AAh, stack_overflow
       sub     EAX,offset DGROUP: __stack_bottom ;Remaining stack left.
       jbe     stack_overflow
endif
ifdef M_UNIX
       mov     EAX,ESP
       sub     EAX,__pastdata  ;Remaining stack left.
endif
ifdef M_X386
       mov     EAX,__pastdata  ;Round up to the next 4k boundary.
       neg     EAX             ;This is the current segment limit of
       and     AX,0f000h       ;the stack.  This can change if malloc()
       add     EAX,ESP         ;(or siblings) is called -- actually brk() 
                               ;is the critical function...
endif
       ret
__chkstack     endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Stack frame generator.
; Called at entry to each function when stack overflow checking
; is turned on.
__chkstk       proc    near
       pop     EDX             ;get offset of return addr
       sub     ESP,EAX         ;create space for local variables
ifdef DOS386
       jbe     short stack_overflow
       _ifs    __stack_bottom ne 66BB55AAh, stack_overflow
       _ifs    ESP be <offset DGROUP:__stack_bottom>,stack_overflow
endif
       jmp     EDX             ;return to caller

__chkstk       endp

ifndef M_UNIX

ifdef DOS386
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Stack overflow jumps here.

stack_overflow:
       mov     edx,offset DGROUP:ovrflw
       jmp     fatmsg
endif
       endcode

;;;;;;;;;;;;;;;;;;;;;;;;;
; Items below are things which are different between Phar Lap DOS386 and the
; X386 memory model so they are put in c.asm to keep the Phar Lap and X386 
; libraries identical.
;
; Copyright Walter Bright
; Modified by Joe Huffman September 17, 1990
;;;;;;;;;;;;;;;;;;;;;;;;; 


; ENOMEM equ     12

_TEXT  segment para public 'CODE'
       assume CS:_TEXT
       c_public sbrk, brk

;;;;;;;;;;;;;;;;;;;;;;;;;;
; Request memory from operating system.
; Attempt to grow the data segment.
; Use:
;      p = sbrk(nbytes);
; Returns:
;      pointer to memory allocated
;      -1 if error
; Modified by Joe Huffman June 20, 1990
;
; NOTE!!! malloc() depends on P[EBP] being the true amount allocated.
; Don't use it for scratch!
func   sbrk
       push    EBP
       mov     EBP,ESP
       uses    <ECX>

       mov     EAX,__pastdata          ;Current setting.
       mov     ECX,P[EBP]              ;Number of bytes to change DS by.
       jecxz   short sbrk_done         ;If zero return current setting.

       add     ECX,EAX                 ;ECX has new pointer to end of DS,
       jc      short sbrk_err1         ;jump if wrap around
       push    EAX                     ;Current end of DS for return value.
       push    ECX                     ;Push new pointer to end of DS.
       callm   brk                     ;Set the new end of segment.
       add     ESP,4

       pop     ECX                     ;return value if was successful.
       or      EAX,EAX                 ;Returns 0 for success.
       jnz     short sbrk_err2

sbrk_done:
       add     EAX,ECX

       unuse   <ECX>
       pop     EBP
       ret

sbrk_err1:
       mov     _errno,ENOMEM

sbrk_err2:
       mov     EAX,-1
       unuse   <ECX>
       pop     EBP
       ret
c_endp sbrk

;;;;;;;;;;;;;;;;;;;;;;;;;
; Set the end point of DS to a new value.
; Use:
;      int brk(char *new_end)
; Returns:
;      0 if succesfull
;      -1 if error.
; June 20, 1990
;;;;;;;;;;;;;;;;;;;;;;;;;

ifdef X386

_brk    proc    near
        mov     eax,4[esp]     ;desired new value for break
        cmp     eax,__x386_break        ;check current location of break
        ja      brk1
;no need to adjust stack limit, update __pastdata, change nothing else.
        mov     __pastdata,eax
brk2:   xor     eax,eax
        ret
brk1:   push    es
        mov     es,__x386_data_16_alias
assume es:__X386_GROUP_16
        mov     ecx,__x386_esp3         ;top of stack
        sub     ecx,__stack             ;result may not be 4 k aligned
        and     cx,0f000h               ;round down to 4 k boundary
        cmp     eax,ecx
;preserve min stack size, also catches wrap around error in round up below
        ja      brk_error
;adjust stack limit and __x386_break
        mov     edx,eax         ;value for __pastdata if cmp eax,esp is okay
        neg     eax
        and     ax,0f000h       ;round up to nearest 4 kbyte boundary
        neg     eax
        mov     al,8            ;allow room for two pushes
        cmp     eax,esp
        ja      brk_error
        xor     al,al           ;take out fudge factor placed in al
        mov     __pastdata,edx  ;brk will not fail so update __pastdata
        mov     ecx,eax
        shr     eax,12          ;adjust for page granular limit
        dec     eax     ;make next address down illegal
comment&
        The variable __x386_set_ss_lim is initialized at run time to point to 
        __x386_dpmi_set_ss_lim if a dpmi host is detected, otherwise, it is 
        initialized to no_dpmi_set_ss_lim.
&
        jmp     __x386_set_ss_lim
no_dpmi_set_ss_lim:
        mov     es:__X386_GROUP_16:__x386_stack_32_des.limit_0,ax 
;low word of limit is now stored in gdt
        shr     eax,8
        or      ah,0c0h ;mov c into high nibble of ah, set granularity and dbit
        mov     es:__X386_GROUP_16:__x386_stack_32_des.type_limit,ah
limit_updated:
;Stack limit is now updated
        push    ss
        pop     ss      ;load new limit from gdt
        mov     __x386_break,ecx        ;update break
        pop     es
        jmp     brk2
brk_error:
        mov     _errno,ENOMEM
        xor     eax,eax
        dec     eax             ;return - 1
        pop     es
        ret
public __x386_dpmi_set_ss_lim
__x386_dpmi_set_ss_lim:
comment&
        Execution branches to this point if dpmi was detected during 
        initialization.  At this point, the original es has been pushed on the 
        stack, eax contains the new segment limit shifted right 12 bits.  ecx 
        contains __x386_break and must be preserved for later use.
        &
        push    ecx
        push    ebx
        mov     bx,ss   ;selector for dpmi
        mov     edx,eax
        shl     edx,12          ;restore to 32 bit address
        or      dx,0fffh        ;set low 12 bits for dpmi
        shld    ecx,edx,16      ;cx:dx now has new limit
;Microsoft windows has serious bugs which can only be avoided by loading a 
;special stack with esp > 2 GBytes or esp < 64 KBytes.  I chose the second 
;option.
        push    esi
        push    edi
        push    ebp
        push    ds
        mov     ax,__x386_data_16_alias
        mov     ds,ax
assume ds:__X386_GROUP_16
;must allocate space on the 16 bit stack in case of a hardware interrupt
        mov     ebp,__x386_esp0
        sub     __x386_esp0,180h
        mov     esi,esp
        mov     edi,ss
        mov     ss,ax
        mov     esp,ebp
        mov     ax,8h
        int     31h             ;call dpmi, set ss limit
        mov     ss,edi
        mov     esp,esi         ;restore 32 bit stack
        add     __x386_esp0,180h
assume ds:DGROUP
        pop     ds
        pop     ebp
        pop     edi
        pop     esi
        pop     ebx
        pop     ecx
        jmp     limit_updated
_brk    endp

else ;X386

func   brk
       push    EBX

       mov     EBX,P[ESP]
       add     EBX,4096 - 1    ;Round up to nearest page size.
       shr     EBX,12          ;Get number of pages.

ife ESeqDS
       mov     AX,DS
       mov     ES,AX
endif

       bdos    4AH             ;resize the segment.
       unuse   <EBX>

       jc      short cerror

       mov     EAX,PS[ESP]     ;Get the new break value.
       mov     __pastdata,EAX  ;Save it for future use.
       clr     EAX
       ret

cerror:
       mov     _errno,EAX      ;errno = value returned by kernel
       sbb     EAX,EAX         ;return value = -1

       ret

c_endp brk

endif ;X386


comment&
        __get_video_selector returns a protected mode selector in ax which 
        points to the video buffer.  The real mode segment value is passed in 
        ax, it is ignored under Pharlap which returns 1ch regardless of the 
        value passed in ax.  In case of X386, a protected mode selector is 
        allocated with base address of X386, if no selectors are available it 
        returns 0 in ax which causes disp to use bios.  If zero was passed in 
        AX, it returns 0 which tells disp to use bios.
&

public __get_video_selector
__get_video_selector       proc    near
ifdef DOS386
        or      AX,AX           ;check for zero
        jz short vid_sel_ret    ;if zero, return zero
        mov     AX,1ch          ;hardwired video buffer selector
vid_sel_ret:
        ret
else  ;DOS386
        or      AX,AX           ;check for zero
        jz short vid_sel_ret    ;if zero, return zero
        cmp     __x386_disp_base,0
        jnz     R1                      ;if not zero, it is already allocated
get_new_selector:
        movzx   EAX,AX
        shl     EAX,4                   ;make absolute address
        push    EAX
        call    __x386_mk_protected_ptr ;returns pointer in DX:EAX
        or      EAX,EAX                 ;zero if successful
        mov     EAX,EDX                 ;selector
        pop     EDX                     ;restore stack, EDX is garbage
        jnz     R1                      ;return zero if failure
        mov     __x386_disp_base,AX     ;store for future calls
vid_sel_ret:
        ret
R1:
        mov     AX,__x386_disp_base
        ret
endif
__get_video_selector    endp

endif   ;M_UNIX

       endcode

       end     __start


