// muscroll.cpp : New control example - MicroScroller
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

#include "paredit.h"
#include "spin.h"

/////////////////////////////////////////////////////////////////////////////
// Example of a dialog with special controls in it

#define NUM_EDIT        4
#define IDC_EDIT_MIN    IDC_EDIT1
#define IDC_BUTTON_MIN  IDC_BUTTON1
	// IDC_EDIT1->IDC_EDIT4 and IDC_BUTTON1->IDC_BUTTON4 must be contiguous

class CSpinEditDlg : public CDialog
{
protected:
	CParsedEdit edit[NUM_EDIT];
public:
	//{{AFX_DATA(CSpinEditDlg)
		enum { IDD = IDD_SPIN_EDIT };
	//}}AFX_DATA
	CSpinEditDlg()
		: CDialog(CSpinEditDlg::IDD)
			{ }

	BOOL OnInitDialog();
	//{{AFX_MSG(CSpinEditDlg)
		virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CSpinEditDlg, CDialog)
	//{{AFX_MSG_MAP(CSpinEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CSpinEditDlg::OnInitDialog()
{
	int value = 1;
	for (int i = 0; i < NUM_EDIT; i++)
	{
		UINT nID = IDC_EDIT_MIN + i;
		edit[i].SubclassEdit(nID, this, PES_NUMBERS);
		SetDlgItemInt(nID, value);
		value++;        // 1, 2, 3, 4

		// associate button with edit item
		CSpinControl* pSpin = (CSpinControl*)GetDlgItem(IDC_BUTTON_MIN + i);
		ASSERT(pSpin != NULL);
			pSpin->SetAssociate(&edit[i]);
	}
	return TRUE;
}

void CSpinEditDlg::OnOK()
{
	int values[NUM_EDIT];
	UINT nID = 0;
	BOOL bOk = TRUE;
	for (int i = 0; bOk && i < NUM_EDIT; i++)
	{
		nID = IDC_EDIT_MIN + i;
		values[i] = GetDlgItemInt(nID, &bOk);
	}

	if (!bOk)
	{
		// report illegal value
		MessageBox("illegal value\n");
		CEdit& badEdit = *(CEdit*)GetDlgItem(nID);
		badEdit.SetSel(0, -1);
		badEdit.SetFocus();
		return;     // don't end dialog
	}

#ifdef _DEBUG
	// dump results, normally you would do something with these
	TRACE("Final values:\n");
	for (i = 0; i < NUM_EDIT; i++)
		TRACE("\t%d\n", values[i]);
#endif
	EndDialog(IDOK);
}

/////////////////////////////////////////////////////////////////////////////
// Run the test

void CTestWindow::OnTestSpinEdit()
{
	HINSTANCE hLibrary;
	if ((hLibrary = LoadLibrary("MUSCROLL.DLL")) < HINSTANCE_ERROR)
	{
		MessageBox("Can not do this test without custom control library");

		// prevent it from happening again
		GetMenu()->EnableMenuItem(IDM_TEST_SPIN_EDIT, MF_DISABLED|MF_GRAYED);
		return;
	}

	TRACE("running dialog with spin controls in it\n");
	CSpinEditDlg dlg;
	dlg.DoModal();

	FreeLibrary(hLibrary);
}


/////////////////////////////////////////////////////////////////////////////
