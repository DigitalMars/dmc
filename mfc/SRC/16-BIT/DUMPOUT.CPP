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
#include <stdarg.h>

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#ifdef _DEBUG   // entire file

/////////////////////////////////////////////////////////////////////////////
// Diagnostic Stream Globals

#ifndef _AFXDLL
extern "C" BOOL NEAR afxTraceEnabled = 0;

#ifdef _WINDOWS
extern "C" int NEAR afxTraceFlags = 0;
#endif
#endif //!_AFXDLL


/////////////////////////////////////////////////////////////////////////////
// Helper routines that can be called from debugger

extern "C" void AFXAPI AfxDump(const CObject* pOb)
{
	afxDump << pOb;
}

/////////////////////////////////////////////////////////////////////////////
// Diagnostic Trace

extern "C" void CDECL
AfxTrace(LPCSTR pszFormat, ...)
{
#ifdef _DEBUG // all AfxTrace output is controlled by afxTraceEnabled
	if (!afxTraceEnabled)
		return;
#endif //_DEBUG

	int nBuf;
#ifndef _WINDLL
	char szBuffer[512];
#else
	static char szBuffer[512];
#endif
	const char* pszLocalFormat;

#ifdef _NEARDATA
	char szFormat[128];
	ASSERT(lstrlen(pszFormat) < 128);
	lstrcpy(szFormat, pszFormat);
	pszLocalFormat = szFormat;
#else
	pszLocalFormat = pszFormat;
#endif

	va_list args;
	va_start(args, pszFormat);

	nBuf = vsprintf(szBuffer, pszLocalFormat, args);
	ASSERT(nBuf < sizeof(szBuffer));

#ifdef _WINDOWS
	if ((afxTraceFlags & 1) && (AfxGetApp() != NULL))
		afxDump << AfxGetApp()->m_pszExeName << ": ";
#endif

	afxDump << szBuffer;
}

#ifdef _AFXDLL

#ifdef _NEARDATA
#error _AFXDLL requires large model
#endif
#ifndef _WINDOWS
#error _AFXDLL requires _WINDOWS
#endif

extern "C"
void AFX_EXPORT CALLBACK AfxTraceV(LPCSTR lpszFormat, const void FAR* lpArgs)
{
	int nBuf;
	char szBuffer[512];

	nBuf = vsprintf(szBuffer, lpszFormat, (va_list)lpArgs);
	ASSERT(nBuf < sizeof(szBuffer));

	if ((afxTraceFlags & 1) && (AfxGetApp() != NULL))
		afxDump << AfxGetApp()->m_pszExeName << ": ";
	afxDump << szBuffer;
}
#endif //_AFXDLL

/////////////////////////////////////////////////////////////////////////////

#endif //_DEBUG, entire file
