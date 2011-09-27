// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"
#include <limits.h>
#define SIZE_T_MAX  UINT_MAX

#ifdef AFX_CORE1_SEG
#pragma code_seg(AFX_CORE1_SEG)
#endif


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG       // most of this file is for debugging

// forward
static void* NEAR PASCAL AllocMemoryDebug(size_t nSize, BOOL bIsObject,
	LPCSTR lpszFileName, int nLine);
static void NEAR PASCAL FreeMemoryDebug(void* pbData, BOOL bIsObject);

/////////////////////////////////////////////////////////////////////////////
// test allocation routines

extern "C" int NEAR afxMemDF = allocMemDF;

void* operator new(size_t nSize)
{
	// memory corrupt before global new
	if (afxMemDF & checkAlwaysMemDF)
		ASSERT(AfxCheckMemory());

	void* p = AllocMemoryDebug(nSize, FALSE, NULL, 0);

	if (p == NULL)
	{
		TRACE1("::operator new(%u) failed - throwing exception\n", nSize);
		AfxThrowMemoryException();
	}

	return p;
}

void* operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
{
	// memory corrupt before global new
	if (afxMemDF & checkAlwaysMemDF)
		ASSERT(AfxCheckMemory());

	void* p = AllocMemoryDebug(nSize, FALSE, lpszFileName, nLine);

	if (p == NULL)
	{
		TRACE1("::operator new(%u) failed - throwing exception\n", nSize);
		AfxThrowMemoryException();
	}

	return p;
}

void operator delete(void* pbData)
{
	// memory corrupt before global delete
	if (afxMemDF & checkAlwaysMemDF)
		ASSERT(AfxCheckMemory());

	FreeMemoryDebug(pbData, FALSE);
}

void* CObject::operator new(size_t nSize)
{
	// memory corrupt before global new
	if (afxMemDF & checkAlwaysMemDF)
		ASSERT(AfxCheckMemory());

	void* p = AllocMemoryDebug(nSize, TRUE, NULL, 0);

	if (p == NULL)
	{
		TRACE1("CObject::operator new(%u) failed - throwing exception\n", nSize);
		AfxThrowMemoryException();
	}

	return p;
}

void*
CObject::operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
{
	// memory corrupt before 'CObject::new'
	if (afxMemDF & checkAlwaysMemDF)
		ASSERT(AfxCheckMemory());

	void* p = AllocMemoryDebug(nSize, TRUE, lpszFileName, nLine);

	if (p == NULL)
	{
		TRACE1("CObject::operator new(%u) failed - throwing exception\n", nSize);
		AfxThrowMemoryException();
	}

	return p;
}

