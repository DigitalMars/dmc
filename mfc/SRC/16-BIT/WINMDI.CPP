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

#ifdef AFX_CORE4_SEG
#pragma code_seg(AFX_CORE4_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDIFrameWnd

IMPLEMENT_DYNCREATE(CMDIFrameWnd, CFrameWnd)

BEGIN_MESSAGE_MAP(CMDIFrameWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CMDIFrameWnd)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_COMMANDHELP, OnCommandHelp)
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_SIZE()

	// MDI Window messages
	ON_UPDATE_COMMAND_UI(ID_WINDOW_ARRANGE, OnUpdateMDIWindowCmd)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_CASCADE, OnUpdateMDIWindowCmd)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_TILE_HORZ, OnUpdateMDIWindowCmd)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_TILE_VERT, OnUpdateMDIWindowCmd)
	ON_COMMAND_EX(ID_WINDOW_ARRANGE, OnMDIWindowCmd)
	ON_COMMAND_EX(ID_WINDOW_CASCADE, OnMDIWindowCmd)
	ON_COMMAND_EX(ID_WINDOW_TILE_HORZ, OnMDIWindowCmd)
	ON_COMMAND_EX(ID_WINDOW_TILE_VERT, OnMDIWindowCmd)
	// WindowNew = NewWindow
	ON_UPDATE_COMMAND_UI(ID_WINDOW_NEW, OnUpdateMDIWindowCmd)
	ON_COMMAND(ID_WINDOW_NEW, OnWindowNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CMDIFrameWnd::CMDIFrameWnd()
{
	m_hWndMDIClient = NULL;
}

BOOL CMDIFrameWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// send to MDI child first - will be re-sent through OnCmdMsg later
	CMDIChildWnd* pActiveChild = MDIGetActive();
	if (pActiveChild != NULL && _AfxCallWndProc(pActiveChild,
	  pActiveChild->m_hWnd, WM_COMMAND, wParam, lParam) != 0)
		return TRUE; // handled by child

	if (CFrameWnd::OnCommand(wParam, lParam))
		return TRUE; // handled through normal mechanism (MDI child or frame)

	HWND hWndCtrl = (HWND)LOWORD(lParam);

	ASSERT(AFX_IDM_FIRST_MDICHILD == 0xFF00);
	if (hWndCtrl == NULL && (LOWORD(wParam) & 0xf000) == 0xf000)
	{
		// menu or accelerator within range of MDI children
		// default frame proc will handle it
		DefWindowProc(WM_COMMAND, wParam, lParam);
		return TRUE;
	}

	return FALSE;   // not handled
}

