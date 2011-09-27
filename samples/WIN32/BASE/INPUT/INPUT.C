
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/****************************************************************************

    PROGRAM: Input.c

    PURPOSE: Input template for Windows applications

    FUNCTIONS:

        WinMain() - calls initialization function, processes message loop
        InitApplication() - initializes window data and registers window
        InitInstance() - saves instance handle and creates main window
        MainWndProc() - processes messages
        About() - processes messages for "About" dialog box

****************************************************************************/

#include "windows.h"
#include <string.h>
#include "input.h"

HANDLE hInst;

CHAR MouseText[48];                                   /* mouse state         */
CHAR ButtonText[48];                                  /* mouse-button state  */
CHAR KeyboardText[48];                                /* keyboard state      */
CHAR CharacterText[48];                               /* latest character    */
CHAR ScrollText[48];                                  /* scroll status       */
CHAR TimerText[48];                                   /* timer state         */
RECT rectMouse;
RECT rectButton;
RECT rectKeyboard;
RECT rectCharacter;
RECT rectScroll;
RECT rectTimer;
INT idTimer;                                          /* timer ID            */
INT nTimerCount = 0;                                  /* current timer count */

/****************************************************************************

    FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)

    PURPOSE: calls initialization function, processes message loop

****************************************************************************/

int APIENTRY WinMain(
    HANDLE hInstance,
    HANDLE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
    )
{
    MSG msg;

    UNREFERENCED_PARAMETER( lpCmdLine );

    if (!hPrevInstance)
        if (!InitApplication(hInstance))
            return (FALSE);

    if (!InitInstance(hInstance, nCmdShow))
        return (FALSE);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.wParam);
}


/****************************************************************************

    FUNCTION: InitApplication(HANDLE)

    PURPOSE: Initializes window data and registers window class

****************************************************************************/

BOOL InitApplication(HANDLE hInstance)
{
    WNDCLASS  wc;

    wc.style = CS_DBLCLKS;          /* double-click messages */
    wc.lpfnWndProc = (WNDPROC) MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH); 
    wc.lpszMenuName =  "InputMenu";
    wc.lpszClassName = "InputWClass";

    return (RegisterClass(&wc));
}


/****************************************************************************

    FUNCTION:  InitInstance(HANDLE, int)

    PURPOSE:  Saves instance handle and creates main window

****************************************************************************/

BOOL InitInstance(
    HANDLE          hInstance,
    INT             nCmdShow)
{
    HWND            hWnd;
    HDC             hDC;
    TEXTMETRIC      textmetric;
    RECT            rect;
    INT             nLineHeight;
 

    hInst = hInstance;

    hWnd = CreateWindow(
        "InputWClass",
        "Input Sample Application",
        WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,  /* horz & vert scroll bars */
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
        return (FALSE);


    hDC = GetDC(hWnd);
    GetTextMetrics(hDC, &textmetric);
    nLineHeight = textmetric.tmExternalLeading + textmetric.tmHeight;

    rect.left   = GetDeviceCaps(hDC, LOGPIXELSX) / 4;   /* 1/4 inch */
    rect.right  = GetDeviceCaps(hDC, HORZRES);
    rect.top    = GetDeviceCaps(hDC, LOGPIXELSY) / 4;   /* 1/4 inch */
    ReleaseDC(hWnd, hDC);
    rect.bottom = rect.top + nLineHeight;
    rectMouse   = rect;

    rect.top += nLineHeight;
    rect.bottom += nLineHeight;
    rectButton = rect;

    rect.top += nLineHeight;
    rect.bottom += nLineHeight;
    rectKeyboard = rect;

    rect.top += nLineHeight;
    rect.bottom += nLineHeight;
    rectCharacter = rect;

    rect.top += nLineHeight;
    rect.bottom += nLineHeight;
    rectScroll = rect;

    rect.top += nLineHeight;
    rect.bottom += nLineHeight;
    rectTimer = rect;
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return (TRUE);

}

/****************************************************************************

    FUNCTION: MainWndProc(HWND, unsigned, WORD, LONG)

    PURPOSE:  Processes messages

    MESSAGES:

        WM_COMMAND    - application menu (About dialog box)
        WM_CREATE     - create window
        WM_MOUSEMOVE  - mouse movement
        WM_LBUTTONDOWN - left mouse button pressed
        WM_LBUTTONUP  - left mouse button released
        WM_LBUTTONDBLCLK - left mouse button double clicked
        WM_KEYDOWN    - key pressed
        WM_KEYUP      - key released
        WM_CHAR       - ASCII character received
        WM_TIMER      - timer has elapsed
        WM_HSCROLL    - mouse click in horizontal scroll bar
        WM_VSCROLL    - mouse click in vertical scroll bar
        WM_PAINT      - update window, draw objects
        WM_DESTROY    - destroy window

    COMMENTS:

        This demonstrates how input messages are received, and what the
        additional information is that comes with the message.

****************************************************************************/

LONG APIENTRY MainWndProc(
        HWND hWnd,
        UINT message,
        UINT wParam,
        LONG lParam)
{
    FARPROC lpProcAbout;

    HDC hDC;                         /* display-context variable     */
    PAINTSTRUCT ps;                  /* paint structure              */
    CHAR HorzOrVertText[12];
    CHAR ScrollTypeText[20];
    RECT rect;

    switch (message) {
        case WM_COMMAND:
            if (LOWORD(wParam) == IDM_ABOUT) {
                lpProcAbout = MakeProcInstance((FARPROC)About, hInst);

                DialogBox(hInst,
                    "AboutBox",
                    hWnd,
                    lpProcAbout);

                FreeProcInstance(lpProcAbout);
                break;
            }
            else
                return (DefWindowProc(hWnd, message, wParam, lParam));

        case WM_CREATE:

            /* Set the timer for five-second intervals */

            idTimer =  SetTimer(hWnd, 0, 5000, (TIMERPROC)(LPVOID)NULL);

            break;

        case WM_MOUSEMOVE:
            wsprintf(MouseText, "WM_MOUSEMOVE: %x, %d, %d",
                wParam, LOWORD(lParam), HIWORD(lParam));
            InvalidateRect(hWnd, &rectMouse, TRUE);
            break;

        case WM_LBUTTONDOWN:
            wsprintf(ButtonText, "WM_LBUTTONDOWN: %x, %d, %d",
                wParam, LOWORD(lParam), HIWORD(lParam));
            InvalidateRect(hWnd, &rectButton, TRUE);
            break;

        case WM_LBUTTONUP:
            wsprintf(ButtonText, "WM_LBUTTONUP: %x, %d, %d",
                wParam, LOWORD(lParam), HIWORD(lParam));
            InvalidateRect(hWnd, &rectButton, TRUE);
            break;

        case WM_LBUTTONDBLCLK:
            wsprintf(ButtonText, "WM_LBUTTONDBLCLK: %x, %d, %d",
                wParam, LOWORD(lParam), HIWORD(lParam));
            InvalidateRect(hWnd, &rectButton, TRUE);
            break;

        case WM_RBUTTONDOWN:
            wsprintf(ButtonText, "WM_RBUTTONDOWN: %x, %d, %d",
                wParam, LOWORD(lParam), HIWORD(lParam));
            InvalidateRect(hWnd, &rectButton, TRUE);
            break;

        case WM_RBUTTONUP:
            wsprintf(ButtonText, "WM_RBUTTONUP: %x, %d, %d",
                wParam, LOWORD(lParam), HIWORD(lParam));
            InvalidateRect(hWnd, &rectButton, TRUE);
            break;

        case WM_RBUTTONDBLCLK:
            wsprintf(ButtonText, "WM_RBUTTONDBLCLK: %x, %d, %d",
                wParam, LOWORD(lParam), HIWORD(lParam));
            InvalidateRect(hWnd, &rectButton, TRUE);
            break;

        case WM_KEYDOWN:
            wsprintf(KeyboardText, "WM_KEYDOWN: %x, %x, %x",
                wParam, LOWORD(lParam), HIWORD(lParam));
            InvalidateRect(hWnd, &rectKeyboard, TRUE);
            break;

        case WM_KEYUP:
            wsprintf(KeyboardText, "WM_KEYUP: %x, %x, %x",
                wParam, LOWORD(lParam), HIWORD(lParam));
            InvalidateRect(hWnd, &rectKeyboard, TRUE);
            break;

        case WM_CHAR:
            wsprintf(CharacterText, "WM_CHAR: %c, %x, %x",
                wParam, LOWORD(lParam), HIWORD(lParam));
            InvalidateRect(hWnd, &rectCharacter, TRUE);
            break;

        case WM_TIMER:
            wsprintf(TimerText, "WM_TIMER: %d seconds",
                nTimerCount += 5);
            InvalidateRect(hWnd, &rectTimer, TRUE);
            break;

        case WM_HSCROLL:
        case WM_VSCROLL:
            strcpy(HorzOrVertText,
                (message == WM_HSCROLL) ? "WM_HSCROLL" : "WM_VSCROLL");
            strcpy(ScrollTypeText,
                (GET_WM_HSCROLL_CODE(wParam, lParam) == SB_LINEUP) ? "SB_LINEUP" :
                (GET_WM_HSCROLL_CODE(wParam, lParam) == SB_LINEDOWN) ? "SB_LINEDOWN" :
                (GET_WM_HSCROLL_CODE(wParam, lParam) == SB_PAGEUP) ? "SB_PAGEUP" :
                (GET_WM_HSCROLL_CODE(wParam, lParam) == SB_PAGEDOWN) ? "SB_PAGEDOWN" :
                (GET_WM_HSCROLL_CODE(wParam, lParam) == SB_THUMBPOSITION) ? "SB_THUMBPOSITION" :
                (GET_WM_HSCROLL_CODE(wParam, lParam) == SB_THUMBTRACK) ? "SB_THUMBTRACK" :
                (GET_WM_HSCROLL_CODE(wParam, lParam) == SB_ENDSCROLL) ? "SB_ENDSCROLL" : "unknown");
            wsprintf(ScrollText, "%s: %s, %x, %x",
                (LPSTR)HorzOrVertText,
                (LPSTR)ScrollTypeText,
                GET_WM_HSCROLL_POS(wParam, lParam),
                (INT)GET_WM_HSCROLL_HWND(wParam, lParam));
            InvalidateRect(hWnd, &rectScroll, TRUE);
            break;

        case WM_PAINT:
            hDC = BeginPaint (hWnd, &ps);

            if (IntersectRect(&rect, &rectMouse, &ps.rcPaint))
                TextOut(hDC, rectMouse.left, rectMouse.top, 
                        MouseText, strlen(MouseText));
            if (IntersectRect(&rect, &rectButton, &ps.rcPaint))
                TextOut(hDC, rectButton.left, rectButton.top, 
                        ButtonText, strlen(ButtonText));
            if (IntersectRect(&rect, &rectKeyboard, &ps.rcPaint))
                TextOut(hDC, rectKeyboard.left, rectKeyboard.top, 
                        KeyboardText, strlen(KeyboardText));
            if (IntersectRect(&rect, &rectCharacter, &ps.rcPaint))
                TextOut(hDC, rectCharacter.left, rectCharacter.top, 
                        CharacterText, strlen(CharacterText));
            if (IntersectRect(&rect, &rectTimer, &ps.rcPaint))
                TextOut(hDC, rectTimer.left, rectTimer.top, 
                        TimerText, strlen(TimerText));
            if (IntersectRect(&rect, &rectScroll, &ps.rcPaint))
                TextOut(hDC, rectScroll.left, rectScroll.top, 
                        ScrollText, strlen(ScrollText));

            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
                    /* Bug?, SetTimer returns an int, KillTimer takes a WORD*/
            KillTimer(hWnd, (WORD)idTimer);                     /* Stops the timer */
            PostQuitMessage(0);
            break;

        default:
            return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (0);
}


/****************************************************************************

    FUNCTION: About(HWND, unsigned, WORD, LONG)

    PURPOSE:  Processes messages for "About" dialog box

    MESSAGES:

        WM_INITDIALOG - initialize dialog box
        WM_COMMAND    - Input received

****************************************************************************/

BOOL APIENTRY About(
        HWND hDlg,
        UINT message,
        UINT wParam,
        LONG lParam)
{
    switch (message) {
        case WM_INITDIALOG:
            return (TRUE);

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK) {
                EndDialog(hDlg, TRUE);
                return (TRUE);
            }
            break;
    }
    return (FALSE);
        UNREFERENCED_PARAMETER(lParam);
}
