;_ cinit.asm   Mon Dec 11 1989   Modified by: Walter Bright */
; Windows support added by G. Eric Engstrom Jan 1991   -D_WINDOWS
; Copyright (C) 1985-1991 by Walter Bright
; All rights reserved.
; Written by Walter Bright
; C initialization

include        macros.asm
include flthead.asm

       public  __psp, __pgmptr
       public  __tab_size
       public  _aenvseg
    ifndef DOS16RM
       public  __datapar
       public  __heapbottom
       public  __pastdata
    endif
       public  __doserrno,_errno
;      public  __oserr
       public  __dos,__dodtors
       public  __osver,__osversion,__osmajor,__osminor,__osmode, __cpumode
    ifdef _WINDOWS
       public  __winver,__winmajor,__winminor
    endif
       public  ___argc,___argv
       public  __argc,__argv
       public  __atexit_max
       public  __atexit_tbl
       public  __atexitp
       c_public  _Exit,_exit,_cinit
       public  _STKHQQ

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

;Function pointers for Microsoft constructors
XIFCB  segment word public 'DATA'
XIFCB  ends
XIFU   segment word public 'DATA'
XIFU   ends
XIFL   segment word public 'DATA'
XIFL   ends
XIFM   segment word public 'DATA'
XIFM   ends
XIFCE  segment word public 'DATA'
XIFCE  ends

;Function pointers for Microsoft destructors
if LPTR
  XOB          segment word public 'BSS'
  XOB          ends
  XO           segment word public 'BSS'
               dd      33 dup(0)
  XO           ends
  XOE          segment word public 'BSS'
  XOE          ends
  DGROUP       group   XOB,XO,XOE
  __atexit_tbl =       XOB
  ATEXIT_END   =       XOE
else
  XOFB         segment word public 'BSS'
  XOFB         ends
  XOF          segment word public 'BSS'
               dd      33 dup(0)
  XOF          ends
  XOFE         segment word public 'BSS'
  XOFE         ends
  DGROUP       group   XOFB,XOF,XOFE
  __atexit_tbl =       XOFB
  ATEXIT_END   =       XOFE
endif

;Stuff all these segments into one group so they can all be accessed by DS
    ife LCODE
DGROUP group   XIFB,XIF,XIFE,XIB,XI,XIE,XCB,XC,XCE,XCFB,XCF,XCFE,_DATA,CONST,_BSS
    else
DGROUP group   XIFB,XIF,XIFE,XCFB,XCF,XCFE,_DATA,CONST,_BSS
    endif

DGROUP group   XIFCB,XIFU,XIFL,XIFM,XIFCE

    ifdef __OS2__
       includelib OS2.LIB
       extrn   DOSGETVERSION:far
       extrn   DOSGETMACHINEMODE:far
       extrn   DOSEXIT:far
       extrn   DOSWRITE:far
    endif
    ifdef _WINDOWS
       extrn   GETVERSION:far
    endif
    if LCODE
       extrn   __setenvp : far
     ifndef _WINDOWS
       extrn   ___get_c_file_info : far
     endif
    else
       extrn   __setenvp : near
     ifndef _WINDOWS
       extrn   ___get_c_file_info : near
     endif
    endif
    

       begdata

__psp          dw      ?               ;segment of program segment prefix
__pgmptr       dd      ?               ;pointer to fully-qualified program name
__osversion    label   word            ;Major-low, minor-high version
__osmajor      label   byte            ;MSC compatibility
__dos          db      ?               ;MS-DOS major version number
__osminor      label   byte            ;MSC compatibility
               db      ?               ;MS_DOS minor version number
__cpumode      db      0               ;= 1 if in protected mode
__osmode       db      0               ;0 for DOS, 1 for OS/2, 2 for Windows
__osver        dw      ?               ;Minor-low, major-high version
__winver       label   word
__winminor     db      ?
__winmajor     db      ?
_STKHQQ        dw      ?               ; ushort bottom of stk + safety margin

    ifdef _MT
    ifdef _WINDOWS
semmsg         db      'Semaphore error',0
    else
semmsg         db      0Dh,0Ah,'Semaphore error',0Dh,0Ah,0
    endif
    endif
    ifdef _WINDOWS
nomem          db      'Not enough memory',0
    else
nomem          db      0Dh,0Ah,'Not enough memory',0Dh,0Ah,0
    endif

       even

__tab_size     dw      8       ; default size of tabs in this system, see tabsize.h
_aenvseg       dw      0       ; Selector of Environment segment
                               ; (DLL's don't have an environment segment)
    ifndef DOS16RM
__datapar      dw      ?       ;# of paragraphs currently in data segment
                               ; (max is 0FFFh)
__pastdata     dw      ?       ;offset of 1 past data segment
__heapbottom   dw      ?       ;lowest offset in heap (used to detect
                               ; free() errors)
    endif