BOOL CMDIFrameWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	CMDIChildWnd* pActiveChild = MDIGetActive();
	// pump through active child FIRST
	if (pActiveChild != NULL &&
	  pActiveChild->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// then pump through normal frame
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


LRESULT CMDIFrameWnd::OnCommandHelp(WPARAM wParam, LPARAM lParam)
{
	if (lParam == 0 && IsTracking())
		lParam = HID_BASE_COMMAND+m_nIDTracking;

	CMDIChildWnd* pActiveChild = MDIGetActive();
	if (pActiveChild != NULL && _AfxCallWndProc(pActiveChild,
	  pActiveChild->m_hWnd, WM_COMMANDHELP, wParam, lParam) != 0)
	{
		// handled by child
		return TRUE;
	}

	if (CFrameWnd::OnCommandHelp(wParam, lParam))
	{
		// handled by our base
		return TRUE;
	}

	if (lParam != 0)
	{
		AfxGetApp()->WinHelp(lParam);
		return TRUE;
	}
	return FALSE;
}

BOOL CMDIFrameWnd::OnCreateClient(LPCREATESTRUCT lpcs,
		CCreateContext* /*pContext*/)
{
	CMenu* pMenu = NULL;
	if (m_hMenuDefault == NULL)
	{
		// default implementation for MFC V1 backward compatibility
		pMenu = GetMenu();
		ASSERT(pMenu != NULL);
		// This is attempting to guess which sub-menu is the Window menu.
		// The Windows user interface guidelines say that the right-most
		// menu on the menu bar should be Help and Window should be one
		// to the left of that.
		int iMenu = pMenu->GetMenuItemCount() - 2;

		// If this assertion fails, your menu bar does not follow the guidelines
		// so you will have to override this function and call CreateClient
		// appropriately or use the MFC V2 MDI functionality.
		ASSERT(iMenu >= 0);
		pMenu = pMenu->GetSubMenu(iMenu);
		ASSERT(pMenu != NULL);
	}
	return CreateClient(lpcs, pMenu);
}

BOOL CMDIFrameWnd::CreateClient(LPCREATESTRUCT lpCreateStruct,
	CMenu* pWindowMenu)
{
	ASSERT(m_hWnd != NULL);
	ASSERT(m_hWndMDIClient == NULL);
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_BORDER |
		WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
		MDIS_ALLCHILDSTYLES;    // allow children to be created invisible
	// will be inset by the frame

	CLIENTCREATESTRUCT ccs;
	ccs.hWindowMenu = pWindowMenu->GetSafeHmenu();
		// set hWindowMenu for MFC V1 backward compatibility
		// for MFC V2, window menu will be set in OnMDIActivate
	ccs.idFirstChild = AFX_IDM_FIRST_MDICHILD;

	if (lpCreateStruct->style & (WS_HSCROLL | WS_VSCROLL))
	{
		// parent MDIFrame's scroll styles move to the MDICLIENT
		dwStyle |= (lpCreateStruct->style & (WS_HSCROLL | WS_VSCROLL));

		// fast way to turn off the scrollbar bits (without a resize)
		::SetWindowLong(m_hWnd, GWL_STYLE,
			::GetWindowLong(m_hWnd, GWL_STYLE) & ~(WS_HSCROLL | WS_VSCROLL));
		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
			SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW |
				SWP_NOACTIVATE | SWP_FRAMECHANGED);
	}

	// Create MDICLIENT control with special IDC
	static char BASED_CODE _szMdiClient[] = "mdiclient";

	if ((m_hWndMDIClient = ::CreateWindowEx(0, _szMdiClient, NULL,
		dwStyle, 0, 0, 0, 0, m_hWnd, (HMENU)AFX_IDW_PANE_FIRST,
		AfxGetInstanceHandle(),
		(LPSTR)(LPCLIENTCREATESTRUCT)&ccs)) == NULL)
	{
		TRACE0("Warning: CMDIFrameWnd::OnCreateClient: failed to create MDICLIENT\n");
		return FALSE;
	}

	return TRUE;
}

LRESULT CMDIFrameWnd::DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	return ::DefFrameProc(m_hWnd, m_hWndMDIClient, nMsg, wParam, lParam);
}

BOOL CMDIFrameWnd::PreTranslateMessage(MSG* pMsg)
{
	// check for special cancel modes for ComboBoxes
	if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_NCLBUTTONDOWN)
		_AfxCancelModes(pMsg->hwnd);    // filter clicks

	// allow hook to consume message
	if (m_pNotifyHook != NULL && m_pNotifyHook->OnPreTranslateMessage(pMsg))
		return TRUE;

	CMDIChildWnd* pActiveChild = MDIGetActive();

	// current active child gets first crack at it
	if (pActiveChild != NULL &&
		pActiveChild->PreTranslateMessage(pMsg))
		return TRUE;

	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		// translate accelerators for frame and any children
		if (m_hAccelTable != NULL &&
			::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
		{
			return TRUE;
		}

		// special processing for MDI accelerators last
		// and only if it is not in SDI mode (print preview)
		if (GetActiveView() == NULL)
		{
			if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
			{
				// the MDICLIENT window may translate it
				if (::TranslateMDISysAccel(m_hWndMDIClient, pMsg))
					return TRUE;
			}
		}
	}

	return FALSE;
}

