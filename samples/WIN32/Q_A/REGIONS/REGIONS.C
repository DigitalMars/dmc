
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/******************************************************************************\
*
*  PROGRAM:     Regions.c
*
*  PURPOSE:     To demonstrate the region APIs in Win32, namely:
*
*                          CombineRgn()
*                          CreateEllipticRgn()
*                          CreateEllipticRgnIndirect()
*                          CreatePolygonRgn()
*                          CreatePolyPolygonRgn()
*                          CreateRectRgn()
*                          FillRgn()
*                          FrameRgn()
*                          GetRgnBox()
*                          InvertRgn()
*                          OffsetRgn()
*                          PaintRgn()
*                          PtInRegion()
*                          RectInRegion()
*                          SetRectRgn()
*
*               The EqualRgn() API is *not* called in this program.
*
*  FUNCTIONS:
*               WinMain()       - initialization, create window, msg loop
*               MainWndProc()   - processes main window msgs
*               AboutDlgProc()  - processes About dialog box msgs
*               RgnBoxDlgProc() - processes RgnBox dialog box msgs
*               MyCreateRgn()   - calls any one of Create*Rgn*() APIs
*               MyCombineRgn()  - calls CombineRgn() API
*               TrackRect()     - creates\draws\deletes a tracking rectangle
*                                 (for use with RectInRegion menu item.)
*               Reset()         - enables\disables menu items, modifies
*                                 global vars (rgnArray[], currRgn).
*
*  COMMENTS:    This program allows a user to create the different kinds
*               of regions available in Win32, and to perform the various
*               operations on them. All operations are immediately
*               reflected on the screen. The order in which the regions
*               are created determines the order in which they are drawn,
*               as well as which operation affects which region(s); for
*               example, the last region created will be drawn on top of
*               the others, and the operations in "Options" menu will
*               affect this region. When two regions are combined the
*               second region will be destroyed.
*
\******************************************************************************/

#include <windows.h>
#include "regions.h"



/******************************************************************************\
*
*  FUNCTION:    WinMain (standard WinMain INPUTS/RETURNS)
*
*  GLOBAL VARS: hInst - Handle of program instance
*
\******************************************************************************/

int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow)
{ HWND hwnd;
  MSG  msg;

  if (!hPrevInstance)
  {
    WNDCLASS wc;

    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC)MainWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject (DKGRAY_BRUSH);
    wc.lpszMenuName  = (LPCTSTR) "Menu";
    wc.lpszClassName = (LPCTSTR) "Main";

    if (!RegisterClass (&wc))
    {
      MessageBox (NULL, (LPCTSTR) "RegisterClass() failed",
                  (LPCTSTR) "Err! - REGIONS", MB_OK | MB_ICONEXCLAMATION);
      return(FALSE);
    }
  }
  hInst = hInstance;
  if (!(hwnd  = CreateWindow ("Main", "Regions API Sample",
                              WS_OVERLAPPED | WS_CAPTION |
                              WS_SYSMENU | WS_MINIMIZEBOX,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              220, 280, NULL, NULL, hInstance, NULL)))
    return (0);

  ShowWindow   (hwnd, nCmdShow);

  while (GetMessage (&msg, NULL, 0,0))
  {
    TranslateMessage (&msg);
    DispatchMessage  (&msg);
  }

  return (msg.wParam);
}



/******************************************************************************\
*
*  FUNCTION:    MainWndProc (standard window procedure INPUTS/RETURNS)
*
*  GLOBAL VARS: hInst    - Handle of program instance
*               rgnArray - Array of RGNSTRUCTs describing regions user has
*                          created
*               currRgn  - Index (in rgnArray) of most recently created
*                          region. If -1 then no regions created. Otherwise,
*                          values range from 0 to MAXRGNSTRUCTS - 1.
*               hMenu    - Handle of main window's menu.
*
*  LOCAL VARS:  rect     - Scratch rectangle used in various places.
*               ptr      - Pointer to a TRACKRECTSTRUCT (used for
*                          tracking rectangle).
*               i        - For-loop variable.
*               hdc      - Scratch HDC used in various places.
*               lButtonFlag - Tells what to do when left mouse button goes
*                             down (eg. nothing, check point in region,
*                             check rect in region).
*               bOffsetRgnFlag - A BOOL which tells whether to offset up
*                                and left or down and right.
*
\******************************************************************************/

