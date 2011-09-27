/*_ sbrk.c   Modified by: Walter Bright
 * Copyright (C) 1991 by Digital Mars
 * All rights reserved.
 * Written by Walter Bright
 */

#include	<stdlib.h>

#ifdef __OS2__
#include "os2lib.h"
#endif
#ifdef __NT__
#include <windows.h>
#endif


/*********************************
 * Allocate a chunk of memory from the operating system.
 * Note: This routine must be _cdecl as the nbytes paramter
 *	 is returned on the stack to the caller (!)
 */
 
#define GLOBAL_BATCH_SIZE 1024

typedef struct _GlobalBatch
{
   struct _GlobalBatch *previous_batch;
   int handle_index;
   void *handle[GLOBAL_BATCH_SIZE];
} GlobalBatch;

#if __NT__
#define GLOBAL_ALLOC(ptr,size) (ptr) = VirtualAlloc (NULL, size, MEM_COMMIT, PAGE_READWRITE)
#define GLOBAL_FREE(ptr) VirtualFree ((void *)ptr, 0, MEM_RELEASE)
#else
#define GLOBAL_ALLOC(ptr,size) if (DosAllocMem(&(ptr),size,0x57)) ptr = NULL
#define GLOBAL_FREE(ptr) DosFreeMem(ptr)
#endif
 
static GlobalBatch *global_list = NULL; 

void * _cdecl sbrk(size_t nbytes)
{
    void *p;
    GlobalBatch *new_batch;

    /*
       Caller should not allocate small sizes
     */
    GLOBAL_ALLOC (p, nbytes);
    if (p == NULL) return (void *)-1;
    
    if (global_list == NULL || 
        global_list->handle_index >= GLOBAL_BATCH_SIZE)
    {
       GLOBAL_ALLOC (new_batch, sizeof(GlobalBatch));
       if (new_batch == NULL)
       {
          GLOBAL_FREE (p);
          return (void *)-1;
       }
       new_batch->previous_batch = global_list;
       global_list = new_batch;
       global_list->handle_index = 0;
    }
    global_list->handle[global_list->handle_index] = p;
    global_list->handle_index ++;
    return p;
}

void _cdecl _sbrk_freeall()
{
    GlobalBatch *next_batch;
    int index;

    while (global_list)
    {
       for (index = 0; index < global_list->handle_index; index++)
       {
          GLOBAL_FREE (global_list->handle[index]);
       }
       next_batch = global_list->previous_batch;
       GLOBAL_FREE (global_list);
       global_list = next_batch;
    }   
}
