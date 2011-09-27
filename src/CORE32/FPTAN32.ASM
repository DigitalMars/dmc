
; static char sccs_id[] = "@(#)fptan32.asm 1.4  12/20/94  16:51:51";
;
; This code is being published by Intel to users of the Pentium(tm)
; processor.  Recipients are authorized to copy, modify, compile, use and
; distribute the code.
;
; Intel makes no warranty of any kind with regard to this code, including
; but not limited to, implied warranties or merchantability and fitness for
; a particular purpose. Intel assumes no responsibility for any errors that
; may appear in this code.
;
; No patent licenses are granted, express or implied.
;
;
;  The following code is a PRELIMINARY IMPLEMENTATION of a
;  software patch for the floating point divide instructions.
;
;

include macros.asm

.386
.387

PATCH_CW  EQU 00ch
PREV_CW  EQU 010h
COSINE  EQU 0  ; These two are overlaid because they
ANGLE  EQU 0  ; are not live at the same time.


STACK_SIZE EQU 014h

ONESMASK EQU 0e000000h


	begdata

fdiv_risk_table DB 0, 1, 0, 0, 4, 0, 0, 7, 0, 0, 10, 0, 0, 13, 0, 0
fdiv_scale DD 03f700000h  ; 0.9375
one_shl_63 DD 05f000000h

	enddata


	begcode	math87

 public fptan_chk
 public _fptan_chk

;
;  PRELIMINARY VERSION for register-register divides.
;


fptan_chk PROC NEAR
_fptan_chk:

 push eax
 sub esp, STACK_SIZE
 fstp tbyte ptr [esp+ANGLE]
 mov eax, [esp+ANGLE+8]
 and eax, 07fffh
 jz use_hardware  ; denormals, ...
 cmp eax, 07fffh
 je use_hardware  ; NaNs, infinities, ...
 mov eax, [esp+ANGLE+4]
 add eax, eax
 jnc use_hardware  ; unnormals (explicit 1 missing)
 fld tbyte ptr [esp+ANGLE]

;
; Check for proper parameter range ( |<angle>| < 2^63)
;
 fabs
 fcomp one_shl_63
 fnstsw ax
 sahf
 jae use_hardware
 
 fld tbyte ptr [esp+ANGLE]
 fsincos
 fstp tbyte ptr [esp+COSINE]
 fld tbyte ptr [esp+COSINE] ; load the denominator (cos(x))
 mov eax, [esp+COSINE+4] ; get mantissa bits 32-64
 add eax,eax   ; shift the one's bit onto carry
 xor eax, ONESMASK  ; invert the bits that must be ones
 test eax, ONESMASK  ; and make sure they are all ones
 jz scale_if_needed  ; if all are one scale numbers
 fdivp st(1), st  ; use of hardware is OK.
 fld1    ; push 1.0 onto FPU stack
 add esp, STACK_SIZE
 pop eax
 ret

scale_if_needed:
 shr eax, 28   ; keep first 4 bits after point
 cmp fdiv_risk_table[eax], ah ; check for (1,4,7,a,d)
 jnz divide_scaled  ; are in potential problem area
 fdivp st(1), st  ; use of hardware is OK.
 fld1    ; push 1.0 onto FPU stack
 add esp, STACK_SIZE
 pop eax
 ret

divide_scaled:
 fwait    ; catch preceding exceptions
 fstcw [esp+PREV_CW]  ; save caller's control word
 mov eax, [esp+PREV_CW] 
 or eax, 033fh  ; mask exceptions, pc=80
 mov [esp+PATCH_CW], eax
 fldcw [esp+PATCH_CW]  ; mask exceptions & pc=80
 fmul fdiv_scale  ; scale denominator by 15/16
 fxch
 fmul fdiv_scale  ; scale numerator by 15/16
 fxch

; This assures correctly rounded result if pc=64 as well

 fldcw [esp+PREV_CW]  ; restore caller's control word
 fdivp st(1), st  ; use of hardware is OK.
 fld1    ; push 1.0 onto FPU stack
 add esp, STACK_SIZE
 pop eax
 ret

use_hardware:
 fld tbyte ptr [esp+ANGLE]
 fptan
 add esp, STACK_SIZE
 pop eax
 ret
fptan_chk ENDP

	endcode	math87
 end

