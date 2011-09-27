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

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Choose Font dialog

IMPLEMENT_DYNAMIC(CFontDialog, CDialog)

CFontDialog::CFontDialog(LPLOGFONT lplfInitial  /* = NULL */,
		DWORD dwFlags /* = CF_EFFECTS | CF_SCREENFONTS */,
		CDC* pdcPrinter /* = NULL */,
		CWnd* pParentWnd /* = NULL */) : CDialog((UINT)0, pParentWnd)
{
	memset(&m_cf, 0, sizeof(m_cf));
	memset(&m_lf, 0, sizeof(m_lf));
	memset(&m_szStyleName, 0, sizeof(m_szStyleName));

	m_nIDHelp = AFX_IDD_FONT;

	m_cf.lStructSize = sizeof(m_cf);
	m_cf.lpszStyle = (LPSTR)&m_szStyleName;
	m_cf.Flags = dwFlags | CF_ENABLEHOOK;
	if (_AfxHelpEnabled())
		m_cf.Flags |= CF_SHOWHELP;
	m_cf.lpfnHook = (COMMDLGPROC)_AfxCommDlgProc;

	if (lplfInitial)
	{
		m_cf.lpLogFont = lplfInitial;
		m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;
		_fmemcpy(&m_lf, m_cf.lpLogFont, sizeof(m_lf));
	}
	else
	{
		m_cf.lpLogFont = &m_lf;
	}

	if (pdcPrinter)
	{
		ASSERT(pdcPrinter->m_hDC != NULL);
		m_cf.hDC = pdcPrinter->m_hDC;
		m_cf.Flags |= CF_PRINTERFONTS;
	}
}

int CFontDialog::DoModal()
{
	ASSERT_VALID(this);
	ASSERT(m_cf.Flags & CF_ENABLEHOOK);
	ASSERT(m_cf.lpfnHook != NULL); // can still be a user hook

	m_cf.hwndOwner = PreModal();
	BOOL bResult = ::ChooseFont(&m_cf);
	PostModal();

	if (bResult)
	{
		// copy logical font from user's initialization buffer (if needed)
		_fmemcpy(&m_lf, m_cf.lpLogFont, sizeof(m_lf));
		return IDOK;
	}
	return IDCANCEL;
}


void CFontDialog::OnOK()
{
	// Common dialogs do not require ::EndDialog
	ASSERT_VALID(this);
	Default();
}

void CFontDialog::OnCancel()
{
	// Common dialogs do not require ::EndDialog
	ASSERT_VALID(this);
	Default();
}

////////////////////////////////////////////////////////////////////////////
// CFontDialog diagnostics

#ifdef _DEBUG
void CFontDialog::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CDialog::Dump(dc);
	AFX_DUMP1(dc, "\nm_cf.hwndOwner = ", (UINT)m_cf.hwndOwner);
	AFX_DUMP1(dc, "\nm_cf.hDC = ", (UINT)m_cf.hDC);
	AFX_DUMP1(dc, "\nm_cf.iPointSize = ", m_cf.iPointSize);
	AFX_DUMP1(dc, "\nm_cf.Flags = ", (LPVOID)m_cf.Flags);
	AFX_DUMP1(dc, "\nm_cf.lpszStyle = ", m_cf.lpszStyle);
	AFX_DUMP1(dc, "\nm_cf.nSizeMin = ", m_cf.nSizeMin);
	AFX_DUMP1(dc, "\nm_cf.nSizeMax = ", m_cf.nSizeMax);
	AFX_DUMP1(dc, "\nm_cf.nFontType = ", m_cf.nFontType);
	AFX_DUMP1(dc, "\nm_cf.rgbColors = ", (LPVOID)m_cf.rgbColors);
	if (m_cf.lpfnHook == (COMMDLGPROC)_AfxCommDlgProc)
		AFX_DUMP0(dc, "\nhook function set to standard MFC hook function");
	else
		AFX_DUMP0(dc, "\nhook function set to non-standard hook function");
}

#endif //_DEBUG

////////////////////////////////////////////////////////////////////////////
