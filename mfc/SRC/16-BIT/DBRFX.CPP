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

/////////////////////////////////////////////////////////////////////////////
// CLongBinary class for holding LONG VARBINARY data

IMPLEMENT_DYNAMIC(CLongBinary, CObject)

#define new DEBUG_NEW

CLongBinary::CLongBinary()
{
	m_hData = NULL;
}

CLongBinary::~CLongBinary()
{
	if (m_hData != NULL)
	{
		::GlobalFree(m_hData);
		m_hData = NULL;
	}
}

#ifdef _DEBUG
void CLongBinary::AssertValid() const
{
	CObject::AssertValid();
}
void CLongBinary::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
	AFX_DUMP0(dc, "\nwith CLongBinary:");
	AFX_DUMP1(dc, "\nm_hData = ", m_hData);
	AFX_DUMP1(dc, "\nm_dwDataLength = ", m_dwDataLength);
}
#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDBByteArray db specific class for holding byte array data
class CDBByteArray : public CByteArray
{
	DECLARE_DYNAMIC(CDBByteArray)

// Operations
	void SetLength(int nNewSize);
};

inline void CDBByteArray::SetLength(int nNewSize)
{
	// Can't grow buffer since ODBC has been SQLBindCol'd on it.
	ASSERT(nNewSize <= m_nMaxSize);
	m_nSize = nNewSize;
}

//////////////////////////////////////////////////////////////////////////////
// CFieldExchange

CFieldExchange::CFieldExchange(UINT nOperation, CRecordset* prs, void* pvField)
{
	ASSERT(nOperation >= BindParam && nOperation <= MaxRFXOperation);
	ASSERT_VALID(prs);
	ASSERT(prs->m_hstmt != SQL_NULL_HSTMT);

	m_nFieldType = noFieldType;
	m_nOperation = nOperation;
	m_prs = prs;
	m_pvField = pvField;

	m_nFields = 0;
	m_nParams = 0;
	m_nParamFields = 0;
	m_bField = FALSE;
	m_bFieldFound = FALSE;
	m_pstr = NULL;
	m_hstmt = SQL_NULL_HSTMT;
	m_par = NULL;
}

BOOL CFieldExchange::IsFieldType(UINT* pnField)
{
	if (m_nFieldType == outputColumn)
	{
		*pnField = ++m_nFields;
		// Recordset's m_nFields must match number of Fields!
		ASSERT(m_nFields <= m_prs->m_nFields);
	}
	else
	{
		// Make sure SetFieldType was called
		ASSERT(m_nFieldType == param);

		*pnField = ++m_nParams;
		// Recordset's m_nParams must match number of Params!
		ASSERT(m_nParams <= m_prs->m_nParams);
	}

	switch (m_nOperation)
	{
	// these can work on either field type
	case SetFieldNull:
	case IsFieldNull:
	case IsFieldNullable:
		return TRUE;

	// only valid on a param field type
	case BindParam:
	case RebindDateParam:
		return m_nFieldType != outputColumn;

	// valid only on an outputColumn field type
	default:
		return m_nFieldType == outputColumn;
	}
}

// Default implementation for RFX functions
void CFieldExchange::Default(const char *szName,
	void* pv, LONG* plLength, int nCType, UINT cbValue, UINT cbPrecision)
{
	RETCODE nRetCode;
	UINT nField = (m_nFieldType == outputColumn)? m_nFields: m_nParams;
	switch (m_nOperation)
	{
	case BindParam:
		if (m_prs->IsFieldFlagNull(nField, param))
			*plLength = SQL_NULL_DATA;
		else
			*plLength = cbValue;
		// For params, CType is same as SQL type
		AFX_SQL_SYNC(::SQLSetParam(m_hstmt, (unsigned short int)nField, (short int)nCType,
			(short int)nCType, (unsigned long int)cbPrecision, 0, pv, plLength));
		if (nRetCode != SQL_SUCCESS)
			m_prs->ThrowDBException(nRetCode, m_hstmt);
		return;

	case RebindDateParam:
		// Only need for date/time parameters
		return;

	case BindFieldToColumn:
		AFX_SQL_SYNC(::SQLBindCol(m_prs->m_hstmt,
			(unsigned short int)nField, (short int)nCType,
			pv, cbValue, plLength));
		if (!m_prs->Check(nRetCode))
			m_prs->ThrowDBException(nRetCode);
		return;

	case Name:
		if (m_prs->IsFieldFlagDirty(nField, m_nFieldType))
		{
			// We require a name
			ASSERT(lstrlen(szName) != 0);

			*m_pstr += m_prs->m_pDatabase->QuoteName(szName);
			*m_pstr += m_lpszSeparator;
		}
		return;

	case NameValue:
		if (m_prs->IsFieldFlagDirty(nField, m_nFieldType))
		{
			*m_pstr += m_prs->m_pDatabase->QuoteName(szName);
			*m_pstr += "=";
		}

		// Fall through
	case Value:
		if (m_prs->IsFieldFlagDirty(nField, m_nFieldType))
		{
			// If user marked column NULL, reflect this in length
			if (m_prs->IsFieldFlagNull(nField, m_nFieldType))
				*plLength = SQL_NULL_DATA;
			else
				*plLength = cbValue;
	
			// If optimizing for bulk add, just set length.
			if(!(m_prs->m_dwOptions & CRecordset::optimizeBulkAdd))
			{
				*m_pstr += "?";
				*m_pstr += m_lpszSeparator;
				m_nParamFields++;
				AFX_SQL_SYNC(::SQLSetParam(m_hstmt, (unsigned short int)m_nParamFields, (short int)nCType,
					// Get actual SQL type - may differ from CType (e.g. DECIMAL)
					(short int)GetColumnType(nField), (unsigned long int)cbPrecision, 0, pv, plLength));
				if (nRetCode != SQL_SUCCESS)
					m_prs->ThrowDBException(nRetCode, m_hstmt);
			}
		}
		return;

	case SetFieldDirty:
		if ((m_pvField == NULL && m_nFieldType == outputColumn)
			|| m_pvField == pv)
		{
			if (m_bField)
				m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_DIRTY, m_nFieldType);
			else
				m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_DIRTY, m_nFieldType);
#ifdef _DEBUG
			m_bFieldFound = TRUE;
#endif // _DEBUG
		}
		return;

	case IsFieldDirty:
		if ((m_pvField == NULL && m_nFieldType == outputColumn)
			|| m_pvField == pv)
		{
			if (m_prs->IsFieldFlagDirty(nField, m_nFieldType))
				m_bField = TRUE;
#ifdef _DEBUG
			m_bFieldFound = TRUE;
#endif // _DEBUG
		}
		return;

	case IsFieldNull:
		if ((m_pvField == NULL && m_nFieldType == outputColumn)
			|| m_pvField == pv)
		{
			if (m_prs->IsFieldFlagNull(nField, m_nFieldType))
				m_bField = TRUE;
#ifdef _DEBUG
			m_bFieldFound = TRUE;
#endif // _DEBUG
		}
		return;

	case IsFieldNullable:
		if ((m_pvField == NULL && m_nFieldType == outputColumn)
			|| m_pvField == pv)
		{
			UINT cbColumn;
			int nScale;
			int nNullable;

			if (m_nFieldType == param)
			{
				// You can certainly set a param NULL, but we don't parse
				// WHERE clauses, so we don't know if it's valid
				m_bField = TRUE;
			}
			else
			{
				ASSERT(m_nFieldType == outputColumn);
				GetColumnType(nField, &cbColumn, &nScale, &nNullable);
				if (nNullable == SQL_NULLABLE ||
					nNullable == SQL_NULLABLE_UNKNOWN)
					m_bField = TRUE;
			}
#ifdef _DEBUG
			m_bFieldFound = TRUE;
#endif // _DEBUG
		}
		return;

	case MarkForUpdate:
		{
			// If user changed field value from previous value, mark field dirty
			BYTE bFlags;
			*m_par >> bFlags;
			if ((bFlags & AFX_SQL_FIELD_FLAG_NULL))
			{
				if (!m_prs->IsFieldFlagNull(nField,
					m_nFieldType))
				{
					m_prs->SetFieldFlags(nField,
						AFX_SQL_FIELD_FLAG_DIRTY, m_nFieldType);
				}
			}
			else
			{
				// TRUE if NULL status differs
				BOOL bDirty = m_prs->IsFieldFlagNull(nField,
					m_nFieldType);

				UINT nLength;
				m_par->Read(&nLength, sizeof(nLength));
				// Lengths differ
				if (nLength != cbValue)
					bDirty = TRUE;

				// Compare values
				BYTE *pbValue = (BYTE *)pv;
				while (nLength--)
				{
					BYTE bSaved;
					*m_par >> bSaved;
					// Values differ
					if (!bDirty && *pbValue++ != bSaved)
						bDirty = TRUE;
				}
				if (bDirty)
				{
					m_prs->SetFieldFlags(nField,
						AFX_SQL_FIELD_FLAG_DIRTY,
						m_nFieldType);
				}
			}

#ifdef _DEBUG
			// Field address must not change - ODBC's SQLBindCol depends upon this
			void* pvSaved;
			m_par->Read(&pvSaved, sizeof(pvSaved));
			if (pvSaved != pv)
			{
				TRACE1("Error: field address (column %u) has changed!\n",
					nField);
				ASSERT(FALSE);
			}
#endif // _DEBUG

			if ((m_pvField == NULL  || m_pvField == pv) &&
				m_prs->IsFieldFlagDirty(nField,
					m_nFieldType))
			{
				m_bField = TRUE;
			}
		}
		return;

	case StoreField:
		*m_par << m_prs->GetFieldFlags(nField);
		if (!m_prs->IsFieldFlagNull(nField, m_nFieldType))
		{
			UINT nLength = cbValue;
			m_par->Write(&nLength, sizeof(nLength));
			m_par->Write(pv, nLength);
		}

