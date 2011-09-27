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

#ifdef AFX_CORE1_SEG
#pragma code_seg(AFX_CORE1_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// Special case for remaining dialog cases
// Most messages will go through the window proc (AfxWndProc) of the
//   subclassed dialog.  Some messages like WM_SETFONT and WM_INITDIALOG
//   are sent directly to the dialog proc only.  These messages cannot be
//   passed on to DefWindowProc() or infinite recursion will result!
// In responding to these messages, you shouldn't call the Default handler

LRESULT CALLBACK AFX_EXPORT
_AfxDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CDialog* pDlg;

	// test for special case (Win 3.0 will call dialog proc instead
	//  of SendMessage for these two messages).
	if (message != WM_SETFONT && message != WM_INITDIALOG)
		return 0L;      // normal handler

	// assume it is already wired up to a permanent one
	pDlg = (CDialog*) CWnd::FromHandlePermanent(hWnd);
	ASSERT(pDlg != NULL);
	ASSERT(pDlg->m_hWnd == hWnd);

	// prepare for callback, make it look like message map call
	LONG lResult = 0;
	MSG oldState = _afxLastMsg;    // save for nesting

	_afxLastMsg.hwnd = hWnd;
	_afxLastMsg.message = message;
	_afxLastMsg.wParam = wParam;
	_afxLastMsg.lParam = lParam;

	TRY
	{
		if (message == WM_SETFONT)
			pDlg->OnSetFont(CFont::FromHandle((HFONT)wParam));
		else // WM_INITDIALOG
			lResult = pDlg->OnInitDialog();
	}
	CATCH_ALL(e)
	{
		// fall through
		TRACE0("Warning: something went wrong in dialog init\n");
		pDlg->EndDialog(IDABORT);  // something went wrong
		ASSERT(FALSE);
	}
	END_CATCH_ALL

	_afxLastMsg = oldState;
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
// CDialog - Modeless and Modal

IMPLEMENT_DYNAMIC(CDialog, CWnd)

BEGIN_MESSAGE_MAP(CDialog, CWnd)
	//{{AFX_MSG_MAP(CDialog)
	ON_WM_CTLCOLOR()
	ON_COMMAND(IDOK, OnOK)
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_MESSAGE(WM_COMMANDHELP, OnCommandHelp)
	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDialog::PreTranslateMessage(MSG* pMsg)
{
	// for modeless processing (or modal)
	ASSERT(m_hWnd != NULL);

	// don't translate dialog messages when in Shift+F1 help mode
	CFrameWnd* pFrameWnd = GetTopLevelFrame();
	if (pFrameWnd != NULL && pFrameWnd->m_bHelpMode)
		return FALSE;

	// filter both messages to dialog and from children
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
		return ::IsDialogMessage(m_hWnd, pMsg);

	return FALSE;
}

WNDPROC* CDialog::GetSuperWndProcAddr()
{
	static WNDPROC NEAR pfnSuper;
	return &pfnSuper;
}


