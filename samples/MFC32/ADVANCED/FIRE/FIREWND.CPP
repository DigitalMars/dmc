// firewnd.cpp : implementation file
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
#include "fire.h"
#include "firewnd.h"

#include <math.h> // for HSI conversion

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

static RGBQUAD rgbStd256[] =
{
	{   0,  0,  0, 0 },	{   0,  0,128, 0 },	{   0,128,  0, 0 },	{   0,128,128, 0 },
	{ 128,  0,  0, 0 },	{ 128,  0,128, 0 },	{ 128,128,  0, 0 },	{ 192,192,192, 0 },
	{ 192,220,192, 0 },	{ 240,202,166, 0 },	{ 238,238,238, 0 },	{ 221,221,221, 0 },
	{ 204,204,204, 0 },	{ 187,187,187, 0 },	{ 170,170,170, 0 },	{ 153,153,153, 0 },
	{ 136,136,136, 0 },	{ 119,119,119, 0 },	{ 102,102,102, 0 },	{  85, 85, 85, 0 },
	{  68, 68, 68, 0 },	{  51, 51, 51, 0 },	{  34, 34, 34, 0 },	{  17, 17, 17, 0 },
	{ 204,255,255, 0 },	{ 153,255,255, 0 },	{ 102,255,255, 0 },	{  51,255,255, 0 },
	{ 255,204,255, 0 },	{ 204,204,255, 0 },	{ 153,204,255, 0 },	{ 102,204,255, 0 },
	{  51,204,255, 0 },	{   0,204,255, 0 },	{ 255,153,255, 0 },	{ 204,153,255, 0 },
	{ 153,153,255, 0 },	{ 102,153,255, 0 },	{  51,153,255, 0 },	{   0,153,255, 0 },
	{ 255,102,255, 0 },	{ 204,102,255, 0 },	{ 153,102,255, 0 },	{ 102,102,255, 0 },
	{  51,102,255, 0 },	{   0,102,255, 0 },	{ 255, 51,255, 0 },	{ 204, 51,255, 0 },
	{ 153, 51,255, 0 },	{ 102, 51,255, 0 },	{  51, 51,255, 0 },	{   0, 51,255, 0 },
	{ 204,  0,255, 0 },	{ 153,  0,255, 0 },	{ 102,  0,255, 0 },	{  51,  0,255, 0 },
	{ 255,255,204, 0 },	{ 204,255,204, 0 },	{ 153,255,204, 0 },	{ 102,255,204, 0 },
	{  51,255,204, 0 },	{   0,255,204, 0 },	{ 255,204,204, 0 },	{ 153,204,204, 0 },
	{ 102,204,204, 0 },	{  51,204,204, 0 },	{   0,204,204, 0 },	{ 255,153,204, 0 },
	{ 204,153,204, 0 },	{ 153,153,204, 0 },	{ 102,153,204, 0 },	{  51,153,204, 0 },
	{   0,153,204, 0 },	{ 255,102,204, 0 },	{ 204,102,204, 0 },	{ 153,102,204, 0 },
	{ 102,102,204, 0 },	{  51,102,204, 0 },	{   0,102,204, 0 },	{ 255, 51,204, 0 },
	{ 204, 51,204, 0 },	{ 153, 51,204, 0 },	{ 102, 51,204, 0 },	{  51, 51,204, 0 },
	{   0, 51,204, 0 },	{ 255,  0,204, 0 },	{ 204,  0,204, 0 },	{ 153,  0,204, 0 },
	{ 102,  0,204, 0 },	{  51,  0,204, 0 },	{ 255,255,153, 0 },	{ 204,255,153, 0 },
	{ 153,255,153, 0 },	{ 102,255,153, 0 },	{  51,255,153, 0 },	{   0,255,153, 0 },
	{ 255,204,153, 0 },	{ 204,204,153, 0 },	{ 153,204,153, 0 },	{ 102,204,153, 0 },
	{  51,204,153, 0 },	{   0,204,153, 0 },	{ 255,153,153, 0 },	{ 204,153,153, 0 },
	{ 102,153,153, 0 },	{  51,153,153, 0 },	{   0,153,153, 0 },	{ 255,102,153, 0 },
	{ 204,102,153, 0 },	{ 153,102,153, 0 },	{ 102,102,153, 0 },	{  51,102,153, 0 },
	{   0,102,153, 0 },	{ 255, 51,153, 0 },	{ 204, 51,153, 0 },	{ 153, 51,153, 0 },
	{ 102, 51,153, 0 },	{  51, 51,153, 0 },	{   0, 51,153, 0 },	{ 255,  0,153, 0 },
	{ 204,  0,153, 0 },	{ 153,  0,153, 0 },	{ 102,  0,153, 0 },	{  51,  0,153, 0 },
	{ 255,255,102, 0 },	{ 204,255,102, 0 },	{ 153,255,102, 0 },	{ 102,255,102, 0 },
	{  51,255,102, 0 },	{   0,255,102, 0 },	{ 255,204,102, 0 },	{ 204,204,102, 0 },
	{ 153,204,102, 0 },	{ 102,204,102, 0 },	{  51,204,102, 0 },	{   0,204,102, 0 },
	{ 255,153,102, 0 },	{ 204,153,102, 0 },	{ 153,153,102, 0 },	{ 102,153,102, 0 },
	{  51,153,102, 0 },	{   0,153,102, 0 },	{ 255,102,102, 0 },	{ 204,102,102, 0 },
	{ 153,102,102, 0 },	{  51,102,102, 0 },	{   0,102,102, 0 },	{ 255, 51,102, 0 },
	{ 204, 51,102, 0 },	{ 153, 51,102, 0 },	{ 102, 51,102, 0 },	{  51, 51,102, 0 },
	{   0, 51,102, 0 },	{ 255,  0,102, 0 },	{ 204,  0,102, 0 },	{ 153,  0,102, 0 },
	{ 102,  0,102, 0 },	{  51,  0,102, 0 },	{ 255,255, 51, 0 },	{ 204,255, 51, 0 },
	{ 153,255, 51, 0 },	{ 102,255, 51, 0 },	{  51,255, 51, 0 },	{   0,255, 51, 0 },
	{ 255,204, 51, 0 },	{ 204,204, 51, 0 },	{ 153,204, 51, 0 },	{ 102,204, 51, 0 },
	{  51,204, 51, 0 },	{   0,204, 51, 0 },	{ 255,153, 51, 0 },	{ 204,153, 51, 0 },
	{ 153,153, 51, 0 },	{ 102,153, 51, 0 },	{  51,153, 51, 0 },	{   0,153, 51, 0 },
	{ 255,102, 51, 0 },	{ 204,102, 51, 0 },	{ 153,102, 51, 0 },	{ 102,102, 51, 0 },
	{  51,102, 51, 0 },	{   0,102, 51, 0 },	{ 255, 51, 51, 0 },	{ 204, 51, 51, 0 },
	{ 153, 51, 51, 0 },	{ 102, 51, 51, 0 },	{   0, 51, 51, 0 },	{ 255,  0, 51, 0 },
	{ 204,  0, 51, 0 },	{ 153,  0, 51, 0 },	{ 102,  0, 51, 0 },	{  51,  0, 51, 0 },
	{ 204,255,  0, 0 },	{ 153,255,  0, 0 },	{ 102,255,  0, 0 },	{  51,255,  0, 0 },
	{ 255,204,  0, 0 },	{ 204,204,  0, 0 },	{ 153,204,  0, 0 },	{ 102,204,  0, 0 },
	{  51,204,  0, 0 },	{ 255,153,  0, 0 },	{ 204,153,  0, 0 },	{ 153,153,  0, 0 },
	{ 102,153,  0, 0 },	{   0,  0,238, 0 },	{   0,  0,221, 0 },	{   0,  0,204, 0 },
	{   0,  0,187, 0 },	{   0,  0,170, 0 },	{   0,  0,153, 0 },	{   0,  0,136, 0 },
	{   0,  0,119, 0 },	{   0,  0,102, 0 },	{   0,  0, 85, 0 },	{   0,  0, 68, 0 },
	{   0,  0, 51, 0 },	{   0,  0, 34, 0 },	{   0,  0, 17, 0 },	{   0,238,  0, 0 },
	{   0,221,  0, 0 },	{   0,204,  0, 0 },	{   0,187,  0, 0 },	{   0,170,  0, 0 },
	{   0,153,  0, 0 },	{   0,136,  0, 0 },	{   0,119,  0, 0 },	{   0,102,  0, 0 },
	{   0, 85,  0, 0 },	{   0, 68,  0, 0 },	{   0, 51,  0, 0 },	{   0, 34,  0, 0 },
	{   0, 17,  0, 0 },	{ 238,  0,  0, 0 },	{ 221,  0,  0, 0 },	{ 204,  0,  0, 0 },
	{ 187,  0,  0, 0 },	{ 170,  0,  0, 0 },	{ 153,  0,  0, 0 },	{ 136,  0,  0, 0 },
	{ 119,  0,  0, 0 },	{ 102,  0,  0, 0 },	{  85,  0,  0, 0 },	{  68,  0,  0, 0 },
	{  51,  0,  0, 0 },	{  34,  0,  0, 0 },	{ 240,251,255, 0 },	{ 164,160,160, 0 },
	{ 128,128,128, 0 },	{   0,  0,255, 0 },	{   0,255,  0, 0 },	{   0,255,255, 0 },
	{ 255,  0,  0, 0 },	{ 255,  0,255, 0 },	{ 255,255,  0, 0 },	{ 255,255,255, 0 }
};					   


