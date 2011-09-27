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

#ifdef AFX_PRINT_SEG
#pragma code_seg(AFX_PRINT_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Helper functions

static long PASCAL NEAR MultMultDivDiv(int factor, int num1, int num2,
		int den1, int den2);

typedef unsigned char uchar;

/////////////////////////////////////////////////////////////////////////////
// Print Preview DC (CPreviewDC)

IMPLEMENT_DYNAMIC(CPreviewDC, CDC)

CPreviewDC::CPreviewDC()
{
	// Initial scale factor and top-left offset
	m_nScaleNum = m_nScaleDen = 1;
	m_sizeTopLeft.cx = m_sizeTopLeft.cy = 8;
	m_hFont = m_hPrinterFont = NULL;
}

void CPreviewDC::SetOutputDC(HDC hDC)
{
	ASSERT(hDC != NULL);
	m_nSaveDCIndex = ::SaveDC(hDC); // restore in ReleaseOutputDC()
	CDC::SetOutputDC(hDC);

	if (m_hAttribDC != NULL)
	{
		MirrorMappingMode(FALSE);

		if (m_hFont)
			::SelectObject(m_hDC, m_hFont);
		else
			MirrorFont();
		MirrorAttributes();
	}
}

void CPreviewDC::ReleaseOutputDC()
{
	ASSERT(m_hDC != NULL);
	::RestoreDC(m_hDC, m_nSaveDCIndex); // Saved in SetOutputDC()
	CDC::ReleaseOutputDC();
}

void CPreviewDC::SetAttribDC(HDC hDC)
{
	ASSERT(hDC != NULL);
	CDC::SetAttribDC(hDC);

	MirrorMappingMode(TRUE);
	MirrorFont();
	MirrorAttributes();
}

CPreviewDC::~CPreviewDC()
{
	ASSERT(m_hDC == NULL);      // Should not have a screen DC at this time
	if (m_hFont)                // Get rid of Font now
	{
		::DeleteObject(m_hFont);
		m_hFont = NULL;
	}
}

void CPreviewDC::SetScaleRatio(int nNumerator, int nDenominator)
{
	m_nScaleNum = nNumerator;
	m_nScaleDen = nDenominator;
	if (m_hAttribDC != NULL)
	{
		MirrorMappingMode(TRUE);
		MirrorFont();
	}
}

// Implementation support
#ifdef _DEBUG
void CPreviewDC::AssertValid() const
{
	CDC::AssertValid();
}


void CPreviewDC::Dump(CDumpContext& dc) const
{
	CDC::Dump(dc);
	AFX_DUMP1(dc, " Scale Factor: ", m_nScaleNum);
	AFX_DUMP1(dc, "/", m_nScaleDen);
	AFX_DUMP0(dc, "\n");
}
#endif

int CPreviewDC::SaveDC()
{
	ASSERT(m_hAttribDC != NULL);
	int nAttribIndex = ::SaveDC(m_hAttribDC);
	if (m_hDC != NULL)
	{
		// remove font from object
		::SelectObject(m_hDC, GetStockObject(SYSTEM_FONT));
		m_nSaveDCDelta = ::SaveDC(m_hDC) - nAttribIndex;
		// Select font back in after save
		::SelectObject(m_hDC, m_hFont);
	}
	else
	{
		m_nSaveDCDelta = 0x7fff;        // impossibly high value
	}
	return nAttribIndex;
}


BOOL CPreviewDC::RestoreDC(int nSavedDC)
{
	ASSERT(m_hAttribDC != NULL);
	BOOL bSuccess = ::RestoreDC(m_hAttribDC, nSavedDC);
	if (bSuccess)
	{
		if (m_nSaveDCDelta != 0x7fff)
		{
			ASSERT(m_hDC != NULL);      // removed Output DC after save

			if (nSavedDC != -1)
				nSavedDC += m_nSaveDCDelta;
			bSuccess = ::RestoreDC(m_hDC, nSavedDC);
			MirrorFont();               // mirror the font
		}
		else
		{
			ASSERT(m_hDC == NULL);      // Added the Output DC after save
		}
	}
	return bSuccess;
}

