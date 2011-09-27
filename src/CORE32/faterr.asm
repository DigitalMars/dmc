;_ faterr.asm   
; Copyright (C) 1985-1994 Digital Mars
; All rights reserved.
; 32 bit fatal error exit
; $Revision: 1.1.1.2 $

include macros.asm

       extrn   __Exit:near

ifdef _WIN32
       extrn __win32_faterr:near
endif

ifdef M_UNIX
       extrn   _write:near
endif

ifdef __OS2__
       extrn   DosWrite:near
endif

ifdef M_UNIX
       begdata
oneline                db      0dh, 0ah, 0
onelineend     equ     $-oneline
       enddata
endif

       begcode faterr
       
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Print out fatal message and terminate program.
; Input:
;      PS[ESP] points to ASCIZ message

       public  ___faterr
___faterr proc near
ifdef M_UNIX
       push    DS
       pop     ES
       mov     EDI,PS[ESP]     ;ES:EDI -> string
       mov     EDX,EDI
       clr     EAX
       mov     ECX,-1
       repne   scasb
       not     ECX
       dec     ECX
       push    ECX              ;Message length
       push    EDX              ;Message string
       push    2                ;stderr
       call    _write
       add     esp,12
       push    onelineend
       push    offset DGROUP:oneline
       push    2                ;stderr
       call    _write           ;write an empty line
       add     esp,12
else
    ifdef _WIN32
       jmp __win32_faterr
    else
    ifdef __OS2__
       ;Determine length of message in ECX
       push    DS
       pop     ES
       mov     EDI,PS[ESP]     ;ES:EDI -> string
       mov     EDX,EDI
       clr     EAX
       mov     ECX,-1
       repne   scasb
       not     ECX
       dec     ECX

       push    EBX             ;space for return value
       mov     EAX,ESP
       push    EAX             ;pointer to return value
       push    ECX             ;message length
       push    EDX             ;pointer to message
       push    2               ;stderr
       call    DosWrite
       add     ESP,4*4
    else
       ;Get pointer to end of string
       push    DS
       pop     ES
       mov     EDI,PS[ESP]     ;ES:EDI -> string
       mov     EDX,EDI
       clr     EAX
       mov     ECX,-1
       repne   scasb

       dec     EDI
       mov     byte ptr [EDI],'$'      ;terminate string with $
       bdos    9
    endif
    endif
    endif
    ifndef _WIN32
       push    1               ;error exit code
       call    __Exit
    endif
___faterr endp

       endcode faterr
       
       end
