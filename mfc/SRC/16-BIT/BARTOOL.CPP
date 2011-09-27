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
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////

// globals for fast drawing (shared globals)
static HDC NEAR hDCGlyphs = NULL;
static HDC NEAR hDCMono = NULL;
static HBRUSH NEAR hbrDither = NULL;

/////////////////////////////////////////////////////////////////////////////
// Init / Term

static HBITMAP PASCAL CreateDitherBitmap();

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

static void PASCAL FreeToolBarDraw()
{
	_AfxExitDelete(hDCMono);
	hDCMono = NULL;

	_AfxExitDelete(hDCGlyphs);
	hDCGlyphs = NULL;

	_AfxExitDelete(hbrDither);  // brushes
	hbrDither = NULL;
}

static void PASCAL InitToolBarDraw()
{
	hDCGlyphs = CreateCompatibleDC(NULL);

	// Mono DC and Bitmap for disabled image
	hDCMono = ::CreateCompatibleDC(NULL);

	HBITMAP hbmGray = ::CreateDitherBitmap();
	if (hbmGray != NULL)
	{
		ASSERT(hbrDither == NULL);
		hbrDither = ::CreatePatternBrush(hbmGray);
		::DeleteObject(hbmGray);
	}

	ASSERT(afxData.pfnFreeToolBar == NULL);
	afxData.pfnFreeToolBar = FreeToolBarDraw;
	if (hDCGlyphs == NULL || hDCMono == NULL || hbrDither == NULL)
		AfxThrowResourceException();
}

/////////////////////////////////////////////////////////////////////////////

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

static void NEAR PASCAL PatB(HDC hDC, int x, int y, int dx, int dy, COLORREF rgb)
{
	SetBkColor(hDC, rgb);
	CRect rect(x, y, x+dx, y+dy);
	ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
}

static HBITMAP PASCAL CreateDitherBitmap()
{
	struct  // BITMAPINFO with 16 colors
	{
		BITMAPINFOHEADER bmiHeader;
		RGBQUAD      bmiColors[16];
	} bmi;
	memset(&bmi, 0, sizeof(bmi));

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = 8;
	bmi.bmiHeader.biHeight = 8;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 1;
	bmi.bmiHeader.biCompression = BI_RGB;

	COLORREF clr = ::GetSysColor(COLOR_BTNFACE);
	bmi.bmiColors[0].rgbBlue = GetBValue(clr);
	bmi.bmiColors[0].rgbGreen = GetGValue(clr);
	bmi.bmiColors[0].rgbRed = GetRValue(clr);

	if (afxData.bWin31)
		clr = ::GetSysColor(COLOR_BTNHIGHLIGHT);
	else
		clr = RGB(255, 255, 255);
	bmi.bmiColors[1].rgbBlue = GetBValue(clr);
	bmi.bmiColors[1].rgbGreen = GetGValue(clr);
	bmi.bmiColors[1].rgbRed = GetRValue(clr);

	// initialize the brushes
	long patGray[8];
	for (int i = 0; i < 8; i++)
	   patGray[i] = (i & 1) ? 0xAAAA5555L : 0x5555AAAAL;

	HDC hDC = GetDC(NULL);
	HBITMAP hbm = CreateDIBitmap(hDC, &bmi.bmiHeader, CBM_INIT,
		(LPBYTE)patGray, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS);
	ReleaseDC(NULL, hDC);

	return hbm;
}


// create a mono bitmap mask:
void CToolBar::CreateMask(int iImage, CPoint ptOffset,
	 BOOL bHilite, BOOL bHiliteShadow)
{
	// initalize whole area with 0's
	PatBlt(hDCMono, 0, 0, m_sizeButton.cx-2, m_sizeButton.cy-2, WHITENESS);

	// create mask based on color bitmap
	// convert this to 1's
	SetBkColor(hDCGlyphs, afxData.clrBtnFace);
	BitBlt(hDCMono, ptOffset.x, ptOffset.y, m_sizeImage.cx, m_sizeImage.cy,
		hDCGlyphs, iImage * m_sizeImage.cx, 0, SRCCOPY);

	if (bHilite)
	{
		// convert this to 1's
		SetBkColor(hDCGlyphs, afxData.clrBtnHilite);
		// OR in the new 1's
		BitBlt(hDCMono, ptOffset.x, ptOffset.y, m_sizeImage.cx, m_sizeImage.cy,
			hDCGlyphs, iImage * m_sizeImage.cx, 0, SRCPAINT);

		if (bHiliteShadow)
			BitBlt(hDCMono, 1, 1, m_sizeButton.cx-3, m_sizeButton.cy-3,
				hDCMono, 0, 0, SRCAND);
	}
}

