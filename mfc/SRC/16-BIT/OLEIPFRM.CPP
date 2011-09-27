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

#ifdef AFX_OLE3_SEG
#pragma code_seg(AFX_OLE3_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(COleIPFrameWnd, CFrameWnd)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// COleIPFrameWnd implementation

BEGIN_MESSAGE_MAP(COleIPFrameWnd, CFrameWnd)
	//{{AFX_MSG_MAP(COleIPFrameWnd)
	ON_WM_SIZE()
	ON_MESSAGE(WM_RECALCPARENT, OnRecalcParent)
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_SIZECHILD, OnResizeChild)
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COleIPFrameWnd::COleIPFrameWnd()
{
	// initialize OLEINPLACEFRAMEINFO struct
	memset(&m_frameInfo, 0, sizeof(m_frameInfo));
	m_frameInfo.cb = sizeof m_frameInfo;

	// initialize in-place state
	m_bUIActive = FALSE;
	m_lpFrame = NULL;
	m_lpDocFrame = NULL;
	m_hOleMenu = NULL;
	m_rectPos.SetRectEmpty();
	m_rectClip.SetRectEmpty();
	m_bInsideRecalc = FALSE;
	m_hSharedMenu = NULL;
	m_hWndFrame = NULL;
	m_hWndDocFrame = NULL;

	AddFrameWnd();  // so WM_IDLEUPDATECMDUI messages are received

	ASSERT_VALID(this);
}

COleIPFrameWnd::~COleIPFrameWnd()
{
	ASSERT_VALID(this);

	// remove from WM_IDLEUPDATECMDUI list
	RemoveFrameWnd();

	// destroy shared menu
	if (m_hSharedMenu != NULL)
		::DestroyMenu(m_hSharedMenu);

	// interfaces to the container should already be released
	_AfxRelease((LPUNKNOWN*)&m_lpFrame);
	_AfxRelease((LPUNKNOWN*)&m_lpDocFrame);
}

int COleIPFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	ASSERT_VALID(this);

	if (CFrameWnd::OnCreate(lpCreateStruct) < 0)
		return -1;

	// need to remove the pending WM_SETMESSAGESTRING from the queue
	MSG msg;
	PeekMessage(&msg, m_hWnd, WM_SETMESSAGESTRING, WM_SETMESSAGESTRING,
		PM_REMOVE|PM_NOYIELD);

	ASSERT_VALID(this);
	return 0;
}

void COleIPFrameWnd::OnDestroy()
{
	// notify the container that the rectangle has changed!
	COleServerDoc* pDoc = (COleServerDoc*)GetActiveDocument();
	if (pDoc != NULL)
	{
		ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COleServerDoc)));

		// close and abort changes to the document
		pDoc->DisconnectViews();
		pDoc->OnCloseDocument();
	}
	CFrameWnd::OnDestroy();
}

BOOL COleIPFrameWnd::OnCreateControlBars(CWnd* pWndFrame, CWnd* pWndDoc)
{
	// no default implementation

	return TRUE;
}

LRESULT COleIPFrameWnd::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	ASSERT_VALID(this);

	// update toolbars which may be on the container
	if (m_hWndFrame != NULL)
		UpdateControlBars(m_hWndFrame);
	if (m_hWndDocFrame != NULL)
		UpdateControlBars(m_hWndDocFrame);

	return 0;
}

LRESULT COleIPFrameWnd::OnSetMessageString(WPARAM wParam, LPARAM lParam)
{
	if (m_lpFrame != NULL)
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

		// notify container of new status text
		m_lpFrame->SetStatusText(lpsz != NULL ? lpsz : "");
	}

	UINT nIDLast = m_nIDLastMessage;
	m_nIDLastMessage = (UINT)wParam;    // new ID (or 0)
	m_nIDTracking = (UINT)wParam;       // so F1 on toolbar buttons work
	return nIDLast;
}

void COleIPFrameWnd::UpdateControlBars(HWND hWnd)
{
	ASSERT(::IsWindow(hWnd));

	// enumerate windows looking for control bars
	for (HWND hWndChild = ::GetTopWindow(hWnd); hWndChild != NULL;
		hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
	{
		CWnd* pWnd = CWnd::FromHandlePermanent(hWndChild);
		if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CControlBar)) &&
			pWnd->GetOwner() == this)
		{
			// update the user-interface before showing
			_AfxCallWndProc(pWnd, pWnd->m_hWnd, WM_IDLEUPDATECMDUI,
				(WPARAM)TRUE, 0);
		}
	}
}

BOOL COleIPFrameWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
	CWnd* pParentWnd, CCreateContext* pContext)
{
	if (pParentWnd != NULL)
		ASSERT_VALID(pParentWnd);

	// only do this once
	ASSERT_VALID_IDR(nIDResource);
	ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

	m_nIDHelp = nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

	// create the window (use child window style create)
	CRect rect(0, 0, 0, 0);
	if (!CWnd::Create(NULL, NULL, dwDefaultStyle, rect, pParentWnd,
		nIDResource, pContext))
	{
		return FALSE;   // will self destruct on failure normally
	}

	// load accelerator resource
	LoadAccelTable(MAKEINTRESOURCE(nIDResource));

	return TRUE;
}

