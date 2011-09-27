// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

// This file contains per-app state/variables for specific subsystems

/////////////////////////////////////////////////////////////////////////////
// CEditView (find and replace state)

struct AFX_FRSTATE      // Find/Replace for CEditView
{
	CFindReplaceDialog* pFindReplaceDlg; // find or replace dialog
	BOOL bFindOnly; // Is pFindReplace the find or replace?
	CString strFind;    // last find string
	CString strReplace; // last replace string
	BOOL bCase; // TRUE==case sensitive, FALSE==not
	int bNext;  // TRUE==search down, FALSE== search up

	AFX_FRSTATE();
};

#ifndef NO_VBX_SUPPORT
/////////////////////////////////////////////////////////////////////////////
// VB specific application globals

class NEAR CVBPict;  // actual class implementation is private

struct AFX_VBSTATE
{
	// For Basic Channel to CFile mapping
	CMapWordToPtr fileMap;

	// for currently loaded CVBControl models
	CMapStringToPtr classMap;

	// Loaded VBX files
	CPtrArray vbxFileList;
	BOOL bPendingVBXRemovals;       // pending removals from vbxFileList

	// For picts
	CVBPict NEAR* pHeadPictList;    // Global Picture List

	// For LOCAL Heap array for VB controls
	CUIntArray heapHandles;         // array of selectors
	UINT selCurrentHeap;            // selector for current heap

	AFX_VBSTATE();
	~AFX_VBSTATE();
};

#ifndef _AFXDLL
extern AFX_VBSTATE NEAR _afxVBState;
#else
#define _afxVBState (*_AfxGetAppData()->appVBState)
#endif

#endif //!NO_VBX_SUPPORT

/////////////////////////////////////////////////////////////////////////////
// OLE state (for both Containers & Servers)

struct AFX_OLESTATE
{
	// application shutdown behavior
	DWORD nObjectCount;
	BOOL bUserCtrl;

#ifdef _AFXDLL
	// class factory support
	COleObjectFactory* pFirstFactory;
	BOOL bExtensionDLL;
#endif

	// init/term support
	BOOL bNeedTerm;
	BOOL bNeedTermCOM;
	LPMALLOC lpTaskMalloc;
	BOOL bNeedTermUI;

	AFX_OLESTATE();
};

#ifndef _AFXDLL
extern AFX_OLESTATE NEAR _afxOleState;
extern COleObjectFactory* NEAR _afxFirstFactory;
#else
#define _afxOleState (*_AfxGetAppData()->appOleState)
#define _afxFirstFactory (_AfxGetAppData()->appOleState->pFirstFactory)
#endif

/////////////////////////////////////////////////////////////////////////////
// Windows Sockets state

struct AFX_SOCKSTATE
{
	void (FAR* lpfnSockCleanup)();
	HWND hSocketWindow;
	CMapWordToPtr mapSocketHandle;
	CMapWordToPtr mapDeadSockets;
	CPtrList listSocketNotifications;
};

/////////////////////////////////////////////////////////////////////////////
// Gray dialog support

#ifndef _AFXDLL
extern HHOOK NEAR _afxHHookOldCbtFilter;
#else
#define _afxHHookOldCbtFilter _AfxGetAppData()->appHHookOldCbtFilter
#endif

/////////////////////////////////////////////////////////////////////////////
