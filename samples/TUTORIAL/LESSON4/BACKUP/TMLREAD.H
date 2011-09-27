// tmlread.h : main header file for the SDIAPP application
//
// Copyright (c) XYZ Corp., 1994. All Rights Reserved.
//
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

// CTMLReadApp: see tmlread.cpp for the implementation of this class
//

class CTMLReadApp : public CWinApp
{
public:
	CTMLReadApp();

// Overrides
	virtual BOOL InitInstance();

// Implementation

	//{{AFX_MSG(CTMLReadApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassExpress will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// VB-Event extern declarations

//{{AFX_VBX_REGISTER()
//}}AFX_VBX_REGISTER
