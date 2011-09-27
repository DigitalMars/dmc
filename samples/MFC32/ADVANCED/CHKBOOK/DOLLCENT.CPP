// dollcent.cpp : implementation of DDX_DollarsCents
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "chkbook.h"
#include <stdio.h>
#include <stdlib.h>

void SetDollarsCents(HWND hWnd, DWORD dwCents);
CString HundredsTensOnes(DWORD dwHundredsTensOnes);
CString TensOnes(DWORD dwTensOnes);


/////////////////////////////////////////////////////////////////////////////
// Public functions

void AFXAPI DDX_DollarsCents(CDataExchange* pDX, int nIDC, DWORD& dwCents)
{
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		if (!GetDollarsCents(hWndCtrl, dwCents))
		{
			AfxMessageBox(IDS_INVALID_DOLLAR_CENT);
			pDX->Fail();
		}
	}
	else
	{
		SetDollarsCents(hWndCtrl, dwCents);
	}
}


BOOL GetDollarsCents(CWnd* pWnd, DWORD& dwCents)
{
	ASSERT(pWnd != NULL);
	return GetDollarsCents(pWnd->m_hWnd, dwCents);
}

BOOL GetDollarsCents(HWND hWnd, DWORD& dwCents)
{
	TCHAR szWindowText[20];
	::GetWindowText(hWnd, szWindowText, 19);
	DWORD dwDollars;
	int nCents;
	TCHAR* psz;
	TCHAR* pszDollars;
	TCHAR* pszCents;

	// strip leading blanks
	for (pszDollars = szWindowText;  *pszDollars == ' ';  pszDollars++)
	{
		if (*pszDollars == 0)
		{
			dwCents = 0;
			return TRUE;
		}
	}

	// parse dollar amount, before optional decimal point
	for (psz = pszDollars; (*psz != '.') && (*psz != ' ') && (*psz != 0); psz++)
	{
		if ((*psz < '0') || (*psz > '9'))
			return FALSE;
	}
	BOOL bDollarsOnly = (*psz == 0);
	*psz = 0;

	if (_tcslen(pszDollars) > 8)
		return FALSE;
	if (_tcslen(pszDollars) == 0)
	{
		dwDollars = 0L;
	}
	else
	{
		dwDollars = _ttol(pszDollars);
		if (dwDollars > ((DWORD)0xffffffff)/100)
			return FALSE;
	}

	if (bDollarsOnly)
	{
		nCents = 0;
	}
	else  // decimal point was found
	{
		// parse cents
		for (pszCents = ++psz; (*psz != 0) && (*psz != ' '); psz++)
		{
			if ((*psz < '0') || (*psz > '9'))
				return FALSE;
		}
		if (*psz == ' ')
		{
			for (psz++ ; *psz != 0; psz++)
			{
				if (*psz != ' ')
					return FALSE;
			}
		}

		int nCentsStrLen = _tcslen(pszCents);
		switch (nCentsStrLen)
		{
			case 0:
				nCents = 0;
				break;
			case 1:
				nCents = _ttoi(pszCents) * 10;
				break;
			case 2:
				nCents = _ttoi(pszCents);
				break;
			default:
				return FALSE;
		}
	}

	dwCents = dwDollars * 100 + nCents;
	return TRUE;
}


void SetDollarsCents(CWnd* pWnd, DWORD dwCents)
{
	ASSERT(pWnd != NULL);
	SetDollarsCents(pWnd->m_hWnd, dwCents);
}

void SetDollarsCents(HWND hWnd, DWORD dwCents)
{
	// Convert the DWORD dollars/cents value to a string and
	// display it in the dollars/cents control.

	// If the dollar cent field has been previously determined by
	// DDX_DollarsCents() to be invalid, then don't update it.
	// Leave the invalid data in the field so the user can correct
	// it, rather than replace it with the literal translation
	// of the INVALID_DOLLARS_CENTS #define value.

	if (dwCents == INVALID_DOLLARS_CENTS)
		return;

	CString str = GetDollarsCentsFormatted(dwCents);
	::SetWindowText(hWnd, str.GetBufferSetLength(20));
}