void CPreviewDC::MirrorAttributes()
{
	ASSERT(m_hAttribDC != NULL);

	if (m_hDC != NULL)
	{
		// extract and re-set Pen and Brush
		HGDIOBJ hTemp = ::SelectObject(m_hAttribDC, ::GetStockObject(BLACK_PEN));
		::SelectObject(m_hAttribDC, hTemp);
		::SelectObject(m_hDC, hTemp);
		hTemp = ::SelectObject(m_hAttribDC, ::GetStockObject(BLACK_BRUSH));
		::SelectObject(m_hAttribDC, hTemp);
		::SelectObject(m_hDC, hTemp);

		SetROP2(GetROP2());
		SetBkMode(GetBkMode());
		SetTextAlign(GetTextAlign());
		SetPolyFillMode(GetPolyFillMode());
		SetStretchBltMode(GetStretchBltMode());
		SetTextColor(GetNearestColor(GetTextColor()));
		SetBkColor(GetNearestColor(GetBkColor()));
	}
}


CGdiObject* CPreviewDC::SelectStockObject(int nIndex)
{
	ASSERT(m_hAttribDC != NULL);

	HGDIOBJ hObj = ::GetStockObject(nIndex);

	switch (nIndex)
	{
	case ANSI_FIXED_FONT:
	case ANSI_VAR_FONT:
	case DEVICE_DEFAULT_FONT:
	case OEM_FIXED_FONT:
	case SYSTEM_FONT:
	case SYSTEM_FIXED_FONT:
		// Handle the stock fonts correctly
		{
			CGdiObject* pObject = CGdiObject::FromHandle(
							::SelectObject(m_hAttribDC, hObj));

			// Don't re-mirror screen font if this is the same font.
			if (m_hPrinterFont == (HFONT) hObj)
				return pObject;

			m_hPrinterFont = (HFONT) hObj;

			ASSERT(m_hPrinterFont != NULL); // Do not allow infinite recursion

			MirrorFont();
			return pObject;
		}

	default:
		if (m_hDC != NULL)
			::SelectObject(m_hDC, hObj);
		return CGdiObject::FromHandle(::SelectObject(m_hAttribDC, hObj));
	}
}

void CPreviewDC::MirrorFont()
{
	if (m_hAttribDC == NULL)
		return;         // Can't do anything without Attrib DC

	if (m_hPrinterFont == NULL)
	{
		SelectStockObject(DEVICE_DEFAULT_FONT); // will recurse
		return;
	}

	if (m_hDC == NULL)
		return;         // can't mirror font without a screen DC

	LOGFONT logFont;
	// Fill the logFont structure with the original info
	::GetObject(m_hPrinterFont, sizeof(LOGFONT), (LPSTR)&logFont);

	TEXTMETRIC tm;

	GetTextFace(LF_FACESIZE, (LPSTR)&logFont.lfFaceName[0]);
	GetTextMetrics(&tm);

	// Set real values based on the Printer's text metrics.

	if (tm.tmHeight < 0)
		logFont.lfHeight = tm.tmHeight;
	else
		logFont.lfHeight = -(tm.tmHeight - tm.tmInternalLeading);

	logFont.lfWidth = 0;
	logFont.lfWeight = tm.tmWeight;
	logFont.lfItalic = tm.tmItalic;
	logFont.lfUnderline = tm.tmUnderlined;
	logFont.lfStrikeOut = tm.tmStruckOut;
	logFont.lfCharSet = tm.tmCharSet;
	logFont.lfPitchAndFamily = tm.tmPitchAndFamily;

	HFONT hNewFont = ::CreateFontIndirect(&logFont);
	::SelectObject(m_hDC, hNewFont);

	::GetTextMetrics(m_hDC, &tm);

	// Is the displayed font too large?

	int cyDesired = -logFont.lfHeight;
	int cyActual;
	if (tm.tmHeight < 0)
		cyActual = -tm.tmHeight;
	else
		cyActual = tm.tmHeight - tm.tmInternalLeading;

	CSize sizeWinExt = ::GetWindowExt(m_hDC);
	CSize sizeVpExt = ::GetViewportExt(m_hDC);

	// Only interested in Extent Magnitudes, not direction
	if (sizeWinExt.cy < 0)
		sizeWinExt.cy = -sizeWinExt.cy;
	if (sizeVpExt.cy < 0)
		sizeVpExt.cy = -sizeVpExt.cy;

	// Convert to screen device coordinates to eliminate rounding
	// errors as a source of SmallFont aliasing

	cyDesired = MulDiv(cyDesired, sizeVpExt.cy, sizeWinExt.cy);
	cyActual = MulDiv(cyActual, sizeVpExt.cy, sizeWinExt.cy);

	ASSERT(cyDesired >= 0 && cyActual >= 0);

	if (cyDesired < cyActual)
	{
		logFont.lfFaceName[0] = 0;      // let the mapper find a good fit

		if ((logFont.lfPitchAndFamily & 0xf0) == FF_DECORATIVE)
			logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_DECORATIVE;
		else
			logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

		HFONT hTempFont = ::CreateFontIndirect(&logFont);
		::SelectObject(m_hDC, hTempFont);           // Select it in.
		::DeleteObject(hNewFont);
		hNewFont = hTempFont;
	}

	if (m_hFont)
		::DeleteObject(m_hFont);    // Delete the old logical font

	m_hFont = hNewFont;         // save the new one
}

