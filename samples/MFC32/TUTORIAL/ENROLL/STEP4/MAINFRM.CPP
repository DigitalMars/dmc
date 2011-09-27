// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Enroll.h"
#include "MainFrm.h"
#include "sectset.h"
#include "coursset.h"
#include "addform.h"
#include "crsform.h"
#include "enroldoc.h"
#include "sectform.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CREATE()
	ON_COMMAND(ID_FORM_COURSES, OnFormCourses)
	ON_UPDATE_COMMAND_UI(ID_FORM_COURSES, OnUpdateFormCourses)
	ON_COMMAND(ID_FORM_SECTIONS, OnFormSections)
	ON_UPDATE_COMMAND_UI(ID_FORM_SECTIONS, OnUpdateFormSections)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
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
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFrameWnd::PreCreateWindow(cs);
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
	pOldActiveView->SetDlgCtrlID(
		pOldActiveView->GetRuntimeClass() == RUNTIME_CLASS(CCourseForm) ?
		IDW_COURSE_FORM : IDW_SECTION_FORM);
	pNewActiveView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
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
