#include "heap.h"


#include <limits.h>
#include <string.h>
#include <stdio.h>

#define FALSE 0
#define TRUE 1

// Debugging:
// DEBUG: basic checking
// HEAP_FILL: fill heap blocks (slow)
// CHECK_BLOCKS: check blocks (slow)
// CHECK_ALL: check heap after each alloc (very slow)

#define DebugMsg(a)
#define DebugMsg1(a, b)
#define DebugMsg2(a, b, c)

#if DEBUG
#include <assert.h>
#define CriticalMsg(a) _assert(a, __FILE__, __LINE__)
#define AssertMsg(a, b) if (a) ; else _assert(b, __FILE__, __LINE__)
#define AssertMsg2(a, b, c, d) if (a) ; else _assert(b, __FILE__, __LINE__)
#else
#define CriticalMsg(a)
#define AssertMsg(a, b)
#define AssertMsg2(a, b, c, d)
#endif

#if _WIN32
const kCoreSize = 0x10000;

#else
const kCoreSize = 0x8000;

#endif

inline void *operator new(size_t, void *p)
{
    return p;
}


const kUnitSize = sizeof(RTLHeapBlockHeader);

const kMinAllocSize = sizeof(RTLHeapBlock) + 16;


////////////////////////////////////////////////////////////
//
// Debug Routines
//

#ifdef CHECK_ALL

#pragma message "Heap checked after each operation (very slow)"

//
// Check the heap
//
void RTLHeap::AssertValid()
{
    Base.pFreeNext->CheckGroup();

    long nCycles = 0;

    register RTLHeapBlock *p = allocp;
    RTLHeapBlock *end = allocp;

    for (;;)
    {
        AssertMsg(nCycles <= nFree, "Heap free chain is invalid");

        p->AssertValid();
        AssertMsg(p->IsFree(), "Claimed block on free chain");

        // Advance to the next free block
        p = p->pFreeNext;

        // If we have looped around...
        if (p == end)
            break;

        nCycles++;
    }

    AssertMsg(nCycles == nFree, "Invalid free item count");
}

#else

inline void RTLHeap::AssertValid()
{
}

#endif

#ifdef CHECK_BLOCKS

#pragma message "Blocks will be checked (slow)"

//
// Check if the current block is valid
//
void RTLHeapBlock::AssertValid()
{
    AssertStr(Status == kRTLHeapFree || Status == kRTLHeapClaimed,
              cFmtStr("Invalid heap block %p %x", this, Status));

    if (pBack)
    {
        AssertMsg(pBack->Status == kRTLHeapFree || pBack->Status == kRTLHeapClaimed,
                  "Invalid backward heap block");
        AssertMsg(&pBack->GetNext() == this, "Invalid back pointer");
    }

    RTLHeapBlock & Forward = GetNext();
    if (Forward.Size)
    {
        AssertMsg(Forward.Status == kRTLHeapFree || Forward.Status == kRTLHeapClaimed,
                  "Invalid forward heap block");
        AssertMsg(Forward.pBack == this, "Invalid back pointer in next block");
    }
}

#else

inline void RTLHeapBlock::AssertValid()
{
}

#endif


RTLHeap::RTLHeap():nFree(0),allocp(&Base)
{
}

//
// Fix the size of the request block
//
size_t RTLHeap::FixSize(size_t Size)
{
    if (Size > UINT_MAX - kUnitSize)
    {
        CriticalMsg("Block too large");
        return 0;
    }

    Size += kUnitSize;

    // If we are asking for less than a heap block...
    if (Size < kMinAllocSize)
        // ...then give the guy at least a heap block's worth
        Size = kMinAllocSize;

    return kUnitSize * (1 + (Size - 1) / kUnitSize);
}

//
// Allocate a block from the heap
//
void *RTLHeap::Alloc(unsigned long Size)
{
    DebugMsg1("Allocating %d bytes", Size);

    AssertValid();

    Size = FixSize(Size);

#ifdef DEBUG
    long nCycles = 0;

#endif

    register RTLHeapBlock *p = allocp;
    RTLHeapBlock *end = allocp;

    // This loop is very time critical
    for (;;)
    {
        AssertMsg(nCycles <= nFree, "Heap free chain is invalid");

        p->AssertValid();
        AssertMsg(p->IsFree(), "Claimed block on free chain");

        DebugMsg2("....checking against %p of size %d", p, p->Size);

        // If the current block is suitable...
        if (p->Size >= Size)
        {
            DebugMsg("....block is suitable");
            break;
        }

        // Advance to the next free block
        p = p->pFreeNext;

        // If we have looped around...
        if (p == end)
        {
            DebugMsg("....getting more core");

            // ..then get more memory
            p = MoreCore(Size);

            if (!p)
            {
                CriticalMsg("Out of memory");
                return 0;
            }

            break;
        }

#ifdef DEBUG
        nCycles++;
#endif
    }

// Store the position to start searching next
// allocp = p->pFreeNext;

// If we have claimed the whole block...
    if (p->Claim(Size))
    {
        // ...then we have one less free item
        nFree--;
        AssertMsg(nFree >= 0, "Invalid free item count");
    }

    allocp->AssertValid();
    AssertMsg(allocp->IsFree(), "Claimed block on free chain");

    void *ap = (RTLHeapBlockFree *) p;

#ifdef HEAP_FILL
    memset(ap, 0xA0, Size - sizeof(RTLHeapBlockHeader));
#endif

    AssertValid();

    AssertMsg2(p->Size >= Size, "Stored size less than required size (%d, %d)", p->Size, Size);

    DebugMsg2("....allocated %d at %p", p->Size, p);
    return ap;
}


