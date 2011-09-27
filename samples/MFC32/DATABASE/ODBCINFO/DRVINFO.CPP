// DrvInfo.cpp : implementation file
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
#include "OdbcInfo.h"
#include "catsets.h"
#include "DrvInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPage property page - base class for all other pages

// static members

CDatabase* CMyPage::m_pDatabase = 0;
CImageList* CMyPage::m_pImageList = 0;
int CMyPage::m_nClients = 0;

// SQLGetInfo helpers
//
CString CMyPage::SQLGetInfoString(UWORD fInfoType)
{
	UCHAR	buffer[200];
	SWORD	cbData;

	::SQLGetInfo(m_pDatabase->m_hdbc,fInfoType,
		(PTR)buffer,200,&cbData);
	return CString(buffer);
}

UWORD CMyPage::SQLGetInfoShort(UWORD fInfoType)
{
	UWORD	value;
	SWORD	cbData;

	::SQLGetInfo(m_pDatabase->m_hdbc,fInfoType,
		(PTR)&value,2,&cbData);
	return value;
}

UDWORD CMyPage::SQLGetInfoLong(UWORD fInfoType)
{
	UDWORD	value;
	SWORD	cbData;

	::SQLGetInfo(m_pDatabase->m_hdbc,fInfoType,
		(PTR)&value,4,&cbData);
	return value;
}

// formatting helpers
//
void CMyPage::ZeroIsNoneLong(CString& string,UWORD fInfoType)
{
	UDWORD value = SQLGetInfoLong(fInfoType);
	if (value)
		string.Format(_T("%d"),value);
	else
		string = _T("<none>");
}

void CMyPage::ZeroIsNoneShort(CString& string,UWORD fInfoType)
{
	UWORD value = SQLGetInfoShort(fInfoType);
	if (value)
		string.Format(_T("%d"),value);
	else
		string = _T("<none>");
}

void CMyPage::YIsYes(CString& string,UWORD fInfoType)
{
	string = SQLGetInfoString(fInfoType);
	if (string == _T("Y"))
		string = _T("Yes");
	else if (string == _T("N"))
		string = _T("No");
}

void CMyPage::YIsSupported(CString& string,UWORD fInfoType)
{
	string = SQLGetInfoString(fInfoType);
	if (string == _T("Y"))
		string = _T("Supported");
	else if (string == _T("N"))
		string = _T("Not Supported");
}

// CMyPage construction and destruction

CMyPage::CMyPage(CDatabase* pDatabase, UINT nIdTemplate)
	: CPropertyPage(nIdTemplate)
{
	// if no other page has set the database, set it
	if (!m_pDatabase)
		m_pDatabase = pDatabase;

	// if no other page has created the image list, create it
	if (!m_pImageList)
	{
		m_pImageList = new CImageList;
		m_pImageList->Create(IDB_IMAGELIST,16,0,RGB(255,255,255));
	}
	m_nClients++;
}

CMyPage::~CMyPage()
{
	// decrement imagelist client count, delete if it becomes 0
	m_nClients--;
	if (!m_nClients)
		delete m_pImageList;
}

/////////////////////////////////////////////////////////////////////////////
// CDriverInfo property page

IMPLEMENT_DYNCREATE(CDriverInfo, CPropertyPage)

CDriverInfo::CDriverInfo(CDatabase* pDatabase)
 : CMyPage(pDatabase,CDriverInfo::IDD)
{
	//{{AFX_DATA_INIT(CDriverInfo)
	m_strSqlUserName = _T("");
	m_strSqlServerName = _T("");
	m_strSqlDbmsName = _T("");
	m_strSqlDbmsVer = _T("");
	m_strSqlDriverName = _T("");
	m_strSqlDriverOdbcVer = _T("");
	m_strSqlDriverVer = _T("");
	m_strSqlOdbcVer = _T("");
	m_strSqlOdbcApiConformance = _T("");
	m_strSqlOdbcSagCliConformance = _T("");
	m_strSqlOdbcSqlConformance = _T("");
	m_strSqlOdbcSqlOptIef = _T("");
	m_strSqlActiveConnections = _T("");
	m_strSqlActiveStatements = _T("");
	m_strSqlMultResultSets = _T("");
	m_strSqlFileUsage = _T("");
	m_strSqlDataSourceReadOnly = _T("");
	m_strSqlDataSourceName = _T("");
	m_strSqlAccessibleProcedures = _T("");
	m_strSqlAccessibleTables = _T("");
	m_strSqlDatabaseName = _T("");
	//}}AFX_DATA_INIT

	OnNewDSN();
}

CDriverInfo::~CDriverInfo()
{
}

void CDriverInfo::DumpToFile(CStdioFile& file)
{
	if (m_pDatabase->IsOpen())
	{
		CString	string;
		file.WriteString(_T("Driver Info\n-----------\n"));
		string.Format(_T("     Data Source Name - %s\n"),m_strSqlDataSourceName);
		file.WriteString(string);
		string.Format(_T("        Database Name - %s\n"),m_strSqlDatabaseName);
		file.WriteString(string);
		string.Format(_T("            User Name - %s\n"),m_strSqlUserName);
		file.WriteString(string);
		string.Format(_T("          Server Name - %s\n"),m_strSqlServerName);
		file.WriteString(string);
		string.Format(_T("                 DBMS - %s version %s\n"),
			m_strSqlDbmsName,m_strSqlDbmsVer);
		file.WriteString(string);
		string.Format(_T("               Driver - %s version %s\n"),
			m_strSqlDriverName,m_strSqlDriverVer);
		file.WriteString(string);
		string.Format(_T("     Driver ODBC Ver. - %s\n"),m_strSqlDriverOdbcVer);
		file.WriteString(string);
		string.Format(_T("         ODBC Version - %s\n"),m_strSqlOdbcVer);
		file.WriteString(string);
		string.Format(_T("      API Conformance - %s\n"),m_strSqlOdbcApiConformance);
		file.WriteString(string);
		string.Format(_T("      SQL Conformance - %s\n"),m_strSqlOdbcSqlConformance);
		file.WriteString(string);
		string.Format(_T("  SAG CLI Conformance - %s\n"),m_strSqlOdbcSagCliConformance);
		file.WriteString(string);
		string.Format(_T("     Optional SQL IEF - %s\n"),m_strSqlOdbcSqlOptIef);
		file.WriteString(string);
		string.Format(_T("   Active Connections - %s\n"),m_strSqlActiveConnections);
		file.WriteString(string);
		string.Format(_T("    Active Statements - %s\n"),m_strSqlActiveStatements);
		file.WriteString(string);
		string.Format(_T(" Multiple Result Sets - %s\n"),m_strSqlMultResultSets);
		file.WriteString(string);
		string.Format(_T("           File Usage - %s\n"),m_strSqlFileUsage);
		file.WriteString(string);
		string.Format(_T("    Read/Write Status - %s\n"),m_strSqlDataSourceReadOnly);
		file.WriteString(string);
		string.Format(_T("Accessible Procedures - %s\n"),m_strSqlAccessibleProcedures);
		file.WriteString(string);
		string.Format(_T("    Accessible Tables - %s\n\n"),m_strSqlAccessibleTables);
		file.WriteString(string);
	}
}

void CDriverInfo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDriverInfo)
	DDX_Text(pDX, IDC_SQL_USER_NAME, m_strSqlUserName);
	DDX_Text(pDX, IDC_SQL_SERVER_NAME, m_strSqlServerName);
	DDX_Text(pDX, IDC_SQL_DBMS_NAME, m_strSqlDbmsName);
	DDX_Text(pDX, IDC_SQL_DBMS_VER, m_strSqlDbmsVer);
	DDX_Text(pDX, IDC_SQL_DRIVER_NAME, m_strSqlDriverName);
	DDX_Text(pDX, IDC_SQL_DRIVER_ODBC_VER, m_strSqlDriverOdbcVer);
	DDX_Text(pDX, IDC_SQL_DRIVER_VER, m_strSqlDriverVer);
	DDX_Text(pDX, IDC_SQL_ODBC_VER, m_strSqlOdbcVer);
	DDX_Text(pDX, IDC_SQL_ODBC_API_CONFORMANCE, m_strSqlOdbcApiConformance);
	DDX_Text(pDX, IDC_SQL_ODBC_SAG_CLI_CONFORMANCE, m_strSqlOdbcSagCliConformance);
	DDX_Text(pDX, IDC_SQL_ODBC_SQL_CONFORMANCE, m_strSqlOdbcSqlConformance);
	DDX_Text(pDX, IDC_SQL_ODBC_SQL_OPT_IEF, m_strSqlOdbcSqlOptIef);
	DDX_Text(pDX, IDC_SQL_ACTIVE_CONNECTIONS, m_strSqlActiveConnections);
	DDX_Text(pDX, IDC_SQL_ACTIVE_STATEMENTS, m_strSqlActiveStatements);
	DDX_Text(pDX, IDC_SQL_MULT_RESULT_SETS, m_strSqlMultResultSets);
	DDX_Text(pDX, IDC_SQL_FILE_USAGE, m_strSqlFileUsage);
	DDX_Text(pDX, IDC_SQL_DATA_SOURCE_READ_ONLY, m_strSqlDataSourceReadOnly);
	DDX_Text(pDX, IDC_DSN, m_strSqlDataSourceName);
	DDX_Text(pDX, IDC_SQL_ACCESSIBLE_PROCEDURES, m_strSqlAccessibleProcedures);
	DDX_Text(pDX, IDC_SQL_ACCESSIBLE_TABLES, m_strSqlAccessibleTables);
	DDX_Text(pDX, IDC_SQL_DATABASE_NAME, m_strSqlDatabaseName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDriverInfo, CPropertyPage)
	//{{AFX_MSG_MAP(CDriverInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDriverInfo message handlers

void CDriverInfo::OnNewDSN()
{
	if (m_pDatabase->IsOpen())
	{
		SWORD	value;
		CString	string;

		m_strSqlDataSourceName = SQLGetInfoString(SQL_DATA_SOURCE_NAME);
		m_strSqlUserName = SQLGetInfoString(SQL_USER_NAME);
		m_strSqlServerName = SQLGetInfoString(SQL_SERVER_NAME);
		m_strSqlDatabaseName = SQLGetInfoString(SQL_DATABASE_NAME);
		m_strSqlDbmsName = SQLGetInfoString(SQL_DBMS_NAME);
		m_strSqlDbmsVer = SQLGetInfoString(SQL_DBMS_VER);
		m_strSqlDriverName = SQLGetInfoString(SQL_DRIVER_NAME);
		m_strSqlDriverVer = SQLGetInfoString(SQL_DRIVER_VER);
		m_strSqlDriverOdbcVer = SQLGetInfoString(SQL_DRIVER_ODBC_VER);
		m_strSqlOdbcVer = SQLGetInfoString(SQL_ODBC_VER);
		switch (SQLGetInfoShort(SQL_ODBC_API_CONFORMANCE))
		{
		case SQL_OAC_NONE:
			m_strSqlOdbcApiConformance = _T("Core");
			break;
		case SQL_OAC_LEVEL1:
			m_strSqlOdbcApiConformance = _T("Level 1");
			break;
		case SQL_OAC_LEVEL2:
			m_strSqlOdbcApiConformance = _T("Level 2");
			break;
		}
		switch (SQLGetInfoShort(SQL_ODBC_SQL_CONFORMANCE))
		{
		case SQL_OSC_MINIMUM:
			m_strSqlOdbcSqlConformance = _T("Minimum");
			break;
		case SQL_OSC_CORE:
			m_strSqlOdbcSqlConformance = _T("Core");
			break;
		case SQL_OSC_EXTENDED:
			m_strSqlOdbcSqlConformance = _T("Extended");
			break;
		}
		switch (SQLGetInfoShort(SQL_ODBC_SAG_CLI_CONFORMANCE))
		{
		case SQL_OSCC_NOT_COMPLIANT:
			m_strSqlOdbcSagCliConformance = _T("No");
			break;
		case SQL_OSCC_COMPLIANT:
			m_strSqlOdbcSagCliConformance = _T("Yes");
			break;
		}
		YIsSupported(m_strSqlOdbcSqlOptIef,SQL_ODBC_SQL_OPT_IEF);
		if (value = SQLGetInfoShort(SQL_ACTIVE_CONNECTIONS))
			m_strSqlActiveConnections.Format(_T("%d"),value);
		else
			m_strSqlActiveConnections = _T("No Limit");
		if (value = SQLGetInfoShort(SQL_ACTIVE_STATEMENTS))
			m_strSqlActiveStatements.Format(_T("%d"),value);
		else
			m_strSqlActiveStatements = _T("No Limit");
		YIsSupported(m_strSqlMultResultSets,SQL_MULT_RESULT_SETS);
		switch (SQLGetInfoShort(SQL_FILE_USAGE))
		{
		case SQL_FILE_NOT_SUPPORTED:
			m_strSqlFileUsage = _T("<n/a>, Driver not single-tier");
			break;
		case SQL_FILE_TABLE:
			m_strSqlFileUsage = _T("Files are Tables");
			break;
		case SQL_FILE_QUALIFIER:
			m_strSqlFileUsage = _T("Files are Qualifiers");
			break;
		}
		
		string = SQLGetInfoString(SQL_DATA_SOURCE_READ_ONLY);
		if (string == _T("Y"))
			m_strSqlDataSourceReadOnly = _T("Data source is read-only");
		else if (string == _T("N"))
			m_strSqlDataSourceReadOnly = _T("Data source is read-write");

		YIsYes(m_strSqlAccessibleProcedures,SQL_ACCESSIBLE_PROCEDURES);
		YIsYes(m_strSqlAccessibleTables,SQL_ACCESSIBLE_TABLES);

		if (IsWindow(m_hWnd))
			UpdateData(FALSE);
	}
}

BOOL CDriverInfo::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CFunctions property page

IMPLEMENT_DYNCREATE(CFunctions, CPropertyPage)

#define	CONVERSIONS_START			0
#define	NUM_CONVERSIONS				1

#define	NUMERIC_START				1
#define	NUM_NUMERICS				24

#define	STRINGS_START				25
#define NUM_STRINGS					19

#define	SYSTEMS_START				44
#define	NUM_SYSTEMS					3

#define	TIMEDATES_START				47
#define	NUM_TIMEDATES				17

SDWORD	CFunctions::m_dwConversionsMasks[] =
{
	SQL_FN_CVT_CONVERT
};

SDWORD	CFunctions::m_dwNumericMasks[] =
{
	SQL_FN_NUM_ABS,
	SQL_FN_NUM_ACOS,
	SQL_FN_NUM_ASIN,
	SQL_FN_NUM_ATAN,
	SQL_FN_NUM_ATAN2,
	SQL_FN_NUM_CEILING,
	SQL_FN_NUM_COS,
	SQL_FN_NUM_COT,
	SQL_FN_NUM_DEGREES,
	SQL_FN_NUM_EXP,
	SQL_FN_NUM_FLOOR,
	SQL_FN_NUM_LOG,
	SQL_FN_NUM_LOG10,
	SQL_FN_NUM_MOD,
	SQL_FN_NUM_PI,
	SQL_FN_NUM_POWER,
	SQL_FN_NUM_RADIANS,
	SQL_FN_NUM_RAND,
	SQL_FN_NUM_ROUND,
	SQL_FN_NUM_SIGN,
	SQL_FN_NUM_SIN,
	SQL_FN_NUM_SQRT,
	SQL_FN_NUM_TAN,
	SQL_FN_NUM_TRUNCATE
};

SDWORD	CFunctions::m_dwStringsMasks[] =
{
	SQL_FN_STR_ASCII,
	SQL_FN_STR_CHAR,
	SQL_FN_STR_CONCAT,
	SQL_FN_STR_DIFFERENCE,
	SQL_FN_STR_INSERT,
	SQL_FN_STR_LCASE,
	SQL_FN_STR_LEFT,
	SQL_FN_STR_LENGTH,
	SQL_FN_STR_LOCATE,
	SQL_FN_STR_LOCATE_2,
	SQL_FN_STR_LTRIM,
	SQL_FN_STR_REPEAT,
	SQL_FN_STR_REPLACE,
	SQL_FN_STR_RIGHT,
	SQL_FN_STR_RTRIM,
	SQL_FN_STR_SOUNDEX,
	SQL_FN_STR_SPACE,
	SQL_FN_STR_SUBSTRING,
	SQL_FN_STR_UCASE
};

SDWORD	CFunctions::m_dwSystemsMasks[] =
{
	SQL_FN_SYS_DBNAME,
	SQL_FN_SYS_IFNULL,
	SQL_FN_SYS_USERNAME,
};

SDWORD	CFunctions::m_dwTimeDatesMasks[] =
{
	SQL_FN_TD_CURDATE,
	SQL_FN_TD_CURTIME,
	SQL_FN_TD_DAYNAME,
	SQL_FN_TD_DAYOFMONTH,
	SQL_FN_TD_DAYOFWEEK,
	SQL_FN_TD_DAYOFYEAR,
	SQL_FN_TD_HOUR,
	SQL_FN_TD_MINUTE,
	SQL_FN_TD_MONTH,
	SQL_FN_TD_MONTHNAME,
	SQL_FN_TD_NOW,
	SQL_FN_TD_QUARTER,
	SQL_FN_TD_SECOND,
	SQL_FN_TD_TIMESTAMPADD,
	SQL_FN_TD_TIMESTAMPDIFF,
	SQL_FN_TD_WEEK,
	SQL_FN_TD_YEAR,
};

