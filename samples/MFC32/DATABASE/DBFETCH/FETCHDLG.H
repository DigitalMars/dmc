// fetchDlg.h : header file
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
// CDbfetchDlg dialog

class CDbfetchDlg : public CDialog
{
// Construction
public:
	CDbfetchDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDbfetchDlg)
	enum { IDD = IDD_DBFETCH_DIALOG };
	CComboBox	m_cbTable;
	CButton	m_buttonGetDataSource;
	CButton	m_buttonQuit;
	CButton	m_buttonGetData;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbfetchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
	CDatabase m_db;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDbfetchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnGetdatasource();
	afx_msg void OnQuitSelect();
	afx_msg void OnGetData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
