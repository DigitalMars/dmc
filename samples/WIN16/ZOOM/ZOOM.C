/**
* ZOOM.C      - Symantec Zoomer
*
*
* Description - Main module for the ZOOM.EXE utility
*
*
* (C)Copyright 1993 by Brian Weed for Symantec Corporation
* All rights reserved.
*
*
* This example will capture an image around the mouse cursor (in real time),
* and copy that image to this window.  The image will be scaled acording to
* the zoom factor which can be controlled by a scroll bar.  This is a usefull
* tool for zooming in to look at graphics.
*
*
* Version    Date       By            Comments
* -------    --------   ------------  -------------------------------------
*  1.0a      05-24-93   Brian Weed    Initial design
*            06-23-93   Brian Weed    Added "Always on Top" feature
*
**/

#include <windows.h>

/* prototypes */
int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
long FAR PASCAL _export WndProc(HWND, UINT, UINT, LONG);
BOOL FAR PASCAL AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

#define ZFMIN    1      /* zoom factor minimum (cannot be less than 1) */
#define ZFMAX   12      /* zoom factor maximum = number of scroll bar levels */
#define ZFINIT   3      /* initial zoom factor */
#define ZABOUT 101      /* about menu item ID */
#define ZONTOP 102      /* 'always on top' menu item ID */
                        
/* globals */
HANDLE hInst;

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    static char szAppName[] = "Zoomer";
    WNDCLASS wndclass;
    HWND hwnd;
    MSG msg;

    /*
    * Only allow 1 instance of this to be loaded at a time
    */
    if (hPrevInstance) {

        /*
        * since it is already loaded, just activate it, and bring it to the top
        * but don't start a new one
        */
        SetActiveWindow(FindWindow(szAppName, szAppName));

    /*
    * otherwise, this is the first instance
    */
    } else {

        /*
        * fill in the window class info
        */
        wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNWINDOW;
        wndclass.lpfnWndProc = WndProc;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.hInstance = hInstance;
        wndclass.hIcon = LoadIcon(hInstance, "ICO_ZOOM");
        wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndclass.hbrBackground = GetStockObject(WHITE_BRUSH);
        wndclass.lpszMenuName = NULL;
        wndclass.lpszClassName = szAppName;

        RegisterClass(&wndclass);

        ShowWindow(hwnd = CreateWindow(szAppName, szAppName, WS_VSCROLL | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME, CW_USEDEFAULT, CW_USEDEFAULT, 160, 140, NULL, NULL, hInstance, NULL), nCmdShow);
        UpdateWindow(hwnd);

        /* keep a global copy of my instance handle for the WndProc */
        hInst = hInstance;

        /*
        * main message loop - continuously check for messages
        */
        while (1) {

            /*
            * if there is a message for our window, then process it
            */
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

                if (msg.message == WM_QUIT)
                    break;

                TranslateMessage(&msg);
                DispatchMessage(&msg);

            } else {

                /*
                * if Zoomer is not minimized then keep sending WM_PAINT
                * messages to keep updating the zoomed image
                */
                if (!IsIconic(hwnd))
                    InvalidateRect(hwnd, NULL, TRUE);
            }
        }
    }

    return(0);
}


