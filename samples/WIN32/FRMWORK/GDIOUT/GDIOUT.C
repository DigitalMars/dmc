// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   gdiout.c
//
//  PURPOSE:  Implements the window procedure for the main application
//    window.
//
//  FUNCTIONS:
//    WndProc       - Processes messages for the main window.
//    MsgCommand    - Handle the WM_COMMAND messages for the main window.
//    MsgPaint      - Handles the WM_MESSAGE message by calling 
//                    DrawGDIOutput.
//    MsgGetMinMaxInfo - Sets minimum window size.
//    MsgDestroy    - Handles the WM_DESTROY message by calling 
//                    PostQuitMessage().
//    CmdExit       - Handles the file exit command by calling destory 
//                    window on the main window.
//    DrawGDIOutput - Demonstrates some common and new GDI output APIs.
//
//  COMMENTS:
//    Message dispatch table -
//      For every message to be handled by the main window procedure
//      place the message number and handler function pointer in
//      rgmsd (the message dispatch table).  Place the prototype
//      for the function in globals.h and the definition of the
//      function in the appropriate module.
//    Command dispatch table -
//      For every command to be handled by the main window procedure
//      place the command number and handler function pointer in
//      rgcmd (the command dispatch table).  Place the prototype
//      for the function in globals.h and the definition of the
//      function in the appropriate module.
//    Globals.h Contains the definitions of the structures and dispatch.c
//      contains the functions that use these structures.
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

// constants
#define NUM_POLYPTS 5
#define NUM_BEZPTS  4
#define NUM_COLORS  6

// cell label strings
#ifdef WIN16
char *aszLabel[] = { "SetPixel", "Pen Widths", "Pen Styles", "Polygon",
                     "Polyline", "Arc", "Chord", "Chord",
                     "Pie", "Ellipse", "RoundRect", "Rectangle",
                     "Pie", "Ellipse", "RoundRect", "Rectangle" };
#else
char *aszLabel[] = { "SetPixelV", "Pen Widths", "Pen Styles", "Polygon",
                     "PolyBezier", "Arc", "Chord", "Chord",
                     "Pie", "Ellipse", "RoundRect", "Rectangle",
                     "Pie", "Ellipse", "RoundRect", "Rectangle" };
#endif

// some colors to use for pens and brushes
COLORREF acrColor[] = { RGB(255, 0, 0), RGB(0, 255, 0),
                        RGB(0, 0, 255), RGB(0, 255, 255),
                        RGB(255, 0, 255), RGB(255, 255, 0) };

// pixel array for SetPixelV demo
int aPixel[] = { 2, 2, 2, 2, 2, 2, 2, 2,
                 2, 0, 0, 0, 0, 0, 0, 2,
                 2, 0, 0, 0, 0, 0, 0, 2,
                 2, 0, 0, 2, 2, 0, 0, 2,
                 2, 0, 0, 2, 2, 0, 0, 2,
                 2, 0, 0, 0, 0, 0, 0, 2,
                 2, 0, 0, 0, 0, 0, 0, 2,
                 2, 2, 2, 2, 2, 2, 2, 2 };

// function prototypes
void DrawGDIOutput(HWND, HDC);


// Main window message table definition.
MSD rgmsd[] =
{
    {WM_COMMAND, MsgCommand},
    {WM_DESTROY, MsgDestroy},
    {WM_PAINT, MsgPaint},
    {WM_GETMINMAXINFO, MsgGetMinMaxInfo}
};

MSDI msdiMain =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpWindow
};


// Main window command table definition.
CMD rgcmd[] =
{
    {IDM_EXIT,  CmdExit},
    {IDM_ABOUT, CmdAbout}
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpWindow
};

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispMessage() function with the main window's message dispatch
//    information (msdiMain) and the message specific information.
//

