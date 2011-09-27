/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */

#include <stdlib.h>
#include <string.h>
#include "semlock.h"
#include "multpool.h"

#if __cplusplus
extern "C" {
#endif    

void *realloc (void *oldblock, size_t newsize)
{
    void *p;
    
    LockSemaphore(_semAlloc);
    p = RTLMultiPool::GetMainHeap()->Realloc(oldblock,newsize);
    UnlockSemaphore(_semAlloc);
    return p;
}

#if __cplusplus
}
#endif

