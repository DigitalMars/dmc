
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/**************************************************************************\
*  wxform.c -- sample program demonstrating the new "World Transform."
*
*  design:  There are a few global handles or pointers in this application,
*   and different routines to operate on them.  The obvious case of this
*   is the three window handles and their associated window procedures.
*   There is also a unique pointer to a track object and a routine to
*   operate on it (i.e. doTrackObject).  All communication is accomplished
*   by sending messages between these procedures.  Each window procedure,
*   and the track object procedure, operate on some set of messages which
*   include some of the standard Windows messages, and also miscellaneous
*   "WM_USER" messages (c.f. wxform.h).
\**************************************************************************/

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "wxform.h"



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
    HICON hicon;

    UNREFERENCED_PARAMETER( lpCmdLine );


    //
    // Detect platform and exit gracefully if not Windows NT.
    //

    {
      OSVERSIONINFO osvi;
      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);

      GetVersionEx (&osvi);
      if (osvi.dwPlatformId != VER_PLATFORM_WIN32_NT) {
        MessageBox (NULL, "This sample will only work correctly on Windows NT.",
          "wxform", MB_OK | MB_ICONSTOP);
        return 0;
      }
    }


    /* Check for previous instance.  If none, then register class. */
    if (!hPrevInstance) {
        WNDCLASS  wc;

        wc.style = 0;
        wc.lpfnWndProc = (WNDPROC)MainWndProc;

        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(hInstance, "TransformIcon");
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
        wc.lpszMenuName =  NULL;
        wc.lpszClassName = "wxform";

        if (!RegisterClass(&wc)) return (FALSE);
    }  /* class registered o.k. */


    /* Create the main window.  Return false if CreateWindow() fails */
    hInst = hInstance;

    hwndMain = CreateWindow(
        "wxform",
        "World Transform Demonstration",
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


    /* create a new track object and paint it for the first time. */
    ptoRect = doTrackObject(NULL, TROB_NEW, hwndMain, 0);
    doTrackObject(ptoRect, TROB_PAINT, hwndMain, 0);


    /* load and display dialog for the world transform matrix.
     *  then fill its entry fields.  Also, get the HICON from the
     *  main window and fill it into the dialog's class structure
     *  for this application.
     */
    hwndTransform = CreateDialog(hInst, "TransformDlg",
                                 hwndMain, (DLGPROC)TransformDlgProc);
    hicon = (HICON) GetClassLong (hwndMain, GCL_HICON);
    SetClassLong (hwndTransform, GCL_HICON, (LONG)hicon);
    showTransform = TRUE;
    SendMessage (hwndTransform, WM_PUTUPFLOATS, 0, (LONG) &ptoRect->xfmChange);

    /* load and display the dialog for the mouse position.
     *  minimize it initially.
     */
    hwndMouse = CreateDialog(hInst, "MouseDlg",
                             hwndMain, (DLGPROC)MouseDlgProc);
    ShowWindow (hwndMouse, SW_SHOWMINIMIZED);
    showMouse = FALSE;


    /* load and display the dialog with the direct manipulation help.
     *  minimize it initially.   (Don't need a unique window procedure.)
     */
    hwndHelp = CreateDialog(hInst, "helpDlg", hwndMain, NULL);
    ShowWindow (hwndHelp, SW_SHOWMINIMIZED);



    /* Loop getting messages and dispatching them. */
    while (GetMessage(&msg,NULL, 0,0)) {
      if (!IsDialogMessage (hwndTransform, &msg))
      if (!IsDialogMessage (hwndMouse, &msg))
      if (!IsDialogMessage (hwndHelp, &msg)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    return (msg.wParam);
}









/**************************************************************************\
*
*  function:  MainWndProc()
*
*  input parameters:  normal window procedure parameters.
*
*  global variables:
*   hwndTransform,
*   hwndMouse - information dialog box window handles.
*   showTransform,
*   showMouse - Booleans recording the retore/minimize state of the dialogs.
*   ptoRect   - pointer to track object in middle of screen.
*
\**************************************************************************/
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static HANDLE hPenGrid;

  switch (message) {

    /**********************************************************************\
    *  WM_CREATE
    *
    * create a pen for later use.
    \**********************************************************************/
    case WM_CREATE:
      hPenGrid  = CreatePen (PS_SOLID, 1, GRIDCOLOR);
    break;



    /**********************************************************************\
    *  WM_DESTROY
    *
    * Complement of WM_CREATE.  send the track object the delete messages,
    *  then call PostQuitMessage.
    \**********************************************************************/
    case WM_DESTROY:
      DeleteObject(hPenGrid);
      doTrackObject(ptoRect, TROB_DELETE, hwnd, lParam);
      PostQuitMessage(0);
    break;



    /**********************************************************************\
    *  WM_SIZE
    *
    * Invalidate the whole window because we reset the origin on paint
    *  messages according to the size.  Also, send the track object a
    *  message so that it will also change its HDC's viewport origin.
    \**********************************************************************/
    case WM_SIZE:
        InvalidateRect (hwnd, NULL, TRUE);
        doTrackObject (ptoRect, TROB_CENTER, hwnd, lParam);
    break;



    /**********************************************************************\
    *  WM_PAINT
    *
    * First invalidate the whole window (forces the object to be painted
    *  fresh, and thus it won't XOR its old self out).  Then draw the
    *  grid and finally draw the object.
    \**********************************************************************/
    case WM_PAINT : {
      PAINTSTRUCT ps;
      HDC hdc;
      RECT  rect;
      POINT point;
      int i;

      InvalidateRect (hwnd, NULL, TRUE);

      hdc = BeginPaint(hwnd, &ps);

      CenterOrigin (hwnd, hdc);
      GetClientRect (hwnd, &rect);
      GetViewportOrgEx(hdc, &point);
      OffsetRect(&rect, -point.x, -point.y );


      /* Draw vertical lines.  Draw three at the origin. */
      SelectObject(hdc, hPenGrid);
      for (i = 0; i<= rect.right; i+=TICKSPACE){
        MoveToEx (hdc, i, rect.top, NULL);
        LineTo (hdc, i, rect.bottom);
        MoveToEx (hdc, -i, rect.top, NULL);
        LineTo (hdc, -i, rect.bottom);
      }
      MoveToEx (hdc, -1, rect.top, NULL);
      LineTo (hdc, -1, rect.bottom);
      MoveToEx (hdc, 1, rect.top, NULL);
      LineTo (hdc, 1, rect.bottom);


      /* Draw horizontal lines.  Draw three at the origin. */
      for (i = 0; i<= rect.bottom; i+=TICKSPACE){
        MoveToEx (hdc, rect.left, i, NULL);
        LineTo (hdc, rect.right, i);
        MoveToEx (hdc, rect.left, -i, NULL);
        LineTo (hdc, rect.right, -i);
      }
      MoveToEx (hdc, rect.left, -1, NULL);
      LineTo (hdc, rect.right, -1);
      MoveToEx (hdc, rect.left, 1, NULL);
      LineTo (hdc, rect.right, 1);

      doTrackObject(ptoRect, TROB_PAINT, hwnd, lParam);

      EndPaint (hwnd, &ps);
    } break;



    /**********************************************************************\
    *  WM_LBUTTONDOWN & WM_RBUTTONDOWN
    * On button down messages, hittest on the track object, and if
    *  it returns true, then send these messages to the track object.
    \**********************************************************************/
    case WM_RBUTTONDOWN:
    case WM_LBUTTONDOWN:
      if (doTrackObject(ptoRect, TROB_HITTEST, hwnd, lParam))
         doTrackObject(ptoRect, message, hwnd, lParam);
    break;



    /**********************************************************************\
    *  WM_LBUTTONUP & WM_RBUTTONDOWN & MW_MOUSEMOVE
    * If the track object is in a "tracking mode" then send it these messages.
    *  If the transform dialog is not minimized, fill it with numbers.
    *  If the mouse dialog is not minimized, fill it with numbers.
    \**********************************************************************/
    case WM_RBUTTONUP:
    case WM_LBUTTONUP:
    case WM_MOUSEMOVE:
      if (ptoRect->Mode) {
        doTrackObject(ptoRect, message, hwnd, lParam);
        if (showTransform)
          SendMessage (hwndTransform, WM_PUTUPFLOATS, 0,
                       (LONG) &ptoRect->xfmChange);
      }

      if (showMouse)
        SendMessage (hwndMouse, WM_PUTUPFLOATS, (DWORD) hwnd, lParam);

    break;


  } /* end switch */
  return (DefWindowProc(hwnd, message, wParam, lParam));
}






/**************************************************************************\
*
*  function:  TransformDlgProc()
*
*  input parameters:  normal window procedure parameters.
*
*  global variables:
*   showTransform  - TRUE if window is restored, FALSE if minimized.
*       maintain the value in this routine for other windows' use.
*   ptoRect - pointer to the track object.
*   showMouse, hwndMain.
*
*  nonstandard messages:
*   WM_PUTUPFLOATS - fill the entry fields with the contents of an XFORM.
\**************************************************************************/
LRESULT CALLBACK TransformDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
XFORM  xform;
char buffer[MAXCHARS];


  switch (message) {


    /**********************************************************************\
    *  WM_INITDIALOG
    *
    * Fill the entry fields with sensible original values.
    \**********************************************************************/
    case WM_INITDIALOG:
      SetDlgItemText(hwnd, IDD_13, "0");
      SetDlgItemText(hwnd, IDD_23, "0");
      SetDlgItemText(hwnd, IDD_33, "1");
    return TRUE;


    /******************************************************************\
    *  WM_PUTUPFLOATS
    *
    *  lParam - pointer to an XFORM structure.
    *   fill the entry fields with the XFORM values.
    \******************************************************************/
    case WM_PUTUPFLOATS: {
      PXFORM pxform;
      pxform = (PXFORM) lParam;

      sprintf (buffer, FORMATFLOAT,pxform->eM11);
      SetDlgItemText(hwnd, IDD_EM11, buffer);
      sprintf (buffer, FORMATFLOAT,pxform->eM12);
      SetDlgItemText(hwnd, IDD_EM12, buffer);
      sprintf (buffer, FORMATFLOAT,pxform->eDx);
      SetDlgItemText(hwnd, IDD_EDX, buffer);

      sprintf (buffer, FORMATFLOAT,pxform->eM21);
      SetDlgItemText(hwnd, IDD_EM21, buffer);
      sprintf (buffer, FORMATFLOAT,pxform->eM22);
      SetDlgItemText(hwnd, IDD_EM22, buffer);
      sprintf (buffer, FORMATFLOAT,pxform->eDy);
      SetDlgItemText(hwnd, IDD_EDY, buffer);

    } return FALSE;



    /******************************************************************\
    *  WM_SIZE
    *
    *  toggle the global variable keeping track of the iconized state
    *   of this window.
    \******************************************************************/
    case WM_SIZE :
      if (wParam == SIZEICONIC)
        showTransform = FALSE;
      else {
        showTransform = TRUE;
        SendMessage (hwnd, WM_PUTUPFLOATS, 0, (LONG) &ptoRect->xfmChange);
      }
    return FALSE;


    case WM_COMMAND:
      /******************************************************************\
      *  WM_COMMAND,  IDD_SETXFORM
      *
      *  take the values from the entry field, fill them into an XFORM
      *   structure and then send the track object the message to use
      *   these values.  Finally, reformat and repaint the entry fields.
      \******************************************************************/
      if (LOWORD(wParam) == IDD_SETXFORM) {
        GetDlgItemText(hwnd, IDD_EM11, buffer, MAXCHARS);
        xform.eM11 = (float) atof (buffer);
        GetDlgItemText(hwnd, IDD_EM12, buffer, MAXCHARS);
        xform.eM12 = (float) atof (buffer);
        GetDlgItemText(hwnd, IDD_EDX, buffer, MAXCHARS);
        xform.eDx = (float) atof (buffer);

        GetDlgItemText(hwnd, IDD_EM21, buffer, MAXCHARS);
        xform.eM21 = (float) atof (buffer);
        GetDlgItemText(hwnd, IDD_EM22, buffer, MAXCHARS);
        xform.eM22 = (float) atof (buffer);
        GetDlgItemText(hwnd, IDD_EDY, buffer, MAXCHARS);
        xform.eDy = (float) atof (buffer);

        // HACK.  The WM_SIZE here is used to flush the GDI buffer in order
        //  to eliminate a very strange bug whereby DPtoLP() doesn't work.
        if (showMouse) SendMessage (hwndMain, WM_SIZE, 0,0);


        doTrackObject (ptoRect, TROB_SETXFORM, hwnd, (LONG) &xform);
        SendMessage (hwnd, WM_PUTUPFLOATS, 0, (LONG) &xform);


      /******************************************************************\
      *  WM_COMMAND,  IDD_IDENTITY
      *
      *  fill a local XFORM structure with the identity matrix.  Now
      *   send the track object the message to use these values.
      *   Finally, reformat and repaint the entry fields.
      \******************************************************************/
      } else if (LOWORD(wParam) == IDD_IDENTITY) {
        xform.eM11 =
        xform.eM22 =  (float) 1.0;
        xform.eDx  =
        xform.eDy  =
        xform.eM12 =
        xform.eM21 =  (float) 0.0;

        // HACK.  The WM_SIZE here is used to flush the GDI buffer in order
        //  to eliminate a very strange bug whereby DPtoLP() doesn't work.
        if (showMouse) SendMessage (hwndMain, WM_SIZE, 0,0);

        doTrackObject (ptoRect, TROB_SETXFORM, hwnd, (LONG) &xform);
        SendMessage (hwnd, WM_PUTUPFLOATS, 0, (LONG) &xform);
      } /* end WM_COMMAND */
    return FALSE;


  } /* end switch */
  return FALSE;
}





