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
// Define global state in ordinary "C" globals

#ifndef _AFXDLL
extern "C"
{
	CWinApp* NEAR afxCurrentWinApp = NULL;
	HINSTANCE NEAR afxCurrentInstanceHandle = NULL;
	HINSTANCE NEAR afxCurrentResourceHandle = NULL;
	const char* NEAR afxCurrentAppName = NULL;
	DWORD NEAR afxTempMapLock = 0;
}
HBRUSH NEAR afxDlgBkBrush = NULL;
COLORREF NEAR afxDlgTextClr = (COLORREF)-1; // not set
#endif // !_AFXDLL

/////////////////////////////////////////////////////////////////////////////
