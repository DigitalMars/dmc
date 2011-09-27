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
#include <malloc.h>

#ifdef AFX_OLE2_SEG
#pragma code_seg(AFX_OLE2_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Common code for all OLE UI dialogs

UINT CALLBACK AFX_EXPORT
_AfxOleHookProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_SETFONT || message == WM_INITDIALOG)
		return (UINT)_AfxDlgProc(hWnd, message, wParam, lParam);

	if (message == WM_COMMAND && LOWORD(wParam) == ID_OLEUIHELP)
	{
		// just translate the message into the AFX standard help command.
		SendMessage(hWnd, WM_COMMAND, ID_HELP, 0);
		return TRUE;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(COleDialog, CDialog)

COleDialog::COleDialog(CWnd* pParentWnd) :
	CDialog((UINT)0, pParentWnd)
{
	m_nLastError = -1;       // no error
}

void COleDialog::OnOK()
{
	// OLE dialogs do not require ::EndDialog
	ASSERT_VALID(this);
	Default();
}

void COleDialog::OnCancel()
{
	// OLE dialogs do not require ::EndDialog
	ASSERT_VALID(this);
	Default();
}

int COleDialog::MapResult(UINT nResult)
{
	// store it for GetLastError()
	m_nLastError = nResult;

	// map OLEUI_OK & OLEUI_CANCEL to IDOK & IDCANCEL
	if (nResult == OLEUI_OK)
		return IDOK;
	if (nResult == OLEUI_CANCEL)
		return IDCANCEL;

	// otherwise, some sort of error
	return IDABORT;
}

/////////////////////////////////////////////////////////////////////////////
// COleDialog diagnostics

#ifdef _DEBUG
void COleDialog::Dump(CDumpContext& dc) const
{
	CDialog::Dump(dc);
	AFX_DUMP1(dc, "\nm_nLastError = ", m_nLastError);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COleBusyDialog implementation

IMPLEMENT_DYNAMIC(COleBusyDialog, COleDialog)

COleBusyDialog::COleBusyDialog(HTASK htaskBlocking, BOOL bNotResponding,
	DWORD dwFlags, CWnd* pParentWnd) : COleDialog(pParentWnd)
{
	memset(&m_bz, 0, sizeof(m_bz)); // initialize structure to 0/NULL

	// fill in common part
	m_bz.cbStruct = sizeof(m_bz);
	m_bz.dwFlags = dwFlags;
	if (bNotResponding)
		m_bz.dwFlags |= BZ_NOTRESPONDINGDIALOG;
	m_bz.lpfnHook = _AfxOleHookProc;

	// specific for this dialog
	m_bz.hTask = htaskBlocking;

	ASSERT_VALID(this);
}

COleBusyDialog::~COleBusyDialog()
{
}

int COleBusyDialog::DoModal()
{
	// Note: we don't call EnableModeless because that in itself implies
	//  outgoing calls.  This dialog is normally brought up when an outgoing
	//  call cannot be made.

	m_bz.hWndOwner = _AfxGetSafeOwner(m_pParentWnd);
	_AfxHookWindowCreate(this);
	int iResult = ::OleUIBusy(&m_bz);
	_AfxUnhookWindowCreate();   // just in case
	Detach();                   // just in case

	if (iResult == OLEUI_CANCEL)
		return IDCANCEL;

	if (iResult == OLEUI_BZ_SWITCHTOSELECTED)
		m_selection = switchTo;
	else if (iResult == OLEUI_BZ_RETRYSELECTED)
		m_selection = retry;
	else if (iResult == OLEUI_BZ_CALLUNBLOCKED)
		m_selection = callUnblocked;
	else
		ASSERT(FALSE);

	return IDOK;
}

/////////////////////////////////////////////////////////////////////////////
// COleBusyDialog diagnostics

#ifdef _DEBUG
void COleBusyDialog::Dump(CDumpContext& dc) const
{
	COleDialog::Dump(dc);

	AFX_DUMP1(dc, "\nm_bz.cbStruct = ", m_bz.cbStruct);
	AFX_DUMP1(dc, "\nm_bz.dwFlags = ", (LPVOID)m_bz.dwFlags);
	AFX_DUMP1(dc, "\nm_bz.hWndOwner = ", (UINT)m_bz.hWndOwner);
	AFX_DUMP1(dc, "\nm_bz.lpszCaption = ", m_bz.lpszCaption);
	AFX_DUMP1(dc, "\nm_bz.lCustData = ", (LPVOID)m_bz.lCustData);
	AFX_DUMP1(dc, "\nm_bz.hInstance = ", (UINT)m_bz.hInstance);
	AFX_DUMP1(dc, "\nm_bz.lpszTemplate = ", (LPVOID)m_bz.lpszTemplate);
	AFX_DUMP1(dc, "\nm_bz.hResource = ", (UINT)m_bz.hResource);
	if (m_bz.lpfnHook == _AfxOleHookProc)
		AFX_DUMP0(dc, "\nhook function set to standard MFC hook function");
	else
		AFX_DUMP0(dc, "\nhook function set to non-standard hook function");

	AFX_DUMP1(dc, "\nm_bz.hTask = ", (UINT)m_bz.hTask);
}
#endif

/////////////////////////////////////////////////////////////////////////////
