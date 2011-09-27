/*_ _ptrchk.c  
 * Copyright (C) 1995 by Digital Mars Corporation
 * All rights reserved
 * $Revision: 1.1.1.1 $
 */

void *_check_stack_bottom=0; // Filled with bottom of stack
void *_check_stack_top=0;    // Filled with top of stack
void *_current_stackptr=0;   // Contains address of Stack Ptr upon function call

#if defined(_WIN32)
#define THREAD_STACKBOTTOM 8 // offset off of fs to stack bottom for current thread
#define THREAD_STACKTOP    4 // offset off of fs to stack top for current thread
#else
extern long _x386_break; 
#endif

#define OVERHEAD 12

void __pascal ptrchk(void *addr);

__declspec(naked) _ptrchk(void *ptr)
{
    __asm
    {
        push ebp
        mov ebp, esp  // Grab ESP to calculate current stack pointer
        // Save all registers
        push es
        push eax
        push ecx
        push edx
        push ebx
        push esi
        push edi

        // For DOSX haven't found mechanism to get this value,
        // so PTRCHK does not use it.
    #if defined(_WIN32)
        // Get address of top of stack
        lea esi, _check_stack_top
        mov eax, dword ptr fs:THREAD_STACKTOP      
        mov [esi], eax 
    #endif

        // Get address of bottom of stack
        lea esi, _check_stack_bottom
    #if defined(_WIN32)
        mov  eax, dword ptr fs:THREAD_STACKBOTTOM  
    #else
        mov eax,_x386_break    //_x386_break contains stack bottom in DOSX
    #endif
        mov [esi], eax

        // Store address of stackptr upon ptrchk call
        lea esi, _current_stackptr
        mov eax,ebp
        add eax,OVERHEAD  // EAX contains value of esp prior to ptrchk entry
        mov [esi],eax

        mov eax, 8[ebp]  // grab ptr

        // call ptrchk to do the actual checking...
        push eax
        call ptrchk

        // Restore Registers
        pop edi
        pop esi
        pop ebx
        pop edx
        pop ecx
        pop eax
        pop es
        mov esp,ebp  // Get proper ESP and return
        pop ebp
        ret 4
    }
}



