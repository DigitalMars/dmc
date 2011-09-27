// addfield.h : header file
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
// CAddField dialog

class CAddField : public CDialog
{
// Construction
public:
	CAddField(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddField)
	enum { IDD = IDD_ADDFIELD };
	CListBox    m_lbFields;
	CString m_strField;
	//}}AFX_DATA

	CSectionSet* m_pSet;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CAddField)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