// Raster Ops
#define ROP_DSPDxax  0x00E20746L
#define ROP_PSDPxax  0x00B8074AL

BOOL CToolBar::DrawButton(HDC hDC, int x, int y, int iImage, UINT nStyle)
{
	ASSERT(hDC != NULL);

	// make the coordinates the interior of the button
	x++;
	y++;
	int dx = m_sizeButton.cx - 2;
	int dy = m_sizeButton.cy - 2;

	// border around button
	PatB(hDC, x,    y-1,    dx, 1,  afxData.clrWindowFrame);
	PatB(hDC, x,    y+dy,   dx, 1,  afxData.clrWindowFrame);
	PatB(hDC, x-1,  y,  1,  dy, afxData.clrWindowFrame);
	PatB(hDC, x+dx, y,  1,  dy, afxData.clrWindowFrame);

	// interior grey
	PatB(hDC, x, y, dx, dy, afxData.clrBtnFace);

	CPoint ptOffset;
	ptOffset.x = (dx - m_sizeImage.cx - 1) >> 1;
	ptOffset.y = (dy - m_sizeImage.cy) >> 1;

	if (nStyle & (TBBS_PRESSED | TBBS_CHECKED))
	{
		// pressed in or down or down disabled
		PatB(hDC, x, y, 1, dy, afxData.clrBtnShadow);
		PatB(hDC, x, y, dx, 1, afxData.clrBtnShadow);
		//For any depressed button, add one to the offsets.
		ptOffset.x++;
		ptOffset.y++;
	}
	else
	{
		// regular button look
		PatB(hDC, x, y, 1, dy - 1, afxData.clrBtnHilite);
		PatB(hDC, x, y, dx - 1, 1, afxData.clrBtnHilite);

		PatB(hDC, x + dx - 1, y, 1, dy, afxData.clrBtnShadow);
		PatB(hDC, x, y + dy-1, dx, 1,   afxData.clrBtnShadow);

		PatB(hDC, x + 1 + dx - 3, y + 1, 1, dy-2, afxData.clrBtnShadow);
		PatB(hDC, x + 1, y + dy - 2, dx - 2, 1, afxData.clrBtnShadow);
	}

	if ((nStyle & TBBS_PRESSED) || !(nStyle & TBBS_DISABLED))
	{
		// normal image version
		BitBlt(hDC, x + ptOffset.x, y + ptOffset.y,
			m_sizeImage.cx, m_sizeImage.cy,
			hDCGlyphs, iImage * m_sizeImage.cx, 0, SRCCOPY);

		if (nStyle & TBBS_PRESSED)
			return TRUE;        // nothing more to do (rest of style is ignored)
	}

	if (nStyle & (TBBS_DISABLED | TBBS_INDETERMINATE))
	{
		// disabled or indeterminate version
		CreateMask(iImage, ptOffset, TRUE, FALSE);

		SetTextColor(hDC, 0L);                  // 0's in mono -> 0 (for ROP)
		SetBkColor(hDC, (COLORREF)0x00FFFFFFL); // 1's in mono -> 1

		if (nStyle & TBBS_DISABLED)
		{
			// disabled - draw the hilighted shadow
			HGDIOBJ hbrOld = SelectObject(hDC, afxData.hbrBtnHilite);
			if (hbrOld != NULL)
			{
				// draw hilight color where we have 0's in the mask
				BitBlt(hDC, x + 1, y + 1,
					m_sizeButton.cx-2, m_sizeButton.cy-2,
					hDCMono, 0, 0, ROP_PSDPxax);
				SelectObject(hDC, hbrOld);
			}
		}

		//BLOCK: always draw the shadow
		{
			HGDIOBJ hbrOld = SelectObject(hDC, afxData.hbrBtnShadow);
			if (hbrOld != NULL)
			{
				// draw the shadow color where we have 0's in the mask
				BitBlt(hDC, x, y, m_sizeButton.cx-2, m_sizeButton.cy-2,
					hDCMono, 0, 0, ROP_PSDPxax);
				SelectObject(hDC, hbrOld);
			}
		}
	}

	// if it is checked do the dither brush avoiding the glyph
	if (nStyle & (TBBS_CHECKED | TBBS_INDETERMINATE))
	{
		HGDIOBJ hbrOld = SelectObject(hDC, hbrDither);
		if (hbrOld != NULL)
		{
			ptOffset.x--;
			ptOffset.y--;
			CreateMask(iImage, ptOffset, ~(nStyle & TBBS_INDETERMINATE),
					nStyle & TBBS_DISABLED);

			SetTextColor(hDC, 0L);              // 0 -> 0
			SetBkColor(hDC, (COLORREF)0x00FFFFFFL); // 1 -> 1

			int delta = (nStyle & TBBS_INDETERMINATE) ? 3 : 1;

			// only draw the dither brush where the mask is 1's
			BitBlt(hDC, x+1, y+1, dx-delta, dy-delta,
				hDCMono, 0, 0, ROP_DSPDxax);
			SelectObject(hDC, hbrOld);
		}
	}

	return TRUE;
}

