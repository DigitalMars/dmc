// chkbook.h : main header file for the CHKBOOK application
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

#include "resource.h"       // main symbols
#include "mainfrm.h"
#include "chkbkfrm.h"
#include "dollcent.h"
#include "fxrecdoc.h"
#include "checkdoc.h"
#include "checkvw.h"
#include "rowview.h"
#include "bookvw.h"

class CChkBookApp : public CWinApp
{
public:
	CChkBookApp();
	CMultiDocTemplate* m_pBookViewTemplate;
	CMultiDocTemplate* m_pCheckViewTemplate;

// Operations
	void UpdateIniFileWithDocPath(LPCTSTR lpszPathName);

// Implementation
protected:
	// overrides of CWinApp
	virtual BOOL InitInstance();
#ifdef _MAC
	virtual BOOL CreateInitialDocument();
#endif
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);

	CString GetDocPathFromIniFile();
	//{{AFX_MSG(CChkBookApp)
	afx_msg void OnFileNew();
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CChkBookApp theApp;

/////////////////////////////////////////////////////////////////////////////
