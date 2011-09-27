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

#ifdef AFX_CORE2_SEG
#pragma code_seg(AFX_CORE2_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CFormView, CScrollView)

BEGIN_MESSAGE_MAP(CFormView, CScrollView)
	//{{AFX_MSG_MAP(CFormView)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CFormView::CFormView(LPCSTR lpszTemplateName)
{
	m_lpszTemplateName = lpszTemplateName;
	m_pCreateContext = NULL;
	m_hWndFocus = NULL;     // focus window is unknown
}

CFormView::CFormView(UINT nIDTemplate)
{
	ASSERT_VALID_IDR(nIDTemplate);
	m_lpszTemplateName = MAKEINTRESOURCE(nIDTemplate);
	m_pCreateContext = NULL;
}

// virtual override of CWnd::Create
BOOL CFormView::Create(LPCSTR /*lpszClassName ignored*/,
	LPCSTR /*lpszWindowName ignored*/, DWORD dwRequestedStyle,
	const RECT& rect, CWnd* pParentWnd, UINT nID,
	CCreateContext* pContext)
{
	ASSERT(pParentWnd != NULL);
	ASSERT(m_lpszTemplateName != NULL);

	m_pCreateContext = pContext;    // save state for later OnCreate

#ifdef _DEBUG
	// dialog template must exist and be invisible with WS_CHILD set
	if (!_AfxCheckDialogTemplate(m_lpszTemplateName, TRUE))
	{
		ASSERT(FALSE);          // invalid dialog template name
		PostNcDestroy();        // cleanup if Create fails too soon
		return FALSE;
	}
#endif //_DEBUG

	HINSTANCE hInst = AfxFindResourceHandle(m_lpszTemplateName, RT_DIALOG);
	_AfxHookWindowCreate(this);
	HWND hWnd = ::CreateDialog(hInst, m_lpszTemplateName,
			pParentWnd->GetSafeHwnd(), NULL);
	if (!_AfxUnhookWindowCreate())
		PostNcDestroy();        // cleanup if Create fails too soon

	if (hWnd == NULL)
		return FALSE;

	ASSERT(hWnd == m_hWnd);
	m_pCreateContext = NULL;

	// we use the style from the template - but make sure that
	//  the WS_BORDER bit is correct
	DWORD dwStyle = GetStyle();
	if ((dwStyle & WS_BORDER) != (dwRequestedStyle & WS_BORDER))
	{
		// toggle WS_BORDER on invisible window
		::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle ^ WS_BORDER);
	}

	_AfxSetDlgCtrlID(m_hWnd, nID);

	CRect rectTemplate;
	GetWindowRect(rectTemplate);
	SetScrollSizes(MM_TEXT, rectTemplate.Size());

	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpszTemplateName))
		return FALSE;

	// force the size requested
	SetWindowPos(NULL, rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		SWP_NOZORDER|SWP_NOACTIVATE);

	// make visible if requested
	if (dwRequestedStyle & WS_VISIBLE)
		ShowWindow(SW_NORMAL);

	return TRUE;
}

void CFormView::OnInitialUpdate()
{
	ASSERT_VALID(this);

	if (!UpdateData(FALSE))
		TRACE0("UpdateData failed during formview initial update\n");
	CScrollView::OnInitialUpdate();
}

int CFormView::OnCreate(LPCREATESTRUCT lpcs)
{
	// since we can't get the create context parameter passed in
	//  through CreateDialog, we use a temporary member variable
	lpcs->lpCreateParams = (LPSTR)m_pCreateContext;
	return CScrollView::OnCreate(lpcs);
}

void CFormView::OnActivateView(
	BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if (SaveFocusControl())
		return;     // don't call base class when focus is already set

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CFormView::OnActivateFrame(UINT nState, CFrameWnd* pFrameWnd)
{
	if (nState == WA_INACTIVE)
		SaveFocusControl();     // save focus when frame loses activation
}

BOOL CFormView::SaveFocusControl()
{
	// save focus window if focus is one this window's controls
	HWND hWndFocus = ::GetFocus();
	if (::IsChild(m_hWnd, hWndFocus))
	{
		m_hWndFocus = hWndFocus;
		return TRUE;
	}
	return FALSE;
}

void CFormView::OnSetFocus(CWnd*)
{
	if (!::IsWindow(m_hWndFocus))
	{
		// invalid or unknown focus window... let windows handle it
		m_hWndFocus = NULL;
		Default();
		return;
	}
	// otherwise, set focus to the last known focus window
	::SetFocus(m_hWndFocus);
}

BOOL CFormView::PreTranslateMessage(MSG* pMsg)
{
	ASSERT(pMsg != NULL);
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);

	// don't translate dialog messages when in Shift+F1 help mode
	CFrameWnd* pFrameWnd = GetTopLevelFrame();
	if (pFrameWnd != NULL && pFrameWnd->m_bHelpMode)
		return FALSE;

	// since 'IsDialogMessage' will eat frame window accelerators,
	//   we call the frame window's PreTranslateMessage first
	if ((pFrameWnd = GetParentFrame()) != NULL)
	{
		if (pFrameWnd->PreTranslateMessage(pMsg))
			return TRUE;        // eaten by frame accelerator
		// check for parent of the frame in case of MDI
		if ((pFrameWnd = pFrameWnd->GetParentFrame()) != NULL &&
		  pFrameWnd->PreTranslateMessage(pMsg))
			return TRUE;        // eaten by frame accelerator
	}

	// filter both messages to dialog and from children
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
		return ::IsDialogMessage(m_hWnd, pMsg);

	return FALSE;
}


void CFormView::OnDraw(CDC* pDC)
{
	ASSERT_VALID(this);

	// do nothing - dialog controls will paint themselves,
	//   and Windows dialog controls do not support printing
#ifdef _DEBUG
	if (pDC->IsPrinting())
		TRACE0("Warning: CFormView does not support printing\n");
#endif
}

WNDPROC* CFormView::GetSuperWndProcAddr()
{
	static WNDPROC NEAR pfnSuper;
	return &pfnSuper;
}

//////////////////////////////////////////////////////////////////////////
// CFormView diagnostics

#ifdef _DEBUG
void CFormView::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);
	CView::Dump(dc);
	AFX_DUMP0(dc, "\nm_lpszTemplateName = ");
	if (HIWORD(m_lpszTemplateName) == 0)
		dc << (int)LOWORD(m_lpszTemplateName);
	else
		dc << m_lpszTemplateName;
}

void CFormView::AssertValid() const
{
	CView::AssertValid();
}
#endif

//////////////////////////////////////////////////////////////////////////
