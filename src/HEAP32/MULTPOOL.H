#if __SC__ || __RCC__
#pragma once
#endif

#ifndef _H_MULTPOOL
#define _H_MULTPOOL

#include <stdlib.h>

class RTLPool;

class RTLMultiPool
{
public:
    RTLMultiPool(unsigned int = 16, unsigned = 0); // default pools 8 - 128

    ~RTLMultiPool();

    void *Alloc(unsigned long cb);
    void *Realloc(void *, unsigned long cb);
    void Free(void *);
    unsigned long GetSize(void *);               // get usable size of the block

    // Needs to be called by RTL startup
    static void CreateMainHeap();

    // Called to access the main heap code
    static inline RTLMultiPool *GetMainHeap();

private:
    void *PoolAlloc(unsigned long);
    void *AllocatorAlloc(unsigned long);
    void PoolFree(void *);
    void AllocatorFree(void *);
    void *SelectAlloc(size_t);
    void SelectFree(void *);

    RTLPool **pPoolArray;

    unsigned long nMaxPool;
    unsigned int nElements;
    unsigned nId;

    static RTLMultiPool *pMainHeap;

#if DEBUG_STATS
    unsigned long _heap_peak;
    unsigned long _heap_outstanding;
    unsigned long _pool_peak;
    unsigned long _pool_outstanding;
    unsigned long *pPoolArrayNum;                // For debugging/optimizing
#endif
};

static inline RTLMultiPool *RTLMultiPool::GetMainHeap()
{
    return pMainHeap;
}

#endif