char*	CFunctions::m_szSqlFunctions[] =
{
	_T("Convert()"),	// conversion
	_T("Abs()"),		// numeric
	_T("Acos()"),
	_T("Asin()"),
	_T("Atan()"),
	_T("Atan2()"),
	_T("Ceiling()"),
	_T("Cos()"),
	_T("Cot()"),
	_T("Degrees()"),
	_T("Exp()"),
	_T("Floor()"),
	_T("Log()"),
	_T("Log10()"),
	_T("Mod()"),
	_T("Pi()"),
	_T("Power()"),
	_T("Radians()"),
	_T("Rand()"),
	_T("Round()"),
	_T("Sign()"),
	_T("Sin()"),
	_T("Sqrt()"),
	_T("Tan()"),
	_T("Truncate()"),
	_T("Ascii()"),
	_T("Char()"),
	_T("Concat()"),
	_T("Difference()"),
	_T("Insert()"),
	_T("LCase()"),
	_T("Left()"),
	_T("Lenght()"),
	_T("Locate()"),
	_T("Locate2()"),
	_T("LTrim()"),
	_T("Repeat()"),
	_T("Replace()"),
	_T("Right()"),
	_T("RTrim()"),
	_T("Soundex()"),
	_T("Space()"),
	_T("Substring()"),
	_T("UCase()"),
	_T("DBName()"),
	_T("IfNull()"),
	_T("UserName()"),
	_T("CurDate()"),
	_T("CurTime()"),
	_T("DayName()"),
	_T("DayOfMonth()"),
	_T("DayOfWeek()"),
	_T("DayOfYear()"),
	_T("Hour()"),
	_T("Minute()"),
	_T("Month()"),
	_T("MonthName()"),
	_T("Now()"),
	_T("Quarter()"),
	_T("Second()"),
	_T("TimeStampAdd()"),
	_T("TimeStampDiff()"),
	_T("Week()"),
	_T("Year()")
};

char* CFunctions::m_szSqlCategories[] =
{
	_T("Conversion"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("Numeric"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("String"),
	_T("System"),
	_T("System"),
	_T("System"),
	_T("TimeDate"),
	_T("TimeDate"),
	_T("TimeDate"),
	_T("TimeDate"),
	_T("TimeDate"),
	_T("TimeDate"),
	_T("TimeDate"),
	_T("TimeDate"),
	_T("TimeDate"),
	_T("TimeDate"),
	_T("TimeDate"),
	_T("TimeDate"),
	_T("TimeDate"),
	_T("TimeDate"),
	_T("TimeDate"),
	_T("TimeDate"),
	_T("TimeDate")
};

char* CFunctions::m_szFunctions[] =
{
	_T("SQLAllocConnect"),
	_T("SQLAllocEnv"),
	_T("SQLAllocStmt"),
	_T("SQLBindCol"),
	_T("SQLCancel"),
	_T("SQLColAttributes"),
	_T("SQLConnect"),
	_T("SQLDescribeCol"),
	_T("SQLDisconnect"),
	_T("SQLError"),
	_T("SQLExecDirect"),
	_T("SQLExecute"),
	_T("SQLFetch"),
	_T("SQLFreeConnect"),
	_T("SQLFreeEnv"),
	_T("SQLFreeStmt"),
	_T("SQLGetCursorName"),
	_T("SQLNumResultCols"),
	_T("SQLPrepare"),
	_T("SQLRowCount"),
	_T("SQLSetCursorName"),
	_T("SQLSetParam"),
	_T("SQLTransact"),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T("SQLColumns"),
	_T("SQLDriverConnect"),
	_T("SQLGetConnectOption"),
	_T("SQLGetData"),
	_T("SQLGetFunctions"),
	_T("SQLGetInfo"),
	_T("SQLGetStmtOption"),
	_T("SQLGetTypeInfo"),
	_T("SQLParamData"),
	_T("SQLPutData"),
	_T("SQLSetConnectOption"),
	_T("SQLSetStmtOption"),
	_T("SQLSpecialColumns"),
	_T("SQLStatistics"),
	_T("SQLTables"),
	_T("SQLBrowseConnect"),
	_T("SQLColumnPrivileges"),
	_T("SQLDataSources"),
	_T("SQLDescribeParam"),
	_T("SQLExtendedFetch"),
	_T("SQLForeignKeys"),
	_T("SQLMoreResults"),
	_T("SQLNativeSql"),
	_T("SQLNumParams"),
	_T("SQLParamOptions"),
	_T("SQLPrimaryKeys"),
	_T("SQLProcedureColumns"),
	_T("SQLProcedures"),
	_T("SQLSetPos"),
	_T("SQLSetScrollOptions"),
	_T("SQLTablePrivileges"),
	_T("SQLDrivers"),
	_T("SQLBindParameter")
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
};

CFunctions::CFunctions(CDatabase* pDatabase)
 : CMyPage(pDatabase, CFunctions::IDD)
{
	//{{AFX_DATA_INIT(CFunctions)
	//}}AFX_DATA_INIT
}

CFunctions::~CFunctions()
{
}

void CFunctions::DumpToFile(CStdioFile& file)
{
	if (m_pDatabase->IsOpen())
	{
		int		i,j;
		CString	string;
		CString	padString;
		UWORD	aFunctions[100];
		SDWORD	value;

		file.WriteString(_T("ODBC API Functions\n------------------\n"));
		::SQLGetFunctions(m_pDatabase->m_hdbc,
				SQL_API_ALL_FUNCTIONS,aFunctions);
		for (i = 0; i < SQL_NUM_FUNCTIONS; i++)
		{
			if (aFunctions[i + 1])
				string.Format(_T("%s - Supported\n"),m_szFunctions[i]);
			else
				string.Format(_T("%s - Not Supported\n"),m_szFunctions[i]);
			padString = _T("");
			int nLimit = 21 - lstrlen(m_szFunctions[i]) / sizeof(TCHAR);
			if (nLimit > 0)
			{
				for (j = 0; j < nLimit; j++)
					padString += ' ';
				file.WriteString(padString);
			}
			file.WriteString(string);
		}
		for (i = 0; i < SQL_EXT_API_LAST - SQL_EXT_API_START + 1; i++)
		{
			if (aFunctions[i + SQL_EXT_API_START])
				string.Format(_T("%s - Supported\n"),m_szFunctions[i + SQL_EXT_API_START - 1]);
			else
				string.Format(_T("%s - Not Supported\n"),m_szFunctions[i + SQL_EXT_API_START - 1]);
			padString = _T("");
			int nLimit = 21 - lstrlen(m_szFunctions[i + SQL_EXT_API_START - 1]) / sizeof(TCHAR);
			if (nLimit > 0)
			{
				for (j = 0; j < nLimit; j++)
					padString += ' ';
				file.WriteString(padString);
			}
			file.WriteString(string);
		}
		file.WriteString(_T("\n"));

		file.WriteString(_T("SQL Conversion Functions\n------------------------\n"));
		value = SQLGetInfoLong(SQL_CONVERT_FUNCTIONS);
		for (i = 0; i < NUM_CONVERSIONS; i++)
		{
			if (value & m_dwConversionsMasks[i])
				string.Format(_T("%s - Supported\n"),m_szSqlFunctions[CONVERSIONS_START + i]);
			else
				string.Format(_T("%s - Not Supported\n"),m_szSqlFunctions[CONVERSIONS_START + i]);
			padString = _T("");
			int nLimit = 21 - lstrlen(m_szSqlFunctions[CONVERSIONS_START + i]) / sizeof(TCHAR);
			if (nLimit > 0)
			{
				for (j = 0; j < nLimit; j++)
					padString += ' ';
				file.WriteString(padString);
			}
			file.WriteString(string);
		}
		file.WriteString(_T("\n"));

		file.WriteString(_T("SQL Numeric Functions\n---------------------\n"));
		value = SQLGetInfoLong(SQL_NUMERIC_FUNCTIONS);
		for (i = 0; i < NUM_NUMERICS; i++)
		{
			if (value & m_dwNumericMasks[i])
				string.Format(_T("%s - Supported\n"),m_szSqlFunctions[NUMERIC_START + i]);
			else
				string.Format(_T("%s - Not Supported\n"),m_szSqlFunctions[NUMERIC_START + i]);
			padString = _T("");
			int nLimit = 21 - lstrlen(m_szSqlFunctions[NUMERIC_START + i]) / sizeof(TCHAR);
			if (nLimit > 0)
			{
				for (j = 0; j < nLimit; j++)
					padString += ' ';
				file.WriteString(padString);
			}
			file.WriteString(string);
		}
		file.WriteString(_T("\n"));

		file.WriteString(_T("SQL String Functions\n--------------------\n"));
		value = SQLGetInfoLong(SQL_STRING_FUNCTIONS);
		for (i = 0; i < NUM_STRINGS; i++)
		{
			if (value & m_dwStringsMasks[i])
				string.Format(_T("%s - Supported\n"),m_szSqlFunctions[STRINGS_START + i]);
			else
				string.Format(_T("%s - Not Supported\n"),m_szSqlFunctions[STRINGS_START + i]);
			padString = _T("");
			int nLimit = 21 - lstrlen(m_szSqlFunctions[STRINGS_START + i]) / sizeof(TCHAR);
			if (nLimit > 0)
			{
				for (j = 0; j < nLimit; j++)
					padString += ' ';
				file.WriteString(padString);
			}
			file.WriteString(string);
		}
		file.WriteString(_T("\n"));

		file.WriteString(_T("SQL System Functions\n--------------------\n"));
		value = SQLGetInfoLong(SQL_SYSTEM_FUNCTIONS);
		for (i = 0; i < NUM_SYSTEMS; i++)
		{
			if (value & m_dwSystemsMasks[i])
				string.Format(_T("%s - Supported\n"),m_szSqlFunctions[SYSTEMS_START + i]);
			else
				string.Format(_T("%s - Not Supported\n"),m_szSqlFunctions[SYSTEMS_START + i]);
			padString = _T("");
			int nLimit = 21 - lstrlen(m_szSqlFunctions[SYSTEMS_START + i]) / sizeof(TCHAR);
			if (nLimit > 0)
			{
				for (j = 0; j < nLimit; j++)
					padString += ' ';
				file.WriteString(padString);
			}
			file.WriteString(string);
		}
		file.WriteString(_T("\n"));

		file.WriteString(_T("SQL Time/Date Functions\n-----------------------\n"));
		value = SQLGetInfoLong(SQL_TIMEDATE_FUNCTIONS);
		for (i = 0; i < NUM_TIMEDATES; i++)
		{
			if (value & m_dwTimeDatesMasks[i])
				string.Format(_T("%s - Supported\n"),m_szSqlFunctions[TIMEDATES_START + i]);
			else
				string.Format(_T("%s - Not Supported\n"),m_szSqlFunctions[TIMEDATES_START + i]);
			padString = _T("");
			int nLimit = 21 - lstrlen(m_szSqlFunctions[TIMEDATES_START + i]) / sizeof(TCHAR);
			if (nLimit > 0)
			{
				for (j = 0; j < nLimit; j++)
					padString += ' ';
				file.WriteString(padString);
			}
			file.WriteString(string);
		}
		file.WriteString(_T("\n"));
	}
}

void CFunctions::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFunctions)
	DDX_Control(pDX, IDC_SQL_FUNCTIONS, m_SqlListControl);
	DDX_Control(pDX, IDC_API_FUNCTIONS, m_ApiListControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFunctions, CPropertyPage)
	//{{AFX_MSG_MAP(CFunctions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFunctions message handlers
void CFunctions::OnNewDSN()
{
	if (::IsWindow(m_hWnd))
	{
		UWORD		aFunctions[100];
		BOOL		bOpen = m_pDatabase->IsOpen();

		SDWORD		value;
		int			nNewItem;

		m_ApiListControl.DeleteAllItems();
		if (bOpen)
			::SQLGetFunctions(m_pDatabase->m_hdbc,
				SQL_API_ALL_FUNCTIONS,aFunctions);
		for (int i = 0; i < SQL_NUM_FUNCTIONS; i++)
		{
			if (bOpen && aFunctions[i + 1])
				m_ApiListControl.InsertItem(i,m_szFunctions[i],1);
			else
				m_ApiListControl.InsertItem(i,m_szFunctions[i],0);
		}
	
		for (i = 0; i < SQL_EXT_API_LAST - SQL_EXT_API_START + 1; i++)
		{
			if (bOpen && aFunctions[i+SQL_EXT_API_START])
				m_ApiListControl.InsertItem(i+SQL_NUM_FUNCTIONS,
					m_szFunctions[i+SQL_EXT_API_START-1],1);
			else
				m_ApiListControl.InsertItem(i+SQL_NUM_FUNCTIONS,
					m_szFunctions[i+SQL_EXT_API_START-1],0);
		}

		m_SqlListControl.DeleteAllItems();

		// conversion function
		if (bOpen)
			value = SQLGetInfoLong(SQL_CONVERT_FUNCTIONS);
		for (i = 0; i < NUM_CONVERSIONS; i++)
		{
			if (bOpen && (value & m_dwConversionsMasks[i]))
				nNewItem = m_SqlListControl.InsertItem(CONVERSIONS_START + i,
					m_szSqlFunctions[CONVERSIONS_START + i],1);
			else
				nNewItem = m_SqlListControl.InsertItem(CONVERSIONS_START + i,
					m_szSqlFunctions[CONVERSIONS_START + i],0);
			m_SqlListControl.SetItem(nNewItem,1,LVIF_TEXT,
				m_szSqlCategories[CONVERSIONS_START + i],0,0,0,0);
		}

		// numeric functions
		if (bOpen)
			value = SQLGetInfoLong(SQL_NUMERIC_FUNCTIONS);
		for (i = 0; i < NUM_NUMERICS; i++)
		{
			if (bOpen && (value & m_dwNumericMasks[i]))
				nNewItem = m_SqlListControl.InsertItem(NUMERIC_START + i,
					m_szSqlFunctions[NUMERIC_START + i],1);
			else
				nNewItem = m_SqlListControl.InsertItem(NUMERIC_START + i,
					m_szSqlFunctions[NUMERIC_START + i],0);
			m_SqlListControl.SetItem(nNewItem,1,LVIF_TEXT,
				m_szSqlCategories[NUMERIC_START + i],0,0,0,0);
		}

		// string functions
		if (bOpen)
			value = SQLGetInfoLong(SQL_STRING_FUNCTIONS);
		for (i = 0; i < NUM_STRINGS; i++)
		{
			if (bOpen && (value & m_dwStringsMasks[i]))
				nNewItem = m_SqlListControl.InsertItem(STRINGS_START + i,
					m_szSqlFunctions[STRINGS_START + i],1);
			else
				nNewItem = m_SqlListControl.InsertItem(STRINGS_START + i,
					m_szSqlFunctions[STRINGS_START + i],0);
			m_SqlListControl.SetItem(nNewItem,1,LVIF_TEXT,
				m_szSqlCategories[STRINGS_START + i],0,0,0,0);
		}

		// system functions
		if (bOpen)
			value = SQLGetInfoLong(SQL_SYSTEM_FUNCTIONS);
		for (i = 0; i < NUM_SYSTEMS; i++)
		{
			if (bOpen && (value & m_dwSystemsMasks[i]))
				nNewItem = m_SqlListControl.InsertItem(SYSTEMS_START + i,
					m_szSqlFunctions[SYSTEMS_START + i],1);
			else
				nNewItem = m_SqlListControl.InsertItem(SYSTEMS_START + i,
					m_szSqlFunctions[SYSTEMS_START + i],0);
			m_SqlListControl.SetItem(nNewItem,1,LVIF_TEXT,
				m_szSqlCategories[SYSTEMS_START + i],0,0,0,0);
		}

		// timedate functions
		if (bOpen)
			value = SQLGetInfoLong(SQL_TIMEDATE_FUNCTIONS);
		for (i = 0; i < NUM_TIMEDATES; i++)
		{
			if (bOpen && (value & m_dwTimeDatesMasks[i]))
				nNewItem = m_SqlListControl.InsertItem(TIMEDATES_START + i,
					m_szSqlFunctions[TIMEDATES_START + i],1);
			else
				nNewItem = m_SqlListControl.InsertItem(TIMEDATES_START + i,
					m_szSqlFunctions[TIMEDATES_START + i],0);
			m_SqlListControl.SetItem(nNewItem,1,LVIF_TEXT,
				m_szSqlCategories[TIMEDATES_START + i],0,0,0,0);
		}
	}
}

BOOL CFunctions::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_ApiListControl.SetImageList(m_pImageList,LVSIL_SMALL);
	m_SqlListControl.SetImageList(m_pImageList,LVSIL_SMALL);

	m_ApiListControl.InsertColumn(0,"",LVCFMT_LEFT,130);
	m_SqlListControl.InsertColumn(0,"Function",LVCFMT_LEFT,120);
	m_SqlListControl.InsertColumn(1,"Category",LVCFMT_LEFT,120);

	OnNewDSN();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CSupportedSQL property page

IMPLEMENT_DYNCREATE(CSupportedSQL, CPropertyPage)

CSupportedSQL::CSupportedSQL(CDatabase* pDatabase)
 : CMyPage(pDatabase,CSupportedSQL::IDD)
{
	//{{AFX_DATA_INIT(CSupportedSQL)
	m_bSqlAlterTableAdd = FALSE;
	m_bSqlAlterTableDrop = FALSE;
	m_strSqlExpressionsInOrderby = _T("");
	m_strSqlGroupBy = _T("");
	m_strSqlOrderByColumnsInSelect = _T("");
	m_strSqlOuterJoins = _T("");
	m_bSqlSqComparison = FALSE;
	m_bSqlSqCorrelatedSubqueries = FALSE;
	m_bSqlSqExists = FALSE;
	m_bSqlSqIn = FALSE;
	m_bSqlSqQuantified = FALSE;
	m_bSqlUUnion = FALSE;
	m_bSqlUUnionAll = FALSE;
	m_strSqlColumnAlias = _T("");
	m_strSqlLikeEscapeClause = _T("");
	m_bSqlPsPositionedDelete = FALSE;
	m_bSqlPsPositionedUpdate = FALSE;
	m_bSqlPsSelectForUpdate = FALSE;
	m_strSqlCorrelationName = _T("");
	//}}AFX_DATA_INIT

	OnNewDSN();
}

CSupportedSQL::~CSupportedSQL()
{
}

void CSupportedSQL::DumpToFile(CStdioFile& file)
{
	if (m_pDatabase->IsOpen())
	{
		BOOL	b;
		CString	string;

		file.WriteString(_T("Supported SQL\n-------------\n"));
		b = FALSE;
		file.WriteString(_T("  ALTER TABLE Support -"));
		if (m_bSqlAlterTableAdd)
		{
			file.WriteString(_T("ADD"));
			b = TRUE;
		}
		if (m_bSqlAlterTableDrop)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("DROP"));
			b = TRUE;
		}
		if (!b)
			file.WriteString(_T(" <none>"));
		file.WriteString(_T("\n"));
		string.Format(_T("       Column Aliases - %s\n"),m_strSqlColumnAlias);
		file.WriteString(string);
		string.Format(_T("    Correlation Names - %s\n"),m_strSqlCorrelationName);
		file.WriteString(string);
		string.Format(_T(" ORDER BY Expressions - %s\n"),m_strSqlExpressionsInOrderby);
		file.WriteString(string);
		string.Format(_T("GROUP BY NonAggregate - %s\n"),m_strSqlGroupBy);
		file.WriteString(string);
		string.Format(_T("LIKE Escape Character - %s\n"),m_strSqlLikeEscapeClause);
		file.WriteString(string);
		string.Format(_T("   ORDER BY in SELECT - %s\n"),m_strSqlOrderByColumnsInSelect);
		file.WriteString(string);
		string.Format(_T("   Outer Join Support - %s\n"),m_strSqlOuterJoins);
		file.WriteString(string);

		file.WriteString(_T("Positioned Statements -"));
		b = FALSE;
		if (m_bSqlPsPositionedDelete)
		{
			file.WriteString(_T("Delete"));
			b = TRUE;
		}
		if (m_bSqlPsPositionedUpdate)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Update"));
			b = TRUE;
		}
		if (m_bSqlPsSelectForUpdate)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Select for Update"));
			b = TRUE;
		}
		if (!b)
			file.WriteString(_T("<none>"));
		file.WriteString(_T("\n"));

		file.WriteString(_T("     Subquery Support -"));
		b = FALSE;
		if (m_bSqlSqCorrelatedSubqueries)
		{
			file.WriteString(_T("Correlated"));
			b = TRUE;
		}
		if (m_bSqlSqComparison)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Comparison"));
			b = TRUE;
		}
		if (m_bSqlSqExists)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("EXISTS"));
			b = TRUE;
		}
		if (m_bSqlSqIn)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("IN"));
			b = TRUE;
		}
		if (m_bSqlSqQuantified)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Quantified"));
			b = TRUE;
		}
		if (!b)
			file.WriteString(_T("<none>"));
		file.WriteString(_T("\n"));

		file.WriteString(_T("        UNION Support -"));
		b = FALSE;
		if (m_bSqlUUnion)
		{
			file.WriteString(_T("UNION"));
			b = TRUE;
		}
		if (m_bSqlUUnionAll)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("UNION with ALL"));
			b = TRUE;
		}
		if (!b)
			file.WriteString(_T("<none>"));
		file.WriteString(_T("\n\n"));
	}
}

