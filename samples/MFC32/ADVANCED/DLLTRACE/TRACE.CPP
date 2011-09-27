// trace.cpp : Contains TRACE.DLL implementation and initialization
//              code.
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

#include <afxwin.h>
#include "traceapi.h"
#include "traceres.h"       // trace resources

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
BOOL FAR PASCAL EXPORT PromptTraceFlags(HWND hWndParent, TracerData FAR* lpData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACE0("Inside Trace DLL\n");

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
	CTracerDLL(LPCTSTR pszAppName) : CWinApp(pszAppName) { }
};

BOOL CTracerDLL::InitInstance()
{
	// any DLL initialization goes here
	TRACE0("TRACE.DLL initializing\n");

	return TRUE;
}

int CTracerDLL::ExitInstance()
{
	// any DLL termination goes here (WEP-like code)
	return CWinApp::ExitInstance();
}

extern "C" BOOL FAR PASCAL EXPORT FilterDllMsg(LPMSG lpMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRY
	{
		return AfxGetThread()->PreTranslateMessage(lpMsg);
	}
	END_TRY
	return FALSE;
}

extern "C" void FAR PASCAL EXPORT ProcessDllIdle()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRY
	{
		// flush it all at once
		long lCount = 0;
		while (AfxGetThread()->OnIdle(lCount))
			lCount++;
	}
	END_TRY
}

CTracerDLL  NEAR tracerDLL(_T("trace.dll"));

/////////////////////////////////////////////////////////////////////////////
