// PropSheet.h : header file
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

/////////////////////////////////////////////////////////////////////////////
// CEnrollSheet

#include "CoursDlg.h"
#include "SectnDlg.h"
#include "InstrDlg.h"
#include "StudDlg.h"

class CEnrollSheet : public CPropertySheet
{
//	DECLARE_DYNAMIC(CEnrollSheet)

// Construction
public:
	CEnrollSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CEnrollSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

protected:
	void AddControlPages(void);

// Attributes
public:
	CCoursePage			m_coursepage;	// Course PropPage
	CSectionPage		m_sectionpage;	// Section PropPage
	CInstructorPage		m_instructpage;	// Instructor PropPage
	CStudentPage		m_studentpage;	// Student PropPage
	CMcsDsc				m_CourseRDC;	// supplies the Cursor for both pages
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnrollSheet)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEnrollSheet();

	// Generated message map functions
protected:

	HICON m_hIcon;

	//{{AFX_MSG(CEnrollSheet)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnDSCNotify(DSCSTATE nState, DSCREASON nReason, BOOL* pBool);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