LRESULT CALLBACK MainWndProc (HWND hWnd, UINT message, WPARAM wParam,
                              LPARAM lParam)
{
  static RECT             rect;
  static PTRACKRECTSTRUCT ptr;
  static BOOL             bOffsetRgnFlag = FALSE;
  static WORD             lButtonFlag = 0;
         int              i;
         HDC              hdc;

  switch (message)
  {
    case WM_CREATE:

      hMenu   = GetMenu (hWnd);
      currRgn = -1;
      Reset (RESET_ALL);
      break;

    case WM_COMMAND:

      switch (LOWORD(wParam))
      {
        case IDM_ERASE:
          Reset (RESET_ALL);
          InvalidateRect (hWnd, NULL, TRUE);
          break;

        case IDM_OFFSETRGN:

          //
          // Offset rgnArray[currRgn].hrgn either down & right, or up &
          //   left (depending on bOffsetRgnFlag). Toggle bOffsetRgnFlag.
          //

          if (bOffsetRgnFlag)

            OffsetRgn (rgnArray[currRgn].hrgn, 15, 10);

          else

            OffsetRgn (rgnArray[currRgn].hrgn, -15, -10);

          bOffsetRgnFlag = ~bOffsetRgnFlag;
          InvalidateRect (hWnd, NULL, TRUE);
          break;

        case IDM_INVERTRGN:

          hdc = GetDC (hWnd);
          InvertRgn (hdc, rgnArray[currRgn].hrgn);
          ReleaseDC (hWnd, hdc);
          break;

        case IDM_FRAMERGN:

          hdc = GetDC (hWnd);

          FrameRgn (hdc, rgnArray[currRgn].hrgn,
                    GetStockObject (LTGRAY_BRUSH), 2, 2);
          ReleaseDC (hWnd, hdc);
          break;

        case IDM_PTINRGN:

          //
          // Uncheck "RectInRgn" menu item, toggle "PtInRgn" menu item,
          //   toggle lButtonFlag
          //

          if (lButtonFlag == IDM_RECTINRGN)
            CheckMenuItem (GetSubMenu (hMenu, 0), IDM_RECTINRGN,
                            MF_UNCHECKED | MF_BYCOMMAND);
          if (lButtonFlag == IDM_PTINRGN)
          {
            CheckMenuItem (GetSubMenu (hMenu, 0), IDM_PTINRGN,
                            MF_UNCHECKED | MF_BYCOMMAND);
            lButtonFlag = 0;
          }
          else
          {
            CheckMenuItem (GetSubMenu (hMenu, 0), IDM_PTINRGN,
                            MF_CHECKED | MF_BYCOMMAND);
            lButtonFlag = IDM_PTINRGN;
          }
          break;

        case IDM_RECTINRGN:

          //
          // Uncheck "PtInRgn" menu item, toggle "RectInRgn" menu item,
          //   toggle lButtonFlag
          //

          if (lButtonFlag == IDM_PTINRGN)

            CheckMenuItem (GetSubMenu (hMenu, 0), IDM_PTINRGN,
                            MF_UNCHECKED | MF_BYCOMMAND);

          if (lButtonFlag == IDM_RECTINRGN)
          {
            CheckMenuItem (GetSubMenu (hMenu, 0), IDM_RECTINRGN,
                            MF_UNCHECKED | MF_BYCOMMAND);
            lButtonFlag = 0;
          }
          else
          {
            CheckMenuItem (GetSubMenu (hMenu, 0), IDM_RECTINRGN,
                            MF_CHECKED | MF_BYCOMMAND);
            lButtonFlag = IDM_RECTINRGN;
          }
          break;

        case IDM_SETRECTRGN:

          //
          // Reset rgnArray[currRgn].hrgn to be the rectangle which
          //   currently bounds it.
          //

          GetRgnBox  (rgnArray[currRgn].hrgn, &rect);
          SetRectRgn (rgnArray[currRgn].hrgn, (int) rect.left,
                      (int) rect.top, (int) rect.right,
                      (int) rect.bottom);
          InvalidateRect (hWnd, NULL, TRUE);
          break;

        case IDM_GETRGNBOX: // put up dlg which call GetRgnBox

          DialogBox (hInst, (LPCTSTR) "RgnBox", hWnd, (DLGPROC)RgnBoxDlgProc);
          break;

        case IDM_ELLIPSE: // create an elliptic region

          MyCreateRgn (ELLIPTIC_RGN);
          InvalidateRect (hWnd, NULL, TRUE);
          break;

        case IDM_POLYPOLYGON: // create a polyPolygon region

          MyCreateRgn (POLYPOLYGON_RGN);
          InvalidateRect (hWnd, NULL, TRUE);
          break;

        case IDM_RECT: // create a rectangular rgn

          MyCreateRgn (RECT_RGN);
          InvalidateRect (hWnd, NULL, TRUE);
          break;

        case IDM_AND:  // combine previous 2 rgns

          MyCombineRgn (RGN_AND);
          InvalidateRect (hWnd, NULL, TRUE);
          break;

        case IDM_COPY: // combine previous 2 rgns

          MyCombineRgn (RGN_COPY);
          InvalidateRect (hWnd, NULL, TRUE);
          break;

        case IDM_DIFF: // combine previous 2 rgns

          MyCombineRgn (RGN_DIFF);
          InvalidateRect (hWnd, NULL, TRUE);
          break;

        case IDM_OR: // combine previous 2 rgns

          MyCombineRgn (RGN_OR);
          InvalidateRect (hWnd, NULL, TRUE);
          break;

        case IDM_XOR: // combine previous 2 rgns

          MyCombineRgn (RGN_XOR);
          InvalidateRect (hWnd, NULL, TRUE);
          break;

        case IDM_ABOUT:  // put up About dialog box

          DialogBox (hInst, (LPCTSTR) "About", hWnd, (DLGPROC)AboutDlgProc);
          break;
      }
      return (0);

    case WM_LBUTTONDOWN:
    {
      //
      // if the "PtInRgn" menu item is checked, then see if the point at
      //   which this event happened is within currRgn, MessageBeep if so.
      //
      // else if the "RectInRgn" menu items are checked, then start
      //   tracking rectangle mode
      //

      if (lButtonFlag == IDM_PTINRGN)
      {
        if (currRgn > -1)

          if (PtInRegion (rgnArray[currRgn].hrgn, (int) LOWORD(lParam),
                          (int) HIWORD(lParam)))
            MessageBeep (0);
      }
      else if (lButtonFlag == IDM_RECTINRGN)

        if ((ptr = TrackRect (NULL, TRECT_NEW, hWnd, lParam)) == NULL)

        //
        // If TrackRect failed to LocalAlloc, then disable the
        //   IDM_RECTINRGN opion.
        //

          EnableMenuItem (GetSubMenu (hMenu, 0), IDM_RECTINRGN,
                          MF_DISABLED | MF_GRAYED | MF_BYCOMMAND);

      return (0);
    }
    case WM_MOUSEMOVE:

      //
      // if in tracking rectangle mode call TrackRect
      //

      if (lButtonFlag == IDM_RECTINRGN && GetCapture () == hWnd)

        TrackRect (ptr, TRECT_PAINT, hWnd, lParam);

      return (0);

    case WM_LBUTTONUP:

      //
      // if in tracking rectangle mode call TrackRect, check out the final
      //   rectangle, and see if it intersects currRgn
      //

      if (lButtonFlag == IDM_RECTINRGN && GetCapture () == hWnd)
      {
        //
        // call TrackRect with TRECT_PAINT so it updates the rectangle
        //   for the last time, then again with TRECT_DELETE so it frees
        //   up the TRACKRECTSTRUCT it allocated
        //

        TrackRect (ptr, TRECT_PAINT, hWnd, lParam);
        rect = ptr->trackRect;
        TrackRect (ptr, TRECT_DELETE, hWnd, lParam);
        if (RectInRegion (rgnArray[currRgn].hrgn, &rect))

          MessageBeep (0);
      }
      return (0);

    case WM_PAINT:
    {
      PAINTSTRUCT ps;
      COLORREF    crColor;
      HBRUSH      hBrush;

      hdc = BeginPaint (hWnd, &ps);

      //
      // For each rgn we created determine appropriate color & draw it
      //

      for (i = 0; i <= currRgn; i++)
      {
        if (rgnArray[i].hrgn != NULL)
        {
          crColor = 0x000000;
          if (rgnArray[i].type & ELLIPTIC_RGN)    // add some red

            crColor |= 0x0000ff;

          if (rgnArray[i].type & POLYPOLYGON_RGN) // add some green

            crColor |= 0x00ff00;

          if (rgnArray[i].type & RECT_RGN)        // add some blue

            crColor |= 0xff0000;

          hBrush = CreateSolidBrush (crColor);

          //
          // Alternately, the following would work:
          //    SelectObject (hdc, hBrush);
          //    FillRgn (hdc, rgnArray[i].hrgn);
          //

          FillRgn (hdc, rgnArray[i].hrgn, hBrush);
          DeleteObject (hBrush);
        }
      }
      EndPaint (hWnd, &ps);
      return (0);
    }
    case WM_DESTROY:

      PostQuitMessage(0);
      return (0);
  }
  return (DefWindowProc(hWnd, message, wParam, lParam));
}



