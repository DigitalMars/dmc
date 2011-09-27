//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       stmdib.cxx
//
//  Contents:   functions for saving and loading DIBs to OLE streams
//
//  Classes:
//
//  Functions:  WriteDibToStream
//              ReadDibFromStream
//              ReadDibBitmapInfoFromStream
//
//  History:    4-22-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include "stmdib.h"
#include "dib.h"
#include <stdlib.h>

//+---------------------------------------------------------------------------
//
//  Function:   WriteDibToStream
//
//  Synopsis:   writes a global handle in CF_DIB format to a stream
//
//  Arguments:  [pstm] - stream
//              [hdib] - DIB handle
//
//  Returns:    S_OK if successful
//
//  History:    4-22-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT WriteDibToStream (IStream * pstm, HANDLE hdib)
{
    BITMAPFILEHEADER    hdr;
    LPBITMAPINFOHEADER  lpbi;
    OFSTRUCT            of;
    HRESULT hr;

    if (!hdib)
        return FALSE;

    lpbi = (LPBITMAPINFOHEADER)GlobalLock (hdib);

    /* Fill in the fields of the file header */
    hdr.bfType      = BFT_BITMAP;
    hdr.bfSize      = GlobalSize (hdib) + sizeof (BITMAPFILEHEADER);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;
    hdr.bfOffBits   = (DWORD)sizeof(BITMAPFILEHEADER) + lpbi->biSize +
        PaletteSize(lpbi);

    /* Write the file header */
    if (FAILED(hr = pstm->Write(&hdr, sizeof (BITMAPFILEHEADER), NULL)))
    {
        return(hr);
    }

    /* Write the DIB header and the bits */
    if (FAILED(hr = pstm->Write(lpbi, GlobalSize (hdib), NULL)))
    {
        return(hr);
    }

    GlobalUnlock (hdib);
    return S_OK;
}

