// typeinfo.h : interface of the CTypeInfo class
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

/////////////////////////////////////////////////////////////////////////////

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

class CTypeInfo : public CRecordset
{
DECLARE_DYNAMIC(CTypeInfo)

public:
	CTypeInfo(CDatabase* pDatabase = NULL);
	BOOL Open(UINT nOpenType = snapshot, LPCSTR lpszSQL = NULL, DWORD dwOptions = none);

// Field/Param Data
	//{{AFX_FIELD(CTypeInfo, CRecordset)
	CString m_strTypeName;
	int m_nDataType;
	long m_lPrecision;
	CString m_strLiteralPrefix;
	CString m_strLiteralSuffix;
	CString m_strCreateParams;
	int m_nNullable;
	int m_nCaseSensitive;
	int m_nSearchable;
	int m_nUnsignedAttribute;
	int m_nMoney;
	int m_nAutoIncrement;
	CString m_strLocalTypeName;
	int m_nMinimumScale;
	int m_nMaximumScale;
	//}}AFX_FIELD

	// SQL data type for which we're querying SQLGetTypeInfo
	SWORD m_fSqlTypeParam;

// Implementation
protected:
	virtual CString GetDefaultConnect();    // default connection string
	virtual CString GetDefaultSQL();    // default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
};
