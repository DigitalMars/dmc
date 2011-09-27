/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1992 - 1995  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/
/*----------------------------------------------------------------------------*\
 *
 *  MCIPUZZLE:
 *
 *    Sample app showing the use of MCIWnd, installable draw procedure
 *
 *    MCIPUZZLE is an application that demonstrates the following:
 *
 *      - Using the MCIWnd window class to play MCI files.
 *
 *      - Using an installable draw procedure with MCIAVI
 *
 *      - Implementing a version of the classic "15" puzzle
 *
 *----------------------------------------------------------------------------*/

#define  STRICT
#define INC_OLE2
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <mmsystem.h>
#include <digitalv.h>
#include <mciavi.h>
#include <vfw.h>

#include "mcipuzzl.h"
#include "puzzle.h"
#include "puzzproc.h"

//---------------------------------------------------------------------------

#define BUFSIZE 260
#define MCIPLAY_APP_CLASS "MCIPuzzle_App"
#define MCIPLAY_DOC_CLASS MCIWND_WINDOW_CLASS
typedef MCI_DGV_SETVIDEO_PARMS * LP_MCI_DGV;

/*-------------------------------------------------------------------------*\
|                                                                          |
|   g l o b a l   v a r i a b l e s                                        |
|                                                                          |
\*------------------------------------------------------------------------*/

static  HINSTANCE ghInstApp;            /* Instance handle */
static  HACCEL    ghAccelApp;
static  HWND      ghwndApp;             /* Handle to parent window */
static  HWND      ghwndMCI;             /* Handle to MCI client window */
static  char      gszBuffer[BUFSIZE];
static  DWORD     gdwHook = 0L;

PUZZLE            gPuzzle;

//---------------------------------------------------------------------------

LRESULT CALLBACK AppWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK AppAbout(HWND, UINT, WPARAM, LPARAM);

static BOOL AppInit(HINSTANCE, HINSTANCE, LPSTR, int);
static BOOL FormatFilterString(UINT, LPSTR);
static void MCI_OnCommand(HWND, int, HWND, UINT);
static void MCI_OnInitMenuPopup(HWND, HMENU, int, BOOL);
static void MCI_OnSize(HWND, UINT, int, int);
static void MCI_OnParentNotify(HWND, UINT, HWND, int);
static void Handle_NotifyMedia(HWND,WPARAM);
static void Handle_NotifySize(HWND,WPARAM);

/*----------------------------------------------------------------------------*\
|   AppAbout( hDlg, msg, wParam, lParam )                                      |
|                                                                              |
|   Description:                                                               |
|       This function handles messages belonging to the "About" dialog box.    |
|       The only message that it looks for is WM_COMMAND, indicating the user  |
|       has pressed the "OK" button.  When this happens, it takes down         |
|       the dialog box.                                                        |
|                                                                              |
|   Arguments:                                                                 |
|       hDlg            window handle of about dialog window                   |
|       msg             message number                                         |
|       wParam          message-dependent                                      |
|       lParam          message-dependent                                      |
|                                                                              |
|   Returns:                                                                   |
|       TRUE if message has been processed, else FALSE                         |
|                                                                              |
\*----------------------------------------------------------------------------*/
BOOL CALLBACK AppAbout(
HWND hwnd,
UINT msg,
WPARAM wParam,
LPARAM lParam)
{
        switch (msg) {
                case WM_COMMAND:
                EndDialog(hwnd,TRUE);
                return TRUE;
        }
        return FALSE;
}

