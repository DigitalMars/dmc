// WWWQuote.cpp : Defines the initialization routines for the DLL.
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

#include "stdafx.h"
#include "WWWQuote.h"
#include "Queries.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWWWQuoteApp

BEGIN_MESSAGE_MAP(CWWWQuoteApp, CWinApp)
	//{{AFX_MSG_MAP(CWWWQuoteApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWWWQuoteApp construction

CWWWQuoteApp::CWWWQuoteApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWWWQuoteApp object

CWWWQuoteApp theApp;


// Database logins
// and resource type names
// these are some of our
// favorite strings

// You might need to change szConnectString to be appropriate for your
// database and database server.

static const TCHAR szConnectString[] = _T("ODBC;UID=sa;PWD=;");
static const TCHAR szHTMLType[] = _T("HTML");


/////////////////////////////////////////////////////////////////////////////
//

CWWWQuote theExtension;

BEGIN_PARSE_MAP(CWWWQuote, CHttpServer)
	DEFAULT_PARSE_COMMAND(Quote, CWWWQuote)
	ON_PARSE_COMMAND(Quote, CWWWQuote, ITS_EMPTY)
	ON_PARSE_COMMAND(Issues, CWWWQuote, ITS_PSTR)
	ON_PARSE_COMMAND_PARAMS("Method")
	ON_PARSE_COMMAND(GetQuotes, CWWWQuote, ITS_PSTR ITS_I4
		ITS_I4 ITS_I4 ITS_I4 ITS_I4 ITS_I4)
	ON_PARSE_COMMAND_PARAMS("Ticker=~ Month=0 Year=0 MinMonth=0 MinYear=0 MaxMonth=0 MaxYear=0")
	ON_PARSE_COMMAND(PreviousMonth, CWWWQuote, ITS_PSTR ITS_I4 ITS_I4 ITS_I4 ITS_I4 ITS_I4 ITS_I4)
	ON_PARSE_COMMAND_PARAMS("Ticker Month Year MinMonth MinYear MaxMonth MaxYear")
	ON_PARSE_COMMAND(NextMonth, CWWWQuote, ITS_PSTR ITS_I4 ITS_I4 ITS_I4 ITS_I4 ITS_I4 ITS_I4)
	ON_PARSE_COMMAND_PARAMS("Ticker Month Year MinMonth MinYear MaxMonth MaxYear")
END_PARSE_MAP(CWWWQuote)

CWWWQuote::CWWWQuote()
{
}

CWWWQuote::~CWWWQuote()
{
}

BOOL CWWWQuote::LoadLongResource(CString& str, UINT nID)
{
	HRSRC hRes;
	HINSTANCE hInst = AfxGetResourceHandle();
	BOOL bResult = FALSE;
	
	hRes = FindResource(hInst, MAKEINTRESOURCE(nID), szHTMLType);
	if (hRes == NULL)
		ISAPITRACE1("Error: Resource %d could not be found\r\n", nID);
	else
	{
		DWORD dwSize = SizeofResource(hInst, hRes);
		if (dwSize == 0)
		{
			str.Empty();
			bResult = TRUE;
		}
		else
		{
			LPTSTR pszStorage = str.GetBufferSetLength(dwSize);

			HGLOBAL hGlob = LoadResource(hInst, hRes);
			if (hGlob != NULL)
			{
				LPVOID lpData = LockResource(hGlob);

				if (lpData != NULL)
				{
					memcpy(pszStorage, lpData, dwSize);
					bResult = TRUE;
				}

				FreeResource(hGlob);
			}
		}
	}

#ifdef _DEBUG
	if (!bResult)
		str.Format(_T("<b>Could not find string %d</b>"), nID);
#endif

	return bResult;
}

void CWWWQuote::WritePageTitle(CHttpServerContext* pCtxt, UINT nID)
{
	CString strTitle;
	CString strAddOn;

	strTitle.LoadString(IDS_PAGE_TITLE);

	if (strAddOn.LoadString(nID))
	{
		strTitle += ": ";
		strTitle += strAddOn;
	}

	*pCtxt << "<title>";
	*pCtxt << strTitle;
	*pCtxt << "</title>";
}

void CWWWQuote::Quote(CHttpServerContext* pCtxt)
{
	StartContent(pCtxt);
	WritePageTitle(pCtxt, IDS_WELCOME);

	CString strOutput;
	LoadLongResource(strOutput, IDS_HTML_WELCOME);
	*pCtxt << strOutput;

	EndContent(pCtxt);
}

