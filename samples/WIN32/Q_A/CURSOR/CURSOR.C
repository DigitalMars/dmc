
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

    PROGRAM: Cursor.c

    PURPOSE: Demonstrates how to manipulate a cursor and select a region

    FUNCTIONS:

        WinMain() - calls initialization function, processes message loop
        InitApplication() - initializes window data and registers window
        InitInstance() - saves instance handle and creates main window
        MainWndProc() - processes messages
        About() - processes messages for "About" dialog box
        sieve() - time consuming function, generates primes

****************************************************************************/

#include <windows.h>
#include <string.h>
#include "cursor.h"

HANDLE hInst;

CHAR str[255];                              /* general-purpose string buffer */

HCURSOR hSaveCursor;                        /* handle to current cursor      */
HCURSOR hHourGlass;                         /* handle to hourglass cursor    */

BOOL bTrack = FALSE;                        /* TRUE if left button clicked   */
INT OrgX = 0, OrgY = 0;                     /* original cursor position      */
INT PrevX = 0, PrevY = 0;                   /* current cursor position       */
INT X = 0, Y = 0;                           /* last cursor position          */
RECT Rect;                                  /* selection rectangle           */

MPOINT ptCursor;                            /* x and y coordinates of cursor */
INT repeat = 1;                             /* repeat count of keystroke     */

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
    MSG msg;                                 /* message                      */

    UNREFERENCED_PARAMETER( lpCmdLine );

    if (!hPrevInstance)
        if (!InitApplication(hInstance))
            return (FALSE);

    if (!InitInstance(hInstance, nCmdShow))
        return (FALSE);

    MessageBox (
          GetFocus ()
        , "Use the mouse button in this program for an example of graphics "
                        "selection, or the <Enter> key for an example of "
                        "using a special cursor to reflect a program state."
        , "Cursor Sample Application"
        , MB_ICONASTERISK | MB_OK
    );

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

    wc.style = 0;
    wc.lpfnWndProc = (WNDPROC) MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(hInstance, "bullseye");
    wc.hbrBackground = GetStockObject(WHITE_BRUSH); 
    wc.lpszMenuName =  "CursorMenu";
    wc.lpszClassName = "CursorWClass";

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

    hInst = hInstance;

    strcpy(str,"");

    hHourGlass = LoadCursor(NULL, IDC_WAIT);  

    hWnd = CreateWindow(
        "CursorWClass",
        "Cursor Sample Application",
        WS_OVERLAPPEDWINDOW,
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

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return (TRUE);

}

/****************************************************************************

    FUNCTION: MainWndProc(HWND, unsigned, WORD, LONG)

    PURPOSE:  Processes messages

    MESSAGES:

        WM_COMMAND     - application menu (About dialog box)
        WM_CHAR        - ASCII key value received
        WM_LBUTTONDOWN - left mouse button
        WM_MOUSEMOVE   - mouse movement
        WM_LBUTTONUP   - left button released
        WM_KEYDOWN     - key pressed
        WM_KEYUPS      - key released
        WM_PAINT       - update window
        WM_DESTROY     - destroy window

    COMMENTS:

        When the left mouse button is pressed, btrack is set to TRUE so that
        the code for WM_MOUSEMOVE will keep track of the mouse and update
        the box accordingly.  Once the button is released, btrack is set to
        FALSE, and the current position is saved.  Holding the SHIFT key
        while pressing the left button will extend the current box rather
        then erasing it and starting a new one.

        When an arrow key is pressed, the cursor is repositioned in the
        direction of the arrow key.  A repeat count is kept so that the
        longer the user holds down the arrow key, the faster it will move.
        As soon as the key is released, the repeat count is set to 1 for
        normal cursor movement.

****************************************************************************/

