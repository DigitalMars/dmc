/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */

#include <stdlib.h>


/* Notes:
 * This routine is called from _ptrchk.asm. The asm module takes care of
 * saving and restoring all registers on the call to _ptrchk. This asm module
 * also initializes the global variables __stack_top, __stack_bottom, and
 * __current_stackptr. These variables contain the necessary information
 * to determine if the address is pointing into the stack or not and if so
 * does it point to a valid area.
 *
 * The following checks are made:
 *   1. If the address points into the stack, check if it is less than the
 *      stack pointer as it was before this routine was called. This value is
 *      computed from the base pointer plus the size of the pushed argument, the
 *      return address and the pushed base pointer itself.
 *   2. Check if the address is within a free heap block.
 * If any of the above conditions is found, execution is halted by causing an
 * actual GP fault.
 */


extern void *_current_stackptr; // Contains value of current stack ptr
extern void *_check_stack_top;  // contains top of stack
extern void *_check_stack_bottom; // Contains "bottom" of stack.  This is the


void __pascal ptrchk(void *addr)
{
    int index;                  // index into anchor list of free blocks

    if (addr <= _check_stack_top && addr >= _check_stack_bottom)
    {
        if (addr < _current_stackptr)
            goto fault;
        goto end;
    }

    // TODO:  Ptrchk should loop through free list making sure address does not point in there.
end:
    return;

fault:
    *((int *) NULL) = 0;        /* generate GP fault */
}