void CWWWQuote::WriteIssuesHeader(CHttpServerContext* pCtxt)
{
	CString strOutput;
	LoadLongResource(strOutput, IDS_ISSUES_HEADER);
	*pCtxt << strOutput;
}

void CWWWQuote::Issues(CHttpServerContext* pCtxt, LPCTSTR pszMethod)
{
	StartContent(pCtxt);
	WritePageTitle(pCtxt, IDS_ISSUES);

	BOOL bByCUSIP;
	CString strOutput;

	if (_tcscmp(pszMethod, _T("ByCUSIP")) == 0)
		bByCUSIP = TRUE;
	else if (_tcscmp(pszMethod, _T("ByName")) == 0)
		bByCUSIP = FALSE;
	else 
	{
		LoadLongResource(strOutput, IDS_HTML_LOOKUPMETHOD);
		*pCtxt << strOutput;
		EndContent(pCtxt);
		return;
	}

	try
	{
		CDatabase db;
		db.Open(_T("Stock Quotes"), FALSE, FALSE, szConnectString, FALSE);
		CIssueQuery IssueQuery(&db, bByCUSIP);

		if (IssueQuery.Open())
		{
			if (IssueQuery.IsBOF())
				*pCtxt << _T("Sorry, no issues are available at this time.\r\n");
			else
			{
				LPCTSTR pszText;
				BOOL bFirstRow = TRUE;

				while (!IssueQuery.IsEOF())
				{
					if (bFirstRow == TRUE)
					{
						WriteIssuesHeader(pCtxt);
						bFirstRow = FALSE;
					}

					if (bByCUSIP)
						pszText = IssueQuery.m_strCUSIP;
					else
						pszText = IssueQuery.m_strName;

					strOutput.Format(_T("<OPTION VALUE=\"%s\">%s\r\n"),
						IssueQuery.m_strTicker, pszText);

					*pCtxt << strOutput;
					IssueQuery.MoveNext();
				}
			}
			IssueQuery.Close();
		}
		else
			*pCtxt << _T("Couldn't open record set\r\n");

		db.Close();
	}
	catch (CDBException* pEx)
	{
		*pCtxt << _T("WWWQuote Failed: Couldn't open data source\r\n");

		TCHAR szError[1024];
		if (pEx->GetErrorMessage(szError, sizeof(szError)))
		{
			*pCtxt << szError;
			*pCtxt << _T("\r\n");
		}
	}

	LoadLongResource(strOutput, IDS_ISSUES_TRAILER);
	*pCtxt << strOutput;

	EndContent(pCtxt);
}

void CWWWQuote::WriteQuoteHeader(CHttpServerContext* pCtxt, LPCTSTR pszTicker)
{
	CString strResource;
	CString strOutput;
	LoadLongResource(strResource, IDS_QUOTE_HEADER);
	
	strOutput.Format(strResource, pszTicker);
	*pCtxt << strOutput;
}

void CWWWQuote::WritePrevButton(CHttpServerContext* pCtxt, LPCTSTR pszTicker,
	int nMonth, int nYear, int nMinMonth, int nMinYear,
	int nMaxMonth, int nMaxYear)
{
	CString strResource;
	CString strOutput;
	LoadLongResource(strResource, IDS_PREV_BUTTON);
	
	strOutput.Format(strResource, pszTicker, nMonth, nYear,
		nMinMonth, nMinYear, nMaxMonth, nMaxYear);
	*pCtxt << strOutput;
}

void CWWWQuote::WriteNextButton(CHttpServerContext* pCtxt, LPCTSTR pszTicker,
	int nMonth, int nYear, int nMinMonth, int nMinYear,
	int nMaxMonth, int nMaxYear)
{
	CString strResource;
	CString strOutput;
	LoadLongResource(strResource, IDS_NEXT_BUTTON);
	
	strOutput.Format(strResource, pszTicker, nMonth, nYear,
		nMinMonth, nMinYear, nMaxMonth, nMaxYear);
	*pCtxt << strOutput;
}

