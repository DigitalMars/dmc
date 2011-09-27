// PrefDial.cpp : implementation file
//
// Part of TMLRead, Symantec C++ 7.0 demonstration program
// Copyright (C) 1995 Symantec Corporation
//
// last mod:
//   BTO, 1/95 (release)

#include "stdafx.h"
#include "tmlread.h"
#include "PrefDial.h"
#include "ViewHdrs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPrefDialog dialog


CPrefDialog::CPrefDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPrefDialog::IDD, pParent)
{

	//{{AFX_DATA_INIT(CPrefDialog)
	nParVSpace		 = 0;
	nMargin		 = 0;
	nIndent		 = 0;
	//}}AFX_DATA_INIT
}

void CPrefDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefDialog)
	DDX_Text(pDX, IDC_PARVSPACE, nParVSpace);
	DDV_MinMaxInt(pDX, nParVSpace, 0, 100);
	DDX_Text(pDX, IDC_MARGIN, nMargin);
	DDV_MinMaxInt(pDX, nMargin, 0, 50);
	DDX_Text(pDX, IDC_INDENT, nIndent);
	DDV_MinMaxInt(pDX, nIndent, 0, 120);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPrefDialog, CDialog)
	//{{AFX_MSG_MAP(CPrefDialog)
	ON_BN_CLICKED(ID_PREFS_DEFAULT,OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPrefDialog message handlers

////
//// OnDefault
//// Called when the Default button in the Preferences dialog is clicked
////
void CPrefDialog::OnDefault()
{
	// TODO: Add your control notification handler code here

    // Set data members to default values
    nParVSpace = CTMLReadView::eDftParVSpace;
    nMargin    = CTMLReadView::eDftMargin;
    nIndent    = CTMLReadView::eDftIndent;

    // Transfer these values to the controls.
    // UpdateData calls DoDataExchange to effect the transfer.
    // The argument determines the direction of the transfer:
    // TRUE causes values to move from the controls to the data members;
    // FALSE, used here, transfers the values of the data members to the controls.
    UpdateData( FALSE);
}
