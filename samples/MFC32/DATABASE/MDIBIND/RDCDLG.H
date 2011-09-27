// OpenControlsDlg.h : header file
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

#include "RDCView.h"

/////////////////////////////////////////////////////////////////////////////
// COpenControlsDlg dialog

class COpenControlsDlg : public CDialog
{
// Construction
public:
	COpenControlsDlg(const CMDIBindDoc* pDocument,CWnd* pParent = NULL);   // constructor
	CRDCView* m_pRDCView;
	CMDIChildWnd* m_pMDIChild;
// Dialog Data
	//{{AFX_DATA(COpenControlsDlg)
	enum { IDD = IDD_WINDOWLIST };
	CString	m_ColName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenControlsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COpenControlsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDropdownColumns();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_CurrentSel; // current selection in Windows list
};
