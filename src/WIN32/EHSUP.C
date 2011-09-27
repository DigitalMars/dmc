/*_ ehsup.c	
 * Copyright (C) 1994 by Digital Mars
 * All rights reserved.
 */

#include "ehsup.h"


/*   
 * Jump to except block, remember to restore EBP prior to doing so...
 * also be sure to save and restore all registers.
 *  it will have the return value from the block in EAX.
 */
 
__declspec(naked) int _call_except_block(void *address,long Callee_EBP)
{
	__asm {	
		push	EBP
		mov	EBP,ESP
		push  ECX
		push	EBX
		push	ESI
		push	EDI
		mov   EBX, dword ptr 8[EBP]  ; Save Function address to call.
		push	EBP                     
		mov	EBP, dword ptr 12[EBP]
		call  EBX
		pop	EBP
		pop	EDI
		pop	ESI
		pop	EBX
		pop ECX
		mov ESP,EBP
		pop	EBP
		ret
	}
}

/*
 * Exception handler setup in local_unwind2.  This eases checking for abnormal termination.
 *  and also allows detection of unwind collisions.
 */
 EXCEPTION_DISPOSITION __cdecl _local_except_handler (struct _EXCEPTION_RECORD *ExceptionRecord,
 	ESTABLISHER_FRAME * EstablisherFrame,
	struct _CONTEXT *ContextRecord,
	void *DispatcherContext
	)
{
  ESTABLISHER_FRAME **temp = (ESTABLISHER_FRAME **)DispatcherContext;  // Used to store info for unwind collisions
    /*
     *  If this is called due to a normal exception,
     *  we will not do anything, tell the system to 
     *  continue searching
     */
    if (!(ExceptionRecord->ExceptionFlags & EXCEPTION_UNWIND)) 
        return ExceptionContinueSearch;
    
    /*
     *  Otherwise, there is a problem.
     *  If the system is called while unwinding, then this means
     * an unwind was intiated within us handling the exception, 
     * this is not good as what would happen is if we were
     * to continue, then the RTL may get called, we would call
     * the exception, which would again initiate the unwind etc.
     */

     *temp = EstablisherFrame;
     return ExceptionCollidedUnwind;
     
    
}


/*
 *Initiate a global unwind by calling the system
 */
__declspec(naked) void __cdecl _global_unwind(ESTABLISHER_FRAME *Frame, EXCEPTION_RECORD *eRecord)
{
    __asm {
		push	EBP
		mov	EBP,ESP
		push  ECX
		push	EBX
		push	ESI
		push	EDI
		push  EBP
		push	0
		push	dword ptr 12[EBP]   ;; Exception Record
		call __system_unwind
		jmp    __unwind_exit
 __system_unwind:
		push	dword ptr 8[EBP]
		call	RtlUnwind
 __unwind_exit:
      pop EBP
		pop	EDI
		pop	ESI
		pop	EBX
		pop ECX
		mov	ESP,EBP
		pop EBP
		ret
    }
}

/*
 * Do a local unwind.  This walks the local scope table, calling any termination handlers.
 *   Frame is the frame of the exception, stop_index is the index to go until
 * and take_addr is 1 to take the &of ebp or 0 to not.
 */
void __cdecl _local_unwind(ESTABLISHER_FRAME *Frame, int stop_index, int take_addr)
{

   SCOPETABLE table;
   int index = Frame->scopeindex;

	// Setup our own exception handler.
   __asm {
		  push   dword ptr -1
		  push   dword ptr 0
        push	offset _local_except_handler
        push	dword ptr fs:_except_list
        mov	fs:_except_list,esp
   }


   // Until stop index and not end of list, look for finally blocks
   for (; index != END_INDEX && index != stop_index; index = table.next) {
       table = Frame->scopetable[index];  // Grab current entry
       // NULL Filters indicate Termination handler
       if (!table.Filter) 
          _call_except_block(table.Handler, (take_addr ? (long)&Frame->ebp:Frame->ebp));
   }

	// Restore exception list
   __asm {
        pop fs:_except_list
        add esp,12   ;; pop off except handler, table, and index
   }
}

__declspec(naked) void __cdecl _local_unwind2(void)
{
  __asm {
    jmp _local_unwind
  }
}

/*
 * Returns:
 *		    1 -- Finally block was called abnormally (I.E. } of TRY not executed)
 *        0 -- Finally block called during normal execution.
 */
__declspec(naked) int __cdecl _abnormal_termination(void)
{
    __asm {
	xor EAX,EAX
        mov ECX,FS:_except_list  ; Get current exception
        cmp 4[ECX],offset _local_except_handler
        jne L1
        mov EAX, 1
L1:
	     ret
	}
}
