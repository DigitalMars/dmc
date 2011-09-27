
#include "multpool.h"
#include "heap.h"
#include "pool.h"

#include "string.h"

#ifndef DEBUG_HEAP
#define DEBUG_HEAP 0
#endif

#if DEBUG_STATS
#pragma message "DEBUG_STATS enabled"
#include "stdio.h"
#endif

#define DebugMsgEx(a, b);
#define DebugMsgEx1(a, b, c);

#if DEBUG
#include <assert.h>
#define AssertMsg(a, b) if (a) ; else _assert(b, __FILE__, __LINE__)
#else
#define AssertMsg(a, b)
#endif

typedef size_t tMPAllocHeader;
const size_t nHeaderSize = sizeof(tMPAllocHeader);

//
// Compute an index from an uncompensated size
//
inline size_t SizeFromIndex(int index)
{
    return (index + 1) << 3;
}

//
// Compute an uncompensated size from an index
//
inline size_t IndexFromSize(size_t size_val)
{
    return (size_val - 1) >> 3;
}


//
// Compute the maximum uncompensated size available  given an uncompensated size
//
inline size_t MaxFit(size_t size_val)
{
    return SizeFromIndex(IndexFromSize(size_val));
}


//
// Free a block from the pool
//
inline void RTLMultiPool::PoolFree(void *pMemInfo)
{
    DebugMsgEx(HEAP, "MultiPool Freeing from Pool");
    unsigned int ix = IndexFromSize(*reinterpret_cast < tMPAllocHeader * >(pMemInfo));

#if DEBUG_STATS
    _pool_outstanding -= *reinterpret_cast < tMPAllocHeader * >(pMemInfo) + nHeaderSize;
    pPoolArrayNum[ix]--;
#endif

    pPoolArray[ix]->Free(pMemInfo);
}



inline void RTLMultiPool::AllocatorFree(void *pMemInfo)
{
#if DEBUG_STATS
    _heap_outstanding -= *reinterpret_cast < tMPAllocHeader * >(pMemInfo) + nHeaderSize;
#endif

    DebugMsgEx(HEAP, "MultiPool Freeing from Heap");
    RTLHeap::GetMainHeap()->Free(pMemInfo);
}



inline void *RTLMultiPool::PoolAlloc(unsigned long cb)
{
    unsigned int ix = IndexFromSize(cb);

    DebugMsgEx(HEAP, "MultiPool Allocating from Pool");

#if DEBUG_STATS
    pPoolArrayNum[ix]++;
    _pool_outstanding += (cb + nHeaderSize);
    _pool_peak = _pool_outstanding > _pool_peak ? _pool_outstanding : _pool_peak;
#endif

    return pPoolArray[ix]->Alloc();
}


inline void *RTLMultiPool::AllocatorAlloc(unsigned long cb)
{
#if DEBUG_STATS
    _heap_outstanding += (cb + nHeaderSize);
    _heap_peak = _heap_outstanding > _heap_peak ? _heap_outstanding : _heap_peak;
#endif

    DebugMsgEx(HEAP, "MultiPool Allocating from Heap");
    return RTLHeap::GetMainHeap()->Alloc(cb + nHeaderSize);
}


//
// Choose the correct allocator
//
inline void *RTLMultiPool::SelectAlloc(size_t size_val)
{
    return size_val <= nMaxPool ? PoolAlloc(size_val) : AllocatorAlloc(size_val);
}


//
// Choose the correct deallocator
//
inline void RTLMultiPool::SelectFree(void *MemPtr)
{
    if (*reinterpret_cast < tMPAllocHeader * >(MemPtr) <= nMaxPool)
        PoolFree(MemPtr);
    else
        AllocatorFree(MemPtr);
}


//
// Allocate something at the specified location
//
inline void *operator new(size_t, RTLPool *p)
{
    return (void *)p;
}


//
// Allocate something at the specified location
//
inline void *operator new(size_t, RTLPool **p)
{
    return (void *)p;
}

#ifdef _ENABLE_ARRAYNEW
inline void *operator new[](size_t, RTLPool **p)
{
    return (void *)p;
}
#endif


RTLMultiPool::RTLMultiPool(unsigned int nelements, unsigned id)
    : nElements(nelements),
      nId(id),
      pPoolArray(0)
#if DEBUG_STATS
      ,
      pPoolArrayNum(0),
      _heap_peak(0),
      _heap_outstanding(0),
      _pool_peak(0),
      _pool_outstanding(0)
#endif
{
    if (nElements)
    {
#if DEBUG_STATS
        pPoolArrayNum = RTLHeap::GetMainHeap()->Alloc(sizeof(unsigned long) * nElements);
#endif

        pPoolArray = (RTLPool **) RTLHeap::GetMainHeap()->Alloc(sizeof(RTLPool *) * nElements);
        AssertMsg(pPoolArray, "Failed to allocate MultiPool");
        new(pPoolArray) (RTLPool *)[nElements];

        nMaxPool = nElements << 3;  // *8

        size_t elemsize;

        for (unsigned int i = 1; i <= nElements; i++)
        {
            elemsize = i << 3;  // *8

            pPoolArray[i - 1] = (RTLPool*) RTLHeap::GetMainHeap()->Alloc(sizeof(RTLPool));
            AssertMsg(pPoolArray[i - 1], "Failed to allocate pool");

            new(pPoolArray[i - 1]) RTLPool(elemsize + nHeaderSize);

#if DEBUG_STATS
            pPoolArrayNum[i - 1] = 0;
#endif

        }
        DebugMsgEx(HEAP, "Allocated MultiPool");
    }
}


