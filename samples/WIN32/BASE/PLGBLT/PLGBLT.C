
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/**************************************************************************\
*  plgblt.c -- sample program demonstrating the new PlgBlt() API.
*
*  design:  There is one main window with one dialog box stretched to fill
*   the top of it.  The parameters for the plgblt painted into the main
*   window are stored in the entry fields of this dialog box.  The user
*   may change these values and see the effect on the blt.
\**************************************************************************/

#include <windows.h>
#include <commdlg.h>
#include <math.h>
#include <stdio.h>
#include "plgblt.h"
#include "track.h"
#include "bitmap.h"



/* global variables. */
PTrackObject ptoDest, ptoSrc, ptoMask = NULL;
HDC          hdcDest, hdcSrc, hdcMask;
HBITMAP               hbmSrc, hbmMask = NULL;

HANDLE hInst;
HWND   hwndMain, hwndDlg;

int nSpin;


#define  BACKGROUNDBRUSH GetStockObject(LTGRAY_BRUSH)



/**************************************************************************\
*
*  function:  WinMain()
*
*  input parameters:  c.f. generic sample
*
\**************************************************************************/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
{
    MSG   msg;
    RECT rect;
    HANDLE haccel;

    UNREFERENCED_PARAMETER( lpCmdLine );


    /* Check for previous instance.  If none, then register class. */
    if (!hPrevInstance) {
        WNDCLASS  wc;

        wc.style = 0;
        wc.lpfnWndProc = (WNDPROC)MainWndProc;

        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(hInstance, "plgbltIcon");
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = BACKGROUNDBRUSH;
        wc.lpszMenuName =  "plgbltMenu";
        wc.lpszClassName = "plgblt";

        if (!RegisterClass(&wc)) return (FALSE);
    }  /* class registered o.k. */


    /* Create the main window.  Return false if CreateWindow() fails */
    hInst = hInstance;

    hwndMain = CreateWindow(
        "plgblt",
        "plgblt",
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


    /* create the top dialog as a child of the main window. */
    hwndDlg = CreateDialog (hInst, "plgbltDlg", hwndMain, (DLGPROC)DlgProc);

    /* Send main window a WM_SIZE message so that it will size the top
     *  dialog correctly.
     */
    GetClientRect (hwndMain, &rect);
    SendMessage (hwndMain, WM_SIZE, 0, (rect.right - rect.left));
    ShowWindow (hwndDlg, SW_SHOW);
    ShowWindow(hwndMain, nCmdShow);


    /* get global handle to the menu */

    /* Load the accelerator table that provides clipboard support. */
    haccel = LoadAccelerators (hInst, "bltAccel");



    /* Loop getting messages and dispatching them. */
    while (GetMessage(&msg,NULL, 0,0)) {
      if (!TranslateAccelerator(hwndMain, haccel, &msg))
      if (!IsDialogMessage (hwndDlg, &msg)){
        DispatchMessage(&msg);
      }

      /* if no messages, and we are spinning, then post spin message. */
      if (!PeekMessage (&msg, hwndMain, 0,0, PM_NOREMOVE) && nSpin)
        PostMessage (hwndMain, WM_SPIN, 0,0);
    }

    /* Return the value from PostQuitMessage */
    return (msg.wParam);
}




/**************************************************************************\
*
*  function:  MainWndProc()
*
*  input parameters:  normal window procedure parameters.
*
*  There are 6 different HDCs used for the main window (in addition to the
*   temporary one returned from BeginPaint).  There are two for each of the
*   three thirds of the window.  The first one contains the bitmap.  The
*   second one is for the track object and is stored in the TRACKOBJECT
*   structure.
*
*  global variables:
*   hwndDlg - dialog with entry fields containing parameters.
*   ptoDest, ptoSrc, ptoMask - pointers to the direct manipulation objects
*   hdcDest, hdcSrc, hdcMask - HDCs for the 3 sub regions of the window.
*   hbmSrc, hbmMask          - bitmap handles for source and mask.
\**************************************************************************/
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static int miniWidth;
static RECT rect;
static HANDLE hPenGrid, hPenSeparator;

  switch (message) {

    /**********************************************************************\
    *  WM_CREATE
    *
    * Get three new HDCs, then create three new track objects.
    *  Each track object has different allowed tracking modes.
    *  Finally create two pens for drawing later on.
    \**********************************************************************/
    case WM_CREATE:
        hdcSrc  = GetDC (hwnd);
        hdcDest = GetDC (hwnd);
        hdcMask = GetDC (hwnd);

        ptoDest = doTrackObject (NULL, TROB_NEW, hwnd,0);
        ptoDest->allowedModes = TMALL;
        ptoSrc  = doTrackObject (NULL, TROB_NEW, hwnd,0);
        ptoSrc->allowedModes = TMMOVE | TMSIZEXY;
        ptoMask = doTrackObject (NULL, TROB_NEW, hwnd,0);
        ptoMask->allowedModes = TMMOVE;

        hPenGrid      = CreatePen (PS_SOLID, 1, GRIDCOLOR);
        hPenSeparator = CreatePen (PS_SOLID, 2*SEPARATORWIDTH, (COLORREF) 0x01000000);

        { HMENU hMenu;
        hMenu = GetMenu (hwnd);
        CheckMenuItem(hMenu, IDM_SPINTOPLEFT, MF_CHECKED);
        CheckMenuItem(hMenu, IDM_SPINOFF    , MF_CHECKED);
        nSpin = FALSE;
        }

    break;


    /**********************************************************************\
    *  WM_DESTROY
    *
    * Complement of WM_CREATE.  Free up all of the HDCs, send all of the
    *  track objects their delete messages, delete the pens,
    *  then call PostQuitMessage.
    \**********************************************************************/
    case WM_DESTROY:
        ReleaseDC (hwnd, hdcSrc );
        ReleaseDC (hwnd, hdcDest);
        ReleaseDC (hwnd, hdcMask);
        doTrackObject (ptoDest, TROB_DELETE, hwnd,0);
        doTrackObject (ptoSrc , TROB_DELETE, hwnd,0);
        doTrackObject (ptoMask, TROB_DELETE, hwnd,0);

        DeleteObject(hPenGrid);
        DeleteObject(hPenSeparator);

        PostQuitMessage(0);
    break;



    /**********************************************************************\
    *  WM_SIZE
    *
    * Stretch the top dialog to fill the width of the main window.
    * Adjust the viewport origins of the 6 HDCs.
    * Set the clip regions of the 6 HDCs.
    \**********************************************************************/
    case WM_SIZE: {
        HRGN hrgn;

        SetWindowPos (hwndDlg, NULL, 0,0, LOWORD(lParam), DIALOGHEIGHT, 0);

        GetClientRect (hwndMain, &rect);
        miniWidth = rect.right/3;

        SetViewportOrgEx (hdcDest,      0,           DIALOGHEIGHT, NULL);
        SetViewportOrgEx (ptoDest->hdc, 0,           DIALOGHEIGHT, NULL);
        SetViewportOrgEx (hdcSrc,       miniWidth,   DIALOGHEIGHT, NULL);
        SetViewportOrgEx (ptoSrc->hdc,  miniWidth,   DIALOGHEIGHT, NULL);
        SetViewportOrgEx (hdcMask,      2*miniWidth, DIALOGHEIGHT, NULL);
        SetViewportOrgEx (ptoMask->hdc, 2*miniWidth, DIALOGHEIGHT, NULL);

        ptoDest->rectClip.left    = 0;
        ptoDest->rectClip.top     = DIALOGHEIGHT;
        ptoDest->rectClip.right   = miniWidth-2*SEPARATORWIDTH;
        ptoDest->rectClip.bottom  = rect.bottom;
        hrgn = CreateRectRgnIndirect (&ptoDest->rectClip);
        SelectClipRgn (hdcDest,      hrgn);
        SelectClipRgn (ptoDest->hdc, hrgn);
        DeleteObject (hrgn);

        ptoSrc->rectClip.left    = miniWidth;
        ptoSrc->rectClip.top     = DIALOGHEIGHT;
        ptoSrc->rectClip.right   = 2*miniWidth-2*SEPARATORWIDTH;
        ptoSrc->rectClip.bottom  = rect.bottom;
        hrgn = CreateRectRgnIndirect (&ptoSrc->rectClip);
        SelectClipRgn (hdcSrc,       hrgn);
        SelectClipRgn (ptoSrc->hdc,  hrgn);
        DeleteObject (hrgn);

        ptoMask->rectClip.left    = 2*miniWidth;
        ptoMask->rectClip.top     = DIALOGHEIGHT;
        ptoMask->rectClip.right   = 3*miniWidth;
        ptoMask->rectClip.bottom  = rect.bottom;
        hrgn = CreateRectRgnIndirect (&ptoMask->rectClip);
        SelectClipRgn (hdcMask,      hrgn);
        SelectClipRgn (ptoMask->hdc, hrgn);
        DeleteObject (hrgn);

        SendMessage (hwndDlg, WM_PUTUPLPOINTS, (DWORD)hdcDest, (LONG)ptoDest);
        SendMessage (hwndDlg, WM_PUTUPSRCRECT, (DWORD)hdcSrc,  (LONG)ptoSrc);
        SendMessage (hwndDlg, WM_PUTUPMASKPT,  (DWORD)hdcMask, (LONG)ptoMask);

        /* repaint the whole window. */
        InvalidateRect (hwnd, NULL, TRUE);
    } break;



    /**********************************************************************\
    *  WM_PAINT
    *
    * miniWidth, rect -- set by WM_SIZE message.
    *
    * First shift the viewport origin down so that 0,0 is the top left
    *  most visible point (out from underneath the top dialog).  Second,
    *  draw the grid with wider lines on the axes.  Finally, read the
    *  values out of the top dialog, do elementary validation, and then
    *  try to call plgblt() with the values.
    \**********************************************************************/
     case WM_PAINT: {
        HDC hdc;
        PAINTSTRUCT ps;

        hdc = BeginPaint(hwnd, &ps);

        /* Draw Separator lines for the three miniareas */
        SelectObject(hdc, hPenSeparator);
        MoveToEx (hdc,   miniWidth-SEPARATORWIDTH,0, NULL);
        LineTo   (hdc,   miniWidth-SEPARATORWIDTH, rect.bottom);
        MoveToEx (hdc, 2*miniWidth-SEPARATORWIDTH,0, NULL);
        LineTo   (hdc, 2*miniWidth-SEPARATORWIDTH, rect.bottom);

        /* Grid the HDCs */
        SelectObject(hdcSrc, hPenGrid);
        DrawGrids (hdcSrc, miniWidth, rect.bottom);
        SelectObject(hdcMask, hPenGrid);
        DrawGrids (hdcMask, miniWidth, rect.bottom);

        /* Draw bitmaps if any, then draw track objects over them. */
        if (hbmSrc)  DrawBitmap (hdcSrc, hbmSrc);
        if (hbmMask) DrawBitmap (hdcMask, hbmMask);
        doTrackObject (ptoSrc , TROB_PAINT, hwnd, 0);
        doTrackObject (ptoMask, TROB_PAINT, hwnd, 0);

        /* paint the left third of the window. */
        SendMessage (hwnd, WM_PLGBLT, 0,0);

        EndPaint (hwnd, &ps);
    } return FALSE;



    /**********************************************************************\
    *  WM_PLGBLT
    *
    * WM_USER message.  This paints the left third of the window.  It
    *  is called on the WM_PAINT message.  It is separated out here because
    *  it is common for just the plgblt() to need to be called and not the
    *  whole window painted.
    \**********************************************************************/
    case WM_PLGBLT: {
        POINT lpPoint[3];
        int XSrc, YSrc, nWidth, nHeight, XMask, YMask;
        BOOL success;
        RECT cliprect;

        doTrackObject (ptoSrc , TROB_PAINT, hwnd, 0);
        doTrackObject (ptoMask, TROB_PAINT, hwnd, 0);

        GetClipBox (hdcDest, &cliprect);
        FillRect (hdcDest, &cliprect,
                  (HBRUSH) GetClassLong (hwnd, GCL_HBRBACKGROUND));
        SelectObject(hdcDest, hPenGrid);

        DrawGrids (hdcDest, miniWidth, rect.bottom);
        if (IsWindow(hwndDlg)) {

          /* Grab points out of the dialog entry fields. */
          lpPoint[0].x = GetDlgItemInt(hwndDlg, DID_P1X, &success, TRUE);
          lpPoint[0].y = GetDlgItemInt(hwndDlg, DID_P1Y, &success, TRUE);
          lpPoint[1].x = GetDlgItemInt(hwndDlg, DID_P2X, &success, TRUE);
          lpPoint[1].y = GetDlgItemInt(hwndDlg, DID_P2Y, &success, TRUE);
          lpPoint[2].x = GetDlgItemInt(hwndDlg, DID_P3X, &success, TRUE);
          lpPoint[2].y = GetDlgItemInt(hwndDlg, DID_P3Y, &success, TRUE);
          XSrc = GetDlgItemInt(hwndDlg, DID_XSRC, &success, TRUE);
          YSrc = GetDlgItemInt(hwndDlg, DID_YSRC, &success, TRUE);
          nWidth = GetDlgItemInt(hwndDlg, DID_WIDTH, &success, TRUE);
          nHeight = GetDlgItemInt(hwndDlg, DID_HEIGHT, &success, TRUE);
          XMask = GetDlgItemInt(hwndDlg, DID_XMASK, &success, TRUE);
          YMask = GetDlgItemInt(hwndDlg, DID_YMASK, &success, TRUE);


          /**********************************************************/
          /**********************************************************/
          PlgBlt (hdcDest, lpPoint,
                  hdcSrc, XSrc, YSrc, nWidth, nHeight,
                  hbmMask, XMask, YMask);
          /**********************************************************/
          /**********************************************************/
        }
        doTrackObject (ptoSrc , TROB_PAINT, hwnd, 0);
        doTrackObject (ptoMask, TROB_PAINT, hwnd, 0);
    } break;



    /**********************************************************************\
    *  WM_LBUTTONDOWN & WM_RBUTTONDOWN
    * On button down messages, hittest on the track object, and if
    *  it returns true, then send these messages to the track object.
    \**********************************************************************/
    case WM_RBUTTONDOWN:
    case WM_LBUTTONDOWN:
      if (doTrackObject(ptoDest, TROB_HITTEST, hwnd, lParam))
         doTrackObject(ptoDest, message, hwnd, lParam);
      else if (doTrackObject(ptoSrc, TROB_HITTEST, hwnd, lParam))
         doTrackObject(ptoSrc, message, hwnd, lParam);
      else if (doTrackObject(ptoMask, TROB_HITTEST, hwnd, lParam))
         doTrackObject(ptoMask, message, hwnd, lParam);
    break;



    /**********************************************************************\
    *  WM_LBUTTONUP & WM_RBUTTONDOWN & MW_MOUSEMOVE
    * If the track object is in a "tracking mode" then send it these messages.
    *  If the transform dialog is not minimized, fill it with numbers.
    *  If the mouse dialog is not minimized, fill it with numbers.
    \**********************************************************************/
    case WM_RBUTTONUP:
    case WM_LBUTTONUP:
      /* user action complete.  Force plgblt() update. */
      PostMessage (hwndMain, WM_PLGBLT, 0,0);
    case WM_MOUSEMOVE:
      if (ptoDest->Mode) {
        doTrackObject(ptoDest, message, hwnd, lParam);
        SendMessage (hwndDlg, WM_PUTUPLPOINTS, (DWORD) hdcDest, (LONG) ptoDest);
      }
      if (ptoSrc->Mode) {
        doTrackObject(ptoSrc, message, hwnd, lParam);
        SendMessage (hwndDlg, WM_PUTUPSRCRECT, (DWORD) hdcSrc, (LONG) ptoSrc);
      }

      if (ptoMask->Mode) {
        doTrackObject(ptoMask, message, hwnd, lParam);
        SendMessage (hwndDlg, WM_PUTUPMASKPT, (DWORD) hdcMask, (LONG) ptoMask);
      }

    break;



    /**********************************************************************\
    *  WM_SETFOCUS
    *
    * The main window should never have the focus.  Set it back
    *  to the top dialog.
    \**********************************************************************/
    case WM_SETFOCUS: SetFocus (hwndDlg);
        return 0;



    /**********************************************************************\
    *  Menu item support.
    *
    \**********************************************************************/
    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        HBITMAP hbmCompat, hbmOld;
        HDC hdcCompat;

        /******************************************************************\
        *  WM_COMMAND, IDM_COPY
        *
        * Create a new bitmap, copy the destination HDC bits into it,
        *  and send the new bitmap to the clipboard.
        \******************************************************************/
        case IDM_COPY: {
          int X[4],Y[4];
          int nWidth, nHeight;
          int Xmin, Ymin, Xmax, Ymax;
          BOOL success;
          int i;

          for (i = 0; i<3 ; i++) {
            X[i] = GetDlgItemInt(hwndDlg, DID_P1X + 2*i, &success, TRUE);
            Y[i] = GetDlgItemInt(hwndDlg, DID_P1Y + 2*i, &success, TRUE);
          }

          X[3] = (X[1] - X[0]) + X[2];
          Y[3] = (Y[2] - Y[0]) + Y[1];

          Xmin = Xmax = X[0];
          Ymin = Ymax = Y[0];

          for (i = 1; i<4 ; i++) {
            Xmin = (X[i] < Xmin) ? X[i] : Xmin;
            Ymin = (Y[i] < Ymin) ? Y[i] : Ymin;
            Xmax = (X[i] > Xmax) ? X[i] : Xmax;
            Ymax = (Y[i] > Ymax) ? Y[i] : Ymax;
          }

          nWidth = Xmax - Xmin;
          nHeight = Ymax - Ymin;

          hdcCompat = CreateCompatibleDC(hdcDest);
          hbmCompat = CreateCompatibleBitmap (hdcDest, nWidth, nHeight);
          hbmOld = SelectObject(hdcCompat,hbmCompat);

          BitBlt (hdcCompat, 0,0,nWidth, nHeight, hdcDest, Xmin,Ymin, SRCCOPY );

          SelectObject(hdcCompat,hbmOld);
          DeleteDC(hdcCompat);

          OpenClipboard (hwnd);
          SetClipboardData (CF_BITMAP,hbmCompat);
          CloseClipboard ();

          DeleteObject (hbmCompat);

        } break;


        /******************************************************************\
        *  WM_COMMAND, IDM_PASTE
        *
        * Get bitmap handle from clipboard, create a new bitmap, draw
        *  the clipboard bitmap into the new one, and store the new
        *  handle in the global hbmSrc.
        \******************************************************************/
        case IDM_PASTE: {
          HBITMAP hbm;
          BITMAP bm;

          OpenClipboard (hwnd);
          if ( hbm = GetClipboardData (CF_BITMAP)) {
            DeleteObject (hbmSrc);

            GetObject (hbm, sizeof(BITMAP), &bm);

            hdcCompat = CreateCompatibleDC(hdcDest);
            hbmCompat = CreateCompatibleBitmap (hdcDest, bm.bmWidth, bm.bmHeight);
            hbmOld = SelectObject(hdcCompat,hbmCompat);

            DrawBitmap (hdcCompat, hbm);

            SelectObject(hdcCompat,hbmOld);
            DeleteDC(hdcCompat);

            hbmSrc = hbmCompat;

            InvalidateRect (hwnd, &ptoSrc->rectClip, TRUE);
            InvalidateRect (hwnd, &ptoDest->rectClip, TRUE);
          }
          CloseClipboard ();
        } break;

        /******************************************************************\
        *  WM_COMMAND, IDM_BOTH
        *
        * Post a COPY and PASTE command message to this window so that with
        *  one key stroke the user can copy the DEST image into the clipboard,
        *  paste it down into the SRC hdc and cause the blt.
        \******************************************************************/
        case IDM_BOTH:

          PostMessage (hwnd, WM_COMMAND, MAKELONG (IDM_COPY , 1), 0);
          PostMessage (hwnd, WM_COMMAND, MAKELONG (IDM_PASTE, 1), 0);

        break;



        /******************************************************************\
        *  WM_COMMAND, IDM_MODE_*
        *
        * manage mutually exclusive menu.
        *  call SetStretchBltMode() for the global destination hdc.
        \******************************************************************/
        case IDM_MODE_BLACKONWHITE:
          { HMENU hMenu;
          hMenu = GetMenu (hwnd);

          CheckMenuItem(hMenu, IDM_MODE_BLACKONWHITE, MF_CHECKED);
          CheckMenuItem(hMenu, IDM_MODE_COLORONCOLOR, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_MODE_WHITEONBLACK, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_MODE_HALFTONE    , MF_UNCHECKED);

          SetStretchBltMode (hdcDest,BLACKONWHITE);
          SendMessage (hwndMain, WM_PLGBLT, 0,0);

          } return 0;

        case IDM_MODE_COLORONCOLOR:
          { HMENU hMenu;
          hMenu = GetMenu (hwnd);

          CheckMenuItem(hMenu, IDM_MODE_BLACKONWHITE, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_MODE_COLORONCOLOR, MF_CHECKED);
          CheckMenuItem(hMenu, IDM_MODE_WHITEONBLACK, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_MODE_HALFTONE    , MF_UNCHECKED);

          SetStretchBltMode (hdcDest,COLORONCOLOR);
          SendMessage (hwndMain, WM_PLGBLT, 0,0);

          } return 0;

        case IDM_MODE_WHITEONBLACK:
          { HMENU hMenu;
          hMenu = GetMenu (hwnd);

          CheckMenuItem(hMenu, IDM_MODE_BLACKONWHITE, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_MODE_COLORONCOLOR, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_MODE_WHITEONBLACK, MF_CHECKED);
          CheckMenuItem(hMenu, IDM_MODE_HALFTONE    , MF_UNCHECKED);

          SetStretchBltMode (hdcDest,WHITEONBLACK);
          SendMessage (hwndMain, WM_PLGBLT, 0,0);

          } return 0;

        case IDM_MODE_HALFTONE    :
          { HMENU hMenu;
          hMenu = GetMenu (hwnd);

          CheckMenuItem(hMenu, IDM_MODE_BLACKONWHITE, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_MODE_COLORONCOLOR, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_MODE_WHITEONBLACK, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_MODE_HALFTONE    , MF_CHECKED);

          SetStretchBltMode (hdcDest,HALFTONE);
          SendMessage (hwndMain, WM_PLGBLT, 0,0);

          } return 0;





        /******************************************************************\
        *  WM_COMMAND, IDM_SPIN*
        *
        * manage mutually exclusive menu.
        *
        \******************************************************************/
        case IDM_SPINOFF:
          { HMENU hMenu;
          hMenu = GetMenu (hwnd);

          CheckMenuItem(hMenu, IDM_SPINOFF, MF_CHECKED);
          CheckMenuItem(hMenu, IDM_SPIN5  , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN10 , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN30 , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN60 , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN90 , MF_UNCHECKED);

          nSpin = FALSE;
          SendMessage (hwndMain, WM_PLGBLT, 0,0);

          } return 0;

        case IDM_SPIN5 :
          { HMENU hMenu;
          hMenu = GetMenu (hwnd);

          CheckMenuItem(hMenu, IDM_SPINOFF, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN5  , MF_CHECKED);
          CheckMenuItem(hMenu, IDM_SPIN10 , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN30 , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN60 , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN90 , MF_UNCHECKED);

          nSpin = 5;

          } return 0;

        case IDM_SPIN10:
          { HMENU hMenu;
          hMenu = GetMenu (hwnd);

          CheckMenuItem(hMenu, IDM_SPINOFF, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN5  , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN10 , MF_CHECKED);
          CheckMenuItem(hMenu, IDM_SPIN30 , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN60 , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN90 , MF_UNCHECKED);

          nSpin = 10;

          } return 0;

        case IDM_SPIN30:
          { HMENU hMenu;
          hMenu = GetMenu (hwnd);

          CheckMenuItem(hMenu, IDM_SPINOFF, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN5  , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN10 , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN30 , MF_CHECKED);
          CheckMenuItem(hMenu, IDM_SPIN60 , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN90 , MF_UNCHECKED);

          nSpin = 30;

          } return 0;

        case IDM_SPIN60:
          { HMENU hMenu;
          hMenu = GetMenu (hwnd);

          CheckMenuItem(hMenu, IDM_SPINOFF, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN5  , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN10 , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN30 , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN60 , MF_CHECKED);
          CheckMenuItem(hMenu, IDM_SPIN90 , MF_UNCHECKED);

          nSpin = 60;

          } return 0;

        case IDM_SPIN90:
          { HMENU hMenu;
          hMenu = GetMenu (hwnd);

          CheckMenuItem(hMenu, IDM_SPINOFF, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN5  , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN10 , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN30 , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN60 , MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPIN90 , MF_CHECKED);

          nSpin = 90;

          } return 0;


        case IDM_FLIPONCE:
          nSpin = 90;
          SendMessage (hwndMain, WM_SPIN, 0,0);
          nSpin = FALSE;
          return 0;



        case IDM_SPINTOPLEFT:
          { HMENU hMenu;
          hMenu = GetMenu (hwnd);

          CheckMenuItem(hMenu, IDM_SPINTOPLEFT, MF_CHECKED);
          CheckMenuItem(hMenu, IDM_SPINCENTER , MF_UNCHECKED);

          } return 0;

        case IDM_SPINCENTER:
          { HMENU hMenu;
          hMenu = GetMenu (hwnd);

          CheckMenuItem(hMenu, IDM_SPINTOPLEFT, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_SPINCENTER , MF_CHECKED);

          } return 0;


        /******************************************************************\
        *  WM_COMMAND, DID_NEW*
        *
        * menu equivalents for buttons on top dialog.  Just pass along
        *  WM_COMMAND messages to the dialog.
        *
        \******************************************************************/
        case DID_NEWSRC:
        case DID_NEWMASK:
          SendMessage (hwndDlg, message, wParam, lParam);
        return 0;


        /******************************************************************\
        *  WM_COMMAND, IDM_ABOUT
        *
        \******************************************************************/
        case IDM_ABOUT:
          DialogBox (hInst, "aboutBox", hwnd, (DLGPROC)About);
        return 0;




      }  /* end switch */

    break;  /* end wm_command */



    /******************************************************************\
    *  WM_SPIN
    *
    * Set up a transform to modify the destination points.
    *  (Note that the transform in hdcDest remains the identity.)
    *  Loop through, reblitting to the transformed points.
    *  Erase behind the old bitmap by keeping track of the region uncovered.
    *
    \******************************************************************/
    case WM_SPIN: {

      XFORM  x;
      HDC hdc;
      float M11, M12, M21, M22;
      int nSteps, i;

      POINT pivot;
      POINT lpPoint[3];
      POINT lpRgnErase[4], lpRgnBmp[4];
      HRGN hrgnErase, hrgnBmp;
      HMENU hMenu;
      int XSrc, YSrc, nWidth, nHeight, XMask, YMask;
      BOOL success;

      /* validate the dialog on top with the parameters in it. */
      if (!IsWindow(hwndDlg)) return 0;

      /* Grab points out of the dialog entry fields. */
      lpPoint[0].x = GetDlgItemInt(hwndDlg, DID_P1X, &success, TRUE);
      lpPoint[0].y = GetDlgItemInt(hwndDlg, DID_P1Y, &success, TRUE);
      lpPoint[1].x = GetDlgItemInt(hwndDlg, DID_P2X, &success, TRUE);
      lpPoint[1].y = GetDlgItemInt(hwndDlg, DID_P2Y, &success, TRUE);
      lpPoint[2].x = GetDlgItemInt(hwndDlg, DID_P3X, &success, TRUE);
      lpPoint[2].y = GetDlgItemInt(hwndDlg, DID_P3Y, &success, TRUE);
      XSrc = GetDlgItemInt(hwndDlg, DID_XSRC, &success, TRUE);
      YSrc = GetDlgItemInt(hwndDlg, DID_YSRC, &success, TRUE);
      nWidth = GetDlgItemInt(hwndDlg, DID_WIDTH, &success, TRUE);
      nHeight = GetDlgItemInt(hwndDlg, DID_HEIGHT, &success, TRUE);
      XMask = GetDlgItemInt(hwndDlg, DID_XMASK, &success, TRUE);
      YMask = GetDlgItemInt(hwndDlg, DID_YMASK, &success, TRUE);


      /* get an HDC we can use to play with transforms. */
      hdc = GetDC (hwnd);
      SetGraphicsMode (hdc, GM_ADVANCED);



      /* check menu check to pivot on top-left corner, or pivot on center. */
      hMenu = GetMenu (hwnd);

      if (GetMenuState(hMenu, IDM_SPINCENTER, MF_BYCOMMAND) & MF_CHECKED) {
        pivot.x = (lpPoint[1].x +lpPoint[2].x)/2;
        pivot.y = (lpPoint[1].y +lpPoint[2].y)/2;
      } else {
        pivot.x = lpPoint[0].x;
        pivot.y = lpPoint[0].y;
      }

      /* nSpin contains values reflecting the number of degrees per step.
       *  fill in the number of steps required (360 / nSpin), and fill in
       *  the precomputed transformation matrices.
       */
      switch (nSpin) {

        case 5:
          nSteps = 72;
          M11 =                   M22 = (float)0.9961946980917;
          M12 = (float)-0.08715574274766; M21 = (float)0.08715574274766;
          break;
        case 10:
          nSteps = 36;
          M11 =                   M22 = (float)0.984808;
          M12 = (float)-0.173648; M21 = (float)0.173648;
          break;
        case 30:
          nSteps = 12;
          M11 =                   M22 = (float)0.866025;
          M12 = (float)-0.5     ; M21 = (float)0.5;
          break;
        case 60:
          nSteps = 6;
          M11 =                   M22 = (float)0.5     ;
          M12 = (float)-0.866025; M21 = (float)0.866025;
          break;
        case 90:
          nSteps = 4;
          M11 =                   M22 = (float)0.0;
          M12 = (float)-1.0     ; M21 = (float)1.0;
          break;
        default:
          MessageBox (hwnd, "nSpin invalid.", "Internal app error.", MB_ICONHAND);
          return 0;

      } /* end switch nSpin */




      /* translate objects from pivot point to origin. */
      x.eM11 = (float)1.0;
      x.eM12 = (float)0.0;
      x.eM21 = (float)0.0;
      x.eM22 = (float)1.0;
      x.eDx = (float)-pivot.x;
      x.eDy = (float)-pivot.y;
      ModifyWorldTransform (hdc, &x, MWT_RIGHTMULTIPLY);

      /* rotate object about origin. */
      x.eM11 = M11;
      x.eM12 = M12;
      x.eM21 = M21;
      x.eM22 = M22;
      x.eDx = (float)0.0;
      x.eDy = (float)0.0;
      ModifyWorldTransform (hdc, &x, MWT_RIGHTMULTIPLY);

      /* translate objects back to pivot point. */
      x.eM11 = (float)1.0;
      x.eM12 = (float)0.0;
      x.eM21 = (float)0.0;
      x.eM22 = (float)1.0;
      x.eDx = (float)pivot.x;
      x.eDy = (float)pivot.y;
      ModifyWorldTransform (hdc, &x, MWT_RIGHTMULTIPLY);



      /* fill in initial region for erasure... the region containing bmp */
      lpRgnErase[0] = lpPoint[0];
      lpRgnErase[1] = lpPoint[1];
      lpRgnErase[3] = lpPoint[2];
      lpRgnErase[2].x =  lpPoint[1].x - lpPoint[0].x;
      lpRgnErase[2].x += lpPoint[2].x - lpPoint[0].x;
      lpRgnErase[2].x += lpPoint[0].x;
      lpRgnErase[2].y =  lpPoint[1].y - lpPoint[0].y;
      lpRgnErase[2].y += lpPoint[2].y - lpPoint[0].y;
      lpRgnErase[2].y += lpPoint[0].y;



      /* loop through transforming the points on each step. */
      for (i= 0; i<nSteps; i++) {


        hrgnErase = CreatePolygonRgn (lpRgnErase, 4, ALTERNATE);

        /* TRANSFORM the lpPoint[] destination extent points */
        LPtoDP (hdc, lpPoint, 3);

        /* create a region containing the new bitmap extents */
        lpRgnBmp[0] = lpPoint[0];
        lpRgnBmp[1] = lpPoint[1];
        lpRgnBmp[3] = lpPoint[2];
        lpRgnBmp[2].x =  lpPoint[1].x - lpPoint[0].x;
        lpRgnBmp[2].x += lpPoint[2].x - lpPoint[0].x;
        lpRgnBmp[2].x += lpPoint[0].x;
        lpRgnBmp[2].y =  lpPoint[1].y - lpPoint[0].y;
        lpRgnBmp[2].y += lpPoint[2].y - lpPoint[0].y;
        lpRgnBmp[2].y += lpPoint[0].y;
        hrgnBmp = CreatePolygonRgn (lpRgnBmp, 4, ALTERNATE);

        /* subtract the new bitmap extents region from the erasure region. */
        CombineRgn (hrgnErase, hrgnErase, hrgnBmp, RGN_DIFF);



        /* while we are here, get points ready for the next loop erasure */
        lpRgnErase[0] = lpPoint[0];
        lpRgnErase[1] = lpPoint[1];
        lpRgnErase[3] = lpPoint[2];
        lpRgnErase[2].x =  lpPoint[1].x - lpPoint[0].x;
        lpRgnErase[2].x += lpPoint[2].x - lpPoint[0].x;
        lpRgnErase[2].x += lpPoint[0].x;
        lpRgnErase[2].y =  lpPoint[1].y - lpPoint[0].y;
        lpRgnErase[2].y += lpPoint[2].y - lpPoint[0].y;
        lpRgnErase[2].y += lpPoint[0].y;



        /**********************************************************/
        PlgBlt (hdcDest, lpPoint,
                hdcSrc, XSrc, YSrc, nWidth, nHeight,
                hbmMask, XMask, YMask);
        /**********************************************************/

        /* need to flush graphics buffer, or regions are erased
         *  before the bitmap is drawn.
         */
        GdiFlush();

        /* erase the newly uncovered region. */
        FillRgn (hdcDest, hrgnErase, BACKGROUNDBRUSH );
        DeleteObject (hrgnErase);
        DeleteObject (hrgnBmp);
      } /* end for loop */


      /* because of roundoff error, the 'nSteps'th rotation will not
       *  always bring the parallelogram around to the 0th position.
       *  So we special case the last position, and do one more erase.
       *  Try commenting this out, and see the little glitches left
       *  on the screen if this comment is unclear.
       */
      lpRgnErase[0] = lpPoint[0];
      lpRgnErase[1] = lpPoint[1];
      lpRgnErase[3] = lpPoint[2];
      lpRgnErase[2].x =  lpPoint[1].x - lpPoint[0].x;
      lpRgnErase[2].x += lpPoint[2].x - lpPoint[0].x;
      lpRgnErase[2].x += lpPoint[0].x;
      lpRgnErase[2].y =  lpPoint[1].y - lpPoint[0].y;
      lpRgnErase[2].y += lpPoint[2].y - lpPoint[0].y;
      lpRgnErase[2].y += lpPoint[0].y;
      hrgnErase = CreatePolygonRgn (lpRgnErase, 4, ALTERNATE);
      lpPoint[0].x = GetDlgItemInt(hwndDlg, DID_P1X, &success, TRUE);
      lpPoint[0].y = GetDlgItemInt(hwndDlg, DID_P1Y, &success, TRUE);
      lpPoint[1].x = GetDlgItemInt(hwndDlg, DID_P2X, &success, TRUE);
      lpPoint[1].y = GetDlgItemInt(hwndDlg, DID_P2Y, &success, TRUE);
      lpPoint[2].x = GetDlgItemInt(hwndDlg, DID_P3X, &success, TRUE);
      lpPoint[2].y = GetDlgItemInt(hwndDlg, DID_P3Y, &success, TRUE);
      lpRgnBmp[0] = lpPoint[0];
      lpRgnBmp[1] = lpPoint[1];
      lpRgnBmp[3] = lpPoint[2];
      lpRgnBmp[2].x =  lpPoint[1].x - lpPoint[0].x;
      lpRgnBmp[2].x += lpPoint[2].x - lpPoint[0].x;
      lpRgnBmp[2].x += lpPoint[0].x;
      lpRgnBmp[2].y =  lpPoint[1].y - lpPoint[0].y;
      lpRgnBmp[2].y += lpPoint[2].y - lpPoint[0].y;
      lpRgnBmp[2].y += lpPoint[0].y;
      hrgnBmp = CreatePolygonRgn (lpRgnBmp, 4, ALTERNATE);
      CombineRgn (hrgnErase, hrgnErase, hrgnBmp, RGN_DIFF);
      FillRgn (hdcDest, hrgnErase, BACKGROUNDBRUSH );
      DeleteObject (hrgnErase);
      DeleteObject (hrgnBmp);







      ReleaseDC (hwnd, hdc);


    } return 0; /* end WM_SPIN message */


  } /* end switch */
  return (DefWindowProc(hwnd, message, wParam, lParam));
}




