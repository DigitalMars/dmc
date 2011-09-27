// WWWQuote.h : main header file for the WWWQUOTE DLL
//

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
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

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWWWQuoteApp
// See WWWQuote.cpp for the implementation of this class
//

class CWWWQuoteApp : public CWinApp
{
public:
	CWWWQuoteApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWWWQuoteApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CWWWQuoteApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
//
//
//

class CWWWQuote : public CHttpServer
{
public:
	CWWWQuote();
	~CWWWQuote();

// parsed commands
	void Quote(CHttpServerContext* pCtxt);
	void Issues(CHttpServerContext* pCtxt, LPCTSTR pszMethod);
	void GetQuotes(CHttpServerContext* pCtxt, LPTSTR pstrTicker,
		int nMonth, int nYear, int nMinMonth, int nMinYear,
		int nMaxMonth, int nMaxYear);
	void PreviousMonth(CHttpServerContext* pCtxt, LPTSTR pstrTicker,
		int nMonth, int nYear, int nMinMonth, int nMinYear,
		int nMaxMonth, int nMaxYear);
	void NextMonth(CHttpServerContext* pCtxt, LPTSTR pstrTicker,
		int nMonth, int nYear, int nMinMonth, int nMinYear,
		int nMaxMonth, int nMaxYear);

// helpers
	void WriteIssuesHeader(CHttpServerContext* pCtxt);
	void WriteQuoteHeader(CHttpServerContext* pCtxt, LPCTSTR pszTicker);

	void WriteNextButton(CHttpServerContext* pCtxt, LPCTSTR pszTicker,
		int nMonth, int nYear, int nMinMonth, int nMinYear,
		int nMaxMonth, int nMaxYear);
	void WritePrevButton(CHttpServerContext* pCtxt, LPCTSTR pszTicker,
		int nMonth, int nYear, int nMinMonth, int nMinYear,
		int nMaxMonth, int nMaxYear);

	BOOL LoadLongResource(CString& str, UINT nID);
	void WritePageTitle(CHttpServerContext* pCtxt, UINT nID);
	DECLARE_PARSE_MAP()
};

