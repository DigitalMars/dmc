// DaoView.h : main header file for the DAOVIEW application
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

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "DaovwDoc.h"		// Document
#include "Crack.h"			// Type conversion helpers
#include "TreeView.h"		// Left hand side view
#include "ListView.h"		// Right hand side view

/////////////////////////////////////////////////////////////////////////////
// CDaoViewApp:
// See DaoView.cpp for the implementation of this class
//

class CDaoViewApp : public CWinApp
{
public:
	CDaoViewApp();

	BOOL m_bShowSystemObjects;
	BOOL m_bShowWarnings;
	BOOL m_bOpenODBC;
	BOOL m_nMaxRecords;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaoViewApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDaoViewApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

void DisplayDaoException(CDaoException* e);
