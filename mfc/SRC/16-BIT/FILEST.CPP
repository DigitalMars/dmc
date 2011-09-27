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
#include <dos.h>

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

////////////////////////////////////////////////////////////////////////////
// Status information for all file classes
// In this file so everyone doesn't get the CTime package

/////////////////////////////////////////////////////////////////////////////
// CFileStatus implementation
#ifdef _DEBUG
void CFileStatus::Dump(CDumpContext& dc) const
{
	AFX_DUMP0(dc, "file status information:");
	AFX_DUMP1(dc, "\nm_ctime = ", m_ctime);
	AFX_DUMP1(dc, "\nm_mtime = ", m_mtime);
	AFX_DUMP1(dc, "\nm_atime = ", m_atime);
	AFX_DUMP1(dc, "\nm_size = ", m_size);
	AFX_DUMP1(dc, "\nm_attribute = ", m_attribute);
	AFX_DUMP1(dc, "\nm_szFullName = ", m_szFullName);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CFile Status implementation
BOOL CFile::GetStatus(CFileStatus& rStatus) const
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);

	//NOTE: cannot return name of file from handle
	rStatus.m_szFullName[0] = '\0';

	struct _stat s;

	if (_fstat(m_hFile, &s) == 0)
	{
		rStatus.m_ctime = CTime(s.st_atime);
		rStatus.m_atime = rStatus.m_ctime;
		rStatus.m_mtime = rStatus.m_ctime;
		rStatus.m_size = s.st_size;
		rStatus.m_attribute = 0;    // dos won't give us this from
									// just a fd, need the path name
		return TRUE;
	}
	return FALSE;
}

BOOL PASCAL CFile::GetStatus(const char* pszFileName, CFileStatus& rStatus)
{
	// first fill in the full name of the file, undefined if we return FALSE
#ifdef _WINDOWS
	// use the canonicalize helper function
	if (!_AfxFullPath(rStatus.m_szFullName, pszFileName))
	{
		rStatus.m_szFullName[0] = '\0';
		return FALSE;
	}
#else
	if (_fullpath(rStatus.m_szFullName, pszFileName, _MAX_PATH) == NULL)
	{
		rStatus.m_szFullName[0] = '\0';
		return FALSE;
	}
#endif

	// finish filling in the structure
	WORD wAttr = CFile::normal | CFile::readOnly |
				CFile::hidden | CFile::system |
				CFile::directory | CFile::archive;

	struct _find_t find;
	char sz[_MAX_PATH];
	AnsiToOem(pszFileName, sz);

	if (_dos_findfirst(sz, wAttr, &find) == 0)
	{
		rStatus.m_mtime = CTime((WORD)find.wr_date, (WORD)find.wr_time);
		rStatus.m_ctime = rStatus.m_mtime;
		rStatus.m_atime = rStatus.m_mtime;

		rStatus.m_size = find.size;
		rStatus.m_attribute = find.attrib;
		return TRUE;
	}
	else
		return FALSE;
}


void PASCAL CFile::SetStatus(const char* pszFileName, const CFileStatus& status)
{
	UINT nErr;
	UINT wAttr;
	char sz[_MAX_PATH];

	AnsiToOem(pszFileName, sz);

	if ((nErr = _dos_getfileattr(sz, &wAttr)) != 0)
		CFileException::ThrowOsError(nErr);

	if (status.m_attribute != wAttr && (wAttr & CFile::readOnly))
	{
		// Set file attribute, only if currently readonly.
		// This way we will be able to modify the time assuming the
		// caller changed the file from readonly.
		if ((nErr = _dos_setfileattr(sz, status.m_attribute)) != 0)
			CFileException::ThrowOsError(nErr);
	}

	if (status.m_mtime.GetTime() != 0)
	{
		WORD wDate, wTime;
		int handle;

		// set the file date/time
		if ((nErr = _dos_open(sz, CFile::modeReadWrite, &handle)) != 0)
			CFileException::ThrowOsError(nErr);

		wDate = (WORD)(((UINT)status.m_mtime.GetYear() - 1980) << 9);
		wDate += (WORD)(((UINT)status.m_mtime.GetMonth()) << 5);
		wDate += (WORD)((UINT)status.m_mtime.GetDay());

		wTime = (WORD)((UINT)(status.m_mtime.GetHour()) << 11);
		wTime += (WORD)((UINT)status.m_mtime.GetMinute() << 5);
		wTime += (WORD)((UINT)status.m_mtime.GetSecond() >> 1);

		if ((nErr = _dos_setftime(handle, wDate, wTime)) != 0)
			CFileException::ThrowOsError(nErr);

		if ((nErr = _dos_close(handle)) != 0)
			CFileException::ThrowOsError(nErr);
	}

	if (status.m_attribute != wAttr && !(wAttr & CFile::readOnly))
	{
		// Set file attribute, only if currently not readonly.
		if ((nErr = _dos_setfileattr(sz, status.m_attribute)) != 0)
			CFileException::ThrowOsError(nErr);
	}

}

///////////////////////////////////////////////////////////////////////////////
// CMemFile::GetStatus implementation

BOOL CMemFile::GetStatus(CFileStatus& rStatus) const
{
	ASSERT_VALID(this);

	rStatus.m_ctime = 0;
	rStatus.m_mtime = 0;
	rStatus.m_atime = 0;
	rStatus.m_size = m_nFileSize;
	rStatus.m_attribute = CFile::normal;
	rStatus.m_szFullName[0] = '\0';
	return TRUE;
}