/////////////////////////////////////////////////////////////////////////////
// CFireWnd

// Initialize the seed for random number generation.  For a more random
// flame this could be initialized by a call to ::rand().  Declaring it
// this way causes the same flame to be drawn each time the application
// is executed.
unsigned long CFireWnd::m_RandSeed = 0x38549391;

CFireWnd::CFireWnd()
{
	m_nDecay = 5;
	m_nFlammability = 385;
	m_nMaxHeat = 223;
	m_nSpreadRate = 20;
	m_nSmoothness = 1;
	m_nDistribution = 1;
	m_nChaos = 50;

	m_nSize = 0;
	m_MaxBurn = 0;
	m_Fire = NULL;

	m_pMemDC = NULL;
	m_pWinDC = NULL;
	m_pOldPalette = NULL;
	m_pOldBitmap = NULL;
}

CFireWnd::~CFireWnd()
{
}

BEGIN_MESSAGE_MAP(CFireWnd, CStatic)
	//{{AFX_MSG_MAP(CFireWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFireWnd Operations

void CFireWnd::InitFire(int nColor)
{
	CreatePalette(nColor);

	CreateBitmap();

	{
		CWnd* pActiveWnd = CWnd::GetActiveWindow();
		if (pActiveWnd != NULL)
			pActiveWnd->SendMessage(WM_QUERYNEWPALETTE);
	}
}