void CMDIFrameWnd::DelayUpdateFrameMenu(HMENU hMenuAlt)
{
	OnUpdateFrameMenu(hMenuAlt);

	m_nIdleFlags |= idleMenu;
}

LRESULT CMDIFrameWnd::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	if (m_nIdleFlags & idleMenu)
	{
		DrawMenuBar();
		m_nIdleFlags &= ~idleMenu;
	}
	return CFrameWnd::OnIdleUpdateCmdUI(wParam, lParam);
}

CFrameWnd* CMDIFrameWnd::GetActiveFrame()
{
	CMDIChildWnd* pActiveChild = MDIGetActive();
	if (pActiveChild == NULL)
		return this;
	return pActiveChild;
}

BOOL CMDIFrameWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if (cs.lpszClass == NULL)
		cs.lpszClass = _afxWndMDIFrame;
	return TRUE;
}

BOOL CMDIFrameWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
	CWnd* pParentWnd, CCreateContext* pContext)
{
	if (!CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle,
	  pParentWnd, pContext))
		return FALSE;

	// save menu to use when no active MDI child window is present
	ASSERT(m_hWnd != NULL);
	m_hMenuDefault = ::GetMenu(m_hWnd);
	if (m_hMenuDefault == NULL)
		TRACE0("Warning: CMDIFrameWnd without a default menu\n");
	return TRUE;
}


void CMDIFrameWnd::OnDestroy()
{
	CFrameWnd::OnDestroy();     // exit and misc cleanup

	// owned menu stored in shared slot for MDIFRAME
	if (m_hMenuDefault != NULL && ::GetMenu(m_hWnd) != m_hMenuDefault)
	{
		// must go through MDI client to get rid of MDI menu additions
		::SendMessage(m_hWndMDIClient, WM_MDISETMENU, 0,
			MAKELONG(m_hMenuDefault, NULL));
		ASSERT(::GetMenu(m_hWnd) == m_hMenuDefault);
	}
}

void CMDIFrameWnd::OnSize(UINT nType, int, int)
{
	// do not call default - it will reposition the MDICLIENT
	if (nType != SIZE_MINIMIZED)
		RecalcLayout();
}

CMDIChildWnd* CMDIFrameWnd::MDIGetActive(BOOL* pbMaximized /*= NULL*/) const
{
	LRESULT l;
	l = ::SendMessage(m_hWndMDIClient, WM_MDIGETACTIVE, 0, 0);
	CMDIChildWnd* pWnd = (CMDIChildWnd*)CWnd::FromHandle((HWND)LOWORD(l));
	ASSERT(pWnd == NULL || pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));

	// check for special pseudo-inactive state
	if (pWnd != NULL && pWnd->m_bPseudoInactive &&
		(pWnd->GetStyle() & WS_VISIBLE) == 0)
	{
		// Window is hidden, active, but m_bPseudoInactive -- return NULL
		pWnd = NULL;
	}

	if (pbMaximized != NULL)
		*pbMaximized = pWnd != NULL && HIWORD(l);

	return pWnd;
}

/////////////////////////////////////////////////////////////////////////////
// CMDIFrameWnd Diagnostics

#ifdef _DEBUG
void CMDIFrameWnd::AssertValid() const
{
	CFrameWnd::AssertValid();
	if (afxData.bWin31)
	{
		ASSERT(m_hWndMDIClient == NULL || ::IsWindow(m_hWndMDIClient));
		ASSERT(m_hMenuDefault == NULL || ::IsMenu(m_hMenuDefault));
	}
}

void CMDIFrameWnd::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);

	AFX_DUMP1(dc, "\nm_hWndMDIClient = ", (UINT)m_hWndMDIClient);
	AFX_DUMP1(dc, "\nm_hMenuDefault = ", (UINT)m_hMenuDefault);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CMDIChildWnd

IMPLEMENT_DYNCREATE(CMDIChildWnd, CFrameWnd)

