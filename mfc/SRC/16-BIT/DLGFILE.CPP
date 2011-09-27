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
#include <dlgs.h>       // for standard control IDs for commdlg

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

static char BASED_CODE szLBSELCH[] = LBSELCHSTRING;
static char BASED_CODE szSHAREVI[] = SHAREVISTRING;
static char BASED_CODE szFILEOK[] = FILEOKSTRING;
static char BASED_CODE szCOLOROK[] = COLOROKSTRING;
static char BASED_CODE szSETRGB[] = SETRGBSTRING;

static const UINT NEAR nMsgLBSELCHANGE = ::RegisterWindowMessage(szLBSELCH);
static const UINT NEAR nMsgSHAREVI = ::RegisterWindowMessage(szSHAREVI);
static const UINT NEAR nMsgFILEOK = ::RegisterWindowMessage(szFILEOK);
static const UINT NEAR nMsgCOLOROK = ::RegisterWindowMessage(szCOLOROK);
const UINT NEAR _afxNMsgSETRGB = ::RegisterWindowMessage(szSETRGB);


UINT CALLBACK AFX_EXPORT
_AfxCommDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_SETFONT || message == WM_INITDIALOG)
		return (UINT)_AfxDlgProc(hWnd, message, wParam, lParam);

	if (message == WM_COMMAND && LOWORD(wParam) == pshHelp)
	{
		// just translate the message into the AFX standard help command.
		SendMessage(hWnd, WM_COMMAND, ID_HELP, 0);
		return TRUE;
	}

	if (message < 0xC000)
		// not a ::RegisterWindowMessage message
		return 0;

	// RegisterWindowMessage - does not copy to lastState buffer, so
	// CWnd::GetCurrentMessage and CWnd::Default will NOT work
	// while in these handlers

	// Get our Window
	// assume it is already wired up to a permanent one
	CDialog* pDlg = (CDialog*) CWnd::FromHandlePermanent(hWnd);
	ASSERT(pDlg != NULL);
	ASSERT(pDlg->m_hWnd == hWnd);
	ASSERT(pDlg->IsKindOf(RUNTIME_CLASS(CDialog)));

	// Dispatch special commdlg messages through our virtual callbacks
	if (message == nMsgSHAREVI)
	{
		ASSERT(pDlg->IsKindOf(RUNTIME_CLASS(CFileDialog)));
		return ((CFileDialog*)pDlg)->OnShareViolation((LPCSTR)lParam);
	}
	else if (message == nMsgFILEOK)
	{
		ASSERT(pDlg->IsKindOf(RUNTIME_CLASS(CFileDialog)));
		return ((CFileDialog*)pDlg)->OnFileNameOK();
	}
	else if (message == nMsgLBSELCHANGE)
	{
		ASSERT(pDlg->IsKindOf(RUNTIME_CLASS(CFileDialog)));
		((CFileDialog*)pDlg)->OnLBSelChangedNotify(wParam, LOWORD(lParam),
				HIWORD(lParam));
		return 0;
	}
	else if (message == nMsgCOLOROK)
	{
		ASSERT(pDlg->IsKindOf(RUNTIME_CLASS(CColorDialog)));
		return ((CColorDialog*)pDlg)->OnColorOK();
	}
	else if (message == _afxNMsgSETRGB)
	{
		// nothing to do here, since this is a SendMessage
		return 0;
	}

	TRACE0("_AfxCommDlgProc: received unknown user message, returning 0\n");
	return 0;
}

////////////////////////////////////////////////////////////////////////////
// FileOpen/FileSaveAs common dialog helper

IMPLEMENT_DYNAMIC(CFileDialog, CDialog)

CFileDialog::CFileDialog(BOOL bOpenFileDialog,
		LPCSTR lpszDefExt /* = NULL */,
		LPCSTR lpszFileName /* = NULL */,
		DWORD dwFlags /* = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT */,
		LPCSTR lpszFilter /* = NULL */,
		CWnd* pParentWnd /* = NULL */) : CDialog((UINT)0, pParentWnd)
{
	memset(&m_ofn, 0, sizeof(m_ofn)); // initialize structure to 0/NULL
	m_szFileName[0] = '\0';
	m_szFileTitle[0] = '\0';

	m_bOpenFileDialog = bOpenFileDialog;
	m_nIDHelp = bOpenFileDialog ? AFX_IDD_FILEOPEN : AFX_IDD_FILESAVE;

	m_ofn.lStructSize = sizeof(m_ofn);
	m_ofn.lpstrFile = (LPSTR)&m_szFileName;
	m_ofn.nMaxFile = sizeof(m_szFileName);
	m_ofn.lpstrDefExt = lpszDefExt;
	m_ofn.lpstrFileTitle = (LPSTR)m_szFileTitle;
	m_ofn.nMaxFileTitle = sizeof(m_szFileTitle);
	m_ofn.Flags |= dwFlags | OFN_ENABLEHOOK;
	if (_AfxHelpEnabled())
		m_ofn.Flags |= OFN_SHOWHELP;
	m_ofn.lpfnHook = (COMMDLGPROC)_AfxCommDlgProc;

	// setup initial file name
	if (lpszFileName != NULL)
		_AfxStrCpy(m_szFileName, lpszFileName, sizeof(m_szFileName));

	// Translate filter into commdlg format (lots of \0)
	if (lpszFilter != NULL)
	{
		m_strFilter = lpszFilter;
		LPSTR pch = m_strFilter.GetBuffer(0); // modify the buffer in place
		// MFC delimits with '|' not '\0'
		while ((pch = _AfxStrChr(pch, '|')) != NULL)
			*pch++ = '\0';
		m_ofn.lpstrFilter = m_strFilter;
		// do not call ReleaseBuffer() since the string contains '\0' characters
	}
}