/**************************************************************************\
*
*  function:  DlgProc()
*
*  input parameters:  normal window procedure parameters.
*
*  Respond to user button presses by getting new bitmaps or by sending
*   the main window a WM_PLGBLT message.  Also handle special user messages
*   for updating the entry fields with the contents of the direct manipulation
*   objects.
*
*  global variables:
*   hwndMain - the main window.  also the parent of this dialog
*   ptoDest, ptoSrc, ptoMask - pointers to the direct manipulation objects
*   hdcDest, hdcSrc, hdcMask - HDCs for the 3 sub regions of the window.
*   hbmSrc, hbmMask          - bitmap handles for source and mask.
\**************************************************************************/
LRESULT CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

  switch (message) {
    /**********************************************************************\
    *  WM_INITDIALOG
    *
    * Fill the entry fields with sensible original values.
    \**********************************************************************/
    case WM_INITDIALOG:
        SetDlgItemText(hwnd, DID_P1X     , "0");
        SetDlgItemText(hwnd, DID_P1Y     , "0");
        SetDlgItemText(hwnd, DID_P2X     , "0");
        SetDlgItemText(hwnd, DID_P2Y     , "0");
        SetDlgItemText(hwnd, DID_P3X     , "0");
        SetDlgItemText(hwnd, DID_P3Y     , "0");
        SetDlgItemText(hwnd, DID_XSRC    , "0");
        SetDlgItemText(hwnd, DID_YSRC    , "0");
        SetDlgItemText(hwnd, DID_WIDTH   , "0");
        SetDlgItemText(hwnd, DID_HEIGHT  , "0");
        SetDlgItemText(hwnd, DID_XMASK   , "0");
        SetDlgItemText(hwnd, DID_YMASK   , "0");
    return TRUE;



    /**********************************************************************\
    *  WM_PUTUPLPOINTS
    *
    * wParam -  HDC with the needed world transform.
    * lParam -  Pointer to the track object.
    *
    * Fill the entry fields for the array of 3 dest parallelogram points.
    *  Conditionally change the first point depending on tracking mode.
    \**********************************************************************/
    case WM_PUTUPLPOINTS: {
        POINT p, origin;
        PTrackObject pto;
        HDC hdc;

        hdc = (HDC) wParam;
        pto = (PTrackObject) lParam;
        GetViewportOrgEx (hdc, &origin);

        if (pto->Mode & TMMOVE) {
          p.x = pto->rect.left;
          p.y = pto->rect.top;
          LPtoDP (pto->hdc, &p, 1);
          p.x -= origin.x; p.y -= origin.y;

          SetDlgItemInt(hwnd, DID_P1X, p.x, TRUE);
          SetDlgItemInt(hwnd, DID_P1Y, p.y, TRUE);

        }

        p.x = pto->rect.right;
        p.y = pto->rect.top;
        LPtoDP (pto->hdc, &p, 1);
        p.x -= origin.x; p.y -= origin.y;

        SetDlgItemInt(hwnd, DID_P2X, p.x, TRUE);
        SetDlgItemInt(hwnd, DID_P2Y, p.y, TRUE);

        p.x = pto->rect.left;
        p.y = pto->rect.bottom;
        LPtoDP (pto->hdc, &p, 1);
        p.x -= origin.x; p.y -= origin.y;
        SetDlgItemInt(hwnd, DID_P3X, p.x, TRUE);
        SetDlgItemInt(hwnd, DID_P3Y, p.y, TRUE);

    } return FALSE;




    /**********************************************************************\
    *  WM_PUTUPSRCRECT
    *
    * wParam -  HDC with the needed world transform.
    * lParam -  Pointer to the track object.
    *
    * Fill the entry fields for the source rectangle points.
    *  Conditionally change <x,y> or <width,height> depending on tracking mode.
    \**********************************************************************/
    case WM_PUTUPSRCRECT: {
        POINT p1,p2, origin;
        PTrackObject pto;
        HDC hdc;

        hdc = (HDC) wParam;
        pto = (PTrackObject) lParam;
        GetViewportOrgEx (hdc, &origin);

        p1.x = pto->rect.left;
        p1.y = pto->rect.top;
        LPtoDP (pto->hdc, &p1, 1);

        p2.x = pto->rect.right;
        p2.y = pto->rect.bottom;
        LPtoDP (pto->hdc, &p2, 1);
        p2.x -= p1.x; p2.y -= p1.y;

        p1.x -= origin.x; p1.y -= origin.y;

        if (!(pto->Mode & TMSIZEXY)) {
          SetDlgItemInt(hwnd, DID_XSRC, p1.x, TRUE);
          SetDlgItemInt(hwnd, DID_YSRC, p1.y, TRUE);
        }

        if (!(pto->Mode & TMMOVE)) {
          SetDlgItemInt(hwnd, DID_WIDTH,  p2.x, TRUE);
          SetDlgItemInt(hwnd, DID_HEIGHT, p2.y, TRUE);
        }
    } return FALSE;



    /**********************************************************************\
    *  WM_PUTUPMASKPT
    *
    * wParam -  HDC with the needed world transform.
    * lParam -  Pointer to the track object.
    *
    * Fill the entry fields for the mask location point.
    \**********************************************************************/
    case WM_PUTUPMASKPT: {
        POINT p1, origin;
        PTrackObject pto;
        HDC hdc;

        hdc = (HDC) wParam;
        pto = (PTrackObject) lParam;
        GetViewportOrgEx (hdc, &origin);

        p1.x = pto->rect.left;
        p1.y = pto->rect.top;
        LPtoDP (pto->hdc, &p1, 1);
        p1.x -= origin.x; p1.y -= origin.y;

        SetDlgItemInt(hwnd, DID_XMASK, p1.x, TRUE);
        SetDlgItemInt(hwnd, DID_YMASK, p1.y, TRUE);

    } return FALSE;




    /**********************************************************************\
    *  WM_COMMAND, DID_DRAW
    *
    * Draw button hit - send main window message to call PlgBlt().
    \**********************************************************************/
    case WM_COMMAND: {
      HBITMAP hbm;

      if (LOWORD(wParam) == DID_DRAW) {
          SendMessage (hwndMain, WM_PLGBLT, 0,0);


    /**********************************************************************\
    *  WM_COMMAND, DID_NEWSRC
    *
    * Try to get a new source bitmap.  Then
    *  invalidate two sub windows so that we force a repaint.
    \**********************************************************************/
      } else if (LOWORD(wParam) == DID_NEWSRC) {
        if ( hbm = GetBitmap (hdcSrc, hInst, FALSE)) {
          DeleteObject (hbmSrc);
          hbmSrc = hbm;
          InvalidateRect (hwndMain, &ptoSrc->rectClip, TRUE);
          InvalidateRect (hwndMain, &ptoDest->rectClip, TRUE);
        }

    /**********************************************************************\
    *  WM_COMMAND, DID_Mask
    *
    * Try to get a new mask bitmap.  Then
    *  invalidate two sub windows so that we force a repaint.
    \**********************************************************************/
      } else if (LOWORD(wParam) == DID_NEWMASK) {
        if ( hbm = GetBitmap (hdcMask,  hInst, TRUE)) {
          DeleteObject (hbmMask);
          hbmMask = hbm;
          InvalidateRect (hwndMain, &ptoMask->rectClip, TRUE);
          InvalidateRect (hwndMain, &ptoDest->rectClip, TRUE);
        }
      }

    } return FALSE; /* end WM_COMMAND */


  } /* end switch */
  return 0;
}



