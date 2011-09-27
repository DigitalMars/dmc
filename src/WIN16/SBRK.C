//_ winsbrk.c
// Copyright (C) 1985-1993 by Digital Mars
// All rights reserved
// Written by Walter Bright
// $Revision: 1.1.1.1 $

#include <stdlib.h>
#include <windows.h>
#include <dos.h>

unsigned _alloc_flags = 0;

/* Windows versions of local heap.
 * We used to just allocate a heap, and use our own very fast
 * code to manage it. Unfortunately, too many people needed to
 * mix the Localxxxx() functions and malloc/free. So malloc/free
 * become just a shell around the Windows functions, sigh.
 * But for far allocations, we can stick with our own allocator.
 */

/* Notice that we do not trust Windows functions to handle 0
 * or NULL input properly, because the behavior for those cases
 * is not documented.
 */

/* Defining __NEAR_FUNCTIONS produces near versions of the memory
 * allocation routines for use in large model.
 */

#if !(__SMALL__ || __MEDIUM__)

/*
	For Large & Compact memory models malloc,calloc,realloc
	all work off of the GLOBAL heap. You may make LocalAlloc
	calls on your own. There are many advantages to local memory
	under windows that would be lost if it was used in the C runtime.

	We stick with the regular ASM code for the storage allocator,
	here just providing a replacement for sbrk().
 */

/////////////////////////////////////////
// Request memory from operating system.
// Attempt to grow the data segment.
// Use:
//	p = sbrk(nbytes);
// Returns:
//	pointer to memory allocated
//	    (first word of allocated memory contains # of bytes allocated)
//	-1 if error


static unsigned short farsel;	// selector of last allocated block
static unsigned short farsize;	// size of last allocated block

// Keep a linked list of all global objects allocated, so we
// can free them all if we like.
static void *glbl_list;

void *sbrk(size_t nbytes)
{
    unsigned newsize;
    HGLOBAL farhandle;
    HGLOBAL newhandle;
    long x;
    void *newp;

    nbytes += sizeof(glbl_list);
    nbytes = (nbytes + 15) & ~0xF;	// round up to paragraph size
    if (nbytes > 16)
    {
	if (farsel == 0)		// if no segment to reallocate
	    goto sbrnew;
	if ((long) farsize + nbytes >= 0x10000)	// if >64Kb
	    goto sbrnew;
	newsize = farsize + nbytes;
	if (newsize > 65518)		// Windows Standard mode, magic limit
	      goto sbrnew;

	x = GlobalHandle(farsel);
	if (!x)
	    goto sbrk3;			// some error that shouldn't happen
	farhandle = (HGLOBAL) x;

	newhandle = GlobalReAlloc(farhandle,newsize,0);
	if (!newhandle)
	    goto sbrnew;
	if (newhandle != farhandle)
	{   // global realloc moved memory on us!
//	    MessageFatalError("Memory was moved unexpectedly.");
	    return (void *) -1L;
	}
	newp = MK_FP(farsel,farsize);
	goto sbrlok;

sbrnew:
	newhandle = GlobalAlloc(GMEM_MOVEABLE | _alloc_flags, nbytes);
	if (!newhandle)
		goto sbrk3;
	newp = GlobalLock(newhandle);
	if (!newp)
	    goto sbrk3;
	newsize = FP_OFF(newp) + nbytes;	// offset of next starting point

	// Prepend new block onto list of global blocks
	*(void **)newp = glbl_list;
	glbl_list = newp;
	((void **)newp)++;

sbrlok:
	farsel = FP_SEG(newp);
	farsize = newsize;
	*(unsigned *)newp = nbytes - sizeof(glbl_list);
	return newp;
    }
sbrk3:
    //errno = ENOMEM;		// fake an ENOMEM
    return (void *) -1L;
}

/////////////////////////////////////////
// Free all global blocks.
// Useful for DLLs.

void _sbrk_freeall()
{
    while (glbl_list)
    {	unsigned short sel;
	HGLOBAL h;

	sel = FP_SEG(glbl_list);
	glbl_list = *(void **)glbl_list;
	h = GlobalHandle(sel);
	GlobalUnlock(h);
	GlobalFree(h);
    }
}
#endif

