/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1992 - 1995  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include "dib.h"


#define  SEEK_CUR 1             /* flags for _lseek */
#define  SEEK_SET 0             /* flags for _lseek */
#define  BFT_BITMAP 0x4d42      /* 'BM' */

/*----------------------------------------------------------------------------*\
|   Macros                                                                     |
\*----------------------------------------------------------------------------*/

#define ISDIB(bft)              ((bft) == BFT_BITMAP)

#define WIDTHBYTES(i)           ((unsigned)((i+31)&(~31))/8)  /* ULONG aligned ! */

#define DibWidthBytesN(lpbi, n) (UINT)WIDTHBYTES((UINT)(lpbi)->biWidth * (UINT)(n))

#define DibWidthBytes(lpbi)     DibWidthBytesN(lpbi, (lpbi)->biBitCount)

#define DibSize(lpbi)           ((lpbi)->biSize + (lpbi)->biSizeImage + (int)(lpbi)->biClrUsed * sizeof(RGBQUAD))

#define DibSizeImage(lpbi)      ((lpbi)->biSizeImage == 0 \
                                        ? ((DWORD)(UINT)DibWidthBytes(lpbi) * (DWORD)(UINT)(lpbi)->biHeight) \
                                        : (lpbi)->biSizeImage)

#define DibNumColors(lpbi)      ((lpbi)->biClrUsed == 0 && (lpbi)->biBitCount <= 8 \
                                        ? (WORD)(1 << (int)(lpbi)->biBitCount)          \
                                        : (WORD)(lpbi)->biClrUsed)
typedef  RGBTRIPLE * LPRGBTRIPLE;

//----------------------------------------------------------------------------

static HDIB ReadDibBitmapInfo(int);

//----------------------------------------------------------------------------
/*
 *   Open a DIB file and return a MEMORY DIB, a memory handle containing..
 *
 *   BITMAP INFO    bi
 *   palette data
 *   bits....
 *
 */
HDIB OpenDIB(
LPSTR szFile)
{
        int                     fh;
        LPBITMAPINFOHEADER  lpbi;
        DWORD           dwLen;
        HDIB                hdib;
        HANDLE              h;
        OFSTRUCT            of;

        if (HIWORD((DWORD)szFile) == 0) {
                fh = LOWORD((DWORD)szFile);
        } else {
                fh = OpenFile(szFile, &of, OF_READ);
        }

        if (fh == -1)
                return NULL;

        hdib = ReadDibBitmapInfo(fh);

        if (!hdib)
                goto error;

        lpbi = (LPVOID)GlobalLock(hdib);

        //How much memory do we need to hold the DIB
        dwLen  = DibSize(lpbi);

        //Can we get more memory?
        GlobalUnlock(hdib);

        h = GlobalReAlloc(hdib,dwLen,0);

        if (!h) {
                GlobalFree(hdib);
                hdib = NULL;
        } else {
                hdib = h;
        }

        if (hdib) {
                lpbi = (LPVOID)GlobalLock(hdib);

                /* read in the bits */
                _hread(fh, DibPtr(lpbi), lpbi->biSizeImage);
        }

error:
        if (HIWORD((DWORD)szFile) != 0)
                _lclose(fh);

        return hdib;
}
//----------------------------------------------------------------------------
/*
 *   Write a global handle in CF_DIB format to a file.
 *
 */
BOOL WriteDIB(
LPSTR szFile,
HDIB hdib)
{
        BITMAPFILEHEADER        hdr;
        LPBITMAPINFOHEADER  lpbi;
        int                 fh;
        OFSTRUCT            of;

        if (!hdib)
                return FALSE;

        if (HIWORD((DWORD)szFile) == 0) {
                fh = LOWORD((DWORD)szFile);
        } else {
                fh = OpenFile(szFile,&of,OF_CREATE|OF_READWRITE);
        }

        if (fh == -1)
                return FALSE;

        lpbi = (LPVOID)GlobalLock(hdib);

        hdr.bfType              = BFT_BITMAP;
        hdr.bfSize          = DibSize(lpbi) + sizeof(BITMAPFILEHEADER);
        hdr.bfReserved1     = 0;
        hdr.bfReserved2     = 0;
        hdr.bfOffBits       = (DWORD)sizeof(BITMAPFILEHEADER) +
                          lpbi->biSize + lpbi->biClrUsed * sizeof(RGBQUAD);

        _lwrite(fh,(LPVOID)&hdr,sizeof(BITMAPFILEHEADER));
        _hwrite(fh,(LPVOID)lpbi,DibSize(lpbi));

        GlobalUnlock(hdib);

        if (HIWORD((DWORD)szFile) != 0)
                _lclose(fh);

        return TRUE;
}
//----------------------------------------------------------------------------
/*
 *  DibFromBitmap()
 *
 *  Will create a global memory block in DIB format that represents the DDB
 *  passed in
 *
 */
HDIB DibFromBitmap(
HBITMAP hbm,
DWORD biStyle,
WORD biBits,
HPALETTE hpal,
WORD wUsage)
{
        BITMAP  bm;
        BITMAPINFOHEADER        bi;
        LPBITMAPINFOHEADER      lpbi;
        DWORD                   dwLen;
        WORD                    nColors; // GetObject will only return 2 bytes
        HDIB                    hdib;
        HANDLE                  h;
        HDC                     hdc;
        WORD                    wBits;

        if (!hbm)
                return NULL;

        if (hpal == NULL)
                hpal = GetStockObject(DEFAULT_PALETTE);

        GetObject(hbm,sizeof(bm),(LPSTR)&bm);
        GetObject(hpal,sizeof(nColors),(LPSTR)&nColors);

        if (biBits == 0)
                wBits = (WORD)(bm.bmPlanes * bm.bmBitsPixel);
        else
                wBits = biBits;

        bi.biSize               = sizeof(BITMAPINFOHEADER);
        bi.biWidth              = bm.bmWidth;
        bi.biHeight             = bm.bmHeight;
        bi.biPlanes             = 1;
        bi.biBitCount           = wBits;
        bi.biCompression        = biStyle;
        bi.biSizeImage          = 0;
        bi.biXPelsPerMeter      = 0;
        bi.biYPelsPerMeter      = 0;
        bi.biClrUsed            = 0;
        bi.biClrImportant       = 0;

        dwLen  = bi.biSize + DibNumColors(&bi) * sizeof(RGBQUAD);

        hdc = CreateCompatibleDC(NULL);
        hpal = SelectPalette(hdc,hpal,FALSE);
        RealizePalette(hdc);  // why is this needed on a MEMORY DC? GDI bug??

        hdib = GlobalAlloc(GMEM_MOVEABLE,dwLen);

        if (!hdib)
                goto exit;

        lpbi = (LPVOID)GlobalLock(hdib);

        *lpbi = bi;

        //call GetDIBits with a NULL lpBits param, so it will calculate the
        //biSizeImage field for us
        GetDIBits(hdc, hbm, 0, (WORD)bi.biHeight,
                NULL, (LPBITMAPINFO)lpbi, wUsage);

        bi = *lpbi;

        //If the driver did not fill in the biSizeImage field, fill it in
        //based on the width, height and bit depth.
        if (bi.biSizeImage == 0) {
                bi.biSizeImage = (DWORD)WIDTHBYTES(bm.bmWidth * wBits) * bm.bmHeight;

                if (biStyle != BI_RGB)
                        bi.biSizeImage = (bi.biSizeImage * 3) / 2;
        }

        //realloc the buffer big enough to hold all the bits
        dwLen = bi.biSize + DibNumColors(&bi) * sizeof(RGBQUAD) + bi.biSizeImage;

        if (h = GlobalReAlloc(hdib,dwLen,0)) {
                hdib = h;
        } else {
                GlobalFree(hdib);
                hdib = NULL;
                goto exit;
        }

        //call GetDIBits with a NON-NULL lpBits param, and actualy get the
        //bits this time
        lpbi = (LPVOID)GlobalLock(hdib);

        GetDIBits(hdc, hbm, 0, (WORD)bi.biHeight,
        DibPtr(lpbi),(LPBITMAPINFO)lpbi, wUsage);

        bi = *lpbi;
        lpbi->biClrUsed = DibNumColors(lpbi);

exit:
        SelectPalette(hdc,hpal,FALSE);
        DeleteDC(hdc);
        return hdib;
}
//----------------------------------------------------------------------------
/*
 *  ReadDibBitmapInfo()
 *
 *  Will read a file in DIB format and return a global HANDLE to it's
 *  BITMAPINFO.  This function will work with both "old" and "new"
 *  bitmap formats, but will allways return a "new" BITMAPINFO
 *
 */