void CSupportedSQL::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSupportedSQL)
	DDX_Check(pDX, IDC_AT_ADD, m_bSqlAlterTableAdd);
	DDX_Check(pDX, IDC_AT_DROP, m_bSqlAlterTableDrop);
	DDX_Text(pDX, IDC_SQL_EXPRESSIONS_IN_ORDERBY, m_strSqlExpressionsInOrderby);
	DDX_Text(pDX, IDC_SQL_GROUP_BY, m_strSqlGroupBy);
	DDX_Text(pDX, IDC_SQL_ORDER_BY_COLUMNS_IN_SELECT, m_strSqlOrderByColumnsInSelect);
	DDX_Text(pDX, IDC_SQL_OUTER_JOINS, m_strSqlOuterJoins);
	DDX_Check(pDX, IDC_SQL_SQ_COMPARISON, m_bSqlSqComparison);
	DDX_Check(pDX, IDC_SQL_SQ_CORRELATED_SUBQUERIES, m_bSqlSqCorrelatedSubqueries);
	DDX_Check(pDX, IDC_SQL_SQ_EXISTS, m_bSqlSqExists);
	DDX_Check(pDX, IDC_SQL_SQ_IN, m_bSqlSqIn);
	DDX_Check(pDX, IDC_SQL_SQ_QUANTIFIED, m_bSqlSqQuantified);
	DDX_Check(pDX, IDC_SQL_U_UNION, m_bSqlUUnion);
	DDX_Check(pDX, IDC_SQL_U_UNION_ALL, m_bSqlUUnionAll);
	DDX_Text(pDX, IDC_SQL_COLUMN_ALIAS, m_strSqlColumnAlias);
	DDX_Text(pDX, IDC_SQL_LIKE_ESCAPE_CLAUSE, m_strSqlLikeEscapeClause);
	DDX_Check(pDX, IDC_SQL_PS_POSITIONED_DELETE, m_bSqlPsPositionedDelete);
	DDX_Check(pDX, IDC_SQL_PS_POSITIONED_UPDATE, m_bSqlPsPositionedUpdate);
	DDX_Check(pDX, IDC_SQL_PS_SELECT_FOR_UPDATE, m_bSqlPsSelectForUpdate);
	DDX_Text(pDX, IDC_SQL_CORRELATION_NAME, m_strSqlCorrelationName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSupportedSQL, CPropertyPage)
	//{{AFX_MSG_MAP(CSupportedSQL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSupportedSQL message handlers

void CSupportedSQL::OnNewDSN()
{
	if (m_pDatabase->IsOpen())
	{
		CString	string;
		SDWORD	longValue;

		longValue = SQLGetInfoLong(SQL_ALTER_TABLE);
		if (longValue & SQL_AT_ADD_COLUMN)
			m_bSqlAlterTableAdd = TRUE;
		else
			m_bSqlAlterTableAdd = FALSE;
		if (longValue & SQL_AT_DROP_COLUMN)
			m_bSqlAlterTableDrop = TRUE;
		else
			m_bSqlAlterTableDrop = FALSE;
		YIsSupported(m_strSqlColumnAlias,SQL_COLUMN_ALIAS);
		switch (SQLGetInfoShort(SQL_CORRELATION_NAME))
		{
		case SQL_CN_NONE:
			m_strSqlCorrelationName = _T("Not Supported");
			break;
		case SQL_CN_DIFFERENT:
			m_strSqlCorrelationName = _T("Supported, table names must be different");
			break;
		case SQL_CN_ANY:
			m_strSqlCorrelationName = _T("Supported");
			break;
		}
		YIsSupported(m_strSqlExpressionsInOrderby,SQL_EXPRESSIONS_IN_ORDERBY);
		switch (SQLGetInfoShort(SQL_GROUP_BY))
		{
		case SQL_GB_NOT_SUPPORTED:
			m_strSqlGroupBy = _T("GROUP BY Not Supported");
			break;
		case SQL_GB_GROUP_BY_EQUALS_SELECT:
			m_strSqlGroupBy = _T("GROUP BY Equals SELECT");
			break;
		case SQL_GB_GROUP_BY_CONTAINS_SELECT:
			m_strSqlGroupBy = _T("GROUP BY Contains SELECT");
			break;
		case SQL_GB_NO_RELATION:
			m_strSqlGroupBy = _T("No Relation");
			break;
		}
		YIsSupported(m_strSqlLikeEscapeClause,SQL_LIKE_ESCAPE_CLAUSE);
		YIsYes(m_strSqlOrderByColumnsInSelect,SQL_ORDER_BY_COLUMNS_IN_SELECT);
		string = SQLGetInfoString(SQL_OUTER_JOINS);
		if (string == _T("N"))
			m_strSqlOuterJoins = _T("No");
		else if (string == _T("Y"))
			m_strSqlOuterJoins = _T("Yes");
		else if (string == _T("P"))
			m_strSqlOuterJoins = _T("Partial");
		else if (string == _T("F"))
			m_strSqlOuterJoins = _T("Full");
		longValue = SQLGetInfoLong(SQL_POSITIONED_STATEMENTS);
		if (longValue & SQL_PS_POSITIONED_DELETE)
			m_bSqlPsPositionedDelete = TRUE;
		else
			m_bSqlPsPositionedDelete = FALSE;
		if (longValue & SQL_PS_POSITIONED_UPDATE)
			m_bSqlPsPositionedUpdate = TRUE;
		else
			m_bSqlPsPositionedUpdate = FALSE;
		if (longValue & SQL_PS_SELECT_FOR_UPDATE)
			m_bSqlPsSelectForUpdate = TRUE;
		else
			m_bSqlPsSelectForUpdate = FALSE;
		longValue = SQLGetInfoLong(SQL_SUBQUERIES);
		if (longValue & SQL_SQ_CORRELATED_SUBQUERIES)
			m_bSqlSqCorrelatedSubqueries = TRUE;
		else
			m_bSqlSqCorrelatedSubqueries = FALSE;
		if (longValue & SQL_SQ_COMPARISON)
			m_bSqlSqComparison = TRUE;
		else
			m_bSqlSqComparison = FALSE;
		if (longValue & SQL_SQ_EXISTS)
			m_bSqlSqExists = TRUE;
		else
			m_bSqlSqExists = FALSE;
		if (longValue & SQL_SQ_IN)
			m_bSqlSqIn = TRUE;
		else
			m_bSqlSqIn = FALSE;
		if (longValue & SQL_SQ_QUANTIFIED)
			m_bSqlSqQuantified = TRUE;
		else
			m_bSqlSqQuantified = FALSE;
		longValue = SQLGetInfoLong(SQL_UNION);
		if (longValue & SQL_U_UNION)
			m_bSqlUUnion = TRUE;
		else
			m_bSqlUUnion = FALSE;
		if (longValue & SQL_U_UNION_ALL)
			m_bSqlUUnionAll = TRUE;
		else
			m_bSqlUUnionAll = FALSE;

		if (IsWindow(m_hWnd))
			UpdateData(FALSE);
	}
}

BOOL CSupportedSQL::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CDataTypes property page

IMPLEMENT_DYNCREATE(CDataTypes, CPropertyPage)

CDataTypes::CDataTypes(CDatabase* pDatabase)
	: m_pGetTypeInfo(0),CMyPage(pDatabase,CDataTypes::IDD),
	m_pTypeDataHead(0),m_pTypeDataTail(0)
{
	//{{AFX_DATA_INIT(CDataTypes)
	m_strSqlType = _T("");
	m_strPrecision = _T("");
	m_strPrefixSuffix = _T("");
	m_strCreateParams = _T("");
	m_strNullable = _T("");
	m_strCaseSensitive = _T("");
	m_strSearchable = _T("");
	m_strMoney = _T("");
	m_strUnsigned = _T("");
	m_strAutoIncrement = _T("");
	m_strMaximumScale = _T("");
	m_strMinimumScale = _T("");
	//}}AFX_DATA_INIT
}

CDataTypes::~CDataTypes()
{
	if (m_pGetTypeInfo)
	{
		if (m_pGetTypeInfo->IsOpen())
			m_pGetTypeInfo->Close();
		delete m_pGetTypeInfo;
	}

	// delete type data list
	TYPEDATA*	p1 = m_pTypeDataHead;
	TYPEDATA*	p2;
	while (p1)
	{
		p2 = p1;
		p1 = p1->pNext;
		delete p2;
	}
}