BEGIN_MESSAGE_MAP(CMDIChildWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CMDIChildWnd)
	ON_WM_MDIACTIVATE()
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CMDIChildWnd::CMDIChildWnd()
{
	m_hMenuShared = NULL;
	m_bPseudoInactive = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMDIChildWnd special processing

LRESULT CMDIChildWnd::DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	return ::DefMDIChildProc(m_hWnd, nMsg, wParam, lParam);
}

BOOL CMDIChildWnd::DestroyWindow()
{
	if (m_hWnd == NULL)
		return FALSE;

	// avoid changing the caption during the destroy message(s)
	CMDIFrameWnd* pFrameWnd = GetMDIFrame();
    HWND hWndFrame = pFrameWnd->m_hWnd;
	ASSERT(::IsWindow(hWndFrame));
    DWORD dwStyle = SetWindowLong(hWndFrame, GWL_STYLE, 
		GetWindowLong(hWndFrame, GWL_STYLE) & ~FWS_ADDTOTITLE);
  	MDIDestroy();
	if (::IsWindow(hWndFrame))
	{
		ASSERT(hWndFrame == pFrameWnd->m_hWnd);
    	SetWindowLong(hWndFrame, GWL_STYLE, dwStyle);
		pFrameWnd->OnUpdateFrameTitle(TRUE);
	}

	return TRUE;
}

BOOL CMDIChildWnd::PreTranslateMessage(MSG* pMsg)
{
	// we can't call 'CFrameWnd::PreTranslate' since it will translate
	//  accelerators in the context of the MDI Child - but since MDI Child
	//  windows don't have menus this doesn't work properly.  MDI Child
	//  accelerators must be translated in context of their MDI Frame.

	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		// use document specific accelerator table over m_hAccelTable
		HACCEL hAccelTable = m_hAccelTable;
		HACCEL hAccel;
		CDocument* pDoc = GetActiveDocument();
		if (pDoc != NULL && (hAccel = pDoc->GetDefaultAccel()) != NULL)
			hAccelTable = hAccel;

		return (hAccelTable != NULL &&
		   ::TranslateAccelerator(GetMDIFrame()->m_hWnd, hAccelTable, pMsg));
	}

	return FALSE;
}

BOOL CMDIChildWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	ASSERT(cs.style & WS_CHILD);
		// MFC V2 requires that MDI Children are created with proper styles,
		//  usually: WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW.
		// See Technical note TN019 for more details on MFC V1->V2 migration.

	if (cs.lpszClass == NULL)
		cs.lpszClass = _afxWndFrameOrView;

	// Win 30 does not support invisible children, so we create it
	//  minimized then ActivateFrame will restore it
	if (!afxData.bWin31 && !(cs.style & WS_VISIBLE))
		cs.style |= WS_MINIMIZE;

	return TRUE;
}

