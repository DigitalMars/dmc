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
#include <malloc.h>

#ifdef AFX_CORE1_SEG
#pragma code_seg(AFX_CORE1_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// other globals (internal library use)

IMPLEMENT_DYNAMIC(CWinApp, CCmdTarget)

#ifndef _AFXDLL
CFrameWnd* NEAR _afxFirstFrameWnd = NULL;
#endif

///////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CWinApp::AssertValid() const
{
	CObject::AssertValid();
	ASSERT(afxCurrentWinApp == this);
	ASSERT(afxCurrentInstanceHandle == m_hInstance);

	POSITION pos = m_templateList.GetHeadPosition();
	while (pos != NULL)
	{
		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
		ASSERT_VALID(pTemplate);
	}
}

void CWinApp::Dump(CDumpContext& dc) const
{
	CCmdTarget::Dump(dc);

	AFX_DUMP1(dc, "\nm_hInstance = ", (UINT)m_hInstance);
	AFX_DUMP1(dc, "\nm_hPrevInstance = ", (UINT)m_hPrevInstance);
	AFX_DUMP1(dc, "\nm_lpCmdLine = ", m_lpCmdLine);
	AFX_DUMP1(dc, "\nm_nCmdShow = ", m_nCmdShow);
	AFX_DUMP1(dc, "\nm_pMainWnd = ", m_pMainWnd);
	AFX_DUMP1(dc, "\nm_pszAppName = ", m_pszAppName);
	AFX_DUMP1(dc, "\nm_bHelpMode = ", m_bHelpMode);
	AFX_DUMP1(dc, "\nm_pszExeName = ", m_pszExeName);
	AFX_DUMP1(dc, "\nm_pszHelpFilePath = ", m_pszHelpFilePath);
	AFX_DUMP1(dc, "\nm_pszProfileName = ", m_pszProfileName);
	AFX_DUMP1(dc, "\nm_hDevMode = ", (UINT)m_hDevMode);
	AFX_DUMP1(dc, "\nm_hDevNames = ", (UINT)m_hDevNames);
	AFX_DUMP1(dc, "\nm_dwPromptContext = ", m_dwPromptContext);

	AFX_DUMP0(dc, "\nm_strRecentFiles[] = ");
	for (int i = 0; i < _AFX_MRU_COUNT; i++)
	{
		if (m_strRecentFiles[i].GetLength() != 0)
			AFX_DUMP1(dc, "\n\tFile: ", m_strRecentFiles[i]);
	}

	AFX_DUMP1(dc, "\nm_nWaitCursorCount = ", m_nWaitCursorCount);
	AFX_DUMP1(dc, "\nm_hcurWaitCursorRestore = ", (UINT)m_hcurWaitCursorRestore);
	AFX_DUMP1(dc, "\nm_nNumPreviewPages = ", m_nNumPreviewPages);
	AFX_DUMP0(dc, "\nm_templateList[] = {");

	AFX_DUMP0(dc, "\nm_msgCur = {");
	AFX_DUMP1(dc, "\n\thwnd = ", (UINT)m_msgCur.hwnd);
	AFX_DUMP1(dc, "\n\tmessage = ", (UINT)m_msgCur.message);
	AFX_DUMP1(dc, "\n\twParam = ", (UINT)m_msgCur.wParam);
	AFX_DUMP1(dc, "\n\tlParam = ", (void FAR*)m_msgCur.lParam);
	AFX_DUMP1(dc, "\n\ttime = ", m_msgCur.time);
	AFX_DUMP1(dc, "\n\tpt = ", CPoint(m_msgCur.pt));
	AFX_DUMP0(dc, "}");

	if (dc.GetDepth() == 0)
		return;

	POSITION pos = m_templateList.GetHeadPosition();
	while (pos != NULL)
	{
		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
		AFX_DUMP1(dc, "\n", pTemplate);
	}
	AFX_DUMP0(dc, "}");
}
#endif


