//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       bitmaps.cxx
//
//  Contents:   bitmap helper functions
//
//  Classes:
//
//  Functions:  DDBChangeColor
//              LoadAndStretch
//
//  History:    6-24-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include <windows.h>
#include "bitmaps.h"

//+---------------------------------------------------------------------------
//
//  Function:   LoadAndStretch
//
//  Synopsis:   Loads a bitmap from our resources, and creates a bitmap
//              "array."  The red portions of the bitmap are considered
//              clear.  Each resource consists of two bitmaps side by
//              side:  an "open folder" and a "closed folder".  These
//              bitmaps are turned into 4 bitmaps:  two selected, and
//              two unselected bitmaps.
//
//  Arguments:  [hinst]        - instance containing the bitmap
//              [hbmpDest]     - destination bitmap array
//              [lpszResource] - name of the bitmap resource
//              [cxBitmap]     - width to stretch to
//              [cyBitmap]     - height to stretch to
//              [crHigh]       - color of the highlighted background
//              [crNorm]       - color of the normal background
//
//  Returns:    TRUE on success.
//              FALSE on failure.
//
//  History:    6-24-94   stevebl   Stolen and modified from original DFView
//
//----------------------------------------------------------------------------

BOOL LoadAndStretch (
    HINSTANCE hinst,
    HBITMAP hbmpDest,
    LPTSTR lpszResource,
    UINT cxBitmap,
    UINT cyBitmap,
    COLORREF crHigh,
    COLORREF crNorm)
{
    HBITMAP hbmpOld1, hbmpOld2, hbmp;
    BITMAP bm;
    int i;
    HDC hdcDest = CreateCompatibleDC(NULL);
    HDC hdcSrc = CreateCompatibleDC(NULL);

    if (NULL == hdcDest || NULL == hdcSrc)
    {
        DeleteDC(hdcSrc);
        DeleteDC(hdcDest);
        return(FALSE);
    }
    for (i=0;  i < 2;  i++)
    {
        hbmp = LoadBitmap(hinst, lpszResource);
        if (NULL == hbmp)
        {
            DeleteDC(hdcSrc);
            DeleteDC(hdcDest);
            return FALSE;
        }

        DDBChangeColor(hbmp, RGB (255,0,0), ((i == 0) ? crHigh : crNorm));
        GetObject(hbmp, sizeof (bm), &bm);

        hbmpOld1 = (HBITMAP) SelectObject(hdcDest, hbmpDest);
        hbmpOld2 = (HBITMAP) SelectObject(hdcSrc, hbmp);

        SetStretchBltMode(hdcDest, COLORONCOLOR);
        StretchBlt(
            hdcDest,
            i * cxBitmap * 2,
            0,
            cxBitmap * 2,
            cyBitmap,
            hdcSrc,
            0,
            0,
            bm.bmWidth,
            bm.bmHeight,
            SRCCOPY);

        SelectObject(hdcDest, hbmpOld1);
        SelectObject(hdcSrc,  hbmpOld2);
        DeleteObject(hbmp);
    }

    DeleteDC(hdcSrc);
    DeleteDC(hdcDest);
    return TRUE;
}



//+---------------------------------------------------------------------------
//
//  Function:   DDBChangeColor
//
//  Synopsis:   Change a particular color in a bitmap to another color.
//              Strategy is to create a monochrome mask, where each pixel
//              in the source bitmap that matches the color we're converting
//              from is set to white (1), and all other colors to black.
//              We then Blt this mask into the original bitmap, with a ROP
//              code that does:
//
//                (~Mask&Source) | (~Mask&Pattern)
//
//              where Pattern is the color we're changing to.
//
//              In other words, wherever the Mask is 1, we want to put the pattern;
//              wherever the Mask is 0, we want to leave the source alone.  By
//              using a Truth Table, you'll find that this ROP code is equivalent
//              to DSPDxax or ROP code 0x00E20746.
//
//              For info on figuring out ROP codes given a set of boolean ops,
//              check out the Windows 3.0 SDK, Reference Volume 2, chapter 11.
//
//  Arguments:  [hBitmap] - bitmap handle
//              [crFrom]  - color to change
//              [crTo]    - new color
//
//  Returns:    TRUE on success.  FALSE on failur
//
//  History:    6-24-94   stevebl   Stolen from original DFView code
//
//----------------------------------------------------------------------------

BOOL DDBChangeColor (HBITMAP hBitmap, COLORREF crFrom, COLORREF crTo)
{
    register int cx, cy;
    BITMAP       bm;
    HDC          hdcBmp, hdcMask;
    HBITMAP      hbmMask, hbmOld1, hbmOld2;
    HBRUSH       hBrush, hbrOld;

    if (!hBitmap)
          return FALSE;

    GetObject (hBitmap, sizeof (bm), &bm);
    cx = bm.bmWidth;
    cy = bm.bmHeight;

    hbmMask = CreateBitmap(cx, cy, 1, 1, NULL);
    hdcMask = CreateCompatibleDC(NULL);
    hdcBmp = CreateCompatibleDC(NULL);
    hBrush = CreateSolidBrush(crTo);

    if (!hdcMask || !hdcBmp || !hBrush || !hbmMask)
    {
        DeleteObject(hbmMask);
        DeleteObject(hBrush);
        DeleteDC(hdcMask);
        DeleteDC(hdcBmp);
        return FALSE;
    }

    hbmOld1 = (HBITMAP) SelectObject (hdcBmp,  hBitmap);
    hbmOld2 = (HBITMAP) SelectObject (hdcMask, hbmMask);
    hbrOld  = (HBRUSH) SelectObject (hdcBmp, hBrush);

    SetBkColor(hdcBmp, crFrom);
    BitBlt(hdcMask, 0, 0, cx, cy, hdcBmp,  0, 0, SRCCOPY);
    SetBkColor(hdcBmp, RGB(255,255,255));
    BitBlt(hdcBmp,  0, 0, cx, cy, hdcMask, 0, 0, DSPDxax);

    SelectObject(hdcBmp,  hbrOld);
    SelectObject(hdcBmp,  hbmOld1);
    SelectObject(hdcMask, hbmOld2);
    DeleteDC(hdcBmp);
    DeleteDC(hdcMask);
    DeleteObject(hBrush);
    DeleteObject(hbmMask);

    return TRUE;
}