void COleIPFrameWnd::OnSize(UINT nType, int cx, int cy)
{
	// recalc layout is not called in OnSize since COleIPFrameWnd does
	//  "inside out" recalc -- which is driven by the size of the
	//  inner most window changing, not the outer most!
}

LRESULT COleIPFrameWnd::OnResizeChild(WPARAM wParam, LPARAM lParam)
{
	// notify the container that the rectangle has changed!
	COleServerDoc* pDoc = (COleServerDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return 0;

	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COleServerDoc)));

	// get new rect and parent
	CRect rectNew;
	rectNew.CopyRect((LPCRECT)lParam);
	CWnd* pParentWnd = GetParent();
	ASSERT_VALID(pParentWnd);

	// convert rectNew relative to pParentWnd
	ClientToScreen(&rectNew);
	pParentWnd->ScreenToClient(&rectNew);

	// adjust the new rectangle for the current control bars
	CWnd* pLeftOver = GetDlgItem(AFX_IDW_PANE_FIRST);
	ASSERT(pLeftOver != NULL);
	CRect rectCur = m_rectPos;
	pLeftOver->CalcWindowRect(&rectCur, CWnd::adjustOutside);
	rectNew.left += m_rectPos.left - rectCur.left;
	rectNew.top += m_rectPos.top - rectCur.top;
	rectNew.right -= rectCur.right - m_rectPos.right;
	rectNew.bottom -= rectCur.bottom - m_rectPos.bottom;
	pDoc->RequestPositionChange(&rectNew);

	return 0;
}

LRESULT COleIPFrameWnd::OnRecalcParent(WPARAM wParam, LPARAM lParam)
{
	// simply call recalc layout (default for CFrameWnd is nothing)
	RepositionFrame(&m_rectPos, &m_rectClip);
	if ((LPRECT)lParam != NULL)
		*(LPRECT)lParam = m_rectPos;
	return (LRESULT)TRUE;
}

void COleIPFrameWnd::RecalcLayout(BOOL bNotify)
{
	ASSERT_VALID(this);

	// do the recalc
	RepositionFrame(&m_rectPos, m_rectClip);
}

void COleIPFrameWnd::RepositionFrame(LPCRECT lpPosRect, LPCRECT lpClipRect)
{
	ASSERT(AfxIsValidAddress(lpPosRect, sizeof(RECT), FALSE));
	ASSERT(AfxIsValidAddress(lpClipRect, sizeof(RECT), FALSE));

	// gaurd against recursion
	if (m_bInsideRecalc)
		return;
	m_bInsideRecalc = TRUE;

	// remember the client area for later
	m_rectPos.CopyRect(lpPosRect);
	m_rectClip.CopyRect(lpClipRect);

	// better have a parent window (only used for inplace)
	CWnd* pParentWnd = GetParent();
	ASSERT_VALID(pParentWnd);

	// first call reposition bars with arbitarily large rect just to
	//  see how much space the bars will take up
	CRect rectBig(0, 0, INT_MAX/2, INT_MAX/2);
	CRect rectLeft;
	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery,
		&rectLeft, &rectBig);

	// grow the rect by the size of the control bars
	CRect rect;
	rect.CopyRect(lpPosRect);
	rect.left -= rectLeft.left;
	rect.top -= rectLeft.top;
	rect.right += INT_MAX/2 - rectLeft.right;
	rect.bottom += INT_MAX/2 - rectLeft.bottom;

	// see how much extra space for non-client areas (such as scrollbars)
	//  that the view needs.
	CWnd* pLeftOver = GetDlgItem(AFX_IDW_PANE_FIRST);
	if (pLeftOver != NULL)
	{
		rectBig.CopyRect(lpPosRect);
		pLeftOver->CalcWindowRect(&rectBig, CWnd::adjustOutside);
		rect.left -= lpPosRect->left - rectBig.left;
		rect.top -= lpPosRect->top - rectBig.top;
		rect.right += rectBig.right - lpPosRect->right;
		rect.bottom += rectBig.bottom - lpPosRect->bottom;
	}

	// adjust for non-client area on the frame window
	CalcWindowRect(&rect, CWnd::adjustOutside);

	// the frame window must be clipped to the visible part in the container
	CRect rectVis;
	rectVis.IntersectRect(&rect, lpClipRect);

	// move the window
	_AfxRepositionWindow(NULL, m_hWnd, &rectVis);

	// now resize the control bars relative to the (now moved) frame
	pParentWnd->ClientToScreen(&rect);
	ScreenToClient(&rect);
	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST,
		CWnd::reposDefault, NULL, &rect);

	// remove recursion lockout
	m_bInsideRecalc = FALSE;
}

BOOL COleIPFrameWnd::PreTranslateMessage(MSG* pMsg)
{
	// try server's accelerators first
	if (CFrameWnd::PreTranslateMessage(pMsg))
		return TRUE;

	// try container's accelerators as last chance
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		OLEINPLACEFRAMEINFO frameInfo = m_frameInfo;
		if (::OleTranslateAccelerator(m_lpFrame, &frameInfo, pMsg) == NOERROR)
			return TRUE;
	}

	return FALSE;   // keystroke not processed.
}

