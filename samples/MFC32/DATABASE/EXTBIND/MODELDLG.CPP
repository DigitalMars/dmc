// modeldlg.cpp : implementation file
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
#include "extbind.h"
#include "nwinddlg.h"
#include "modeldlg.h"
  
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pNWind = NULL;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADDPUB, OnAddpub)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog(	);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	CString strAboutMenu;
	strAboutMenu.LoadString(IDS_ABOUTBOX);
	if (!strAboutMenu.IsEmpty())
	{
#ifdef _MAC
		// On the Macintosh, the "About..." menu item is already there.  We
		//  just need to rename it and attach it to the about command.
		pSysMenu->ModifyMenu(0, MF_BYPOSITION, IDM_ABOUTBOX, strAboutMenu);
#else
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
#endif
	}
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// create the DBGrid control & connect it to IDC_RDCPUBS
	RECT r;
	GetClientRect(&r);
	r.bottom-=100;
	r.top=r.bottom-170; // nearly the bottom of the dialog
	r.left+=10;r.right-=10; // some margins to look better
	m_pGrid.Create(_T("Pubs Employees table"),WS_VISIBLE | WS_CHILD,
	r,this,IDC_GRIDPUBS);
	LPUNKNOWN pCursor=GetDlgItem(IDC_RDCPUBS)->GetDSCCursor();
	ASSERT(pCursor!=NULL);
	m_pGrid.SetDataSource(pCursor);	//bind the grid
	m_pGrid.BindProperty(0x9,GetDlgItem(IDC_RDCPUBS));
	//	create the RDC for NorthWind Employee table
	// to be bound when the Northwind dialog comes up in CNwindDialog::OnInitDialog()
	GetClientRect(&r);
	r.top+=5;
	r.bottom=r.top+30;
	r.right-=10; // some margins to look better
	r.left=r.right-140; // lenght of the control
	m_pDSC.Create(_T("NorthWind"),WS_VISIBLE | WS_CHILD,
	r,this,IDC_RDCNWIND);
	m_pDSC.SetDataSourceName(_T("NorthWind"));
	m_pDSC.SetSql(_T("SELECT FirstName, LastName, BirthDate, HireDate from Employees"));
	m_pDSC.SetUserName(_T("admin"));
	m_pDSC.SetPassword(_T(""));
	m_pDSC.SetCursorDriver(2);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMainDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMainDlg::OnOK() 
{
	// if our northwind dialog isn't already up, create it and display it
	// otherwise, just set focus to it

	if (m_pNWind == NULL)
	{
		m_pNWind = new CNwindDialog(this);
		if (m_pNWind->Create() == TRUE)
			GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
	else
		m_pNWind->SetActiveWindow();
}


void CMainDlg::OnAddpub() 
{
	CString name;
	CMsmask* p_maskedit;

	p_maskedit=(CMsmask*) GetDlgItem(IDC_MASKEDBOX2); // retrieve the OCX control with first name
	ASSERT(p_maskedit!=NULL);
	if (p_maskedit==NULL)
		return;
	name=p_maskedit->GetText();
	p_maskedit=(CMsmask*) GetDlgItem(IDC_MASKEDBOX3); // retrieve the OCX control with last name
	ASSERT(p_maskedit!=NULL);
	if (p_maskedit==NULL)
		return;
	name=_T("Pubs:") + name + _T(" ") + p_maskedit->GetText();
	CListBox* pList = (CListBox*) GetDlgItem(IDC_LIST);
	ASSERT(pList != NULL);
	if (pList != NULL)
		pList->AddString(name);
}


void CMainDlg::BoxDone()
{
	// this function is called by the northwind dialog as it terminates
	// just reset our pushbutton to be enabled again.
	// I _don't_ delete the MFC CDialog object because the dialog's own
	// code will do that.
	
	m_pNWind = NULL;
	// don't delete m_pNWind; !
	GetDlgItem(IDOK)->EnableWindow();
}

BEGIN_EVENTSINK_MAP(CMainDlg, CDialog)
	ON_PROPNOTIFY_RANGE(CMainDlg, IDC_MASKEDBOX2, IDC_MASKEDBOX4, MASKDISPID_TEXT, OnRequestEdit, OnChanged)
	ON_DSCNOTIFY(CMainDlg, IDC_RDCNWIND, OnDSCNotifyNwind)
	ON_DSCNOTIFY(CMainDlg, IDC_RDCPUBS, OnDSCNotifyPubs)
END_EVENTSINK_MAP()

BOOL CMainDlg::OnRequestEdit(UINT nCtl, BOOL* pBool)
{	//this handler is never called in optimistic DataBinding architecture supplied by MFC
	return TRUE;
}

BOOL CMainDlg::OnChanged(UINT nCtl)
{
	// process the notification from nCtl control
	return TRUE;
}

BOOL CMainDlg::OnDSCNotifyNwind(DSCSTATE nState, DSCREASON nReason, BOOL* pBool)
{
	// process the notification from RDC NorthWind
	return TRUE;
}

BOOL CMainDlg::OnDSCNotifyPubs(DSCSTATE nState, DSCREASON nReason, BOOL* pBool)
{
	// process the notification from RDC Pubs
	return TRUE;
}