//
// Reallocate a block
//
void *RTLHeap::Realloc(void *ap, unsigned long Size)
{
    DebugMsg2("Realloc %p to %d", ap, Size);

    // If we have a NULL pointer...
    if (!ap)
    {
        DebugMsg("Null pointer for realloc");
        // ...then just do an allocation
        return Alloc(Size);
    }

    AssertValid();

    RTLHeapBlock *p = (RTLHeapBlock *) (RTLHeapBlockFree *) ap;
    size_t OldSize = p->Size - sizeof(RTLHeapBlockHeader);

    // If the current block is too small...
    if (OldSize < Size)
    {
        DebugMsg1("....current block too small %d", OldSize);

        size_t FixedSize = FixSize(Size);

        DebugMsg1("Fixed size = %d", FixedSize);

        // If we can merge with the next block...
        if (p->MergeForward())
        {
            DebugMsg1("....merged forward and now block is %d", p->Size);

            nFree--;            // We now have one less free item

            // If the block is now big enough...
            if (p->Size >= FixedSize)
            {
                DebugMsg("....using merged block");

                // If we didn't reclaim the whole block...
                if (!p->Reclaim(FixedSize, Base))
                {
                    // ...then we have a new free item
                    DebugMsg("....didn't use all of it");
                    nFree++;
                }

                AssertValid();

                DebugMsg1("....returing %p", p);

                return (RTLHeapBlockFree *) p;
            }
        }

        // Allocate a new block
        void *pNew = Alloc(Size);

        // Copy the data portion of the old block
        if (pNew)
            memcpy(pNew, ap, OldSize);

        // Now free the old block
        Free(ap);

        return pNew;
    }

    return ap;
}


//
// Allocate another chunk of memory
//
RTLHeapBlock *RTLHeap::MoreCore(size_t Size)
{
    if (Size < kCoreSize)
        Size = kCoreSize;
    else
	    Size += Size/5;  // Allocate 20% more to eliminate fragmentation

    char *cp = (char *)sbrk(Size + sizeof(Size));

    // If there is no more memory...
    if ((int) cp == -1)
        // ...then indicate this
        return 0;

    // Mark the end of the block with a zero size
    *(size_t *) (cp + Size) = 0;

    RTLHeapBlock *up = new((void *) cp) RTLHeapBlock(Size, Base);

    up->AssertValid();

    nFree++;

    return up;
}


//
// Free the given block
//
void RTLHeap::Free(void *ap)
{
    if (!ap)
        return;

    AssertValid();

    RTLHeapBlock *p = (RTLHeapBlock *) (RTLHeapBlockFree *) ap;

    p->AssertValid();
    AssertMsg(p->IsClaimed(), "Block already free");

    p->Status = kRTLHeapFree;

#ifdef HEAP_FILL
    memset(ap, 0xF0, p->Size - sizeof(RTLHeapBlockHeader));
#endif

    p->AssertValid();

    p->InsertAfter(Base);
    nFree++;

    if (p->MergeForward())
    {
        nFree--;
        AssertMsg(nFree >= 0, "Invalid free item count");
    }

    if (p->MergeBackward())
    {
        nFree--;
        AssertMsg(nFree >= 0, "Invalid free item count");
    }

    // allocp = &Base;

    AssertValid();
}


////////////////////////////////////////////////////////////
//
// CLASS: RTLHeapBlockHeader
//
RTLHeapBlockHeader::RTLHeapBlockHeader(size_t Size):pBack(0),Size(Size),Status(kRTLHeapFree)
{
}


////////////////////////////////////////////////////////////
//
// CLASS: RTLHeapBlock
//

RTLHeapBlock::RTLHeapBlock()
{
    pFreeNext = this;
    pFreePrev = this;
}

RTLHeapBlock::RTLHeapBlock(size_t Size, RTLHeapBlock & Block)
              :RTLHeapBlockHeader(Size)
{
    InsertAfter(Block);
}


//
// Insert this block after the given block on the free list
//
void RTLHeapBlock::InsertAfter(RTLHeapBlock & Block)
{
    pFreeNext = Block.pFreeNext;
    pFreePrev = &Block;
    Block.pFreeNext = this;
    pFreeNext->pFreePrev = this;
}


