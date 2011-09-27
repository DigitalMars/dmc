
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/******************************Module*Header*******************************\
* Module Name: mdi.c
*
* MDI demonstration
*   +   Associating data with a MDI window (using Set/GetWindowLong )
*
* Dependencies:
*
*   (#defines)
*   (#includes)
*       MDI.H
*
\**************************************************************************/
#define STRICT
#include <stdlib.h>
#include "mdi.h"
#include <stdarg.h>

HANDLE ghModule;
HWND   ghwndMain   = NULL;
HWND   ghwndClient = NULL;

HMENU  hMenu,      hMenuWindow;
HMENU  hChildMenu, hChildMenuWindow;

CHAR   gszFile[20];
CHAR   gszMapName[20];

//
// Instance data for each MDI child window
//
typedef struct _PerWndInfo {
    HWND    hParent;
    HWND    hTextWnd;
    RECT    rcClient;
    char    CaptionBarText[SIZEOFCAPTIONTEXT];
} INFO, *PINFO;

//
// Forward declarations.
//
BOOL InitializeApp   (void);
LRESULT CALLBACK MainWndProc     (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MDIWndProc      (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK About           (HWND, UINT, WPARAM, LPARAM);
LONG APIENTRY TextWndProc     (HWND, UINT, DWORD, LONG);
VOID vTest           (PINFO);

/***************************************************************************\
* WinMain
*
*
* History:
* 11-Feb-1992   Petrus Wong []
*   Added conditional compile statement for internal build environment.
* 04-17-91      ????      Created.
\***************************************************************************/
int PASCAL WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    MSG    msg;
    HANDLE hAccel;

    ghModule = GetModuleHandle(NULL);
    if (!InitializeApp()) {
        MessageBox(ghwndMain, "MDI: InitializeApp failure!", "Error", MB_OK);
        return 0;
    }

    if (!(hAccel = LoadAccelerators (ghModule, MAKEINTRESOURCE(ACCEL_ID))))
        MessageBox(ghwndMain, "MDI: Load Accel failure!", "Error", MB_OK);


    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator( ghwndMain, hAccel, &msg) &&
            !TranslateMDISysAccel(  ghwndClient, &msg)          ) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 1;

    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
}


/***************************************************************************\
* InitializeApp
*
* History:
* 11-Feb-1992   Petrus Wong []
*   Name changes.
* 09-09-91      Created.
\***************************************************************************/

BOOL InitializeApp(void)
{
    WNDCLASS wc;

    wc.style            = CS_OWNDC;
    wc.lpfnWndProc      = (WNDPROC) MainWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = sizeof(LONG);
    wc.hInstance        = ghModule;
    wc.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)(COLOR_APPWORKSPACE);
    wc.lpszMenuName     = "MainMenu";
    wc.lpszClassName    = "MDIDemoClass";

    if (!RegisterClass(&wc))
        return FALSE;

    wc.lpfnWndProc      = (WNDPROC) MDIWndProc;
    wc.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = "MDIClass";

    if (!RegisterClass(&wc))
        return FALSE;

    wc.style            = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = (WNDPROC) TextWndProc;
    wc.hIcon            = NULL;
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)(COLOR_BTNSHADOW);
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = "Text";

    if (!RegisterClass(&wc))
            return FALSE;



    hMenu       = LoadMenu(ghModule, "MainMenu");
    hChildMenu  = LoadMenu(ghModule, "ChildMenu");
    hMenuWindow      = GetSubMenu(hMenu, 1);
    hChildMenuWindow = GetSubMenu(hChildMenu, 2);

    ghwndMain = CreateWindowEx(0L, "MDIDemoClass", "MDI Demonstration",
            WS_OVERLAPPED   | WS_CAPTION     | WS_BORDER       |
            WS_THICKFRAME   | WS_MAXIMIZEBOX | WS_MINIMIZEBOX  |
            WS_CLIPCHILDREN | WS_VISIBLE     | WS_SYSMENU,
            80, 70, 400, 300,
            NULL, hMenu, ghModule, NULL);

    if (ghwndMain == NULL)
        return FALSE;

    SetWindowLong(ghwndMain, GWL_USERDATA, 0L);

    SetFocus(ghwndMain);    /* set initial focus */

    return TRUE;
}


/***************************************************************************\
* MainWndProc
*
* History:
* 11-Feb-1992   Petrus Wong []
*   Name changes.  Added comments.
* 09-09-91      Created.
\***************************************************************************/