CFont* CPreviewDC::SelectObject(CFont* pFont)
{
	if (pFont == NULL)
		return NULL;

	ASSERT(m_hAttribDC != NULL);
	ASSERT_VALID(pFont);

	CFont* pOldFont = (CFont*) CGdiObject::FromHandle(
				::SelectObject(m_hAttribDC, pFont->m_hObject));

	// If same as already selected, don't re-mirror screen font
	if (m_hPrinterFont != pFont->m_hObject)
	{
		m_hPrinterFont = (HFONT)pFont->m_hObject;
		MirrorFont();
	}

	return pOldFont;
}


// Drawing-Attribute Functions

COLORREF CPreviewDC::SetBkColor(COLORREF crColor)
{
	ASSERT(m_hAttribDC != NULL);
	if (m_hDC != NULL)
		::SetBkColor(m_hDC, ::GetNearestColor(m_hAttribDC, crColor));
	return ::SetBkColor(m_hAttribDC, crColor);
}


COLORREF CPreviewDC::SetTextColor(COLORREF crColor)
{
	ASSERT(m_hAttribDC != NULL);
	if (m_hDC != NULL)
		::SetTextColor(m_hDC, ::GetNearestColor(m_hAttribDC, crColor));
	return ::SetTextColor(m_hAttribDC, crColor);
}


int CPreviewDC::SetMapMode(int nMapMode)
{
	ASSERT(m_hAttribDC != NULL);
	int nModeOld = ::SetMapMode(m_hAttribDC, nMapMode);
	MirrorMappingMode(TRUE);
	return nModeOld;
}

CPoint CPreviewDC::SetViewportOrg(int x, int y)
{
	ASSERT(m_hAttribDC != NULL);
	CPoint ptOrgOld = ::SetViewportOrg(m_hAttribDC, x, y);
	MirrorViewportOrg();
	return ptOrgOld;
}

CPoint CPreviewDC::OffsetViewportOrg(int nWidth, int nHeight)
{
	ASSERT(m_hAttribDC != NULL);
	CPoint ptOrgOld = ::OffsetViewportOrg(m_hAttribDC, nWidth, nHeight);
	MirrorViewportOrg();
	return ptOrgOld;
}


CSize CPreviewDC::SetViewportExt(int x, int y)
{
	ASSERT(m_hAttribDC != NULL);
	CSize sizeExtOld = ::SetViewportExt(m_hAttribDC, x, y);
	MirrorMappingMode(TRUE);
	return sizeExtOld;
}

CSize CPreviewDC::ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom)
{
	ASSERT(m_hAttribDC != NULL);
	CSize sizeExtOld = ::ScaleViewportExt(m_hAttribDC, xNum, xDenom,
														yNum, yDenom);
	MirrorMappingMode(TRUE);
	return sizeExtOld;
}

CSize CPreviewDC::SetWindowExt(int x, int y)
{
	ASSERT(m_hAttribDC != NULL);
	CSize sizeExtOld = ::SetWindowExt(m_hAttribDC, x, y);
	MirrorMappingMode(TRUE);
	return sizeExtOld;
}


