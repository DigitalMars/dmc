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
#include <dde.h>        // for DDE execute shell requests

#ifdef AFX_CORE4_SEG
#pragma code_seg(AFX_CORE4_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRect for creating windows with the default position/size

const CRect AFXAPI_DATA CFrameWnd::rectDefault(CW_USEDEFAULT, CW_USEDEFAULT,
	0 /* 2*CW_USEDEFAULT */, 0 /* 2*CW_USEDEFAULT */);

// in this file for IsKindOf library granularity (IsKindOf references these)
IMPLEMENT_DYNCREATE(CFrameWnd, CWnd)
IMPLEMENT_DYNAMIC(CView, CWnd)
IMPLEMENT_DYNAMIC(CControlBar, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CFrameWnd

BEGIN_MESSAGE_MAP(CFrameWnd, CWnd)
	//{{AFX_MSG_MAP(CFrameWnd)
	// windows messages
	ON_WM_INITMENUPOPUP()
	ON_WM_MENUSELECT()
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_ENTERIDLE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_NCDESTROY()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_WM_SYSCOMMAND()
	ON_WM_DROPFILES()
	ON_WM_QUERYENDSESSION()
	ON_WM_ENDSESSION()
	ON_WM_SETCURSOR()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_ENABLE()

	ON_MESSAGE(WM_COMMANDHELP, OnCommandHelp)
	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	ON_MESSAGE(WM_DDE_INITIATE, OnDDEInitiate)
	ON_MESSAGE(WM_DDE_EXECUTE, OnDDEExecute)
	ON_MESSAGE(WM_DDE_TERMINATE, OnDDETerminate)

	// turning on and off standard frame gadgetry
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_STATUS_BAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_TOOLBAR, OnBarCheck)

	// turning on and off standard mode indicators
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CAPS, OnUpdateKeyIndicator)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_NUM, OnUpdateKeyIndicator)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SCRL, OnUpdateKeyIndicator)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrameWnd construction/destruction

CFrameWnd::CFrameWnd()
{
	ASSERT(m_hWnd == NULL);

	m_nWindow = -1; // unknown
	m_bAutoMenuEnable = TRUE;       // auto enable on by default
	m_lpfnCloseProc = NULL;
	m_hMenuDefault = NULL;
	m_hAccelTable = NULL;
	m_nIDHelp = 0;
	m_nIDTracking = 0;
	m_nIDLastMessage = 0;
	m_pViewActive = NULL;

	m_cModalStack = 0;              // initialize modality support
	m_phWndDisable = NULL;
	m_pNotifyHook = NULL;
	m_hMenuAlt = NULL;
	m_nIdleFlags = 0;               // no idle work at start
	m_rectBorder.SetRectEmpty();

	m_bHelpMode = HELP_INACTIVE;    // not in Shift+F1 help mode
	m_dwPromptContext = 0;

	m_pNextFrameWnd = NULL;         // not in list yet
}

CFrameWnd::~CFrameWnd()
{
}

void CFrameWnd::AddFrameWnd()
{
	// hook it into the CFrameWnd list
	m_pNextFrameWnd = _afxFirstFrameWnd;
	_afxFirstFrameWnd = this;
}

void CFrameWnd::RemoveFrameWnd()
{
	// remove this frame window from the list of frame windows
	CFrameWnd* pFrameWnd = _afxFirstFrameWnd;
	ASSERT(pFrameWnd != NULL);
	if (pFrameWnd == this)
	{
		// special case for first factory in the list -- point first at next
		_afxFirstFrameWnd = m_pNextFrameWnd;
	}
	else
	{
		// find the link that points to the one we are removing
		while (pFrameWnd->m_pNextFrameWnd != this)
		{
			pFrameWnd = pFrameWnd->m_pNextFrameWnd;
			ASSERT(pFrameWnd != NULL);  // must find it before end
		}
		// and point it to the next one
		pFrameWnd->m_pNextFrameWnd = m_pNextFrameWnd;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Special processing etc

BOOL CFrameWnd::LoadAccelTable(LPCSTR lpszResourceName)
{
	ASSERT(m_hAccelTable == NULL);  // only do once
	ASSERT(lpszResourceName != NULL);

	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_ACCELERATOR);
	m_hAccelTable = ::LoadAccelerators(hInst, lpszResourceName);
	return (m_hAccelTable != NULL);
}

BOOL CFrameWnd::PreTranslateMessage(MSG* pMsg)
{
	// check for special cancel modes for ComboBoxes
	if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_NCLBUTTONDOWN)
		_AfxCancelModes(pMsg->hwnd);    // filter clicks

	// allow hook to consume message
	if (m_pNotifyHook != NULL && m_pNotifyHook->OnPreTranslateMessage(pMsg))
		return TRUE;

	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		// use document specific accelerator table over m_hAccelTable
		HACCEL hAccelTable = m_hAccelTable;
		HACCEL hAccel;
		CDocument* pDoc = GetActiveDocument();
		if (pDoc != NULL && (hAccel = pDoc->GetDefaultAccel()) != NULL)
			hAccelTable = hAccel;

		// finally, translate the message
		return (hAccelTable != NULL &&
		  ::TranslateAccelerator(m_hWnd, hAccelTable, pMsg));
	}

	return FALSE;
}