BOOL CToolBar::PrepareDrawButton(DrawState& ds)
{
	ASSERT(m_hbmImageWell != NULL);
	ASSERT(m_sizeButton.cx > 2 && m_sizeButton.cy > 2);

	// We need to kick-start the bitmap selection process.
	ds.hbmOldGlyphs = (HBITMAP)SelectObject(hDCGlyphs, m_hbmImageWell);
	ds.hbmMono = CreateBitmap(m_sizeButton.cx-2, m_sizeButton.cy-2,
					1, 1, NULL);
	ds.hbmMonoOld = (HBITMAP)SelectObject(hDCMono, ds.hbmMono);
	if (ds.hbmOldGlyphs == NULL || ds.hbmMono == NULL || ds.hbmMonoOld == NULL)
	{
		TRACE0("Error: can't draw toolbar\n");
		if (ds.hbmMono)
			DeleteObject(ds.hbmMono);
		return FALSE;
	}
	return TRUE;
}

void CToolBar::EndDrawButton(DrawState& ds)
{
	SelectObject(hDCMono, ds.hbmMonoOld);
	DeleteObject(ds.hbmMono);
	SelectObject(hDCGlyphs, ds.hbmOldGlyphs);
}

/////////////////////////////////////////////////////////////////////////////
// CToolBar creation etc

struct AFX_TBBUTTON
{
	UINT nID;        // Command ID that this button sends
	UINT nStyle;    // TBBS_ styles
	int iImage;     // index into mondo bitmap of this button's picture
						// or size of this spacer
};

inline AFX_TBBUTTON* CToolBar::_GetButtonPtr(int nIndex) const
{
	ASSERT(nIndex >= 0 && nIndex < m_nCount);
	ASSERT(m_pData != NULL);
	return ((AFX_TBBUTTON*)m_pData) + nIndex;
}

// DIBs use BGR format
#define RGB_TO_BGR(r,g,b)       RGB(b,g,r)

static inline COLORREF RGBtoBGR(COLORREF clr)
{
	return RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr));
}

