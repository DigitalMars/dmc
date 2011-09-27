// ColumnPage.h : header file
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
// CColumnPage dialog

class CColumnPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CColumnPage)

// Construction
public:
	CColumnPage();
	~CColumnPage();

// Dialog Data
	//{{AFX_DATA(CColumnPage)
	enum { IDD = IDD_COLUMNPAGE };
	BOOL	m_bLength;
	BOOL	m_bNullability;
	BOOL	m_bPrecision;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CColumnPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CColumnPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
