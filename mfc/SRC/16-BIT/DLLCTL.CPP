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

#ifndef _AFXCTL
#error This file is for the _AFXCTL build only
#endif

#ifndef _AFXDLL
#error This file must be compiled with _AFXDLL
#endif

#include <stdarg.h>
#include <malloc.h>

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// _AFXDLL support

static AFX_EXTENSION_MODULE NEAR coreDLL = { NULL, NULL };
static CDynLinkLibrary* _ctlDynLinkLibBase = NULL;

/////////////////////////////////////////////////////////////////////////////
//  Per-app initialization
//      Called once per module to initialize module specific data.

BOOL WINAPI _AfxAppDataInit()
{
	ASSERT(coreDLL.hModule != NULL);

	// called for each module run with the OC25.DLL
	AFX_APPDATA* pAppData = _AfxGetAppData();
	if (pAppData->cbSize < sizeof(AFX_APPDATA) ||
		(int)pAppData->cbSize < 0)
	{
#ifdef _DEBUG
		AfxOutputDebugString("FATAL ERROR: Illegal size for AFX_APPDATA\n\r");
		ASSERT(FALSE);
#endif
		return FALSE;
	}
	if (pAppData->wVersion != 0x251)
	{
#ifdef _DEBUG
		AfxOutputDebugString("FATAL ERROR: Illegal version for AFX_APPDATA\n\r");
#endif
		ASSERT(FALSE);
		return FALSE;
	}

	// make sure we have a routines for memory allocator, and abort
	ASSERT(_AfxGetAppData()->lpfnAppAlloc != NULL);
	ASSERT(_AfxGetAppData()->lpfnAppFree != NULL);
	ASSERT(_AfxGetAppData()->lpfnAppAbort != NULL);

	// check handle maps
	ASSERT(pAppData->appMapHGDIOBJ != NULL);
	ASSERT(pAppData->appMapHDC != NULL);
	ASSERT(pAppData->appMapHMENU != NULL);
	ASSERT(pAppData->appMapHWND != NULL);

#ifdef _DEBUG
	// check debug entries
	ASSERT(_AfxGetAppDebug() != NULL);
	ASSERT(_AfxGetAppDebug()->lpfnTraceV != NULL);
	ASSERT(_AfxGetAppDebug()->lpszAllocFileName == NULL);
		// internal App data memory is not tracked
#endif //_DEBUG

	// make sure sub-system state structs were allocated
	if (pAppData->appOleState == NULL)
		pAppData->appOleState = new AFX_OLESTATE;

	// make sure sub-system state structs were allocated
	if (pAppData->appOleState == NULL)
		return FALSE;   // failed to load

	ASSERT(pAppData->appOleState != NULL);

	// other sanity asserts : AFX_APPDATA must be zero initialized!
	ASSERT(pAppData->appExceptionContext.m_pLinkTop == NULL);

	// any CRuntimeClasses registered between last DLL loaded and now must
	//  be ours
	ASSERT(pAppData->pFirstAppClass == NULL);
	pAppData->pFirstAppClass = CRuntimeClass::pFirstClass;
	CRuntimeClass::pFirstClass = NULL;  // clear for next DLL

	// there is a CDynLinkLibrary object for each DLL available for each app.
	// hook in MFCDLL first (always last on the resource handler chain)
	// If this is a control, we may already have loaded OC25.DLL when this
	// is called.
	if (_ctlDynLinkLibBase == NULL)
	{
		_ctlDynLinkLibBase = new CDynLinkLibrary(coreDLL);   // will add to list
		_ctlDynLinkLibBase->m_bSystem = TRUE;
	}
	ASSERT(pAppData->pFirstDLL != NULL);

	return TRUE;
}

//  _AfxAppDataTerm
//      Called once per module to terminate module specific data.

void WINAPI _AfxAppDataTerm()
{
	AFX_APPDATA* pAppData = _AfxGetAppData();

	// free the DLL info blocks
	CDynLinkLibrary* pDLLEnd = NULL;
	if (pAppData->m_pID != AfxGetBaseModuleContext())
		pDLLEnd = _ctlDynLinkLibBase;

	CDynLinkLibrary* pDLL;
	CDynLinkLibrary* pDLLNext;
	for (pDLL = pAppData->pFirstDLL; pDLL != pDLLEnd; pDLL = pDLLNext)
	{
		pDLLNext = pDLL->m_pNextDLL;        // save next pointer before delete
		delete pDLL;
	}
	pAppData->pFirstDLL = pDLLEnd;

	// free any MFC library implementation state
	delete pAppData->appOleState;
	pAppData->appOleState = NULL;
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
	m_bSystem = FALSE;

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

	HINSTANCE hInst;

	// first current resource handle, unless it is the runtime DLL.
	if (_AfxGetAppData()->m_pID != AfxGetBaseModuleContext())
	{
		hInst = AfxGetResourceHandle();
		if (::FindResource(hInst, lpszName, lpszType) != NULL)
			return hInst;
	}

	// check for non-system (control) DLLs in proper order
	CDynLinkLibrary* pDLL;
	for (pDLL = _AfxGetAppData()->pFirstDLL; pDLL != NULL;
		pDLL = pDLL->m_pNextDLL)
	{
		if (!pDLL->m_bSystem && pDLL->m_hResource != NULL &&
			::FindResource(pDLL->m_hResource, lpszName, lpszType) != NULL)
		{
			// found it in a DLL
			return pDLL->m_hResource;
		}
	}

	// check language specific resource next
	hInst = _AfxGetAppData()->appLangDLL;
	if (hInst != NULL && ::FindResource(hInst, lpszName, lpszType) != NULL)
		return hInst;

	// check for system DLLs in proper order
	for (pDLL = _AfxGetAppData()->pFirstDLL; pDLL != NULL;
		pDLL = pDLL->m_pNextDLL)
	{
		if (pDLL->m_bSystem && pDLL->m_hResource != NULL &&
			::FindResource(pDLL->m_hResource, lpszName, lpszType) != NULL)
		{
			// found it in a DLL
			return pDLL->m_hResource;
		}
	}

	// if failed to find resource, return application
	return AfxGetResourceHandle();
}

