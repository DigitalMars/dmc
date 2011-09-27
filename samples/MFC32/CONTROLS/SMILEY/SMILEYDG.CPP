// SmileyDg.cpp : implementation file
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
#include "Smiley.h"
#include "SmileyDg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
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
// CSmileyDlg dialog

CSmileyDlg::CSmileyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSmileyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSmileyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSmileyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmileyDlg)
	DDX_Control(pDX, IDC_SAD, m_ctlSad);
	DDX_Control(pDX, IDC_SMILECTRL1, m_ctlSmile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSmileyDlg, CDialog)
	//{{AFX_MSG_MAP(CSmileyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDBLCLK()
	ON_CONTROL(BN_CLICKED, IDC_SAD, OnSad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSmileyDlg Event Map

BEGIN_EVENTSINK_MAP(CSmileyDlg, CDialog)
	ON_EVENT(CSmileyDlg, IDC_SMILECTRL1, DISPID_CLICKIN, OnClickIn, VTS_XPOS_PIXELS VTS_YPOS_PIXELS)
	ON_EVENT(CSmileyDlg, IDC_SMILECTRL1, DISPID_CLICKOUT, OnClickOut, VTS_NONE)
END_EVENTSINK_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSmileyDlg message handlers

BOOL CSmileyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	

	// Set the 'Caption' Property in the CSmileCtl OLE Control.  The
	// SetWindowText API will take care of this for us.

	m_ctlSmile.SetWindowText(_T("Smile!"));
	
	
	// Set the Value of the Sad Check Button based upon the value of
	// the 'Sad' property in the CSmileCtl OLE Control.

	m_ctlSad.SetCheck(m_ctlSmile.GetSad());


	// Use the Control's BorderStyle stock property to put a border around
	// the control using the CWnd::ModifyStyle API.

	m_ctlSmile.ModifyStyle(0, WS_BORDER);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSmileyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSmileyDlg::OnPaint() 
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
HCURSOR CSmileyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CSmileyDlg::OnLButtonDblClk(UINT, CPoint)
{
	// Here the Container handles the WM_LBUTTONDBLCLK message where it can
	// do its own processing.  In this case, it sets the BackColor and
	// ForeColor properties in the control.  This handler is only called 
	// when double clicks occur OUTSIDE the control.  To handle double 
	// clicks inside the control, have the control fire the stock DblClk
	// event and then process it in the CSmileyDlg EVENTSINK map.

	OLE_COLOR backClr = m_ctlSmile.GetBackColor();
	OLE_COLOR foreClr = m_ctlSmile.GetForeColor();

	// Reverse the Colors!
	m_ctlSmile.SetBackColor(foreClr);
	m_ctlSmile.SetForeColor(backClr);
}

/////////////////////////////////////////////////////////////////////////////
// CSmileyDlg Command Handlers

void CSmileyDlg::OnSad()
{
	m_ctlSmile.SetSad((m_ctlSad.GetCheck() != 0));
}


/////////////////////////////////////////////////////////////////////////////
// CSmileyDlg Event Handlers

// These handlers are called when the user clicks with the mouse inside and
// outside the 'face' on the CSmileyCtl OLE Control.

BOOL CSmileyDlg::OnClickIn(OLE_XPOS_PIXELS, OLE_YPOS_PIXELS)
{
	// We don't make use of the X and Y coordinates of the mouse click
	// passed to us by the event so we don't declare parameter variables
	// for them.

	// Call the 'Wink' Method in the CSmileCtl OLE Control
	m_ctlSmile.Wink(TRUE);

	return TRUE;	// We handled the event.  No need for futher processing
}

BOOL CSmileyDlg::OnClickOut()
{
	// Call the 'Wink' Method in the CSmileCtl OLE Control
	m_ctlSmile.Wink(FALSE);

	return TRUE;	// We handled the event.  No need for futher processing
}
