// sectform.cpp : implementation of the CSectionForm class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "enroll.h"
#include "sectset.h"
#include "coursset.h"
#include "enroldoc.h"
#include "sectform.h"
#include "addfield.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// For dynamically added fields
#define IDC_EDIT_EXTRA 200
#define IDC_STATIC_EXTRA 300

/////////////////////////////////////////////////////////////////////////////
// CSectionForm

IMPLEMENT_DYNCREATE(CSectionForm, CRecordView)

BEGIN_MESSAGE_MAP(CSectionForm, CRecordView)
	//{{AFX_MSG_MAP(CSectionForm)
	ON_CBN_SELENDOK(IDC_COURSELIST, OnSelendokCourselist)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_OPTIONS_ADDFIELDS, OnOptionsAddfields)
	ON_COMMAND(ID_FILE_PRINT, CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRecordView::OnFilePrintPreview)
	ON_COMMAND(ID_RECORD_ADD, OnRecordAdd)
	ON_COMMAND(ID_RECORD_REFRESH, OnRecordRefresh)
	ON_COMMAND(ID_RECORD_DELETE, OnRecordDelete)
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
	while (!m_listEdit.IsEmpty())
	{
		delete m_listEdit.GetHead();
		m_listEdit.RemoveHead();
	}

	while (!m_listStatic.IsEmpty())
	{
		delete m_listStatic.GetHead();
		m_listStatic.RemoveHead();
	}
}

void CSectionForm::DoDataExchange(CDataExchange* pDX)
{
	CRecordView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSectionForm)
	DDX_Control(pDX, IDC_STATIC_CAPACITY, m_staticLast);
	DDX_Control(pDX, IDC_CAPACITY, m_editLast);
	DDX_Control(pDX, IDC_SECTION, m_ctlSection);
	DDX_Control(pDX, IDC_COURSELIST, m_ctlCourseList);
	DDX_FieldCBString(pDX, IDC_COURSELIST, m_pSet->m_CourseID, m_pSet);
	DDX_FieldText(pDX, IDC_CAPACITY, m_pSet->m_Capacity, m_pSet);
	DDX_FieldText(pDX, IDC_INSTRUCTOR, m_pSet->m_InstructorID, m_pSet);
	DDX_FieldText(pDX, IDC_ROOM, m_pSet->m_RoomNo, m_pSet);
	DDX_FieldText(pDX, IDC_SCHEDULE, m_pSet->m_Schedule, m_pSet);
	DDX_FieldText(pDX, IDC_SECTION, m_pSet->m_SectionNo, m_pSet);
	//}}AFX_DATA_MAP

	// Make calls for any fields added at run-time
	if (!m_pSet->m_listValue.IsEmpty())
	{
		UINT nField=0;
		POSITION posValue = m_pSet->m_listValue.GetHeadPosition();
		while (posValue)
		{
			DDX_FieldText(pDX, IDC_EDIT_EXTRA+nField,
				m_pSet->m_listValue.GetNext(posValue), m_pSet);
			nField++;
		}
	}
}


void CSectionForm::OnInitialUpdate()
{
	m_pSet = &GetDocument()->m_sectionSet;

	// Fill the combo box with all of the courses
	CEnrollDoc* pDoc = GetDocument();
	if (!pDoc->m_courseSet.Open())
		return;

	// Parameterize and sort the course recordset
	m_pSet->m_strFilter = "CourseID = ?";
	m_pSet->m_strCourseIDParam = pDoc->m_courseSet.m_CourseID;
	m_pSet->m_strSort = "SectionNo";
	m_pSet->m_pDatabase = pDoc->m_courseSet.m_pDatabase;

	CRecordView::OnInitialUpdate();

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
	return STATIC_DOWNCAST(CEnrollDoc, m_pDocument);
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

void CSectionForm::OnOptionsAddfields()
{
	CAddField addfield;

	addfield.m_pSet = m_pSet;
	if (addfield.DoModal() != IDOK || addfield.m_strField.IsEmpty())
		return;

	CEdit* pedit;
	CStatic* pstatic;
	if (m_listEdit.IsEmpty())
	{
		// Find coordinates of Schedule edit control
		m_editLast.GetWindowRect(&m_rc);
		ScreenToClient(&m_rc);
		m_nOffset = (m_rc.bottom - m_rc.top) + 10;

		// Find coords of Schedule label
		m_staticLast.GetWindowRect(&m_rcStatic);
		ScreenToClient(&m_rcStatic);

	}

	pedit = new CEdit();
	m_listEdit.AddTail((CObject*)pedit);
	m_rc.top += m_nOffset;
	m_rc.bottom += m_nOffset;
	pedit->Create(WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
		m_rc, this, IDC_EDIT_EXTRA+m_listEdit.GetCount()-1);
	pedit->SetFont(GetFont());

	pstatic = new CStatic();
	m_listStatic.AddTail((CObject*)pstatic);
	m_rcStatic.top += m_nOffset;
	m_rcStatic.bottom += m_nOffset;
	CString Label = addfield.m_strField;
	Label += ":";
	pstatic->Create(Label, WS_CHILD | WS_VISIBLE, m_rcStatic, this,
		IDC_STATIC_EXTRA+m_listStatic.GetCount()-1);
	pstatic->SetFont(GetFont());

	// Resize the form to fit newly added controls
	CSize size = GetTotalSize();
	size.cy += m_nOffset;
	SetScrollSizes(MM_TEXT, size);
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE);

	m_pSet->Close();
	m_pSet->AddTextField(addfield.m_strField);
	m_pSet->Open();

	// Fill new controls with data
	UpdateData(FALSE);
}
