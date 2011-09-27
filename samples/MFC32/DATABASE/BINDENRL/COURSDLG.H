// CoursDlg.h : header file
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
// CCoursePage dialog

#ifndef INC_COURPAGE_H
#define INC_COURPAGE_H
#include "mcsdsc.h"

class CCoursePage : public CPropertyPage
{
//	DECLARE_DYNCREATE(CCoursePage)

// Construction
public:
	CCoursePage();
	~CCoursePage();
	CMcsDsc* m_pCourseRDC;
	OnCourseChanged();
// Dialog Data
	//{{AFX_DATA(CCoursePage)
	enum { IDD = IDD_COURSE };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCoursePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_initiated;	

	// Generated message map functions
	//{{AFX_MSG(CCoursePage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif
