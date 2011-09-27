// DataSrcDlg.h : header file
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

/////////////////////////////////////////////////////////////////////////////
// CDataSrcDlg dialog

class CDataSrcDlg : public CDialog
{
// Construction
public:
CDataSrcDlg::CDataSrcDlg(CString &DSN,CString &Sql,CString &User,CString &Pw,
						 CWnd* pParent=NULL);

// Dialog Data
	//{{AFX_DATA(CDataSrcDlg)
	enum { IDD = IDD_DATASOURCE };
	CString	m_DSN;
	CString	m_User;
	CString	m_Sql;
	CString	m_Pw;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataSrcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataSrcDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
