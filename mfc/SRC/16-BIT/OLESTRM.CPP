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

#ifdef AFX_OLE_SEG
#pragma code_seg(AFX_OLE_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// COleStreamFile implementation

IMPLEMENT_DYNAMIC(COleStreamFile, CFile)

/////////////////////////////////////////////////////////////////////////////
// COleStreamFile Constructors and Destructors

COleStreamFile::COleStreamFile(LPSTREAM lpStream)
{
	m_lpStream = lpStream;
}

COleStreamFile::~COleStreamFile()
{
	if (m_lpStream != NULL && m_bCloseOnDelete)
	{
		Close();
		ASSERT(m_lpStream == NULL);
	}
}

LPSTREAM COleStreamFile::Detach()
{
	LPSTREAM lpStream = m_lpStream;
	m_lpStream = NULL;  // detach and transfer ownership of m_lpStream
	return lpStream;
}

void COleStreamFile::Attach(LPSTREAM lpStream)
{
	ASSERT(m_lpStream == NULL); // already attached to an LPSTREAM?
	ASSERT(lpStream != NULL);

	m_lpStream = lpStream;
	m_bCloseOnDelete = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// OLE CFileException helpers

void AFXAPI _AfxFillOleFileException(CFileException* pError, HRESULT hr)
{
	ASSERT(pError != NULL);
	ASSERT(FAILED(hr));

	int cause;  // portable CFileException.m_cause
	SCODE sc = GetScode(hr);

	// error codes 255 or less are DOS/Win32 error codes
	if (SCODE_SEVERITY(sc) == SEVERITY_ERROR &&
		SCODE_FACILITY(sc) == FACILITY_STORAGE &&
		SCODE_CODE(sc) < 0x100)
	{
		ASSERT(SCODE_CODE(sc) != 0);

		// throw an exception matching to the DOS error
		//  (NOTE: only the DOS error part of the SCODE becomes m_lOsError)
		cause = CFileException::OsErrorToException(SCODE_CODE(sc));
		sc = (SCODE)SCODE_CODE(sc);
	}
	else
	{
		// attempt some conversion of storage specific error codes to generic
		//  CFileException causes...
		switch (sc)
		{
		case STG_E_INUSE:
		case STG_E_SHAREREQUIRED:
			cause = CFileException::sharingViolation;
			break;

		case STG_E_NOTCURRENT:
		case STG_E_REVERTED:
		case STG_E_CANTSAVE:
		case STG_E_OLDFORMAT:
		case STG_E_OLDDLL:
			cause = CFileException::generic;
			break;

		default:
			cause = CFileException::generic;
			break;
		}
	}

	// fill in pError
	pError->m_cause = cause;
	pError->m_lOsError = (LONG)sc;
}

void AFXAPI _AfxThrowOleFileException(HRESULT hr)
{
	// ignore non-failure codes
	if (!FAILED(hr))
		return;

	// otherwise, construct and exception and throw it
	CFileException e;
	_AfxFillOleFileException(&e, hr);
	AfxThrowFileException(e.m_cause, e.m_lOsError);
}

/////////////////////////////////////////////////////////////////////////////
// COleStreamFile Attributes

static CTime PASCAL NEAR ConvertFileTime(FILETIME* pFileTime)
{
	// convert first to DOS date/time
	WORD wDosDate;
	WORD wDosTime;
	CoFileTimeToDosDateTime(pFileTime, &wDosDate, &wDosTime);

	// then convert to CTime
	return CTime(wDosDate, wDosTime);
}

BOOL COleStreamFile::GetStatus(CFileStatus& rStatus) const
{
	ASSERT_VALID(this);
	ASSERT(m_lpStream != NULL);

	// get status of the stream
	STATSTG statstg;
	if (m_lpStream->Stat(&statstg, 0) != NOERROR)
		return FALSE;

	// map to CFileStatus struct
	rStatus.m_mtime = ConvertFileTime(&statstg.mtime);
	rStatus.m_ctime = ConvertFileTime(&statstg.ctime);
	rStatus.m_atime = ConvertFileTime(&statstg.atime);
	ASSERT(statstg.cbSize.HighPart == 0);
	rStatus.m_size = statstg.cbSize.LowPart;
	rStatus.m_attribute = 0;
	rStatus.m_szFullName[0] = '\0';
	if (statstg.pwcsName != NULL)
	{
		// name was returned -- copy and free it
		_AfxFullPath(rStatus.m_szFullName, statstg.pwcsName);
		_AfxFreeTaskMem(statstg.pwcsName);
	}
	return TRUE;
}

DWORD COleStreamFile::GetPosition() const
{
	ASSERT_VALID(this);
	ASSERT(m_lpStream != NULL);

	ULARGE_INTEGER liPosition;
	LARGE_INTEGER liZero; LISet32(liZero, 0);
	HRESULT hr = m_lpStream->Seek(liZero, STREAM_SEEK_CUR, &liPosition);
	if (hr != NOERROR)
		_AfxThrowOleFileException(hr);

	ASSERT(liPosition.HighPart == 0);
	return liPosition.LowPart;
}

/////////////////////////////////////////////////////////////////////////////
// COleStreamFile Operations

BOOL COleStreamFile::OpenStream(LPSTORAGE lpStorage, LPCSTR lpszStreamName,
	DWORD nOpenFlags, CFileException* pError)
{
	ASSERT_VALID(this);
	ASSERT(m_lpStream == NULL);
	ASSERT(lpStorage != NULL);
	ASSERT(AfxIsValidString(lpszStreamName));
	ASSERT(pError == NULL ||
		AfxIsValidAddress(pError, sizeof(CFileException)));

	HRESULT hr = lpStorage->OpenStream(lpszStreamName, NULL, nOpenFlags, 0,
		&m_lpStream);
	if (FAILED(hr) && pError != NULL)
		_AfxFillOleFileException(pError, hr);

	ASSERT(FAILED(hr) || m_lpStream != NULL);
	return !FAILED(hr);
}

BOOL COleStreamFile::CreateStream(LPSTORAGE lpStorage, LPCSTR lpszStreamName,
	DWORD nOpenFlags, CFileException* pError)
{
	ASSERT_VALID(this);
	ASSERT(m_lpStream == NULL);
	ASSERT(lpStorage != NULL);
	ASSERT(AfxIsValidString(lpszStreamName));
	ASSERT(pError == NULL ||
		AfxIsValidAddress(pError, sizeof(CFileException)));

	HRESULT hr = lpStorage->CreateStream(lpszStreamName, nOpenFlags, 0, 0,
		&m_lpStream);
	if (FAILED(hr) && pError != NULL)
		_AfxFillOleFileException(pError, hr);

	ASSERT(FAILED(hr) || m_lpStream != NULL);
	return !FAILED(hr);
}

BOOL COleStreamFile::CreateMemoryStream(CFileException* pError)
{
	ASSERT_VALID(this);
	ASSERT(pError == NULL ||
		AfxIsValidAddress(pError, sizeof(CFileException)));

	HRESULT hr = CreateStreamOnHGlobal(NULL, TRUE, &m_lpStream);
	if (FAILED(hr) && pError != NULL)
		_AfxFillOleFileException(pError, hr);

	ASSERT(FAILED(hr) || m_lpStream != NULL);
	return !FAILED(hr);
}

/////////////////////////////////////////////////////////////////////////////
// COleStreamFile Overrides

CFile* COleStreamFile::Duplicate() const
{
	ASSERT_VALID(this);
	ASSERT(m_lpStream != NULL);

	LPSTREAM lpStream;
	HRESULT hr = m_lpStream->Clone(&lpStream);
	if (FAILED(hr))
		_AfxThrowOleFileException(hr);

	ASSERT(lpStream != NULL);
	COleStreamFile* pFile;

	TRY
	{
		// attempt to create the stream
		pFile = new COleStreamFile(lpStream);
		pFile->m_bCloseOnDelete = m_bCloseOnDelete;
	}
	CATCH_ALL(e)
	{
		// cleanup cloned stream
		lpStream->Release();
		THROW_LAST();
	}
	END_CATCH_ALL

	ASSERT(pFile != NULL);
	return pFile;
}

LONG COleStreamFile::Seek(LONG lOff, UINT nFrom)
{
	ASSERT_VALID(this);
	ASSERT(m_lpStream != NULL);

	ASSERT(STREAM_SEEK_SET == begin);
	ASSERT(STREAM_SEEK_CUR == current);
	ASSERT(STREAM_SEEK_END == end);

	ULARGE_INTEGER liNewPosition;
	LARGE_INTEGER liOff;
	LISet32(liOff, lOff);
	HRESULT hr = m_lpStream->Seek(liOff, nFrom, &liNewPosition);
	if (hr != NOERROR)
		_AfxThrowOleFileException(hr);

	ASSERT(liNewPosition.HighPart == 0);
	return liNewPosition.LowPart;
}

void COleStreamFile::SetLength(DWORD dwNewLen)
{
	ASSERT_VALID(this);
	ASSERT(m_lpStream != NULL);

	ULARGE_INTEGER liNewLen;
	ULISet32(liNewLen, dwNewLen);
	HRESULT hr = m_lpStream->SetSize(liNewLen);
	if (hr != NOERROR)
		_AfxThrowOleFileException(hr);
}

DWORD COleStreamFile::GetLength() const
{
	ASSERT_VALID(this);
	ASSERT(m_lpStream != NULL);

	// get status of the stream
	STATSTG statstg;
	HRESULT hr = m_lpStream->Stat(&statstg, STATFLAG_NONAME);
	if (hr != NOERROR)
		_AfxThrowOleFileException(hr);

	// map to CFileStatus struct
	ASSERT(statstg.cbSize.HighPart == 0);
	return statstg.cbSize.LowPart;
}

UINT COleStreamFile::Read(void FAR* lpBuf, UINT nCount)
{
	ASSERT_VALID(this);
	ASSERT(m_lpStream != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nCount));

	// read from the stream
	DWORD dwBytesRead;
	HRESULT hr = m_lpStream->Read(lpBuf, nCount, &dwBytesRead);
	if (hr != NOERROR)
		_AfxThrowOleFileException(hr);

	// always return number of bytes read (will always fit in UINT)
	return (UINT)dwBytesRead;
}