void CFrameWnd::PostNcDestroy()
{
	// default for frame windows is to allocate them on the heap
	//  the default post-cleanup is to 'delete this'.
	// never explicitly call 'delete' on a CFrameWnd, use DestroyWindow instead
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CFrameWnd support for context sensitive help.

void CFrameWnd::ExitHelpMode()
{
	// if not in help mode currently, this is a no-op
	if (!m_bHelpMode)
		return;

	// only post new WM_EXITHELPMODE message if one doesn't already exist
	//  in the queue.
	MSG msg;
	if (!::PeekMessage(&msg, m_hWnd, WM_EXITHELPMODE, WM_EXITHELPMODE,
		PM_REMOVE|PM_NOYIELD))
	{
		VERIFY(::PostMessage(m_hWnd, WM_EXITHELPMODE, 0, 0));
	}

	// release capture if this window has it
	if (::GetCapture() == m_hWnd)
		::ReleaseCapture();

	m_bHelpMode = HELP_INACTIVE;
}

BOOL CFrameWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (GetParent() == NULL && nHitTest ==  HTERROR &&
		(message == WM_LBUTTONDOWN || message == WM_MBUTTONDOWN ||
		 message == WM_RBUTTONDOWN) && !GetProp(m_hWnd, "StayDisabled"))
	{
		// activate the last active window if not active
		CWnd* pLastActive = GetLastActivePopup();
		if (pLastActive != NULL && pLastActive != GetActiveWindow())
		{
			pLastActive->SetActiveWindow();
			return TRUE;
		}
	}
	ASSERT_VALID(GetTopLevelFrame());
	if (GetTopLevelFrame()->m_bHelpMode)
	{
		SetCursor(afxData.hcurHelp);
		return TRUE;
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CFrameWnd::OnEnable(BOOL bEnable)
{
	if (bEnable && GetProp(m_hWnd, "StayDisabled"))
	{
		// Work around for MAPI support. This makes sure the main window
		// remains disabled even when the mail system is booting.
		EnableWindow(FALSE);
		::SetFocus(NULL);
		return;
	}

	Default();
}

LRESULT CFrameWnd::OnCommandHelp(WPARAM, LPARAM lParam)
{
	if (lParam == 0)
	{
		if (IsTracking())
			lParam = HID_BASE_COMMAND+m_nIDTracking;
		else
			lParam = HID_BASE_RESOURCE+m_nIDHelp;
	}
	if (lParam != 0)
	{
		AfxGetApp()->WinHelp(lParam);
		return TRUE;
	}
	return FALSE;
}

LRESULT CFrameWnd::OnHelpHitTest(WPARAM, LPARAM)
{
	if (m_nIDHelp != 0)
		return HID_BASE_RESOURCE+m_nIDHelp;
	else
		return 0;
}

BOOL CFrameWnd::OnCommand(WPARAM wParam, LPARAM lParam)
	// return TRUE if command invocation was attempted
{
	HWND hWndCtrl = (HWND)LOWORD(lParam);
	UINT nID = LOWORD(wParam);
	ASSERT_VALID(GetTopLevelFrame());
	if (GetTopLevelFrame()->m_bHelpMode && hWndCtrl == NULL &&
		nID != ID_HELP && nID != ID_DEFAULT_HELP && nID != ID_CONTEXT_HELP)
	{
		ASSERT(nID != 0);
		if (!SendMessage(WM_COMMANDHELP, 0, HID_BASE_COMMAND+nID))
			SendMessage(WM_COMMAND, ID_DEFAULT_HELP);
		return TRUE;
	}
	return CWnd::OnCommand(wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CFrameWnd support for modality

void CFrameWnd::BeginModalState()
{
	ASSERT(m_hWnd != NULL);
	ASSERT(::IsWindow(m_hWnd));

	// allow stacking, but don't do anything
	if (++m_cModalStack > 1)
		return;

	// determine top-level parent, since that is the true parent of any
	//  modeless windows anyway...
	CWnd* pParent = GetTopLevelParent();

	// first count all windows that need to be disabled
	UINT nCount = 0;
	HWND hWnd = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
	while (hWnd != NULL)
	{
		if (::IsWindowEnabled(hWnd) &&
			CWnd::FromHandlePermanent(hWnd) != NULL &&
			_AfxIsDescendant(pParent->m_hWnd, hWnd))
			++nCount;
		hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
	}
	if (nCount == 0)
		return;

	m_phWndDisable = new HWND[nCount+1];

	// disable all windows connected to this frame (and add them to the list)
	UINT nIndex = 0;
	hWnd = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
	while (hWnd != NULL)
	{
		if (::IsWindowEnabled(hWnd) &&
			CWnd::FromHandlePermanent(hWnd) != NULL &&
			_AfxIsDescendant(pParent->m_hWnd, hWnd))
		{
			::EnableWindow(hWnd, FALSE);
			ASSERT(nIndex < nCount);
			m_phWndDisable[nIndex] = hWnd;
			++nIndex;
		}
		hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
	}

	// terminate the list with a NULL
	ASSERT(nIndex < nCount+1);
	m_phWndDisable[nIndex] = NULL;
}

void CFrameWnd::EndModalState()
{
	// pop one off the stack (don't undo modalness unless stack is down to zero)
	if (m_cModalStack == 0 || --m_cModalStack > 0 || m_phWndDisable == NULL)
		return;

	// enable all the windows disabled by BeginModalState
	ASSERT(m_phWndDisable != NULL);
	UINT nIndex = 0;
	while (m_phWndDisable[nIndex] != NULL)
	{
		ASSERT(m_phWndDisable[nIndex] != NULL);
		ASSERT(::IsWindow(m_phWndDisable[nIndex]));
		::EnableWindow(m_phWndDisable[nIndex], TRUE);
		++nIndex;
	}
	delete[] (void*)m_phWndDisable;
	m_phWndDisable = NULL;
}

void CFrameWnd::ShowOwnedWindows(BOOL bShow)
{
	// walk through all top-level windows
	HWND hWnd = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
	while (hWnd != NULL)
	{
		if (_AfxIsDescendant(m_hWnd, hWnd) &&
			CWnd::FromHandlePermanent(hWnd) != NULL)
		{
			DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE);
			if (!bShow && (dwStyle & (WS_VISIBLE|WS_DISABLED)) == WS_VISIBLE)
				::ShowWindow(hWnd, SW_HIDE);
			else if (bShow && (dwStyle & WS_VISIBLE) == 0)
				::ShowWindow(hWnd, SW_SHOWNOACTIVATE);
		}
		hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFrameWnd second phase creation

BOOL CFrameWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if (cs.lpszClass == NULL)
		cs.lpszClass = _afxWndFrameOrView;  // COLOR_WINDOW background
	return TRUE;
}

BOOL CFrameWnd::Create(LPCSTR lpszClassName,
	LPCSTR lpszWindowName,
	DWORD dwStyle,
	const RECT& rect,
	CWnd* pParentWnd,
	LPCSTR lpszMenuName,
	DWORD dwExStyle,
	CCreateContext* pContext)
{
	if (pParentWnd == NULL)
		pParentWnd = (CFrameWnd*)AfxGetApp()->m_pMainWnd;

	HMENU hMenu = NULL;
	if (lpszMenuName != NULL)
	{
		// load in a menu that will get destroyed when window gets destroyed
		HINSTANCE hInst = AfxFindResourceHandle(lpszMenuName, RT_MENU);
		if ((hMenu = ::LoadMenu(hInst, lpszMenuName)) == NULL)
		{
			TRACE0("Warning: failed to load menu for CFrameWnd\n");
			PostNcDestroy();            // perhaps delete the C++ object
			return FALSE;
		}
	}

	m_strTitle = lpszWindowName;    // save title for later

	if (!CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle,
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
		pParentWnd->GetSafeHwnd(), hMenu, (LPSTR)pContext))
	{
		TRACE0("Warning: failed to create CFrameWnd\n");
		return FALSE;
	}

	return TRUE;
}


BOOL CFrameWnd::OnCreateClient(LPCREATESTRUCT, CCreateContext* pContext)
{
	// default create client will create a view if asked for it
	if (pContext != NULL)
	{
		// try to create view object from RuntimeClass
		if (pContext->m_pNewViewClass != NULL)
		{
			CWnd* pView = (CWnd*)pContext->m_pNewViewClass->CreateObject();
				// NOTE: can be a CWnd with PostNcDestroy self cleanup

			if (pView == NULL)
			{
				TRACE1("Warning: Dynamic create of view type %Fs failed\n",
					pContext->m_pNewViewClass->m_lpszClassName);
				return FALSE;
			}
			ASSERT(pView->IsKindOf(RUNTIME_CLASS(CWnd)));
			// Views are always created with a border !
			if (!pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
				CRect(0,0,0,0), this, AFX_IDW_PANE_FIRST, pContext))
			{
				TRACE0("Warning: couldn't create view for frame\n");
				return FALSE;       // can't continue without a view
			}
		}
	}

	return TRUE;
}

int CFrameWnd::OnCreate(LPCREATESTRUCT lpcs)
{
	CCreateContext* pContext = (CCreateContext*)
		_AfxGetPtrFromFarPtr(lpcs->lpCreateParams);

	return OnCreateHelper(lpcs, pContext);
}

int CFrameWnd::OnCreateHelper(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	if (CWnd::OnCreate(lpcs) == -1)
		return -1;

	// create special children first
	if (!OnCreateClient(lpcs, pContext))
	{
		TRACE0("Failed to create client pane/view for frame\n");
		return -1;
	}

	// post message for initial message string
	PostMessage(WM_SETMESSAGESTRING, (WPARAM)AFX_IDS_IDLEMESSAGE, 0L);

	// make sure the child windows have been properly sized
	RecalcLayout();

	return 0;   // create ok
}

LPCSTR CFrameWnd::GetIconWndClass(DWORD dwDefaultStyle, UINT nIDResource)
{
	ASSERT_VALID_IDR(nIDResource);

	HINSTANCE hInst = AfxFindResourceHandle(
		MAKEINTRESOURCE(nIDResource), RT_GROUP_ICON);
	HICON hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(nIDResource));
	if (hIcon != NULL)
	{
		CREATESTRUCT cs;
		memset(&cs, 0, sizeof(CREATESTRUCT));
		cs.style = dwDefaultStyle;
		PreCreateWindow(cs);
			// will fill lpszClassName with default WNDCLASS name
			// ignore instance handle from PreCreateWindow.

		WNDCLASS wndcls;
		if (cs.lpszClass != NULL &&
			GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wndcls) &&
			wndcls.hIcon != hIcon)
		{
			// register a very similar WNDCLASS
			return AfxRegisterWndClass(wndcls.style,
				wndcls.hCursor, wndcls.hbrBackground, hIcon);
		}
	}
	return NULL;        // just use the default
}

