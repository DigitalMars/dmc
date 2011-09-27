
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#include <windows.h>    // includes basic windows functionality
#include <stdio.h>      // includes standard file i/o functionality
#include <string.h>     // includes string functions
#include "hooktest.h"

#define NUMHOOKS 7

/* Global variables */

typedef struct _MYHOOKDATA {
    int nType;
    HOOKPROC hkprc;
    HHOOK hhook;
} MYHOOKDATA;

MYHOOKDATA myhookdata[NUMHOOKS];

HWND         hwndMain;
HANDLE       hInst;


/****************************************************************************
*
*    FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)
*
*    PURPOSE: calls initialization function, processes message loop
*
*    COMMENTS:
*
*
****************************************************************************/

int APIENTRY WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
    )
{

    MSG msg;                         /* message                      */

        if (!InitApplication(hInstance)) /* Initialize shared things */
            return (FALSE);              /* Exits if unable to initialize     */

    hInst = hInstance;

    /* Perform initializations that apply to a specific instance */

    if (!InitInstance(hInstance, nCmdShow))
        return (FALSE);

    // register window message for FindText() and ReplaceText() hook procs
    /* Acquire and dispatch messages until a WM_QUIT message is received. */

    while (GetMessage(&msg,        /* message structure                      */
            NULL,                  /* handle of window receiving the message */
            0,                     /* lowest message to examine              */
            0))                    /* highest message to examine             */
        {
        TranslateMessage(&msg);    /* Translates virtual key codes           */
        DispatchMessage(&msg);     /* Dispatches message to window           */
    }
    return (msg.wParam);           /* Returns the value from PostQuitMessage */

    // avoid compiler warnings at W3
    lpCmdLine;
    hPrevInstance;
}


/****************************************************************************
*
*    FUNCTION: InitApplication(HANDLE)
*
*    PURPOSE: Initializes window data and registers window class
*
*    COMMENTS:
*
*        In this function, we initialize a window class by filling out a data
*        structure of type WNDCLASS and calling the Windows RegisterClass()
*        function.
*
****************************************************************************/

BOOL InitApplication(HANDLE hInstance)       /* current instance             */
{
    WNDCLASS  wc;

    /* Fill in window class structure with parameters that describe the       */
    /* main window.                                                           */

    wc.style = 0;                       /* Class style(s).                    */
    wc.lpfnWndProc = (WNDPROC)MainWndProc;       /* Function to retrieve messages for  */
                                        /* windows of this class.             */
    wc.cbClsExtra = 0;                  /* No per-class extra data.           */
    wc.cbWndExtra = 0;                  /* No per-window extra data.          */
    wc.hInstance = hInstance;           /* Application that owns the class.   */
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH); 
    wc.lpszMenuName =  "HookMenu";   /* Name of menu resource in .RC file. */
    wc.lpszClassName = "HookWClass"; /* Name used in call to CreateWindow. */

    /* Register the window class and return success/failure code. */

    return (RegisterClass(&wc));

}


/****************************************************************************
*
*    FUNCTION:  InitInstance(HANDLE, int)
*
*    PURPOSE:  Saves instance handle and creates main window
*
*    COMMENTS:
*
*        In this function, we save the instance handle in a static variable and
*        create and display the main program window.
*
****************************************************************************/

BOOL InitInstance(
    HANDLE          hInstance,          /* Current instance identifier.       */
    int             nCmdShow)           /* Param for first ShowWindow() call. */
{

    /* Save the instance handle in static variable, which will be used in  */
    /* many subsequence calls from this application to Windows.            */

    hInst = hInstance;

    /* Create a main window for this application instance.  */

    hwndMain = CreateWindow(
        "HookWClass",                 /* See RegisterClass() call.          */
        "Hooks Sample Application",   /* Text for window title bar.         */
        WS_OVERLAPPEDWINDOW,            /* Window style.                      */
        CW_USEDEFAULT,                  /* Default horizontal position.       */
        CW_USEDEFAULT,                  /* Default vertical position.         */
        CW_USEDEFAULT,                  /* Default width.                     */
        CW_USEDEFAULT,                  /* Default height.                    */
        NULL,                           /* Overlapped windows have no parent. */
        NULL,                           /* Use the window class menu.         */
        hInstance,                      /* This instance owns this window.    */
        NULL                            /* Pointer not needed.                */
    );

    /* If window could not be created, return "failure" */

    if (!hwndMain)
        return (FALSE);

    /* Make the window visible; update its client area; and return "success" */

    ShowWindow(hwndMain, nCmdShow);  /* Show the window                        */
    UpdateWindow(hwndMain);          /* Sends WM_PAINT message                 */
    return (TRUE);               /* Returns the value from PostQuitMessage */

}

