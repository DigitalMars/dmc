// datadlg.h : header file
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

#include "bulkset.h"

/////////////////////////////////////////////////////////////////////////////
// CDataDialog dialog

class CDataDialog : public CDialog
{
// Construction
public:
	CDataDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataDialog)
	enum { IDD = IDD_GETDATA_DAILOG };
	CListCtrl	m_listData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CDynamicBulkSet* m_prs;

	// Generated message map functions
	//{{AFX_MSG(CDataDialog)
	afx_msg void OnFirst();
	afx_msg void OnLast();
	afx_msg void OnNext();
	afx_msg void OnPrev();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetRecordset(CDynamicBulkSet* prs);
	virtual int DoModal();

	void AddColumns();
	void FillData();

	virtual BOOL OnInitDialog();
};
