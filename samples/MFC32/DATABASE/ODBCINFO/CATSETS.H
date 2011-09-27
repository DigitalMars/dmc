// catsets.h

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef CATSETS_H
#define CATSETS_H

//---------------------------------------------------------------------
// CGetTypeInfo - results from SQLGetTypeInfo()

class CGetTypeInfo : public CRecordset
{
	virtual CString GetDefaultConnect() { return ""; }
	virtual CString GetDefaultSQL() { return ""; }

public:
					CGetTypeInfo(CDatabase* pDatabase);
	BOOL			Open(short fSqlType = SQL_ALL_TYPES,
					     UINT  nOpenType = forwardOnly);

	CString			m_strTypeName;
	int				m_fDataType;
	long			m_nPrecision;
	CString			m_strLiteralPrefix;
	CString			m_strLiteralSuffix;
	CString			m_strCreateParams;
	int				m_fNullable;
	BOOL			m_bCaseSensitive;
	int				m_fSearchable;
	int				m_fUnsignedAttribute;
	BOOL			m_bMoney;
	int				m_fAutoIncrement;
	CString			m_strLocalTypeName;
	int				m_nMinimumScale;
	int				m_nMaximumScale;

	virtual void	DoFieldExchange(CFieldExchange*);
};

#endif
