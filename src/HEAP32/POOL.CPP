#include "rtlheap.h"
#include "pool.h"

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// DEBUG_USE_AFTER_FREE:
// Turning this on disables free and clobbers memory.  This is useful for
// making sure nothing is using freed memory.

extern "C"
{
// Creates a pool for allocating fixed-size items.
HRTLPOOL RTLPoolCreate(size_t Size)
    {
    return reinterpret_cast<HRTLPOOL>(new RTLPool(Size));
    }

void *RTLPoolAlloc(HRTLPOOL hPool)
    {
    return reinterpret_cast<RTLPool *>(hPool)->Alloc();
    }

void RTLPoolFree(HRTLPOOL hPool, void *pData)
    {
    reinterpret_cast<RTLPool *>(hPool)->Free(pData);
    }

void RTLPoolDestroy(HRTLPOOL hPool)
    {
    delete reinterpret_cast<RTLPool *>(hPool);
    }
}

#define FALSE 0
#define TRUE 1

#define DebugMsg(a)
#define DebugMsgEx1(a, b, c);
#define DebugMsgEx2(a, b, c, d);

#if DEBUG
#define CriticalMsg(a) _assert(a, __FILE__, __LINE__)
#define AssertMsg(a, b) if (a) ; else _assert(b, __FILE__, __LINE__)
#else
#define CriticalMsg(a)
#define AssertMsg(a, b)
#endif

struct sRTLPoolBlock;

struct sRTLFreePoolPart
{
    sRTLPoolBlock *pNextFree;
};


#if TRACK_ALLOCS

const nStackLevels = 8;


#if !__NT__
#include <windows.h>
#include <toolhelp.h>
typedef WORD MOFFSET;
#else
#include <tchar.h>
#include <windows.h>
typedef DWORD MOFFSET;
typedef struct tagSTACKTRACEENTRY
{
    WORD dwSize;
    WORD wSS;
    MOFFSET wBP;
    WORD wCS;
    MOFFSET wIP;
    HMODULE hModule;
    WORD wSegment;
    WORD wFlags;
} STACKTRACEENTRY;

#define MAX_MODULE_NAME MAX_PATH
extern "C" {
extern HINSTANCE __module_handle;
}

typedef struct tagMODULEENTRY
{
    DWORD dwSize;
    char szModule[MAX_MODULE_NAME + 1];
    HMODULE hModule;
    WORD wcUsage;
    char szExePath[MAX_PATH + 1];
    WORD wNext;
} MODULEENTRY;

static BOOL StackTraceCSIPFirst(STACKTRACEENTRY *lpStackTrace,
            WORD wSS, WORD wCS, MOFFSET wIP, MOFFSET wBP)

{
    lpStackTrace->wSS = wSS;
    lpStackTrace->wCS = wCS;
    lpStackTrace->wIP = wIP;
    lpStackTrace->wBP = wBP;
    lpStackTrace->hModule = 0;
    lpStackTrace->wSegment = 0;
    lpStackTrace->wFlags = 0;
    return TRUE;
}

static BOOL StackTraceNext(STACKTRACEENTRY *lpStackTrace)

{
    MOFFSET *pBP = (MOFFSET *)lpStackTrace->wBP;

    if (!pBP || IsBadReadPtr(pBP, 4))
        return FALSE;

    lpStackTrace->wBP = pBP[0];
    lpStackTrace->wIP = pBP[1];
    lpStackTrace->hModule = 0;
    lpStackTrace->wSegment = 0;
    lpStackTrace->wFlags = 0;

    return TRUE;
}

#endif

#if TRACK_ALLOCS
static int FillStackArray(int, int, void **);
#endif


static int FillStackArray(int Skip, int MaxFrames, void **p)
{
    STACKTRACEENTRY ste;
    ste.dwSize = sizeof(STACKTRACEENTRY);

    // Get the CS, SS, IP, and BP
    WORD wCS;
    WORD wSS;
    MOFFSET wIP;
    MOFFSET wBP;
    MOFFSET wBX;

    _asm
    {
    #if __NT__
        mov wBX, ebx          ; Save BX
        mov wCS, cs
        mov wSS, ss

        mov ebx, ebp
        mov eax, ss:[ebx]     ; Get BP for caller
        mov wBP, eax

        mov eax, ss:[ebx+4]   ; Get return address for caller
        mov wIP, eax

        mov ebx, wBX         ; Restore BX
    #else
        mov wBX, bx          ; Save BX

        mov wCS, cs
        mov wSS, ss

        mov bx, bp
        mov ax, ss:[bx]     ; Get BP for caller
        mov wBP, ax

        mov ax, ss:[bx+2]   ; Get return address for near caller
        mov wIP, ax

        mov bx, wBX         ; Restore BX
    #endif
    }

    if (!StackTraceCSIPFirst(&ste, wSS, wCS, wIP, wBP))
        return 0;

    for (int j = 0; j < Skip; j++)
    {
        if (!StackTraceNext(&ste))
            return 0;
    }

    for (int i = 0; i < MaxFrames; i++)
    {
    #if __NT__
        *p++ = reinterpret_cast<void *>(ste.wIP);
    #else
        *p++ = reinterpret_cast<void *>((static_cast<long>(ste.wSegment) << 16) + ste.wIP);
    #endif
        if (!StackTraceNext(&ste))
            break;
    }

    return i;
}