BOOL CDialog::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	if ((nCode != CN_COMMAND && nCode != CN_UPDATE_COMMAND_UI) ||
			!IS_COMMAND_ID(nID) || nID >= 0xf000)
	{
		// control notification or non-command button or system command
		return FALSE;       // not routed any further
	}

	// if we have an owner window, give it second crack
	CWnd* pOwner = GetWindow(GW_OWNER);
	if (pOwner != NULL)
	{
#ifdef _DEBUG
		if (afxTraceFlags & 8)
			TRACE1("Routing command id 0x%04X to owner window\n", nID);
#endif
		ASSERT(pOwner != this);
		if (pOwner->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

	// last crack goes to the WinApp
	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL)
	{
#ifdef _DEBUG
		if (afxTraceFlags & 8)
			TRACE1("Routing command id 0x%04X to app\n", nID);
#endif
		if (pApp->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

#ifdef _DEBUG
	if (afxTraceFlags & 8)
	{
		TRACE2("IGNORING command id 0x%04X sent to %Fs dialog\n", nID,
				GetRuntimeClass()->m_lpszClassName);
	}
#endif
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// Modeless Dialogs have 2-phase construction

CDialog::CDialog()
{
	ASSERT(m_hWnd == NULL);
	AFX_ZERO_INIT_OBJECT(CWnd);
}

CDialog::~CDialog()
{
	if (m_hWnd != NULL)
	{
		TRACE0("Warning: calling DestroyWindow in CDialog::~CDialog\n");
		TRACE0("\tOnDestroy or PostNcDestroy in derived class will not be called\n");
		DestroyWindow();
	}
}


BOOL CDialog::Create(LPCSTR lpszTemplateName, CWnd* pParentWnd)
{
	ASSERT(_AFX_FP_SEG(lpszTemplateName) == 0 ||
		AfxIsValidString(lpszTemplateName));

	if (pParentWnd == NULL)
		pParentWnd = AfxGetMainWnd();

	if (pParentWnd != NULL)
		ASSERT_VALID(pParentWnd);

	m_lpDialogTemplate = lpszTemplateName;  // used for help
	if (HIWORD(m_lpDialogTemplate) == 0 && m_nIDHelp == 0)
		m_nIDHelp = LOWORD(m_lpDialogTemplate);

#ifdef _DEBUG
	if (!_AfxCheckDialogTemplate(lpszTemplateName, FALSE))
	{
		ASSERT(FALSE);          // invalid dialog template name
		PostNcDestroy();        // cleanup if Create fails too soon
		return FALSE;
	}
#endif //_DEBUG

	HINSTANCE hInst = AfxFindResourceHandle(lpszTemplateName, RT_DIALOG);
	_AfxHookWindowCreate(this);
	HWND hWnd = ::CreateDialog(hInst, lpszTemplateName,
		pParentWnd->GetSafeHwnd(), (DLGPROC)_AfxDlgProc);
	if (!_AfxUnhookWindowCreate())
		PostNcDestroy();        // cleanup if Create fails too soon

	if (hWnd == NULL)
		return FALSE;
	ASSERT(hWnd == m_hWnd);
	return TRUE;
}

typedef void FAR* LPCDLGTEMPLATE;

BOOL CDialog::CreateIndirect(const void FAR* lpDialogTemplate, CWnd* pParentWnd)
{
	if (pParentWnd == NULL)
		pParentWnd = AfxGetMainWnd();

	if (pParentWnd != NULL)
		ASSERT_VALID(pParentWnd);

	_AfxHookWindowCreate(this);
	HWND hWnd = ::CreateDialogIndirect(AfxGetInstanceHandle(),
		(LPCDLGTEMPLATE)lpDialogTemplate, pParentWnd->GetSafeHwnd(),
		(DLGPROC)_AfxDlgProc);
	if (!_AfxUnhookWindowCreate())
		PostNcDestroy();        // cleanup if Create fails too soon

	if (hWnd == NULL)
		return FALSE;
	ASSERT(hWnd == m_hWnd);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Modal Dialogs

// Modal Constructors just save parameters
CDialog::CDialog(LPCSTR lpszTemplateName, CWnd* pParentWnd)
{
	ASSERT(_AFX_FP_SEG(lpszTemplateName) == 0 ||
		AfxIsValidString(lpszTemplateName));

	AFX_ZERO_INIT_OBJECT(CWnd);
	m_pParentWnd = pParentWnd;
	m_lpDialogTemplate = lpszTemplateName;
	if (HIWORD(m_lpDialogTemplate) == 0)
		m_nIDHelp = LOWORD(m_lpDialogTemplate);
}

CDialog::CDialog(UINT nIDTemplate, CWnd* pParentWnd)
{
	AFX_ZERO_INIT_OBJECT(CWnd);
	m_pParentWnd = pParentWnd;
	m_lpDialogTemplate = MAKEINTRESOURCE(nIDTemplate);
	m_nIDHelp = nIDTemplate;
}

BOOL CDialog::InitModalIndirect(HGLOBAL hDialogTemplate)
{
	// must be called on an empty constructed CDialog
	ASSERT(m_lpDialogTemplate == NULL);
	ASSERT(m_hDialogTemplate == NULL);

	m_hDialogTemplate = hDialogTemplate;
	return TRUE;        // always ok (DoModal actually brings up dialog)
}

HWND CDialog::PreModal()
{
	// cannot call DoModal on a dialog already constructed as modeless
	ASSERT(m_hWnd == NULL);

	AfxGetApp()->EnableModeless(FALSE);

	// find parent HWND
	HWND hWndParent = _AfxGetSafeOwner(m_pParentWnd);
	_AfxHookWindowCreate(this);

	return hWndParent;
}

void CDialog::PostModal()
{
	_AfxUnhookWindowCreate();   // just in case
	Detach();               // just in case

	AfxGetApp()->EnableModeless(TRUE);
}

int CDialog::DoModal()
{
	int     nResult;

	// can be constructed with a resource template or InitModalIndirect
	ASSERT(m_lpDialogTemplate != NULL || m_hDialogTemplate != NULL);

	HWND hWndParent = PreModal();
	if (m_lpDialogTemplate != NULL)
	{
		HINSTANCE hInst = AfxFindResourceHandle(m_lpDialogTemplate, RT_DIALOG);
		nResult = ::DialogBox(hInst, m_lpDialogTemplate,
			hWndParent, (DLGPROC)_AfxDlgProc);
	}
	else
	{
		HINSTANCE hInst = AfxGetInstanceHandle();
		nResult = ::DialogBoxIndirect(hInst, m_hDialogTemplate,
			hWndParent, (DLGPROC)_AfxDlgProc);
	}
	PostModal();

	return nResult;
}

/////////////////////////////////////////////////////////////////////////////
// Standard CDialog implementation

BOOL PASCAL _AfxHelpEnabled()
{
	// help is enabled if the app has a handler for ID_HELP
	AFX_CMDHANDLERINFO info;

	// try main window first
	CWnd* pWnd = AfxGetMainWnd();
	if (pWnd != NULL && pWnd->OnCmdMsg(ID_HELP, CN_COMMAND, NULL, &info))
		return TRUE;

	// try app last
	return AfxGetApp()->OnCmdMsg(ID_HELP, CN_COMMAND, NULL, &info);
}

void CDialog::OnSetFont(CFont*)
{
	// ignore it
}

BOOL CDialog::OnInitDialog()
{
	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpDialogTemplate))
		return FALSE;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init\n");
		EndDialog(IDABORT);
		return FALSE;
	}

	CWnd* pHelpButton = GetDlgItem(ID_HELP);
	if (pHelpButton != NULL)
		pHelpButton->ShowWindow(_AfxHelpEnabled() ? SW_SHOW : SW_HIDE);

	return TRUE;    // set focus to first one
}

void CDialog::OnOK()
{
	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination\n");
		// the UpdateData routine will set focus to correct item
		return;
	}
	EndDialog(IDOK);
}