HBITMAP AFXAPI AfxLoadSysColorBitmap(HINSTANCE hInst, HRSRC hRsrc)
{
	static struct
	{
		COLORREF bgrFrom;
		int iSysColorTo;
	}
	BASED_CODE sysColorMap[] =
	{
		// mapping from color in DIB to system color
		{RGB_TO_BGR(000,000,000),  COLOR_BTNTEXT},     // black
		{RGB_TO_BGR(128,128,128),  COLOR_BTNSHADOW},   // dark grey
		{RGB_TO_BGR(192,192,192),  COLOR_BTNFACE},     // bright grey
		{RGB_TO_BGR(255,255,255),  COLOR_BTNHIGHLIGHT} // white
	};
	const int nMaps = afxData.bWin31 ? 4 : 3;

	HGLOBAL hglb;
	if ((hglb = ::LoadResource(hInst, hRsrc)) == NULL)
		return NULL;

	LPBITMAPINFOHEADER lpBitmapInfo = (LPBITMAPINFOHEADER)LockResource(hglb);
	if (lpBitmapInfo == NULL)
		return NULL;

	COLORREF FAR* pColorTable;
	pColorTable = (COLORREF FAR*)
		(((LPSTR)lpBitmapInfo) + (UINT)lpBitmapInfo->biSize);
		// Color table is in BGR DIB format

	// save old color table so we will not change the bitmap
	const int nColorTableSize = 16;
	COLORREF clrSaveColors[nColorTableSize];
	_fmemcpy(clrSaveColors, pColorTable, nColorTableSize*sizeof(COLORREF));

	for (int iColor = 0; iColor < nColorTableSize; iColor++)
	{
		// look for matching BGR color in original
		for (int i = 0; i < nMaps; i++)
		{
			if (pColorTable[iColor] == sysColorMap[i].bgrFrom)
			{
				pColorTable[iColor] = RGBtoBGR(::GetSysColor(
					sysColorMap[i].iSysColorTo));
				break;
			}
		}
	}

	int nWidth = (int)lpBitmapInfo->biWidth;
	int nHeight = (int)lpBitmapInfo->biHeight;
	HDC hDCScreen = ::GetDC(NULL);
	HBITMAP hbm = ::CreateCompatibleBitmap(hDCScreen, nWidth, nHeight);
	::ReleaseDC(NULL, hDCScreen);

	if (hbm != NULL)
	{
		HBITMAP hbmOld = (HBITMAP)::SelectObject(hDCGlyphs, hbm);

		LPSTR lpBits;
		lpBits = (LPSTR)(lpBitmapInfo + 1);
		lpBits += (1 << (lpBitmapInfo->biBitCount)) * sizeof(RGBQUAD);

		StretchDIBits(hDCGlyphs, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight,
			lpBits, (LPBITMAPINFO)lpBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		SelectObject(hDCGlyphs, hbmOld);
	}

	_fmemcpy(pColorTable, clrSaveColors, nColorTableSize*sizeof(COLORREF));
		// restore old color table

	::UnlockResource(hglb);
	::FreeResource(hglb);
	return hbm;
}

IMPLEMENT_DYNAMIC(CToolBar, CControlBar)

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

CToolBar::CToolBar()
{
	m_hbmImageWell = NULL;
	m_hInstImageWell = NULL;
	m_hRsrcImageWell = NULL;
	m_iButtonCapture = -1;      // nothing captured

	// UISG standard sizes
	m_sizeButton.cx = 24;
	m_sizeButton.cy = 22;
	m_sizeImage.cx = 16;
	m_sizeImage.cy = 15;
	m_cxDefaultGap = 6;         // 6 pixels between buttons
	m_cyTopBorder = m_cyBottomBorder = 2;   // 2 pixel for top/bottom gaps

	// initialize the toolbar drawing engine
	if (afxData.pfnFreeToolBar == NULL)
		InitToolBarDraw();
}

CToolBar::~CToolBar()
{
	if (m_hbmImageWell != NULL)
		::DeleteObject(m_hbmImageWell);
}

BOOL CToolBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
	if (pParentWnd != NULL)
		ASSERT_VALID(pParentWnd);   // must have a parent

	dwStyle |= WS_CLIPSIBLINGS;
		// force WS_CLIPSIBLINGS (avoids SetWindowPos bugs)

	// create the HWND
	CRect rect;
	rect.SetRectEmpty();
	if (!CWnd::Create(_afxWndControlBar, NULL, dwStyle, rect, pParentWnd, nID))
		return FALSE;
		// NOTE: Parent must resize itself for control bar to be resized

	// set height
	CRect rectSize;
	rectSize.SetRectEmpty();
	CalcInsideRect(rectSize);       // will be negative size
	m_sizeFixedLayout.cy = m_sizeButton.cy - rectSize.Height();
	ASSERT(m_sizeFixedLayout.cx == 32767);  // max size
	return TRUE;
}

void CToolBar::SetSizes(SIZE sizeButton, SIZE sizeImage)
{
	ASSERT_VALID(this);
	ASSERT(sizeButton.cx > 0 && sizeButton.cy > 0);
	ASSERT(sizeImage.cx > 0 && sizeImage.cy > 0);
	// button must be big enough to hold image + 3 pixels on each side
	ASSERT(sizeButton.cx >= sizeImage.cx + 6);
	ASSERT(sizeButton.cy >= sizeImage.cy + 6);

	m_sizeButton = sizeButton;
	m_sizeImage = sizeImage;
	// set height

	CRect rectSize;
	rectSize.SetRectEmpty();
	CalcInsideRect(rectSize);       // will be negative size
	m_sizeFixedLayout.cy = m_sizeButton.cy - rectSize.Height();
	ASSERT(m_sizeFixedLayout.cx == 32767);  // max size
	Invalidate();   // just to be nice if called when toolbar is visible
}

void CToolBar::SetHeight(int cyHeight)
{
	ASSERT_VALID(this);

	m_sizeFixedLayout.cy = cyHeight;
	ASSERT(m_sizeFixedLayout.cx == 32767);  // max size
	m_cyTopBorder = (cyHeight - m_sizeButton.cy) / 2;
	if (m_cyTopBorder < 0)
	{
		TRACE1("Warning: CToolBar::SetHeight(%d) is smaller than button\n",
			cyHeight);
		m_cyTopBorder = 0;  // will clip at bottom
	}
	// bottom border will be ignored (truncate as needed)
	Invalidate();   // just to be nice if called when toolbar is visible
}

