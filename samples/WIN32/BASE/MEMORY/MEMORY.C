
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/****************************** Module Header ******************************\
* Module Name: memory.c
\***************************************************************************/

#include <stdlib.h>

#include "memory.h"
#include <stdarg.h>

extern CreateMapFile();
extern CreateMap();
extern MapView();
extern OpenMap();


HANDLE ghModule;
HWND   ghwndMain = NULL;

HMENU  hMenu,       hMenuWindow;
HMENU  hServerMenu, hServerMenuWindow;
HMENU  hClientMenu, hClientMenuWindow;

CHAR   gszFile[20];
CHAR   gszMapName[20];


typedef struct _PerWndInfo {
//    HWND    hMainFrame;
    HWND    hParent;
    HWND    hThisWnd;           // CR!     used in locating the node
//    LPVOID  pShrMem1;         // LATER!  create more clients/servers
    RECT    rcClient;
    char    CaptionBarText[SIZEOFCAPTIONTEXT];
} PERWNDINFO, *PPERWNDINFO;


typedef struct _node {
    PERWNDINFO      ChildWnd;
    HANDLE          hNext;
} NODE, *PNODE;


/*
 * Forward declarations.
 */
BOOL InitializeApp   (void);
LONG APIENTRY MainWndProc     (HWND, UINT, DWORD, LONG);
LONG APIENTRY ServerWndProc   (HWND, UINT, DWORD, LONG);
LONG APIENTRY ClientWndProc   (HWND, UINT, DWORD, LONG);
BOOL CALLBACK About          (HWND, UINT, DWORD, LONG);
BOOL CALLBACK FileType       (HWND, UINT, DWORD, LONG);
BOOL CALLBACK MapFileName     (HWND, UINT, DWORD, LONG);
LONG APIENTRY TextWndProc     (HWND, UINT, DWORD, LONG);

/***************************************************************************\
* main
*
*
* History:
* 04-17-91 ????      Created.
\***************************************************************************/

int WINAPI WinMain(
    HANDLE hInstance,
    HANDLE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    MSG msg;

    // this will change to something more reasonable

    ghModule = GetModuleHandle(NULL);
    if (!InitializeApp()) {
        MessageBox(ghwndMain, "memory: InitializeApp failure!", "Error", MB_OK);
        return 0;
    }

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (hMenuWindow && IsWindow(hMenuWindow))
        DestroyMenu(hMenuWindow);
    if (hClientMenuWindow && IsWindow(hClientMenuWindow))
        DestroyMenu(hClientMenuWindow);
    if (hServerMenuWindow && IsWindow(hServerMenuWindow))
        DestroyMenu(hServerMenuWindow);
    if (hMenu && IsWindow(hMenu))
        DestroyMenu(hMenu);
    if (hClientMenu && IsWindow(hClientMenu))
        DestroyMenu(hClientMenu);
    if (hServerMenu && IsWindow(hServerMenu))
        DestroyMenu(hServerMenu);

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
* 09-09-91 Petrus Wong  Created.
\***************************************************************************/

BOOL InitializeApp(void)
{
    WNDCLASS wc;

    wc.style            = CS_OWNDC;
    wc.lpfnWndProc      = (WNDPROC)MainWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = sizeof(LONG);
    wc.hInstance        = ghModule;
    wc.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)(COLOR_APPWORKSPACE);
    wc.lpszMenuName     = "MainMenu";
    wc.lpszClassName    = "MemoryClass";

    if (!RegisterClass(&wc))
        return FALSE;

    wc.style            = CS_OWNDC;
    wc.lpfnWndProc      = (WNDPROC)ServerWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;                      // LATER sizeof(LONG);
    wc.hInstance        = ghModule;
    wc.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)(COLOR_APPWORKSPACE);
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = "ServerClass";

    if (!RegisterClass(&wc))
        return FALSE;

    wc.style            = CS_OWNDC;
    wc.lpfnWndProc      = (WNDPROC)ClientWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;                      // LATER sizeof(LONG);
    wc.hInstance        = ghModule;
    wc.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)(COLOR_APPWORKSPACE);
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = "ClientClass";

    if (!RegisterClass(&wc))
        return FALSE;

    wc.style            = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = (WNDPROC)TextWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = ghModule;
    wc.hIcon            = NULL;
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)(COLOR_BTNSHADOW);
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = "Text";

    if (!RegisterClass(&wc))
            return FALSE;



    hMenu       = LoadMenu(ghModule, "MainMenu");
    hServerMenu = LoadMenu(ghModule, "ServerMenu");
    hClientMenu = LoadMenu(ghModule, "ClientMenu");
    hMenuWindow       = GetSubMenu(hMenu, 1);
    hServerMenuWindow = GetSubMenu(hServerMenu, 2);
    hClientMenuWindow = GetSubMenu(hClientMenu, 2);

    ghwndMain = CreateWindowEx(0L, "MemoryClass", "Memory",
            WS_OVERLAPPED   | WS_CAPTION     | WS_BORDER       |
            WS_THICKFRAME   | WS_MAXIMIZEBOX | WS_MINIMIZEBOX  |
            WS_CLIPCHILDREN | WS_VISIBLE     | WS_SYSMENU,
            80, 70, 400, 300,
            NULL, hMenu, ghModule, NULL);

    if (ghwndMain == NULL)
        return FALSE;

    SetWindowLong(ghwndMain, GWL_USERDATA, 0L);

    //SetFocus(ghwndMain);    /* set initial focus */

    return TRUE;
}


