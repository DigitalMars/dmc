// DAOTDlg.h : header file
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
// CDAOTableDlg dialog

class CDAOTableDlg : public CDialog
{
// Construction
public:
	void createNewDatabase();
	void enableControlsForOpenDatabase(BOOL bEnable = TRUE);
	CDaoDatabase * m_pDatabase;
	CDAOTableDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDAOTableDlg)
	enum { IDD = IDD_DAOTABLE_DIALOG };
	CComboBox	m_QueryNameComboControl;
	CComboBox	m_TableNameComboControl;
	CString	m_strDatabaseName;
	CString	m_strTableName;
	CString	m_strQueryName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDAOTableDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDAOTableDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonConnect();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnButtonTableFields();
	afx_msg void OnButtonTableIndexes();
	afx_msg void OnButtonQueryDefinition();
	afx_msg void OnDropdownComboTableName();
	afx_msg void OnDropdownComboQueryName();
	afx_msg void OnButtonDeleteTable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
