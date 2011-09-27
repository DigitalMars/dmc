
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
* Module Name: demo.c
\***************************************************************************/


/*      ******MULTI-THREADED GUIDE********


There are aspects to a multi-threaded app, that need be explained,
so that a programmer understand the pitfalls of a multi-threaded app,
and avoid future problems in a multi-threaded application.

The primary pifalls involve the usage of GDI objects. (DC's, regions, etc).
GDI is the only subsytem in NT win32, that does NOT serialize access
to its objects.  This is for speed/throughput reasons.  The premise
of GDI objects, is that an object is per process owned, per thread locked.

        This means if you have multiple threads, accessing the same GDI object,
you will have MAJOR headaches, because while one thread deletes an object,
the other may be using it.  The model we put forward towards developers, is
the following model.

        Seperate threads for

                > Input
                > File IO
                > Printing
                > Graphics

* Note its ok to have graphics/Printing threads, because the use a different
  device.


Note that you can be single threaded, and have no GDI object serialization
problems. What types of problems occur, is when seperate threads attempt
to delete/use a GDI object, or share an object between multiple threads.
Since an object is per thread locked, means you will get an unexpected
error within the other thread.  Many windows programmers, do not expect
certain calls to ever fail, but a poorly constructed multi-threaded app
under win32  (given the above scenerios) may fail, and if not checked,
will cause subsequent GDI calls to fail, ultimately causing the GDI client
side server to die, taking the app with it.

Thus its most important to realize that while multi-threaded apps are cool,
they must be programmed with respect to the above guidelines.

*/

                                                                                                                                          
#include <stdlib.h>

#include "mltithrd.h"
#include <stdarg.h>

#define CCOLORS 16
#define CPOINTS 20
#define COBJS   1


/*
 * Some handy globals.
 */
HANDLE ghModule;
HWND   ghwndMain = NULL;
HBRUSH ghbrWhite, ghbrBlack;
HBRUSH gahbrColor[CCOLORS];
int    cmdDemo   = 0;
BOOL   bKillMe = FALSE;

HMENU  hMenu, hMenuWindow;

DWORD aclr[CCOLORS] = {
    0x00000000,    // black
    0x007F0000,    // red
    0x00007F00,    // green
    0x007F7F00,    // mustard/brown
    0x0000007F,    // blue
    0x007F007F,    // magenta
    0x00007F7F,    // cyan
    0x00BFBFBF,    // light gray
    0x007F7F7F,    // dark gray
    0x00FF0000,    // bright red
    0x0000FF00,    // bright green
    0x00FFFF00,    // bright yellow
    0x000000FF,    // bright blue
    0x00FF00FF,    // bright magenta
    0x0000FFFF,    // bright cyan
    0x00FFFFFF     // bright white
};


typedef struct _b {
    POINT apt[CPOINTS];
    int dx;
    int dy;
    int vx;
    int vy;
    int ax;
    int ay;
    int iColor;
} B;


typedef struct _ThreadBlockInfo {
        HANDLE  hThread;
        BOOL    bKillThrd;
        HWND    hwndClient;
        HWND    hwndThreadWindow;
        LONG    lThreadId;
        char    CaptionBarText[SIZEOFCAPTIONTEXT];
        B       gab[COBJS];
        RECT    rcClient;
        HDC     hdcThreadWindow;
} THREADBLOCKINFO, *PTHREADBLOCKINFO;


typedef struct _node {
    THREADBLOCKINFO ThreadWindow;
    HANDLE          hNext;
} NODE, *PNODE;


/*
 * Forward declarations.
 */
BOOL InitializeApp   (void);
int WINAPI MainWndProc     (HWND, UINT, DWORD, LONG);
int WINAPI ThreadWndProc   (HWND, UINT, DWORD, LONG);
int WINAPI About                 (HWND, UINT, DWORD, LONG);
LONG StartBounce     (PTHREADBLOCKINFO);
void DrawBox         (int, int, HBRUSH, PTHREADBLOCKINFO);
void MoveBox         (int *px, int *py, int *pdx, int *pdy, int *pvx, int *pvy, int ax, int ay, PTHREADBLOCKINFO pThreadBlockInfo);
void BounceProc      (PTHREADBLOCKINFO);
BOOL StartDemo       (int, PTHREADBLOCKINFO);
int  GetRandomVector ();
void InitializeBoxes (BOOL, PTHREADBLOCKINFO);

