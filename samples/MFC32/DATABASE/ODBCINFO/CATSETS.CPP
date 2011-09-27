// catsets.cpp

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
#include "catsets.h"

//---------------------------------------------------------------------
// CGetTypeInfo

CGetTypeInfo::CGetTypeInfo(CDatabase* pDatabase)
	: CRecordset(pDatabase)
{
	m_strTypeName			= _T("");
	m_fDataType				= 0;
	m_nPrecision			= 0;
	m_strLiteralPrefix		= _T("");
	m_strLiteralSuffix		= _T("");
	m_strCreateParams		= _T("");
	m_fNullable				= 0;
	m_bCaseSensitive		= 0;
	m_fSearchable			= 0;
	m_fUnsignedAttribute	= 0;
	m_bMoney				= 0;
	m_fAutoIncrement		= 0;
	m_strLocalTypeName		= _T("");
	m_nMinimumScale			= 0;
	m_nMaximumScale			= 0;
	m_nFields = 15;
}

BOOL CGetTypeInfo::Open(short fSqlType,
	UINT nOpenType)
{
	RETCODE	nRetCode;
	UWORD	bFunctionExists;

	// make suer SQLGetTypeInfo exists
	AFX_SQL_SYNC(::SQLGetFunctions(m_pDatabase->m_hdbc,
		SQL_API_SQLGETTYPEINFO,&bFunctionExists));
	if (!Check(nRetCode) || !bFunctionExists)
	{
		if (!bFunctionExists)
			TRACE(_T("SQLGetTypeInfo not supported\n"));
		return FALSE;
	}

	// Cache state info and allocate hstmt
	SetState(nOpenType, NULL, readOnly);
	if(!AllocHstmt())
		return FALSE;
	
	TRY
	{
		OnSetOptions(m_hstmt);

		// Build SQL and prep/execute or just execute direct
		AllocStatusArrays();

		// call the ODBC function
		AFX_SQL_ASYNC(this,::SQLGetTypeInfo(m_hstmt,fSqlType));
 		if (!Check(nRetCode))
			ThrowDBException(nRetCode,m_hstmt);

		// Allocate memory and cache info
		AllocAndCacheFieldInfo();
		AllocRowset();

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

void CGetTypeInfo::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX,_T("TYPE_NAME"),m_strTypeName);
	RFX_Int(pFX,_T("DATA_TYPE"),m_fDataType);
	RFX_Long(pFX,_T("PRECISION"),m_nPrecision);
	RFX_Text(pFX,_T("LITERAL_PREFIX"),m_strLiteralPrefix);
	RFX_Text(pFX,_T("LITERAL_SUFFIX"),m_strLiteralSuffix);
	RFX_Text(pFX,_T("CREATE_PARAMS"),m_strCreateParams);
	RFX_Int(pFX,_T("NULLABLE"),m_fNullable);
	RFX_Bool(pFX,_T("CASE_SENSITIVE"),m_bCaseSensitive);
	RFX_Int(pFX,_T("SEARCHABLE"),m_fSearchable);
	RFX_Int(pFX,_T("UNSIGNED_ATTRIBUTE"),m_fUnsignedAttribute);
	RFX_Bool(pFX,_T("MONEY"),m_bMoney);
	RFX_Int(pFX,_T("AUTO_INCREMENT"),m_fAutoIncrement);
	RFX_Text(pFX,_T("LOCAL_TYPE_NAME"),m_strLocalTypeName);
	RFX_Int(pFX,_T("MINIMUM_SCALE"),m_nMinimumScale);
	RFX_Int(pFX,_T("MAXIMUM_SCALE"),m_nMaximumScale);
}
