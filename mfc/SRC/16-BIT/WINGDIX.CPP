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
#include "winhand_.h"

#ifdef AFX_CORE4_SEG
#pragma code_seg(AFX_CORE4_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// More coordinate transforms (in separate file to avoid transitive refs)

#define HIMETRIC_INCH   2540    // HIMETRIC units per inch

void CDC::DPtoHIMETRIC(LPSIZE lpSize) const
{
	ASSERT(AfxIsValidAddress(lpSize, sizeof(SIZE)));

	int nMapMode;
	if (this != NULL && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
		nMapMode != MM_TEXT)
	{
		// when using a constrained map mode, map against physical inch
		((CDC*)this)->SetMapMode(MM_HIMETRIC);
		DPtoLP(lpSize);
		((CDC*)this)->SetMapMode(nMapMode);
	}
	else
	{
		// map against logical inch for non-constrained mapping modes
		int cxPerInch, cyPerInch;
		if (this != NULL)
		{
			ASSERT_VALID(this);
			ASSERT(m_hDC != NULL);  // no HDC attached or created?
			cxPerInch = GetDeviceCaps(LOGPIXELSX);
			cyPerInch = GetDeviceCaps(LOGPIXELSY);
		}
		else
		{
			cxPerInch = afxData.cxPixelsPerInch;
			cyPerInch = afxData.cyPixelsPerInch;
		}
		ASSERT(cxPerInch != 0 && cyPerInch != 0);
		lpSize->cx = MulDiv(lpSize->cx, HIMETRIC_INCH, cxPerInch);
		lpSize->cy = MulDiv(lpSize->cy, HIMETRIC_INCH, cyPerInch);
	}
}

void CDC::HIMETRICtoDP(LPSIZE lpSize) const
{
	ASSERT(AfxIsValidAddress(lpSize, sizeof(SIZE)));

	int nMapMode;
	if (this != NULL && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
		nMapMode != MM_TEXT)
	{
		// when using a constrained map mode, map against physical inch
		((CDC*)this)->SetMapMode(MM_HIMETRIC);
		LPtoDP(lpSize);
		((CDC*)this)->SetMapMode(nMapMode);
	}
	else
	{
		// map against logical inch for non-constrained mapping modes
		int cxPerInch, cyPerInch;
		if (this != NULL)
		{
			ASSERT_VALID(this);
			ASSERT(m_hDC != NULL);  // no HDC attached or created?
			cxPerInch = GetDeviceCaps(LOGPIXELSX);
			cyPerInch = GetDeviceCaps(LOGPIXELSY);
		}
		else
		{
			cxPerInch = afxData.cxPixelsPerInch;
			cyPerInch = afxData.cyPixelsPerInch;
		}
		ASSERT(cxPerInch != 0 && cyPerInch != 0);
		lpSize->cx = MulDiv(lpSize->cx, cxPerInch, HIMETRIC_INCH);
		lpSize->cy = MulDiv(lpSize->cy, cyPerInch, HIMETRIC_INCH);
	}
}

void CDC::LPtoHIMETRIC(LPSIZE lpSize) const
{
	ASSERT(AfxIsValidAddress(lpSize, sizeof(SIZE)));

	LPtoDP(lpSize);
	DPtoHIMETRIC(lpSize);
}

void CDC::HIMETRICtoLP(LPSIZE lpSize) const
{
	ASSERT(AfxIsValidAddress(lpSize, sizeof(SIZE)));

	HIMETRICtoDP(lpSize);
	DPtoLP(lpSize);
}

/////////////////////////////////////////////////////////////////////////////
// out-of-line CRect, CSize, etc. helpers

void CRect::NormalizeRect()
{
	int nTemp;
	if (left > right)
	{
		nTemp = left;
		left = right;
		right = nTemp;
	}
	if (top > bottom)
	{
		nTemp = top;
		top = bottom;
		bottom = nTemp;
	}
}

/////////////////////////////////////////////////////////////////////////////
