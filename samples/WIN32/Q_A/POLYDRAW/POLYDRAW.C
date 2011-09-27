
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/**************************************************************************\
*  polydraw.c -- sample program demonstrating PolyDraw().
*
* The user is able to place control points on the screen, and then
*  drag them.  The PolyDraw() call is made to dynamically show the
*  resulting curve.
\**************************************************************************/

#include <windows.h>
#include "polydraw.h"



/**************************************************************************\
*
*  function:  WinMain()
*
*  input parameters:  c.f. generic sample
*
\**************************************************************************/
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    HANDLE hInst;
    HWND   hwndMain;
    MSG   msg;

    UNREFERENCED_PARAMETER( lpCmdLine );


    /* Check for previous instance.  If none, then register class. */
    if (!hPrevInstance) {
        WNDCLASS  wc;

        wc.style = CS_VREDRAW;
        wc.lpfnWndProc = (WNDPROC)MainWndProc;

        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(hInstance, "polydrawIcon");
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
        wc.lpszMenuName =  NULL;
        wc.lpszClassName = "polydraw";

        if (!RegisterClass(&wc)) return (FALSE);
    }  /* class registered o.k. */


    /* Create the main window.  Return false if CreateWindow() fails */
    hInst = hInstance;

    hwndMain = CreateWindow(
        "polydraw",
        "PolyDraw",
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (!hwndMain) return (FALSE);
    ShowWindow(hwndMain, nCmdShow);
    UpdateWindow(hwndMain);


    /* Loop getting messages and dispatching them. */
    while (GetMessage(&msg,NULL, 0,0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (msg.wParam);
}



/**************************************************************************\
*
*  function:  MainWndProc()
*
*  input parameters:  normal window procedure parameters.
*
*  global variables: none.
*
*  static variables:
*   svPoints[] - array that stores the points for the GDI calls.
*   svTypes[]  - array of bytes to store the type for the PolyDraw() calls.
*   svIndex    - when tracking, the index of the point we are modifying.
*   svNumDown  - the number of points currently in the client area.
*   hPen*     - Pens to draw the lines, curves, or erase with.
*
* The PolyDraw() function is different from PolyBezier in that the current
*  point is taken to be the first point, and the first point passed in in
*  the array is actually the second point in the bezier or line or whatever,
*  thus ...
*        PolyDraw (hdc, &svPoints[1], &svTypes[1], svNumDown);
*  instead of ...
*        PolyDraw (hdc, svPoints, svTypes, svNumDown+1);
*
\**************************************************************************/
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static HANDLE hPenGrey, hPenLine, hPenpolydraw;
static POINT svPoints [MAXPOINTS];
static BYTE  svTypes  [MAXPOINTS];
static int   svNumDown;
static int   svIndex;

  switch (message) {


    /**********************************************************************\
    *  WM_CREATE
    *
    * Initialize the static variables.
    *  Then create three pens for drawing with later.
    \**********************************************************************/
    case WM_CREATE:
        svNumDown = NONE;
        svIndex = NONE;
        hPenLine     = CreatePen (PS_SOLID, 1, (COLORREF) 0x0100000f);
        hPenpolydraw = CreatePen (PS_SOLID, 1, (COLORREF) 0x01000005);
        hPenGrey     = CreatePen (PS_SOLID, 1, (COLORREF) 0x01000007);
    break;


    /**********************************************************************\
    *  WM_DESTROY
    *
    * Complement of the WM_CREATE message.  Delete the pens that were
    *  created and then call postquitmessage.
    \**********************************************************************/
    case WM_DESTROY:
      DeleteObject (hPenLine);
      DeleteObject (hPenpolydraw);
      DeleteObject (hPenGrey);

      PostQuitMessage(0);
    break;




    /**********************************************************************\
    *  WM_LBUTTONDOWN
    *
    * Hittest to see if the user is selecting an existing point.
    *  If not, check for point overflow.
    *  If o.k. then add a new point to the current list.
    \**********************************************************************/
    case WM_LBUTTONDOWN: {
      int i;
      RECT rectNear;

      /* Construct a rectangle around the mouse point. */
      rectNear.left   = LOWORD(lParam) - HITSIZE;
      rectNear.right  = LOWORD(lParam) + HITSIZE;
      rectNear.top    = HIWORD(lParam) - HITSIZE;
      rectNear.bottom = HIWORD(lParam) + HITSIZE;

      /* step through all points so far.  hittest mouse location against
       *  each one.  If we hit one, set index, capture mouse and exit
       *  window procedure.
       */
      for (i = 0; i<= svNumDown; i++) {
        if (PtInRect (&rectNear, svPoints[i])) {
          svIndex = i;
          SetCapture (hwnd);
          return 0;
        }
      }

      /* we did not hit an old point, watch for overflow. */
      if (svNumDown == (MAXPOINTS-1)) {
        MessageBox (hwnd,
                    "Points may be dragged with left mouse button.",
                    "No more points allowed",
                    MB_OK | MB_ICONSTOP);
        return 0;
      }

      /* don't have old point, still room for more.  Put down one more. */
      if (GetCapture() != hwnd) {
        svNumDown++;
        svPoints[svNumDown].x = LOWORD(lParam);
        svPoints[svNumDown].y = HIWORD(lParam);

        /* set the type value in the array dependent upon keyboard state. */
        if (wParam & MK_SHIFT)        svTypes [svNumDown] = PT_MOVETO;
        else if (wParam & MK_CONTROL) svTypes [svNumDown] = PT_BEZIERTO;
        else                          svTypes [svNumDown] = PT_LINETO;

        InvalidateRect (hwnd, NULL, TRUE);
      }

    }break;


    /**********************************************************************\
    *  WM_MOUSEMOVE
    *
    * If the curve is tracking the mouse, the erase the old curve by drawing
    *  over it with the grey pen.  Redraw the new curves with the proper pens.
    \**********************************************************************/
    case WM_MOUSEMOVE: {
      if (GetCapture() == hwnd) {
        HDC hdc;

        hdc = GetDC (hwnd);
        MoveToEx (hdc, svPoints[0].x, svPoints[0].y, NULL);

        /* Erase the old lines and curves. */
        SelectObject(hdc, hPenGrey);
        Polyline (hdc, svPoints, svNumDown+1);
        PolyDraw (hdc, &svPoints[1], &svTypes[1], svNumDown);

        /* Change the point to match the new mouse position. */
        svPoints[svIndex].x = (int)(short)LOWORD(lParam);
        svPoints[svIndex].y = (int)(short)HIWORD(lParam);

        MoveToEx (hdc, svPoints[0].x, svPoints[0].y, NULL);
        SelectObject(hdc, hPenLine);
        Polyline (hdc, svPoints, svNumDown+1);
        SelectObject(hdc, hPenpolydraw);
        PolyDraw (hdc, &svPoints[1], &svTypes[1], svNumDown);

        ReleaseDC (hwnd, hdc);
      }
    }break;



    /**********************************************************************\
    *  WM_LBUTTONUP
    *
    * Finished tracking.  Release the mouse capture, and force repaint.
    \**********************************************************************/
    case WM_LBUTTONUP:
      if (GetCapture() == hwnd) {
        SetCapture (NULL);
        InvalidateRect (hwnd, NULL, FALSE);
      }
    break;


    /**********************************************************************\
    *  WM_RBUTTONDOWN
    *
    * Re-Initialize the static variables, and force a repaint
    \**********************************************************************/
    case WM_RBUTTONDOWN:
      svNumDown = NONE;
      svIndex = NONE;
      InvalidateRect (hwnd, NULL, TRUE);
    break;



    /**********************************************************************\
    *  WM_PAINT
    *
    * Write the instructions at the bottom of the window.  (Note that these
    *  lines may be erased by user MOUSEMOVE actions.)  Draw the polyline
    *  then draw the PolyDraw.
    \**********************************************************************/
    case WM_PAINT: {
      HDC hdc;
      PAINTSTRUCT ps;
      RECT  rect;
      int   height,x,y;

      hdc = BeginPaint(hwnd, &ps);

      /* set the background mode, the write out the 4 instruction strings. */
      SetBkMode (hdc, TRANSPARENT);
      height = GetSystemMetrics (SM_CYCAPTION);
      GetClientRect (hwnd, &rect);
      x = rect.left + 10;
      y = rect.bottom - 4*height;
      TextOut (hdc, x,y, instructions0,sizeof(instructions0)-1);
      y = rect.bottom - 3*height;
      TextOut (hdc, x,y, instructions1,sizeof(instructions1)-1);
      y = rect.bottom - 2*height;
      TextOut (hdc, x,y, instructions2,sizeof(instructions2)-1);
      y = rect.bottom - 1*height;
      TextOut (hdc, x,y, instructions3,sizeof(instructions3)-1);


      /* Move to the first point in the array and then call polydraw */
      MoveToEx (hdc, svPoints[0].x, svPoints[0].y, NULL);

      SelectObject(hdc, hPenLine);
      Polyline (hdc, svPoints, svNumDown+1);
      SelectObject(hdc, hPenpolydraw);
      PolyDraw (hdc, &svPoints[1], &svTypes[1], svNumDown);

      EndPaint (hwnd, &ps);
    } return FALSE;

  } /* end switch */
  return (DefWindowProc(hwnd, message, wParam, lParam));
}
