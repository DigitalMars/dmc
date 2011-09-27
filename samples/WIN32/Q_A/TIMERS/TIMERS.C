
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
*  PROGRAM:     TIMERS.C
*
*  PURPOSE:     To demonstrate the use of APIs SetTimer() and KillTimer().
*
*  FUNCTIONS:   WinMain               - standard WinMain()
*               MainWndProc           - main window procedure
*               TimerFunc             - handles timer messages
*               SetButtonPositions    - sizes buttons to client area
*               SetFlashRectPositions - sizes flashing rectangles to
*                                         client area
*               DrawStuff()             - draws separator, headers,
*                                         rectangles
*
*  COMMENTS:    When a timer is started it's corresponding rectangle (in
*               the client area) is inverted each time a WM_TIMER is
*               received or each timer the TimerFunc() is called.
*
\******************************************************************************/

#include <windows.h>
#include <string.h>
#include "timers.h"



/******************************************************************************\
*
*  FUNCTION:    WinMain (standard WinMain INPUTS/RETURNS)
*
*  GLOBAL VARS: ghInst - handle of program instance
*
*  LOCAL VARS:  hwnd - handle of the main standard window
*               msg  - msg to get/dispatch
*
\******************************************************************************/

int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow) {
  HWND hwnd;
  MSG msg;

  if (!hPrevInstance)
  {
    WNDCLASS wc;

    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)MainWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject (WHITE_BRUSH);
    wc.lpszMenuName  = (LPCTSTR) NULL;
    wc.lpszClassName = (LPCTSTR) "TIMERS";

    if (!RegisterClass (&wc))
    {
      MessageBox (NULL, (LPCTSTR) "WinMain(): RegisterClass() failed",
                  (LPCTSTR) "Err! - TIMERS", MB_OK | MB_ICONEXCLAMATION);
      return(FALSE);
    }
  }

  ghInst = hInstance;
  if (!(hwnd = CreateWindow ("TIMERS", "TIMERS Sample Application",
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, ghInst, NULL)))
    return (0);

  ShowWindow (hwnd, nCmdShow);

  while (GetMessage (&msg, NULL, 0,0))
  {
    TranslateMessage (&msg);
    DispatchMessage  (&msg);
  }
  return (msg.wParam);
  UNREFERENCED_PARAMETER(lpCmdLine);
}



/******************************************************************************\
*
*  FUNCTION:    MainWndProc (standard window procedure INPUTS/RETURNS)
*
*  GLOBAL VARS: ghInst       - handle of program instance
*               gahwndButtons - array of button window handles
*               gaFlashRects  - array of flashing rectangles coordinates
*
\******************************************************************************/

LRESULT CALLBACK MainWndProc (HWND hwnd, UINT message, WPARAM wParam,
                              LPARAM lParam)
{
  switch (message)
  {
    case WM_CREATE:
    {
      LONG i;

      //
      // Create the buttons to turn timer on/off
      //

      for (i= ID_TIMER1; i <= ID_TIMER4; i++)
      {
        gahwndButtons[i] = CreateWindow ("button", "Start timer",
                                       WS_CHILD | BS_PUSHBUTTON,
                                       0, 0, 0, 0, hwnd, NULL,
                                       ghInst, NULL);
        SetWindowLong (gahwndButtons[i], GWL_ID, i);
      }

      //
      // Have TIMER1 started initially (giving the user a clue what to do)
      //

      PostMessage (hwnd, WM_COMMAND, (DWORD) ID_TIMER1,
                   (LONG) gahwndButtons[ID_TIMER1]);
      break;
    }

    case WM_COMMAND:
    {
      char buf[BUFSIZE];
      WORD id;

      id = LOWORD (wParam);
      GetWindowText ((HWND) lParam, buf, BUFSIZE);
      if (!strcmp (buf, "Start timer"))
      {
        //
        // Set button text to "Stop timer" and start the timer. For
        //   timers 1&2 we'll specify a WNDPROC ("TimerFunc") which will
        //   get called for each timer tick. For timers 3&4 we'll just
        //   have WM_TIMER messages sent to this window procedure.
        //

        SetWindowText ((HWND) lParam, (LPTSTR) "Stop timer");
        if (id < ID_TIMER3)
        {
          if (SetTimer (hwnd, (UINT) id, (UINT) (id * TIMERINTERVAL),
                        (TIMERPROC) TimerFunc) == 0)

            MessageBox (NULL, (LPCTSTR) "MainWndProc(): SetTimer() failed",
                        (LPCTSTR) "Err! - TIMERS", MB_OK | MB_ICONEXCLAMATION);
        }
        else
        {
          if (SetTimer (hwnd, (UINT) id, (UINT) (id * TIMERINTERVAL),
                        NULL) == 0)

            MessageBox (NULL, (LPCTSTR) "MainWndProc(): SetTimer() failed",
                        (LPCTSTR) "Err! - TIMERS", MB_OK | MB_ICONEXCLAMATION);
        }
      }
      else
      {
        SetWindowText ((HWND) lParam, (LPTSTR) "Start timer");
        KillTimer (hwnd, (UINT) id);
      }
      break;
    }

    case WM_TIMER:
    {
      //
      // Invert the gaFlashRect associated with this timer event
      //

      HDC hdc = GetDC (hwnd);

      InvertRect (hdc, &gaFlashRects[wParam]); // wParam = timerId
      ReleaseDC  (hwnd, hdc);
      break;
    }

    case WM_SIZE:
    {
      int width  = (int) LOWORD(lParam);
      int height = (int) HIWORD(lParam);

      //
      // User resized the window, so reposition button & flashing rects
      //

      SetButtonPositions (width, height);
      SetFlashRectPositions (width, height);
      break;
    }

    case WM_PAINT:
    {
      PAINTSTRUCT ps;

      BeginPaint (hwnd, &ps);
      DrawStuff (hwnd, ps.hdc);
      EndPaint (hwnd, &ps);
      break;
    }

    case WM_DESTROY:

      PostQuitMessage(0);
      break;

    default:

      return (DefWindowProc(hwnd, message, wParam, lParam));
  }
  return (0);
}