void CDataTypes::DumpToFile(CStdioFile& file)
{
	if (m_pDatabase->IsOpen())
	{
		CString	string;
		CString	workString;
		SDWORD	value;

		file.WriteString(_T("Data Types\n----------\n\n"));
		file.WriteString(_T("Native type         SQL Type          Precision   Min Scale  Max Scale\n"));
		file.WriteString(_T("-----------         --------          ---------   ---------  ---------\n"));
		if (m_pGetTypeInfo)
		{
			if (m_pGetTypeInfo->IsOpen())
				m_pGetTypeInfo->Close();
			delete m_pGetTypeInfo;
		}

		m_pGetTypeInfo = new CGetTypeInfo(m_pDatabase);
		m_pGetTypeInfo->Open();

		while (!m_pGetTypeInfo->IsEOF())
		{
			string = m_pGetTypeInfo->m_strTypeName;
			while (string.GetLength() < 20)
				string += ' ';
			switch (m_pGetTypeInfo->m_fDataType)
			{
			case SQL_BIGINT:
				string += _T("SQL_BIGINT        ");
				break;
			case SQL_BINARY:
				string += _T("SQL_BINARY        ");
				break;
			case SQL_BIT:
				string += _T("SQL_BIT           ");
				break;
			case SQL_CHAR:
				string += _T("SQL_CHAR          ");
				break;
			case SQL_DATE:
				string += _T("SQL_DATE          ");
				break;
			case SQL_DECIMAL:
				string += _T("SQL_DECIMAL       ");
				break;
			case SQL_DOUBLE:
				string += _T("SQL_DOUBLE        ");
				break;
			case SQL_FLOAT:
				string += _T("SQL_FLOAT         ");
				break;
			case SQL_INTEGER:
				string += _T("SQL_INTEGER       ");
				break;
			case SQL_LONGVARBINARY:
				string += _T("SQL_LONGVARBINARY ");
				break;
			case SQL_LONGVARCHAR:
				string += _T("SQL_LONGVARCHAR   ");
				break;
			case SQL_NUMERIC:
				string += _T("SQL_NUMERIC       ");
				break;
			case SQL_REAL:
				string += _T("SQL_REAL          ");
				break;
			case SQL_SMALLINT:
				string += _T("SQL_SMALLINT      ");
				break;
			case SQL_TIME:
				string += _T("SQL_TIME          ");
				break;
			case SQL_TIMESTAMP:
				string += _T("SQL_TIMESTAMP     ");
				break;
			case SQL_TINYINT:
				string += _T("SQL_TINYINT       ");
				break;
			case SQL_VARBINARY:
				string += _T("SQL_VARBINARY     ");
				break;
			case SQL_VARCHAR:
				string += _T("SQL_VARCHAR       ");
				break;
			default:
				string += _T("<unknown type>    ");
				break;
			}
			if (m_pGetTypeInfo->IsFieldNull(&(m_pGetTypeInfo->m_nPrecision)))
				string += _T("<n/a>       ");
			else
			{
				workString.Format(_T("%-11d "),m_pGetTypeInfo->m_nPrecision);
				string += workString;
			}
			if (m_pGetTypeInfo->IsFieldNull(&(m_pGetTypeInfo->m_nMinimumScale)))
				string += _T("<n/a>      ");
			else
			{
				workString.Format(_T("%-5d      "),m_pGetTypeInfo->m_nMinimumScale);
				string += workString;
			}
			if (m_pGetTypeInfo->IsFieldNull(&(m_pGetTypeInfo->m_nMaximumScale)))
			
				string += _T("<n/a>      \n");
			else
			{
				workString.Format(_T("%-5d\n"),m_pGetTypeInfo->m_nMaximumScale);
				string += workString;
			}

			file.WriteString(string);

			m_pGetTypeInfo->MoveNext();
		}
		file.WriteString(_T("\n"));
		m_pGetTypeInfo->Close();
		delete m_pGetTypeInfo;

		file.WriteString(_T("Native type         Nullable  Unsigned  Auto Inc  Money  Case Sensitive\n"));
		file.WriteString(_T("-----------         --------  --------  --------  -----  --------------\n"));
		m_pGetTypeInfo = new CGetTypeInfo(m_pDatabase);
		m_pGetTypeInfo->Open();

		while (!m_pGetTypeInfo->IsEOF())
		{
			string = m_pGetTypeInfo->m_strTypeName;
			while (string.GetLength() < 20)
				string += ' ';
			switch (m_pGetTypeInfo->m_fNullable)
			{
			case SQL_NO_NULLS:
				string += _T("No        ");
				break;
			case SQL_NULLABLE:
				string += _T("Yes       ");
				break;
			case SQL_NULLABLE_UNKNOWN:
			default:
				string += _T("Unknown   ");
				break;
			}

			if (m_pGetTypeInfo->IsFieldNull(&(m_pGetTypeInfo->m_fUnsignedAttribute)))
				string += _T("<n/a>     ");
			else if (m_pGetTypeInfo->m_fUnsignedAttribute)
				string += _T("Yes       ");
			else
				string += _T("No        ");
			
			if (m_pGetTypeInfo->IsFieldNull(&(m_pGetTypeInfo->m_fAutoIncrement)))
				string += _T("<n/a>     ");
			else if (m_pGetTypeInfo->m_fAutoIncrement)
				string += _T("Yes       ");
			else
				string += _T("No        ");

			if (m_pGetTypeInfo->m_bMoney)
				string += _T("Yes    ");
			else
				string += _T("No     ");

			if (m_pGetTypeInfo->m_bCaseSensitive)
				string += _T("Yes\n");
			else
				string += _T("No\n");

			file.WriteString(string);
			
			m_pGetTypeInfo->MoveNext();
		}
		file.WriteString(_T("\n"));
		m_pGetTypeInfo->Close();
		delete m_pGetTypeInfo;
		
		file.WriteString(_T("Native type         Lit. Prefix,Suffix  Create Params    Searchable\n"));
		file.WriteString(_T("-----------         ------------------  -------------    ----------\n"));
		m_pGetTypeInfo = new CGetTypeInfo(m_pDatabase);
		m_pGetTypeInfo->Open();

		while (!m_pGetTypeInfo->IsEOF())
		{
			string = m_pGetTypeInfo->m_strTypeName;
			while (string.GetLength() < 20)
				string += ' ';

			if (m_pGetTypeInfo->IsFieldNull(&(m_pGetTypeInfo->m_strLiteralPrefix)))
				workString = _T("<n/a>");
			else
				workString = m_pGetTypeInfo->m_strLiteralPrefix;
			workString += ',';
			if (m_pGetTypeInfo->IsFieldNull(&(m_pGetTypeInfo->m_strLiteralSuffix)))
				workString += _T("<n/a>");
			else
				workString += m_pGetTypeInfo->m_strLiteralSuffix;
			while (workString.GetLength() < 20)
				workString += ' ';
			string += workString;
			if (m_pGetTypeInfo->IsFieldNull(&(m_pGetTypeInfo->m_strCreateParams)))
				string += _T("<none>           ");
			else
			{
				workString = m_pGetTypeInfo->m_strCreateParams;
				while (workString.GetLength() < 17)
					workString += ' ';
				string += workString;
			}
			switch (m_pGetTypeInfo->m_fSearchable)
			{
			case SQL_UNSEARCHABLE:
				string += _T("No\n");
				break;
			case SQL_LIKE_ONLY:
				string += _T("Only with LIKE\n");
				break;
			case SQL_ALL_EXCEPT_LIKE:
				string += _T("with all except LIKE\n");
				break;
			case SQL_SEARCHABLE:
				string += _T("Yes\n");
				break;
			}

			file.WriteString(string);
			
			m_pGetTypeInfo->MoveNext();
		}
		file.WriteString(_T("\n"));
		m_pGetTypeInfo->Close();
		delete m_pGetTypeInfo;

		file.WriteString(_T("Native type         Convert() Conversions\n"));
		file.WriteString(_T("-----------         ---------------------\n"));
		m_pGetTypeInfo = new CGetTypeInfo(m_pDatabase);
		m_pGetTypeInfo->Open();

		while (!m_pGetTypeInfo->IsEOF())
		{
			string = m_pGetTypeInfo->m_strTypeName;
			while (string.GetLength() < 20)
				string += ' ';
			
			switch (m_pGetTypeInfo->m_fDataType)
			{
			case SQL_BIGINT:
				value = SQLGetInfoLong(SQL_CONVERT_BIGINT);
				break;
			case SQL_BINARY:
				value = SQLGetInfoLong(SQL_CONVERT_BINARY);
				break;
			case SQL_BIT:
				value = SQLGetInfoLong(SQL_CONVERT_BIT);
				break;
			case SQL_CHAR:
				value = SQLGetInfoLong(SQL_CONVERT_CHAR);
				break;
			case SQL_DATE:
				value = SQLGetInfoLong(SQL_CONVERT_DATE);
				break;
			case SQL_DECIMAL:
				value = SQLGetInfoLong(SQL_CONVERT_DECIMAL);
				break;
			case SQL_DOUBLE:
				value = SQLGetInfoLong(SQL_CONVERT_DOUBLE);
				break;
			case SQL_FLOAT:
				value = SQLGetInfoLong(SQL_CONVERT_FLOAT);
				break;
			case SQL_INTEGER:
				value = SQLGetInfoLong(SQL_CONVERT_INTEGER);
				break;
			case SQL_LONGVARBINARY:
				value = SQLGetInfoLong(SQL_CONVERT_LONGVARBINARY);
				break;
			case SQL_LONGVARCHAR:
				value = SQLGetInfoLong(SQL_CONVERT_LONGVARCHAR);
				break;
			case SQL_NUMERIC:
				value = SQLGetInfoLong(SQL_CONVERT_NUMERIC);
				break;
			case SQL_REAL:
				value = SQLGetInfoLong(SQL_CONVERT_REAL);
				break;
			case SQL_SMALLINT:
				value = SQLGetInfoLong(SQL_CONVERT_SMALLINT);
				break;
			case SQL_TIME:
				value = SQLGetInfoLong(SQL_CONVERT_TIME);
				break;
			case SQL_TIMESTAMP:
				value = SQLGetInfoLong(SQL_CONVERT_TIMESTAMP);
				break;
			case SQL_TINYINT:
				value = SQLGetInfoLong(SQL_CONVERT_TINYINT);
				break;
			case SQL_VARBINARY:
				value = SQLGetInfoLong(SQL_CONVERT_VARBINARY);
				break;
			case SQL_VARCHAR:
				value = SQLGetInfoLong(SQL_CONVERT_VARCHAR);
				break;
			default:
				value = 0;
				break;
			}
			if (!value)
				string += _T("<none>");
			else
			{
				BOOL	b = FALSE;
				PrintConversions(b,value,SQL_CVT_BIGINT,string,_T("SQL_BIGINT"),file);
				PrintConversions(b,value,SQL_CVT_BINARY,string,_T("SQL_BINARY"),file);
				PrintConversions(b,value,SQL_CVT_BIT,string,_T("SQL_BIT"),file);
				PrintConversions(b,value,SQL_CVT_CHAR,string,_T("SQL_CHAR"),file);
				PrintConversions(b,value,SQL_CVT_DATE,string,_T("SQL_DATE"),file);
				PrintConversions(b,value,SQL_CVT_DECIMAL,string,_T("SQL_DECIMAL"),file);
				PrintConversions(b,value,SQL_CVT_DOUBLE,string,_T("SQL_DOUBLE"),file);
				PrintConversions(b,value,SQL_CVT_FLOAT,string,_T("SQL_FLOAT"),file);
				PrintConversions(b,value,SQL_CVT_INTEGER,string,_T("SQL_INTEGER"),file);
				PrintConversions(b,value,SQL_CVT_LONGVARBINARY,string,_T("SQL_LONGVARBINARY"),file);
				PrintConversions(b,value,SQL_CVT_LONGVARCHAR,string,_T("SQL_LONGVARCHAR"),file);
				PrintConversions(b,value,SQL_CVT_NUMERIC,string,_T("SQL_NUMERIC"),file);
				PrintConversions(b,value,SQL_CVT_REAL,string,_T("SQL_REAL"),file);
				PrintConversions(b,value,SQL_CVT_SMALLINT,string,_T("SQL_SMALLINT"),file);
				PrintConversions(b,value,SQL_CVT_TIME,string,_T("SQL_TIME"),file);
				PrintConversions(b,value,SQL_CVT_TIMESTAMP,string,_T("SQL_TIMESTAMP"),file);
				PrintConversions(b,value,SQL_CVT_TINYINT,string,_T("SQL_TINYINT"),file);
				PrintConversions(b,value,SQL_CVT_VARBINARY,string,_T("SQL_VARBINARY"),file);
				PrintConversions(b,value,SQL_CVT_VARCHAR,string,_T("SQL_VARCHAR"),file);
			}
			string += _T("\n\n");

			file.WriteString(string);
			
			m_pGetTypeInfo->MoveNext();
		}
		file.WriteString(_T("\n"));
		m_pGetTypeInfo->Close();

		delete m_pGetTypeInfo;
		m_pGetTypeInfo = 0;
		file.WriteString(_T("\n"));

	}
}

void CDataTypes::PrintConversions(BOOL& flag,SDWORD value,SDWORD mask,
					  CString& string,const CString& strType,CStdioFile& file)
{
	CString	workString;
	if (value & mask)
	{
		if (string.GetLength() + strType.GetLength() > 75)
		{
			string += _T("\n");
			file.WriteString(string);
			string = _T("                    ") + strType;
		}
		else
		{
			if (flag)
				string += _T(",");
			string += strType;
		}
		flag = TRUE;
	}
}

void CDataTypes::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataTypes)
	DDX_Control(pDX, IDC_CONVERSIONS, m_lbConversions);
	DDX_Control(pDX, IDC_DATA_TYPE_LIST, m_lbDataTypes);
	DDX_Text(pDX, IDC_SQL_TYPE, m_strSqlType);
	DDX_Text(pDX, IDC_PRECISION, m_strPrecision);
	DDX_Text(pDX, IDD_PREFIX_SUFFIX, m_strPrefixSuffix);
	DDX_Text(pDX, IDD_CREATE_PARAMS, m_strCreateParams);
	DDX_Text(pDX, IDC_NULLABLE, m_strNullable);
	DDX_Text(pDX, IDC_CASE_SENSITIVE, m_strCaseSensitive);
	DDX_Text(pDX, IDC_SEARCHABLE, m_strSearchable);
	DDX_Text(pDX, IDC_MONEY, m_strMoney);
	DDX_Text(pDX, IDC_UNSIGNED, m_strUnsigned);
	DDX_Text(pDX, IDC_AUTO_INCREMENT, m_strAutoIncrement);
	DDX_Text(pDX, IDC_MAXIMUM_SCALE, m_strMaximumScale);
	DDX_Text(pDX, IDC_MINIMUM_SCALE, m_strMinimumScale);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataTypes, CPropertyPage)
	//{{AFX_MSG_MAP(CDataTypes)
	ON_LBN_SELCHANGE(IDC_DATA_TYPE_LIST, OnSelchangeDataTypeList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataTypes message handlers

void CDataTypes::OnNewDSN()
{
	if (IsWindow(m_hWnd) && m_pDatabase->IsOpen())
	{
		if (m_pGetTypeInfo)
		{
			if (m_pGetTypeInfo->IsOpen())
				m_pGetTypeInfo->Close();
			delete m_pGetTypeInfo;
		}

		// delete type data list
		TYPEDATA*	p1 = m_pTypeDataHead;
		TYPEDATA*	p2;
		while (p1)
		{
			p2 = p1;
			p1 = p1->pNext;
			delete p2;
		}
		m_pTypeDataHead = m_pTypeDataTail = 0;

		m_lbDataTypes.ResetContent();
		m_pGetTypeInfo = new CGetTypeInfo(m_pDatabase);
		m_pGetTypeInfo->Open();
		while (!m_pGetTypeInfo->IsEOF())
		{
			TYPEDATA*	pTypeData = new TYPEDATA;
			pTypeData->strTypeName = m_pGetTypeInfo->m_strTypeName;
			pTypeData->nSqlType = m_pGetTypeInfo->m_fDataType;
			if (m_pGetTypeInfo->IsFieldNull(&(m_pGetTypeInfo->m_nPrecision)))
				pTypeData->nPrecision = -1;
			else
				pTypeData->nPrecision = m_pGetTypeInfo->m_nPrecision;
			if (m_pGetTypeInfo->IsFieldNull(&(m_pGetTypeInfo->m_strLiteralPrefix)))
				pTypeData->strLiteralPrefix = "";
			else
				pTypeData->strLiteralPrefix = m_pGetTypeInfo->m_strLiteralPrefix;
			if (m_pGetTypeInfo->IsFieldNull(&(m_pGetTypeInfo->m_strLiteralSuffix)))
				pTypeData->strLiteralSuffix = "";
			else
				pTypeData->strLiteralSuffix = m_pGetTypeInfo->m_strLiteralSuffix;
			if (m_pGetTypeInfo->IsFieldNull(&(m_pGetTypeInfo->m_strCreateParams)))
				pTypeData->strCreateParams = "";
			else
				pTypeData->strCreateParams = m_pGetTypeInfo->m_strCreateParams;
			pTypeData->nNullable = m_pGetTypeInfo->m_fNullable;
			pTypeData->bCaseSensitive = m_pGetTypeInfo->m_bCaseSensitive;
			pTypeData->nSearchable = m_pGetTypeInfo->m_fSearchable;
			if (m_pGetTypeInfo->IsFieldNull(
				&(m_pGetTypeInfo->m_fUnsignedAttribute)))
			{
				pTypeData->nUnsigned = -1;
			}
			else if (m_pGetTypeInfo->m_fUnsignedAttribute)
				pTypeData->nUnsigned = 1;
			else
				pTypeData->nUnsigned = 0;
			pTypeData->bMoney = m_pGetTypeInfo->m_bMoney;
			if (m_pGetTypeInfo->IsFieldNull(
				&(m_pGetTypeInfo->m_fAutoIncrement)))
			{
				pTypeData->nAutoIncrement = -1;
			}
			else if (m_pGetTypeInfo->m_fAutoIncrement)
				pTypeData->nAutoIncrement = 1;
			else
				pTypeData->nAutoIncrement = 0;
			if (m_pGetTypeInfo->IsFieldNull(&(m_pGetTypeInfo->m_nMinimumScale)))
				pTypeData->nMinimumScale = -1;
			else
				pTypeData->nMinimumScale = m_pGetTypeInfo->m_nMinimumScale;
			if (m_pGetTypeInfo->IsFieldNull(&(m_pGetTypeInfo->m_nMaximumScale)))
				pTypeData->nMaximumScale = -1;
			else
				pTypeData->nMaximumScale = m_pGetTypeInfo->m_nMaximumScale;

			pTypeData->pNext = 0;
			InsertTypeData(pTypeData);
			m_lbDataTypes.AddString(m_pGetTypeInfo->m_strTypeName);
			m_pGetTypeInfo->MoveNext();
		}
		m_pGetTypeInfo->Close();
		delete m_pGetTypeInfo;
		m_pGetTypeInfo = 0;

		if (m_lbDataTypes.GetCount())
		{
			m_lbDataTypes.SetCurSel(0);
			OnSelchangeDataTypeList();
		}
	}
}

void CDataTypes::InsertTypeData(TYPEDATA* pTypeData)
{
	TYPEDATA*	p1 = m_pTypeDataTail;
	if (!p1)
		m_pTypeDataHead = m_pTypeDataTail = pTypeData;
	else
	{
		p1->pNext = pTypeData;
		m_pTypeDataTail = pTypeData;
	}
}

BOOL CDataTypes::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	OnNewDSN();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataTypes::OnSelchangeDataTypeList() 
{
	if (m_pTypeDataHead)
	{
		CString	string;
		m_lbDataTypes.GetText(m_lbDataTypes.GetCurSel(),string);

		TYPEDATA* pTypeData = m_pTypeDataHead;
		while (pTypeData)
		{
			if (pTypeData->strTypeName == string)
				break;
			pTypeData = pTypeData->pNext;
		}
		if (pTypeData)
		{
			// note: table lookup would be bettter here
			switch (pTypeData->nSqlType)
			{
			case SQL_BIGINT:
				m_strSqlType = _T("SQL_BIGINT");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_BIGINT));
				break;
			case SQL_BINARY:
				m_strSqlType = _T("SQL_BINARY");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_BINARY));
				break;
			case SQL_BIT:
				m_strSqlType = _T("SQL_BIT");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_BIT));
				break;
			case SQL_CHAR:
				m_strSqlType = _T("SQL_CHAR");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_CHAR));
				break;
			case SQL_DATE:
				m_strSqlType = _T("SQL_DATE");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_DATE));
				break;
			case SQL_DECIMAL:
				m_strSqlType = _T("SQL_DECIMAL");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_DECIMAL));
				break;
			case SQL_DOUBLE:
				m_strSqlType = _T("SQL_DOUBLE");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_DOUBLE));
				break;
			case SQL_FLOAT:
				m_strSqlType = _T("SQL_FLOAT");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_FLOAT));
				break;
			case SQL_INTEGER:
				m_strSqlType = _T("SQL_INTEGER");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_INTEGER));
				break;
			case SQL_LONGVARBINARY:
				m_strSqlType = _T("SQL_LONGVARBINARY");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_LONGVARBINARY));
				break;
			case SQL_LONGVARCHAR:
				m_strSqlType = _T("SQL_LONGVARCHAR");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_LONGVARCHAR));
				break;
			case SQL_NUMERIC:
				m_strSqlType = _T("SQL_NUMERIC");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_NUMERIC));
				break;
			case SQL_REAL:
				m_strSqlType = _T("SQL_REAL");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_REAL));
				break;
			case SQL_SMALLINT:
				m_strSqlType = _T("SQL_SMALLINT");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_SMALLINT));
				break;
			case SQL_TIME:
				m_strSqlType = _T("SQL_TIME");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_TIME));
				break;
			case SQL_TIMESTAMP:
				m_strSqlType = _T("SQL_TIMESTAMP");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_TIMESTAMP));
				break;
			case SQL_TINYINT:
				m_strSqlType = _T("SQL_TINYINT");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_TINYINT));
				break;
			case SQL_VARBINARY:
				m_strSqlType = _T("SQL_VARBINARY");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_VARBINARY));
				break;
			case SQL_VARCHAR:
				m_strSqlType = _T("SQL_VARCHAR");
				ShowConversions(SQLGetInfoLong(SQL_CONVERT_VARCHAR));
				break;
			default:
				m_strSqlType = _T("<new or driver-specific type>");
				ShowConversions(0);
				break;
			}
			if (pTypeData->nPrecision == -1)
				m_strPrecision = _T("<n/a>");
			else
				m_strPrecision.Format("%d",pTypeData->nPrecision);
			if (pTypeData->strLiteralPrefix == "")
				m_strPrefixSuffix = _T("<n/a>");
			else
				m_strPrefixSuffix = pTypeData->strLiteralPrefix;
			m_strPrefixSuffix += ",";
			if (pTypeData->strLiteralSuffix == "")
				m_strPrefixSuffix += _T("<n/a>");
			else
				m_strPrefixSuffix += pTypeData->strLiteralSuffix;
			if (pTypeData->strCreateParams == "")
				m_strCreateParams = _T("<none>");
			else
				m_strCreateParams = pTypeData->strCreateParams;
			switch (pTypeData->nNullable)
			{
			case SQL_NO_NULLS:
				m_strNullable = _T("No");
				break;
			case SQL_NULLABLE:
				m_strNullable = _T("Yes");
				break;
			case SQL_NULLABLE_UNKNOWN:
				m_strNullable = _T("Unknown");
				break;
			}
			if (pTypeData->bCaseSensitive)
				m_strCaseSensitive = _T("Yes");
			else
				m_strCaseSensitive = _T("No");

			switch (pTypeData->nSearchable)
			{
			case SQL_UNSEARCHABLE:
				m_strSearchable = _T("No");
				break;
			case SQL_LIKE_ONLY:
				m_strSearchable = _T("Only with LIKE");
				break;
			case SQL_ALL_EXCEPT_LIKE:
				m_strSearchable = _T("with all except LIKE");
				break;
			case SQL_SEARCHABLE:
				m_strSearchable = _T("Yes");
				break;
			}

			switch (pTypeData->nUnsigned)
			{
			case -1:
				m_strUnsigned = _T("<n/a>");
				break;
			case 0:
				m_strUnsigned = _T("No");
				break;
			case 1:
				m_strUnsigned = _T("Yes");
				break;
			}

			if (pTypeData->bMoney)
				m_strMoney = _T("Yes");
			else
				m_strMoney = _T("No");

			switch (pTypeData->nAutoIncrement)
			{
			case -1:
				m_strAutoIncrement = _T("<n/a>");
				break;
			case 0:
				m_strAutoIncrement = _T("No");
				break;
			case 1:
				m_strAutoIncrement = _T("Yes");
				break;
			}

			if (pTypeData->nMinimumScale == -1)
				m_strMinimumScale = _T("<n/a>");
			else
				m_strMinimumScale.Format(_T("%d"),pTypeData->nMinimumScale);

			if (pTypeData->nMaximumScale == -1)
				m_strMaximumScale = _T("<n/a>");
			else
				m_strMaximumScale.Format(_T("%d"),pTypeData->nMaximumScale);

		}
		else
			m_strSqlType = _T("<n/a>?");
	}
	else
		m_strSqlType = _T("<n/a>");

	UpdateData(FALSE);
}

