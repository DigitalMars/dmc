// crsform.cpp : implementation file
//

#include "stdafx.h"
#include "DaoEnrol.h"

#include "coursese.h"
#include "addform.h"
#include "crsform.h"
#include "sectset.h"
#include "denrldoc.h"
#include "mainfrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCourseForm

IMPLEMENT_DYNCREATE(CCourseForm, CAddForm)

BEGIN_MESSAGE_MAP(CCourseForm, CAddForm)
	//{{AFX_MSG_MAP(CCourseForm)
	ON_COMMAND(ID_RECORD_ADD, OnRecordAdd)
	ON_COMMAND(ID_RECORD_DELETE, OnRecordDelete)
	ON_COMMAND(ID_RECORD_REFRESH, OnRecordRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

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
	CDaoRecordView::DoDataExchange(pDX);
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

/////////////////////////////////////////////////////////////////////////////
// CCourseForm message handlers

CDaoRecordset* CCourseForm::OnGetRecordset()
{
	return m_pSet;
}

void CCourseForm::OnInitialUpdate()
{
	CDaoEnrolDoc* pDoc = (CDaoEnrolDoc*)GetDocument();
	CDaoDatabase* pDatabase = pDoc->GetDatabase();
	if (!pDatabase->IsOpen())
		return;
	m_pSet = &pDoc->m_courseSetForForm;
	m_pSet->m_strSort = "CourseID";
	m_pSet->m_pDatabase = pDatabase;
	CDaoRecordView::OnInitialUpdate();
}


void CCourseForm::OnRecordAdd()
{
	CAddForm::RecordAdd();
	m_ctlCourseID.SetReadOnly(FALSE);
}


void CCourseForm::OnRecordDelete()
{
	// The STDREG.MDB Student Registration database in Access Format
	// does not require a programmatic validation to
	// assure that a course is not deleted if any sections exist.
	// That is because the STDREG.MDB database has been pre-built with
	// such an referential integrity rule.  If the user tries to
	// delete a course that has a section, a CDBException will be
	// thrown, and ENROLL will display the SQL error message 
	// informing the user that the course cannot be deleted.  
	//
	// A Student Registration database initialized by the STDREG
	// tool will not have any such built-in referential integrity
	// rules.  For such databases, the following code assumes the
	// burden of assuring that the course is not deleted if a section
	// exists.  The reason that STDREG does not add referential
	// integrity checks to the Student Registration database is that
	// some databases such as SQL Server do not offer SQL, via ODBC,
	// for creating referential integrity rules such as "FOREIGN KEY".
	//
	// The deletion of a course is not the only place ENROLL 
	// needs a programmatic referential integrity check.  Another example
	// is a check that a duplicate course or seciton is not added.
	// For simplicity, ENROLL does not make these other checks.


	CDaoEnrolDoc* pDoc = (CDaoEnrolDoc*)GetDocument();
	CsectionSet sectionSet;
	sectionSet.m_pDatabase = pDoc->GetDatabase();
	sectionSet.m_strFilter = "CourseID = CourseIDParam";
	sectionSet.m_strCourseIDParam = m_pSet->m_CourseID;
	try
	{
		sectionSet.Open();
	}
	catch (CDaoException* e)
	{
		AfxMessageBox(e->m_pErrorInfo->m_strDescription);
		e->Delete();
	}
	if (!sectionSet.IsEOF())
	{
		AfxMessageBox(IDS_CANNOT_DELETE_COURSE_WITH_SECTION);
		return;
	}

	CUpdateHint hint;
	hint.m_strCourse = m_pSet->m_CourseID;
	if (CAddForm::RecordDelete())
		GetDocument()->UpdateAllViews(this, HINT_DELETE_COURSE, &hint);
}

void CCourseForm::OnRecordRefresh()
{
	if (m_bAddMode == TRUE)
	{
		m_pSet->CancelUpdate();
		m_ctlCourseID.SetReadOnly(TRUE);
	}
	CAddForm::RecordRefresh();
}
