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
#ifdef _WINDOWS
#include "winhand_.h"
#endif

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CException

IMPLEMENT_DYNAMIC(CException, CObject)      // abstract class

/////////////////////////////////////////////////////////////////////////////
// AFX_EXCEPTION_CONTEXT (thread global state)

#ifdef _USRDLL
// global state for AFX_EXCEPTION_CONTEXT handle map
static CMapWordToPtr mapHTASK;

// global state for simple single task cache
static HTASK NEAR hTaskCache;
static AFX_EXCEPTION_CONTEXT* NEAR pContextCache;

#ifndef _PORTABLE
extern int cdecl AfxCriticalNewHandler(size_t nSize);
#endif

// out-of-line helper for getting exception context for current task
AFX_EXCEPTION_CONTEXT* AFXAPI AfxGetExceptionContext()
{
	HTASK hTaskCurrent = ::GetCurrentTask();
	ASSERT(hTaskCurrent != NULL);
	if (hTaskCurrent != hTaskCache)
	{
		AFX_EXCEPTION_CONTEXT* pContextLookup;
		if (!mapHTASK.Lookup((WORD)hTaskCurrent, (void*&)pContextLookup))
		{
			// need cached temporary task context for TRY below
			AFX_EXCEPTION_CONTEXT context;
			context.m_pLinkTop = (AFX_EXCEPTION_LINK*)BEFORE_START_POSITION;
			hTaskCache = hTaskCurrent;
			pContextCache = &context;

			TRY
			{
				// We don't want the user to see these memory
				//  allocations, so we turn tracing off.
#ifdef _DEBUG
				BOOL bEnable = AfxEnableMemoryTracking(FALSE);
#endif
#ifndef _PORTABLE
				_PNH pnhOldHandler = _AfxSetNewHandler(AfxCriticalNewHandler);
#endif

				// allocate new task local context
				pContextLookup = new AFX_EXCEPTION_CONTEXT;
				pContextLookup->m_pLinkTop = NULL;

				// and insert it into the map
				mapHTASK.SetAt((WORD)hTaskCurrent, pContextLookup);

#ifndef _PORTABLE
				_AfxSetNewHandler(pnhOldHandler);
#endif
#ifdef _DEBUG
				AfxEnableMemoryTracking(bEnable);
#endif
			}
			CATCH_ALL(e)
			{
				TRACE0("Error: Failed allocation of AFX_EXCEPTION_CONTEXT!\n");
				AfxTerminate();
				ASSERT(FALSE);  // not reached
			}
			END_CATCH_ALL
		}
		hTaskCache = hTaskCurrent;
		pContextCache = pContextLookup;
	}
	ASSERT(pContextCache != NULL);
	return pContextCache;
}

// simulate global variable afxExceptionContext
#define afxExceptionContext (*AfxGetExceptionContext())
#endif  // _USRDLL

// single threaded, assume 1 global exception context
#ifndef _WINDLL
static AFX_EXCEPTION_CONTEXT NEAR afxExceptionContext;
#define AfxGetExceptionContext() (&afxExceptionContext)
#endif

#ifdef _AFXDLL
#define afxExceptionContext (_AfxGetAppData()->appExceptionContext)
#define AfxGetExceptionContext() (&_AfxGetAppData()->appExceptionContext)
#endif

#ifndef _WINDOWS
extern "C" void __cdecl longjmp(int[_AFX_JBLEN], int);
#endif

/////////////////////////////////////////////////////////////////////////////
// AFX_EXCEPTION_LINK linked 'jmpbuf' and out-of-line helpers

AFX_EXCEPTION_LINK::AFX_EXCEPTION_LINK()
{
	// setup initial link state
	m_nType = 0;
	m_pException = NULL;            // no current exception for TRY block
	m_bAutoDelete = FALSE;

	// wire into top of exception link stack
	AFX_EXCEPTION_CONTEXT* pContext = AfxGetExceptionContext();
	m_pLinkPrev = pContext->m_pLinkTop;
	pContext->m_pLinkTop = this;
}

// out-of-line cleanup called from inline AFX_EXCEPTION_LINK destructor
void AFXAPI AfxTryCleanupProc()
{
	AFX_EXCEPTION_CONTEXT* pContext = AfxGetExceptionContext();
	AFX_EXCEPTION_LINK* pLinkTop = pContext->m_pLinkTop;
	ASSERT(pLinkTop != NULL);

	// delete current exception
	if (pLinkTop->m_pException && pLinkTop->m_bAutoDelete)
		delete pLinkTop->m_pException;

	// remove ourself from the top of the chain
	pContext->m_pLinkTop = pLinkTop->m_pLinkPrev;

#ifdef _USRDLL
	if (pLinkTop->m_pLinkPrev == NULL)
	{
		// current task should be cached
		ASSERT(pContext == pContextCache);
		ASSERT(hTaskCache == ::GetCurrentTask());

		// remove from the map and delete the temp handle
		VERIFY(mapHTASK.RemoveKey((WORD)hTaskCache));
		delete pContextCache;
		hTaskCache = NULL;
	}
#endif
}