/***************************************************************************\
* main
\***************************************************************************/

int WINAPI WinMain(
    HINSTANCE  hInstance,
    HINSTANCE  hPrevInstance,
    LPSTR   lpCmdLine,
    int     nShowCmd)
{
    MSG msg;

    // this will change to something more reasonable

    ghModule = GetModuleHandle(NULL);

    if (!InitializeApp()) {
        MessageBox(ghwndMain, "MLTITHRD: InitializeApp failure!", "Error", MB_OK);
        return 0;
    }

/*
** DEMO MODE - These PostMessages are for Demonstration Only
*/

    PostMessage(ghwndMain, WM_COMMAND, MM_BOUNCE, 0);
    PostMessage(ghwndMain, WM_COMMAND, MM_BOUNCE, 0);
    PostMessage(ghwndMain, WM_COMMAND, MM_BOUNCE, 0);
    PostMessage(ghwndMain, WM_COMMAND, MM_BOUNCE, 0);
    PostMessage(ghwndMain, WM_COMMAND, IDM_TILE, 0);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 1;
}


/***************************************************************************\
* InitializeApp
\***************************************************************************/

BOOL InitializeApp(void)
{
    WNDCLASS wc;
    int i;

    srand(51537);

    for (i = 0; i < CCOLORS; i++) {
        gahbrColor[i] = CreateSolidBrush(aclr[i]);
    }

    ghbrWhite = CreateSolidBrush(0x00FFFFFF);
    ghbrBlack = CreateSolidBrush(0x00000000);

    wc.style            = CS_OWNDC;
    wc.lpfnWndProc      = (WNDPROC) MainWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = sizeof(LONG);
    wc.hInstance        = ghModule;
    wc.hIcon            = LoadIcon(ghModule,MAKEINTRESOURCE(APPICON));
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = ghbrWhite;
    wc.lpszMenuName     = "MainMenu";
    wc.lpszClassName    = "MltithrdClass";

    if (!RegisterClass(&wc))
        return FALSE;

    wc.style            = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = (WNDPROC) ThreadWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = ghModule;
    wc.hIcon            = LoadIcon(ghModule,MAKEINTRESOURCE(APPICON));
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = ghbrWhite;
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = "ThreadClass";

    if (!RegisterClass(&wc))
        return FALSE;

    hMenu = LoadMenu(ghModule, "MainMenu");
    hMenuWindow = GetSubMenu(hMenu, 1);

    ghwndMain = CreateWindowEx(0L, "MltithrdClass", "Mltithrd",
            WS_OVERLAPPED | WS_CAPTION | WS_BORDER | WS_THICKFRAME |
            WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CLIPCHILDREN |
            WS_VISIBLE | WS_SYSMENU,
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
\***************************************************************************/

int WINAPI MainWndProc(
    HWND hwnd,
    UINT message,
    DWORD wParam,
    LONG lParam)
{
    static int         iCount=1;
    static HWND        hwndClient;
    CLIENTCREATESTRUCT clientcreate;
    HWND               hwndChildWindow;


    switch (message) {


      case WM_CREATE:
        SetWindowLong(hwnd, 0, (LONG)NULL);

        clientcreate.hWindowMenu  = hMenuWindow;
        clientcreate.idFirstChild = 1;

        hwndClient = CreateWindow("MDICLIENT", NULL,
                                    WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
                                    0,0,0,0,
                                    hwnd, NULL, ghModule, (LPVOID)&clientcreate);
        return 0L;

      case WM_DESTROY: {
        HANDLE hHead, hTmp;
        PNODE  pHead;

        bKillMe = TRUE;

        hHead = (HANDLE) GetWindowLong(hwnd, 0);
        if (hHead) {
          if ((pHead = (PNODE) LocalLock(hHead))==NULL)
            MessageBox(ghwndMain, "Failed in LocalLock!", "Error", MB_OK);
          while (pHead->hNext != NULL) {

            hTmp = hHead;
            hHead = pHead->hNext;
            LocalUnlock(hTmp);

            if (LocalFree(hTmp)!=NULL)
                MessageBox(ghwndMain, "Failed in LocalFree!", "Error", MB_OK);
            if ((pHead = (PNODE) LocalLock(hHead))==NULL)
                MessageBox(ghwndMain, "Failed in LocalLock, hHead!", "Error", MB_OK);
          }
          LocalUnlock(hHead);

          if (LocalFree(hHead)!=NULL)
            MessageBox(ghwndMain, "LocalFree failed to free hHead!", "Error", MB_OK);
        }
        PostQuitMessage(0);
        return 0L;
      }

#ifdef LATER
      case WM_LBUTTONDOWN:
        InitializeBoxes(TRUE, &ThreadWindow);
        break;
#endif

      case WM_COMMAND:

        switch (LOWORD(wParam)) {
            case IDM_TILE:
                SendMessage(hwndClient, WM_MDITILE, 0L, 0L);
                return 0L;
            case IDM_CASCADE:
                SendMessage(hwndClient, WM_MDICASCADE, 0L, 0L);
                return 0L;
            case IDM_ARRANGE:
                SendMessage(hwndClient, WM_MDIICONARRANGE, 0L, 0L);
                return 0L;
            case MM_BOUNCE: {
                HANDLE hNode, hHead;
                HANDLE hThrd;
                PNODE  pNode;
                MDICREATESTRUCT mdicreate;

                hNode = LocalAlloc(LHND, (WORD) sizeof(NODE));
                if (hNode) {
                    if ((pNode = (PNODE)LocalLock(hNode))==NULL)
                        MessageBox(ghwndMain, "Failed in LocalLock, hNode!", "Error", MB_OK);

                    wsprintf((LPSTR)&(pNode->ThreadWindow.CaptionBarText), "Thread Window %d", iCount);

                    mdicreate.szClass = "ThreadClass";
                    mdicreate.szTitle = (LPSTR)&(pNode->ThreadWindow.CaptionBarText);
                    mdicreate.hOwner  = ghModule;
                    mdicreate.x       =
                    mdicreate.y       =
                    mdicreate.cx      =
                    mdicreate.cy      = CW_USEDEFAULT;
                    mdicreate.style   = 0l;
                    mdicreate.lParam  = 0L;

                    /*Create Child Window*/
                    hwndChildWindow = (HANDLE) SendMessage(hwndClient, WM_MDICREATE,
                                                  0L,
                                                  (LONG)(LPMDICREATESTRUCT)&mdicreate);

                    if (hwndChildWindow == NULL) {
                        MessageBox(ghwndMain, "Failed in Creating Thread Window!", "Error", MB_OK);
                        return 0L;
                    }

                    // This stuff is here for simplicity.  Initialization should
                    //   be done in StartDemo
                    pNode->ThreadWindow.hwndClient       = hwndClient;
                    pNode->ThreadWindow.hwndThreadWindow = hwndChildWindow;
                    hHead = (HANDLE)GetWindowLong(hwnd, 0);
                    pNode->hNext = hHead;
                    SetWindowLong(hwnd, 0, (LONG) hNode);

            // Create the thread suspended so we can alter its priority
            // before it begins to run.

                    hThrd = CreateThread(NULL, 0,
                                         (LPTHREAD_START_ROUTINE)StartBounce,
                                         &pNode->ThreadWindow,
                                         CREATE_SUSPENDED,
                                         (LPDWORD) &pNode->ThreadWindow.lThreadId );
                    if (hThrd) {

                         // I really don't need this info, it won't even be
                         // updated right the way for this thread will be
                         // preempted.
                         pNode->ThreadWindow.hThread = hThrd;
                         iCount++;

             // This way the primary (input) thread will be higher priority
             // than the bouncers, thus keeping MLTITHRD responsive to user
             // input.

             SetThreadPriority(hThrd, THREAD_PRIORITY_BELOW_NORMAL);

             // Everything's set, let it go.

             ResumeThread(hThrd);

                    } else {
                              MessageBox(ghwndMain, "Create Thread Failed!", "Error", MB_OK);
                    }
                    LocalUnlock(hNode);

                } else {
                            MessageBox(ghwndMain, "Failed to Allocate Node!", "Error", MB_OK);
                }
                return 0L;
            }

            case MM_ABOUT:
                if (DialogBox(ghModule, "AboutBox", ghwndMain, (DLGPROC)About) == -1)
                        MessageBox(ghwndMain, "Mltithrd: About Dialog Creation Error!", "Error", MB_OK);
                return 0L;

            default:
                return DefFrameProc(hwnd,  hwndClient, message, wParam, lParam);
        }

    default:

        return DefFrameProc(hwnd,  hwndClient, message, wParam, lParam);
    }
}

/********************************************************************\
* ThreadWndProc
\***************************************************************************/

int WINAPI ThreadWndProc(
    HWND hwnd,
    UINT message,
    DWORD wParam,
    LONG lParam)
{
    switch (message) {
        case WM_SIZE:

        case WM_CREATE: {
            PTHREADBLOCKINFO pThreadBlockInfo;
            PNODE            pHead;
            HANDLE           hHead, hTmp;



            // now find match
            hHead = (HANDLE) GetWindowLong(ghwndMain, 0);
            if (hHead) {
                if ((pHead = (PNODE)LocalLock(hHead))==NULL)
                    MessageBox(ghwndMain, "Failed in LocalLock!", "Error", MB_OK);

                while ((pHead->ThreadWindow.hwndThreadWindow != hwnd) &&
                      (pHead->hNext != NULL)) {
                   hTmp = hHead;
                   hHead = pHead->hNext;
                   LocalUnlock(hTmp);

                   if ((pHead = (PNODE) LocalLock(hHead))==NULL)
                        MessageBox(ghwndMain, "Failed in LocalLock!", "Error", MB_OK);
                }
                if (pHead->ThreadWindow.hwndThreadWindow == hwnd) {
                    pThreadBlockInfo = &pHead->ThreadWindow;
                    goto Thread_Found;
                } else {
                    //MessageBox(ghwndMain, "Trouble - Can't find the thread node!", "Error", MB_OK);
                    goto Thread_Out;
                }

Thread_Found:
                if (!GetClientRect(pThreadBlockInfo->hwndThreadWindow,
                                   &pThreadBlockInfo->rcClient))
                    MessageBox(ghwndMain, "Failed in GetClientRect!", "Error", MB_OK);
Thread_Out:
                LocalUnlock(hHead);

                return DefMDIChildProc(hwnd, message, wParam, lParam);
            } else {
                        //MessageBox(ghwndMain, "Can't GetWindowLong(ghwndMain,0) !", "Error", MB_OK);
            }           return DefMDIChildProc(hwnd, message, wParam, lParam);
        }

        case WM_CLOSE: {
            PTHREADBLOCKINFO pThreadBlockInfo;
            PNODE            pHead;
            HANDLE           hHead, hTmp;



            // now find match
            hHead = (HANDLE) GetWindowLong(ghwndMain, 0);
            if (hHead) {
                if ((pHead = (PNODE)LocalLock(hHead))==NULL)
                    MessageBox(ghwndMain, "Failed in LocalLock!", "Error", MB_OK);

                while ((pHead->ThreadWindow.hwndThreadWindow != hwnd) &&
                      (pHead->hNext != NULL)) {
                   hTmp = hHead;
                   hHead = pHead->hNext;
                   LocalUnlock(hTmp);

                   if ((pHead = (PNODE) LocalLock(hHead))==NULL)
                        MessageBox(ghwndMain, "Failed in LocalLock!", "Error", MB_OK);
                }
                if (pHead->ThreadWindow.hwndThreadWindow == hwnd) {
                    pThreadBlockInfo = &pHead->ThreadWindow;
                    goto Thread_Found1;
                } else {
                    //MessageBox(ghwndMain, "Trouble - Can't find the thread node!", "Error", MB_OK);
                    goto Thread_Out1;
                }

Thread_Found1:
                pThreadBlockInfo->bKillThrd = TRUE;
                CloseHandle(pThreadBlockInfo->hThread);

Thread_Out1:
                LocalUnlock(hHead);

            } else {
                        //MessageBox(ghwndMain, "Can't GetWindowLong(ghwndMain,0) !", "Error", MB_OK);
            }
            return DefMDIChildProc(hwnd, message, wParam, lParam);
        }

        case WM_DESTROY:


            return 0L;

        default:
                return DefMDIChildProc(hwnd, message, wParam, lParam);
    }

}


/***************************************************************************\
* About
\***************************************************************************/

int WINAPI About(
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

    lParam;
    hDlg;
}

/* Multithreading concerns for this function						 */
/* 																	 */
/* 1. For optimized code, any member of pThreadBlockInfo may be		 */
/*    regarded by the compiler as loop-invariant and enregistered.	 */
/*    That means that if another thread changes the value of 		 */
/*    bKillThrd, you will never know it. To remedy this situation, 	 */
/*    declare the function:                        					 */
/*                                                                   */        
/*    long Startbounce(volatile PTHREADBLOCKINFO pThreadBlockInfo);  */        
/*                                                                   */        
/* 2. Members of pThreadBlockInfo may be changed incompletely in	 */
/*    another thread before that thread has to yield the processor.	 */
/*    If there is no other consumer of this information, then no 	 */
/*    action is required; however, it this memory is used for inter- */
/*    thread communication, surround any updates to members with 	 */
/*    EnterCriticalSection and LeaveCriticalSection.				 */
                
long StartBounce(PTHREADBLOCKINFO pThreadBlockInfo)
{
    if (!StartDemo(MM_BOUNCE, pThreadBlockInfo))
        return(0);

    cmdDemo = MM_BOUNCE;
        /* Here everythings been initialized, ThreadBlockINfo, etc*/
        /* Now loop and call BounceProc to draw and move boxes*/

        /* NOTE NOTE, each thread MUST have its own message loop!*/

    while (TRUE) {

      if (!bKillMe && !pThreadBlockInfo->bKillThrd) {
        BounceProc(pThreadBlockInfo);
      } else {
                break;
      }
    }

    ExitThread(0);
}


BOOL StartDemo(
    int cmd,
    PTHREADBLOCKINFO pThreadBlockInfo)
{

    // Initializing the rcClient for the bouncing box
    // Better be initializing here for once CreateThread is called, this
    //   thread got the time slice; parent thread is preempted.
    if (!GetClientRect(pThreadBlockInfo->hwndThreadWindow,
                       &pThreadBlockInfo->rcClient))
        MessageBox(ghwndMain, "Failed in GetClientRect!", "Error", MB_OK);
    pThreadBlockInfo->bKillThrd = FALSE;

    InitializeBoxes(FALSE, pThreadBlockInfo);

    return TRUE;
        UNREFERENCED_PARAMETER(cmd);
}


void InitializeBoxes(
    BOOL fVectorsOnly,
    PTHREADBLOCKINFO pThreadBlockInfo)
{
    int j;
    int i;

    /*
     * Initialize bounce arrays...
     */
    for (i = 0; i < COBJS; i++) {

        if (!fVectorsOnly) {
            for (j = 0; j < CPOINTS; j++) {
                pThreadBlockInfo->gab[i].apt[j].x = 0;
                pThreadBlockInfo->gab[i].apt[j].y = 0;
            }
            pThreadBlockInfo->gab[i].iColor = 0;
        }

        if (GetRandomVector() <= 3) {
            if (GetRandomVector() >= 4) {
                pThreadBlockInfo->gab[i].vx = 4;
            } else {
                pThreadBlockInfo->gab[i].vx = 1;
            }

            pThreadBlockInfo->gab[i].vy = GetRandomVector();
        } else {
            if (GetRandomVector() >= 4) {
                pThreadBlockInfo->gab[i].vy = 4;
            } else {
                pThreadBlockInfo->gab[i].vy = 1;
            }

            pThreadBlockInfo->gab[i].vx = GetRandomVector();
        }

        pThreadBlockInfo->gab[i].dx = pThreadBlockInfo->gab[i].vx;
        pThreadBlockInfo->gab[i].dy = pThreadBlockInfo->gab[i].vy;

        pThreadBlockInfo->gab[i].ax = GetRandomVector() + 3;
        pThreadBlockInfo->gab[i].ay = GetRandomVector() + 3;
    }
}

int GetRandomVector()
{
    /*
     * Returns a number between 1 and 6.
     */
    return (rand() % 6) + 1;
}

void BounceProc( PTHREADBLOCKINFO pThreadBlockInfo)
{
    int i;
        LONG lCurrentThreadId;

#ifdef LATER
// not erasing looks cool
    /*
     * Erase...
     */
    for (i = 0; i < COBJS; i++)
        DrawBox(gab[i].apt[0].x, gab[i].apt[0].y, ghbrWhite);
#endif


    /*
     * Move, and bounce on wall
     */
    for (i = 0; i < COBJS; i++)
        MoveBox((int *)&pThreadBlockInfo->gab[i].apt[0].x, (int *)&pThreadBlockInfo->gab[i].apt[0].y,
                (int *)&pThreadBlockInfo->gab[i].dx, (int *)&pThreadBlockInfo->gab[i].dy,
                (int *)&pThreadBlockInfo->gab[i].vx, (int *)&pThreadBlockInfo->gab[i].vy,
                (int)pThreadBlockInfo->gab[i].ax, (int)pThreadBlockInfo->gab[i].ay, pThreadBlockInfo);

    /*
     * Draw new...
     */
    for (i = 0; i < COBJS; i++) {
        DrawBox(pThreadBlockInfo->gab[i].apt[0].x, pThreadBlockInfo->gab[i].apt[0].y,
                gahbrColor[pThreadBlockInfo->gab[i].iColor++ % CCOLORS], pThreadBlockInfo);
    }

        UNREFERENCED_PARAMETER(lCurrentThreadId);
}

void DrawBox(
    int x,
    int y,
    HBRUSH hbr,
    PTHREADBLOCKINFO pThreadBlockInfo)
{
    HBRUSH hbrOld;

    pThreadBlockInfo->hdcThreadWindow =
        GetDC(pThreadBlockInfo->hwndThreadWindow);
    if (pThreadBlockInfo->hdcThreadWindow) {
       hbrOld = SelectObject(pThreadBlockInfo->hdcThreadWindow, hbr);
       BitBlt(pThreadBlockInfo->hdcThreadWindow, x, y, 20, 20, NULL, 0, 0, PATCOPY);
       SelectObject(pThreadBlockInfo->hdcThreadWindow, hbrOld);
       ReleaseDC(pThreadBlockInfo->hwndThreadWindow, pThreadBlockInfo->hdcThreadWindow);
    } else {
            MessageBox(ghwndMain, "Failed in GetDC!", "Error", MB_OK);
    }
}

void MoveBox(
    int *px,
    int *py,
    int *pdx,
    int *pdy,
    int *pvx,
    int *pvy,
    int ax,
    int ay,
        PTHREADBLOCKINFO pThreadBlockInfo)
{

    (*pdx)--;
    if (*pdx == 0) {
        *px -= ax;
        *pdx = *pvx;
    } else if (*pdx == 3) {
        *px += ax;
        *pdx = *pvx;
    }

    (*pdy)--;
    if (*pdy == 0) {
        *py -= ay;
        *pdy = *pvy;
    } else if (*pdy == 3) {
        *py += ay;
        *pdy = *pvy;
    }

    if (*px < pThreadBlockInfo->rcClient.left) {
        *px = pThreadBlockInfo->rcClient.left;

        if (*pvx >= 4)
            *pvx -= 3;
        else
            *pvx += 3;

        *pdx = *pvx;
    }

    if (*px > (pThreadBlockInfo->rcClient.right - 20)) {
        *px = pThreadBlockInfo->rcClient.right - 20;

        if (*pvx >= 4)
            *pvx -= 3;
        else
            *pvx += 3;

        *pdx = *pvx;
    }

    if (*py < pThreadBlockInfo->rcClient.top) {
        *py = pThreadBlockInfo->rcClient.top;

        if (*pvy >= 4)
            *pvy -= 3;
        else
            *pvy += 3;

        *pdy = *pvy;
    }

    if (*py > (pThreadBlockInfo->rcClient.bottom - 20)) {
        *py = pThreadBlockInfo->rcClient.bottom - 20;

        if (*pvy >= 4)
            *pvy -= 3;
        else
            *pvy += 3;

        *pdy = *pvy;
    }
}