CWinApp::CWinApp(const char* pszAppName)
{
	m_pszAppName = pszAppName;
	// in non-running state until WinMain
	m_hInstance = NULL;
	m_pMainWnd = NULL;
	m_pszHelpFilePath = NULL;
	m_pszProfileName = NULL;
	m_pszExeName = NULL;
	m_atomApp = m_atomSystemTopic = NULL;
	m_nSafetyPoolSize = 512;        // default size
	m_pSafetyPoolBuffer = NULL;     // get's allocated in idle

	ASSERT(afxCurrentWinApp == NULL); // only one CWinApp object please
	afxCurrentWinApp = this; // hook for WinMain

#ifdef _DEBUG
	m_nDisablePumpCount = 0;
#endif

	m_nWaitCursorCount = 0;
	m_hcurWaitCursorRestore = NULL;

	m_hDevMode = NULL;
	m_hDevNames = NULL;

	m_nNumPreviewPages = 0;     // not specified (defaults to 1)
	m_lpfnCleanupVBXFiles = NULL;   // Will be set by LoadVBXFile if VBX linked

	m_lpfnOleTerm = NULL;   // will be set if AfxOleInit called
	m_lpfnOleFreeLibraries = NULL;
	m_pMessageFilter = NULL;

	m_nCmdStack = 5120;     // Windows specifies 5k as minimum
	m_nMsgStack = 1024;     // allow other messages to process with 1K

	m_bHelpMode = FALSE;
}


BOOL CWinApp::InitApplication()
{
	return TRUE;
}

BOOL CWinApp::InitInstance()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// App termination

#ifdef AFX_INIT_SEG     // init as well as termination code in 1 segment
#pragma code_seg(AFX_INIT_SEG)
#endif

CWinApp::~CWinApp()
{
	// for cleanup - delete all document templates
	while (!m_templateList.IsEmpty())
		delete (CDocTemplate*)m_templateList.RemoveHead();
	m_templateList.RemoveAll(); // clean up list overhead
	for (int iMRU = 0; iMRU < _AFX_MRU_COUNT; iMRU++)
		m_strRecentFiles[iMRU].Empty();

	// free printer info
	if (m_hDevMode != NULL)
		::GlobalFree(m_hDevMode);
	if (m_hDevNames != NULL)
		::GlobalFree(m_hDevNames);

	// free atoms if used
	if (m_atomApp != NULL)
		::GlobalDeleteAtom(m_atomApp);
	if (m_atomSystemTopic != NULL)
		::GlobalDeleteAtom(m_atomSystemTopic);
}

int CWinApp::ExitInstance()
{
	SaveStdProfileSettings();
	return m_msgCur.wParam; // Returns the value from PostQuitMessage
}

/////////////////////////////////////////////////////////////////////////////

#ifdef AFX_CORE1_SEG
#pragma code_seg(AFX_CORE1_SEG)
#endif

BOOL CWinApp::PumpMessage()
{
#ifdef _DEBUG
	if (m_nDisablePumpCount != 0)
	{
		TRACE0("Error: CWinApp::PumpMessage() called when not permitted\n");
		ASSERT(FALSE);
	}
#endif

	if (!::GetMessage(&m_msgCur, NULL, NULL, NULL))
	{
#ifdef _DEBUG
		if (afxTraceFlags & 2)
			TRACE0("PumpMessage - Received WM_QUIT\n");
		m_nDisablePumpCount++; // application must die
			// NOTE: prevents calling message loop things in 'ExitInstance'
			// will never be decremented
#endif
		return FALSE;
	}

#ifdef _DEBUG
	if (afxTraceFlags & 2)
		_AfxTraceMsg("PumpMessage", &m_msgCur);
#endif

	// process this message
	if (!PreTranslateMessage(&m_msgCur))
	{
		::TranslateMessage(&m_msgCur);
		::DispatchMessage(&m_msgCur);
	}
	return TRUE;
}

// Main running routine until application exits
int CWinApp::Run()
{
	if (m_pMainWnd == NULL && AfxOleGetUserCtrl())
	{
		// Not launched /Embedding or /Automation, but has no main window!
		TRACE0("Warning: 'm_pMainWnd' is NULL in CWinApp::Run"
				" - quitting application\n");
		::PostQuitMessage(0);
	}

	// Acquire and dispatch messages until a WM_QUIT message is received.
	for (; ;)
	{
		LONG lIdleCount = 0;
		// check to see if we can do idle work
		while (!::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE) &&
			OnIdle(lIdleCount++))
		{
			// more work to do
		}

		// either we have a message, or OnIdle returned false

		if (!PumpMessage())
			break;
	}

	return ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Stubs for standard implementation

