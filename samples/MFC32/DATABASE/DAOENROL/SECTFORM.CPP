// sectform.cpp : implementation of the CSectionForm class
//

#include "stdafx.h"
#include "DaoEnrol.h"

#include "sectset.h"
#include "coursese.h"
#include "denrldoc.h"
#include "sectform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSectionForm

IMPLEMENT_DYNCREATE(CSectionForm, CDaoRecordView)

BEGIN_MESSAGE_MAP(CSectionForm, CDaoRecordView)
	//{{AFX_MSG_MAP(CSectionForm)
	ON_CBN_SELENDOK(IDC_COURSELIST, OnSelendokCourselist)
	ON_COMMAND(ID_RECORD_ADD, OnRecordAdd)
	ON_COMMAND(ID_RECORD_DELETE, OnRecordDelete)
	ON_COMMAND(ID_RECORD_REFRESH, OnRecordRefresh)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CDaoRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CDaoRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CDaoRecordView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSectionForm construction/destruction

CSectionForm::CSectionForm()
	: CDaoRecordView(CSectionForm::IDD)
{
	//{{AFX_DATA_INIT(CSectionForm)
	m_pSet = NULL;
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_bAddMode = FALSE;
}

CSectionForm::~CSectionForm()
{
}

void CSectionForm::DoDataExchange(CDataExchange* pDX)
{
	CDaoRecordView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSectionForm)
	DDX_Control(pDX, IDC_SECTION, m_ctlSection);
	DDX_Control(pDX, IDC_COURSELIST, m_ctlCourseList);
	DDX_FieldText(pDX, IDC_INSTRUCTOR, m_pSet->m_InstructorID, m_pSet);
	DDX_FieldText(pDX, IDC_ROOM, m_pSet->m_RoomNo, m_pSet);
	DDX_FieldText(pDX, IDC_SCHEDULE, m_pSet->m_Schedule, m_pSet);
	DDX_FieldText(pDX, IDC_SECTION, m_pSet->m_SectionNo, m_pSet);
	DDX_FieldText(pDX, IDC_CAPACITY, m_pSet->m_Capacity, m_pSet);
	DDX_FieldCBString(pDX, IDC_COURSELIST, m_pSet->m_CourseID, m_pSet);
	//}}AFX_DATA_MAP
}

BOOL CSectionForm::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
 
	return CDaoRecordView::PreCreateWindow(cs);
}

void CSectionForm::OnInitialUpdate()
{
	CDaoEnrolDoc* pDoc = GetDocument();
	m_pSet = &pDoc->m_sectionSet;
	m_pSet->m_pDatabase = pDoc->GetDatabase();
	if (!m_pSet->m_pDatabase->IsOpen())
		return;

	// Fill the combo box with all of the courses

	pDoc->m_courseSet.m_strSort = "CourseID";
	if (pDoc->m_courseSet.m_pDatabase == NULL)
		pDoc->m_courseSet.m_pDatabase = pDoc->GetDatabase();
 	try
 	{
 		pDoc->m_courseSet.Open();
 	}
 	catch(CDaoException* e)
 	{
 		AfxMessageBox(e->
			m_pErrorInfo->m_strDescription);
 		e->Delete();
 		return;
 	}
 
  	// Filter, parameterize and sort the 
 	// CSectionSet recordset
 
  	m_pSet->m_strFilter = 
		"CourseID = CourseIDParam";
  	m_pSet->m_strCourseIDParam = 
		pDoc->m_courseSet.m_CourseID;
  	m_pSet->m_strSort = "SectionNo";
  	m_pSet->m_pDatabase = 
		pDoc->m_courseSet.m_pDatabase;
 
	CDaoRecordView::OnInitialUpdate();

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
	CDaoRecordView::AssertValid();
}

void CSectionForm::Dump(CDumpContext& dc) const
{
	CDaoRecordView::Dump(dc);
}

CDaoEnrolDoc* CSectionForm::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDaoEnrolDoc)));
	return (CDaoEnrolDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSectionForm database support
CDaoRecordset* CSectionForm::OnGetRecordset()
{
	return m_pSet;
}


/////////////////////////////////////////////////////////////////////////////
// CSectionForm message handlers

void CSectionForm::OnSelendokCourselist() 
{
	// TODO: Add your control notification handler code here
	if (!m_pSet->IsOpen() )
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
	// If already in add mode, complete the previous new record
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
	try
	{
		m_pSet->Delete();
	}
 	catch(CDaoException* e)
 	{
 		AfxMessageBox(e->
			m_pErrorInfo->m_strDescription);
 		e->Delete();
	}

	// Move to the next record after the one just deleted
	m_pSet->MoveNext();

	// If we moved off the end of file, move back to last record
	if (m_pSet->IsEOF())
		m_pSet->MoveLast();
	
	// If the recordset is now empty, clear the fields left over
	// from the deleted record
	if (m_pSet->IsBOF())
		m_pSet->SetFieldNull(NULL);
	UpdateData(FALSE);
	
}

void CSectionForm::OnRecordRefresh() 
{
	if (m_bAddMode)
	{
		m_pSet->CancelUpdate();
		m_pSet->Move(0);
		m_ctlSection.SetReadOnly(TRUE);
		m_bAddMode = FALSE;
	}
	// Copy fields from recordset to form, thus
	// overwriting any changes the user may have made
	// on the form
	UpdateData(FALSE);	 
	
}

BOOL CSectionForm::OnMove(UINT nIDMoveCommand)
{
	if (m_bAddMode)
 	{
 		if (!UpdateData())
 			return FALSE;
 		try
 		{
 			m_pSet->Update();
 		}
 		catch(CDaoException* e)
 		{
 			AfxMessageBox(e->
				m_pErrorInfo->m_strDescription);
 			e->Delete();
 		}
 		m_bAddMode = FALSE;
 		m_ctlSection.SetReadOnly(TRUE);
	 	m_pSet->Requery();
	 	UpdateData();
	}
	CDaoRecordView::OnMove(nIDMoveCommand);
 	return TRUE;
//	else
//	{
//		AfxMessageBox(e->m_pErrorInfo->m_strDescription);
//	}
}