LONG APIENTRY MainWndProc(HWND hWnd, UINT message, UINT wParam, LONG lParam)
{
    FARPROC lpProcAbout;
    POINT pt;
    HDC hDC;

    switch (message) {
        case WM_COMMAND:
            // LOWORD added for portability
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

        case WM_CHAR:
            if (wParam == '\r') {
                SetCapture(hWnd);

                /* Set the cursor to an hourglass */

                hSaveCursor = SetCursor(hHourGlass);

                strcpy (str, "Calculating prime numbers...");
                InvalidateRect (hWnd, NULL, TRUE);
                UpdateWindow (hWnd);
                wsprintf(str, "Calculated %d primes.       ", sieve());
                InvalidateRect (hWnd, NULL, TRUE);
                UpdateWindow (hWnd);

                SetCursor(hSaveCursor);          /* Restores previous cursor */
                ReleaseCapture();
            }
            break;

        case WM_LBUTTONDOWN:
            bTrack = TRUE;
            strcpy (str, "");
            PrevX = LOWORD(lParam);
            PrevY = HIWORD(lParam);
            if (!(wParam & MK_SHIFT)) {       /* If shift key is not pressed */
                OrgX = LOWORD(lParam);
                OrgY = HIWORD(lParam);
            }
            InvalidateRect (hWnd, NULL, TRUE);
            UpdateWindow (hWnd);

            /* Capture all input even if the mouse goes outside of window */

            SetCapture(hWnd);
            break;

        case WM_MOUSEMOVE:
            {
                RECT        rectClient;
                INT         NextX;
                INT         NextY;

                if (bTrack) {
                    NextX = (SHORT)LOWORD(lParam);
                    NextY = (SHORT)HIWORD(lParam);

                    /* Do not draw outside the window's client area */

                    GetClientRect (hWnd, &rectClient);
                    if (NextX < rectClient.left) {
                        NextX = rectClient.left;
                    } else if (NextX >= rectClient.right) {
                        NextX = rectClient.right - 1;
                    }
                    if (NextY < rectClient.top) {
                        NextY = rectClient.top;
                    } else if (NextY >= rectClient.bottom) {
                        NextY = rectClient.bottom - 1;
                    }

                    /* If the mouse position has changed, then clear the */
                    /* previous rectangle and draw the new one.          */

                    if ((NextX != PrevX) || (NextY != PrevY)) {
                        hDC = GetDC(hWnd);
                        SetROP2(hDC, R2_NOT);          /* Erases the previous box */
                        MoveToEx(hDC, OrgX, OrgY, NULL);
                        LineTo(hDC, OrgX, PrevY);
                        LineTo(hDC, PrevX, PrevY);
                        LineTo(hDC, PrevX, OrgY);
                        LineTo(hDC, OrgX, OrgY);

                        /* Get the current mouse position */

                        PrevX = NextX;
                        PrevY = NextY;
                        MoveToEx(hDC, OrgX, OrgY, NULL); /* Draws the new box */
                        LineTo(hDC, OrgX, PrevY);
                        LineTo(hDC, PrevX, PrevY);
                        LineTo(hDC, PrevX, OrgY);
                        LineTo(hDC, OrgX, OrgY);
                        ReleaseDC(hWnd, hDC);
                    }
                }
            }
            break;

        case WM_LBUTTONUP:
            bTrack = FALSE;                 /* No longer creating a selection */
            ReleaseCapture();               /* Releases hold on mouse input */

            X = LOWORD(lParam);             /* Saves the current value      */
            Y = HIWORD(lParam);
            break;

        case WM_KEYDOWN:
            if (wParam != VK_LEFT && wParam != VK_RIGHT
                    && wParam != VK_UP && wParam != VK_DOWN)
                break;

            MPOINT2POINT(ptCursor, pt);
            GetCursorPos(&pt);

            /* Convert screen coordinates to client coordinates */

            ScreenToClient(hWnd, &pt);
            POINT2MPOINT(pt, ptCursor);
            repeat++;                           /* Increases the repeat rate */

            switch (wParam) {

            /* Adjust cursor position according to which key was pressed. */

                case VK_LEFT:
                    ptCursor.x -= repeat;
                    break;

                case VK_RIGHT:
                    ptCursor.x += repeat;
                    break;

                case VK_UP:
                    ptCursor.y -= repeat;
                    break;

                case VK_DOWN:
                    ptCursor.y += repeat;
                    break;

            }

            /* Get the client boundaries */

            GetClientRect(hWnd, &Rect);

            /* Do not draw outside the window's client area */

            MPOINT2POINT(ptCursor, pt);
            if (pt.x >= Rect.right)
                pt.x = Rect.right - 1;
            else if (pt.x < Rect.left)
                pt.x = Rect.left;
            if (pt.y >= Rect.bottom)
                pt.y = Rect.bottom - 1;
            else if (pt.y < Rect.top)
                pt.y = Rect.top;

            /* Convert the coordinates to screen coordinates */

            ClientToScreen(hWnd, &pt);
            SetCursorPos(pt.x, pt.y);
            break;

        case WM_KEYUP:
            repeat = 1;                          /* Clears the repeat count. */
            break;

        case WM_ACTIVATE:
            if (!GetSystemMetrics(SM_MOUSEPRESENT)) {
                if (!HIWORD(wParam)) {
                    // LOWORD added for portability
                    if (LOWORD(wParam)) {
                        SetCursor(LoadCursor(hInst, "bullseye"));
                        pt.x = X;
                        pt.y = Y;
                        ClientToScreen(hWnd, &pt);
                        SetCursorPos(pt.x, pt.y);
                    }
                    ShowCursor(wParam);
                }
            }
            break;

        case WM_PAINT:
            {
                PAINTSTRUCT     ps;

                hDC = BeginPaint (hWnd, &ps);
                if (OrgX != PrevX || OrgY != PrevY) {
                    MoveToEx(hDC, OrgX, OrgY, NULL);
                    LineTo(hDC, OrgX, PrevY);
                    LineTo(hDC, PrevX, PrevY);
                    LineTo(hDC, PrevX, OrgY);
                    LineTo(hDC, OrgX, OrgY);
                }
                TextOut (hDC, 1, 1, str, strlen (str));
                EndPaint (hWnd, &ps);
            }
            break;

        case WM_DESTROY:
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

BOOL APIENTRY About(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    switch (message) {
        case WM_INITDIALOG:
            return (TRUE);

        case WM_COMMAND:
            // LOWORD added for portability
            if (LOWORD(wParam) == IDOK
                || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, TRUE);
                return (TRUE);
            }
            break;
    }
    return (FALSE);
        UNREFERENCED_PARAMETER(lParam);
}


/****************************************************************************

    FUNCTION: Sieve()

    PURPOSE:  Example of time consuming process

    COMMENTS:

        Sieve of Eratosthenes, BYTE, Volume 8, Number 1, by Jim Gilbreath
        and Gary Gilbreath.  Code changed to give correct results.

        One could return the count, and after restoring the cursor, use
        sprintf() to copy the information to a string which could then be
        put up in a MessageBox().

****************************************************************************/

#define NITER   20                                 /* number of iterations */
#define SIZE    8190

CHAR flags[SIZE+1]={ 0};

INT sieve() {
    INT i,k;
    INT iter, count;

    for (iter = 1; iter <= NITER; iter++) {      /* Does sieve NITER times */
        count = 0;
        for (i = 0; i <= SIZE; i++)              /* Sets all flags TRUE    */
            flags[i] = TRUE;

        for (i = 2; i <= SIZE; i++) {
            if (flags[i] ) {                        /* Found a prime?       */
                for (k = i + i; k <= SIZE; k += i)
                    flags[k] = FALSE;              /* Cancelsits multiples */
                count++;
            }
        }
    }
    return (count);
}
