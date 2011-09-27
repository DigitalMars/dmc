// StudDlg.h : header file
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
// CStudentPage dialog
#ifndef INC_STUDPAGE_H
#define INC_STUDPAGE_H
#include "mcsdsc.h"
// #include "msdgridctrl.h"

class CStudentPage : public CPropertyPage
{
//	DECLARE_DYNCREATE(CStudentPage)

// Construction
public:
	CStudentPage();
	~CStudentPage();

// Dialog Data
	//{{AFX_DATA(CStudentPage)
	enum { IDD = IDD_STUDENT };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStudentPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
// CMsDgridCtrl m_GridCtrl;

	// Generated message map functions
	//{{AFX_MSG(CStudentPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif
