;_ cinit.asm
; Copyright (C) 1985-1994 Digital Mars
; All rights reserved.
; C initialization for Phar Lap DOS386 and UNIX System V
; $Revision: 1.1.1.3 $

;define DOS386 for Phar Lap
;define M_UNIX or M_XENIX for Unix (default)
;define __OS2__ for OS/2 2.0

include macros.asm
include flthead.asm

ifdef _WIN32
  DATAGRP      EQU     FLAT
else
  DATAGRP      EQU     DGROUP
endif

       extrn   ___faterr:near

ifdef _WIN32
       extrn   _GetCurrentThreadId@0:near
       extrn   _GetEnvironmentStrings@0:near
       extrn   _GetVersion@0:near
ifndef _DLL
       extrn   ___rtl_init_cppexceptions:near
       extrn   ___rtl_clean_cppexceptions:near
endif
       extrn   _FreeEnvironmentStringsA@4:near
ifndef _WINDLL
       extrn   ___callve:near
endif
endif

ifdef _NOCTOR
       extrn   __sti_init_std_files:near
       extrn   __sti_getenv:near
       extrn   __8087_init:near

endif
ifndef _DLL
       extrn   __setenvp:near
endif
        public __osmode,__cpumode,__dodtors
ifndef _DLL
        public  ___argc,___argv
        public  __argc,__argv
endif
        public __cinit
ifndef _DLL
        public __envptr
endif
        public __pgmptr
ifdef _WIN32
ifndef _DLL
        public  ___wargv, __wargv
        public  __wenvptr
endif
        public  __wpgmptr
endif
        public  __tab_size
        public  __atexit_max,__atexit_tbl, __atexitp

XIFCB          segment dword use32 public 'DATA'
XIFCB          ends
XIFU           segment dword use32 public 'DATA'
XIFU           ends
XIFL           segment dword use32 public 'DATA'
XIFL           ends
XIFM           segment dword use32 public 'DATA'
XIFM           ends
XIFCE          segment dword use32 public 'DATA'
XIFCE          ends
XOFB           segment dword use32 public 'BSS'
__atexit_tbl   label   dword
XOFB           ends
XOF            segment dword use32 public 'BSS'
               ;dd      65 dup(0)               ;64 for _atexit_tbl + 1 for null ptr
		org	4 * 65
XOF            ends
XOFE           segment dword use32 public 'BSS'
XOFE           ends
DGROUP         group   XIFCB,XIFU,XIFL,XIFM,XIFCE,XOFB,XOF,XOFE

; Microsoft Static Constructurs and Destructors
ifdef _WIN32
.CRT$XIA segment para use32 public 'DATA'
public __xi_a
__xi_a dd 0
.CRT$XIA ends

.CRT$XIZ segment para use32 public 'DATA'
public __xi_z
__xi_z dd 0
.CRT$XIZ ends

.CRT$XCA segment para use32 public 'DATA'
public __xc_a
__xc_a dd 0
.CRT$XCA ends

.CRT$XCZ segment para use32 public 'DATA'
public __xc_z
__xc_z dd 0
.CRT$XCZ ends

.CRT$XPA segment para use32 public 'DATA'
public __xp_a
__xp_a dd 0
.CRT$XPA ends

.CRT$XPZ segment para use32 public 'DATA'
public __xp_z
__xp_z dd 0
.CRT$XPZ ends

.CRT$XTA segment para use32 public 'DATA'
public __xt_a
__xt_a dd 0
.CRT$XTA ends

.CRT$XTZ segment para use32 public 'DATA'
public __xt_z
__xt_z dd 0
.CRT$XTZ ends
endif

       begdata

if COFF OR ELF
       extrn __xib:dword
       extrn __xie:dword
       extrn __xcb:dword
       extrn __xce:dword
endif


__atexitp      dd      0
__atexit_max   dw      0
__tab_size     dw      8       ; default size of tabs in this system, see tabsize.h

__cpumode       dw      1      ;protected mode for all 32 bit.
__pgmptr        dd      ?      ;pointer to fully-qualified program name
ifdef _WIN32
__wpgmptr       dd      ?      ;pointer to fully-qualified Unicode program name
endif
ifdef DOS386
       public __psp,__osversion,__osver,__osmajor,__dos,__osminor,
       public __DOS386_version,__DOS386_environ

__psp           dw     SEG_PSP ;segment of program segment prefix
__osversion     label   dword
__osmajor       label   byte   ;MSC compatibility
__dos           db      ?      ;MS-DOS major version number
__osminor       label   byte   ;MSC compatibility
                db     ?       ;MS_DOS minor version number
                dw     0