/**************************************************************************\
*
*  function:  MouseDlgProc()
*
*  input parameters:  normal window procedure parameters.
*
*  global variables:
*   showMouse  -- TRUE if window is restored, FALSE if minimized.
*       maintain the value in this routine for other windows' use.
*   ptoRect - pointer to the track object.  Needed for DPtoLP()
*
*  nonstandard messages:
*   WM_PUTUPFLOATS - fill the entry fields with the mouse position.
*
\**************************************************************************/
LRESULT CALLBACK MouseDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
char buffer[MAXCHARS];

  switch (message) {

    /******************************************************************\
    *  WM_PUTUPFLOATS
    *
    *  wParam - contains the hwnd for the main window.
    *  lParam - contains the mouse position in device coordinates.
    *           (c.f. WM_MOUSEMOVE)
    \******************************************************************/
    case WM_PUTUPFLOATS: {
      POINT pScreen, pWorld;
      HWND hwndMain;

      hwndMain = (HWND) wParam;
      pScreen.x = pWorld.x = LOWORD(lParam);
      pScreen.y = pWorld.y  = HIWORD(lParam);

      sprintf (buffer, "%d", pScreen.x);
      SetDlgItemText(hwnd, IDD_DEVICEX, buffer);
      sprintf (buffer, "%d", pScreen.y);
      SetDlgItemText(hwnd, IDD_DEVICEY, buffer);

      ClientToScreen (hwndMain, &pScreen);
      sprintf (buffer, "%d", pScreen.x);
      SetDlgItemText(hwnd, IDD_SCREENX, buffer);
      sprintf (buffer, "%d", pScreen.y);
      SetDlgItemText(hwnd, IDD_SCREENY, buffer);

      DPtoLP (ptoRect->hdc, &pWorld, 1);
      sprintf (buffer, FORMATFLOAT, (float) pWorld.x);
      SetDlgItemText(hwnd, IDD_WORLDX , buffer);
      sprintf (buffer, FORMATFLOAT, (float) pWorld.y);
      SetDlgItemText(hwnd, IDD_WORLDY , buffer);

    } return FALSE;



    /******************************************************************\
    *  WM_SIZE
    *
    *  toggle the global variable keeping track of the iconized state
    *   of this window.
    \******************************************************************/
    case WM_SIZE :
      if (wParam == SIZEICONIC)
        showMouse = FALSE;
      else
        showMouse = TRUE;
    return FALSE;

  }
  return FALSE;
}