int CFileDialog::DoModal()
{
	ASSERT_VALID(this);
	ASSERT(m_ofn.Flags & OFN_ENABLEHOOK);
	ASSERT(m_ofn.lpfnHook != NULL); // can still be a user hook

	BOOL bResult;

	m_ofn.hwndOwner = PreModal();
	if (m_bOpenFileDialog)
		bResult = ::GetOpenFileName(&m_ofn);
	else
		bResult = ::GetSaveFileName(&m_ofn);
	PostModal();

	return bResult ? IDOK : IDCANCEL;
}

CString CFileDialog::GetFileName() const
{
	ASSERT_VALID(this);
	char szFile[_MAX_PATH];

	if (m_ofn.nFileExtension == 0 ||
			m_ofn.lpstrFile[m_ofn.nFileExtension] == '\0')
		return m_ofn.lpstrFile + m_ofn.nFileOffset;
	else
	{
		ASSERT(m_ofn.nFileExtension - m_ofn.nFileOffset < sizeof(szFile));
		_AfxStrCpy(szFile, m_ofn.lpstrFile + m_ofn.nFileOffset,
				m_ofn.nFileExtension - m_ofn.nFileOffset);
		return szFile;
	}
}

UINT CFileDialog::OnShareViolation(LPCSTR)
{
	ASSERT_VALID(this);

	// Do not call Default() if you override
	return OFN_SHAREWARN; // default
}

BOOL CFileDialog::OnFileNameOK()
{
	ASSERT_VALID(this);

	// Do not call Default() if you override
	return FALSE;
}

void CFileDialog::OnLBSelChangedNotify(UINT, UINT, UINT)
{
	ASSERT_VALID(this);

	// Do not call Default() if you override
	// no default processing needed
}

void CFileDialog::OnOK()
{
	// Common dialogs do not require ::EndDialog
	ASSERT_VALID(this);
	Default();
}

void CFileDialog::OnCancel()
{
	// Common dialogs do not require ::EndDialog
	ASSERT_VALID(this);
	Default();
}

////////////////////////////////////////////////////////////////////////////
// CFileDialog diagnostics

#ifdef _DEBUG
void CFileDialog::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CDialog::Dump(dc);

	if (m_bOpenFileDialog)
		AFX_DUMP0(dc, "\nFile open dialog");
	else
		AFX_DUMP0(dc, "\nFile save dialog");
	AFX_DUMP1(dc, "\nm_ofn.hwndOwner = ", (UINT)m_ofn.hwndOwner);
	AFX_DUMP1(dc, "\nm_ofn.nFilterIndex = ", m_ofn.nFilterIndex);
	AFX_DUMP1(dc, "\nm_ofn.lpstrFile = ", m_ofn.lpstrFile);
	AFX_DUMP1(dc, "\nm_ofn.nMaxFile = ", m_ofn.nMaxFile);
	AFX_DUMP1(dc, "\nm_ofn.lpstrFileTitle = ", m_ofn.lpstrFileTitle);
	AFX_DUMP1(dc, "\nm_ofn.nMaxFileTitle = ", m_ofn.nMaxFileTitle);
	AFX_DUMP1(dc, "\nm_ofn.lpstrTitle = ", m_ofn.lpstrTitle);
	AFX_DUMP1(dc, "\nm_ofn.Flags = ", (LPVOID)m_ofn.Flags);
	AFX_DUMP1(dc, "\nm_ofn.lpstrDefExt = ", m_ofn.lpstrDefExt);
	AFX_DUMP1(dc, "\nm_ofn.nFileOffset = ", m_ofn.nFileOffset);
	AFX_DUMP1(dc, "\nm_ofn.nFileExtension = ", m_ofn.nFileExtension);

	AFX_DUMP0(dc, "\nm_ofn.lpstrFilter = ");
	LPCSTR lpstrItem = m_ofn.lpstrFilter;
	char* pszBreak = "|";

	while (lpstrItem != NULL && *lpstrItem != '\0')
	{
		dc << lpstrItem << pszBreak;
		lpstrItem += lstrlen(lpstrItem) + 1;
	}
	if (lpstrItem != NULL)
		dc << pszBreak;

	AFX_DUMP0(dc, "\nm_ofn.lpstrCustomFilter = ");
	lpstrItem = m_ofn.lpstrCustomFilter;
	while (lpstrItem != NULL && *lpstrItem != '\0')
	{
		dc << lpstrItem << pszBreak;
		lpstrItem += lstrlen(lpstrItem) + 1;
	}
	if (lpstrItem != NULL)
		dc << pszBreak;

	if (m_ofn.lpfnHook == (COMMDLGPROC)_AfxCommDlgProc)
		AFX_DUMP0(dc, "\nhook function set to standard MFC hook function");
	else
		AFX_DUMP0(dc, "\nhook function set to non-standard hook function");
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////////////
