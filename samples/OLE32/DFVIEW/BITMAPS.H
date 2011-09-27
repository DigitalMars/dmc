//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       bitmaps.h
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

#ifndef __BITMAPS_H__
#define __BITMAPS_H__

#define DSPDxax 0x00E20746L

BOOL DDBChangeColor (
    HBITMAP hBitmap,
    COLORREF crFrom,
    COLORREF crTo);

BOOL LoadAndStretch (
    HINSTANCE hinst,
    HBITMAP hbmpDest,
    LPTSTR lpszResource,
    UINT cxBitmap,
    UINT cyBitmap,
    COLORREF crHigh,
    COLORREF crNorm);

#endif //__BITMAPS_H__
