/*_ ehsup2.c	
 * Copyright (C) 1994 by Digital Mars
 * All rights reserved.
 */

#include "ehsup.h"

/*
 * Handle NT Exception
 */
 
EXCEPTION_DISPOSITION __cdecl _except_handler2(EXCEPTION_RECORD *eRecord,
                   ESTABLISHER_FRAME * frame,CONTEXT *context,void *dispatchercontext)
{
    // Temporary variables
    int index;
    SCOPETABLE table;
    int result;
    struct _EXCEPTION_POINTERS eptr;
    
    // If the system is unwinding, then we need to call only termination handlers
    // in the current frame
    if (eRecord->ExceptionFlags & EXCEPTION_UNWIND) 
       _local_unwind(frame,END_INDEX,0);  // _local_unwind calls all termination handlers for current frame
    // Otherwise, look for something to handle the exception
    else {
       // Fill in exceptioninfo
       eptr.ContextRecord = context;
       eptr.ExceptionRecord = eRecord;
       frame->exceptioninfo = &eptr;
       
       // Search for handler.
       for (index = frame->scopeindex; index != END_INDEX; index = table.next) {
           table = frame->scopetable[index];  // grab current table entry
           if (table.Filter) {
               result = _call_except_block(table.Filter,frame->ebp);
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
                     _local_unwind(frame,index,0);
                     _call_except_block(table.Handler,frame->ebp);
                     return ExceptionContinueSearch;
               }
           }
       }
          
    }
    return ExceptionContinueSearch;
}
      