void CWWWQuote::GetQuotes(CHttpServerContext* pCtxt, LPTSTR pstrTicker,
	int nMonth, int nYear, int nMinMonth, int nMinYear, int nMaxMonth,
	int nMaxYear)
{
	StartContent(pCtxt);
	WritePageTitle(pCtxt, IDS_GETQUOTES);

	CString strOutput;

	// if there's an error, complain to the user

	if ((nYear == 0 && nMonth != 99)
		|| nMonth == 0 || _tcscmp(pstrTicker, _T("~")) == 0)
 	{
		LoadLongResource(strOutput, IDS_GET_QUOTES_ERROR);
		*pCtxt << strOutput;
		return;
	}

	// Otherwise, try and fetch the quotes
	// CQuoteQuery knows how to handle the (nMonth == 99) case
	
	try
	{
		CDatabase db;
		db.Open(_T("Stock Quotes"), FALSE, FALSE, szConnectString, FALSE);

		// if any one of these isn't supplied, we need to fetch them

		if (nMinYear == 0 || nMaxYear == 0 ||
			nMinMonth == 0 || nMaxMonth == 0)
		{
			CRangeQuery RangeQuery(&db, pstrTicker);
			if (!RangeQuery.Open())
			{
				LoadLongResource(strOutput, IDS_GET_QUOTES_ERROR);
				*pCtxt << strOutput;
				return;
			}
			RangeQuery.Close();

			nMinYear = RangeQuery.m_tMin.GetYear();
			nMaxYear = RangeQuery.m_tMax.GetYear();
			nMinMonth = RangeQuery.m_tMin.GetMonth();
			nMaxMonth = RangeQuery.m_tMax.GetMonth();
			nMonth = nMaxMonth;
			nYear = nMaxYear;
		}
		
		CQuoteQuery QuoteQuery(&db, pstrTicker, nMonth, nYear);

		if (QuoteQuery.Open())
		{
			if (QuoteQuery.IsBOF())
				*pCtxt << _T("Sorry, no quotes are available at this time.\r\n");
			else
			{
				BOOL bFirstRow = TRUE;

				while (!QuoteQuery.IsEOF())
				{
					if (bFirstRow == TRUE)
					{
						WriteQuoteHeader(pCtxt, pstrTicker);
						bFirstRow = FALSE;
					}

					CString strFormatDate;
					strFormatDate = QuoteQuery.m_Date.Format(_T("%m/%d/%Y"));

					if (QuoteQuery.m_Volume != -1)
					{
						strOutput.Format(IDS_QUOTEROW,
							(LPCTSTR) strFormatDate,
							(LPCTSTR) QuoteQuery.m_HighAsk,
							(LPCTSTR) QuoteQuery.m_LowBid,
							(LPCTSTR) QuoteQuery.m_CloseAvg, QuoteQuery.m_Volume);
					}
					else
					{
						strOutput.Format(IDS_HOLIDAY,
							(LPCTSTR) strFormatDate);
					}

					*pCtxt << strOutput;
					QuoteQuery.MoveNext();
				}
			}
			QuoteQuery.Close();

			*pCtxt << _T("</TABLE>");

			if (nYear < nMaxYear || (nYear == nMaxYear && nMonth < nMaxMonth))
				WriteNextButton(pCtxt, pstrTicker, nMonth, nYear,
					nMinMonth, nMinYear, nMaxMonth, nMaxYear);

			if (nYear > nMinYear || (nYear == nMinYear && nMonth > nMinMonth))
				WritePrevButton(pCtxt, pstrTicker, nMonth, nYear,
					nMinMonth, nMinYear, nMaxMonth, nMaxYear);
		}
		else
			*pCtxt << _T("Couldn't open record set\r\n");

		db.Close();
	}
	catch (CDBException* pEx)
	{
		*pCtxt << _T("WWWQuote Failed: Couldn't open data source\r\n");

		TCHAR szError[1024];
		if (pEx->GetErrorMessage(szError, sizeof(szError)))
		{
			*pCtxt << szError;
			*pCtxt << _T("\r\n");
		}
	}

	EndContent(pCtxt);
}


void CWWWQuote::PreviousMonth(CHttpServerContext* pCtxt, LPTSTR pstrTicker,
		int nMonth, int nYear, int nMinMonth, int nMinYear,
		int nMaxMonth, int nMaxYear)
{
	nMonth--;
	if (nMonth == 0)
	{
		nMonth = 12;
		nYear--;
	}

	GetQuotes(pCtxt, pstrTicker, nMonth, nYear, nMinMonth, nMinYear,
		nMaxMonth, nMaxYear);
}

void CWWWQuote::NextMonth(CHttpServerContext* pCtxt, LPTSTR pstrTicker,
		int nMonth, int nYear, int nMinMonth, int nMinYear,
		int nMaxMonth, int nMaxYear)
{
	nMonth++;
	if (nMonth == 13)
	{
		nMonth = 1;
		nYear++;
	}

	GetQuotes(pCtxt, pstrTicker, nMonth, nYear, nMinMonth, nMinYear,
		nMaxMonth, nMaxYear);
}

