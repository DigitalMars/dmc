/*_ jmp.c    
 * Copyright (C) 1985-2001 by Digital Mars
 * All Rights Reserved
 * www.digitalmars.com
 * Written by Walter Bright
 */

#include <setjmp.h>

#if defined(_WIN32)
#include <ehsup.h>
#endif

#if !defined(_WIN32)

#define _IP	0
#define _CS	4
#define _BP	8
#define _SP	12
#define _SS	16
#define _BX	20
#define _SI	24
#define _DI	28
#define _ES	32
#define _DS	36

#else

#define _BP 0
#define _BX 4
#define _DI 8
#define _SI 12
#define _SP 16
#define _IP	20
#define _Ereg 24
#define _Etrylevel 28
#define _Ereserved 32
#define _Ehandler 36
#define _Excptdata 40

#define SYMANTEC_SETJMP_VAL 0x83678080   //SCPP

#endif

#define PS 4   /* Offset on ESP to first argument on stack
				  Assuming nothing was pushed on the stack.*/


#ifdef _setjmp
#undef _setjmp
#endif

__declspec(naked) int _setjmp(jmp_buf env)
{
    __asm {
    	mov	ECX,PS[ESP]      ; move env into ecx
	    mov	_SP[ECX],ESP	;save old registers
    	mov	_BP[ECX],EBP
	    mov	_BX[ECX],EBX
    	mov	_SI[ECX],ESI
    	mov	_DI[ECX],EDI
#if !defined(_WIN32)
    	mov	_CS[ECX],CS
    	mov	_SS[ECX],SS
    	mov	_ES[ECX],ES
    	mov	_DS[ECX],DS
#else 
        mov  _Ereserved[ECX], (unsigned long) 0  ;; So longjmp knows it is not unwinding
        mov  _Etrylevel[ECX], (unsigned long) -1
        mov  EAX,FS:_except_list
        mov  _Ereg[ECX],EAX
        cmp EAX,dword ptr -1
        je __setjmp_ret
        mov EAX,12[EAX]  
        mov _Etrylevel[ECX],EAX
#endif
 __setjmp_ret:
    	mov	EAX,[ESP]
    	mov	_IP[ECX],EAX	;save return address offset
    	xor	EAX,EAX		;always returns 0
    	ret	
    }
}


__declspec(naked) void longjmp(jmp_buf env, int retval)
{
	__asm {
#if !defined(_WIN32)
	    pop	EAX		;throw away return address
	    pop	ECX		;get env
	    pop	EAX		;AX = val
	    test EAX,EAX
	    jnz	short L1
	    inc	EAX		;don't allow val == 0
     L1:
	    mov	SS,_SS[ECX]
        mov	DS,_DS[ECX]
	    mov	ES,_ES[ECX]
	    mov	ESP,_SP[ECX]	;restore environment
	    add	ESP,PS		;compensate for ret addr from setjmp()
	    mov	EBP,_BP[ECX]
	    mov	EBX,_BX[ECX]
	    mov	ESI,_SI[ECX]
	    mov	EDI,_DI[ECX]
	    push dword ptr _CS[ECX]
	    push dword ptr _IP[ECX]
	    retf		
#else
        pop EAX             ;; Throw away Return Address
        pop ECX             ;; ECX == env
        mov ESI,_Ereg[ECX]  ;; ESI == Exception List in ENV.
        cmp ESI,dword ptr FS:_except_list  ;; Are Reg and FS:0 the same??
        je __longjmp_L1 ;; Yes, no need to global unwind
        push 0  ;; Exception Record is NULL
        push ESI  ;;Frame stored in Env.
        call _global_unwind
        add esp, 8
        
__longjmp_L1:   
        test ESI,ESI  ;; Is there even one registred?
        je __longjmp_Ret  ;; No, do a simple longjmp
        mov EAX, _Ereserved[ECX]     ;; Make sure this jump buffer is setup from setmp3
        cmp EAX, dword ptr SYMANTEC_SETJMP_VAL
        jne  __longjmp_L2  ;; No, assume the unwind is local_unwind
        mov EAX,_Ehandler[ECX]
        test EAX,EAX
        je __longjmp_Ret
        push ECX   ;; Save ECX prior to call
        push ECX
        call EAX
        pop ECX  ;; Restore ECX
        jmp __longjmp_Ret
__longjmp_L2:
	push ECX
        push dword ptr 1
        push dword ptr -1
        push ESI
        call _local_unwind
        add esp, 12
	pop ECX
__longjmp_Ret:  
        pop EAX  ;; EAX has retval
        test EAX,EAX  ;; Make sure Val is not 0
        jne __longjmp_L3
        inc EAX
__longjmp_L3:
        mov EBP,_BP[ECX]
        mov EBX,_BX[ECX]
        mov EDI,_DI[ECX]
        mov ESI,_SI[ECX]
        mov ESP,_SP[ECX]
        add ESP, dword ptr 4  ;; Compensate for Setjmp Ret Addr
        mov EDX,_IP[ECX]  ;; EDX contains address to jmp to
        push EDX
        ret
#endif
    } 
}


