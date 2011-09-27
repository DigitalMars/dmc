// candlg.cpp : implementation of the CCancelDlg class
//

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "vcterm.h"
#include "CanDlg.h"

#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCancelDlg dialog


CCancelDlg::CCancelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCancelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCancelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCancelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCancelDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCancelDlg, CDialog)
	//{{AFX_MSG_MAP(CCancelDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCancelDlg message handlers

void CCancelDlg::OnCancel()
{
	// Signal MainFrame window to cease transmitting data.
	((CMainFrame*)GetParent())->m_bCancel = TRUE;
	DestroyWindow();
}

void CCancelDlg::PostNcDestroy()
{
	delete this;
}