CSize CPreviewDC::ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom)
{
	ASSERT(m_hAttribDC != NULL);
	CSize sizeExtOld = ::ScaleWindowExt(m_hAttribDC, xNum, xDenom,
														yNum, yDenom);
	MirrorMappingMode(TRUE);
	return sizeExtOld;
}


// Text Functions
/////////////////////////////////////////////////////////////////////////////
// Private helpers for TextOut functions

static int ComputeNextTab(int x, UINT nTabStops, LPINT lpnTabStops,
										int nTabOrigin, int nTabWidth)
{
	x -= nTabOrigin;        // normalize position to tab origin
	for (UINT i = 0; i < nTabStops; i++, lpnTabStops++)
	{
		if (*lpnTabStops > x)
			return *lpnTabStops + nTabOrigin;
	}
	return (x / nTabWidth + 1) * nTabWidth + nTabOrigin;
}

//
// Compute a character delta table for correctly positioning the screen
// font characters where the printer characters will appear on the page
//
CSize CPreviewDC::ComputeDeltas(int& x, LPCSTR lpszString, UINT &nCount,
	BOOL bTabbed, UINT nTabStops, LPINT lpnTabStops, int nTabOrigin,
	char* pszOutputString, int* pnDxWidths, int& nRightFixup)
{
	ASSERT_VALID(this);

	TEXTMETRIC tmAttrib;
	TEXTMETRIC tmScreen;
	::GetTextMetrics(m_hAttribDC, &tmAttrib);
	::GetTextMetrics(m_hDC, &tmScreen);

	CSize sizeExtent = ::GetTextExtent(m_hAttribDC, "A", 1);

	CPoint ptCurrent;
	UINT nAlignment = ::GetTextAlign(m_hAttribDC);
	BOOL bUpdateCP = (nAlignment & TA_UPDATECP) != 0;
	if (bUpdateCP)
	{
		ptCurrent = ::GetCurrentPosition(m_hDC);
		x = ptCurrent.x;
	}

	LPCSTR lpszCurChar = lpszString;
	LPCSTR lpszStartRun = lpszString;
	int* pnCurDelta = pnDxWidths;
	int nStartRunPos = x;
	int nCurrentPos = x;
	int nStartOffset = 0;

	int nTabWidth = 0;
	if (bTabbed)
	{
		if (nTabStops == 1)
		{
			nTabWidth = lpnTabStops[0];
		}
		else
		{
			// Get default size of a tab
			nTabWidth = LOWORD(::GetTabbedTextExtent(m_hAttribDC,
													"\t", 1, 0, NULL));
		}
	}

	for (UINT i = 0; i < nCount; i++)
	{
		BOOL bSpace = ((uchar)*lpszCurChar == (uchar)tmAttrib.tmBreakChar);
		if (bSpace || (bTabbed && *lpszCurChar == '\t'))
		{
			// bSpace will be either TRUE (==1) or FALSE (==0).  For spaces
			// we want the space included in the GetTextExtent, for tabs we
			// do not want the tab included
			int nRunLength = (int)(lpszCurChar - lpszStartRun) + bSpace;

			CSize sizeExtent = ::GetTextExtent(m_hAttribDC, lpszStartRun,
				nRunLength);
			int nNewPos = nStartRunPos + sizeExtent.cx
				- tmAttrib.tmOverhang;

			// now, if this is a Tab (!bSpace), compute the next tab stop
			if (!bSpace)
			{
				nNewPos = ComputeNextTab(nNewPos, nTabStops, lpnTabStops,
								nTabOrigin, nTabWidth);
			}

			// Add this width to previous width
			if (pnCurDelta == pnDxWidths)
				nStartOffset += nNewPos - nCurrentPos;
			else
				*(pnCurDelta-1) += nNewPos - nCurrentPos;

			nCurrentPos = nNewPos;

			nStartRunPos = nCurrentPos;     // set start of run
			// *lpszCurChar must be SBC: tmBreakChar or '\t'
			lpszStartRun = lpszCurChar + 1;
		}
		else
		{
			// For the non-tabbed or non-tab-character case
			int cxScreen;
			if (_AfxIsDBCSLeadByte(*lpszCurChar))
			{
				cxScreen = tmScreen.tmAveCharWidth;
				*pnCurDelta = tmAttrib.tmAveCharWidth;
			}
			else
			{
				::GetCharWidth(m_hDC, (uchar)*lpszCurChar,
					(uchar)*lpszCurChar, &cxScreen);
				if (!::GetCharWidth(m_hAttribDC, (uchar)*lpszCurChar,
					(uchar)*lpszCurChar, pnCurDelta))
				{
					// If printer driver fails the above call, use the average width
					*pnCurDelta = tmAttrib.tmAveCharWidth;
				}
			}
			*pnCurDelta -= tmAttrib.tmOverhang;
			cxScreen -= tmScreen.tmOverhang;
			nCurrentPos += *pnCurDelta;     // update current position

			// Center character in allotted space
			if (pnCurDelta != pnDxWidths)
			{
				int diff = (*pnCurDelta - cxScreen) / 2;
				*pnCurDelta -= diff;
				*(pnCurDelta - 1) += diff;
			}
			*pszOutputString++ = *lpszCurChar;
			if (_AfxIsDBCSLeadByte(*lpszCurChar))
			{
				*pszOutputString++ = *(lpszCurChar+1);  // Copy trailing byte
				*(pnCurDelta + 1) = *pnCurDelta;        // DBC is double wide
				nCurrentPos += *pnCurDelta;
				pnCurDelta++;
				i++;
			}
			pnCurDelta++;
		}
		lpszCurChar = AnsiNext(lpszCurChar);
	}

	nAlignment &= TA_CENTER|TA_RIGHT;
	sizeExtent.cx = nCurrentPos - x;
	nRightFixup = 0;

	if (nAlignment == TA_LEFT)
		x += nStartOffset;      // Full left side adjustment
	else if (nAlignment == TA_CENTER)
		x += nStartOffset/2;    // Adjust Center by 1/2 left side adjustment
	else if (nAlignment == TA_RIGHT)
		nRightFixup = nStartOffset; // Right adjust needed later if TA_UPDATECP

	if (bUpdateCP)
		::MoveTo(m_hDC, x, ptCurrent.y);

	nCount = (UINT)(pnCurDelta - pnDxWidths);   // number of characters output
	return sizeExtent;
}


