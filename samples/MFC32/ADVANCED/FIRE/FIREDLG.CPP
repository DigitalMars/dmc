// firedlg.cpp : implementation file
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
#include "fire.h"
#include "firedlg.h"

#ifdef _DEBUG
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
	virtual BOOL OnInitDialog();
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
// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
// CFireDlg dialog

CFireDlg::CFireDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFireDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFireDlg)
	m_nSetting = 0;
	//}}AFX_DATA_INIT
	m_nSettingMin = 0;
	m_nSettingMax = 0;
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFireDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFireDlg)
	DDX_Control(pDX, IDC_SETTINGCTRL, m_SettingCtrl);
	DDX_Control(pDX, IDC_SETTING, m_Setting);
	DDX_Control(pDX, IDC_APPLY, m_Apply);
	DDX_Control(pDX, IDC_PROPERTY, m_Property);
	DDX_Control(pDX, IDC_HEIGHT, m_Height);
	DDX_Control(pDX, IDC_COLOR, m_Color);
	DDX_Control(pDX, IDC_BURN, m_Burn);
	DDX_Control(pDX, IDC_FIRE, m_FireWnd);
	DDX_Text(pDX, IDC_SETTING, m_nSetting);
	DDV_MinMaxInt(pDX, m_nSetting, m_nSettingMin, m_nSettingMax);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFireDlg, CDialog)
	//{{AFX_MSG_MAP(CFireDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_NOTIFY(TVN_SELCHANGED, IDC_PROPERTY, OnPropertySelchanged)
	ON_NOTIFY(TCN_SELCHANGE, IDC_COLOR, OnColorSelchange)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_SETTING, OnSettingChange)
	ON_WM_ACTIVATE()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFireDlg message handlers

BOOL CFireDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();

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
	
	m_FireWnd.InitFire(CFireWnd::red);

	TC_ITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_TEXT;
    TabCtrlItem.pszText = "Wood";
	m_Color.InsertItem( 0, &TabCtrlItem );
    TabCtrlItem.pszText = "Natural Gas";
	m_Color.InsertItem( 1, &TabCtrlItem );
    TabCtrlItem.pszText = "Kryptonite";
	m_Color.InsertItem( 2, &TabCtrlItem );

	m_Height.SetRange(1, 100);
	m_Height.SetTicFreq(10);
	m_Height.SetPos(25);

	m_Burn.SetRange( 0, 100 );
	m_Burn.SetPos( 25 );

	m_SettingCtrl.SetRange( 0, 100 );
	m_SettingCtrl.SetBase( 10 );
	m_SettingCtrl.SetPos( 0 );

	TV_INSERTSTRUCT TreeCtrlItem;

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM;
	TreeCtrlItem.item.pszText = "Fire";
	TreeCtrlItem.item.lParam = 0;
	HTREEITEM hTreeItem1 = m_Property.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "Decay";
	TreeCtrlItem.item.lParam = 1;
	m_Property.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.item.pszText = "Flammability";
	TreeCtrlItem.item.lParam = 2;
	m_Property.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.item.pszText = "Maximum Heat";
	TreeCtrlItem.item.lParam = 3;
	m_Property.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.item.pszText = "Spread Rate";
	TreeCtrlItem.item.lParam = 4;
	m_Property.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.item.pszText = "Size";
	TreeCtrlItem.item.lParam = 5;
	m_Property.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.item.pszText = "Render";
	TreeCtrlItem.item.lParam = 0;
	HTREEITEM hTreeItem2 = m_Property.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem2;
	TreeCtrlItem.item.pszText = "Smoothness";
	TreeCtrlItem.item.lParam = 6;
	m_Property.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.item.pszText = "Distribution";
	TreeCtrlItem.item.lParam = 7;
	m_Property.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.item.pszText = "Chaos";
	TreeCtrlItem.item.lParam = 8;
	m_Property.InsertItem(&TreeCtrlItem);

	m_Property.Expand(hTreeItem1,TVE_EXPAND);
	m_Property.Expand(hTreeItem2,TVE_EXPAND);

	m_uTimerID = SetTimer(0x451,33,NULL);

	m_Apply.EnableWindow( FALSE );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFireDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFireDlg::OnPaint() 
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
HCURSOR CFireDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFireDlg::OnColorSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nColor = m_Color.GetCurSel();

	switch(nColor)
	{
	case 0:
		m_FireWnd.InitFire(CFireWnd::red);
		break;
	case 1:
		m_FireWnd.InitFire(CFireWnd::blue);
		break;
	case 2:
		m_FireWnd.InitFire(CFireWnd::green);
		break;
	}
			
	*pResult = 0;
}

void CFireDlg::OnSettingChange() 
{
	if (m_Apply.m_hWnd != NULL)
		m_Apply.EnableWindow();
}

