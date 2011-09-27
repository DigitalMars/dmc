// typeinfo.cpp: implementation of the CTypeInfo class
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
#include "typeinfo.h"

/////////////////////////////////////////////////////////////////////////////
// CTypeInfo implementation

IMPLEMENT_DYNAMIC(CTypeInfo, CRecordset)

CTypeInfo::CTypeInfo(CDatabase* pDatabase)
	: CRecordset(pDatabase)
{
	//{{AFX_FIELD_INIT(CTypeInfo)
	m_strTypeName = "";
	m_nDataType = 0;
	m_lPrecision = 0;
	m_strLiteralPrefix = "";
	m_strLiteralSuffix = "";
	m_strCreateParams = "";
	m_nNullable = 0;
	m_nCaseSensitive = 0;
	m_nSearchable = 0;
	m_nUnsignedAttribute = 0;
	m_nMoney = 0;
	m_nAutoIncrement = 0;
	m_strLocalTypeName = "";
	m_nMinimumScale = 0;
	m_nMaximumScale = 0;
	m_nFields = 15;
	//}}AFX_FIELD_INIT
}

BOOL CTypeInfo::Open(UINT nOpenType, LPCSTR lpszSQL, DWORD dwOptions)
{
	ASSERT(lpszSQL == NULL);
	ASSERT(!IsOpen());
	RETCODE nRetCode;

	// Cache state info and allocate hstmt
	SetState(nOpenType, lpszSQL, dwOptions);
	if(!AllocHstmt())
		return FALSE;
	
	TRY
	{
		OnSetOptions(m_hstmt);

		// call the ODBC catalog function with data member params
		AFX_SQL_ASYNC(this, ::SQLGetTypeInfo(m_hstmt, m_fSqlTypeParam));
		if (!Check(nRetCode))
			AfxThrowDBException(nRetCode, m_pDatabase, m_hstmt);

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

		// Give derived classes a call before binding
		PreBindFields();

		// Fetch the first row of data
		MoveNext();

		// If EOF, then result set empty, so set BOF as well
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

CString CTypeInfo::GetDefaultConnect()
{
	// this minimal connect string will cause ODBC login dialog to be brought up
	return "ODBC;";
}

CString CTypeInfo::GetDefaultSQL()
{
	// there is no default SQL - a direct ODBC call is made instead
	ASSERT(FALSE);
	return "!";
}

void CTypeInfo::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CTypeInfo)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, "type_name", m_strTypeName);
	RFX_Int(pFX, "data_type", m_nDataType);
	RFX_Long(pFX, "precision", m_lPrecision);
	RFX_Text(pFX, "literal_prefix", m_strLiteralPrefix);
	RFX_Text(pFX, "literal_suffix", m_strLiteralSuffix);
	RFX_Text(pFX, "create_params", m_strCreateParams);
	RFX_Int(pFX, "nullable", m_nNullable);
	RFX_Int(pFX, "case_sensitive", m_nCaseSensitive);
	RFX_Int(pFX, "searchable", m_nSearchable);
	RFX_Int(pFX, "unsigned_attribute", m_nUnsignedAttribute);
	RFX_Int(pFX, "money", m_nMoney);
	RFX_Int(pFX, "auto_increment", m_nAutoIncrement);
	RFX_Text(pFX, "local_type_name", m_strLocalTypeName);
	RFX_Int(pFX, "minimum_scale", m_nMinimumScale);
	RFX_Int(pFX, "maximum_scale", m_nMaximumScale);
	//}}AFX_FIELD_MAP
}