void CDialog::OnCancel()
{
	EndDialog(IDCANCEL);
}

/////////////////////////////////////////////////////////////////////////////
// Gray background support

HBRUSH CDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	LRESULT lResult;
	if (pWnd->SendChildNotifyLastMsg(&lResult))
		return (HBRUSH)lResult;     // eat it

	if (!GrayCtlColor(pDC->m_hDC, pWnd->GetSafeHwnd(), nCtlColor,
	  afxDlgBkBrush, afxDlgTextClr))
		return (HBRUSH)Default();
	return afxDlgBkBrush;
}

// implementation of OnCtlColor for default gray backgrounds
//   (works for any window containing controls)
//  return value of FALSE means caller must call DefWindowProc's default
//  TRUE means that 'hbrGray' will be used and the appropriate text
//    ('clrText') and background colors are set.
BOOL PASCAL CWnd::GrayCtlColor(HDC hDC, HWND hWnd, UINT nCtlColor,
	HBRUSH hbrGray, COLORREF clrText)
{
	ASSERT(hDC != NULL);

	if (hbrGray == NULL ||
		nCtlColor == CTLCOLOR_EDIT || nCtlColor == CTLCOLOR_MSGBOX ||
		nCtlColor == CTLCOLOR_SCROLLBAR)
	{
		return FALSE;
	}

	if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		// only handle requests to draw the space between edit and drop button
		//  in a drop-down combo (not a drop-down list)
		if (!_AfxIsComboBoxControl(hWnd, (UINT)CBS_DROPDOWN))
			return FALSE;
	}

	// set background color and return handle to brush
	LOGBRUSH logbrush;
	VERIFY(::GetObject(hbrGray, sizeof(LOGBRUSH), (LPSTR)&logbrush));
	::SetBkColor(hDC, logbrush.lbColor);
	if (clrText == (COLORREF)-1)
		clrText = ::GetSysColor(COLOR_WINDOWTEXT);  // normal text
	::SetTextColor(hDC, clrText);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Centering dialog support (works for any non-child window)

