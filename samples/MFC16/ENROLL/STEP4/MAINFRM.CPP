// mainfrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "enroll.h"
#include "mainfrm.h"
#include "sectset.h"
#include "coursset.h"
#include "addform.h"
#include "crsform.h"
#include "enroldoc.h"
#include "sectform.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_FORM_COURSES, OnFormCourses)
	ON_UPDATE_COMMAND_UI(ID_FORM_COURSES, OnUpdateFormCourses)
	ON_COMMAND(ID_FORM_SECTIONS, OnFormSections)
	ON_UPDATE_COMMAND_UI(ID_FORM_SECTIONS, OnUpdateFormSections)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

// toolbar buttons - IDs are command buttons
static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
		ID_SEPARATOR,
	ID_FILE_PRINT,
		ID_SEPARATOR,
	ID_RECORD_FIRST,
	ID_RECORD_PREV,
	ID_RECORD_NEXT,
	ID_RECORD_LAST,
		ID_SEPARATOR,
	ID_RECORD_ADD,
	ID_RECORD_REFRESH,
	ID_RECORD_DELETE,
		ID_SEPARATOR,
	ID_APP_ABOUT,
};

static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(buttons,
		  sizeof(buttons)/sizeof(UINT)))
	{
		TRACE("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE("Failed to create status bar\n");
		return -1;      // fail to create
	}

	return 0;
}

void CMainFrame::SwitchToForm(int nForm)
{
	CView* pOldActiveView = GetActiveView();
	CView* pNewActiveView = (CView*)GetDlgItem(nForm);
	if (pNewActiveView == NULL)
	{
		if (nForm == IDW_COURSE_FORM)
			pNewActiveView = (CView*)new CCourseForm;
		else
			pNewActiveView = (CView*)new CSectionForm;


		CCreateContext context;
		context.m_pCurrentDoc = pOldActiveView->GetDocument();
		pNewActiveView->Create(NULL, NULL, 0L, CFrameWnd::rectDefault,
			this, nForm, &context);
		pNewActiveView->OnInitialUpdate();
	}

	SetActiveView(pNewActiveView);
	pNewActiveView->ShowWindow(SW_SHOW);
	pOldActiveView->ShowWindow(SW_HIDE);
	::SetWindowWord(pOldActiveView->m_hWnd, GWW_ID,
		(pOldActiveView->GetRuntimeClass() == RUNTIME_CLASS(CCourseForm))?
		IDW_COURSE_FORM : IDW_SECTION_FORM);
	::SetWindowWord(pNewActiveView->m_hWnd, GWW_ID, AFX_IDW_PANE_FIRST);
	RecalcLayout();
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnFormCourses()
{
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CCourseForm)))
		return; // already active
	SwitchToForm(IDW_COURSE_FORM);
}

void CMainFrame::OnUpdateFormCourses(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetActiveView()->IsKindOf(RUNTIME_CLASS(CCourseForm)));
}

void CMainFrame::OnFormSections()
{
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CSectionForm)))
		return; // already active
	SwitchToForm(IDW_SECTION_FORM);
}

void CMainFrame::OnUpdateFormSections(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetActiveView()->IsKindOf(RUNTIME_CLASS(CSectionForm)));
}
