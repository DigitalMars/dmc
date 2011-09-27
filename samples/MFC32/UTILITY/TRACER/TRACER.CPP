// tracer.cpp : MFC Windows trace control application
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
#include "resource.h"
#include <afxwin.h>

/////////////////////////////////////////////////////////////////////////////
// Dialog class

class CPromptDlg : public CDialog
{
public:
	CPromptDlg();
	//{{AFX_DATA(CPromptDlg)
	enum { IDD = IDD_PROMPT };
	BOOL    m_bEnabled;
	BOOL    m_b0;
	BOOL    m_b1;
	BOOL    m_b2;
	BOOL    m_b3;
	BOOL    m_b4;
	BOOL    m_b5;
	BOOL    m_b6;
	//}}AFX_DATA

	void Save();
	virtual void DoDataExchange(CDataExchange* pDX);

#ifdef _MAC
	virtual BOOL OnInitDialog(void);
#endif

	//{{AFX_MSG(CPromptDlg)
	afx_msg void OnPaint();
	//}}AFX_MSG
#ifdef _MAC
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
#endif
	DECLARE_MESSAGE_MAP()

protected:
	HICON	m_hIcon;
};

BEGIN_MESSAGE_MAP(CPromptDlg, CDialog)
	//{{AFX_MSG_MAP(CPromptDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
#ifdef _MAC
	ON_WM_SYSCOMMAND()
#endif
END_MESSAGE_MAP()

static TCHAR BASED_CODE szSection[] = _T("Diagnostics");

CPromptDlg::CPromptDlg()
	: CDialog(CPromptDlg::IDD)
{
	m_bEnabled = AfxGetApp()->GetProfileInt(szSection, _T("TraceEnabled"), 1);
	UINT nFlags = AfxGetApp()->GetProfileInt(szSection, _T("TraceFlags"), 0);
	//{{AFX_DATA_INIT(CPromptDlg)
	m_b0 = (nFlags & 1) != 0;
	m_b1 = (nFlags & 2) != 0;
	m_b2 = (nFlags & 4) != 0;
	m_b3 = (nFlags & 8) != 0;
	m_b4 = (nFlags & 0x10) != 0;
	m_b5 = (nFlags & 0x20) != 0;
	//}}AFX_DATA_INIT

	m_hIcon = AfxGetApp()->LoadIcon(AFX_IDI_STD_FRAME);
}

void CPromptDlg::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CPromptDlg)
	DDX_Check(pDX, IDC_ENABLEALL, m_bEnabled);
	DDX_Check(pDX, IDC_BIT0, m_b0);
	DDX_Check(pDX, IDC_BIT1, m_b1);
	DDX_Check(pDX, IDC_BIT2, m_b2);
	DDX_Check(pDX, IDC_BIT3, m_b3);
	DDX_Check(pDX, IDC_BIT4, m_b4);
	DDX_Check(pDX, IDC_BIT5, m_b5);
	DDX_Check(pDX, IDC_BIT6, m_b6);
	//}}AFX_DATA_MAP
}

void CPromptDlg::Save()
{
	UINT nFlags = 0;
	if (m_b0)
		nFlags |= 1;
	if (m_b1)
		nFlags |= 2;
	if (m_b2)
		nFlags |= 4;
	if (m_b3)
		nFlags |= 8;
	if (m_b4)
		nFlags |= 0x10;
	if (m_b5)
		nFlags |= 0x20;
	if (m_b6)
		nFlags |= 0x40;

	if (!AfxGetApp()->WriteProfileInt(szSection, _T("TraceEnabled"), m_bEnabled) ||
	   !AfxGetApp()->WriteProfileInt(szSection, _T("TraceFlags"), nFlags))
	{
		CString strMsg;
		strMsg.LoadString(IDS_WRITE_INI_FILE_FAILED);
		AfxMessageBox(strMsg);
	}
}


/////////////////////////////////////////////////////////////////////////////
// Application class

class CTracerApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};

CTracerApp NEAR theTracerApp;

BOOL CTracerApp::InitInstance()
{
	Enable3dControls();
#ifndef _MAC
	m_pszProfileName = _T("AFX.INI");       // Profile API goes to AFX.INI
#else
	m_pszProfileName = _T("AFX Preferences"); // Profile API goes to AFX Preferences
#endif

	CPromptDlg  dlg;
	if (dlg.DoModal() == IDOK)
		dlg.Save();

	::PostQuitMessage(0);       // Exit application
	return FALSE;               // FALSE means CWinApp::Run is not called
}

/////////////////////////////////////////////////////////////////////////////

#ifdef _MAC

void CPromptDlg::OnSysCommand(UINT nID, LPARAM lParam)
{

	if (LOWORD(nID) == ID_APP_ABOUT)
	{
		CDialog dlg("IDD_ABOUTBOX");
		dlg.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


BOOL CPromptDlg::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	CMenu* pMenu = GetSystemMenu(FALSE);
	pMenu->ModifyMenu(0, MF_BYPOSITION, ID_APP_ABOUT, "About Tracer");

	return TRUE;
}
#endif

void CPromptDlg::OnPaint() 
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
