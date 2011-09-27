// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: client.c
//
//  PURPOSE: Handles general routines for the GDI Input sample.
//
//  FUNCTIONS:
//    CreateClientWindow  - Creates the client window that handles input.
//    ClientWndProc       - Processes messages for the client window.
//    CmdDrawMode         - Changes the current drawing mode.
//    CmdFill             - Toggles the fill mode.
//    CmdCreatePen        - Puts up Pen Style dialog box.
//    CmdCreateBrush      - Puts up Brush Style dialog box.  
//    CmdClear            - Forces client window to repaint.
//    MsgClientCreate     - Creates initial pen and brush objects.  
//    InitDrawObjects     - Initializes the pen and brush used for drawing.
//    MsgClientDestroy    - Frees owned objects.
//    MsgClientKeyDown    - Looks for Escape key to cancel drawing.
//
//    MsgClientLButtonDown
//    MsgClientMouseMove => Pass mouse messages to the current handlers.
//    MsgClientLButtonUp
//
//    PixelLBDown
//    PixelMouseMove    ==> Draw individual pixels with SetPixelV.
//    PixelLBUp
//
//    BezierLBDown
//    BezierMouseMove   ==> Select 4 points for drawing a Bezier curve.
//    BezierLBUp
//
//    RectLBDown
//    RectMouseMove     ==> Select 2 points for a line, rect, or ellipse.
//    RectLBUp
//
//    LineDraw              Given 2 points, draw a line, rect, or ellipse.
//    RectDraw          ==> These functions all take identical parameters
//    EllipseDraw           so they can be used interchangeably.
//
//    StartRubberBand     - Initiates drawing procedure.
//    EndRubberBand       - Ends drawing procedure.
//
//    SizeClientWindow    - Creates scrollbars if client can't display entire bitmap.                                                         
//    MsgClientPaint      - Paints the client window in response to WM_PAINT message.
//    MsgClientScroll     - Scrolls the client window in response to scroll message.
//
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include  <commctrl.h>          // prototypes and structure for the common controls.

#include "globals.h"            // prototypes specific to this application
#include "resource.h"
#include "toolbar.h"            // prototypes for the tool bar
#include "statbar.h"            // prototypes for the status bar
#include "pendlg.h"             // ID's and prototypes for the Pen dialog
#include "brushdlg.h"           // ID's and prototypes for the Brush dialog
#include "dibutil.h"

#ifdef SetPixelV  // temporary workaround since SetPixelV isn't working yet
#undef SetPixelV
#endif
#define SetPixelV SetPixel

// Global variables local to this file

HPEN    hpenBlack;              // Useful stock objects
HBRUSH  hbrNull;

HPEN    hpenDraw;               // for drawing lines and frames
HBRUSH  hbrReal;                // for filling interiors
HBRUSH  hbrDraw;                // equals either hbrReal or hbrNull

HDC     hdcRB;                  // DC for Rubber Banding
BOOL    bDrawing;               // Flag to indicate when we are drawing
UINT    uDrawMode;              // IDM_LINE, IDM_RECT, etc.

#define BEZ_MAXPOINTS   4       // # of points in a Bezier curve
POINT   pPoints[BEZ_MAXPOINTS]; // Stores coordinates for various drawing fns
UINT    cPoints;                // Current # of points

// for working with bitmap in memory DC
HDC     hdcMem;
HBITMAP hbmOld;
HPALETTE hpalRB, hpalMem;

// for scrolling
int cxHorzScrollPos = 0;
int cyVertScrollPos = 0;

// The following 3 function pointers are used to point to the current
// mouse handling functons.  Different drawing types can each define a
// set of mouse functions.  When a new drawing mode is selected, these
// pointers are set to point to the correct functions for that mode.
// The main mouse message handlers then dereference these pointers to
// call the correct handlers.

LRESULT  (*pfnLBDown)   (HWND, POINT);      // current mouse handler
LRESULT  (*pfnLBUp)     (HWND, POINT);      // functions for rubber
LRESULT  (*pfnMouseMove)(HWND, POINT);      // banding and such


// The Line, Rectangle, and Ellipse modes use a common set of mouse
// handlers that select 2 points.  The following function pointer is
// used so that the correct drawing function is always used.  Like the
// mouse function pointers, this is set when the drawing mode is
// selected.

BOOL (*pfnDrawRect)(HDC, POINT, POINT);     // current drawing function

// Mouse handling functions for different types of drawing.
LRESULT  PixelLBDown(HWND, POINT);          // Pixels
LRESULT  PixelLBUp(HWND, POINT);
LRESULT  PixelMouseMove(HWND, POINT);

LRESULT  BezierLBDown(HWND, POINT);         // Bezier Curves
LRESULT  BezierLBUp(HWND, POINT);
LRESULT  BezierMouseMove(HWND, POINT);

LRESULT  RectLBDown(HWND, POINT);           // 2-point selection
LRESULT  RectLBUp(HWND, POINT);             // Used for Lines, Rects,
LRESULT  RectMouseMove(HWND, POINT);        // and Ellipses

// Generic drawing functions for use with the above Rect functions
BOOL     LineDraw(HDC, POINT, POINT);
BOOL     RectDraw(HDC, POINT, POINT);
BOOL     EllipseDraw(HDC, POINT, POINT);