BOOL CFrameWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
	CWnd* pParentWnd, CCreateContext* pContext)
{
	// only do this once
	ASSERT_VALID_IDR(nIDResource);
	ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

	m_nIDHelp = nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

	CString strFullString;
	if (strFullString.LoadString(nIDResource))
		AfxExtractSubString(m_strTitle, strFullString, 0);    // first sub-string

	if (!Create(GetIconWndClass(dwDefaultStyle, nIDResource),
	  m_strTitle, dwDefaultStyle, rectDefault,
	  pParentWnd, MAKEINTRESOURCE(nIDResource), 0L, pContext))
		return FALSE;   // will self destruct on failure normally

	// save the default menu handle
	ASSERT(m_hWnd != NULL);
	m_hMenuDefault = ::GetMenu(m_hWnd);

	// load accelerator resource
	LoadAccelTable(MAKEINTRESOURCE(nIDResource));

	if (pContext == NULL)   // send initial update
		SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

	return TRUE;
}

void CFrameWnd::OnUpdateFrameMenu(HMENU hMenuAlt)
{
	if (hMenuAlt == NULL)
	{
		// attempt to get default menu from document
		CDocument* pDoc = GetActiveDocument();
		if (pDoc != NULL)
			hMenuAlt = pDoc->GetDefaultMenu();
		// use default menu stored in frame if none from document
		if (hMenuAlt == NULL)
			hMenuAlt = m_hMenuDefault;
	}
	// finally, set the menu
	::SetMenu(m_hWnd, hMenuAlt);
}

/////////////////////////////////////////////////////////////////////////////
// CFrameWnd closing down