/*----------------------------------------------------------------------------*\
|   AppInit ( hInstance, hPrevInstance )                                       |
|                                                                              |
|   Description:                                                               |
|       This is called when the application is first loaded into               |
|       memory.  It performs all initialization that doesn't need to be done   |
|       once per instance.                                                     |
|                                                                              |
|   Arguments:                                                                 |
|       hPrevInstance   instance handle of previous instance                   |
|       hInstance       instance handle of current instance                    |
|                                                                              |
|   Returns:                                                                   |
|       TRUE if successful, FALSE if not                                       |
|                                                                              |
\*----------------------------------------------------------------------------*/
static BOOL AppInit(
HINSTANCE hInst,
HINSTANCE hPrev,
LPSTR szCmd,
int sw)
{
        WNDCLASS cls;
        WORD wVer;
        char szAppName[BUFSIZE];

        /* Save instance handle for DialogBox */
        ghInstApp = hInst;

        LoadString( ghInstApp, IDS_APPNAME, szAppName, BUFSIZE );

        /* first let's make sure we are running on 1.1 */
        wVer = HIWORD(VideoForWindowsVersion());
        if (wVer < 0x010a){
                char szTitle[BUFSIZE];
                /* oops, we are too old, blow out of here */
                LoadString( ghInstApp, IDS_APPERR, szTitle, BUFSIZE );
                LoadString( ghInstApp, IDS_VFWTOOOLD, gszBuffer, BUFSIZE );
                MessageBeep(MB_ICONHAND);
                MessageBox(NULL, gszBuffer, szTitle, MB_OK|MB_ICONSTOP);
                return FALSE;
        }

        ghAccelApp = LoadAccelerators(ghInstApp, "AppAccel");

        if (!hPrev) {
                cls.hCursor        = LoadCursor(NULL,IDC_ARROW);
                cls.hIcon          = LoadIcon(hInst,"AppIcon");
                cls.lpszMenuName   = "AppMenu";
                cls.lpszClassName  = MCIPLAY_APP_CLASS;
                cls.hbrBackground  = (HBRUSH)COLOR_APPWORKSPACE+1;
                cls.hInstance      = hInst;
                cls.style          = 0;
                cls.lpfnWndProc    = (WNDPROC)AppWndProc;
                cls.cbClsExtra     = 0;
                cls.cbWndExtra     = 0;

                if (!RegisterClass(&cls))
                        return FALSE;
        }

        ghwndApp = CreateWindow(
                MCIPLAY_APP_CLASS,
                szAppName,
                WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                CW_USEDEFAULT,
                0,
                160,
                120,
                (HWND)NULL,       /* no parent */
                (HMENU)NULL,      /* use class menu */
                (HANDLE)hInst,     /* handle to window instance */
                (LPSTR)NULL);     /* no params to pass on */

        InitPuzzle(&gPuzzle, TRUE /* FALSE to not scramble */);

        /* Make window visible according to the way the app is activated */
        ShowWindow(ghwndApp,sw);

        return TRUE;
}

/*----------------------------------------------------------------------------*\
|   WinMain( hInstance, hPrevInstance, lpszCmdLine, cmdShow )                  |
|                                                                              |
|   Description:                                                               |
|       The main procedure for the App.  After initializing, it just goes      |
|       into a message-processing loop until it gets a WM_QUIT message         |
|       (meaning the app was closed).                                          |
|                                                                              |
|   Arguments:                                                                 |
|       hInstance       instance handle of this instance of the app            |
|       hPrevInstance   instance handle of previous instance, NULL if first    |
|       lpszCmdLine     ->null-terminated command line                         |
|       cmdShow         specifies how the window is initially displayed        |
|                                                                              |
|   Returns:                                                                   |
|       The exit code as specified in the WM_QUIT message.                     |
|                                                                              |
\*----------------------------------------------------------------------------*/
int PASCAL WinMain(
HINSTANCE hInstance,
HINSTANCE hPrevInstance,
LPSTR szCmdLine,
int sw)
{
        MSG msg;

        if (!AppInit(hInstance,hPrevInstance,szCmdLine,sw))
                return FALSE;

        //Polling messages from event queue
        for (;;) {
                if (PeekMessage(&msg, NULL, 0, 0,PM_REMOVE)) {
                        if (msg.message == WM_QUIT)
                                break;

                        if (ghAccelApp && ghwndApp && TranslateAccelerator(ghwndApp, ghAccelApp, &msg))
                                continue;

                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                        } else {
                        // idle time here, DONT BE A PIG!
                        WaitMessage();
                }
        }
        return msg.wParam;
}