/***************************************************************************\
* MainWndProc
*
* History:
* 09-09-91 Petrus Wong  Created.
\***************************************************************************/

long APIENTRY MainWndProc(
    HWND hwnd,
    UINT message,
    DWORD wParam,
    LONG lParam)
{
    static int         iSvrCount=1;
    static int         iCltCount=1;
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
            case IDM_TILE:
                SendMessage(ghwndClient, WM_MDITILE, 0L, 0L);
                return 0L;
            case IDM_CASCADE:
                SendMessage(ghwndClient, WM_MDICASCADE, 0L, 0L);
                return 0L;
            case IDM_ARRANGE:
                SendMessage(ghwndClient, WM_MDIICONARRANGE, 0L, 0L);
                return 0L;

            case MM_SERVER: {
                HANDLE hNode, hHead;
                PNODE  pNode;
                MDICREATESTRUCT mdicreate;

                hNode = LocalAlloc(LHND, (WORD) sizeof(NODE));
                if (hNode) {
                    if ((pNode = (PNODE)LocalLock(hNode)) == NULL)
                        MessageBox(ghwndMain, "Failed in LocalLock, hNode", "Error", MB_OK);

                    wsprintf((LPSTR) &(pNode->ChildWnd.CaptionBarText),
                             "Server %d", iSvrCount);

                    mdicreate.szClass = "ServerClass";
                    mdicreate.szTitle = (LPTSTR)&(pNode->ChildWnd.CaptionBarText);
                    mdicreate.hOwner  = ghModule;
                    mdicreate.x       =
                    mdicreate.y       =
                    mdicreate.cx      =
                    mdicreate.cy      = CW_USEDEFAULT;
                    mdicreate.style   = 0l;
                    mdicreate.lParam  = 0L;

                    /*Create Child Window*/
                    hwndChildWindow =
                        (HANDLE) SendMessage(ghwndClient, WM_MDICREATE,
                                    0L,
                                    (LONG)(LPMDICREATESTRUCT)&mdicreate);

                    if (hwndChildWindow == NULL) {
                        MessageBox(ghwndMain, "Failed in Creating Child Window", "Error", MB_OK);
                        return 0L;
                    }

                    pNode->ChildWnd.hParent      = ghwndClient;
                    pNode->ChildWnd.hThisWnd     = hwndChildWindow;
                    hHead = (HANDLE)GetWindowLong(hwnd, 0);
                    pNode->hNext = hHead;
                    SetWindowLong(hwnd, 0, (LONG) hNode);

                    iSvrCount++;


                    LocalUnlock(hNode);
                } else {
                            MessageBox(ghwndMain, "Failed to Allocate Node!", "Error", MB_OK);
                }
                return 0L;
            }

            case MM_CLIENT: {
                HANDLE hNode, hHead;
                PNODE  pNode;
                MDICREATESTRUCT mdicreate;

                hNode = LocalAlloc(LHND, (WORD) sizeof(NODE));
                if (hNode) {
                    if ((pNode = (PNODE)LocalLock(hNode))==NULL)
                        MessageBox(ghwndMain, "Failed in LocalLock, hNode!", "Error", MB_OK);

                    wsprintf((LPSTR) &(pNode->ChildWnd.CaptionBarText),
                             "Client %d", iCltCount);

                    mdicreate.szClass = "ClientClass";
                    mdicreate.szTitle = (LPSTR) &(pNode->ChildWnd.CaptionBarText);
                    mdicreate.hOwner  = ghModule;
                    mdicreate.x       =
                    mdicreate.y       =
                    mdicreate.cx      =
                    mdicreate.cy      = CW_USEDEFAULT;
                    mdicreate.style   = 0l;
                    mdicreate.lParam  = 0L;

                    /*Create Child Window*/
                    hwndChildWindow =
                        (HANDLE) SendMessage(ghwndClient, WM_MDICREATE,
                                    0L,
                                    (LONG)(LPMDICREATESTRUCT)&mdicreate);

                    if (hwndChildWindow == NULL) {
                        MessageBox(ghwndMain, "Failed in Creating Child Window!", "Error", MB_OK);
                        return 0L;
                    }

                    pNode->ChildWnd.hParent      = ghwndClient;
                    pNode->ChildWnd.hThisWnd     = hwndChildWindow;
                    hHead = (HANDLE)GetWindowLong(hwnd, 0);
                    pNode->hNext = hHead;
                    SetWindowLong(hwnd, 0, (LONG) hNode);

                    iCltCount++;

                    LocalUnlock(hNode);

                } else {
                            MessageBox(ghwndMain, "Failed to Allocate Node!", "Error", MB_OK);
                }
                return 0L;
            }

            case MM_ABOUT:
                if (DialogBox(ghModule, "AboutBox", ghwndMain, (DLGPROC)About) == -1)
                    MessageBox(ghwndMain, "DEMO: About Dialog Creation Error!", "Error", MB_OK);
                return 0L;

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

/********************************************************************\
* ServerWndProc
*
* History:
* 04-17-91 ????      Created.
* 09-09-91 Petrus Wong  Rewrote.
\***************************************************************************/

long APIENTRY ServerWndProc(
    HWND hwnd,
    UINT message,
    DWORD wParam,
    LONG lParam)
{
    static HANDLE MapFileHandle = NULL;
    static HANDLE hMem1         = NULL;
    static LPVOID pShrMem1      = NULL;
    static HANDLE hEdit;
    static BOOL   bDirty        = FALSE;
    static HWND   hTextWnd;

    switch (message) {
        case WM_COMMAND: {

          switch (LOWORD(wParam)) {
            case MM_OPT_1: { //Create File

                    SetWindowText(hTextWnd, "Creating Map File");
                    switch (DialogBox(ghModule, "FileType", hwnd, (DLGPROC)FileType)) {
                        case -1:
                            MessageBox(ghwndMain, "Server: File dialog box creation failed!", "Error", MB_OK);
                            SetWindowText(hTextWnd, "File dialog box creation failed");
                            break;
                        case IDBTN_MAP:
                            if ((MapFileHandle = (HANDLE) CreateMapFile(gszFile)) == (HANDLE)NULL) {
                               MessageBox(ghwndMain, "Server: CreateMapFile() failed!", "Error", MB_OK);
                               SetWindowText(hTextWnd, "Map File creation failed");
                            }
                            else {
                               SetWindowText(hTextWnd, "Select 'Create File Mapping...'");
                            }
                            break;
                        default:
                            MapFileHandle = (HANDLE) 0xFFFFFFFF;
                            EnableMenuItem(hServerMenu, MM_OPT_2, MF_ENABLED);
                            break;
                    }
                    return 0L;
                }
            case MM_OPT_2: { //Create File Mapping

                    SetWindowText(hTextWnd, "Creating File Mapping");
                    if (MapFileHandle != NULL ) {
                        switch (DialogBox(ghModule, "MapName", hwnd, (DLGPROC)MapFileName)) {
                            case -1:
                                MessageBox(ghwndMain, "Server: Map dialog box creation failed!", "Error", MB_OK);
                                SetWindowText(hTextWnd, "Map dialog box creation failed");
                                break;
                            case IDBTN_OK:
                                if ((hMem1 = (HANDLE) CreateMap((HANDLE *) &MapFileHandle, gszMapName) ) == (HANDLE)NULL) {
                                    MessageBox(ghwndMain, "Server: CreateMap() failed!", "Error", MB_OK);
                                    SetWindowText(hTextWnd, "File Mapping creation failed");
                                }
                                else {
                                    EnableMenuItem(hServerMenu, MM_OPT_3, MF_ENABLED);
                                    SetWindowText(hTextWnd, "Select 'Map View of File'");
                                }
                                break;
                            default:
                                break;
                        }
                    } else {
                            MessageBox(ghwndMain, "Server: MapFileHandle is NULL!", "Error", MB_OK);
                            SetWindowText(hTextWnd, "Advice: Create server file first");
                           }
                return 0L;
                }
            case MM_OPT_3: { //Map View of File
                    SetWindowText(hTextWnd, "Mapping view of File");
                    if (hMem1) {
                        if ((pShrMem1 = (LPVOID)MapView(&hMem1)) == (LPVOID)NULL) {
                            MessageBox(ghwndMain, "Server: MapView() failed!", "Error", MB_OK);
                            SetWindowText(hTextWnd, "Map view of File failed");
                        }
                        else {
                            EnableMenuItem(hServerMenu, MM_OPT_4, MF_ENABLED);
                            SetWindowText(hTextWnd, "Select 'Access' to enter text");
                        }
                    } else {
                            MessageBox(ghwndMain, "Server: Mapping view of File Failed!", "Error", MB_OK);
                            SetWindowText(hTextWnd, "Advice: Create File Mapping First");
                           }
                return 0L;
                }
            case MM_OPT_4: { //Access
                    RECT    rcl;

                    SetWindowText(hTextWnd, "Accessing Server for writing");

                    if (pShrMem1) {
                        GetClientRect(hwnd, &rcl);
                        hEdit = CreateWindow("edit", (LPSTR) NULL,
                                      WS_CHILD      | WS_VISIBLE     | WS_HSCROLL |
                                      WS_VSCROLL    | WS_BORDER      | ES_LEFT |
                                      ES_MULTILINE  | ES_AUTOHSCROLL |
                                      ES_AUTOVSCROLL,
                                      0,0, rcl.right-rcl.left,
                                      rcl.bottom-rcl.top-GetWindowLong(hTextWnd, GWL_USERDATA),
                                      hwnd, (HMENU)1, ghModule, (LPVOID)NULL);
                        if (hEdit) {
                            SetFocus(hEdit);
                        } else {
                            MessageBox(ghwndMain, "Server: Create MLE failed!", "Error", MB_OK);
                            SetWindowText(hTextWnd, "Failed to create MLE");
                        }
                    } else {
                            MessageBox(ghwndMain, "Server: Accessing for writing Failed!", "Error", MB_OK);
                            SetWindowText(hTextWnd, "Advice: Map view of File First");
                           }

                return 0L;
                }

          }

          switch (HIWORD(wParam)) {
            case EN_UPDATE: {
                if ((hEdit != NULL) && (hEdit == (HANDLE)lParam) ) {
                    bDirty = TRUE;
                }
                return 0L;
                }
          }

        }
        case WM_TIMER:
            if (bDirty && hEdit) {

//
// EM_GETHANDLE doesn't work for Win32s
//
#if 0
                HANDLE hBuf;
                BYTE * pBuf;

                hBuf = (HANDLE) SendMessage(hEdit, EM_GETHANDLE, 0L, 0L);
                if (hBuf) {
                    if ((pBuf = (BYTE *)LocalLock(hBuf)) == NULL) {
                        MessageBox(ghwndMain, "Can't get at Edit Control's data!", "Error", MB_OK);
                    } else {
                        strncpy(pShrMem1, pBuf, strlen(pBuf)+1);
                        bDirty = FALSE;
                    }
                    LocalUnlock(hBuf);
                } else {
                    MessageBox(ghwndMain, "Failed in SendMessage EM_GETHANDLE!", "Error", MB_OK);
                }
#endif

// so use WM_GETTEXT instead...

                int     iCnt;
                iCnt = SendMessage(hEdit, WM_GETTEXT, (WPARAM)4000, (LPARAM)pShrMem1);

                if (iCnt) {
                    bDirty = FALSE;
                }

            }
            return 0L;

        case WM_SETFOCUS:
            //if (hEdit)
                //SetFocus(hEdit);
            return DefMDIChildProc(hwnd, message, wParam, lParam);

        case WM_MDIACTIVATE:
            if ((HWND) lParam == hwnd) {
                SendMessage(GetParent(hwnd), WM_MDISETMENU,
                            (DWORD)  hServerMenu,
                            (LONG)   hServerMenuWindow) ;
                DrawMenuBar(GetParent(GetParent(hwnd))) ;
                //SetFocus(hEdit);
            }
            return 0;

        case WM_SIZE:
            if (hEdit)
                MoveWindow(hEdit, 0, 0,
                           LOWORD(lParam),
                           HIWORD(lParam)-GetWindowLong(hTextWnd, GWL_USERDATA),
                           TRUE);
            MoveWindow(hTextWnd,
                       0,
                       HIWORD(lParam) - GetWindowLong(hTextWnd, GWL_USERDATA),
                       LOWORD(lParam),
                       HIWORD(lParam), TRUE);

            return DefMDIChildProc(hwnd, message, wParam, lParam);

        case WM_CREATE: {
            PPERWNDINFO      pWndInfo;
            PNODE            pHead;
            HANDLE           hHead, hTmp;
            RECT             rect;

            GetClientRect(hwnd, &rect);
            hTextWnd = CreateWindow("Text", NULL,
                                    WS_BORDER | SS_LEFT | WS_CHILD | WS_VISIBLE,
                                    0, 0, 0, 0,
                                    hwnd,
                                    (HMENU) 2,
                                    ghModule,
                                    NULL);

            SetWindowText(hTextWnd, "Select 'Create File...'");

            // now find match
            hHead = (HANDLE) GetWindowLong(ghwndMain, 0);
            if (hHead) {
                if ((pHead = (PNODE)LocalLock(hHead))==NULL)
                    MessageBox(ghwndMain, "Failed in LocalLock!", "Error", MB_OK);

                while ((pHead->ChildWnd.hThisWnd != hwnd) &&
                      (pHead->hNext != NULL)) {
                   hTmp = hHead;
                   hHead = pHead->hNext;
                   LocalUnlock(hTmp);
                   if ((pHead = (PNODE) LocalLock(hHead))==NULL)
                        MessageBox(ghwndMain, "Failed in LocalLock!", "Error", MB_OK);
                }
                if (pHead->ChildWnd.hThisWnd == hwnd) {
                    pWndInfo = &pHead->ChildWnd;
                    goto Wnd_Found;
                } else {
                    //MessageBox(ghwndMain, "Trouble - Can't find the node!", "Error", MB_OK);
                    goto Wnd_Out;
                }

Wnd_Found:
                if (!GetClientRect(pWndInfo->hThisWnd,
                                   &pWndInfo->rcClient))
                    MessageBox(ghwndMain, "Failed in GetClientRect!", "Error", MB_OK);
Wnd_Out:
                LocalUnlock(hHead);
                return DefMDIChildProc(hwnd, message, wParam, lParam);
            } else {
                        //MessageBox(ghwndMain, "Can't GetWindowLong(ghwndMain,0)!", "Error", MB_OK);
            }           return DefMDIChildProc(hwnd, message, wParam, lParam);

        }

        case WM_CLOSE: {
            PPERWNDINFO      pWndInfo;
            PNODE            pHead, pTrail;
            HANDLE           hHead, hTmp;

            SendMessage(GetParent(hwnd), WM_MDISETMENU,
                            (DWORD) hMenu,
                            (LONG)   hMenuWindow) ;
            DrawMenuBar(GetParent(GetParent(hwnd))) ;


            // now find match
            hHead = (HANDLE) GetWindowLong(ghwndMain, 0);
            if (hHead) {
                if ((pHead = (PNODE)LocalLock(hHead))==NULL)
                    MessageBox(ghwndMain, "Failed in LocalLock!", "Error", MB_OK);

                pTrail = pHead;
                while ((pHead->ChildWnd.hThisWnd != hwnd) &&
                      (pHead->hNext != NULL)) {
                   hTmp = hHead;
                   pTrail = pHead;
                   hHead = pHead->hNext;
                   LocalUnlock(hTmp);
                   if ((pHead = (PNODE) LocalLock(hHead))==NULL)
                        MessageBox(ghwndMain, "Failed in LocalLock!", "Error", MB_OK);
                }
                if (pHead->ChildWnd.hThisWnd == hwnd) {
                    pWndInfo = &pHead->ChildWnd;
                    goto Wnd_Found1;
                } else {
                    //MessageBox(ghwndMain, "Trouble - Can't find the thread node!", "Error", MB_OK);
                    goto Wnd_Out1;
                }

Wnd_Found1:     if (pTrail == pHead)
                    SetWindowLong(ghwndMain, 0, (LONG) pHead->hNext);
                else
                    pTrail->hNext = pHead->hNext;

                //if (!LocalUnlock(hHead)) {
                    LocalFree(hHead);
                    return DefMDIChildProc(hwnd, message, wParam, lParam);
                //}
Wnd_Out1:
                LocalUnlock(hHead);
            } else {
                        //MessageBox(ghwndMain, "Can't GetWindowLong(ghwndMain,0) !", "Error", MB_OK);
            }
            return DefMDIChildProc(hwnd, message, wParam, lParam);

        }

        case WM_DESTROY:
            KillTimer(hwnd, 1);
            return 0L;

        case WM_PAINT:
            return DefMDIChildProc(hwnd, message, wParam, lParam);

        default:
                return DefMDIChildProc(hwnd, message, wParam, lParam);
    }

}

/********************************************************************\
* ClientWndProc
*
* History:
* 04-17-91 ????      Created.
* 09-09-91 Petrus Wong  Rewrote.
\***************************************************************************/

long APIENTRY ClientWndProc(
    HWND hwnd,
    UINT message,
    DWORD wParam,
    LONG lParam)
{
    static HANDLE hMem1    = NULL;
    static LPVOID pShrMem1 = NULL;
    static HANDLE hEdit;
    static HANDLE hTextWnd;

    switch (message) {
        case WM_COMMAND: {

          switch (LOWORD(wParam)) {
            case MM_OPT_5: { //Open File Mapping
                    SetWindowText(hTextWnd, "Opening Mapping File");
                    switch (DialogBox(ghModule, "MapName", hwnd, (DLGPROC)MapFileName)) {
                        case -1:
                            MessageBox(ghwndMain, "Client: Map dialog box creation failed!", "Error", MB_OK);
                            break;
                        case IDBTN_OK:
                            if ((hMem1 = (HANDLE) OpenMap(gszMapName)) == (HANDLE)NULL) {
                                MessageBox(ghwndMain, "Client: Open File Mapping failed!", "Error", MB_OK);
                                SetWindowText(hTextWnd, "Advice: Make sure Map file is created on Server");
                            } else {
                                EnableMenuItem(hClientMenu, MM_OPT_6, MF_ENABLED);
                                SetWindowText(hTextWnd, "Select 'Map View of File'");
                            }
                            break;
                        default:
                            break;
                    }
                return 0L;
                }
            case MM_OPT_6: { //Map View of File
                    SetWindowText(hTextWnd, "Mapping view of File");
                    if (hMem1) {
                        if ((pShrMem1 = (LPVOID) MapView(&hMem1)) == (LPVOID)NULL)
                            MessageBox(ghwndMain, "Client: MapView() failed!", "Error", MB_OK);
                        else {
                            EnableMenuItem(hClientMenu, MM_OPT_7, MF_ENABLED);
                            SetWindowText(hTextWnd, "Select 'Access' for reading Server ");
                        }
                    } else {
                            MessageBox(ghwndMain, "Client: Mapping view of File Failed!", "Error", MB_OK);
                            SetWindowText(hTextWnd, "Advice: Open File Mapping first");
                           }
                return 0L;
                }
            case MM_OPT_7: { //Access
                    RECT    rcl;

                    SetWindowText(hTextWnd, "Accessing Server for reading");
                    if (pShrMem1) {
                        GetClientRect(hwnd, &rcl);

                        hEdit = CreateWindow("edit", NULL,
                                      WS_CHILD      | WS_VISIBLE     | WS_HSCROLL |
                                      WS_VSCROLL    | WS_BORDER      | ES_LEFT |
                                      ES_MULTILINE  | ES_AUTOHSCROLL | ES_READONLY |
                                      ES_AUTOVSCROLL,
                                      0,0, rcl.right-rcl.left,
                                      rcl.bottom-rcl.top-GetWindowLong(hTextWnd, GWL_USERDATA),
                                      hwnd, (HMENU) 1, ghModule, NULL);
                        if (hEdit) {
                            //SetFocus(hEdit);
                            SendMessage(hEdit, WM_SETTEXT, 0L, (LONG)pShrMem1);
                            SetTimer(hwnd, 2, 10000, NULL);
                            EnableMenuItem(hClientMenu, MM_OPT_8, MF_ENABLED);
                        } else {
                            MessageBox(ghwndMain, "Client: Create MLE failed!", "Error", MB_OK);
                        }
                    } else {
                            MessageBox(ghwndMain, "Client: Accessing for reading Failed!", "Error", MB_OK);
                            SetWindowText(hTextWnd, "Advice: Map View of File first");
                           }

                return 0L;
                }

             case MM_OPT_8: { // refresh now
                HANDLE hActive;

                hActive = (HANDLE) SendMessage(GetParent(hwnd), WM_MDIGETACTIVE, 0L, 0L);
                SendMessage(hEdit, WM_SETTEXT, 0L, (LONG)pShrMem1);
                //SetFocus(hActive);
                return 0L;
                }
          }
        }
        case WM_SETFOCUS:
            //if (hEdit)
                //SetFocus(hEdit);
            return DefMDIChildProc(hwnd, message, wParam, lParam);

        case WM_TIMER:  {
            HANDLE hActive;

            if (hEdit) {
                hActive = (HANDLE) SendMessage(GetParent(hwnd), WM_MDIGETACTIVE, 0L, 0L);
                SendMessage(hEdit, WM_SETTEXT, 0L, (LONG)pShrMem1);
                //SetFocus(hActive);
            }
            return 0L;
        }

        case WM_MDIACTIVATE:
            if ((HWND) lParam == hwnd) {
                SendMessage(GetParent(hwnd), WM_MDISETMENU,
                            (DWORD)  hClientMenu,
                            (LONG)   hClientMenuWindow) ;
                DrawMenuBar(GetParent(GetParent(hwnd))) ;
                //SetFocus(hEdit);
            }
            return 0;

        case WM_SIZE:
            if (hEdit)
                MoveWindow(hEdit, 0, 0,
                           LOWORD(lParam),
                           HIWORD(lParam)-GetWindowLong(hTextWnd, GWL_USERDATA),
                           TRUE);
            MoveWindow(hTextWnd,
                       0,
                       HIWORD(lParam) - GetWindowLong(hTextWnd, GWL_USERDATA),
                       LOWORD(lParam),
                       HIWORD(lParam), TRUE);
            return DefMDIChildProc(hwnd, message, wParam, lParam);

        case WM_CREATE: {
            PPERWNDINFO      pWndInfo;
            PNODE            pHead;
            HANDLE           hHead, hTmp;
            RECT             rect;

            GetClientRect(hwnd, &rect);
            hTextWnd = CreateWindow("Text", NULL,
                                    WS_BORDER | SS_LEFT | WS_CHILD | WS_VISIBLE,
                                    0, 0, 0, 0,
                                    hwnd,
                                    (HMENU) 2,
                                    ghModule,
                                    NULL);

            SetWindowText(hTextWnd, "Select 'Open File...'");

            // now find match
            hHead = (HANDLE) GetWindowLong(ghwndMain, 0);
            if (hHead) {
                if ((pHead = (PNODE)LocalLock(hHead))==NULL)
                    MessageBox(ghwndMain, "Failed in LocalLock!", "Error", MB_OK);

                while ((pHead->ChildWnd.hThisWnd != hwnd) &&
                      (pHead->hNext != NULL)) {
                   hTmp = hHead;
                   hHead = pHead->hNext;
                   LocalUnlock(hTmp);
                   if ((pHead = (PNODE) LocalLock(hHead))==NULL)
                        MessageBox(ghwndMain, "Failed in LocalLock!", "Error", MB_OK);
                }
                if (pHead->ChildWnd.hThisWnd == hwnd) {
                    pWndInfo = &pHead->ChildWnd;
                    goto Wnd_Found;
                } else {
                    //MessageBox(ghwndMain, "Trouble - Can't find the node!", "Error", MB_OK);
                    goto Wnd_Out;
                }

Wnd_Found:
                if (!GetClientRect(pWndInfo->hThisWnd,
                                   &pWndInfo->rcClient))
                    MessageBox(ghwndMain, "Failed in GetClientRect!", "Error", MB_OK);
Wnd_Out:
                LocalUnlock(hHead);
                return DefMDIChildProc(hwnd, message, wParam, lParam);
            } else {
                        //MessageBox(ghwndMain, "Can't GetWindowLong(ghwndMain,0) !", "Error", MB_OK);
            }           return DefMDIChildProc(hwnd, message, wParam, lParam);

        }

        case WM_CLOSE: {
            PPERWNDINFO      pWndInfo;
            PNODE            pHead, pTrail;
            HANDLE           hHead, hTmp;

            SendMessage(GetParent(hwnd), WM_MDISETMENU,
                            (DWORD) hMenu,
                            (LONG)   hMenuWindow) ;
            DrawMenuBar(GetParent(GetParent(hwnd))) ;


            // now find match
            hHead = (HANDLE) GetWindowLong(ghwndMain, 0);
            if (hHead) {
                if ((pHead = (PNODE)LocalLock(hHead))==NULL)
                    MessageBox(ghwndMain, "Failed in LocalLock!", "Error", MB_OK);

                pTrail = pHead;
                while ((pHead->ChildWnd.hThisWnd != hwnd) &&
                      (pHead->hNext != NULL)) {
                   hTmp = hHead;
                   pTrail = pHead;
                   hHead = pHead->hNext;
                   LocalUnlock(hTmp);
                   if ((pHead = (PNODE) LocalLock(hHead))==NULL)
                        MessageBox(ghwndMain, "Failed in LocalLock!", "Error", MB_OK);
                }
                if (pHead->ChildWnd.hThisWnd == hwnd) {
                    pWndInfo = &pHead->ChildWnd;
                    goto Wnd_Found1;
                } else {
                    //MessageBox(ghwndMain, "Trouble - Can't find the thread node!", "Error", MB_OK);
                    goto Wnd_Out1;
                }

Wnd_Found1:     if (pTrail == pHead)
                    SetWindowLong(ghwndMain, 0, (LONG) pHead->hNext);
                else
                    pTrail->hNext = pHead->hNext;

                //if (!LocalUnlock(hHead)) {
                    LocalFree(hHead);
                    return DefMDIChildProc(hwnd, message, wParam, lParam);
                //}
Wnd_Out1:
                LocalUnlock(hHead);
            } else {
                        //MessageBox(ghwndMain, "Can't GetWindowLong(ghwndMain,0)!", "Error", MB_OK);
            }
            return DefMDIChildProc(hwnd, message, wParam, lParam);

        }

        case WM_DESTROY:
            KillTimer(hwnd, 2);
            return 0l;

        default:
                return DefMDIChildProc(hwnd, message, wParam, lParam);
    }

}

/***************************************************************************\
* About
*
* About dialog proc.
*
* History:
* 04-13-91 ????      Created.
* 09-09-91 Petrus Wong  Rewrote.
\***************************************************************************/

BOOL CALLBACK APIENTRY About(
    HWND hDlg,
    UINT message,
    DWORD wParam,
    LONG lParam)
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
* MapFileName
*
* MapFileName dialog proc.
*
* History:
* 09-09-91 Petrus Wong  Created.
\***************************************************************************/

BOOL CALLBACK MapFileName(
    HWND hDlg,
    UINT message,
    DWORD wParam,
    LONG lParam)
{

    switch (message) {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        switch (wParam) {
            case IDBTN_OK: {

                if ((GetDlgItemText(hDlg, IDEDIT_MAPNAME, gszMapName, 20)) == 0) {
                        MessageBox(ghwndMain, "MapFileName: Can't get edit text!", "Error", MB_OK);
                        strncpy(gszMapName, "MapName1", 10);        // default name
                }
                EndDialog(hDlg, IDBTN_OK);
                break;
            }

        }

    }

    return FALSE;

    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(hDlg);
}

/***************************************************************************\
* FileType
*
* FileType dialog proc.
*
* History:
* 09-09-91 Petrus Wong  Created.
\***************************************************************************/

BOOL CALLBACK FileType(
    HWND hDlg,
    UINT message,
    DWORD wParam,
    LONG lParam)
{

    switch (message) {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        switch (wParam) {
            case IDBTN_PAGE: {
                EndDialog(hDlg, IDBTN_PAGE);
                break;
            }

            case IDBTN_MAP: {
                  if ((GetDlgItemText(hDlg, IDEDIT_MAPFILE, gszFile, 20)) == 0)
                      EndDialog(hDlg, IDBTN_PAGE);  // default to use PAGE file
                  else
                      EndDialog(hDlg, IDBTN_MAP);

                  break;
            }
        }

    }

    return FALSE;

    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(hDlg);
}


/*************************************************************************
*
* TextWndProc
*
* Text Window proc.
*
* History:
* 10-07-91  Petrus Wong Created.
*
\***************************************************************************/

LONG APIENTRY TextWndProc (HWND hwnd, UINT message, DWORD wParam, LONG lParam)
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

            SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lf), &lf, FALSE);

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
