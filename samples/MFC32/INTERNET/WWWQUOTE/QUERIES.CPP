// IssueQuery.cpp : implementation file
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
// CIssueQuery

IMPLEMENT_DYNAMIC(CIssueQuery, CRecordset)

CIssueQuery::CIssueQuery(CDatabase* pdb, BOOL bByCUSIP)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CIssueQuery)
	m_strCUSIP = _T("");
	m_strName = _T("");
	m_strTicker = _T("");
	m_nFields = 3;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_bByCUSIP = bByCUSIP;
}


CString CIssueQuery::GetDefaultConnect()
{
	return _T("ODBC;DSN=Stock Quotes");
}

CString CIssueQuery::GetDefaultSQL()
{
	CString strQuery;

	if (m_bByCUSIP)
		strQuery = _T("SELECT Ticker, CUSIP, Name FROM Issues ORDER BY CUSIP");
	else
		strQuery = _T("SELECT Ticker, CUSIP, Name FROM Issues ORDER BY Name");

	return strQuery;
}

void CIssueQuery::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CIssueQuery)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[Ticker]"), m_strTicker);
	RFX_Text(pFX, _T("[CUSIP]"), m_strCUSIP);
	RFX_Text(pFX, _T("[Name]"), m_strName);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CIssueQuery diagnostics

#ifdef _DEBUG
void CIssueQuery::AssertValid() const
{
	CRecordset::AssertValid();
}

void CIssueQuery::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CQuoteQuery

IMPLEMENT_DYNAMIC(CQuoteQuery, CRecordset)

CQuoteQuery::CQuoteQuery(CDatabase* pdb, LPCTSTR strTicker, int nMonth, int nYear)
	: CRecordset(pdb), m_strTicker(strTicker)
{
	//{{AFX_FIELD_INIT(CQuoteQuery)
	m_Volume = 0;
	m_HighAsk = _T("");
	m_LowBid = _T("");
	m_CloseAvg = _T("");
	m_nFields = 5;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;

	m_nMonth = nMonth;
	m_nYear = nYear;
}


CString CQuoteQuery::GetDefaultConnect()
{
	return _T("ODBC;DSN=Stock Quotes");
}

const int nMonthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

CString CQuoteQuery::GetDefaultSQL()
{
	CString strQuery;

	strQuery.Format("SELECT Date, Volume, HighAsk, LowBid, CloseAvg"
					"  FROM Issues, Quotes"
					" WHERE Date BETWEEN {ts '%4.4d-%2.2d-01 00:00:00'}"
					"   AND {ts '%4.4d-%2.2d-%2.2d 00:00:00'}"
					"   AND Issues.Ticker = '%s'"
					"   AND Issues.Ticker = Quotes.Ticker"
					" ORDER BY Date",
					m_nYear, m_nMonth, 
					m_nYear, m_nMonth, nMonthDays[m_nMonth-1], 
					m_strTicker);

	return strQuery;
}

void CQuoteQuery::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CQuoteQuery)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Date(pFX, _T("[Date]"), m_Date);
	RFX_Long(pFX, _T("[Volume]"), m_Volume);
	RFX_Text(pFX, _T("[HighAsk]"), m_HighAsk);
	RFX_Text(pFX, _T("[LowBid]"), m_LowBid);
	RFX_Text(pFX, _T("[CloseAvg]"), m_CloseAvg);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CQuoteQuery diagnostics

#ifdef _DEBUG
void CQuoteQuery::AssertValid() const
{
	CRecordset::AssertValid();
}

void CQuoteQuery::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRangeQuery

IMPLEMENT_DYNAMIC(CRangeQuery, CRecordset)

CRangeQuery::CRangeQuery(CDatabase* pdb, LPCTSTR pszTicker)
	: CRecordset(pdb), m_strTicker(pszTicker)
{
	//{{AFX_FIELD_INIT(CRangeQuery)
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}

CString CRangeQuery::GetDefaultConnect()
{
	return _T("ODBC;DSN=Stock Quotes");
}

CString CRangeQuery::GetDefaultSQL()
{
	CString str;
	str.Format("SELECT MIN(Date) 'Lo', MAX(Date) 'Hi' "
			" FROM Quotes WHERE Ticker = '%s'", m_strTicker);
	return str;
}

void CRangeQuery::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRangeQuery)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Date(pFX, "Lo", m_tMin);
	RFX_Date(pFX, "Hi", m_tMax);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CRangeQuery diagnostics

#ifdef _DEBUG
void CRangeQuery::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRangeQuery::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

