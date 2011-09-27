
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
*  PROGRAM:     LINKTEST.C
*
*  PURPOSE:     A simple demonstration of an executable file linking
*               with and calling into a DLL we created. This necessitates
*               the creation of a .LIB file for the DLL, which is used
*               when linking the .EXE. Doing this eliminates the need
*               for explicitly loading the DLL (via LoadLibrary()) at
*               runtime.
*
*  FUNTIONS:    WinMain     - initialization, create window, msg loop
*               MainWndProc - processes main window msgs
*               ThreadProc  - makes a single call into "THE_DLL.DLL"
*               AboutDlgProc- processes about dialog messages
*
\******************************************************************************/

#include <windows.h>
#include "linktest.h"
#include "the_dll.h"



/******************************************************************************\
*
*  FUNCTION:    WinMain (standard WinMain INPUTS/RETURNS)
*
*  GLOBAL VARS: ghwndMain - handle of main app window
*
*  LOCAL VARS:  msg - msg to get/dispatch
*
\******************************************************************************/

int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow)
{ MSG msg;

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
    wc.hbrBackground = GetStockObject (WHITE_BRUSH);
    wc.lpszMenuName  = (LPCTSTR) "Menu";
    wc.lpszClassName = (LPCTSTR) "LINKTEST";

    if (!RegisterClass (&wc))
    {
      MessageBox (NULL, (LPCTSTR) "WinMain(): RegisterClass() failed",
                  (LPCTSTR) "Err! - LINKTEST", MB_OK | MB_ICONEXCLAMATION);
      return(FALSE);
    }
  }

  if (!(ghwndMain = CreateWindow ("LINKTEST",
                                  "LINKTEST Sample Application",
                                  WS_OVERLAPPEDWINDOW,
                                  CW_USEDEFAULT, CW_USEDEFAULT,
                                  CW_USEDEFAULT, CW_USEDEFAULT,
                                  NULL, NULL, hInstance, NULL)))
    return (0);

  ShowWindow (ghwndMain, nCmdShow);

  while (GetMessage (&msg, NULL, 0, 0))
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
*  COMMENTS:    Menuitems IDM_DLLFUNCTION1 through IDM_DLLDIALOGBOX are
*               all processed by calling their respective functions in
*               "THE_DLL.DLL". The "CreateThread" menuitem causes a thread
*               to be created- the thread then calls into "THE_DLL.DLL".
*
\******************************************************************************/

LRESULT CALLBACK MainWndProc (HWND hwnd, UINT message, WPARAM wParam,
                              LPARAM lParam)
{
  switch (message)
  {
    case WM_COMMAND:

      switch (LOWORD(wParam))
      {
        case IDM_CREATETHREAD:
        {
          DWORD  threadId;

          CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) ThreadProc,
                        NULL, 0, &threadId);
          break;
        }

        case IDM_DLLFUNCTION1:

          DLLFunction1 ();
          break;

        case IDM_DLLFUNCTION2:

          DLLFunction2 ((int) 0);
          break;

        case IDM_DLLFUNCTION3:

          DLLFunction3 ((HANDLE) NULL);
          break;

        case IDM_DLLFUNCTION4:

          DLLFunction4 ((HWND) NULL);
          break;

        case IDM_DLLDIALOGBOX:

          DLLDialogBox (hwnd);
          break;

        case IDM_ABOUT:

          DialogBox (GetModuleHandle (NULL), (LPCTSTR)"About", hwnd,
                     (DLGPROC) AboutDlgProc);
          break;

        default:

          break;
      }
      break;

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
*  FUNCTION:    Thread
*
*  GLOBAL VARS: ghwndMain - handle of main app window
*
\******************************************************************************/

void ThreadProc ()
{
  MessageBox (ghwndMain, (LPCTSTR) "calling DLLFunction1",
              (LPCTSTR) "ThreadProc()", MB_OK);
  DLLFunction1 ();
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