//
// Debugging destrutor: Let system free memory at app terminate.
//
RTLMultiPool::~RTLMultiPool()
{
#if DEBUG_STATS
    DebugMsg1("*** Dumping Pool info for MultiPool id: %u ***", nId);
    DebugMsg2("       Direct Heap: outstanding=%ldk  peak=%ldk\n", _heap_outstanding / 1024, _heap_peak / 1024);
    DebugMsg2("    Multipool heap: outstanding=%ldk  peak=%ldk\n", _pool_outstanding / 1024, _pool_peak / 1024);

    for (int i = 0; i < nElements; i++)
    {
        DebugMsg6("    PoolArray[%d] (%d bytes)  = %ld items outstanding, %ld Blocks (%ld bytes each, %ldk total)",
                  i,
                  (i + 1) * 8,
                  pPoolArrayNum[i],
                  pPoolArray[i]->GetBlockNum(),
                  pPoolArray[i]->GetBlockSize(),
        pPoolArray[i]->GetBlockNum() * (pPoolArray[i]->GetBlockSize()) / 1024);

        unsigned long diff = pPoolArray[i]->GetTakes() - pPoolArray[i]->GetFrees();

        DebugMsg4("            %ld takes - %ld frees = %ld (%ldk) unfreed",
                  pPoolArray[i]->GetTakes(),
                  pPoolArray[i]->GetFrees(),
                  diff, ((i + 1) * 8 * diff) / 1024);

        DebugMsg2("            %ld maximum takes (%ldk)",
                  pPoolArray[i]->GetMaxTakes(),
                  (pPoolArray[i]->GetMaxTakes() * (i + 1) * 8) / 1024);
    }
#endif
}


//
// External API to alloc memoery
//
void *RTLMultiPool::Alloc(unsigned long cb)
{
    if (!cb)
        cb = 1;  // For VC compatibility, allow 0 sized allocations.

    void *pMem = SelectAlloc(cb);

    if (!pMem)
        return 0;

    // now store size, and return the actual requested memory
    *reinterpret_cast < tMPAllocHeader * >(pMem) = cb;
    return (void *) ((reinterpret_cast < tMPAllocHeader * >(pMem)) + 1);
}



void *RTLMultiPool::Realloc(void *pMem, unsigned long cb)
{
    DebugMsgEx(HEAP, "MultiPool reallocating");

    if (!pMem)
        return RTLMultiPool::Alloc(cb);

    if (!cb)
    {
        RTLMultiPool::Free(pMem);
        return 0;
    }

    tMPAllocHeader *pMemInfo = reinterpret_cast < tMPAllocHeader * >(pMem) - 1;

    // If we are shrinking...
    if (cb <= *pMemInfo)
        // ...then don't do anything
        return pMem;

    // Growing...

    // If we are in the pool...
    if (*pMemInfo <= nMaxPool)
    {
        // ...and we still want to remain within the same pool
        if (cb <= MaxFit(*pMemInfo))
        {
            // ...then just change the size
            *pMemInfo = cb;
            return pMem;
        }

        // Just alloc, copy, and free.
        void *pNewMem = SelectAlloc(cb);

        if (pNewMem)
        {
            // Set the size (warning: conversion from unsigned long to
            // tMPAllocHeader)
            *reinterpret_cast < tMPAllocHeader * >(pNewMem) = cb;

            memcpy(reinterpret_cast < tMPAllocHeader * >(pNewMem) + 1, pMem, *pMemInfo);
            SelectFree(pMemInfo);
            return reinterpret_cast < tMPAllocHeader * >(pNewMem) + 1;
        }
    }

    // ...else we are not in a pool and we want to get bigger...
    else
    {
        // Let the subordinate realloc do the work
        void *pNewMem = RTLHeap::GetMainHeap()->Realloc(pMemInfo, cb + nHeaderSize);

        if (pNewMem)
        {
            // Set the size (warning: conversion from unsigned long to
            // tMPAllocHeader)
            *reinterpret_cast < tMPAllocHeader * >(pNewMem) = cb;
            return reinterpret_cast < tMPAllocHeader * >(pNewMem) + 1;
        }
    }

    return 0;
}


//
// External interface for freeing memory
//
void RTLMultiPool::Free(void *pMem)
{
    if (!pMem)
        return;

    tMPAllocHeader *pMemInfo = (reinterpret_cast < tMPAllocHeader * >(pMem)) - 1;

    SelectFree(pMemInfo);
}


//
//  Get the usable size of a block.
//
unsigned long RTLMultiPool::GetSize(void *pMem)
{
    DebugMsgEx1(HEAP, "RTLMultiPool::GetSize %p", pMem);

    tMPAllocHeader *pMemInfo = reinterpret_cast < tMPAllocHeader * >(pMem) - 1;

    if (!pMem)
        return 1;

    DebugMsgEx1(HEAP, "...Size is %lu", *pMemInfo);
    return (*pMemInfo);
}


void RTLMultiPool::CreateMainHeap()
{
    // Make sure the secondary heap is created
    RTLHeap::CreateMainHeap();

    static RTLMultiPool MainHeap;

    pMainHeap = &MainHeap;
}


RTLMultiPool *RTLMultiPool::pMainHeap = 0;
