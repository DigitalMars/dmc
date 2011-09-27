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
#include <stdarg.h>
#include <ctype.h>

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

//////////////////////////////////////////////////////////////////////////////
// More sophisticated construction

CString::CString(char ch, int nLength)
{
	ASSERT(!_AfxIsDBCSLeadByte(ch));    // can't create a lead byte string
	if (nLength < 1)
		// return empty string if invalid repeat count
		Init();
	else
	{
		AllocBuffer(nLength);
		memset(m_pchData, ch, nLength);
	}
}

CString::CString(const char* pch, int nLength)
{
	if (nLength == 0)
		Init();
	else
	{
		ASSERT(pch != NULL);
		AllocBuffer(nLength);
		memcpy(m_pchData, pch, nLength);
	}
}

//////////////////////////////////////////////////////////////////////////////
// Additional constructors for far string data

#ifdef _NEARDATA

CString::CString(LPCSTR lpch, int nLen)
{
	if (nLen == 0)
		Init();
	else
	{
		AllocBuffer(nLen);
		_fmemcpy(m_pchData, lpch, nLen);
	}
}

#endif //_NEARDATA

//////////////////////////////////////////////////////////////////////////////
// Assignment operators

const CString& CString::operator =(char ch)
{
	ASSERT(!_AfxIsDBCSLeadByte(ch));    // can't set single lead byte
	AssignCopy(1, &ch);
	return *this;
}

//////////////////////////////////////////////////////////////////////////////
// less common string expressions

CString AFXAPI operator +(const CString& string1, char ch)
{
	CString s;
	s.ConcatCopy(string1.m_nDataLength, string1.m_pchData, 1, &ch);
	return s;
}


CString AFXAPI operator +(char ch, const CString& string)
{
	CString s;
	s.ConcatCopy(1, &ch, string.m_nDataLength, string.m_pchData);
	return s;
}

//////////////////////////////////////////////////////////////////////////////
// Very simple sub-string extraction

CString CString::Mid(int nFirst) const
{
	return Mid(nFirst, m_nDataLength - nFirst);
}

CString CString::Mid(int nFirst, int nCount) const
{
	ASSERT(nFirst >= 0);
	ASSERT(nCount >= 0);

	// out-of-bounds requests return sensible things
	if (nFirst + nCount > m_nDataLength)
		nCount = m_nDataLength - nFirst;
	if (nFirst > m_nDataLength)
		nCount = 0;

	CString dest;
	AllocCopy(dest, nCount, nFirst, 0);
	return dest;
}

CString CString::Right(int nCount) const
{
	ASSERT(nCount >= 0);

	if (nCount > m_nDataLength)
		nCount = m_nDataLength;

	CString dest;
	AllocCopy(dest, nCount, m_nDataLength-nCount, 0);
	return dest;
}

CString CString::Left(int nCount) const
{
	ASSERT(nCount >= 0);

	if (nCount > m_nDataLength)
		nCount = m_nDataLength;

	CString dest;
	AllocCopy(dest, nCount, 0, 0);
	return dest;
}

// strspn equivalent
CString CString::SpanIncluding(const char* pszCharSet) const
{
	ASSERT(pszCharSet != NULL);
	return Left(strspn(m_pchData, pszCharSet));
}

// strcspn equivalent
CString CString::SpanExcluding(const char* pszCharSet) const
{
	ASSERT(pszCharSet != NULL);
	return Left(strcspn(m_pchData, pszCharSet));
}

//////////////////////////////////////////////////////////////////////////////
// Finding

int CString::ReverseFind(char ch) const
{
	char* psz;
#ifdef _WINDOWS
	if (afxData.bDBCS)
	{
		// compare forward remembering the last match
		char* pszLast = NULL;
		psz = m_pchData;
		while (*psz != '\0')
		{
			if (*psz == ch)
				pszLast = psz;
			psz = _AfxAnsiNext(psz);
		}
		psz = pszLast;
	}
	else
#endif
		psz = (char*)strrchr(m_pchData, ch);

	return (psz == NULL) ? -1 : (int)(psz - m_pchData);
}

// find a sub-string (like strstr)
int CString::Find(const char* pszSub) const
{
	ASSERT(pszSub != NULL);
	char* psz;

#ifdef _WINDOWS
	if (afxData.bDBCS)
	{
		for (psz = m_pchData; TRUE; psz = _AfxAnsiNext(psz))
		{
			// stop looking if at end of string
			if (*psz == '\0')
			{
				psz = NULL;
				break;
			}

			// compare the substring against current position
			const char* psz1 = psz;
			const char* psz2 = pszSub;
			while (*psz2 == *psz1 && *psz2 != '\0')
				++psz1, ++psz2;

			// continue looking unless there was a match
			if (*psz2 == '\0')
				break;
		}
	}
	else
#endif
		psz = (char*)strstr(m_pchData, pszSub);

	// return -1 for not found, distance from beginning otherwise
	return (psz == NULL) ? -1 : (int)(psz - m_pchData);
}

