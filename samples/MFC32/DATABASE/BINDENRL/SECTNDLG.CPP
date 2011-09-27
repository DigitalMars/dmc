// SectnDlg.cpp : implementation file
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
#include "SectnDlg.h"
#include "cboxctrl.h"
#include "msmask.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSectionPage property page

// IMPLEMENT_DYNCREATE(CSectionPage, CPropertyPage)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSectionPage::CSectionPage() : CPropertyPage(CSectionPage::IDD)
{
	m_initiated=FALSE;
	//{{AFX_DATA_INIT(CSectionPage)
	//}}AFX_DATA_INIT
}

CSectionPage::~CSectionPage()
{
}

void CSectionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSectionPage)
	//}}AFX_DATA_MAP
}

BOOL CSectionPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	CMsmask* pCourseID = (CMsmask*) GetDlgItem(IDC_COURSE);
	// bind the edit
	if(m_pCourseRDC!=NULL)
		pCourseID->BindDefaultProperty(0x16,VT_BSTR,_T("CourseID"),m_pCourseRDC); 
	if(m_pCourseRDC!=NULL)
	{
		m_initiated=TRUE;
		OnCourseChanged();	// adjust the rowset for Course & students
	}
	return TRUE;
}

BOOL CSectionPage::OnCourseChanged()
{
	if(m_initiated==FALSE)
		return TRUE;
	TRACE(_T("-----------Course changed for section.\n"));
	CMcsDsc* pSectionRDC = (CMcsDsc*) GetDlgItem(IDC_RDCSECTION);
	CMsmask* pCourseID = (CMsmask*) GetDlgItem(IDC_COURSE);
	ASSERT(pSectionRDC!=NULL && pCourseID!=NULL);
	if(pSectionRDC!=NULL && pCourseID!=NULL)
	{	// change the DataSource and refresh everything
		CString strCourseID=pCourseID->GetText();
		strCourseID=_T("SELECT * from Section WHERE CourseID='")+strCourseID+_T("'");
		pSectionRDC->SetSql(strCourseID);
		pSectionRDC->Refresh();
		return TRUE;
	}
	return FALSE;
}

BOOL CSectionPage::OnStudentsChanged()
{
	if(m_initiated==FALSE)
		return TRUE;
	TRACE(_T("----------- Section changed for Students.\n"));
	CMcsDsc* pStudentRDC = (CMcsDsc*) GetDlgItem(IDC_RDCSTUDENT);
	CMsmask* pCourseID = (CMsmask*) GetDlgItem(IDC_COURSE);
	CMsmask* pSection = (CMsmask*) GetDlgItem(IDC_SECTION);
	ASSERT(pStudentRDC!=NULL && pCourseID!=NULL && pSection!=NULL);
	if(pStudentRDC!=NULL && pCourseID!=NULL && pSection!=NULL)
	{	// change the DataSource and refresh everything
		CString strCourseID=pCourseID->GetText();
		CString strSection=pSection->GetText();
		// get the SQL defining the columns
		CString strSQL=_T("SELECT Student.StudentID, Name, GradYear, Grade from Student, Enrollment WHERE Student.StudentID=Enrollment.StudentID");
		// add the search condition to restrain the rowset to active section only
		strCourseID=strSQL+
			_T(" and Enrollment.CourseID='")+strCourseID+_T("'")+
			_T(" and Enrollment.SectionNo='")+strSection+_T("'");
		pStudentRDC->SetSql(strCourseID);
		pStudentRDC->Refresh();
		return TRUE;
	}
	return FALSE;
}

BEGIN_MESSAGE_MAP(CSectionPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSectionPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSectionPage event handlers
BEGIN_EVENTSINK_MAP(CSectionPage, CPropertyPage)
	ON_DSCNOTIFY(CSectionPage, IDC_RDCSECTION, OnDSCSectionNotify)
END_EVENTSINK_MAP()


//////////////////////////////////////////////////////
// This function is called each time the event was fired in IDC_RDCSECTION control
// We use it to adjust STUDENT query parameter in 
//
BOOL CSectionPage::OnDSCSectionNotify(DSCSTATE nState, DSCREASON nReason, BOOL* pBool)
{	// Section RDC's state changed
	if(nReason==dscMove && nState==dscDidEvent) // row moved in course cursor
	{ // notify the student list
		return OnStudentsChanged(); // pass the event 
	}
	return TRUE;    // event handled


}