void COleStreamFile::Write(const void FAR* lpBuf, UINT nCount)
{
	ASSERT_VALID(this);
	ASSERT(m_lpStream != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nCount, FALSE));

	// write to the stream
	DWORD dwBytesWritten;
	HRESULT hr = m_lpStream->Write(lpBuf, nCount, &dwBytesWritten);
	if (hr != NOERROR)
		_AfxThrowOleFileException(hr);

	// if no error, all bytes should have been written
	ASSERT((UINT)dwBytesWritten == nCount);
}

void COleStreamFile::LockRange(DWORD dwPos, DWORD dwCount)
{
	ASSERT_VALID(this);
	ASSERT(m_lpStream != NULL);

	// convert parameters to long integers
	ULARGE_INTEGER liPos;
	ULISet32(liPos, dwPos);
	ULARGE_INTEGER liCount;
	ULISet32(liCount, dwCount);

	// then lock the region
	HRESULT hr = m_lpStream->LockRegion(liPos, liCount, LOCK_EXCLUSIVE);
	if (hr != NOERROR)
		_AfxThrowOleFileException(hr);
}

void COleStreamFile::UnlockRange(DWORD dwPos, DWORD dwCount)
{
	ASSERT_VALID(this);
	ASSERT(m_lpStream != NULL);

	// convert parameters to long integers
	ULARGE_INTEGER liPos;
	ULISet32(liPos, dwPos);
	ULARGE_INTEGER liCount;
	ULISet32(liCount, dwCount);

	// then lock the region
	HRESULT hr = m_lpStream->UnlockRegion(liPos, liCount, LOCK_EXCLUSIVE);
	if (hr != NOERROR)
		_AfxThrowOleFileException(hr);
}

