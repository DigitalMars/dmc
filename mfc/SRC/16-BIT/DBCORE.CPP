// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"

#ifdef AFX_DB_SEG
#pragma code_seg(AFX_DB_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

#ifdef _DEBUG
BOOL bTraceSql = FALSE;
static char BASED_CODE szOdbcCall[] = "odbccall.txt";
#endif // _DEBUG

#ifndef _AFXDLL
static WORD NEAR _afxWaitForDataSource = 0;
#else
#define _afxWaitForDataSource _AfxGetAppData()->appWaitForDataSource
#endif // _AFXDLL

#ifndef _AFXDLL
#ifndef _USRDLL
static HENV  afxHenvAllConnections = SQL_NULL_HENV;
static int  afxAllocatedConnections = 0;
#endif // _USRDLL
#else
#define afxHenvAllConnections    (_AfxGetAppData()->appHenvAllConnections)
#define afxAllocatedConnections  (_AfxGetAppData()->appAllocatedConnections)
#endif // _AFXDLL

#ifdef _USRDLL
// struct of info to store in handle map
struct HENVINFO
{
	HENV henvAllConnections;
	int nAllocatedConnections;
};

// global state for HenvInfo handle map
static CMapWordToPtr mapHTASK;

// global state for simple single task cache
static HTASK NEAR hTaskCache;
static struct HENVINFO* pHenvinfoCache;

#ifndef _PORTABLE
extern int cdecl AfxCriticalNewHandler(size_t nSize);
#endif

// out-of-line helper for getting henvInfo for current task
HENVINFO* AFXAPI AfxGetHenvinfo()
{
	HTASK hTaskCurrent = ::GetCurrentTask();
	ASSERT(hTaskCurrent != NULL);
	if (hTaskCurrent != hTaskCache)
	{
		HENVINFO* pHenvinfoLookup;
		if (!mapHTASK.Lookup((WORD)hTaskCurrent, (void*&)pHenvinfoLookup))
		{
			TRY
			{
				// We don't want the user to see these memory
				// allocations, so we turn the tracing off.
#ifdef _DEBUG
				BOOL bEnable = AfxEnableMemoryTracking(FALSE);
#endif
#ifndef _PORTABLE
				_PNH pnhOldHandler = _AfxSetNewHandler(AfxCriticalNewHandler);
#endif

				// allocate new task HENVINFO
				pHenvinfoLookup = new HENVINFO;
				pHenvinfoLookup->henvAllConnections = SQL_NULL_HENV;
				pHenvinfoLookup->nAllocatedConnections = 0;

				// and insert it into the map
				mapHTASK.SetAt((WORD)hTaskCurrent, pHenvinfoLookup);

#ifndef _PORTABLE
				_AfxSetNewHandler(pnhOldHandler);
#endif
#ifdef _DEBUG
				AfxEnableMemoryTracking(bEnable);
#endif
			}
			CATCH_ALL(e)
			{
				TRACE0("Error: Failed allocation of HENVINFO!\n");
				THROW_LAST();
			}
			END_CATCH_ALL
		}
		hTaskCache = hTaskCurrent;
		pHenvinfoCache = pHenvinfoLookup;
	}
	ASSERT(pHenvinfoCache != NULL);
	return pHenvinfoCache;
}

// out-of-line cleanup of allocated HENVINFO structs called from CDatabase::Close
void AFXAPI AfxDeleteHenvinfo()
{
	HENVINFO* pHenvinfo = AfxGetHenvinfo();

	// current henvinfo and task should be cached
	ASSERT(pHenvinfo == pHenvinfoCache);
	ASSERT(hTaskCache == ::GetCurrentTask());

	// remove the henvinfo from the map and delete
	VERIFY(mapHTASK.RemoveKey((WORD)hTaskCache));
	delete pHenvinfo;
	hTaskCache = NULL;
}

#define afxHenvAllConnections (AfxGetHenvinfo()->henvAllConnections)
#define afxAllocatedConnections (AfxGetHenvinfo()->nAllocatedConnections)

#endif // _USRDLL

/////////////////////////////////////////////////////////////////////////////
// CDBException

IMPLEMENT_DYNAMIC(CDBException, CException)

void AFXAPI AfxThrowDBException(RETCODE nRetCode, CDatabase* pdb,
	HSTMT hstmt)
{
	CDBException* pException = new CDBException(nRetCode);
	if (nRetCode == SQL_ERROR && pdb != NULL)
	{
		pException->BuildErrorString(pdb, hstmt);
	}
	else if (nRetCode > AFX_SQL_ERROR && nRetCode < AFX_SQL_ERROR_MAX)
	{
		VERIFY(pException->m_strError.LoadString(AFX_IDP_SQL_FIRST+(nRetCode-AFX_SQL_ERROR)));
		TRACE1("%s\n", (LPCSTR)pException->m_strError);
	}

	THROW(pException);
}

CDBException::CDBException(RETCODE nRetCode)
{
	m_nRetCode = nRetCode;
}

CDBException::~CDBException()
{
}

static char BASED_CODE sz00000[] = "00000";
void CDBException::BuildErrorString(CDatabase* pdb, HSTMT hstmt)
{
	ASSERT_VALID(this);

	if (pdb != NULL)
	{
		int nOutlen;
		RETCODE nRetCode;
		CString strMsg;
		CString strState;
		char* szState;
		SDWORD lNative;

		szState = strState.GetBuffer(SQL_SQLSTATE_SIZE);

		AFX_SQL_SYNC(::SQLError(afxHenvAllConnections, pdb->m_hdbc,
			hstmt, (UCHAR  FAR*) szState, &lNative,
			(UCHAR FAR*) strMsg.GetBufferSetLength(SQL_MAX_MESSAGE_LENGTH),
			SQL_MAX_MESSAGE_LENGTH, (short FAR*)&nOutlen));

		while ((nRetCode == SQL_SUCCESS || nRetCode == SQL_SUCCESS_WITH_INFO) &&
			lstrcmp(szState, sz00000) != 0)
		{
			strMsg.ReleaseBuffer();
			strState.ReleaseBuffer();

			char szNative[50];
			wsprintf(szNative, ",Native:%ld,Origin:", lNative);
			m_strStateNativeOrigin += szNative;

			// transfer [origin] from message string to StateNativeOrigin string
			int nCloseBracket;
			while (!strMsg.IsEmpty() &&
				strMsg[0] == '[' && (nCloseBracket = strMsg.Find(']')) >= 0)
			{
				// Skip ']'
				nCloseBracket++;
				strState += strMsg.Left(nCloseBracket);
				// Skip ' ', if present
				if (strMsg[nCloseBracket] == ' ')
					nCloseBracket++;
				strMsg = strMsg.Right(strMsg.GetLength() - nCloseBracket);
			}
			strState += "\n";
			m_strStateNativeOrigin += "State:" + strState;
			m_strError += strMsg + "\n";

#ifdef _DEBUG
			TraceErrorMessage(strMsg);
			TraceErrorMessage("State:" + strState);
#endif // _DEBUG

			szState = strState.GetBuffer(SQL_SQLSTATE_SIZE);

			AFX_SQL_SYNC(::SQLError(afxHenvAllConnections, pdb->m_hdbc,
				hstmt, (UCHAR  FAR*) szState, &lNative,
				(UCHAR FAR*) strMsg.GetBuffer(SQL_MAX_MESSAGE_LENGTH),
				SQL_MAX_MESSAGE_LENGTH, (short FAR*)&nOutlen));
		}
	strState.ReleaseBuffer();
	strMsg.ReleaseBuffer();
	}
}

#ifdef _DEBUG
void CDBException::TraceErrorMessage(LPCTSTR szTrace) const
{
	CString strTrace = szTrace;

	if (strTrace.GetLength() <= 80)
		TRACE1("%s\n", szTrace);
	else
	{
		// Display 80 chars/line
		while (strTrace.GetLength() > 80)
		{
			TRACE1("%s\n", (LPCSTR)strTrace.Left(80));
			strTrace = strTrace.Right(strTrace.GetLength() - 80);
		}
		TRACE1("%s\n", (LPCTSTR)strTrace);
	}
}
#endif // _DEBUG

void CDBException::Empty()
{
	m_strError.Empty();
	m_strStateNativeOrigin.Empty();
}

#ifdef _DEBUG
void CDBException::AssertValid() const
{
	CObject::AssertValid();
}
#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDatabase

IMPLEMENT_DYNAMIC(CDatabase, CObject)

CDatabase::CDatabase()
{
	m_hdbc = SQL_NULL_HDBC;
	m_hstmt = SQL_NULL_HSTMT;
	m_bUpdatable = FALSE;
	m_bTransactions = FALSE;
#ifdef _DEBUG
	m_bTransactionPending = FALSE;
#endif // _DEBUG
	m_dwLoginTimeout = DEFAULT_LOGIN_TIMEOUT;
	m_dwQueryTimeout = DEFAULT_QUERY_TIMEOUT;
	m_dwWait = 0;
	m_dwMinWaitForDataSource = DEFAULT_MIN_WAIT_FOR_DATASOURCE;
	m_dwMaxWaitForDataSource = DEFAULT_MAX_WAIT_FOR_DATASOURCE;
	m_bStripTrailingSpaces = FALSE;
	// Be a good windows app
	m_bAsync = TRUE;
}

CDatabase::~CDatabase()
{
	ASSERT_VALID(this);

	Free();
}