/******************************************************************************\
*
*  FUNCTION:    AboutDlgProc (standard dialog procedure INPUTS/RETURNS)
*
*  COMMENTS:    Displays "about" message
*
\******************************************************************************/

BOOL CALLBACK AboutDlgProc (HWND hwnd, UINT message, WPARAM wParam,
                               LPARAM lParam)
{ switch (message)
  {
    case WM_COMMAND:

      if (LOWORD(wParam) == IDOK)
      {
        EndDialog(hwnd, TRUE);
        return (TRUE);
      }
      return (TRUE);
  }
  return (FALSE);
}



/******************************************************************************\
*
*  FUNCTION:    RgnBoxDlgProc (standard dialog procedure INPUTS/RETURNS)
*
*  GLOBAL VARS: rgnArray - Array of RGNSTRUCTs describing regions user has
*                          created
*               currRgn  - Index (in rgnArray) of most recently created
*                          region. If -1 then no regions created. Other-
*                          wise, values range from 0 to MAXRGNSTRUCTS - 1.
*
*  COMMENTS:    Displays bounding rectangle of most recently created
*               region.
*
\******************************************************************************/

BOOL CALLBACK RgnBoxDlgProc (HWND hwnd, UINT message, WPARAM wParam,
                             LPARAM lParam)
{
  switch (message)
  {
    case WM_INITDIALOG:
    {
      RECT rect;
      char buf[15];

      //
      // get the bounding rect of currRgn, fill in the dlg controls
      //   with text string describing the bounding rect.
      //

      if (GetRgnBox (rgnArray[currRgn].hrgn, &rect) == SIMPLEREGION)

        SetDlgItemText (hwnd, 102, "SIMPLEREGION");

      else

        SetDlgItemText (hwnd, 102, "COMPLEXREGION");

      wsprintf (buf, "%ld", rect.left);
      SetDlgItemText (hwnd, 107, buf);
      wsprintf (buf, "%ld", rect.top);
      SetDlgItemText (hwnd, 108, buf);
      wsprintf (buf, "%ld", rect.right);
      SetDlgItemText (hwnd, 109, buf);
      wsprintf (buf, "%ld", rect.bottom);
      SetDlgItemText (hwnd, 110, buf);

      return (TRUE);
    }

    case WM_COMMAND:

      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
      {
        EndDialog(hwnd, TRUE);
        return (TRUE);
      }
      return (TRUE);
  }
  return (FALSE);
}



