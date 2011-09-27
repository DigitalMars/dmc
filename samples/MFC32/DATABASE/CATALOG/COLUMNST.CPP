// sqlcols.cpp: implementation of the CColumns class
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
#include "columnst.h"

/////////////////////////////////////////////////////////////////////////////
// CColumns implementation

IMPLEMENT_DYNAMIC(CColumns, CRecordset)

CColumns::CColumns(CDatabase* pDatabase)
	: CRecordset(pDatabase)
{
	//{{AFX_FIELD_INIT(CColumns)
	m_strQualifier = "";
	m_strOwner = "";
	m_strTableName = "";
	m_strColumnName = "";
	m_nDataType = 0;
	m_strTypeName = "";
	m_lPrecision = 0;
	m_lLength = 0;
	m_nScale = 0;
	m_nRadix = 0;
	m_nFields = 11;
	//}}AFX_FIELD_INIT
	m_strQualifierParam = "";
	m_strOwnerParam = "";
	m_strTableNameParam = "";
	m_strColumnNameParam = "";
}

BOOL CColumns::Open(UINT nOpenType, LPCSTR lpszSQL, DWORD dwOptions)
{
	ASSERT(lpszSQL == NULL);

	RETCODE	nRetCode;

	// Cache state info and allocate hstmt
	SetState(nOpenType,NULL,noDirtyFieldCheck | dwOptions);
	if (!AllocHstmt())
		return FALSE;

	TRY
	{
		OnSetOptions(m_hstmt);
		AllocStatusArrays();

		// call the ODBC catalog function with data member params
		AFX_SQL_ASYNC(this, ::SQLColumns(m_hstmt,
			(m_strQualifierParam.IsEmpty()? (UCHAR FAR *)NULL: (UCHAR FAR *)(const char*)m_strQualifierParam), SQL_NTS,
			(m_strOwnerParam.IsEmpty()? (UCHAR FAR *)NULL: (UCHAR FAR *)(const char*)m_strOwnerParam), SQL_NTS,
			(m_strTableNameParam.IsEmpty()? (UCHAR FAR *)NULL: (UCHAR FAR *)(const char*)m_strTableNameParam), SQL_NTS,
			NULL, SQL_NTS));
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

CString CColumns::GetDefaultConnect()
{
	// this minimal connect string will cause ODBC login dialog to be brought up
	return "ODBC;";
}

CString CColumns::GetDefaultSQL()
{
	// there is no default SQL - a direct ODBC call is made instead
	ASSERT(FALSE);
	return "!";
}

void CColumns::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CColumns)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, "table_qualifier", m_strQualifier);
	RFX_Text(pFX, "table_owner", m_strOwner);
	RFX_Text(pFX, "table_name", m_strTableName);
	RFX_Text(pFX, "column_name", m_strColumnName);
	RFX_Int(pFX, "data_type", m_nDataType);
	RFX_Text(pFX, "type_name", m_strTypeName);
	RFX_Long(pFX, "precision", m_lPrecision);
	RFX_Long(pFX, "length", m_lLength);
	RFX_Int(pFX, "scale", m_nScale);
	RFX_Int(pFX, "radix", m_nRadix);
	RFX_Int(pFX, "nullable", m_nNullable);
	//}}AFX_FIELD_MAP
}