/**************************************************************************\
*  function:  CenterOrigin()
*
*  input parameters:
*   hwnd - window with client we want the center of.
*   hdc - device context which we set the Viewport origin of.
*
\**************************************************************************/
VOID CenterOrigin (HWND hwnd, HDC hdc)
{
RECT  rect;
POINT center;

    GetClientRect (hwnd, &rect);
    center.x = rect.right / 2;
    center.y = rect.bottom /2;

    SetViewportOrgEx (hdc, center.x, center.y, NULL);
    return;
}






/**************************************************************************\
*
*  function:  doTrackObject()
*
*  input parameters:
*   pto -  pointer to a track object.
*   msg -  message selecting what action to take.  Values may include WM_*'s
*           (see case statements below for more information.)
*   hwnd - Window handle for the window the track object exists within.
*   lParam - Usually fourth param to window proc. varies based on msg.
*
*  global variables:  none.
*
*  coordinate spaces:  There are three coordinate spaces of interest here,
*   and this routine is frequently switching between them...
*
*           WORLD                   DEVICE                  SCREEN
*
*      object coordinates       input mouse pos       used w/ SetCursorPos()
*         (pto->rect)          (lParam for WM_*)
*
*             ----->  LPtoDP() ---->    ----> ClientToScreen() -->
*             <-----  DPtoLP() <----    <---- ScreenToClient() <--
*
*   in addition, the HDC has an offset origin.  Device coordinates for the
*   mouse (lParam) never take this into account, but it is necessary to
*   translate them in order to get direct manipulation right.
*
\**************************************************************************/
PTrackObject doTrackObject(PTrackObject pto, int msg, HWND hwnd, LONG lParam)
{
  if ((pto == NULL) && (msg != TROB_NEW))  return NULL;

  switch (msg) {


    /**********************************************************************\
    *  TROB_NEW
    *
    * Allocate new PTrackObject structure.  Fill in default values
    *  for the fields of the structure.  Set up the HDC correctly.
    * return - pointer to the new object.
    \**********************************************************************/
    case  TROB_NEW: {
        PTrackObject  pto;

        /* with LPTR returned value is a pointer. */
        pto = (PTrackObject) LocalAlloc (LPTR, sizeof (TrackObject));

        /* initialize the HDC and other fields. */
        pto->hdc = GetDC(hwnd);
        SetGraphicsMode (pto->hdc, GM_ADVANCED);
        SetROP2(pto->hdc, R2_NOT);
        SelectObject (pto->hdc, GetStockObject (NULL_BRUSH));
        SelectObject(pto->hdc, CreatePen (PS_SOLID, 2, (COLORREF) 0x01000009));
        pto->Mode = TMNONE;
        doTrackObject (pto, TROB_CENTER, hwnd, lParam);
        GetWorldTransform (pto->hdc, &(pto->xfmChange));

        /* initialize the size. */
        pto->rect.top = pto->rect.left = 0;
        pto->rect.bottom = pto->rect.right = TICKSPACE*5;

        return (pto);
    }


    /**********************************************************************\
    *  TROB_DELETE
    *
    * Delete the pen that we created, release the DC,
    *  free up the memory allocated for the object.
    \**********************************************************************/
    case  TROB_DELETE:
        DeleteObject (SelectObject (pto->hdc, GetStockObject (BLACK_PEN)));
        doTrackObject (pto, TROB_PAINT, hwnd, lParam);
        ReleaseDC (hwnd, pto->hdc);
        LocalFree (LocalHandle ((LPSTR)pto));
    return NULL;



    /**********************************************************************\
    *  TROB_CENTER
    *
    * Called in order to reset the view port origin in the track objects
    *  hdc whenever the client window changes size.  This hdc is thus kept
    *  synchronized with the hdc that the axes are painted into.
    \**********************************************************************/
    case TROB_CENTER: {
        CenterOrigin (hwnd, pto->hdc);
        return (pto);
    }



    /**********************************************************************\
    *  TROB_PAINT
    *
    * Paint the object into its hdc.  Called half the time to erase
    *  the object, and half the time to redraw it.
    \**********************************************************************/
    case TROB_PAINT: {
        Rectangle (pto->hdc, pto->rect.left+1, pto->rect.top+1,
                             pto->rect.left+INC, pto->rect.top+INC);

        Rectangle (pto->hdc, pto->rect.left, pto->rect.top,
                             pto->rect.right, pto->rect.bottom);
    } return NULL;



    /**********************************************************************\
    *  TROB_SETXFORM
    *
    * lParam - pointer to the new transform.
    *  set the new transform into the HDC, then update xfmChange.
    \**********************************************************************/
    case TROB_SETXFORM: {
        doTrackObject (pto, TROB_PAINT, hwnd, lParam);
        SetWorldTransform(pto->hdc, (PXFORM) lParam);
        GetWorldTransform(pto->hdc, &pto->xfmChange);
        doTrackObject (pto, TROB_PAINT, hwnd, lParam);
    } return NULL;



    /**********************************************************************\
    *  TROB_HITTEST
    *
    * Check the point sent in in the lParam to see if it lays within
    *  the bounds of the objects defining rectangle.
    * return - pointer to the object iff the point is in rectangle,
    *  otherwise return NULL.
    \**********************************************************************/
    case TROB_HITTEST:{
        POINT  mouWorld;
        mouWorld.x = LOWORD(lParam);
        mouWorld.y = HIWORD(lParam);

        DPtoLP (pto->hdc, &mouWorld, 1);

        if (PtInRect (&pto->rect, mouWorld))  return pto;
        else  return NULL;
    }



    /**********************************************************************\
    *  WM_LBUTTONDOWN &  WM_RBUTTONDOWN
    *
    * Capture the mouse, set the tracking mode depending on the mouse
    *  location in world coordinates, reset the mouse position.
    *
    \**********************************************************************/
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN: {
      POINT  newmouScreen;
      POINT  mouWorld;

      mouWorld.x = LOWORD(lParam);
      mouWorld.y = HIWORD(lParam);
      DPtoLP (pto->hdc, &mouWorld, 1);

      /* upper left hand corner. right button is no-op. */
      if ((mouWorld.x <= (pto->rect.right  / 2)) &&
          (mouWorld.y <= (pto->rect.bottom / 2))) {
          if (msg == WM_RBUTTONDOWN) return NULL;
          pto->Mode = TMMOVE;
          newmouScreen.x = pto->rect.left;
          newmouScreen.y = pto->rect.top;

      /* lower left hand corner */
      } else if ((mouWorld.x <= (pto->rect.right  / 2)) &&
          (mouWorld.y > (pto->rect.bottom / 2))) {

          pto->Mode = (msg == WM_RBUTTONDOWN) ? TMSHEARY : TMSIZEY;
          newmouScreen.x = pto->rect.left;
          newmouScreen.y = pto->rect.bottom;

      /* upper right hand corner */
      } else if ((mouWorld.x > (pto->rect.right  / 2)) &&
          (mouWorld.y <= (pto->rect.bottom / 2))) {

          pto->Mode = (msg == WM_RBUTTONDOWN) ? TMSHEARX : TMSIZEX;
          newmouScreen.x = pto->rect.right;
          newmouScreen.y = pto->rect.top;

      /* lower right hand corner */
      } else if ((mouWorld.x > (pto->rect.right  / 2)) &&
          (mouWorld.y > (pto->rect.bottom / 2))) {

          pto->Mode = (msg == WM_RBUTTONDOWN) ? TMROTATE : TMSIZEXY;
          newmouScreen.x = pto->rect.right;
          newmouScreen.y = pto->rect.bottom;
      }

      SetCapture(hwnd);
      LPtoDP (pto->hdc, &newmouScreen, 1);
      ClientToScreen (hwnd, &newmouScreen);
      SetCursorPos (newmouScreen.x,newmouScreen.y);

      GetWorldTransform (pto->hdc, &pto->xfmDown);
    } return NULL;



    /**********************************************************************\
    *  WM_MOUSEMOVE
    *
    * this is where almost all of the interesting calculation is done.
    *  First clip the mouse location to be in client rectangle, then
    *  call MouseMove() to handle the different tracking modes.
    \**********************************************************************/
    case WM_MOUSEMOVE: {
      RECT  rect;
      GetClientRect (hwnd, &rect);

      if ((short) LOWORD(lParam) < (short)rect.left)
        lParam = MAKELONG ((WORD)rect.left, HIWORD(lParam));

      if (LOWORD(lParam) > (WORD)rect.right)
        lParam = MAKELONG ((WORD)rect.right, HIWORD(lParam));

      if ((short) HIWORD(lParam) < (short)rect.top)
        lParam = MAKELONG (LOWORD(lParam), (WORD)rect.top);

      if (HIWORD(lParam) > (WORD)rect.bottom)
        lParam = MAKELONG (LOWORD(lParam),(WORD)rect.bottom);

      MouseMove (pto, msg, hwnd, lParam);

    } return NULL;



    /**********************************************************************\
    *  WM_RBUTTONUP & WM_LBUTTONUP
    *
    * simply release the mouse capture, and set the mode to TMNONE.
    \**********************************************************************/
    case WM_RBUTTONUP:
    case WM_LBUTTONUP: {
      if (pto->Mode) {
         ReleaseCapture();
         pto->Mode = TMNONE;
      }
    } return NULL;

  }  /* end switch(msg) */
}