static char BASED_CODE lpszODBC[] = "ODBC;";
BOOL CDatabase::Open(LPCSTR lpszDSN, BOOL bExclusive /* = FALSE */,
		BOOL bReadonly /* = FALSE */, LPCSTR lpszConnect /* = "ODBC;" */)
{
	char szConnectOutput[MAX_CONNECT_LEN];
	int cbConnStrOut;

	ASSERT_VALID(this);
	ASSERT(lpszDSN == NULL || AfxIsValidString(lpszDSN));
	ASSERT(lpszConnect == NULL || AfxIsValidString(lpszConnect));

	// Exclusive access not supported.
	ASSERT(!bExclusive);
	m_bUpdatable = !bReadonly;

	CString strTemp;
	TRY
	{
		if (lpszConnect != NULL)
			m_strConnect = lpszConnect;

		// For VB/Access compatibility, require "ODBC;" (or "odbc;")
		// prefix to the connect string
		strTemp = m_strConnect.Left(sizeof(lpszODBC)-1);
		if (lstrcmpi(strTemp, lpszODBC) != 0)
		{
#ifdef _DEBUG
			TRACE0("Error: Missing \"ODBC;\" prefix on connect string\n");
#endif // _DEBUG
			return FALSE;
		}

		// Strip "ODBC;"
		m_strConnect = m_strConnect.Right(m_strConnect.GetLength()
			- (sizeof(lpszODBC)-1));

		if (lpszDSN != NULL && lstrlen(lpszDSN) != 0)
		{
			// Append "DSN=" lpszDSN
			static char BASED_CODE lpszDSNEqual[] = ";DSN=";
			m_strConnect += lpszDSNEqual;
			m_strConnect += lpszDSN;
		}

		AllocConnect();

		RETCODE nRetCode;
		// Turn on cursor lib support
		AFX_SQL_SYNC(::SQLSetConnectOption(m_hdbc,
			SQL_ODBC_CURSORS, SQL_CUR_USE_ODBC));

		int cbResult;
		AFX_SQL_SYNC(::SQLDriverConnect(m_hdbc,
			_AfxGetSafeOwner(NULL),
			(UCHAR FAR*)(const char *)m_strConnect, SQL_NTS,
			(UCHAR FAR*)szConnectOutput, sizeof(szConnectOutput),
			(short FAR*)&cbConnStrOut, SQL_DRIVER_COMPLETE));

		// If user hit 'Cancel'
		if (nRetCode == SQL_NO_DATA_FOUND)
		{
			Free();
			return FALSE;
		}

		if (!Check(nRetCode))
		{
#ifdef _DEBUG
			if ((AfxGetApp()->m_pMainWnd)->GetSafeHwnd() == NULL)
				TRACE0("Error: No default window (AfxGetApp()->m_pMainWnd) for SQLDriverConnect\n");
#endif // _DEBUG
			ThrowDBException(nRetCode);
		}

		// Connect strings must have "ODBC;"
		m_strConnect = lpszODBC;
		// Save connect string returned from ODBC
		m_strConnect += szConnectOutput;

		UINT nAPIConformance;
		AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_ODBC_API_CONFORMANCE,
			&nAPIConformance, sizeof(nAPIConformance),
			(short FAR*)&cbResult));
		if (!Check(nRetCode))
			ThrowDBException(nRetCode);

		if (nAPIConformance < SQL_OAC_LEVEL1)
			ThrowDBException(AFX_SQL_ERROR_API_CONFORMANCE);

		UINT nSQLConformance;
		AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_ODBC_SQL_CONFORMANCE,
			&nSQLConformance, sizeof(nSQLConformance),
			(short FAR*)&cbResult));
		if (!Check(nRetCode))
			ThrowDBException(nRetCode);

		if (nSQLConformance < SQL_OSC_MINIMUM)
			ThrowDBException(AFX_SQL_ERROR_SQL_CONFORMANCE);

		UINT nCursorBehavior;
		AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_CURSOR_COMMIT_BEHAVIOR,
			&nCursorBehavior, sizeof(nCursorBehavior),
			(short FAR*)&cbResult));
		if (Check(nRetCode))
		{
			if (nCursorBehavior == SQL_CR_PRESERVE)
			{
				AFX_SQL_SYNC(::SQLGetInfo(m_hdbc,
					SQL_CURSOR_ROLLBACK_BEHAVIOR, &nCursorBehavior,
					sizeof(nCursorBehavior), (short FAR*)&cbResult));
				if (Check(nRetCode) && nCursorBehavior == SQL_CR_PRESERVE)
					m_bTransactions = TRUE;
			}
		}

		if (m_bUpdatable)
		{
			// Make sure data source is Updatable
			char szReadOnly[10];
			AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_DATA_SOURCE_READ_ONLY,
				szReadOnly, sizeof(szReadOnly), (short FAR*)&cbResult));
			if (Check(nRetCode) && cbResult == 1)
				m_bUpdatable = !(lstrcmp(szReadOnly, "Y") == 0);
			else
				m_bUpdatable = FALSE;
#ifdef _DEBUG
			if (!m_bUpdatable && (afxTraceFlags & 0x20))
				TRACE0("Warning: data source is readonly\n");
#endif // _DEBUG
		}
		else
		{
			// Make data source is !Updatable
			AFX_SQL_SYNC(::SQLSetConnectOption(m_hdbc,
				SQL_ACCESS_MODE, SQL_MODE_READ_ONLY));
		}

		char szIDQuoteChar[2];
		AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_IDENTIFIER_QUOTE_CHAR,
			szIDQuoteChar, sizeof(szIDQuoteChar), (short FAR*)&cbResult));
		if (Check(nRetCode) && cbResult == 1)
			m_chIDQuoteChar = szIDQuoteChar[0];
		else
			m_chIDQuoteChar = ' ';


		char szDriverName[32];
		AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_DRIVER_NAME,
			szDriverName, sizeof(szDriverName), (short FAR*)&cbResult));
		if (lstrcmpi(szDriverName, "simba.dll") == 0)
		{
			m_bStripTrailingSpaces = TRUE;
		}


#ifdef _DEBUG
		if (afxTraceFlags & 0x20)
		{
			int cbResult;
			char szInfo[64];
			AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_DBMS_NAME,
				szInfo, sizeof(szInfo), (short FAR*)&cbResult));
			if (Check(nRetCode))
			{
				TRACE1("DBMS: %s", szInfo);

				AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_DBMS_VER,
					szInfo, sizeof(szInfo), (short FAR*)&cbResult));
				if (Check(nRetCode))
					TRACE1(", Version: %s\n", szInfo);
			}
		}
#endif // _DEBUG
	}
	CATCH_ALL(e)
	{
		strTemp.Empty();
		Free();
		THROW_LAST();
	}
	END_CATCH_ALL

	return TRUE;
}

BOOL CDatabase::ExecuteSQL(LPCSTR lpszSQL)
{
	BOOL bSuccess = FALSE;
	RETCODE nRetCode;

	ASSERT_VALID(this);
	ASSERT(AfxIsValidString(lpszSQL));
	// Can't close till all pending Async operations have completed
	ASSERT(!InWaitForDataSource());

	ASSERT(m_hstmt == SQL_NULL_HSTMT);
	AFX_SQL_SYNC(::SQLAllocStmt(m_hdbc, &m_hstmt));
	if (!Check(nRetCode))
		ThrowDBException(nRetCode);

	TRY
	{
		OnSetOptions(m_hstmt);

		AFX_SQL_ASYNC(this, ::SQLExecDirect(m_hstmt, (UCHAR FAR*)lpszSQL, SQL_NTS));
		if (Check(nRetCode))
		{
			do
			{
				UINT nResultColumns;

				AFX_SQL_ASYNC(this, ::SQLNumResultCols(m_hstmt, (short FAR*)&nResultColumns));
				if (nResultColumns != 0)
				{
					do
					{
						AFX_SQL_ASYNC(this, ::SQLFetch(m_hstmt));
					} while (Check(nRetCode) && nRetCode != SQL_NO_DATA_FOUND);
				}
				AFX_SQL_ASYNC(this, ::SQLMoreResults(m_hstmt));
			} while (Check(nRetCode) && nRetCode != SQL_NO_DATA_FOUND);
		}

		bSuccess = (nRetCode == SQL_NO_DATA_FOUND);
	}
	CATCH_ALL(e)
	{
		::SQLCancel(m_hstmt);
		AFX_SQL_SYNC(::SQLFreeStmt(m_hstmt, SQL_DROP));
		m_hstmt = SQL_NULL_HSTMT;
		THROW_LAST();
	}
	END_CATCH_ALL

	AFX_SQL_SYNC(::SQLFreeStmt(m_hstmt, SQL_DROP));
	m_hstmt = SQL_NULL_HSTMT;
	return bSuccess;
}

// Shutdown pending query for CDatabase's private m_hstmt
void CDatabase::Cancel()
{
	ASSERT_VALID(this);
	ASSERT(m_hdbc != SQL_NULL_HDBC);

	::SQLCancel(m_hstmt);
}

// Disconnect connection
void CDatabase::Close()
{
	ASSERT_VALID(this);
	// Can't close till all pending Async operations have completed
	ASSERT(!InWaitForDataSource());

	if (m_hdbc != SQL_NULL_HDBC)
	{
		RETCODE nRetCode;
		AFX_SQL_SYNC(::SQLDisconnect(m_hdbc));
		AFX_SQL_SYNC(::SQLFreeConnect(m_hdbc));
		m_hdbc = SQL_NULL_HDBC;
		ASSERT(afxAllocatedConnections != 0);
		afxAllocatedConnections--;
	}
}

// Silently disconnect and free all ODBC resources.  Don't throw any exceptions
void CDatabase::Free()
{
	ASSERT_VALID(this);

	// Trap failures upon close
	TRY
	{
		Close();
	}
	CATCH_ALL(e)
	{
		// Nothing we can do
		TRACE0("Error: exception by CDatabase::Close() ignored in CDatabase::Free()\n");
	}
	END_CATCH_ALL

	// free henv if refcount goes to 0
	if (afxHenvAllConnections != SQL_NULL_HENV)
	{
		ASSERT(afxAllocatedConnections >= 0);
		if (afxAllocatedConnections == 0)
		{
			// free last connection - release HENV
			RETCODE nRetCodeEnv = ::SQLFreeEnv(afxHenvAllConnections);
#ifdef _DEBUG
			if (nRetCodeEnv != SQL_SUCCESS)
			{
				// Nothing we can do
				TRACE0("Error: SQLFreeEnv failure ignored in CDatabase::Free()\n");
			}
#endif // _DEBUG

#ifndef _USRDLL
			afxHenvAllConnections = SQL_NULL_HENV;
#else
			// clean up the HENVINFO task map
			AfxDeleteHenvinfo();
#endif // _USRDLL
		}
	}
}

void CDatabase::OnSetOptions(HSTMT hstmt)
{
	RETCODE nRetCode;
	ASSERT_VALID(this);
	ASSERT(m_hdbc != SQL_NULL_HDBC);

	if (m_dwQueryTimeout != -1)
	{
		// Attempt to set query timeout.  Ignore failure
		AFX_SQL_SYNC(::SQLSetStmtOption(hstmt, SQL_QUERY_TIMEOUT,
			m_dwQueryTimeout));
		if (!Check(nRetCode))
		{
			// Don't try again
			m_dwQueryTimeout = -1;
		}
	}

	// Attempt to set AFX_SQL_ASYNC.  Ignore failure
	if (m_bAsync)
	{
		AFX_SQL_SYNC(::SQLSetStmtOption(hstmt, SQL_ASYNC_ENABLE, m_bAsync));
		if (!Check(nRetCode))
		{
			m_bAsync = FALSE;
		}
	}
}

CString CDatabase::GetDatabaseName() const
{
	char szName[MAX_TNAME_LEN];
	int cbName;
	RETCODE nRetCode;

	ASSERT_VALID(this);
	ASSERT(m_hdbc != SQL_NULL_HDBC);

	AFX_SQL_SYNC(::SQLGetInfo(m_hdbc, SQL_DATABASE_NAME,
		szName, sizeof(szName), (short FAR*)&cbName));
	if (!Check(nRetCode))
		szName[0] = '\0';

	CString strName(szName);
	return strName;
}

BOOL CDatabase::BeginTrans()
{
	ASSERT_VALID(this);
	ASSERT(m_hdbc != SQL_NULL_HDBC);

	if (!m_bTransactions)
		return FALSE;

	// Only 1 level of transactions supported
	ASSERT(!m_bTransactionPending);

	RETCODE nRetCode;
	AFX_SQL_SYNC(::SQLSetConnectOption(m_hdbc, SQL_AUTOCOMMIT, 0));
#ifdef _DEBUG
	m_bTransactionPending = TRUE;
#endif // _DEBUG
	return Check(nRetCode);
}

BOOL CDatabase::CommitTrans()
{
	ASSERT_VALID(this);
	ASSERT(m_hdbc != SQL_NULL_HDBC);

	if (!m_bTransactions)
		return FALSE;

	// BeginTrans must be called first
	ASSERT(m_bTransactionPending);

	RETCODE nRetCode;
	AFX_SQL_SYNC(::SQLTransact(afxHenvAllConnections, m_hdbc, SQL_COMMIT));
	BOOL bSuccess = Check(nRetCode);
	// Turn back on auto commit
	AFX_SQL_SYNC(::SQLSetConnectOption(m_hdbc, SQL_AUTOCOMMIT, 1));
#ifdef _DEBUG
	m_bTransactionPending = FALSE;
#endif // _DEBUG
	return bSuccess;
}

BOOL CDatabase::Rollback()
{
	ASSERT_VALID(this);
	ASSERT(m_hdbc != SQL_NULL_HDBC);

	if (!m_bTransactions)
		return FALSE;

	// BeginTrans must be called first
	ASSERT(m_bTransactionPending);

	RETCODE nRetCode;
	AFX_SQL_SYNC(::SQLTransact(afxHenvAllConnections, m_hdbc, SQL_ROLLBACK));
	BOOL bSuccess = Check(nRetCode);
	// Turn back on auto commit
	AFX_SQL_SYNC(::SQLSetConnectOption(m_hdbc, SQL_AUTOCOMMIT, 1));
#ifdef _DEBUG
	m_bTransactionPending = FALSE;
#endif // _DEBUG
	return bSuccess;
}

