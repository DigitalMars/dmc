// vbcircle.h : main header file for the VBCIRCLE application
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
// CVBCircleApp:
// See vbcircle.cpp for the implementation of this class
//

class CVBCircleApp : public CWinApp
{
public:
	CVBCircleApp();

// Overrides
	virtual BOOL InitInstance();

// Implementation

	//{{AFX_MSG(CVBCircleApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// VBX control notifications

//{{AFX_VBX_REGISTER()
	extern UINT NEAR VBN_CLICKIN;
	extern UINT NEAR VBN_CLICKOUT;
//}}AFX_VBX_REGISTER

/////////////////////////////////////////////////////////////////////////////
