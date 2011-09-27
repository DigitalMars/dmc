// bulkset.cpp : implementation file
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
#include "dbfetch.h"
#include "bulkset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBulkRecordsetMod

IMPLEMENT_DYNAMIC(CBulkRecordsetMod, CRecordset)

CBulkRecordsetMod::CBulkRecordsetMod(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CBulkRecordsetMod)
	//}}AFX_FIELD_INIT
	m_nDefaultType = dynaset;
}


BOOL CBulkRecordsetMod::Open(UINT nOpenType,
	LPCTSTR lpszSQL, DWORD dwOptions)
{
	ASSERT(dwOptions & useMultiRowFetch);
	return CRecordset::Open(nOpenType, lpszSQL, dwOptions);
}

BOOL CBulkRecordsetMod::RowsetUpdate(WORD wRow, WORD wLockType)
{
	ASSERT(wRow >= 0 && wRow <= GetRowsetSize());

	RETCODE nRetCode;
	AFX_ODBC_CALL(::SQLSetPos(m_hstmt, wRow, SQL_UPDATE, wLockType));

	return ValidateMod(wRow, SQL_ROW_UPDATED);
}

BOOL CBulkRecordsetMod::RowsetAdd(WORD wRow, WORD wLockType)
{
	// User may allocate an extra row buffer for the Add
	// (if user adds more than 1, must override)
	ASSERT(wRow >= 0 && wRow <= GetRowsetSize() + 1);

	RETCODE nRetCode;
	AFX_ODBC_CALL(::SQLSetPos(m_hstmt, wRow, SQL_ADD, wLockType));

	return ValidateMod(wRow, SQL_ROW_ADDED);
}

BOOL CBulkRecordsetMod::RowsetDelete(WORD wRow, WORD wLockType)
{
	ASSERT(wRow >= 0 && wRow <= GetRowsetSize());

	RETCODE nRetCode;
	AFX_ODBC_CALL(::SQLSetPos(m_hstmt, wRow, SQL_DELETE, wLockType));

	return ValidateMod(wRow, SQL_ROW_DELETED);
}

BOOL CBulkRecordsetMod::ValidateMod(WORD wRow, WORD wExpectedStatus)
{
	BOOL bReturn = TRUE;

	if (wRow != 0)
		bReturn = GetRowStatus(wRow) == wExpectedStatus;
	else
	{
		for (WORD wNum = 1; wNum <= GetRowsetSize(); wNum++)
		{
			// If any row status not expected, then validate fails
			if (GetRowStatus(wNum) != wExpectedStatus)
				bReturn = FALSE;
		}
	}

	return bReturn;
}

/////////////////////////////////////////////////////////////////////////////
// CDynamicBulkSet

IMPLEMENT_DYNAMIC(CDynamicBulkSet, CBulkRecordsetMod)

CDynamicBulkSet::CDynamicBulkSet(CDatabase* pdb)
	: CBulkRecordsetMod(pdb)
{
	m_nDefaultType = dynaset;
	m_nAllocatedFields = 0;

	m_ppvData = NULL;
	m_ppvLengths = NULL;
}

void CDynamicBulkSet::Close()
{
	CRecordset::Close();

	delete [] m_ppvData;
	delete [] m_ppvLengths;
}

void CDynamicBulkSet::DoBulkFieldExchange(CFieldExchange* pFX)
{
	// Allocate the buffer
	if (pFX->m_nOperation == CFieldExchange::AllocMultiRowBuffer &&
		m_nAllocatedFields == 0)
	{
		// get the field count
		m_ppvData = new void*[GetODBCFieldCount()];
		memset(m_ppvData, 0, sizeof(void*) * GetODBCFieldCount());
		m_ppvLengths = new void*[GetODBCFieldCount()];
		memset(m_ppvLengths, 0, sizeof(void*) * GetODBCFieldCount());

		m_nAllocatedFields = GetODBCFieldCount();
		m_nFields = m_nAllocatedFields;
	}

	// Should never get to here without field buffer allocation
	ASSERT(m_nAllocatedFields != 0);

	pFX->SetFieldType(CFieldExchange::outputColumn);
	for (int nNum = 0; nNum < GetODBCFieldCount(); nNum++)
	{
		RFX_Text_Bulk(pFX, _T("Dummy"), (LPSTR*)&m_ppvData[nNum],
			(long**)&m_ppvLengths[nNum], MAX_TEXT_LEN);
	}
}

void CDynamicBulkSet::CheckRowsetError(RETCODE nRetCode)
{
	// Always IGNORE data truncated warnings, as the code
	// purposely truncates text to 40 chars...

	if (nRetCode == SQL_SUCCESS_WITH_INFO)
	{
		CDBException e(nRetCode);
		// Build the error string but don't send nuisance output to TRACE window
		e.BuildErrorString(m_pDatabase, m_hstmt, FALSE);

		if (e.m_strStateNativeOrigin.Find(_T("State:01004")) >= 0)
		{
			// Do nothing here for this app
		}
		else if (e.m_strStateNativeOrigin.Find(_T("State:01S01")) >= 0)
		{
			e.Empty();
			TRACE0("Error: fetching row from server.\n");
			ThrowDBException(AFX_SQL_ERROR_ROW_FETCH);
		}
		else
		{
#ifdef _DEBUG
			// Not a truncation or row fetch warning so send debug output
			if (afxTraceFlags & traceDatabase)
			{
				TRACE0("Warning: ODBC Success With Info, ");
				e.TraceErrorMessage(e.m_strError);
				e.TraceErrorMessage(e.m_strStateNativeOrigin);
			}
#endif // _DEBUG
		}
	}
	else if (!Check(nRetCode))
		ThrowDBException(nRetCode);
}

/////////////////////////////////////////////////////////////////////////////
// CTables - borrowed from Catalog2

CTables::CTables(CDatabase* pDatabase)
	: CRecordset(pDatabase)
{
	m_strTableQualifier	= _T("");
	m_strTableOwner		= _T("");
	m_strTableName		= _T("");
	m_strTableType		= _T("");
	m_strRemarks		= _T("");
	m_nFields = 5;
}

BOOL CTables::Open(LPCSTR pszTableQualifier,
	LPCSTR pszTableOwner,LPCSTR pszTableName,LPCSTR pszTableType,
	UINT nOpenType)
{
	ASSERT(m_pDatabase != NULL);
	ASSERT(m_pDatabase->IsOpen());

	RETCODE	nRetCode;
	UWORD	bFunctionExists;

	// make sure SQLTables exists
	AFX_SQL_SYNC(::SQLGetFunctions(m_pDatabase->m_hdbc,
		SQL_API_SQLTABLES,&bFunctionExists));
	if (!Check(nRetCode) || !bFunctionExists)
	{
		if (!bFunctionExists)
			TRACE(_T("SQLTables not supported\n"));
		return FALSE;
	}

	m_nOpenType = nOpenType;
	m_bUpdatable = FALSE;

	// make sure hstmt is allocated
	if (m_hstmt == SQL_NULL_HSTMT)
	{
		AFX_SQL_SYNC(::SQLAllocStmt(m_pDatabase->m_hdbc,&m_hstmt));
		if (!Check(nRetCode))
			ThrowDBException(nRetCode,m_hstmt);
	}

	OnSetOptions(m_hstmt);

	TRY
	{
		// call the ODBC function
		AFX_SQL_ASYNC(this,::SQLTables(m_hstmt,
			(UCHAR FAR*)pszTableQualifier,SQL_NTS,
			(UCHAR FAR*)pszTableOwner,SQL_NTS,
			(UCHAR FAR*)pszTableName,SQL_NTS,
			(UCHAR FAR*)pszTableType,SQL_NTS));

 		if (!Check(nRetCode))
			ThrowDBException(nRetCode,m_hstmt);

		// Allocate memory and cache info
		AllocAndCacheFieldInfo();
		AllocRowset();

		// Allocate the field info and status arrays if
		// not done already in BuildSelectSQL
		if ((m_nFields != 0 || m_nParams != 0) &&
			m_rgFieldInfos == NULL)
		{
			AllocStatusArrays();
		}

		MoveNext();
	}

	CATCH_ALL(e)
	{
		Close();
		THROW_LAST();
	}
	END_CATCH_ALL

	return TRUE;
}

void CTables::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX,_T("TABLE_QUALIFIER"),m_strTableQualifier);
	RFX_Text(pFX,_T("TABLE_OWNER"),m_strTableOwner);
	RFX_Text(pFX,_T("TABLE_NAME"),m_strTableName);
	RFX_Text(pFX,_T("TABLE_TYPE"),m_strTableType);
	RFX_Text(pFX,_T("REMARKS"),m_strRemarks);
}