void CDataTypes::ShowConversions(SDWORD dwDataType)
{
	m_lbConversions.ResetContent();
	if (!dwDataType)
		m_lbConversions.AddString(_T("<none>"));
	else
	{
		if (dwDataType & SQL_CVT_BIGINT)
			m_lbConversions.AddString(_T("SQL_BIGINT"));
		if (dwDataType & SQL_CVT_BINARY)
			m_lbConversions.AddString(_T("SQL_BINARY"));
		if (dwDataType & SQL_CVT_BIT)
			m_lbConversions.AddString(_T("SQL_BIT"));
		if (dwDataType & SQL_CVT_CHAR)
			m_lbConversions.AddString(_T("SQL_CHAR"));
		if (dwDataType & SQL_CVT_DATE)
			m_lbConversions.AddString(_T("SQL_DATE"));
		if (dwDataType & SQL_CVT_DECIMAL)
			m_lbConversions.AddString(_T("SQL_DECIMAL"));
		if (dwDataType & SQL_CVT_DOUBLE)
			m_lbConversions.AddString(_T("SQL_DOUBLE"));
		if (dwDataType & SQL_CVT_FLOAT)
			m_lbConversions.AddString(_T("SQL_FLOAT"));
		if (dwDataType & SQL_CVT_INTEGER)
			m_lbConversions.AddString(_T("SQL_INTEGER"));
		if (dwDataType & SQL_CVT_LONGVARBINARY)
			m_lbConversions.AddString(_T("SQL_LONGVARBINARY"));
		if (dwDataType & SQL_CVT_LONGVARCHAR)
			m_lbConversions.AddString(_T("SQL_LONGVARCHAR"));
		if (dwDataType & SQL_CVT_NUMERIC)
			m_lbConversions.AddString(_T("SQL_NUMERIC"));
		if (dwDataType & SQL_CVT_REAL)
			m_lbConversions.AddString(_T("SQL_REAL"));
		if (dwDataType & SQL_CVT_SMALLINT)
			m_lbConversions.AddString(_T("SQL_SMALLINT"));
		if (dwDataType & SQL_CVT_TIME)
			m_lbConversions.AddString(_T("SQL_TIME"));
		if (dwDataType & SQL_CVT_TIMESTAMP)
			m_lbConversions.AddString(_T("SQL_TIMESTAMP"));
		if (dwDataType & SQL_CVT_TINYINT)
			m_lbConversions.AddString(_T("SQL_TINYINT"));
		if (dwDataType & SQL_CVT_VARBINARY)
			m_lbConversions.AddString(_T("SQL_VARBINARY"));
		if (dwDataType & SQL_CVT_VARCHAR)
			m_lbConversions.AddString(_T("SQL_VARCHAR"));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CIdentifiers property page

IMPLEMENT_DYNCREATE(CIdentifiers, CPropertyPage)

CIdentifiers::CIdentifiers(CDatabase* pDatabase)
 : CMyPage(pDatabase,CIdentifiers::IDD)
{
	//{{AFX_DATA_INIT(CIdentifiers)
	m_strSqlIdentifierCase = _T("");
	m_strSqlIdentifierQuoteChar = _T("");
	m_strSqlOwnerTerm = _T("");
	m_strSqlProcedureTerm = _T("");
	m_strSqlQualifierLocation = _T("");
	m_strSqlQualifierNameSeparator = _T("");
	m_strSqlQualifierTerm = _T("");
	m_strSqlQuotedIdentifierCase = _T("");
	m_strSqlSearchPatternEscape = _T("");
	m_strSqlTableTerm = _T("");
	m_strSqlSpecialCharacters = _T("");
	//}}AFX_DATA_INIT

	OnNewDSN();
}

CIdentifiers::~CIdentifiers()
{
}

void CIdentifiers::DumpToFile(CStdioFile& file)
{
	if (m_pDatabase->IsOpen())
	{
		CString	bufferString;
		CString	string;
		CString	workString;

		file.WriteString(_T("Identifiers\n-----------\n\n"));
		file.WriteString(_T("Driver-Specific Keywords:\n-------------------------\n"));

		char	buffer[32000];
		SWORD	cbData;
		::SQLGetInfo(m_pDatabase->m_hdbc,SQL_KEYWORDS,
			(PTR)buffer,32000,&cbData);
		bufferString = buffer;
		int		n;
		string = "";
		BOOL	b = FALSE;
		while ((n = bufferString.Find(',')) != -1)
		{
			workString = bufferString.Left(n);
			bufferString = bufferString.Right(bufferString.GetLength() - n - 1);
			if (string.GetLength() + workString.GetLength() > 75)
			{
				string += _T("\n");
				file.WriteString(string);
				string = workString;
			}
			else
			{
				if (b)
					string += _T(",");
				string += workString;
			}
			b = TRUE;
		}
		string += _T("\n\n");
		file.WriteString(string);

		string.Format(_T("          Identifier Case - %s\n"),m_strSqlIdentifierCase);
		file.WriteString(string);
		string.Format(_T("   Quoted Identifier Case - %s\n"),m_strSqlQuotedIdentifierCase);
		file.WriteString(string);
		string.Format(_T("    Search Pattern Escape - %s\n"),m_strSqlSearchPatternEscape);
		file.WriteString(string);
		string.Format(_T("    Identifier Quote Char - %s\n"),m_strSqlIdentifierQuoteChar);
		file.WriteString(string);
		string.Format(_T("       Qualifier Location - %s\n"),m_strSqlQualifierLocation);
		file.WriteString(string);
		string.Format(_T("      Qualifier Separator - %s\n"),m_strSqlQualifierNameSeparator);
		file.WriteString(string);
		string.Format(_T("               Owner Term - %s\n"),m_strSqlOwnerTerm);
		file.WriteString(string);
		string.Format(_T("           Procedure Term - %s\n"),m_strSqlProcedureTerm);
		file.WriteString(string);
		string.Format(_T("           Qualifier Term - %s\n"),m_strSqlQualifierTerm);
		file.WriteString(string);
		string.Format(_T("                TableTerm - %s\n\n"),m_strSqlTableTerm);
		file.WriteString(string);

		file.WriteString(_T("Special Characters:\n-------------------\n"));
		if (m_strSqlSpecialCharacters.GetLength() == 0)
			file.WriteString(_T("<none>\n\n"));
		else
		{
			string = m_strSqlSpecialCharacters;
			while (string.GetLength() > 75)
			{
				workString = string.Left(75);
				string = string.Right(string.GetLength() - n - 1);
				workString += _T("\n");
				file.WriteString(workString);
			}
			string += _T("\n");
			file.WriteString(string);
		}
		file.WriteString(_T("\n"));
	}
}

void CIdentifiers::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIdentifiers)
	DDX_Control(pDX, IDC_DRIVER_KEYWORDS, m_lbDriverKeywords);
	DDX_Control(pDX, IDC_ODBC_KEYWORDS, m_lbOdbcKeywords);
	DDX_Text(pDX, IDC_SQL_IDENTIFIER_CASE, m_strSqlIdentifierCase);
	DDX_Text(pDX, IDC_SQL_IDENTIFIER_QUOTE_CHAR, m_strSqlIdentifierQuoteChar);
	DDX_Text(pDX, IDC_SQL_OWNER_TERM, m_strSqlOwnerTerm);
	DDX_Text(pDX, IDC_SQL_PROCEDURE_TERM, m_strSqlProcedureTerm);
	DDX_Text(pDX, IDC_SQL_QUALIFIER_LOCATION, m_strSqlQualifierLocation);
	DDX_Text(pDX, IDC_SQL_QUALIFIER_NAME_SEPARATOR, m_strSqlQualifierNameSeparator);
	DDX_Text(pDX, IDC_SQL_QUALIFIER_TERM, m_strSqlQualifierTerm);
	DDX_Text(pDX, IDC_SQL_QUOTED_IDENTIFIER_CASE, m_strSqlQuotedIdentifierCase);
	DDX_Text(pDX, IDC_SQL_SEARCH_PATTERN_ESCAPE, m_strSqlSearchPatternEscape);
	DDX_Text(pDX, IDC_SQL_TABLE_TERM, m_strSqlTableTerm);
	DDX_Text(pDX, IDC_SQL_SPECIAL_CHARACTERS, m_strSqlSpecialCharacters);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIdentifiers, CPropertyPage)
	//{{AFX_MSG_MAP(CIdentifiers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIdentifiers message handlers

void CIdentifiers::OnNewDSN()
{
	if (m_pDatabase->IsOpen())
	{
		switch (SQLGetInfoShort(SQL_IDENTIFIER_CASE))
		{
		case SQL_IC_UPPER:
			m_strSqlIdentifierCase = _T("Upper");
			break;
		case SQL_IC_LOWER:
			m_strSqlIdentifierCase = _T("Lower");
			break;
		case SQL_IC_SENSITIVE:
			m_strSqlIdentifierCase = _T("Sensitive");
			break;
		case SQL_IC_MIXED:
			m_strSqlIdentifierCase = _T("Mixed");
			break;
		}
		m_strSqlIdentifierQuoteChar = SQLGetInfoString(SQL_IDENTIFIER_QUOTE_CHAR);
		m_strSqlOwnerTerm = SQLGetInfoString(SQL_OWNER_TERM);
		m_strSqlProcedureTerm = SQLGetInfoString(SQL_PROCEDURE_TERM);
		switch (SQLGetInfoShort(SQL_QUALIFIER_LOCATION))
		{
		case SQL_QL_START:
			m_strSqlQualifierLocation = _T("Start");
			break;
		case SQL_QL_END:
			m_strSqlQualifierLocation = _T("End");
			break;
		}
		m_strSqlQualifierNameSeparator = SQLGetInfoString(SQL_QUALIFIER_NAME_SEPARATOR);
		m_strSqlQualifierTerm = SQLGetInfoString(SQL_QUALIFIER_TERM);
		switch (SQLGetInfoShort(SQL_QUOTED_IDENTIFIER_CASE))
		{
		case SQL_IC_UPPER:
			m_strSqlQuotedIdentifierCase = _T("Upper");
			break;
		case SQL_IC_LOWER:
			m_strSqlQuotedIdentifierCase = _T("Lower");
			break;
		case SQL_IC_SENSITIVE:
			m_strSqlQuotedIdentifierCase = _T("Sensitive");
			break;
		case SQL_IC_MIXED:
			m_strSqlQuotedIdentifierCase = _T("Mixed");
			break;
		}
		m_strSqlSearchPatternEscape = SQLGetInfoString(SQL_SEARCH_PATTERN_ESCAPE);
		m_strSqlSpecialCharacters = SQLGetInfoString(SQL_SPECIAL_CHARACTERS);
		m_strSqlTableTerm = SQLGetInfoString(SQL_TABLE_TERM);
		if (IsWindow(m_hWnd))
		{
			char	buffer[32000];
			SWORD	cbData;
			::SQLGetInfo(m_pDatabase->m_hdbc,SQL_KEYWORDS,
				(PTR)buffer,32000,&cbData);
			CString	string = buffer;
			int		n;
			m_lbDriverKeywords.ResetContent();
			while ((n = string.Find(',')) != -1)
			{
				m_lbDriverKeywords.AddString(string.Left(n));
				string = string.Right(string.GetLength() - n - 1);
			}

			UpdateData(FALSE);
		}
	}
}