static BOOL PASCAL IsHelpKey(LPMSG lpMsg)
	// return TRUE only for non-repeat F1 keydowns.
{
	return lpMsg->message == WM_KEYDOWN &&
		   lpMsg->wParam == VK_F1 &&
		   !(HIWORD(lpMsg->lParam) & KF_REPEAT) &&
		   GetKeyState(VK_SHIFT) >= 0 &&
		   GetKeyState(VK_CONTROL) >= 0 &&
		   GetKeyState(VK_MENU) >= 0;
}

BOOL CWinApp::PreTranslateMessage(MSG* pMsg)
{
	HWND hWnd;
	CWnd* pWnd;
	CWnd* pMainWnd = GetMainWnd();

	// walk from the target window up to the desktop window checking
	//  if any window wants to translate this message
	for (hWnd = pMsg->hwnd; hWnd != NULL; hWnd = ::GetParent(hWnd))
	{
		if ((pWnd = CWnd::FromHandlePermanent(hWnd)) != NULL)
		{
			// target window is a C++ window
			if (pWnd->PreTranslateMessage(pMsg))
				return TRUE; // trapped by target window (eg: accelerators)

			if (pWnd == pMainWnd)
				return FALSE;       // got to our main window without interest
		}
	}

	// in case of modeless dialogs, last chance route through main window's
	//   accelerator table
	if (pMainWnd != NULL && pMainWnd->PreTranslateMessage(pMsg))
		return TRUE; // trapped by main window (eg: accelerators)

	return FALSE;       // no special processing
}

/////////////////////////////////////////////////////////////////////////////
// WinHelp Helper

void CWinApp::WinHelp(DWORD dwData, UINT nCmd /* = HELP_CONTEXT */)
{
	CWnd* pMainWnd = (CFrameWnd*)GetMainWnd();
	ASSERT_VALID(pMainWnd);

	// return global app help mode state to FALSE (backward compatibility)
	m_bHelpMode = FALSE;

	AfxGetMainWnd()->WinHelp(dwData, nCmd);
}

/////////////////////////////////////////////////////////////////////////////
// Message Filter processing

static inline BOOL IsEnterKey(LPMSG lpMsg)
{
	return lpMsg->message == WM_KEYDOWN && lpMsg->wParam == VK_RETURN;
}

static inline BOOL IsButtonUp(LPMSG lpMsg)
{
	return lpMsg->message == WM_LBUTTONUP;
}

BOOL CWinApp::ProcessMessageFilter(int code, LPMSG lpMsg)
{
	if (lpMsg == NULL)
		return FALSE;   // not handled

	CFrameWnd* pFrameWnd;
	switch (code)
	{
	case MSGF_MENU:
		// Shift+F1 is only valid on CFrameWnd windows
		pFrameWnd = (CFrameWnd*)CWnd::FromHandle(lpMsg->hwnd);
		if (pFrameWnd != NULL && pFrameWnd->IsFrameWnd())
		{
			ASSERT_VALID(pFrameWnd->GetTopLevelFrame());
			if (GetMainWnd() != NULL && pFrameWnd->IsTracking() &&
				pFrameWnd->GetTopLevelFrame()->m_bHelpMode &&
				(IsEnterKey(lpMsg) || IsButtonUp(lpMsg)))
			{
				GetMainWnd()->SendMessage(WM_COMMAND, ID_HELP);
				return TRUE;
			}
		}
		// fall through...

	case MSGF_DIALOGBOX:    // handles message boxes as well.
		if (GetMainWnd() != NULL && IsHelpKey(lpMsg))
		{
			GetMainWnd()->SendMessage(WM_COMMAND, ID_HELP);
			return TRUE;
		}
		if (code == MSGF_DIALOGBOX && m_pActiveWnd != NULL &&
			lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST)
		{
			// need to translate messages for the in-place container
			static BOOL bWorking;
			if (bWorking)
				return FALSE;
			bWorking = TRUE;    // avoid reentering this code
			MSG msg = *lpMsg;
			if (m_pActiveWnd->IsWindowEnabled() && PreTranslateMessage(&msg))
			{
				bWorking = FALSE;
				return TRUE;
			}
			bWorking = FALSE;   // ok again
		}
		break;
	}

	return FALSE;   // default to not handled
}