BOOL CToolBar::LoadBitmap(LPCSTR lpszResourceName)
{
	ASSERT_VALID(this);
	ASSERT(lpszResourceName != NULL);

	if (m_hbmImageWell != NULL)
		::DeleteObject(m_hbmImageWell);     // get rid of old one

	m_hInstImageWell = AfxFindResourceHandle(lpszResourceName, RT_BITMAP);

	if ((m_hRsrcImageWell = ::FindResource(m_hInstImageWell,
		lpszResourceName, RT_BITMAP)) == NULL)
		return FALSE;

	m_hbmImageWell = AfxLoadSysColorBitmap(m_hInstImageWell,m_hRsrcImageWell);
	return (m_hbmImageWell != NULL);
}

BOOL CToolBar::SetButtons(const UINT FAR* lpIDArray, int nIDCount)
{
	ASSERT_VALID(this);
	ASSERT(nIDCount >= 1);  // must be at least one of them
	ASSERT(lpIDArray == NULL ||
		AfxIsValidAddress(lpIDArray, sizeof(UINT) * nIDCount, FALSE));

	// first allocate array for panes and copy initial data
	if (!AllocElements(nIDCount, sizeof(AFX_TBBUTTON)))
		return FALSE;
	ASSERT(nIDCount == m_nCount);

	if (lpIDArray != NULL)
	{
		int iImage = 0;
		// go through them adding buttons
		AFX_TBBUTTON* pTBB = (AFX_TBBUTTON*)m_pData;
		for (int i = 0; i < nIDCount; i++, pTBB++)
		{
			ASSERT(pTBB != NULL);
			if ((pTBB->nID = *lpIDArray++) == 0)
			{
				// separator
				pTBB->nStyle = TBBS_SEPARATOR;
				// width of separator includes 2 pixel overlap
				pTBB->iImage = m_cxDefaultGap + 2;
			}
			else
			{
				// a command button with image
				pTBB->nStyle = TBBS_BUTTON;
				pTBB->iImage = iImage++;
			}
		}
	}
	return TRUE;
}

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBar attribute access

int CToolBar::CommandToIndex(UINT nIDFind) const
{
	ASSERT_VALID(this);

	AFX_TBBUTTON* pTBB = _GetButtonPtr(0);
	for (int i = 0; i < m_nCount; i++, pTBB++)
		if (pTBB->nID == nIDFind)
			return i;
	return -1;
}

UINT CToolBar::GetItemID(int nIndex) const
{
	ASSERT_VALID(this);

	return _GetButtonPtr(nIndex)->nID;
}

void CToolBar::GetItemRect(int nIndex, LPRECT lpRect) const
{
	ASSERT_VALID(this);
	ASSERT(nIndex >= 0 && nIndex < m_nCount);
	ASSERT(AfxIsValidAddress(lpRect, sizeof(RECT)));

	CRect rect;
	rect.SetRectEmpty();        // only need top and left
	CalcInsideRect(rect);
	AFX_TBBUTTON* pTBB = (AFX_TBBUTTON*)m_pData;
	for (int iButton = 0; iButton < nIndex; iButton++, pTBB++)
	{
		ASSERT(pTBB != NULL);
		// skip this button or separator
		rect.left += (pTBB->nStyle & TBBS_SEPARATOR) ?
						pTBB->iImage : m_sizeButton.cx;
		rect.left--;    // go back one for overlap
	}
	ASSERT(iButton == nIndex);
	ASSERT(pTBB == _GetButtonPtr(nIndex));

	// button or image width
	int cx = (pTBB->nStyle & TBBS_SEPARATOR) ? pTBB->iImage : m_sizeButton.cx;
	lpRect->right = (lpRect->left = rect.left) + cx;
	lpRect->bottom = (lpRect->top = rect.top) + m_sizeButton.cy;
}

inline UINT CToolBar::_GetButtonStyle(int nIndex) const
{
	return _GetButtonPtr(nIndex)->nStyle;
}

void CToolBar::_SetButtonStyle(int nIndex, UINT nStyle)
{
	AFX_TBBUTTON* pTBB = _GetButtonPtr(nIndex);
	if (pTBB->nStyle != nStyle)
	{
		// change the style and invalidate
		pTBB->nStyle = nStyle;
		InvalidateButton(nIndex);
	}
}