/******************************************************************************\
*
*  FUNCTION:    TimerFunc
*
*  INPUTS:      hwnd     - window associated with timer
*               uMsg     - WM_TIMER message
*               idEvent  - timer's ID
*               dwTime   - current system time
*
*  RETURNS:     0
*
*  GLOBAL VARS: gaFlashRects - array of flashing rectangles coordinates
*
*  LOCAL VARS:  hdc - device context of main window
*
*  COMMENTS:    Inverts the gaFlashRect corresponding to idEvent
*
\******************************************************************************/

void CALLBACK TimerFunc (HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
  HDC hdc = GetDC (hwnd);

  InvertRect (hdc, &gaFlashRects[idEvent]);
  ReleaseDC  (hwnd, hdc);
}



/******************************************************************************\
*
*  FUNCTION:    SetButtonPositions
*
*  INPUTS:      cx - new width of main window
*               cy - new height of main window
*
*  GLOBAL VARS: gahwndButtons - array of button HWNDs (unmodified)
*
*  COMMENTS:    Repositions buttons according to new window size (cx,cy)
*
\******************************************************************************/

void SetButtonPositions (int cx, int cy)
{
  SetWindowPos (gahwndButtons[ID_TIMER1], NULL,     // upper-left button
                BUTTONBORDER,
                cy/2 - BUTTONBORDER - BUTTONHEIGHT,
                cx/2 - 2*BUTTONBORDER,
                BUTTONHEIGHT,
                SWP_SHOWWINDOW);

  SetWindowPos (gahwndButtons[ID_TIMER2], NULL,     // upper-right button
                cx/2 + BUTTONBORDER,
                cy/2 - BUTTONBORDER - BUTTONHEIGHT,
                cx/2 - 2*BUTTONBORDER,
                BUTTONHEIGHT,
                SWP_SHOWWINDOW);

  SetWindowPos (gahwndButtons[ID_TIMER3], NULL,     // lower-left button
                BUTTONBORDER,
                cy - BUTTONBORDER - BUTTONHEIGHT,
                cx/2 - 2*BUTTONBORDER,
                BUTTONHEIGHT,
                SWP_SHOWWINDOW);

  SetWindowPos (gahwndButtons[ID_TIMER4], NULL,     // lower-right button
                cx/2 + BUTTONBORDER,
                cy - BUTTONBORDER - BUTTONHEIGHT,
                cx/2 - 2*BUTTONBORDER,
                BUTTONHEIGHT,
                SWP_SHOWWINDOW);
}



/******************************************************************************\
*
*  FUNCTION:    SetgaFlashRectPositions
*
*  INPUTS:      cx - new width of main window
*               cy - new height of main window
*
*  GLOBAL VARS: gaFlashRects - array of flashing rectangles coordinates
*                            (modified)
*
*  COMMENTS:    Repositions flashing rectangles according to new window
*               size (cx,cy)
*
\******************************************************************************/