/////////////////////////////////////////////////////////////////////////////
// CString formatting

#define FORCE_NEAR		0x1000
#define FORCE_FAR		0x2000

#ifndef _AfxAnsiNext
#define _AfxAnsiNext(p) (p+1)
#endif

void CString::Format(const char* lpszFormat, ...)
{
	va_list argList;
	va_start(argList, lpszFormat);

	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (const char* lpsz = lpszFormat; *lpsz != '\0'; lpsz = _AfxAnsiNext(lpsz))
	{
		// handle '%' character, but watch out for '%%'
		if (*lpsz != '%' || *(lpsz = _AfxAnsiNext(lpsz)) == '%')
		{
			++nMaxLen;
			if (_AfxIsDBCSLeadByte(*lpsz))
				++nMaxLen;
			continue;
		}

		int nItemLen = 0;

		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != '\0'; lpsz = _AfxAnsiNext(lpsz))
		{
			// check for valid flags
			if (*lpsz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' ||
				*lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = atoi(lpsz);
			for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = _AfxAnsiNext(lpsz))
				;
		}
		ASSERT(nWidth >= 0);

		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			lpsz = _AfxAnsiNext(lpsz);

			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(argList, int);
				lpsz = _AfxAnsiNext(lpsz);
			}
			else
			{
				nPrecision = atoi(lpsz);
				for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = _AfxAnsiNext(lpsz))
					;
			}
			ASSERT(nPrecision >= 0);
		}

		// should be on type modifier or specifier
		int nModifier = 0;
		switch (*lpsz)
		{
		// modifiers that affect size
		case 'F':
			nModifier = FORCE_FAR;
			lpsz = _AfxAnsiNext(lpsz);
			break;
		case 'N':
			nModifier = FORCE_NEAR;
			lpsz = _AfxAnsiNext(lpsz);
			break;

		// modifiers that do not affect size
		case 'h':
		case 'l':
		case 'L':
			lpsz = _AfxAnsiNext(lpsz);
			break;
		}

		// now should be on specifier
		switch (*lpsz | nModifier)
		{
		// single characters
		case 'c':
		case 'C':
			nItemLen = 2;
			va_arg(argList, TCHAR);
			break;

		// strings
		case 's':
		case 'S':
			nItemLen = lstrlen(va_arg(argList, char*));
			nItemLen = max(1, nItemLen);
			break;
		case 's'|FORCE_NEAR:
		case 'S'|FORCE_NEAR:
			nItemLen = lstrlen(va_arg(argList, char NEAR*));
			nItemLen = max(1, nItemLen);
			break;
		case 's'|FORCE_FAR:
		case 'S'|FORCE_FAR:
			nItemLen = lstrlen(va_arg(argList, char FAR*));
			nItemLen = max(1, nItemLen);
			break;
		}

		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			nItemLen = max(nItemLen, nWidth);
			if (nPrecision != 0)
				nItemLen = min(nItemLen, nPrecision);
		}
		else
		{
			switch (*lpsz)
			{
			// integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				va_arg(argList, int);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			case 'e':
			case 'f':
			case 'g':
			case 'G':
				va_arg(argList, _AFXDOUBLE);
				nItemLen = 128;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			case 'p':
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			// no output
			case 'n':
				va_arg(argList, int*);
				break;

			default:
				ASSERT(FALSE);  // unknown formatting option
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}
	va_end(argList);

	va_start(argList, lpszFormat);// restart the arg list
	GetBuffer(nMaxLen);
	VERIFY(vsprintf(m_pchData, lpszFormat, argList) <= GetAllocLength());
	ReleaseBuffer();

	va_end(argList);
}

void CString::TrimRight()
{
	// find beginning of trailing spaces by starting at beginning (DBCS aware)
	char* lpsz = m_pchData;
	char* lpszLast = NULL;
	while (*lpsz != '\0')
	{
		if (isspace(*lpsz))
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
			lpszLast = NULL;
		lpsz = _AfxAnsiNext(lpsz);
	}

	if (lpszLast != NULL)
	{
		// truncate at trailing space start
		*lpszLast = '\0';
		m_nDataLength = lpszLast - m_pchData;
	}
}

void CString::TrimLeft()
{
	// find first non-space character
	const char* lpsz = m_pchData;
	while (isspace(*lpsz))
		lpsz = _AfxAnsiNext(lpsz);

	// fix up data and length
	int nDataLength = m_nDataLength - (lpsz - m_pchData);
	memmove(m_pchData, lpsz, (nDataLength+1)*sizeof(TCHAR));
	m_nDataLength = nDataLength;
}

///////////////////////////////////////////////////////////////////////////////