BOOL CPreviewDC::TextOut(int x, int y, LPCSTR lpszString, int nCount)
{
	return ExtTextOut(x, y, 0, NULL, lpszString, nCount, NULL);
}

BOOL CPreviewDC::ExtTextOut(int x, int y, UINT nOptions, LPRECT lpRect,
			LPCSTR lpszString, UINT nCount, LPINT lpDxWidths)
{
	ASSERT(m_hDC != NULL);
	ASSERT(m_hAttribDC != NULL);
	ASSERT(lpszString != NULL);
	ASSERT(lpDxWidths == NULL ||
			AfxIsValidAddress(lpDxWidths, sizeof(int) * nCount, FALSE));
	ASSERT(AfxIsValidAddress(lpszString, nCount));

	int* pDeltas = NULL;
	char* pOutputString = NULL;
	int nRightFixup;

	if (lpDxWidths == NULL)
	{
		if (nCount == 0)    // Do nothing
			return TRUE;

		TRY
		{
			pDeltas = new int[nCount];
			pOutputString = new char[nCount];
		}
		CATCH_ALL(e)
		{
			delete [] pDeltas;  // in case it was allocated
			return FALSE;   // Could not allocate buffer, cannot display
		}
		END_CATCH_ALL

		ComputeDeltas(x, (LPSTR)lpszString, nCount, FALSE, 0, NULL, 0,
										pOutputString, pDeltas, nRightFixup);

		lpDxWidths = pDeltas;
		lpszString = pOutputString;
	}

	BOOL bSuccess = ::ExtTextOut(m_hDC, x, y, nOptions, lpRect, lpszString,
														nCount, lpDxWidths);
	if (bSuccess && (GetTextAlign() & TA_UPDATECP))
	{
		CPoint pt = ::GetCurrentPosition(m_hDC);
		MoveTo(pt.x - nRightFixup, pt.y);
	}
	delete [] pDeltas;
	delete [] pOutputString;

	return bSuccess;
}

