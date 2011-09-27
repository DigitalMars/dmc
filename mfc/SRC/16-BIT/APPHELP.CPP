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

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Help and other support

// Strings in format ".....%1 .... %2 ...." etc.

void AFXAPI AfxFormatStrings(CString& rString, UINT nIDS,
		LPCSTR FAR* rglpsz, int nString)
{
	char szFormat[256];
	if (!_AfxLoadString(nIDS, szFormat) != 0)
	{
		TRACE1("Error: failed to load AfxFormatString string 0x%04x\n", nIDS);
		ASSERT(FALSE);
		return;
	}
	AfxFormatStrings(rString, szFormat, rglpsz, nString);
}

void AFXAPI AfxFormatStrings(CString& rString, LPCSTR lpszFormat,
		LPCSTR FAR* rglpsz, int nString)
{
	// NOTE: will not work for strings > 255 characters

	int nTotalLen = lstrlen(lpszFormat);
	for (int i = 0; i < nString; i++)
	{
		if (rglpsz[i] != NULL)
			nTotalLen += lstrlen(rglpsz[i]);
	}

	LPCSTR pchSrc = lpszFormat;
	char* pchDest = rString.GetBuffer(nTotalLen+1);
	while (*pchSrc != '\0')
	{
		if (pchSrc[0] == '%' && (pchSrc[1] >= '1' && pchSrc[1] <= '9'))
		{
			i = pchSrc[1] - '1';
			pchSrc += 2;
			if (i >= nString)
			{
				TRACE1("Error: illegal string index requested %d\n", i);
				*pchDest++ = '?';
			}
			else if (rglpsz[i] != NULL)
			{
				lstrcpy(pchDest, rglpsz[i]);
				pchDest += strlen(pchDest);
			}
		}
		else
		{
			if (_AfxIsDBCSLeadByte(*pchSrc))
				*pchDest++ = *pchSrc++; // copy first of 2 bytes
			*pchDest++ = *pchSrc++;
		}
	}
	rString.ReleaseBuffer((int)((const char*)pchDest - (const char*)rString));
			// Release will assert if we went too far
}

void AFXAPI AfxFormatString1(CString& rString, UINT nIDS, LPCSTR lpsz1)
{
	AfxFormatStrings(rString, nIDS, &lpsz1, 1);
}

void AFXAPI AfxFormatString2(CString& rString, UINT nIDS, LPCSTR lpsz1,
		LPCSTR lpsz2)
{
	LPCSTR rglpsz[2];
	rglpsz[0] = lpsz1;
	rglpsz[1] = lpsz2;
	AfxFormatStrings(rString, nIDS, rglpsz, 2);
}

/////////////////////////////////////////////////////////////////////////////