// Other helper functions
void StartRubberBand(HWND);
void EndRubberBand(HWND);
UINT BitmapWidth(HBITMAP);
UINT BitmapHeight(HBITMAP);

// Client window message handling functions
static LRESULT MsgClientCreate         (HWND, UINT, WPARAM, LPARAM);
static LRESULT MsgClientDestroy        (HWND, UINT, WPARAM, LPARAM);
static LRESULT MsgClientMouseMove      (HWND, UINT, WPARAM, LPARAM);
static LRESULT MsgClientLButtonDown    (HWND, UINT, WPARAM, LPARAM);
static LRESULT MsgClientLButtonUp      (HWND, UINT, WPARAM, LPARAM);
static LRESULT MsgClientKeyDown        (HWND, UINT, WPARAM, LPARAM);   
static LRESULT MsgClientPaint          (HWND, UINT, WPARAM, LPARAM);
static LRESULT MsgClientScroll         (HWND, UINT, WPARAM, LPARAM);     

// Client window message table definitions.
MSD rgmsdClient[] =
{
    {WM_MOUSEMOVE,       MsgClientMouseMove       },
    {WM_LBUTTONDOWN,     MsgClientLButtonDown     },
    {WM_LBUTTONUP,       MsgClientLButtonUp       },
    {WM_KEYDOWN,         MsgClientKeyDown         },
    {WM_CREATE,          MsgClientCreate          },
    {WM_DESTROY,         MsgClientDestroy         },   
    {WM_PAINT,           MsgClientPaint           },
    {WM_HSCROLL,         MsgClientScroll          },
    {WM_VSCROLL,         MsgClientScroll          }
};


MSDI msdiClient =
{
    sizeof(rgmsdClient) / sizeof(MSD),
    rgmsdClient,
    edwpWindow
};


//
//  FUNCTION: CreateClientWindow(HWND)
//
//  PURPOSE: Create the client window.
//
//  PARAMETERS:
//    hwndParent - The parent (main) window.
//
//  RETURN VALUE:
//    HWND of client window or null if failure.
//
//  COMMENTS:
//

HWND CreateClientWindow(HWND hwndParent)
{
    return CreateWindowEx(0, 
                          "ClientWndClass",
                          NULL,
                          WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
                          -100, -100, 10, 10,   // dummy initial coordinates
                          hwndParent, 
                          (HMENU)-1, 
                          hInst, 
                          NULL);
}


//
//  FUNCTION: ClientWndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the client window.
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
//    Call the DispMessage() function with the client window's message
//    dispatch information (msdiClient) and the message specific information.
//