/******************************************************************************\
*
*  FUNCTION:    MyCreateRgn
*
*  INPUTS:      rgnStyle - The type of region to create (ELLIPTIC_RGN,
*                          POLYPOLYGON_RGN, RECT_RGN)
*
*  GLOBAL VARS: rgnArray          - Array of RGNSTRUCTs describing regions
*                                   user has created.
*               currRgn           - Index (in rgnArray) of most recently
*                                   created region. If -1 then no regions
*                                   created. Otherwise, values range from
*                                   0 to MAXRGNSTRUCTS-1.
*               hMenu             - Handle of main window's menu.
*               ellipseRect       - Rectangle bounding elliptical region.
*               polyPolygonPoints - The points for the polyPolygon region.
*               polyPolygonCounts - The count of points for each polygon
*                                   in the polyPolygon region.
*               rectRect          - The points for the rectangular region.
*
*  COMMENTS:    Creates a region (based on rgnStyle), increments currRgn,
*               and fills in rgnArray[currRgn].
*
\******************************************************************************/

void MyCreateRgn (WORD rgnStyle)
{
  if (++currRgn == 0)         // if this is 1st region enable options menu

    Reset (ENABLE_OPTIONS);

  else if (currRgn == 1)      // if this is 2nd region enable combine menu

    Reset (ENABLE_COMBINERGN);

  rgnArray[currRgn].type = rgnStyle;

  switch (rgnStyle)
  {
    case ELLIPTIC_RGN:

      //
      //  The following will also work here:
      //     CreateEllipticRgn ((int) ellipseRect.left,
      //                        (int) ellipseRect.top,
      //                        (int) ellipseRect.right,
      //                        (int) ellipseRect.bottom);
      //

      rgnArray[currRgn].hrgn = CreateEllipticRgnIndirect (&ellipseRect);

      EnableMenuItem (GetSubMenu (hMenu, 1), IDM_ELLIPSE,
                      MF_GRAYED | MF_DISABLED | MF_BYCOMMAND);
      break;

    case POLYPOLYGON_RGN:

      //
      // Alternately, several calls to CreatePolygonRgn could be made.
      //

      rgnArray[currRgn].hrgn = CreatePolyPolygonRgn (polyPolygonPoints,
                                                     polyPolygonCounts,
                                                     POLYPOLYGONCOUNT,
                                                     WINDING);
      EnableMenuItem (GetSubMenu (hMenu, 1), IDM_POLYPOLYGON,
                      MF_GRAYED | MF_DISABLED | MF_BYCOMMAND);
      break;

    case RECT_RGN:

      //
      // Alternately, a call to CreatePolygonRgn could be made.
      //

      rgnArray[currRgn].hrgn = CreateRectRgn (rectRect.left,
                                              rectRect.top,
                                              rectRect.right,
                                              rectRect.bottom);

      EnableMenuItem (GetSubMenu (hMenu, 1), IDM_RECT,
                      MF_GRAYED | MF_DISABLED | MF_BYCOMMAND);
      break;
  }
}