void CFireDlg::OnPropertySelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	switch(pNMTreeView->itemNew.lParam)
	{
	case 0:
		m_nSetting = 0;
		m_nSettingMin = 0;
		m_nSettingMax = 0;
		break;
	case 1:
		m_nSetting = m_FireWnd.m_nDecay;
		m_nSettingMin = 1;
		m_nSettingMax = 100;
		m_SettingCtrl.SetRange(m_nSettingMin, m_nSettingMax);
		break;
	case 2:
		m_nSetting = m_FireWnd.m_nFlammability;
		m_nSettingMin = 1;
		m_nSettingMax = 399;
		m_SettingCtrl.SetRange(m_nSettingMin, m_nSettingMax);
		break;
	case 3:
		m_nSetting = m_FireWnd.m_nMaxHeat;
		m_nSettingMin = 0;
		m_nSettingMax = 223;
		m_SettingCtrl.SetRange(m_nSettingMin, m_nSettingMax);
		break;
	case 4:
		m_nSetting = m_FireWnd.m_nSpreadRate;
		m_nSettingMin = 1;
		m_nSettingMax = 100;
		m_SettingCtrl.SetRange(m_nSettingMin, m_nSettingMax);
		break;
	case 5:
		{
			CSize size = m_FireWnd.GetBitmapSize();
			m_nSetting = m_FireWnd.m_nSize;
			m_nSettingMin = 40;
			m_nSettingMax = size.cx;
			m_SettingCtrl.SetRange(m_nSettingMin, m_nSettingMax);
		}
		break;
	case 6:
		m_nSetting = m_FireWnd.m_nSmoothness;
		m_nSettingMin = 0;
		m_nSettingMax = 5;
		m_SettingCtrl.SetRange(m_nSettingMin, m_nSettingMax);
		break;
	case 7:
		m_nSetting = m_FireWnd.m_nDistribution;
		m_nSettingMin = 0;
		m_nSettingMax = 10;
		m_SettingCtrl.SetRange(m_nSettingMin, m_nSettingMax);
		break;
	case 8:
		m_nSetting = m_FireWnd.m_nChaos;
		m_nSettingMin = 1;
		m_nSettingMax = 100;
		m_SettingCtrl.SetRange(m_nSettingMin, m_nSettingMax);
		break;
	}
	
	if(pNMTreeView->itemNew.lParam == 0)
	{
		UpdateData(FALSE);
		m_Apply.EnableWindow(FALSE);
		m_Setting.EnableWindow(FALSE);
		m_SettingCtrl.EnableWindow(FALSE);
	}
	else
	{
		UpdateData(FALSE);
		m_Setting.EnableWindow();
		m_SettingCtrl.EnableWindow();
	}

	*pResult = 0;
}

void CFireDlg::OnApply() 
{
	if (!UpdateData())
		return;

	HTREEITEM hTreeItem = m_Property.GetSelectedItem();
	
	TV_ITEM TreeCtrlItem;
	TreeCtrlItem.mask = TVIF_HANDLE	| TVIF_PARAM;
	TreeCtrlItem.hItem = hTreeItem;

	m_Property.GetItem(&TreeCtrlItem);

	switch(TreeCtrlItem.lParam)
	{
	case 0:
		m_nSetting = 0;
		break;
	case 1:
		m_FireWnd.m_nDecay = m_nSetting;
		break;
	case 2:
		m_FireWnd.m_nFlammability = m_nSetting;
		break;
	case 3:
		m_FireWnd.m_nMaxHeat = m_nSetting;
		break;
	case 4:
		m_FireWnd.m_nSpreadRate = m_nSetting;
		break;
	case 5:
		m_FireWnd.m_nSize = m_nSetting;
		break;
	case 6:
		m_FireWnd.m_nSmoothness = m_nSetting;
		break;
	case 7:
		m_FireWnd.m_nDistribution = m_nSetting;
		break;
	case 8:
		m_FireWnd.m_nChaos = m_nSetting;
		break;
	}

	m_Apply.EnableWindow( FALSE );
}

void CFireDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	KillTimer(m_uTimerID);
}

void CFireDlg::OnTimer(UINT nIDEvent) 
{
	CDialog::OnTimer(nIDEvent);

	m_FireWnd.RenderFlame();
	m_FireWnd.PaintFlame();
	if ((rand() % 10) == 0)
		m_Burn.SetPos( rand() % 100 );

	// Eat spurious WM_TIMER messages
	MSG msg;
	while(::PeekMessage(&msg, m_hWnd, WM_TIMER, WM_TIMER, PM_REMOVE));
}

void CFireDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	
	m_FireWnd.SendMessage(WM_QUERYNEWPALETTE);	// redo the palette if necessary
}

void CFireDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_FireWnd.SetMaxBurn(100 - m_Height.GetPos());
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