//----------------------------------------------------------------------------
// MCI_OnCommand - Handles WM_COMMAND
//----------------------------------------------------------------------------
static void MCI_OnCommand(
HWND hwnd,
int id,
HWND hwndCtl,
UINT codeNotify)
{
        switch(id) {
                case MENU_ABOUT:
                DialogBox(ghInstApp,MAKEINTRESOURCE(ABOUTBOX),hwnd,AppAbout);
                break;

                case MENU_EXIT:
                PostMessage(hwnd,WM_CLOSE,0,0L);
                break;

                case MENU_CLOSE:
                PostMessage(ghwndMCI, WM_CLOSE, 0, 0L);
                break;

                case MENU_OPEN:
                {
                        OPENFILENAME ofn;
                        char szFileName[BUFSIZE];
                        char szFilter[BUFSIZE];

                        FormatFilterString( IDS_FILTERSTRING, szFilter );

                        /* prompt user for file to open */
                        ofn.lStructSize = sizeof(OPENFILENAME);
                        ofn.hwndOwner = hwnd;
                        ofn.hInstance = NULL;
                        ofn.lpstrFilter = szFilter;
                        ofn.lpstrCustomFilter = NULL;
                        ofn.nMaxCustFilter = 0;
                        ofn.nFilterIndex = 0;
                        *szFileName = '\0';
                        ofn.lpstrFile = szFileName;
                        ofn.nMaxFile = BUFSIZE;
                        ofn.lpstrFileTitle = NULL;
                        ofn.nMaxFileTitle = 0;
                        ofn.lpstrInitialDir = NULL;
                        ofn.lpstrTitle = "Open";
                        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
                        ofn.nFileOffset = 0;
                        ofn.nFileExtension = 0;
                        ofn.lpstrDefExt = NULL;
                        ofn.lCustData = 0;
                        ofn.lpfnHook = NULL;
                        ofn.lpTemplateName = NULL;

                        if (GetOpenFileName(&ofn)) {
                                MCIWndOpen(ghwndMCI, (LPARAM)(LPSTR)szFileName, 0);
                                InitPuzzle(&gPuzzle, TRUE /* FALSE to not scramble */);
                        }
                }
                break;


                /* Movie Menu (for use by accelerators) */
                case IDM_PLAY:
                        MCIWndPlay(ghwndMCI);
                        break;

                case IDM_RPLAY:
                        MCIWndPlayReverse(ghwndMCI);
                        break;

      // Solve or scramble the puzzle, as appropriate
                case IDM_SOLVE:
                case IDM_SCRAMBLE:
                    InitPuzzle(&gPuzzle, id == IDM_SCRAMBLE);
                    InvalidateRect(ghwndMCI, NULL, FALSE);
                    break;
        }
        return;
}

//----------------------------------------------------------------------------
// MCI_OnSize - Handles WM_SIZE
//----------------------------------------------------------------------------
static void MCI_OnSize(
HWND hwnd,
UINT state,
int cx,
int cy)
{
        MoveWindow(ghwndMCI,0,0,cx,cy,TRUE);
        return;
}
//----------------------------------------------------------------------------
// MCI_OnParentNotify - Handles WM_PARENTNOTIFY
//----------------------------------------------------------------------------
static void MCI_OnParentNotify(
HWND hwnd,
UINT msg,
HWND hwndChild,
int idChild)
{
    if (msg == WM_LBUTTONDOWN && ghwndMCI) {
        RECT    rc;
        POINT   pt;

        pt.x = (int)LOWORD((DWORD)hwndChild);
        pt.y = (int)HIWORD((DWORD)hwndChild);

        ClientToScreen(hwnd, &pt);

        MCIWndGetDest(ghwndMCI, &rc);

        if (rc.bottom != rc.top && rc.right != rc.left) {
            ScreenToClient(ghwndMCI, &pt);

            ClickPuzzle(&gPuzzle,
                    (pt.x - rc.left) * PSIZE / (rc.right - rc.left),
                    (pt.y - rc.top) * PSIZE / (rc.bottom - rc.top));

            InvalidateRect(ghwndMCI, &rc, FALSE);
        }
    }
    return;
}
/*----------------------------------------------------------------------------*\
|   AppWndProc( hwnd, msg, wParam, lParam )                                    |
|                                                                              |
|   Description:                                                               |
|       The window proc for the app's main (tiled) window.  This processes all |
|       of the parent window's messages.                                       |
|                                                                              |
|   Arguments:                                                                 |
|       hwnd            window handle for the parent window                    |
|       msg             message number                                         |
|       wParam          message-dependent                                      |
|       lParam          message-dependent                                      |
|                                                                              |
|   Returns:                                                                   |
|       0 if processed, nonzero if ignored                                     |
|                                                                              |
\*----------------------------------------------------------------------------*/
LRESULT CALLBACK AppWndProc(
HWND hwnd,
UINT msg,
WPARAM wParam,
LPARAM lParam)
{

    switch (msg) {
       case WM_CREATE:
                ghwndMCI = MCIWndCreate(hwnd, ghInstApp, WS_CHILD | WS_VISIBLE |
                        MCIWNDF_NOTIFYMEDIA | MCIWNDF_NOTIFYSIZE | MCIWNDF_NOMENU, 0);
                break;

        case WM_COMMAND:
                HANDLE_WM_COMMAND(hwnd, wParam, lParam, MCI_OnCommand);
                break;

        case WM_PALETTECHANGED:
        case WM_QUERYNEWPALETTE:
        case WM_ACTIVATE:
                // Forward palette-related messages through to the MCIWnd,
                // so it can do the right thing.
                if (ghwndMCI)
                        return SendMessage(ghwndMCI, msg, wParam, lParam);
                break;

        case WM_SIZE:
                HANDLE_WM_SIZE(hwnd, wParam, lParam, MCI_OnSize);
                break;

        case WM_DESTROY:
                PostQuitMessage(0);
                return (0);
                break;

       case WM_PARENTNOTIFY:
                HANDLE_WM_PARENTNOTIFY(hwnd, wParam, lParam, MCI_OnParentNotify);
                break;

       case MCIWNDM_NOTIFYMEDIA:
                Handle_NotifyMedia(hwnd, wParam);
                break;

       case MCIWNDM_NOTIFYSIZE:
                Handle_NotifySize(hwnd, wParam);
                break;
        }

        return DefWindowProc(hwnd,msg,wParam,lParam);
}

