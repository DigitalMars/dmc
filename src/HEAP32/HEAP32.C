/*_ heap.c
 * Copyright (C) 1993 by Digital Mars
 * All Rights Reserved
 * Written by Walter Bright
 * $Revision: 1.1.1.1 $ 
 */
 
#include <stdlib.h>
#include <mem.h>

/********************************
 * Check heap for any errors.
 */

int _heapchk()
{
/*    _HEAPEMPTY
      _HEAPBADBEGIN
      _HEAPOK
      _HEAPBADNODE */
      
    return _HEAPOK;
}

/********************************
 * Set all free blocks to value.
 * We recommend using 0xF4 as the fill value, because this
 * is the 8088 HLT instruction.
 */

int _heapset(unsigned value)
{
    //TODO:  Walk free list, setting all blocks to value passed in.
    return _HEAPOK;      
}

/***********************************
 * Add block of memory to the heap.
 */

//int _heapadd(void *buf,size_t nbytes)

/************************************
 * Release unused memory back to operating system.
 */

//int _heapmin()

/************************************
 * Expand the size of an allocated block without moving it.
 * Difference from MSC: you cannot _expand an already free'd block.
 */

//void *_expand(void *p,size_t newsize)
