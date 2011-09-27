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

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CDialogBar, CControlBar)

CDialogBar::CDialogBar()
{
}

CDialogBar::~CDialogBar()
{
	DestroyWindow();    // avoid PostNcDestroy problems
}

BOOL CDialogBar::Create(CWnd* pParentWnd, LPCSTR lpszTemplateName,
			UINT nStyle, UINT nID)
{
	ASSERT(pParentWnd != NULL);
	ASSERT(lpszTemplateName != NULL);

#ifdef _DEBUG
	// dialog template must exist and be invisible with WS_CHILD set
	if (!_AfxCheckDialogTemplate(lpszTemplateName, TRUE))
	{
		ASSERT(FALSE);          // invalid dialog template name
		PostNcDestroy();        // cleanup if Create fails too soon
		return FALSE;
	}
#endif //_DEBUG

	HINSTANCE hInst = AfxFindResourceHandle(lpszTemplateName, RT_DIALOG);
	_AfxHookWindowCreate(this);
	HWND hWnd = ::CreateDialog(hInst, lpszTemplateName,
		pParentWnd->GetSafeHwnd(), NULL);
	if (!_AfxUnhookWindowCreate())
		PostNcDestroy();        // cleanup if Create fails too soon

	if (hWnd == NULL)
		return FALSE;
	ASSERT(hWnd == m_hWnd);

	// Dialog template MUST specify that the Dialog
	// is an invisible child window

	_AfxSetDlgCtrlID(m_hWnd, nID);
	CRect rect;
	GetWindowRect(&rect);
	m_sizeFixedLayout = rect.Size();    // set fixed size

	// OR in the control bar sub-style
	::SetWindowLong(m_hWnd, GWL_STYLE, GetStyle() | nStyle | WS_CLIPSIBLINGS);

	// initialize VBX controls etc
	if (!ExecuteDlgInit(lpszTemplateName))
		return FALSE;

	// force the size to zero - resizing bar will occur later
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW);

	return TRUE;
}


WNDPROC* CDialogBar::GetSuperWndProcAddr()
{
	static WNDPROC NEAR pfnSuper;
	return &pfnSuper;
}

///////////////////////////////////////////////////////////////////////////

void CDialogBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

///////////////////////////////////////////////////////////////////////////
// CDialogBar diagnostics

#ifdef _DEBUG
void CDialogBar::AssertValid() const
{
	CControlBar::AssertValid();
}

void CDialogBar::Dump(CDumpContext& dc) const
{
	CControlBar::Dump(dc);
}
#endif

///////////////////////////////////////////////////////////////////////////
