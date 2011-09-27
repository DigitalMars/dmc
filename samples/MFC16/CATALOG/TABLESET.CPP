// sqltable.cpp : implementation of the CTables class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"
#include "tableset.h"

/////////////////////////////////////////////////////////////////////////////
// CTables implementation

IMPLEMENT_DYNAMIC(CTables, CRecordset)

CTables::CTables(CDatabase* pDatabase)
	: CRecordset(pDatabase)
{
	//{{AFX_FIELD_INIT(CTables)
	m_strQualifier = "";
	m_strOwner = "";
	m_strName = "";
	m_strType = "";
	m_strRemarks = "";
	m_nFields = 5;
	//}}AFX_FIELD_INIT
	m_strQualifierParam = "";
	m_strOwnerParam = "";
	m_strNameParam = "";
	m_strTypeParam = "";
}

BOOL CTables::Open(UINT nOpenType /* = snapshot */,
	LPCSTR lpszSQL /* = NULL */, DWORD dwOptions /* = none */)
{
	RETCODE nRetCode;
	ASSERT(lpszSQL == NULL);

	// Allocation and opening of database not supported
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

	TRY
	{
		// set any options, like timeouts, scrolling options
		OnSetOptions(m_hstmt);

		// call the ODBC catalog function with data member params
		AFX_SQL_ASYNC(this, (::SQLTables)(m_hstmt,
			(m_strQualifierParam.IsEmpty()? (UCHAR FAR *)NULL: (UCHAR FAR *)(const char*)m_strQualifierParam), SQL_NTS,
			(m_strOwnerParam.IsEmpty()? (UCHAR FAR *)NULL: (UCHAR FAR *)(const char*)m_strOwnerParam), SQL_NTS,
			(m_strNameParam.IsEmpty()? (UCHAR FAR *)NULL: (UCHAR FAR *)(const char*)m_strNameParam), SQL_NTS,
			(m_strTypeParam.IsEmpty()? (UCHAR FAR *)NULL: (UCHAR FAR *)(const char*)m_strTypeParam), SQL_NTS));
		if (!Check(nRetCode))
		{
			AfxThrowDBException(nRetCode, m_pDatabase, m_hstmt);
		}
		// load first record
		MoveFirst();
	}
	CATCH_ALL(e)
	{
		Close();
		THROW_LAST();
	}
	END_CATCH_ALL
	return TRUE;
}

CString CTables::GetDefaultConnect()
{
	return "ODBC;";
}

CString CTables::GetDefaultSQL()
{
	// should SQLTables directly, so GetSQL should never be called
	ASSERT(FALSE);
	return "!";
}

void CTables::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CTables)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, "table_qualifier", m_strQualifier);
	RFX_Text(pFX, "table_owner", m_strOwner);
	RFX_Text(pFX, "table_name", m_strName);
	RFX_Text(pFX, "table_type", m_strType);
	RFX_Text(pFX, "remarks", m_strRemarks);
	//}}AFX_FIELD_MAP
}