// called when exception not caught in a WndProc
LRESULT CWinApp::ProcessWndProcException(CException* e, const MSG* pMsg)
{
	if (pMsg->message == WM_CREATE)
	{
		return -1;  // just fail
	}
	else if (pMsg->message == WM_PAINT)
	{
		// force validation of window to prevent getting WM_PAINT again
		ValidateRect(pMsg->hwnd, NULL);
		return 0;
	}

	UINT nIDP = AFX_IDP_INTERNAL_FAILURE;   // generic message string
	LRESULT lResult = 0;        // sensible default
	if (pMsg->message == WM_COMMAND)
	{
		if ((HWND)LOWORD(pMsg->lParam) == NULL)
			nIDP = AFX_IDP_COMMAND_FAILURE; // command (not from a control)
		lResult = (LRESULT)TRUE;        // pretend the command was handled
	}
	if (e->IsKindOf(RUNTIME_CLASS(CMemoryException)))
	{
		nIDP = AFX_IDP_FAILED_MEMORY_ALLOC;
		AfxMessageBox(nIDP, MB_ICONEXCLAMATION|MB_SYSTEMMODAL);
	}
	else if (!e->IsKindOf(RUNTIME_CLASS(CUserException)))
	{
		// user has not been alerted yet of this somewhat catastrophic problem
		AfxMessageBox(nIDP, MB_ICONSTOP);
	}

	return lResult; // sensible default return from most WndProc functions
}

/////////////////////////////////////////////////////////////////////////////
// CWinApp idle processing

BOOL CWinApp::OnIdle(LONG lCount)
{
	// OLE LRPC calls are bad during idle
	COleMessageFilter* pFilter = AfxOleGetMessageFilter();
	if (pFilter != NULL)
		pFilter->BeginBusyState();

	if (lCount <= 0)
	{
		// send WM_IDLEUPDATECMDUI to the main window
		CWnd* pMainWnd = m_pMainWnd;
		if (pMainWnd != NULL && pMainWnd->m_hWnd != NULL &&
			pMainWnd->IsWindowVisible())
		{
			_AfxCallWndProc(pMainWnd, pMainWnd->m_hWnd,
				WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
			pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
				(WPARAM)TRUE, 0, TRUE, TRUE);
		}
		// send WM_IDLEUPDATECMDUI to all frame windows
		CFrameWnd* pFrameWnd = _afxFirstFrameWnd;
		while (pFrameWnd != NULL)
		{
			if (pFrameWnd->m_hWnd != NULL && pFrameWnd != pMainWnd &&
				pFrameWnd->IsWindowVisible())
			{
				_AfxCallWndProc(pFrameWnd, pFrameWnd->m_hWnd,
					WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
				pFrameWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
					(WPARAM)TRUE, 0, TRUE, TRUE);
			}
			pFrameWnd = pFrameWnd->m_pNextFrameWnd;
		}
		// call doc-template idle hook
		POSITION pos = m_templateList.GetHeadPosition();
		while (pos != NULL)
		{
			CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
			ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
			pTemplate->OnIdle();
		}
	}
	else if (lCount == 1)
	{
		if (afxTempMapLock == 0)
		{
			// free temp maps, VBX DLLs, OLE DLLs, etc.
			AfxLockTempMaps();
			AfxUnlockTempMaps();
		}
	}

	// ok for LRPC calls again
	if (pFilter != NULL)
		pFilter->EndBusyState();

	return lCount < 1;  // more to do if lCount < 1
}

/////////////////////////////////////////////////////////////////////////////
// Access to m_pMainWnd & m_pActiveWnd

CWnd* CWinApp::GetMainWnd()
{
	if (m_pActiveWnd != NULL)
		return m_pActiveWnd;    // probably in-place active

	// when not inplace active, just return main window
	return m_pMainWnd;
}

/////////////////////////////////////////////////////////////////////////////
// force WinMain or LibMain inclusion

#ifdef _WINDLL      // any DLL
extern "C" int PASCAL LibMain(HINSTANCE, WORD, WORD, LPSTR);
static FARPROC linkAddr = (FARPROC) LibMain;
#else
extern "C" int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
static FARPROC linkAddr = (FARPROC) WinMain;
#endif //!_WINDLL

/////////////////////////////////////////////////////////////////////////////