CPalette* CFireWnd::GetPalette()
{
	if (m_Palette.m_hObject != NULL)
		return &m_Palette;
	else
		return NULL;
}

void CFireWnd::CreatePalette(int nColor)
{
	UINT i;

	if (m_Palette.m_hObject != NULL)
		m_Palette.DeleteObject();

	memcpy( m_rgbPalette, rgbStd256, sizeof(RGBQUAD) * 256 );

	int r,g,b;

	switch(nColor)
	{
	case blue:
		b =	256+256+255;
		g =	256+255;
		r = 255;
		break;
	case green:
		g =	256+256+255;
		r = 256+255;
		b =	255;
		break;
	case red:
	default:
		r =	256+256+255;
		g =	256+255;
		b = 255;
		break;
	}

	for(i = 239; i > 15; i--)
	{
		m_rgbPalette[i].rgbRed = (r > 255) ? 255 : r;
		m_rgbPalette[i].rgbGreen = (g > 255) ? 255 : g;
		m_rgbPalette[i].rgbBlue = (b > 255) ? 255 : b;
		r = (r > 3) ? (r - 4) : 0;
		g = (g > 3) ? (g - 4) : 0;
		b = (b > 3) ? (b - 4) : 0;
	}

	LPLOGPALETTE lpLogPal;
	lpLogPal = (LPLOGPALETTE) new BYTE[sizeof(LOGPALETTE) + ((255) * sizeof(PALETTEENTRY))];
	lpLogPal->palVersion = 0x0300;
	lpLogPal->palNumEntries = 256;

	for (i = 0; i < 256; i++)
	{
		lpLogPal->palPalEntry[i].peRed = m_rgbPalette[i].rgbRed;
		lpLogPal->palPalEntry[i].peGreen = m_rgbPalette[i].rgbGreen;
		lpLogPal->palPalEntry[i].peBlue = m_rgbPalette[i].rgbBlue;
		lpLogPal->palPalEntry[i].peFlags = 0;
	}

	VERIFY( m_Palette.CreatePalette( lpLogPal ) );
	delete [] (BYTE *)lpLogPal;
}

CSize CFireWnd::GetBitmapSize()
{
	if (m_Bitmap.m_hObject == NULL)
		return CSize(0,0);

	BITMAP bm;
	m_Bitmap.GetObject( sizeof(BITMAP), &bm );
	return CSize(bm.bmWidth, bm.bmHeight);
}

#define WIDTHBYTES(bits) ((((bits) + 31) / 32) * 4)

