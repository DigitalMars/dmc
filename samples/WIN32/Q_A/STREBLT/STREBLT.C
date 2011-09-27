
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
*  streblt.c -- sample program demonstrating StretchBlt()
*
*  design:  There is one main window with one dialog box streblted to fill
*   the top of it.  The parameters for the StretchBlt() are stored in the
*   entry fields of this dialog box.  The user may change these values and
*   see the effect on the blt.  The top dialog also offers a chance to set
*   the streblt blt mode, to select pattern brushes for the hdc, and to
*   select from standard raster operations
\**************************************************************************/


#include <windows.h>
#include <commdlg.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "streblt.h"
#include "track.h"
#include "bitmap.h"


/* Global variables */
HANDLE hInst;
HWND   hwndMain, hwndDlg;

PTrackObject ptoSrc, ptoDest = NULL;
HDC          hdcSrc, hdcDest;
HBITMAP      hbmSrc = NULL;

#define NONE -1
int strebltMode = NONE;
int iPatternBrush = NONE;





/**************************************************************************\
*
*  function:  WinMain()
*
*  input parameters:  c.f. generic sample
*
\**************************************************************************/
int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow)
{
    MSG   msg;
    RECT rect;
    HANDLE haccel;
    DWORD dwVersion;

    UNREFERENCED_PARAMETER( lpCmdLine );

    // check if running on Windows NT, if not, display notice and terminate
    dwVersion = GetVersion();
    if( !(dwVersion < 0x80000000) && (LOBYTE(LOWORD(dwVersion)) < 4))
    {
        MessageBoxA( NULL,
            "This sample application cannot be run on Windows 3.1.\n"
            "This application will now terminate.",
            "StreBlt",
            MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
        return( 1 );
    }

    /* Check for previous instance.  If none, then register class. */
    if (!hPrevInstance) {
        WNDCLASS  wc;

        wc.style = 0;
        wc.lpfnWndProc = (WNDPROC)MainWndProc;

        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(hInstance, "strebltIcon");
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
        wc.lpszMenuName =  NULL;
        wc.lpszClassName = "streblt";

        if (!RegisterClass(&wc)) return (FALSE);
    }  /* class registered o.k. */


    /* Create the main window.  Return false if CreateWindow() fails */
    hInst = hInstance;

    hwndMain = CreateWindow(
        "streblt",
        "StretchBlt()",
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
    hwndDlg = CreateDialog (hInst, "strebltDlg", hwndMain, (DLGPROC)DlgProc);

    /* Send main window a WM_SIZE message so that it will size the top
     *  dialog correctly.  Also, force a repaint of the main window
     *  now that the dialog is there and we can draw the arc.
     */
    GetClientRect (hwndMain, &rect);
    SendMessage (hwndMain, WM_SIZE, 0, (rect.right - rect.left));
    ShowWindow (hwndDlg, SW_SHOW);
    ShowWindow(hwndMain, nCmdShow);


    /* Load the accelerator table that provides clipboard support. */
    haccel = LoadAccelerators (hInst, "bltAccel");


    /* Loop getting messages and dispatching them. */
    while (GetMessage(&msg,NULL, 0,0)) {
      if (!TranslateAccelerator(hwndMain, haccel, &msg))
      if (!IsDialogMessage (hwndDlg, &msg)) {
        DispatchMessage(&msg);
      }
    }

    return (msg.wParam);           /* Returns the value from PostQuitMessage */
}




/**************************************************************************\
*
*  function:  MainWndProc()
*
*  input parameters:  normal window procedure parameters.
*
*  This is the main window window procedure.  Allocate needed structures
*   at create time and free them at destroy time.  Pass the mouse messages
*   down to the track objects when appropriate.  Size the top dialog on
*   size messages.  The window is painted in two halfs.  One for the source
*   region and one for the destination region.  The StretchBlt() call is
*   separated out as a user message so that the call can be made without
*   erasing the whole window.
*
*  global variables:
*   hwndDlg  - dialog with entry fields containing arc parameters.
*   ptoSrc   - pointer to the source track object
*   hdcSrc   - device context for the right half the window.
*   ptoDest  - pointer to the destination track object
*   hdcDest  - device context for the left half the window.
*   hbmSrc   - bitmap handle for the source bitmap.
*
*   strebltMode - Selection from combo box, to be used in SetStretchBltMode()
*   iPatternBrush - Selection from combo box, set into the hdc.
\**************************************************************************/
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static int miniWidth;
  static RECT rect;
  char buffer[MAXCHARS];
  static HANDLE hPenGrid, hPenSeparator;


  switch (message) {

    /**********************************************************************\
    *  WM_CREATE
    *
    * Get the two hdc's for this window, allocate two track objects,
    *  and then create two pens to be used when painting the windows.
    \**********************************************************************/
    case WM_CREATE:
      hdcSrc  = GetDC (hwnd);
      hdcDest = GetDC (hwnd);

      ptoDest = doTrackObject (NULL, TROB_NEW, hwnd,0);
      ptoSrc  = doTrackObject (NULL, TROB_NEW, hwnd,0);

      hPenGrid      = CreatePen (PS_SOLID, 1, GRIDCOLOR);
      hPenSeparator = CreatePen (PS_SOLID, 2*SEPARATORWIDTH, (COLORREF) 0x01000000);
    break;


    /**********************************************************************\
    *  WM_DESTROY
    *
    * Complement of WM_CREATE.  Free up all of the HDCs, send all of the
    *  track objects their delete messages, then call PostQuitMessage.
    \**********************************************************************/
    case WM_DESTROY:
      ReleaseDC (hwnd, hdcSrc );
      ReleaseDC (hwnd, hdcDest);
      doTrackObject (ptoDest, TROB_DELETE, hwnd,0);
      doTrackObject (ptoSrc , TROB_DELETE, hwnd,0);

      DeleteObject(hPenGrid);
      DeleteObject(hPenSeparator);

      PostQuitMessage(0);
    break;



    /**********************************************************************\
    *  WM_SIZE
    *
    * streblt the top dialog to fill the width of the main window.
    * Adjust the viewport origins of the 4 HDCs.
    * Set the clip regions of the 4 HDCs.
    \**********************************************************************/
    case WM_SIZE: {
        HRGN hrgn;

        SetWindowPos (hwndDlg, NULL, 0,0, LOWORD(lParam), DIALOGHEIGHT, 0);

        GetClientRect (hwndMain, &rect);
        miniWidth = rect.right/2;

        SetViewportOrgEx (hdcDest,      0,           DIALOGHEIGHT, NULL);
        SetViewportOrgEx (ptoDest->hdc, 0,           DIALOGHEIGHT, NULL);
        SetViewportOrgEx (hdcSrc,       miniWidth,   DIALOGHEIGHT, NULL);
        SetViewportOrgEx (ptoSrc->hdc,  miniWidth,   DIALOGHEIGHT, NULL);

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
        ptoSrc->rectClip.right   = 2*miniWidth;
        ptoSrc->rectClip.bottom  = rect.bottom;
        hrgn = CreateRectRgnIndirect (&ptoSrc->rectClip);
        SelectClipRgn (hdcSrc,       hrgn);
        SelectClipRgn (ptoSrc->hdc,  hrgn);
        DeleteObject (hrgn);

        SendMessage (hwndDlg, WM_PUTUPDESTRECT, (DWORD)hdcDest, (LONG)ptoDest);
        SendMessage (hwndDlg, WM_PUTUPSRCRECT, (DWORD)hdcSrc,  (LONG)ptoSrc);

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
    *  try to call StretchBlt() with the values.
    \**********************************************************************/
     case WM_PAINT: {
        HDC hdc;
        PAINTSTRUCT ps;

        hdc = BeginPaint(hwnd, &ps);

        /* Draw Separator lines for the three miniareas */
        SelectObject(hdc, hPenSeparator);
        MoveToEx (hdc, miniWidth-SEPARATORWIDTH,0, NULL);
        LineTo (hdc, miniWidth-SEPARATORWIDTH , rect.bottom);

        /* Grid the 2 HDCs */
        SelectObject (hdcSrc, hPenGrid);
        DrawGrids (hdcSrc, miniWidth, rect.bottom);
        SelectObject (hdcDest, hPenGrid);
        DrawGrids (hdcDest, miniWidth, rect.bottom);

        /* if a source bitmap has been selected, draw it. */
        if (hbmSrc)  DrawBitmap (hdcSrc, hbmSrc);

        /* paint the source track object on top of the bitmap */
        doTrackObject (ptoSrc , TROB_PAINT, hwnd, 0);

        /* make the call to StretchBlt() for the left half the window */
        SendMessage (hwnd, WM_STRETCHBLT, 0,0);

        EndPaint (hwnd, &ps);
    } return FALSE;



    /**********************************************************************\
    *  WM_STRETCHBLT
    *
    * Separate out the StretchBlt() call from the rest of the WM_PAINT
    *  case in this user message.  In this way, we can blt() without erasing
    *  and/or repaint all of the rest of the window.  Send this message when
    *  only the blt() should be done.  Invalidate the left half of the window
    *  when it should be erased, re-grided, and then blt()ed.
    \**********************************************************************/
    case WM_STRETCHBLT: {
        int X,    Y,    nWidth,    nHeight,
            XSrc, YSrc, nSrcWidth, nSrcHeight;
        LONG rop, ropByte;
        BOOL success;

        /* erase source track object, or top left sides will show up in blt. */
        doTrackObject (ptoSrc , TROB_PAINT, hwnd, 0);

        /* Get the parameters out of the top dialog box and call StretchBlt */
        if (IsWindow(hwndDlg)) {

          X = GetDlgItemInt(hwndDlg, DID_X, &success, TRUE);
          Y = GetDlgItemInt(hwndDlg, DID_Y, &success, TRUE);
          nWidth = GetDlgItemInt(hwndDlg, DID_WIDTH, &success, TRUE);
          nHeight = GetDlgItemInt(hwndDlg, DID_HEIGHT, &success, TRUE);
          XSrc = GetDlgItemInt(hwndDlg, DID_XSRC, &success, TRUE);
          YSrc = GetDlgItemInt(hwndDlg, DID_YSRC, &success, TRUE);
          nSrcWidth = GetDlgItemInt(hwndDlg,  DID_SRCWIDTH, &success, TRUE);
          nSrcHeight = GetDlgItemInt(hwndDlg, DID_SRCHEIGHT, &success, TRUE);

          /* get high order ROP byte and shift left by two bytes. */
          GetDlgItemText(hwndDlg, DID_ROP0, buffer, MAXCHARS);
          sscanf (buffer, "%lx", &ropByte);
          rop = ropByte * 256 * 256;

          /* get the next ROP byte and shift left by one byte. */
          GetDlgItemText(hwndDlg, DID_ROP1, buffer, MAXCHARS);
          sscanf (buffer, "%lx", &ropByte);
          rop += ropByte * 256;

          /* finally, get the low order ROP byte. */
          GetDlgItemText(hwndDlg, DID_ROP2, buffer, MAXCHARS);
          sscanf (buffer, "%lx", &ropByte);
          rop += ropByte;

          /* set streblt mode.  (user selects via combo box.) */
          if (strebltMode != NONE)
            SetStretchBltMode (hdcDest, strebltMode);

          /* select the pattern brush.  (user selects via combo box.) */
          if (iPatternBrush != NONE)
            SelectObject (hdcDest, GetStockObject (iPatternBrush));

          /**********************************************************/
          /**********************************************************/
          StretchBlt (hdcDest, X,    Y,    nWidth,    nHeight,
                      hdcSrc,  XSrc, YSrc, nSrcWidth, nSrcHeight, rop);
          /**********************************************************/
          /**********************************************************/
        }

        /* redraw source track object which was erased temporarily. */
        doTrackObject (ptoSrc , TROB_PAINT, hwnd, 0);
    } break;



    /**********************************************************************\
    *  WM_LBUTTONDOWN
    * On button down messages, hittest on the track object, and if
    *  it returns true, then send these messages to the track object.
    \**********************************************************************/
    case WM_LBUTTONDOWN:
      if (doTrackObject(ptoDest, TROB_HITTEST, hwnd, lParam))
         doTrackObject(ptoDest, message, hwnd, lParam);
      else if (doTrackObject(ptoSrc, TROB_HITTEST, hwnd, lParam))
         doTrackObject(ptoSrc, message, hwnd, lParam);
    break;



    /**********************************************************************\
    *  WM_LBUTTONUP
    * If the track object is in a "tracking mode" then send it this message.
    *  For the source track object, just send a STRETCHBLT message.
    *  For the dest. track object, invalidate the left half causing erase.
    \**********************************************************************/
    case WM_LBUTTONUP:
      if (ptoDest->Mode) {
        doTrackObject(ptoDest, message, hwnd, lParam);
        SendMessage (hwndDlg, WM_PUTUPDESTRECT, (DWORD) hdcDest, (LONG) ptoDest);
        InvalidateRect (hwndMain, &ptoSrc->rectClip, TRUE);
        InvalidateRect (hwndMain, &ptoDest->rectClip, TRUE);
      }
      if (ptoSrc->Mode) {
        doTrackObject(ptoSrc, message, hwnd, lParam);
        SendMessage (hwndDlg, WM_PUTUPSRCRECT, (DWORD) hdcSrc, (LONG) ptoSrc);
        PostMessage (hwndMain, WM_STRETCHBLT, 0,0);
      }
    break;


    /**********************************************************************\
    *  MW_MOUSEMOVE
    * If the track object is in a "tracking mode" then send it these messages.
    *  Also, update the coordinates in the top dialog box.
    \**********************************************************************/
    case WM_MOUSEMOVE:
      if (ptoDest->Mode) {
        doTrackObject(ptoDest, message, hwnd, lParam);
        SendMessage (hwndDlg, WM_PUTUPDESTRECT, (DWORD) hdcDest, (LONG) ptoDest);
      }
      if (ptoSrc->Mode) {
        doTrackObject(ptoSrc, message, hwnd, lParam);
        SendMessage (hwndDlg, WM_PUTUPSRCRECT, (DWORD) hdcSrc, (LONG) ptoSrc);
      }
    break;





    /**********************************************************************\
    *  Accelerator & clipboard support.
    *
    * Certain key strokes (c.f. *.rc) will cause the following WM_COMMAND
    *  messages.  In response the app will copy a bitmap into the clipboard
    *  or paste down from it.  In both cases, it is necessary to create a
    *  new bitmap since a bitmap in the clipboard belongs to the clipboard
    *  and not to the application.
    \**********************************************************************/
    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        HBITMAP hbmCompat, hbmOld;
        HDC hdcCompat;

        /******************************************************************\
        *  WM_COMMAND, AID_COPY
        *
        * Create a new bitmap, copy the destination HDC bits into it,
        *  and send the new bitmap to the clipboard.
        \******************************************************************/
        case AID_COPY: {
          int X,Y, nWidth, nHeight;
          BOOL success;

          X = GetDlgItemInt(hwndDlg, DID_X, &success, TRUE);
          Y = GetDlgItemInt(hwndDlg, DID_Y, &success, TRUE);
          nWidth = GetDlgItemInt(hwndDlg, DID_WIDTH, &success, TRUE);
          nHeight = GetDlgItemInt(hwndDlg, DID_HEIGHT, &success, TRUE);


          hdcCompat = CreateCompatibleDC(hdcDest);
          hbmCompat = CreateCompatibleBitmap (hdcDest, nWidth, nHeight);
          hbmOld = SelectObject(hdcCompat,hbmCompat);

          BitBlt (hdcCompat, 0,0,nWidth, nHeight, hdcDest, X,Y, SRCCOPY );

          SelectObject(hdcCompat,hbmOld);
          DeleteDC(hdcCompat);

          OpenClipboard (hwnd);
          SetClipboardData (CF_BITMAP,hbmCompat);
          CloseClipboard ();

          DeleteObject (hbmCompat);

        } break;


        /******************************************************************\
        *  WM_COMMAND, AID_PASTE
        *
        * Get bitmap handle from clipboard, create a new bitmap, draw
        *  the clipboard bitmap into the new one, and store the new
        *  handle in the global hbmSrc.
        \******************************************************************/
        case AID_PASTE: {
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
        *  WM_COMMAND, AID_CYCLE
        *
        * Post a COPY and PASTE command message to this window so that with
        *  one key stroke the user can copy the DEST image into the clipboard,
        *  paste it down into the SRC hdc and cause the blt.
        \******************************************************************/
        case AID_CYCLE:
          PostMessage (hwnd, WM_COMMAND, MAKELONG (AID_COPY , 1), 0);
          PostMessage (hwnd, WM_COMMAND, MAKELONG (AID_PASTE, 1), 0);
        break;
      }  /* end switch */

    break;  /* end wm_command */


  } /* end switch */
  return (DefWindowProc(hwnd, message, wParam, lParam));
}






/**************************************************************************\
*
*  function:  DlgProc()
*
*  input parameters:  normal window procedure parameters.
*
*  This is the dialog that sits on top of the window (it is streblted on the
*   WM_SIZE message of the main window).  In this window procedure, handle
*   the command messages that arise as the user selects buttons on this
*   dialog.  Also handle user messages for filling in entry field values.
*
*  global variables:
*   hwndMain - the main window.  also the parent of this dialog
*   ptoSrc   - pointer to the source track object
*   hdcSrc   - device context for the right half the window.
*   ptoDest  - pointer to the destination track object
*   hdcDest  - device context for the left half the window.
*   hbmSrc   - bitmap handle for the source bitmap.
*
*   strebltMode - Selection from combo box, to be used in SetStretchMode()
*   iPatternBrush - Selection from combo box, set into the hdc.
\**************************************************************************/
BOOL CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  char buffer[MAXCHARS];

  UNREFERENCED_PARAMETER(lParam);

  switch (message) {
    /**********************************************************************\
    *  WM_INITDIALOG
    *
    * Fill the entry fields with sensible original values.
    *  Also fill the combo boxes with the text entries from lookup tables.
    \**********************************************************************/
    case WM_INITDIALOG: {
      int i;

      for ( i = DID_X; i<= DID_SRCHEIGHT; i++)
        SetDlgItemText(hwnd, i , "0");

      SetDlgItemText(hwnd, DID_ROP0  , "cc");
      SetDlgItemText(hwnd, DID_ROP1  , "00");
      SetDlgItemText(hwnd, DID_ROP2  , "20");

      for (i = 0; i< NMODES; i++)
        SendDlgItemMessage (hwnd, DID_CB_MODE, CB_INSERTSTRING,
                            (DWORD) -1, (LONG)strebltModes[i].String);

      for (i = 0; i< NPATTERNS; i++)
        SendDlgItemMessage (hwnd, DID_CB_PATTERN, CB_INSERTSTRING,
                            (DWORD) -1, (LONG)Patterns[i].String);

      for (i = 0; i< NROPS; i++)
        SendDlgItemMessage (hwnd, DID_CB_ROPS, CB_INSERTSTRING,
                            (DWORD) -1, (LONG)StandardROPs[i].String);


    } return TRUE;


    /**********************************************************************\
    *  WM_PUTUPDESTRECT
    *
    * wParam -  HDC with the needed world transform.
    * lParam -  Pointer to the track object.
    *
    * Fill the entry fields for the destination rectangle points.
    *  Conditionally change <x,y> or <width,height> depending on tracking mode.
    \**********************************************************************/
    case WM_PUTUPDESTRECT: {
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
          SetDlgItemInt(hwnd, DID_X, p1.x, TRUE);
          SetDlgItemInt(hwnd, DID_Y, p1.y, TRUE);
        }

        if (!(pto->Mode & TMMOVE)) {
          SetDlgItemInt(hwnd, DID_WIDTH,  p2.x, TRUE);
          SetDlgItemInt(hwnd, DID_HEIGHT, p2.y, TRUE);
        }
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
          SetDlgItemInt(hwnd, DID_SRCWIDTH,  p2.x, TRUE);
          SetDlgItemInt(hwnd, DID_SRCHEIGHT, p2.y, TRUE);
        }
    } return FALSE;




    case WM_COMMAND:

      switch (LOWORD(wParam)) {

        /******************************************************************\
        *  WM_COMMAND, DID_DRAW
        *
        * Draw button hit - send main window message to call StretchBlt().
        \******************************************************************/
        case DID_DRAW:
          SendMessage (hwndMain, WM_STRETCHBLT, 0,0);
        break;


        /******************************************************************\
        *  WM_COMMAND, DID_NEWSRC
        *
        * Try to get a new source bitmap. If successful, store it in the
        *  global handle (hbmSrc) then invalidate two sub windows so that
        *  we force a complete repaint in both.
        \******************************************************************/
        case DID_NEWSRC: {
          HBITMAP hbm;
          if ( hbm = GetBitmap (hdcSrc, hInst, FALSE)) {
            DeleteObject (hbmSrc);
            hbmSrc = hbm;
            InvalidateRect (hwndMain, &ptoSrc->rectClip, TRUE);
            InvalidateRect (hwndMain, &ptoDest->rectClip, TRUE);
          }

        } break;


        /******************************************************************\
        *  WM_COMMAND, DID_CB_MODE
        *
        * A new streblt mode was selected.  Look up the value, store it
        *  in a global variable (strebltMode), then force a new StretchBlt
        \******************************************************************/
        case DID_CB_MODE:
          if (HIWORD (wParam) == CBN_SELCHANGE) {
            int iSel;

            iSel = SendMessage ((HWND) lParam, CB_GETCURSEL, 0,0);
            strebltMode = strebltModes[iSel].Value;

            SendMessage (hwndMain, WM_STRETCHBLT, 0,0);
          }
        break;


        /******************************************************************\
        *  WM_COMMAND, DID_CB_PATTERN
        *
        * A new pattern brush was selected.  Look up the value, store it
        *  in a global variable (iPatternBrush), then force a new StretchBlt
        \******************************************************************/
        case DID_CB_PATTERN:
          if (HIWORD (wParam) == CBN_SELCHANGE) {
            int iSel;

            iSel = SendMessage ((HWND) lParam, CB_GETCURSEL, 0,0);
            iPatternBrush = Patterns[iSel].Value;

            SendMessage (hwndMain, WM_STRETCHBLT, 0,0);
          }
        break;



        /******************************************************************\
        *  WM_COMMAND, DID_CB_ROPS
        *
        * A new standard rop entry was selected.  Lookup the value,
        *  then break it into three parts, convert to hex, and put
        *  it in the proper entry fields.
        \******************************************************************/
        case DID_CB_ROPS:
          if (HIWORD (wParam) == CBN_SELCHANGE) {
            int iSel, rop;

            iSel = SendMessage ((HWND) lParam, CB_GETCURSEL, 0,0);
            rop = StandardROPs[iSel].Value;
            sprintf (buffer, "%2x", (rop & 0x000000ff));
            SetDlgItemText(hwnd, DID_ROP2, buffer);
            rop /= 256;
            sprintf (buffer, "%2x", (rop & 0x000000ff));
            SetDlgItemText(hwnd, DID_ROP1, buffer);
            rop /= 256;
            sprintf (buffer, "%2x", (rop & 0x000000ff));
            SetDlgItemText(hwnd, DID_ROP0, buffer);

            SendMessage (hwndMain, WM_STRETCHBLT, 0,0);
          }
        break;




      } /* end switch */


    return FALSE; /* end WM_COMMAND */


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