CSize CPreviewDC::TabbedTextOut(int x, int y, LPCSTR lpszString, int nCount,
			int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin)
{
	ASSERT(m_hAttribDC != NULL);
	ASSERT(m_hDC != NULL);
	ASSERT(lpszString != NULL);
	ASSERT(AfxIsValidAddress(lpszString, nCount));
	ASSERT(lpnTabStopPositions == NULL ||
			AfxIsValidAddress(lpnTabStopPositions, sizeof(int) * nTabPositions,
				FALSE));

	if (nCount <= 0)
		return 0;         // nCount is zero, there is nothing to print

	int* pDeltas = NULL;
	char* pOutputString = NULL;
	int nRightFixup;

	TRY
	{
		pDeltas = new int[nCount];
		pOutputString = new char[nCount];
	}
	CATCH_ALL(e)
	{
		delete [] pDeltas;
		return 0;           // signify error
	}
	END_CATCH_ALL

	UINT uCount = nCount;
	CSize sizeFinalExtent = ComputeDeltas(x, lpszString, uCount, TRUE,
							nTabPositions, lpnTabStopPositions, nTabOrigin,
							pOutputString, pDeltas, nRightFixup);

	BOOL bSuccess = ExtTextOut(x, y, 0, NULL, pOutputString, uCount, pDeltas);

	delete [] pDeltas;
	delete [] pOutputString;

	if (bSuccess && (GetTextAlign() & TA_UPDATECP))
	{
		CPoint pt = ::GetCurrentPosition(m_hDC);
		MoveTo(pt.x - nRightFixup, pt.y);
	}

	return sizeFinalExtent;
}

// This one is too complicated to do character-by-character output positioning
// All we really need to do here is mirror the current position
int CPreviewDC::DrawText(LPCSTR lpszString, int nCount, LPRECT lpRect,
					UINT nFormat)
{
	ASSERT(m_hAttribDC != NULL);
	ASSERT(m_hDC != NULL);
	ASSERT(lpszString != NULL);
	ASSERT(lpRect != NULL);
	ASSERT(AfxIsValidAddress(lpRect, sizeof(RECT)));
	ASSERT(nCount == -1 ?
		AfxIsValidString(lpszString) :
		AfxIsValidAddress(lpszString, nCount, FALSE));

	int retVal = ::DrawText(m_hDC, lpszString, nCount, lpRect, nFormat);

	CPoint pos = ::GetCurrentPosition(m_hDC);
	::MoveTo(m_hAttribDC, pos.x, pos.y);
	return retVal;
}

BOOL CPreviewDC::GrayString(CBrush*,
				BOOL (CALLBACK EXPORT*)(HDC, LPARAM, int),
					LPARAM lpData, int nCount, int x, int y, int, int)
{
	TRACE0("TextOut() substituted for GrayString() in Print Preview.\n");
	return TextOut(x, y, (LPCSTR)lpData, nCount);
}

int CPreviewDC::Escape(int nEscape, int nCount, LPCSTR lpszInData, void FAR* lpOutData)
{
	// The tact here is to NOT allow any of the document control escapes
	// to be passed through.  Elimination of StartDoc and EndDoc should
	// eliminate anything actually going to the printer.  Also anything
	// that actually draws something will be filtered.
	//

	ASSERT(m_hAttribDC != NULL);

	switch (nEscape)
	{
	case NEXTBAND:
	case SETCOLORTABLE:
	case GETCOLORTABLE:
	case FLUSHOUTPUT:
	case DRAFTMODE:
	case QUERYESCSUPPORT:
	case GETPHYSPAGESIZE:
	case GETPRINTINGOFFSET:
	case GETSCALINGFACTOR:
	case GETPENWIDTH:
	case SETCOPYCOUNT:
	case SELECTPAPERSOURCE:
	case GETTECHNOLOGY:
	case SETLINECAP:
	case SETLINEJOIN:
	case SETMITERLIMIT:
	case BANDINFO:
	case GETVECTORPENSIZE:
	case GETVECTORBRUSHSIZE:
	case ENABLEDUPLEX:
	case GETSETPAPERBINS:
	case GETSETPRINTORIENT:
	case ENUMPAPERBINS:
	case SETDIBSCALING:
	case ENUMPAPERMETRICS:
	case GETSETPAPERMETRICS:
	case GETEXTENDEDTEXTMETRICS:
	case GETEXTENTTABLE:
	case GETPAIRKERNTABLE:
	case GETTRACKKERNTABLE:
	case ENABLERELATIVEWIDTHS:
	case ENABLEPAIRKERNING:
	case SETKERNTRACK:
	case SETALLJUSTVALUES:
	case SETCHARSET:
	case SET_BACKGROUND_COLOR:
	case SET_SCREEN_ANGLE:
	case SET_SPREAD:
		return ::Escape(m_hAttribDC, nEscape, nCount, lpszInData, lpOutData);
	default:
		return 0;
	}
}