//
// Merge this block with the previous block if possible
//
int RTLHeapBlock::MergeBackward()
{
    if (!pBack || pBack->IsClaimed())
        return FALSE;

    RTLHeapBlock *p = pBack;

    p->AssertValid();

    RTLHeapBlock & Next = GetNext();

#ifdef DEBUG
    if (Next.Size)
        Next.AssertValid();
#endif

    p->Size += Size;

    if (Next.Size)
    {
        Next.pBack = p;
        Next.AssertValid();
    }

    Remove();

#ifdef HEAP_FILL
// Blast out this block
    memset(this, 0xAB, Size);
#endif

    p->AssertValid();

    return TRUE;
}


//
// Merge this block with the next block if possible
//
int RTLHeapBlock::MergeForward()
{
    RTLHeapBlock & Next = GetNext();

    if (!Next.Size || Next.IsClaimed())
        return FALSE;

    Next.AssertValid();

    RTLHeapBlock & NextNext = Next.GetNext();

#ifdef DEBUG
    if (NextNext.Size)
        NextNext.AssertValid();
#endif

    Size += Next.Size;

    if (NextNext.Size)
    {
        NextNext.pBack = this;
        NextNext.AssertValid();
    }

    Next.Remove();

#ifdef HEAP_FILL
// Blast out the forward block
    memset(&Next, 0xAF, Next.Size);
#endif

    AssertValid();

    return TRUE;
}


//
// Get the next sorted block after this block
//
RTLHeapBlock & RTLHeapBlock::GetNext()
{
    return *(RTLHeapBlock *) ((char *) this + Size);
}


//
// Remove this block from the free chain
//
void RTLHeapBlock::Remove()
{
    AssertMsg(pFreeNext != this, "Can't delete last free block");
    AssertMsg(pFreePrev != this, "Invalid links detect during remove");
    AssertMsg(IsFree(), "Can't remove a block which isn't on the free chain");
    pFreeNext->pFreePrev = pFreePrev;
    pFreePrev->pFreeNext = pFreeNext;
}


//
// Claim the given number of bytes of this block
//
int RTLHeapBlock::Claim(size_t Needed)
{
    AssertMsg(Needed <= Size, "Block too small to claim");
    AssertValid();
    AssertMsg(IsFree(), "Can't claim a claimed block");

    RTLHeapBlock & FreePrev = *pFreePrev;
    pFreePrev->AssertValid();
    AssertMsg(pFreePrev->IsFree(), "Claimed block on free chain");

    Remove();
    Status = kRTLHeapClaimed;

    return Reclaim(Needed, FreePrev);
}


//
// Reclaim the given number of bytes of this block
//
int RTLHeapBlock::Reclaim(size_t Needed, RTLHeapBlock & Block)
{
    AssertMsg(Needed <= Size, "Block too small to reclaim");
    AssertValid();
    AssertMsg(IsClaimed(), "Can't reclaim a freeded block");

    // Compute what our new size would be
    size_t NewSize = Size - Needed;

    if (NewSize <= kMinAllocSize)
        return TRUE;

    Size = Needed;

    // Create a new block at the tail of this block
    RTLHeapBlock & NewBlock = GetNext();

#ifdef HEAP_FILL
    memset((char *) &NewBlock, 0xDE, NewSize);
#endif

    NewBlock.pBack = this;
    NewBlock.Size = NewSize;
    NewBlock.Status = kRTLHeapFree;
    NewBlock.InsertAfter(Block);

    // Fix the back pointer of the block after the new block
    RTLHeapBlock & NextBlock = NewBlock.GetNext();
    if (NextBlock.Size)
    {
        NextBlock.pBack = &NewBlock;
        NextBlock.AssertValid();
    }

    NewBlock.AssertValid();

    AssertValid();

    return FALSE;
}


//
// Validate the group of blocks associated with this block
//
void RTLHeapBlock::CheckGroup()
{   RTLHeapBlock *p;

    AssertValid();

    for (p = pBack; p; p = p->pBack)
        p->AssertValid();

    for (p = &this->GetNext(); p->Size; p = &p->GetNext())
        p->AssertValid();
}


//
//  Get the usable size of a block.
//
unsigned long RTLHeap::GetSize(void *ap)
{
    DebugMsg1("GetSize %p", ap);

    // If we have a NULL pointer...
    if (!ap)
    {
        DebugMsg("Null pointer for GetSize");
        return -1;
    }

    AssertValid();

    RTLHeapBlock *p = (RTLHeapBlock *) (RTLHeapBlockFree *) ap;
    size_t Size = p->Size - sizeof(RTLHeapBlockHeader);

    DebugMsg2("... GetSize %p is %lu", ap, Size);
    return Size;
}


void RTLHeap::CreateMainHeap()
{
    static RTLHeap MainHeap;

    pMainHeap = &MainHeap;
}

RTLHeap *RTLHeap::pMainHeap = 0;
