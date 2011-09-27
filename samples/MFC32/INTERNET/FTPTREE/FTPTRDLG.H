// FTPTREEDlg.h : header file
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

#include "afxinet.h"
#include "FTPtrCtl.h"

/////////////////////////////////////////////////////////////////////////////
// CFTPTREEDlg dialog

class CFTPTREEDlg : public CDialog
{
// Construction
public:
	CFTPTREEDlg(CWnd* pParent = NULL);	// standard constructor
	~CFTPTREEDlg();						// custom destructor

// Dialog Data
	//{{AFX_DATA(CFTPTREEDlg)
	enum { IDD = IDD_FTPTREE_DIALOG };
	CMyTreeCtrl	m_FtpTreeCtl;
	CComboBox	m_comboFtpSite;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFTPTREEDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CInternetSession* m_pInetSession;
	CFtpConnection* m_pFtpConnection;
	CSize m_OrigSize;


	// Generated message map functions
	//{{AFX_MSG(CFTPTREEDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBrowseSite();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