void SetFlashRectPositions (LONG cx, LONG cy)
{                                                 /* upper-left rect  */
  gaFlashRects[ID_TIMER1].left   = BUTTONBORDER;
  gaFlashRects[ID_TIMER1].top    = 3*TEXTHEIGHT + BUTTONBORDER;
  gaFlashRects[ID_TIMER1].right  = cx/2 - BUTTONBORDER;
  gaFlashRects[ID_TIMER1].bottom = cy/2 - BUTTONHEIGHT - 2*BUTTONBORDER;

                                                  /* upper-right rect */
  gaFlashRects[ID_TIMER2].left   = cx/2 + BUTTONBORDER;
  gaFlashRects[ID_TIMER2].top    = 3*TEXTHEIGHT + BUTTONBORDER;
  gaFlashRects[ID_TIMER2].right  = cx - BUTTONBORDER;
  gaFlashRects[ID_TIMER2].bottom = cy/2 - BUTTONHEIGHT - 2*BUTTONBORDER;

                                                  /* lower-left rect  */
  gaFlashRects[ID_TIMER3].left   = BUTTONBORDER;
  gaFlashRects[ID_TIMER3].top    = cy/2 + 3*TEXTHEIGHT + BUTTONBORDER;
  gaFlashRects[ID_TIMER3].right  = cx/2 - BUTTONBORDER;
  gaFlashRects[ID_TIMER3].bottom = cy - BUTTONHEIGHT - 2*BUTTONBORDER;

                                                  /* lower-right rect */
  gaFlashRects[ID_TIMER4].left   = cx/2 + BUTTONBORDER;
  gaFlashRects[ID_TIMER4].top    = cy/2 + 3*TEXTHEIGHT + BUTTONBORDER;
  gaFlashRects[ID_TIMER4].right  = cx - BUTTONBORDER;
  gaFlashRects[ID_TIMER4].bottom = cy - BUTTONHEIGHT - 2*BUTTONBORDER;
}



/******************************************************************************\
*
*  FUNCTION:    DrawStuff
*
*  INPUTS:      hwnd - main window handle
*               hdc  - DC for main window
*
*  GLOBAL VARS: gaFlashRects - array of flashing rectangles coordinates
*                            (not modified)
*
*  LOCAL VARS:  rect - client rectangle
*               hbr  - temporary brush handle
*
*  COMMENTS:    Draws separators, "Timer #" headers, & rectangles
*
\******************************************************************************/

void DrawStuff (HWND hwnd, HDC hdc)
{
  RECT   rect;
  HBRUSH hbr;

  //
  // Draw the separator lines between the timer info "windows"
  //

  GetClientRect (hwnd, &rect);
  MoveToEx (hdc, rect.right/2, 0, NULL);
  LineTo (hdc, rect.right/2, rect.bottom);

  MoveToEx (hdc, 0, rect.bottom/2, NULL);
  LineTo (hdc, rect.right, rect.bottom/2);

  //
  // Draw the "Timer #" headers
  //

  TextOut (hdc, BUTTONBORDER, TEXTHEIGHT, "Timer 1:", 8);
  TextOut (hdc, rect.right/2 + BUTTONBORDER, TEXTHEIGHT, "Timer 2:", 8);
  TextOut (hdc, BUTTONBORDER, rect.bottom/2 + TEXTHEIGHT, "Timer 3:", 8);
  TextOut (hdc, rect.right/2 + BUTTONBORDER, rect.bottom/2 + TEXTHEIGHT,
           "Timer 4:", 8);

  //
  // Draw the rectangles to flash on timer messages
  //

  hbr = CreateSolidBrush (0x0000ff);
  FillRect (hdc, &gaFlashRects[ID_TIMER1], hbr);
  DeleteObject (hbr);
  hbr = CreateSolidBrush (0x00ff00);
  FillRect (hdc, &gaFlashRects[ID_TIMER2], hbr);
  DeleteObject (hbr);
  hbr = CreateSolidBrush (0xff0000);
  FillRect (hdc, &gaFlashRects[ID_TIMER3], hbr);
  DeleteObject (hbr);
  hbr = CreateSolidBrush (0x2266aa);
  FillRect (hdc, &gaFlashRects[ID_TIMER4], hbr);
  DeleteObject (hbr);
}
