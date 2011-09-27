// crsform.cpp : implementation file
//

#include "stdafx.h"
#include "enroll.h"
#include "coursset.h"
#include "addform.h"
#include "crsform.h"
#include "sectset.h"
#include "enroldoc.h"
#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCourseForm

IMPLEMENT_DYNCREATE(CCourseForm, CAddForm)

CCourseForm::CCourseForm()
	: CAddForm(CCourseForm::IDD)
{
	//{{AFX_DATA_INIT(CCourseForm)
	m_pSet = NULL;
	//}}AFX_DATA_INIT
}

CCourseForm::~CCourseForm()
{
}

void CCourseForm::DoDataExchange(CDataExchange* pDX)
{
	CAddForm::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCourseForm)
	DDX_Control(pDX, IDC_COURSEID, m_ctlCourseID);
	DDX_FieldText(pDX, IDC_COURSEID, m_pSet->m_CourseID, m_pSet);
	DDX_FieldText(pDX, IDC_HOURS, m_pSet->m_Hours, m_pSet);
	DDX_FieldText(pDX, IDC_TITLE, m_pSet->m_CourseTitle, m_pSet);
	//}}AFX_DATA_MAP
}

BOOL CCourseForm::OnMove(UINT nIDMoveCommand)
{
	BOOL bWasAddMode = FALSE;
	CString strCourseID;
	if (m_bAddMode == TRUE)
	{
		m_ctlCourseID.GetWindowText(strCourseID);
		bWasAddMode = TRUE;
	}
	if (CAddForm::OnMove(nIDMoveCommand))
	{
		m_ctlCourseID.SetReadOnly(TRUE);
		if (bWasAddMode == TRUE)
		{
			CUpdateHint hint;
			hint.m_strCourse = strCourseID;
			GetDocument()->UpdateAllViews(this, HINT_ADD_COURSE, &hint);
		}
		return TRUE;
	}
	return FALSE;
}


BEGIN_MESSAGE_MAP(CCourseForm, CAddForm)
	//{{AFX_MSG_MAP(CCourseForm)
	ON_COMMAND(ID_RECORD_ADD, OnRecordAdd)
	ON_COMMAND(ID_RECORD_DELETE, OnRecordDelete)
	ON_COMMAND(ID_RECORD_REFRESH, OnRecordRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCourseForm message handlers

CRecordset* CCourseForm::OnGetRecordset()
{
	return m_pSet;
}

void CCourseForm::OnInitialUpdate()
{
	CEnrollDoc* pDoc = (CEnrollDoc*)GetDocument();
	CDatabase* pDatabase = pDoc->GetDatabase();
	if (!pDatabase->IsOpen())
		return;
	m_pSet = &pDoc->m_courseSetForForm;
	m_pSet->m_strSort = "CourseID";
	m_pSet->m_pDatabase = pDatabase;
	CAddForm::OnInitialUpdate();
}


void CCourseForm::OnRecordAdd()
{
	CAddForm::RecordAdd();
	m_ctlCourseID.SetReadOnly(FALSE);
}


void CCourseForm::OnRecordDelete()
{
	CUpdateHint hint;
	hint.m_strCourse = m_pSet->m_CourseID;
	if (CAddForm::RecordDelete())
		GetDocument()->UpdateAllViews(this, HINT_DELETE_COURSE, &hint);
}

void CCourseForm::OnRecordRefresh()
{
	if (m_bAddMode == TRUE)
		m_ctlCourseID.SetReadOnly(TRUE);
	CAddForm::RecordRefresh();
}