LRESULT CALLBACK ClientWndProc(HWND   hwnd, 
                               UINT   uMessage, 
                               WPARAM wparam, 
                               LPARAM lparam)
{
    return DispMessage(&msdiClient, hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: SizeClientWindow(HWND)
//
//  PURPOSE: Resizes the client window when the main window's size has changed or
//           a bitmap is created or loaded.  Attempts to keep as much of the bitmap
//	     visible as possible.
//
//  PARAMETERS:
//    hwnd - handle of Client window.
//
//  RETURN VALUE:
//    none.
//
//  COMMENTS:
//

void SizeClientWindow(HWND hwnd)    
{
    RECT rc, rcClient, rcFrame; 
    int  cxRange = 0, cyRange = 0;     // Range needed for horz and vert 
    int cxBitmap, cyBitmap;
    
    if (!hBitmap)
    {
        // no bitmap, so the client window is not needed
        MoveWindow(hWndClient, 0, 0, 0, 0, TRUE);
        return;
    }
                           
    cxBitmap = BitmapWidth(hBitmap);
    cyBitmap = BitmapHeight(hBitmap);
	                           
    rcClient.left = rcClient.top = 0;
   
    // adjust size of client window to bitmap       
    rcClient.right = cxBitmap;
    rcClient.bottom = cyBitmap;     
    
    // get screen coordinates of toolbar window
    GetWindowRect(hWndToolbar, &rc);
    
    // convert rc.right, rc.bottom to client coordinates
    ScreenToClient(hwnd, ((LPPOINT)&rc) + 1);
    
    // adjust origin of Client window to be flush below Toolbar
    rcClient.top = rc.bottom + 1;
 
    // get screen coordinates of statusbar window
    GetWindowRect(hWndStatusbar, &rc);
    
    // convert rc.left, rc.top to client coordinates
    ScreenToClient(hwnd, (LPPOINT)&rc);
                                        
    // get size of frame window
    GetClientRect(hwnd, &rcFrame);
                                            
    // if client window would overlap statusbar, adjust the client window's size to be
    // flush with the top of the statusbar window instead
    if (rc.top < rcClient.bottom + rcClient.top)
    {
        // rcClient.bottom is the height in the MoveWindow call which follows below
        rcClient.bottom = rc.top - rcClient.top;  
      
        // widen the client window to accomodate a vertical scrollbar    
        rcClient.right += GetSystemMetrics(SM_CXVSCROLL);     
        
        // setup vertical scrollbar range
        cyRange = cyBitmap - rcClient.bottom;      
    }
 
    // adjust width of Client window to fit into frame window
    if (rcClient.right > rcFrame.right)   
    {
        // rcClient.right is the width in the MoveWindow call which follows below        
        rcClient.right = rcFrame.right;
      
        // make room for horizontal scrollbar if possible
        rcClient.bottom = min(rcClient.bottom + GetSystemMetrics(SM_CYHSCROLL), 
                              rc.top - rcClient.top);    
        					  
        // setup horizontal scrollbar range
        cxRange = cxBitmap - rcClient.right;
        	
        // recalculate vertical scrollbar range
        cyRange = cyBitmap - rcClient.bottom + GetSystemMetrics(SM_CYHSCROLL);            
        	
        if (cyRange > 0)					
            // make allowance for a vertical scrollbar in the horizontal range
            cxRange += GetSystemMetrics(SM_CXVSCROLL);            	
    }        
   
    // Set the ranges we've calculated (0 to 0 means invisible scrollbar)
    SetScrollRange(hWndClient, SB_VERT, 0, cyRange, TRUE);
    SetScrollRange(hWndClient, SB_HORZ, 0, cxRange, TRUE); 
    
    // reset global variable if a scrollbar is turned off                               
    if (cyRange == 0)
        cyVertScrollPos = 0;                                    
    if (cxRange == 0)
        cxHorzScrollPos = 0;    

    // resize the client window
    MoveWindow(hWndClient,
               rcClient.left,
               rcClient.top,
               rcClient.right,
               rcClient.bottom,
               TRUE);   
           
    InvalidateRect(hWndClient, NULL, FALSE);        
}


//
//  FUNCTION: CmdDrawMode(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Handles WM_COMMAND messages to change drawing mode.
//
//  PARAMETERS:
//    hwnd - The main window.
//    wCommand - IDM_LINE, IDM_RECT, etc.
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
       
LRESULT CmdDrawMode(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{   
    if (wCommand == uDrawMode)  // First see if the mode has changed
        return 0;

     // Update the menu and toolbar states
  
    CheckMenuItem(hMenu, uDrawMode, MF_BYCOMMAND | MF_UNCHECKED);
    uDrawMode = wCommand;
    CheckMenuItem(hMenu, uDrawMode, MF_BYCOMMAND | MF_CHECKED);

    SendMessage(hWndToolbar, TB_CHECKBUTTON, uDrawMode, MAKELONG(TRUE, 0));

    // Set the pointers for the mouse handler functions to
    // the appropriate functions.

    switch(uDrawMode)
    {
        case IDM_PIXEL:
            pfnLBDown     = PixelLBDown;
            pfnLBUp       = PixelLBUp;
            pfnMouseMove  = PixelMouseMove;
            break;

        case IDM_BEZIER:
            cPoints       = 0;
            pfnLBDown     = BezierLBDown;
            pfnLBUp       = BezierLBUp;
            pfnMouseMove  = BezierMouseMove;
            break;

        case IDM_RECT:
            pfnDrawRect   = RectDraw;
            pfnLBDown     = RectLBDown;
            pfnLBUp       = RectLBUp;
            pfnMouseMove  = RectMouseMove;
            break;

        case IDM_ELLIPSE:
            pfnDrawRect   = EllipseDraw;
            pfnLBDown     = RectLBDown;
            pfnLBUp       = RectLBUp;
            pfnMouseMove  = RectMouseMove;
            break;

        default:
        case IDM_LINE:
            pfnDrawRect   = LineDraw;
            pfnLBDown     = RectLBDown;
            pfnLBUp       = RectLBUp;
            pfnMouseMove  = RectMouseMove;
            break;
    }

    return 0;
}


//
//  FUNCTION: CmdFill(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Handles the IDM_FILL and IDM_NOFILL command messages.
//    Controls whether closed objects (rect's and ellipses) are filled
//    with a brush when drawn.
//
//  PARAMETERS:
//    hwnd     - The main window.
//    wCommand - IDM_FILL or IDM_NOFILL
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//    The toolbar has 2 buttons (IDM_FILL and IDM_NOFILL) but the
//    menu only has IDM_FILL, so this function will turn filling
//    OFF if wCommand is IDM_NOFILL, and TOGGLE the ON/OFF state
//    if wCommand is IDM_FILL.
//

LRESULT CmdFill(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    static BOOL bFill;

    if (IDM_NOFILL == wCommand)
        bFill = FALSE;                          // turn filling OFF
    else
        bFill = !bFill;                         // toggle fill state

    // Update the menu and toolbar states

    CheckMenuItem(hMenu,
                  IDM_FILL,
                  MF_BYCOMMAND | (bFill ? MF_CHECKED : MF_UNCHECKED));

    SendMessage(hWndToolbar,
                TB_CHECKBUTTON,
                (bFill ? IDM_FILL : IDM_NOFILL),
                MAKELONG(TRUE, 0));

    // Update hbrDraw
    hbrDraw = (bFill ? hbrReal : hbrNull);

    return 0;
}


//
//  FUNCTION: CmdCreatePen(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Handles the IDM_CREATEPEN command message. Puts up a dialog
//    to let the user set the pen style to use for drawing.
//
//  PARAMETERS:
//    hwnd     - The main window.
//    wCommand - IDM_CREATEPEN (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//

LRESULT CmdCreatePen(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    LOGPEN lp = logPen;     // copy for the dialog to munge on

    if (DialogBoxParam(hInst, "PenDlg", hwnd, (DLGPROC)PenDlg, (LPARAM)&lp))
    {
        DeleteObject(hpenDraw);                     // delete old pen
        logPen = lp;                                // save changes
        hpenDraw = CreatePenIndirect(&logPen);      // create new pen
    }

    return 0;
}


//
//  FUNCTION: CmdCreateBrush(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Handles the IDM_CREATEBRUSH command message. Puts up a dialog
//    to let the user set the brush style to use for drawing.
//
//  PARAMETERS:
//    hwnd - The main window.
//    wCommand - IDM_CREATEBRUSH (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//

LRESULT CmdCreateBrush(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    LOGBRUSH lb = logBrush; // copy for the dialog to munge on

    if (DialogBoxParam(hInst, "BrushDlg", hwnd, (DLGPROC)BrushDlg, (LPARAM)&lb))
    {
        DeleteObject(hbrReal);                      // delete old brush
        logBrush = lb;                              // save changes
        hbrReal = CreateBrushIndirect(&logBrush);   // create new brush

        // If "Fill Objects" is turned on, we need to update hbrDraw
        if (GetMenuState(hMenu, IDM_FILL, MF_BYCOMMAND) & MF_CHECKED)
            hbrDraw = hbrReal;
    }

    return 0;
}


//
//  FUNCTION: CmdClear(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Clears the contents of the client window.
//
//  PARAMETERS:
//    hwnd     - The main window.
//    wCommand - IDM_CLEAR (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdClear(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{       
    char szBuffer[9];
    int iResult;
       
    LoadString(hInst, IDS_UNTITLED, szBuffer, sizeof(szBuffer));
              
    // warn the user if important data might be lost
    if (strcmp(szCurrentFile, szBuffer))
    {
        // query the user before blowing old bits away
        iResult = MessageBox(hwnd, 
                             "Rename initialized bitmap as Untitled?", 
                             SZAPPNAME, 
                             MB_YESNOCANCEL);
                             
        switch (iResult)
        {
            case IDYES:
                // rename the file before continuting, otherwise the original
                // bitmap file may be inadvertently overwritten on disk
                strcpy(szCurrentFile, szBuffer);
                break;
                
            case IDNO:
                // allow the operation to continue
                break;
                
            case IDCANCEL:
                // don't erase the drawing surface
                return 0;
        }        
    }

    InitDIBSection(hWndClient);
    
    return 0;
}


//
//  FUNCTION: MsgClientCreate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handles the WM_CREATE message for the client window.
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
//  COMMENTS:
//    Initializes pen and brush objects.
//

LRESULT MsgClientCreate(HWND   hwnd, 
                        UINT   uMessage, 
                        WPARAM wparam, 
                        LPARAM lparam)
{
    // Initialize pens and brushes

    hpenBlack = GetStockObject(BLACK_PEN);
    hbrNull   = GetStockObject(NULL_BRUSH);
                
    InitDrawObjects();            
 
    return 0;
}


//
//  FUNCTION: InitDrawObjects()
//
//  PURPOSE: Initializes the pen and brush used for drawing.
//
//  PARAMETERS:
//    none
//
//  RETURN VALUE:
//
//  COMMENTS:
//    
//

void InitDrawObjects()
{
    hpenDraw  = hpenBlack;
    hbrReal   = GetStockObject(GRAY_BRUSH);

    hbrDraw = hbrReal;

    GetObject(hpenDraw, sizeof(logPen), &logPen);
    GetObject(hbrReal, sizeof(logBrush), &logBrush);
}


//
//  FUNCTION: MsgClientDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handles WM_DESTROY message for the client window.
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
//    General clean up happens here.
//

LRESULT MsgClientDestroy(HWND   hwnd, 
                         UINT   uMessage, 
                         WPARAM wparam, 
                         LPARAM lparam)
{
    // Delete the pen and brush used for drawing.  Don't need to delete
    // hpenBlack and hbrNull since these are always stock objects.

    DeleteObject(hpenDraw);
    DeleteObject(hbrReal);

    return 0;
}


//
//  FUNCTION: MsgClientLButtonDown(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handles WM_LBUTTONDOWN message.
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Mouse coordinates
//
//  RETURN VALUE:
//
//  COMMENTS:
//    Initiates dragging/drawing operation by saving the mouse
//    position and capturing mouse input.
//

LRESULT MsgClientLButtonDown(HWND   hwnd,
                             UINT   uMessage,
                             WPARAM wparam,
                             LPARAM lparam)
{
    POINT pt;

    // If we aren't already drawing, set focus to the client window
    // so we can get Escape key messages should the user decide to 
    // quit drawing.

    if (!bDrawing)
        SetFocus(hwnd);

    // Get mouse position
    pt.x = (int)(short)LOWORD(lparam) + cxHorzScrollPos;
    pt.y = (int)(short)HIWORD(lparam) + cyVertScrollPos;

    // Call the current LBDown handler
    return (*pfnLBDown)(hwnd, pt);
}


//
//  FUNCTION: MsgClientMouseMove(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handles WM_MOUSEMOVE message.
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Mouse coordinates
//
//  RETURN VALUE:
//
//  COMMENTS:
//    Performs "rubber-banding" by erasing the previous image and
//    redrawing the object using an XOR ROP code.
//

LRESULT MsgClientMouseMove(HWND   hwnd,
                           UINT   uMessage,
                           WPARAM wparam,
                           LPARAM lparam)
{
    POINT pt;
    static char szBuf[20] ;     // Array for formatting mouse coordinates

    // Get new mouse position
    pt.x = (int)(short)LOWORD(lparam) + cxHorzScrollPos;
    pt.y = (int)(short)HIWORD(lparam) + cyVertScrollPos;
                                      
    // Update the status bar with the new position
    wsprintf(szBuf, "%d,%d", pt.x, pt.y);
    UpdateStatusBar(szBuf, 2, 0);

    // Call the current MouseMove handler
    return (*pfnMouseMove)(hwnd, pt);
}


//
//  FUNCTION: MsgClientLButtonUp(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handles WM_LBUTTONUP message.
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Mouse coordinates
//
//  RETURN VALUE:
//
//  COMMENTS:
//    Erases previous rubber-band image and draws the object in
//      the final position.
//

LRESULT MsgClientLButtonUp(HWND   hwnd,
                           UINT   uMessage,
                           WPARAM wparam,
                           LPARAM lparam)
{
    POINT pt;

    // Get new mouse position
    pt.x = (int)(short)LOWORD(lparam) + cxHorzScrollPos;
    pt.y = (int)(short)HIWORD(lparam) + cyVertScrollPos;
  
    // Call the current LBUp handler
    return (*pfnLBUp)(hwnd, pt);
}


//
//  FUNCTION: MsgClientKeyDown(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handles WM_KEYDOWN message.
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number (Unused)
//    wparam    - Key code (VK_xxx)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//  COMMENTS:
//    Looks for VK_ESCAPE key only.  If user hits the escape key
//    while drawing, cancel the drawing operation.
//

LRESULT MsgClientKeyDown(HWND   hwnd,
                         UINT   uMessage,
                         WPARAM wparam,
                         LPARAM lparam)
{
    if (VK_ESCAPE == wparam && bDrawing)
    {
        EndRubberBand(hwnd);
        cPoints = 0;

        // get rid of residue from the aborted drawing operation
        InvalidateRect(hwnd, NULL, TRUE);
    }

    return 0;
}


//
//  FUNCTION: xxxLBDown(HWND, UINT, WPARAM, LPARAM)
//  FUNCTION: xxxMouseMove(HWND, UINT, WPARAM, LPARAM)
//  FUNCTION: xxxLBUp(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handles WM_LBUTTONDOWN, WM_MOUSEMOVE, and WM_LBUTTONUP
//
//  PARAMETERS:
//
//    hwnd - Window handle
//    pt   - Mouse coordinates
//
//  RETURN VALUE:
//    Always return 0
//
//  COMMENTS:
//    These functions are called by the actual Msgxxx mouse message
//    handler functions (above) to perform input processing.  Three
//    sets of these functions are implemented here.  One to input
//    individual pixels; one to select 2 points (used to draw a
//    line, rectangle, or ellipse); and one to select 4 points (for
//    drawing Bezier curves).  Additional functions could be "plugged
//    in" to support other types of drawing.
//
//    In general, xxxLBDown initializes the drawing process, for example
//    by saving the initial coordinates and calling StartRubberBand;
//    xxxMouseMove updates the coordinates; and xxxLBUp performs any
//    final processing and clean up (EndRubberBand).
//

// 
//  FUNCTION: PixelLBDown(HWND, UINT, WPARAM, LPARAM)
//  FUNCTION: PixelMouseMove(HWND, UINT, WPARAM, LPARAM)
//  FUNCTION: PixelLBUp(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Perform single point selection (individual pixels).
//

LRESULT PixelLBDown(HWND hwnd, POINT pt)
{
    // Get a DC and capture mouse input
    StartRubberBand(hwnd);
    SetROP2(hdcRB, R2_COPYPEN);
    SetROP2(hdcMem, R2_COPYPEN);
    
    // Draw the pixel (use current pen color)
    SetPixelV(hdcRB, pt.x, pt.y, logPen.lopnColor);

    // do it again in memory DC
    SetPixelV(hdcMem, pt.x, pt.y, logPen.lopnColor);

    return 0;
}


LRESULT PixelMouseMove(HWND hwnd, POINT pt)
{
    // If we are currently drawing, draw the pixel
    if (bDrawing)
    {
        SetPixelV(hdcRB, pt.x, pt.y, logPen.lopnColor);
        
         // do it again in memory DC
        SetPixelV(hdcMem, pt.x, pt.y, logPen.lopnColor);
    }

    return 0;
}


LRESULT PixelLBUp(HWND hwnd, POINT pt)
{   
    // Don't draw the pixel again here (it's already drawn at LBDown
    // and MouseMove time).  Just need to clean up.

    if (bDrawing)
        EndRubberBand(hwnd);

    return 0;
}


// 
//  FUNCTION: BezierLBDown(HWND, UINT, WPARAM, LPARAM)
//  FUNCTION: BezierMouseMove(HWND, UINT, WPARAM, LPARAM)
//  FUNCTION: BezierLBUp(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Perform 4-point selection for drawing Bezier curves.
//

LRESULT BezierLBDown(HWND hwnd, POINT pt)
{
    // If this is the first click, initialize rubber banding.

    if (0 == cPoints)
        StartRubberBand(hwnd);

    return 0;
}


LRESULT BezierMouseMove(HWND hwnd, POINT pt)
{
    int i;

    if (!bDrawing || 0 == cPoints)              // Are we currently drawing?
        return 0;

    // Are we rubber banding a Bezier?
    if (cPoints >= 2)
        PolyBezier(hdcRB, pPoints, BEZ_MAXPOINTS); // Erase previous Bezier

    // Erase previous line segment
    LineDraw(hdcRB, pPoints[cPoints - 1], pPoints[cPoints]);

    // Set the rest of the points to be the same as this one
    for (i = cPoints; i < BEZ_MAXPOINTS; i++)
        pPoints[i] = pt;

    // Draw new line segment
    LineDraw(hdcRB, pPoints[cPoints - 1], pPoints[cPoints]);

    // Are we rubber banding a Bezier?
    if (cPoints >= 2)
        PolyBezier(hdcRB, pPoints, BEZ_MAXPOINTS);

    return 0;
}


LRESULT BezierLBUp(HWND hwnd, POINT pt)
{
    int i;

    if (!bDrawing)          // Are we currently drawing?
        return 0;

    if (cPoints >= 2)       // Are we rubber banding a Bezier?
    {
        // Erase previous Bezier
        PolyBezier(hdcRB, pPoints, BEZ_MAXPOINTS);

        if (cPoints == BEZ_MAXPOINTS - 1) // Is this the last point?
        {
            // If so, then erase all line segments.
            for (i = BEZ_MAXPOINTS - 1; i > 0; i--)
                LineDraw(hdcRB, pPoints[i - 1], pPoints[i]);
        }
    }
    // else do *not* erase the previous line segments

    // Save new position and increment # of points
    pPoints[cPoints++] = pt;

    if (cPoints < BEZ_MAXPOINTS)    // Still more points to get.
    {
        // Set the rest of the points to be the same as the current one
        for (i = cPoints; i < BEZ_MAXPOINTS; i++)
            pPoints[i] = pt;

        // Draw new line segment
        LineDraw(hdcRB, pPoints[cPoints - 1], pPoints[cPoints]);

        // Once we have at least 2 points let's draw the Bezier curve
        if (cPoints >= 2)
            PolyBezier(hdcRB, pPoints, BEZ_MAXPOINTS);
    }
    else    // Finish this Bezier and quit drawing.
    {
        // Setup DC for 'real' drawing
        SetROP2(hdcRB, R2_COPYPEN);
        SetBkMode(hdcRB, TRANSPARENT);
        SelectObject(hdcRB, hpenDraw);

        // Draw Bezier in permanent position
        PolyBezier(hdcRB, pPoints, cPoints);

        // De-select the pen and clean up rubber banding stuff.
        SelectObject(hdcRB, hpenBlack);     
        
        // do it again in Memory DC         
        SetROP2(hdcMem, R2_COPYPEN);
        SetBkMode(hdcMem, TRANSPARENT);
        SelectObject(hdcMem, hpenDraw);

        // Draw Bezier in permanent position
        PolyBezier(hdcMem, pPoints, cPoints);

        // De-select the pen and clean up rubber banding stuff.
        SelectObject(hdcMem, hpenBlack);
        
        EndRubberBand(hwnd);
        cPoints = 0;
    }

    return 0;
}


// 
//  FUNCTION: RectLBDown(HWND, UINT, WPARAM, LPARAM)
//  FUNCTION: RectMouseMove(HWND, UINT, WPARAM, LPARAM)
//  FUNCTION: RectLBUp(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Perform 2-point selection for drawing lines, rectangles,
//    and ellipses.
//

LRESULT RectLBDown(HWND hwnd, POINT pt)
{
    // Current position = starting position = mouse position
    pPoints[0] = pPoints[1] = pt;

    // Initialize rubber banding
    StartRubberBand(hwnd);

    // Draw initial state
    (*pfnDrawRect)(hdcRB, pPoints[0], pPoints[1]);

    return 0;
}


LRESULT RectMouseMove(HWND hwnd, POINT pt)
{
    if (!bDrawing)          // Are we currently drawing?
        return 0;

    // Un-draw previous position by redrawing
    (*pfnDrawRect)(hdcRB, pPoints[0], pPoints[1]);

    // Save new position
    pPoints[1] = pt;

    // Draw in new position
    (*pfnDrawRect)(hdcRB, pPoints[0], pPoints[1]);

    return 0;
}


LRESULT RectLBUp(HWND hwnd, POINT pt)
{
    if (!bDrawing)          // Are we currently drawing?
        return 0;

    // Un-draw previous position by redrawing
    (*pfnDrawRect)(hdcRB, pPoints[0], pPoints[1]);

    // Save new position
    pPoints[1] = pt;

    // Setup DC for 'real' drawing
    SetROP2(hdcRB, R2_COPYPEN);
    SetBkMode(hdcRB, TRANSPARENT);
    SelectObject(hdcRB, hpenDraw);
    SelectObject(hdcRB, hbrDraw);

    // Draw object in permanent position
    (*pfnDrawRect)(hdcRB, pPoints[0], pPoints[1]);

    // De-select the pen and brush by selecting stock objects 
    // and clean up rubber banding stuff.
    SelectObject(hdcRB, hpenBlack);
    SelectObject(hdcRB, hbrNull); 
    
    // draw it again onto bitmap in memory DC   
    SetROP2(hdcMem, R2_COPYPEN);
    SetBkMode(hdcMem, TRANSPARENT);
    SelectObject(hdcMem, hpenDraw);
    SelectObject(hdcMem, hbrDraw);

    // Draw object in permanent position
    (*pfnDrawRect)(hdcMem, pPoints[0], pPoints[1]);

    // De-select the pen and brush by selecting stock objects 
    // and clean up rubber banding stuff.
    SelectObject(hdcMem, hpenBlack);
    SelectObject(hdcMem, hbrNull);
    
    EndRubberBand(hwnd);

    return 0;
}


//
//  FUNCTION: xxxxDraw(HDC hdc, POINT pt1, POINT pt2)
//
//  PURPOSE: Draws a Line, Rectangle, or Ellipse at the coordinates
//    given by pt1 and pt2.
//
//  PARAMETERS:
//    hdc   - Device context to draw into.
//    pt1   - Beginning point (upper-left corner of rect)
//    pt2   - Ending point    (lower-right corner of rect)
//
//  RETURN VALUE:
//    TRUE for success, FALSE otherwise.
//
//  COMMENTS:
//    Assumes the DC is already correctly set up.
//
//

BOOL LineDraw(HDC hdc, POINT pt1, POINT pt2)
{
    MoveToEx(hdc, pt1.x, pt1.y, NULL);
    return LineTo(hdc, pt2.x, pt2.y);
}


BOOL RectDraw(HDC hdc, POINT pt1, POINT pt2)
{
    return Rectangle(hdc, pt1.x, pt1.y, pt2.x, pt2.y);
}


BOOL EllipseDraw(HDC hdc, POINT pt1, POINT pt2)
{
    return Ellipse(hdc, pt1.x, pt1.y, pt2.x, pt2.y);
}


//
//  FUNCTION: StartRubberBand(HWND)
//
//  PURPOSE: Sets up DC for rubber banding and captures the mouse.
//
//  PARAMETERS:
//    hwnd - Window for GetDC
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//

void StartRubberBand(HWND hwnd)
{         
    // Get a DC to draw in
    hdcRB = GetDC(hwnd);                        
    
    // adjust the viewport origin for scrolled view
    SetViewportOrgEx(hdcRB, -1 * cxHorzScrollPos, -1 * cyVertScrollPos, NULL); 

    // R2_NOT causes drawing with a pen to invert the screen pixels.
    // This makes it easy to erase something by drawing it a 2nd time.
    SetROP2(hdcRB, R2_NOT);

    // For rubber banding with R2_NOT, we use a single pixel pen and
    // a NULL brush.
    SelectObject(hdcRB, hpenBlack);
    SelectObject(hdcRB, hbrNull);

    SetCapture(hwnd);                           // Capture mouse input
    bDrawing = TRUE;     
    
    // select the DIB's palette into the screen DC so it will select into the
    // memory DC properly
    if (hPalette)
        hpalRB = SelectPalette(hdcRB, hPalette, TRUE);
    
    // setup a memory DC to update the bitmap  
    // do not adjust its viewport because we are only scrolling the screen DC
    // i.e., we aren't viewing the image in the memory DC
    hdcMem = CreateCompatibleDC(hdcRB);      
    hbmOld = SelectObject(hdcMem, hBitmap);

    if (hPalette)
        hpalMem = SelectPalette(hdcMem, hPalette, TRUE);   
}


//
//  FUNCTION: EndRubberBand(HWND)
//
//  PURPOSE: Releases rubber banding DC and mouse capture.
//
//  PARAMETERS:
//    hwnd - Window for ReleaseDC
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//

void EndRubberBand(HWND hwnd)
{   
    BOOL fOldChanges = fChanges; 
    
    if (hpalMem)
        SelectPalette(hdcMem, hpalMem, FALSE);
    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);
    fChanges = TRUE;
    EnableMenuItem(hMenu, IDM_FILESAVE, MF_ENABLED);
    SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_FILESAVE, MAKELONG(TRUE, 0));
                 
    if (hpalRB)
        SelectPalette(hdcRB, hpalRB, FALSE);
    SetViewportOrgEx(hdcRB, 0, 0, NULL);
    ReleaseDC(hwnd, hdcRB);
    ReleaseCapture();    
    bDrawing = FALSE;
    
    if (fOldChanges == FALSE)
        // need to update main window text now so '*' is included for changes
        SetWindowTitle(GetParent(hwnd), szCurrentFile);
}        


