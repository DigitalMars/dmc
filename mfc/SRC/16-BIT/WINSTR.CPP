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

#ifdef AFX_CORE1_SEG
#pragma code_seg(AFX_CORE1_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Windows extensions to strings

BOOL CString::LoadString(UINT nID)
{
	ASSERT(nID != 0);       // 0 is an illegal string ID
	// NOTE: resource strings limited to 255 characters
	char szBuffer[256];
	UINT nSize;
	if ((nSize = _AfxLoadString(nID, szBuffer)) == 0)
		return FALSE;
	AssignCopy(nSize, szBuffer);
	return TRUE;
}

void CString::AnsiToOem()
{
	::AnsiToOem(m_pchData, m_pchData);
}

void CString::OemToAnsi()
{
	::OemToAnsi(m_pchData, m_pchData);
}

#ifndef _AFXDLL     // non-DLL version
int PASCAL _AfxLoadString(UINT nID, char* szBuf)
{
	ASSERT(AfxIsValidAddress(szBuf, 256));  // must be big enough for 256 bytes
#ifdef _DEBUG
	// LoadString without annoying warning from the Debug kernel if the
	//  segment containing the string is not present
	if (::FindResource(AfxGetResourceHandle(),
	   MAKEINTRESOURCE((nID>>4)+1), RT_STRING) == NULL)
		return 0; // not found
#endif //_DEBUG
	return ::LoadString(AfxGetResourceHandle(), nID, szBuf, 255);
}
#endif //!_AFXDLL

/////////////////////////////////////////////////////////////////////////////

BOOL AFXAPI AfxExtractSubString(CString& rString, LPCSTR lpszFullString,
	int iSubString, char chSep)
{
	if (lpszFullString == NULL)
		return FALSE;

	while (iSubString--)
	{
		lpszFullString = _AfxStrChr(lpszFullString, chSep);
		if (lpszFullString == NULL)
		{
			rString.Empty();        // return empty string as well
			return FALSE;
		}
		lpszFullString++;       // point past the separator
	}
	LPCSTR lpchEnd = _AfxStrChr(lpszFullString, chSep);
	int nLen = (lpchEnd == NULL) ?
		lstrlen(lpszFullString) : (int)(lpchEnd - lpszFullString);
	ASSERT(nLen >= 0);
	_fmemcpy(rString.GetBufferSetLength(nLen), lpszFullString, nLen);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// DBCS helpers

BOOL PASCAL _AfxIsDBCSLeadByte(BYTE b)
{
	if (afxData.bWin31)
		return ::IsDBCSLeadByte(b);

	// slow way of doing this with AnsiNext for Win30 compatibility
	char sz[3];
	sz[0] = b;
	sz[1] = 'X';    // pretend
	sz[2] = '\0';
	return _AfxAnsiNext(sz) != sz+1;
}

/////////////////////////////////////////////////////////////////////////////