// Screen for errors.
BOOL CDatabase::Check(RETCODE nRetCode) const
{
	ASSERT_VALID(this);

	switch (nRetCode)
	{
	case SQL_SUCCESS_WITH_INFO:
#ifdef _DEBUG
		if (afxTraceFlags & 0x20)
		{
			CDBException e(nRetCode);
			TRACE0("Warning: ODBC Success With Info, ");
			e.BuildErrorString((CDatabase*)this, SQL_NULL_HSTMT);
		}
#endif // _DEBUG

		// Fall through

	case SQL_SUCCESS:
	case SQL_NO_DATA_FOUND:
		return TRUE;
	}

	return FALSE;
}

BOOL PASCAL CDatabase::InWaitForDataSource()
{
	return _afxWaitForDataSource != 0;
}

void CDatabase::OnWaitForDataSource(BOOL bStillExecuting)
{
	ASSERT_VALID(this);
	ASSERT(m_hdbc != SQL_NULL_HDBC);

	if (!bStillExecuting)
	{
		// If never actually waited. . .
		if (m_dwWait == 0)
			return;

		if (m_dwWait == m_dwMaxWaitForDataSource)
			AfxGetApp()->DoWaitCursor(-1);      // EndWaitCursor
		m_dwWait = 0;
		_afxWaitForDataSource--;
#ifdef _DEBUG
		if (afxTraceFlags & 0x20)  // if db tracing
			TRACE0("DONE WAITING for datasource\n");
#endif // _DEBUG
		return;
	}

	if (m_dwWait == 0)
	{
		_afxWaitForDataSource++;
		// 1st call, wait for min amount of time
		m_dwWait = m_dwMinWaitForDataSource;
#ifdef _DEBUG
		if (afxTraceFlags & 0x20)  // if db tracing
			TRACE0("WAITING for datasource\n");
#endif // _DEBUG
	}
	else
	{
		if (m_dwWait == m_dwMinWaitForDataSource)
		{
			// 2nd call, wait max time, put up wait cursor
			m_dwWait = m_dwMaxWaitForDataSource;
			AfxGetApp()->DoWaitCursor(1);      // BeginWaitCursor
		}
	}

	DWORD clockFirst = GetTickCount();
	while (GetTickCount() - clockFirst < m_dwWait)
	{
		MSG msg;

		if (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
		{
			TRY
			{
				AfxGetApp()->PumpMessage();
			}
			CATCH_ALL(e)
			{
				TRACE0("Error: caught exception in OnWaitForDataSource - continuing\n");
			}
			END_CATCH_ALL
		}
		else
			AfxGetApp()->OnIdle(-1);
	}
}

//////////////////////////////////////////////////////////////////////////////
// CDatabase internal functions

CString CDatabase::QuoteName(const char *szName)
{
	// Replace all '[' or ']' with driver dependent ID quote char
	char sz[128+1];
	LPSTR pch = sz;

	_AfxStrCpy(sz, szName, sizeof(sz));
	while ((pch = _AfxStrChr(pch, '[')) != NULL)
		*pch++ = m_chIDQuoteChar;

	pch = sz;
	while ((pch = _AfxStrChr(pch, ']')) != NULL)
		*pch++ = m_chIDQuoteChar;
	return sz;
}

// Allocate an henv (first time called) and hdbc
void CDatabase::AllocConnect()
{
	ASSERT_VALID(this);

	if (m_hdbc != SQL_NULL_HDBC)
		return;

#ifdef _AFXDLL
	if (!_AfxGetAppData()->bDBExtensionDLL)
	{
		extern AFX_EXTENSION_MODULE NEAR _afxExtensionDLL;  // see dlldb.cpp
		new CDynLinkLibrary(_afxExtensionDLL);
		_AfxGetAppData()->bDBExtensionDLL = TRUE;
	}
#endif

	RETCODE nRetCode;
	if (afxHenvAllConnections == SQL_NULL_HENV)
	{
		ASSERT(afxAllocatedConnections == 0);

		// need to allocate an environment for first connection
		AFX_SQL_SYNC(::SQLAllocEnv(&afxHenvAllConnections));
		if (!Check(nRetCode))
		{
			AfxThrowMemoryException();  // fatal
		}
	}

	ASSERT(afxHenvAllConnections != SQL_NULL_HENV);
	AFX_SQL_SYNC(::SQLAllocConnect(afxHenvAllConnections, &m_hdbc));
	if (!Check(nRetCode))
	{
		ThrowDBException(nRetCode); // fatal
	}

	afxAllocatedConnections++;    // allocated at least
	ASSERT(m_hdbc != SQL_NULL_HDBC);

#ifdef _DEBUG
	if (bTraceSql)
	{
		::SQLSetConnectOption(m_hdbc, SQL_OPT_TRACEFILE,
			(DWORD)szOdbcCall);
		::SQLSetConnectOption(m_hdbc, SQL_OPT_TRACE, 1);
	}
#endif // _DEBUG

	AFX_SQL_SYNC(::SQLSetConnectOption(m_hdbc, SQL_LOGIN_TIMEOUT,
		m_dwLoginTimeout));
#ifdef _DEBUG
	if (nRetCode != SQL_SUCCESS && nRetCode != SQL_SUCCESS_WITH_INFO &&
		(afxTraceFlags & 0x20))
		TRACE0("Warning: Failure setting login timeout\n");
#endif // _DEBUG

	if (!m_bUpdatable)
	{
		AFX_SQL_SYNC(::SQLSetConnectOption(m_hdbc, SQL_ACCESS_MODE,
			SQL_MODE_READ_ONLY));
#ifdef _DEBUG
		if (nRetCode != SQL_SUCCESS && nRetCode != SQL_SUCCESS_WITH_INFO &&
			(afxTraceFlags & 0x20))
		{
			TRACE0("Warning: Failure setting read only access mode\n");
		}
#endif // _DEBUG
	}

}

//////////////////////////////////////////////////////////////////////////////
// CDatabase diagnostics

#ifdef _DEBUG
void CDatabase::AssertValid() const
{
	CObject::AssertValid();
}

void CDatabase::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	AFX_DUMP0(dc, "\nwith database:");
	AFX_DUMP1(dc, "\nm_hdbc = ", m_hdbc);
	AFX_DUMP1(dc, "\nm_strConnect = ", m_strConnect);
	AFX_DUMP1(dc, "\nm_bUpdatable = ", m_bUpdatable);
	AFX_DUMP1(dc, "\nm_bTransactions = ", m_bTransactions);
	AFX_DUMP1(dc, "\nm_bTransactionPending = ", m_bTransactionPending);
	AFX_DUMP1(dc, "\nm_dwLoginTimeout = ", m_dwLoginTimeout);
	AFX_DUMP1(dc, "\nm_dwQueryTimeout = ", m_dwQueryTimeout);
	AFX_DUMP1(dc, "\nm_bAsync = ", m_bAsync);

	if (dc.GetDepth() > 0)
	{
		AFX_DUMP0(dc, "\nwith env:");
		AFX_DUMP1(dc, "\nafxAllocatedConnections = ", afxAllocatedConnections);
		AFX_DUMP1(dc, "\nafxHenvAllConnections = ", afxHenvAllConnections);
	}
}

#endif //_DEBUG


//////////////////////////////////////////////////////////////////////////////
// CRecordset

IMPLEMENT_DYNAMIC(CRecordset, CObject)

CRecordset::CRecordset(CDatabase* pDatabase)
{
	ASSERT(pDatabase == NULL || AfxIsValidAddress(pDatabase, sizeof(CDatabase)));
	m_pDatabase = pDatabase;

	m_nOpenType = snapshot;
	m_nEditMode = noMode;

	m_bBOF = TRUE;
	m_bEOF = TRUE;
	m_bEOFSeen = FALSE;
	m_bDeleted = FALSE;
	m_bAppendable = FALSE;
	m_bExtendedFetch = FALSE;
	m_bUpdatable = FALSE;
	m_bScrollable = FALSE;
	m_bRecordsetDb = FALSE;
	m_nLockMode = optimistic;

	m_dwWait = 0;
	m_nFields = 0;
	m_nParams = 0;
	m_nFieldsBound = 0;
	m_lCurrentRecord = AFX_CURRENT_RECORD_UNDEFINED;
	m_lRecordCount = 0;

	m_pmemfile = NULL;
	m_par = NULL;
	m_pbFieldFlags = NULL;
	m_pbParamFlags = NULL;
	m_plFieldLength = NULL;
	m_plParamLength = NULL;
	m_pvFieldProxy = NULL;
	m_pvParamProxy = NULL;

	m_hstmtUpdate = SQL_NULL_HSTMT;
	m_hstmt = SQL_NULL_HSTMT;
	if (m_pDatabase != NULL && m_pDatabase->IsOpen())
	{
		ASSERT_VALID(m_pDatabase);
		TRY
		{
			RETCODE nRetCode;
			AFX_SQL_SYNC(::SQLAllocStmt(m_pDatabase->m_hdbc, &m_hstmt));
			if (!Check(nRetCode))
				ThrowDBException(SQL_INVALID_HANDLE);
		}
		CATCH_ALL(e)
		{
			ASSERT(m_hstmt == SQL_NULL_HSTMT);
		}
		END_CATCH_ALL
	}
}

CRecordset::~CRecordset()
{
	if (!m_bRecordsetDb)
		m_pDatabase = NULL;

	ASSERT_VALID(this);

	TRY
	{
		Close();
		if (m_bRecordsetDb)
		{
			delete m_pDatabase;
			m_pDatabase = NULL;
		}
	}
	CATCH_ALL(e)
	{
		// Nothing we can do
		TRACE0("Error: Exception ignored in ~CRecordset()\n");
	}
	END_CATCH_ALL
}

