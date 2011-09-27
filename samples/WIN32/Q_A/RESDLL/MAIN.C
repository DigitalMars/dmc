/******************************************************************************\
*
*  PROGRAM:     MAIN.C
*
*  PURPOSE:     To test the resource-only DLL "THE_DLL.DLL".
*
*  FUNCTIONS:   WinMain()     - initialization, create window, msg loop
*               MainWndProc() - processes main window msgs
*               DrawBitmap()  - draw a specified bitmap in a specified DC
*
*  COMMENTS:    Loads the bitmap, the cursor, and the icon from
*               THE_DLL.DLL. Uses the first two ni registering the
*               app's window class. Uses the bitmap when painting
*               the client area.
*
*
*                           Microsoft Developer Support
*                  Copyright (c) 1992-1995 Microsoft Corporation
*
\******************************************************************************/

#include <windows.h>
#include "Main.h"
#include "the_dll.h"



/******************************************************************************\
*
*  FUNCTION:    WinMain (standard WinMain INPUTS/RETURNS)
*
*  GLOBAL VARS: ghLib - library instance handle
*
*  LOCAL VARS:  hwnd - handle of the main standard window
*               msg  - msg to get/dispatch
*
\******************************************************************************/

int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,  int nCmdShow)
{
  HWND hwnd;
  MSG msg;


  if ((ghLib = LoadLibrary ((LPTSTR)"the_dll.dll")) == NULL)
  {
    MessageBox (NULL, (LPCTSTR) "WinMain(): LoadLibrary() failed",
                (LPCTSTR) "Err! - RESDLL", MB_OK | MB_ICONEXCLAMATION);
    return 0;
  }

  if (!hPrevInstance)
  {
    WNDCLASS wc;

    wc.style         = CS_HREDRAW | CS_VREDRAW;;
    wc.lpfnWndProc   = (WNDPROC)MainWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon (ghLib, "dllicon");
    wc.hCursor       = LoadCursor (ghLib, "dllcursor");
    wc.hbrBackground = GetStockObject (WHITE_BRUSH);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = (LPCTSTR) "RESDLL";

    if (!RegisterClass (&wc))
    {
      MessageBox (NULL, (LPCTSTR) "WinMain(): RegisterClass() failed",
                  (LPCTSTR) "Err! - MAIN", MB_OK | MB_ICONEXCLAMATION);
      return(FALSE);
    }
  }

  if (!(hwnd = CreateWindow ("RESDLL", "RESDLL Sample Application",
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, hInstance, NULL)))
    return (0);

  ShowWindow (hwnd, nCmdShow);

  while (GetMessage (&msg, NULL, 0,0))
  {
    TranslateMessage (&msg);
    DispatchMessage  (&msg);
  }
  FreeLibrary (ghLib);
  return (msg.wParam);
}



/******************************************************************************\
*
*  FUNCTION:    MainWndProc (standard window procedure INPUTS/RETURNS)
*
*  GLOBAL VARS: ghLib - library instance handle
*
*  LOCAL VARS:  hbm - handle of bitmap in THE_DLL.DLL
*
\******************************************************************************/

LRESULT CALLBACK MainWndProc (HWND hwnd, UINT message, WPARAM wParam,
                              LPARAM lParam)
{
  static HBITMAP hbm;

  switch (message)
  {
    case WM_CREATE:

      hbm = LoadBitmap (ghLib, "dllbitmap");
      break;

    case WM_PAINT:
    {
      RECT        rect;
      PAINTSTRUCT ps;

      GetClientRect (hwnd, &rect);
      BeginPaint (hwnd, &ps);

      //
      // draw the dllbitmap centered in middle of our client rect
      //

      DrawBitmap (ps.hdc, hbm,
                  rect.right/2 - DLLBITMAP_WIDTH/2,
                  rect.bottom/2 - DLLBITMAP_HEIGHT/2);
      EndPaint (hwnd, &ps);
      return 0;
    }

    case WM_DESTROY:

      DeleteObject ((HANDLE) hbm);
      PostQuitMessage (0);
      break;

    default:

      return (DefWindowProc(hwnd, message, wParam, lParam));
  }
  return (0);
}



/******************************************************************************\
*
*  FUNCTION:    DrawBitmap
*
*  INPUTS:      hdc    - device context in which to draw bitmap
*               hbm    - handle of bitmap to draw
*               xStart - x-coordinate of upper-left corner of destination
*                        rectangle
*               yStart - y-coordinate of upper-left corner of destination
*                        rectangle
*
*  LOCAL VARS:  bm     - BITMAP info of "hbm"
*               hdcMem - a memory DC used for blt-ing
*
*  COMMENTS:    Draws a bitmap "hbm" in a DC "hdc" given the upper-left
*               corner "xStart,yStart" of a destination rectangle.
*
\******************************************************************************/

void DrawBitmap (HDC hdc, HBITMAP hbm, int xStart, int yStart)
{
  BITMAP bm;
  HDC    hdcMem;

  hdcMem = CreateCompatibleDC (hdc);
  SelectObject (hdcMem, hbm);
  SetMapMode (hdcMem, GetMapMode(hdc));

  GetObject (hbm, sizeof(BITMAP), (LPSTR)&bm);
  BitBlt (hdc, xStart, yStart, bm.bmWidth, bm.bmHeight,
          hdcMem, 0, 0, SRCCOPY);

  DeleteDC(hdcMem);
}