BOOL CIdentifiers::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_lbOdbcKeywords.ResetContent();
	CString string = SQL_ODBC_KEYWORDS;
	int		n;
	while ((n = string.Find(',')) != -1)
	{
		m_lbOdbcKeywords.AddString(string.Left(n));
		string = string.Right(string.GetLength() - n - 1);
	}

	char	buffer[32000];
	SWORD	cbData;
	::SQLGetInfo(m_pDatabase->m_hdbc,SQL_KEYWORDS,
		(PTR)buffer,32000,&cbData);
	string = buffer;
	m_lbDriverKeywords.ResetContent();
	while ((n = string.Find(',')) != -1)
	{
		m_lbDriverKeywords.AddString(string.Left(n));
		string = string.Right(string.GetLength() - n - 1);
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CLimits property page

#define	NUM_INTERVALS	9

IMPLEMENT_DYNCREATE(CLimits, CPropertyPage)

UWORD CLimits::m_nIntervalMasks[] =
{
	SQL_FN_TSI_FRAC_SECOND,
	SQL_FN_TSI_SECOND,
	SQL_FN_TSI_MINUTE,
	SQL_FN_TSI_HOUR,
	SQL_FN_TSI_DAY,
	SQL_FN_TSI_WEEK,
	SQL_FN_TSI_MONTH,
	SQL_FN_TSI_QUARTER,
	SQL_FN_TSI_YEAR
};

char* CLimits::m_szIntervalNames[] =
{
	_T("Second Fraction"),
	_T("Second"),
	_T("Minute"),
	_T("Hour"),
	_T("Day"),
	_T("Week"),
	_T("Month"),
	_T("Quarter"),
	_T("Year")
};

CLimits::CLimits(CDatabase* pDatabase)
 : CMyPage(pDatabase,CLimits::IDD)
{
	//{{AFX_DATA_INIT(CLimits)
	m_strSqlMaxBinaryLiteralLen = _T("");
	m_strSqlMaxCharLiteralLen = _T("");
	m_strSqlMaxColumnNameLen = _T("");
	m_strSqlMaxColumnsInGroupBy = _T("");
	m_strSqlMaxColumnsInIndex = _T("");
	m_strSqlMaxColumnsInOrderBy = _T("");
	m_strSqlMaxColumnsInSelect = _T("");
	m_strSqlMaxColumnsInTable = _T("");
	m_strSqlMaxCursorNameLen = _T("");
	m_strSqlMaxIndexSize = _T("");
	m_strSqlMaxOwnerNameLen = _T("");
	m_strSqlMaxProcedureNameLen = _T("");
	m_strSqlMaxQualifierNameLen = _T("");
	m_strSqlMaxRowSize = _T("");
	m_strSqlMaxRowSizeIncludesLong = _T("");
	m_strSqlMaxStatementLen = _T("");
	m_strSqlMaxTableNameLen = _T("");
	m_strSqlMaxTablesInSelect = _T("");
	m_strSqlMaxUserNameLen = _T("");
	//}}AFX_DATA_INIT

	OnNewDSN();
}

CLimits::~CLimits()
{
}

void CLimits::DumpToFile(CStdioFile& file)
{
	if (m_pDatabase->IsOpen())
	{
		CString	string;
		BOOL	b;

		file.WriteString(_T("Limits\n------\n\n"));
		string.Format(_T("Max Binary Literal Length - %s\n"),m_strSqlMaxBinaryLiteralLen);
		file.WriteString(string);
		string.Format(_T("  Max Char Literal Length - %s\n"),m_strSqlMaxCharLiteralLen);
		file.WriteString(string);
		string.Format(_T("   Max Column Name Length - %s\n"),m_strSqlMaxColumnNameLen);
		file.WriteString(string);
		string.Format(_T("  Max Columns in GROUP BY - %s\n"),m_strSqlMaxColumnsInGroupBy);
		file.WriteString(string);
		string.Format(_T("     Max Columns in Index - %s\n"),m_strSqlMaxColumnsInIndex);
		file.WriteString(string);
		string.Format(_T("  Max Columns in ORDER BY - %s\n"),m_strSqlMaxColumnsInOrderBy);
		file.WriteString(string);
		string.Format(_T("    Max Columns in SELECT - %s\n"),m_strSqlMaxColumnsInSelect);
		file.WriteString(string);
		string.Format(_T("     Max Columns in Table - %s\n"),m_strSqlMaxColumnsInTable);
		file.WriteString(string);
		string.Format(_T("   Max Cursor Name Length - %s\n"),m_strSqlMaxCursorNameLen);
		file.WriteString(string);
		string.Format(_T("           Max Index Size - %s\n"),m_strSqlMaxIndexSize);
		file.WriteString(string);
		string.Format(_T("    Max Owner Name Length - %s\n"),m_strSqlMaxOwnerNameLen);
		file.WriteString(string);
		string.Format(_T("Max Procedure Name Length - %s\n"),m_strSqlMaxProcedureNameLen);
		file.WriteString(string);
		string.Format(_T("Max Qualifier Name Length - %s\n"),m_strSqlMaxQualifierNameLen);
		file.WriteString(string);
		string.Format(_T("             Max Row Size - %s\n"),m_strSqlMaxRowSize);
		file.WriteString(string);
		string.Format(_T("Max Rowsize Includes Long - %s\n"),m_strSqlMaxRowSizeIncludesLong);
		file.WriteString(string);
		string.Format(_T("     Max Statement Length - %s\n"),m_strSqlMaxStatementLen);
		file.WriteString(string);
		string.Format(_T("    Max Table Name Length - %s\n"),m_strSqlMaxTableNameLen);
		file.WriteString(string);
		string.Format(_T("     Max Tables In SELECT - %s\n"),m_strSqlMaxTablesInSelect);
		file.WriteString(string);
		string.Format(_T("     Max User Name Length - %s\n\n"),m_strSqlMaxUserNameLen);
		file.WriteString(string);

		if (!(SQLGetInfoLong(SQL_TIMEDATE_FUNCTIONS) & SQL_FN_TD_TIMESTAMPADD))
			file.WriteString(_T("Timestamp Add Intervals: <n/a>\n\n"));
		else
		{
			file.WriteString(_T("Timestamp Add Intervals:\n"));
			UDWORD	fAddIntervals = SQLGetInfoLong(SQL_TIMEDATE_ADD_INTERVALS);
			b = FALSE;
			for (int i = 0; i < NUM_INTERVALS; i++)
			{
				if (m_nIntervalMasks[i] & fAddIntervals)
				{
					if (b)
						file.WriteString(_T(","));
					file.WriteString(m_szIntervalNames[i]);
					b = TRUE;
				}
			}
			file.WriteString(_T("\n\n"));
		}
		if (!(SQLGetInfoLong(SQL_TIMEDATE_FUNCTIONS) & SQL_FN_TD_TIMESTAMPDIFF))
			file.WriteString(_T("Timestamp Diff Intervals: <n/a>\n\n"));
		else
		{
			file.WriteString(_T("Timestamp Diff Intervals:\n"));
			UDWORD	fDiffIntervals = SQLGetInfoLong(SQL_TIMEDATE_DIFF_INTERVALS);
			b = FALSE;
			for (int i = 0; i < NUM_INTERVALS; i++)
			{
				if (m_nIntervalMasks[i] & fDiffIntervals)
				{
					if (b)
						file.WriteString(_T(","));
					file.WriteString(m_szIntervalNames[i]);
					b = TRUE;
				}
			}
			file.WriteString(_T("\n\n"));
		}

		file.WriteString(_T("\n"));
	}
}

void CLimits::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLimits)
	DDX_Control(pDX, IDC_SQL_TIMEDATE_DIFF_INTERVALS, m_lcSqlTimedateDiffIntervals);
	DDX_Control(pDX, IDC_SQL_TIMEDATE_ADD_INTERVALS, m_lcSqlTimedateAddIntervals);
	DDX_Text(pDX, IDC_SQL_MAX_BINARY_LITERAL_LEN, m_strSqlMaxBinaryLiteralLen);
	DDX_Text(pDX, IDC_SQL_MAX_CHAR_LITERAL_LEN, m_strSqlMaxCharLiteralLen);
	DDX_Text(pDX, IDC_SQL_MAX_COLUMN_NAME_LEN, m_strSqlMaxColumnNameLen);
	DDX_Text(pDX, IDC_SQL_MAX_COLUMNS_IN_GROUP_BY, m_strSqlMaxColumnsInGroupBy);
	DDX_Text(pDX, IDC_SQL_MAX_COLUMNS_IN_INDEX, m_strSqlMaxColumnsInIndex);
	DDX_Text(pDX, IDC_SQL_MAX_COLUMNS_IN_ORDER_BY, m_strSqlMaxColumnsInOrderBy);
	DDX_Text(pDX, IDC_SQL_MAX_COLUMNS_IN_SELECT, m_strSqlMaxColumnsInSelect);
	DDX_Text(pDX, IDC_SQL_MAX_COLUMNS_IN_TABLE, m_strSqlMaxColumnsInTable);
	DDX_Text(pDX, IDC_SQL_MAX_CURS0R_NAME_LEN, m_strSqlMaxCursorNameLen);
	DDX_Text(pDX, IDC_SQL_MAX_INDEX_SIZE, m_strSqlMaxIndexSize);
	DDX_Text(pDX, IDC_SQL_MAX_OWNER_NAME_LEN, m_strSqlMaxOwnerNameLen);
	DDX_Text(pDX, IDC_SQL_MAX_PROCEDURE_NAME_LEN, m_strSqlMaxProcedureNameLen);
	DDX_Text(pDX, IDC_SQL_MAX_QUALIFIER_NAME_LEN, m_strSqlMaxQualifierNameLen);
	DDX_Text(pDX, IDC_SQL_MAX_ROW_SIZE, m_strSqlMaxRowSize);
	DDX_Text(pDX, IDC_SQL_MAX_ROW_SIZE_INCLUDES_LONG, m_strSqlMaxRowSizeIncludesLong);
	DDX_Text(pDX, IDC_SQL_MAX_STATEMENT_LEN, m_strSqlMaxStatementLen);
	DDX_Text(pDX, IDC_SQL_MAX_TABLE_NAME_LEN, m_strSqlMaxTableNameLen);
	DDX_Text(pDX, IDC_SQL_MAX_TABLES_IN_SELECT, m_strSqlMaxTablesInSelect);
	DDX_Text(pDX, IDC_SQL_MAX_USER_NAME_LEN, m_strSqlMaxUserNameLen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLimits, CPropertyPage)
	//{{AFX_MSG_MAP(CLimits)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLimits message handlers

void CLimits::OnNewDSN()
{
	if (m_pDatabase->IsOpen())
	{
		ZeroIsNoneLong(m_strSqlMaxBinaryLiteralLen,SQL_MAX_BINARY_LITERAL_LEN);
		ZeroIsNoneLong(m_strSqlMaxCharLiteralLen,SQL_MAX_CHAR_LITERAL_LEN);
		ZeroIsNoneShort(m_strSqlMaxColumnNameLen,SQL_MAX_COLUMN_NAME_LEN);
		ZeroIsNoneShort(m_strSqlMaxColumnsInGroupBy,SQL_MAX_COLUMNS_IN_GROUP_BY);
		ZeroIsNoneShort(m_strSqlMaxColumnsInIndex,SQL_MAX_COLUMNS_IN_INDEX);
		ZeroIsNoneShort(m_strSqlMaxColumnsInOrderBy,SQL_MAX_COLUMNS_IN_ORDER_BY);
		ZeroIsNoneShort(m_strSqlMaxColumnsInSelect,SQL_MAX_COLUMNS_IN_SELECT);
		ZeroIsNoneShort(m_strSqlMaxColumnsInTable,SQL_MAX_COLUMNS_IN_TABLE);
		ZeroIsNoneShort(m_strSqlMaxCursorNameLen,SQL_MAX_CURSOR_NAME_LEN);
		ZeroIsNoneLong(m_strSqlMaxIndexSize,SQL_MAX_INDEX_SIZE);
		ZeroIsNoneShort(m_strSqlMaxOwnerNameLen,SQL_MAX_OWNER_NAME_LEN);
		ZeroIsNoneShort(m_strSqlMaxProcedureNameLen,SQL_MAX_PROCEDURE_NAME_LEN);
		ZeroIsNoneShort(m_strSqlMaxQualifierNameLen,SQL_MAX_QUALIFIER_NAME_LEN);
		ZeroIsNoneLong(m_strSqlMaxRowSize,SQL_MAX_ROW_SIZE);
		YIsYes(m_strSqlMaxRowSizeIncludesLong,SQL_MAX_ROW_SIZE_INCLUDES_LONG);
		ZeroIsNoneLong(m_strSqlMaxStatementLen,SQL_MAX_STATEMENT_LEN);
		ZeroIsNoneShort(m_strSqlMaxTableNameLen,SQL_MAX_TABLE_NAME_LEN);
		ZeroIsNoneShort(m_strSqlMaxTablesInSelect,SQL_MAX_TABLES_IN_SELECT);
		ZeroIsNoneShort(m_strSqlMaxUserNameLen,SQL_MAX_USER_NAME_LEN);
	
		if (IsWindow(m_hWnd))
		{
			UDWORD	fAddIntervals = SQLGetInfoLong(SQL_TIMEDATE_ADD_INTERVALS);
			UDWORD	fDiffIntervals = SQLGetInfoLong(SQL_TIMEDATE_DIFF_INTERVALS);

			m_lcSqlTimedateAddIntervals.DeleteAllItems();
			if (!(SQLGetInfoLong(SQL_TIMEDATE_FUNCTIONS) & SQL_FN_TD_TIMESTAMPADD))
				m_lcSqlTimedateAddIntervals.InsertItem(0,_T("<n/a>"));
			else
			{
				for (int i = 0; i < NUM_INTERVALS; i++)
				{
					int	nCheck = 0;
					if (m_nIntervalMasks[i] & fAddIntervals)
						nCheck = 1;
					m_lcSqlTimedateAddIntervals.InsertItem(
						i,m_szIntervalNames[i],nCheck);
				}
			}

			m_lcSqlTimedateDiffIntervals.DeleteAllItems();
			if (!(SQLGetInfoLong(SQL_TIMEDATE_FUNCTIONS) & SQL_FN_TD_TIMESTAMPDIFF))
				m_lcSqlTimedateDiffIntervals.InsertItem(0,_T("<n/a>"));
			else
			{
				for (int i = 0; i < NUM_INTERVALS; i++)
				{
					int	nCheck = 0;
					if (m_nIntervalMasks[i] & fDiffIntervals)
						nCheck = 1;
					m_lcSqlTimedateDiffIntervals.InsertItem(
						i,m_szIntervalNames[i],nCheck);
				}
			}

			UpdateData(FALSE);
		}
	}
}

BOOL CLimits::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_lcSqlTimedateDiffIntervals.SetImageList(m_pImageList,LVSIL_SMALL);
	m_lcSqlTimedateAddIntervals.SetImageList(m_pImageList,LVSIL_SMALL);

	m_lcSqlTimedateDiffIntervals.InsertColumn(0,"",LVCFMT_LEFT,100);
	m_lcSqlTimedateAddIntervals.InsertColumn(0,"",LVCFMT_LEFT,100);

	UDWORD	fAddIntervals = SQLGetInfoLong(SQL_TIMEDATE_ADD_INTERVALS);
	UDWORD	fDiffIntervals = SQLGetInfoLong(SQL_TIMEDATE_DIFF_INTERVALS);

	m_lcSqlTimedateAddIntervals.DeleteAllItems();
	if (!(SQLGetInfoLong(SQL_TIMEDATE_FUNCTIONS) & SQL_FN_TD_TIMESTAMPADD))
		m_lcSqlTimedateAddIntervals.InsertItem(0,_T("<n/a>"));
	else
	{
		for (int i = 0; i < NUM_INTERVALS; i++)
		{
			int	nCheck = 0;
			if (m_nIntervalMasks[i] & fAddIntervals)
				nCheck = 1;
			m_lcSqlTimedateAddIntervals.InsertItem(
				i,m_szIntervalNames[i],nCheck);
		}
	}

	m_lcSqlTimedateDiffIntervals.DeleteAllItems();
	if (!(SQLGetInfoLong(SQL_TIMEDATE_FUNCTIONS) & SQL_FN_TD_TIMESTAMPDIFF))
		m_lcSqlTimedateDiffIntervals.InsertItem(0,_T("<n/a>"));
	else
	{
		for (int i = 0; i < NUM_INTERVALS; i++)
		{
			int	nCheck = 0;
			if (m_nIntervalMasks[i] & fDiffIntervals)
				nCheck = 1;
			m_lcSqlTimedateDiffIntervals.InsertItem(
				i,m_szIntervalNames[i],nCheck);
		}
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CMisc1 property page

IMPLEMENT_DYNCREATE(CMisc1, CPropertyPage)

CMisc1::CMisc1(CDatabase* pDatabase)
 : CMyPage(pDatabase,CMisc1::IDD)
{
	//{{AFX_DATA_INIT(CMisc1)
	m_strSqlTxnCapable = _T("");
	m_strSqlCursorCommitBehavior = _T("");
	m_strSqlCursorRollbackBehavior = _T("");
	m_bSqlTxnReadCommitted = FALSE;
	m_bSqlTxnReadUncommitted = FALSE;
	m_bSqlTxnRepeatableRead = FALSE;
	m_bSqlTxnSerializable = FALSE;
	m_bSqlTxnVersioning = FALSE;
	m_strSqlDefaultTxnIsolation = _T("");
	m_strSqlMultipleActiveTxn = _T("");
	m_bSqlBpClose = FALSE;
	m_bSqlBpDelete = FALSE;
	m_bSqlBpDrop = FALSE;
	m_bSqlBpOtherHstmt = FALSE;
	m_bSqlBpScroll = FALSE;
	m_bSqlBpTransaction = FALSE;
	m_bSqlBpUpdate = FALSE;
	m_bSqlLckExclusive = FALSE;
	m_bSqlLckNoChange = FALSE;
	m_bSqlLckUnlock = FALSE;
	m_bSqlSsAdditions = FALSE;
	m_bSqlSsDeletions = FALSE;
	m_bSqlSsUpdates = FALSE;
	//}}AFX_DATA_INIT

	OnNewDSN();
}

CMisc1::~CMisc1()
{
}

void CMisc1::DumpToFile(CStdioFile& file)
{
	if (m_pDatabase->IsOpen())
	{
		CString	string;
		CString	workString;
		BOOL	b;

		file.WriteString(_T("Miscellaneous\n-------------\n\n"));

		string.Format(_T(" Transaction Capabilities - %s\n"),m_strSqlTxnCapable);
		file.WriteString(string);
		string.Format(_T("   Cursor Commit Behavior - %s\n"),m_strSqlCursorCommitBehavior);
		file.WriteString(string);
		string.Format(_T(" Cursor Rollback Behavior - %s\n"),m_strSqlCursorRollbackBehavior);
		file.WriteString(string);
		file.WriteString(_T("  Trans. Isolation Levels - "));
		b = FALSE;
		if (m_bSqlTxnReadCommitted)
		{
			file.WriteString(_T("Read Commit"));
			b = TRUE;
		}
		if (m_bSqlTxnReadUncommitted)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Read Uncommit"));
			b = TRUE;
		}
		if (m_bSqlTxnRepeatableRead)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Repeat Read"));
			b = TRUE;
		}
		if (m_bSqlTxnSerializable)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Serializable"));
			b = TRUE;
		}
		if (m_bSqlTxnVersioning)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Versioning"));
			b = TRUE;
		}
		if (!b)
			file.WriteString(_T("<none>"));
		file.WriteString(_T("\n"));
		string.Format(_T(" Default Trans. Isolation - %s\n"),m_strSqlDefaultTxnIsolation);
		file.WriteString(string);
		string.Format(_T("   Multiple Active Trans. - %s\n"),m_strSqlMultipleActiveTxn);
		file.WriteString(string);
		file.WriteString(_T("     Bookmark Persistance - "));
		b = FALSE;
		if (m_bSqlBpClose)
		{
			file.WriteString(_T("Close"));
			b = TRUE;
		}
		if (m_bSqlBpDelete)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Delete"));
			b = TRUE;
		}
		if (m_bSqlBpDrop)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Drop"));
			b = TRUE;
		}
		if (m_bSqlBpOtherHstmt)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Other HSTMT"));
			b = TRUE;
		}
		if (m_bSqlBpScroll)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Scroll"));
			b = TRUE;
		}
		if (m_bSqlBpTransaction)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Transaction"));
			b = TRUE;
		}
		if (m_bSqlBpUpdate)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Update"));
			b = TRUE;
		}
		if (!b)
			file.WriteString(_T("<none>"));
		file.WriteString(_T("\n"));

		file.WriteString(_T("               Lock Types - "));
		b = FALSE;
		if (m_bSqlLckExclusive)
		{
			file.WriteString(_T("Exclusive"));
			b = TRUE;
		}
		if (m_bSqlLckNoChange)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("No Change"));
			b = TRUE;
		}
		if (m_bSqlLckUnlock)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Unlock"));
			b = TRUE;
		}
		if (!b)
			file.WriteString(_T("<none>"));
		file.WriteString(_T("\n"));

		file.WriteString(_T("       Static Sensitivity - "));
		b = FALSE;
		if (m_bSqlSsAdditions)
		{
			file.WriteString(_T("Additions"));
			b = TRUE;
		}
		if (m_bSqlSsDeletions)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Deletions"));
			b = TRUE;
		}
		if (m_bSqlSsUpdates)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Updates"));
			b = TRUE;
		}
		if (!b)
			file.WriteString(_T("<none>"));
		file.WriteString(_T("\n\n"));
	}
}