static char BASED_CODE szWhere[] = " WHERE ";
static char BASED_CODE szOrderBy[] = " ORDER BY ";
static char BASED_CODE szSelect[] = "SELECT ";
static char BASED_CODE szFrom[] = " FROM ";
static char BASED_CODE szForUpdateOf[] = " FOR UPDATE OF ";
BOOL CRecordset::Open(UINT nOpenType /* = snapshot */,
	LPCSTR lpszSQL /* = NULL */, DWORD dwOptions /* = none */)
{
	ASSERT_VALID(this);
	ASSERT(lpszSQL == NULL || AfxIsValidString(lpszSQL));
	ASSERT(nOpenType == dynaset || nOpenType == snapshot ||
		nOpenType == forwardOnly);
	ASSERT(dwOptions == 0 || (dwOptions & appendOnly) ||
		(dwOptions & readOnly) || (dwOptions & (appendOnly | optimizeBulkAdd)));

	m_nOpenType = nOpenType;
	m_dwOptions = dwOptions;
	m_bAppendable = (dwOptions & CRecordset::appendOnly) != 0 ||
		(dwOptions & CRecordset::readOnly) == 0;
	m_bUpdatable = (dwOptions & CRecordset::readOnly) == 0 &&
		(dwOptions & CRecordset::appendOnly) == 0;

	// Can't update forward only cursor
	ASSERT(!((nOpenType == forwardOnly) && m_bUpdatable));

	RETCODE nRetCode;
	if (m_hstmt == SQL_NULL_HSTMT)
	{
		CString strDefaultConnect;
		TRY
		{
			if (m_pDatabase == NULL)
			{
				m_pDatabase = new CDatabase();
				m_bRecordsetDb = TRUE;
			}

			strDefaultConnect = GetDefaultConnect();
			// If not already opened, attempt to open
			if (!m_pDatabase->IsOpen() &&
				!m_pDatabase->Open("", FALSE, FALSE, strDefaultConnect))
				return FALSE;

			AFX_SQL_SYNC(::SQLAllocStmt(m_pDatabase->m_hdbc, &m_hstmt));
			if (!Check(nRetCode))
				ThrowDBException(SQL_INVALID_HANDLE);
		}
		CATCH_ALL(e)
		{
#ifdef _DEBUG
			if (afxTraceFlags & 0x20)
				TRACE0("Error: CDatabase create for CRecordset failed\n");
#endif // _DEBUG
			strDefaultConnect.Empty();
			if (m_bRecordsetDb)
			{
				delete m_pDatabase;
				m_pDatabase = NULL;
			}
			ASSERT(m_hstmt == SQL_NULL_HSTMT);
			THROW_LAST();
		}
		END_CATCH_ALL
	}

	// Musn't open new recordset till any pending async ops have completed
	ASSERT(!m_pDatabase->InWaitForDataSource());

	CString strName;
	TRY
	{
		// Allocate flag and length arrays if not already
		if ((m_nFields != 0 && m_pbFieldFlags == NULL) ||
			(m_nParams != 0 && m_pbParamFlags == NULL))
			AllocFlags();

		OnSetOptions(m_hstmt);

		if (lpszSQL == NULL)
			m_strSQL = GetDefaultSQL();
		else
			m_strSQL = lpszSQL;

		// Set any supplied params
		if (m_nParams != 0)
		{
			UINT nParams = BindParams(m_hstmt);
			ASSERT(nParams == m_nParams);
		}

		// Check for SELECT keyword
		strName = m_strSQL.Left(sizeof(szSelect)-1);
		if (lstrcmpi(strName, szSelect) == 0)
		{
			// Find table name
			strName = m_strSQL;
			strName.MakeUpper();
			CString strFrom = szFrom;
			int nFrom = strName.Find(strFrom);
			LPCSTR lpsz = nFrom < 0 ? NULL : (LPCSTR)strName + nFrom;
			if (lpsz == NULL)
			{
				m_bAppendable = m_bUpdatable = FALSE;

#ifdef _DEBUG
				if (afxTraceFlags & 0x20)
					TRACE0("Warning: Missing 'FROM', recordset not updatable\n");
#endif // _DEBUG
			}
			else
			{
				// remove chars upto and including " FROM "
				strName = m_strSQL.Right(m_strSQL.GetLength() -
					(lpsz - (LPCSTR)strName + sizeof(szFrom)-1));

				// Search for table name
				for (char *pchName = strName.GetBuffer(0);
					*pchName > 0xD && *pchName != ',' && *pchName != ' ';
					pchName = _AfxAnsiNext(pchName))
					;

				// If list of names, we can't update
				if (*pchName == ',')
				{
					m_bAppendable = m_bUpdatable = FALSE;
#ifdef _DEBUG
					if (afxTraceFlags & 0x20)
						TRACE0("Warning: multi-table recordset not updatable\n");
#endif // _DEBUG
				}
				else
				{
					*pchName = '\0';
					// get table name
					m_strTableName = m_pDatabase->QuoteName(strName);
				}
				strName.ReleaseBuffer();
			}
		}
		else
		{
			// Parse for query procedure call keyword
			static char BASED_CODE szCall[] = "{call ";
			strName = m_strSQL.Left(sizeof(szCall)-1);
			if (lstrcmpi(strName, szCall) != 0)
			{
				for (char *pchName = m_strSQL.GetBuffer(0);
					*pchName > 0xD && *pchName != ',' && *pchName != ' ';
					pchName = _AfxAnsiNext(pchName))
					;

				// If list of names, we can't update
				if (*pchName == ',')
				{
					m_bAppendable = m_bUpdatable = FALSE;
					m_strTableName = m_strSQL;
#ifdef _DEBUG
					if (afxTraceFlags & 0x20)
						TRACE0("Warning: multi-table recordset not updatable\n");
#endif // _DEBUG
				}
				else
				{
					// Assume m_strSQL is a table name
					m_strTableName = m_pDatabase->QuoteName(m_strSQL);
				}

				m_strSQL.ReleaseBuffer();

				if (m_nFields == 0)
				{
					// no fields to bind to
#ifdef _DEBUG
					if (afxTraceFlags & 0x20)
						TRACE0("Warning: No output fields defined for recordset\n");
#endif // _DEBUG
					m_strSQL.Empty();
					m_bAppendable = m_bUpdatable = FALSE;
					m_bAppendable = FALSE;
					return TRUE;
				}
				else
				{
					// build query to retrieve table data
					BuildTableQuery();
				}
			}
		}
		strName.Empty();

		if (!m_strFilter.IsEmpty())
		{
			m_strSQL += szWhere;
			m_strSQL += m_strFilter;
		}

		if (!m_strSort.IsEmpty())
		{
			m_strSQL += szOrderBy;
			m_strSQL += m_strSort;
		}

		if (m_bUpdatable)
			m_strSQL += szForUpdateOf;

		// Prepare an hstmt with the query
		AFX_SQL_ASYNC(this, ::SQLPrepare(m_hstmt,
			(UCHAR FAR*)(const char *)m_strSQL, SQL_NTS));
		if (!Check(nRetCode))
			ThrowDBException(nRetCode);

		// now try to execute the SQL Query
		AFX_SQL_ASYNC(this, ::SQLExecute(m_hstmt));
		if (!Check(nRetCode))
			ThrowDBException(nRetCode);

		// Give derived classes a call before binding
		PreBindFields();

		MoveFirst();
	}
	CATCH_ALL(e)
	{
		strName.Empty();
		Close();
		THROW_LAST();
	}
	END_CATCH_ALL

	return TRUE;
}

void CRecordset::Close()
{
	ASSERT_VALID(this);
	// Can't close till all pending Async operations have completed
	ASSERT(!m_pDatabase->InWaitForDataSource());

	// This will force a requery for cursor name if reopened.
	m_strCursorName.Empty();

	m_nEditMode = noMode;

	delete m_par;
	m_par = NULL;

	delete m_pmemfile;
	m_pmemfile = NULL;

	delete m_pbFieldFlags;
	m_pbFieldFlags = NULL;

	delete m_pbParamFlags;
	m_pbParamFlags = NULL;

	if (m_pvFieldProxy != NULL)
	{
		// The first element is the count
		UINT nCount = *(UINT *)m_pvFieldProxy[0];
		for (UINT nField = 0; nField != nCount+1; nField++)
			delete m_pvFieldProxy[nField];

		delete m_pvFieldProxy;
		m_pvFieldProxy = NULL;
	}

	if (m_pvParamProxy != NULL)
	{
		// The first element is the count
		UINT nCount = *(UINT *)m_pvParamProxy[0];
		for (UINT nParam = 0; nParam != nCount+1; nParam++)
			delete m_pvParamProxy[nParam];

		delete m_pvParamProxy;
		m_pvParamProxy = NULL;
	}

	delete m_plFieldLength;
	m_plFieldLength = NULL;

	delete m_plParamLength;
	m_plParamLength = NULL;

	RETCODE nRetCode;
	if (m_hstmt != SQL_NULL_HSTMT)
	{
		// If database hasn't been deleted. . .
		if (m_pDatabase != SQL_NULL_HDBC)
		{
			AFX_SQL_SYNC(::SQLFreeStmt(m_hstmt, SQL_DROP));
		}
		m_hstmt = SQL_NULL_HSTMT;
	}

	if (m_hstmtUpdate != SQL_NULL_HSTMT)
	{
		// If database hasn't been deleted. . .
		if (m_pDatabase != SQL_NULL_HDBC)
		{
			AFX_SQL_SYNC(::SQLFreeStmt(m_hstmtUpdate, SQL_DROP));
		}
		m_hstmtUpdate = SQL_NULL_HSTMT;
	}

	m_bBOF = TRUE;
	m_bEOF = TRUE;
	m_bDeleted = FALSE;
	m_bAppendable = FALSE;
	m_bExtendedFetch = FALSE;
	m_bUpdatable = FALSE;
	m_bScrollable = FALSE;
	m_nLockMode = optimistic;

	m_dwWait = 0;
	m_nFieldsBound = 0;
}


BOOL CRecordset::IsFieldDirty(void* pv)
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	// If not in update op fields can't be dirty
	// must compare saved and current values
	if (m_nEditMode == noMode)
		return FALSE;

	ASSERT(m_pmemfile != NULL);

	// Must compare values to find dirty fields
	if (m_nEditMode == edit)
		MarkForUpdate();
	else
		MarkForAddNew();

	CFieldExchange fx(CFieldExchange::IsFieldDirty, this);
	fx.m_pvField = pv;

	DoFieldExchange(&fx);
	// pv not found: pv must be 'value' argument to an RFX call
	ASSERT(fx.m_bFieldFound);

	return fx.m_bField;
}

BOOL CRecordset::IsFieldNull(void* pv)
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	CFieldExchange fx(CFieldExchange::IsFieldNull, this);
	fx.m_pvField = pv;

	DoFieldExchange(&fx);
	// pv not found: pv must be 'value' argument to an RFX call
	ASSERT(fx.m_bFieldFound);

	return fx.m_bField;
}

BOOL CRecordset::IsFieldNullable(void* pv)
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	CFieldExchange fx(CFieldExchange::IsFieldNullable, this);
	fx.m_pvField = pv;

	DoFieldExchange(&fx);
	// pv not found: pv must be 'value' argument to an RFX call
	ASSERT(fx.m_bFieldFound);

	return fx.m_bField;
}

