// TablePage.h : header file
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
// CTablePage dialog

class CTablePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTablePage)

// Construction
public:
	CTablePage();
	~CTablePage();

// Dialog Data
	//{{AFX_DATA(CTablePage)
	enum { IDD = IDD_TABLEPAGE };
	BOOL	m_bSynonyms;
	BOOL	m_bSystemTables;
	BOOL	m_bViews;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTablePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTablePage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