void CFrameWnd::OnClose()
{
	if (m_lpfnCloseProc != NULL && !(*m_lpfnCloseProc)(this))
		return;

	// Note: only queries the active document
	CDocument* pDocument = GetActiveDocument();
	if (pDocument != NULL && !pDocument->CanCloseFrame(this))
	{
		// document can't close right now -- don't close it
		return;
	}
	if (AfxGetApp()->m_pMainWnd == this)
	{
		// try to save all documents
		if (pDocument == NULL && !AfxGetApp()->SaveAllModified())
			return;     // don't close it

		// hide the application's windows before closing all the documents
		AfxGetApp()->HideApplication();

		// close all documents first
		AfxGetApp()->CloseAllDocuments(FALSE);

		// don't exit if there are outstanding component objects
		if (!AfxOleCanExitApp())
		{
			// take user out of control of the app
			AfxOleSetUserCtrl(FALSE);

			// don't destroy the main window and close down just yet
			//  (there are outstanding component (OLE) objects)
			return;
		}

		// there are cases where destroying the documents may destroy the
		//  main window of the application.
		if (AfxGetApp()->m_pMainWnd == NULL)
		{
			::PostQuitMessage(0);
			return;
		}
	}

	// allow the document to cleanup before the window is destroyed
	if (pDocument != NULL)
		pDocument->PreCloseFrame(this);

	// then destroy the window
	DestroyWindow();
}

void CFrameWnd::OnDestroy()
{
	// reset menu to default before final shutdown
	if (m_hMenuDefault != NULL && ::GetMenu(m_hWnd) != m_hMenuDefault)
	{
		::SetMenu(m_hWnd, m_hMenuDefault);
		ASSERT(::GetMenu(m_hWnd) == m_hMenuDefault);
	}

	// Automatically quit when the main window is destroyed.
	if (AfxGetApp()->m_pMainWnd == this)
	{
		// closing the main application window
		::WinHelp(m_hWnd, NULL, HELP_QUIT, 0L);
		// will call PostQuitMessage in CWnd::OnNcDestroy
	}
	CWnd::OnDestroy();
}

void CFrameWnd::OnNcDestroy()
{
	// WM_NCDESTROY is the absolute LAST message sent.
#ifndef _USRDLL
	if (AfxGetApp()->m_pMainWnd == this)
	{
		// we don't want to exit if there are outstanding component objects
		if (AfxOleCanExitApp())
		{
			// ok to post quit message and let app exit!
			PostQuitMessage(0);
		}
	}
#endif
	CWnd::OnNcDestroy();        // does detach and PostNcDestroy cleanup
}

/////////////////////////////////////////////////////////////////////////////
// CFrameWnd command/message routing

BOOL CFrameWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	CView* pActiveView = GetActiveView();
	// pump through current view FIRST
	if (pActiveView != NULL &&
	  pActiveView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// then pump through frame
	if (CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// last but not least, pump through app
	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL &&
	  pApp->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	return FALSE;
}

// Delegate scroll messages to active view as well
void CFrameWnd::OnHScroll(UINT, UINT, CScrollBar*)
{
	CWnd* pActiveView = GetActiveView();
	if (pActiveView != NULL)
	{
		const MSG* pMsg = GetCurrentMessage();
		pActiveView->SendMessage(WM_HSCROLL, pMsg->wParam, pMsg->lParam);
	}
}

void CFrameWnd::OnVScroll(UINT, UINT, CScrollBar*)
{
	CWnd* pActiveView = GetActiveView();
	if (pActiveView != NULL)
	{
		const MSG* pMsg = GetCurrentMessage();
		pActiveView->SendMessage(WM_VSCROLL, pMsg->wParam, pMsg->lParam);
	}
}

void CFrameWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	ASSERT_VALID(GetActiveFrame());

	// get active view (use active frame if no active view)
	CView* pActiveView = GetActiveView();
	if (pActiveView == NULL)
		pActiveView = GetActiveFrame()->GetActiveView();

	if (nState != WA_INACTIVE && !bMinimized)
	{
		// when frame gets activated, re-activate current view
		if (pActiveView != NULL)
			pActiveView->OnActivateView(TRUE, pActiveView, pActiveView);
	}

	// always notify the view of frame activations
	if (pActiveView != NULL)
		pActiveView->OnActivateFrame(nState, this);

	CWnd::OnActivate(nState, pWndOther, bMinimized);
}

void CFrameWnd::OnActivateApp(BOOL bActive, HTASK hTask)
{
	if (m_pNotifyHook != NULL)
	{
		// allow OnFrameWindowActivate to be sent to in-place items
		m_pNotifyHook->OnActivate(bActive);
	}

	if (!bActive)
	{
		// exit Shift+F1 help mode if becoming deactivated!
		ExitHelpMode();

		// deactivate current active view
		if (AfxGetApp()->m_pMainWnd == this)
		{
			CView* pActiveView = GetActiveFrame()->GetActiveView();
			if (pActiveView != NULL)
				pActiveView->OnActivateView(FALSE, pActiveView, pActiveView);
		}
	}

	CWnd::OnActivateApp(bActive, hTask);
}

