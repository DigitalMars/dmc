// StreamDib.cpp : implementation file
//

#include "stdafx.h"
#include "AccsPict.h"
#include "StrmDib.h"


/////////////////////////////////////////////////////////////////////////////
// Definitions

IMPLEMENT_DYNAMIC(CStreamDib, CObject)
									  
#ifdef _DEBUG
#define new DEBUG_NEW

#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStreamDib

CStreamDib::CStreamDib()
{
	m_lpbi = NULL;
	m_size = CSize(0, 0);	// Initialize size to NULL;

	m_Palette.m_hObject = NULL;
	m_Bitmap.m_hObject = NULL;
}

CStreamDib::~CStreamDib()
{
	if (m_lpbi != NULL)
	{
		delete m_lpbi;
		m_lpbi = NULL;
	}

	// Kill off bitmap and palette
	if (m_Palette.m_hObject != NULL)
		m_Palette.DeleteObject();

	if (m_Bitmap.m_hObject != NULL)
		m_Bitmap.DeleteObject();
}


/////////////////////////////////////////////////////////////////////////////
// CStreamDib Attributes

CPalette* CStreamDib::GetPalette()
{
	if (m_Palette.m_hObject == NULL)
		return NULL;

	return &m_Palette;
}

CBitmap* CStreamDib::GetBitmap()
{
	if (m_Bitmap.m_hObject == NULL)
		return NULL;

	return &m_Bitmap;
}


CSize CStreamDib::GetSize()
{
	return m_size;
}

/////////////////////////////////////////////////////////////////////////////
// CStreamDib Operations

BOOL CStreamDib::LoadBitmap(LPSTREAM lpStream)
{
	LPBYTE lpFileHeader = NULL;
	ULONG ulRead;
	int nColors;
	LPBYTE lpInfo = NULL;
	BYTE* pBits = NULL;

	TRY
	{
		if (lpStream == NULL)
			AfxThrowMemoryException();

		// Read in the BITMAPFILEHEADER
		lpFileHeader = new BYTE[sizeof(BITMAPFILEHEADER)];
		if (FAILED(lpStream->Read((void*)lpFileHeader, 
			sizeof(BITMAPFILEHEADER), &ulRead)))
			AfxThrowMemoryException();

		ASSERT(ulRead == sizeof(BITMAPFILEHEADER));

		LPBITMAPFILEHEADER lpfh = (LPBITMAPFILEHEADER)lpFileHeader;
		if (lpfh->bfType != ((WORD) 'B' | ('M' << 8)))
			AfxThrowMemoryException();

		// Read in the BITMAPINFO struct
		// size equals BITMAPINFOHEADER plus difference between
		// the bfOffBits and the header.  SO, sounds like you
		// should allocate a bfOffBits
		DWORD dwSize = lpfh->bfOffBits - sizeof(BITMAPFILEHEADER);
		lpInfo = new BYTE[dwSize];

		if (FAILED(lpStream->Read((void*)lpInfo, dwSize, &ulRead)))
			AfxThrowMemoryException();
		ASSERT(ulRead == dwSize);

		LPBITMAPINFO lpbi = (LPBITMAPINFO)lpInfo;

		// This code only supports 2, 16, and 256 color bitmaps
		if (lpbi->bmiHeader.biBitCount != 1 &&
		    lpbi->bmiHeader.biBitCount != 4 &&
				lpbi->bmiHeader.biBitCount != 8)
			AfxThrowMemoryException();

		if (lpbi->bmiHeader.biClrUsed != 0)
			nColors = lpbi->bmiHeader.biClrUsed;
		else
			nColors = 1 << lpbi->bmiHeader.biBitCount;

		MergePalette(lpbi->bmiColors, nColors);

		CSize size(lpbi->bmiHeader.biWidth, lpbi->bmiHeader.biHeight);
		m_size = size;
		CreateBitmap(size);
	
		// Read in the actual BITMAP
		dwSize = lpfh->bfSize - lpfh->bfOffBits;
		pBits = new BYTE[dwSize];

		if (FAILED(lpStream->Read((void*)pBits, dwSize, &ulRead)))
			AfxThrowMemoryException();
		ASSERT(ulRead == dwSize);

		// BLOCK: Set the DIB sections bits from the DIB we are loading
		{
			CWindowDC dc(NULL);
			VERIFY( SetDIBits( dc.m_hDC, (HBITMAP)m_Bitmap.m_hObject, 0, 
				lpbi->bmiHeader.biHeight, pBits, lpbi, DIB_RGB_COLORS ) 
				== lpbi->bmiHeader.biHeight );
		}

		delete lpInfo;
		delete lpFileHeader;
		delete pBits;
		lpInfo = NULL;
		lpFileHeader = NULL;
		pBits = NULL;
	}
	CATCH(CMemoryException, e)
	{
		if (lpInfo != NULL)
		{
			delete lpInfo;
			lpInfo = NULL;
		}

		if (lpFileHeader != NULL)
		{
			delete lpFileHeader;
			lpFileHeader = NULL;
		}

		if (pBits != NULL)
		{
			delete pBits;
			pBits = NULL;
		}

		return FALSE;
	}
	END_CATCH
}


