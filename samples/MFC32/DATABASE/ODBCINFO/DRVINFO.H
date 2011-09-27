// DrvInfo.h : header file
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

class CMyPage : public CPropertyPage
{
protected:

	// SQLGetInfo helpers
	static CString		SQLGetInfoString(UWORD);
	static UWORD		SQLGetInfoShort(UWORD);
	static UDWORD		SQLGetInfoLong(UWORD);

	// formatting helpers
	static void			ZeroIsNoneLong(CString&,UWORD);
	static void			ZeroIsNoneShort(CString&,UWORD);
	static void			YIsYes(CString&,UWORD);
	static void			YIsSupported(CString&,UWORD);
	
	// the database we're showing info about
	static CDatabase*	m_pDatabase;

	// the shared image list for check marks in CListCtrl
	static CImageList*	m_pImageList;

	// the number of active property page objects using
	// the shared imagelist
	static int			m_nClients;

public:

	CMyPage(CDatabase* pDatabase, UINT nIdTemplate);

	virtual ~CMyPage();

	virtual void OnNewDSN() = 0;

	virtual void DumpToFile(CStdioFile&) = 0;
};


/////////////////////////////////////////////////////////////////////////////
// CDriverInfo property page

class CDriverInfo : public CMyPage
{
	DECLARE_DYNCREATE(CDriverInfo)

// Construction
public:
	CDriverInfo(CDatabase* pDatabase = NULL);
	~CDriverInfo();

	void DumpToFile(CStdioFile&);

// Dialog Data
	//{{AFX_DATA(CDriverInfo)
	enum { IDD = IDD_DRIVERINFO };
	CString	m_strSqlUserName;
	CString	m_strSqlServerName;
	CString	m_strSqlDbmsName;
	CString	m_strSqlDbmsVer;
	CString	m_strSqlDriverName;
	CString	m_strSqlDriverOdbcVer;
	CString	m_strSqlDriverVer;
	CString	m_strSqlOdbcVer;
	CString	m_strSqlOdbcApiConformance;
	CString	m_strSqlOdbcSagCliConformance;
	CString	m_strSqlOdbcSqlConformance;
	CString	m_strSqlOdbcSqlOptIef;
	CString	m_strSqlActiveConnections;
	CString	m_strSqlActiveStatements;
	CString	m_strSqlMultResultSets;
	CString	m_strSqlFileUsage;
	CString	m_strSqlDataSourceReadOnly;
	CString	m_strSqlDataSourceName;
	CString	m_strSqlAccessibleProcedures;
	CString	m_strSqlAccessibleTables;
	CString	m_strSqlDatabaseName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDriverInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDriverInfo)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
public:
	void OnNewDSN();

	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CFunctions dialog

class CFunctions : public CMyPage
{
	DECLARE_DYNCREATE(CFunctions)

	static char*	m_szFunctions[];
	static char*	m_szSqlFunctions[];
	static char*	m_szSqlCategories[];
	static SDWORD	m_dwConversionsMasks[];
	static SDWORD	m_dwNumericMasks[];
	static SDWORD	m_dwStringsMasks[];
	static SDWORD	m_dwSystemsMasks[];
	static SDWORD	m_dwTimeDatesMasks[];

// Construction
public:
	CFunctions(CDatabase* pDatabase = NULL);
	~CFunctions();

	void DumpToFile(CStdioFile&);

// Dialog Data
	//{{AFX_DATA(CFunctions)
	enum { IDD = IDD_FUNCTIONS };
	CListCtrl	m_SqlListControl;
	CListCtrl	m_ApiListControl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFunctions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFunctions)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
public:
	void OnNewDSN(void);

	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CSupportedSQL dialog

class CSupportedSQL : public CMyPage
{
	DECLARE_DYNCREATE(CSupportedSQL)

// Construction
public:
	CSupportedSQL(CDatabase* pDatabase = NULL);
	~CSupportedSQL();

	void DumpToFile(CStdioFile&);

// Dialog Data
	//{{AFX_DATA(CSupportedSQL)
	enum { IDD = IDD_SUPPORTED_SQL };
	BOOL	m_bSqlAlterTableAdd;
	BOOL	m_bSqlAlterTableDrop;
	CString	m_strSqlExpressionsInOrderby;
	CString	m_strSqlGroupBy;
	CString	m_strSqlOrderByColumnsInSelect;
	CString	m_strSqlOuterJoins;
	BOOL	m_bSqlSqComparison;
	BOOL	m_bSqlSqCorrelatedSubqueries;
	BOOL	m_bSqlSqExists;
	BOOL	m_bSqlSqIn;
	BOOL	m_bSqlSqQuantified;
	BOOL	m_bSqlUUnion;
	BOOL	m_bSqlUUnionAll;
	CString	m_strSqlColumnAlias;
	CString	m_strSqlLikeEscapeClause;
	BOOL	m_bSqlPsPositionedDelete;
	BOOL	m_bSqlPsPositionedUpdate;
	BOOL	m_bSqlPsSelectForUpdate;
	CString	m_strSqlCorrelationName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSupportedSQL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSupportedSQL)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