BOOL CMDIChildWnd::Create(LPCSTR lpszClassName,
	LPCSTR lpszWindowName, DWORD dwStyle,
	const RECT& rect, CMDIFrameWnd* pParentWnd,
	CCreateContext* pContext)
{
	if (pParentWnd == NULL)
	{
		CWnd* pMainWnd = AfxGetApp()->m_pMainWnd;
		ASSERT(pMainWnd != NULL);
		ASSERT(pMainWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)));
		pParentWnd = (CMDIFrameWnd*)pMainWnd;
	}

	// first copy into a CREATESTRUCT for PreCreate
	CREATESTRUCT cs;
	cs.dwExStyle = 0L;
	cs.lpszClass = lpszClassName;
	cs.lpszName = lpszWindowName;
	cs.style = dwStyle;
	cs.x = rect.left;
	cs.y = rect.top;
	cs.cx = rect.right - rect.left;
	cs.cy = rect.bottom - rect.top;
	cs.hwndParent = pParentWnd->m_hWnd;
	cs.hMenu = NULL;
	cs.hInstance = AfxGetInstanceHandle();
	cs.lpCreateParams = (LPVOID)pContext;

	if (!PreCreateWindow(cs))
	{
		PostNcDestroy();
		return FALSE;
	}
	ASSERT(cs.dwExStyle == 0);  // must be zero for MDI Children
	ASSERT(cs.hwndParent == pParentWnd->m_hWnd);    // must not change

	// now copy into a MDICREATESTRUCT for real create
	MDICREATESTRUCT mcs;
	mcs.szClass = cs.lpszClass;
	mcs.szTitle = cs.lpszName;
	mcs.hOwner = cs.hInstance;
	mcs.x = cs.x;
	mcs.y = cs.y;
	mcs.cx = cs.cx;
	mcs.cy = cs.cy;
	mcs.style = cs.style;
	mcs.lParam = (LONG)cs.lpCreateParams;

	// Win 30 requires we have an MDI Window menu before creation
	if (!afxData.bWin31 && m_hMenuShared != NULL &&
	  ::GetMenu(pParentWnd->m_hWnd) == pParentWnd->m_hMenuDefault)
		::SendMessage(pParentWnd->m_hWndMDIClient, WM_MDISETMENU, 0,
			MAKELONG(NULL, pParentWnd->GetWindowMenuPopup(m_hMenuShared)));

	_AfxHookWindowCreate(this);
	HWND hWnd = (HWND)::SendMessage(pParentWnd->m_hWndMDIClient,
		WM_MDICREATE, 0, (LONG)(LPSTR)&mcs);
	if (!_AfxUnhookWindowCreate())
		PostNcDestroy();        // cleanup if MDICREATE fails too soon

	pParentWnd->MDIGetActive();

	if (hWnd == NULL)
		return FALSE;
	ASSERT(hWnd == m_hWnd);
	return TRUE;
}

BOOL CMDIChildWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
		CWnd* pParentWnd, CCreateContext* pContext)
{
	// only do this once
	ASSERT_VALID_IDR(nIDResource);
	ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);
	ASSERT(m_hMenuShared == NULL);      // only do once

	m_nIDHelp = nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

	// parent must be MDI Frame (or NULL for default)
	ASSERT(pParentWnd == NULL || pParentWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)));
	// will be a child of MDIClient
	ASSERT(!(dwDefaultStyle & WS_POPUP));
	dwDefaultStyle |= WS_CHILD;

	// if available - get MDI child menus from doc template
	ASSERT(m_hMenuShared == NULL);      // only do once
	CMultiDocTemplate* pTemplate;
	if (pContext != NULL &&
		(pTemplate = (CMultiDocTemplate*)pContext->m_pNewDocTemplate) != NULL)
	{
		ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CMultiDocTemplate)));
		// get shared menu from doc template
		m_hMenuShared = pTemplate->m_hMenuShared;
		m_hAccelTable = pTemplate->m_hAccelTable;
	}
	else
	{
		TRACE0("Warning: no shared menu/acceltable for MDI Child window\n");
			// if this happens, programmer must load these manually
	}

	CString strFullString, strTitle;
	if (strFullString.LoadString(nIDResource))
		AfxExtractSubString(strTitle, strFullString, 0);    // first sub-string

	ASSERT(m_hWnd == NULL);
	if (!Create(GetIconWndClass(dwDefaultStyle, nIDResource),
	  strTitle, dwDefaultStyle, rectDefault,
	  (CMDIFrameWnd*)pParentWnd, pContext))
	{
		return FALSE;	// will self destruct on failure normally
	}

	// it worked !
	return TRUE;
}


void CMDIChildWnd::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// update our parent frame - in case we are now maximized or not
	GetMDIFrame()->OnUpdateFrameTitle(TRUE);
}

