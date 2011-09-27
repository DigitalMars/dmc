// ccvui.h : main header file for the CCVUI application
//
// Updated: 15th April 2004
//
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCVUI_H__D066096B_8B48_4ECE_8FCC_C3CEF2F945E1__INCLUDED_)
#define AFX_CCVUI_H__D066096B_8B48_4ECE_8FCC_C3CEF2F945E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#	error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCcvUiApp:
// See ccvui.cpp for the implementation of this class
//

class CCcvUiApp : public CWinApp
{
public:
	CCcvUiApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCcvUiApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCcvUiApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCVUI_H__D066096B_8B48_4ECE_8FCC_C3CEF2F945E1__INCLUDED_)