public:
	void OnNewDSN();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDataTypes dialog

struct TYPEDATA
{
	CString		strTypeName;
	int			nSqlType;
	int			nPrecision;
	CString		strLiteralPrefix;
	CString		strLiteralSuffix;
	CString		strCreateParams;
	int			nNullable;
	BOOL		bCaseSensitive;
	int			nSearchable;
	int			nUnsigned;
	BOOL		bMoney;
	int			nAutoIncrement;
	int			nMinimumScale;
	int			nMaximumScale;
	TYPEDATA*	pNext;
};

class CDataTypes : public CMyPage
{
	DECLARE_DYNCREATE(CDataTypes)

// Construction
public:
	CDataTypes(CDatabase* pDatabase = NULL);
	~CDataTypes();

	void DumpToFile(CStdioFile&);

	CGetTypeInfo*	m_pGetTypeInfo;

	TYPEDATA*		m_pTypeDataHead;
	TYPEDATA*		m_pTypeDataTail;

// Dialog Data
	//{{AFX_DATA(CDataTypes)
	enum { IDD = IDD_DATA_TYPES };
	CListBox	m_lbConversions;
	CListBox	m_lbDataTypes;
	CString	m_strSqlType;
	CString	m_strPrecision;
	CString	m_strPrefixSuffix;
	CString	m_strCreateParams;
	CString	m_strNullable;
	CString	m_strCaseSensitive;
	CString	m_strSearchable;
	CString	m_strMoney;
	CString	m_strUnsigned;
	CString	m_strAutoIncrement;
	CString	m_strMaximumScale;
	CString	m_strMinimumScale;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDataTypes)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDataTypes)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDataTypeList();
	//}}AFX_MSG

	// helpers
	void InsertTypeData(TYPEDATA*);
	void ShowConversions(SDWORD);
	void PrintConversions(BOOL&,SDWORD,SDWORD,CString&,const CString&,CStdioFile&);

public:
	void OnNewDSN();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CIdentifiers dialog

class CIdentifiers : public CMyPage
{
	DECLARE_DYNCREATE(CIdentifiers)

// Construction
public:
	CIdentifiers(CDatabase* pDatabase = NULL);
	~CIdentifiers();

	void DumpToFile(CStdioFile&);

// Dialog Data
	//{{AFX_DATA(CIdentifiers)
	enum { IDD = IDD_IDENTIFIERS };
	CListBox	m_lbDriverKeywords;
	CListBox	m_lbOdbcKeywords;
	CString	m_strSqlIdentifierCase;
	CString	m_strSqlIdentifierQuoteChar;
	CString	m_strSqlOwnerTerm;
	CString	m_strSqlProcedureTerm;
	CString	m_strSqlQualifierLocation;
	CString	m_strSqlQualifierNameSeparator;
	CString	m_strSqlQualifierTerm;
	CString	m_strSqlQuotedIdentifierCase;
	CString	m_strSqlSearchPatternEscape;
	CString	m_strSqlTableTerm;
	CString	m_strSqlSpecialCharacters;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CIdentifiers)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CIdentifiers)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

public:
	void OnNewDSN();

	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CLimits dialog

class CLimits : public CMyPage
{
	DECLARE_DYNCREATE(CLimits)

// Construction
public:
	CLimits(CDatabase* pDatabase = NULL);
	~CLimits();

	void DumpToFile(CStdioFile&);

// Dialog Data
	//{{AFX_DATA(CLimits)
	enum { IDD = IDD_LIMITS };
	CListCtrl	m_lcSqlTimedateDiffIntervals;
	CListCtrl	m_lcSqlTimedateAddIntervals;
	CString	m_strSqlMaxBinaryLiteralLen;
	CString	m_strSqlMaxCharLiteralLen;
	CString	m_strSqlMaxColumnNameLen;
	CString	m_strSqlMaxColumnsInGroupBy;
	CString	m_strSqlMaxColumnsInIndex;
	CString	m_strSqlMaxColumnsInOrderBy;
	CString	m_strSqlMaxColumnsInSelect;
	CString	m_strSqlMaxColumnsInTable;
	CString	m_strSqlMaxCursorNameLen;
	CString	m_strSqlMaxIndexSize;
	CString	m_strSqlMaxOwnerNameLen;
	CString	m_strSqlMaxProcedureNameLen;
	CString	m_strSqlMaxQualifierNameLen;
	CString	m_strSqlMaxRowSize;
	CString	m_strSqlMaxRowSizeIncludesLong;
	CString	m_strSqlMaxStatementLen;
	CString	m_strSqlMaxTableNameLen;
	CString	m_strSqlMaxTablesInSelect;
	CString	m_strSqlMaxUserNameLen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLimits)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLimits)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	static UWORD	m_nIntervalMasks[];
	static char		*m_szIntervalNames[];