long FAR PASCAL _export WndProc (HWND hwnd, UINT message, UINT wParam, LONG lParam)
{
    static short length, height;
    static POINT old;
    static int factor;
    static HDC hdc;
    static BOOL ontop;

    HWND hWndDesktop;
    HDC hdcDesktop;
    POINT cursor;
    HMENU hmenu;
    int yMin, yMax;

    switch (message) {

        /*
        * on Initialization
        */
        case WM_CREATE:

            /* keep a static copy of our Device Context */
            hdc = GetDC(hwnd);

            /* set initial zoom factor */
            factor = ZFINIT;

            /* initial position of "Always on top" menu item is UNCHECKED */
            ontop = FALSE;

            /* set initial scroll bar thumb position */
            GetScrollRange(hwnd, SB_VERT, &yMin, &yMax);
            SetScrollPos(hwnd, SB_VERT, (factor-ZFMIN)*yMax/(ZFMAX-ZFMIN), TRUE);

            /*
            * get starting window size
            */
            length = ((CREATESTRUCT FAR *)lParam)->cx;
            height = ((CREATESTRUCT FAR *)lParam)->cy;

            /*
            * add the some private menu items to the System Menu
            */
            hmenu = GetSystemMenu(hwnd, FALSE);

            /* add the "always on top" menu item */
            AppendMenu(hmenu, MF_SEPARATOR, 0, (LPSTR) NULL);
            AppendMenu(hmenu, MF_STRING, ZONTOP, "Always on &Top");

            /* add the "about box " menu item */
            AppendMenu(hmenu, MF_SEPARATOR, 0, (LPSTR) NULL);
            AppendMenu(hmenu, MF_STRING, ZABOUT, "&About...");
            return(0);

        /*
        * user clicked on a system menu item
        */
        case WM_SYSCOMMAND:

            switch (wParam) {

                /*
                * user clicked on About... menu item
                */
                case ZABOUT:
                    DialogBox(hInst, "ABOUT", hwnd, (FARPROC)AboutDlgProc);
                    break;

                /*
                * toggle the 'Always On Top'
                */
                case ZONTOP:

                    /* toggle */
                    ontop = ~ontop;

                    /* check/uncheck the menu item */
                    CheckMenuItem(GetSystemMenu(hwnd, FALSE), ZONTOP, MF_BYCOMMAND | (ontop ? MF_CHECKED : MF_UNCHECKED));

                    /*
                    * toggle 'topmost' attribute of this window
                    */
                    SetWindowPos(hwnd, (ontop ? HWND_TOPMOST : HWND_NOTOPMOST), 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
                    break;
            }
            break;

        /*
        * user has resized the window
        */
        case WM_SIZE:

            /*
            * get new window coordinates
            */
            length = LOWORD(lParam);
            height = HIWORD(lParam);

            return(0);

        /*
        * scroll bar determines zoom factor
        */
        case WM_VSCROLL:

            /* get current scroll range */
            GetScrollRange(hwnd, SB_VERT, &yMin, &yMax);

            switch (wParam) {

                /*
                * scroll bar is at bottom (mimimum zoom)
                */
                case SB_BOTTOM:
                    factor = ZFMIN;
                    break;

                /*
                * scroll bar is at the top (maximum zoom)
                */
                case SB_TOP:
                    factor = ZFMAX;
                    break;

                /*
                * decrease the zoom factor by 1 (ie. 2:1 -> 1:1)
                */
                case SB_LINEUP:
                case SB_PAGEUP:
                    if (--factor < ZFMIN)
                        factor = ZFMIN;
                    break;

                /*
                * increase the zoom factor by 1
                */
                case SB_LINEDOWN:
                case SB_PAGEDOWN:
                    if (++factor > ZFMAX)
                        factor = ZFMAX;
                    break;

                /*
                * user has dragged scroll bar thumb so calculate new
                * thumb position and zoom factor
                */
                case SB_THUMBTRACK:
                case SB_THUMBPOSITION:
                    factor = (LOWORD(lParam)/(yMax/(ZFMAX-ZFMIN))) + ZFMIN;
                    break;
            }

            /* set thumbnail at the new location */
            SetScrollPos(hwnd, SB_VERT, (factor-ZFMIN)*yMax/(ZFMAX-ZFMIN), TRUE);
            return(0);

        /*
        * show captured image in client area (stretch if needed)
        */
        case WM_PAINT:

            /* get current cursor position */
            GetCursorPos(&cursor);

            /*
            * don't update the image if the cursor has not moved.
            * this also helps keep the cursor from flickering when it is still.
            */
            if (cursor.x != old.x || cursor.y != old.y) {

                /* store current cursor position */
                old.x = cursor.x;
                old.y = cursor.y;

                /* get the DC for the current desktop */
                hdcDesktop = GetDC(hWndDesktop = GetDesktopWindow());

                /*
                * copy the image directly from one screen location into our window
                * stretching if necessary (proportional stretching is important).
                */
                StretchBlt(hdc, 0, 0, length, height, hdcDesktop, cursor.x - length/(2*factor), cursor.y - height/(2*factor), length/factor, height/factor, SRCCOPY);

                /* free the desktop DC */
                ReleaseDC(hWndDesktop, hdcDesktop);
            }

            /* remove the WM_PAINT message from the queue */
            ValidateRect(hwnd, NULL);

            return(0);

        /*
        * close window
        */
        case WM_DESTROY:
            PostQuitMessage (0) ;
            return(0);
    }

    /* handle all other messages with defaults */
    return(DefWindowProc(hwnd, message, wParam, lParam));
}

/*
* handle imput during About box
*/
BOOL FAR PASCAL AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {

    case WM_INITDIALOG:
        return(TRUE);

    case WM_COMMAND:
        EndDialog(hDlg, TRUE);
        return(TRUE);
    }

    return(FALSE);
}
