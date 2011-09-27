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
#include <io.h>
#include <limits.h>
#include <malloc.h>

#include <sys\types.h>
#include <sys\stat.h>
#ifdef _PORTABLE
#include <sys\locking.h>
#endif

#ifdef AFX_CORE1_SEG
#pragma code_seg(AFX_CORE1_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

////////////////////////////////////////////////////////////////////////////
// Low level File i/o portability layer
// DOS functions not provided by C Runtimes


#include <dos.h>
#ifdef _WINDOWS
extern "C" void FAR PASCAL DOS3Call();
#define DOSCALL call DOS3Call
#else // _DOS
#define DOSCALL int 21h
#endif // _WINDOWS

#ifndef _PORTABLE
// Non-Portable, DOSCALL specific implementation
//  Most efficient and gives most accurate failure codes

#pragma optimize("qgel", off) // assembler cannot be globally optimized

static UINT PASCAL _Afx_seek(UINT hFile, LONG lOff, UINT nMode,
			DWORD FAR* lpdwNew)
{
	UINT res;
	*lpdwNew = -1;
	_asm {
			mov     bx, hFile
			mov     ax, nMode
			mov     ah, 42h
			mov     dx, word ptr lOff
			mov     cx, word ptr lOff+2
			DOSCALL
			jc      __seek_err
			les     bx, lpdwNew
			mov     word ptr es:[bx], ax
			mov     word ptr es:[bx+2], dx
			xor     ax, ax
	__seek_err:
			mov     res, ax
	}
	return res;
}

static UINT PASCAL _Afx_rename(LPCSTR lpszOld, LPCSTR lpszNew)
{
	UINT res;
	_asm {
			mov     bx, ds
			lds     dx, lpszOld
			les     di, lpszNew
			mov     ax, 5600h
			DOSCALL
			jc      __rename_err
			xor     ax, ax
	__rename_err:
			mov     res, ax
			mov     ds, bx
	}
	return res;
}

static UINT PASCAL _Afx_remove(LPCSTR lpsz)
{
	UINT res;
	_asm {
			mov     bx, ds
			lds     dx, lpsz
			mov     ax, 4100h
			DOSCALL
			jc      __remove_err
			xor     ax, ax
	__remove_err:
			mov     res, ax
			mov     ds, bx
	}
	return res;
}

static UINT PASCAL _Afx_lock(UINT hFile, DWORD dwStart, DWORD dwLen, BOOL bUnlock)
{
	UINT res;
	_asm {
			mov     ax, bUnlock
			mov     ah, 5ch
			mov     bx, hFile
			mov     dx, word ptr dwStart
			mov     cx, word ptr dwStart+2
			mov     di, word ptr dwLen
			mov     si, word ptr dwLen+2
			DOSCALL
			jc      __lock_err1
			xor     ax, ax
	__lock_err1:
			mov     res, ax

	}
	return res;
}

#pragma optimize("", on)    // return to default optimizations

#else   // _PORTABLE implementations

// Portable, C-Runtime implementation
//  More portable but can gives less accurate failure codes
static UINT PASCAL _Afx_seek(UINT hFile, LONG lOff, UINT nMode,
			DWORD FAR* lpdwNew)
{
	*lpdwNew = (DWORD)_lseek((int)hFile, lOff, (int)nMode);
	if (*lpdwNew == (DWORD)-1L)
	{
		if (errno == EBADF)
			return 0x6; // invalid file
		else
			return 0x19; // bad seek
	}
	return 0;
}

static UINT PASCAL _Afx_rename(LPCSTR lpszOld, LPCSTR lpszNew)
{
	char* pszOld;
	char* pszNew;
#ifdef _NEARDATA
	char szOld[_MAX_PATH];
	char szNew[_MAX_PATH];
	lstrcpy(szOld, lpszOld);
	lstrcpy(szNew, lpszNew);
	pszOld = szOld;
	pszNew = szNew;
#else
	pszOld = (LPSTR)lpszOld;
	pszNew = (LPSTR)lpszNew;
#endif
	if (rename(pszOld, pszNew) != 0)
	{
		if (errno == EACCES)
			return 0x5; // access denied
		else
			return 0x2; // file not found
	}
	return 0;
}

static UINT PASCAL _Afx_remove(LPCSTR lpsz)
{
	char* psz;
#ifdef _NEARDATA
	char sz[_MAX_PATH];
	lstrcpy(sz, lpsz);
	psz = sz;
#else
	psz = (LPSTR)lpsz;
#endif
	if (remove(psz) != 0)
	{
		if (errno == EACCES)
			return 0x5; // access denied
		else
			return 0x2; // file not found
	}
	return 0;
}