void CToolBar::GetButtonInfo(int nIndex, UINT& nID, UINT& nStyle, int& iImage) const
{
	ASSERT_VALID(this);

	AFX_TBBUTTON* pTBB = _GetButtonPtr(nIndex);
	nID = pTBB->nID;
	nStyle = pTBB->nStyle;
	iImage = pTBB->iImage;
}

void CToolBar::SetButtonInfo(int nIndex, UINT nID, UINT nStyle, int iImage)
{
	ASSERT_VALID(this);

	AFX_TBBUTTON* pTBB = _GetButtonPtr(nIndex);
	pTBB->nID = nID;
	pTBB->iImage = iImage;
	pTBB->nStyle = nStyle;
	InvalidateButton(nIndex);
}

void CToolBar::DoPaint(CDC* pDC)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	CControlBar::DoPaint(pDC);      // draw border
	CRect rect;
	GetClientRect(rect);
	CalcInsideRect(rect);
	// force the full size of the button
	rect.bottom = rect.top + m_sizeButton.cy;

	DrawState ds;
	if (!PrepareDrawButton(ds))
		return;     // something went wrong

	AFX_TBBUTTON* pTBB = (AFX_TBBUTTON*)m_pData;
	for (int iButton = 0; iButton < m_nCount; iButton++, pTBB++)
	{
		ASSERT(pTBB != NULL);
		if (pTBB->nStyle & TBBS_SEPARATOR)
		{
			// separator
			rect.right = rect.left + pTBB->iImage;
		}
		else
		{
			rect.right = rect.left + m_sizeButton.cx;
			if (::RectVisible(pDC->m_hDC, &rect))
			{
				DrawButton(pDC->m_hDC, rect.left, rect.top,
					pTBB->iImage, pTBB->nStyle);
			}
		}
		// adjust for overlap
		rect.left = rect.right - 1;
	}
	EndDrawButton(ds);
}

void CToolBar::InvalidateButton(int nIndex)
{
	ASSERT_VALID(this);

	CRect rect;
	GetItemRect(nIndex, &rect);
	InvalidateRect(rect, FALSE);    // don't erase background
}


int CToolBar::HitTest(CPoint point) // in window relative coords
{
	CRect rect;
	rect.SetRectEmpty();        // only need top and left
	CalcInsideRect(rect);
	if (point.y < rect.top || point.y >= rect.top + m_sizeButton.cy)
		return -1;      // no Y hit

	AFX_TBBUTTON* pTBB = (AFX_TBBUTTON*)m_pData;
	ASSERT(pTBB != NULL);
	for (int iButton = 0; iButton < m_nCount; iButton++, pTBB++)
	{
		if (point.x < rect.left)
			break;      // missed it
		rect.left += (pTBB->nStyle & TBBS_SEPARATOR) ?
						pTBB->iImage : m_sizeButton.cx;
		rect.left--;    // go back one for overlap
		if (point.x <= rect.left && !(pTBB->nStyle & TBBS_SEPARATOR))
			return iButton;     // hit !
	}
	return -1;      // nothing hit
}


/////////////////////////////////////////////////////////////////////////////
// CToolBar message handlers

BEGIN_MESSAGE_MAP(CToolBar, CControlBar)
	//{{AFX_MSG_MAP(CToolBar)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CANCELMODE()
	ON_WM_SYSCOLORCHANGE()
	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CToolBar::OnLButtonDown(UINT, CPoint point)
{
	if ((m_iButtonCapture = HitTest(point)) < 0)
		return;     // nothing hit

	AFX_TBBUTTON* pTBB = _GetButtonPtr(m_iButtonCapture);
	ASSERT(!(pTBB->nStyle & TBBS_SEPARATOR));
	if (pTBB->nStyle & TBBS_DISABLED)
	{
		m_iButtonCapture = -1;
		return;     // don't press it
	}

	SetCapture();
	pTBB->nStyle |= TBBS_PRESSED;
	InvalidateButton(m_iButtonCapture);
	UpdateWindow(); // immediate feedback

	GetOwner()->SendMessage(WM_SETMESSAGESTRING, (WPARAM)pTBB->nID);
}

LRESULT CToolBar::OnHelpHitTest(WPARAM wParam, LPARAM lParam)
{
	CPoint point = lParam;
	int iButton = HitTest(point);
	if (iButton == -1)
		return CControlBar::OnHelpHitTest(wParam, lParam);
	AFX_TBBUTTON* pTBB = _GetButtonPtr(iButton);
	ASSERT(pTBB->nID != 0);
	return HID_BASE_COMMAND+pTBB->nID;
}

