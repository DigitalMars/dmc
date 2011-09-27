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
#include <limits.h>
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
// CMemFile implementation

IMPLEMENT_DYNAMIC(CMemFile, CFile)

CMemFile::CMemFile(UINT nGrowBytes /* = 1024 */)
{
	ASSERT(nGrowBytes <= UINT_MAX);

	m_hFile = hFileNull;
	m_nGrowBytes = nGrowBytes;
	m_nPosition = 0;
	m_nBufferSize = 0;
	m_nFileSize = 0;
	m_lpBuffer = NULL;
	m_nBufferSize = 0;
}

CMemFile::~CMemFile()
{
	// Close should have already been called, but we check anyway
	if (m_lpBuffer)
		Close();
	ASSERT(m_lpBuffer == NULL);

	m_nGrowBytes = 0;
	m_nPosition = 0;
	m_nBufferSize = 0;
	m_nFileSize = 0;
}

BYTE FAR* CMemFile::Alloc(DWORD nBytes)
{
	if (nBytes > UINT_MAX)
		return NULL;    // bigger than 64K not supported

	return (BYTE FAR*)_fmalloc((UINT)nBytes);
}

BYTE FAR* CMemFile::Realloc(BYTE FAR* lpMem, DWORD nBytes)
{
	if (nBytes > UINT_MAX)
		return NULL;    // bigger than 64K not supported

	return (BYTE FAR*)_frealloc(lpMem, (UINT)nBytes);
}

#pragma optimize("i", off)
BYTE FAR* CMemFile::Memcpy(BYTE FAR* lpMemTarget, const BYTE FAR* lpMemSource,
	UINT nBytes)
{
	ASSERT(lpMemTarget != NULL);
	ASSERT(lpMemSource != NULL);

	// Note: AfxIsValidAddress doesn't handled tiled selectors, but
	//  _fmemcpy does.  We avoid incorrect assertions when dealing
	//  with such huge pointers.
	if (HIWORD(lpMemTarget) == HIWORD((BYTE _huge*)lpMemTarget + nBytes))
		ASSERT(AfxIsValidAddress(lpMemTarget, nBytes));
	if (HIWORD(lpMemSource) == HIWORD((BYTE _huge*)lpMemSource + nBytes))
		ASSERT(AfxIsValidAddress(lpMemSource, nBytes, FALSE));

	return (BYTE FAR*)_fmemcpy(lpMemTarget, lpMemSource, nBytes);
}
#pragma optimize("", on)

void CMemFile::Free(BYTE FAR* lpMem)
{
	ASSERT(lpMem != NULL);

	_ffree(lpMem);
}

DWORD CMemFile::GetPosition() const
{
	ASSERT_VALID(this);
	return (DWORD)m_nPosition;
}

void CMemFile::GrowFile(DWORD dwNewLen)
{
	ASSERT_VALID(this);

	if (dwNewLen > m_nBufferSize)
	{
		// grow the buffer
		DWORD dwNewBufferSize = (DWORD)m_nBufferSize;

		while (dwNewBufferSize < dwNewLen)
			dwNewBufferSize += m_nGrowBytes;

		BYTE FAR* lpNew;
		if (m_lpBuffer == NULL)
			lpNew = Alloc(dwNewBufferSize);
		else
			lpNew = Realloc(m_lpBuffer, dwNewBufferSize);

		if (lpNew == NULL)
			AfxThrowMemoryException();

		m_lpBuffer = lpNew;
		m_nBufferSize = dwNewBufferSize;
	}
	ASSERT_VALID(this);
}

void CMemFile::SetLength(DWORD dwNewLen)
{
	ASSERT_VALID(this);

	if (dwNewLen > m_nBufferSize)
		GrowFile(dwNewLen);

	if (dwNewLen < m_nPosition)
		m_nPosition = dwNewLen;

	m_nFileSize = dwNewLen;
	ASSERT_VALID(this);
}


