#if __SC__ || __RCC__
#pragma once
#endif

#ifndef _POOL_H
#define _POOL_H

#include <malloc.h>
#include "heap.h"

//
// class RTLPool
//
// Pooled Allocator to be used by operator new, operator delete overrides
// RTLPool will keep a freelist of items of the same size.
// When the freelist is empty it will alloc them in multiple blocks
//
// Instances of RTLPool *must* be staticly allocated,
// typically as static instance variables of the client class,
// as there is no destructor and thus there would be a memory leak if NOT static.
//

struct sRTLPoolBlock;

class RTLPool
{
public:
    // How Big will each item be, and HowMany per alloc()'d Bucket
    RTLPool(size_t ElemSize, unsigned HowMany = 0);

    void *Alloc(size_t);
    void *Alloc();

    void Free(void *);
    void Free(void *, size_t);

    void DumpAllocs();

    static void DumpPools();

    // static void ShrinkPoolsIter(uint msec) for IdleLoop ?

#if TRACK_ALLOCS
    // For debugging/optimization:
    unsigned long GetBlockNum();
    unsigned long GetTakes();
    unsigned long GetBlockSize();
    unsigned long GetFrees();
    unsigned long GetMaxTakes();
#endif

private:
    int fMaxedOut;
    void ThreadNewBlock();

    sRTLPoolBlock *pFreeList;
    size_t nElementSize;
    unsigned nBlockingFactor;
    size_t nBlockSize;

    RTLPool *pNextPool;

#if TRACK_ALLOCS
    unsigned long nBlocks;
    unsigned long nInUse;
    unsigned long nAllocs;
    unsigned long nFrees;
    unsigned long nMaxTakes;
    sRTLPoolBlock *pAllocList;
#endif

    static RTLPool *pPools;
};


#if TRACK_ALLOCS
inline unsigned long RTLPool::GetBlockNum()
{
    return nBlocks;
}

inline unsigned long RTLPool::GetBlockSize()
{
    return nBlockSize;
}

inline unsigned long RTLPool::GetTakes()
{
    return nAllocs;
}

inline unsigned long RTLPool::GetFrees()
{
    return nFrees;
}

inline unsigned long RTLPool::GetMaxTakes()
{
    return nMaxTakes;
}
#endif


inline void *RTLPool::Alloc(size_t HowBig)
{
    return HowBig == nElementSize ? Alloc() : RTLHeap::GetMainHeap()->Alloc(HowBig);
}


inline void RTLPool::Free(void *p, size_t HowBig)
{
    if (HowBig == nElementSize || HowBig == 0)
        Free(p);
    else
        RTLHeap::GetMainHeap()->Free(p);
}
#endif
