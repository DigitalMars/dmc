// dollcent.cpp : implementation of DDX_DollarsCents
//

#include "stdafx.h"
#include "chkbook.h"
#include <stdio.h>
#include <stdlib.h>

void SetDollarsCents(HWND hWnd, DWORD dwCents);
CString HundredsTensOnes(DWORD dwHundredsTensOnes);
CString TensOnes(DWORD dwTensOnes);


static char* BASED_CODE szTeens[10] =
{
	"Ten",
	"Eleven",
	"Twelve",
	"Thirteen",
	"Fourteen",
	"Fifteen",
	"Sixteen",
	"Seventeen",
	"Eighteen",
	"Nineteen"
};

static char* BASED_CODE szTens[10] =
{
	"?",
	"Ten",
	"Twenty",
	"Thirty",
	"Fourty",
	"Fifty",
	"Sixty",
	"Seventy",
	"Eighty",
	"Ninety"
};

static char* BASED_CODE szOnes[10] =
{
	"Zero",
	"One",
	"Two",
	"Three",
	"Four",
	"Five",
	"Six",
	"Seven",
	"Eight",
	"Nine"
};


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
	char szWindowText[20];
	::GetWindowText(hWnd, szWindowText, 19);
	DWORD dwDollars;
	int nCents;
	char* pc;
	char* szDollars;
	char* szCents;

	// strip leading blanks
	for (szDollars = szWindowText;  *szDollars == ' ';  szDollars++)
	{
		if (*szDollars == 0)
		{
			dwCents = 0;
			return TRUE;
		}
	}

	// parse dollar amount, before optional decimal point
	for (pc = szDollars; (*pc != '.') && (*pc != ' ') && (*pc != 0); pc++)
	{
		if ((*pc < '0') || (*pc > '9'))
			return FALSE;
	}
	BOOL bDollarsOnly = (*pc == 0);
	*pc = 0;

	if (strlen(szDollars) > 8)
		return FALSE;
	if (strlen(szDollars) == 0)
	{
		dwDollars = 0L;
	}
	else
	{
		dwDollars = atol(szDollars);
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
		for (szCents = ++pc; (*pc != 0) && (*pc != ' '); pc++)
		{
			if ((*pc < '0') || (*pc > '9'))
				return FALSE;
		}
		if (*pc == ' ')
		{
			for (pc++ ; *pc != 0; pc++)
			{
				if (*pc != ' ')
					return FALSE;
			}
		}

		int nCentsStrLen = strlen(szCents);
		switch (nCentsStrLen)
		{
			case 0:
				nCents = 0;
				break;
			case 1:
				nCents = atoi(szCents) * 10;
				break;
			case 2:
				nCents = atoi(szCents);
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
		return "???";
	}
	char szWindowText[20];
	DWORD dwDollars = dwCents / 100;
	WORD wCents = (WORD)(dwCents - 100 * dwDollars);
	char szCents[6];
	sprintf(szCents, "%u", wCents+100);
	sprintf(szWindowText, "%lu.%s", dwDollars, szCents+1);
	return CString(szWindowText);
}


CString GetDollarsCentsText(DWORD dwCents)
{
	CString str;
	if (dwCents == INVALID_DOLLARS_CENTS)
	{
		str = "???";
		return str;
	}

	DWORD dwDollars = dwCents / 100;
	WORD wCents = (WORD)(dwCents - (dwDollars * 100));
	if (dwDollars == 0L)
	{
		str = "Zero ";
	}
	else
	{
		if (dwDollars >= 1000000)
		{
			DWORD dwMillions = dwDollars / 1000000;
			CString strMillions = HundredsTensOnes(dwMillions);
			str = strMillions + " Million ";
			dwDollars -= (dwMillions * 1000000);
		}
		if (dwDollars >= 1000)
		{
			DWORD dwThousands = dwDollars / 1000;
			CString strThousands = HundredsTensOnes(dwThousands);
			str += strThousands + " Thousand ";
			dwDollars -= (dwThousands * 1000);
		}
		if (dwDollars > 0)
		{
			CString strHundredsTensOnes = HundredsTensOnes(dwDollars);
			str += strHundredsTensOnes + " ";
		}
	}
	char szCents[10];
	CString strCents(_itoa(wCents, szCents, 10));
	str += "and " + strCents + "/100ths Dollars";
	return str;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

CString HundredsTensOnes(DWORD dwHundredsTensOnes)
{
	CString str;
	if (dwHundredsTensOnes >= 100)
	{
		DWORD dwHundreds = dwHundredsTensOnes / 100;
		CString strHundreds = szOnes[dwHundreds];
		str = strHundreds + " Hundred ";
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
	CString str;
	if (dwTensOnes > 19)
	{
		DWORD dwTens = dwTensOnes / 10;
		str += szTens[dwTens];
		dwTensOnes -= (dwTens * 10);
		if (dwTensOnes > 0)
		{
			CString strOnes = szOnes[dwTensOnes];
			str += "-" + strOnes;
		}
	}
	else
	if (dwTensOnes >= 10)
	{
		CString strTeens = szTeens[dwTensOnes - 10];
		str += strTeens;
	}
	else
	{
		CString strOnes = szOnes[dwTensOnes];
		str += strOnes;
	}
	return str;
}