/******************************************************************************\
*
*  FUNCTION:    MyCombineRgn
*
*  INPUTS:      fnCombineMode- Operation to perform on the two regions
*                              (RGN_AND, RGN_COPY, RGN_DIFF, RGN_OR,
*                              RGN_XOR)
*
*  GLOBAL VARS: rgnArray- Array of RGNSTRUCTs describing regions user has
*                         created
*               currRgn - Index (in rgnArray) of most recently created
*                         region. If -1 then no regions created. Otherwise,
*                         values range from 0 to MAXRGNSTRUCTS - 1.
*
*  COMMENTS:    Combines the two regions rgnArray[currRgn-1] and
*               rgnArray[currRgn] using fnCombineMode, placing the
*               results in rgnArray[currRgn-1]. Destroys rgnArray[currRgn]
*               and decrements currRgn.
*
\******************************************************************************/

void MyCombineRgn (int fnCombineMode)
{
  if (currRgn >= 1) // only if two or more regions have been created
  {
    CombineRgn (rgnArray[currRgn-1].hrgn, rgnArray[currRgn-1].hrgn,
                rgnArray[currRgn].hrgn, fnCombineMode);

    rgnArray[currRgn-1].type |= rgnArray[currRgn].type;

    //
    // Delete the second of the two regions after combining
    //

    DeleteObject (rgnArray[currRgn--].hrgn);

    if (currRgn == 0)

      Reset (DISABLE_COMBINERGN);
  }
}



