// FTPTREEDlg.cpp : implementation file
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
#include "FTPTREE.h"
#include "FTPtrDlg.h"

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
// CFTPTREEDlg dialog

CFTPTREEDlg::CFTPTREEDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFTPTREEDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFTPTREEDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CFTPTREEDlg::~CFTPTREEDlg()
{
	// clean up any objets that are still lying around
	if (m_pFtpConnection != NULL)
	{
		m_pFtpConnection->Close();
		delete m_pFtpConnection;
	}
	if (m_pInetSession != NULL)
	{
		m_pInetSession->Close();
		delete m_pInetSession;
	}
}

void CFTPTREEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFTPTREEDlg)
	DDX_Control(pDX, IDC_FTPTREE, m_FtpTreeCtl);
	DDX_Control(pDX, IDC_FTPSITE, m_comboFtpSite);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFTPTREEDlg, CDialog)
	//{{AFX_MSG_MAP(CFTPTREEDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BROWSE_SITE, OnBrowseSite)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFTPTREEDlg message handlers

BOOL CFTPTREEDlg::OnInitDialog()
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
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CRect rect;
	GetWindowRect(rect);
	m_OrigSize.cx = rect.Width();
	m_OrigSize.cy = rect.Height();

	// FTPTREE-specific initialization follows...
	m_pFtpConnection = NULL;

	// the CInternetSession will not be closed or deleted until the dialog is closed
	CString str;
	if (!str.LoadString(IDS_APPNAME)) str = _T("AppUnknown");
	m_pInetSession = new CInternetSession(str, PRE_CONFIG_INTERNET_ACCESS);

	// Alert the user if the internet session could not be started and close app
	if (!m_pInetSession)
	{
		AfxMessageBox(IDS_BAD_SESSION, MB_OK);
		OnCancel();
	}

	CImageList* pImageList = new CImageList();
	pImageList->Create(16, 15, TRUE, 3, 2);

	CBitmap bitmap;
	for (UINT iCnt=IDB_BMERROR; iCnt<=IDB_BMFILE; iCnt++)
	{
		bitmap.LoadBitmap(iCnt);
		pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
		bitmap.DeleteObject();
	}

	m_FtpTreeCtl.SetImageList(pImageList, TVSIL_NORMAL);

	// Initialize Tree Control here (a "No Connection" error will be displayed)
	m_FtpTreeCtl.PopulateTree(NULL, (unsigned char const *)NULL);

	// Initialize combo box box
	m_comboFtpSite.GetLBText(0, str);
	m_comboFtpSite.SetWindowText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CFTPTREEDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.

void CFTPTREEDlg::OnPaint()
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
// the minimized window.
HCURSOR CFTPTREEDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFTPTREEDlg::OnBrowseSite()
{

	// OnBrowseSite() provides some basic error-checking before passing a pointer
	// to a FtpConnection (and possibly a path to expand) to the tree control
	CString strFtpSite;
	CString strServerName;
	CString strObject;
	INTERNET_PORT nPort;
	DWORD dwServiceType;

	if (m_pFtpConnection != NULL)
	{
		delete m_pFtpConnection;	// the connection will be automatically closed
		m_pFtpConnection = NULL;
	}

	m_comboFtpSite.GetWindowText(strFtpSite);

	// update the combo box so the most recently used URL is at the top
	int nListCount = m_comboFtpSite.GetCount();
	CString strTemp;
	while (nListCount > 0)
	{
		m_comboFtpSite.GetLBText(--nListCount, strTemp);
		if (strFtpSite.Compare(strTemp) == 0)
		{
			m_comboFtpSite.DeleteString(nListCount);
			nListCount = 0;
		}
	}

	m_comboFtpSite.InsertString(0, strFtpSite);

	// the combo box might have 6 entries now; if so, delete the last one
	if (m_comboFtpSite.GetCount() > 5)
			m_comboFtpSite.DeleteString(5);


	// check to see if this is a reasonable URL --
	// ie "ftp://servername/dirs" or just "servername/dirs"

	if (!AfxParseURL(strFtpSite, dwServiceType, strServerName, strObject, nPort))
	{
		// try adding the "ftp://" protocol
		CString strFtpURL = _T("ftp://");
		strFtpURL += strFtpSite;

		if (!AfxParseURL(strFtpURL, dwServiceType, strServerName, strObject, nPort))
		{
			AfxMessageBox(IDS_INVALID_URL, MB_OK);
		}
	}

	CWaitCursor cursor;	// this will automatically display a wait cursor

	// replace the MRU URL in the combo's edit box
	m_comboFtpSite.SetWindowText(strFtpSite);
	m_comboFtpSite.SetEditSel(-1,-1);

	// Now open an annonymous FTP connection to the server
	if ((dwServiceType == INTERNET_SERVICE_FTP) && !strServerName.IsEmpty())
	{
		try
		{
			m_pFtpConnection = m_pInetSession->GetFtpConnection(strServerName);
		}
		catch (CInternetException* pEx)
		{
			// catch errors from WinINet
			TCHAR szErr[1024];
			if (pEx->GetErrorMessage(szErr, 1024))
				AfxMessageBox(szErr, MB_OK);
			else
				AfxMessageBox(IDS_EXCEPTION, MB_OK);
			pEx->Delete();

			m_pFtpConnection = NULL;
		}
	}
	else
	{
		AfxMessageBox(IDS_INVALID_URL, MB_OK);
	}


	// PopulateTree() will display an error if the FTP connection could not be made,
	// otherwise, it grabs the root listing and expands any folder indicated by
	// the site name
	if (m_pFtpConnection != NULL)
	{
		m_FtpTreeCtl.PopulateTree(m_pFtpConnection, strObject);
	}
	else
	{
		m_FtpTreeCtl.PopulateTree(NULL, (unsigned char *) NULL);
	}
}



void CFTPTREEDlg::OnSize(UINT nType, int cx, int cy)
{

	CDialog::OnSize(nType, cx, cy);

	// resize the tree control to appropriately match the new size
	if ((cx > 0) && (cy > 0))
	{
		CRect rect;
		if (m_FtpTreeCtl.m_hWnd != NULL)
		{
			m_FtpTreeCtl.GetWindowRect(rect);
			ScreenToClient(rect);
			m_FtpTreeCtl.MoveWindow(rect.left, rect.top,
									cx-(2*rect.left), cy-rect.top-rect.left);
		}
	}

}

void CFTPTREEDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	CDialog::OnGetMinMaxInfo(lpMMI);

	static BOOL bSetSize;

	if (bSetSize)
	{
		lpMMI->ptMinTrackSize.x = m_OrigSize.cx;
		lpMMI->ptMinTrackSize.y = m_OrigSize.cy;
	}
	else
	{
		bSetSize = TRUE;
	}
}
