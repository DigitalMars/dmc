#if __SC__ || __RCC__
#pragma once
#endif

#ifndef _H_HEAP
#define _H_HEAP

#include <stdlib.h>

class RTLHeapBlock;

enum eRTLHeapStatus
{
    kRTLHeapFree = 0x98a1,
    kRTLHeapClaimed = 0x2d3f
};

// Note: Algorithm apparently requires packing
#pragma pack(1)

class RTLHeapBlockHeader
{
public:
    RTLHeapBlockHeader(size_t = 0);

    // Note: The Size field must be first because a zero size marks
    // the end of the block

    size_t Size;
    RTLHeapBlock *pBack;
    eRTLHeapStatus Status;
};

class RTLHeapBlockFree
{
public:
    RTLHeapBlock * pFreePrev;
    RTLHeapBlock *pFreeNext;
};

class RTLHeapBlock : public RTLHeapBlockHeader,public RTLHeapBlockFree
{
public:
    RTLHeapBlock();
    RTLHeapBlock(size_t, RTLHeapBlock &);
    int MergeForward();
    int MergeBackward();
    int Claim(size_t);
    int Reclaim(size_t, RTLHeapBlock &);
    void InsertAfter(RTLHeapBlock &);
    void Remove();
    RTLHeapBlock & GetNext();
    int IsFree();
    int IsClaimed();
    void AssertValid();
    void CheckGroup();
};
#pragma pack()

inline int RTLHeapBlock::IsFree()
{
    return Status == kRTLHeapFree;
}

inline int RTLHeapBlock::IsClaimed()
{
    return Status == kRTLHeapClaimed;
}


class RTLHeap
{
public:
    RTLHeap();

    void *Alloc(unsigned long cb);
    void *Realloc(void *, unsigned long cb);
    void Free(void *);
    unsigned long GetSize(void *);

    // Needs to be called by RTL startup (or Pool code)
    static void CreateMainHeap();

    // Called to access the main heap code
    static inline RTLHeap *GetMainHeap();

private:
    size_t FixSize(size_t);
    RTLHeapBlock *MoreCore(size_t nu);
    void AssertValid();

    long nFree;
    RTLHeapBlock Base;
    RTLHeapBlock *allocp;

    static RTLHeap *pMainHeap;
};

static inline RTLHeap *RTLHeap::GetMainHeap()
{
    return pMainHeap;
}

#endif