//+---------------------------------------------------------------------------
//
//  Function:   ReadDibFromStream
//
//  Synopsis:   reads a DIB stored in CF_DIB format from a stream
//
//  Arguments:  [pstm]  - pointer to the stream
//              [phDib] - pointer to recieve the DIB handle
//
//  History:    4-22-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT ReadDibFromStream (IStream * pstm, HANDLE * phDib)
{
    BITMAPINFOHEADER    bi;
    LPBITMAPINFOHEADER  lpbi;
    DWORD       dwLen = 0;
    DWORD       dwBits;
    HANDLE      hdib;
    HANDLE      h;
    OFSTRUCT        of;
    HRESULT hr;

    *phDib = NULL;

    if (FAILED(hr = ReadDibBitmapInfoFromStream(pstm, &hdib)))
    {
        return(hr);
    }
    DibInfo(hdib,&bi);

    /* Calculate the memory needed to hold the DIB */
    dwBits = bi.biSizeImage;
    dwLen  = bi.biSize + (DWORD)PaletteSize (&bi) + dwBits;

    /* Try to increase the size of the bitmap info. buffer to hold the DIB */
    h = GlobalReAlloc(hdib, dwLen, GHND);
    if (!h)
    {
        GlobalFree(hdib);
        return(E_OUTOFMEMORY);
    }
    hdib = h;

    /* Read in the bits */

    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);
    if (FAILED(hr = pstm->Read(((LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize(lpbi)), dwBits, NULL)))
    {
        return(hr);
    }
    GlobalUnlock(hdib);

    *phDib = hdib;
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Function:   ReadDibBitmapInfoFromStream
//
//  Synopsis:   reads a file in DIB format and retrieves a global HANDLE to
//              its BITMAPINFO.
//
//  Arguments:  [pstm] - stream
//              [phbi] - pointer to receive HANDLE to BITMAPINFO
//
//  Returns:    S_OK on success
//
//  History:    4-22-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT ReadDibBitmapInfoFromStream(IStream * pstm, HANDLE * phbi)
{
    ULARGE_INTEGER libOff;
    LARGE_INTEGER dlibMove;
    HANDLE    hbi = NULL;
    int       size;
    int       i;
    WORD      nNumColors;

    RGBQUAD FAR       *pRgb;
    BITMAPINFOHEADER   bi;
    BITMAPCOREHEADER   bc;
    LPBITMAPINFOHEADER lpbi;
    BITMAPFILEHEADER   bf;
    DWORD          dwWidth = 0;
    DWORD          dwHeight = 0;
    WORD           wPlanes, wBitCount;
    HRESULT hr;

    *phbi = NULL;

    /* Reset file pointer and read file header */

    dlibMove.HighPart = 0;
    dlibMove.LowPart = 0;
    if (FAILED(hr = pstm->Seek(dlibMove, STREAM_SEEK_CUR, &libOff)))
    {
        return(hr);
    }
    if (FAILED(hr = pstm->Read(&bf, sizeof(bf), NULL)))
    {
        return(hr);
    }

    /* Do we have a RC HEADER? */
    if (!ISDIB (bf.bfType))
    {
        bf.bfOffBits = 0L;
        dlibMove.HighPart = libOff.HighPart;
        dlibMove.LowPart = libOff.LowPart;
        if (FAILED(hr = pstm->Seek(dlibMove, STREAM_SEEK_SET, NULL)))
        {
            return(hr);
        }
    }
    if (FAILED(hr = pstm->Read(&bi, sizeof(bi), NULL)))
    {
        return(hr);
    }

    nNumColors = DibNumColors (&bi);

    /* Check the nature (BITMAPINFO or BITMAPCORE) of the info. block
     * and extract the field information accordingly. If a BITMAPCOREHEADER,
     * transfer it's field information to a BITMAPINFOHEADER-style block
     */
    switch (size = (int)bi.biSize)
    {
    case sizeof (BITMAPINFOHEADER):
        break;

    case sizeof (BITMAPCOREHEADER):
        bc = *(BITMAPCOREHEADER*)&bi;
        dwWidth   = (DWORD)bc.bcWidth;
        dwHeight  = (DWORD)bc.bcHeight;
        wPlanes   = bc.bcPlanes;
        wBitCount = bc.bcBitCount;
        bi.biSize           = sizeof(BITMAPINFOHEADER);
        bi.biWidth          = dwWidth;
        bi.biHeight         = dwHeight;
        bi.biPlanes         = wPlanes;
        bi.biBitCount       = wBitCount;
        bi.biCompression    = BI_RGB;
        bi.biSizeImage      = 0;
        bi.biXPelsPerMeter  = 0;
        bi.biYPelsPerMeter  = 0;
        bi.biClrUsed        = nNumColors;
        bi.biClrImportant   = nNumColors;
        dlibMove.HighPart = 0;
        dlibMove.LowPart = sizeof (BITMAPCOREHEADER) - sizeof (BITMAPINFOHEADER);
        if (FAILED(hr = pstm->Seek(dlibMove, STREAM_SEEK_CUR, NULL)))
        {
            return(hr);
        }
        break;

    default:
        /* Not a DIB! */
        return E_FAIL;
    }

    /*  Fill in some default values if they are zero */
    if (bi.biSizeImage == 0)
    {
        bi.biSizeImage = WIDTHBYTES ((DWORD)bi.biWidth * bi.biBitCount)
            * bi.biHeight;
    }
    if (bi.biClrUsed == 0)
        bi.biClrUsed = DibNumColors(&bi);

    /* Allocate for the BITMAPINFO structure and the color table. */
    hbi = GlobalAlloc (GHND, (LONG)bi.biSize + nNumColors * sizeof(RGBQUAD));
    if (!hbi)
        return NULL;
    lpbi = (LPBITMAPINFOHEADER)GlobalLock (hbi);
    *lpbi = bi;

    /* Get a pointer to the color table */
    pRgb = (RGBQUAD FAR *)((LPSTR)lpbi + bi.biSize);
    if (nNumColors)
    {
        if (size == sizeof(BITMAPCOREHEADER))
        {
            /* Convert a old color table (3 byte RGBTRIPLEs) to a new
             * color table (4 byte RGBQUADs)
             */
             if (FAILED(hr = pstm->Read(pRgb, nNumColors * sizeof(RGBTRIPLE), NULL)))
             {
                 return(hr);
             }

            for (i = nNumColors - 1; i >= 0; i--)
            {
                RGBQUAD rgb;

                rgb.rgbRed  = ((RGBTRIPLE FAR *)pRgb)[i].rgbtRed;
                rgb.rgbBlue = ((RGBTRIPLE FAR *)pRgb)[i].rgbtBlue;
                rgb.rgbGreen    = ((RGBTRIPLE FAR *)pRgb)[i].rgbtGreen;
                rgb.rgbReserved = (BYTE)0;

                pRgb[i] = rgb;
            }
        }
        else
        {
            if (FAILED(hr = pstm->Read(pRgb, nNumColors * sizeof(RGBQUAD), NULL)))
            {
                return(hr);
            }
        }
    }

    if (bf.bfOffBits != 0L)
    {
        dlibMove.HighPart = libOff.HighPart;
        dlibMove.LowPart = libOff.LowPart + bf.bfOffBits;
        if (FAILED(hr = pstm->Seek(dlibMove, STREAM_SEEK_SET, NULL)))
        {
            return(hr);
        }
    }

    GlobalUnlock(hbi);
    *phbi = hbi;
    return(S_OK);
}



