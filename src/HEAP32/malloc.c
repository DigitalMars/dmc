/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */

#include <stdlib.h>
#include "semlock.h"
#include "multpool.h"

#if __cplusplus
extern "C" {
#endif

void *malloc (size_t m_size)
{
#if USEHEAP
    /* The +2 is because there's a buffer overflow somewhere in stlport.
     * It is triggered by stltutorial\ex13-01.cpp
     */
#if _WIN32
    /* Set a max size because of access violations in Win32 Heap functions.
     * Size is determined experimentally.
     */
    if (m_size < 0xD5550000)
	return HeapAlloc(_default_heap, 0, m_size + 2);
    return NULL;
#else
    return HeapAlloc(_default_heap, 0, m_size + 2);
#endif
#else
    void *p;
    
    LockSemaphore(_semAlloc);
    p = RTLMultiPool::GetMainHeap()->Alloc(m_size);
    UnlockSemaphore(_semAlloc);
    
    return p;
#endif
}

#if __cplusplus
}
#endif