void COleStreamFile::Abort()
{
	ASSERT_VALID(this);

	if (m_lpStream != NULL)
	{
		m_lpStream->Revert();
		_AfxRelease((LPUNKNOWN*)&m_lpStream);
	}
}

void COleStreamFile::Flush()
{
	ASSERT_VALID(this);
	ASSERT(m_lpStream != NULL);

	// commit will return an error only if the stream is transacted
	HRESULT hr = m_lpStream->Commit(0);
	if (hr != NOERROR)
		_AfxThrowOleFileException(hr);

}

void COleStreamFile::Close()
{
	ASSERT_VALID(this);
	ASSERT(m_lpStream != NULL);

	if (m_lpStream != NULL)
	{
		// commit the stream via Flush (which can be overriden)
		Flush();
		_AfxRelease((LPUNKNOWN*)&m_lpStream);
	}
}

/////////////////////////////////////////////////////////////////////////////
// COleStreamFile diagnostics

#ifdef _DEBUG
void COleStreamFile::AssertValid() const
{
	CFile::AssertValid();
}

void COleStreamFile::Dump(CDumpContext& dc) const
{
	CFile::Dump(dc);
	AFX_DUMP1(dc, "\nm_lpStream = ", m_lpStream);
}
#endif

////////////////////////////////////////////////////////////////////////////