/**************************************************************************\
*  function:  MouseMove()
*
*  input parameters:
*   pto -  pointer to a track object.
*   msg -  not used.
*   hwnd - Window handle for the window the track object exists within.
*   lParam - Usually fourth param to window proc. varies based on msg.
*
*  The tracking behavior which the user observers when moving the mouse
*   is based on the current tracking mode of the object.  This is usually
*   determined on the mouse down event (c.f. TM*).  First erase the old
*   object, then figure out the change to the transform matrix, finally
*   change the world transform matrix and redraw the object.
*
*  Tranform:
*    (    eM11        eM12        0   )
*    (    eM21        eM22        0   )
*    (    eDx         eDy         1   )
*
*   xDevice = (xWorld * eM11) + (yWorld * eM21) + eDx
*   yDevice = (xWorld * eM12) + (yWorld * eM22) + eDy
*
*   In this routine the Device (mouse location) and World (rectangle corner)
*   points are known.  Therefore, the two equations above are solved for
*   the desired matrix entry value (e.g. eM11, 1M12, ... eDy).  The tracking
*   mode determines which one of these entries may be changed.  E.g. scaling
*   in X modifies eM11 while shearing in X modifies eM12.  So rather than
*   using the world transform to map from world to device points, we are
*   back-computing the proper contents of the world transform.
*
\**************************************************************************/
VOID MouseMove(PTrackObject pto, int msg, HWND hwnd, LONG lParam)
{
POINT  mouWorld, mouDevice, orgDevice;

    UNREFERENCED_PARAMETER(msg);

    doTrackObject(pto, TROB_PAINT, hwnd, lParam);
    mouDevice.x = mouWorld.x = LOWORD(lParam);
    mouDevice.y = mouWorld.y = HIWORD(lParam);

    SetWorldTransform(pto->hdc, &pto->xfmDown);
    DPtoLP (pto->hdc, &mouWorld, 1);

    /* offset the mouse device point for the viewport's origin. */
    GetViewportOrgEx (pto->hdc, &orgDevice);
    mouDevice.x -= orgDevice.x;
    mouDevice.y -= orgDevice.y;

    GetWorldTransform(pto->hdc, &pto->xfmChange);

    switch (pto->Mode) {
      /*******************************************************\
      *    (     1         xShear       0   )
      *    (     0           1          0   )
      *    (     0           0          1   )
      *
      * xWorld = rect.left == 0;
      \*******************************************************/
      case TMSHEARX: {
        pto->xfmChange.eM12 = (float) mouDevice.y;
        pto->xfmChange.eM12 -=pto->xfmChange.eDy;
        pto->xfmChange.eM12 /=(float) pto->rect.right ;
        SetWorldTransform (pto->hdc, &pto->xfmChange);
      } break;


      /*******************************************************\
      *    (     1           0          0   )
      *    (   yShear        1          0   )
      *    (     0           0          1   )
      *
      * yWorld = rect.top == 0;
      \*******************************************************/
      case TMSHEARY: {
        pto->xfmChange.eM21 = (float) mouDevice.x;
        pto->xfmChange.eM21 -=pto->xfmChange.eDx;
        pto->xfmChange.eM21 /=(float) pto->rect.bottom ;
        SetWorldTransform (pto->hdc, &pto->xfmChange);

      } break;


      /*******************************************************\
      *    (   cos(a)      -sin(a)      0   )
      *    (   sin(a)       cos(a)      0   )
      *    (     0           0          1   )
      *
      * a == rotation angle.  Since mouse in in lower right,
      *  we need to shift this back 45 degrees (assuming that
      *  straight down is 0 degrees).  Thus we actually compute
      *  cos(a) = cos(b - 45) = cos(b)sin(45) + cos(45)sin(45)
      *  where b is angle from the origin to the mouse (x,y)
      *  cos(45) = sin(45) ~= 0.707107
      *  cos(b) = y/r    sin(b) = x/r
      *
      \*******************************************************/
      case TMROTATE: {
        float r;

        /* translate back to the origin. */
        pto->xfmChange.eDx = pto->xfmChange.eDy = (float)0.0;
        SetWorldTransform (pto->hdc, &pto->xfmChange);

        /* rotate about the origin. */
        r = (float) sqrt( (double)(mouWorld.x * mouWorld.x) +
                          (double)(mouWorld.y * mouWorld.y));

        pto->xfmChange.eM11 = (float) mouWorld.y / r;
        pto->xfmChange.eM11 += (float) mouWorld.x / r;
        pto->xfmChange.eM11 *= (float) 0.707107;
        pto->xfmChange.eM22 = pto->xfmChange.eM11;

        pto->xfmChange.eM12 = (float) mouWorld.y / r;
        pto->xfmChange.eM12 -= (float) mouWorld.x / r;
        pto->xfmChange.eM12 *= (float) 0.707107;
        pto->xfmChange.eM21 = -pto->xfmChange.eM12;

        pto->xfmChange.eDx = pto->xfmChange.eDy = (float)0.0;

        ModifyWorldTransform (pto->hdc, &pto->xfmChange, MWT_RIGHTMULTIPLY);

        /* translate back to the original offset. */
        pto->xfmChange.eM11 =
        pto->xfmChange.eM22 = (float) 1.0;
        pto->xfmChange.eM12 =
        pto->xfmChange.eM21 = (float) 0.0;

        pto->xfmChange.eDx = pto->xfmDown.eDx;
        pto->xfmChange.eDy = pto->xfmDown.eDy;
        ModifyWorldTransform (pto->hdc, &pto->xfmChange, MWT_RIGHTMULTIPLY);
        GetWorldTransform (pto->hdc, &pto->xfmChange);
      } break;


      /*******************************************************\
      *    (  Size X         0          0   )
      *    (     0        Size Y        0   )
      *    (     0           0          1   )
      *
      \*******************************************************/
      case TMSIZEXY: {
        pto->xfmChange.eM11 = (float) mouDevice.x;
        pto->xfmChange.eM11 -=pto->xfmChange.eDx;
        pto->xfmChange.eM11 -=((float) pto->rect.bottom * pto->xfmChange.eM21);
        pto->xfmChange.eM11 /=(float) pto->rect.right ;

        pto->xfmChange.eM22 = (float) mouDevice.y;
        pto->xfmChange.eM22 -=pto->xfmChange.eDy;
        pto->xfmChange.eM22 -=((float) pto->rect.right  * pto->xfmChange.eM12);
        pto->xfmChange.eM22 /=(float) pto->rect.bottom ;
        SetWorldTransform (pto->hdc, &pto->xfmChange);
      } break;


      /*******************************************************\
      *    (  Size X         0          0   )
      *    (     0           1          0   )
      *    (     0           0          1   )
      *
      * yWorld = rect.top == 0;
      \*******************************************************/
      case TMSIZEX: {
        pto->xfmChange.eM11 = (float) mouDevice.x;
        pto->xfmChange.eM11 -=pto->xfmChange.eDx;
        pto->xfmChange.eM11 /=(float) pto->rect.right ;
        SetWorldTransform (pto->hdc, &pto->xfmChange);
      } break;


      /*******************************************************\
      *    (     1           0          0   )
      *    (     0        Size Y        0   )
      *    (     0           0          1   )
      *
      * xWorld = rect.left == 0;
      \*******************************************************/
      case TMSIZEY: {
        pto->xfmChange.eM22 = (float) mouDevice.y;
        pto->xfmChange.eM22 -=pto->xfmChange.eDy;
        pto->xfmChange.eM22 /=(float) pto->rect.bottom ;
        SetWorldTransform (pto->hdc, &pto->xfmChange);
      } break;


      /*******************************************************\
      *    (     1           0          0   )
      *    (     0           1          0   )
      *    (   Move x      Move y       1   )
      *
      * xWorld = rect.left == 0;
      * yWorld = rect.top == 0;
      \*******************************************************/
      case TMMOVE: {
        pto->xfmChange.eDx = (float) mouDevice.x ;
        pto->xfmChange.eDy = (float) mouDevice.y ;
        SetWorldTransform (pto->hdc, &pto->xfmChange);
      } break;
    } /* end switch */

    doTrackObject(pto, TROB_PAINT, hwnd, lParam);

    return;
 }