void CObject::operator delete(void* pbData)
{
	// memory corrupt before 'CObject::delete'
	if (afxMemDF & checkAlwaysMemDF)
		ASSERT(AfxCheckMemory());

	FreeMemoryDebug(pbData, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// allocation failure hook, tracking turn on

// Checking _AfxGetAppDebug() for bTrackingOn causes infinite
// recursion in a control if task memory is not initialized.

#ifndef _AFXDLL
static BOOL NEAR bTrackingOn = TRUE;
#else
#ifndef _AFXCTL
#define bTrackingOn (_AfxGetAppDebug()->bMemoryTracking)
#else
extern BOOL bAppDataInit;

#define bTrackingOn (bAppDataInit && _AfxGetAppDebug()->bMemoryTracking)
#endif
#endif

BOOL AFXAPI _AfxDefaultAllocHook(size_t, BOOL, LONG)
	{ return TRUE; }

static AFX_ALLOC_HOOK NEAR pfnAllocHook = _AfxDefaultAllocHook;

AFX_ALLOC_HOOK AFXAPI AfxSetAllocHook(AFX_ALLOC_HOOK pfnNewHook)
{
	AFX_ALLOC_HOOK pfnOldHook = pfnAllocHook;
	pfnAllocHook = pfnNewHook;
	return pfnOldHook;
}

#ifndef _AFXDLL     // In DLL version, exported by MFC250D.DLL (see dllinit.cpp)
BOOL AFXAPI AfxEnableMemoryTracking(BOOL bTrack)
{
	BOOL bOldTrackingOn = bTrackingOn;
	bTrackingOn = bTrack;
	return bOldTrackingOn;
}
#endif //!_AFXDLL

/////////////////////////////////////////////////////////////////////////////
// stop on a specific memory request

#ifndef _PORTABLE
static LONG NEAR lStopRequest = 0;
static AFX_ALLOC_HOOK NEAR pfnOldStopHook = NULL;

#pragma optimize("qgel", off) // assembler cannot be globally optimized
extern "C" void AFXAPI AfxStop()
{
	// set a breakpoint on this routine from debugger
	TRACE0("AfxStop() stopping under the debugger\n");
	_asm { int 3 };
	TRACE0("AfxStop() continues\n");
}
#pragma optimize("", on)

BOOL AFXAPI _AfxTestAllocStop(size_t nSize, BOOL bIsObject,
		LONG lRequest)
{
	if (lRequest == lStopRequest)
	{
		TRACE1("Allocating block # %ld\n", lRequest);
		AfxStop();
	}

	// otherwise just pass on to other hook
	return (*pfnOldStopHook)(nSize, bIsObject, lRequest);
}

// Obsolete API
void AFXAPI AfxSetAllocStop(LONG lRequestNumber)
{
	if (pfnOldStopHook == NULL)
		pfnOldStopHook = AfxSetAllocHook(_AfxTestAllocStop);

	lStopRequest = lRequestNumber;
}
#endif

/////////////////////////////////////////////////////////////////////////////
// AFX Memory Management diagnostics - malloc-like
//

// we keep statistics on what memory is/was used
static LONG NEAR lTotalAlloc;// total bytes of memory allocated
static LONG NEAR lCurAlloc; // current bytes of memory allocated
static LONG NEAR lMaxAlloc; // maximum bytes of memory allocated at any one time

// we keep a request count to use in replaying memory consumption
static LONG NEAR lRequestLast = 0;
#define lNotTracked 0       // if not tracked

// for diagnostic purpose, blocks are allocated with extra information and
//  stored in a doubly-linked list.  This makes all blocks registered with
//  how big they are, when they were allocated and what they are used for.

static struct CBlockHeader* NEAR pFirstBlock = NULL;    // add in reverse order

//  A no-mans-land area is allocated before and after the actual data:
//      ---------
//          start of CBlockHeader pFirstBlocker (linkage and statistical info)
//          no man's land before actual data
//          app pointer-> actual data
//          no man's land after actual data
//      ---------

#define nNoMansLandSize     4       // # of bytes

// The following values are non-zero, contant, odd, large, and atypical
//      Non-zero values help find bugs assuming zero filled data.
//      Constant values are good so that memory filling is deterministic
//          (to help make bugs reproducable).  Of course it is bad if
//          the contant filling of weird values masks a bug.
//      Mathematically odd numbers are good for finding bugs assuming a cleared
//          lower bit, as well as useful for trapping on the Mac.
//      Large numbers (byte values at least) are less typical, and are good
//          at finding bad addresses.
//      Atypical values (i.e. not too often) are good since they typically
//          cause early detection in code.
//      For the case of no-man's land and free blocks, if you store to any
//          of these locations, the memory integrity checker will detect it.

#define bNoMansLandFill     0xFD    // fill no-man's land with this
#define bDeadLandFill       0xDD    // fill free objects with this
#define bCleanLandFill      0xCD    // fill new objects with this

// three uses for registered blocks
static char BASED_CODE szFree[] = "Free";
static char BASED_CODE szObject[] = "Object";
static char BASED_CODE szNonObject[] = "Non-Object";
static char BASED_CODE szDamage[] = "Damage";
static LPCSTR BASED_CODE blockUseName[CMemoryState::nBlockUseMax] =
	{ szFree, szObject, szNonObject };

struct CBlockHeader
{
	struct CBlockHeader* pBlockHeaderNext;
	struct CBlockHeader* pBlockHeaderPrev;
	LPCSTR              lpszFileName;
	int                 nLine;
	size_t              nDataSize;
	enum CMemoryState::blockUsage use;
	LONG                lRequest;
	BYTE                gap[nNoMansLandSize];
	// followed by:
	//  BYTE            data[nDataSize];
	//  BYTE            anotherGap[nNoMansLandSize];
	BYTE* pbData()
		{ return (BYTE*) (this + 1); }
};

static LONG _afxBreakAlloc = -1;    // for debugging memory leaks

static void* NEAR PASCAL
AllocMemoryDebug(size_t nSize, BOOL bIsObject, LPCSTR lpszFileName, int nLine)
// Allocate a memory block of the specific nSize with extra diagnostic
//      support (padding on either nSize of block + linkage)
// Mark it either as object (stores a non-primitive object) or just bits
{
	ASSERT(nSize > 0);

	LONG    lRequest;
	lRequest = bTrackingOn ? ++lRequestLast : lNotTracked;

	if (lRequest == _afxBreakAlloc)
		_asm int 3; // break into debugger at specific memory leak

	// forced failure
	if (!(*pfnAllocHook)(nSize, bIsObject, lRequest))
	{
		TRACE2("diagnostic memory allocation failure at file %Fs line %d\n",
			lpszFileName, nLine);
		return NULL;
	}

	if (!(afxMemDF & allocMemDF))
		return malloc(nSize);

	// Diagnostic memory allocation from this point on
	if (nSize > (size_t)SIZE_T_MAX - nNoMansLandSize - sizeof(CBlockHeader))
	{
		TRACE1("Error: memory allocation: tried to allocate %u bytes\n", nSize);
		TRACE0("  object too large or negative size\n");
		AfxThrowMemoryException();
	}

	// keep track of total amount of memory allocated
	lTotalAlloc += nSize;
	lCurAlloc += nSize;

	if (lCurAlloc > lMaxAlloc)
		lMaxAlloc = lCurAlloc;

	struct CBlockHeader* p = (struct CBlockHeader*)
	   malloc(sizeof(CBlockHeader) + nSize + nNoMansLandSize);

	if (p == NULL)
		return NULL;

	if (pFirstBlock)
		pFirstBlock->pBlockHeaderPrev = p;

	p->pBlockHeaderNext = pFirstBlock;
	p->pBlockHeaderPrev = NULL;
	p->lpszFileName = lpszFileName;
	p->nLine = nLine;
	p->nDataSize = nSize;
	p->use = bIsObject ? CMemoryState::objectBlock : CMemoryState::bitBlock;
	p->lRequest = lRequest;

	// fill in gap before and after real block
	memset(p->gap, bNoMansLandFill, nNoMansLandSize);
	memset(p->pbData() + nSize, bNoMansLandFill, nNoMansLandSize);

	// fill data with silly value (but non-zero)
	memset(p->pbData(), bCleanLandFill, nSize);

	// link blocks together
	pFirstBlock = p;
	return (void*)p->pbData();
}


// debugging free
static void NEAR PASCAL
FreeMemoryDebug(void* pbData, BOOL bIsObject)
{
	if (pbData == NULL)
		return;

	if (!(afxMemDF & allocMemDF))
	{
		free(pbData);
		return;
	}

	struct CBlockHeader* p = ((struct CBlockHeader*) pbData)-1;

	// make sure we are freeing what we think we are:
	// error if freeing incorrect memory type such as using
	// delete to deallocate memory that has been allocated
	// with malloc, or vice versa; or using global delete on
	// a CObject derived object; or using CObject delete on
	// a generic memory block.
	ASSERT(p->use == (bIsObject ? CMemoryState::objectBlock
		: CMemoryState::bitBlock));

	// keep track of total amount of memory allocated
	lCurAlloc -= p->nDataSize;

	p->use = CMemoryState::freeBlock;

	// optionally reclaim memory
	if (!(afxMemDF & delayFreeMemDF))
	{
		// remove from the linked list
		if (p->pBlockHeaderNext)
			p->pBlockHeaderNext->pBlockHeaderPrev = p->pBlockHeaderPrev;

		if (p->pBlockHeaderPrev)
		{
			p->pBlockHeaderPrev->pBlockHeaderNext = p->pBlockHeaderNext;
		}
		else
		{
			ASSERT(pFirstBlock == p);
			pFirstBlock = p->pBlockHeaderNext;
		}

		// fill the entire block including header with dead-land-fill
		memset(p, bDeadLandFill,
			sizeof(CBlockHeader) + p->nDataSize + nNoMansLandSize);
		free(p);
	}
	else
	{
		// keep memory around as dead space
		memset(p->pbData(), bDeadLandFill, p->nDataSize);
	}
}

static BOOL CheckBytes(BYTE* pb, WORD bCheck, size_t nSize)
{
	BOOL bOkay = TRUE;
	while (nSize--)
	{
		if (*pb++ != bCheck)
		{
			TRACE3("memory check error at $%08lX = $%02X, should be $%02X\n",
				(BYTE FAR*) (pb-1),*(pb-1), bCheck);
			bOkay = FALSE;
		}
	}
	return bOkay;
}


BOOL AFXAPI AfxCheckMemory()
  // check all of memory (look for memory tromps)
{
	if (!(afxMemDF & allocMemDF))
		return TRUE;        // can't do any checking

	BOOL    allOkay = TRUE;

	// check all allocated blocks
	struct CBlockHeader* p;
	for (p = pFirstBlock; p != NULL; p = p->pBlockHeaderNext)
	{
		BOOL okay = TRUE;       // this block okay ?
		LPCSTR blockUse;

		if (p->use >= 0 && p->use < CMemoryState::nBlockUseMax)
			blockUse = blockUseName[p->use];
		else
			blockUse = szDamage;

		// first check no-mans-land gaps
		if (!CheckBytes(p->gap, bNoMansLandFill, nNoMansLandSize))
		{
			TRACE2("DAMAGE: before %Fs block at $%08lX\n", blockUse,
				(BYTE FAR*) p->pbData());
			okay = FALSE;
		}

		if (!CheckBytes(p->pbData() + p->nDataSize, bNoMansLandFill,
		  nNoMansLandSize))
		{
			TRACE2("DAMAGE: after %Fs block at $%08lX\n", blockUse,
				(BYTE FAR*) p->pbData());
			okay = FALSE;
		}

		// free blocks should remain undisturbed
		if (p->use == CMemoryState::freeBlock &&
		  !CheckBytes(p->pbData(), bDeadLandFill, p->nDataSize))
		{
			TRACE1("DAMAGE: on top of Free block at $%08lX\n",
				(BYTE FAR*) p->pbData());
			okay = FALSE;
		}

		if (!okay)
		{
			// report some more statistics about the broken object

			if (p->lpszFileName != NULL)
				TRACE3("%Fs allocated at file %Fs(%d)\n", blockUse,
					p->lpszFileName, p->nLine);

			TRACE3("%Fs located at $%08lX is %u bytes long\n", blockUse,
				(BYTE FAR*) p->pbData(), p->nDataSize);

			allOkay = FALSE;
		}
	}
	return allOkay;
}


// -- true if block of exact size, allocated on the heap
// -- set *plRequestNumber to request number (or 0)
BOOL AFXAPI AfxIsMemoryBlock(const void* pData, UINT nBytes,
		LONG* plRequestNumber)
{
	if (!(afxMemDF & allocMemDF))
	{
		// no tracking memory allocator
		if (plRequestNumber != NULL)
			*plRequestNumber = 0;
		return AfxIsValidAddress(pData, nBytes);    // the best we can do
	}

	// otherwise we can check to make sure this was allocated with tracking
	struct CBlockHeader* p = ((struct CBlockHeader*)pData) - 1;

	if (AfxIsValidAddress(p, sizeof(CBlockHeader)) &&
		(p->use == CMemoryState::objectBlock ||
			p->use == CMemoryState::bitBlock) &&
		AfxIsValidAddress(pData, nBytes) &&
		p->nDataSize == nBytes)
	{
		if (plRequestNumber != NULL)
			*plRequestNumber = p->lRequest;
		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMemoryState

CMemoryState::CMemoryState()
{
	m_pBlockHeader = NULL;
}

// fills 'this' with the difference, returns TRUE if significant
BOOL CMemoryState::Difference(const CMemoryState& oldState,
		const CMemoryState& newState)
{
	BOOL bSignificantDifference = FALSE;
	for (int use = 0; use < CMemoryState::nBlockUseMax; use++)
	{
		m_lSizes[use] = newState.m_lSizes[use] - oldState.m_lSizes[use];
		m_lCounts[use] = newState.m_lCounts[use] - oldState.m_lCounts[use];

		if ((m_lSizes[use] != 0 || m_lCounts[use] != 0) &&
		  use != CMemoryState::freeBlock)
			bSignificantDifference = TRUE;
	}
	m_lHighWaterCount = newState.m_lHighWaterCount - oldState.m_lHighWaterCount;
	m_lTotalCount = newState.m_lTotalCount - oldState.m_lTotalCount;

	return bSignificantDifference;
}


void CMemoryState::DumpStatistics() const
{
	for (int use = 0; use < CMemoryState::nBlockUseMax; use++)
	{
		TRACE3("%ld bytes in %ld %Fs Blocks\n", m_lSizes[use],
			m_lCounts[use], blockUseName[use]);
	}

	TRACE1("Largest number used: %ld bytes\n", m_lHighWaterCount);
	TRACE1("Total allocations: %ld bytes\n", m_lTotalCount);
}

// -- fill with current memory state
void CMemoryState::Checkpoint()
{
	if (!(afxMemDF & allocMemDF))
		return;     // can't do anything

	m_pBlockHeader = pFirstBlock;
	for (int use = 0; use < CMemoryState::nBlockUseMax; use++)
		m_lCounts[use] = m_lSizes[use] = 0;

	struct CBlockHeader* p;
	for (p = pFirstBlock; p != NULL; p = p->pBlockHeaderNext)
	{
		if (p->lRequest == lNotTracked)
		{
			// ignore it for statistics
		}
		else if (p->use >= 0 && p->use < CMemoryState::nBlockUseMax)
		{
			m_lCounts[p->use]++;
			m_lSizes[p->use] += p->nDataSize;
		}
		else
		{
			TRACE1("Bad memory block found at $%08lX\n", (BYTE FAR*) p);
		}
	}

	m_lHighWaterCount = lMaxAlloc;
	m_lTotalCount = lTotalAlloc;
}

// Dump objects created after this memory state was checkpointed
// Will dump all objects if this memory state wasn't checkpointed
// Dump all objects, report about non-objects also
// List request number in {}
void CMemoryState::DumpAllObjectsSince() const
{
	if (!(afxMemDF & allocMemDF))
	{
		TRACE0("Debugging allocator turned off, can't dump objects\n");
		return;
	}

	struct CBlockHeader* pBlockStop;

	TRACE0("Dumping objects ->\n");
	pBlockStop = m_pBlockHeader;

	struct CBlockHeader* p;
	for (p = pFirstBlock; p != NULL && p != pBlockStop;
		p = p->pBlockHeaderNext)
	{
		char sz[255];

		if (p->lRequest == lNotTracked)
		{
			// ignore it for dumping
		}
		else if (p->use == CMemoryState::objectBlock)
		{
			CObject* pObject = (CObject*) p->pbData();

			TRACE1("{%ld} ", p->lRequest);
			if (p->lpszFileName != NULL)
			{
				if (!AfxIsValidAddress(p->lpszFileName, 1, FALSE))
					sprintf(sz, "#File Error#(%d) : ", p->nLine);
				else
					sprintf(sz, "%Fs(%d) : ", p->lpszFileName, p->nLine);
				afxDump << (LPCSTR)sz;
			}

#if !defined(_NEARDATA) || defined(_M_I86MM)
			// with large vtable, verify that object and vtable are valid
			if (!AfxIsValidAddress(*(void FAR**)pObject, sizeof(void FAR*), FALSE) ||
				!AfxIsValidAddress(pObject, pObject->GetRuntimeClass()->m_nObjectSize))
#else
			// with near vtable, verify that object and vtable are valid
			if (!AfxIsValidAddress(*(void**)pObject, sizeof(void*), FALSE) ||
				!AfxIsValidAddress(pObject, pObject->GetRuntimeClass()->m_nObjectSize))
#endif
			{
				// short form for trashed objects
				sprintf(sz, "an invalid object at $%08lX, %u bytes long\n",
					(BYTE FAR*) p->pbData(), p->nDataSize);
				afxDump << (LPCSTR)sz;
			}
			else if (afxDump.GetDepth() > 0)
			{
				// long form
				pObject->Dump(afxDump);
				afxDump << "\n";
			}
			else
			{
				// short form
				sprintf(sz, "a %Fs object at $%08lX, %u bytes long\n",
					pObject->GetRuntimeClass()->m_lpszClassName,
					(BYTE FAR*) p->pbData(), p->nDataSize);
				afxDump << (LPCSTR)sz;
			}
		}
		else if (p->use == CMemoryState::bitBlock)
		{
			TRACE1("{%ld} ", p->lRequest);
			if (p->lpszFileName != NULL)
			{
				if (!AfxIsValidAddress(p->lpszFileName, 1, FALSE))
					sprintf(sz, "#File Error#(%d) : ", p->nLine);
				else
					sprintf(sz, "%Fs(%d) : ", p->lpszFileName, p->nLine);
				afxDump << (LPCSTR)sz;
			}

			sprintf(sz, "non-object block at $%08lX, %u bytes long\n",
				(BYTE FAR*) p->pbData(), p->nDataSize);
			afxDump << (LPCSTR)sz;
		}
	}
	TRACE0("Object dump complete.\n");
}

/////////////////////////////////////////////////////////////////////////////
// Enumerate all objects allocated in the diagnostic memory heap

void AFXAPI
AfxDoForAllObjects(void (*pfn)(CObject*, void*), void* pContext)
{
	if (!(afxMemDF & allocMemDF))
		return;         // sorry not enabled

	struct CBlockHeader* p;
	for (p = pFirstBlock; p != NULL; p = p->pBlockHeaderNext)
	{
		if (p->lRequest == lNotTracked)
		{
			// ignore it for iteration
		}
		else if (p->use == CMemoryState::objectBlock)
		{
			CObject* pObject = (CObject*) p->pbData();
			(*pfn)(pObject, pContext);
		}
	}
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Special hooks for _AFXDLL
// Application exports memory allocator for MFCDLL to use

#ifdef _AFXDLL

#include <new.h>

extern "C"
void* AFX_EXPORT CALLBACK AfxAppAlloc(size_t nBytes)
{
#ifdef _DEBUG
	if (AfxGetApp() == NULL)
	{
		TRACE0("Warning: calling new or malloc before initialization\n"
			"\tmay fail with NULL without throwing an exception or may\n"
			"\tcrash if called from an extension DLL!\n");
	}
	// jump to appropriate diagnostic allocator and tracking routine
	if (_AfxGetAppDebug()->lpszAllocFileName == NULL)
	{
		if (_AfxGetAppDebug()->bAllocObj)
			return CObject::operator new(nBytes);
		else
			return operator new(nBytes);
	}
	else
	{
		// else -- pass source line info
		if (_AfxGetAppDebug()->bAllocObj)
			return CObject::operator new(nBytes,
				_AfxGetAppDebug()->lpszAllocFileName,
				_AfxGetAppDebug()->nAllocLine);
		else
			return operator new(nBytes,
				_AfxGetAppDebug()->lpszAllocFileName,
				_AfxGetAppDebug()->nAllocLine);
	}
#else
	return malloc(nBytes);
#endif
}

extern "C"
void AFX_EXPORT CALLBACK AfxAppFree(void* p)
{
#ifdef _DEBUG
	if (AfxGetApp() == NULL)
	{
		TRACE0("Warning: calling delete or free before initialization\n"
			"\tmay fail with NULL without throwing an exception or may\n"
			"\tcrash if called from an extension DLL!\n");
	}
	// jump to appropriate diagnostic allocator and tracking routine
	if (_AfxGetAppDebug()->bAllocObj)
		CObject::operator delete(p);
	else
		operator delete(p);
#else
	if (p != NULL)
		free(p);
#endif
}

extern "C"
FARPROC AFX_EXPORT CALLBACK AfxAppSetNewHandler(FARPROC pnh)
{
	return (FARPROC)_set_new_handler((_PNH)pnh);
}

extern "C"
void* AFX_EXPORT CALLBACK AfxAppReAlloc(void* pOld, size_t nSize)
{
#ifdef _DEBUG
	// use AfxAppAlloc and AfxAppFree to simulate realloc
	void* pNew = AfxAppAlloc(nSize);
	if (pOld != NULL)
	{
		// copy old data to new block
		struct CBlockHeader* p = ((struct CBlockHeader*)pOld)-1;
		memcpy(pNew, pOld, min(nSize, p->nDataSize));
		AfxAppFree(pOld);
	}
	return pNew;
#else
	return realloc(pOld, nSize);
#endif
}

int cdecl AfxNewHandler(size_t /* nSize */)
{
	//  AFX memory allocation will never return "NULL" it will always throw
	//      a memory exception instead
	AfxThrowMemoryException();
	return 0;
}

// hook in our own new_handler
static BOOL AfxMemInitialize()
{
	_set_new_handler(AfxNewHandler);
	return TRUE;
}

static BOOL NEAR bInitialized = AfxMemInitialize();
		// a way to force initialization

#endif //_AFXDLL

/////////////////////////////////////////////////////////////////////////////
// Init seg objects

#pragma warning(disable: 4073)  // disable warning about using init_seg
#pragma init_seg(lib)

/////////////////////////////////////////////////////////////////////////////
// Final exit code for control runtime.

#ifdef _AFXCTL

class AFX_EXITFINAL
{
public:
	~AFX_EXITFINAL();
};

AFX_EXITFINAL::~AFX_EXITFINAL()
{
	extern void _AfxDestroyAllTaskData();

	_AfxDestroyAllTaskData();
}

static AFX_EXITFINAL NEAR afxExitFinal;

#endif //_AFXCTL

/////////////////////////////////////////////////////////////////////////////
// Automatic debug memory diagnostics

#ifdef _DEBUG
#ifdef _WINDOWS // AFX_EXITDUMP comes too late for non-Windows app termination

class AFX_EXITDUMP
{
public:
	~AFX_EXITDUMP();
};

AFX_EXITDUMP::~AFX_EXITDUMP()
{
	// only dump leaks when there are in fact leaks
	CMemoryState msNow;
	msNow.Checkpoint();

	if (msNow.m_lCounts[CMemoryState::objectBlock] != 0 ||
		msNow.m_lCounts[CMemoryState::bitBlock] != 0)
	{
		// dump objects since empty state since difference detected.
		TRACE0("Detected memory leaks!\n");
		afxDump.SetDepth(1);    // just 1 line each
		CMemoryState msEmpty;   // construct empty memory state object
		msEmpty.DumpAllObjectsSince();
	}
}

static AFX_EXITDUMP NEAR afxExitDump;

#endif //_WINDOWS
#endif //_DEBUG

// NOTE: Do NOT place any new object allocations after this point.  If you
//  do they will be constructed before all other objects -- probably not
//  what you want!

/////////////////////////////////////////////////////////////////////////////