static char szDataTruncated[] = "State:01004";
static char szRowFetch[] = "State:01S01";
void CRecordset::Move(long lRows)
{
	RETCODE nRetCode;
	ASSERT_VALID(m_pDatabase);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	// First call - fields haven't been bound
	if (m_nFieldsBound == 0)
	{
		InitRecord();

		// First move completed
		return;
	}

	if (m_nFieldsBound > 0)
	{
		// Reset field flags - mark all clean, all non-null
		memset(m_pbFieldFlags, 0, m_nFields);
	}

	// Clear any edit mode that was set
	ReleaseCopyBuffer();

	if (lRows == 0)
		// Do nothing
		return;

	BOOL bForward = lRows > 0;
	if (!m_bScrollable && lRows != AFX_MOVE_NEXT)
		ThrowDBException(AFX_SQL_ERROR_RECORDSET_FORWARD_ONLY);

	WORD wFetchType;
	if (lRows == AFX_MOVE_FIRST)
	{
		wFetchType = SQL_FETCH_FIRST;
		lRows = 1;
	}
	else
	{
		if (lRows == AFX_MOVE_LAST)
		{
			wFetchType = SQL_FETCH_LAST;
			lRows = 1;
		}
		else
		{
			if (bForward)
			{
				// if already at EOF, throw an exception
				if (m_bEOF)
					ThrowDBException(AFX_SQL_ERROR_NO_DATA_FOUND);

				wFetchType = SQL_FETCH_NEXT;
			}
			else
			{
				// if already at BOF, throw an exception
				if (m_bBOF)
					ThrowDBException(AFX_SQL_ERROR_NO_DATA_FOUND);

				lRows = -lRows;
				wFetchType = SQL_FETCH_PREV;
			}
		}
	}

	DWORD dwRowsMoved = 1;
	WORD wStatus = SQL_ROW_SUCCESS;
	WORD wFetchNext = wFetchType;
	nRetCode = SQL_SUCCESS;
	// Skip deleted rows
	while (Check(nRetCode) && nRetCode != SQL_NO_DATA_FOUND && lRows != 0)
	{
		if (!m_bScrollable)
		{
			AFX_SQL_ASYNC(this, ::SQLFetch(m_hstmt));
		}
		else
		{
			AFX_SQL_ASYNC(this, ::SQLExtendedFetch(m_hstmt, wFetchNext, 0L,
				&dwRowsMoved, &wStatus));
		}

		if (wFetchType == SQL_FETCH_FIRST || wFetchType == SQL_FETCH_LAST)
		{
			// If doing MoveFirst/Last and first/last record is deleted, must do MoveNext/Prev
			wFetchNext = (WORD)((bForward)? SQL_FETCH_PREV: SQL_FETCH_NEXT);
		}

		m_bDeleted = (wStatus == SQL_ROW_DELETED);
		if (!m_bDeleted)
		{
			lRows--;
			if (nRetCode != SQL_NO_DATA_FOUND)
			{
				if (wFetchType == SQL_FETCH_FIRST)
				{
					m_lCurrentRecord = 0;
				}
				else if (wFetchType == SQL_FETCH_LAST)
				{
					if (m_bEOFSeen)
						m_lCurrentRecord = m_lRecordCount-1;
					else
						m_lRecordCount = m_lCurrentRecord = AFX_CURRENT_RECORD_UNDEFINED;
				}
				else if (m_lCurrentRecord != AFX_CURRENT_RECORD_UNDEFINED)
				{
					if (bForward)
						m_lCurrentRecord++;
					else
					{
						// If on deleted record or past end, current record already decremented
						if (!m_bDeleted && !m_bEOF)
							m_lCurrentRecord--;
					}
				}
			}
		}
	}

	if (nRetCode == SQL_SUCCESS_WITH_INFO)
	{
		CDBException e(nRetCode);
		e.BuildErrorString(m_pDatabase, m_hstmt);
		if (e.m_strStateNativeOrigin.Find(szRowFetch) >= 0)
		{
			e.Empty();
			TRACE0("Error: SQLExtendedFetch failed during Move operation.\n");
			ThrowDBException(AFX_SQL_ERROR_ROW_FETCH);
		}
		if (e.m_strStateNativeOrigin.Find(szDataTruncated) >= 0)
		{
			e.Empty();
			TRACE0("Error: field data truncated during Move operation.\n");
			ThrowDBException(AFX_SQL_ERROR_DATA_TRUNCATED);
		}
	}
	else
	{
		if (!Check(nRetCode))
			ThrowDBException(nRetCode);
	}

	if (nRetCode != SQL_NO_DATA_FOUND)
	{
		ASSERT(wStatus != SQL_ROW_NOROW && dwRowsMoved == 1);
		if (m_lCurrentRecord+1 > m_lRecordCount)
			m_lRecordCount = m_lCurrentRecord+1;
		Fixups();
		m_bBOF = FALSE;
		m_bEOF = FALSE;
		return;
	}

	// Only deleted records are left in set
	if (m_bDeleted)
	{
		m_bEOF = m_bBOF = m_bEOFSeen = TRUE;
		return;
	}

	// SQL_NO_DATA_FOUND
	if (bForward)
	{
		// hit end of set
		m_bEOF = TRUE;

		// If current record is known
		if (m_lCurrentRecord != AFX_CURRENT_RECORD_UNDEFINED)
		{
			m_bEOFSeen = TRUE;
			m_lRecordCount = m_lCurrentRecord+1;
		}
	}
	else
	{
		m_bBOF = TRUE;
		m_lCurrentRecord = AFX_CURRENT_RECORD_BOF;
	}
}

void CRecordset::AddNew()
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	// we can't construct an INSERT statement w/o any columns
	ASSERT(m_nFields != 0);

	if (!m_bAppendable)
		ThrowDBException(AFX_SQL_ERROR_RECORDSET_READONLY);

	if (m_nFieldsBound == 0)
	{
		m_nFieldsBound = BindFieldsToColumns();
		ASSERT(m_nFields == m_nFieldsBound);
	}

	if (m_nEditMode == noMode)
	{
		// First addnew call, cache record values
		StoreFields();
	}
	else
	{
		// subsequent Edit/AddNew call.  Restore values, save them again
		LoadFields();
		StoreFields();
	}
	SetFieldNull(NULL);
	SetFieldDirty(NULL, FALSE);

	m_nEditMode = addnew;
}

void CRecordset::Edit()
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	// we can't construct an UPDATE statement w/o any columns
	ASSERT(m_nFields != 0);

	if (!m_bUpdatable)
		ThrowDBException(AFX_SQL_ERROR_RECORDSET_READONLY);

	if (m_bEOF || m_bBOF || m_bDeleted)
		ThrowDBException(AFX_SQL_ERROR_NO_CURRENT_RECORD);

	if (m_nFieldsBound == 0)
	{
		m_nFieldsBound = BindFieldsToColumns();
		ASSERT(m_nFieldsBound == m_nFields);
	}

	if (m_nOpenType == dynaset && m_nLockMode == pessimistic)
	{
		RETCODE nRetCode;
		AFX_SQL_ASYNC(this, ::SQLSetPos(m_hstmt, 1, FALSE, SQL_LCK_EXCLUSIVE));
		if (!Check(nRetCode))
			ThrowDBException(nRetCode);
	}

	if (m_nEditMode == noMode)
	{
		// First edit call, cache record values
		StoreFields();
	}
	else
	{
		// subsequent Edit/AddNew call.  Restore values, save them again
		LoadFields();
		StoreFields();
	}

	m_nEditMode = edit;
}

BOOL CRecordset::Update()
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	if (m_nEditMode != addnew && m_nEditMode != edit)
		ThrowDBException(AFX_SQL_ERROR_ILLEGAL_MODE);
	return UpdateInsertDelete();
}

void CRecordset::Delete()
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	if (m_nEditMode != noMode)
		ThrowDBException(AFX_SQL_ERROR_ILLEGAL_MODE);
	UpdateInsertDelete();
}

void CRecordset::SetFieldDirty(void* pv, BOOL bDirty)
{
	ASSERT_VALID(this);

	CFieldExchange fx(CFieldExchange::SetFieldDirty, this);
	fx.m_pvField = pv;
	fx.m_bField = bDirty;
	DoFieldExchange(&fx);
	// pv not found: pv must be 'value' argument to an RFX call
	ASSERT(fx.m_bFieldFound);
}

void CRecordset::SetFieldNull(void* pv, BOOL bNull)
{
	ASSERT_VALID(this);

	CFieldExchange fx(CFieldExchange::SetFieldNull, this);
	fx.m_pvField = pv;
	fx.m_bField = bNull;
	DoFieldExchange(&fx);
	// pv not found: pv must be 'value' argument to an RFX call
	ASSERT(fx.m_bFieldFound);
}

void CRecordset::SetLockingMode(UINT nLockMode)
{
	ASSERT(IsOpen());
	if (nLockMode == pessimistic)
	{
		RETCODE nRetCode;
		UDWORD udTypes;
		SWORD cbResult;
		AFX_SQL_SYNC(::SQLGetInfo(m_pDatabase->m_hdbc, SQL_LOCK_TYPES,
			&udTypes, sizeof(udTypes), &cbResult));
		if (!Check(nRetCode) ||
			!(udTypes & SQL_LCK_EXCLUSIVE))
			ThrowDBException(AFX_SQL_ERROR_LOCK_MODE_NOT_SUPPORTED);
	}
	m_nLockMode = nLockMode;
}

BOOL CRecordset::Requery()
{
	RETCODE nRetCode;

	ASSERT_VALID(this);
	ASSERT(IsOpen());
	// Can't requery till all pending Async operations have completed
	ASSERT(!m_pDatabase->InWaitForDataSource());

	TRY
	{
		// Shutdown current query
		AFX_SQL_SYNC(::SQLFreeStmt(m_hstmt, SQL_CLOSE));

		// Rebind date/time parameters
		RebindDateParams(m_hstmt);

		// now attempt to reexecute the SQL Query
		AFX_SQL_ASYNC(this, ::SQLExecute(m_hstmt));
		if (!Check(nRetCode))
		{
			TRACE0("Error: Requery attempt failed.\n");
			ThrowDBException(nRetCode);
		}

		m_nFieldsBound = 0;
		InitRecord();
	}
	CATCH_ALL(e)
	{
		Close();
		THROW_LAST();
	}
	END_CATCH_ALL

	return TRUE;    // all set
}

// Shutdown any pending query for CRecordset's hstmt's
void CRecordset::Cancel()
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	::SQLCancel(m_hstmt);

	// If Update hstmt has been allocated, shut it down also
	if (m_hstmtUpdate != SQL_NULL_HSTMT)
		::SQLCancel(m_hstmtUpdate);
}

CString CRecordset::GetDefaultConnect()
{
	ASSERT_VALID(this);
	return lpszODBC;
}