// _AfxLoadString must not only check for the appropriate string segment
//   in the resource file, but also that the string is non-zero
int PASCAL _AfxLoadString(UINT nID, char* pszBuf)
{
	ASSERT(AfxIsValidAddress(pszBuf, 256)); // must be big enough for 256 bytes

	LPCSTR lpszName = MAKEINTRESOURCE((nID>>4)+1);
	HINSTANCE hInst;
	int nLen;

	// first current resource handle, unless it is the runtime DLL.
	if (_AfxGetAppData()->m_pID != AfxGetBaseModuleContext())
	{
		hInst = AfxGetResourceHandle();
		if (::FindResource(hInst, lpszName, RT_STRING) != NULL &&
			(nLen = ::LoadString(hInst, nID, pszBuf, 255)) != 0)
		{
			// found a non-zero string in app
			return nLen;
		}
	}

	// check non-system (control) DLLs in proper order
	CDynLinkLibrary* pDLL;
	for (pDLL = _AfxGetAppData()->pFirstDLL; pDLL != NULL;
		pDLL = pDLL->m_pNextDLL)
	{
		if (!pDLL->m_bSystem && (hInst = pDLL->m_hResource) != NULL &&
		  ::FindResource(hInst, lpszName, RT_STRING) != NULL &&
		  (nLen = ::LoadString(hInst, nID, pszBuf, 255)) != 0)
		{
			return nLen;
		}
	}

	// check language specific DLL next
	hInst = _AfxGetAppData()->appLangDLL;
	if (hInst != NULL && ::FindResource(hInst, lpszName, RT_STRING) != NULL &&
		(nLen = ::LoadString(hInst, nID, pszBuf, 255)) != 0)
	{
		// found a non-zero string in language DLL
		return nLen;
	}

	// check system DLLs in proper order
	for (pDLL = _AfxGetAppData()->pFirstDLL; pDLL != NULL;
		pDLL = pDLL->m_pNextDLL)
	{
		if (pDLL->m_bSystem && (hInst = pDLL->m_hResource) != NULL &&
		  ::FindResource(hInst, lpszName, RT_STRING) != NULL &&
		  (nLen = ::LoadString(hInst, nID, pszBuf, 255)) != 0)
		{
			return nLen;
		}
	}

	return 0;       // did not find it
}

/////////////////////////////////////////////////////////////////////////////
// Language specific DLL support

#define SIZE_ABBREV 4

struct LOCALE_ENTRY
{
	LCID lcid;
	char szAbbrev[SIZE_ABBREV];
};

static LOCALE_ENTRY BASED_CODE leMap[] =
{
	0x0004, "CHT", 0x0404, "CHT", 0x0804, "CHS", 0x0c04, "CHH",
	0x1004, "CHI", 0x0005, "CSY", 0x0405, "CSY", 0x0006, "DAN",
	0x0406, "DAN", 0x0007, "DEU", 0x0407, "DEU", 0x0807, "DES",
	0x0c07, "DEA", 0x0008, "ELL", 0x0408, "ELL",
	0x0809, "ENG", 0x0c09, "ENA", 0x1009, "ENC",
	0x1409, "ENZ", 0x1809, "ENI", 0x000a, "ESP", 0x040a, "ESP",
	0x080a, "ESM", 0x0c0a, "ESN", 0x000b, "FIN", 0x040b, "FIN",
	0x000c, "FRA", 0x040c, "FRA", 0x080c, "FRB", 0x0c0c, "FRC",
	0x100c, "FRS", 0x000e, "HUN", 0x040e, "HUN", 0x000f, "ISL",
	0x040f, "ISL", 0x0010, "ITA", 0x0410, "ITA", 0x0810, "ITS",
	0x0011, "JPN", 0x0411, "JPN", 0x0012, "KOR", 0x0412, "KOR",
	0x0013, "NLD", 0x0413, "NLD", 0x0813, "NLB", 0x0014, "NOR",
	0x0414, "NOR", 0x0814, "NON", 0x0015, "PLK", 0x0415, "PLK",
	0x0016, "PTB", 0x0416, "PTB", 0x0816, "PTG", 0x0019, "RUS",
	0x0419, "RUS", 0x001b, "SKY", 0x041b, "SKY", 0x001d, "SVE",
	0x041d, "SVE", 0x001f, "TRK", 0x041f, "TRK",
};