static UINT PASCAL _Afx_lock(UINT hFile, DWORD dwStart, DWORD dwLen, BOOL bUnlock)
{
	DWORD dwPos;
	DWORD dw;
	UINT nErr;

	if (_Afx_seek(hFile, 0, 0, &dwPos) != 0)
		return 0;
	if ((nErr = _Afx_seek(hFile, (LONG)dwStart, 0, &dw)) != 0)
	{
		_Afx_seek(hFile, dwPos, 0, &dw);
		return nErr;
	}
	if (_locking(hFile, bUnlock ? _LK_UNLCK : _LK_NBLCK, dwLen) != 0)
	{
		switch (errno)
		{
		case EACCES:
			return 0x21; // lock violation
		case EBADF:
			return 0x6; // bad file handle
		default:
			return 0x1;
		}
	}
	_Afx_seek(hFile, dwPos, 0, &dw);
	return 0;
}
#endif // _PORTABLE

////////////////////////////////////////////////////////////////////////////
// CFile implementation

IMPLEMENT_DYNAMIC(CFile, CObject)

CFile::CFile()
{
	m_hFile = hFileNull;
	m_bCloseOnDelete = FALSE;
}

CFile::CFile(int hFile)
{
	m_hFile = hFile;
	m_bCloseOnDelete = FALSE;
}

CFile::CFile(const char* pszFileName, UINT nOpenFlags)
{
	ASSERT(AfxIsValidString(pszFileName));

	CFileException e;
	if (!Open(pszFileName, nOpenFlags, &e))
		AfxThrowFileException(e.m_cause, e.m_lOsError);
}

CFile::~CFile()
{
	if (m_hFile != (UINT)hFileNull && m_bCloseOnDelete)
		Close();
}

CFile* CFile::Duplicate() const
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);

	CFile* pFile = new CFile(hFileNull);
	pFile->m_hFile = (UINT) _dup((int)m_hFile);
	ASSERT(((int)(pFile->m_hFile)) != -1);
	pFile->m_bCloseOnDelete = m_bCloseOnDelete;
	return pFile;
}

BOOL CFile::Open(const char* pszFileName, UINT nOpenFlags,
	CFileException* pException /* = NULL */)
{
	ASSERT(AfxIsValidString(pszFileName));
	ASSERT(pException == NULL || AfxIsValidAddress(pException, sizeof(CFileException)));
	ASSERT((nOpenFlags & typeText) == 0);

	// CFile objects are always binary and _dos_open does not need flag
	nOpenFlags &= ~(UINT)CFile::typeBinary;

	m_bCloseOnDelete = FALSE;
	m_hFile = (UINT)hFileNull;

	ASSERT_VALID(this);

	char szOemPath[_MAX_PATH];
	UINT nErr;

	AnsiToOem(pszFileName, szOemPath);

	if (nOpenFlags & (UINT)CFile::modeCreate)
	{
		if ((nErr = _dos_creat(szOemPath, CFile::normal, (int*)&m_hFile)) != 0)
		{
			if (pException != NULL)
			{
				pException->m_lOsError = nErr;
				pException->m_cause = CFileException::OsErrorToException(nErr);
				return FALSE; // file was not created
			}
		}
		if ((nErr = _dos_close(m_hFile)) != 0)
		{
			// try to delete the file and throw modeCreate exception
			_Afx_remove(szOemPath);
			if (pException != NULL)
			{
				pException->m_lOsError = nErr;
				pException->m_cause = CFileException::OsErrorToException(nErr);
			}
			return FALSE;
		}
	}

	// the file has been modeCreated if needed, now open it
	if ((nErr = _dos_open(szOemPath, nOpenFlags & ~(UINT)CFile::modeCreate, (int*)&m_hFile)) != 0)
	{
		// throw open exception if requested
		if (pException != NULL)
		{
			pException->m_lOsError = nErr;
			pException->m_cause = CFileException::OsErrorToException(nErr);
		}
		return FALSE;
	}
	m_bCloseOnDelete = TRUE;
	return TRUE;
}

UINT CFile::Read(void FAR* lpBuf, UINT nCount)
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);
	ASSERT(lpBuf != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nCount));

	UINT nRead = 0;
	UINT nErr;

	if ((nErr = _dos_read(m_hFile, lpBuf, nCount, &nRead)) != 0)
		CFileException::ThrowOsError(nErr);

	return nRead;
}