static char szInfoRange[] = "State:S1096";
void CRecordset::OnSetOptions(HSTMT hstmt)
{
	ASSERT_VALID(this);
	ASSERT(hstmt != SQL_NULL_HSTMT);

	// Inherit options settings from CDatabase
	m_pDatabase->OnSetOptions(hstmt);

	// ODBC "cursor" is forwardOnly by default
	if (m_nOpenType == forwardOnly)
	{
		ASSERT(!m_bUpdatable);
		return;
	}

	RETCODE nRetCode;
	UWORD wDriverScrollSupport;
	// If SQLExtendedFetch not supported open forwardOnly (uses SQLFetch)
	AFX_SQL_SYNC(::SQLGetFunctions(m_pDatabase->m_hdbc,
		SQL_API_SQLEXTENDEDFETCH, &wDriverScrollSupport));
	if (!Check(nRetCode))
	{
		TRACE0("Error: ODBC failure determining whether recordset is scrollable.\n");
		ThrowDBException(nRetCode);
	}
	m_bScrollable = wDriverScrollSupport;
	if (!m_bScrollable)
	{
#ifdef _DEBUG
		if (afxTraceFlags & 0x20)
		{
			TRACE0("Warning: SQLExtendedFetch not supported by driver\n");
			TRACE0("and/or cursor library not loaded. Opening forwardOnly.\n");
		}
#endif // _DEBUG
		m_bUpdatable = FALSE;
		return;
	}

	char szResult[30];
	SWORD nResult;
	// Snapshots and dynasets require ODBC v2.0 or higher
	AFX_SQL_SYNC(::SQLGetInfo(m_pDatabase->m_hdbc, SQL_ODBC_VER,
		szResult, sizeof(szResult), &nResult));
	if (!Check(nRetCode))
	{
		TRACE0("Error: ODBC failure checking for driver capabilities.\n");
		ThrowDBException(nRetCode);
	}
	if (szResult[0] == '0' && szResult[1] < '2')
		ThrowDBException(AFX_SQL_ERROR_ODBC_V2_REQUIRED);

	// Determine scroll options supported by driver
	UDWORD dwDriverScrollOptions;
	AFX_SQL_SYNC(::SQLGetInfo(m_pDatabase->m_hdbc, SQL_SCROLL_OPTIONS,
		&dwDriverScrollOptions, sizeof(dwDriverScrollOptions), &nResult));

	UDWORD dwScrollOptions = SQL_CURSOR_FORWARD_ONLY;
	if (m_nOpenType == dynaset)
	{
		// Dnaset support requires ODBC's keyset driven model
		if (!(dwDriverScrollOptions & SQL_SO_KEYSET_DRIVEN))
			ThrowDBException(AFX_SQL_ERROR_DYNASET_NOT_SUPPORTED);
		dwScrollOptions = SQL_CURSOR_KEYSET_DRIVEN;

		// Make sure changes in row values on datasource can be detected
		AFX_SQL_SYNC(::SQLGetInfo(m_pDatabase->m_hdbc, SQL_ROW_UPDATES,
			szResult, sizeof(szResult), &nResult));
		if (!Check(nRetCode))
		{
			TRACE0("Error: ODBC failure checking for dynaset capabilities.\n");
			ThrowDBException(nRetCode);
		}
		if (!(szResult[0] == 'Y' || szResult[0] == 'y'))
			ThrowDBException(AFX_SQL_ERROR_ROW_UPDATE_NOT_SUPPORTED);
	}
	else
	{
		// Snapshot support requires ODBC's static cursor model
		if (!(dwDriverScrollOptions & SQL_SO_STATIC))
			ThrowDBException(AFX_SQL_ERROR_SNAPSHOT_NOT_SUPPORTED);
		dwScrollOptions = SQL_CURSOR_STATIC;
	}

	// Make sure positioned updates ('delete/update ... where current of ...') supported
	UDWORD dwDriverUpdateOptions;
	AFX_SQL_SYNC(::SQLGetInfo(m_pDatabase->m_hdbc, SQL_POSITIONED_STATEMENTS,
		&dwDriverUpdateOptions, sizeof(dwDriverUpdateOptions), &nResult));
	if(nRetCode != SQL_SUCCESS)
	{
		CDBException e(nRetCode);
		e.BuildErrorString(m_pDatabase, m_hstmt);
		if (e.m_strStateNativeOrigin.Find(szInfoRange) >= 0)
			// Driver doesn't support SQL_POSITIONED_STATEMENTS info option
			m_bUpdatable = FALSE;
		else
		{
			e.Empty();
			TRACE0("Error: determining if positioned updates supported.\n");
			ThrowDBException(nRetCode);
		}
	}
	else if (!((dwDriverUpdateOptions & SQL_PS_POSITIONED_DELETE) &&
		(dwDriverUpdateOptions & SQL_PS_POSITIONED_UPDATE)))
		m_bUpdatable = FALSE;

	UDWORD dwConcurrency = SQL_CONCUR_READ_ONLY;
	if (m_bUpdatable && m_pDatabase->m_bUpdatable)
	{
		UDWORD dwDriverConcurrencyOptions;
		AFX_SQL_SYNC(::SQLGetInfo(m_pDatabase->m_hdbc, SQL_SCROLL_CONCURRENCY,
			&dwDriverConcurrencyOptions, sizeof(dwDriverConcurrencyOptions), &nResult));
		if (!Check(nRetCode))
		{
			TRACE0("Error: ODBC failure checking recordset updatability.\n");
			ThrowDBException(nRetCode);
		}

		if (m_nLockMode == pessimistic)
		{
			if (dwDriverConcurrencyOptions & SQL_SCCO_LOCK)
				dwConcurrency = SQL_CONCUR_LOCK;
#ifdef _DEBUG
			else
			{
				if (afxTraceFlags & 0x20)
					TRACE0("Warning: locking not supported, setting recordset read only.\n");
			}
#endif // _DEBUG
		}
		else
		{
			// Use cheapest, most concur. model
			if (dwDriverConcurrencyOptions & SQL_SCCO_OPT_TIMESTAMP)
				dwConcurrency = SQL_CONCUR_TIMESTAMP;
			else if (dwDriverConcurrencyOptions & SQL_SCCO_OPT_VALUES)
				dwConcurrency = SQL_CONCUR_VALUES;
			else if (dwDriverConcurrencyOptions & SQL_SCCO_LOCK)
				dwConcurrency = SQL_CONCUR_LOCK;
		}
	}

	// Set cursor type (Let rowset size default to 1).
	AFX_SQL_SYNC(::SQLSetStmtOption(hstmt, SQL_CURSOR_TYPE, dwScrollOptions));
	if (!Check(nRetCode))
	{
		TRACE0("Error: ODBC failure setting recordset cursor type.\n");
		ThrowDBException(nRetCode);
	}

	// Set the concurrency model.
	AFX_SQL_SYNC(::SQLSetStmtOption(hstmt, SQL_CONCURRENCY, dwConcurrency));
	if (!Check(nRetCode))
	{
		TRACE0("Error: ODBC failure setting recordset concurrency.\n");
		ThrowDBException(nRetCode);
	}
}

void CRecordset::OnWaitForDataSource(BOOL bStillExecuting)
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	m_pDatabase->OnWaitForDataSource(bStillExecuting);
}


// Screen for errors.
BOOL CRecordset::Check(RETCODE nRetCode) const
{
	ASSERT_VALID(this);

	switch (nRetCode)
	{
	case SQL_SUCCESS_WITH_INFO:
#ifdef _DEBUG
		if (afxTraceFlags & 0x20)
		{
			CDBException e(nRetCode);
			TRACE0("Warning: ODBC Success With Info, ");
			e.BuildErrorString(m_pDatabase, m_hstmt);
		}
#endif // _DEBUG

		// Fall through

	case SQL_SUCCESS:
	case SQL_NO_DATA_FOUND:
	case SQL_NEED_DATA:
		return TRUE;
	}

	return FALSE;
}

void CRecordset::PreBindFields()
{
	// Do nothing
}

//////////////////////////////////////////////////////////////////////////////
// CRecordset internal functions

// Bind fields (if not already bound), then retrieve 1st record
void CRecordset::InitRecord()
{
	RETCODE nRetCode;

	// fields to bind
	if (m_nFields != 0)
	{
		m_nFieldsBound = BindFieldsToColumns();
		// m_nFields doesn't reflect number of
		// RFX_ output column calls in DoFieldExchange
		ASSERT(m_nFields == m_nFieldsBound);

		// Reset field flags - mark all clean, all non-null
		memset(m_pbFieldFlags, 0, m_nFields);
	}
	else
	{
		// No fields to bind, don't try to bind again
		m_nFieldsBound = -1;
	}

	ReleaseCopyBuffer();
	m_bEOFSeen = m_bBOF = m_bEOF = TRUE;
	m_bDeleted = FALSE;

	// prime the pump, retrieve first record
	if (!m_bScrollable)
	{
		AFX_SQL_ASYNC(this, ::SQLFetch(m_hstmt));
	}
	else
	{
		WORD wStatus;
		DWORD dwRowsMoved;

		AFX_SQL_ASYNC(this, ::SQLExtendedFetch(m_hstmt, SQL_FETCH_NEXT, 0L,
			&dwRowsMoved, &wStatus));
	}

	if (nRetCode == SQL_SUCCESS_WITH_INFO)
	{
		CDBException e(nRetCode);
		e.BuildErrorString(m_pDatabase, m_hstmt);
		if (e.m_strStateNativeOrigin.Find(szRowFetch) >= 0)
		{
			e.Empty();
			TRACE0("Error: SQLExtendedFetch failed during Move operation.\n");
			ThrowDBException(AFX_SQL_ERROR_ROW_FETCH);
		}
		if (e.m_strStateNativeOrigin.Find(szDataTruncated) >= 0)
		{
			e.Empty();
			TRACE0("Error: field data truncated during Move operation.\n");
			ThrowDBException(AFX_SQL_ERROR_DATA_TRUNCATED);
		}
	}
	else
	{
		if (!Check(nRetCode))
			ThrowDBException(nRetCode);
	}

	// if data found, bound fields now filled with first record
	if (nRetCode != SQL_NO_DATA_FOUND)
	{
		Fixups();
		m_bEOFSeen = m_bBOF = m_bEOF = FALSE;
		m_lCurrentRecord = 0;
		m_lRecordCount = 1;
	}
	else
	{
		// If recordset empty, set all values to NULL
		SetFieldNull(NULL);
	}
}

// "SELECT <user column name list> FROM <table name>"
void CRecordset::BuildTableQuery()
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	// If select statement we must have number of bound fields specified for us
	ASSERT(m_nFields != 0 && m_nFields <= 255);

	m_strSQL.Empty();
	m_strSQL = szSelect;
	// Set all fields dirty. AppendNames only outputs dirty field names
	SetFieldDirty(NULL);
	if (AppendNames(&m_strSQL, ",") == 0)
		ThrowDBException(AFX_SQL_ERROR_EMPTY_COLUMN_LIST);

	// Overwrite final ',' separator with ' '
	ASSERT(m_strSQL[m_strSQL.GetLength()-1] == ',');
	m_strSQL.SetAt(m_strSQL.GetLength()-1, ' ');

	m_strSQL += szFrom;
	m_strSQL += m_strTableName;
}

//////////////////////////////////////////////////////////////////////////////
// CRecordset RFX implementations

void CRecordset::AllocFlags()
{
	TRY
	{
		if (m_nFields != 0)
		{
			m_pbFieldFlags = new BYTE[m_nFields];
			memset(m_pbFieldFlags, 0, m_nFields);

			m_plFieldLength = new LONG[m_nFields];
			memset(m_plFieldLength, 0, m_nFields*sizeof(LONG));
		}

		if (m_nParams != 0)
		{
			m_pbParamFlags = new BYTE[m_nParams];
			memset(m_pbParamFlags, 0, m_nParams);

			m_plParamLength = new LONG[m_nParams];
			memset(m_plParamLength, 0, m_nParams*sizeof(LONG));
		}
	}
	CATCH_ALL(e)
	{
		Close();
		THROW_LAST();
	}
	END_CATCH_ALL
}

BYTE CRecordset::GetFieldFlags(UINT nField, UINT nFieldType /* = outputColumn */)
{
	ASSERT_VALID(this);
	ASSERT(nFieldType == CFieldExchange::outputColumn || nFieldType == CFieldExchange::param);

	if (nFieldType == CFieldExchange::outputColumn)
	{
		ASSERT(nField != 0 && nField <= m_nFields);
		if (m_pbFieldFlags == NULL)
			AllocFlags();

		return m_pbFieldFlags[nField-1];
	}
	else
	{
		ASSERT(nField != 0 && nField <= m_nParams);
		if (m_pbParamFlags == NULL)
			AllocFlags();

		return m_pbParamFlags[nField-1];
	}
}

void CRecordset::SetFieldFlags(UINT nField, BYTE bFlags, UINT nFieldType /* = outputColumn */)
{
	ASSERT_VALID(this);
	ASSERT(nFieldType == CFieldExchange::outputColumn || nFieldType == CFieldExchange::param);

	if (nFieldType == CFieldExchange::outputColumn)
	{
		ASSERT(nField != 0 && nField <= m_nFields);
		if (m_pbFieldFlags == NULL)
			AllocFlags();

		m_pbFieldFlags[nField-1] |= bFlags;
	}
	else
	{
		ASSERT(nField != 0 && nField <= m_nParams);
		if (m_pbParamFlags == NULL)
			AllocFlags();

		m_pbParamFlags[nField-1] |= bFlags;
	}
}

void CRecordset::ClearFieldFlags(UINT nField, BYTE bFlags, UINT nFieldType /* = outputColumn */)
{
	ASSERT_VALID(this);
	ASSERT(nFieldType == CFieldExchange::outputColumn || nFieldType == CFieldExchange::param);

	if (nFieldType == CFieldExchange::outputColumn)
	{
		ASSERT(nField != 0 && nField <= m_nFields);
		if (m_pbFieldFlags == NULL)
			AllocFlags();

		m_pbFieldFlags[nField-1] &= ~bFlags;
	}
	else
	{
		ASSERT(nField != 0 && nField <= m_nParams);
		if (m_pbParamFlags == NULL)
			AllocFlags();

		m_pbParamFlags[nField-1] &= ~bFlags;
	}
}