CString GetDollarsCentsFormatted(DWORD dwCents)
{
	if (dwCents == INVALID_DOLLARS_CENTS)
	{
		CString str;
		str.LoadString(IDS_UNKNOWN);
		return str;
	}

	DWORD dwDollars = dwCents / 100;
	WORD wCents = (WORD)(dwCents - 100 * dwDollars);

	CString str;
	str.Format(_T("%lu.%02u"), dwDollars, wCents);
	return str;
}


CString GetDollarsCentsText(DWORD dwCents)
{
	CString str, strTemp;
	if (dwCents == INVALID_DOLLARS_CENTS)
	{
		str.LoadString(IDS_UNKNOWN);
		return str;
	}

	DWORD dwDollars = dwCents / 100;
	WORD wCents = (WORD)(dwCents - (dwDollars * 100));
	if (dwDollars == 0L)
	{
		str.LoadString(IDS_ONES_0);   // "Zero"
		str += ' ';
	}
	else
	{
		if (dwDollars >= 1000000)
		{
			DWORD dwMillions = dwDollars / 1000000;
			CString strMillions = HundredsTensOnes(dwMillions);
			strTemp.LoadString(IDS_MILLION);    // "Million"
			str = strMillions;
			str += ' ';
			str += strTemp;
			str += ' ';
			dwDollars -= (dwMillions * 1000000);
		}
		if (dwDollars >= 1000)
		{
			DWORD dwThousands = dwDollars / 1000;
			CString strThousands = HundredsTensOnes(dwThousands);
			strTemp.LoadString(IDS_THOUSAND);   // "Thousand"
			str += strThousands;
			str += ' ';
			str += strTemp;
			str += ' ';
			dwDollars -= (dwThousands * 1000);
		}
		if (dwDollars > 0)
		{
			CString strHundredsTensOnes = HundredsTensOnes(dwDollars);
			str += strHundredsTensOnes;
			str += ' ';
		}
	}
	TCHAR szCents[10];
	CString strCents(_itot(wCents, szCents, 10));
	strTemp.LoadString(IDS_AND);    // "and"
	str += strTemp;
	str += ' ';
	str += strCents;
	strTemp.LoadString(IDS_HUNDRETHS_DOLLARS);  // "/100ths Dollars"
	str += strTemp;
	return str;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

CString HundredsTensOnes(DWORD dwHundredsTensOnes)
{
	CString str, strTemp;
	if (dwHundredsTensOnes >= 100)
	{
		DWORD dwHundreds = dwHundredsTensOnes / 100;
		CString strHundreds;
		strHundreds.LoadString(IDS_ONES_0 + dwHundreds);
		strTemp.LoadString(IDS_HUNDRED);
		str = strHundreds;
		str += ' ';
		str += strTemp;
		str += ' ';
		dwHundredsTensOnes -= (dwHundreds * 100);
	}
	if (dwHundredsTensOnes > 0)
	{
		CString strTensOnes = TensOnes(dwHundredsTensOnes);
		str += strTensOnes;
	}
	return str;
}


CString TensOnes(DWORD dwTensOnes)
{
	CString str, strTemp;
	if (dwTensOnes > 19)
	{
		DWORD dwTens = dwTensOnes / 10;
		strTemp.LoadString(IDS_TENS_0 + dwTens);
		str += strTemp;
		dwTensOnes -= (dwTens * 10);
		if (dwTensOnes > 0)
		{
			CString strOnes;
			strOnes.LoadString(IDS_ONES_0 + dwTensOnes);
			str += '-';
			str += strOnes;
		}
	}
	else
	if (dwTensOnes >= 10)
	{
		CString strTeens;
		strTeens.LoadString(IDS_TEENS_10 + dwTensOnes - 10);
		str += strTeens;
	}
	else
	{
		CString strOnes;
		strOnes.LoadString(IDS_ONES_0 + dwTensOnes);
		str += strOnes;
	}
	return str;
}
