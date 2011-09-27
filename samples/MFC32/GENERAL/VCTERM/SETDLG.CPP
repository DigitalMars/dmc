// setdlg.cpp : implementation of the CSettingsDlg class

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
#include "vcterm.h"
#include "setdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg dialog

// set up initial settings in dialog to mirror  
// the current settings in the MSCOMM control.
CSettingsDlg::CSettingsDlg(CMainFrame* pParent)
	: CDialog(CSettingsDlg::IDD, pParent)
{
	int index, baudrate = 300, parity;
	CString str = pParent->GetCommCtrl()->GetSettings();
	index = str.Find((TCHAR)',');

	// calculate radio button index for current baud rate setting
	for (int nSetting = 0; baudrate< _ttoi(str.Left(index)); nSetting++)
		baudrate = baudrate << 1;
	baudrate = nSetting;

	// calculate radio button index for current parity setting
	switch(str[index+1])
	{
		case (TCHAR)'N':
		case (TCHAR)'n': parity=0; break;
		case (TCHAR)'O': 
		case (TCHAR)'o': parity=1; break;
		case (TCHAR)'E': 
		case (TCHAR)'e': parity=2; break;
	}

	//{{AFX_DATA_INIT(CSettingsDlg)
	m_baud = baudrate;
	m_comport = pParent->GetCommCtrl()->GetCommPort()-1; 
	m_databits = str[index+3] - 55;            // convert to integer value -7
	m_echo = pParent->GetEditCtrl()->m_bEcho;
	m_flow = pParent->GetCommCtrl()->GetHandshaking();
	m_parity = parity;
	m_stopbits = str[index+5] - 49;            // convert to integer value -1
	//}}AFX_DATA_INIT
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsDlg)
	DDX_Radio(pDX, IDC_BAUD1, m_baud);
	DDX_Radio(pDX, IDC_COM1, m_comport);
	DDX_Radio(pDX, IDC_DATABITS1, m_databits);
	DDX_Radio(pDX, IDC_ECHO1, m_echo);
	DDX_Radio(pDX, IDC_FLOW1, m_flow);
	DDX_Radio(pDX, IDC_PARITY1, m_parity);
	DDX_Radio(pDX, IDC_STOPBITS1, m_stopbits);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg message handlers

void CSettingsDlg::OnOK() 
{
	// update various dialog data members
	if (!UpdateData(TRUE))
	{
		AfxMessageBox(IDS_UPDATEDATAFAILED);
		return;
	}

	// set commctrl properties to current dialog settings
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	int baudrate = 300;
	TCHAR parity;
	CString str;

	// calculate baud rate from corresponding radio button index
	for (int nSetting = 0; nSetting<m_baud; nSetting++)
		baudrate = baudrate << 1;

	switch(m_parity)
	{
		case 0: parity = (TCHAR)'N'; break;
		case 1: parity = (TCHAR)'O'; break;
		case 2: parity = (TCHAR)'E'; break;
	}

	// update commctrl's Settings property
	str.Format(_T("%d,%C,%d,%d"),baudrate,parity,m_databits+7,m_stopbits+1);	
	pMainWnd->GetCommCtrl()->SetSettings(str);

	// set echo, handshaking (flow) and comm port settings here!!!
	pMainWnd->GetEditCtrl()->m_bEcho = m_echo;
	pMainWnd->GetCommCtrl()->SetHandshaking(m_flow);
	pMainWnd->GetCommCtrl()->SetCommPort(m_comport+1); // port settings start at 1

	EndDialog(IDOK);
}