static void ::LogMsg(const char *format, ...)
{
    static FILE *pFile = 0;

    if (!format)
    {
        if (pFile)
            fflush(pFile);
        return;
    }

    if (!pFile)
    {
        char szBuff[MAX_PATH];
        ::GetModuleFileName(__module_handle, szBuff, MAX_PATH-1);
        int n = strlen(szBuff);
        strcpy(&szBuff[n-3], "alc");
        pFile = fopen(szBuff, "w");
    }

    static char buffer[2048];
    va_list arg_ptr;
    va_start(arg_ptr, format);
    int n = _vsnprintf(buffer, 2047, format, arg_ptr);
    fprintf(pFile, "%s\n", buffer);
    va_end(arg_ptr);
}
#endif


#if TRACK_ALLOCS
struct sRTLAllocPoolPart
{
    void *Stack[nStackLevels];
    sRTLPoolBlock *pNextAlloc;
    sRTLPoolBlock *pPrevAlloc;
};

struct sRTLPoolBlock : sRTLAllocPoolPart,sRTLFreePoolPart
#else
struct sRTLPoolBlock : sRTLFreePoolPart
#endif
{
};

#if DUMP_POOLS
struct sRTLPoolDumper
{
    ~sRTLPoolDumper();
};

sRTLPoolDumper::~sRTLPoolDumper()
{
    RTLPool::DumpPools();
    ::LogMsg(0); // Flush
}

sRTLPoolDumper PoolDumper;

#endif

RTLPool *RTLPool::pPools(0);


//
// Constructor
//
RTLPool::RTLPool(size_t ElemSize, unsigned HowMany)
    : fMaxedOut(FALSE),
      nElementSize(ElemSize),
      pFreeList(0)

#if TRACK_ALLOCS
      // Debug Support:
      ,
      nBlocks(0),
      nInUse(0),
      nAllocs(0),
      nFrees(0),
      nMaxTakes(0),
      pAllocList(0)
#endif
{
    if (nElementSize < sizeof(sRTLFreePoolPart))
        nElementSize = sizeof(sRTLFreePoolPart);

#if TRACK_ALLOCS
    unsigned long RealElemSize = nElementSize + sizeof(sRTLAllocPoolPart);

#else
    unsigned long RealElemSize = nElementSize;

#endif

    if (HowMany)
    {
        unsigned long bigSize = RealElemSize * HowMany;

        nBlockSize = bigSize;

        // Check that both the multiple doesn't overflow the unsigned long
        // and that the copy from unsigned long to size_t doesn't lose:
        AssertMsg(HowMany == bigSize / RealElemSize && bigSize == nBlockSize, "Block size to big");

        nBlockingFactor = HowMany;
    }

    // Allow blocking to default
    else
    {
        nBlockingFactor = 4;
        nBlockSize = 4 * RealElemSize;
    }

    pNextPool = pPools;
    pPools = this;
}


//
// Refill an empty freelist
//
void RTLPool::ThreadNewBlock()
{
    static const size_t kMaxBlockSize = 0x8000; // 32k

    DebugMsgEx1(HEAP, "Getting new block of %d", nBlockSize);

    AssertMsg(!pFreeList, "ThreadNew called when not empty");

    // First get a new batch ...
    pFreeList = reinterpret_cast < sRTLPoolBlock * >(RTLHeap::GetMainHeap()->Alloc(nBlockSize));

    if (!pFreeList)
        return;

#if TRACK_ALLOCS
    unsigned long RealElemSize = nElementSize + sizeof(sRTLAllocPoolPart);

#else
    unsigned long RealElemSize = nElementSize;

#endif

    DebugMsgEx2(HEAP, "Threading New block: BlockSize = %u, ElemSize = %u", nBlockSize, RealElemSize);

#if DEBUG
    nBlocks++;
#endif

    // ... Then start threading it, starting with the last element ...
    sRTLPoolBlock *p = reinterpret_cast < sRTLPoolBlock * >(
                                                            reinterpret_cast < char *>(pFreeList) + (nBlockingFactor - 1) * RealElemSize);

    sRTLPoolBlock *pPrev = 0;

    for (;;)
    {
        p->pNextFree = pPrev;

        // ... and work back to the first ...
        if (p == pFreeList)
            break;

        pPrev = p;
        p = reinterpret_cast < sRTLPoolBlock * >(reinterpret_cast < char *>(p) - RealElemSize);
    }

    // next time we need to get a new block, get twice as much.

    // guard against rotating off high bit:
    if ((nBlockSize <= (kMaxBlockSize >> 1)) && ((nBlockSize << 1) <= kMaxBlockSize)) // nBlockSize*2 <=
                                                                                      // kMaxBlockSize?
    {
        nBlockSize <<= 1;
        nBlockingFactor <<= 1;
    }
    else if (!fMaxedOut)
    {
        // Don't increase the size again!
        fMaxedOut = TRUE;

        // Get as close to kMaxBlockSize as we can, without going over.
        nBlockSize = (kMaxBlockSize / RealElemSize) * RealElemSize;
        nBlockingFactor = nBlockSize / RealElemSize;
    }
}