void CToolBar::OnMouseMove(UINT, CPoint point)
{
	if (m_iButtonCapture < 0)
		return;     // not captured
	AFX_TBBUTTON* pTBB = _GetButtonPtr(m_iButtonCapture);
	ASSERT(!(pTBB->nStyle & TBBS_SEPARATOR));

	UINT nNewStyle = (pTBB->nStyle & ~TBBS_PRESSED);
	if (GetCapture() != this)
	{
		m_iButtonCapture = -1; // lost capture
		GetOwner()->SendMessage(WM_SETMESSAGESTRING,
			(WPARAM)AFX_IDS_IDLEMESSAGE);
	}
	else
	{
		// should be pressed if still hitting the captured button
		if (HitTest(point) == m_iButtonCapture)
			nNewStyle |= TBBS_PRESSED;
	}
	_SetButtonStyle(m_iButtonCapture, nNewStyle);
	UpdateWindow(); // immediate feedback
}

void CToolBar::OnLButtonUp(UINT, CPoint point)
{
	if (m_iButtonCapture < 0)
		return;     // not captured

	AFX_TBBUTTON* pTBB = _GetButtonPtr(m_iButtonCapture);
	ASSERT(!(pTBB->nStyle & TBBS_SEPARATOR));
	UINT nIDCmd = 0;

	UINT nNewStyle = (pTBB->nStyle & ~TBBS_PRESSED);
	if (GetCapture() == this)
	{
		// we did not lose the capture
		ReleaseCapture();
		if (HitTest(point) == m_iButtonCapture &&
		  !(pTBB->nStyle & TBBS_DISABLED))
		{
			// pressed, will send command notification
			nIDCmd = pTBB->nID;

			if (pTBB->nStyle & TBBS_CHECKBOX)
			{
				// auto check: three state => down
				if (nNewStyle & TBBS_INDETERMINATE)
					nNewStyle &= ~TBBS_INDETERMINATE;

				nNewStyle ^= TBBS_CHECKED;
			}
		}
	}

	_SetButtonStyle(m_iButtonCapture, nNewStyle);
	m_iButtonCapture = -1;
	UpdateWindow(); // immediate feedback

	// first flush the drag mode
	GetOwner()->SendMessage(WM_SETMESSAGESTRING, (WPARAM)AFX_IDS_IDLEMESSAGE);

	if (nIDCmd != 0)
		GetOwner()->SendMessage(WM_COMMAND, nIDCmd);    // send command
}

void CToolBar::OnCancelMode()
{
	if (m_iButtonCapture < 0)
		return;     // not captured

	AFX_TBBUTTON* pTBB = _GetButtonPtr(m_iButtonCapture);
	ASSERT(!(pTBB->nStyle & TBBS_SEPARATOR));
	UINT nNewStyle = (pTBB->nStyle & ~TBBS_PRESSED);
	if (GetCapture() == this)
		ReleaseCapture();
	_SetButtonStyle(m_iButtonCapture, nNewStyle);
	m_iButtonCapture = -1;
	UpdateWindow();
	GetOwner()->SendMessage(WM_SETMESSAGESTRING, (WPARAM)AFX_IDS_IDLEMESSAGE);
}

