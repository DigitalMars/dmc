/* Copyright (C) 1986-1994 by Digital Mars. $Revision$ */

#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <scrtl.h>


/* Notes:
 *  This routine is called from _ptrchk.asm. The asm module takes care of
 * saving and restoring all registers on the call to _ptrchk. This asm module
 * also initializes the global variables __stack_bottom, and
 * __current_stackptr. These variables contain the necessary information
 * to determine if the address is pointing into an unused area of the stack.
 *   
 * The following three checks are made:
 *   1. If the address is above the bottom of the stack, check if it is less 
 *      than the stack pointer as it was before this routine was called. This 
 *      value is computed from the base pointer plus the size of the pushed 
 *      argument, the  return address and the pushed base pointer itself.
 *   2. Check if the address is within a free heap block.
 * If any of the above conditions is found, execution is halted by causing an 
 * actual GP fault.
 */

extern void *_check_stack_bottom;  // Points to bottom of stack for checking purposes
extern void *_current_stackptr;

void __pascal ptrchk (void *addr)
{
  struct heapfree *freeblock;
  

  // Is address between stack bottom and current ptr.
  // if so, generate gp fault.
  if (addr >= _check_stack_bottom) {
    if (addr < _current_stackptr)
	    goto fault;
  }

  // Otherwise, loop through free list.
  // if pointer points at a block in free list, then generate gp fault.
  for (freeblock = _baslnk.next; freeblock != &_baslnk; freeblock = freeblock->next)
      if ( (addr >= freeblock) && (addr < ((char *)freeblock + freeblock->size)) )
		      goto fault;
  return;

fault:
    *((int *)NULL) = 0; /* generate GP fault */
}