__osver         dw     ?
                dw     0
__osmode        db     0       ;DOS
__DOS386_version dd    ?
__DOS386_environ dd    ?
       public  __8087
__8087         dd      0
else
ifdef __OS2__
       public  __osversion,__osmajor,__dos,__osminor
__osversion label dword
__osmajor   label dword        ;MSC compatibility
__dos            dd    ?       ;MS-DOS major version number
__osminor   label dword        ;MSC compatibility
                 dd    ?       ;MS_DOS minor version number
__osmode         db    1       ;OS/2
else
ifdef _WIN32
       public  __osver,__winver,__winmajor,__winminor
__osver          dd    ?
__winver         label dword
__winminor       db    ?
__winmajor       db    ?
                 dw    0
__osmode         db    2       ;Windows
else
       public  __8087
__8087         dd      0
endif
endif
endif

    ifdef _MT
semmsg         db      0Dh,0Ah,'Semaphore error',0Dh,0Ah,0
    endif
nomem          db      0Dh,0Ah,'Not enough memory',0Dh,0Ah,0

ifndef _DLL

__envptr       dd      0	;Pointer to the environment (char *_envptr)
ifdef _WIN32
__wenvptr      dd      0	;Pointer to the environment (wchar_t *_wenvptr)
endif

endif

;Globals for argc and argv, so they are accessible and modifiable by
;static constructors.
___argc        label           dword
__argc         dd      ?       ;number of args

___argv        label           dword
__argv         dd      ?       ;filled in by C.ASM
ifdef _WIN32
___wargv       label           dword
__wargv        dd      0       ;filled in by C.ASM (wchar_t *_wargv).
endif

ifndef M_UNIX
ifndef M_XENIX
       public  __FE_DFL_ENV
__FE_DFL_ENV   fenv_t  <>              ;default floating point environment
;_FE_DFL_ENV has been moved to fltenv.c because SCO's cvtomf utility
;can not handle struc's properly.
endif
endif

ifndef _DLL  ;; Client of SCCRT.DLL should not have their own copy of this

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;The layout of this must match struct THREAD,
;as it is the thread 1 data

               public  __thread1
__thread1      label   word
               dd      0               ;t_stackoff
               dd      0               ;t_parameter
               dd      0               ;t_func
               dd      0               ;t_stacktop
;__oserr       label   word            ;Lattice C compatibility
ifndef M_UNIX
ifndef M_XENIX
;For Xenix and Unix this has been moved to sys.asm to prevent multiple defines.
       public  _errno
_errno         dd      0               ;global error number
endif
endif
               dd      0               ;t_strtok
    ifdef _WIN32
               dd      0               ; t_id
               dd      0               ; t_hndl
               dd      0               ;t_envptr
    endif
               dd      9 dup (0)       ;t_tm

ifndef M_UNIX
ifndef M_XENIX
       public  __fe_cur_env
__fe_cur_env   fenv_t  <>              ;t_fenv
;__fe_cur_env has been moved to fltenv.c for Unix because SCO's cvtomf
;utility can not handle struc's properly.
endif
endif
               db      26 dup (0)      ;t_asctime
               db      32 dup (0)      ;t_digstr
        public  __doserrno
__doserrno     dd      0
    ifdef _WIN32
; see to MT.H, STDIO.ASM, CINIT.ASM(This file).
               dd      0               ;t_wcstok	wcstok()
               dd      0               ;t_wenvptr	wgetenv()
               dw      26 dup (0)      ;t_wasctime	wasctime()
    endif
        public __cppeh_sv
    ifdef _WIN32
__cppeh_sv     dd      119 dup (0)       ;global statics for cplusplus exceptions..
					; (must match sizeof(struct Staticvar))
    else
__cppeh_sv     dd      110 dup (0)
    endif  ;; _WIN32
endif  ;; _DLL

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

       enddata

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

       begcode cinit

