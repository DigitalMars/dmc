// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"

#ifdef AFX_CORE1_SEG
#pragma code_seg(AFX_CORE1_SEG)
#endif

/////////////////////////////////////////////////////////////////////////////
// Standard WinMain implementation
//  Can be replaced as long as 'AfxWinInit' is called first

#ifdef _AFXDLL
#define WinMain AfxWinMain
#endif

#ifndef _USRDLL
extern "C"
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	int nReturnCode = -1;

	// AFX internal initialization
	if (!AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
		goto InitFailure;

	// App global initializations (rare)
	if (hPrevInstance == NULL && !AfxGetApp()->InitApplication())
		goto InitFailure;

	// Perform specific initializations
	if (!AfxGetApp()->InitInstance())
	{
		nReturnCode = AfxGetApp()->ExitInstance();
		goto InitFailure;
	}

	ASSERT_VALID(AfxGetApp());

	nReturnCode = AfxGetApp()->Run();

InitFailure:
	AfxWinTerm();
	return nReturnCode;
}

#else
// _USRDLL library initialization

extern "C"
int FAR PASCAL LibMain(HINSTANCE hInstance,
	WORD wDataSegment, WORD wHeapSize, LPSTR lpszCmdLine)
{
	// Initialize DLL's instance(/module) not the app's
	if (!AfxWinInit(hInstance, NULL, lpszCmdLine, 0))
	{
		AfxWinTerm();
		return 0;       // Init Failed
	}

	// initialize the single instance DLL
	if (AfxGetApp() != NULL && !AfxGetApp()->InitInstance())
	{
		AfxGetApp()->ExitInstance();
		AfxWinTerm();
		return 0;
	}

	// nothing to run
	return 1;   // ok
}

#endif

#ifdef _USRDLL

#ifdef AFX_CORE1_SEG
#pragma code_seg()
#endif

#pragma code_seg("WEP_TEXT")

extern "C" int CALLBACK _WEP(int nExitType)
{
	if (nExitType == WEP_SYSTEM_EXIT)
		return TRUE;

#ifdef _DEBUG
	if (!afxData.bWin31)
	{
		// AfxTrace() may cause Windows to fault under 3.0 when called from
		// inside the WEP, so it is disabled while executing WEP code.
		afxTraceEnabled = FALSE;
	}

	TRACE0("Inside of MFC-supplied _WEP, calling (virtual) CWinApp::ExitInstance\n");

	// Check for missing AfxLockTempMap calls
	if (afxTempMapLock != 0)
		TRACE1("Warning: Temp map lock count non-zero (%ld)\n",
			afxTempMapLock);
#endif

	// NOTE: An application should be careful not to use too much stack space
	//  in the ExitInstance function.  Under Windows 3.0, the WEP is called
	//  on a very small stack owned by the kernel.
	//  Calling Windows USER functions, in particular, is not advised.
	if (AfxGetApp() != NULL)
		AfxGetApp()->ExitInstance();

	// Now it is safe to cleanup the library.
	AfxWinTerm();

	return TRUE;
}

#endif //_USRDLL

/////////////////////////////////////////////////////////////////////////////
