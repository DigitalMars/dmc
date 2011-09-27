// AddQyDlg.h : header file
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

/////////////////////////////////////////////////////////////////////////////
// CAddQueryDlg dialog

class CAddQueryDlg : public CDialog
{
// Construction
public:
	CDaoQueryDef * m_pQueryDef;
	CDaoDatabase * m_pDatabase;
	void initializer();
	CAddQueryDlg(CDaoDatabase *pDatabase, CString strQueryName, CWnd* pParent = NULL);
	CAddQueryDlg(CWnd* pParent = NULL);   // standard constructor

	CDaoQueryDefInfo m_QI;

// Dialog Data
	//{{AFX_DATA(CAddQueryDlg)
	enum { IDD = IDD_ADD_QUERYDEF_DLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddQueryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddQueryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddQueryDef();
	afx_msg void OnDeleteQuerydef();
	afx_msg void OnDone();
	afx_msg void OnModifyQueryDef();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
