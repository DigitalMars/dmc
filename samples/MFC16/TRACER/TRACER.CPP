// tracer.cpp : MFC Windows trace control application
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// WinHelp documentation provided with the library.
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
	//}}AFX_DATA

	void Save();
	virtual void DoDataExchange(CDataExchange* pDX);
	//{{AFX_MSG(CPromptDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CPromptDlg, CDialog)
	//{{AFX_MSG_MAP(CPromptDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static char BASED_CODE szSection[] = "Diagnostics";

CPromptDlg::CPromptDlg()
	: CDialog(CPromptDlg::IDD)
{
	m_bEnabled = AfxGetApp()->GetProfileInt(szSection, "TraceEnabled", 0);
	UINT nFlags = AfxGetApp()->GetProfileInt(szSection, "TraceFlags", 0);
	//{{AFX_DATA_INIT(CPromptDlg)
	m_b0 = (nFlags & 1) != 0;
	m_b1 = (nFlags & 2) != 0;
	m_b2 = (nFlags & 4) != 0;
	m_b3 = (nFlags & 8) != 0;
	m_b4 = (nFlags & 0x10) != 0;
	m_b5 = (nFlags & 0x20) != 0;
	//}}AFX_DATA_INIT
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

	if (!AfxGetApp()->WriteProfileInt(szSection, "TraceEnabled", m_bEnabled) ||
	   !AfxGetApp()->WriteProfileInt(szSection, "TraceFlags", nFlags))
	{
		AfxMessageBox("Unable to Write to Initialization File");
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
	m_pszProfileName = "AFX.INI";       // Profile API goes to AFX.INI
	SetDialogBkColor();

	CPromptDlg  dlg;
	if (dlg.DoModal() == IDOK)
		dlg.Save();

	::PostQuitMessage(0);       // Exit application
	return FALSE;               // FALSE means CWinApp::Run is not called
}

/////////////////////////////////////////////////////////////////////////////
