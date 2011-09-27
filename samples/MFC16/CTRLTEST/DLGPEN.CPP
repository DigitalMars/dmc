// dlgpen.cpp : regular dialog template using pen controls
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
// Dialog class

class CPenEditDlg : public CDialog
{
public:
	//{{AFX_DATA(CPenEditDlg)
		enum { IDD = IDD_SUB_PENEDIT };
	//}}AFX_DATA
	CPenEditDlg()
		: CDialog(CPenEditDlg::IDD)
		{ }

	// access to controls is through inline helpers
	CHEdit& Edit1()
				{ return *(CHEdit*)GetDlgItem(IDC_EDIT1); }
	CHEdit& Edit2()
				{ return *(CHEdit*)GetDlgItem(IDC_EDIT2); }
	CBEdit& Edit3()
				{ return *(CBEdit*)GetDlgItem(IDC_EDIT3); }
	CBEdit& Edit4()
				{ return *(CBEdit*)GetDlgItem(IDC_EDIT4); }

	BOOL OnInitDialog();
	//{{AFX_MSG(CPenEditDlg)
		virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CPenEditDlg, CDialog)
	//{{AFX_MSG_MAP(CPenEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CPenEditDlg::OnInitDialog()
{
	// any special init goes here

	// There are many ways to customize HEdit and BEdit controls which
	//  can be done in OnInitDialog.
	//  Here is just one example of setting BOXLAYOUT to draw complete
	//  rectangles for boxes (regardless of global defaults which is
	//  usually combs).
	CBEdit* pBEdit = &Edit4();

	CRect rect;
	pBEdit->GetClientRect(rect);        // how high is it ?
	BOXLAYOUT boxlayout;
	pBEdit->GetBoxLayout(&boxlayout);   // what are the current settings ?
	boxlayout.style |= BXS_RECT;
	boxlayout.cyCusp = rect.Height() - 10;  // a reasonable size
	VERIFY(pBEdit->SetBoxLayout(&boxlayout));

	return TRUE;
}

void CPenEditDlg::OnOK()
{
#ifdef _DEBUG
	// dump results, normally you would do something with these
	CString s;
	Edit1().GetWindowText(s);
	TRACE("edit1 = '%s'\n", (const char*) s);
	Edit2().GetWindowText(s);
	TRACE("edit2 = '%s'\n", (const char*) s);
	Edit3().GetWindowText(s);
	TRACE("edit3 = '%s'\n", (const char*) s);
	Edit4().GetWindowText(s);
	TRACE("edit4 = '%s'\n", (const char*) s);
#endif

	EndDialog(IDOK);
}

/////////////////////////////////////////////////////////////////////////////
// Run the test

void CTestWindow::OnTestPenEditFromTemplate()
{
	TRACE("running dialog with BEDIT controls in it\n");
	CPenEditDlg dlg;
	dlg.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
