/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */

#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <scrtl.h>


/* Notes:
 *  This routine is called from _ptrchk.asm.  The asm module takes care of
 * saving and restoring all registers on the call to _ptrchk. This asm module
 * also initializes the global variables __stack_top, __stack_bottom, and
 * __current_stackptr. These variables contain the necessary information
 * to determine if the address is pointing into the stack or not and if so
 * does it point to a valid area.
 *
 * The following three checks are made:
 *   1. If the address is in the stack , check if it is less than the
 *      stack pointer as it was before this routine was called. These values
 *      are all computed in _ptrchk.asm and stored in the global variables
 *      discussed above.
 *   2. For MSDOS programs only, check if the address is NULL. Windows will catch
 *      this as a GP fault anyway.
 *   3. Check if the address is within a free heap block.
 * If any of the above conditions is found, execution is halted by calling the fatal
 *   error routine for MSDOS or by causing an actual GP fault for Windows.
 */


// Since Windows is a 16-Bit protected mode environment, no need to
// normalize the pointers.
#ifndef _WINDOWS
#define NORMALIZE(addr)  ((unsigned long)_FP_SEG(addr)*(unsigned long)16+(unsigned long)_FP_OFF(addr)) 
#else
#define NORMALIZE(addr)	 addr
#endif

// Globals setup in _ptrchk.asm to aid in stack checking.
extern void __far *_check_stack_top;
extern void __far *_check_stack_bottom;
extern void __far *_current_stackptr;

void __pascal ptrchk (void __far *addr)
{
  struct heapfree __far *freeblock;	 // Pointer to current block on _baslnk list
  char __far *endblock; // Pointer to end of current free block
  struct heapfree __far *local_baslnk; // Contains address of _baslnk for comparison purposes


// Check if a valid stack ptr.
// If pointer is in stack, then if ptr is < then current stack, fault
// otherwise, leave as no need to loop through free block.

// In 16-bit protected mode, only do stack checking if
// Segment of address is SS
#ifdef _WINDOWS
  if (_FP_SEG(addr) == _FP_SEG(_current_stackptr)) {
#endif
   if (NORMALIZE(addr) <= NORMALIZE(_check_stack_bottom) && NORMALIZE(addr) >= NORMALIZE(_check_stack_top)) {
    if (NORMALIZE(addr) < NORMALIZE(_current_stackptr))
	    goto fault;
	 goto end;
   }

#ifdef _WINDOWS
  }
#endif

// In 16-bit dos, if ptr is null, then we should generate a gp fault, else leave
# ifndef _WINDOWS
    if (addr == NULL)
      goto fault;
# else
    if (addr == NULL)
	   goto end;
# endif

// In near model windows, we use the sytem to allocate,
// no way curently to test pointer, so simply return
#if !(_WINDOWS && (__SMALL__ || __MEDIUM__))
  // Loop through free list, checking if addr points in there
  // Remember to NORMALIZE Pointers!!
  local_baslnk = &_baslnk;
  for (freeblock = local_baslnk->next; NORMALIZE(freeblock) != NORMALIZE(local_baslnk); freeblock = freeblock->next) {
   
    // For 16-bit protected mode, do check only if points into
	 // same segment as free block.
    #ifdef _WINDOWS
	 if (_FP_SEG(addr) == _FP_SEG(freeblock)) {
	 #endif

	 // Calculate address of end of block to use in comparison, NORMALIZE macro did not like 
	 // Computation passed to it.
	 endblock = (char __far *)freeblock + freeblock->size;
    if (NORMALIZE(addr) >= NORMALIZE(freeblock) && NORMALIZE(addr) < NORMALIZE(endblock)) 
       goto fault;
	 
    #ifdef _WINDOWS
	 }
	 #endif

  }
#endif

end:
  return;

fault:
# ifdef _WINDOWS
    *((int __far *)NULL) = 0; /* generate GP fault */
# else
    __faterr ("Pointer check fault");
# endif
}