UINT CRecordset::BindParams(HSTMT hstmt)
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	CFieldExchange fx(CFieldExchange::BindParam, this);
	fx.m_hstmt = hstmt;

	DoFieldExchange(&fx);

	return fx.m_nParams;
}

void CRecordset::RebindDateParams(HSTMT hstmt)
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	CFieldExchange fx(CFieldExchange::RebindDateParam, this);
	fx.m_hstmt = hstmt;

	DoFieldExchange(&fx);
}

UINT CRecordset::BindFieldsToColumns()
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	ASSERT(m_nFieldsBound == 0);
	ASSERT(m_nFields != 0 && m_nFields <= 255);

	CFieldExchange fx(CFieldExchange::BindFieldToColumn, this);
	fx.m_hstmt = m_hstmt;
	DoFieldExchange(&fx);
	return fx.m_nFields;
}

// After Move operation, reflect status and lengths of columns in RFX fields
void CRecordset::Fixups()
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	ASSERT(m_nFieldsBound != 0);
	CFieldExchange fx(CFieldExchange::Fixup, this);
	fx.m_hstmt = m_hstmt;
	DoFieldExchange(&fx);
}

UINT CRecordset::AppendNames(CString* pstr, LPCSTR lpszSeparator)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(pstr, sizeof(CString)));
	ASSERT(AfxIsValidString(lpszSeparator));
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	CFieldExchange fx(CFieldExchange::Name, this);
	fx.m_pstr = pstr;
	fx.m_lpszSeparator = lpszSeparator;

	DoFieldExchange(&fx);

	return fx.m_nFields;
}

LONG* CRecordset::GetFieldLength(CFieldExchange* pFX)
{
	ASSERT_VALID(this);

	if (pFX->m_nFieldType == CFieldExchange::outputColumn)
	{
		ASSERT(pFX->m_nFields != 0  && pFX->m_nFields <= m_nFields);
		if (m_pbFieldFlags == NULL)
			AllocFlags();

		return &m_plFieldLength[pFX->m_nFields-1];
	}
	else
	{

		ASSERT(pFX->m_nParams != 0 && pFX->m_nParams <= m_nParams);
		if (m_pbParamFlags == NULL)
			AllocFlags();

		return &m_plParamLength[pFX->m_nParams-1];
	}
}

// For each "changed" column, append <column name>=<column value>,
UINT CRecordset::AppendNamesValues(HSTMT hstmt, CString* pstr,
	LPCSTR lpszSeparator)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(pstr, sizeof(CString)));
	ASSERT(AfxIsValidString(lpszSeparator));
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	ASSERT(hstmt != SQL_NULL_HSTMT);

	CFieldExchange fx(CFieldExchange::NameValue, this);
	fx.m_pstr = pstr;
	fx.m_lpszSeparator = lpszSeparator;
	fx.m_hstmt = hstmt;

	DoFieldExchange(&fx);

	return fx.m_nFields;
}

// For each "changed" column, append <column value>,
UINT CRecordset::AppendValues(HSTMT hstmt, CString* pstr,
	LPCSTR lpszSeparator)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(pstr, sizeof(CString)));
	ASSERT(AfxIsValidString(lpszSeparator));
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	ASSERT(hstmt != SQL_NULL_HSTMT);

	CFieldExchange fx(CFieldExchange::Value, this);
	fx.m_pstr = pstr;
	fx.m_lpszSeparator = lpszSeparator;
	fx.m_hstmt = hstmt;

	DoFieldExchange(&fx);

	return fx.m_nFields;
}


// Cache fields of copy buffer in a CMemFile with CArchive
void CRecordset::StoreFields()
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	ASSERT(m_nFieldsBound != 0);
	CFieldExchange fx(CFieldExchange::StoreField, this);
	// could be left around if no call to Update after AddNew/Edit
	delete m_par;
	m_par = NULL;
	delete m_pmemfile;
	m_pmemfile = NULL;

	m_pmemfile = new CMemFile();
	TRY
	{
		fx.m_par = m_par = new CArchive(m_pmemfile, CArchive::store);
		DoFieldExchange(&fx);
		m_par->Close();
	}
	CATCH_ALL(e)
	{
		delete m_par;
		m_par = NULL;
		delete m_pmemfile;
		m_pmemfile = NULL;

		THROW_LAST();
	}
	END_CATCH_ALL
}

// Restore fields of copy buffer from archived memory file
void CRecordset::LoadFields()
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	ASSERT(m_nFieldsBound != 0);
	CFieldExchange fx(CFieldExchange::LoadField, this);
	ASSERT(m_pmemfile != NULL && m_par != NULL);
	m_pmemfile->SeekToBegin();

	// free 'store' archive, allocate 'load' archive
	// (do here instead of StoreFields to re-use of alloc'd memory)
	delete m_par;
	m_par = NULL;
	m_par = new CArchive(m_pmemfile, CArchive::load);

	fx.m_par = m_par;
	// Clear flags
	memset(m_pbFieldFlags, 0, m_nFields);
	DoFieldExchange(&fx);

	// free archive and cache memory
	delete m_par;
	m_par = NULL;
	delete m_pmemfile;
	m_pmemfile = NULL;
}

void CRecordset::MarkForUpdate()
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	// Must have already stored field values in memfile
	ASSERT(m_pmemfile != NULL);

	CFieldExchange fx(CFieldExchange::MarkForUpdate, this);
	m_pmemfile->SeekToBegin();

	ASSERT(m_par != NULL);
	delete m_par;
	m_par = NULL;
	m_par = new CArchive(m_pmemfile, CArchive::load);
	fx.m_par = m_par;

	DoFieldExchange(&fx);
}

void CRecordset::MarkForAddNew()
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	CFieldExchange fx(CFieldExchange::MarkForAddNew, this);
	DoFieldExchange(&fx);
}

BOOL CRecordset::GetFieldInfo(void* pv, CFieldInfo* pfi)
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	// Use frame variable if user doesn't supply an fi
	CFieldInfo fi;
	if (pfi == NULL)
		pfi = &fi;

	ASSERT(AfxIsValidAddress(pfi, sizeof(CFieldInfo)));
	CFieldExchange fx(CFieldExchange::GetFieldInfoValue, this);
	pfi->pv = pv;
	fx.m_pfi = pfi;

	DoFieldExchange(&fx);

	return fx.m_bFieldFound;
}

BOOL CRecordset::GetFieldInfo(UINT nField, CFieldInfo* pfi)
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	// Use frame variable if user doesn't supply an fi
	CFieldInfo fi;
	if (pfi == NULL)
		pfi = &fi;

	ASSERT(AfxIsValidAddress(pfi, sizeof(CFieldInfo)));
	if (nField >= m_nFields)
	{
		return UnboundFieldInfo(nField, pfi);
	}

	CFieldExchange fx(CFieldExchange::GetFieldInfoOrdinal, this);
	pfi->nField = nField;
	fx.m_pfi = pfi;

	DoFieldExchange(&fx);

	return fx.m_bFieldFound;
}

