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
#include <malloc.h>

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

////////////////////////////////////////////////////////////////////////////
// Serialize member functions for low level classes put here
// for code swapping improvements

// CString serialization code
// String format: if < 255 chars: len:BYTE, characters in bytes
//              if >= 255 characters: 0xff, len:WORD, characters in bytes

CArchive& AFXAPI operator<<(CArchive& ar, const CString& string)
{
	if (string.m_nDataLength < 255)
	{
		ar << (BYTE) string.m_nDataLength;
	}
	else
	{
		ar << (BYTE) 0xff;
		ar << (WORD) string.m_nDataLength;
	}
	ar.Write(string.m_pchData, string.m_nDataLength);
	return ar;
}

CArchive& AFXAPI operator>>(CArchive& ar, CString& string)
{
	string.Empty();

	BYTE bLen;
	ar >> bLen;

	WORD nNewLen;
	if (bLen == 0xff)
		// read word of length
		ar >> nNewLen;
	else
		nNewLen = bLen;

	// read in as normal characters
	if (nNewLen != 0)
	{
		string.AllocBuffer(nNewLen);
		if (ar.Read(string.m_pchData, nNewLen) != nNewLen)
			AfxThrowArchiveException(CArchiveException::endOfFile);
	}
	return ar;
}

// Runtime class serialization code
CRuntimeClass* PASCAL CRuntimeClass::Load(CArchive& ar, UINT* pwSchemaNum)
{
	WORD nLen;
	char szClassName[64];
	CRuntimeClass* pClass;

	ar >> (WORD&)(*pwSchemaNum) >> nLen;

	if (nLen >= sizeof(szClassName) || ar.Read(szClassName, nLen) != nLen)
		return NULL;
	szClassName[nLen] = '\0';

#ifndef _AFXDLL
	for (pClass = pFirstClass; pClass != NULL; pClass = pClass->m_pNextClass)
	{
		if (lstrcmp(szClassName, pClass->m_lpszClassName) == 0)
			return pClass;
	}
#else
	// all registered CRuntimeClasses should be owned by the app or a DLL
	ASSERT(CRuntimeClass::pFirstClass == NULL);

	// first walk through the app specific classes
	for (pClass = _AfxGetAppData()->pFirstAppClass; pClass != NULL;
		pClass = pClass->m_pNextClass)
	{
		if (lstrcmp(szClassName, pClass->m_lpszClassName) == 0)
			return pClass;
	}
	// now walk through the classes in different DLLs
	CDynLinkLibrary* pDLL;
	for (pDLL = _AfxGetAppData()->pFirstDLL; pDLL != NULL;
		pDLL = pDLL->m_pNextDLL)
	{
		for (pClass = pDLL->m_pFirstSharedClass; pClass != NULL;
			pClass = pClass->m_pNextClass)
		{
			if (lstrcmp(szClassName, pClass->m_lpszClassName) == 0)
				return pClass;
		}
	}
#endif //_AFXDLL

	return NULL; // not found
}

void CRuntimeClass::Store(CArchive& ar)
	// Stores a class ref
{
	WORD nLen = (WORD)lstrlen(m_lpszClassName);

	ar << (WORD)m_wSchema << nLen;
	ar.Write(m_lpszClassName, nLen);
}

////////////////////////////////////////////////////////////////////////////
// Archive object input/output

// minimum buffer size
enum { nBufSizeMin = 128 };

////////////////////////////////////////////////////////////////////////////