void CFile::Write(const void FAR* lpBuf, UINT nCount)
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);
	ASSERT(lpBuf != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nCount, FALSE));

	if (nCount == 0)
		return;

	UINT nWritten = 0;
	UINT nErr;

	if ((nErr = _dos_write(m_hFile, lpBuf, nCount, &nWritten)) != 0)
		CFileException::ThrowOsError(nErr);

	if (nCount != nWritten)
		AfxThrowFileException(CFileException::diskFull);
}

LONG CFile::Seek(LONG lOff, UINT nFrom)
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);
	ASSERT(nFrom == CFile::begin || nFrom == CFile::end || nFrom == CFile::current);

	DWORD dwNew;
	UINT nErr;

	if ((nErr = _Afx_seek(m_hFile, lOff, nFrom, &dwNew)) != 0)
		CFileException::ThrowOsError(nErr);

	return dwNew;
}

DWORD CFile::GetPosition() const
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);

	DWORD dwPos;
	UINT nErr;

	if ((nErr = _Afx_seek(m_hFile, 0, CFile::current, &dwPos)) != 0)
		CFileException::ThrowOsError(nErr);

	return dwPos;
}

#pragma optimize("qgel", off) // assembler cannot be globally optimized
void CFile::Flush()
{
	ASSERT_VALID(this);

	UINT nErr;

	if (m_hFile == (UINT)hFileNull)
		return;

#ifndef _PORTABLE
	// SmartDrive 4.0 workaround, carry flag incorrectly propogated
	_asm { CLC }
#endif

	if ((nErr = _dos_commit(m_hFile)) != 0)
		CFileException::ThrowOsError(nErr);
}
#pragma optimize("", on)    // return to default optimizations

void CFile::Close()
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);

	UINT nErr = 0;

	if (m_hFile != (UINT)hFileNull)
		nErr = _dos_close(m_hFile);

	m_hFile = hFileNull;
	m_bCloseOnDelete = FALSE;

	if (nErr != 0)
		CFileException::ThrowOsError(nErr);
}

void CFile::Abort()
{
	ASSERT_VALID(this);
	if (m_hFile != (UINT)hFileNull)
	{
		// close but ignore errors
		_dos_close(m_hFile);
		m_hFile = (UINT)hFileNull;
	}
}

void CFile::LockRange(DWORD dwPos, DWORD dwCount)
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);

	UINT nErr;

	if ((nErr = _Afx_lock(m_hFile, dwPos, dwCount, FALSE)) != 0)
		CFileException::ThrowOsError(nErr);
}

void CFile::UnlockRange(DWORD dwPos, DWORD dwCount)
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);

	UINT nErr;

	if ((nErr = _Afx_lock(m_hFile, dwPos, dwCount, TRUE)) != 0)
		CFileException::ThrowOsError(nErr);
}

void CFile::SetLength(DWORD dwNewLen)
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);

	UINT nErr;
	UINT nWritten = 0;

	Seek(dwNewLen, CFile::begin);

	if ((nErr = _dos_write(m_hFile, NULL, 0, &nWritten)) != 0)
		CFileException::ThrowOsError(nErr);
}

DWORD CFile::GetLength() const
{
	ASSERT_VALID(this);

	DWORD dwLen, dwCur;

	// Seek is a non const operation
	dwCur = ((CFile*)this)->Seek(0L, CFile::current);
	dwLen = ((CFile*)this)->SeekToEnd();
	VERIFY(dwCur == (DWORD)(((CFile*)this)->Seek(dwCur, CFile::begin)));

	return dwLen;
}

void PASCAL CFile::Rename(const char* pszOldName, const char* pszNewName)
{
	char szOld[_MAX_PATH];
	char szNew[_MAX_PATH];
	UINT nErr;

	AnsiToOem(pszOldName, szOld);
	AnsiToOem(pszNewName, szNew);

	if ((nErr = _Afx_rename(szOld, szNew)) != 0)
		CFileException::ThrowOsError(nErr);

}

void PASCAL CFile::Remove(const char* pszFileName)
{
	UINT nErr;
	char sz[_MAX_PATH];

	AnsiToOem(pszFileName, sz);

	if ((nErr = _Afx_remove(sz)) != 0)
		CFileException::ThrowOsError(nErr);
}

#ifdef _DEBUG
void CFile::AssertValid() const
{
	CObject::AssertValid();
	// we permit the descriptor m_hFile to be any value for derived classes
}

void CFile::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CObject::Dump(dc);
	AFX_DUMP1(dc, " with handle ", (UINT)m_hFile);
}
#endif

/////////////////////////////////////////////////////////////////////////////
