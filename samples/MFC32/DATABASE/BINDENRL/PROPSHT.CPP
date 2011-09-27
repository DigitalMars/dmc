// PropSheet.cpp : implementation file
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
#include "propsht.h"
#include "EnrolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnrollSheet

// IMPLEMENT_DYNAMIC(CEnrollSheet, CPropertySheet)

CEnrollSheet::CEnrollSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	//{{AFX_DATA_INIT(CEnrollSheet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AddControlPages();
}

CEnrollSheet::CEnrollSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	//{{AFX_DATA_INIT(CEnrollSheet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AddControlPages();
}

CEnrollSheet::~CEnrollSheet()
{
}

void CEnrollSheet::AddControlPages()
{
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;

	m_coursepage.m_pCourseRDC=&m_CourseRDC;	  // the cursor is available inside Course dlg 
	m_sectionpage.m_pCourseRDC=&m_CourseRDC;  // the cursor is available inside Section dlg

    AddPage(&m_coursepage);
    AddPage(&m_sectionpage);
    AddPage(&m_instructpage);
    AddPage(&m_studentpage);
}

void CEnrollSheet::DoDataExchange(CDataExchange* pDX)
{
	CPropertySheet::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnrollSheet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEnrollSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CEnrollSheet)
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnrollSheet message handlers

BOOL CEnrollSheet::OnInitDialog()
{
   	RECT r={ 0,0,150,30 };	 // must create the control common to all prop pages
	m_CourseRDC.Create(NULL,_T("Courses"),
					   WS_VISIBLE | WS_CHILD, r, this, IDC_RDCCOURSE);
	m_CourseRDC.SetDataSourceName(_T("Student Registration"));
	m_CourseRDC.SetSql(_T("SELECT * from Course"));
	m_CourseRDC.SetUserName(_T("admin"));
	m_CourseRDC.SetPassword(_T(""));
	m_CourseRDC.SetCursorDriver(2); // server side cursor


	CPropertySheet::OnInitDialog(); // init the sheet before getting ClientRect

	GetClientRect(&r);
	r.bottom-=4;r.top=r.bottom-30;  // place 30x150 control in
	r.left+=2;r.right=r.left+150;   // left-bottom sheet corner
	m_CourseRDC.MoveWindow(&r,FALSE);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	CString strAboutMenu;
	strAboutMenu.LoadString(IDS_ABOUTBOX);
	if (!strAboutMenu.IsEmpty())
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	}

	// Set the icon for this dialog.  The framework does this automatically
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;
}

void CEnrollSheet::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CPropertySheet::OnSysCommand(nID, lParam);
	}
}

// The system calls this to obtain the cursor to display while the user drags
// the minimized window.
HCURSOR CEnrollSheet::OnQueryDragIcon() 
{
	return (HCURSOR) m_hIcon;
}

/////////////////////////////////////////////////////////////////////////////
// CEnrollSheet event handlers
BEGIN_EVENTSINK_MAP(CEnrollSheet, CPropertySheet)
	ON_DSCNOTIFY(CEnrollSheet, IDC_RDCCOURSE, OnDSCNotify)
END_EVENTSINK_MAP()

//////////////////////////////////////////////////////
// This function is called each time the event was fired in IDC_RDCCOURSE control
// We use it to adjust COURSE & STUDENT query parameters in 
// course & section pages
//
BOOL CEnrollSheet::OnDSCNotify(DSCSTATE nState, DSCREASON nReason, BOOL* pBool)
{	// Course RDC's state changed 
	if(nReason==dscMove && nState==dscDidEvent) // row moved in course cursor
	{	// notify datasources in all pages about the move
		m_coursepage.OnCourseChanged();
		m_sectionpage.OnCourseChanged();
	}
	return TRUE;  // event handled

}

