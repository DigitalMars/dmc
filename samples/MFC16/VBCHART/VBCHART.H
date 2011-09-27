// vbchart.h : main header file for the VBCHART application
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



#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CVbChartApp:
// See vbchart.cpp for the implementation of this class
//

class CVbChartApp : public CWinApp
{
public:
	CVbChartApp();

// Overrides
	virtual BOOL InitInstance();

// Implementation

	//{{AFX_MSG(CVbChartApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// VB-Event extern declarations

//{{AFX_VBX_REGISTER()
	extern UINT NEAR VBN_CLICK;
	extern UINT NEAR VBN_ROWCOLCHANGE;
	extern UINT NEAR VBN_SELCHANGE;
	extern UINT NEAR VBN_GOTFOCUS;
//}}AFX_VBX_REGISTER

/////////////////////////////////////////////////////////////////////////////