__cinit proc   near
    ifdef _WIN32
    ifndef _DLL
       call    _GetCurrentThreadId@0
       mov     DWORD PTR __thread1+018h, EAX   ;store first t_id value
       call    _GetEnvironmentStrings@0          ;set up envptr for build_environ's use.
       mov     __envptr,EAX
    endif
       call    _GetVersion@0                     ;set up version stuff
       xchg    AH,AL
       mov     word ptr __winver,AX
       shr     EAX,16
       mov     word ptr __osver,AX
    endif
    ifdef DOS386
       mov     AX,3000h                        ;set up version stuff
       bdos
       mov     word ptr __osversion,AX         ;<- Actually, this line is already done in cx
       xchg    AH,AL                           ;and cp.asm. I would remove it from them but
       mov     __osver,AX                      ;it doesn't seem worth destablizing the code.
       mov     EAX,__argv
       mov     EAX,[EAX]
       mov     __pgmptr,EAX
    endif
    ifndef _DLL
       call    __setenvp
    endif
       inc     AX
       jne     C2
       push    offset DATAGRP:nomem
       call    ___faterr
C2:
    ifdef _WIN32
    ifndef _WINDLL
       call ___callve                   ; Initialize VE
    endif
    ifndef _DLL
       call ___rtl_init_cppexceptions    ; Initialize CPP Exceptions before constructors
    endif
    endif
       call doctors                 ;perform static constructors
       ret
__cinit endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Perform static constructors
; Go backwards through the list, so we do what was linked in last, first.

doctors                proc    near
  ifdef _NOCTOR
                       call    __sti_init_std_files
                       call    __8087_init
                       call    __sti_getenv
  else
                       uses    <ECX,EDI>
    if COFF OR ELF
                       mov     EDI,offset DGROUP:__xie
               CT1:    _ifs    EDI be <offset DGROUP:__xib>,CT2
    else
                       mov     EDI,offset DATAGRP:XIE
               CT1:    _ifs    EDI be <offset DATAGRP:XIB>,CT2
    endif
                       sub     EDI,SIZEPTR
                       mov     ECX,[EDI]
                       jecxz   short CT1       ;skip null pointers
                       call    ECX
                       jmps    CT1
                       ; Call microsoft initializers
               CT2:    mov     EAX,offset DATAGRP:XOFE
                       sub     EAX,offset DATAGRP:XOFB
                       sar     EAX,1
                       sar     EAX,1
                       mov     __atexit_max,AX
                       mov     EDI,offset DATAGRP:XIFCE
               CT3:    _if     EDI be <offset DATAGRP:XIFCB>,CT4
                       sub     EDI,SIZEPTR
                       mov     ECX,[EDI]
                       jecxz   short CT3       ;skip null pointers
                       call    ECX
                       jmp     CT3
               CT4:
ifdef _WIN32
                       ; Call MS VC++ Initializers
                       mov     EDI,offset __xi_z
               CT5:    _ifs    EDI be <offset __xi_a>,CT6
                       sub     EDI,SIZEPTR
                       mov     ECX,[EDI]
                       jecxz   short CT5
                       call    ECX
                       jmp     CT5
               CT6:    mov     EDI,offset __xc_z
               CT7:    _ifs    EDI be <offset __xc_a>,CT8
                       sub     EDI,SIZEPTR
                       mov     ECX,[EDI]
                       jecxz   short CT7
                       call    ECX
                       jmp     CT7
               CT8:
endif
			unuse   <EDI,ECX>
  endif
                       ret
doctors                endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Perform static destructors
; Go in the reverse order that the constructors were called.

__dodtors      proc    near
  ifndef _NOCTOR
       uses    <EAX,ECX,EDI>
       ;Call near dtors
       clr     eax
    if _FLAT
       mov     edi,offset FLAT:XCB
DT1:   _ifs    edi ae <offset FLAT:XCE>,DT2
    else
if COFF OR ELF
       mov     edi,offset DGROUP:__xcb
DT1:   _ifs    edi ae <offset DGROUP:__xce>,DT2
else
       mov     edi,offset DGROUP:XCB
DT1:   _ifs    edi ae <offset DGROUP:XCE>,DT2
endif
    endif
       mov     ecx,[edi]
       add     edi,4
       jecxz   short DT1       ;skip null pointers
       mov     [edi-4], eax    ; clear pointer so we only do this once (could happen with DLL)
       call    ecx
       jmps    DT1

DT2:
    ifdef _WIN32
    ifndef _DLL
    call ___rtl_clean_cppexceptions
    push __envptr
    call _FreeEnvironmentStringsA@4
    endif
    endif
    unuse   <edi,ecx,EAX>
  endif
       ret
__dodtors      endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Something went wrong with the semaphores.
; Print an error message and abort the program.

    ifdef _MT
       public  __semerr
__semerr proc  near
       push    offset FLAT:semmsg
       call    ___faterr
__semerr endp
    endif

       endcode cinit

       end