CArchive::CArchive(CFile* pFile, UINT nMode,
	int nBufSize /* = 512 */, void FAR* lpBuf /* = NULL */)
{
	ASSERT_VALID(pFile);

	m_nMode = nMode;

	// initialize the buffer.  minimum size is 128
	m_lpBufStart = (BYTE FAR*)lpBuf;

	if (nBufSize < nBufSizeMin)
	{
		// force use of private buffer of minimum size
		m_nBufSize = nBufSizeMin;
		m_lpBufStart = NULL;
	}
	else
		m_nBufSize = nBufSize;

	if (m_lpBufStart == NULL)
	{
		m_lpBufStart = (BYTE FAR*)_fmalloc(m_nBufSize);
		m_bUserBuf = FALSE;
	}
	else
		m_bUserBuf = TRUE;

	ASSERT(m_lpBufStart != NULL);
	ASSERT(AfxIsValidAddress(m_lpBufStart, m_nBufSize));

	m_lpBufMax = m_lpBufStart + m_nBufSize;
	m_lpBufCur = (IsLoading()) ? m_lpBufMax : m_lpBufStart;
	m_pFile = pFile;
	m_pLoadArray = NULL;
	m_pDocument = NULL;
	m_bForceFlat = TRUE;

	ASSERT(m_pStoreMap == NULL);        // same as m_pLoadArray
}

CArchive::~CArchive()
{
	// Close makes m_pFile NULL.  If it is not NULL, we must Close the
	// CArchive.
	if (m_pFile != NULL && !(m_nMode & bNoFlushOnDelete))
		Close();

	Abort();	// abort completely shuts down the archive
}

void CArchive::Abort()
{
	ASSERT(m_lpBufStart == NULL || 
		AfxIsValidAddress(m_lpBufStart, (UINT)(m_lpBufMax - m_lpBufStart)));
	ASSERT(m_lpBufCur == NULL || 
		AfxIsValidAddress(m_lpBufCur, (UINT)(m_lpBufMax - m_lpBufCur)));

	// disconnect from the file
	m_pFile = NULL;

	if (!m_bUserBuf)
	{
		_ffree(m_lpBufStart);
		m_lpBufStart = NULL;
	}

	// m_pStoreMap and m_pLoadArray are unioned, so we only need to delete one
	ASSERT((CObject*)m_pStoreMap == (CObject*)m_pLoadArray);
	delete (CObject*)m_pLoadArray;
	m_pLoadArray = NULL;
}

void CArchive::Close()
{
	ASSERT_VALID(m_pFile);

	Flush();
	m_pFile = NULL;
}

UINT CArchive::Read(void FAR* lpBuf, UINT nMax)
{
	ASSERT_VALID(m_pFile);
	ASSERT(lpBuf != NULL);
	ASSERT(m_lpBufStart != NULL);
	ASSERT(m_lpBufCur != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nMax));
	ASSERT(AfxIsValidAddress(m_lpBufStart, (UINT)(m_lpBufMax - m_lpBufStart)));
	ASSERT(AfxIsValidAddress(m_lpBufCur, (UINT)(m_lpBufMax - m_lpBufCur)));
	ASSERT(IsLoading());

	if (nMax == 0)
		return 0;

	UINT nRead = 0;
	while (nMax > 0)
	{
		UINT nCopy = min(nMax, (UINT)(m_lpBufMax - m_lpBufCur));
		_fmemcpy(lpBuf, m_lpBufCur, nCopy);
		m_lpBufCur += nCopy;
		lpBuf = ((BYTE FAR*)lpBuf) + nCopy;
		nMax -= nCopy;
		nRead += nCopy;
		if (nMax != 0)
		{
			TRY
			{
				FillBuffer(min(nMax, (UINT)m_nBufSize));
			}
			CATCH(CArchiveException, e)
			{
				// Reached end of file
				if (e->m_cause != CArchiveException::endOfFile)
					THROW_LAST();

				nCopy = (UINT)(m_lpBufMax - m_lpBufStart);
				_fmemcpy(lpBuf, m_lpBufCur, nCopy);
				return nRead + nCopy;
			}
			END_CATCH
		}
	}

	return nRead;
}