/****************************************************************************
*
*    FUNCTION: MainWndProc(HWND, unsigned, WORD, LONG)
*
*    PURPOSE:  Processes messages
*
*    COMMENTS:
*
*        This function processes all messages sent to the window.  When the
*        user chooses one of the options from one of the menus, the command
*        is processed here and passed onto the function for that command.
*        This function also processes the special "FindReplace" message that
*        this application registers for hook processing of the FindText()
*        and ReplaceText() common dialog functions.
*
****************************************************************************/



LONG APIENTRY MainWndProc(hwndMain, uMsg, wParam, lParam)
HWND hwndMain;
UINT uMsg;
UINT wParam;
LONG lParam;
{
    static BOOL aHooks[NUMHOOKS];
    int index;
    static HMENU hmenu;


    switch (uMsg) {
        case WM_CREATE:

            /* Save the menu handle. */

            hmenu = GetMenu(hwndMain);

            /*
             * Initialize structures with hook data. The menu-item
             * identifiers are defined as 0 through 6 in the
             * header file. They can be used to identify array
             * elements both here and during the WM_COMMAND
             * message.
             */

            myhookdata[IDM_CALLWNDPROC].nType = WH_CALLWNDPROC;
            myhookdata[IDM_CALLWNDPROC].hkprc = CallWndProc;

            myhookdata[IDM_CBT].nType = WH_CBT;
            myhookdata[IDM_CBT].hkprc = CBTProc;
            myhookdata[IDM_DEBUG].nType = WH_DEBUG;
            myhookdata[IDM_DEBUG].hkprc = DebugProc;
            myhookdata[IDM_GETMESSAGE].nType = WH_GETMESSAGE;
            myhookdata[IDM_GETMESSAGE].hkprc = GetMsgProc;
            myhookdata[IDM_KEYBOARD].nType = WH_KEYBOARD;

            myhookdata[IDM_KEYBOARD].hkprc = KeyboardProc;
            myhookdata[IDM_MOUSE].nType = WH_MOUSE;
            myhookdata[IDM_MOUSE].hkprc = MouseProc;
            myhookdata[IDM_MSGFILTER].nType = WH_MSGFILTER;
            myhookdata[IDM_MSGFILTER].hkprc = MessageProc;

            /* Initialize all flags in the array to FALSE. */
            memset(aHooks, FALSE, sizeof(aHooks));

            return 0;

        case WM_COMMAND:

            switch (LOWORD(wParam)) {

                case IDM_EXIT:
                    PostQuitMessage(0);
                    break;

                /*
                 * The user selected a hook command from the menu.
                 */

                case IDM_CALLWNDPROC:
                case IDM_CBT:
                case IDM_DEBUG:
                case IDM_GETMESSAGE:
                case IDM_KEYBOARD:
                case IDM_MOUSE:
                case IDM_MSGFILTER:

                    /*
                     * Use the menu-item identifier as an index
                     * into the array of structures with hook data.

                     */

                    index = LOWORD(wParam);

                    /*
                     * If the selected type of hook procedure isn't
                     * installed yet, install it and check the
                     * associated menu item.
                     */

                    if (!aHooks[index]) {
                        myhookdata[index].hhook = SetWindowsHookEx(
                            myhookdata[index].nType,
                            myhookdata[index].hkprc,
                            (HINSTANCE) NULL, GetCurrentThreadId());
                        CheckMenuItem(hmenu, index,
                            MF_BYCOMMAND | MF_CHECKED);
                        aHooks[index] = TRUE;
                    }

                    /*
                     * If the selected type of hook procedure is
                     * already installed, remove it and remove the
                     * check mark from the associated menu item.
                     */


                    else {
                        UnhookWindowsHookEx(myhookdata[index].hhook);
                        CheckMenuItem(hmenu, index,
                            MF_BYCOMMAND | MF_UNCHECKED);
                        aHooks[index] = FALSE;
                        InvalidateRect(hwndMain, NULL, TRUE);
                        UpdateWindow(hwndMain);
                    }

           }
            break;

             case WM_DESTROY:
                 PostQuitMessage(0);
                 break;

        default:
            return DefWindowProc(hwndMain, uMsg, wParam, lParam);
    }
    return 0;
}

/****************************************************************
  WH_CALLWNDPROC hook procedure
 ****************************************************************/