HDIB ReadDibBitmapInfo(
int fh)
{
        DWORD                   off;
        HDIB                    hbi = NULL;
        int                     size;
        int                     i;
        WORD                    wNumColors;
        LPRGBQUAD               lpRgb;
        BITMAPINFOHEADER        bi;
        BITMAPCOREHEADER        bc;
        LPBITMAPINFOHEADER      lpbi;
        BITMAPFILEHEADER        bf;

        if (fh == -1)
                return NULL;

        off = _llseek(fh,0L,SEEK_CUR);

        if (sizeof(bf) != _lread(fh,(LPVOID)&bf,sizeof(bf)))
                return NULL;

        //do we have a RC HEADER?
        if (!ISDIB(bf.bfType)) {
                bf.bfOffBits = 0L;
                _llseek(fh,off,SEEK_SET);
        }

        if (sizeof(bi) != _lread(fh,(LPVOID)&bi,sizeof(bi)))
                return NULL;

        wNumColors = (WORD) DibNumColors(&bi);

        //what type of bitmap info is this?
        switch (size = (int)bi.biSize) {

                case sizeof(BITMAPCOREHEADER):
                        bc = *(BITMAPCOREHEADER*)&bi;
                        bi.biSize               = sizeof(BITMAPINFOHEADER);
                        bi.biWidth              = (DWORD)bc.bcWidth;
                        bi.biHeight             = (DWORD)bc.bcHeight;
                        bi.biPlanes             =  (WORD)bc.bcPlanes;
                        bi.biBitCount           =  (WORD)bc.bcBitCount;
                        bi.biCompression        = BI_RGB;
                        bi.biSizeImage          = 0;
                        bi.biXPelsPerMeter      = 0;
                        bi.biYPelsPerMeter      = 0;
                        bi.biClrUsed            = (DWORD)(1 << bc.bcBitCount);
                        bi.biClrImportant       = (DWORD)(1 << bc.bcBitCount);

                        _llseek(fh,(LONG)sizeof(BITMAPCOREHEADER)-sizeof(BITMAPINFOHEADER),SEEK_CUR);

                        break;

                case sizeof(BITMAPINFOHEADER):
                default:
                        break;

        }

        //fill in some default values!
        if (bi.biSizeImage == 0)
                bi.biSizeImage = DibSizeImage(&bi);

        if (bi.biClrUsed == 0)
                bi.biClrUsed = DibNumColors(&bi);

                hbi = GlobalAlloc( GMEM_MOVEABLE, (LONG)bi.biSize + ((int)wNumColors * sizeof(RGBQUAD)) );

        if (!hbi)
                return NULL;

        lpbi = (LPVOID)GlobalLock(hbi);
        *lpbi = bi;

        if (lpbi->biSize > sizeof(bi)) {
                if (_lread(fh, (LPBYTE) lpbi + sizeof(bi), (UINT)lpbi->biSize - sizeof(bi))
                                        != (lpbi->biSize - sizeof(bi))) {
                        GlobalFree(hbi);
                        return NULL;
                }
        }

        lpRgb = DibColors(lpbi);

        if (wNumColors) {
                if (size == sizeof(BITMAPCOREHEADER)) {

                        //convert a old color table (3 byte entries) to a new
                        //color table (4 byte entries)
                        _lread(fh,(LPSTR)lpRgb, ((int)wNumColors * sizeof(RGBTRIPLE)) );

                        for (i=(int)wNumColors-1; i>=0; i--) {
                                RGBQUAD rgb;

                                rgb.rgbRed      = ((LPRGBTRIPLE)lpRgb)[i].rgbtRed;
                                rgb.rgbBlue     = ((LPRGBTRIPLE)lpRgb)[i].rgbtBlue;
                                rgb.rgbGreen    = ((LPRGBTRIPLE)lpRgb)[i].rgbtGreen;
                                rgb.rgbReserved = (BYTE)0;

                                lpRgb[i] = rgb;
                        }
                } else {
                        _lread( fh, (LPSTR)lpRgb, ((int)wNumColors * sizeof(RGBQUAD)) );
                }
        }

        if (bf.bfOffBits != 0L)
                _llseek(fh,off + bf.bfOffBits,SEEK_SET);

        GlobalUnlock(hbi);

        return hbi;
}
