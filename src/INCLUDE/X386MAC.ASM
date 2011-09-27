; Copyright Doug Huffman
; Route 1 Box 54A
; Lenore, ID 83541

; February 14, 1991
; FAX:   208-263-8772
; email: doug@proto.com
comment&
_______________________________________________________________________________
                                  x386mac.asm
_______________________________________________________________________________

Include file for x386 things, defines privilege level zero segments and some 
macros.
&

__X386_DIAG     equ     0

;16 bit data segment
__X386_DATASEG_16      segment para use16 public 'DATA16'
__X386_DATASEG_16      ends

__X386_GROUP_16  group  __X386_DATASEG_16

pushseg macro
        push    ds
        push    es
        push    fs
        push    gs
endm

popseg  macro
        pop     gs
        pop     fs
        pop     es
        pop     ds
endm

begdata_16 macro
;data segment for real mode and priv level 0 activities
__X386_DATASEG_16       segment para use16 public 'DATA16'
        align 4         ;dword align
endm

enddata_16 macro
__X386_DATASEG_16       ends
endm

;;;;;;;;;;;;;;;;;;;;;;;;;
; Call a real mode function.  Returns with Carry set if an error.  EAX has
; error status or return value from real mode function.  All information is
; passed in general registers.  Registers EAX, EBX, ECX and ES are destroyed 
; on entry.  Registers EDX,ESI,EDI are passed through to real mode function.
; All general registers are returned intact from real mode function.
;requires extrn __x386_version:word within begdata segment
; Syntax:
;       callr   function_name,size_of_stack_args_in_bytes
; or:
;       callr   function_name
;
; December 17, 1990
;;;;;;;;;;;;;;;;;;;;;;;;;


callr  macro   f_name,argsize
        LOCAL   err,callr1,callr2
extrn   __realmode_start:byte,__realmode_end:byte
        push    ECX
        cmp     __x386_version,0        ;0 indicates pharlap
        je      callr1                  ;jump if pharlap
        pop     ECX
        push    EDX
        push    ESI
        push    EDI
        mov     ax,2509h
;get segments on X386 extender only, returns __X386_CODESEG_16 in BX
        int     21h
        push    BX              ;push real mode code segment
        mov     EBX,offset __X386_CODESEG_16:f_name
        push    BX              ;push real mode offset
        pop     EBX
        pop     EDI
        pop     ESI
        pop     EDX
        jmp     callr2
callr1:
        pop     ECX
ife ESeqDS
        mov     AX,DS           ;;ES:EBX will point to start of real mode stuff
        mov     ES,AX
endif
        lea     EBX,__realmode_start
        lea     ECX,__realmode_end
        sub     ECX,EBX         ;;Get length of real mode stuff needed.
        mov     AX,250fh        ;;Get real mode address.
        int     21h
        jc      err

        mov     EBX,ECX         ;;Put real mode start address in EBX.
        lea     EAX,f_name      ;;Get offset from start of real mode stuff.
        lea     ECX,__realmode_start
        sub     EAX,ECX
        add     EBX,EAX         ;;Add the function offset.
callr2:
ifnb    <argsize>
        mov     ECX,argsize
else
        clr     ECX             ;;Number of words on stack is 0.
endif
        mov     AX,250eh        ;;Call the real mode function.
        int     21h
err:
        endm

;The following descriptor is for code and data segments, it defaults to a real 
;address compatible data segment with a base at zero. See gdt.asm for more
;information regarding descriptors

app_descriptor struc
        limit_0         dw      0ffffh          ;limit 0 - 15
        base_0          dw      0               ;base 0 - 15
        base_16         db      0               ;base 16 - 23
        dpl_type        db      10010011b       ;dpl and type
        type_limit      db      0               ;type and limit 16 - 19
        base_24         db      0               ;base 24 - 31
app_descriptor ends