LRESULT CALLBACK WndProc(HWND   hwnd, 
                         UINT   uMessage, 
                         WPARAM wparam, 
                         LPARAM lparam)
{
    return DispMessage(&msdiMain, hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_COMMAND messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - WM_COMMAND (Unused)
//    GET_WM_COMMAND_ID(wparam,lparam)   - Command identifier
//    GET_WM_COMMAND_HWND(wparam,lparam) - Control handle
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispCommand() function with the main window's command dispatch
//    information (cmdiMain) and the command specific information.
//

LRESULT MsgCommand(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&cmdiMain, hwnd, wparam, lparam);
}


//
//  FUNCTION: MsgPaint(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Paints the client area.
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//   
//

LRESULT MsgPaint(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    PAINTSTRUCT ps;

    BeginPaint(hwnd, &ps);

    // do the actual output
    DrawGDIOutput(hwnd, ps.hdc);

    EndPaint(hwnd, &ps);
    
    return 0;
}

//
//  FUNCTION: MsgGetMinMaxInfo(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Sets minimum window size.
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Address of MINMAXINFO structure
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//   
//

LRESULT MsgGetMinMaxInfo(HWND   hwnd,
                         UINT   uMessage,
                         WPARAM wparam,
                         LPARAM lparam)
{
    static int     cxMin=0;
    static int     cyMin=0;
    MINMAXINFO FAR *lpmmi;

    lpmmi = (MINMAXINFO FAR *)lparam;
    if (cxMin == 0 && cyMin == 0)
    {
        HDC        hdc;
        int        i;
        TEXTMETRIC tm;
        SIZE       size;

        
        hdc = GetDC(hwnd);
        
        // find minimum allowable window size by finding widest cell label string
        for (i = 0; i < 16; i ++)
        {
            GetTextExtentPoint(hdc, aszLabel[i], lstrlen(aszLabel[i]), &size);
            cxMin = cyMin = max(cxMin, size.cx);
        }

        // add one-character-wide margin
        GetTextMetrics(hdc, &tm);
        cxMin += 2 * tm.tmAveCharWidth;
        cyMin += 2 * tm.tmAveCharWidth;
        
        ReleaseDC(hwnd, hdc);
    }
    lpmmi->ptMinTrackSize.x = 4 * cxMin;
    lpmmi->ptMinTrackSize.y = 4 * cyMin;

    return 0;
}

//
//  FUNCTION: MsgDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Calls PostQuitMessage().
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    PostQuitMessage(0);
    return 0;
}

//
//  FUNCTION: CmdExit(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_EXIT (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdExit(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    DestroyWindow(hwnd);
    return 0;
}

//
//  FUNCTION: DrawGDIOutput(HWND, HDC)
//
//  PURPOSE: Demonstrates some GDI drawing routines.
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    hdc       - Device context handle
//
//  RETURN VALUE:
//
//    None.
//
//  COMMENTS:
//   
//