void CToolBar::OnSysColorChange()
{
	// re-initialize global dither brush
	HBITMAP hbmGray = ::CreateDitherBitmap();
	if (hbmGray != NULL)
	{
		HBRUSH hbrNew = ::CreatePatternBrush(hbmGray);
		if (hbrNew != NULL)
		{
			if (hbrDither != NULL)
				::DeleteObject(hbrDither);      // free old one
			hbrDither = hbrNew;
		}
		::DeleteObject(hbmGray);
	}

	// re-color bitmap for toolbar
	if (m_hbmImageWell != NULL)
	{
		HBITMAP hbmNew;
		hbmNew = AfxLoadSysColorBitmap(m_hInstImageWell, m_hRsrcImageWell);
		if (hbmNew != NULL)
		{
			::DeleteObject(m_hbmImageWell);     // free old one
			m_hbmImageWell = hbmNew;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CToolBar idle update through CToolCmdUI class

class CToolCmdUI : public CCmdUI        // class private to this file !
{
public: // re-implementations only
	virtual void Enable(BOOL bOn);
	virtual void SetCheck(int nCheck);
	virtual void SetText(LPCSTR lpszText);
};

void CToolCmdUI::Enable(BOOL bOn)
{
	m_bEnableChanged = TRUE;
	CToolBar* pToolBar = (CToolBar*)m_pOther;
	ASSERT(pToolBar != NULL);
	ASSERT(pToolBar->IsKindOf(RUNTIME_CLASS(CToolBar)));
	ASSERT(m_nIndex < m_nIndexMax);

	UINT nNewStyle = pToolBar->_GetButtonStyle(m_nIndex) & ~TBBS_DISABLED;
	if (!bOn)
		nNewStyle |= TBBS_DISABLED;
	ASSERT(!(nNewStyle & TBBS_SEPARATOR));
	pToolBar->_SetButtonStyle(m_nIndex, nNewStyle);
}

void CToolCmdUI::SetCheck(int nCheck)
{
	ASSERT(nCheck >= 0 && nCheck <= 2); // 0=>off, 1=>on, 2=>indeterminate
	CToolBar* pToolBar = (CToolBar*)m_pOther;
	ASSERT(pToolBar != NULL);
	ASSERT(pToolBar->IsKindOf(RUNTIME_CLASS(CToolBar)));
	ASSERT(m_nIndex < m_nIndexMax);

	UINT nNewStyle = pToolBar->_GetButtonStyle(m_nIndex) &
				~(TBBS_CHECKED | TBBS_INDETERMINATE);
	if (nCheck == 1)
		nNewStyle |= TBBS_CHECKED;
	else if (nCheck == 2)
		nNewStyle |= TBBS_INDETERMINATE;
	ASSERT(!(nNewStyle & TBBS_SEPARATOR));
	pToolBar->_SetButtonStyle(m_nIndex, nNewStyle | TBBS_CHECKBOX);
}

void CToolCmdUI::SetText(LPCSTR)
{
	// ignore it
}


void CToolBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CToolCmdUI state;
	state.m_pOther = this;

	state.m_nIndexMax = (UINT)m_nCount;
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
	  state.m_nIndex++)
	{
		AFX_TBBUTTON* pTBB = _GetButtonPtr(state.m_nIndex);
		state.m_nID = pTBB->nID;

		// ignore pressed buttons or separators
		if (!(pTBB->nStyle & (TBBS_PRESSED|TBBS_SEPARATOR)))
			state.DoUpdate(pTarget, bDisableIfNoHndler);
	}

	// update the dialog controls added to the toolbar
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}


/////////////////////////////////////////////////////////////////////////////
// CToolBar diagnostics

#ifdef _DEBUG
void CToolBar::AssertValid() const
{
	CControlBar::AssertValid();
	if (afxData.bWin31)
		ASSERT(m_hbmImageWell == NULL || ::IsGDIObject(m_hbmImageWell));

	if (m_hbmImageWell != NULL)
	{
		ASSERT(m_hRsrcImageWell != NULL);
		ASSERT(m_hInstImageWell != NULL);
	}
}

void CToolBar::Dump(CDumpContext& dc) const
{
	CControlBar::Dump(dc);
	AFX_DUMP1(dc, "\nm_hbmImageWell = ", (UINT)m_hbmImageWell);
	AFX_DUMP1(dc, "\nm_hInstImageWell = ", (UINT)m_hInstImageWell);
	AFX_DUMP1(dc, "\nm_hRsrcImageWell = ", (UINT)m_hRsrcImageWell);
	AFX_DUMP1(dc, "\nm_iButtonCapture = ", m_iButtonCapture);
	AFX_DUMP1(dc, "\nm_sizeButton = ", m_sizeButton);
	AFX_DUMP1(dc, "\nm_sizeImage = ", m_sizeImage);

	if (dc.GetDepth() > 0)
	{
		for (int i = 0; i < m_nCount; i++)
		{
			AFX_TBBUTTON* pTBB = _GetButtonPtr(i);
			AFX_DUMP1(dc, "\ntoolbar button[", i); AFX_DUMP0(dc, "] = { ");
			AFX_DUMP1(dc, "\n\tnID = ", pTBB->nID);
			AFX_DUMP1(dc, "\n\tnStyle = ", pTBB->nStyle);
			if (pTBB->nStyle & TBBS_SEPARATOR)
				AFX_DUMP1(dc, "\n\tiImage (separator width) = ", pTBB->iImage);
			else
				AFX_DUMP1(dc,"\n\tiImage (bitmap image index) = ",pTBB->iImage);
			AFX_DUMP0(dc, "\n\t}");
		}
	}
}
#endif

/////////////////////////////////////////////////////////////////////////////