void CMDIChildWnd::ActivateFrame(int nCmdShow)
{
	BOOL bVisibleThen = (GetStyle() & WS_VISIBLE) != 0;

	CMDIFrameWnd* pFrameWnd = GetMDIFrame();
	BOOL bMaximized;
	pFrameWnd->MDIGetActive(&bMaximized);
	if (nCmdShow == -1 && bMaximized)
		nCmdShow = SW_SHOWMAXIMIZED;    // maximize if previous frame was
	CFrameWnd::ActivateFrame(nCmdShow);

	// Note: Update the m_bPseudoInactive flag.  This is used to handle the
	//  last MDI child getting hidden.  Windows provides no way to deactivate
	//  an MDI child window.

	BOOL bVisibleNow = (GetStyle() & WS_VISIBLE) != 0;
	if (bVisibleNow == bVisibleThen)
		return;

	if (!bVisibleNow)
	{
		// get current active window according to Windows MDI
		LRESULT l;
		l = ::SendMessage(pFrameWnd->m_hWndMDIClient, WM_MDIGETACTIVE, 0, 0);
		HWND hWnd = (HWND)LOWORD(l);
		if (hWnd != m_hWnd)
		{
			// not active any more -- window must have been deactivated
			ASSERT(!m_bPseudoInactive);
			return;
		}

		// try next window
		ASSERT(hWnd != NULL);
		pFrameWnd->MDINext();

		// see if it has been deactivated now...
		l = ::SendMessage(pFrameWnd->m_hWndMDIClient, WM_MDIGETACTIVE, 0, 0);
		hWnd = (HWND)LOWORD(l);
		if (hWnd == m_hWnd)
		{
			// still active -- fake deactivate it
			ASSERT(hWnd != NULL);
			OnMDIActivate(FALSE, NULL, this);
			m_bPseudoInactive = TRUE;   // so MDIGetActive returns NULL
		}
	}
	else if (m_bPseudoInactive)
	{
		// if state transitioned from not visible to visible, but
		//  was pseudo deactivated -- send activate notify now
		OnMDIActivate(TRUE, this, NULL);
		ASSERT(!m_bPseudoInactive); // should get set in OnMDIActivate!
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMDIChildWnd Diagnostics

#ifdef _DEBUG
void CMDIChildWnd::AssertValid() const
{
	CFrameWnd::AssertValid();
	if (afxData.bWin31)
	{
		ASSERT(m_hMenuShared == NULL || ::IsMenu(m_hMenuShared));
	}
}

void CMDIChildWnd::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
	AFX_DUMP1(dc, "\nm_hMenuShared = ", (UINT)m_hMenuShared);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Smarts for the "Window" menu

HMENU CMDIFrameWnd::GetWindowMenuPopup(HMENU hMenuBar)
	// find which popup is the "Window" menu
{
	if (hMenuBar == NULL)
		return NULL;

	int iItem = ::GetMenuItemCount(hMenuBar);
	while (iItem--)
	{
		HMENU hMenuPop = ::GetSubMenu(hMenuBar, iItem);
		if (hMenuPop != NULL)
		{
			int iItemMax = ::GetMenuItemCount(hMenuPop);
			for (int iItemPop = 0; iItemPop < iItemMax; iItemPop++)
			{
				UINT nID = GetMenuItemID(hMenuPop, iItemPop);
				if (nID >= AFX_IDM_WINDOW_FIRST && nID <= AFX_IDM_WINDOW_LAST)
					return hMenuPop;
			}
		}
	}

	// no default menu found
	TRACE0("WARNING: GetWindowMenuPopup failed!\n");
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Smarts for updating the window menu based on the current child

void CMDIFrameWnd::OnUpdateFrameMenu(HMENU hMenuAlt)
{
	CMDIChildWnd* pActiveWnd = MDIGetActive();
	if (pActiveWnd != NULL)
	{
		// let child update the menu bar
		pActiveWnd->OnUpdateFrameMenu(TRUE, pActiveWnd, hMenuAlt);
	}
	else
	{
		// no child active, so have to update it ourselves
		//  (we can't send it to a child window, since pActiveWnd is NULL)
		if (hMenuAlt == NULL)
			hMenuAlt = m_hMenuDefault;  // use default
		HMENU hMenuLast = NULL;
		if (!afxData.bWin31)    // Windows 3.0 needs to remove the separator
			hMenuLast = GetWindowMenuPopup(::GetMenu(m_hWnd));
		::SendMessage(m_hWndMDIClient, WM_MDISETMENU, 0,
			MAKELONG(hMenuAlt, hMenuLast));
	}
}

/////////////////////////////////////////////////////////////////////////////
// MDI Child Extensions

// walk up two parents for MDIFrame that owns MDIChild (skip MDIClient)
CMDIFrameWnd* CMDIChildWnd::GetMDIFrame()
{
	ASSERT(IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));
	ASSERT(m_hWnd != NULL);
	HWND hWndMDIClient = ::GetParent(m_hWnd);
	ASSERT(hWndMDIClient != NULL);

	CMDIFrameWnd* pMDIFrame;
	pMDIFrame = (CMDIFrameWnd*)CWnd::FromHandle(::GetParent(hWndMDIClient));
	ASSERT(pMDIFrame != NULL);
	ASSERT(pMDIFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)));
	ASSERT(pMDIFrame->m_hWndMDIClient == hWndMDIClient);
	ASSERT_VALID(pMDIFrame);
	return pMDIFrame;
}