public:
	void OnNewDSN();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMisc1 dialog

class CMisc1 : public CMyPage
{
	DECLARE_DYNCREATE(CMisc1)

// Construction
public:
	CMisc1(CDatabase* pDatabase = NULL);
	~CMisc1();

	void DumpToFile(CStdioFile&);

// Dialog Data
	//{{AFX_DATA(CMisc1)
	enum { IDD = IDD_MISC1 };
	CString	m_strSqlTxnCapable;
	CString	m_strSqlCursorCommitBehavior;
	CString	m_strSqlCursorRollbackBehavior;
	BOOL	m_bSqlTxnReadCommitted;
	BOOL	m_bSqlTxnReadUncommitted;
	BOOL	m_bSqlTxnRepeatableRead;
	BOOL	m_bSqlTxnSerializable;
	BOOL	m_bSqlTxnVersioning;
	CString	m_strSqlDefaultTxnIsolation;
	CString	m_strSqlMultipleActiveTxn;
	BOOL	m_bSqlBpClose;
	BOOL	m_bSqlBpDelete;
	BOOL	m_bSqlBpDrop;
	BOOL	m_bSqlBpOtherHstmt;
	BOOL	m_bSqlBpScroll;
	BOOL	m_bSqlBpTransaction;
	BOOL	m_bSqlBpUpdate;
	BOOL	m_bSqlLckExclusive;
	BOOL	m_bSqlLckNoChange;
	BOOL	m_bSqlLckUnlock;
	BOOL	m_bSqlSsAdditions;
	BOOL	m_bSqlSsDeletions;
	BOOL	m_bSqlSsUpdates;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMisc1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMisc1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
public:
	void OnNewDSN();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMisc2 dialog

class CMisc2 : public CMyPage
{
	DECLARE_DYNCREATE(CMisc2)

// Construction
public:
	CMisc2(CDatabase* pDatabase = NULL);
	~CMisc2();

	void DumpToFile(CStdioFile&);

// Dialog Data
	//{{AFX_DATA(CMisc2)
	enum { IDD = IDD_MISC2 };
	BOOL	m_bSqlOuDmlStatements;
	BOOL	m_bSqlOuIndexDefinition;
	BOOL	m_bSqlOuPrivilegeDefinition;
	BOOL	m_bSqlOuProcedureInvocation;
	BOOL	m_bSqlOuTableDefinition;
	BOOL	m_bSqlQuDmlStatements;
	BOOL	m_bSqlQuIndexDefinition;
	BOOL	m_bSqlQuPrivilegeDefinition;
	BOOL	m_bSqlQuProcedureInvocation;
	BOOL	m_bSqlQuTableDefinition;
	BOOL	m_bSqlSoDynamic;
	BOOL	m_bSqlSoForwardOnly;
	BOOL	m_bSqlSoKeysetDriven;
	BOOL	m_bSqlSoMixed;
	BOOL	m_bSqlSoStatic;
	BOOL	m_bSqlSccoLock;
	BOOL	m_bSqlSccoOptRowver;
	BOOL	m_bSqlSccoOptValues;
	BOOL	m_bSqlSccoReadOnly;
	BOOL	m_bSqlPosAdd;
	BOOL	m_bSqlPosDelete;
	BOOL	m_bSqlPosPosition;
	BOOL	m_bSqlPosRefresh;
	BOOL	m_bSqlPosUpdate;
	BOOL	m_bSqlFdFetchAbsolute;
	BOOL	m_bSqlFdFetchBookmark;
	BOOL	m_bSqlFdFetchFirst;
	BOOL	m_bSqlFdFetchLast;
	BOOL	m_bSqlFdFetchNext;
	BOOL	m_bSqlFdFetchPrior;
	BOOL	m_bSqlFdFetchRelative;
	BOOL	m_bSqlFdFetchResume;
	BOOL	m_bSqlGdAnyColumn;
	BOOL	m_bSqlGdAnyOrder;
	BOOL	m_bSqlGdBlock;
	BOOL	m_bSqlGdBound;
	CString	m_strSqlConcatNullBehavior;
	CString	m_strSqlNeedLongDataLen;
	CString	m_strSqlNonNullableColumns;
	CString	m_strSqlNullCollation;
	CString	m_strSqlProcedures;
	CString	m_strSqlRowUpdates;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMisc2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMisc2)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
public:
	void OnNewDSN();

	DECLARE_MESSAGE_MAP()

};