void DrawGDIOutput(HWND hwnd, HDC hdc)
{
    RECT       rcClient;        // size of client area
    int        cxCell, cyCell;  // dimensions of each drawing area
    int        nMargin;         // margins for each drawing area
    TEXTMETRIC tm;              // text metrics
    SIZE       size;            // text extents
    int        i, j;            // loop indexes
    int        y;               // vertical line position
    int        nModePrev;       // previous fill mode
    int        nWidth;          // pen width
    int        nLineSpacing;    // vert. spacing between pen style/width lines
    HPEN       hpen, hpenPrev;  // pens
    HBRUSH     hbr, hbrPrev;    // brushes
    HBRUSH     hbrNull;
    POINT      aPolyPts[NUM_POLYPTS];   // Polygon point array
    POINT      aBezPts[NUM_BEZPTS];     // PolyBezier point array

    // for non-filled drawing
    hbrNull = GetStockObject(NULL_BRUSH);

    // get some metrics for doing our drawing
    GetClientRect(hwnd, &rcClient);
    cxCell = rcClient.right / 4;
    cyCell = rcClient.bottom / 4;
    GetTextMetrics(hdc, &tm);
    nMargin = tm.tmHeight + tm.tmExternalLeading;

    // draw cell borders
    for (i = 0; i < 4; i++)
    {
        MoveToEx(hdc, i * cxCell, 0, NULL);
        LineTo(hdc, i * cxCell, rcClient.bottom-1);
        MoveToEx(hdc, 0, i * cyCell, NULL);
        LineTo(hdc, rcClient.right-1, i * cyCell);
    }
    MoveToEx(hdc, rcClient.right-1, 0, NULL);
    LineTo(hdc, rcClient.right-1, rcClient.bottom);
    MoveToEx(hdc, 0, rcClient.bottom-1, NULL);
    LineTo(hdc, rcClient.right, rcClient.bottom-1);

    // label each cell
    nModePrev = SetBkMode(hdc, TRANSPARENT);
    for (i = 0; i < 16; i++)
    {
        GetTextExtentPoint(hdc, aszLabel[i], lstrlen(aszLabel[i]), &size);                             
        TextOut(hdc,
                ((i % 4) * cxCell + (cxCell / 2))-(size.cx / 2),
                (i / 4 + 1) * cyCell - nMargin,
                aszLabel[i],
                lstrlen(aszLabel[i]));
    }
    SetBkMode(hdc, nModePrev);
    
    // SetPixelV
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
#ifdef WIN16
            SetPixel(hdc,
                     nMargin + i * ((cxCell - 2 * nMargin) / 7),
                     nMargin + j * ((cyCell - 2 * nMargin) / 7),
                     acrColor[aPixel[i + j * 8]]);
#else
            SetPixelV(hdc,
                      nMargin + i * ((cxCell - 2 * nMargin) / 7),
                      nMargin + j * ((cyCell - 2 * nMargin) / 7),
                      acrColor[aPixel[i + j * 8]]);
#endif
        }
    }

    // Pen Widths
    nLineSpacing = (cyCell - 2 * nMargin) / 5;
    for (i = 0, nWidth = 1, y = nMargin;
         i < 5;
         i++, nWidth *= 2, y += nLineSpacing)
    {
        hpen     = CreatePen(PS_SOLID, nWidth, acrColor[i]);
        hpenPrev = SelectObject(hdc, hpen);
        MoveToEx(hdc, cxCell + nMargin, y, NULL);
        LineTo(hdc, 2 * cxCell - nMargin, y);
        SelectObject(hdc, hpenPrev);
        DeleteObject(hpen);
    }

    // Pen Styles
    nLineSpacing = (cyCell - 2 * nMargin) / 5;
    nModePrev    = SetBkMode(hdc, TRANSPARENT);
    for (i = 0, y = nMargin; i < 5; i++, y += nLineSpacing)
    {
        // since the pen styles (PS_SOLID, etc.) are zero-based,
        //   we can use the loop index for the pen style
        hpen     = CreatePen(i, 1, acrColor[i]);
        hpenPrev = SelectObject(hdc, hpen);
        MoveToEx(hdc, 2 * cxCell + nMargin, y, NULL);
        LineTo(hdc, 3 * cxCell - nMargin, y);
        SelectObject(hdc, hpenPrev);
        DeleteObject(hpen);
    }
    SetBkMode(hdc, nModePrev);

    // Polygon
    aPolyPts[0].x = 3 * cxCell + cxCell / 2;
    aPolyPts[0].y = nMargin;
    aPolyPts[1].x = 4 * cxCell - nMargin - (cxCell - 2 * nMargin) / 5;
    aPolyPts[1].y = cyCell - nMargin;
    aPolyPts[2].x = 3 * cxCell + nMargin + (cxCell - 2 * nMargin) / 8;
    aPolyPts[2].y = nMargin + (cyCell - 2 * nMargin) / 3;
    aPolyPts[3].x = 4 * cxCell - nMargin - (cxCell - 2 * nMargin) / 8;
    aPolyPts[3].y = nMargin + (cyCell - 2 * nMargin) / 3;
    aPolyPts[4].x = 3 * cxCell + nMargin + (cxCell - 2 * nMargin) / 5;
    aPolyPts[4].y = cyCell - nMargin;
    nModePrev  = SetPolyFillMode(hdc, ALTERNATE);
    hbr        = CreateSolidBrush(acrColor[0]);
    hbrPrev    = SelectObject(hdc, hbr);
    Polygon(hdc, aPolyPts, NUM_POLYPTS);
    SetPolyFillMode(hdc, nModePrev);
    SelectObject(hdc, hbrPrev);
    DeleteObject(hbr);

    // PolyBezier (WIN32) or Polyline (WIN16)
    aBezPts[0].x = nMargin;
    aBezPts[0].y = 2 * cyCell - cyCell / 2;
    aBezPts[1].x = nMargin + 3 * (cxCell - 2 * nMargin) / 4;
    aBezPts[1].y = cyCell + nMargin;
    aBezPts[2].x = nMargin + (cxCell - 2 * nMargin) / 4;
    aBezPts[2].y = 2 * cyCell - nMargin;
    aBezPts[3].x = cxCell - nMargin;
    aBezPts[3].y = 2 * cyCell - cyCell / 2;
#ifdef WIN16
    Polyline(hdc, aBezPts, NUM_BEZPTS);
#else
    PolyBezier(hdc, aBezPts, NUM_BEZPTS);