CWnd* CMDIChildWnd::GetMessageBar()
{
	// status bar/message bar owned by parent MDI frame
	return GetMDIFrame()->GetMessageBar();
}

void CMDIChildWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// update our parent window first
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);

	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave child window alone!

	CDocument* pDocument = GetActiveDocument();
	if (bAddToTitle && pDocument != NULL)
	{
		char szOld[256];
		GetWindowText(szOld, sizeof(szOld));
		char szText[256];

		lstrcpy(szText, pDocument->GetTitle());
		if (m_nWindow > 0)
			wsprintf(szText + lstrlen(szText), ":%d", m_nWindow);

		// set title if changed, but don't remove completely
		if (lstrcmp(szText, szOld) != 0)
			SetWindowText(szText);
	}
}

void CMDIChildWnd::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd*)
{
	m_bPseudoInactive = FALSE;  // must be happening for real

	// send deactivate notification to active view
	CView* pActiveView = GetActiveView();
	if (!bActivate && pActiveView != NULL)
		pActiveView->OnActivateView(FALSE, pActiveView, pActiveView);

	// allow hook to short circuit normal activation
	BOOL bHooked = FALSE;
	if (m_pNotifyHook != NULL && m_pNotifyHook->OnDocActivate(bActivate))
		bHooked = TRUE;

	// update titles (don't AddToTitle if deactivate last)
	if (!bHooked)
		OnUpdateFrameTitle(bActivate || (pActivateWnd != NULL));

	// re-activate the appropriate view
	if (bActivate)
	{
		if (pActiveView != NULL && GetMDIFrame() == GetActiveWindow())
			pActiveView->OnActivateView(TRUE, pActiveView, pActiveView);
	}

	// update menus
	if (!bHooked)
	{
		OnUpdateFrameMenu(bActivate, pActivateWnd, NULL);
		GetMDIFrame()->DrawMenuBar();
	}
}

void CMDIChildWnd::OnUpdateFrameMenu(BOOL bActivate, CWnd* pActivateWnd,
	HMENU hMenuAlt)
{
	CMDIFrameWnd* pFrame = GetMDIFrame();

	if (hMenuAlt == NULL && bActivate)
	{
		// attempt to get default menu from document
		CDocument* pDoc = GetActiveDocument();
		if (pDoc != NULL)
			hMenuAlt = pDoc->GetDefaultMenu();
	}

	// use default menu stored in frame if none from document
	if (hMenuAlt == NULL)
		hMenuAlt = m_hMenuShared;

	if (hMenuAlt != NULL && bActivate)
	{
		ASSERT(pActivateWnd == this);

		// activating child, set parent menu
		::SendMessage(pFrame->m_hWndMDIClient, WM_MDISETMENU, 0,
			MAKELONG(hMenuAlt, pFrame->GetWindowMenuPopup(hMenuAlt)));
	}
	else if (hMenuAlt != NULL && !bActivate && pActivateWnd == NULL)
	{
		// destroying last child
		HMENU hMenuLast = NULL;
		if (!afxData.bWin31)    // Windows 3.0 needs to remove the separator
		{
			HMENU hMenuCur = ::GetMenu(pFrame->m_hWnd);
			if (hMenuCur != NULL)
				hMenuLast = pFrame->GetWindowMenuPopup(hMenuCur);
		}
		::SendMessage(pFrame->m_hWndMDIClient, WM_MDISETMENU, 0,
			MAKELONG(pFrame->m_hMenuDefault, hMenuLast));
	}
	else
	{
		// Refresh MDI Window menu (even if non-shared menu)
		if (afxData.bWin31)
			::SendMessage(pFrame->m_hWndMDIClient, WM_MDISETMENU, TRUE, 0L);
		return; // no need to redraw the menu, only a submenu has been changed
	}
}

int CMDIChildWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// call base class with lParam context (not MDI one)
	MDICREATESTRUCT FAR* lpmcs;
	lpmcs = (MDICREATESTRUCT FAR*)lpCreateStruct->lpCreateParams;
	CCreateContext* pContext = (CCreateContext*)
		_AfxGetPtrFromFarPtr((LPVOID)lpmcs->lParam);

	return OnCreateHelper(lpCreateStruct, pContext);
}

/////////////////////////////////////////////////////////////////////////////
// Special UI processing depending on current active child

void CMDIFrameWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave it alone!

	// allow hook to set the title (used for OLE 2.0 support)
	if (m_pNotifyHook != NULL && m_pNotifyHook->OnUpdateFrameTitle())
		return;

	CMDIChildWnd* pActiveChild;
	CDocument* pDocument = GetActiveDocument();
	if (bAddToTitle &&
	  (pActiveChild = MDIGetActive()) != NULL &&
	  (pActiveChild->GetStyle() & WS_MAXIMIZE) == 0 &&
	  (pDocument != NULL ||
	   (pDocument = pActiveChild->GetActiveDocument()) != NULL))
		UpdateFrameTitleForDocument(pDocument->GetTitle());
	else
		UpdateFrameTitleForDocument(NULL);
}

/////////////////////////////////////////////////////////////////////////////
// Standard MDI Commands

// Two function for all standard MDI "Window" commands
void CMDIFrameWnd::OnUpdateMDIWindowCmd(CCmdUI* pCmdUI)
{
	ASSERT(m_hWndMDIClient != NULL);
	pCmdUI->Enable(MDIGetActive() != NULL);
}

BOOL CMDIFrameWnd::OnMDIWindowCmd(UINT nID)
{
	ASSERT(m_hWndMDIClient != NULL);

	UINT msg;
	UINT wParam = 0;
	switch (nID)
	{
	default:
		return FALSE;       // not for us
	case ID_WINDOW_ARRANGE:
		msg = WM_MDIICONARRANGE;
		break;
	case ID_WINDOW_CASCADE:
		msg = WM_MDICASCADE;
		break;
	case ID_WINDOW_TILE_HORZ:
		wParam = MDITILE_HORIZONTAL;
		// fall through
	case ID_WINDOW_TILE_VERT:
		ASSERT(MDITILE_VERTICAL == 0);
		msg = WM_MDITILE;
		break;
	}

	::SendMessage(m_hWndMDIClient, msg, wParam, 0);
	return TRUE;
}

void CMDIFrameWnd::OnWindowNew()
{
	CMDIChildWnd* pActiveChild = MDIGetActive();
	CDocument* pDocument;
	if (pActiveChild == NULL ||
	  (pDocument = pActiveChild->GetActiveDocument()) == NULL)
	{
		TRACE0("Warning: No active document for WindowNew command\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		return;     // command failed
	}

	// otherwise we have a new frame !
	CDocTemplate* pTemplate = pDocument->GetDocTemplate();
	ASSERT_VALID(pTemplate);
	CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDocument, pActiveChild);
	if (pFrame == NULL)
	{
		TRACE0("Warning: failed to create new frame\n");
		return;     // command failed
	}

	pTemplate->InitialUpdateFrame(pFrame, pDocument);
}

////////////////////////////////////////////////////////////////////////////
