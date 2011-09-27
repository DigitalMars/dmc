// DlgParam.h : header file
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
// CDlgParams dialog

class CDlgParams : public CDialog
{
// Construction
public:
	CDlgParams(CWnd* pParent = NULL);   // standard constructor

	void SetInfo(CDaoDatabase* pDB,LPCTSTR lpszQueryDef);
// Dialog Data
	//{{AFX_DATA(CDlgParams)
	enum { IDD = IDD_DIALOG2 };
	CListCtrlEx	m_ctlList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgParams)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDaoDatabase* m_pDB;
	CString m_strQueryDef;

	// Generated message map functions
	//{{AFX_MSG(CDlgParams)
	virtual BOOL OnInitDialog();
	afx_msg void OnEndlabeleditListview1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