LRESULT CALLBACK MainWndProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    static int         iMDICount=1;
    CLIENTCREATESTRUCT clientcreate;
    HWND               hwndChildWindow;


    switch (message) {

      case WM_CREATE:
        SetWindowLong(hwnd, 0, (LONG)NULL);

        clientcreate.hWindowMenu  = hMenuWindow;
        clientcreate.idFirstChild = 1;

        ghwndClient = CreateWindow("MDICLIENT", NULL,
                                    WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
                                    0,0,0,0,
                                    hwnd, NULL, ghModule, (LPVOID)&clientcreate);
        return 0L;

      case WM_DESTROY: {
        PostQuitMessage(0);
        return 0L;
      }

      case WM_COMMAND:

        switch (LOWORD(wParam)) {
            //
            // Getting default MDI functionalities...
            //
            case IDM_TILE:
                SendMessage(ghwndClient, WM_MDITILE, 0L, 0L);
                return 0L;
            case IDM_CASCADE:
                SendMessage(ghwndClient, WM_MDICASCADE, 0L, 0L);
                return 0L;
            case IDM_ARRANGE:
                SendMessage(ghwndClient, WM_MDIICONARRANGE, 0L, 0L);
                return 0L;

            //
            // Creates MDI child
            //
            case MM_MDI: {
                HANDLE hInfo;
                PINFO  pInfo;
                MDICREATESTRUCT mdicreate;

                //
                // Allocating memory for INFO to be associated with the
                //  new child
                //
                hInfo = LocalAlloc(LHND, (WORD) sizeof(INFO));
                if (hInfo) {
                    if ((pInfo = (PINFO)LocalLock(hInfo)) == NULL)
                        MessageBox(ghwndMain, "Failed in LocalLock", "Error", MB_OK);

                    wsprintf((LPSTR) &(pInfo->CaptionBarText),
                             "MDI %d", iMDICount);
                    pInfo->hParent    = ghwndClient;

                    mdicreate.szClass = "MDIClass";
                    mdicreate.szTitle = (LPTSTR)&(pInfo->CaptionBarText);
                    mdicreate.hOwner  = ghModule;
                    mdicreate.x       =
                    mdicreate.y       =
                    mdicreate.cx      =
                    mdicreate.cy      = CW_USEDEFAULT;
                    mdicreate.style   = 0L;
                    //
                    // passing the handle of the per MDI child INFO to the
                    //  child MDI window for storage
                    //
                    mdicreate.lParam  = (LONG) hInfo;

                    /*Create Child Window*/
                    hwndChildWindow =
                        (HANDLE) SendMessage(ghwndClient, WM_MDICREATE,
                                    0L,
                                    (LONG)(LPMDICREATESTRUCT)&mdicreate);

                    if (hwndChildWindow == NULL) {
                        MessageBox(ghwndMain, "Failed in Creating Child Window", "Error", MB_OK);
                        return 0L;
                    }

                    iMDICount++;

                    LocalUnlock(hInfo);
                } else {
                            MessageBox(ghwndMain, "Failed to Allocate INFO data!", "Error", MB_OK);
                }
                return 0L;
            }


            case MM_ABOUT:
                if (DialogBox(ghModule, "AboutBox", ghwndMain, (DLGPROC)About) == -1)
                        MessageBox(ghwndMain, "DEMO: About Dialog Creation Error!", "Error", MB_OK);
                return 0L;

            //
            // Passes these WM_COMMAND messages to the appropriate active child
            //  window proc for processing
            //
            case MM_OPT_1:
            case MM_OPT_2:
            case MM_OPT_3:
            case MM_OPT_4:
            case MM_OPT_5:
            case MM_OPT_6:
            case MM_OPT_7:
            case MM_OPT_8: {
                HWND hActiveChild;

                hActiveChild = (HANDLE) SendMessage(ghwndClient, WM_MDIGETACTIVE, 0L, 0L);
                if (hActiveChild)
                    SendMessage(hActiveChild, WM_COMMAND, wParam, lParam);
                return 0L;
            }

            default:
                return DefFrameProc(hwnd,  ghwndClient, message, wParam, lParam);
        }

    default:

        return DefFrameProc(hwnd,  ghwndClient, message, wParam, lParam);
    }
}

/***************************************************************************\
* MDIWndProc
*
* History:
* 11-Feb-1992   Petrus Wong []
*   Name changes. Added comments.
* 09-09-91      Petrus Wong Rewrote.
* 04-17-91 ????      Created.
\***************************************************************************/

