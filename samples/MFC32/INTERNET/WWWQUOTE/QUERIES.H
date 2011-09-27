// IssueQuery.h : header file
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

/////////////////////////////////////////////////////////////////////////////
// CIssueQuery recordset

class CIssueQuery : public CRecordset
{
public:
	CIssueQuery(CDatabase* pDatabase, BOOL bByCUSIP);
	DECLARE_DYNAMIC(CIssueQuery)

// Field/Param Data
	//{{AFX_FIELD(CIssueQuery, CRecordset)
	CString	m_strCUSIP;
	CString	m_strName;
	//}}AFX_FIELD
	CString m_strTicker;
	BOOL m_bByCUSIP;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIssueQuery)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CQuoteQuery recordset

class CQuoteQuery : public CRecordset
{
public:
	CQuoteQuery(CDatabase* pdb, LPCTSTR strTicker, int nMonth, int nYear);
	DECLARE_DYNAMIC(CQuoteQuery)

// Field/Param Data
	//{{AFX_FIELD(CQuoteQuery, CRecordset)
	CTime   m_Date;
	long	m_Volume;
	CString	m_HighAsk;
	CString	m_LowBid;
	CString	m_CloseAvg;
	//}}AFX_FIELD

	int m_nYear;
	int m_nMonth;
	CString m_strTicker;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuoteQuery)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CRangeQuery recordset

class CRangeQuery : public CRecordset
{
public:
	CRangeQuery(CDatabase* pDatabase, LPCTSTR pszTicker);
	DECLARE_DYNAMIC(CRangeQuery)

// Field/Param Data
	//{{AFX_FIELD(CRangeQuery, CRecordset)
	//}}AFX_FIELD
	CTime	m_tMin;
	CTime	m_tMax;

	CString m_strTicker;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRangeQuery)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};


