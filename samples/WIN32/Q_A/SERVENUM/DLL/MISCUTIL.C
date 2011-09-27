// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   miscutil.c
//
//  PURPOSE:  Contains miscutil helper functions
//
//  FUNCTIONS:
//    CenterWindow - Center one window over another.
//    SetBitmapBkgnd - Sets the background color of a bitmap
//    CreateStdFont - Creates a SansSerif font by pointsize
//
//  AUTHOR: Craig Link - Microsoft Developer Support
//

#include <windows.h>            // required for all Windows applications
#include <tchar.h>
#include "miscutil.h"
#include "mem.h"

//
//  FUNCTION: CenterWindow(HWND, HWND)
//
//  PURPOSE:  Center one window over another.
//
//  PARAMETERS:
//    hwndChild - The handle of the window to be centered.
//    hwndParent- The handle of the window to center on.
//
//  RETURN VALUE:
//
//    TRUE  - Success
//    FALSE - Failure
//
//  COMMENTS:
//
//    Dialog boxes take on the screen position that they were designed
//    at, which is not always appropriate. Centering the dialog over a
//    particular window usually results in a better position.
//

BOOL CenterWindow(HWND hwndChild, HWND hwndParent)
{
    RECT    rcChild, rcParent;
    int     cxChild, cyChild, cxParent, cyParent;
    int     cxScreen, cyScreen, xNew, yNew;
    HDC     hdc;

    // Get the Height and Width of the child window
    GetWindowRect(hwndChild, &rcChild);
    cxChild = rcChild.right - rcChild.left;
    cyChild = rcChild.bottom - rcChild.top;

    // Get the Height and Width of the parent window
    GetWindowRect(hwndParent, &rcParent);
    cxParent = rcParent.right - rcParent.left;
    cyParent = rcParent.bottom - rcParent.top;

    // Get the display limits
    hdc = GetDC(hwndChild);
    cxScreen = GetDeviceCaps(hdc, HORZRES);
    cyScreen = GetDeviceCaps(hdc, VERTRES);
    ReleaseDC(hwndChild, hdc);

    // Calculate new X position, then adjust for screen
    xNew = rcParent.left + ((cxParent - cxChild) / 2);
    if (xNew < 0)
    {
        xNew = 0;
    }
    else if ((xNew + cxChild) > cxScreen)
    {
        xNew = cxScreen - cxChild;
    }

    // Calculate new Y position, then adjust for screen
    yNew = rcParent.top  + ((cyParent - cyChild) / 2);
    if (yNew < 0)
    {
        yNew = 0;
    }
    else if ((yNew + cyChild) > cyScreen)
    {
        yNew = cyScreen - cyChild;
    }

    // Set it, and return
    return SetWindowPos(hwndChild,
                        NULL,
                        xNew, yNew,
                        0, 0,
                        SWP_NOSIZE | SWP_NOZORDER);
}


//
//  FUNCTION: SetBitmapBkgnd( HINSTANCE hinst, LPCTSTR szName, COLORREF rgbBkgnd )
//
//  PURPOSE:  Sets the background color of a bitmap
//
//  PARAMETERS:
//    hinst - module containing bitmap data
//    szName - Name of bitmap in resource
//    rgbBkgnd - color to set the background
//
//  RETURN VALUE:
//    Non-NULL - Handle to new bitmap
//    NULL - Failure
//
//  COMMENTS:
//    The bitmap loaded in from the resource has a GREEN background
//    The background color is changed by modifing the bmp's palette
//    entry.  szName can be a value return by MAKEINTRESOURCE

HBITMAP SetBitmapBkgnd( HINSTANCE hinst, LPCTSTR szName, COLORREF rgbBkgnd )
{
    LPBYTE              lpb;
    HBITMAP             hbmp;
    LPBITMAPINFOHEADER  lpbi;
    LPBITMAPINFOHEADER  lpbiTemp;
    HANDLE              h;
    HDC                 hdc;
    LPDWORD             lprgb;
    DWORD               cbClrTable;

#define RGBQ(dw) RGB(GetBValue(dw),GetGValue(dw),GetRValue(dw))

    h = LoadResource (hinst, FindResource(hinst, szName, RT_BITMAP));

    lpbi = (LPBITMAPINFOHEADER) LockResource(h);

    if (!lpbi)
        return NULL;

    if (lpbi->biSize != sizeof(BITMAPINFOHEADER))
        return NULL;

    if (lpbi->biBitCount != 4)
        return NULL;


    cbClrTable = ((int)lpbi->biClrUsed ?
                  (int)lpbi->biClrUsed :
                  (1 << (int)lpbi->biBitCount)) * sizeof(RGBQUAD);

    if ( ERROR_SUCCESS != MallocMem( &lpbiTemp, sizeof(BITMAPINFOHEADER) + cbClrTable + lpbi->biSizeImage ) )
        return NULL;

    memcpy( lpbiTemp, lpbi, sizeof(BITMAPINFOHEADER) + cbClrTable + lpbi->biSizeImage );

    // Calcluate the pointer to the Bits information
    // First skip over the header structure
    lprgb = (LPDWORD)((LPBYTE)(lpbiTemp) + lpbiTemp->biSize);

    // Skip the color table entries, if any
    lpb = (LPBYTE)lprgb + cbClrTable;

    lprgb[10] = RGBQ(rgbBkgnd);         // green

    hdc = GetDC(NULL);

    hbmp = CreateDIBitmap (hdc, lpbiTemp, CBM_INIT, (LPVOID)lpb,
                          (LPBITMAPINFO)lpbiTemp, DIB_RGB_COLORS);

    FreeMem( &lpbiTemp );
    ReleaseDC(NULL, hdc);
    UnlockResource(h);
    FreeResource(h);

    return(hbmp);
}


//
//  FUNCTION: CreateStdFont (HWND hwnd, int heightPoints)
//
//  PURPOSE:  Creates a "MS Sans Serif" font of desired point size
//
//  PARAMETERS:
//    hwnd - window handle for DC
//    heightPoints - point size of desired font
//
//  RETURN VALUE:
//    Non-NULL - Handle to new font
//    NULL - Failure
//
//  COMMENTS:
//

HFONT CreateStdFont (HWND hwnd, int heightPoints)
{
    int heightPix;
    int heightTwips;
    UINT cyPixPerInch;
    UINT cyTwipsPerPixel;
    HDC hdc;
    LOGFONT lf;

    hdc = GetDC(hwnd);
    cyPixPerInch = GetDeviceCaps(hdc, LOGPIXELSY);
    ReleaseDC(hwnd, hdc);

    cyTwipsPerPixel = 1440 / cyPixPerInch;
    heightTwips = heightPoints * 20 + 10;   // 10 is for round-up
    heightPix = heightTwips / cyTwipsPerPixel;

    lf.lfHeight = -heightPix;
    lf.lfWidth = 0;
    lf.lfEscapement = 0;
    lf.lfOrientation = 0;
    lf.lfWeight = FW_THIN;
    lf.lfItalic = 0;
    lf.lfUnderline = 0;
    lf.lfStrikeOut = 0;
    lf.lfCharSet = ANSI_CHARSET;
    lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfPitchAndFamily = FF_SWISS;
    _tcscpy(lf.lfFaceName, TEXT("MS Sans Serif") );

    return CreateFontIndirect(&lf);
}
