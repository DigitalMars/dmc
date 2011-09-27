
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
*  PROGRAM:     LOADTEST.C
*
*  PURPOSE:     A simple demonstration of LoadLibrary()-ing a DLL at
*               runtime and obtaining function addresses within the DLL.
*
*  FUNTIONS:    WinMain     - initialization, create window, msg loop
*               MainWndProc - processes main window msgs
*               ThreadProc  - makes a single call into "THE_DLL.DLL"
*               AboutDlgProc- processes about dialog messages
*
\******************************************************************************/

#include <windows.h>
#include "loadtest.h"
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
{
  MSG msg;

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
    wc.lpszClassName = (LPCTSTR) "LOADTEST";

    if (!RegisterClass (&wc))
    {
      MessageBox (NULL, (LPCTSTR) "WinMain(): RegisterClass() failed",
                  (LPCTSTR) "Err! - LOADTEST", MB_OK | MB_ICONEXCLAMATION);
      return(FALSE);
    }
  }

  if (!(ghwndMain = CreateWindow ("LOADTEST", "LOADTEST Sample Application",
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
*  GLOBAL VARS: pfnDLLFunction1 - pointer to DLLFunction1
*               pfnDLLFunction2 - pointer to DLLFunction2
*               pfnDLLFunction3 - pointer to DLLFunction3
*               pfnDLLFunction4 - pointer to DLLFunction4
*               pfnDLLDialogBox - pointer to DLLDialogBox
*
*  LOCAL VARS:  hLib     - handle of THE_DLL.DLL
*               hSubMenu - handle of our "Options" submenu
*
*  COMMENTS:    Menuitems IDM_DLLFUNCTION1 through IDM_DLLDIALOGBOX are
*               all processed by calling their respective functions in
*               "THE_DLL.DLL". The "CreateThread" menuitem causes a thread
*               to be created- the thread then calls into "THE_DLL.DLL".
*
\******************************************************************************/

LRESULT CALLBACK MainWndProc (HWND hwnd, UINT message, WPARAM wParam,
                              LPARAM lParam)
{ static HANDLE hLib = NULL;
  static HMENU  hSubMenu;

  switch (message)
  {
    case WM_CREATE:
    {
      HMENU hMenu = GetMenu (hwnd);

      hSubMenu = GetSubMenu (hMenu, 0);
      FixMenu (IDM_FREELIBRARY, hSubMenu);
      break;
    }

    case WM_COMMAND:

      switch (LOWORD(wParam))
      {
        case IDM_LOADLIBRARY:

          if (!hLib)
          {
            if (!(hLib = LoadLibrary ("THE_DLL.DLL")))

              MessageBox (hwnd,
                          (LPCTSTR) "MainWndProc(): LoadLibrary () failed",
                          (LPCTSTR) "Err! - LOADTEST",
                          MB_OK | MB_ICONEXCLAMATION);
            else
            {
              gpfnDLLFunction1 = (PFNDLL) GetProcAddress (hLib,
                                                          "DLLFunction1");
              gpfnDLLFunction2 = (PFNDLL) GetProcAddress (hLib,
                                                          "DLLFunction2");
              gpfnDLLFunction3 = (PFNDLL) GetProcAddress (hLib,
                                                          "DLLFunction3");
              gpfnDLLFunction4 = (PFNDLL) GetProcAddress (hLib,
                                                          "DLLFunction4");
              gpfnDLLDialogBox = (PFNDLL) GetProcAddress (hLib,
                                                          "DLLDialogBox");
              FixMenu (IDM_LOADLIBRARY, hSubMenu);
            }
          }
          break;

        case IDM_FREELIBRARY:

          if (hLib)
          {
            FreeLibrary (hLib);
            gpfnDLLFunction1 = NULL;
            gpfnDLLFunction2 = NULL;
            gpfnDLLFunction3 = NULL;
            gpfnDLLFunction4 = NULL;
            gpfnDLLDialogBox = NULL;
            FixMenu (IDM_FREELIBRARY, hSubMenu);
            hLib = NULL;
          }
          break;

        case IDM_CREATETHREAD:
        {
          DWORD tid; /* thread identifier */

          if (gpfnDLLFunction1)

            CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)
                          ThreadProc, NULL, 0, &tid);

          break;
        }

        case IDM_DLLFUNCTION1:

          if (gpfnDLLFunction1)

            (gpfnDLLFunction1) ();

          break;

        case IDM_DLLFUNCTION2:

          if (gpfnDLLFunction2)

            (gpfnDLLFunction2) ((int) 0);

          break;

        case IDM_DLLFUNCTION3:

          if (gpfnDLLFunction3)

            (gpfnDLLFunction3) ((HANDLE) NULL);

          break;

        case IDM_DLLFUNCTION4:

          if (gpfnDLLFunction4)

            (gpfnDLLFunction4) ((HWND) NULL);

          break;

        case IDM_DLLDIALOGBOX:

          if (gpfnDLLDialogBox)

            (gpfnDLLDialogBox) (hwnd);

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
*  GLOBAL VARS: ghwndMain        - handle of main app window
*               pfnDLLFunction1 - pointer to DLLFunction1
*
*  COMMENTS:    Makes a call into "THE_DLL.DLL", and then waits for the
*               main window to call TerminateThread().
*
\******************************************************************************/

void ThreadProc ()
{
  MessageBox (ghwndMain, (LPCTSTR) "calling DLLFunction1",
              (LPCTSTR) "ThreadProc()", MB_OK);
  (gpfnDLLFunction1) ();
}



/******************************************************************************\
*
*  FUNCTION:    FixMenu
*
*  INPUTS:      choice   - IDM_LOADLIBRARY or IBM_FREELIBRARY
*               hSubMenu - handle of submenu, the items of which we'll
*                          enable/disable
*
*  LOCAL VARS:  i - loop variable
*
*  COMMENTS:    Enables/disables menuitems depending on whether THE_DLL
*               is loaded/unloaded.
*
\******************************************************************************/

void FixMenu (UINT choice, HMENU hSubMenu)
{
  UINT i;

  if (choice == IDM_LOADLIBRARY)
  {
    EnableMenuItem (hSubMenu, IDM_LOADLIBRARY, MF_DISABLED | MF_GRAYED |
                                               MF_BYCOMMAND);

    for (i = IDM_FREELIBRARY; i <= IDM_DLLDIALOGBOX; i++)

      EnableMenuItem (hSubMenu, i, MF_ENABLED | MF_BYCOMMAND);
  }
  else /* choice == IDM_FREELIBRARY */
  {
    EnableMenuItem (hSubMenu, IDM_LOADLIBRARY, MF_ENABLED | MF_BYCOMMAND);

    for (i = IDM_FREELIBRARY; i <= IDM_DLLDIALOGBOX; i++)

      EnableMenuItem (hSubMenu, i, MF_DISABLED | MF_GRAYED |MF_BYCOMMAND);

  }
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
