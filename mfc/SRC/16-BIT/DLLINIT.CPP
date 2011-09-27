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
#include "winhand_.h"       // for creation of CHandleMap

#include <stdarg.h>
#include <malloc.h>

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifndef _AFXDLL
#error file must be compiled with _AFXDLL
#endif

/////////////////////////////////////////////////////////////////////////////
// _AFXDLL support

static AFX_EXTENSION_MODULE NEAR coreDLL = { NULL, NULL };

/////////////////////////////////////////////////////////////////////////////
// Per-app initialization

BOOL WINAPI _AfxAppDataInit()
{
	ASSERT(coreDLL.hModule != NULL);

	ASSERT(sizeof(AFX_APPDATA) <= 160);     // hard coded limit in 'afxdll.asm'
#ifdef _DEBUG
	ASSERT(sizeof(AFX_APPDEBUG) <= 32);     // hard coded limit in 'afxdll.asm'
#endif

	// called for each Application's instance run with the MFC250.DLL
	if (_AfxGetAppData()->cbSize < sizeof(AFX_APPDATA) ||
		(int)_AfxGetAppData()->cbSize < 0)
	{
#ifdef _DEBUG
		AfxOutputDebugString("FATAL ERROR: Illegal size for AFX_APPDATA\n\r");
		AfxOutputDebugString(" AFX_NULL segment must start at DGROUP:0010\n\r");
		ASSERT(FALSE);
#endif
		return FALSE;
	}
	if (_AfxGetAppData()->wVersion != 0x250)
	{
#ifdef _DEBUG
		AfxOutputDebugString("FATAL ERROR: Illegal version for AFX_APPDATA\n\r");
#endif
		ASSERT(FALSE);
		return FALSE;
	}

	// make sure we have a routines for memory allocator, VBX and abort
	ASSERT(_AfxGetAppData()->lpfnAppAlloc != NULL);
	ASSERT(_AfxGetAppData()->lpfnAppFree != NULL);
	ASSERT(_AfxGetAppData()->lpfnVBApiEntry != NULL);
	ASSERT(_AfxGetAppData()->lpfnAppAbort != NULL);

	// make sure sub-system state structs were allocated
	ASSERT(_AfxGetAppData()->appVBState == NULL);
	ASSERT(_AfxGetAppData()->appLastFRState == NULL);

	// splitter window state
	ASSERT(_AfxGetAppData()->hcurSplitLast == NULL);
	ASSERT(_AfxGetAppData()->hcurSplitDestroy == NULL);

	// allocate structures for sub-system state
	//  (allocated with malloc and then constructed in-place)
	_AfxGetAppData()->appVBState = new AFX_VBSTATE;
		(AFX_VBSTATE FAR *)_nmalloc(sizeof(AFX_VBSTATE));
	_AfxGetAppData()->appLastFRState = new AFX_FRSTATE;
	if (_AfxGetAppData()->appOleState == NULL)
		_AfxGetAppData()->appOleState = new AFX_OLESTATE;

	// make sure sub-system state structs were allocated
	if (_AfxGetAppData()->appVBState == NULL ||
		_AfxGetAppData()->appLastFRState == NULL ||
		_AfxGetAppData()->appOleState == NULL)
	{
		return FALSE;   // failed to load
	}

	ASSERT(_AfxGetAppData()->appVBState != NULL);
	ASSERT(_AfxGetAppData()->appLastFRState != NULL);
	ASSERT(_AfxGetAppData()->appOleState != NULL);

	// other sanity asserts : AFX_APPDATA must be zero initialized!
	ASSERT(_AfxGetAppData()->appExceptionContext.m_pLinkTop == NULL);
	ASSERT(_AfxGetAppData()->appInstMail == NULL);

	ASSERT(_AfxGetAppData()->appSockState == NULL);

#ifdef _DEBUG
	// check debug entries
	ASSERT(_AfxGetAppDebug() != NULL);
	ASSERT(_AfxGetAppDebug()->lpfnTraceV != NULL);
	ASSERT(_AfxGetAppDebug()->lpszAllocFileName == NULL);
	ASSERT(!_AfxGetAppDebug()->bMemoryTracking);    // tracking off
		// internal App data memory is not tracked
#endif //_DEBUG

	// allocate handle maps
	ASSERT(_AfxGetAppData()->appMapHGDIOBJ == NULL);
	_AfxGetAppData()->appMapHGDIOBJ = new CHandleMap(RUNTIME_CLASS(CGdiObject),
		offsetof(CGdiObject, m_hObject));
	ASSERT(_AfxGetAppData()->appMapHDC == NULL);
	_AfxGetAppData()->appMapHDC = new CHandleMap(RUNTIME_CLASS(CDC),
		offsetof(CDC, m_hDC), 2);
	ASSERT(_AfxGetAppData()->appMapHMENU == NULL);
	_AfxGetAppData()->appMapHMENU = new CHandleMap(RUNTIME_CLASS(CMenu),
		offsetof(CMenu, m_hMenu));
	ASSERT(_AfxGetAppData()->appMapHWND == NULL);
	_AfxGetAppData()->appMapHWND = new CHandleMap(RUNTIME_CLASS(CWnd),
		offsetof(CWnd, m_hWnd));

	// any CRuntimeClasses registered between last DLL loaded and now must
	//  be ours
	ASSERT(_AfxGetAppData()->pFirstAppClass == NULL);
	_AfxGetAppData()->pFirstAppClass = CRuntimeClass::pFirstClass;
	CRuntimeClass::pFirstClass = NULL;  // clear for next DLL

	// there is a CDynLinkLibrary object for each DLL available for each app.
	// hook in MFCDLL first (always last on the resource handler chain)
	ASSERT(_AfxGetAppData()->pFirstDLL == NULL);    // must be first added
	new CDynLinkLibrary(coreDLL);   // will add to list
	ASSERT(_AfxGetAppData()->pFirstDLL != NULL);

	// other initialization
	_AfxGetAppData()->appDlgTextClr = (COLORREF)-1; // use normal text

#ifdef _DEBUG
	// turn on memory tracking from now on
	_AfxGetAppDebug()->bMemoryTracking = TRUE;
#endif
	return TRUE;
}