//
//  FUNCTION: MsgClientPaint(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Paints the client window in response to WM_PAINT message.
//
//  PARAMETERS:
//    hwnd - Handle of Client window.
//    uMessage - (unused)
//    wparam - (unused)
//    lparam - (unused)
//
//  RETURN VALUE:
//    0
//
//  COMMENTS:
//

LRESULT MsgClientPaint(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HDC         hDC;                 // Handle to DC,
    PAINTSTRUCT ps;                  // Painting structure  
    RECT        rcClient, rcDDB;     // Client and bitmap rectangles
    int         xScroll, yScroll;    // Scroll positions 

    // Begin painting
    hDC = BeginPaint(hwnd, &ps);

    // if there is a bitmap, display it
    if (hBitmap)
    {                      
    	int dx = BitmapWidth(hBitmap);
    	int dy = BitmapHeight(hBitmap);
    
        // Get scroll bar positions
        xScroll  = GetScrollPos(hwnd, SB_HORZ);
        yScroll  = GetScrollPos(hwnd, SB_VERT);  

        // Set up the necessary rectangles -- i.e. the rectangle
        // we're rendering into, and the rectangle in the bitmap
        GetClientRect(hwnd, &rcClient);

        rcDDB.left   = xScroll;
        rcDDB.top    = yScroll;
        rcDDB.right  = xScroll + RECTWIDTH(&rcClient);
        rcDDB.bottom = yScroll + RECTHEIGHT(&rcClient);		
		            
        if (rcDDB.right > dx)
        {
            dx -= rcDDB.right;

            rcDDB.right     += dx;
            rcClient.right  += dx;
        }
      
        if (rcDDB.bottom > dy)
        {
            dy -= rcDDB.bottom;
            
            rcDDB.bottom    += dy;
            rcClient.bottom += dy;
        }

        // Go do the actual painting.
        PaintBitmap(hDC, &rcClient, hBitmap, &rcDDB, hPalette);
    }   
    else
    {         
        // Turn off scroll bars in case they were on
        SetScrollRange (hwnd, SB_VERT, 0, 0, TRUE);
        SetScrollRange (hwnd, SB_HORZ, 0, 0, TRUE);         
        cxHorzScrollPos = cyVertScrollPos = 0;
    }   
		
    EndPaint(hwnd, &ps);
    return 0;
}


