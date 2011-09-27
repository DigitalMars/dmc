
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
* uniput.c -- input of unicode characters.
\**************************************************************************/
#define UNICODE

#include <windows.h>
#include <commdlg.h>
#include "uniput.h"


/* Misc. defines for size, color, and appearance of drawing. */
#define GRIDCOLOR  PALETTEINDEX (6)
#define TICKSPACE  20
#define FWW_STEPS  40


HANDLE hFile = INVALID_HANDLE_VALUE;

HANDLE OpenNewFile(HWND);


TCHAR szHelpPathName[] = TEXT("uniput.HLP");

HANDLE hInst;
HWND   hwndMain;
HWND   hwndClient;
HWND   hwndStatus;

int gShowhex, gShowNames;

#define STATUSHEIGHT (UCFONTHEIGHT/2 + 3*SBORDER)


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
    MSG    msg;
    CLIENTCREATESTRUCT clientcreate;
    WNDCLASS  wc;
    DWORD dwVersion;


    UNREFERENCED_PARAMETER( lpCmdLine );
    UNREFERENCED_PARAMETER(  nCmdShow );
    
    /* Check to make sure we are running on Windows NT, not Win95 or Win32s */
    dwVersion = GetVersion();
    if (dwVersion & 0x80000000)
    {
        MessageBoxA(NULL, 
            "This sample application can only be run on Windows NT\n"
            "This application will now terminate",
            "UniPut",
            MB_ICONSTOP | MB_OK | MB_SETFOREGROUND);
        return(1);                
    }     
    
    hInst = hInstance;
    
    /* Check for previous instance.  If none, then register class. */
    if (!hPrevInstance) {

      wc.style = 0;
      wc.lpfnWndProc = (WNDPROC)MainWndProc;

      wc.cbClsExtra = 0;
      wc.cbWndExtra = 0;
      wc.hInstance = hInstance;
      wc.hIcon = LoadIcon(hInstance, TEXT("uniputIcon"));
      wc.hCursor = LoadCursor(NULL, IDC_ARROW);
      wc.hbrBackground = NULL;
      wc.lpszMenuName =  TEXT("uniputMenu");
      wc.lpszClassName = TEXT("uniput");

      if (!RegisterClass(&wc)) return (FALSE);

    }  /* class registered o.k. */


    /* Create the main window.  Return false if CreateWindow() fails */
    hwndMain = CreateWindow(
        TEXT("uniput"),
        TEXT("Input unicode characters"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL, NULL, hInst, NULL);

    if (!hwndMain) return (FALSE);


    /* create the MDI "client" window. */
    clientcreate.hWindowMenu  = GetSubMenu(GetMenu(hwndMain),1);
    clientcreate.idFirstChild = 1;
    hwndClient = CreateWindow(
        TEXT("MDICLIENT"),
        NULL,
        WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE | MDIS_ALLCHILDSTYLES,
        0,0,0,0,
        hwndMain, NULL, hInst, (LPVOID)&clientcreate);
    if (!hwndClient) return (FALSE);



    /* create status bar */
    wc.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)StatusWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject (LTGRAY_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = TEXT("status");
    if (!RegisterClass(&wc)) return (FALSE);

    hwndStatus = CreateWindow(
        TEXT("status"),
        TEXT("status"),
        WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | WS_BORDER,
        0,0,
        GetSystemMetrics(SM_CXFULLSCREEN),
        STATUSHEIGHT,
        hwndMain, NULL, hInst, 0);
    if (!hwndStatus) return (FALSE);


    ShowWindow(hwndMain, SW_SHOWDEFAULT);
    SetCursor (LoadCursor (NULL, IDC_WAIT));


    /* Register display window class, and do class initialization. */
    if (!openDisplay()) return (FALSE);

    /* set the menus to be correct */
    SendMessage (hwndMain, WM_COMMAND, IDM_SHOWHEX,0);
    SendMessage (hwndMain, WM_COMMAND, IDM_SHOWNAMES,0);



    /* Create one MDI child window to get started */
    SendMessage (hwndMain, WM_COMMAND, IDM_NEWWINDOW, 0);

    SetCursor (LoadCursor (NULL, IDC_ARROW));



    /* Loop getting messages and dispatching them. */
    while (GetMessage(&msg,NULL, 0,0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    closeDisplay();
    return (msg.wParam);
}



/**************************************************************************\
*
*  function:  MainWndProc()
*
*  input parameters:  normal window procedure parameters.
*
\**************************************************************************/
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
HWND hwndActive;

  switch (message) {

    /**********************************************************************\
    *  WM_CREATE
    \**********************************************************************/
    case WM_CREATE: {

    }


    /**********************************************************************\
    *  WM_SIZE
    *
    * When the frame is sized, then change the size of the MDI client,
    *  excluding the status bar, and change the size and position of the
    *  status bar.
    \**********************************************************************/
    case WM_SIZE: {

      RECT r;
      GetClientRect (hwnd, &r);
      r.bottom -= STATUSHEIGHT;

      SetWindowPos (hwndClient, HWND_BOTTOM,
                    r.left, r.top, r.right, r.bottom, 0);

      /* size & position status bar: left, right, & top just out of view */
      SetWindowPos (hwndStatus, HWND_TOP,
        r.left-1, r.bottom,
        r.right+2, STATUSHEIGHT+1, 0);


      return TRUE;

    } break;



    /**********************************************************************\
    *  WMU_CHARACTER
    *
    * global variable: hFile
    *
    * user message sent to main window when user hits a glyph square.
    *  if the main window has a file open, then write this unicode
    *  character to that file.
    \**********************************************************************/
    case WMU_CHARACTER: {
        DWORD nBytes;
        WCHAR tChar;

      tChar = (WCHAR)lParam;

      /* show it on the status bar */
      SendMessage (hwndStatus, WMU_CHARACTER, 0, lParam);

      if (hFile != INVALID_HANDLE_VALUE)
        WriteFile (hFile, &tChar, sizeof(WCHAR), &nBytes, NULL);

    } break;



    /**********************************************************************\
    *  WM_COMMAND
    *
    \**********************************************************************/
    case WM_COMMAND: {

      switch (LOWORD(wParam)) {

        /******************************************************************\
        *  WM_COMMAND, IDM_NEWWINDOW
        *
        * global - hwndClient, hInst
        *
        * Simply create a new MDI child window.
        \******************************************************************/
        case IDM_NEWWINDOW: {
          HWND hwndDisplay;

          hwndDisplay = CreateMDIWindow(
              DISPLAYCLASS,
              TEXT("Display"),
              WS_CHILD | WS_CLIPSIBLINGS | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE |
                         WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_HSCROLL,
              CW_USEDEFAULT,
              CW_USEDEFAULT,
              CW_USEDEFAULT,
              CW_USEDEFAULT,
              hwndClient, hInst, 0);
        } break;


        /******************************************************************\
        *  WM_COMMAND, IDM_NEWFILE
        *
        * global - hFile.
        \******************************************************************/
        case IDM_NEWFILE: {
          DWORD nBytes;

          hFile = OpenNewFile (hwnd);
          /* Write the unicode byte order mark */
          WriteFile (hFile, "\xFE\xFF", 2, &nBytes, NULL);
        } break;


        /******************************************************************\
        *  WM_COMMAND, IDM_CLOSEFILE
        *
        * global - hFile.
        \******************************************************************/
        case IDM_CLOSEFILE:
          CloseHandle (hFile);
          hFile = INVALID_HANDLE_VALUE;
          SendMessage (hwndStatus, WMU_SETTARGETNAME, 0, (LPARAM)NULL);
        break;


        case IDM_HELP:
          WinHelp( hwnd, szHelpPathName, HELP_INDEX, (DWORD) NULL );
        break;


        case IDM_ABOUT:
          DialogBox (hInst, TEXT("aboutBox"), hwnd, (DLGPROC) About);
        break;



        /******************************************************************\
        *  WM_COMMAND, IDM_SHOWHEX
        *
        * Check menu on and off.  Set global value: gShowHex
        \******************************************************************/
        case IDM_SHOWHEX:
          if (GetMenuState (GetMenu (hwnd),IDM_SHOWHEX, MF_BYCOMMAND)
                == MF_CHECKED) {
            CheckMenuItem(GetMenu (hwnd), LOWORD(wParam) , MF_UNCHECKED);
            gShowhex = FALSE;
            RedrawWindow (hwnd, NULL, NULL, RDW_INVALIDATE |RDW_ALLCHILDREN);
          } else {
            CheckMenuItem(GetMenu (hwnd), LOWORD(wParam) , MF_CHECKED);
            gShowhex = TRUE;
            RedrawWindow (hwnd, NULL, NULL, RDW_INVALIDATE |RDW_ALLCHILDREN);
          }
        break;


        /******************************************************************\
        *  WM_COMMAND, IDM_SHOWNAMES
        *
        * Check menu on and off.  Set global value: gShowNames
        \******************************************************************/
        case IDM_SHOWNAMES:
          if (GetMenuState (GetMenu (hwnd),IDM_SHOWNAMES, MF_BYCOMMAND)
                == MF_CHECKED) {
            CheckMenuItem(GetMenu (hwnd), LOWORD(wParam) , MF_UNCHECKED);
            gShowNames = FALSE;
          } else {
            CheckMenuItem(GetMenu (hwnd), LOWORD(wParam) , MF_CHECKED);
            gShowNames = TRUE;
          }
          // PROBLEM:  Need to send message to all child windows.

          hwndActive = (HWND)SendMessage (hwndClient, WM_MDIGETACTIVE, 0,0);
          SendMessage (hwndActive, WMU_NEWRANGE, 0,0);
          InvalidateRect (hwndActive, NULL, TRUE);

        break;


        /******************************************************************\
        *  WM_COMMAND, IDM_CHAR*
        *
        * manage mutually exclusive menu indicating number of chars per line
        \******************************************************************/
        case IDM_CHAR10:
        case IDM_CHAR16:
        case IDM_CHAR20:
        case IDM_CHAR32:
        case IDM_CHAR40:
        case IDM_CHAR64:

        { HMENU hMenu;
          int nCharPerLine;

          hMenu = GetMenu (hwnd);

          CheckMenuItem(hMenu, IDM_CHAR10, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_CHAR16, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_CHAR20, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_CHAR32, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_CHAR40, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_CHAR64, MF_UNCHECKED);

          CheckMenuItem(hMenu, LOWORD(wParam), MF_CHECKED);

          /* notice the value is coded into the menu i.d. */
          nCharPerLine = LOWORD(wParam)-IDM_CHARBASE;
          hwndActive = (HWND)SendMessage (hwndClient, WM_MDIGETACTIVE, 0,0);
          SendMessage (hwndActive, WMU_SETNCHAR, nCharPerLine ,0);
          InvalidateRect (hwndActive, NULL, TRUE);

        } break;



        /******************************************************************\
        *  WM_COMMAND, IDM_FONT*
        *
        * manage mutually exclusive menu indicating font height
        \******************************************************************/
        case IDM_FONT18:
        case IDM_FONT36:
        case IDM_FONT72:

        { HMENU hMenu;
          int Width, Height;
          hMenu = GetMenu (hwnd);

          CheckMenuItem(hMenu, IDM_FONT18, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_FONT36, MF_UNCHECKED);
          CheckMenuItem(hMenu, IDM_FONT72, MF_UNCHECKED);

          CheckMenuItem(hMenu, LOWORD(wParam), MF_CHECKED);

          /* notice the value is coded into the menu i.d. */
          Height = LOWORD(wParam)-IDM_FONTBASE;
          /* scale the width proportionately */
          Width = Height * UCFONTWIDTH/UCFONTHEIGHT;

          hwndActive = (HWND)SendMessage (hwndClient, WM_MDIGETACTIVE, 0,0);
          SendMessage (hwndActive, WMU_NEWFONT, (WPARAM)Width,(LPARAM)Height);
          InvalidateRect (hwndActive, NULL, TRUE);

        } break;



        /******************************************************************\
        *  WM_COMMAND, IDM_MDI*
        *
        * misc. MDI responsibilities.  hwndClient does real work.
        \******************************************************************/
        case IDM_MDICASCADE: SendMessage (hwndClient, WM_MDICASCADE, 0,0); break;
        case IDM_MDITILE:    SendMessage (hwndClient, WM_MDITILE, MDITILE_HORIZONTAL,0); break;
        break;




      } /* end switch */
    } break;  /* end WM_COMMAND */





    /**********************************************************************\
    *  WM_DESTROY
    \**********************************************************************/
    case WM_DESTROY:
      WinHelp( hwnd, szHelpPathName, (UINT) HELP_QUIT, (DWORD) NULL );
      PostQuitMessage(0);
    break;


  } /* end switch */
  return (DefFrameProc(hwnd, hwndClient, message, wParam, lParam));
}



/***************************************************************************\
*    FUNCTION: About
\***************************************************************************/
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







/***************************************************************************\
*
*    FUNCTION: OpenNewFile(HWND)
*
*    PURPOSE:  Invokes common dialog function to open a file and opens it.
*
\***************************************************************************/
HANDLE OpenNewFile( HWND hwnd )
{
   TCHAR szFile[MAX_PATH],szFileTitle[MAX_PATH];
   OPENFILENAME OpenFileName;


   wsprintf (szFile, TEXT(""));
   wsprintf (szFileTitle, TEXT(""));

   OpenFileName.lStructSize       = sizeof(OPENFILENAME);
   OpenFileName.hwndOwner         = hwnd;
   OpenFileName.hInstance         = (HANDLE) hInst;
   OpenFileName.lpstrFilter       = NULL;
   OpenFileName.lpstrCustomFilter = NULL;
   OpenFileName.nMaxCustFilter    = 0L;
   OpenFileName.nFilterIndex      = 1L;
   OpenFileName.lpstrFile         = szFile;
   OpenFileName.nMaxFile          = sizeof(szFile);
   OpenFileName.lpstrFileTitle    = szFileTitle;
   OpenFileName.nMaxFileTitle     = sizeof(szFileTitle);
   OpenFileName.lpstrInitialDir   = NULL;
   OpenFileName.lpstrTitle        = TEXT("Open a File");

   OpenFileName.nFileOffset       = 0;
   OpenFileName.nFileExtension    = 0;
   OpenFileName.lpstrDefExt       = NULL;

   OpenFileName.lCustData         = 0;

   OpenFileName.Flags = OFN_HIDEREADONLY;

   if (!GetOpenFileName(&OpenFileName)) return INVALID_HANDLE_VALUE;

   /* update status bar with the file title. */
   SendMessage (hwndStatus, WMU_SETTARGETNAME, 0, (LPARAM)OpenFileName.lpstrFileTitle);

   return (CreateFile(OpenFileName.lpstrFile,
                      GENERIC_WRITE,
                      0,
                      NULL,
                      CREATE_ALWAYS,
                      FILE_ATTRIBUTE_NORMAL,
                      NULL));
}
