// RootPage.h : Interface for CRootPage class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// Note that your filters probably won't have an #ifdef like this one.
// The project file for this sample allows you to build the sample as
// a statically linked regular MFC DLL (that is, with _AFXDLL defined)
// or as a DLL that doesn't use MFC classes aside from the ISAPI 
// support classes (that is, without _AFXDLL defined).

/////////////////////////////////////////////////////////////////////////////
// CRootPage dialog

class CHttpSvrDoc;

class CRootPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRootPage)
	CRootPage();

// Construction
public:
	CHttpSvrDoc* m_pDoc;
	CRootPage( CHttpSvrDoc* pDoc );
	~CRootPage();

// Dialog Data
	//{{AFX_DATA(CRootPage)
	enum { IDD = IDD_ROOTPAGE };
	CString	m_strRoot;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRootPage)
	public:
	virtual void OnOK();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRootPage)
	afx_msg void OnReset();
	afx_msg void OnChangeRootDir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