LRESULT CALLBACK MDIWndProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{

    switch (message) {
        case WM_COMMAND: {
          HANDLE      hInfo;
          PINFO       pInfo;
          HWND        hTextWnd;

          //
          // Retrieving this child window's INFO data for displaying
          //    messages in the text window
          //
          hInfo = (HANDLE) GetWindowLong(hwnd, 0);
          if (hInfo) {
              if ((pInfo = (PINFO)LocalLock(hInfo)) == NULL)
                  MessageBox(ghwndMain, "Failed in LocalLock", "Error", MB_OK);

              hTextWnd = pInfo->hTextWnd;
              vTest(pInfo);
              LocalUnlock(hInfo);
          } else {
              MessageBox(ghwndMain, "Can't get hInfo!", "Error", MB_OK);
          }


          switch (LOWORD(wParam)) {
            case MM_OPT_1: {
               SetWindowText(hTextWnd, "MM_OPT_1");
               return 0L;
            }

            case MM_OPT_2: {
               SetWindowText(hTextWnd, "MM_OPT_2");
               return 0L;
            }

            case MM_OPT_3: {
               SetWindowText(hTextWnd, "MM_OPT_3");
               return 0L;
            }

            case MM_OPT_4: {
               SetWindowText(hTextWnd, "MM_OPT_4");
               return 0L;
            }
            default:
               return 0L;

          }

        }
        case WM_SETFOCUS:
            break;

        //
        // Potentially, you can set different menu for different MDI
        //  child which is currently being active.
        //
        case WM_MDIACTIVATE:
            if ((HWND) lParam == hwnd) {
                SendMessage(GetParent(hwnd), WM_MDISETMENU,
                            (DWORD)  hChildMenu,
                            (LONG)   hChildMenuWindow) ;
                DrawMenuBar(GetParent(GetParent(hwnd))) ;
            }
            return 0L;

        //
        // Whenever the MDI child window is resized, its children has to be
        //  resized accordingly.
        //
        case WM_SIZE: {
            HANDLE      hInfo;
            PINFO       pInfo;
            HWND        hTextWnd;

            //
            // First, get the text window's handle from the per MDI child
            //  INFO data structure
            //
            hInfo = (HANDLE) GetWindowLong(hwnd, 0);
            if (hInfo) {
                if ((pInfo = (PINFO)LocalLock(hInfo)) == NULL)
                    MessageBox(ghwndMain, "Failed in LocalLock", "Error", MB_OK);

                hTextWnd = pInfo->hTextWnd;
                LocalUnlock(hInfo);
            } else {
                MessageBox(ghwndMain, "Can't get hInfo!", "Error", MB_OK);
            }

            //
            // Always, put the text window at the bottom of the MDI window
            //
            MoveWindow(hTextWnd,
                       0,
                       HIWORD(lParam) - GetWindowLong(hTextWnd, GWL_USERDATA),
                       LOWORD(lParam),
                       HIWORD(lParam), TRUE);
            break;
        }

        //
        // Creates the text window for this MDI child and saves its handle
        //  in the per MDI child INFO data structure.
        //
        case WM_CREATE: {
            PINFO           pInfo;
            HANDLE          hInfo;
            HWND            hTextWnd;

            hTextWnd = CreateWindow("Text", NULL,
                                    WS_BORDER | SS_LEFT | WS_CHILD | WS_VISIBLE,
                                    0, 0, 0, 0,
                                    hwnd,
                                    (HMENU) 2,
                                    ghModule,
                                    NULL);

            SetWindowText(hTextWnd, "Select 'Option' menu items");
            //
            // INFO was allocated in the MDIWndProc at MM_MDI time and is
            //  passed to us at WM_CREATE time...
            //
            hInfo = (HANDLE) ((LPMDICREATESTRUCT) ((LPCREATESTRUCT) lParam)->lpCreateParams)->lParam ;
            if (hInfo) {
                if ((pInfo = (PINFO)LocalLock(hInfo)) == NULL)
                    MessageBox(ghwndMain, "Failed in LocalLock", "Error", MB_OK);

                if (!GetClientRect(hwnd, &pInfo->rcClient))
                    MessageBox(ghwndMain, "Failed in GetClientRect!", "Error", MB_OK);

                pInfo->hTextWnd = hTextWnd;
                //
                // Save the handle to INFO in our window structure
                //
                SetWindowLong(hwnd, 0, (LONG) hInfo);
                LocalUnlock(hInfo);
            } else {
                MessageBox(ghwndMain, "Can't allocate hInfo!", "Error", MB_OK);
            }

            break;
        }

        //
        // Free the INFO data that associates with this window
        //  also, reset the menu.
        //
        case WM_CLOSE: {
            HANDLE           hInfo;

            SendMessage(GetParent(hwnd), WM_MDISETMENU,
                            (DWORD) hMenu,
                            (LONG)  hMenuWindow) ;
            DrawMenuBar(GetParent(GetParent(hwnd))) ;

            hInfo = (HANDLE) GetWindowLong(hwnd, 0);
            LocalFree(hInfo);
            break;
        }

        default:
            return DefMDIChildProc(hwnd, message, wParam, lParam);

    } //switch
    return DefMDIChildProc(hwnd, message, wParam, lParam);
}