;Globals for argc and argv, so they are accessible and modifiable by
;static constructors.
___argc  label word                    ; MS compatible version
__argc dw      ?                       ;number of args

___argv  label word                    ; MS compatible version
__argv dw      ?                       ;filled in by C.ASM
    if LPTR
       dw      seg DGROUP
    endif
    if LPTR
__atexitp      dd      0
    else
__atexitp      dw      0
    endif
__atexit_max   dw      0

    ifndef __OS2__
;Stuff for overlay handler
       comm    __intno:byte    ;interrupt number that calls overlay service routine
       comm    __ovlflag:byte  ;set to !=0 if overlay handler is installed
       comm    __ovlvec:dword  ;previous overlay interrupt vector
    endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;The layout of this must match struct THREAD,
;as it is the thread 1 data

               public  __thread1
__thread1      label   word
               dw      0
               dw      0
               dw      0
;__oserr       label   word            ;Lattice C compatibility
_errno         dw      0               ;global error number
               dd      0               ;t_strtok
               dw      9 dup (0)       ;t_tm
       public  __fe_cur_env
__fe_cur_env   fenv_t  <>              ;current floating point environment
               db      26 dup (0)      ;t_asctime
               db      32 dup (0)      ;t_digstr
__doserrno     dw      0
       public  __cppeh_sv
if LCODE
;; Large or Medium
if LPTR
__cppeh_sv     db      350 dup (0)     ; globals for CPP exception handling.
else
__cppeh_sv     db      268 dup (0)
endif

else
;; Compact or Small
if LPTR
__cppeh_sv     db      330 dup (0)
else
__cppeh_sv     db      248 dup (0)
endif

endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

       public  __FE_DFL_ENV
__FE_DFL_ENV    fenv_t  <>              ;default floating point environment

_olddtor dw 0
         dw 0

       enddata

       begcode cinit

; Far entry point for DLL initialization code
       public  __cinit_f
__cinit_f proc far
    ife LCODE
       WINENTER_NF
       call    _cinit
        WINLEAVE_NF
       ret
    endif
__cinit_f endp

func   _cinit
       WINENTER_NF
       push    DI

       ;Figure out what OS version we're running under.
       ;We already did this in C.ASM, but do it again for MS Windows' benefit
    ifdef __OS2__
       push    DS
       pushi   AX,<offset DGROUP:__dos>
       call    DOSGETVERSION
       mov     AX,word ptr __dos
       xchg    AH,AL
       mov     word ptr __dos,AX       ;swap halves

       ;Find out if we are in real mode
       
       push    DS
       pushi   AX,<offset DGROUP:__cpumode>
       call    DOSGETMACHINEMODE
    else
    ifndef _WINDOWS
       bdos    30h
       mov     __osversion,AX
       xchg    AH,AL
       mov     __osver,AX
    else
       call    far ptr GETVERSION
       xchg    AH,AL
       mov     __winver,AX
       mov     __osver,DX
       xchg    DH,DL
       mov     __osversion,DX
    endif
    endif
    if LPTR
       les     BX,dword ptr __argv
       les     BX,[BX]
       mov     word ptr __pgmptr,BX
       mov     word ptr __pgmptr+2,ES
    else
       mov     BX,__argv
       mov     AX,[BX]
       mov     word ptr __pgmptr,AX
       mov     AX,DS
       mov     word ptr __pgmptr+2,AX
    endif
    
       mov     AX,SS:0Ah       ;WINDOWS puts top of stack here
       add     AX,100h         ;a safety margin for system calls
       mov     _STKHQQ,AX      ;save here

       call    doctors         ;perform static constructors
       call    __setenvp       ; Initialize environ prior to static constructors

    ifndef _WINDOWS
       call    ___get_c_file_info
    endif
       inc     AX
       jne     C1
       mov     DX,offset DGROUP:nomem
       jmp     F1
C1:    pop     DI
        WINLEAVE_NF
       ret
c_endp _cinit

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Terminate execution of C program.
; Input:
;      2[SP]   error code (ignored if not >= MS-DOS 2.00)

func	_Exit
c_endp	_Exit

func   _exit
    ifdef __OS2__
       mov     BP,SP
       pushi   AX,1            ;1 = terminate all threads in this process
       push    P-2[BP]         ;error code
       call    DOSEXIT
    else
       WINENTER_NF
    ifndef _WINDOWS
       ;Terminate overlay handler if it's installed
       _if     __ovlflag e 0, E2       ;if not installed
       mov     AL,__intno              ;get overlay interrupt number
       push    DS
       lds     DX,__ovlvec             ;DS:DX = previous vector
       mov     AH,25h
       bdos
       pop     DS
E2:
       bdos    30h             ;get DOS version number
       _if     AL b 2, E1      ;if pre-DOS 2.00
    endif
       mov     BP,SP
       mov     AL,P-2[BP]      ;AL = error code
       bdos    04Ch            ;Terminate a process (Exit)