/******************************************************************************\
*
*  FUNCTION:    Reset
*
*  INPUTS:      action - Describes which reset action to take, eg. which
*                        menus to enable/disable etc...
*
*  GLOBAL VARS: rgnArray - Array of RGNSTRUCTs describing regions user has
*                          created
*               currRgn  - Index (in rgnArray) of most recently created
*                          region. If -1 then no regions created. Other-
*                          wise, values range from 0 to MAXRGNSTRUCTS - 1.
*               hMenu    - Handle of main window's menu.
*
*  COMMENTS:    Sets appropriate menu states and deletes old regions
*               according to "action" parameter.
*
\******************************************************************************/

void Reset (WORD action)
{
  UINT i;

  switch (action)
  {
    case RESET_ALL:
      if (currRgn > 0)

        for (i = 0; i <= (UINT) currRgn; i++)

          DeleteObject (rgnArray[i].hrgn);

      currRgn = -1;
      for (i = IDM_ERASE; i <= IDM_GETRGNBOX; i++)

        EnableMenuItem (GetSubMenu (hMenu, 0), i, MF_DISABLED | MF_GRAYED
                                                  | MF_BYCOMMAND);
      for (i = IDM_ELLIPSE; i <= IDM_RECT; i++)

        EnableMenuItem (GetSubMenu (hMenu, 1), i, MF_ENABLED |
                                                  MF_BYCOMMAND);
      for (i = IDM_AND; i <= IDM_XOR; i++)

        EnableMenuItem (GetSubMenu (hMenu, 2), i, MF_DISABLED | MF_GRAYED
                                                  | MF_BYCOMMAND);
      break;

   case ENABLE_OPTIONS:

      for (i = IDM_ERASE; i <= IDM_GETRGNBOX; i++)

        EnableMenuItem (GetSubMenu (hMenu, 0), i, MF_ENABLED |
                                                  MF_BYCOMMAND);
      break;

   case ENABLE_COMBINERGN:

      for (i = IDM_AND; i <= IDM_XOR; i++)

        EnableMenuItem (GetSubMenu (hMenu, 2), i, MF_ENABLED |
                                                  MF_BYCOMMAND);
      break;

   case DISABLE_COMBINERGN:

      for (i = IDM_AND; i <= IDM_XOR; i++)

        EnableMenuItem (GetSubMenu (hMenu, 2), i, MF_DISABLED | MF_GRAYED
                                                  | MF_BYCOMMAND);
      break;
  }
}


/******************************************************************************\
*
*  FUNCTION:    TrackRect()
*
*  INPUTS:      ptr    - Pointer to a PTRACKRECTSTRUCT.
*               action - Message selecting what action to take.
*               hWnd   - Window handle for the window the track rect
*                        exists within.
*               lParam - Fourth param to window proc (as with WM_MOUSEMOVE,
*                        WM_LBUTTONDOWN, WM_LBUTTONUP...) desribing
*                        event location.
*
*  RETURNS:     A pointer to a new TRACKRECTSTRUCT if msg == TRECT_NEW,
*               NULL if msg != NEW
*
*  COMMENTS:    This function provides tracking rectangle functionality.
*
\******************************************************************************/