#define TICKSPACE     20

/**************************************************************************\
*
*  function:  DrawGrids()
*
*  input parameters:
*   hdc - Device context to draw into.
*   width, height - size of the rectangle to fill with grids.
*
*  global variables:  none.
*
\**************************************************************************/
VOID DrawGrids (HDC hdc, int width, int height)
{
int i;

    /* Draw vertical lines. Double at the axis */
    for (i = 0; i<= width; i+=TICKSPACE){
      MoveToEx (hdc, i, 0, NULL);
      LineTo (hdc, i, height);
    }
    MoveToEx (hdc, 1, 0, NULL);
    LineTo (hdc, 1, height);

    /* Draw horizontal lines. Double at the axis */
    for (i = 0; i<= height; i+=TICKSPACE){
      MoveToEx (hdc, 0,i, NULL);
      LineTo (hdc, width,i);
    }
    MoveToEx (hdc, 0, 1, NULL);
    LineTo (hdc, width,1);

  return;
}




/****************************************************************************
    FUNCTION: About
****************************************************************************/
LRESULT CALLBACK About(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if ((message == WM_COMMAND) && (LOWORD(wParam) == IDOK)) {
    EndDialog (hwnd, TRUE);
    return TRUE;
  }
  if ((message == WM_SYSCOMMAND) && (wParam == SC_CLOSE)) {
    EndDialog (hwnd, TRUE);
    return TRUE;
  }
  return FALSE;
}