#endif

    // Arc
    Arc(hdc,
        cxCell + nMargin,
        cyCell + nMargin,
        2 * cxCell - nMargin,
        2 * cyCell - nMargin,
        2 * cxCell,
        2 * cyCell,
        cxCell,
        cyCell);

    // Chord
    hbrPrev = SelectObject(hdc, hbrNull);
    Chord(hdc,
          2 * cxCell + nMargin,
          cyCell + nMargin,
          3 * cxCell - nMargin,
          2 * cyCell - nMargin,
          3 * cxCell,
          2 * cyCell,
          2 * cxCell,
          cyCell);
    SelectObject(hdc, hbrPrev);

    // filled Chord
    nModePrev = SetBkMode(hdc, TRANSPARENT);
    hbr       = CreateHatchBrush(HS_BDIAGONAL, acrColor[1]);
    hbrPrev   = SelectObject(hdc, hbr);
    Chord(hdc,
          3 * cxCell + nMargin,
          cyCell + nMargin,
          4 * cxCell - nMargin,
          2 * cyCell - nMargin,
          4 * cxCell,
          2 * cyCell,
          3 * cxCell,
          cyCell);
    SelectObject(hdc, hbrPrev);
    DeleteObject(hbr);
    SetBkMode(hdc, nModePrev);

    // for the non-filled drawing, we use a NULL brush
    hbrPrev = SelectObject(hdc, hbrNull);    

    // Pie
    Pie(hdc,
        nMargin,
        2 * cyCell + nMargin,
        cxCell - nMargin,
        3 * cyCell - nMargin,
        cxCell,
        2 * cyCell + cyCell / 2,
        0,
        2 * cyCell);

    // Ellipse
    Ellipse(hdc,
            1 * cxCell + nMargin,
            2 * cyCell + nMargin,
            2 * cxCell - nMargin,
            3 * cyCell - nMargin);

    // RoundRect
    RoundRect(hdc,
              2 * cxCell + nMargin,
              2 * cyCell + nMargin,
              3 * cxCell - nMargin,
              3 * cyCell - nMargin,
              2 * nMargin,
              2 * nMargin);

    // Rectangle
    Rectangle(hdc,
              3 * cxCell + nMargin,
              2 * cyCell + nMargin,
              4 * cxCell - nMargin,
              3 * cyCell - nMargin);

    // done with non-filled drawing, so de-select our NULL brush
    SelectObject(hdc, hbrPrev);

    // for hatch-filled drawing, we set the background mode
    //   to TRANSPARENT so the background color shows through
    //   the hatched brush
    nModePrev = SetBkMode(hdc, TRANSPARENT);

    // filled Pie
    hbr     = CreateHatchBrush(HS_CROSS, acrColor[2]);
    hbrPrev = SelectObject(hdc, hbr);
    Pie(hdc,
        nMargin,
        3 * cyCell + nMargin,
        cxCell - nMargin,
        4 * cyCell - nMargin,
        cxCell,
        3 * cyCell + cyCell / 2,
        0,
        3 * cyCell);
    SelectObject(hdc, hbrPrev);
    DeleteObject(hbr);

    // filled Ellipse
    hbr     = CreateHatchBrush(HS_DIAGCROSS, acrColor[3]);
    hbrPrev = SelectObject(hdc, hbr);
    Ellipse(hdc,
            1 * cxCell + nMargin,
            3 * cyCell + nMargin,
            2 * cxCell - nMargin,
            4 * cyCell - nMargin);
    SelectObject(hdc, hbrPrev);
    DeleteObject(hbr);

    // filled Roundrect
    hbr     = CreateHatchBrush(HS_FDIAGONAL, acrColor[4]);
    hbrPrev = SelectObject(hdc, hbr);
    RoundRect(hdc,
              2 * cxCell + nMargin,
              3 * cyCell + nMargin,
              3 * cxCell - nMargin,
              4 * cyCell - nMargin,
              2 * nMargin,
              2 * nMargin);
    SelectObject(hdc, hbrPrev);
    DeleteObject(hbr);

    // filled Rectangle
    hbr     = CreateHatchBrush(HS_VERTICAL, acrColor[5]);
    hbrPrev = SelectObject(hdc, hbr);
    Rectangle(hdc,
              3 * cxCell + nMargin,
              3 * cyCell + nMargin,
              4 * cxCell - nMargin,
              4 * cyCell - nMargin);
    SelectObject(hdc, hbrPrev);
    DeleteObject(hbr);
    
    // done with hatch-filled drawing, so restore background mode
    SetBkMode(hdc, nModePrev);
}