void CPreviewDC::MirrorMappingMode(BOOL bCompute)
{
	ASSERT(m_hAttribDC != NULL);
	if (bCompute)
	{
		//
		// The following formula is used to compute the screen's viewport extent
		// From the printer and screen information and the Printer's Viewport
		// Extents.  (Note:  This formula is used twice, once for horizontal
		// and once for vertical)
		//
		// It is assumed that the Window Extents are maintained as equal.
		//
		//                  m * LogPixPerInch(Screen) * VpExt(Printer)
		// VpExt(Screen) = -------------------------------------------------
		//                          n * LogPixPerInch(Printer)
		//
		// Where m/n is the scaling factor.  (m/n > 1 is expansion)
		//

		m_sizeVpExt = ::GetViewportExt(m_hAttribDC);
		m_sizeWinExt = ::GetWindowExt(m_hAttribDC);

		while (m_sizeWinExt.cx > -0x4000 && m_sizeWinExt.cx < 0x4000 &&
			   m_sizeVpExt.cx  > -0x4000 && m_sizeVpExt.cx  < 0x4000)
		{
			m_sizeWinExt.cx <<= 1;
			m_sizeVpExt.cx  <<= 1;
		}

		while (m_sizeWinExt.cy > -0x4000 && m_sizeWinExt.cy < 0x4000 &&
			   m_sizeVpExt.cy  > -0x4000 && m_sizeVpExt.cy  < 0x4000)
		{
			m_sizeWinExt.cy <<= 1;
			m_sizeVpExt.cy  <<= 1;
		}

		long lTempExt = MultMultDivDiv(m_sizeVpExt.cx,
			m_nScaleNum, afxData.cxPixelsPerInch,
			m_nScaleDen, ::GetDeviceCaps(m_hAttribDC, LOGPIXELSX));

		while (lTempExt < -0x8000L || lTempExt > 0x7fffL)
		{
			lTempExt >>= 1;
			m_sizeWinExt.cx >>= 1;
		}

		ASSERT(m_sizeWinExt.cx != 0);
		m_sizeVpExt.cx = (int)lTempExt;

		lTempExt = MultMultDivDiv(m_sizeVpExt.cy,
			m_nScaleNum, afxData.cyPixelsPerInch,
			m_nScaleDen, ::GetDeviceCaps(m_hAttribDC, LOGPIXELSY));

		while (lTempExt < -0x8000L || lTempExt > 0x7fffL)
		{
			lTempExt >>= 1;
			m_sizeWinExt.cy >>= 1;
		}

		ASSERT(m_sizeWinExt.cy != 0);
		m_sizeVpExt.cy = (int)lTempExt;
	}

	if (m_hDC != NULL)
	{
		::SetMapMode(m_hDC, MM_ANISOTROPIC);
		::SetWindowExt(m_hDC, m_sizeWinExt.cx, m_sizeWinExt.cy);
		::SetViewportExt(m_hDC, m_sizeVpExt.cx, m_sizeVpExt.cy);

		// Now that the Logical Units are syncronized, we can set the Viewport Org
		MirrorViewportOrg();
	}
}


void CPreviewDC::MirrorViewportOrg()
{
	if (m_hAttribDC == NULL || m_hDC == NULL)
		return;

	CPoint ptVpOrg = ::GetViewportOrg(m_hAttribDC);
	PrinterDPtoScreenDP(&ptVpOrg);
	ptVpOrg += m_sizeTopLeft;
	::SetViewportOrg(m_hDC, ptVpOrg.x, ptVpOrg.y);

	CPoint ptWinOrg = ::GetWindowOrg(m_hAttribDC);
	::SetWindowOrg(m_hDC, ptWinOrg.x, ptWinOrg.y);
}

