// tracer.cpp : Contains TRACER.DLL implementation and initialization
//              code.
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include <afxwin.h>
#include "traceapi.h"
#include "traceres.h"       // tracer resources

#ifndef _DEBUG
#error This source file must be compiled with _DEBUG defined
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialog class

class CPromptDlg : public CDialog
{
public:
	CPromptDlg(BOOL bEnabled, UINT nFlags, CWnd* pParent);

	//{{AFX_DATA(CPromptDlg)
	enum { IDD = IDD_PROMPT };
	BOOL    m_bEnabled;
	BOOL    m_b0;
	BOOL    m_b1;
	BOOL    m_b2;
	BOOL    m_b3;
	BOOL    m_b4;
	//}}AFX_DATA

	UINT CombineFlags();
	virtual void DoDataExchange(CDataExchange* pDX);
	//{{AFX_MSG(CPromptDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CPromptDlg, CDialog)
	//{{AFX_MSG_MAP(CPromptDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CPromptDlg::CPromptDlg(BOOL bEnabled, UINT nFlags, CWnd* pParent)
	: CDialog(CPromptDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPromptDlg)
	m_bEnabled = bEnabled;
	m_b0 = (nFlags & 1) != 0;
	m_b1 = (nFlags & 2) != 0;
	m_b2 = (nFlags & 4) != 0;
	m_b3 = (nFlags & 8) != 0;
	m_b4 = (nFlags & 0x10) != 0;
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
	//}}AFX_DATA_MAP
}

UINT CPromptDlg::CombineFlags()
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
	return nFlags;
}

/////////////////////////////////////////////////////////////////////////////
// Public C interface

extern "C"
BOOL FAR PASCAL _export PromptTraceFlags(HWND hWndParent, TracerData FAR* lpData)
{
	TRACE("Inside Tracer DLL\n");

	// All DLL entry points should have a top-level TRY/CATCH block.
	// Otherwise, it would be possible to throw an uncaught exception from
	//  an the DLL.  This would most likely cause a crash.

	TRY
	{
		CPromptDlg dlg(lpData->bEnabled, lpData->flags,
			CWnd::FromHandle(hWndParent));

		if (dlg.DoModal() != IDOK)
			return FALSE;

		// update the data
		lpData->bEnabled = dlg.m_bEnabled;
		lpData->flags = dlg.CombineFlags();
	}
	CATCH_ALL(e)
	{
		// a failure caused an exception.
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Library init

class CTracerDLL : public CWinApp
{
public:
	virtual BOOL InitInstance(); // Initialization
	virtual int ExitInstance();  // Termination (WEP-like code)

	// nothing special for the constructor
	CTracerDLL(const char* pszAppName)
		: CWinApp(pszAppName)
		{ }
};

BOOL CTracerDLL::InitInstance()
{
	// any DLL initialization goes here
	TRACE("TRACER.DLL initializing\n");
	SetDialogBkColor();     // grey dialogs in the DLL as well
	return TRUE;
}

int CTracerDLL::ExitInstance()
{
	// any DLL termination goes here (WEP-like code)
	return CWinApp::ExitInstance();
}


extern "C" BOOL FAR PASCAL _export FilterDllMsg(LPMSG lpMsg)
{
	TRY
	{
		return AfxGetApp()->PreTranslateMessage(lpMsg);
	}
	END_TRY
	return FALSE;
}

extern "C" void FAR PASCAL _export ProcessDllIdle()
{
	TRY
	{
		// flush it all at once
		long lCount = 0;
		while (AfxGetApp()->OnIdle(lCount))
			lCount++;
	}
	END_TRY
}

CTracerDLL  NEAR tracerDLL("tracer.dll");

/////////////////////////////////////////////////////////////////////////////
