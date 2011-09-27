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
// Choose Color dialog

IMPLEMENT_DYNAMIC(CColorDialog, CDialog)

BEGIN_MESSAGE_MAP(CColorDialog, CDialog)
	//{{AFX_MSG_MAP(CColorDialog)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COLORREF AFXAPI_DATA CColorDialog::clrSavedCustom[16] = { RGB(255, 255, 255),
	RGB(255, 255, 255), RGB(255, 255, 255), RGB(255, 255, 255),
	RGB(255, 255, 255), RGB(255, 255, 255), RGB(255, 255, 255),
	RGB(255, 255, 255), RGB(255, 255, 255), RGB(255, 255, 255),
	RGB(255, 255, 255), RGB(255, 255, 255), RGB(255, 255, 255),
	RGB(255, 255, 255), RGB(255, 255, 255), RGB(255, 255, 255) };

CColorDialog::CColorDialog(COLORREF clrInit /* = 0 */,
		DWORD dwFlags /* = 0 */,
		CWnd* pParentWnd /* = NULL */) : CDialog((UINT)0, pParentWnd)
{
	memset(&m_cc, 0, sizeof(m_cc));
	m_nIDHelp = AFX_IDD_COLOR;

	m_cc.lStructSize = sizeof(m_cc);
	m_cc.lpCustColors = (COLORREF FAR*)&clrSavedCustom;
	m_cc.Flags = dwFlags | CC_ENABLEHOOK;
	if (_AfxHelpEnabled())
		m_cc.Flags |= CC_SHOWHELP;
	m_cc.lpfnHook = (COMMDLGPROC)_AfxCommDlgProc;

	if ((m_cc.rgbResult = clrInit) != 0)
		m_cc.Flags |= CC_RGBINIT;
}

int CColorDialog::DoModal()
{
	ASSERT_VALID(this);
	ASSERT(m_cc.Flags & CC_ENABLEHOOK);
	ASSERT(m_cc.lpfnHook != NULL); // can still be a user hook

	m_cc.hwndOwner = PreModal();
	BOOL bResult = ::ChooseColor(&m_cc);
	PostModal();

	return bResult ? IDOK : IDCANCEL;
}

BOOL CColorDialog::OnColorOK()
{
	ASSERT_VALID(this);
	// Do not call Default() if you override
	return FALSE;
}

void CColorDialog::SetCurrentColor(COLORREF clr)
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);

	SendMessage(_afxNMsgSETRGB, 0, (DWORD)clr);
}

void CColorDialog::OnOK()
{
	// Common dialogs do not require ::EndDialog
	ASSERT_VALID(this);

	Default();
}

void CColorDialog::OnCancel()
{
	// Common dialogs do not require ::EndDialog
	ASSERT_VALID(this);
	Default();
}

// The color tracker in the COMMDLG.DLL can't handle grey backgrounds,
//  so we force the default with this override.

HBRUSH CColorDialog::OnCtlColor(CDC*, CWnd*, UINT)
{
	return (HBRUSH)Default();
}

////////////////////////////////////////////////////////////////////////////
// CColorDialog diagnostics

#ifdef _DEBUG
void CColorDialog::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);
	CDialog::Dump(dc);

	AFX_DUMP1(dc, "\nm_cc.hwndOwner = ", (UINT)m_cc.hwndOwner);
	AFX_DUMP1(dc, "\nm_cc.rgbResult = ", (LPVOID)m_cc.rgbResult);
	AFX_DUMP1(dc, "\nm_cc.Flags = ", (LPVOID)m_cc.Flags);
	AFX_DUMP0(dc, "\nm_cc.lpCustColors ");
	for (int iClr = 0; iClr < 16; iClr++)
		AFX_DUMP1(dc, "\n\t", (LPVOID)m_cc.lpCustColors[iClr]);
	if (m_cc.lpfnHook == (COMMDLGPROC)_AfxCommDlgProc)
		AFX_DUMP0(dc, "\nhook function set to standard MFC hook function");
	else
		AFX_DUMP0(dc, "\nhook function set to non-standard hook function");
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////////////
