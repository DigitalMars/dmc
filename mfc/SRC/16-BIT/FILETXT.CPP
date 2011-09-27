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
#include <errno.h>

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#define CFile_shareMask ((UINT)(CFile::shareExclusive | CFile::shareDenyWrite | CFile::shareDenyRead | CFile::shareDenyNone | CFile::shareCompat))

// buffer size for small and medium model Read/Write operations
#define nLocalBuf ((UINT)512)

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

////////////////////////////////////////////////////////////////////////////
// CStdioFile implementation

IMPLEMENT_DYNAMIC(CStdioFile, CFile)

CStdioFile::CStdioFile()
{
	m_pStream = NULL;
}

CStdioFile::CStdioFile(FILE* pOpenStream) : CFile(hFileNull)
{
	m_pStream = pOpenStream;
	m_hFile = _fileno(pOpenStream);
	ASSERT(!m_bCloseOnDelete);
}

CStdioFile::CStdioFile(const char* pszFileName, UINT nOpenFlags)
{
	ASSERT(pszFileName != NULL);
	ASSERT(AfxIsValidString(pszFileName));

	CFileException e;
	if (!Open(pszFileName, nOpenFlags, &e))
		AfxThrowFileException(e.m_cause, e.m_lOsError);
}

CStdioFile::~CStdioFile()
{
	ASSERT_VALID(this);

	if (m_pStream != NULL && m_bCloseOnDelete)
		Close();
}

BOOL CStdioFile::Open(const char* pszFileName, UINT nOpenFlags,
	CFileException* pException)
{
	ASSERT(pException == NULL || AfxIsValidAddress(pException, sizeof(CFileException)));
	ASSERT(pszFileName != NULL);
	ASSERT(AfxIsValidString(pszFileName));
	ASSERT((nOpenFlags & CFile_shareMask) == 0);

	m_hFile = hFileNull;
	m_bCloseOnDelete = FALSE;

	char szMode[4];
	int nMode = 0;

	if (nOpenFlags & CFile::modeCreate)
		szMode[nMode++] = 'w';
	else if (nOpenFlags & CFile::modeWrite)
		szMode[nMode++] = 'a';
	else
		szMode[nMode++] = 'r';

	if (nOpenFlags & CFile::modeReadWrite)
		szMode[nMode++] = '+';

	if (nOpenFlags & CFile::typeBinary)
		szMode[nMode++] = 'b';
	else
		szMode[nMode++] = 't';

	szMode[nMode++] = '\0';

	char szOemPath[_MAX_PATH];
	AnsiToOem(pszFileName, szOemPath);

	if ((m_pStream = fopen(szOemPath, szMode)) == NULL)
	{
		if (pException != NULL)
		{
			pException->m_lOsError = _doserrno;
			pException->m_cause = CFileException::OsErrorToException(_doserrno);
		}
		return FALSE;
	}

	m_hFile = _fileno(m_pStream);
	m_bCloseOnDelete = TRUE;
	return TRUE;
}

UINT CStdioFile::Read(void FAR* lpBuf, UINT nCount)
{
	ASSERT_VALID(this);
	ASSERT(m_pStream != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nCount));

	UINT nRead = 0;

#ifdef _NEARDATA
	// S/M model, need to copy lpBuf to NEAR data (_read is NEAR)
	char buf[nLocalBuf];
	UINT nReadReq = 0;
	UINT nReadCur = 0;
	UINT nReadT = 0;

	while (nReadCur < nCount)
	{
		nReadReq = ((nCount - nRead < nLocalBuf) ? nCount - nRead : nLocalBuf);
		nReadT = fread(buf, sizeof(BYTE), nReadReq, m_pStream);
		nRead += nReadT;

		_fmemcpy(lpBuf, buf, nReadT);
		lpBuf = (BYTE FAR*)lpBuf + nReadT;

		if (nReadT < nReadReq)
		{
			if (feof(m_pStream) && !ferror(m_pStream))
				break;
			else
			{
				clearerr(m_pStream);
				AfxThrowFileException(CFileException::generic, _doserrno);
			}
		}
		nReadCur += nReadReq;
	}

#else
	if ((nRead = fread(lpBuf, sizeof(BYTE), nCount, m_pStream)) == 0 && !feof(m_pStream))
		AfxThrowFileException(CFileException::generic, _doserrno);
#endif
	if (ferror(m_pStream))
	{
		clearerr(m_pStream);
		AfxThrowFileException(CFileException::generic, _doserrno);
	}
	return nRead;
}

