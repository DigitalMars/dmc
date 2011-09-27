// InstrDlg.h : header file
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

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CInstructorPage dialog
#ifndef INC_INSTRPAGE_H
#define INC_INSTRPAGE_H
#include "mcsdsc.h"

class CInstructorPage : public CPropertyPage
{
//	DECLARE_DYNCREATE(CInstructorPage)

// Construction
public:
	CInstructorPage();
	~CInstructorPage();

// Dialog Data
	//{{AFX_DATA(CInstructorPage)
	enum { IDD = IDD_INSTRUCTOR };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CInstructorPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
//	CMsDgridCtrl m_GridCtrl;

	// Generated message map functions
	//{{AFX_MSG(CInstructorPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif
