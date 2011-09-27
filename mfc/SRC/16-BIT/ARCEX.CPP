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

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG

// character strings to use for dumping CArchiveException
static char BASED_CODE szNone[] = "none";
static char BASED_CODE szGeneric[] = "generic";
static char BASED_CODE szReadOnly[] = "readOnly";
static char BASED_CODE szEndOfFile[] = "endOfFile";
static char BASED_CODE szWriteOnly[] = "writeOnly";
static char BASED_CODE szBadIndex[] = "badIndex";
static char BASED_CODE szBadClass[] = "badClass";
static char BASED_CODE szBadSchema[] = "badSchema";

static LPCSTR BASED_CODE rgszCArchiveExceptionCause[] =
{
	szNone,
	szGeneric,
	szReadOnly,
	szEndOfFile,
	szWriteOnly,
	szBadIndex,
	szBadClass,
	szBadSchema,
};

static char BASED_CODE szUnknown[] = "unknown";
#endif

/////////////////////////////////////////////////////////////////////////////
// CArchiveException

IMPLEMENT_DYNAMIC(CArchiveException, CException)
#define new DEBUG_NEW

#ifdef _DEBUG
void
CArchiveException::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
	AFX_DUMP0(dc, " m_cause = ");
	if (m_cause >= 0 &&
		m_cause < sizeof(rgszCArchiveExceptionCause) / sizeof(LPCSTR))
	{
		dc << rgszCArchiveExceptionCause[m_cause];
	}
	else
	{
		dc << szUnknown;
	}
}
#endif //_DEBUG

void AFXAPI AfxThrowArchiveException(int cause)
{
#ifdef _DEBUG
	LPCSTR lpsz;
	if (cause >= 0 &&
		cause < sizeof(rgszCArchiveExceptionCause) / sizeof(LPCSTR))
	{
		lpsz = rgszCArchiveExceptionCause[cause];
	}
	else
	{
		lpsz = szUnknown;
	}
	TRACE1("CArchive exception: %Fs\n", lpsz);
#endif //_DEBUG

	THROW(new CArchiveException(cause));
}