LRESULT CALLBACK CallWndProc(nCode, wParam, lParam)
int nCode;
WPARAM wParam;
LPARAM lParam;
{
    CHAR szCWPBuf[256];
    HDC hdc;

    static int c = 0;
    int cch;

    if (nCode < 0)  /* do not process message */
        return CallNextHookEx(myhookdata[IDM_CALLWNDPROC].hhook, nCode,
                wParam, lParam);

    hdc = GetDC(hwndMain);

    switch (nCode) {
        case HC_ACTION:
            cch = wsprintf(szCWPBuf,
               "CALLWNDPROC - tsk: %ld, msg: %d, %d times %20s",
                wParam, nCode, c++, "" );
            TextOut(hdc, 2, 15, szCWPBuf, cch);
            break;

        default:
            break;
    }

    ReleaseDC(hwndMain, hdc);
    return CallNextHookEx(myhookdata[IDM_CALLWNDPROC].hhook, nCode,
        wParam, lParam);
}

/****************************************************************
  WH_GETMESSAGE hook procedure
 ****************************************************************/

LRESULT CALLBACK GetMsgProc(nCode, wParam, lParam)
int nCode;
WPARAM wParam;

LPARAM lParam;
{
    CHAR szMSGBuf[256];
    CHAR szRem[16];
    HDC hdc;
    static int c = 0;
    int cch;

    if (nCode < 0) /* do not process message */
        return CallNextHookEx(myhookdata[IDM_GETMESSAGE].hhook, nCode,
            wParam, lParam);

    switch (nCode) {
        case HC_ACTION:
            switch (wParam) {
                case PM_REMOVE:
                    lstrcpy(szRem, "PM_REMOVE");
                    break;

                case PM_NOREMOVE:
                    lstrcpy(szRem, "PM_NOREMOVE");
                    break;

                default:
                    lstrcpy(szRem, "Unknown");
                    break;
            }

            hdc = GetDC(hwndMain);

            cch = wsprintf(szMSGBuf,
                "GETMESSAGE - wParam: %s, msg: %d, %d times %20s",
                szRem, nCode, c++, "");
            TextOut(hdc, 2, 35, szMSGBuf, cch);
            break;

        default:
            break;
    }

    ReleaseDC(hwndMain, hdc);
    return CallNextHookEx(myhookdata[IDM_GETMESSAGE].hhook, nCode,
        wParam, lParam);
}

/****************************************************************
  WH_DEBUG hook procedure
 ****************************************************************/

LRESULT CALLBACK DebugProc(nCode, wParam, lParam)

int nCode;
WPARAM wParam;
LPARAM lParam;
{
    CHAR szBuf[128];
    HDC hdc;
    static int c = 0;
    int cch;

    if (nCode < 0)  /* do not process message */
        return CallNextHookEx(myhookdata[IDM_DEBUG].hhook, nCode,
            wParam, lParam);

    hdc = GetDC(hwndMain);

    switch (nCode) {
        case HC_ACTION:
            cch = wsprintf(szBuf,
                "DEBUG - nCode: %d, tsk: %ld, %d times %20s",
                nCode,wParam, c++, "");
            TextOut(hdc, 2, 55, szBuf, cch);
            break;

        default:
            break;
    }

    ReleaseDC(hwndMain, hdc);
    return CallNextHookEx(myhookdata[IDM_DEBUG].hhook, nCode, wParam,
        lParam);
}

/****************************************************************
  WH_CBT hook procedure
 ****************************************************************/

LRESULT CALLBACK CBTProc(nCode, wParam, lParam)
int nCode;
WPARAM wParam;
LPARAM lParam;

{
    CHAR szBuf[128];
    CHAR szCode[128];
    HDC hdc;
    static int c = 0;
    int cch;

    if (nCode < 0)  /* do not process message */
        return CallNextHookEx(myhookdata[IDM_CBT].hhook, nCode, wParam,
            lParam);

    hdc = GetDC(hwndMain);

    switch (nCode) {
        case HCBT_ACTIVATE:
            lstrcpy(szCode, "HCBT_ACTIVATE");
            break;

        case HCBT_CLICKSKIPPED:
            lstrcpy(szCode, "HCBT_CLICKSKIPPED");
            break;

        case HCBT_CREATEWND:
            lstrcpy(szCode, "HCBT_CREATEWND");
            break;

        case HCBT_DESTROYWND:
            lstrcpy(szCode, "HCBT_DESTROYWND");
            break;

        case HCBT_KEYSKIPPED:
            lstrcpy(szCode, "HCBT_KEYSKIPPED");
            break;

        case HCBT_MINMAX:
            lstrcpy(szCode, "HCBT_MINMAX");
            break;

        case HCBT_MOVESIZE:
            lstrcpy(szCode, "HCBT_MOVESIZE");
            break;

        case HCBT_QS:
            lstrcpy(szCode, "HCBT_QS");
            break;

        case HCBT_SETFOCUS:
            lstrcpy(szCode, "HCBT_SETFOCUS");
            break;

        case HCBT_SYSCOMMAND:

            lstrcpy(szCode, "HCBT_SYSCOMMAND");
            break;

        default:
            lstrcpy(szCode, "Unknown");
            break;
    }

    cch = wsprintf(szBuf, "CBT - nCode: %s, tsk: %ld, %d times %20s",
        szCode, wParam, c++, "");
    TextOut(hdc, 2, 75, szBuf, cch);
    ReleaseDC(hwndMain, hdc);
    return CallNextHookEx(myhookdata[IDM_CBT].hhook, nCode, wParam,
        lParam);
}