/*--------------------------------------------------------------+
| Handle_NotifyMedia                                            |
|                                                               |
+--------------------------------------------------------------*/
static void Handle_NotifyMedia(
HWND hwnd,
WPARAM wParam)
{
        MCI_DGV_SETVIDEO_PARMS dgv;
        DWORD dw;
        UINT uDevice;

        if (ghwndMCI == 0)
                ghwndMCI = (HWND) wParam;

        if (!gdwHook)
                gdwHook = (DWORD)ICAVIDrawProc;

        dgv.dwValue = gdwHook;

        dgv.dwItem = MCI_AVI_SETVIDEO_DRAW_PROCEDURE;

        uDevice = MCIWndGetDeviceID(ghwndMCI);
        if (uDevice) {
                dw = mciSendCommand(uDevice, MCI_SETVIDEO,
                               MCI_DGV_SETVIDEO_ITEM | MCI_DGV_SETVIDEO_VALUE,
                               (DWORD)(LP_MCI_DGV)&dgv);

                if (dw != 0) {
                        char szTitle[BUFSIZE];

                        LoadString( ghInstApp, IDS_APPERR, szTitle, BUFSIZE );
                        LoadString( ghInstApp, IDS_MCIERROR, gszBuffer, BUFSIZE );

                        MessageBox(hwnd, gszBuffer, szTitle, MB_OK | MB_ICONHAND);
                }
        }
        return;
}


/*--------------------------------------------------------------+
| Handle_NotifySize                                             |
|                                                               |
+--------------------------------------------------------------*/
static void Handle_NotifySize(
HWND hwnd,
WPARAM wParam)
{
        RECT rc;
        if (ghwndMCI == 0)
                ghwndMCI = (HWND) wParam;

        GetWindowRect(ghwndMCI, &rc);
        AdjustWindowRect(&rc, GetWindowLong(hwnd, GWL_STYLE), TRUE);
        SetWindowPos(hwnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
                SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
        return;
}
/*--------------------------------------------------------------+
| FormatFilterString                                            |
|                                                               |
+--------------------------------------------------------------*/
static BOOL FormatFilterString(
UINT uID,
LPSTR lpszString )
{
        int nLength, nCtr = 0;
        char chWildCard;

        *lpszString = 0;

        nLength = LoadString( ghInstApp, uID, lpszString, BUFSIZE );

        chWildCard = lpszString[nLength-1];

        while( lpszString[nCtr] ) {
                if( lpszString[nCtr] == chWildCard )
                        lpszString[nCtr] = 0;
                nCtr++;
        }

        return TRUE;
}