void CPreviewDC::SetTopLeftOffset(CSize sizeTopLeft)
{
	ASSERT(m_hAttribDC != NULL);
	m_sizeTopLeft = sizeTopLeft;
	MirrorViewportOrg();
}

void CPreviewDC::ClipToPage()
{
	ASSERT(m_hAttribDC != NULL);
	ASSERT(m_hDC != NULL);
	// Create a rect in Screen Device coordinates that is one pixel larger
	// on all sides than the actual page.  This is to hide the fact that
	// the printer to screen mapping mode is approximate and may result
	// in rounding error.

	CPoint pt(::GetDeviceCaps(m_hAttribDC, HORZRES),
				::GetDeviceCaps(m_hAttribDC, VERTRES));
	PrinterDPtoScreenDP(&pt);

	// Set the screen dc to MM_TEXT and no WindowOrg for the interesection

	::SetMapMode(m_hDC, MM_TEXT);
	::SetWindowOrg(m_hDC, 0, 0);
	::SetViewportOrg(m_hDC, m_sizeTopLeft.cx, m_sizeTopLeft.cy);
	::IntersectClipRect(m_hDC, -1, -1, pt.x + 2, pt.y + 2);

	// Resynchronize the mapping mode
	MirrorMappingMode(FALSE);
}

// These conversion functions can be used without an attached screen DC

void CPreviewDC::PrinterDPtoScreenDP(LPPOINT lpPoint) const
{
	ASSERT(m_hAttribDC != NULL);

	CSize sizePrinterVpExt = ::GetViewportExt(m_hAttribDC);
	CSize sizePrinterWinExt = ::GetWindowExt(m_hAttribDC);

	long xScreen = MultMultDivDiv(lpPoint->x,
		sizePrinterWinExt.cx, m_sizeVpExt.cx,
		sizePrinterVpExt.cx, m_sizeWinExt.cx);

	if (xScreen < -0x8000L)
		lpPoint->x = (int)-0x8000L;
	else if (xScreen > 0x7fff)
		lpPoint->x = 0x7fff;
	else
		lpPoint->x = (int)xScreen;

	long yScreen = MultMultDivDiv(lpPoint->y,
		sizePrinterWinExt.cy, m_sizeVpExt.cy,
		sizePrinterVpExt.cy, m_sizeWinExt.cy);

	if (yScreen < -0x8000L)
		lpPoint->y = (int)-0x8000L;
	else if (yScreen > 0x7fff)
		lpPoint->y = 0x7fff;
	else
		lpPoint->y = (int)yScreen;
}

static long PASCAL NEAR MultMultDivDiv(int factor,
	int num1, int num2, int den1, int den2)
{

	long numerator = (long)num1 * (long)num2;   // Guaranteed no overflow
	long denominator = (long)den1 * (long)den2; // Guaranteed no overflow

	long temp = numerator < 0 ? -numerator : numerator;

	for (int nBitsInNum = 0; temp != 0; nBitsInNum++)
		temp >>= 1;

	temp = factor < 0 ? -factor : factor;
	for (int nBitsInFactor = 0; temp != 0; nBitsInFactor++)
		temp >>= 1;

	nBitsInNum += nBitsInFactor;

	//
	// normalizing the denominator to positive results in an easier
	// determination of whether there is overflow
	//
	if (denominator < 0)
	{
		denominator = -denominator;
		numerator = -numerator;
	}

	// Get the product of factor * numerator representable in a long
	// while distributing loss of presision across all three numerator terms
	// Adjust denominator as well
	//
	while (nBitsInNum-- > 31)
	{
		numerator >>= 1;
		denominator >>= 1;
		if (nBitsInNum-- <= 31)
			break;
		numerator >>= 1;
		denominator >>= 1;
		if (nBitsInNum-- <= 31)
			break;
		factor >>= 1;
		denominator >>= 1;
	}

	numerator *= factor;

	if (denominator == 0)
		return numerator < 0 ? 0x80000000L : 0x07ffffffL;

	return (numerator + denominator/2) / denominator;
}
