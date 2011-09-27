// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
// MODULE: miscutil.h
//
// PURPOSE:
//    Contains declarations misc. functions
//

#ifndef _MISCUTIL_H
#define _MISCUTIL_H

//-------------------------------------------------------------
// misc prototypes

BOOL CenterWindow(HWND hwndChild, HWND hwndParent);
HBITMAP SetBitmapBkgnd( HINSTANCE hinst, LPCTSTR szName, COLORREF rgbBkgnd );
HFONT CreateStdFont (HWND hwnd, int heightPoints);

#endif

