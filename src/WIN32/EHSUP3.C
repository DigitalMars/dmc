/*_ ehsup3.c	
 * Copyright (C) 1994 by Digital Mars
 * All rights reserved.
 */

#include "ehsup.h"

/*
 * Handle NT Exception
 */
 
EXCEPTION_DISPOSITION __cdecl _except_handler3(EXCEPTION_RECORD *eRecord,
                   ESTABLISHER_FRAME * frame,CONTEXT *context,void *dispatchercontext)
{
    // Temporary variables
    int index;
    SCOPETABLE table;
    int result;
    struct _EXCEPTION_POINTERS **eptrptr;  /* Contains address that compiler uses for exception pointers  */
    struct _EXCEPTION_POINTERS eptr; /* contains info to pass to compiler */

    // If the system is unwinding, then we need to call only termination handlers
    // in the current frame
    if (eRecord->ExceptionFlags & EXCEPTION_UNWIND) 
       _local_unwind(frame,END_INDEX,1);  // _local_unwind calls all termination handlers for current frame
    // Otherwise, look for something to handle the exception
    else {
        
        /*
          Fill in information for GetExceptionInformation
          
          The info to fill in is located 4 bytes prior to the frame pointer
         */
       eptrptr = (struct _EXCEPTION_POINTERS **)((char *)frame - sizeof(eptrptr));
       eptr.ExceptionRecord = eRecord;
       eptr.ContextRecord = context;
       *eptrptr = &eptr;
       
       // Search for handler.
       for (index = frame->scopeindex; index != END_INDEX; index = table.next) {
           table = frame->scopetable[index];  // grab current table entry
           if (table.Filter) {
               result = _call_except_block(table.Filter,(long)&frame->ebp);
               switch (result) {
                case EXCEPTION_CONTINUE_EXECUTION:
                     return ExceptionContinueExecution;
                case EXCEPTION_CONTINUE_SEARCH:
                     break;
                case EXCEPTION_EXECUTE_HANDLER:
                     // We found something to handle the exception
                     // First do a global unwind to call all termination handlers not in this frame
                     // Then call termination handlers in current frame
                     // Finally call handler.
                     _global_unwind(frame,NULL);
                     _local_unwind(frame,index,1);
                     _call_except_block(table.Handler,(long)&frame->ebp);
                     return ExceptionContinueSearch;
               }
           }
       }
          
    }
    return ExceptionContinueSearch;
}


#include <setjmp.h>

/* Some Offsets off of _JUMP_BUFFER to data */
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

/*
 *  Do a local_unwind, called from LongJmp when user is 
 *  using SEH and longjmp in combination.
 */
__declspec(naked) void __stdcall _seh_longjmp_unwind(_JUMP_BUFFER *jb)
{
    __asm {
            push EBP        ;; Save EBP
            mov ECX,8[ESP]  ;; ECX contains jb
            mov EBP, _BP[ECX] ;; grab EBP of Frame and store into EBP, for local unwind to use
            push dword ptr 1  ;; inform local_unwind to take addr for EBP
            mov EAX,_Etrylevel[ECX]  ;; Grab frame to local unwind
            push EAX    ;; Give local unwind the index
            mov EAX,_Ereg[ECX]
            push EAX  ;; Finally push the frame
            call _local_unwind
            add ESP,12 ;; restore stack
            pop EBP  ;; make sure to restore EBP
            ret
    }
}
            
      