#if defined(_WIN32)
/*
 * This routine is replaces setjmp under NT. It sets up jb such that longjmp 
 * can initiate a system unwind and call termination handlers.
 */
__declspec(naked) int _setjmp3(_JUMP_BUFFER jb, int Data_count,...)
{
    __asm {
        mov EDX,PS[ESP]   ;; Store jb into edx
        mov	_SP[EDX],ESP	;;save old registers
        mov _BP[EDX], EBP
        mov _BX[EDX], EBX
        mov _DI[EDX], EDI
        mov _SI[EDX], ESI
        mov _Ereserved[EDX],(unsigned long)SYMANTEC_SETJMP_VAL
        mov  EAX,[ESP]
        mov _IP[EDX], EAX  ;; Address to jump back to
        
        ;; Fill in Unwind_handler with default values
        mov _Ehandler[EDX],(unsigned long)0    ;; No handler by default
        
        ;; Fill in Exception List information
        mov EAX, FS:_except_list
        mov _Ereg[EDX], EAX
        
        cmp  EAX, dword ptr -1   
        je  __setjmp3_ret   ;; If it is -1, then no except info will be present, so return
        
        ;; Get count off of the stack
        mov ECX,PS+4[ESP]   ;; ECX has count of number of items
        test ECX,ECX
        jne __setjmp3_item1   ;; Go get unwind handler
  __trylevel_fromfs:        
         ;; Get Trylevel from EAX (FS:0)
        ;; FS:0 + 12 will give you current trylevel.  See ESTABLISHER_FRAME in setjmp.h
        mov EAX,12[EAX]
        mov _Etrylevel[EDX],EAX
        jmp __setjmp3_ret
        
  __setjmp3_item1:        
        mov EAX,PS+8[ESP]   ;;EAX will now contain unwind handler
        mov _Ehandler[EDX], EAX   ;; Copy it.
        dec ECX    ;; Decrement Count
        jne __setjmp3_item2   ;; Go get trylevel
        jmp __trylevel_fromfs    
        
  __setjmp3_item2:
        mov EAX,PS+12[ESP]  ;; EDX will contain try level
        mov _Etrylevel[EDX],EAX
        dec ECX
        je __setjmp3_ret
        ;; Copy remaining items into except data, use movsd
        cmp ECX,(unsigned long)6  ;; Make sure count is not > 6
        jbe __setjmp3_movdata
        mov ECX, (unsigned long)6  
  __setjmp3_movdata:        
        push ESI
        push EDI
        lea ESI, PS+24[ESP]  ;; Copy location of first item into ESI
        lea EDI, _Excptdata[EDX]  ;; Destination is jump buffer
        rep movsd  ;; Copy data.
        pop EDI
        pop ESI
  __setjmp3_ret:
        xor EAX,EAX   ;; Always return 0
        ret
      }
}
#endif