void CFrameWnd::OnSysCommand(UINT nID, LONG lParam)
{
	ASSERT_VALID(GetTopLevelFrame());
	if (!GetTopLevelFrame()->m_bHelpMode)
	{
		// don't interfere with system commands if not in help mode
		CWnd::OnSysCommand(nID, lParam);
		return;
	}

	switch (nID & 0xFFF0)
	{
	case SC_SIZE:
	case SC_MOVE:
	case SC_MINIMIZE:
	case SC_MAXIMIZE:
	case SC_NEXTWINDOW:
	case SC_PREVWINDOW:
	case SC_CLOSE:
	case SC_RESTORE:
	case SC_TASKLIST:
		if (!SendMessage(WM_COMMANDHELP, 0,
		  HID_BASE_COMMAND+ID_COMMAND_FROM_SC(nID & 0xFFF0)))
			SendMessage(WM_COMMAND, ID_DEFAULT_HELP);
		break;

	default:
		// don't interfere with system commands we don't know about
		CWnd::OnSysCommand(nID, lParam);
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// default frame processing

// default drop processing will try to open the file
void CFrameWnd::OnDropFiles(HDROP hDropInfo)
{
	SetActiveWindow();      // activate us first !
	UINT nFiles = ::DragQueryFile(hDropInfo, -1, NULL, 0);

	for (UINT iFile = 0; iFile < nFiles; iFile++)
	{
		char szFileName[_MAX_PATH];
		::DragQueryFile(hDropInfo, iFile, szFileName, _MAX_PATH);
		AfxGetApp()->OpenDocumentFile(szFileName);
	}
	::DragFinish(hDropInfo);
}

// query end session for main frame will try to close it all down
BOOL CFrameWnd::OnQueryEndSession()
{
	if (AfxGetApp()->m_pMainWnd == this)
		return AfxGetApp()->SaveAllModified();

	return TRUE;
}

// when Windows session ends, close all documents
void CFrameWnd::OnEndSession(BOOL bEnding)
{
	if (bEnding && AfxGetApp()->m_pMainWnd == this)
	{
		AfxOleSetUserCtrl(TRUE);    // keeps from randomly shutting down
		AfxGetApp()->CloseAllDocuments(TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Support for Shell DDE Execute messages

LRESULT CFrameWnd::OnDDEInitiate(WPARAM wParam, LPARAM lParam)
{
	CWinApp* pApp = AfxGetApp();
	if (LOWORD(lParam) != 0 && HIWORD(lParam) != 0 &&
	  (ATOM)LOWORD(lParam) == pApp->m_atomApp &&
	  (ATOM)HIWORD(lParam) == pApp->m_atomSystemTopic)
	{
		// make duplicates of the incoming atoms (really adding a reference)
		char szAtomName[_MAX_PATH];
		VERIFY(GlobalGetAtomName(pApp->m_atomApp,
			szAtomName, sizeof szAtomName) != 0);
		VERIFY(GlobalAddAtom(szAtomName) == pApp->m_atomApp);
		VERIFY(GlobalGetAtomName(pApp->m_atomSystemTopic,
			szAtomName, sizeof szAtomName) != 0);
		VERIFY(GlobalAddAtom(szAtomName) == pApp->m_atomSystemTopic);

		// send the WM_DDE_ACK (caller will delete duplicate atoms)
		::SendMessage((HWND)wParam, WM_DDE_ACK, (WPARAM)m_hWnd,
			MAKELPARAM(pApp->m_atomApp, pApp->m_atomSystemTopic));
	}
	return 0L;
}

// always ACK the execute command - even if we do nothing
LRESULT CFrameWnd::OnDDEExecute(WPARAM wParam, LPARAM lParam)
{
	// get the command string
	HGLOBAL hData = (HGLOBAL)HIWORD(lParam);
	char szCommand[_MAX_PATH * 2];
	LPCSTR lpsz = (LPCSTR)GlobalLock(hData);
	_AfxStrCpy(szCommand, lpsz, sizeof(szCommand));
	GlobalUnlock(hData);

	// acknowedge now - before attempting to execute
	::PostMessage((HWND)wParam, WM_DDE_ACK, (WPARAM)m_hWnd,
		MAKELPARAM(0x8000, hData));

	// execute the command
	if (!AfxGetApp()->OnDDECommand(szCommand))
		TRACE1("Error: failed to execute DDE command '%s'\n", szCommand);
	return 0L;
}

LRESULT CFrameWnd::OnDDETerminate(WPARAM wParam, LPARAM lParam)
{
	::PostMessage((HWND)wParam, WM_DDE_TERMINATE, (WPARAM)m_hWnd, lParam);
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// CFrameWnd attributes

CView* CFrameWnd::GetActiveView() const
{
	ASSERT(m_pViewActive == NULL ||
		m_pViewActive->IsKindOf(RUNTIME_CLASS(CView)));
	return m_pViewActive;
}

void CFrameWnd::SetActiveView(CView* pViewNew, BOOL bNotify)
{
#ifdef _DEBUG
	if (pViewNew != NULL)
	{
		ASSERT(IsChild(pViewNew));
		ASSERT(pViewNew->IsKindOf(RUNTIME_CLASS(CView)));
	}
#endif //_DEBUG

	CView* pViewOld = m_pViewActive;
	if (pViewNew == pViewOld)
		return;     // do not re-activate if SetActiveView called more than once

	m_pViewActive = NULL;   // no active for the following processing

	// deactivate the old one
	if (pViewOld != NULL)
		pViewOld->OnActivateView(FALSE, pViewNew, pViewOld);

	// if the OnActivateView moves the active window,
	//    that will veto this change
	if (m_pViewActive != NULL)
		return;     // already set
	m_pViewActive = pViewNew;

	// activate
	if (pViewNew != NULL && bNotify)
		pViewNew->OnActivateView(TRUE, pViewNew, pViewOld);
}

/////////////////////////////////////////////////////////////////////////////
// Special view swapping/activation

void CFrameWnd::OnSetFocus(CWnd* pOldWnd)
{
	if (m_pViewActive != NULL)
		m_pViewActive->SetFocus();
	else
		CWnd::OnSetFocus(pOldWnd);
}

CDocument* CFrameWnd::GetActiveDocument()
{
	ASSERT_VALID(this);
	CView* pView = GetActiveView();
	if (pView != NULL)
		return pView->GetDocument();
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Command prompts

extern HMENU _afxTrackingMenu;

void CFrameWnd::OnInitMenuPopup(CMenu* pMenu, UINT, BOOL bSysMenu)
{
	_AfxCancelModes(m_hWnd);

	if (bSysMenu)
		return;     // don't support system menu

	ASSERT(pMenu != NULL);
	// check the enabled state of various menu items

	CCmdUI state;
	state.m_pMenu = pMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// determine if menu is popup in top-level menu and set m_pOther to
	//  it if so (m_pParentMenu == NULL indicates that it is secondary popup)
	HMENU hParentMenu;
	if (_afxTrackingMenu == pMenu->m_hMenu)
		state.m_pParentMenu = pMenu;    // parent == child for tracking popup
	else
	{
		CWnd* pParent = GetTopLevelParent();
			// child windows don't have menus -- need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pMenu->m_hMenu)
				{
					// when popup is found, m_pParentMenu is containing menu
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
	  state.m_nIndex++)
	{
		state.m_nID = pMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // menu separator or invalid cmd - ignore it

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// possibly a popup menu, route to first item of that popup
			state.m_pSubMenu = pMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // first item of popup can't be routed to
			}
			state.DoUpdate(this, FALSE);    // popups are never auto disabled
		}
		else
		{
			// normal menu item
			// Auto enable/disable if frame window has 'm_bAutoMenuEnable'
			//    set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, m_bAutoMenuEnable && state.m_nID < 0xF000);
		}

		// adjust for menu deletions and additions
		UINT nCount = pMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}

void CFrameWnd::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU /*hSysMenu*/)
{
	// set the tracking state (update on idle)
	if (nFlags == 0xFFFF)
	{
		// cancel menu operation (go back to idle now)
		ASSERT_VALID(GetTopLevelFrame());
		if (!GetTopLevelFrame()->m_bHelpMode)
			m_nIDTracking = AFX_IDS_IDLEMESSAGE;
		else
			m_nIDTracking = AFX_IDS_HELPMODEMESSAGE;
		SendMessage(WM_SETMESSAGESTRING, (WPARAM)m_nIDTracking);
		ASSERT(m_nIDTracking == m_nIDLastMessage);
	}
	else if (nItemID == 0 ||
		nFlags & (MF_SEPARATOR|MF_POPUP|MF_MENUBREAK|MF_MENUBARBREAK))
	{
		// nothing should be displayed
		m_nIDTracking = 0;
	}
	else if (nItemID >= 0xF000 && nItemID < 0xF1F0) // max of 31 SC_s
	{
		// special strings table entries for system commands
		m_nIDTracking = ID_COMMAND_FROM_SC(nItemID);
		ASSERT(m_nIDTracking >= AFX_IDS_SCFIRST &&
			m_nIDTracking < AFX_IDS_SCFIRST + 31);
	}
	else if (nItemID >= AFX_IDM_FIRST_MDICHILD)
	{
		// all MDI Child windows map to the same help id
		m_nIDTracking = AFX_IDS_MDICHILD;
	}
	else
	{
		// track on idle
		m_nIDTracking = nItemID;
	}

	// when running in-place, it is necessary to cause a message to
	//  be pumped through the queue.
	if (m_nIDTracking != m_nIDLastMessage && GetParent() != NULL)
		PostMessage(WM_NULL);
}

LRESULT CFrameWnd::OnSetMessageString(WPARAM wParam, LPARAM lParam)
{
	CWnd* pMessageBar = GetMessageBar();
	if (pMessageBar != NULL)
	{
		LPCSTR lpsz = NULL;
		char szBuffer[256];
		// set the message bar text
		if (lParam != NULL)
		{
			ASSERT(wParam == 0);    // can't have both an ID and a string
			// set an explicit string
			lpsz = (LPCSTR)lParam;
		}
		else if (wParam != 0)
		{
			// use the wParam as a string ID
			if (_AfxLoadString(wParam, szBuffer) != 0)
				lpsz = szBuffer;
			else
				TRACE1("Warning: no message line prompt for ID 0x%04X\n",
					(UINT)wParam);
		}

		if (lpsz != NULL)
		{
			// ignore tooltip text
			LPSTR lpszCR = _AfxStrChr(szBuffer, '\n');
			if (lpszCR != NULL)
				*lpszCR = '\0';
		}

		pMessageBar->SetWindowText(lpsz);
	}

	UINT nIDLast = m_nIDLastMessage;
	m_nIDLastMessage = (UINT)wParam;    // new ID (or 0)
	m_nIDTracking = (UINT)wParam;       // so F1 on toolbar buttons work
	return nIDLast;
}

CWnd* CFrameWnd::GetMessageBar()
{
	return GetDescendantWindow(AFX_IDW_STATUS_BAR, TRUE);
}

void CFrameWnd::OnEnterIdle(UINT nWhy, CWnd* /*pWho*/)
{
	if (nWhy != MSGF_MENU || m_nIDTracking == m_nIDLastMessage)
		return;

	SendMessage(WM_SETMESSAGESTRING, (WPARAM)m_nIDTracking);
	ASSERT(m_nIDTracking == m_nIDLastMessage);
}

/////////////////////////////////////////////////////////////////////////////
// CFrameWnd standard control bar management

void CFrameWnd::OnSysColorChange()
{
	afxData.UpdateSysColors();

	// Recolor the global brushes used by control bars
	if (!(GetStyle() & WS_CHILD))
		SendMessageToDescendants(WM_SYSCOLORCHANGE, 0, 0L, TRUE, TRUE);
}

void CFrameWnd::OnUpdateControlBarMenu(CCmdUI* pCmdUI)
{
	ASSERT(ID_VIEW_STATUS_BAR == AFX_IDW_STATUS_BAR);
	ASSERT(ID_VIEW_TOOLBAR == AFX_IDW_TOOLBAR);

	CWnd* pBar;
	if ((pBar = GetDescendantWindow(pCmdUI->m_nID, TRUE)) == NULL)
	{
		pCmdUI->ContinueRouting();
		return; // not for us
	}

	pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
}

BOOL CFrameWnd::OnBarCheck(UINT nID)
{
	ASSERT(ID_VIEW_STATUS_BAR == AFX_IDW_STATUS_BAR);
	ASSERT(ID_VIEW_TOOLBAR == AFX_IDW_TOOLBAR);

	CWnd* pBar;
	if ((pBar = GetDescendantWindow(nID, TRUE)) == NULL)
		return FALSE;   // not for us

	// toggle visible state
	pBar->ShowWindow((pBar->GetStyle() & WS_VISIBLE) == 0 ? SW_SHOW : SW_HIDE);
	RecalcLayout();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Support for standard status bar

void CFrameWnd::OnUpdateKeyIndicator(CCmdUI* pCmdUI)
{
	UINT nVK;

	switch (pCmdUI->m_nID)
	{
	case ID_INDICATOR_CAPS:
		nVK = VK_CAPITAL;
		break;

	case ID_INDICATOR_NUM:
		nVK = VK_NUMLOCK;
		break;

	case ID_INDICATOR_SCRL:
		nVK = VK_SCROLL;
		break;

	default:
		TRACE1("Warning: OnUpdateKeyIndicator - unknown indicator 0x%04X\n",
			pCmdUI->m_nID);
		pCmdUI->ContinueRouting();
		return; // not for us
	}

	pCmdUI->Enable(::GetKeyState(nVK) & 1);
		// enable static text based on toggled key state
	ASSERT(pCmdUI->m_bEnableChanged);
}

/////////////////////////////////////////////////////////////////////////////
// Setting title of frame window - UISG standard

void CFrameWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave it alone!

	// allow hook to set the title (used for OLE 2.0 support)
	if (m_pNotifyHook != NULL && m_pNotifyHook->OnUpdateFrameTitle())
		return;

	CDocument* pDocument = GetActiveDocument();
	if (bAddToTitle && pDocument != NULL)
		UpdateFrameTitleForDocument(pDocument->GetTitle());
	else
		UpdateFrameTitleForDocument(NULL);
}

void CFrameWnd::UpdateFrameTitleForDocument(const char* pszDocName)
{
	// get old text for comparison against new text
	char szOld[256];
	GetWindowText(szOld, sizeof(szOld));

	// copy first part of title loaded at time of frame creation
	char szText[256];
	lstrcpy(szText, m_strTitle);

	// get name of currently active view
	if (pszDocName != NULL)
	{
		lstrcat(szText, " - ");
		lstrcat(szText, pszDocName);
		// add current window # if needed
		if (m_nWindow > 0)
			wsprintf(szText + lstrlen(szText), ":%d", m_nWindow);
	}

	// set title if changed, but don't remove completely
			// NOTE: will be excessive for MDI Frame with maximized child
	if (lstrcmp(szText, szOld) != 0)
		SetWindowText(szText);
}

/////////////////////////////////////////////////////////////////////////////

void CFrameWnd::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{
	// default implementation changes control bars, menu and main pane window

	CFrameWnd* pActiveFrame = GetActiveFrame();
	ASSERT_VALID(pActiveFrame);
	if (bPreview && pActiveFrame->m_pNotifyHook != NULL)
		pActiveFrame->m_pNotifyHook->OnDocActivate(FALSE);

	// Set visibility of standard ControlBars (only the first 32)
	DWORD dwOldStates = 0;
	HWND hWnd;
	for (hWnd = ::GetTopWindow(m_hWnd); hWnd != NULL;
								hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT))
	{
		CControlBar* pBar;
		UINT nID = _AfxGetDlgCtrlID(hWnd);
		if (nID >= AFX_IDW_CONTROLBAR_FIRST &&
			nID <= AFX_IDW_CONTROLBAR_FIRST+31 &&
			(pBar = (CControlBar*)CWnd::FromHandlePermanent(hWnd)) != NULL &&
			pBar->IsKindOf(RUNTIME_CLASS(CControlBar)) &&
			pBar->GetOwner() == this)
		{
			DWORD dwMask = 1L << (nID - AFX_IDW_CONTROLBAR_FIRST);

			if (pBar->IsVisible())
				dwOldStates |= dwMask;      // save if previously visible

			if (pState->dwStates & dwMask)
				pBar->DelayShow();
			else
				pBar->DelayHide();
		}
	}
	pState->dwStates = dwOldStates; // save for restore

	if (bPreview)
	{
		// Entering Print Preview

		ASSERT(m_lpfnCloseProc == NULL);    // no chaining
		m_lpfnCloseProc = pState->lpfnCloseProc;

		// Get rid of the menu first (will resize the window)
		pState->hMenu = ::GetMenu(m_hWnd);
		if (pState->hMenu != NULL)
		{
			// Invalidate before SetMenu since we are going to replace
			//  the frame's client area anyway
			Invalidate();
			SetMenu(NULL);
			m_nIdleFlags &= ~idleMenu;  // avoid any idle menu processing
		}

		// Save the accelerator table and remove it.
		pState->hAccelTable = m_hAccelTable;
		m_hAccelTable = NULL;

		// Hide the main pane
		hWnd = ::GetDlgItem(m_hWnd, pState->nIDMainPane);
		ASSERT(hWnd != NULL);       // must be one that we are hiding!
		::ShowWindow(hWnd, SW_HIDE);

		// Make room for the PreviewView by changing AFX_IDW_PANE_FIRST's ID
		// to AFX_IDW_PREVIEW_FIRST
		if (pState->nIDMainPane != AFX_IDW_PANE_FIRST)
			hWnd = ::GetDlgItem(m_hWnd, AFX_IDW_PANE_FIRST);
		if (hWnd != NULL)
			_AfxSetDlgCtrlID(hWnd, AFX_IDW_PANE_SAVE);

		if ((::GetWindowLong(m_hWnd, GWL_STYLE) & (WS_HSCROLL|WS_VSCROLL)) != 0)
		{
			TRACE0("Warning: Scroll Bars in Frame Windows may cause"
				" unusual behaviour\n");
		}
	}
	else
	{
		// Leaving Preview

		m_lpfnCloseProc = NULL;

		// shift original AFX_IDW_PANE_FIRST back to its rightful ID
		hWnd = ::GetDlgItem(m_hWnd, AFX_IDW_PANE_SAVE);
		if (hWnd != NULL)
			_AfxSetDlgCtrlID(hWnd, AFX_IDW_PANE_FIRST);

		// now show main pane that was hidden
		if (pState->nIDMainPane != AFX_IDW_PANE_FIRST)
			hWnd = ::GetDlgItem(m_hWnd, pState->nIDMainPane);
		ASSERT(hWnd != NULL);
		::ShowWindow(hWnd, SW_SHOW);

		// put the menu back in place if it was removed before
		if (pState->hMenu != NULL)
		{
			// Invalidate before SetMenu since we are going to replace
			//  the frame's client area anyway
			Invalidate();
			::SetMenu(m_hWnd, pState->hMenu);
		}

		if (pActiveFrame->m_pNotifyHook != NULL)
			pActiveFrame->m_pNotifyHook->OnDocActivate(TRUE);

		// Restore the Accelerator table
		m_hAccelTable = pState->hAccelTable;
	}
}

void CFrameWnd::DelayUpdateFrameMenu(HMENU hMenuAlt)
{
	m_hMenuAlt = hMenuAlt;
	m_nIdleFlags |= idleMenu;
}

LRESULT CFrameWnd::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	// update menu if necessary
	if (m_nIdleFlags & idleMenu)
		OnUpdateFrameMenu(m_hMenuAlt);

	// update title if necessary
	if (m_nIdleFlags & idleTitle)
		OnUpdateFrameTitle(TRUE);

	// recalc layout if necessary
	if (m_nIdleFlags & idleLayout)
	{
		RecalcLayout(m_nIdleFlags & idleNotify);
		UpdateWindow();
	}

	// set the current message string if necessary
	if (m_nIDTracking != m_nIDLastMessage)
	{
		SendMessage(WM_SETMESSAGESTRING, (WPARAM)m_nIDTracking);
		ASSERT(m_nIDTracking == m_nIDLastMessage);
	}
	m_nIdleFlags = 0;

	return 0;
}

CFrameWnd* CFrameWnd::GetActiveFrame()
{
	// by default, the active frame is the frame itself
	//  (MDI is different)
	return this;
}

void CFrameWnd::RecalcLayout(BOOL bNotify)
{
	// call the layout hook -- OLE 2.0 support uses this hook
	if (bNotify && m_pNotifyHook != NULL)
		m_pNotifyHook->OnRecalcLayout();

	// reposition all the child windows (regardless of ID)
	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposExtra, &m_rectBorder);
}

// CFrameWnd implementation of OLE border space negotiation
BOOL CFrameWnd::NegotiateBorderSpace(UINT nBorderCmd, LPRECT lpRectBorder)
{
	CRect border, request;

	switch (nBorderCmd)
	{
	case borderGet:
		ASSERT(lpRectBorder != NULL);
		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery,
			lpRectBorder);
		break;

	case borderRequest:
		// always OK to set empty rectangle or when window is iconic
		if (lpRectBorder == NULL || IsIconic())
			return TRUE;

		// get border space available
		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &border);

		// calculate what the new border space would be
		request.left = border.left + lpRectBorder->left;
		request.top = border.top + lpRectBorder->top;
		request.right = border.right - lpRectBorder->right;
		request.bottom = border.bottom - lpRectBorder->bottom;

		// return FALSE if rect is empty, otherwise TRUE
		return request.right > request.left && request.bottom > request.top;

	case borderSet:
		if (lpRectBorder == NULL)
		{
			if (!m_rectBorder.IsRectNull())
			{
				// releasing all border space -- recalc needed
				m_rectBorder.SetRectEmpty();
				return TRUE;
			}
			// original rect is empty & lpRectBorder is NULL, no recalc needed
			return FALSE;
		}
		if (lpRectBorder == NULL || !m_rectBorder.EqualRect(lpRectBorder))
		{
			// the rects are different -- recalc needed
			m_rectBorder.CopyRect(lpRectBorder);
			return TRUE;
		}
		return FALSE;   // no recalc needed

	default:
		ASSERT(FALSE);  // invalid CFrameWnd::BorderCmd
	}

	return TRUE;
}

void CFrameWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);    // important for MDI Children
	if (nType != SIZE_MINIMIZED)
		RecalcLayout();
}