//
// Allocate one item from the pool
//
void *RTLPool::Alloc()
{
    // Refill the free list if needed
    if (!pFreeList)
    {
        ThreadNewBlock();

        // Check for out-of-memory
        if (!pFreeList)
            return 0;
    }

#if TRACK_ALLOCS
    nInUse++;

    nAllocs++;
    if (nAllocs - nFrees > nMaxTakes)
        nMaxTakes = nAllocs - nFrees;
#endif

    sRTLPoolBlock *p = pFreeList;

    DebugMsgEx1(HEAP, "Alloc'd item @%#p", p);

    pFreeList = pFreeList->pNextFree;

#if HEAP_CHECK
    // Check that the new node isn't still on the list somehow.
    for (sRTLPoolBlock * p1 = pFreeList; p1; p1 = p1->pNextFree)
        AssertMsg(p1 != p, "Rethreading already freed");
#endif

#if TRACK_ALLOCS
    // cross link to next element
    if (pAllocList)
        pAllocList->pPrevAlloc = p;

    p->pNextAlloc = pAllocList;

    // cross link to head
    p->pPrevAlloc = 0;
    pAllocList = p;


    FillStackArray(3, nStackLevels, p->Stack);
#endif

    // Note: Side cast
    return static_cast < sRTLFreePoolPart * >(p);
}


//
// Put memory back on the free chain
//
void RTLPool::Free(void *p)
{
#if DEBUG_USE_AFTER_FREE
#pragma message "Pool allocs will never free"
    memset(p, 0xdd, nElementSize);
    return;
#endif

    DebugMsgEx1(HEAP, "Returning item %p to freelist", p);

    // Note: Side cast
    sRTLPoolBlock *fp = static_cast < sRTLPoolBlock * >(reinterpret_cast < sRTLFreePoolPart * >(p));

#if TRACK_ALLOCS
    // Cross link next element to previous element
    if (fp->pNextAlloc)
        fp->pNextAlloc->pPrevAlloc = fp->pPrevAlloc;

    if (fp->pPrevAlloc)
        fp->pPrevAlloc->pNextAlloc = fp->pNextAlloc;
    else
        pAllocList = fp->pNextAlloc;

    fp->pPrevAlloc = fp->pNextAlloc = (void *) -1;
#endif

#if HEAP_CHECK
    // Prevent Circular free list (and resulting memleak & corruption)
    for (sRTLPoolBlock * p1 = pFreeList; p1; p1 = p1->pNextFree)
        AssertStr(p1 != fp, cFmtStr("Rethreading already freed %#p", this));
#endif

#if TRACK_ALLOCS
    AssertMsg(nInUse, "Freeing once more than Alloc'd");
    nInUse--;

    nFrees++;
#endif

    fp->pNextFree = pFreeList;

    pFreeList = fp;
}


//
// Dump allocated blocks
//
void RTLPool::DumpAllocs()
{
#if TRACK_ALLOCS
    if (!pAllocList)
    {
        LogMsg("No outstanding allocs");
        return;
    }

    for (sRTLPoolBlock * p = pAllocList; p; p = p->pNextAlloc)
    {
        LogMsg("[%p %p %p %p %p %p %p %p]", p->Stack[0],
                p->Stack[1], p->Stack[2],
                p->Stack[3], p->Stack[4],
                p->Stack[5], p->Stack[6],
                p->Stack[7]);
    }


#endif
}


//
// Dump out all pools
//
void RTLPool::DumpPools()
{
#if DUMP_POOLS
    LogMsg("DumpPools()");

    for (RTLPool * p = pPools; p; p = p->pNextPool)
    {
        LogMsg("Pool: ES=%d BF=%d BS=%d Bs=%lu A=%lu F=%lu",
                p->nElementSize,
                p->nBlockingFactor,
                p->nBlockSize,
                p->nBlocks,
                p->nAllocs,
                p->nFrees);
#if TRACK_ALLOCS
        p->DumpAllocs();
#endif
    }

#endif
}
