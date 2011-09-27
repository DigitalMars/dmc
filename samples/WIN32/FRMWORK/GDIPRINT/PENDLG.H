// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
// PURPOSE:
//    Contains declarations for the pen style dialog.
//

//
// Because the Windows NT headers don't include the GDI symbols when calls
// from RC, this makes sure that the PS_SOLID etc values are defined.
//

#ifndef PS_SOLID

/* Pen Styles */
#define PS_SOLID            0
#define PS_DASH             1       /* -------  */
#define PS_DOT              2       /* .......  */
#define PS_DASHDOT          3       /* _._._._  */
#define PS_DASHDOTDOT       4       /* _.._.._  */
#define PS_NULL             5
#define PS_INSIDEFRAME      6
#define PS_USERSTYLE        7
#define PS_ALTERNATE        8

#endif // ifndef PS_SOLID

//-------------------------------------------------------------------------
// Control ID's for the Pen dialog

#define IDD_PENWIDTH                301
#define IDD_PENEXAMPLE              302
#define IDD_PENCOLOR                303

#define IDD_PENSTYLE                350
#define IDD_SOLIDPEN                (IDD_PENSTYLE + PS_SOLID)
#define IDD_DASH                    (IDD_PENSTYLE + PS_DASH)
#define IDD_DOT                     (IDD_PENSTYLE + PS_DOT)
#define IDD_DASHDOT                 (IDD_PENSTYLE + PS_DASHDOT)
#define IDD_DASHDOTDOT              (IDD_PENSTYLE + PS_DASHDOTDOT)
#define IDD_NULLPEN                 (IDD_PENSTYLE + PS_NULL)
#define IDD_INSIDEFRAME             (IDD_PENSTYLE + PS_INSIDEFRAME)
#define IDD_PENFIRST                IDD_SOLIDPEN
#define IDD_PENLAST                 IDD_INSIDEFRAME

// Prototype for the Pen dialog procedure
LRESULT CALLBACK PenDlg(HWND, UINT, WPARAM, LPARAM);
