// AddDbDlg.h : header file
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
// CAddDatabaseDlg dialog

class CAddDatabaseDlg : public CDialog
{
// Construction
public:
	CDaoDatabase ** m_ppDatabase;
	void initializer();
	CAddDatabaseDlg(CDaoDatabase **ppDatabase, CString strDatabaseName, CWnd *pParent = NULL);
	CAddDatabaseDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddDatabaseDlg)
	enum { IDD = IDD_ADD_DATABASE_DLG };
	CString	m_strDatabaseName;
	BOOL	m_bEncrypt;
	int		m_nVersion;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddDatabaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddDatabaseDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