void CWnd::CenterWindow(CWnd* pAlternateOwner /*= NULL*/)
{
	CRect rcScreen(0, 0, ::GetSystemMetrics(SM_CXSCREEN),
		::GetSystemMetrics(SM_CYSCREEN));

	// hWndOwner is the window we should center ourself in
	HWND hWndOwner = (pAlternateOwner != NULL) ?
			pAlternateOwner->m_hWnd : ::GetWindow(m_hWnd, GW_OWNER);

	// rcParent is the rectange we should center ourself in
	CRect rcParent;
	if (hWndOwner == NULL)
		rcParent = rcScreen;
	else
		::GetWindowRect(hWndOwner, &rcParent);

	// find ideal center point
	int xMid = (rcParent.left + rcParent.right) / 2;
	int yMid = (rcParent.top + rcParent.bottom) / 2;

	// find dialog's upper left based on that
	CRect rcDlg;
	GetWindowRect(&rcDlg);
	int xLeft = xMid - rcDlg.Width() / 2;
	int yTop = yMid - rcDlg.Height() / 2;

	// if the dialog is outside the screen, move it inside
	if (xLeft < 0)
		xLeft = 0;
	else if (xLeft + rcDlg.Width() > rcScreen.right)
		xLeft = rcScreen.right - rcDlg.Width();

	if (yTop < 0)
		yTop = 0;
	else if (yTop + rcDlg.Height() > rcScreen.bottom)
		yTop = rcScreen.bottom - rcDlg.Height();

	SetWindowPos(NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER);
}

/////////////////////////////////////////////////////////////////////////////
// CDialog support for context sensitive help.

LRESULT CDialog::OnCommandHelp(WPARAM, LPARAM lParam)
{
	if (lParam == 0 && m_nIDHelp != 0)
		lParam = HID_BASE_RESOURCE + m_nIDHelp;
	if (lParam != 0)
	{
		AfxGetApp()->WinHelp(lParam);
		return TRUE;
	}
	return FALSE;
}

LRESULT CDialog::OnHelpHitTest(WPARAM, LPARAM)
{
	if (m_nIDHelp != 0)
		return HID_BASE_RESOURCE + m_nIDHelp;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDialog Diagnostics

#ifdef _DEBUG
void CDialog::AssertValid() const
{
	CWnd::AssertValid();
}

void CDialog::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
	AFX_DUMP0(dc, "\nm_lpDialogTemplate = ");
	if (HIWORD(m_lpDialogTemplate) == 0)
		dc << (int)LOWORD(m_lpDialogTemplate);
	else
		dc << m_lpDialogTemplate;
	AFX_DUMP1(dc, "\nm_hDialogTemplate = ", (UINT)m_hDialogTemplate);
	AFX_DUMP1(dc, "\nm_pParentWnd = ", (void*)m_pParentWnd);
	AFX_DUMP1(dc, "\nm_nIDHelp = ", m_nIDHelp);
}

// diagnostic routine to check for and decode dialog templates
// return FALSE if a program error occurs (i.e. bad resource ID or
//   bad dialog styles).
BOOL AFXAPI _AfxCheckDialogTemplate(LPCSTR lpszResource, BOOL bInvisibleChild)
{
	ASSERT(lpszResource != NULL);

	HINSTANCE hInst = AfxFindResourceHandle(lpszResource, RT_DIALOG);
	HRSRC hResource = ::FindResource(hInst, lpszResource, RT_DIALOG);
	if (hResource == NULL)
	{
		if (HIWORD(lpszResource) != 0)
			TRACE1("ERROR: Cannot find dialog template named '%Fs'\n",
				lpszResource);
		else
			TRACE1("ERROR: Cannot find dialog template with IDD 0x%04X\n",
				LOWORD(lpszResource));
		return FALSE;
	}

	if (!bInvisibleChild)
		return TRUE;        // that's all we need to check

	// we must check that the dialog template is for an invisible child
	//  window that can be used for a form-view or dialog-bar
	HGLOBAL hTemplate = ::LoadResource(hInst, hResource);
	if (hTemplate == NULL)
	{
		TRACE0("Warning: LoadResource failed for dialog template\n");
		// this is only a warning, the real call to CreateDialog will fail
		return TRUE;        // not a program error - just out of memory
	}
	// style is first DWORD in resource
	DWORD dwStyle = *(DWORD FAR*)::LockResource(hTemplate);
	::UnlockResource(hTemplate);
	::FreeResource(hTemplate);

	if (dwStyle & WS_VISIBLE)
	{
		if (HIWORD(lpszResource) != 0)
			TRACE1("ERROR: Dialog named '%Fs' must be invisible\n",
				lpszResource);
		else
			TRACE1("ERROR: Dialog with IDD 0x%04X must be invisible\n",
				LOWORD(lpszResource));
		return FALSE;
	}
	if (!(dwStyle & WS_CHILD))
	{
		if (HIWORD(lpszResource) != 0)
			TRACE1("ERROR: Dialog named '%Fs' must have the child style\n",
				lpszResource);
		else
			TRACE1("ERROR: Dialog with IDD 0x%04X must have the child style\n",
				LOWORD(lpszResource));
		return FALSE;
	}

	return TRUE;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