void CStdioFile::Write(const void FAR* lpBuf, UINT nCount)
{
	ASSERT_VALID(this);
	ASSERT(m_pStream != NULL);
	ASSERT(lpBuf != NULL);

	ASSERT(AfxIsValidAddress(lpBuf, nCount, FALSE));

#ifdef _NEARDATA
	// S/M model, need to copy lpBuf to NEAR data (_read is NEAR)
	void FAR* lpBufT = (void FAR*)lpBuf;
	UINT nWrite = 0;

	while (nWrite < nCount)
	{
		char buf[nLocalBuf];
		UINT nWriteT;

		_fmemcpy(buf, lpBufT,(nWriteT = nCount - nWrite < nLocalBuf ? nCount - nWrite : nLocalBuf));
		if (fwrite(buf, sizeof(BYTE), nWriteT, m_pStream) != nWriteT)
			AfxThrowFileException(CFileException::generic, _doserrno);
		lpBufT = (BYTE FAR*)lpBufT + nWriteT;
		nWrite += nWriteT;
	}
#else
	if (fwrite(lpBuf, sizeof(BYTE), nCount, m_pStream) == (size_t)-1)
		AfxThrowFileException(CFileException::generic, _doserrno);
#endif
}


void CStdioFile::WriteString(LPCSTR lpsz)
{
	ASSERT(lpsz != NULL);
	ASSERT(m_pStream != NULL);

	register char ch;
	while ((ch = *lpsz++) != '\0')
		if (fputc(ch, m_pStream) == EOF)
			AfxThrowFileException(CFileException::diskFull, _doserrno);
}

LPSTR CStdioFile::ReadString(LPSTR lpsz, UINT nMax)
{
	ASSERT(lpsz != NULL);
	ASSERT(AfxIsValidAddress(lpsz, nMax));
	ASSERT(m_pStream != NULL);

	UINT nRead = 0;
	LPSTR lpszT = lpsz;

	while ((UINT)nRead < nMax - 1)
	{
		int chOrEOF; // because it could be an EOF which isn't a char

		if ((chOrEOF = fgetc(m_pStream)) == EOF)
		{
			if (feof(m_pStream))
				break;
			// real error
			clearerr(m_pStream);
			AfxThrowFileException(CFileException::generic, _doserrno);
		}
		nRead++;
		if ((*lpszT++ = (char)chOrEOF) == '\n')
			break;
	}
	*lpszT = '\0';
	return (lpsz == lpszT ? NULL : lpsz);
}

LONG
CStdioFile::Seek(LONG lOff, UINT nFrom)
{
	ASSERT_VALID(this);
	ASSERT(nFrom == CFile::begin || nFrom == CFile::end || nFrom == CFile::current);
	ASSERT(sizeof(fpos_t) <= sizeof(DWORD));
	ASSERT(m_pStream != NULL);

	fpos_t pos;

	if (fseek(m_pStream, lOff, nFrom) != 0)
		AfxThrowFileException(CFileException::badSeek, _doserrno);

	fgetpos(m_pStream, &pos);
	return (DWORD)pos;
}


DWORD CStdioFile::GetPosition() const
{
	ASSERT_VALID(this);
	ASSERT(sizeof(fpos_t) <= sizeof(DWORD));
	ASSERT(m_pStream != NULL);

	fpos_t pos;

	if (fgetpos(m_pStream, &pos) != 0)
		AfxThrowFileException(CFileException::invalidFile, _doserrno);

	return (DWORD)pos;
}


void CStdioFile::Flush()
{
	ASSERT_VALID(this);

	if (m_pStream != NULL && fflush(m_pStream) != 0)
		AfxThrowFileException(CFileException::diskFull, _doserrno);
}

void CStdioFile::Close()
{
	ASSERT_VALID(this);
	ASSERT(m_pStream != NULL);

	int nErr = 0;

	if (m_pStream != NULL)
		nErr = fclose(m_pStream);

	m_hFile = hFileNull;
	m_bCloseOnDelete = FALSE;
	m_pStream = NULL;

	if (nErr != 0)
		AfxThrowFileException(CFileException::diskFull, _doserrno);
}


void CStdioFile::Abort()
{
	ASSERT_VALID(this);

	if (m_pStream != NULL && m_bCloseOnDelete)
		fclose(m_pStream);  // close but ignore errors
	m_hFile = hFileNull;
	m_pStream = NULL;
	m_bCloseOnDelete = FALSE;
}


CFile* CStdioFile::Duplicate() const
{
	ASSERT_VALID(this);
	ASSERT(m_pStream != NULL);

	AfxThrowNotSupportedException();
	return NULL;
}

void CStdioFile::LockRange(DWORD /* dwPos */, DWORD /* dwCount */)
{
	ASSERT_VALID(this);
	ASSERT(m_pStream != NULL);

	AfxThrowNotSupportedException();
}


void CStdioFile::UnlockRange(DWORD /* dwPos */, DWORD /* dwCount */)
{
	ASSERT_VALID(this);
	ASSERT(m_pStream != NULL);

	AfxThrowNotSupportedException();
}

#ifdef _DEBUG
void CStdioFile::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CFile::Dump(dc);

	AFX_DUMP1(dc, " and FILE* ", (void*)m_pStream);
	AFX_DUMP0(dc, " ");
}
#endif