// out-of-line implementation of CATCH and AND_CATCH
BOOL AFXAPI AfxCatchProc(CRuntimeClass* pClass)
{
	ASSERT(pClass != NULL);

	AFX_EXCEPTION_CONTEXT* pContext = AfxGetExceptionContext();
	ASSERT(pContext->m_pLinkTop != NULL);
	CException* pException = pContext->m_pLinkTop->m_pException;
	ASSERT(pException != NULL);
	return pException->IsKindOf(pClass);
}

// out-of-line implementation for END_CATCH and THROW_LAST
void AFXAPI AfxThrowLast()
{
	AfxThrow(NULL, FALSE);
}

// out-of-line implementation for THROW
void AFXAPI AfxThrow(CException* pNewException, BOOL bShared)
{
	// get current exception context for running task
	AFX_EXCEPTION_CONTEXT* pContext = AfxGetExceptionContext();

	// check for THROW_LAST() first
	if (pNewException == NULL)
	{
		ASSERT(pContext->m_pLinkTop != NULL);
		pNewException = pContext->m_pLinkTop->m_pException;
		bShared = !pContext->m_pLinkTop->m_bAutoDelete;
	}
	ASSERT_VALID(pNewException);

	TRACE1("Warning: Throwing an Exception of type %Fs\n",
		pNewException->GetRuntimeClass()->m_lpszClassName);

	// walk the handlers -- throw to first available
	for (;;)
	{
		if (pContext->m_pLinkTop == NULL)
		{
			// uncaught exception, terminate
			TRACE1("Error: Un-caught Exception (%Fs)\n",
				pNewException->GetRuntimeClass()->m_lpszClassName);
			AfxTerminate();
			ASSERT(FALSE);  // not reached
		}

		if (pContext->m_pLinkTop->m_pException != NULL)
		{
			// a THROW during a CATCH block -- this link will not be
			//  destructed, so it is necessary to do all the cleanup that
			//  the destructor would do.
			AFX_EXCEPTION_LINK* pTemp = pContext->m_pLinkTop;
			if (pTemp->m_pException != pNewException && pTemp->m_bAutoDelete)
				delete pTemp->m_pException;
			pTemp->m_pException = NULL;

			// unlink the top exception link
			pContext->m_pLinkTop = pTemp->m_pLinkPrev;
			pTemp->m_pLinkPrev = NULL;
		}
		else
		{
			// throw the exception to the top handler (if appropriate type)
			AFX_EXCEPTION_LINK* pReceiver = pContext->m_pLinkTop;
			if (pReceiver->m_nType == 0)
			{
				// setup the receiver's context for the new exception
				pReceiver->m_pException = pNewException;
				pReceiver->m_bAutoDelete = !bShared;

				// and jump into the handler...
#ifdef _WINDOWS
				::Throw(pReceiver->m_jumpBuf, 1);
#else
				::longjmp(pReceiver->m_jumpBuf, 1);
#endif
				ASSERT(FALSE);  // not reached
			}

			// otherwise just call cleanup proc
			(*pReceiver->m_callback.pfnCleanup)(pReceiver);
		}
	}

	ASSERT(FALSE);  // not reached
}

/////////////////////////////////////////////////////////////////////////////
// Global exception terminate handling - Obsolete API
//   (useful for non-Windows MS-DOS apps only)

#ifndef _AFXDLL
static AFX_TERM_PROC NEAR pfnTerminate = AfxAbort;

void AFXAPI AfxTerminate()
{
	TRACE0("AfxTerminate called\n");
	(*pfnTerminate)();
}

AFX_TERM_PROC AFXAPI AfxSetTerminate(AFX_TERM_PROC pfnNew)
{
	AFX_TERM_PROC pfnOld = pfnTerminate;
	pfnTerminate = pfnNew;
	return pfnOld;
}
#endif //!_AFXDLL

/////////////////////////////////////////////////////////////////////////////
// Standard exceptions

IMPLEMENT_DYNAMIC(CMemoryException, CException)
static CMemoryException NEAR simpleMemoryException;
void AFXAPI AfxThrowMemoryException()
	{ AfxThrow(&simpleMemoryException, TRUE); }

IMPLEMENT_DYNAMIC(CNotSupportedException, CException)
static CNotSupportedException NEAR simpleNotSupportedException;
void AFXAPI AfxThrowNotSupportedException()
	{ AfxThrow(&simpleNotSupportedException, TRUE); }

/////////////////////////////////////////////////////////////////////////////
