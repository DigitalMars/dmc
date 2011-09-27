// checkvw.cpp : implementation of the CCheckView class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"
#include "chkbook.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CCheckView, CFormView)

BEGIN_MESSAGE_MAP(CCheckView, CFormView)
	//{{AFX_MSG_MAP(CCheckView)
	ON_COMMAND(ID_EDIT_COMMIT_CHECK, OnEditCommitCheck)
	ON_EN_CHANGE(IDC_AMOUNTNUM, OnAmountNumChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Construction, destruction

CCheckView::CCheckView()
	: CFormView(CCheckView::IDD)
{
	//{{AFX_DATA_INIT(CCheckView)
	m_nCheckNo = 0;
	m_dwCents = 0;
	//}}AFX_DATA_INIT
}


CCheckView::~CCheckView()
{
}

/////////////////////////////////////////////////////////////////////////////
// Operations

void CCheckView::OnUpdate(CView*, LPARAM lHint, CObject* pHint)
{
	// OnUpdate() is called whenever the document has changed and,
	// therefore, the view needs to redisplay some or all of itself.

	CChkBookDoc* pDoc = GetDocument();
	if (pDoc->GetRecordCount() == 0)
		return;

	if (DYNAMIC_DOWNCAST(CFixedLenRecHint, pHint) != NULL)
	{
		m_nCheckNo = LOWORD(lHint) + pDoc->GetFirstCheckNo();
	}
	else
	{
		m_nCheckNo = pDoc->GetActiveCheckNo();
	}

	GetDocument()->GetCheck(m_nCheckNo, m_dwCents, m_strPayTo,
		m_strDate, m_strMemo);

	UpdateData(FALSE);  // Copy the view form object's data to the
						// controls (via DoDataExchange).

	Invalidate();       // Repaint the entire form view.
}


void CCheckView::DoDataExchange(CDataExchange* pDX)
{
	// ClassWizard maintains mappings between data of the CCheckView
	// class and the controls in the dialog template resource for this
	// form view.

	//{{AFX_DATA_MAP(CCheckView)
	DDX_Text(pDX, IDC_CHECKNO, m_nCheckNo);
	DDX_Text(pDX, IDC_DATE, m_strDate);
	DDX_Text(pDX, IDC_MEMO, m_strMemo);
	DDX_Text(pDX, IDC_PAYTO, m_strPayTo);
	DDX_DollarsCents(pDX, IDC_AMOUNTNUM, m_dwCents);
	//}}AFX_DATA_MAP

	// Update the written-out dollars and cents (eg, "nineteen and
	// 98/100ths dollars") after loading or saving the numeric
	// dollars and cents control (IDC_AMOUNTNUM).

	CString str = GetDollarsCentsText(m_dwCents);
	GetDlgItem(IDC_AMOUNTTEXT)->SetWindowText(str);
}


BOOL CCheckView::MaybeCommitDirtyCheck()
{
	// Ask the user to confirm the discarding of uncommitted changes
	// to the active check.
	// Return FALSE if the user wants to cancel the change-selection
	// command that would have discarded the uncommitted changes; and
	// set the focus

	DWORD dwCents;
	CString strPayTo, strDate, strMemo;

	// If there is a validation error, then let UpdateData()
	// return focus to the offending control.
	if (!UpdateData())
		return FALSE;

	CChkBookDoc* pDoc = GetDocument();
	ASSERT(m_nCheckNo == pDoc->GetActiveCheckNo());
	pDoc->GetCheck(m_nCheckNo, dwCents, strPayTo, strDate, strMemo);
	if (   (dwCents == m_dwCents)
		&& (strPayTo == m_strPayTo)
		&& (strDate == m_strDate)
		&& (strMemo == m_strMemo))
		return TRUE;

	switch (AfxMessageBox(IDS_PROMPT_COMMIT_DIRTY_CHECK,
		MB_YESNOCANCEL))
	{
	case IDYES:
		pDoc->UpdateCheck(this, m_nCheckNo, m_dwCents,
			m_strPayTo, m_strDate, m_strMemo);
		return TRUE;

	case IDNO:
		return TRUE;

	case IDCANCEL:
		CMDIChildWnd* pChildWnd = (CMDIChildWnd*)GetParentFrame();
		pChildWnd->GetMDIFrame()->MDIActivate(pChildWnd);
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CCheckView message handlers


void CCheckView::OnEditCommitCheck()
{
	// This handler is called when the user chooses the Edit Commit
	// Check menu command or toolbar button.

	// Copy dialog fields to the view object's data members.
	// If there are validation errors, then abandon the commit.
	if (!UpdateData())
		return;


	ASSERT(m_nCheckNo == GetDocument()->GetActiveCheckNo());

	// Update the document with the values entered by the user for
	// this check.
	GetDocument()->UpdateCheck(this, m_nCheckNo, m_dwCents, m_strPayTo,
		m_strDate, m_strMemo);
}


void CCheckView::OnAmountNumChange()
{
	// When the dollar/cents amount numeric field (eg, "19.98") is changed
	// by the user, then update the dollar/cents text field
	// (eg, "Nineteen and 98/100ths Dollars").

	DWORD dwCents;
	CWnd* pCtlAmountText = GetDlgItem(IDC_AMOUNTTEXT);
	if (GetDollarsCents(GetDlgItem(IDC_AMOUNTNUM), dwCents))
	{
		CString str = GetDollarsCentsText(dwCents);
		pCtlAmountText->SetWindowText(str);
	}
	else
	{
		CString str;
		str.LoadString(IDS_UNKNOWN);
		pCtlAmountText->SetWindowText(str);
	}
}