/////////////////////////////////////////////////////////////////////////////
// Special-case context sensitive help

void COleIPFrameWnd::OnContextHelp()
{
	if (m_bHelpMode == HELP_ACTIVE || !CanEnterHelpMode())
		return;

	// notify container that we are entering context sensitive help
	BOOL bHelpMode = m_bHelpMode;
	m_bHelpMode = HELP_ACTIVE;
	ASSERT(m_lpFrame != NULL);
	if (m_lpFrame->ContextSensitiveHelp(TRUE) != NOERROR ||
		(m_lpDocFrame != NULL && m_lpDocFrame->ContextSensitiveHelp(TRUE) != NOERROR))
	{
		m_bHelpMode = HELP_INACTIVE;
		return;
	}
	m_bHelpMode = bHelpMode;

	// now enter context sensitive help mode ourselves
	CFrameWnd::OnContextHelp();

	if (m_bHelpMode == HELP_INACTIVE)
	{
		// make sure container exits context sensitive help mode
		m_lpFrame->ContextSensitiveHelp(FALSE);
		if (m_lpDocFrame != NULL)
			m_lpDocFrame->ContextSensitiveHelp(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// In-place activation startup

BOOL COleIPFrameWnd::BuildSharedMenu(COleServerDoc* pDoc)
{
	// get runtime class from the doc template
	CDocTemplate* pTemplate = pDoc->GetDocTemplate();
	ASSERT_VALID(pTemplate);
	HMENU hMenuOLE = pTemplate->m_hMenuInPlaceServer;

	// create shared menu
	ASSERT(m_hSharedMenu == NULL);
	if ((m_hSharedMenu = ::CreateMenu()) == NULL)
		return FALSE;

	// start out by getting menu from container
	memset(&m_menuWidths, 0, sizeof m_menuWidths);
	if (m_lpFrame->InsertMenus(m_hSharedMenu, &m_menuWidths) != NOERROR)
	{
		::DestroyMenu(m_hSharedMenu);
		m_hSharedMenu = NULL;
		return FALSE;
	}
	// container shouldn't touch these
	ASSERT(m_menuWidths.width[1] == 0);
	ASSERT(m_menuWidths.width[3] == 0);
	ASSERT(m_menuWidths.width[5] == 0);

	// only copy the popups if there is a menu loaded
	if (hMenuOLE == NULL)
		return TRUE;

	// insert our menu popups amongst the container menus
	_AfxMergeMenus(CMenu::FromHandle(m_hSharedMenu),
		CMenu::FromHandle(hMenuOLE), &m_menuWidths.width[0], 1);

	// finally create the special OLE menu descriptor
	m_hOleMenu = ::OleCreateMenuDescriptor(m_hSharedMenu, &m_menuWidths);

	return m_hOleMenu != NULL;
}

void COleIPFrameWnd::DestroySharedMenu(COleServerDoc* pDoc)
{
	if (m_hSharedMenu == NULL)
	{
		ASSERT(m_hOleMenu == NULL);
		return;
	}

	CDocTemplate* pTemplate = pDoc->GetDocTemplate();
	HMENU hMenuOLE = pTemplate->m_hMenuInPlaceServer;
	if (hMenuOLE == NULL)
		return;

	// remove our menu popups from the shared menu
	_AfxUnmergeMenus(CMenu::FromHandle(m_hSharedMenu), CMenu::FromHandle(hMenuOLE));

	// allow container to remove its items from the menu
	ASSERT(m_lpFrame != NULL);
	VERIFY(m_lpFrame->RemoveMenus(m_hSharedMenu) == NOERROR);

	// now destroy the menu
	::DestroyMenu(m_hSharedMenu);
	m_hSharedMenu = NULL;
	if (m_hOleMenu != NULL)
	{
		VERIFY(::OleDestroyMenuDescriptor(m_hOleMenu) == NOERROR);
		m_hOleMenu = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// COleIPFrameWnd diagnostics

#ifdef _DEBUG
void COleIPFrameWnd::AssertValid() const
{
	CFrameWnd::AssertValid();
	if (m_hSharedMenu != NULL)
		ASSERT(::IsMenu(m_hSharedMenu));
}

void COleIPFrameWnd::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
	AFX_DUMP1(dc, "\nm_lpFrame = ", m_lpFrame);
	AFX_DUMP1(dc, "\nm_lpDocFrame = ", m_lpDocFrame);
	AFX_DUMP1(dc, "\nm_hOleMenu = ", m_hOleMenu);
	AFX_DUMP1(dc, "\nm_rectPos = ", m_rectPos);
	AFX_DUMP1(dc, "\nm_rectClip = ", m_rectClip);
	AFX_DUMP1(dc, "\nm_bInsideRecalc = ", m_bInsideRecalc);
	AFX_DUMP1(dc, "\nm_hSharedMenu = ", m_hSharedMenu);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