//
//  FUNCTION: MsgClientScroll(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Scrolls the client window in response to scroll message.
//
//  PARAMETERS:
//    hwnd - handle of Client window. 
//    uMessage - the scroll message.
//    wParam - contains scroll position and scrollbar type
//    lParam - (unused)
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//    MsgClientScroll performs scrolling in both horiziontal and vertical directions.
//    If the user clicks on one of the scrolling arrows, the window is scrolled
//    by (1 / SCROLL_RATIO) of the client area.  For example, if 
//    SCROLL_RATIO == 4, then the client area is moved over a 1/4 of the 
//    width or height (as the case may be) of the screen.  If the user pages
//    up/down the window is scrolled an amount equal to the client area's
//    width or height as the case may be. If the user moves the thumb to an 
//    absolute position, the window is scrolled accordingly.
//
//

#define SCROLL_RATIO 4

LRESULT MsgClientScroll(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int nPos;          // Scrollbar position.
    int nMin;          // Minumum scroll bar value.
    int nMax;          // Maximum scroll bar value.
    int nMove;         // How much to move.
    int nLine;         // # of pixels for LINEUP/LINEDOWN
    int nPage;         // # of pixels for PAGEUP/PAGEDOWN
    int nHorzOrVert;   // SB_HORZ if doing horizontal, SB_VERT if doing vertical
    RECT rcClient;     // Client area.

    GetClientRect(hwnd, &rcClient);

    if (uMessage == WM_HSCROLL)
    {
        nHorzOrVert = SB_HORZ;
        nPage = BitmapWidth(hBitmap) - RECTWIDTH(&rcClient);
    }
    else
    {
        nHorzOrVert = SB_VERT;
        nPage = BitmapHeight(hBitmap) - RECTHEIGHT(&rcClient);
    }

    if (nPage < 0)
    {
        OutputDebugString("Scrolling not necessary\r\n");
        return 0;
    }

    // On a SB_LINEUP/SB_LINEDOWN we will move the picture by
    //  1 / SCROLL_RATIO of the page amount (i.e. if SCROLL_RATIO
    //  is 4, it will scroll the picture a quarter of the way)

    nLine = nPage / SCROLL_RATIO;
    if (nLine == 0)
        nLine = 1;

    nPos = GetScrollPos(hwnd, nHorzOrVert);
    GetScrollRange(hwnd, nHorzOrVert, &nMin, &nMax);

    switch (GET_WM_HSCROLL_CODE(wparam, lparam))
    {
        case SB_LINEDOWN:             // One line right.
            nMove = nLine;
            break;

        case SB_LINEUP:               // One line left.
            nMove = -nLine;
            break;

        case SB_PAGEDOWN:             // One page right.
            nMove = nPage;
            break;

        case SB_PAGEUP:               // One page left.
            nMove = -nPage;
            break;

        case SB_THUMBPOSITION:        // Absolute position.
            nMove = GET_WM_HSCROLL_POS(wparam, lparam) - nPos;
            break;

        default:                      // No change.
            nMove = 0;
            break;
    }

    if (nMove)
    {
        nPos += nMove;

        if (nPos < nMin)
        {
            nMove -= nPos - nMin;
            nPos = nMin;
        }

        if (nPos > nMax)
        {
            nMove -= nPos - nMax;
            nPos = nMax;
        }

        if (nMove)
        {
            SetScrollPos(hwnd, nHorzOrVert, nPos, TRUE);

            if (nHorzOrVert == SB_HORZ)
            {
                ScrollWindow(hwnd, -nMove, 0, NULL, NULL);

                // set the global variable
                cxHorzScrollPos = nPos;
            }
            else 
            {
                ScrollWindow(hwnd, 0, -nMove, NULL, NULL);

                // set the global variable
                cyVertScrollPos = nPos;
            }
        }
    }
    return 0;
}

UINT BitmapWidth(HBITMAP hbm)
{
	BITMAP bm;

	if (!hbm)
		return 0;
		
	GetObject(hbm, sizeof(bm), &bm);	
	return bm.bmWidth;
}  

UINT BitmapHeight(HBITMAP hbm)
{
	BITMAP bm;

	if (!hbm)
		return 0;
		
	GetObject(hbm, sizeof(bm), &bm);	
	return bm.bmHeight;
}