UINT CMemFile::Read(void FAR* lpBuf, UINT nCount)
{
	ASSERT_VALID(this);
	ASSERT(lpBuf != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nCount));

	UINT nRead;

	if (m_nPosition > m_nFileSize)
		return 0;

	if (m_nPosition + nCount > m_nFileSize)
		nRead = (UINT)(m_nFileSize - m_nPosition);
	else
		nRead = nCount;

	Memcpy((BYTE FAR*)lpBuf, (BYTE _huge*)m_lpBuffer + m_nPosition, nRead);
	m_nPosition += nRead;

	ASSERT_VALID(this);

	return nRead;
}

void CMemFile::Write(const void FAR* lpBuf, UINT nCount)
{
	ASSERT_VALID(this);
	ASSERT(lpBuf != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nCount, FALSE));

	if (m_nPosition + nCount > m_nBufferSize)
		GrowFile(m_nPosition + nCount);

	ASSERT(m_nPosition + nCount <= m_nBufferSize);

	Memcpy((BYTE _huge*)m_lpBuffer + m_nPosition, (BYTE FAR*)lpBuf, nCount);

	m_nPosition += nCount;

	if (m_nPosition > m_nFileSize)
		m_nFileSize = m_nPosition;

	ASSERT_VALID(this);
}


LONG CMemFile::Seek(LONG lOff, UINT nFrom)
{
	ASSERT_VALID(this);
	ASSERT(nFrom == CFile::begin || nFrom == CFile::end || nFrom == CFile::current);

	LONG lNewPos = m_nPosition;

	if (nFrom == CFile::begin)
		lNewPos = lOff;
	else if (nFrom == CFile::current)
		lNewPos += lOff;
	else if (nFrom == CFile::end)
		lNewPos = m_nFileSize + lOff;
	else
		return -1;

	if (lNewPos < 0)
		AfxThrowFileException(CFileException::badSeek);

	m_nPosition = lNewPos;

	ASSERT_VALID(this);
	return m_nPosition;
}

void CMemFile::Flush()
{
	ASSERT_VALID(this);
}

void CMemFile::Close()
{
	ASSERT_VALID(this);
	m_nGrowBytes = 0;
	m_nPosition = 0;
	m_nBufferSize = 0;
	m_nFileSize = 0;
	if (m_lpBuffer)
		Free(m_lpBuffer);
	m_lpBuffer = NULL;
}

void CMemFile::Abort()
{
	ASSERT_VALID(this);

	Close();
}

void CMemFile::LockRange(DWORD /* dwPos */, DWORD /* dwCount */)
{
	ASSERT_VALID(this);
	AfxThrowNotSupportedException();
}


void CMemFile::UnlockRange(DWORD /* dwPos */, DWORD /* dwCount */)
{
	ASSERT_VALID(this);
	AfxThrowNotSupportedException();
}

CFile* CMemFile::Duplicate() const
{
	ASSERT_VALID(this);
	AfxThrowNotSupportedException();
	return NULL;
}

#ifdef _DEBUG
void CMemFile::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CFile::Dump(dc);

	AFX_DUMP1(dc, "\n\tfile size = ", m_nFileSize);
	AFX_DUMP1(dc, "\n\tbuffer size = ", m_nBufferSize);
	AFX_DUMP1(dc, "\n\tposition = ", m_nPosition);
	AFX_DUMP1(dc, "\n\tgrowth rate = ", m_nGrowBytes);
}

void CMemFile::AssertValid() const
{
	CFile::AssertValid();

	ASSERT((m_lpBuffer == NULL && m_nBufferSize == 0) ||
		AfxIsValidAddress(m_lpBuffer, (UINT)m_nBufferSize));
	ASSERT(m_nFileSize <= m_nBufferSize);
	// m_nPosition might be after the end of file, so we cannot ASSERT
	// its validity
}

#endif // _DEBUG
