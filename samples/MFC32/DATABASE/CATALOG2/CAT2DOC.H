// catalog2Doc.h : interface of the CCatalog2Doc class
//
/////////////////////////////////////////////////////////////////////////////

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

class CCatalog2Doc : public CDocument
{
protected: // create from serialization only
	CCatalog2Doc();
	DECLARE_DYNCREATE(CCatalog2Doc)

// Attributes
public:

	// current database
	CDatabase	m_Database;

	// table stuff
	CTables*	m_pTableset;
	BOOL		m_bSystemTables;
	BOOL		m_bViews;
	BOOL		m_bSynonyms;

	// column stuff
	CColumns*	m_pColumnset;
	BOOL		m_bLength;
	BOOL		m_bPrecision;
	BOOL		m_bNullability;

	// level possibilities
	enum Level
	{
		levelNone,
		levelTable,
		levelColumn
	};

	Level		m_nLevel;

// Operations
public:

	void	SetLevel(Level nLevel);
	CString	GetDSN();
	void	FetchColumnInfo(LPCSTR lpszName);
	BOOL	FetchTableInfo();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatalog2Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL
	virtual BOOL OnOpenDocument();

// Implementation
public:
	virtual ~CCatalog2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// helpers
	int		GetProfileValue(LPCTSTR lpszSection,LPCTSTR lpszItem);

// Generated message map functions
protected:
	//{{AFX_MSG(CCatalog2Doc)
	afx_msg void OnViewSettings();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
