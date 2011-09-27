/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */

#include <stdlib.h>
#include "semlock.h"
#include "multpool.h"

#if __cplusplus
extern "C" {
#endif

void *malloc (size_t m_size)
{
    void *p;
    
    LockSemaphore(_semAlloc);
    p = RTLMultiPool::GetMainHeap()->Alloc(m_size);
    UnlockSemaphore(_semAlloc);
    
    return p;
}

#if __cplusplus
}
#endif

