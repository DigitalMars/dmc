// MySheet.cpp : implementation file
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
#include "OdbcInfo.h"
#include "MySheet.h"
#include "catsets.h"
#include "DrvInfo.h"

#include "aboutdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// stolen from afximpl.h...
#define _countof(array) (sizeof(array)/sizeof(array[0]))

#define WM_NEW_DSN (WM_USER + 100)
#define USC_DUMPTOFILE	0xEFFF		// user defined sys commands must be < 0xF000

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet

IMPLEMENT_DYNAMIC(CMyPropertySheet, CPropertySheet)

CMyPropertySheet::CMyPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CMyPropertySheet::CMyPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CMyPropertySheet::~CMyPropertySheet()
{
	delete m_pbtnDatabase;
	delete m_pchkCursorLib;
	delete m_pbtnAbout;
}


BEGIN_MESSAGE_MAP(CMyPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CMyPropertySheet)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_WM_INITMENUPOPUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet message handlers

BOOL CMyPropertySheet::OnInitDialog() 
{

	// let MFC do it's thing...
	BOOL bRet = CPropertySheet::OnInitDialog();

	// ...now we do ours.
	int rgiButtons[] = { IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP };

	// get the size of one button
	CRect	rect;
	GetDlgItem(rgiButtons[0])->GetWindowRect(&rect);
	CFont* pFont = GetDlgItem(rgiButtons[0])->GetFont();

	// remove existing buttons
	for (int i = 0; i < _countof(rgiButtons); i++)
	{
		HWND hWnd = ::GetDlgItem(m_hWnd, rgiButtons[i]);
		if (hWnd != NULL)
		{
			::ShowWindow(hWnd, SW_HIDE);
			::EnableWindow(hWnd, FALSE);
		}
	}

	// now make our own...

	// data source button
	m_pbtnDatabase = new CButton;
	ScreenToClient(&rect);
	int	nDiff = rect.right - rect.left;
	rect.right = nDiff + 6;
	rect.left = 6;
	m_pbtnDatabase->Create(_T("&Data Source"),WS_CHILD | WS_VISIBLE |
		BS_PUSHBUTTON | WS_TABSTOP,rect,this,1001);
	m_pbtnDatabase->SetFont(pFont);

	// cursor library check box
	rect.right += nDiff + 106;
	rect.left += nDiff + 6;
	m_pchkCursorLib = new CButton;
	m_pchkCursorLib->Create(_T("&Use Cursor Library"),WS_CHILD | WS_VISIBLE |
		BS_AUTOCHECKBOX | WS_TABSTOP,rect,this,1002);
	m_pchkCursorLib->SetFont(pFont);
	m_pchkCursorLib->SetCheck(1);

	// about button
	m_pbtnAbout = new CButton;
	CRect	clientRect;
	GetClientRect(&clientRect);
	rect.right = clientRect.right - 6;
	rect.left = clientRect.right - 6 - nDiff;
	m_pbtnAbout->Create(_T("&About"),WS_CHILD | WS_VISIBLE |
		BS_PUSHBUTTON | WS_TABSTOP,rect,this,1003);
	m_pbtnAbout->SetFont(pFont);

	// modify system menu & window styles so we can be
	// minimized and restored
	//
	ModifyStyle(0,WS_MINIMIZEBOX | WS_SYSMENU);
	GetSystemMenu(FALSE)->InsertMenu(1,MF_BYPOSITION,SC_MINIMIZE,
		_T("Mi&nimize"));
	GetSystemMenu(FALSE)->InsertMenu(0,MF_BYPOSITION,SC_RESTORE,
		_T("&Restore"));
	GetSystemMenu(FALSE)->InsertMenu(3,MF_BYPOSITION | MF_SEPARATOR);
	GetSystemMenu(FALSE)->InsertMenu(3,MF_BYPOSITION,USC_DUMPTOFILE,
		_T("&Dump to File"));
	GetSystemMenu(FALSE)->InsertMenu(3,MF_BYPOSITION | MF_SEPARATOR);

	return bRet;
}

BOOL CMyPropertySheet::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == 1001)
	{
		if (((COdbcInfoApp*)AfxGetApp())->m_Database.IsOpen())
			((COdbcInfoApp*)AfxGetApp())->m_Database.Close();
		if (m_bCursorLib = m_pchkCursorLib->GetCheck())
			((COdbcInfoApp*)AfxGetApp())->m_Database.Open(NULL);
		else
			((COdbcInfoApp*)AfxGetApp())->m_Database.Open(
				NULL,FALSE,FALSE,"ODBC;",FALSE);
		if (((COdbcInfoApp*)AfxGetApp())->m_Database.IsOpen())
		{
			int nPages = GetPageCount();
			for (int i = 0; i < nPages; i++)
				((CMyPage*)GetPage(i))->OnNewDSN();
		}
		return 0;
	}
	else if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == 1003)
	{
		CAboutDlg	dlg;
		dlg.DoModal();
		return 0;
	}

	return CPropertySheet::OnCommand(wParam, lParam);
}

// OnPaint, OnEraseBkgnd, and OnQueryDragIcon supplied so that we
// appear properly when minimized on Windows NT (Not needed for Win95).

void CMyPropertySheet::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (IsIconic())
	{
		DefWindowProc(WM_ICONERASEBKGND,(WORD)dc.m_hDC,0L);
		CRect	rc;
		GetClientRect(&rc);
		rc.left = (rc.right - ::GetSystemMetrics(SM_CXICON)) >> 1;
		rc.top = (rc.bottom - ::GetSystemMetrics(SM_CYICON)) >> 1;
		HICON	hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		dc.DrawIcon(rc.left,rc.top,hIcon);
	}	
	// Do not call CPropertySheet::OnPaint() for painting messages
}

BOOL CMyPropertySheet::OnEraseBkgnd(CDC* pDC) 
{
	if (IsIconic())
		return TRUE;
	return CPropertySheet::OnEraseBkgnd(pDC);
}

HCURSOR CMyPropertySheet::OnQueryDragIcon() 
{
	return AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyPropertySheet::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// menu item will not be enabled if database is not opened
	//
	if (nID == USC_DUMPTOFILE)
	{
		// note: no fancy error handling here
		//
		CFileDialog	dlg(TRUE);
		if (dlg.DoModal() == IDOK)
		{
			CStdioFile	file;
			if (file.Open(dlg.GetPathName(),
				CFile::typeText | CFile::modeCreate | CFile::modeWrite))
			{
				if (m_bCursorLib)
					file.WriteString(_T("Cursor Library is Loaded\n\n"));
				else
					file.WriteString(_T("Cursor Library is Not Loaded\n\n"));
				int nPages = GetPageCount();
				for (int i = 0; i < nPages; i++)
					((CMyPage*)GetPage(i))->DumpToFile(file);
				file.Close();
			}
			else
				AfxMessageBox(_T("Could not open output file"));
		}
		return;
	}
	
	CPropertySheet::OnSysCommand(nID, lParam);
}

void CMyPropertySheet::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CPropertySheet::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	
	if (((COdbcInfoApp*)AfxGetApp())->m_Database.IsOpen())
		GetSystemMenu(FALSE)->EnableMenuItem(4,MF_BYPOSITION | MF_ENABLED);
	else
		GetSystemMenu(FALSE)->EnableMenuItem(4,MF_BYPOSITION | MF_GRAYED);
	
}
