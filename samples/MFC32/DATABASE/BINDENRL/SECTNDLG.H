// SectnDlg.h : header file
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
// CSectionPage dialog

#ifndef INC_SECTNPAGE_H
#define INC_SECTNPAGE_H
#include "mcsdsc.h"

class CSectionPage : public CPropertyPage
{
//	DECLARE_DYNCREATE(CSectionPage)

// Construction
public:
	CSectionPage();
	~CSectionPage();
	CMcsDsc* m_pCourseRDC;
	BOOL OnCourseChanged();
	BOOL OnStudentsChanged();
// Dialog Data
	//{{AFX_DATA(CSectionPage)
	enum { IDD = IDD_SECTION };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSectionPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_initiated; // flag set by OnInitDialog()	
	// Generated message map functions
	//{{AFX_MSG(CSectionPage)
	afx_msg BOOL OnDSCSectionNotify(DSCSTATE nState, DSCREASON nReason, BOOL* pBool);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif