// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
// PURPOSE:
//    Contains declarations for the brush style dialog.
//

//
// Because the Windows NT headers don't include the GDI symbols when calls
// from RC, this makes sure that the BS_SOLID etc values are defined.
//

#ifndef BS_SOLID

/* Brush Styles */
#define BS_SOLID            0
#define BS_NULL             1
#define BS_HOLLOW           BS_NULL
#define BS_HATCHED          2
#define BS_PATTERN          3
#define BS_INDEXED          4
#define BS_DIBPATTERN       5
#define BS_DIBPATTERNPT     6
#define BS_PATTERN8X8       7
#define BS_DIBPATTERN8X8    8

/* Hatch Styles */
#define HS_HORIZONTAL       0       /* ----- */
#define HS_VERTICAL         1       /* ||||| */
#define HS_FDIAGONAL        2       /* \\\\\ */
#define HS_BDIAGONAL        3       /* ///// */
#define HS_CROSS            4       /* +++++ */
#define HS_DIAGCROSS        5       /* xxxxx */

#endif // ifndef BS_SOLID

//-------------------------------------------------------------------------
// Controls ID's for the Brush dialogs

#define IDD_BRUSHEXAMPLE            201
#define IDD_BRUSHCOLOR              202

#define IDD_BRUSHSTYLE              230
#define IDD_SOLIDBRUSH              (IDD_BRUSHSTYLE + BS_SOLID)
#define IDD_NULLBRUSH               (IDD_BRUSHSTYLE + BS_NULL)
#define IDD_HATCHBRUSH              (IDD_BRUSHSTYLE + BS_HATCHED)
#define IDD_BRUSHFIRST              IDD_SOLIDBRUSH
#define IDD_BRUSHLAST               IDD_HATCHBRUSH

#define IDD_HATCHSTYLE              260
#define IDD_HORIZONTAL              (IDD_HATCHSTYLE + HS_HORIZONTAL)
#define IDD_VERTICAL                (IDD_HATCHSTYLE + HS_VERTICAL)
#define IDD_FDIAGONAL               (IDD_HATCHSTYLE + HS_FDIAGONAL)
#define IDD_BDIAGONAL               (IDD_HATCHSTYLE + HS_BDIAGONAL)
#define IDD_CROSS                   (IDD_HATCHSTYLE + HS_CROSS)
#define IDD_DIAGCROSS               (IDD_HATCHSTYLE + HS_DIAGCROSS)
#define IDD_HATCHFIRST              IDD_HORIZONTAL
#define IDD_HATCHLAST               IDD_DIAGCROSS

// Prototypes for the Brush dialog procedure
LRESULT CALLBACK BrushDlg(HWND, UINT, WPARAM, LPARAM);