PTRACKRECTSTRUCT TrackRect (PTRACKRECTSTRUCT ptr, int action, HWND hWnd,
                            LPARAM lParam)
{
  if ((ptr == NULL) && (action != TRECT_NEW))  return NULL;

  switch (action)
  {
    //
    // TRECT_NEW: Allocate new PTRACKRECTSTRUCT. Fill in initial values
    //            for the fields of the structure. Set up the HDC
    //            correctly.
    //
    // return - pointer to the new object.
    //

    case TRECT_NEW:
    {
      PTRACKRECTSTRUCT ptr;

      if ((ptr = LocalAlloc (LPTR, sizeof (TRACKRECTSTRUCT))) == NULL)
      {
        MessageBox (hWnd, (LPCTSTR) "REGIONS: LocalAlloc() failed", "Err!",
                    MB_OK | MB_ICONEXCLAMATION);
        return NULL;
      }

      //
      // initialize the HDC and other fields.
      //

      ptr->hdc = GetDC(hWnd);
      SetROP2(ptr->hdc, R2_NOT);
      SelectObject (ptr->hdc, GetStockObject (NULL_BRUSH));
      SelectObject (ptr->hdc, CreatePen (PS_SOLID, 2,
                   (COLORREF) 0x01000009));

      //
      // initialize the size
      //

      ptr->trackRect.left = ptr->trackRect.right =
                            (LONG) (ptr->xOrigin = LOWORD (lParam));
      ptr->trackRect.top = ptr->trackRect.bottom =
                            (LONG) (ptr->yOrigin = HIWORD (lParam));

      SetCapture (hWnd);
      GetClientRect (hWnd, &(ptr->clientRect));
      return (ptr);
    }

    //
    // TRECT_DELETE: Free up the memory allocated for the tracking rect.
    //               Also, erase the last rectangle we drew.
    //

    case  TRECT_DELETE:

      Rectangle (ptr->hdc, (int) ptr->trackRect.left,
                           (int) ptr->trackRect.top,
                           (int) ptr->trackRect.right,
                           (int) ptr->trackRect.bottom);
      ReleaseDC (hWnd, ptr->hdc);
      LocalFree (LocalHandle ((LPSTR)ptr));
      ReleaseCapture ();
      return NULL;

    //
    // TRECT_PAINT: Draw the tracking rectangle (involves erasing last
    //              rectangle at old coordinates, and drawing new rect
    //              at new coordinates).
    //

    case TRECT_PAINT:

      //
      // Remove the last rectangle we painted by painting over it again
      //

      Rectangle (ptr->hdc, (int) ptr->trackRect.left,
                           (int) ptr->trackRect.top,
                           (int) ptr->trackRect.right,
                           (int) ptr->trackRect.bottom);

      //
      // We want to restrict our Rectangle() calls to paint within the
      //   client area, so do a bounds check on lParam and reset any
      //   values which fall outside the client area (eg. <1,
      //   >clientRect.left, >clientRect.top).
      //

      if ((SHORT)LOWORD(lParam) < 1)

        lParam &= 0xffff0000;

      else if (LOWORD(lParam) > (WORD) (ptr->clientRect.right- 1))

        lParam = MAKELONG ((WORD) ptr->clientRect.right - 1,
                           HIWORD(lParam));

      if ((SHORT)HIWORD(lParam) < 1)

        lParam &= 0x0000ffff;

      else if (HIWORD(lParam) > (WORD) (ptr->clientRect.bottom - 1))

        lParam = MAKELONG (LOWORD(lParam),
                           (WORD) ptr->clientRect.bottom - 1);

      ptr->trackRect.left   = (LONG) (ptr->xOrigin > LOWORD(lParam)
                               ? LOWORD(lParam) : ptr->xOrigin);
      ptr->trackRect.right  = (LONG) (ptr->xOrigin > LOWORD(lParam)
                               ? ptr->xOrigin : LOWORD(lParam));
      ptr->trackRect.top    = (LONG) (ptr->yOrigin > HIWORD(lParam)
                               ? HIWORD(lParam) : ptr->yOrigin);
      ptr->trackRect.bottom = (LONG) (ptr->yOrigin > HIWORD(lParam)
                               ? ptr->yOrigin : HIWORD(lParam));

      //
      // Redraw the tracking rectangle
      //

      Rectangle (ptr->hdc, (int) ptr->trackRect.left,
                           (int) ptr->trackRect.top,
                           (int) ptr->trackRect.right,
                           (int) ptr->trackRect.bottom);
      return NULL;
  }
}
