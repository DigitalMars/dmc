// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"
#include <limits.h>

#ifdef AFX_OLE_SEG
#pragma code_seg(AFX_OLE_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// OLE 2.0 OLE_DATA init structure

OLE_DATA NEAR _oleData;

OLE_DATA::OLE_DATA()
{
	// Clipboard formats
	static char BASED_CODE szNative[] = "Native";
	cfNative = ::RegisterClipboardFormat(szNative);
	ASSERT(cfNative != NULL);
	static char BASED_CODE szOwnerLink[] = "OwnerLink";
	cfOwnerLink = ::RegisterClipboardFormat(szOwnerLink);
	ASSERT(cfOwnerLink != NULL);
	static char BASED_CODE szObjectLink[] = "ObjectLink";
	cfObjectLink = ::RegisterClipboardFormat(szObjectLink);
	ASSERT(cfObjectLink != NULL);

	// OLE 2.0 Clipboard formats
	static char BASED_CODE szEmbeddedObject[] = "Embedded Object";
	cfEmbeddedObject = ::RegisterClipboardFormat(szEmbeddedObject);
	ASSERT(cfEmbeddedObject != NULL);
	static char BASED_CODE szEmbedSource[] = "Embed Source";
	cfEmbedSource = ::RegisterClipboardFormat(szEmbedSource);
	ASSERT(cfEmbedSource != NULL);
	static char BASED_CODE szLinkSource[] = "Link Source";
	cfLinkSource = ::RegisterClipboardFormat(szLinkSource);
	ASSERT(cfLinkSource != NULL);
	static char BASED_CODE szObjectDescriptor[] = "Object Descriptor";
	cfObjectDescriptor = ::RegisterClipboardFormat(szObjectDescriptor);
	ASSERT(cfObjectDescriptor != NULL);
	static char BASED_CODE szLinkSourceDescriptor[] = "Link Source Descriptor";
	cfLinkSourceDescriptor = ::RegisterClipboardFormat(szLinkSourceDescriptor);
	ASSERT(cfLinkSourceDescriptor != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// OLE 2.0 IMalloc implementation (only for _DEBUG)

#ifdef _DEBUG
/////////////////////////////////////////////////////////////////////////////
// COleDebugMalloc (debug IMalloc implementation)

class COleDebugMalloc : public CCmdTarget
{
public:
// Interface Maps
	BEGIN_INTERFACE_PART(Malloc, IMalloc)
		STDMETHOD_(void FAR*, Alloc)(ULONG);
		STDMETHOD_(void FAR*, Realloc)(void FAR*, ULONG);
		STDMETHOD_(void, Free)(void FAR*);
		STDMETHOD_(ULONG, GetSize)(void FAR*);
		STDMETHOD_(int, DidAlloc)(void FAR*);
		STDMETHOD_(void, HeapMinimize)();
	END_INTERFACE_PART(Malloc)

	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// COleDebugMalloc::XMalloc implementation

BEGIN_INTERFACE_MAP(COleDebugMalloc, CCmdTarget)
	INTERFACE_PART(COleDebugMalloc, IID_IMalloc, Malloc)
END_INTERFACE_MAP()

STDMETHODIMP_(ULONG) COleDebugMalloc::XMalloc::AddRef()
{
	METHOD_PROLOGUE(COleDebugMalloc, Malloc)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleDebugMalloc::XMalloc::Release()
{
	METHOD_PROLOGUE(COleDebugMalloc, Malloc)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleDebugMalloc::XMalloc::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(COleDebugMalloc, Malloc)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

static char NEAR szZeroSize[1];

struct AFX_ALLOC_HEADER
{
	DWORD dwType;   // type of memory (global or checked)
	DWORD dwSize;   // size of memory block
};

#define TYPE_CHECKED    0x5555  // special tag for sanity checks

#define MAX_LOCAL   0xFF00
#define HEADER_SIZE sizeof(AFX_ALLOC_HEADER)

static ULONG _oleBreakAlloc = -1;
static void FAR* _oleBreakFree = NULL;

STDMETHODIMP_(void FAR*) COleDebugMalloc::XMalloc::Alloc(ULONG cb)
{
	METHOD_PROLOGUE(COleDebugMalloc, Malloc)
	ASSERT_VALID(pThis);

	if (cb > (ULONG)UINT_MAX)
		return NULL;    // greater than 64K not supported

	// special "break at allocation of" debugging feature
	if (cb == _oleBreakAlloc)
		_asm int 3; // break into debugger to record callstack info!

	// allocate of zero bytes must return special zero sized item
	if (cb == 0)
		return szZeroSize;

	AFX_ALLOC_HEADER FAR* pHeader = NULL;
	TRY
	{
		pHeader = (AFX_ALLOC_HEADER*) new char[(size_t)cb + HEADER_SIZE];
		pHeader->dwType = TYPE_CHECKED;
		pHeader->dwSize = cb;
	}
	CATCH_ALL(e)
	{
		return NULL;
	}
	END_CATCH_ALL

	return (char FAR*)pHeader + HEADER_SIZE;
}

STDMETHODIMP_(void FAR*) COleDebugMalloc::XMalloc::Realloc(void FAR* pv, ULONG cb)
{
	METHOD_PROLOGUE(COleDebugMalloc, Malloc)
	ASSERT_VALID(pThis);

	if (szZeroSize == pv)
		pv = NULL;

	// attempt alloc to new size
	void FAR* pvNew = Alloc(cb);
	if (pvNew == NULL)
		return NULL;

	// copy old contents to new block
	if (pv != NULL)
		memcpy(_AfxGetPtrFromFarPtr(pvNew), _AfxGetPtrFromFarPtr(pv),
			min((size_t)GetSize(pv), (size_t)cb));

	// it worked -- free old one return new
	Free(pv);
	return pvNew;
}

STDMETHODIMP_(void) COleDebugMalloc::XMalloc::Free(void FAR* pv)
{
	METHOD_PROLOGUE(COleDebugMalloc, Malloc)
	ASSERT_VALID(pThis);

	if (pv == szZeroSize || pv == NULL)
		return;

	if (_oleBreakFree == pv)
		_asm int 3; // break into debugger to record callstack info!

	// get block header and verify
	AFX_ALLOC_HEADER FAR* pBlockHeader =
		(AFX_ALLOC_HEADER FAR*)((char FAR*)pv - HEADER_SIZE);
	ASSERT(pBlockHeader->dwType == TYPE_CHECKED);
	ASSERT(pBlockHeader->dwSize != 0);

	// deallocate the block
	delete[] (BYTE*)_AfxGetPtrFromFarPtr(pBlockHeader);
}

STDMETHODIMP_(ULONG) COleDebugMalloc::XMalloc::GetSize(void FAR* pv)
{
	METHOD_PROLOGUE(COleDebugMalloc, Malloc)
	ASSERT_VALID(pThis);

	// handle special cases
	if (pv == szZeroSize)
		return 0;
	if (pv == NULL)
		return -1;

	// get block header and verify
	AFX_ALLOC_HEADER FAR* pBlockHeader =
		(AFX_ALLOC_HEADER FAR*)((char FAR*)pv - HEADER_SIZE);
	ASSERT(pBlockHeader->dwType == TYPE_CHECKED);
	ASSERT(pBlockHeader->dwSize != 0);

	// return the size
	return pBlockHeader->dwSize;
}

STDMETHODIMP_(int) COleDebugMalloc::XMalloc::DidAlloc(void FAR* pv)
{
	METHOD_PROLOGUE(COleDebugMalloc, Malloc)
	ASSERT_VALID(pThis);

	return -1;  // not supported
}

STDMETHODIMP_(void) COleDebugMalloc::XMalloc::HeapMinimize()
{
	METHOD_PROLOGUE(COleDebugMalloc, Malloc)
	ASSERT_VALID(pThis);

	// do nothing - not necessary to implement
}

// Define global debug allocator for application
#ifdef _AFXDLL
#define _afxDebugMalloc (_AfxGetAppDebug()->appDebugMalloc)
#else
static COleDebugMalloc NEAR debugMalloc;
#define _afxDebugMalloc (&debugMalloc)
#endif

#endif  //_DEBUG

// OLE provided clipboard Window procedure address
WNDPROC NEAR _afxPrevClipboardWndProc;

// replacement for OLE provided clipboard Window procedure
LRESULT CALLBACK AFX_EXPORT _AfxClipboardWndProc(HWND, UINT, WPARAM, LPARAM);

#ifndef _AFXCTL
/////////////////////////////////////////////////////////////////////////////
// OLE 2.0 initialization & termination

BOOL AFXAPI AfxOleInit()
{
	ASSERT(!_afxOleState.bNeedTerm);    // calling it twice?

	// check OLE build version (OLE 2.01a is 23.622)
	DWORD dwVersion = OleBuildVersion();
	if (HIWORD(dwVersion) != 23 || LOWORD(dwVersion) < 622)
	{
		TRACE0("Error: OLE 2.0 DLLs are NOT compatible.\n");
		return FALSE;
	}

#ifdef _DEBUG
#ifdef _AFXDLL
	// for DLL version, the debug allocator is not created until init time
	_afxDebugMalloc = new COleDebugMalloc;
#endif
#endif

	// first, initialize OLE
#ifdef _DEBUG
	ASSERT_VALID(_afxDebugMalloc);
	SCODE sc = GetScode(::OleInitialize(&_afxDebugMalloc->m_xMalloc));
#else
	SCODE sc = GetScode(::OleInitialize(NULL));
#endif
	if (FAILED(sc))
	{
#ifdef _DEBUG
		// warn about non-NULL success codes
		TRACE1("Warning: OleInitialize returned scode = %s.\n",
			AfxGetFullScodeString(sc));
#endif
		goto InitFailed;
	}
	// termination required when OleInitialize does not fail
	_afxOleState.bNeedTerm = TRUE;

	// Note: this code avoids a bug in the WOW subsystem on Windows NT.
	//  The problem is that WOW sends a WM_DESTROYCLIPBOARD when the
	//  clipboard window does not own the clipboard, and the OLE implementation
	//  does not handle this case.  The fix is to subclass the OLE
	//  provided clipboard window such that WM_DESTROYCLIPBOARD is not
	//  passed through unless the clipboard window is the current clipboard
	//  owner.

	//BLOCK: work-around for OLE clipboard bug under Windows NT.
	{
		// force clipboard window to be created (OLE creates it on the fly)
		::OleFlushClipboard();

		// search for clipboard window belonging to this task
		HWND hWndClip = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
		HTASK hCurTask = ::GetCurrentTask();
		while (hWndClip != NULL)
		{
			if (::GetWindowTask(hWndClip) == hCurTask)
			{
				static char BASED_CODE szClipboardClass[] = "CLIPBOARDWNDCLASS";
				char szTemp[sizeof(szClipboardClass)+1];
					// one extra character (so longer class will not match)

				VERIFY(::GetClassName(hWndClip, szTemp, sizeof szTemp) != 0);
				if (::lstrcmpi(szTemp, szClipboardClass) == 0)
					break;
			}

			hWndClip = ::GetWindow(hWndClip, GW_HWNDNEXT);
		}

		// Windows NT 3.5 and later fix this bug, and change the
		// clipboard implementation such that the logic above will
		// not find a window.  Since the problem is gone, there is
		// no need to subclass the clipboard window in that case.

		if (hWndClip != NULL)
		{
			// subclass the clipboard window
			_afxPrevClipboardWndProc = (WNDPROC)::SetWindowLong(hWndClip,
				GWL_WNDPROC, (LONG)(WNDPROC)_AfxClipboardWndProc);
			ASSERT(_afxPrevClipboardWndProc != NULL);
		}
	}

	// get task allocator
	if (::CoGetMalloc(MEMCTX_TASK, &_afxOleState.lpTaskMalloc) != NOERROR)
	{
		ASSERT(_afxOleState.lpTaskMalloc == NULL);
		goto InitFailed;
	}

#ifdef _AFXDLL
	if (!_afxOleState.bExtensionDLL)
	{
		extern AFX_EXTENSION_MODULE NEAR _afxExtensionDLL;  // see dllole.cpp
		new CDynLinkLibrary(_afxExtensionDLL);
		_afxOleState.bExtensionDLL = TRUE;
	}
#endif

	// hook idle time and exit time for required OLE cleanup
	ASSERT_VALID(AfxGetApp());
	AfxGetApp()->m_lpfnOleFreeLibraries = CoFreeUnusedLibraries;
	AfxGetApp()->m_lpfnOleTerm = AfxOleTerm;

	// allocate and initialize default message filter
	if (AfxGetApp()->m_pMessageFilter == NULL)
	{
		AfxGetApp()->m_pMessageFilter = new COleMessageFilter;
		ASSERT(AfxOleGetMessageFilter() != NULL);
		AfxOleGetMessageFilter()->Register();
	}

	return TRUE;

InitFailed:
	AfxOleTerm();
	return FALSE;
}

void CALLBACK AfxOleTerm()
{
	// release clipboard ownership
	COleDataSource::FlushClipboard();

	// revoke all class factories
	COleObjectFactory::RevokeAll();

	// destroy message filter (may be derived class)
	delete AfxGetApp()->m_pMessageFilter;
	AfxGetApp()->m_pMessageFilter = NULL;

	// release task memory allocator
	_AfxRelease((LPUNKNOWN*)&_afxOleState.lpTaskMalloc);

	// terminate OLE last
	if (_afxOleState.bNeedTerm)
	{
		::OleUninitialize();
		_afxOleState.bNeedTerm = FALSE;
	}

#ifdef _DEBUG
#ifdef _AFXDLL
	// for DLL version, the debug allocator needs to be deleted
	delete _afxDebugMalloc;
	_afxDebugMalloc = NULL;
#endif
#endif
}
#endif //!_AFXCTL

/////////////////////////////////////////////////////////////////////////////
// OLE 2.0 task memory allocation support

void FAR* AFXAPI _AfxAllocTaskMem(size_t nSize)
{
#ifdef _AFXCTL
	// Since an OLE control can never be sure when a task will go away,
	// it should never cache the task allocator.

	return OleStdMalloc(nSize);
#else
	if (_afxOleState.lpTaskMalloc == NULL)
		return NULL;

	void FAR* p = _afxOleState.lpTaskMalloc->Alloc(nSize);
	return p;
#endif
}

void AFXAPI _AfxFreeTaskMem(void FAR* p)
{
#ifdef _AFXCTL
	// Since an OLE control can never be sure when a task will go away,
	// it should never cache the task allocator.

	if (p != NULL)
		OleStdFree(p);
#else
	if (_afxOleState.lpTaskMalloc == NULL)
		return;

	_afxOleState.lpTaskMalloc->Free(p);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CWinApp support for parsing OLE command line

static BOOL PASCAL NEAR ParseOption(LPSTR lpszCmdLine, LPCSTR lpszOption)
{
	int nLen = lstrlen(lpszOption);
	while (*lpszCmdLine != 0)
	{
		if ((*lpszCmdLine == '-' || *lpszCmdLine == '/') &&
			_fstrncmp(lpszOption, lpszCmdLine+1, nLen) == 0)
		{
			// remove the option from the command line
			int nCmdLen = lstrlen(lpszCmdLine);
			_fmemmove(lpszCmdLine, lpszCmdLine+nLen+1, nCmdLen-nLen);
			return TRUE;
		}
		lpszCmdLine++;
	}
	return FALSE;
}

BOOL CWinApp::RunEmbedded()
{
	// hard coded non-localized name
	static char BASED_CODE szEmbedding[] = "Embedding";
	if (ParseOption(m_lpCmdLine, szEmbedding))
	{
		AfxOleSetUserCtrl(FALSE);
		return TRUE;
	}
	return FALSE;   // not run with /Embedding
}

BOOL CWinApp::RunAutomated()
{
	// hard coded non-localized name
	static char BASED_CODE szAutomation[] = "Automation";
	if (ParseOption(m_lpCmdLine, szAutomation))
	{
		AfxOleSetUserCtrl(FALSE);
		return TRUE;
	}
	return FALSE;   // not run with /Automation
}

/////////////////////////////////////////////////////////////////////////////