///////////////////////////////////////////////////////////////////////////
// Implementation Functions

void CStreamDib::MergePalette(RGBQUAD* pColors, int nColors)
{
	CMap<DWORD, DWORD, BOOL, BOOL> PaletteMap;
	DWORD dword;
	BOOL bScrap;
	int i;
	POSITION pos;

	memcpy( m_rgbPalette, rgbStd256, sizeof(RGBQUAD) * 256 );

	for(i=0; i<nColors; i++)
	{
		PaletteMap.SetAt(MAKEDWORD(pColors[i]),TRUE);
	}

	i = 0;
	while (PaletteMap.GetCount() < 256)
		PaletteMap.SetAt(MAKEDWORD(m_rgbPalette[i++]),FALSE);
		 

	for(i=0,pos=PaletteMap.GetStartPosition(); pos!=NULL; i++)
	{
		PaletteMap.GetNextAssoc(pos,dword,bScrap);			
		m_rgbPalette[i]=MAKERGBQUAD(dword);
	}
}


void CStreamDib::CreateBitmap(CSize& size)
{
	BYTE*			pBits;

	if (m_Bitmap.m_hObject != NULL)
		m_Bitmap.DeleteObject();

	ASSERT(m_lpbi == NULL);

	// Fill in the BITMAPINFOHEADER
	m_lpbi = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER) 
		+ (256 * sizeof(RGBQUAD))];
	m_lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_lpbi->bmiHeader.biWidth = size.cx;
	m_lpbi->bmiHeader.biHeight = size.cy;
	m_lpbi->bmiHeader.biPlanes = 1;
	m_lpbi->bmiHeader.biBitCount = 8;
	m_lpbi->bmiHeader.biCompression = BI_RGB;
	m_lpbi->bmiHeader.biSizeImage = WIDTHBYTES((DWORD)size.cx * 8) * size.cy;
	m_lpbi->bmiHeader.biXPelsPerMeter = 0;
	m_lpbi->bmiHeader.biYPelsPerMeter = 0;
	m_lpbi->bmiHeader.biClrUsed = 0;
	m_lpbi->bmiHeader.biClrImportant = 0;

	// Fill in the color table
	UINT uUsage = DIB_RGB_COLORS;

	memcpy( m_lpbi->bmiColors, m_rgbPalette, sizeof(RGBQUAD) * 256 );

	CWindowDC dc(NULL);
	HBITMAP hBitmap = CreateDIBSection( dc.m_hDC, m_lpbi, uUsage, 
		(VOID**)&pBits, NULL, 0 );
	
	m_Bitmap.Attach(hBitmap);

	delete [] (BYTE *)m_lpbi;
	m_lpbi = NULL;
}