/***************************************************************************\
* About
*
* About dialog proc.
*
* History:
* 09-09-91      Petrus Wong Rewrote.
* 04-13-91 ????      Created.
\***************************************************************************/

BOOL CALLBACK About (
    HWND hDlg,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (message) {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        if (wParam == IDOK)
            EndDialog(hDlg, wParam);
        break;
    }

    return FALSE;

    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(hDlg);
}

/***************************************************************************\
*
* TextWndProc
*
* Text Window procedure for displaying miscellaneous messages to user.
*
* History:
* 10-07-91
*   3D text output
*
\***************************************************************************/

LONG WINAPI TextWndProc (HWND hwnd, UINT message, DWORD wParam, LONG lParam)
{
    static HFONT hFont = (HFONT) NULL;

    switch (message)
    {
    case WM_CREATE:
        {
            LOGFONT    lf;
            HDC        hDC;
            HFONT      hOldFont;
            TEXTMETRIC tm;
            RECT       rect;
            LONG       lHeight;

            SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lf), (PVOID) &lf, (UINT)FALSE);

            hDC = GetDC(hwnd);
            // this is the height for 8 point size font in pixels
            lf.lfHeight = 8 * GetDeviceCaps(hDC, LOGPIXELSY) / 72;

            hFont = CreateFontIndirect(&lf);
            hOldFont = SelectObject(hDC, hFont);
            GetTextMetrics(hDC, &tm);
            GetClientRect(GetParent(hwnd), &rect);

            // base the height of the window on size of text
            lHeight = tm.tmHeight+6*GetSystemMetrics(SM_CYBORDER)+2;
            // saved the height for later reference
            SetWindowLong(hwnd, GWL_USERDATA, lHeight);
            SetWindowPos(hwnd, NULL,
                    0,
                    rect.bottom-lHeight,
                    rect.right-rect.left,
                    lHeight,
                    SWP_NOZORDER | SWP_NOMOVE);

            ReleaseDC(hwnd, hDC);
            break;
        }

    case WM_DESTROY:
            if (hFont)
                DeleteObject(hFont);
            break;

    case WM_SETTEXT:
            DefWindowProc(hwnd, message, wParam, lParam);
            InvalidateRect(hwnd,NULL,FALSE);
            UpdateWindow(hwnd);
            return 0L;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            RECT   rc;
            char   ach[128];
            int    len, nxBorder, nyBorder;
            HFONT  hOldFont = NULL;

            BeginPaint(hwnd, &ps);

            GetClientRect(hwnd,&rc);

            nxBorder = GetSystemMetrics(SM_CXBORDER);
            rc.left  += 9*nxBorder;
            rc.right -= 9*nxBorder;

            nyBorder = GetSystemMetrics(SM_CYBORDER);
            rc.top    += 3*nyBorder;
            rc.bottom -= 3*nyBorder;

            // 3D Text
            len = GetWindowText(hwnd, ach, sizeof(ach));
            SetBkColor(ps.hdc, GetSysColor(COLOR_BTNFACE));

            SetBkMode(ps.hdc, TRANSPARENT);
            SetTextColor(ps.hdc, RGB(64,96,96));
            if (hFont)
                hOldFont = SelectObject(ps.hdc, hFont);
            ExtTextOut(ps.hdc, rc.left+2*nxBorder+2, rc.top+2, ETO_OPAQUE | ETO_CLIPPED,
                        &rc, ach, len, NULL);

            SetTextColor(ps.hdc, RGB(128,128,128));
            if (hFont)
                hOldFont = SelectObject(ps.hdc, hFont);
            ExtTextOut(ps.hdc, rc.left+2*nxBorder+1, rc.top+1, ETO_CLIPPED,
                        &rc, ach, len, NULL);

            SetTextColor(ps.hdc, RGB(255,255,255));
            if (hFont)
                hOldFont = SelectObject(ps.hdc, hFont);
            ExtTextOut(ps.hdc, rc.left+2*nxBorder, rc.top, ETO_CLIPPED,
                        &rc, ach, len, NULL);

            SetBkMode(ps.hdc, OPAQUE);

            if (hOldFont)
                SelectObject(ps.hdc, hOldFont);

            EndPaint(hwnd, &ps);
            return 0L;
        }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

VOID vTest(PINFO pInfo)
{
    OutputDebugString("Inside vTest()\n");

    UNREFERENCED_PARAMETER(pInfo);
}