void CMisc1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMisc1)
	DDX_Text(pDX, IDC_SQL_TXN_CAPABLE, m_strSqlTxnCapable);
	DDX_Text(pDX, IDC_SQL_CURSOR_COMMIT_BEHAVIOR, m_strSqlCursorCommitBehavior);
	DDX_Text(pDX, IDC_SQL_CURSOR_ROLLBACK_BEHAVIOR, m_strSqlCursorRollbackBehavior);
	DDX_Check(pDX, IDC_SQL_TXN_READ_COMMITTED, m_bSqlTxnReadCommitted);
	DDX_Check(pDX, IDC_SQL_TXN_READ_UNCOMMITTED, m_bSqlTxnReadUncommitted);
	DDX_Check(pDX, IDC_SQL_TXN_REPEATABLE_READ, m_bSqlTxnRepeatableRead);
	DDX_Check(pDX, IDC_SQL_TXN_SERIALIZABLE, m_bSqlTxnSerializable);
	DDX_Check(pDX, IDC_SQL_TXN_VERSIONING, m_bSqlTxnVersioning);
	DDX_Text(pDX, IDC_SQL_DEFAULT_TXN_ISOLATION, m_strSqlDefaultTxnIsolation);
	DDX_Text(pDX, IDC_SQL_MULTIPLE_ACTIVE_TXN, m_strSqlMultipleActiveTxn);
	DDX_Check(pDX, IDC_SQL_BP_CLOSE, m_bSqlBpClose);
	DDX_Check(pDX, IDC_SQL_BP_DELETE, m_bSqlBpDelete);
	DDX_Check(pDX, IDC_SQL_BP_DROP, m_bSqlBpDrop);
	DDX_Check(pDX, IDC_SQL_BP_OTHER_HSTMT, m_bSqlBpOtherHstmt);
	DDX_Check(pDX, IDC_SQL_BP_SCROLL, m_bSqlBpScroll);
	DDX_Check(pDX, IDC_SQL_BP_TRANSACTION, m_bSqlBpTransaction);
	DDX_Check(pDX, IDC_SQL_BP_UPDATE, m_bSqlBpUpdate);
	DDX_Check(pDX, IDC_SQL_LCK_EXCLUSIVE, m_bSqlLckExclusive);
	DDX_Check(pDX, IDC_SQL_LCK_NO_CHANGE, m_bSqlLckNoChange);
	DDX_Check(pDX, IDC_SQL_LCK_UNLOCK, m_bSqlLckUnlock);
	DDX_Check(pDX, IDC_SQL_SS_ADDITIONS, m_bSqlSsAdditions);
	DDX_Check(pDX, IDC_SQL_SS_DELETIONS, m_bSqlSsDeletions);
	DDX_Check(pDX, IDC_SQL_SS_UPDATES, m_bSqlSsUpdates);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMisc1, CPropertyPage)
	//{{AFX_MSG_MAP(CMisc1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMisc1 message handlers

void CMisc1::OnNewDSN()
{
	if (m_pDatabase->IsOpen())
	{
		SDWORD	longValue;

		switch (SQLGetInfoShort(SQL_TXN_CAPABLE))
		{
		case SQL_TC_NONE:
			m_strSqlTxnCapable = _T("None");
			m_strSqlCursorCommitBehavior = _T("<n/a>");
			m_strSqlCursorRollbackBehavior = _T("<n/a>");
			m_bSqlTxnReadUncommitted = FALSE;
			m_bSqlTxnReadCommitted = FALSE;
			m_bSqlTxnRepeatableRead = FALSE;
			m_bSqlTxnSerializable = FALSE;
			m_bSqlTxnVersioning = FALSE;
			m_strSqlDefaultTxnIsolation = _T("<n/a>");
			m_strSqlMultipleActiveTxn = _T("<n/a>");
			if (IsWindow(m_hWnd))
				UpdateData(FALSE);
			return;
		case SQL_TC_DML:
			m_strSqlTxnCapable = _T("DDL statements cause an error");
			break;
		case SQL_TC_DDL_COMMIT:
			m_strSqlTxnCapable = _T("DDL statements cause a commit");
			break;
		case SQL_TC_DDL_IGNORE:
			m_strSqlTxnCapable = _T("DDL statements are ignored");
			break;
		case SQL_TC_ALL:
			m_strSqlTxnCapable = _T("DDL and DML statements supported");
			break;
		}

		switch (SQLGetInfoShort(SQL_CURSOR_COMMIT_BEHAVIOR))
		{
		case SQL_CB_DELETE:
			m_strSqlCursorCommitBehavior = _T("Close Cursors, Delete Statements");
			break;
		case SQL_CB_CLOSE:
			m_strSqlCursorCommitBehavior = _T("Close Cursors");
			break;
		case SQL_CB_PRESERVE:
			m_strSqlCursorCommitBehavior = _T("Preserve Cursors");
			break;
		}

		switch (SQLGetInfoShort(SQL_CURSOR_ROLLBACK_BEHAVIOR))
		{
		case SQL_CB_DELETE:
			m_strSqlCursorRollbackBehavior = _T("Close Cursors, Delete Statements");
			break;
		case SQL_CB_CLOSE:
			m_strSqlCursorRollbackBehavior = _T("Close Cursors");
			break;
		case SQL_CB_PRESERVE:
			m_strSqlCursorRollbackBehavior = _T("Preserve Cursors");
			break;
		}

		longValue = SQLGetInfoLong(SQL_TXN_ISOLATION_OPTION);
		if (longValue & SQL_TXN_READ_UNCOMMITTED)
			m_bSqlTxnReadUncommitted = TRUE;
		else
			m_bSqlTxnReadUncommitted = FALSE;
		if (longValue & SQL_TXN_READ_COMMITTED)
			m_bSqlTxnReadCommitted = TRUE;
		else
			m_bSqlTxnReadCommitted = FALSE;
		if (longValue & SQL_TXN_REPEATABLE_READ)
			m_bSqlTxnRepeatableRead = TRUE;
		else
			m_bSqlTxnRepeatableRead = FALSE;
		if (longValue & SQL_TXN_SERIALIZABLE)
			m_bSqlTxnSerializable = TRUE;
		else
			m_bSqlTxnSerializable = FALSE;
		if (longValue & SQL_TXN_VERSIONING)
			m_bSqlTxnVersioning = TRUE;
		else
			m_bSqlTxnVersioning = FALSE;

		switch (SQLGetInfoLong(SQL_DEFAULT_TXN_ISOLATION))
		{
		case SQL_TXN_READ_UNCOMMITTED:
			m_strSqlDefaultTxnIsolation = _T("Read Uncommitted");
			break;
		case SQL_TXN_READ_COMMITTED:
			m_strSqlDefaultTxnIsolation = _T("Read Committed");
			break;
		case SQL_TXN_REPEATABLE_READ:
			m_strSqlDefaultTxnIsolation = _T("Repeatable Read");
			break;
		case SQL_TXN_SERIALIZABLE:
			m_strSqlDefaultTxnIsolation = _T("Serializable");
			break;
		case SQL_TXN_VERSIONING:
			m_strSqlDefaultTxnIsolation = _T("Versioning");
			break;
		}
		YIsSupported(m_strSqlMultipleActiveTxn,SQL_MULTIPLE_ACTIVE_TXN);
		longValue = SQLGetInfoLong(SQL_BOOKMARK_PERSISTENCE);
		if (longValue & SQL_BP_CLOSE)
			m_bSqlBpClose = TRUE;
		else
			m_bSqlBpClose = FALSE;
		if (longValue & SQL_BP_DELETE)
			m_bSqlBpDelete = TRUE;
		else
			m_bSqlBpDelete = FALSE;
		if (longValue & SQL_BP_DROP)
			m_bSqlBpDrop = TRUE;
		else
			m_bSqlBpDrop = FALSE;
		if (longValue & SQL_BP_SCROLL)
			m_bSqlBpScroll = TRUE;
		else
			m_bSqlBpScroll = FALSE;
		if (longValue & SQL_BP_TRANSACTION)
			m_bSqlBpTransaction = TRUE;
		else
			m_bSqlBpTransaction = FALSE;
		if (longValue & SQL_BP_UPDATE)
			m_bSqlBpUpdate = TRUE;
		else
			m_bSqlBpUpdate = FALSE;
		if (longValue & SQL_BP_OTHER_HSTMT)
			m_bSqlBpOtherHstmt = TRUE;
		else
			m_bSqlBpOtherHstmt = FALSE;

		longValue = SQLGetInfoLong(SQL_LOCK_TYPES);
		if (longValue & SQL_LCK_NO_CHANGE)
			m_bSqlLckNoChange = TRUE;
		else
			m_bSqlLckNoChange = FALSE;
		if (longValue & SQL_LCK_EXCLUSIVE)
			m_bSqlLckExclusive = TRUE;
		else
			m_bSqlLckExclusive = FALSE;
		if (longValue & SQL_LCK_UNLOCK)
			m_bSqlLckUnlock = TRUE;
		else
			m_bSqlLckUnlock = FALSE;

		longValue = SQLGetInfoLong(SQL_STATIC_SENSITIVITY);
		if (longValue & SQL_SS_ADDITIONS)
			m_bSqlSsAdditions = TRUE;
		else
			m_bSqlSsAdditions = FALSE;
		if (longValue & SQL_SS_DELETIONS)
			m_bSqlSsDeletions = TRUE;
		else
			m_bSqlSsDeletions = FALSE;
		if (longValue & SQL_SS_UPDATES)
			m_bSqlSsUpdates = TRUE;
		else
			m_bSqlSsUpdates = FALSE;

		if (IsWindow(m_hWnd))
			UpdateData(FALSE);
	}
}

BOOL CMisc1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CMisc2 property page

IMPLEMENT_DYNCREATE(CMisc2, CPropertyPage)

CMisc2::CMisc2(CDatabase* pDatabase)
	: CMyPage(pDatabase,CMisc2::IDD)
 {
	//{{AFX_DATA_INIT(CMisc2)
	m_bSqlOuDmlStatements = FALSE;
	m_bSqlOuIndexDefinition = FALSE;
	m_bSqlOuPrivilegeDefinition = FALSE;
	m_bSqlOuProcedureInvocation = FALSE;
	m_bSqlOuTableDefinition = FALSE;
	m_bSqlQuDmlStatements = FALSE;
	m_bSqlQuIndexDefinition = FALSE;
	m_bSqlQuPrivilegeDefinition = FALSE;
	m_bSqlQuProcedureInvocation = FALSE;
	m_bSqlQuTableDefinition = FALSE;
	m_bSqlSoDynamic = FALSE;
	m_bSqlSoForwardOnly = FALSE;
	m_bSqlSoKeysetDriven = FALSE;
	m_bSqlSoMixed = FALSE;
	m_bSqlSoStatic = FALSE;
	m_bSqlSccoLock = FALSE;
	m_bSqlSccoOptRowver = FALSE;
	m_bSqlSccoOptValues = FALSE;
	m_bSqlSccoReadOnly = FALSE;
	m_bSqlPosAdd = FALSE;
	m_bSqlPosDelete = FALSE;
	m_bSqlPosPosition = FALSE;
	m_bSqlPosRefresh = FALSE;
	m_bSqlPosUpdate = FALSE;
	m_bSqlFdFetchAbsolute = FALSE;
	m_bSqlFdFetchBookmark = FALSE;
	m_bSqlFdFetchFirst = FALSE;
	m_bSqlFdFetchLast = FALSE;
	m_bSqlFdFetchNext = FALSE;
	m_bSqlFdFetchPrior = FALSE;
	m_bSqlFdFetchRelative = FALSE;
	m_bSqlFdFetchResume = FALSE;
	m_bSqlGdAnyColumn = FALSE;
	m_bSqlGdAnyOrder = FALSE;
	m_bSqlGdBlock = FALSE;
	m_bSqlGdBound = FALSE;
	m_strSqlConcatNullBehavior = _T("");
	m_strSqlNeedLongDataLen = _T("");
	m_strSqlNonNullableColumns = _T("");
	m_strSqlNullCollation = _T("");
	m_strSqlProcedures = _T("");
	m_strSqlRowUpdates = _T("");
	//}}AFX_DATA_INIT

	OnNewDSN();
}

CMisc2::~CMisc2()
{
}

