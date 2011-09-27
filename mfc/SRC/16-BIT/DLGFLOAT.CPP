// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.

// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include <float.h>              // floating point precision

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Extra data validation procs for float/double support
//  see "dlgdata.cpp" for non-floating point support
/////////////////////////////////////////////////////////////////////////////

static BOOL PASCAL NEAR _AfxSimpleFloatParse(const char* pszText, double& d)
{
	ASSERT(pszText != NULL);
	while (*pszText == ' ' || *pszText == '\t')
		pszText++;

	ASSERT(!_AfxIsDBCSLeadByte(*pszText));
	char chFirst = pszText[0];
	d = strtod(pszText, (char**)&pszText);
	if (d == 0.0 && chFirst != '0')
		return FALSE;   // could not convert
	while (*pszText == ' ' || *pszText == '\t')
		pszText++;
	ASSERT(!_AfxIsDBCSLeadByte(*pszText));

	if (*pszText != '\0')
		return FALSE;   // not terminated properly

	return TRUE;
}


void PASCAL _AfxTextFloatFormat(CDataExchange* pDX, int nIDC,
	void* pData, double value, int nSizeGcvt)
{
	ASSERT(pData != NULL);

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	char szT[64];
	if (pDX->m_bSaveAndValidate)
	{
		::GetWindowText(hWndCtrl, szT, sizeof(szT));
		double d;
		if (!_AfxSimpleFloatParse(szT, d))
		{
			AfxMessageBox(AFX_IDP_PARSE_REAL);
			pDX->Fail();            // throws exception
		}
		if (nSizeGcvt == FLT_DIG)
			*((float*)pData) = (float)d;
		else
			*((double*)pData) = d;
	}
	else
	{
		_gcvt(value, nSizeGcvt, szT);
		_AfxSmartSetWindowText(hWndCtrl, szT);
	}
}


void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, float& value)
{
	_AfxTextFloatFormat(pDX, nIDC, &value, value, FLT_DIG);
}

void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, double& value)
{
	_AfxTextFloatFormat(pDX, nIDC, &value, value, DBL_DIG);
}

/////////////////////////////////////////////////////////////////////////////
// Validation procs

static void NEAR PASCAL FailMinMaxReal(CDataExchange* pDX,
	 double minVal, double maxVal, int precision, UINT nIDPrompt)
	// error string must have '%1' and '%2' in it
{
	if (!pDX->m_bSaveAndValidate)
	{
		TRACE0("Warning: initial dialog data is out of range\n");
		return;         // don't stop now
	}
	char szMin[32], szMax[32];
	CString prompt;
	AfxFormatString2(prompt, nIDPrompt,
		_gcvt(minVal, precision, szMin),
		_gcvt(maxVal, precision, szMax));
	AfxMessageBox(prompt, MB_ICONEXCLAMATION, nIDPrompt);
	prompt.Empty(); // exception prep
	pDX->Fail();
}

void AFXAPI DDV_MinMaxFloat(CDataExchange* pDX, float const& value, float minVal, float maxVal)
{
	ASSERT(minVal <= maxVal);
	if (value < minVal || value > maxVal)
		FailMinMaxReal(pDX, (double)minVal, (double)maxVal, FLT_DIG,
			AFX_IDP_PARSE_REAL_RANGE);
}

void AFXAPI DDV_MinMaxDouble(CDataExchange* pDX, double const& value, double minVal, double maxVal)
{
	ASSERT(minVal <= maxVal);
	if (value < minVal || value > maxVal)
		FailMinMaxReal(pDX, (double)minVal, (double)maxVal, DBL_DIG,
			AFX_IDP_PARSE_REAL_RANGE);
}

/////////////////////////////////////////////////////////////////////////////