static const nLocaleEntries = sizeof(leMap) / sizeof(LOCALE_ENTRY);

static int _AfxGetLocaleInfo(LCID lcid, LPSTR lpData, int nMax)
{
	if (nMax < SIZE_ABBREV)
		return 0;   // Need 4 characters for the abbreviation.

	for (int i = 0; i < nLocaleEntries; i++)
	{
		if (leMap[i].lcid == lcid)
		{
			memcpy(lpData, leMap[i].szAbbrev, SIZE_ABBREV);
			return SIZE_ABBREV;
		}
	}

	return 0;
}

void AFXAPI _AfxSetResourceLocale(LCID lcid)
{
	// load language specific DLL (based on locale ID)
	char szLangName[4]; // 3 characters max

	AFX_APPDATA* pAppData = _AfxGetAppData();
	if (!pAppData->bLangDLLInit &&
		_AfxGetLocaleInfo(lcid, szLangName, sizeof(szLangName)) > 0)
	{
		// build DLL name from 3 character locale identifier
		ASSERT(pAppData->appLangDLL == NULL);
		ASSERT(szLangName[0] != '\0');
		static const char BASED_CODE szPrefix[] = "OC25";
		char szLangDLL[13]; // Note: 8.3 name
		lstrcpy(szLangDLL, szPrefix);
		lstrcat(szLangDLL, szLangName);
		lstrcat(szLangDLL, ".DLL");

		// attempt to load the DLL
		UINT errMode = SetErrorMode(SEM_FAILCRITICALERRORS |
			SEM_NOOPENFILEERRORBOX);
		HINSTANCE hinst = LoadLibrary(szLangDLL);
		SetErrorMode(errMode);

		if (hinst > HINSTANCE_ERROR)
			pAppData->appLangDLL = hinst;
	}

	pAppData->bLangDLLInit = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Library init and WEP

extern "C"
int CALLBACK LibMain(HINSTANCE hInstance, WORD w1, WORD w2, LPSTR pstr1)
{
#ifdef _DEBUG
	if (!AfxDiagnosticInit())
		return 0;

	AfxOutputDebugString("OC25D.DLL Initializing!\n\r");
#endif //_DEBUG

	// Shared initialization
	AfxInitExtensionModule(coreDLL, hInstance);

	// Initialize DLL's instance(/module) not the app's
	if (!AfxWinInit(hInstance, NULL, pstr1, 0))
	{
		AfxWinTerm();
		return 0;       // Init Failed
	}

	// initialize the single instance DLL
	if (AfxGetApp() != NULL && !AfxGetApp()->InitInstance())
	{
		AfxGetApp()->ExitInstance();
		AfxWinTerm();
		return 0;
	}

	// Save a copy of our module data for later use.
	AfxPopModuleContext(NULL, TRUE);

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
	FORCE_VECTOR(CWnd)
	FORCE_VECTOR(CWordArray)

	// Control OLE objects.
	FORCE_VECTOR(COleDataObject)
	FORCE_VECTOR(COleDataSource)
	FORCE_VECTOR(COleDispatchDriver)
	FORCE_VECTOR(COleDropSource)
	FORCE_VECTOR(COleDropTarget)
	FORCE_VECTOR(COleResizeBar)
	FORCE_VECTOR(COleStreamFile)
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

	ASSERT(_AfxGetAppData()->m_pID == AfxGetBaseModuleContext());

#ifdef _DEBUG
	if (!afxData.bWin31)
	{
		// AfxTrace() may cause Windows to fault under 3.0 when called from
		// inside the WEP, so it is disabled while executing WEP code.
		afxTraceEnabled = FALSE;
	}

	TRACE0("Inside of OC25D.DLL _WEP, calling (virtual) CWinApp::ExitInstance\n");

	// Check for missing AfxLockTempMap calls
	if (afxTempMapLock != 0)
		TRACE1("Warning: Temp map lock count non-zero (%ld)\n",
			afxTempMapLock);
#endif

	// free language specific DLL
	AFX_APPDATA* pAppData = _AfxGetAppData();
	if (pAppData->appLangDLL != NULL)
	{
		::FreeLibrary(pAppData->appLangDLL);
		pAppData->appLangDLL = NULL;
	}

	// NOTE: An application should be careful not to use too much stack space
	//  in the ExitInstance function.  Under Windows 3.0, the WEP is called
	//  on a very small stack owned by the kernel.
	//  Calling Windows USER functions, in particular, is not advised.
	if (AfxGetApp() != NULL)
		AfxGetApp()->ExitInstance();

	// Now it is safe to cleanup the library.
	AfxWinTerm();

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