#ifdef _DEBUG
		{
		// Save field address
		m_par->Write(&pv, sizeof(pv));
		}
#endif // _DEBUG
		return;

	case LoadField:
		{
			BYTE bFlags;
			*m_par >> bFlags;
			m_prs->SetFieldFlags(nField, bFlags,
					m_nFieldType);
			if (!m_prs->IsFieldFlagNull(nField,
				m_nFieldType))
			{
				UINT nLength;
				m_par->Read(&nLength, sizeof(nLength));
				*plLength = nLength;
				m_par->Read(pv, nLength);
			}
			else
				*plLength = SQL_NULL_DATA;

#ifdef _DEBUG
			// Field address must not change - ODBC's SQLBindCol depends upon this
			void* pvSaved;
			m_par->Read(&pvSaved, sizeof(pvSaved));
			if (pvSaved != pv)
			{
				TRACE1("Error: field address (column %u) has changed!\n",
					nField);
				ASSERT(FALSE);
			}
#endif // _DEBUG
			return;
		}

	default:
		ASSERT(FALSE);
	}
}

// Note: CString.m_pchData must not be changed.  This address is registered
// with ODBC and must remain valid until the recordset is released.
void AFXAPI RFX_Text(CFieldExchange* pFX, const char *szName,
	CString& value, int nMaxLength, int nColumnType)
{
	ASSERT(AfxIsValidAddress(pFX, sizeof(CFieldExchange)));
	ASSERT(AfxIsValidString(szName));
	ASSERT(AfxIsValidAddress(&value, sizeof(CString)));

	RETCODE nRetCode;
	UINT nField;
	if (!pFX->IsFieldType(&nField))
		return;

	LONG* plLength = pFX->m_prs->GetFieldLength(pFX);
	switch (pFX->m_nOperation)
	{
	default:
		pFX->Default(szName, value.GetBuffer(0), plLength,
			SQL_C_CHAR, value.GetLength(), value.GetLength());
		value.ReleaseBuffer();
		return;

	case CFieldExchange::BindParam:
		*plLength = SQL_NTS;
		// Preallocate to nMaxLength
		value.GetBufferSetLength(nMaxLength);
		AFX_SQL_SYNC(::SQLSetParam(pFX->m_hstmt, (unsigned short int)nField, (short int)SQL_C_CHAR,
			(short int)nColumnType, (unsigned long int)value.GetLength(), 0, value.GetBuffer(0), plLength));
		value.ReleaseBuffer();
		if (nRetCode != SQL_SUCCESS)
			pFX->m_prs->ThrowDBException(nRetCode, pFX->m_hstmt);
		return;

	case CFieldExchange::GetFieldInfoValue:
		if (pFX->m_pfi->pv == &value)
		{
			pFX->m_pfi->nField = nField-1;
			value.ReleaseBuffer();
			goto LFieldFound;
		}
		value.ReleaseBuffer();
		return;

	case CFieldExchange::GetFieldInfoOrdinal:
		if (nField-1 == pFX->m_pfi->nField)
		{
LFieldFound:
			pFX->m_pfi->nDataType = AFX_RFX_TEXT;
			pFX->m_pfi->strName = szName;
			pFX->m_pfi->pv = &value;
			value.ReleaseBuffer();
			pFX->m_pfi->dwSize = nMaxLength;
			// Make sure field found only once
			ASSERT(pFX->m_bFieldFound == FALSE);
			pFX->m_bFieldFound = TRUE;
		}
		return;

	case CFieldExchange::BindFieldToColumn:
		{
			UINT cbColumn;
			int nSqlType = pFX->GetColumnType(nField, &cbColumn);
			switch (nSqlType)
			{
			default:
				pFX->m_prs->ThrowDBException(AFX_SQL_ERROR_FIELD_SCHEMA_MISMATCH);

			case SQL_LONGVARCHAR:
			case SQL_CHAR:
			case SQL_VARCHAR:
				break;

			case SQL_DECIMAL:
			case SQL_NUMERIC:
				// Add room for sign and decimal point
				cbColumn += 2;
				break;
			}

			// Constrain to user specified max length
			if (cbColumn > (UINT)nMaxLength)
				cbColumn = nMaxLength;
			AFX_SQL_SYNC(::SQLBindCol(pFX->m_prs->m_hstmt,
				(unsigned short int)nField, SQL_C_CHAR,
				value.GetBufferSetLength(cbColumn+1), cbColumn+1, plLength));
			value.ReleaseBuffer();
			if (!pFX->m_prs->Check(nRetCode))
				pFX->m_prs->ThrowDBException(nRetCode);
		}
		return;

	case CFieldExchange::Fixup:
		if (*plLength == SQL_NULL_DATA)
		{
			pFX->m_prs->SetFieldFlags(nField, AFX_SQL_FIELD_FLAG_NULL,
				pFX->m_nFieldType);
			value.GetBufferSetLength(0);
			value.ReleaseBuffer();
		}
		else
		{
			LPSTR lpsz = value.GetBuffer(0);
			if (pFX->m_prs->m_pDatabase->m_bStripTrailingSpaces)
			{
				// find first trailing space
				LPSTR lpszFirstTrailing = NULL;
				while (*lpsz != '\0')
				{
					if (*lpsz != ' ')
						lpszFirstTrailing = NULL;
					else
					{
						if (lpszFirstTrailing == NULL)
							lpszFirstTrailing = lpsz;
					}
					lpsz = _AfxAnsiNext(lpsz);
				}
				// truncate
				if (lpszFirstTrailing != NULL)
					*lpszFirstTrailing = '\0';

			}
			value.ReleaseBuffer();
			*plLength = value.GetLength();
		}
		return;

	case CFieldExchange::SetFieldNull:
		if ((pFX->m_pvField == NULL &&
			pFX->m_nFieldType == CFieldExchange::outputColumn) ||
			pFX->m_pvField == &value)
		{
			if (pFX->m_bField)
			{
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				// Set string 0 length
				value.GetBufferSetLength(0);
				value.ReleaseBuffer();
				*plLength = SQL_NULL_DATA;
			}
			else
			{
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				*plLength = SQL_NTS;
			}
#ifdef _DEBUG
			pFX->m_bFieldFound = TRUE;
#endif // _DEBUG
		}
		return;


	case CFieldExchange::SetFieldDirty:
	case CFieldExchange::IsFieldDirty:
	case CFieldExchange::IsFieldNull:
	case CFieldExchange::IsFieldNullable:
		// pv arg should be &value instead of GetBuffer for these calls
		pFX->Default(szName, &value, plLength,
			SQL_C_CHAR, sizeof(value), sizeof(value));
		return;

	case CFieldExchange::MarkForAddNew:
		// can force writing of psuedo-null value (as a non-null) by setting field dirty
		if (!pFX->m_prs->IsFieldFlagDirty(nField, pFX->m_nFieldType))
		{
			if (!value.IsEmpty())
			{
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_DIRTY, pFX->m_nFieldType);
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
			}
		}
		return;

	case CFieldExchange::MarkForUpdate:
		{
			if (value.IsEmpty())
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
			else
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);

			BYTE bFlags;
			*pFX->m_par >> bFlags;
			if ((bFlags & AFX_SQL_FIELD_FLAG_NULL))
			{
				if (!pFX->m_prs->IsFieldFlagNull(nField,
					pFX->m_nFieldType))
				{
					pFX->m_prs->SetFieldFlags(nField,
						AFX_SQL_FIELD_FLAG_DIRTY, pFX->m_nFieldType);
				}
			}
			else
			{
				// Saved field is not NULL. current field null, so field dirty
				BOOL bDirty = pFX->m_prs->IsFieldFlagNull(nField,
					pFX->m_nFieldType);
				CString valueSaved;

				*pFX->m_par >> valueSaved;
				if (bDirty || value.Compare(valueSaved) != 0)
					pFX->m_prs->SetFieldFlags(nField,
						AFX_SQL_FIELD_FLAG_DIRTY, pFX->m_nFieldType);
			}

#ifdef _DEBUG
			// Buffer address must not change - ODBC's SQLBindCol depends upon this
			char* pchSaved;
			pFX->m_par->Read(&pchSaved, sizeof(pchSaved));
			if (pchSaved != value.GetBuffer(0))
			{
				TRACE1("Error: CString buffer (column %u) address has changed!\n",
					nField);
				ASSERT(FALSE);
			}
			value.ReleaseBuffer();
#endif // _DEBUG

			if ((pFX->m_pvField == NULL  || pFX->m_pvField == &value) &&
				pFX->m_prs->IsFieldFlagDirty(nField,
					pFX->m_nFieldType))
			{
				pFX->m_bField = TRUE;
			}
		}
		return;

	case CFieldExchange::StoreField:
		*pFX->m_par << pFX->m_prs->GetFieldFlags(nField);
		if (!pFX->m_prs->IsFieldFlagNull(nField, pFX->m_nFieldType))
		{
			*pFX->m_par << value;
		}