void CMisc2::DumpToFile(CStdioFile& file)
{
	if (m_pDatabase->IsOpen())
	{
		CString	string;
		BOOL	b;

		string.Format(_T("    Need Long Data Length - %s\n"),m_strSqlNeedLongDataLen);
		file.WriteString(string);
		string.Format(_T("     Non-Nullable Columns - %s\n"),m_strSqlNonNullableColumns);
		file.WriteString(string);
		string.Format(_T("       Null Concatenation - %s\n"),m_strSqlConcatNullBehavior);
		file.WriteString(string);
		string.Format(_T("           Null Collation - %s\n"),m_strSqlNullCollation);
		file.WriteString(string);
		string.Format(_T("               Procedures - %s\n"),m_strSqlProcedures);
		file.WriteString(string);
		string.Format(_T("              Row Updates - %s\n"),m_strSqlRowUpdates);
		file.WriteString(string);

		file.WriteString(_T("          Fetch Direction - "));
		b = FALSE;
		if (m_bSqlFdFetchAbsolute)
		{
			file.WriteString(_T("Absolute"));
			b = TRUE;
		}
		if (m_bSqlFdFetchBookmark)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Bookmark"));
			b = TRUE;
		}
		if (m_bSqlFdFetchFirst)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("First"));
			b = TRUE;
		}
		if (m_bSqlFdFetchLast)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Last"));
			b = TRUE;
		}
		if (m_bSqlFdFetchNext)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Next"));
			b = TRUE;
		}
		if (m_bSqlFdFetchPrior)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Prior"));
			b = TRUE;
		}
		if (m_bSqlFdFetchRelative)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Relative"));
			b = TRUE;
		}
		if (m_bSqlFdFetchResume)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Resume"));
			b = TRUE;
		}
		if (!b)
			file.WriteString(_T("<none>"));
		file.WriteString(_T("\n"));

		file.WriteString(_T("      Get Data Extensions - "));
		b = FALSE;
		if (m_bSqlGdAnyColumn)
		{
			file.WriteString(_T("Any Column"));
			b = TRUE;
		}
		if (m_bSqlGdAnyOrder)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Any Order"));
			b = TRUE;
		}
		if (m_bSqlGdBlock)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Block"));
			b = TRUE;
		}
		if (m_bSqlGdBound)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Bound"));
			b = TRUE;
		}
		if (!b)
			file.WriteString(_T("<none>"));
		file.WriteString(_T("\n"));

		file.WriteString(_T("    Positioned Operations - "));
		b = FALSE;
		if (m_bSqlPosAdd)
		{
			file.WriteString(_T("Add"));
			b = TRUE;
		}
		if (m_bSqlPosDelete)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Delete"));
			b = TRUE;
		}
		if (m_bSqlPosPosition)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Position"));
			b = TRUE;
		}
		if (m_bSqlPosRefresh)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Refresh"));
			b = TRUE;
		}
		if (m_bSqlPosUpdate)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Update"));
			b = TRUE;
		}
		if (!b)
			file.WriteString(_T("<none>"));
		file.WriteString(_T("\n"));

		file.WriteString(_T("       Scroll Concurrency - "));
		b = FALSE;
		if (m_bSqlSccoLock)
		{
			file.WriteString(_T("Lock"));
			b = TRUE;
		}
		if (m_bSqlSccoOptRowver)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Opt/RowVer"));
			b = TRUE;
		}
		if (m_bSqlSccoOptValues)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Opt/Values"));
			b = TRUE;
		}
		if (m_bSqlSccoReadOnly)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Read Only"));
			b = TRUE;
		}
		if (!b)
			file.WriteString(_T("<none>"));
		file.WriteString(_T("\n"));

		file.WriteString(_T("           Scroll Options - "));
		b = FALSE;
		if (m_bSqlSoDynamic)
		{
			file.WriteString(_T("Dynamic"));
			b = TRUE;
		}
		if (m_bSqlSoForwardOnly)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Forward Only"));
			b = TRUE;
		}
		if (m_bSqlSoKeysetDriven)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Keyset Driven"));
			b = TRUE;
		}
		if (m_bSqlSoMixed)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Mixed"));
			b = TRUE;
		}
		if (m_bSqlSoStatic)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Static"));
			b = TRUE;
		}
		if (!b)
			file.WriteString(_T("<none>"));
		file.WriteString(_T("\n"));

		file.WriteString(_T("              Owner Usage - "));
		b = FALSE;
		if (m_bSqlOuDmlStatements)
		{
			file.WriteString(_T("DML Stmt"));
			b = TRUE;
		}
		if (m_bSqlOuIndexDefinition)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Index Def"));
			b = TRUE;
		}
		if (m_bSqlOuPrivilegeDefinition)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Priv Def"));
			b = TRUE;
		}
		if (m_bSqlOuProcedureInvocation)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Proc Invoke"));
			b = TRUE;
		}
		if (m_bSqlOuTableDefinition)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Table Def"));
			b = TRUE;
		}
		if (!b)
			file.WriteString(_T("<none>"));
		file.WriteString(_T("\n"));

		file.WriteString(_T("          Qualifier Usage - "));
		b = FALSE;
		if (m_bSqlQuDmlStatements)
		{
			file.WriteString(_T("DML Stmt"));
			b = TRUE;
		}
		if (m_bSqlQuIndexDefinition)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Index Def"));
			b = TRUE;
		}
		if (m_bSqlQuPrivilegeDefinition)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Priv Def"));
			b = TRUE;
		}
		if (m_bSqlQuProcedureInvocation)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Proc Invoke"));
			b = TRUE;
		}
		if (m_bSqlQuTableDefinition)
		{
			if (b)
				file.WriteString(_T(","));
			file.WriteString(_T("Table Def"));
			b = TRUE;
		}
		if (!b)
			file.WriteString(_T("<none>"));
		file.WriteString(_T("\n"));



	}
}

void CMisc2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMisc2)
	DDX_Check(pDX, IDC_SQL_OU_DML_STATEMENTS, m_bSqlOuDmlStatements);
	DDX_Check(pDX, IDC_SQL_OU_INDEX_DEFINITION, m_bSqlOuIndexDefinition);
	DDX_Check(pDX, IDC_SQL_OU_PRIVILEGE_DEFINITION, m_bSqlOuPrivilegeDefinition);
	DDX_Check(pDX, IDC_SQL_OU_PROCEDURE_INVOCATION, m_bSqlOuProcedureInvocation);
	DDX_Check(pDX, IDC_SQL_OU_TABLE_DEFINITION, m_bSqlOuTableDefinition);
	DDX_Check(pDX, IDC_SQL_QU_DML_STATEMENTS, m_bSqlQuDmlStatements);
	DDX_Check(pDX, IDC_SQL_QU_INDEX_DEFINITION, m_bSqlQuIndexDefinition);
	DDX_Check(pDX, IDC_SQL_QU_PRIVILEGE_DEFINITION, m_bSqlQuPrivilegeDefinition);
	DDX_Check(pDX, IDC_SQL_QU_PROCEDURE_INVOCATION, m_bSqlQuProcedureInvocation);
	DDX_Check(pDX, IDC_SQL_QU_TABLE_DEFINITION, m_bSqlQuTableDefinition);
	DDX_Check(pDX, IDC_SQL_SO_DYNAMIC, m_bSqlSoDynamic);
	DDX_Check(pDX, IDC_SQL_SO_FORWARD_ONLY, m_bSqlSoForwardOnly);
	DDX_Check(pDX, IDC_SQL_SO_KEYSET_DRIVEN, m_bSqlSoKeysetDriven);
	DDX_Check(pDX, IDC_SQL_SO_MIXED, m_bSqlSoMixed);
	DDX_Check(pDX, IDC_SQL_SO_STATIC, m_bSqlSoStatic);
	DDX_Check(pDX, IDC_SQL_SCCO_LOCK, m_bSqlSccoLock);
	DDX_Check(pDX, IDC_SQL_SCCO_OPT_ROWVER, m_bSqlSccoOptRowver);
	DDX_Check(pDX, IDC_SQL_SCCO_OPT_VALUES, m_bSqlSccoOptValues);
	DDX_Check(pDX, IDC_SQL_SCCO_READ_ONLY, m_bSqlSccoReadOnly);
	DDX_Check(pDX, IDC_SQL_POS_ADD, m_bSqlPosAdd);
	DDX_Check(pDX, IDC_SQL_POS_DELETE, m_bSqlPosDelete);
	DDX_Check(pDX, IDC_SQL_POS_POSITION, m_bSqlPosPosition);
	DDX_Check(pDX, IDC_SQL_POS_REFRESH, m_bSqlPosRefresh);
	DDX_Check(pDX, IDC_SQL_POS_UPDATE, m_bSqlPosUpdate);
	DDX_Check(pDX, IDC_SQL_FD_FETCH_ABSOLUTE, m_bSqlFdFetchAbsolute);
	DDX_Check(pDX, IDC_SQL_FD_FETCH_BOOKMARK, m_bSqlFdFetchBookmark);
	DDX_Check(pDX, IDC_SQL_FD_FETCH_FIRST, m_bSqlFdFetchFirst);
	DDX_Check(pDX, IDC_SQL_FD_FETCH_LAST, m_bSqlFdFetchLast);
	DDX_Check(pDX, IDC_SQL_FD_FETCH_NEXT, m_bSqlFdFetchNext);
	DDX_Check(pDX, IDC_SQL_FD_FETCH_PRIOR, m_bSqlFdFetchPrior);
	DDX_Check(pDX, IDC_SQL_FD_FETCH_RELATIVE, m_bSqlFdFetchRelative);
	DDX_Check(pDX, IDC_SQL_FD_FETCH_RESUME, m_bSqlFdFetchResume);
	DDX_Check(pDX, IDC_SQL_GD_ANY_COLUMN, m_bSqlGdAnyColumn);
	DDX_Check(pDX, IDC_SQL_GD_ANY_ORDER, m_bSqlGdAnyOrder);
	DDX_Check(pDX, IDC_SQL_GD_BLOCK, m_bSqlGdBlock);
	DDX_Check(pDX, IDC_SQL_GD_BOUND, m_bSqlGdBound);
	DDX_Text(pDX, IDC_SQL_CONCAT_NULL_BEHAVIOR, m_strSqlConcatNullBehavior);
	DDX_Text(pDX, IDC_SQL_NEED_LONG_DATA_LEN, m_strSqlNeedLongDataLen);
	DDX_Text(pDX, IDC_SQL_NON_NULLABLE_COLUMNS, m_strSqlNonNullableColumns);
	DDX_Text(pDX, IDC_SQL_NULL_COLLATION, m_strSqlNullCollation);
	DDX_Text(pDX, IDC_SQL_PROCEDURES, m_strSqlProcedures);
	DDX_Text(pDX, IDC_SQL_ROW_UPDATES, m_strSqlRowUpdates);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMisc2, CPropertyPage)
	//{{AFX_MSG_MAP(CMisc2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMisc2 message handlers

void CMisc2::OnNewDSN()
{
	if (m_pDatabase->IsOpen())
	{
		SDWORD	longValue;

		YIsYes(m_strSqlNeedLongDataLen,SQL_NEED_LONG_DATA_LEN);
		switch (SQLGetInfoShort(SQL_NON_NULLABLE_COLUMNS))
		{
		case SQL_NNC_NULL:
			m_strSqlNonNullableColumns = _T("Not Supported");
			break;
		case SQL_NNC_NON_NULL:
			m_strSqlNonNullableColumns = _T("Supported");
			break;
		}
		switch (SQLGetInfoShort(SQL_CONCAT_NULL_BEHAVIOR))
		{
		case SQL_CB_NULL:
			m_strSqlConcatNullBehavior = _T("Result is NULL");
			break;
		case SQL_CB_NON_NULL:
			m_strSqlConcatNullBehavior = _T("Result is not NULL");
			break;
		}
		switch (SQLGetInfoShort(SQL_NULL_COLLATION))
		{
		case SQL_NC_END:
			m_strSqlNullCollation = _T("End");
			break;
		case SQL_NC_HIGH:
			m_strSqlNullCollation = _T("High");
			break;
		case SQL_NC_LOW:
			m_strSqlNullCollation = _T("Low");
			break;
		case SQL_NC_START:
			m_strSqlNullCollation = _T("Start");
			break;
		}
		YIsSupported(m_strSqlProcedures,SQL_PROCEDURES);
		YIsYes(m_strSqlRowUpdates,SQL_ROW_UPDATES);
		longValue = SQLGetInfoLong(SQL_FETCH_DIRECTION);
		if (longValue & SQL_FD_FETCH_NEXT)
			m_bSqlFdFetchNext = TRUE;
		else
			m_bSqlFdFetchNext = FALSE;
		if (longValue & SQL_FD_FETCH_FIRST)
			m_bSqlFdFetchFirst = TRUE;
		else
			m_bSqlFdFetchFirst = FALSE;
		if (longValue & SQL_FD_FETCH_LAST)
			m_bSqlFdFetchLast = TRUE;
		else
			m_bSqlFdFetchLast = FALSE;
		if (longValue & SQL_FD_FETCH_PRIOR)
			m_bSqlFdFetchPrior = TRUE;
		else
			m_bSqlFdFetchPrior = FALSE;
		if (longValue & SQL_FD_FETCH_ABSOLUTE)
			m_bSqlFdFetchAbsolute = TRUE;
		else
			m_bSqlFdFetchAbsolute = FALSE;
		if (longValue & SQL_FD_FETCH_RELATIVE)
			m_bSqlFdFetchRelative = TRUE;
		else
			m_bSqlFdFetchRelative = FALSE;
		if (longValue & SQL_FD_FETCH_RESUME)
			m_bSqlFdFetchResume = TRUE;
		else
			m_bSqlFdFetchResume = FALSE;
		if (longValue & SQL_FD_FETCH_BOOKMARK)
			m_bSqlFdFetchBookmark = TRUE;
		else
			m_bSqlFdFetchBookmark = FALSE;

		longValue = SQLGetInfoLong(SQL_GETDATA_EXTENSIONS);
		if (longValue & SQL_GD_ANY_COLUMN)
			m_bSqlGdAnyColumn = TRUE;
		else
			m_bSqlGdAnyColumn = FALSE;
		if (longValue & SQL_GD_ANY_ORDER)
			m_bSqlGdAnyOrder = TRUE;
		else
			m_bSqlGdAnyOrder = FALSE;
		if (longValue & SQL_GD_BLOCK)
			m_bSqlGdBlock = TRUE;
		else
			m_bSqlGdBlock = FALSE;
		if (longValue & SQL_GD_BOUND)
			m_bSqlGdBound = TRUE;
		else
			m_bSqlGdBound = FALSE;

		longValue = SQLGetInfoLong(SQL_POS_OPERATIONS);
		if (longValue & SQL_POS_POSITION)
			m_bSqlPosPosition = TRUE;
		else
			m_bSqlPosPosition = FALSE;
		if (longValue & SQL_POS_REFRESH)
			m_bSqlPosRefresh = TRUE;
		else
			m_bSqlPosRefresh = FALSE;
		if (longValue & SQL_POS_UPDATE)
			m_bSqlPosUpdate = TRUE;
		else
			m_bSqlPosUpdate = FALSE;
		if (longValue & SQL_POS_DELETE)
			m_bSqlPosDelete = TRUE;
		else
			m_bSqlPosDelete = FALSE;
		if (longValue & SQL_POS_ADD)
			m_bSqlPosAdd = TRUE;
		else
			m_bSqlPosAdd = FALSE;

		longValue = SQLGetInfoLong(SQL_SCROLL_CONCURRENCY);
		if (longValue & SQL_SCCO_READ_ONLY)
			m_bSqlSccoReadOnly = TRUE;
		else
			m_bSqlSccoReadOnly = FALSE;
		if (longValue & SQL_SCCO_LOCK)
			m_bSqlSccoLock = TRUE;
		else
			m_bSqlSccoLock = FALSE;
		if (longValue & SQL_SCCO_OPT_ROWVER)
			m_bSqlSccoOptRowver = TRUE;
		else
			m_bSqlSccoOptRowver = FALSE;
		if (longValue & SQL_SCCO_OPT_VALUES)
			m_bSqlSccoOptValues = TRUE;
		else
			m_bSqlSccoOptValues = FALSE;
		
		longValue = SQLGetInfoLong(SQL_SCROLL_OPTIONS);
		if (longValue & SQL_SO_FORWARD_ONLY)
			m_bSqlSoForwardOnly = TRUE;
		else
			m_bSqlSoForwardOnly = FALSE;
		if (longValue & SQL_SO_STATIC)
			m_bSqlSoStatic = TRUE;
		else
			m_bSqlSoStatic = FALSE;
		if (longValue & SQL_SO_KEYSET_DRIVEN)
			m_bSqlSoKeysetDriven = TRUE;
		else
			m_bSqlSoKeysetDriven = FALSE;
		if (longValue & SQL_SO_DYNAMIC)
			m_bSqlSoDynamic = TRUE;
		else
			m_bSqlSoDynamic = FALSE;
		if (longValue & SQL_SO_MIXED)
			m_bSqlSoMixed = TRUE;
		else
			m_bSqlSoMixed = FALSE;

		longValue = SQLGetInfoLong(SQL_OWNER_USAGE);
		if (longValue & SQL_OU_DML_STATEMENTS)
			m_bSqlOuDmlStatements = TRUE;
		else
			m_bSqlOuDmlStatements = FALSE;
		if (longValue & SQL_OU_PROCEDURE_INVOCATION)
			m_bSqlOuProcedureInvocation = TRUE;
		else
			m_bSqlOuProcedureInvocation = FALSE;
		if (longValue & SQL_OU_TABLE_DEFINITION)
			m_bSqlOuTableDefinition = TRUE;
		else
			m_bSqlOuTableDefinition = FALSE;
		if (longValue & SQL_OU_INDEX_DEFINITION)
			m_bSqlOuIndexDefinition = TRUE;
		else
			m_bSqlOuIndexDefinition = FALSE;
		if (longValue & SQL_OU_PRIVILEGE_DEFINITION)
			m_bSqlOuPrivilegeDefinition = TRUE;
		else
			m_bSqlOuPrivilegeDefinition = FALSE;

		longValue = SQLGetInfoLong(SQL_QUALIFIER_USAGE);
		if (longValue & SQL_QU_DML_STATEMENTS)
			m_bSqlQuDmlStatements = TRUE;
		else
			m_bSqlQuDmlStatements = FALSE;
		if (longValue & SQL_QU_PROCEDURE_INVOCATION)
			m_bSqlQuProcedureInvocation = TRUE;
		else
			m_bSqlQuProcedureInvocation = FALSE;
		if (longValue & SQL_QU_TABLE_DEFINITION)
			m_bSqlQuTableDefinition = TRUE;
		else
			m_bSqlQuTableDefinition = FALSE;
		if (longValue & SQL_QU_INDEX_DEFINITION)
			m_bSqlQuIndexDefinition = TRUE;
		else
			m_bSqlQuIndexDefinition = FALSE;
		if (longValue & SQL_QU_PRIVILEGE_DEFINITION)
			m_bSqlQuPrivilegeDefinition = TRUE;
		else
			m_bSqlQuPrivilegeDefinition = FALSE;

		if (IsWindow(m_hWnd))
			UpdateData(FALSE);
	}
}

BOOL CMisc2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
