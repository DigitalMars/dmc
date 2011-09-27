
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
* status.c - module to support the status bar.
*
* resized by main frame window.
* Title and type text may be set in display.c
* text and painting managed in this module.
\**************************************************************************/
#define UNICODE

#include <windows.h>
#include "uniput.h"


/* prototype for function to do pseudo-3D outline */
VOID framechildwindow (HDC, HWND, HWND);


/* logfont for the font selected into the static windows on status bar */
LOGFONT logfontsmall = {
      UCFONTHEIGHT /2 ,
      UCFONTWIDTH /2,
       0 ,
       0 ,
     400 ,
       0 ,
       0 ,
       0 ,
       UNICODE_CHARSET ,
       0 ,
       0 ,
       2 ,
       2 ,
      TEXT("Lucida Sans Unicode")};



HWND hwndStatic0, hwndStatic1, hwndStatic2;

/**************************************************************************\
*
*  function:  StatusWndProc()
*
*  input parameters:  normal window procedure parameters.
*
*  global variables:
\**************************************************************************/
LRESULT CALLBACK StatusWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static HFONT hfontStatic;
RECT rect, clientrect;


  switch (message) {

    /**********************************************************************\
    *  WM_CREATE
    *
    * Create two static child windows, and select a font for them.
    \**********************************************************************/
    case WM_CREATE: {
      GetClientRect (hwnd, &clientrect);

      hwndStatic0 = CreateWindow(
              TEXT("STATIC"),
              TEXT("Target Window"),
              WS_CHILD | WS_VISIBLE | SS_CENTER,
              0,0,0,0,
              hwnd, NULL, hInst, 0);

      hwndStatic1 = CreateWindow(
              TEXT("STATIC"),
              TEXT("Unicode/Ansi"),
              WS_CHILD | WS_VISIBLE | SS_CENTER,
              0,0,0,0,
              hwnd, NULL, hInst, 0);

      hwndStatic2 = CreateWindow(
              TEXT("STATIC"),
              TEXT(""),
              WS_CHILD | WS_VISIBLE | SS_RIGHT,
              0,0,0,0,
              hwnd, NULL, hInst, 0);


      hfontStatic = CreateFontIndirect (&logfontsmall);
      SendMessage (hwndStatic0, WM_SETFONT, (WPARAM) hfontStatic, 0);
      SendMessage (hwndStatic1, WM_SETFONT, (WPARAM) hfontStatic, 0);
      SendMessage (hwndStatic2, WM_SETFONT, (WPARAM) hfontStatic, 0);
    } break;


    /**********************************************************************\
    *  WM_DESTROY
    *
    * free the font we created at WM_CREATE time.
    * hwndStatic* child windows are destroyed automatically.
    \**********************************************************************/
    case WM_DESTROY:
      DeleteObject (hfontStatic);
    break;


    /**********************************************************************\
    *  WM_CTLCOLORSTATIC
    \**********************************************************************/
    case WM_CTLCOLORSTATIC: {
      HDC hdc;

      hdc = (HDC) wParam;
      SetBkMode (hdc, TRANSPARENT);
      return (LRESULT)GetStockObject (LTGRAY_BRUSH);
    } break;


    /**********************************************************************\
    *  WM_SIZE
    *
    * Resize the static controls to stretch them to always fill the status
    *  bar minus some border area.
    \**********************************************************************/
    case WM_SIZE: {
      GetClientRect (hwnd, &clientrect);
      rect.top = clientrect.top + SBORDER;
      rect.bottom = clientrect.bottom - SBORDER;

      rect.left = SBORDER;
      rect.right = rect.left + SFIELD0;

      SetWindowPos (hwndStatic0, HWND_TOP,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top, 0);


      rect.left = 3*SBORDER + SFIELD0;
      rect.right = rect.left + SFIELD1;

      SetWindowPos (hwndStatic1, HWND_TOP,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top, 0);

      rect.left  = 5*SBORDER + SFIELD0 + SFIELD1;
      rect.right = clientrect.right - SBORDER;

      SetWindowPos (hwndStatic2, HWND_TOP,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top, 0);

      return TRUE;
    } break;




    /**********************************************************************\
    *  WMU_SETTARGETNAME
    *
    * wParam - BOOL... TRUE: hwndTarget is unicode, FALSE: ansi
    * lParam - pointer to new string.
    *
    \**********************************************************************/
    case WMU_SETTARGETNAME:

      SetWindowText (hwndStatic0,(LPCTSTR) lParam);

      if (wParam == FALSE)
        SetWindowText (hwndStatic1,TEXT ("Ansi"));
      else
        SetWindowText (hwndStatic1,TEXT ("Unicode"));

    break;


#define MAXCHARS 255

    /**********************************************************************\
    *  WMU_CHARACTER
    *
    * wParam - WCHAR value
    *
    * Display a string of characters in the second static window. This is
    *  a FIFO buffer, if we fill it, shift characters to the left, and
    *  set the string back into the static control.
    *
    \**********************************************************************/
    case WMU_CHARACTER: {
      TCHAR buffer[MAXCHARS+1];
      int nchar;
      int i;

      GetWindowText (hwndStatic2,buffer, MAXCHARS+1);
      nchar = GetWindowTextLength (hwndStatic2);

      if (nchar >= MAXCHARS) {
        nchar = MAXCHARS-1;
        for (i= 0; i<(MAXCHARS-1); i++)
          buffer[i] = buffer[i+1];
      }

      buffer[nchar] = (TCHAR) wParam;
      buffer[nchar+1] = (TCHAR) '\000';

      SetWindowText (hwndStatic2,buffer);

    } break;



    /**********************************************************************\
    *  WM_PAINT
    *
    * Gray background is done automatically, just outline the two static
    *  text windows with 3-D effect.
    \**********************************************************************/
    case WM_PAINT: {
      HDC hdc;
      PAINTSTRUCT ps;

      hdc = BeginPaint(hwnd, &ps);

      framechildwindow (hdc,hwnd, hwndStatic0);
      framechildwindow (hdc,hwnd, hwndStatic1);
      framechildwindow (hdc,hwnd, hwndStatic2);

      EndPaint (hwnd, &ps);
    } return FALSE; /* end WM_PAINT */

  } /* end switch */
  return (DefWindowProc(hwnd, message, wParam, lParam));
}



/**************************************************************************\
*
*  function:  framechildwindow()
*
* Simply draw a 3D frame around child window.
*
\**************************************************************************/
VOID framechildwindow (HDC hdc, HWND hwndParent, HWND hwndChild)
{
RECT rect;

      GetWindowRect (hwndChild, &rect);

      /* minor hack... assumes RECT is two points, right field starting first */
      ScreenToClient (hwndParent, (LPPOINT)&rect);
      ScreenToClient (hwndParent, (LPPOINT)&(rect.right));

      InflateRect (&rect, 1, 1);
      FrameRect (hdc, &rect, GetStockObject (GRAY_BRUSH));
      InflateRect (&rect, -1, -1);
      SelectObject (hdc, GetStockObject (WHITE_PEN));
      MoveToEx (hdc, rect.right, rect.top, NULL);
      LineTo (hdc,rect.right, rect.bottom);
      LineTo (hdc,rect.left, rect.bottom);

    return;
}
