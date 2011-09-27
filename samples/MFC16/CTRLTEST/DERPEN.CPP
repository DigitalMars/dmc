// derpen.cpp : C++ derived HEdit/BEdit control example
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "ctrltest.h"


// we need the MFC extensions for PenWindows
#include <afxpen.h>

/////////////////////////////////////////////////////////////////////////////

class CDerPenEditDlg : public CDialog
{
protected:
	// 2 Handwriting edit items
	CHEdit m_edit1, m_edit2;
	// 2 Boxed Handwriting edit items
	CBEdit m_edit3, m_edit4;
	// static labels for them all
	CStatic m_static1, m_static2, m_static3, m_static4;

	// font for the dialog
	CFont*  m_pFont;
public:
	//{{AFX_DATA(CDerPenEditDlg)
		enum { IDD = IDD_DERIVED_EDIT };
	//}}AFX_DATA
	CDerPenEditDlg()
		: CDialog(CDerPenEditDlg::IDD)
		{ }

	BOOL OnInitDialog();
	void OnSetFont(CFont* pFont)
			{ m_pFont = pFont; }

	//{{AFX_MSG(CDerPenEditDlg)
		virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CDerPenEditDlg, CDialog)
	//{{AFX_MSG_MAP(CDerPenEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// pen helpers

void SetAlc(CHEdit& rHEdit, ALC alcNew)
{
	RC rc;      // recognition context
	VERIFY(rHEdit.GetRC(&rc));
	rc.alc = alcNew;
	VERIFY(rHEdit.SetRC(&rc));
}

/////////////////////////////////////////////////////////////////////////////

BOOL CDerPenEditDlg::OnInitDialog()
{
	// This is an example of the _incorrect_ way to create a dialog
	// see other comments in DERTEST.CPP
	const int yStart = 8;
	int cyGap = 36;

	CPoint whereLabel(10, yStart + 4);
	CSize sizeLabel(80, 24);

	CPoint whereEdit(90, yStart);
	CSize sizeEdit(140, 30);

	m_static1.Create("Letters:", WS_VISIBLE | WS_CHILD | SS_LEFT,
		CRect(whereLabel, sizeLabel), this, -1);
	m_static1.SetFont(m_pFont);
	whereLabel.y += cyGap;
	m_edit1.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER,
		CRect(whereEdit, sizeEdit), this, IDC_EDIT1);
	m_edit1.SetFont(m_pFont);
	SetAlc(m_edit1, ALC_ALPHA);
	whereEdit.y += cyGap;

	m_static2.Create("Numbers:", WS_VISIBLE | WS_CHILD | SS_LEFT,
		CRect(whereLabel, sizeLabel), this, -1);
	m_static2.SetFont(m_pFont);
	whereLabel.y += cyGap;
	m_edit2.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER,
		CRect(whereEdit, sizeEdit), this, IDC_EDIT2);
	m_edit2.SetFont(m_pFont);
	SetAlc(m_edit2, ALC_NUMERIC);
	whereEdit.y += cyGap;

	// followed by 2 boxed edit items (slightly taller)
	sizeEdit.cy += 10;
	cyGap += 10;
	m_static3.Create("Letters:", WS_VISIBLE | WS_CHILD | SS_LEFT,
		CRect(whereLabel, sizeLabel), this, -1);
	m_static3.SetFont(m_pFont);
	whereLabel.y += cyGap;
	m_edit3.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER,
		CRect(whereEdit, sizeEdit), this, IDC_EDIT3);
	m_edit3.SetFont(m_pFont);
	SetAlc(m_edit3, ALC_ALPHA);
	whereEdit.y += cyGap;

	m_static4.Create("Numbers:", WS_VISIBLE | WS_CHILD | SS_LEFT,
		CRect(whereLabel, sizeLabel), this, -1);
	whereLabel.y += cyGap;
	m_static4.SetFont(m_pFont);
	m_edit4.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER,
		CRect(whereEdit, sizeEdit), this, IDC_EDIT4);
	m_edit4.SetFont(m_pFont);
	SetAlc(m_edit4, ALC_NUMERIC);
	whereEdit.y += cyGap;

	// change the dialog height so everything fits
	int yBottom = whereEdit.y + cyGap * 2; // extra space
	CRect rect;
	GetWindowRect(rect);
	VERIFY(SetWindowPos(NULL, -1, -1, rect.Width(), yBottom,
		SWP_NOMOVE|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOACTIVATE));

	// set focus to first one
	m_edit1.SetFocus();
	return FALSE;   // focus set
}

void CDerPenEditDlg::OnOK()
{
#ifdef _DEBUG
	// dump results, normally you would do something with these
	CString s;
	m_edit1.GetWindowText(s);
	TRACE("edit1 = '%s'\n", (const char*) s);
	m_edit2.GetWindowText(s);
	TRACE("edit2 = '%s'\n", (const char*) s);
	m_edit3.GetWindowText(s);
	TRACE("edit3 = '%s'\n", (const char*) s);
	m_edit4.GetWindowText(s);
	TRACE("edit4 = '%s'\n", (const char*) s);
#endif

	EndDialog(IDOK);
}

/////////////////////////////////////////////////////////////////////////////
// Run the test

void CTestWindow::OnTestPenEditFromCode()
{
	TRACE("running dialog containing CBEdit objects\n");
	CDerPenEditDlg dlg;
	dlg.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