BOOL CFrameWnd::OnEraseBkgnd(CDC* pDC)
{
	if (m_pViewActive != NULL)
		return TRUE;        // active view will erase/paint itself
	// for view-less frame just use the default background fill
	return CWnd::OnEraseBkgnd(pDC);
}

void CFrameWnd::ActivateFrame(int nCmdShow)
	// nCmdShow is the normal show mode this frame should be in
{
	if (!IsWindowVisible())
		ShowWindow((nCmdShow == -1) ? SW_SHOW : nCmdShow); // show it
	else if (IsIconic())
		ShowWindow((nCmdShow == -1) ? SW_RESTORE : nCmdShow);
	else if (nCmdShow == SW_HIDE)
		ShowWindow(nCmdShow);

	if (GetStyle() & WS_VISIBLE)
	{
		BringWindowToTop();
		HWND hWndLastPop = ::GetLastActivePopup(m_hWnd);
		if (hWndLastPop != NULL && hWndLastPop != m_hWnd)
			::BringWindowToTop(hWndLastPop);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFrameWnd Diagnostics

#ifdef _DEBUG
void CFrameWnd::AssertValid() const
{
	CWnd::AssertValid();
	if (m_pViewActive != NULL)
		ASSERT_VALID(m_pViewActive);
}

void CFrameWnd::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
	AFX_DUMP1(dc, "\nm_hAccelTable = ", (UINT)m_hAccelTable);
	AFX_DUMP1(dc, "\nm_nWindow = ", m_nWindow);
	AFX_DUMP1(dc, "\nm_nIDHelp = ", m_nIDHelp);
	AFX_DUMP1(dc, "\nm_nIDTracking = ", m_nIDTracking);
	AFX_DUMP1(dc, "\nm_nIDLastMessage = ", m_nIDLastMessage);
	AFX_DUMP0(dc, "\nm_pViewActive = ");
	if (m_pViewActive)
		dc << m_pViewActive;
	else
		AFX_DUMP0(dc, "none");
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
