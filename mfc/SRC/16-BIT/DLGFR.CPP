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
#include <stddef.h>     // for offsetof macro

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Find/FindReplace dialogs

IMPLEMENT_DYNAMIC(CFindReplaceDialog, CDialog)

CFindReplaceDialog::CFindReplaceDialog()
{
	memset(&m_fr, 0, sizeof(m_fr));
	m_szFindWhat[0] = '\0';
	m_szReplaceWith[0] = '\0';

	m_fr.Flags = FR_ENABLEHOOK;
	if (_AfxHelpEnabled())
		m_fr.Flags |= FR_SHOWHELP;
	m_fr.lpfnHook = (COMMDLGPROC)_AfxCommDlgProc;
	m_fr.lStructSize = sizeof(m_fr);
	m_fr.lpstrFindWhat = (LPSTR)m_szFindWhat;
}

void CFindReplaceDialog::PostNcDestroy()
{
	ASSERT(m_hWnd == NULL);
	delete this;
}


BOOL CFindReplaceDialog::Create(BOOL bFindDialogOnly,
		LPCSTR lpszFindWhat,
		LPCSTR lpszReplaceWith /* = NULL */,
		DWORD dwFlags /* = FR_DOWN */,
		CWnd* pParentWnd /* = NULL */)
{
	ASSERT_VALID(this);
	ASSERT(m_fr.Flags & FR_ENABLEHOOK);
	ASSERT(m_fr.lpfnHook != NULL);

	HWND hWnd;

	m_nIDHelp = bFindDialogOnly ? AFX_IDD_FIND : AFX_IDD_REPLACE;

	m_fr.wFindWhatLen = sizeof(m_szFindWhat);
	m_fr.lpstrReplaceWith = (LPSTR)m_szReplaceWith;
	m_fr.wReplaceWithLen = sizeof(m_szReplaceWith);
	m_fr.Flags |= dwFlags;

	if (pParentWnd == NULL)
		m_fr.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	else
	{
		ASSERT_VALID(pParentWnd);
		m_fr.hwndOwner = pParentWnd->m_hWnd;
	}
	ASSERT(m_fr.hwndOwner != NULL); // must have a parent for modeless dialog


	if (lpszFindWhat)
		_AfxStrCpy(m_szFindWhat, lpszFindWhat, sizeof(m_szFindWhat));

	if (lpszReplaceWith)
		_AfxStrCpy(m_szReplaceWith, lpszReplaceWith, sizeof(m_szReplaceWith));

	_AfxHookWindowCreate(this);
	if (bFindDialogOnly)
		hWnd = ::FindText(&m_fr);
	else
		hWnd = ::ReplaceText(&m_fr);
	if (!_AfxUnhookWindowCreate())
		PostNcDestroy();

	if (hWnd == NULL)
		return FALSE;
	ASSERT(hWnd == m_hWnd);
	return TRUE;
}

void CFindReplaceDialog::OnOK()
{
	// Common dialogs do not require ::EndDialog
	ASSERT_VALID(this);
	Default();
}

void CFindReplaceDialog::OnCancel()
{
	// Common dialogs do not require ::EndDialog
	ASSERT_VALID(this);
	Default();
}

CFindReplaceDialog* PASCAL CFindReplaceDialog::GetNotifier(LPARAM lParam)
{
	ASSERT(lParam != NULL);
	CFindReplaceDialog* pDlg;

	pDlg = (CFindReplaceDialog*)(lParam - offsetof(CFindReplaceDialog, m_fr));
	ASSERT_VALID(pDlg);
	ASSERT(pDlg->IsKindOf(RUNTIME_CLASS(CFindReplaceDialog)));

	return pDlg;
}

////////////////////////////////////////////////////////////////////////////
// CFindReplaceDialog diagnostics

#ifdef _DEBUG
void CFindReplaceDialog::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CDialog::Dump(dc);

	AFX_DUMP1(dc, "\nm_fr.hwndOwner = ", (UINT)m_fr.hwndOwner);
	AFX_DUMP1(dc, "\nm_fr.Flags = ", (LPVOID)m_fr.Flags);
	AFX_DUMP1(dc, "\nm_fr.lpstrFindWhat = ", m_fr.lpstrFindWhat);
	AFX_DUMP1(dc, "\nm_fr.lpstrReplaceWith = ", m_fr.lpstrReplaceWith);
	if (m_fr.lpfnHook == (COMMDLGPROC)_AfxCommDlgProc)
		AFX_DUMP0(dc, "\nhook function set to standard MFC hook function");
	else
		AFX_DUMP0(dc, "\nhook function set to non-standard hook function");
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////////////
