// SectForm.cpp : implementation of the CSectionForm class
//

#include "stdafx.h"
#include "Enroll.h"

#include "SectSet.h"
#include "CoursSet.h"
#include "EnrolDoc.h"
#include "SectForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSectionForm

IMPLEMENT_DYNCREATE(CSectionForm, CRecordView)

BEGIN_MESSAGE_MAP(CSectionForm, CRecordView)


	//{{AFX_MSG_MAP(CSectionForm)
	ON_CBN_SELENDOK(IDC_COURSELIST, OnSelendokCourselist)
	ON_COMMAND(ID_RECORD_ADD, OnRecordAdd)
	ON_COMMAND(ID_RECORD_DELETE, OnRecordDelete)
	ON_COMMAND(ID_RECORD_REFRESH, OnRecordRefresh)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRecordView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSectionForm construction/destruction

CSectionForm::CSectionForm()
	: CRecordView(CSectionForm::IDD)
{
	//{{AFX_DATA_INIT(CSectionForm)
	m_pSet = NULL;
	//}}AFX_DATA_INIT
	m_bAddMode = FALSE;

}

CSectionForm::~CSectionForm()
{
}

void CSectionForm::DoDataExchange(CDataExchange* pDX)
{
	CRecordView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSectionForm)
	DDX_Control(pDX, IDC_COURSELIST, m_ctlCourseList);
	DDX_FieldText(pDX, IDC_SECTION, m_pSet->m_SectionNo, m_pSet);
	DDX_Control(pDX, IDC_SECTION, m_ctlSection);
	DDX_FieldText(pDX, IDC_INSTRUCTOR, m_pSet->m_InstructorID, m_pSet);
	DDX_FieldText(pDX, IDC_ROOM, m_pSet->m_RoomNo, m_pSet);
	DDX_FieldText(pDX, IDC_SCHEDULE, m_pSet->m_Schedule, m_pSet);
	DDX_FieldText(pDX, IDC_CAPACITY, m_pSet->m_Capacity, m_pSet);
	DDX_FieldCBString(pDX, IDC_COURSELIST, m_pSet->m_CourseID, m_pSet);
	//}}AFX_DATA_MAP
}

BOOL CSectionForm::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRecordView::PreCreateWindow(cs);
}

void CSectionForm::OnInitialUpdate()
{
	m_pSet = &GetDocument()->m_sectionSet;

	// Fill the combo box with all of the courses

	CEnrollDoc* pDoc = GetDocument();
	pDoc->m_courseSet.m_strSort = "CourseID";
	if (!pDoc->m_courseSet.Open())
		return;

	// Filter, parameterize and sort the course recordset
	m_pSet->m_strFilter = "CourseID = ?";
	m_pSet->m_strCourseIDParam = pDoc->m_courseSet.m_CourseID;
	m_pSet->m_strSort = "SectionNo";
	m_pSet->m_pDatabase = pDoc->m_courseSet.m_pDatabase;

	CRecordView::OnInitialUpdate();

	m_ctlCourseList.ResetContent();
	if (pDoc->m_courseSet.IsOpen())
	{
		while (!pDoc->m_courseSet.IsEOF())
		{
			m_ctlCourseList.AddString(
				pDoc->m_courseSet.m_CourseID);
			pDoc->m_courseSet.MoveNext();
		}
	}
	m_ctlCourseList.SetCurSel(0);
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
	CRecordView::AssertValid();
}

void CSectionForm::Dump(CDumpContext& dc) const
{
	CRecordView::Dump(dc);
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

void CSectionForm::OnRecordDelete()
{
	TRY
	{
		m_pSet->Delete();
	}
	CATCH(CDBException, e)
	{
		AfxMessageBox(e->m_strError);
		return;
	}
	END_CATCH

	// Move to the next record after the one just deleted
		m_pSet->MoveNext();

	// If we moved off the end of file, then move back to last record
	if (m_pSet->IsEOF())
		m_pSet->MoveLast();

	// If the recordset is now empty, then clear the fields
	// left over from the deleted record
	if (m_pSet->IsBOF())
		m_pSet->SetFieldNull(NULL);
	UpdateData(FALSE);
}

void CSectionForm::OnRecordRefresh()
{
	if (m_bAddMode == TRUE)
	{
		m_pSet->Move(AFX_MOVE_REFRESH);
		m_ctlSection.SetReadOnly(TRUE);
		m_bAddMode = FALSE;
	}
	// Copy fields from recordset to form, thus
	// overwriting any changes user may have made
	// on the form
	UpdateData(FALSE);

}
BOOL CSectionForm::OnMove(UINT nIDMoveCommand)
{
	if (m_bAddMode)
	{
		if (!UpdateData())
			return FALSE;
		TRY
		{
			m_pSet->Update();
		}
		CATCH(CDBException, e)
		{
			AfxMessageBox(e->m_strError);
			return FALSE;
		}
		END_CATCH

		m_pSet->Requery();
		UpdateData(FALSE);
		m_ctlSection.SetReadOnly(TRUE);
		m_bAddMode = FALSE;
		return TRUE;
	}
	else
	{
		return CRecordView::OnMove(nIDMoveCommand);
	}
}