void WINAPI _AfxAppDataTerm()
{
	// free the DLL info blocks
	CDynLinkLibrary* pDLL;
	CDynLinkLibrary* pDLLNext;
	for (pDLL = _AfxGetAppData()->pFirstDLL; pDLL != NULL; pDLL = pDLLNext)
	{
		pDLLNext = pDLL->m_pNextDLL;        // save next pointer before delete
		delete pDLL;
	}

	// free any MFC library implementation state
	delete _AfxGetAppData()->appVBState;
	_AfxGetAppData()->appVBState = NULL;
	delete _AfxGetAppData()->appLastFRState;
	_AfxGetAppData()->appLastFRState = NULL;
	delete _AfxGetAppData()->appOleState;
	_AfxGetAppData()->appOleState = NULL;

	// free splitter window state (cursors)
	if (_AfxGetAppData()->hcurSplitLast != NULL)
	{
		SetCursor(afxData.hcurArrow);
		::DestroyCursor(_AfxGetAppData()->hcurSplitLast);
		_AfxGetAppData()->hcurSplitLast = NULL;
	}
	if (_AfxGetAppData()->hcurSplitDestroy != NULL)
	{
		::DestroyCursor(_AfxGetAppData()->hcurSplitDestroy);
		_AfxGetAppData()->hcurSplitDestroy = NULL;
	}

	// free MAPI state
	if (_AfxGetAppData()->appInstMail != NULL)
	{
		::FreeLibrary(_AfxGetAppData()->appInstMail);
		_AfxGetAppData()->appInstMail = NULL;
	}

	if (_AfxGetAppData()->appSockState != NULL)
	{
		_AfxGetAppData()->appSockState->lpfnSockCleanup();
		ASSERT(_AfxGetAppData()->appSockState == NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDynLinkLibrary class

IMPLEMENT_DYNAMIC(CDynLinkLibrary, CCmdTarget)

// Constructor - will wire into the current application's list
CDynLinkLibrary::CDynLinkLibrary(AFX_EXTENSION_MODULE& state)
{
	ASSERT(state.hModule != NULL);
	m_hResource = m_hModule = state.hModule;
	m_pFirstSharedClass = state.pFirstSharedClass;

	// Insert at the head of the list (extensions will go in front of core DLL)
	m_pNextDLL = _AfxGetAppData()->pFirstDLL;
	_AfxGetAppData()->pFirstDLL = this;
}

CDynLinkLibrary::~CDynLinkLibrary()
{
}

#ifdef _DEBUG
void CDynLinkLibrary::AssertValid() const
{
	ASSERT(m_hModule != NULL);
}

void CDynLinkLibrary::Dump(CDumpContext& dc) const
{
	CCmdTarget::Dump(dc);

	AFX_DUMP1(dc, "\nm_hModule = ", (UINT)m_hModule);
	AFX_DUMP1(dc, "\nm_hResource = ", (UINT)m_hResource);
	char szName[64];
	GetModuleFileName(m_hModule, szName, sizeof(szName));
	AFX_DUMP1(dc, "\nmodule name = ", szName);
}
#endif //_DEBUG

// This is called in an extension DLL's LibMain
//  It makes a copy of the DLL's HMODULE as well as all the CRuntimeClass
//   objects that have been registered as part of the normal C++ static
//   constructors called before LibMain.
void AFXAPI AfxInitExtensionModule(AFX_EXTENSION_MODULE& state, HMODULE hModule)
{
	ASSERT(hModule != NULL);
	state.hModule = hModule;
	state.pFirstSharedClass = CRuntimeClass::pFirstClass;
	CRuntimeClass::pFirstClass = NULL;  // clear for next DLL or EXE
}

/////////////////////////////////////////////////////////////////////////////
// Resource helpers

HINSTANCE AFXAPI AfxFindResourceHandle(LPCSTR lpszName, LPCSTR lpszType)
{
	ASSERT(lpszName != NULL);
	ASSERT(lpszType != NULL);

	// first check the app
	HINSTANCE hInstApp = AfxGetResourceHandle();
	if (::FindResource(hInstApp, lpszName, lpszType) != NULL)
		return hInstApp;

	// check for DLLs in proper order
	CDynLinkLibrary* pDLL;
	for (pDLL = _AfxGetAppData()->pFirstDLL; pDLL != NULL;
		pDLL = pDLL->m_pNextDLL)
	{
		if (pDLL->m_hResource != NULL &&
			::FindResource(pDLL->m_hResource, lpszName, lpszType) != NULL)
		{
			// found it in a DLL
			return pDLL->m_hResource;
		}
	}

	// if failed to find resource, return application
	return hInstApp;
}

// _AfxLoadString must not only check for the appropriate string segment
//   in the resource file, but also that the string is non-zero
int PASCAL _AfxLoadString(UINT nID, char* pszBuf)
{
	ASSERT(AfxIsValidAddress(pszBuf, 256)); // must be big enough for 256 bytes

	LPCSTR lpszName = MAKEINTRESOURCE((nID>>4)+1);
	HINSTANCE hInst;
	int nLen;

	// first check the app
	hInst = AfxGetResourceHandle();
	if (::FindResource(hInst, lpszName, RT_STRING) != NULL &&
		(nLen = ::LoadString(hInst, nID, pszBuf, 255)) != 0)
	{
		// found a non-zero string in app
		return nLen;
	}

	// check for DLLs in proper order
	CDynLinkLibrary* pDLL;
	for (pDLL = _AfxGetAppData()->pFirstDLL; pDLL != NULL;
		pDLL = pDLL->m_pNextDLL)
	{
		if ((hInst = pDLL->m_hResource) != NULL &&
		  ::FindResource(hInst, lpszName, RT_STRING) != NULL &&
		  (nLen = ::LoadString(hInst, nID, pszBuf, 255)) != 0)
		{
			return nLen;
		}
	}

	return 0;       // did not find it
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Library init and WEP

extern "C"
int CALLBACK LibMain(HINSTANCE hInstance, WORD, WORD, LPSTR)
{
#ifdef _DEBUG
	AfxOutputDebugString("MFCDLL Initializing!\n\r");
#endif //_DEBUG
	// Shared initialization
	AfxInitExtensionModule(coreDLL, hInstance);

	return 1;   // ok
}

#ifdef AFX_CORE1_SEG
#pragma code_seg("AFX_NOTUSED_TEXT")
#endif

#include "afxpen.h"
#define FORCE_VECTOR(className) { new className[1]; }

void CWinApp::_ForceLinkage()
{
	ASSERT(FALSE);      // dummy routine for building DLL

	// A partial list of all the classes in MFC which have a constructor
	//  with zero arguments.    We generate code for each class that allocates
	//  an array 1 big.  This forces the linker to include the
	//  'vector-deleting destructors' instead of the
	//  'scalar-deleting destructors'.  Without this, deleting arrays of MFC
	//  objects in an application using _AFXDLL version of MFC is not possible.

	// Only those classes where allocating an array of objects makes
	//  sense is included in this list.

	FORCE_VECTOR(CBEdit)
	FORCE_VECTOR(CBitmap)
	FORCE_VECTOR(CBitmapButton)
	FORCE_VECTOR(CBrush)
	FORCE_VECTOR(CButton)
	FORCE_VECTOR(CByteArray)
	FORCE_VECTOR(CCmdTarget)
	FORCE_VECTOR(CComboBox)
	FORCE_VECTOR(CDC)
	FORCE_VECTOR(CDWordArray)
	FORCE_VECTOR(CDialogBar)
	FORCE_VECTOR(CEdit)
	FORCE_VECTOR(CFile)
	FORCE_VECTOR(CFont)
	FORCE_VECTOR(CFrameWnd)
	FORCE_VECTOR(CHEdit)
	FORCE_VECTOR(CListBox)
	FORCE_VECTOR(CMapPtrToPtr)
	FORCE_VECTOR(CMapPtrToWord)
	FORCE_VECTOR(CMapStringToOb)
	FORCE_VECTOR(CMapStringToPtr)
	FORCE_VECTOR(CMapStringToString)
	FORCE_VECTOR(CMapWordToOb)
	FORCE_VECTOR(CMapWordToPtr)
	FORCE_VECTOR(CMemFile)
	FORCE_VECTOR(CMenu)
	FORCE_VECTOR(CMetaFileDC)
	FORCE_VECTOR(CObArray)
	FORCE_VECTOR(CObList)
	FORCE_VECTOR(CPalette)
	FORCE_VECTOR(CPen)
	FORCE_VECTOR(CPtrArray)
	FORCE_VECTOR(CPtrList)
	FORCE_VECTOR(CRectTracker)
	FORCE_VECTOR(CRgn)
	FORCE_VECTOR(CScrollBar)
	FORCE_VECTOR(CSharedFile)
	FORCE_VECTOR(CSplitterWnd)
	FORCE_VECTOR(CStatic)
	FORCE_VECTOR(CStatusBar)
	FORCE_VECTOR(CStdioFile)
	FORCE_VECTOR(CString)
	FORCE_VECTOR(CStringArray)
	FORCE_VECTOR(CStringList)
	FORCE_VECTOR(CTime)
	FORCE_VECTOR(CTimeSpan)
	FORCE_VECTOR(CToolBar)
	FORCE_VECTOR(CUIntArray)
	FORCE_VECTOR(CVBControl)
	FORCE_VECTOR(CWnd)
	FORCE_VECTOR(CWordArray)
	FORCE_VECTOR(CTabControl)

}

#ifdef AFX_CORE1_SEG
#pragma code_seg("WEP_TEXT")
#endif

#pragma optimize("", off)
extern "C" void _fpmath(void);

extern "C" int CALLBACK _WEP(int nExitType)
{
	if (nExitType == WEP_SYSTEM_EXIT)
		return TRUE;

	_asm
	{
		mov  bx,2
		call _fpmath
	}

	return TRUE;
}

#pragma optimize("", on)

/////////////////////////////////////////////////////////////////////////////
// Helper routines to delegate to app provided implementation

#ifdef AFX_CORE4_SEG
#pragma code_seg(AFX_CORE4_SEG)
#endif

#ifdef _DEBUG

extern "C" void CDECL
AfxTrace(LPCSTR pszFormat, ...)
{
	va_list args;
	va_start(args, pszFormat);

	(_AfxGetAppDebug()->lpfnTraceV)(pszFormat, args);
}

extern "C" void AFXAPI
AfxAssertFailedLine(LPCSTR lpszFileName, int nLine)
{
	(_AfxGetAppDebug()->lpfnAssertFailed)(lpszFileName, nLine);
}

BOOL AFXAPI AfxEnableMemoryTracking(BOOL bNewTrackingOn)
{
	BOOL bOldTrackingOn = _AfxGetAppDebug()->bMemoryTracking;
	_AfxGetAppDebug()->bMemoryTracking = bNewTrackingOn;
	return bOldTrackingOn;
}

#endif //_DEBUG

void AFXAPI AfxAbort()
{
#ifdef _DEBUG
	AfxOutputDebugString("DLL's AfxAbort called\n\r");
	AfxOutputDebugString(" - calling app provided AbortProc\n\r");
#endif //_DEBUG
	(_AfxGetAppData()->lpfnAppAbort)();
	ASSERT(FALSE);      // NOT REACHED !!
}

void AFXAPI AfxTerminate()
{
	// hookability not provided
	AfxAbort();
}

/////////////////////////////////////////////////////////////////////////////
// Memory allocation is done by app !

void* operator new(size_t nSize)
{
#ifdef _DEBUG
	ASSERT(_AfxGetAppData()->lpfnAppAlloc != NULL);
	ASSERT(_AfxGetAppDebug()->lpszAllocFileName == NULL);
	_AfxGetAppDebug()->bAllocObj = FALSE;
#endif //_DEBUG
	void* p = (_AfxGetAppData()->lpfnAppAlloc)(nSize);
	if (p == NULL)
		AfxThrowMemoryException();
	return p;
}

void* operator new(size_t nSize, LPCSTR pFileName, int nLine)
{
#ifdef _DEBUG
	ASSERT(_AfxGetAppData()->lpfnAppAlloc != NULL);
	_AfxGetAppDebug()->lpszAllocFileName = pFileName;
	_AfxGetAppDebug()->nAllocLine = nLine;
	_AfxGetAppDebug()->bAllocObj = FALSE;
#endif //_DEBUG

	void* p = (_AfxGetAppData()->lpfnAppAlloc)(nSize);

#ifdef _DEBUG
	_AfxGetAppDebug()->lpszAllocFileName = NULL;
#endif //_DEBUG

	if (p == NULL)
		AfxThrowMemoryException();
	return p;
}

void operator delete(void* pbData)
{
	if (pbData == NULL)
		return;
#ifdef _DEBUG
	ASSERT(_AfxGetAppData()->lpfnAppFree != NULL);
	_AfxGetAppDebug()->bAllocObj = FALSE;
#endif //_DEBUG
	(*_AfxGetAppData()->lpfnAppFree)(pbData);
}

/////////////////////////////////////////////////////////////////////////////
// Additional CObject new/delete operators for memory tracking

#ifdef _DEBUG
void* CObject::operator new(size_t nSize)
{
	ASSERT(_AfxGetAppData()->lpfnAppAlloc != NULL);
	ASSERT(_AfxGetAppDebug()->lpszAllocFileName == NULL);
	_AfxGetAppDebug()->bAllocObj = TRUE;
	void* p = (_AfxGetAppData()->lpfnAppAlloc)(nSize);
	if (p == NULL)
		AfxThrowMemoryException();
	return p;
}

void* CObject::operator new(size_t nSize, LPCSTR pFileName, int nLine)
{
	ASSERT(_AfxGetAppData()->lpfnAppAlloc != NULL);
	_AfxGetAppDebug()->lpszAllocFileName = pFileName;
	_AfxGetAppDebug()->nAllocLine = nLine;
	_AfxGetAppDebug()->bAllocObj = TRUE;

	void* p = (_AfxGetAppData()->lpfnAppAlloc)(nSize);
	_AfxGetAppDebug()->lpszAllocFileName = NULL;
	if (p == NULL)
		AfxThrowMemoryException();
	return p;
}

void CObject::operator delete(void* pbData)
{
	if (pbData == NULL)
		return;
	ASSERT(_AfxGetAppData()->lpfnAppFree != NULL);
	_AfxGetAppDebug()->bAllocObj = TRUE;
	(*_AfxGetAppData()->lpfnAppFree)(pbData);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// we must also replace any direct calls to malloc/free

extern "C"
void __far* __cdecl _fmalloc(size_t nSize)
{
#ifdef _DEBUG
	ASSERT(_AfxGetAppData()->lpfnAppAlloc != NULL);
	ASSERT(_AfxGetAppDebug()->lpszAllocFileName == NULL);
	_AfxGetAppDebug()->bAllocObj = FALSE;
	BOOL bOldTracking = _AfxGetAppDebug()->bMemoryTracking;
	_AfxGetAppDebug()->bMemoryTracking = FALSE; // mallocs not tracked
#endif //_DEBUG
	void* p = (_AfxGetAppData()->lpfnAppAlloc)(nSize);
#ifdef _DEBUG
	_AfxGetAppDebug()->bMemoryTracking = bOldTracking;
#endif //_DEBUG
	if (p == NULL)
		AfxThrowMemoryException();
	return p;
}

extern "C"
void __cdecl _ffree(void __far* p)
{
#ifdef _DEBUG
	ASSERT(_AfxGetAppData()->lpfnAppFree != NULL);
	_AfxGetAppDebug()->bAllocObj = FALSE;
#endif //_DEBUG
	(*_AfxGetAppData()->lpfnAppFree)(p);
}

extern "C"
void __far* __cdecl _frealloc(void __far* pOld, size_t nSize)
{
#ifdef _DEBUG
	ASSERT(_AfxGetAppData()->lpfnAppReAlloc != NULL);
	_AfxGetAppDebug()->bAllocObj = FALSE;
#endif //_DEBUG
	ASSERT(_AfxGetAppData()->lpfnAppReAlloc != NULL);
	return (_AfxGetAppData()->lpfnAppReAlloc)(pOld, nSize);
}

/////////////////////////////////////////////////////////////////////////////
// and finally replace the _set_new_handler

_PNH AFXAPI _AfxSetNewHandler(_PNH pnh)
{
	ASSERT(_AfxGetAppData()->lpfnAppSetNewHandler != NULL);
	return (_PNH)(_AfxGetAppData()->lpfnAppSetNewHandler)((FARPROC)pnh);
}

/////////////////////////////////////////////////////////////////////////////
// Also stub out the runtime init 'setenvp' routine to avoid malloc calls

extern "C" void _cdecl _setenvp()
{
}

/////////////////////////////////////////////////////////////////////////////

#ifdef AFX_CORE1_SEG
#pragma code_seg("AFX_VECTDEL_TEXT")
#endif

/////////////////////////////////////////////////////////////////////////////
