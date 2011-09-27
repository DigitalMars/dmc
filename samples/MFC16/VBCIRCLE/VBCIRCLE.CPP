// vbcircle.cpp : Defines the class behaviors for the application.
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


#include "stdafx.h"
#include "vbcircle.h"

#include "frame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVbcircleApp

BEGIN_MESSAGE_MAP(CVBCircleApp, CWinApp)
	//{{AFX_MSG_MAP(CVBCircleApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVBCircleApp construction
// Place all significant initialization in InitInstance

CVBCircleApp::CVBCircleApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVBCircleApp object

CVBCircleApp NEAR theApp;

/////////////////////////////////////////////////////////////////////////////
// CVBCircleApp initialization

BOOL CVBCircleApp::InitInstance()
{
	SetDialogBkColor();        // set dialog background color
	EnableVBX();               // Initialize VBX support

	// Check to see that the VBX control is present
	if (LoadVBXFile("Circ3.vbx") > HINSTANCE_ERROR)
		UnloadVBXFile("Circ3.vbx");
	else
	{
		AfxMessageBox("Cannot Load Circ3.VBX\nPlace the file on the path",
						MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}



	CFrameWnd* pMainFrame = new CVBCircleFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	m_pMainWnd = pMainFrame;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

void CVBCircleApp::OnAppAbout()
{
	CDialog(IDD_ABOUTBOX).DoModal();
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_VBX_REGISTER_MAP()
	UINT NEAR VBN_CLICKIN = AfxRegisterVBEvent("CLICKIN");
	UINT NEAR VBN_CLICKOUT = AfxRegisterVBEvent("CLICKOUT");
//}}AFX_VBX_REGISTER_MAP

/////////////////////////////////////////////////////////////////////////////
// CVBCircleApp commands
