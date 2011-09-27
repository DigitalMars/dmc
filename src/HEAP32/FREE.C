/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */

#include <windows.h>
#include <stdlib.h>
#include "semlock.h"
#include "multpool.h"

#if __cplusplus
extern "C" {
#endif
void free(void *block)
{
    LockSemaphore(_semAlloc);
    RTLMultiPool::GetMainHeap()->Free(block);
    UnlockSemaphore(_semAlloc);
}

#if __cplusplus
}
#endif
