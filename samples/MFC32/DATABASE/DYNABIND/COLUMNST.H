// columnst.h : interface of the CColumns class
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

class CColumns : public CRecordset
{
DECLARE_DYNAMIC(CColumns)

public:
	CColumns(CDatabase* pDatabase = NULL);
	BOOL Open(UINT nOpenType = forwardOnly, LPCSTR lpszSQL = NULL,
		DWORD dwOptions = readOnly);

// Field/Param Data
	//{{AFX_FIELD(CColumns, CRecordset)
	CString m_strQualifier;
	CString m_strOwner;
	CString m_strTableName;
	CString m_strColumnName;
	int  m_nDataType;
	CString m_strTypeName;
	long m_lPrecision;
	long m_lLength;
	int m_nScale;
	int m_nRadix;
	int m_nNullable;
	//}}AFX_FIELD

	// Table we're enumerating columns for
	CString m_strQualifierParam;
	CString m_strOwnerParam;
	CString m_strTableNameParam;
	CString m_strColumnNameParam;

// Implementation
protected:
	virtual CString GetDefaultConnect();    // default connection string
	virtual CString GetDefaultSQL();    // default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
};