void CArchive::Write(const void FAR* lpBuf, UINT nMax)
{
	ASSERT_VALID(m_pFile);
	ASSERT(m_lpBufStart != NULL);
	ASSERT(m_lpBufCur != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nMax, FALSE));  // read-only access needed
	ASSERT(AfxIsValidAddress(m_lpBufStart, (UINT)(m_lpBufMax - m_lpBufStart)));
	ASSERT(AfxIsValidAddress(m_lpBufCur, (UINT)(m_lpBufMax - m_lpBufCur)));
	ASSERT(IsStoring());

	while (nMax > 0)
	{
		UINT nCopy = min(nMax, (UINT)(m_lpBufMax - m_lpBufCur));
		_fmemcpy(m_lpBufCur, lpBuf, nCopy);
		m_lpBufCur += nCopy;
		lpBuf = ((BYTE FAR*)lpBuf) + nCopy;
		nMax -= nCopy;
		if (nMax != 0)
		{
			// write out the current buffer to file
			if (m_lpBufCur != m_lpBufStart)
				m_pFile->Write(m_lpBufStart, (UINT)(m_lpBufCur - m_lpBufStart));

			// restore buffer to initial state
			m_lpBufCur = m_lpBufStart;
		}
	}
}

void CArchive::Flush()
{
	ASSERT(m_lpBufStart != NULL);
	ASSERT(m_lpBufCur != NULL);
	ASSERT_VALID(m_pFile);
	ASSERT(m_lpBufStart != NULL);
	ASSERT(m_lpBufCur != NULL);
	ASSERT(AfxIsValidAddress(m_lpBufStart, (UINT)(m_lpBufMax - m_lpBufStart)));
	ASSERT(AfxIsValidAddress(m_lpBufCur, (UINT)(m_lpBufMax - m_lpBufCur)));

	if (IsLoading())
	{
		// unget the characters in the buffer, seek back unused amount
		m_pFile->Seek(-(m_lpBufMax - m_lpBufCur), CFile::current);
		m_lpBufCur = m_lpBufMax;    // empty
	}
	else
	{
		// write out the current buffer to file
		if (m_lpBufCur != m_lpBufStart)
		{
			m_pFile->Write(m_lpBufStart, (UINT)(m_lpBufCur - m_lpBufStart));
			m_pFile->Flush();
		}

		// restore buffer to initial state
		m_lpBufCur = m_lpBufStart;
	}
}

void CArchive::FillBuffer(UINT nBytesNeeded)
{
	ASSERT(IsLoading());
	ASSERT_VALID(m_pFile);
	ASSERT(m_lpBufStart != NULL);
	ASSERT(m_lpBufCur != NULL);
	ASSERT(nBytesNeeded > 0);
	ASSERT(AfxIsValidAddress(m_lpBufStart, (UINT)(m_lpBufMax - m_lpBufStart)));
	ASSERT(AfxIsValidAddress(m_lpBufCur, (UINT)(m_lpBufMax - m_lpBufCur)));

	// fill up the current buffer from file
	if (m_lpBufCur > m_lpBufStart)
	{
		// there is at least some room to fill
		UINT nUnused = 0; // bytes remaining in buffer
		UINT nActual = 0; // bytes read from file

		if ((int)(nUnused = (UINT)(m_lpBufMax - m_lpBufCur)) > 0)
		{
			_fmemcpy(m_lpBufStart, m_lpBufCur,(size_t)(m_lpBufMax-m_lpBufCur));
				   // copy unused
		}

		// read to satisfy nBytesNeeded or nLeft if possible
		nActual = nUnused;
		UINT nLeft = m_nBufSize-nUnused;
		UINT nBytes;
		BYTE FAR* lpTemp = m_lpBufStart + nUnused;
		do 
		{
			nBytes = m_pFile->Read(lpTemp, nLeft);
			lpTemp = lpTemp + nBytes;
			nActual += nBytes;
			nLeft -= nBytes;
		}
		while (nBytes > 0 && nLeft > 0 && nActual < nBytesNeeded);

		m_lpBufCur = m_lpBufStart;
		m_lpBufMax = m_lpBufStart + nActual;
	}

	// not enough data to fill request?
	if ((UINT)(m_lpBufMax - m_lpBufCur) < nBytesNeeded)
		AfxThrowArchiveException(CArchiveException::endOfFile);
}

/////////////////////////////////////////////////////////////////////////////