void CFireWnd::CreateBitmap()
{
	if (m_pMemDC != NULL)
	{
		m_pMemDC->SelectObject(m_pOldBitmap);
		delete m_pMemDC;
		m_pMemDC = NULL;
		m_pOldBitmap = NULL;
	}

	if (m_pWinDC != NULL)
	{
		m_pWinDC->SelectObject(m_pOldPalette);
		delete m_pWinDC;
		m_pWinDC = NULL;
		m_pOldPalette = NULL;
	}

	if (m_Bitmap.m_hObject != NULL)
		m_Bitmap.DeleteObject();

	if (m_Fire != NULL)
	{
		delete m_Fire;
		m_Fire = NULL;
	}

	CRect rect;
	CSize size;

	GetClientRect(&rect);
	size = rect.Size();

	if (m_MaxBurn == 0)
		m_MaxBurn = (int)(size.cy * 0.75);

	if (m_nSize == 0)
		m_nSize = (int)(size.cx / 2);

	m_Fire = new BYTE[size.cx];
	
	LPBITMAPINFO lpbi;

	// Fill in the BITMAPINFOHEADER
	lpbi = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD))];
	lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbi->bmiHeader.biWidth = size.cx;
	lpbi->bmiHeader.biHeight = size.cy;
	lpbi->bmiHeader.biPlanes = 1;
	lpbi->bmiHeader.biBitCount = 8;
	lpbi->bmiHeader.biCompression = BI_RGB;
	lpbi->bmiHeader.biSizeImage = WIDTHBYTES((DWORD)size.cx * 8) * size.cy;
	lpbi->bmiHeader.biXPelsPerMeter = 0;
	lpbi->bmiHeader.biYPelsPerMeter = 0;
	lpbi->bmiHeader.biClrUsed = 0;
	lpbi->bmiHeader.biClrImportant = 0;

	// Fill in the color table
	UINT uUsage = DIB_RGB_COLORS;
	memcpy( lpbi->bmiColors, m_rgbPalette, sizeof(RGBQUAD) * 256 );

	m_pWinDC = new CWindowDC(this);

	HBITMAP hBitmap = CreateDIBSection( m_pWinDC->m_hDC, lpbi, uUsage, (void **)&m_pBits, NULL, 0 );

	delete [] (BYTE *)lpbi;

	ASSERT(hBitmap != NULL);

	m_Bitmap.Attach( hBitmap );

	for (int y = 0; y < size.cy; y++ )
	{
		BYTE* pRow = (m_pBits + WIDTHBYTES((DWORD)size.cx * 8) * y);
   		for (int x = 0; x < size.cx; x++ )
			*pRow++ = 16;
	}

	m_pMemDC = new CDC;
	
	VERIFY( m_pMemDC->CreateCompatibleDC(m_pWinDC) );

	m_pOldPalette = m_pWinDC->SelectPalette( &m_Palette, FALSE );
	m_pWinDC->RealizePalette();
	m_pOldBitmap = m_pMemDC->SelectObject( &m_Bitmap );

	for (int x = 0; x < size.cx; x++ )
		m_Fire[x] = 16;
}

//
//	This routine was adapted from a pascal routine written by
//	Frank Jan Sorensen Alias:Frank Patxi (fjs@lab.jt.dk)
//
//	See AboutBox for a special thank you for all his work.
//

void CFireWnd::RenderFlame()
{
	CSize size = GetBitmapSize();
	int xStart, xEnd, x, y;
	BYTE* pRow;
	BYTE* pNextRow;

	xStart = (size.cx - m_nSize) / 2;
	xEnd = xStart + m_nSize + 1;

	{
		pRow = m_pBits;
		for (x=0;x<size.cx;x++)
		{
			if (x < (xStart + m_nDistribution) || x >= (xEnd - m_nDistribution))
				m_Fire[x] = 16;
			*pRow++ = m_Fire[x];
		}
	}

	for (y = m_MaxBurn; y > 0; y--)
	{
		pRow = (m_pBits + WIDTHBYTES((DWORD)size.cx * 8) * y);
		pNextRow = (m_pBits + WIDTHBYTES((DWORD)size.cx * 8) * (y - 1));

		if ((rand() % 2) == 0)
		{
			for (x = 0; x < size.cx; x++)
			{
				BurnPoint(pRow, pNextRow);
				pRow++;
				pNextRow++;
			}
		}
		else
		{
			pRow += size.cx - 1;
			pNextRow += size.cx - 1;
			for (x = 0; x < size.cx; x++)
			{
				BurnPoint(pRow, pNextRow);
				pRow--;
				pNextRow--;
			}
		}
	}

	int MaxHeat = m_nMaxHeat + 16;

	if ( rand() % (400 - m_nFlammability) == 0)
	{
		int off = m_nSize - 5;
		off = rand() % off;
		off += xStart;

		for (x = off; x < off + 5; x++)
			m_Fire[x] = 239;
	}

	for (x = xStart; x < xEnd; x++)
	{
		if (m_Fire[x] < MaxHeat)
		{
			int val = rand() % m_nChaos+1;
			val -= m_nChaos / 2;
			val += m_nSpreadRate;
			val += m_Fire[x];

			if ( val > MaxHeat) 
				m_Fire[x] = MaxHeat;
			else if ( val < 16)
				m_Fire[x] = 16;
			else
				m_Fire[x] = val;
 		}
		else
			m_Fire[x] = MaxHeat;
	}

	if (m_nSmoothness > 0)
	{
		xStart += m_nSmoothness;
		xEnd -= m_nSmoothness;
		for (x = xStart; x < xEnd; x++)
		{
			int val = 0;
			for (y = x - m_nSmoothness; y < x + 1 + m_nSmoothness; y++)
				val += m_Fire[y];

			m_Fire[x] = val / (2*m_nSmoothness+1);
		}
	}
}