#ifdef _DEBUG
		{
		// Save address of character buffer
		char* pch = value.GetBuffer(0);
		pFX->m_par->Write(&pch, sizeof(pch));
		value.ReleaseBuffer();
		}
#endif // _DEBUG
		return;

	case CFieldExchange::LoadField:
		{
			BYTE bFlags;
			*pFX->m_par >> bFlags;
			pFX->m_prs->SetFieldFlags(nField, bFlags,
				pFX->m_nFieldType);
			if (!pFX->m_prs->IsFieldFlagNull(nField,
				pFX->m_nFieldType))
			{
				CString strT;
				*pFX->m_par >> strT;
				value = strT;
				*plLength = value.GetLength();
			}
			else
			{
				*plLength = SQL_NULL_DATA;
			}

#ifdef _DEBUG
			// Buffer address must not change - ODBC's SQLBindCol depends upon this
			char* pchSaved;
			pFX->m_par->Read(&pchSaved, sizeof(pchSaved));
			if (pchSaved != value.GetBuffer(0))
			{
				TRACE1("Error: CString buffer (column %u) address has changed!\n",
					nField);
				ASSERT(FALSE);
			}
			value.ReleaseBuffer();
#endif // _DEBUG
		}
		return;
	}
}

void AFXAPI RFX_Int(CFieldExchange* pFX, const char *szName,
	int& value)
{
	ASSERT(AfxIsValidAddress(pFX, sizeof(CFieldExchange)));
	ASSERT(AfxIsValidString(szName));

	UINT nField;
	if (!pFX->IsFieldType(&nField))
		return;

	LONG* plLength = pFX->m_prs->GetFieldLength(pFX);
	switch (pFX->m_nOperation)
	{
	case CFieldExchange::BindFieldToColumn:
		if (pFX->GetColumnType(nField) != SQL_C_SHORT)
			pFX->m_prs->ThrowDBException(AFX_SQL_ERROR_FIELD_SCHEMA_MISMATCH);
		// fall through

	default:
LDefault:
		pFX->Default(szName, &value, plLength, SQL_C_SHORT, sizeof(value), 5);
		return;

	case CFieldExchange::Fixup:
		if (*plLength == SQL_NULL_DATA)
		{
			pFX->m_prs->SetFieldFlags(nField,
				AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
			value = AFX_RFX_INT_PSEUDO_NULL;
		}
		return;

	case CFieldExchange::SetFieldNull:
		if ((pFX->m_pvField == NULL &&
			pFX->m_nFieldType == CFieldExchange::outputColumn) ||
			pFX->m_pvField == &value)
		{
			if (pFX->m_bField)
			{
				// Mark fields null
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				value = AFX_RFX_INT_PSEUDO_NULL;
				*plLength = SQL_NULL_DATA;
			}
			else
			{
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				*plLength = sizeof(value);
			}
#ifdef _DEBUG
			pFX->m_bFieldFound = TRUE;
#endif // _DEBUG
		}
		return;

	case CFieldExchange::MarkForAddNew:
		// can force writing of psuedo-null value (as a non-null) by setting field dirty
		if (!pFX->m_prs->IsFieldFlagDirty(nField, pFX->m_nFieldType))
		{
			if (value != AFX_RFX_INT_PSEUDO_NULL)
			{
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_DIRTY, pFX->m_nFieldType);
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
			}
		}
		return;

	case CFieldExchange::MarkForUpdate:
		if (value != AFX_RFX_INT_PSEUDO_NULL)
			pFX->m_prs->ClearFieldFlags(nField,
				AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
		goto LDefault;

	case CFieldExchange::GetFieldInfoValue:
		if (pFX->m_pfi->pv == &value)
		{
			pFX->m_pfi->nField = nField-1;
			goto LFieldFound;
		}
		return;

	case CFieldExchange::GetFieldInfoOrdinal:
		if (nField-1 == pFX->m_pfi->nField)
		{
LFieldFound:
			pFX->m_pfi->nDataType = AFX_RFX_INT;
			pFX->m_pfi->strName = szName;
			pFX->m_pfi->pv = &value;
			pFX->m_pfi->dwSize = sizeof(value);
			// Make sure field found only once
			ASSERT(pFX->m_bFieldFound == FALSE);
			pFX->m_bFieldFound = TRUE;
		}
		return;
	}
}

void AFXAPI RFX_Long(CFieldExchange* pFX, const char *szName, long& value)
{
	ASSERT(AfxIsValidAddress(pFX, sizeof(CFieldExchange)));
	ASSERT(AfxIsValidString(szName));

	UINT nField;
	if (!pFX->IsFieldType(&nField))
		return;

	LONG* plLength = pFX->m_prs->GetFieldLength(pFX);
	switch (pFX->m_nOperation)
	{
	case CFieldExchange::BindFieldToColumn:
		if (pFX->GetColumnType(nField) != SQL_C_LONG)
			pFX->m_prs->ThrowDBException(AFX_SQL_ERROR_FIELD_SCHEMA_MISMATCH);
		// fall through

	default:
LDefault:
		pFX->Default(szName, &value, plLength, SQL_C_LONG, sizeof(value), 10);
		return;

	case CFieldExchange::Fixup:
		if (*plLength == SQL_NULL_DATA)
		{
			pFX->m_prs->SetFieldFlags(nField,
				AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
			value = AFX_RFX_LONG_PSEUDO_NULL;
		}
		return;

	case CFieldExchange::SetFieldNull:
		if ((pFX->m_pvField == NULL &&
			pFX->m_nFieldType == CFieldExchange::outputColumn) ||
			pFX->m_pvField == &value)
		{
			if (pFX->m_bField)
			{
				// Mark fields null
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				value = AFX_RFX_LONG_PSEUDO_NULL;
				*plLength = SQL_NULL_DATA;
			}
			else
			{
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				*plLength = sizeof(value);
			}
#ifdef _DEBUG
			pFX->m_bFieldFound = TRUE;
#endif // _DEBUG
		}
		return;

	case CFieldExchange::MarkForAddNew:
		// can force writing of psuedo-null value (as a non-null) by setting field dirty
		if (!pFX->m_prs->IsFieldFlagDirty(nField, pFX->m_nFieldType))
		{
			if (value != AFX_RFX_LONG_PSEUDO_NULL)
			{
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_DIRTY, pFX->m_nFieldType);
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
			}
		}
		return;

	case CFieldExchange::MarkForUpdate:
		if (value != AFX_RFX_LONG_PSEUDO_NULL)
			pFX->m_prs->ClearFieldFlags(nField,
				AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
		goto LDefault;

	case CFieldExchange::GetFieldInfoValue:
		if (pFX->m_pfi->pv == &value)
		{
			pFX->m_pfi->nField = nField-1;
			goto LFieldFound;
		}
		return;

	case CFieldExchange::GetFieldInfoOrdinal:
		if (nField-1 == pFX->m_pfi->nField)
		{
LFieldFound:
			pFX->m_pfi->nDataType = AFX_RFX_LONG;
			pFX->m_pfi->strName = szName;
			pFX->m_pfi->pv = &value;
			pFX->m_pfi->dwSize = sizeof(value);
			// Make sure field found only once
			ASSERT(pFX->m_bFieldFound == FALSE);
			pFX->m_bFieldFound = TRUE;
		}
		return;
	}
}

void AFXAPI RFX_Byte(CFieldExchange* pFX, const char *szName,
	BYTE& value)
{
	ASSERT(AfxIsValidAddress(pFX, sizeof(CFieldExchange)));
	ASSERT(AfxIsValidString(szName));

	UINT nField;
	if (!pFX->IsFieldType(&nField))
		return;

	LONG* plLength = pFX->m_prs->GetFieldLength(pFX);
	switch (pFX->m_nOperation)
	{
	case CFieldExchange::BindFieldToColumn:
		if (pFX->GetColumnType(nField) != SQL_TINYINT)
			pFX->m_prs->ThrowDBException(AFX_SQL_ERROR_FIELD_SCHEMA_MISMATCH);
		// fall through

	default:
LDefault:
		pFX->Default(szName, &value, plLength, SQL_TINYINT, sizeof(value), 3);
		break;

	case CFieldExchange::Fixup:
		if (*plLength == SQL_NULL_DATA)
		{
			pFX->m_prs->SetFieldFlags(nField,
				AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
			value = AFX_RFX_BYTE_PSEUDO_NULL;
		}
		return;

	case CFieldExchange::SetFieldNull:
		if ((pFX->m_pvField == NULL &&
			pFX->m_nFieldType == CFieldExchange::outputColumn) ||
			pFX->m_pvField == &value)
		{
			if (pFX->m_bField)
			{
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				value = AFX_RFX_BYTE_PSEUDO_NULL;
				*plLength = SQL_NULL_DATA;
			}
			else
			{
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				*plLength = sizeof(value);
			}
#ifdef _DEBUG
			pFX->m_bFieldFound = TRUE;
#endif // _DEBUG
		}
		return;

	case CFieldExchange::MarkForAddNew:
		// can force writing of psuedo-null value (as a non-null) by setting field dirty
		if (!pFX->m_prs->IsFieldFlagDirty(nField, pFX->m_nFieldType))
		{
			if (value != AFX_RFX_BYTE_PSEUDO_NULL)
			{
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_DIRTY, pFX->m_nFieldType);
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
			}
		}
		return;

	case CFieldExchange::MarkForUpdate:
		if (value != AFX_RFX_BYTE_PSEUDO_NULL)
			pFX->m_prs->ClearFieldFlags(nField,
				AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
		goto LDefault;

	case CFieldExchange::GetFieldInfoValue:
		if (pFX->m_pfi->pv == &value)
		{
			pFX->m_pfi->nField = nField-1;
			goto LFieldFound;
		}
		return;

	case CFieldExchange::GetFieldInfoOrdinal:
		if (nField-1 == pFX->m_pfi->nField)
		{
LFieldFound:
			pFX->m_pfi->nDataType = AFX_RFX_BYTE;
			pFX->m_pfi->strName = szName;
			pFX->m_pfi->pv = &value;
			pFX->m_pfi->dwSize = sizeof(value);
			// Make sure field found only once
			ASSERT(pFX->m_bFieldFound == FALSE);
			pFX->m_bFieldFound = TRUE;
		}
		return;
	}
}


void AFXAPI RFX_Bool(CFieldExchange* pFX, const char *szName,
	BOOL& value)
{
	ASSERT(AfxIsValidAddress(pFX, sizeof(CFieldExchange)));
	ASSERT(AfxIsValidString(szName));

	UINT nField;
	if (!pFX->IsFieldType(&nField))
		return;

	LONG* plLength = pFX->m_prs->GetFieldLength(pFX);
	switch (pFX->m_nOperation)
	{
	case CFieldExchange::BindFieldToColumn:
		if (pFX->GetColumnType(nField) != SQL_BIT)
			pFX->m_prs->ThrowDBException(AFX_SQL_ERROR_FIELD_SCHEMA_MISMATCH);
		// Fall through

	default:
LDefault:
		pFX->Default(szName, &value, plLength, SQL_BIT, sizeof(value), 1);
		return;

	case CFieldExchange::Fixup:
		if (*plLength == SQL_NULL_DATA)
		{
			pFX->m_prs->SetFieldFlags(nField,
				AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
			value = AFX_RFX_BOOL_PSEUDO_NULL;
		}
		else
			// Cast BYTE into BOOL (int)
			value = *(BYTE *)&value;
		return;

	case CFieldExchange::SetFieldNull:
		if ((pFX->m_pvField == NULL &&
			pFX->m_nFieldType == CFieldExchange::outputColumn) ||
			pFX->m_pvField == &value)
		{
			if (pFX->m_bField)
			{
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				value = AFX_RFX_BOOL_PSEUDO_NULL;
				*plLength = SQL_NULL_DATA;
			}
			else
			{
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				*plLength = sizeof(value);
			}
#ifdef _DEBUG
			pFX->m_bFieldFound = TRUE;
#endif // _DEBUG
		}
		return;

	case CFieldExchange::MarkForAddNew:
		// can force writing of psuedo-null value (as a non-null) by setting field dirty
		if (!pFX->m_prs->IsFieldFlagDirty(nField, pFX->m_nFieldType))
		{
			if (value != AFX_RFX_BOOL_PSEUDO_NULL)
			{
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_DIRTY, pFX->m_nFieldType);
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
			}
		}
		return;

	case CFieldExchange::MarkForUpdate:
		if (value != AFX_RFX_BOOL_PSEUDO_NULL)
			pFX->m_prs->ClearFieldFlags(nField,
				AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
		goto LDefault;

	case CFieldExchange::GetFieldInfoValue:
		if (pFX->m_pfi->pv == &value)
		{
			pFX->m_pfi->nField = nField-1;
			goto LFieldFound;
		}
		return;

	case CFieldExchange::GetFieldInfoOrdinal:
		if (nField-1 == pFX->m_pfi->nField)
		{
LFieldFound:
			pFX->m_pfi->nDataType = AFX_RFX_BOOL;
			pFX->m_pfi->strName = szName;
			pFX->m_pfi->pv = &value;
			pFX->m_pfi->dwSize = sizeof(value);
			// Make sure field found only once
			ASSERT(pFX->m_bFieldFound == FALSE);
			pFX->m_bFieldFound = TRUE;
		}
		return;
	}
}


// Note: CByteArray.m_pData must not be changed.  This address is registered
// with ODBC and must remain valid until the recordset is released.
void AFXAPI RFX_Binary(CFieldExchange* pFX, const char *szName,
	CByteArray& value, int nMaxLength)
{
	ASSERT(AfxIsValidAddress(pFX, sizeof(CFieldExchange)));
	ASSERT(AfxIsValidString(szName));

	RETCODE nRetCode;
	UINT nField;
	if (!pFX->IsFieldType(&nField))
		return;

	LONG* plLength = pFX->m_prs->GetFieldLength(pFX);
	switch (pFX->m_nOperation)
	{
	default:
LDefault:
		pFX->Default(szName, (value.GetSize() > 0)? &value[0] : NULL, plLength,
			SQL_C_BINARY, (int)value.GetSize(), (UINT)value.GetSize());
		return;

	case CFieldExchange::BindFieldToColumn:
		{
			UINT cbColumn;
			int nSqlType = pFX->GetColumnType(nField, &cbColumn);

			if (nSqlType != SQL_BINARY && nSqlType != SQL_VARBINARY &&
				nSqlType != SQL_LONGVARBINARY)
			{
				pFX->m_prs->ThrowDBException(AFX_SQL_ERROR_FIELD_SCHEMA_MISMATCH);
			}

			// Constrain to user specified max length
			if (cbColumn > (UINT)nMaxLength)
				cbColumn = nMaxLength;
			value.SetSize(cbColumn);
			AFX_SQL_SYNC(::SQLBindCol(pFX->m_prs->m_hstmt,
				(unsigned short int)nField, SQL_C_BINARY,
				&value[0], (LONG)cbColumn, plLength));
			if (!pFX->m_prs->Check(nRetCode))
				pFX->m_prs->ThrowDBException(nRetCode);
		}
		return;

	case CFieldExchange::Fixup:
		if (*plLength == SQL_NULL_DATA)
		{
			pFX->m_prs->SetFieldFlags(nField,
				AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
			value.SetSize(1);
			value[0] = AFX_RFX_BYTE_PSEUDO_NULL;
		}
		else
		{
			ASSERT(*plLength <= (LONG)nMaxLength);
			((CDBByteArray&)value).SetLength((UINT)*plLength);
		}
		return;

	case CFieldExchange::SetFieldDirty:
	case CFieldExchange::IsFieldDirty:
	case CFieldExchange::IsFieldNull:
	case CFieldExchange::IsFieldNullable:
		pFX->Default(szName,
			&value, plLength, SQL_C_BINARY, sizeof(value), sizeof(value));
		return;

	case CFieldExchange::SetFieldNull:
		if ((pFX->m_pvField == NULL &&
			pFX->m_nFieldType == CFieldExchange::outputColumn) ||
			pFX->m_pvField == &value)
		{
			if (pFX->m_bField)
			{
				// Mark fields null
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				value.SetSize(1);
				value[0] = AFX_RFX_BYTE_PSEUDO_NULL;
				*plLength = SQL_NULL_DATA;
			}
			else
			{
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				*plLength = value.GetSize();
			}
#ifdef _DEBUG
			pFX->m_bFieldFound = TRUE;
#endif // _DEBUG
		}
		return;

	case CFieldExchange::MarkForAddNew:
		// can force writing of psuedo-null value (as a non-null) by setting field dirty
		if (!pFX->m_prs->IsFieldFlagDirty(nField, pFX->m_nFieldType))
		{
		if (value.GetSize() != 1 || value[0] != AFX_RFX_BYTE_PSEUDO_NULL)
			{
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_DIRTY, pFX->m_nFieldType);
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
			}
		}
		return;

	case CFieldExchange::MarkForUpdate:
		if (value.GetSize() != 1 || value[0] != AFX_RFX_BYTE_PSEUDO_NULL)
			pFX->m_prs->ClearFieldFlags(nField,
				AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
		goto LDefault;

	case CFieldExchange::GetFieldInfoValue:
		if (pFX->m_pfi->pv == &value)
		{
			pFX->m_pfi->nField = nField-1;
			goto LFieldFound;
		}
		return;

	case CFieldExchange::GetFieldInfoOrdinal:
		if (nField-1 == pFX->m_pfi->nField)
		{
LFieldFound:
			pFX->m_pfi->nDataType = AFX_RFX_BINARY;
			pFX->m_pfi->strName = szName;
			pFX->m_pfi->pv = &value;
			pFX->m_pfi->dwSize = nMaxLength;
			// Make sure field found only once
			ASSERT(pFX->m_bFieldFound == FALSE);
			pFX->m_bFieldFound = TRUE;
		}
		return;
	}
}


void AFXAPI RFX_Date(CFieldExchange* pFX, const char *szName,
	CTime& value)
{
	ASSERT(AfxIsValidAddress(pFX, sizeof(CFieldExchange)));
	ASSERT(AfxIsValidString(szName));

	RETCODE nRetCode;
	UINT nField;
	if (!pFX->IsFieldType(&nField))
		return;

	LONG* plLength = pFX->m_prs->GetFieldLength(pFX);
	switch (pFX->m_nOperation)
	{
	default:
LDefault:
		pFX->Default(szName, &value, plLength, SQL_C_TIMESTAMP,
			sizeof(value), TIMESTAMP_PRECISION);
		return;
	case CFieldExchange::BindParam:
		{
			TIMESTAMP_STRUCT* pts;
			if (pFX->m_prs->IsFieldFlagNull(nField, CFieldExchange::param))
			{
				pts = NULL;
				*plLength = SQL_NULL_DATA;
			}
			else
			{
				// If first field requiring a proxy, allocate array
				if (pFX->m_prs->m_pvParamProxy == NULL)
				{
					// Include extra element for the count (first element)
					pFX->m_prs->m_pvParamProxy = new void*[pFX->m_prs->m_nParams+1];
					UINT *pnCount = new UINT;
					*pnCount = pFX->m_prs->m_nParams;
					pFX->m_prs->m_pvParamProxy[0] = pnCount;
					memset(&pFX->m_prs->m_pvParamProxy[1], 0,
						pFX->m_prs->m_nParams*sizeof(void*));
				}

				// Allocate TIMESTAMP_STRUCT if necessary for SQLSetParam
				if (pFX->m_prs->m_pvParamProxy[nField] == NULL)
				{
					pts = new TIMESTAMP_STRUCT;
					pFX->m_prs->m_pvParamProxy[nField] = pts;
				}
				else
					pts = (TIMESTAMP_STRUCT *)pFX->m_prs->m_pvParamProxy[nField];

				pts->year = (short int)value.GetYear();
				pts->month = (unsigned short int)value.GetMonth();
				pts->day = (unsigned short int)value.GetDay();
				pts->hour = (unsigned short int)value.GetHour();
				pts->minute = (unsigned short int)value.GetMinute();
				pts->second = (unsigned short int)value.GetSecond();
				pts->fraction = 0;

				*plLength = sizeof(TIMESTAMP_STRUCT);
			}

			AFX_SQL_SYNC(::SQLSetParam(pFX->m_hstmt,
				(unsigned short int)nField,
				// SQLType same as CType for Params
				SQL_C_TIMESTAMP, SQL_C_TIMESTAMP, TIMESTAMP_PRECISION,
				0, pts, plLength));
			if (nRetCode != SQL_SUCCESS)
				pFX->m_prs->ThrowDBException(nRetCode, pFX->m_hstmt);
		}
		return;

		case CFieldExchange::RebindDateParam:
		{
			if (pFX->m_prs->m_pvParamProxy[nField] != NULL)
			{
				// Fill buffer (expected by SQLSetParam) with new param data
				TIMESTAMP_STRUCT* pts;
				pts = (TIMESTAMP_STRUCT *)pFX->m_prs->m_pvParamProxy[nField];
				pts->year = (SWORD)value.GetYear();
				pts->month = (UWORD)value.GetMonth();
				pts->day = (UWORD)value.GetDay();
				pts->hour = (UWORD)value.GetHour();
				pts->minute = (UWORD)value.GetMinute();
				pts->second = (UWORD)value.GetSecond();
				pts->fraction = 0;
			}
		}
		return;

	case CFieldExchange::BindFieldToColumn:
		{
			int nSqlType = pFX->GetColumnType(nField);
			if (nSqlType != SQL_DATE && nSqlType != SQL_TIME &&
				nSqlType != SQL_TIMESTAMP)
			{
				pFX->m_prs->ThrowDBException(AFX_SQL_ERROR_FIELD_SCHEMA_MISMATCH);
			}

			// If first field requiring a proxy, allocate array
			if (pFX->m_prs->m_pvFieldProxy == NULL)
			{
				// Include extra element for the count (first element)
				pFX->m_prs->m_pvFieldProxy = new void*[pFX->m_prs->m_nFields+1];
				UINT *pnCount = new UINT;
				*pnCount = pFX->m_prs->m_nFields;
				pFX->m_prs->m_pvFieldProxy[0] = pnCount;
				memset(&pFX->m_prs->m_pvFieldProxy[1], 0,
					pFX->m_prs->m_nFields*sizeof(void*));
			}

			// Allocate TIMESTAMP_STRUCT for SQLBindCol if necessary
			if (pFX->m_prs->m_pvFieldProxy[nField] == NULL)
				pFX->m_prs->m_pvFieldProxy[nField] = new TIMESTAMP_STRUCT;

			AFX_SQL_SYNC(::SQLBindCol(pFX->m_prs->m_hstmt, (unsigned short int)nField,
				SQL_C_TIMESTAMP, pFX->m_prs->m_pvFieldProxy[nField],
				(UINT)sizeof(TIMESTAMP_STRUCT), plLength));
			if (!pFX->m_prs->Check(nRetCode))
				pFX->m_prs->ThrowDBException(nRetCode);
		}
		return;

	case CFieldExchange::Fixup:
		if (*plLength == SQL_NULL_DATA)
		{
			pFX->m_prs->SetFieldFlags(nField,
				AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
			value = AFX_RFX_DATE_PSEUDO_NULL;
		}
		else
		{
			TIMESTAMP_STRUCT* pts =
				(TIMESTAMP_STRUCT*)pFX->m_prs->m_pvFieldProxy[nField];
			if (pts->year < 1970 || pts->year > 2038)
			{
				// Time value out of range, return NULL
#ifdef _DEBUG
				if (afxTraceFlags & 0x20)
					TRACE0("Warning: date value out of range, returning NULL value\n");
#endif // _DEBUG
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				value = AFX_RFX_DATE_PSEUDO_NULL;
			}
			else
			{
#ifdef _DEBUG
				if (afxTraceFlags & 0x20 && pts->fraction != 0)
					TRACE0("Warning: ignoring milliseconds\n");
#endif // _DEBUG
				value = CTime(pts->year, pts->month, pts->day,
					pts->hour, pts->minute, pts->second);
			}
		}
		return;

	case CFieldExchange::NameValue:
		if (pFX->m_prs->IsFieldFlagDirty(nField, pFX->m_nFieldType))
		{
			*pFX->m_pstr += pFX->m_prs->m_pDatabase->QuoteName(szName);
			*pFX->m_pstr += "=";
		}
		// Fall through

	case CFieldExchange::Value:
		if (pFX->m_prs->IsFieldFlagDirty(nField, pFX->m_nFieldType))
		{
			TIMESTAMP_STRUCT* pts = (TIMESTAMP_STRUCT*)pFX->m_prs->m_pvFieldProxy[nField];
			if (pFX->m_prs->IsFieldFlagNull(nField, pFX->m_nFieldType))
			{
				*plLength = SQL_NULL_DATA;
			}
			else
			{
				*plLength = sizeof(TIMESTAMP_STRUCT);

				pts->year = (short int)value.GetYear();
				pts->month = (unsigned short int)value.GetMonth();
				pts->day = (unsigned short int)value.GetDay();
				pts->hour = (unsigned short int)value.GetHour();
				pts->minute = (unsigned short int)value.GetMinute();
				pts->second = (unsigned short int)value.GetSecond();
				pts->fraction = 0;
			}

			// If optimizing for bulk add, just set length & proxy value
			if(!(pFX->m_prs->m_dwOptions & CRecordset::optimizeBulkAdd))
			{
				*pFX->m_pstr += "?";
				*pFX->m_pstr += pFX->m_lpszSeparator;
				pFX->m_nParamFields++;
				AFX_SQL_SYNC(::SQLSetParam(pFX->m_hstmt,
					(unsigned short int)pFX->m_nParamFields, SQL_C_TIMESTAMP,
					(short int)pFX->GetColumnType(nField),
					TIMESTAMP_PRECISION, 0, pts, plLength));
			}
		}
		return;

	case CFieldExchange::SetFieldNull:
		if ((pFX->m_pvField == NULL &&
			pFX->m_nFieldType == CFieldExchange::outputColumn) ||
			pFX->m_pvField == &value)
		{
			if (pFX->m_bField)
			{
				// Mark fields null
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				value = AFX_RFX_DATE_PSEUDO_NULL;
				*plLength = SQL_NULL_DATA;
			}
			else
			{
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				*plLength = sizeof(TIMESTAMP_STRUCT);
			}
#ifdef _DEBUG
			pFX->m_bFieldFound = TRUE;
#endif // _DEBUG
		}
		return;

	case CFieldExchange::MarkForAddNew:
		// can force writing of psuedo-null value (as a non-null) by setting field dirty
		if (!pFX->m_prs->IsFieldFlagDirty(nField, pFX->m_nFieldType))
		{
			CTime timeNull = AFX_RFX_DATE_PSEUDO_NULL;
			if (value != timeNull)
			{
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_DIRTY, pFX->m_nFieldType);
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
			}
		}
		return;

	case CFieldExchange::MarkForUpdate:
		{
			CTime timeNull = AFX_RFX_DATE_PSEUDO_NULL;
			if (value != timeNull)
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
		}
		goto LDefault;

	case CFieldExchange::LoadField:
		{
			TIMESTAMP_STRUCT* pts = (TIMESTAMP_STRUCT*)pFX->m_prs->m_pvFieldProxy[nField];
			BYTE bFlags;
			*pFX->m_par >> bFlags;
			pFX->m_prs->SetFieldFlags(nField, bFlags,
				pFX->m_nFieldType);
			if (!pFX->m_prs->IsFieldFlagNull(nField,
				pFX->m_nFieldType))
			{
				UINT nLength;
				pFX->m_par->Read(&nLength, sizeof(nLength));
				*plLength = sizeof(TIMESTAMP_STRUCT);

				*pFX->m_par >> value;
				// Must restore proxy for correct WHERE CURRENT OF operations
				pts->year = (short int)value.GetYear();
				pts->month = (unsigned short int)value.GetMonth();
				pts->day = (unsigned short int)value.GetDay();
				pts->hour = (unsigned short int)value.GetHour();
				pts->minute = (unsigned short int)value.GetMinute();
				pts->second = (unsigned short int)value.GetSecond();
				pts->fraction = 0;
			}
			else
				*plLength = SQL_NULL_DATA;

#ifdef _DEBUG
			// Field address must not change - ODBC's SQLBindCol depends upon this
			void* pvSaved;
			pFX->m_par->Read(&pvSaved, sizeof(pvSaved));
			if (pvSaved != &value)
			{
				TRACE1("Error: field address (column %u) has changed!\n",
					nField);
				ASSERT(FALSE);
			}
#endif // _DEBUG
		}
		return;

	case CFieldExchange::GetFieldInfoValue:
		if (pFX->m_pfi->pv == &value)
		{
			pFX->m_pfi->nField = nField-1;
			goto LFieldFound;
		}
		return;

	case CFieldExchange::GetFieldInfoOrdinal:
		if (nField-1 == pFX->m_pfi->nField)
		{
LFieldFound:
			pFX->m_pfi->nDataType = AFX_RFX_DATE;
			pFX->m_pfi->strName = szName;
			pFX->m_pfi->pv = &value;
			pFX->m_pfi->dwSize = sizeof(value);
			// Make sure field found only once
			ASSERT(pFX->m_bFieldFound == FALSE);
			pFX->m_bFieldFound = TRUE;
		}
		return;
	}
}

void AFXAPI RFX_LongBinary(CFieldExchange* pFX, const char *szName,
	CLongBinary& value)
{
	int w;
	ASSERT(AfxIsValidAddress(pFX, sizeof(CFieldExchange)));
	ASSERT(AfxIsValidString(szName));

	RETCODE nRetCode;
	UINT nField;
	if (!pFX->IsFieldType(&nField))
		return;

	LONG* plLength = pFX->m_prs->GetFieldLength(pFX);
	switch (pFX->m_nOperation)
	{
	case CFieldExchange::Name:
	case CFieldExchange::IsFieldNull:
	case CFieldExchange::IsFieldDirty:
	case CFieldExchange::SetFieldDirty:
	case CFieldExchange::IsFieldNullable:
		pFX->Default(szName, &value, plLength, SQL_C_DEFAULT, 0, 0);
		return;

#ifdef _DEBUG
	case CFieldExchange::BindParam:
		// CLongBinary parameters are not supported
		ASSERT(FALSE);

	case CFieldExchange::BindFieldToColumn:
		// We use SQLGetData upon Fixup instead of SQLBindCol
	case CFieldExchange::MarkForAddNew:
	case CFieldExchange::MarkForUpdate:
		// We do not archive LongBinary values
	case CFieldExchange::StoreField:
	case CFieldExchange::LoadField:
		// We do not archive LongBinary values
#endif // _DEBUG
	default:
		return;

	case CFieldExchange::Fixup:
		{
			// Give empty buffer to find size of entire LongBinary
			AFX_SQL_ASYNC(pFX->m_prs, ::SQLGetData(pFX->m_prs->m_hstmt,
				(unsigned short int)nField, SQL_C_DEFAULT, &w, 0, plLength));
			if (!pFX->m_prs->Check(nRetCode))
				pFX->m_prs->ThrowDBException(nRetCode);
			if (nRetCode == SQL_NO_DATA_FOUND)
			{
				pFX->m_prs->SetFieldFlags(nField, AFX_SQL_FIELD_FLAG_NULL,
					pFX->m_nFieldType);
				*plLength = SQL_NULL_DATA;
			}
			// If not truncated, must be null or SQL_NO_TOTAL
			if (nRetCode == SQL_SUCCESS || nRetCode == SQL_NO_DATA_FOUND)
			{
				if (*plLength == SQL_NULL_DATA)
				{
					value.m_dwDataLength = 0;
					pFX->m_prs->SetFieldFlags(nField,
						AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
					return;
				}
				else
				{
					ASSERT(*plLength == SQL_NO_TOTAL);
					// We require total length info from ODBC
					pFX->m_prs->ThrowDBException(AFX_SQL_ERROR_SQL_NO_TOTAL);
				}
			}

			ASSERT(nRetCode == SQL_SUCCESS_WITH_INFO);
			ASSERT(*plLength > 0);
			if (value.m_hData != NULL &&
				::GlobalSize(value.m_hData) < (DWORD)*plLength)
			{
				::GlobalFree(value.m_hData);
				value.m_hData = NULL;
			}

			if (value.m_hData == NULL)
			{
				value.m_hData = ::GlobalAlloc(GMEM_MOVEABLE, *plLength);
				if (value.m_hData == NULL)
					AfxThrowMemoryException();
			}
			value.m_dwDataLength = *plLength;

			const BYTE _huge* lpLongBinary;
			lpLongBinary = (const BYTE _huge*)::GlobalLock(value.m_hData);
			if (lpLongBinary == NULL)
			{
				::GlobalFree(value.m_hData);
				value.m_hData = NULL;
				AfxThrowMemoryException();
			}

			DWORD dwDataLength = 0;
			do
			{
				DWORD dwChunkSize = value.m_dwDataLength - dwDataLength;
				if (dwChunkSize > 0x8000)
					dwChunkSize = 0x8000;

				// Ignore expected data truncated warnings
				AFX_SQL_ASYNC(pFX->m_prs, ::SQLGetData(pFX->m_prs->m_hstmt,
					(unsigned short int)nField, SQL_C_BINARY,
					(UCHAR FAR*)lpLongBinary, dwChunkSize, plLength));

				dwDataLength += dwChunkSize;
				lpLongBinary += dwChunkSize;
			} while (nRetCode == SQL_SUCCESS || nRetCode == SQL_SUCCESS_WITH_INFO);

			::GlobalUnlock(value.m_hData);
			if (!pFX->m_prs->Check(nRetCode))
				pFX->m_prs->ThrowDBException(nRetCode);
		}
		return;

	case CFieldExchange::NameValue:
		if (pFX->m_prs->IsFieldFlagDirty(nField, pFX->m_nFieldType))
		{
			*pFX->m_pstr += pFX->m_prs->m_pDatabase->QuoteName(szName);
			*pFX->m_pstr += "=";
		}

		// Fall through
	case CFieldExchange::Value:
		if (pFX->m_prs->IsFieldFlagDirty(nField, pFX->m_nFieldType))
		{
			// If user marked column NULL, reflect this in length
			if (pFX->m_prs->IsFieldFlagNull(nField, pFX->m_nFieldType))
				*plLength = SQL_NULL_DATA;
			else
			{
				// Indicate data will be sent after SQLExecute
				*plLength = SQL_DATA_AT_EXEC;
			}

			// If optimizing for bulk add, just set length.
			if(!(pFX->m_prs->m_dwOptions & CRecordset::optimizeBulkAdd))
			{
				*pFX->m_pstr += "?";
				*pFX->m_pstr += pFX->m_lpszSeparator;
				pFX->m_nParamFields++;
				AFX_SQL_SYNC(::SQLSetParam(pFX->m_hstmt, (unsigned short int)pFX->m_nParamFields, SQL_C_DEFAULT,
					// Get actual SQL type - may differ from CType (e.g. DECIMAL)
					(short int)pFX->GetColumnType(nField), value.m_dwDataLength, 0, &value, plLength));
				if (nRetCode != SQL_SUCCESS)
					pFX->m_prs->ThrowDBException(nRetCode, pFX->m_hstmt);
			}
		}
		return;

	case CFieldExchange::SetFieldNull:
		if ((pFX->m_pvField == NULL &&
			pFX->m_nFieldType == CFieldExchange::outputColumn) ||
			pFX->m_pvField == &value)
		{
			if (pFX->m_bField)
			{
				// Mark fields null
				pFX->m_prs->SetFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				value.m_dwDataLength = 0;
				*plLength = SQL_NULL_DATA;
			}
			else
			{
				pFX->m_prs->ClearFieldFlags(nField,
					AFX_SQL_FIELD_FLAG_NULL, pFX->m_nFieldType);
				*plLength = SQL_DATA_AT_EXEC;
			}
#ifdef _DEBUG
			pFX->m_bFieldFound = TRUE;
#endif // _DEBUG
		}
		return;

	case CFieldExchange::GetFieldInfoValue:
		if (pFX->m_pfi->pv == &value)
		{
			pFX->m_pfi->nField = nField-1;
			goto LFieldFound;
		}
		return;

	case CFieldExchange::GetFieldInfoOrdinal:
		if (nField-1 == pFX->m_pfi->nField)
		{
LFieldFound:
			pFX->m_pfi->nDataType = AFX_RFX_LONGBINARY;
			pFX->m_pfi->strName = szName;
			pFX->m_pfi->pv = &value;
			pFX->m_pfi->dwSize = sizeof(value);
			// Make sure field found only once
			ASSERT(pFX->m_bFieldFound == FALSE);
			pFX->m_bFieldFound = TRUE;
		}
		return;
	}
}

int CFieldExchange::GetColumnType(int nColumn, UINT* pcbColumn,
	int* pnScale, int* pnNullable)
{
	ASSERT(AfxIsValidAddress(this, sizeof(CFieldExchange)));
	ASSERT(pcbColumn == NULL || AfxIsValidAddress(pcbColumn, sizeof(UINT)));
	ASSERT(pnScale == NULL || AfxIsValidAddress(pnScale, sizeof(int)));
	ASSERT(pnNullable == NULL || AfxIsValidAddress(pnNullable, sizeof(int)));

	RETCODE nRetCode;
	int cbName = 0;
	int nSqlType;
	DWORD dwT;

#ifdef _DEBUG
	UINT nResultColumns;
	AFX_SQL_ASYNC(m_prs, ::SQLNumResultCols(m_prs->m_hstmt, (short FAR*)&nResultColumns));
	// Query probably requesting column info before query has been successfully executed
	ASSERT(nColumn >= 1 && (UINT)nColumn <= nResultColumns);
#endif // _DEBUG

	AFX_SQL_ASYNC(m_prs, ::SQLDescribeCol(m_prs->m_hstmt, (unsigned short int)nColumn,
		NULL, 0, (short FAR*)&cbName, (short FAR*)&nSqlType,
		&dwT, (pnScale == NULL) ? (short FAR*)NULL : (short FAR*)pnScale,
		(pnNullable == NULL) ? (short FAR*)NULL : (short FAR*)pnNullable));

	if (!m_prs->Check(nRetCode))
	{
		m_prs->ThrowDBException(nRetCode);
	}

	if (pcbColumn != NULL)
	{
		if (dwT > MAXINT-1)
			dwT = MAXINT-1;
		*pcbColumn = (UINT)dwT;
	}

	return nSqlType;
}

//////////////////////////////////////////////////////////////////////////////
// Inline function declarations expanded out-of-line

#ifndef _AFX_ENABLE_INLINES

static char BASED_CODE _szAfxDbInl[] = "afxdb.inl";
#undef THIS_FILE
#define THIS_FILE _szAfxDbInl
#define _AFXDBRFX_INLINE
#include "afxdb.inl"

#endif
