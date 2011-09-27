// CoursDlg.cpp : implementation file
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

#include "stdafx.h"
#include "CoursDlg.h"
#include "Msmask.h"
#include "lboxctrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoursePage property page

// IMPLEMENT_DYNCREATE(CCoursePage, CPropertyPage)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCoursePage::CCoursePage() : CPropertyPage(CCoursePage::IDD)
{
	m_initiated=FALSE;
	//{{AFX_DATA_INIT(CCoursePage)
	//}}AFX_DATA_INIT
}

CCoursePage::~CCoursePage()
{
}

void CCoursePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCoursePage)
	//}}AFX_DATA_MAP
}

BOOL CCoursePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	if(m_pCourseRDC!=NULL)
	{
		LPUNKNOWN pCursor=m_pCourseRDC->GetDSCCursor();
		ASSERT(pCursor!=NULL);
		CLbox1Ctrl* pWnd = (CLbox1Ctrl*) GetDlgItem(IDC_DBLIST1);
		ASSERT(pWnd!=NULL);
		pWnd->BindProperty(0x3,m_pCourseRDC);//bind the RowSource property
		pWnd->SetListField(_T("CourseID"));
		pWnd->SetBoundColumn(_T("CourseID"));
 		pWnd->SetRowSource(pCursor);
		// CString strWnd;
		// m_GridCtrl.SetCaption("Bound to "+": "+m_pRDC->GetCaption());
		GetDlgItem(IDC_COURSE)->BindDefaultProperty(0x16,VT_BSTR,
			_T("CourseID"),m_pCourseRDC);		// bind the edit
		GetDlgItem(IDC_TITLE)->BindDefaultProperty(0x16,VT_BSTR,
			_T("CourseTitle"),m_pCourseRDC);	// bind the edit
		GetDlgItem(IDC_HOURS)->BindDefaultProperty(0x16,VT_BSTR,
			_T("Hours"),m_pCourseRDC);			// bind the edit
 
		m_initiated=TRUE;
		OnCourseChanged(); // adjust the rowset for students
	}

	SetModified(TRUE);  // allow the APPLY button to become active

	return TRUE;
}


BEGIN_MESSAGE_MAP(CCoursePage, CPropertyPage)
	//{{AFX_MSG_MAP(CCoursePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoursePage message handlers

BOOL CCoursePage::OnCourseChanged()
{
	if(m_initiated==FALSE)
		return TRUE;
	TRACE(_T("-----------Course changed for students.\n"));
	CMcsDsc* pStudentRDC = (CMcsDsc*) GetDlgItem(IDC_RDCSTUDENT);
	CMsmask* pCourseID = (CMsmask*) GetDlgItem(IDC_COURSE);
	ASSERT(pStudentRDC!=NULL && pCourseID!=NULL);
	if(pStudentRDC!=NULL && pCourseID!=NULL)
	{	
		// change the DataSource and refresh everything
		CString strCourseID=pCourseID->GetText();
		CString strSQL=pStudentRDC->GetSql(); // not yet like this

		strCourseID=_T("SELECT Student.StudentID, Name, GradYear, Grade, SectionNo from Student,Enrollment WHERE Student.StudentID=Enrollment.StudentID and Enrollment.CourseID='")+strCourseID+_T("'");
		pStudentRDC->SetSql(strCourseID);
		pStudentRDC->Refresh();
		return TRUE;
	}
	return FALSE;
}