E1:
    ifndef _WINDOWS
       push    __psp
       xor     AX,AX
       push    AX              ;push &(PSP:0)
       .retf                   ;far return to DOS
    endif
    endif
c_endp _exit

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Perform static constructors
; Go backwards through the list, so we do what was linked in last, first.

doctors        proc    near
       ;Call far ctors
       mov     DI,offset DGROUP:XIFE
CT2:   _if     DI be <offset DGROUP:XIFB>,CT1
       sub     DI,4
       mov     AX,[DI]
       or      AX,2[DI]
       jz      CT2                             ;skip null pointers
       call    dword ptr [DI]
       jmp     CT2

CT1:
    ife LCODE                  ;no near ctors if M or L models
       ;Call near ctors
       mov     DI,offset DGROUP:XIE
CT3:   _if     DI be <offset DGROUP:XIB>,CT4
       dec     DI
       dec     DI
       mov     CX,[DI]
       jcxz    CT3             ;skip null pointers
       call    CX
       jmp     CT3
    endif

; Call microsoft initializers
CT4:
if LPTR
       mov     AX,offset DGROUP:XOE
       sub     AX,offset DGROUP:XOB
else
       mov     AX,offset DGROUP:XOFE
       sub     AX,offset DGROUP:XOFB
endif
       ;mov     AX,offset DGROUP:ATEXIT_END
       ;sub     AX,offset DGROUP:__atexit_tbl
       sar     AX,1
       sar     AX,1
       mov     __atexit_max,AX
       mov     DI,offset DGROUP:XIFCE
CT6:   _if     DI be <offset DGROUP:XIFCB>,CT7
       sub     DI,4
       mov     AX,[DI]
       or      AX,2[DI]
       jz      CT6                             ;skip null pointers
       call    dword ptr [DI]
       jmp     CT6

CT7:   ret
doctors        endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Perform static destructors
; Go in the reverse order that the constructors were called.

func   _dodtors
       WINENTER_NF
    ife LCODE                  ;no near dtors if M or L models
       ;Call near dtors
       mov     DI,offset DGROUP:XCB
DT3:   _if     DI ae <offset DGROUP:XCE>,DT1
       mov     CX,[DI]
       inc     DI
       inc     DI
       jcxz    DT3             ;skip null pointers
;Make sure that this dtor is not called again
        mov     word ptr [DI-2], 0
       call    CX
        jmp     DT3
    endif

DT1:   ;Call far dtors
       mov     DI,offset DGROUP:XCFB
DT2:   _if     DI ae <offset DGROUP:XCFE>,DT4
        mov     AX,2[di]
        mov     word ptr _olddtor+2, ax
       mov     AX,[DI]
        mov     word ptr _olddtor, ax
        add     DI,4
        or      AX,-2[DI]
       jz      DT2                             ;skip null pointers
;Make sure that this dtor is not called again
        mov     word ptr [DI-4], 0
        mov     word ptr [DI-2], 0
        push    DI                              ;DI isn't perserved
        call    dword ptr _olddtor
        pop     DI
        jmp     DT2

DT4:   WINLEAVE_NF
       ret
c_endp _dodtors

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Print out fatal message and terminate program.
; Input:
;      DS:DX   points to ASCIZ message

       public  ___faterr
___faterr proc near
       push    BP
       mov     BP,SP
       mov     DX,P[BP]
F1:
    ifdef __OS2__
       ;Determine length of message in CX
       push    DS
       pop     ES
       mov     DI,DX           ;ES:DI -> string
       clr     AX
       mov     CX,-1
       repne   scasb
       not     CX
       dec     CX

       mov     AX,2
       sub     SP,AX           ;leave space for return value
       mov     BX,SP           ;and remember address
       push    AX              ;stderr file handle
       push    DS              ;message segment
       push    DX              ;message offset
       push    CX              ;message length
       push    SS              ;return value segment
       push    BX              ;return value offset
       call    DOSWRITE
    else
    ifdef _WINDOWS
       push    DS
       push    DX
       call    MESSAGEFATALERROR
    else
       ;Get pointer to end of string
       push    DS
       pop     ES
       mov     DI,DX           ;ES:DI -> string
       clr     AX
       mov     CX,-1
       repne   scasb

       dec     DI
       mov     byte ptr [DI],'$'       ;terminate string with $
       bdos    9
    endif
    endif
       mov     AX,1            ;error exit code
       push    AX
       call    __exit          ;abort
       ;never reached
___faterr endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Something went wrong with the semaphores.
; Print an error message and abort the program.

    ifdef _MT
       c_public _semerr
func   _semerr
       mov     DX,offset DGROUP:semmsg
       jmp     F1
c_endp _semerr
    endif

       endcode cinit

       end
