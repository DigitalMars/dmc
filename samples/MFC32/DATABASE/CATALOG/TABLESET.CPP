// sqltable.cpp : implementation of the CTables class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
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

BOOL CTables::Open(UINT nOpenType, LPCSTR lpszSQL,
	DWORD dwOptions)
{
	ASSERT(lpszSQL == NULL);

	RETCODE	nRetCode;

	// Cache state info and allocate hstmt
	SetState(nOpenType,NULL,noDirtyFieldCheck);
	if (!AllocHstmt())
		return FALSE;

	TRY
	{
		OnSetOptions(m_hstmt);
		AllocStatusArrays();

		// call the ODBC catalog function with data member params
		AFX_SQL_ASYNC(this, (::SQLTables)(m_hstmt,
			(m_strQualifierParam.IsEmpty()? (UCHAR FAR *)NULL: (UCHAR FAR *)(const char*)m_strQualifierParam), SQL_NTS,
			(m_strOwnerParam.IsEmpty()? (UCHAR FAR *)NULL: (UCHAR FAR *)(const char*)m_strOwnerParam), SQL_NTS,
			(m_strNameParam.IsEmpty()? (UCHAR FAR *)NULL: (UCHAR FAR *)(const char*)m_strNameParam), SQL_NTS,
			(m_strTypeParam.IsEmpty()? (UCHAR FAR *)NULL: (UCHAR FAR *)(const char*)m_strTypeParam), SQL_NTS));
		if (!Check(nRetCode))
			ThrowDBException(nRetCode, m_hstmt);

		// Allocate memory and cache info
		AllocAndCacheFieldInfo();
		AllocRowset();

		// Fetch the first row of data
		MoveNext();

		// If EOF, result set is empty, set BOF as well
		m_bBOF = m_bEOF;
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
