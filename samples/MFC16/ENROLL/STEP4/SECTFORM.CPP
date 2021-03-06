// sectform.cpp : implementation of the CSectionForm class
//

#include "stdafx.h"
#include "enroll.h"
#include "sectset.h"
#include "coursset.h"
#include "enroldoc.h"
#include "addform.h"
#include "sectform.h"
#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSectionForm

IMPLEMENT_DYNCREATE(CSectionForm, CAddForm)

BEGIN_MESSAGE_MAP(CSectionForm, CAddForm)
	//{{AFX_MSG_MAP(CSectionForm)
	ON_CBN_SELENDOK(IDC_COURSELIST, OnSelendokCourselist)
	ON_COMMAND(ID_RECORD_REFRESH, OnRecordRefresh)
	ON_COMMAND(ID_RECORD_ADD, OnRecordAdd)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRecordView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSectionForm construction/destruction

CSectionForm::CSectionForm()
	: CAddForm(CSectionForm::IDD)
{
	//{{AFX_DATA_INIT(CSectionForm)
	m_pSet = NULL;
	//}}AFX_DATA_INIT
}

CSectionForm::~CSectionForm()
{
}

void CSectionForm::DoDataExchange(CDataExchange* pDX)
{
	CAddForm::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSectionForm)
	DDX_Control(pDX, IDC_SECTION, m_ctlSection);
	DDX_Control(pDX, IDC_COURSELIST, m_ctlCourseList);
	DDX_FieldCBString(pDX, IDC_COURSELIST, m_pSet->m_CourseID, m_pSet);
	DDX_FieldText(pDX, IDC_CAPACITY, m_pSet->m_Capacity, m_pSet);
	DDX_FieldText(pDX, IDC_INSTRUCTOR, m_pSet->m_InstructorID, m_pSet);
	DDX_FieldText(pDX, IDC_ROOM, m_pSet->m_RoomNo, m_pSet);
	DDX_FieldText(pDX, IDC_SCHEDULE, m_pSet->m_Schedule, m_pSet);
	DDX_FieldText(pDX, IDC_SECTION, m_pSet->m_SectionNo, m_pSet);
	//}}AFX_DATA_MAP
}


void CSectionForm::OnInitialUpdate()
{
	CEnrollDoc* pDoc = GetDocument();
	m_pSet = &pDoc->m_sectionSet;
	CDatabase* pDatabase = pDoc->GetDatabase();
	if (!pDatabase->IsOpen())
		return;

	// Fill the combo box with all of the courses

	pDoc->m_courseSet.m_pDatabase = pDatabase;
	pDoc->m_courseSet.m_strSort = "CourseID";
	if (!pDoc->m_courseSet.Open())
		return;

	// Parameterize and sort the course recordset
	m_pSet->m_strFilter = "CourseID = ?";
	m_pSet->m_strCourseIDParam = pDoc->m_courseSet.m_CourseID;
	m_pSet->m_strSort = "SectionNo";
	m_pSet->m_pDatabase = pDatabase;

	CAddForm::OnInitialUpdate();

	m_ctlCourseList.ResetContent();
	if (pDoc->m_courseSet.IsOpen())
	{
		while (pDoc->m_courseSet.IsEOF() != TRUE)
		{
			m_ctlCourseList.AddString(
				pDoc->m_courseSet.m_CourseID);
			pDoc->m_courseSet.MoveNext();
		}
	}
	m_ctlCourseList.SetCurSel(0);
}

void CSectionForm::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	BOOL bReselectCombo = FALSE;
	int nIndex;
	CUpdateHint* pUpdateHint;
	if (lHint != 0)
	{
		pUpdateHint = (CUpdateHint*)pHint;
		ASSERT(pUpdateHint->IsKindOf(RUNTIME_CLASS(CUpdateHint)));
		switch (lHint)
		{
			case HINT_ADD_COURSE:
				m_ctlCourseList.AddString(pUpdateHint->m_strCourse);
				bReselectCombo = TRUE;
				break;
			case HINT_DELETE_COURSE:
				nIndex = m_ctlCourseList.FindStringExact(0,
					pUpdateHint->m_strCourse);
				ASSERT(nIndex != CB_ERR);
				m_ctlCourseList.DeleteString(nIndex);
				bReselectCombo = TRUE;
				break;
		}
	}
	if (bReselectCombo)
	{
		nIndex = m_ctlCourseList.FindStringExact(0,
			m_pSet->m_strCourseIDParam);
		if (nIndex == CB_ERR)
			return;
		m_ctlCourseList.SetCurSel(nIndex);
	}
	CAddForm::OnUpdate(pSender, lHint, pHint);
}

/////////////////////////////////////////////////////////////////////////////
// CSectionForm printing

BOOL CSectionForm::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSectionForm::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSectionForm::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}



/////////////////////////////////////////////////////////////////////////////
// CSectionForm diagnostics

#ifdef _DEBUG
void CSectionForm::AssertValid() const
{
	CAddForm::AssertValid();
}

void CSectionForm::Dump(CDumpContext& dc) const
{
	CAddForm::Dump(dc);
}

CEnrollDoc* CSectionForm::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEnrollDoc)));
	return (CEnrollDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSectionForm database support

CRecordset* CSectionForm::OnGetRecordset()
{
	return m_pSet;
}


BOOL CSectionForm::OnMove(UINT nIDMoveCommand)
{
	if (CAddForm::OnMove(nIDMoveCommand))
		m_ctlSection.SetReadOnly(TRUE);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CSectionForm message handlers


void CSectionForm::OnSelendokCourselist()
{
	if (!m_pSet->IsOpen())
		return;
	m_ctlCourseList.GetLBText(m_ctlCourseList.GetCurSel(),
		m_pSet->m_strCourseIDParam);
	if (!m_bAddMode)
	{
		m_pSet->Requery();
		if (m_pSet->IsEOF())
		{
			m_pSet->SetFieldNull(&(m_pSet->m_CourseID), FALSE);
			m_pSet->m_CourseID = m_pSet->m_strCourseIDParam;
		}
		UpdateData(FALSE);
	}
}

void CSectionForm::OnRecordAdd()
{
	// If already in add mode, then complete previous new record
	if (m_bAddMode)
		OnMove(ID_RECORD_FIRST);

	CString strCurrentCourse = m_pSet->m_CourseID;
	m_pSet->AddNew();
	m_pSet->SetFieldNull(&(m_pSet->m_CourseID), FALSE);
	m_pSet->m_CourseID = strCurrentCourse;
	m_bAddMode = TRUE;
	m_ctlSection.SetReadOnly(FALSE);
	UpdateData(FALSE);
}

void CSectionForm::OnRecordRefresh()
{
	if (m_bAddMode == TRUE)
		m_ctlSection.SetReadOnly(TRUE);
	CAddForm::RecordRefresh();
}