/****************************************************************
  WH_MOUSE hook procedure

 ****************************************************************/

LRESULT CALLBACK MouseProc(nCode, wParam, lParam)
int nCode;
WPARAM wParam;
LPARAM lParam;
{
    CHAR szBuf[128];
    HDC hdc;
    static int c = 0;
    int cch;
	LPMOUSEHOOKSTRUCT lpMouseHookStruct;

    if (nCode < 0)  /* do not process the message */
        return CallNextHookEx(myhookdata[IDM_MOUSE].hhook, nCode,
            wParam, lParam);


    hdc = GetDC(hwndMain);

	lpMouseHookStruct = (LPMOUSEHOOKSTRUCT)lParam; 
    cch = wsprintf(szBuf,
        "MOUSE - nCode: %d, msg: %d, x: %d, y: %d, %d times %20s",
        nCode, wParam, lpMouseHookStruct->pt.x, lpMouseHookStruct->pt.y, 
        c++, "" );
    TextOut(hdc, 2, 95, szBuf, cch);
    ReleaseDC(hwndMain, hdc);
    return CallNextHookEx(myhookdata[IDM_MOUSE].hhook, nCode, wParam,

        lParam);
}

/****************************************************************
  WH_KEYBOARD hook procedure
 ****************************************************************/

LRESULT CALLBACK KeyboardProc(nCode, wParam, lParam)
int nCode;
WPARAM wParam;
LPARAM lParam;
{
    CHAR szBuf[128];
    HDC hdc;
    static int c = 0;
    int cch;

    if (nCode < 0)  /* do not process message */
        return CallNextHookEx(myhookdata[IDM_KEYBOARD].hhook, nCode,
            wParam, lParam);

    hdc = GetDC(hwndMain);

    cch = wsprintf(szBuf, "KEYBOARD - nCode: %d, vk: %d, %d times %20s",
        nCode, wParam, c++, "" );
    TextOut(hdc, 2, 115, szBuf, cch);
    ReleaseDC(hwndMain, hdc);
    return CallNextHookEx(myhookdata[IDM_KEYBOARD].hhook, nCode, wParam,
        lParam);
}

/****************************************************************
  WH_MSGFILTER hook procedure
 ****************************************************************/

LRESULT CALLBACK MessageProc(nCode, wParam, lParam)
int nCode;
WPARAM wParam;

LPARAM lParam;
{
    CHAR szBuf[128];
    CHAR szCode[32];
    HDC hdc;
    static int c = 0;
    int cch;

    if (nCode < 0)  /* do not process message */
        return CallNextHookEx(myhookdata[IDM_MSGFILTER].hhook, nCode,
            wParam, lParam);

    switch (nCode) {
        case MSGF_DIALOGBOX:
            lstrcpy(szCode, "MSGF_DIALOGBOX");
            break;


        case MSGF_MENU:
            lstrcpy(szCode, "MSGF_MENU");
            break;

        case MSGF_SCROLLBAR:
            lstrcpy(szCode, "MSGF_SCROLLBAR");
            break;

        case MSGF_NEXTWINDOW:
            lstrcpy(szCode, "MSGF_NEXTWINDOW");
            break;

        default:
            wsprintf(szCode, "Unknown: %d", nCode);
            break;
    }

    hdc = GetDC(hwndMain);
    cch = wsprintf(szBuf,
        "MSGFILTER  nCode: %s, msg: %d, %d times %20s",
        szCode, nCode, c++, "" );
    TextOut(hdc, 2, 135, szBuf, cch);
    ReleaseDC(hwndMain, hdc);
    return CallNextHookEx(myhookdata[IDM_MSGFILTER].hhook, nCode,
        wParam, lParam);
}