void CFireWnd::PaintFlame(CDC* pDC)
{
	if (m_Palette.m_hObject != NULL  && m_Bitmap.m_hObject != NULL)
	{
		CSize size = GetBitmapSize();

		if (pDC != NULL)
		{
			CDC dc;
			CPalette *pOldPalette;

			pOldPalette = pDC->SelectPalette( &m_Palette, FALSE );
			pDC->RealizePalette();
			VERIFY( dc.CreateCompatibleDC(pDC) );
			CBitmap* pOldBitmap = dc.SelectObject( &m_Bitmap );
			pDC->BitBlt( 0, 0, size.cx, size.cy, &dc, 0, 0, SRCCOPY );
			dc.SelectObject(pOldBitmap);
			pDC->SelectPalette( pOldPalette, TRUE );
		}
		else
		{
			m_pWinDC->BitBlt( 0, 0, size.cx, size.cy, m_pMemDC, 0, 0, SRCCOPY );
		}
	}
}

void CFireWnd::SetMaxBurn(int nMax)
{
	if (m_Palette.m_hObject != NULL  && m_Bitmap.m_hObject != NULL)
	{
		CSize size = GetBitmapSize();

		m_MaxBurn = (int)(size.cy * (nMax/100.0));

		for (int y = size.cy; y > m_MaxBurn; y-- )
		{
			BYTE* pRow = (m_pBits + WIDTHBYTES((DWORD)size.cx * 8) * y);
	   		for (int x = 0; x < size.cx; x++ )
				*pRow++ = 16;
		}
	}
}

int CFireWnd::GetMaxBurn()
{
	return m_MaxBurn;
}

/////////////////////////////////////////////////////////////////////////////
// CFireWnd message handlers

void CFireWnd::OnDestroy() 
{
	CStatic::OnDestroy();
	
	if (m_pMemDC != NULL)
	{
		m_pMemDC->SelectObject(m_pOldBitmap);
		delete m_pMemDC;
		m_pMemDC = NULL;
		m_pOldBitmap = NULL;
	}

	if (m_pWinDC != NULL)
	{
		m_pWinDC->SelectObject(m_pOldPalette);
		delete m_pWinDC;
		m_pWinDC = NULL;
		m_pOldPalette = NULL;
	}

	if (m_Bitmap.m_hObject != NULL)
		m_Bitmap.DeleteObject();

	if (m_Palette.m_hObject != NULL)
		m_Palette.DeleteObject();

	if (m_Fire != NULL)
	{
		delete m_Fire;
		m_Fire = NULL;
	}
}

void CFireWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	PaintFlame(&dc);
}

void CFireWnd::OnPaletteChanged(CWnd* pFocusWnd) 
{
	if (pFocusWnd == this || IsChild(pFocusWnd))
		return;

	OnQueryNewPalette();
}

BOOL CFireWnd::OnQueryNewPalette() 
{
	if (GetPalette() == NULL)
		return FALSE;

	{
		CClientDC dc(this);
		CPalette* pOldPalette = dc.SelectPalette(&m_Palette,
			GetCurrentMessage()->message == WM_PALETTECHANGED);
		UINT nChanged = dc.RealizePalette();
		dc.SelectPalette(pOldPalette, TRUE);

		if (nChanged == 0)	// no change to our mapping
			return FALSE;
	}

	// some changes have been made; invalidate
	Invalidate(FALSE);

	return TRUE;
}