// Perform Update (m_nModeEdit == edit), Insert (addnew),
// or Delete (noMode)
BOOL CRecordset::UpdateInsertDelete()
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	// Can't close till all pending Async operations have completed
	ASSERT(!m_pDatabase->InWaitForDataSource());

	long lRowsAffected = 0;

	// Delete mode
	if (m_nEditMode == addnew)
	{
		if (!m_bAppendable)
			ThrowDBException(AFX_SQL_ERROR_RECORDSET_READONLY);
	}
	else
	{
		if (!m_bUpdatable)
		{
			ThrowDBException(AFX_SQL_ERROR_RECORDSET_READONLY);
		}

		// Requires currency
		if (m_bEOF || m_bBOF || m_bDeleted)
		{
			ThrowDBException(AFX_SQL_ERROR_NO_CURRENT_RECORD);
		}
	}

	// Update or AddNew is NOP w/o at least 1 changed field
	if (m_nEditMode != noMode && !IsFieldDirty(NULL))
	{
		return FALSE;
	}

	// Use SQLPrepare & SQLExecute if optimizing for multiple calls to AddNew
	BOOL bNullHstmt = (m_hstmtUpdate == SQL_NULL_HSTMT);

	RETCODE nRetCode;
	if (m_hstmtUpdate == SQL_NULL_HSTMT)
	{
		AFX_SQL_SYNC(::SQLAllocStmt(m_pDatabase->m_hdbc, &m_hstmtUpdate));
		if (!Check(nRetCode))
		{
			AfxThrowDBException(nRetCode, m_pDatabase, m_hstmtUpdate);
		}
	}
	else
	{
		AFX_SQL_SYNC(::SQLFreeStmt(m_hstmtUpdate, SQL_CLOSE));
		if (!Check(nRetCode))
			goto LErrRetCode;

		// Re-use (prepared) hstmt & param binding if optimizeBulkAdd option
		if (!(m_dwOptions & optimizeBulkAdd))
		{
			AFX_SQL_SYNC(::SQLFreeStmt(m_hstmtUpdate, SQL_RESET_PARAMS));
			if (!Check(nRetCode))
			{
	LErrRetCode:
				// Bad hstmt, free it and try allocating another one
				AFX_SQL_SYNC(::SQLFreeStmt(m_hstmtUpdate, SQL_DROP));
				m_hstmtUpdate = SQL_NULL_HSTMT;

				AFX_SQL_SYNC(::SQLAllocStmt(m_pDatabase->m_hdbc, &m_hstmtUpdate));
				if (!Check(nRetCode))
				{
					AfxThrowDBException(nRetCode, m_pDatabase, m_hstmtUpdate);
				}
			}
		}
	}

	if (!(m_dwOptions & optimizeBulkAdd) || bNullHstmt)
		OnSetOptions(m_hstmtUpdate);

	CString strSQL;
	TRY
	{
		static char BASED_CODE szComma[] = ",";
		
		if (!(m_dwOptions & optimizeBulkAdd) || bNullHstmt)
		{
			switch (m_nEditMode)
			{
			case noMode:
				// DELETE FROM <tablename> WHERE CURRENT OF
				{
					static char BASED_CODE szDelete[] = "DELETE FROM ";
					strSQL = szDelete;
					strSQL += m_strTableName;
				}
				break;

			case addnew:
				// INSERT INTO <tablename> (<colname1>[,<colname2>]) VALUES (?[,?])

				{
					// Must be first bulk add, temporarily set options.
					if(m_dwOptions & optimizeBulkAdd)
					{
						m_dwOptions &= ~optimizeBulkAdd;
						m_dwOptions |= firstBulkAdd;
					}
		
					static char BASED_CODE szInsertInto[] = "INSERT INTO ";
					strSQL = szInsertInto;

					strSQL += m_strTableName;

					static char BASED_CODE szLeftParen[] = " (";
					strSQL += szLeftParen;

					// Append column names
					AppendNames(&strSQL, szComma);

					// overwrite last ',' with ')'
					ASSERT(strSQL[strSQL.GetLength()-1] == ',');
					strSQL.SetAt(strSQL.GetLength()-1, ')');

					static char BASED_CODE szValuesParen[] = " VALUES (";
					strSQL += szValuesParen;

					// Append values
					AppendValues(m_hstmtUpdate, &strSQL, szComma);

					// overwrite last ',' with ')'
					ASSERT(strSQL[strSQL.GetLength()-1] == ',');
					strSQL.SetAt(strSQL.GetLength()-1, ')');

					// Must be first bulk add, reset options.
					if(m_dwOptions & firstBulkAdd)
					{
						m_dwOptions &= ~firstBulkAdd;
						m_dwOptions |= optimizeBulkAdd;
					}
				}
				break;

			case edit:
				// UPDATE <tablename> SET <colname1>=?[,<colname2>=?] WHERE CURRENT OF

				{
					static char BASED_CODE szUpdate[] = "UPDATE ";
					strSQL = szUpdate;
					strSQL += m_strTableName;

					static char BASED_CODE szSet[] = " SET ";
					strSQL += szSet;

					AppendNamesValues(m_hstmtUpdate, &strSQL, szComma);

					// overwrite last ',' with ' '
					ASSERT(strSQL[strSQL.GetLength()-1] == ',');
					strSQL.SetAt(strSQL.GetLength()-1, ' ');
				}
				break;
			}

			// Update and Delete need "WHERE CURRENT OF <cursorname>"
			if (m_nEditMode == edit || m_nEditMode == noMode)
			{
				static char BASED_CODE szWhereCurrentOf[] = " WHERE CURRENT OF ";
				strSQL += szWhereCurrentOf;

				// Cache cursor name assigned by ODBC
				if (m_strCursorName.IsEmpty())
				{
					// Get predefined cursor name from datasource
					char szCursorName[MAX_CURSOR_NAME+1];
					int cbLength = sizeof(szCursorName)-1;
					AFX_SQL_SYNC(::SQLGetCursorName(m_hstmt, (UCHAR FAR*)szCursorName,
						sizeof(szCursorName), (short FAR*)&cbLength));
					if (!Check(nRetCode))
						ThrowDBException(nRetCode);
					m_strCursorName = szCursorName;
				}
				strSQL += m_strCursorName;
			}
		}
		else
		{
			// Make sure length array is correct and set datetime proxy values
			AppendValues(m_hstmtUpdate, &strSQL, szComma);
		}

		if (!(m_dwOptions & optimizeBulkAdd))
		{
			AFX_SQL_ASYNC(this, ::SQLExecDirect(m_hstmtUpdate, (UCHAR FAR*)(const char *)strSQL, SQL_NTS));
			if (!Check(nRetCode))
				ThrowDBException(nRetCode, m_hstmtUpdate);
		}
		else
		{
			// This presumes that dirty fields do not change
			if (bNullHstmt)
			{
				AFX_SQL_ASYNC(this, ::SQLPrepare(m_hstmtUpdate, (UCHAR FAR*)(const char *)strSQL, SQL_NTS));
				if (!Check(nRetCode))
					ThrowDBException(nRetCode, m_hstmtUpdate);
			}

			AFX_SQL_ASYNC(this, ::SQLExecute(m_hstmtUpdate));
			if (!Check(nRetCode))
				ThrowDBException(nRetCode, m_hstmtUpdate);
		}

		while (nRetCode == SQL_NEED_DATA)
		{
			void FAR* pv;
			AFX_SQL_ASYNC(this, ::SQLParamData(m_hstmtUpdate, &pv));
			if (!Check(nRetCode))
			{
				// Cache away error
				CDBException* pException = new CDBException(nRetCode);
				pException->BuildErrorString(m_pDatabase, m_hstmtUpdate);
				// Then cancel Execute operation
				Cancel();
				THROW(pException);
			}

			while (nRetCode == SQL_NEED_DATA)
			{
				CLongBinary* pLongBinary = (CLongBinary*)_AfxGetPtrFromFarPtr(pv);
				ASSERT_VALID(pLongBinary);

				const BYTE _huge* lpData = (const BYTE _huge*)::GlobalLock(pLongBinary->m_hData);
				ASSERT(lpData != NULL);

				DWORD dwDataLength = 0;
				while (dwDataLength != pLongBinary->m_dwDataLength)
				{
					DWORD dwSend = pLongBinary->m_dwDataLength-dwDataLength;
					if (dwSend > 0x8000)
						dwSend = 0x8000;
					AFX_SQL_ASYNC(this, ::SQLPutData(m_hstmtUpdate, (PTR)lpData, dwSend));
					if (!Check(nRetCode))
					{
						::GlobalUnlock(pLongBinary->m_hData);
						// Cache away error
						CDBException* pException = new CDBException(nRetCode);
						pException->BuildErrorString(m_pDatabase, m_hstmtUpdate);
						// Then cancel Execute operation
						Cancel();
						THROW(pException);
					}
					lpData += dwSend;
					dwDataLength += dwSend;
				}
				// Check for another DATA_AT_EXEC
				AFX_SQL_ASYNC(this, ::SQLParamData(m_hstmtUpdate, &pv));
				::GlobalUnlock(pLongBinary->m_hData);
				if (!Check(nRetCode))
					ThrowDBException(nRetCode, m_hstmtUpdate);
			}
		}

		AFX_SQL_SYNC(::SQLRowCount(m_hstmtUpdate, &lRowsAffected));
		if (!Check(nRetCode) || lRowsAffected == -1)
			// Assume 1 row affected if # rows affected can't be determined
			lRowsAffected = 1;
		else
		{
			if (lRowsAffected != 1)
			{
#ifdef _DEBUG
				if (afxTraceFlags & 0x20)
					TRACE1("Warning: %u rows affected by update operation (expected 1).\n",
						lRowsAffected);
#endif // _DEBUG
				ThrowDBException(lRowsAffected == 0?
					AFX_SQL_ERROR_NO_ROWS_AFFECTED :
					AFX_SQL_ERROR_MULTIPLE_ROWS_AFFECTED);
			}
		}
	}
	CATCH_ALL(e)
	{
		strSQL.Empty();
		THROW_LAST();
	}
	END_CATCH_ALL

	strSQL.Empty();

	TRY
	{
		// Delete
		switch (m_nEditMode)
		{
		case noMode:
			// Decrement record count
			if (m_lCurrentRecord > 0)
			{
				if (m_lRecordCount > 0)
					m_lRecordCount--;
				m_lCurrentRecord--;
			}

			// indicate on a deleted record
			m_bDeleted = TRUE;
			// Set all fields to NULL
			SetFieldNull(NULL);
			break;

		case addnew:
			if (m_lCurrentRecord >= 0)
			{
				if (m_lRecordCount != -1)
					m_lRecordCount++;
				m_lCurrentRecord++;
			}
			// Fall through

		case edit:
			// Update, AddNew
			ReleaseCopyBuffer();
			break;
		}
	}
	CATCH_ALL(e)
	{
		// Fall through - must return TRUE since record updated
	}
	END_CATCH_ALL

	return TRUE;
}

void CRecordset::ReleaseCopyBuffer()
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	switch (m_nEditMode)
	{
	// Update
	case edit:
		// keep updated values
		// free archive and cache memory
		delete m_par;
		m_par = NULL;
		delete m_pmemfile;
		m_pmemfile = NULL;
		break;

	// Insert
	case addnew:
		// Restore copy buffer to pre-AddNew values
		// regardless of success of Insert operation
		LoadFields();
		break;

	// Delete
	case noMode:
		// no copy buffer to release on a delete call
		break;
	}
	m_nEditMode = noMode;
}

// Field is beyond bound columns, get info field directly from data source
BOOL CRecordset::UnboundFieldInfo(UINT nField, CFieldInfo* pfi)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(pfi, sizeof(CFieldInfo)));
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	pfi->nField = nField;
	nField++;

	// Make sure nField falls within number of columns in the result set
	UINT nResultColumns;
	RETCODE nRetCode;
	AFX_SQL_ASYNC(this, ::SQLNumResultCols(m_hstmt, (short FAR*)&nResultColumns));
	if (nField > nResultColumns)
		return FALSE;

	char szName[65];
	int cbName = sizeof(szName);
	int nSqlType;
	int nScale;
	UINT nNullable;
	AFX_SQL_ASYNC(this, ::SQLDescribeCol(m_hstmt, (unsigned short int)nField,
		(UCHAR*)szName, sizeof(szName), (short FAR*)&cbName, (short FAR*)&nSqlType,
		&pfi->dwSize, (short FAR*)&nScale, (short FAR*)&nNullable));
	if (!Check(nRetCode))
		return FALSE;

	pfi->strName = szName;
	pfi->pv = NULL;

	switch (nSqlType)
	{
	case SQL_BIT:
		pfi->nDataType = AFX_RFX_BOOL;
		break;

	case SQL_TINYINT:
		pfi->nDataType = AFX_RFX_BYTE;
		break;

	case SQL_SMALLINT:
		pfi->nDataType = AFX_RFX_INT;
		break;

	case SQL_INTEGER:
		pfi->nDataType = AFX_RFX_LONG;
		break;

	case SQL_REAL:
		pfi->nDataType = AFX_RFX_SINGLE;
		break;

	case SQL_FLOAT:
	case SQL_DOUBLE:
		pfi->nDataType = AFX_RFX_DOUBLE;
		break;

	case SQL_DATE:
	case SQL_TIME:
	case SQL_TIMESTAMP:
		pfi->nDataType = AFX_RFX_DATE;
		break;

	case SQL_BINARY:
	case SQL_VARBINARY:
		pfi->nDataType = AFX_RFX_BINARY;
		break;

	case SQL_DECIMAL:   // ODBC default xfer type
	case SQL_NUMERIC:   // ODBC default xfer type
	case SQL_CHAR:
	case SQL_VARCHAR:
		pfi->nDataType = AFX_RFX_TEXT;
		break;

	case SQL_LONGVARCHAR:
	case SQL_LONGVARBINARY:
		pfi->nDataType = AFX_RFX_LONGBINARY;
		break;

	default:
		ASSERT(FALSE);

	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// CRecordset diagnostics

#ifdef _DEBUG
void CRecordset::AssertValid() const
{
	CObject::AssertValid();
	if (m_pDatabase != NULL)
		m_pDatabase->AssertValid();
}

void CRecordset::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
	AFX_DUMP0(dc, "\nwith recordset:");
	AFX_DUMP1(dc, "\nm_nOpenType = ", m_nOpenType);
	AFX_DUMP1(dc, "\nm_strSQL = ", m_strSQL);
	AFX_DUMP1(dc, "\nm_hstmt = ", m_hstmt);
	AFX_DUMP1(dc, "\nm_bRecordsetDb = ", m_bRecordsetDb);

	AFX_DUMP1(dc, "\nm_bScrollable = ", m_bScrollable);
	AFX_DUMP1(dc, "\nm_bUpdatable = ", m_bUpdatable);
	AFX_DUMP1(dc, "\nm_bAppendable = ", m_bAppendable);

	AFX_DUMP1(dc, "\nm_nFields = ", m_nFields);
	AFX_DUMP1(dc, "\nm_nFieldsBound = ", m_nFieldsBound);
	AFX_DUMP1(dc, "\nm_nParams = ", m_nParams);

	AFX_DUMP1(dc, "\nm_bEOF = ", m_bEOF);
	AFX_DUMP1(dc, "\nm_bBOF = ", m_bBOF);
	AFX_DUMP1(dc, "\nm_bDeleted = ", m_bEOF);

	AFX_DUMP1(dc, "\nm_bLockMode = ", m_nLockMode);
	AFX_DUMP1(dc, "\nm_nEditMode = ", m_nEditMode);
	AFX_DUMP1(dc, "\nm_strCursorName = ", m_strCursorName);
	AFX_DUMP1(dc, "\nm_hstmtUpdate = ", m_hstmtUpdate);

	if (dc.GetDepth() > 0)
	{
		if (m_pDatabase == NULL)
			AFX_DUMP0(dc, "\nwith no database");
		else
			m_pDatabase->Dump(dc);
	}
}

#endif //_DEBUG


//////////////////////////////////////////////////////////////////////////////
// Inline function declarations expanded out-of-line

#ifndef _AFX_ENABLE_INLINES

static char BASED_CODE _szAfxDbInl[] = "afxdb.inl";
#undef THIS_FILE
#define THIS_FILE _szAfxDbInl
#define _AFXDBCORE_INLINE
#include "afxdb.inl"

#endif

/////////////////////////////////////////////////////////////////////////////
