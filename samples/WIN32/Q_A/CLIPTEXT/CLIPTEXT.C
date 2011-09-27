
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

    PROGRAM: Cliptext.c

    PURPOSE: Demonstrates copying text to and from the clipboard

    FUNCTIONS:

        WinMain() - calls initialization function, processes message loop
        InitApplication() - initializes window data and registers window
        InitInstance() - saves instance handle and creates main window
        MainWndProc() - processes messages
        About() - processes messages for "About" dialog box
        OutOfMemory() - displays warning message

****************************************************************************/

#include "cliptext.h"
#include <string.h>

HANDLE hInst;
HANDLE hAccTable;
HWND   hwnd;

HANDLE hText = NULL;

CHAR szInitialClientAreaText[] = 
    "This program demonstrates the use of the Edit menu to copy and "
    "paste text to and from the clipboard.  Try using the Copy command " 
    "to move this text to the clipboard, and the Paste command to replace "
    "this text with data from another application.  \r\n\r\n"
    "You might want to try running Notepad and Clipbrd alongside this "
    "application so that you can watch the data exchanges take place.  ";

HANDLE hData, hClipData;                            /* handles to clip data  */
LPSTR lpData, lpClipData;                           /* pointers to clip data */

/* functions declared here, because of MIPS lack of passing C_DEFINES*/
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, INT);
LONG APIENTRY MainWndProc(HWND, UINT, UINT, LONG);
BOOL APIENTRY About(HWND, UINT, UINT, LONG);
VOID OutOfMemory(VOID);

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

    /* Only translate message if it is not an accelerator message */

        if (!TranslateAccelerator(hwnd, hAccTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg); 
        }
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
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH); 
    wc.lpszMenuName =  "CliptextMenu";
    wc.lpszClassName = "CliptextWClass";

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
    LPSTR           lpszText;

    hInst = hInstance;

    hAccTable = LoadAccelerators(hInst, "ClipTextAcc");

    if (!(hText 
          = GlobalAlloc(GMEM_MOVEABLE,(DWORD)sizeof(szInitialClientAreaText)))) {
        OutOfMemory();
        return (FALSE);
    }
      
    if (!(lpszText = GlobalLock(hText))) {
        OutOfMemory();
        return (FALSE);
    }

    strcpy(lpszText, szInitialClientAreaText);
    GlobalUnlock(hText);

    hwnd = CreateWindow(
        "CliptextWClass",
        "Cliptext Sample Application",
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

    if (!hwnd)
        return (FALSE);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    return (TRUE);

}

/****************************************************************************

    FUNCTION: MainWndProc(HWND, unsigned, WORD, LONG)

    PURPOSE:  Processes messages

    MESSAGES:

        WM_COMMAND    - message from menu
        WM_INITMENU   - initialize menu
        WM_PAINT      - update window
        WM_DESTROY    - destroy window

    COMMENTS:

        WM_INITMENU - when this message is received, the application checks
        to see if there is any text data in the clipboard, and enables or
        disables the Paste menu item accordingly.

        Seclecting the Copy menu item will send the text "Hello Windows" to
        the clipboard.

        Seclecting the Paste menu item will copy whatever text is in the
        clipboard to the application window.

****************************************************************************/

LONG APIENTRY MainWndProc(HWND hWnd, UINT message, UINT wParam, LONG lParam)
{
    FARPROC lpProcAbout;
    HDC hDC;
    PAINTSTRUCT ps;
    RECT rectClient;
    LPSTR lpszText;

    switch (message) {

        case WM_INITMENU:
                if (wParam == (UINT)GetMenu(hWnd)) {
                if (OpenClipboard(hWnd)) {
                    if (IsClipboardFormatAvailable(CF_TEXT)
                        || IsClipboardFormatAvailable(CF_OEMTEXT))
                        EnableMenuItem((HMENU)wParam, IDM_PASTE, MF_ENABLED);
                    else
                        EnableMenuItem((HMENU)wParam, IDM_PASTE, MF_GRAYED);
                    CloseClipboard();
                    return (TRUE);
                }
                else                           /* Clipboard is not available */
                    return (FALSE);

            }
            return (TRUE);

        case WM_COMMAND:
                switch(LOWORD(wParam)) {
                case IDM_ABOUT:
                    lpProcAbout = MakeProcInstance((FARPROC)About, hInst);
                    DialogBox(hInst, "AboutBox", hWnd, lpProcAbout);
                    FreeProcInstance(lpProcAbout);
                    break;

                /* file menu commands */

                case IDM_NEW:
                case IDM_OPEN:
                case IDM_SAVE:
                case IDM_SAVEAS:
                case IDM_PRINT:
                    MessageBox (
                          GetFocus ()
                        , "Command not implemented."
                        , "ClipText Sample Application"
                        , MB_ICONASTERISK | MB_OK
                    );
                    break;  

                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
    
                /* edit menu commands */

                case IDM_UNDO:
                case IDM_CLEAR:
                    MessageBox (
                          GetFocus ()
                        , "Command not implemented."
                        , "ClipText Sample Application"
                        , MB_ICONASTERISK | MB_OK
                    );
                    break;  

                case IDM_CUT:
                case IDM_COPY:

                    if (hText != NULL) {

                        /* Allocate memory and copy the string to it */

                        if (!(hData 
                             = GlobalAlloc(GMEM_DDESHARE, GlobalSize (hText)))) {
                            OutOfMemory();
                            return (TRUE);
                        }
                        if (!(lpData = GlobalLock(hData))) {
                            OutOfMemory();
                            return (TRUE);
                        }
                        if (!(lpszText = GlobalLock (hText))) {
                            OutOfMemory();
                            return (TRUE);
                        }
                                    strcpy(lpData, lpszText);
                        GlobalUnlock(hData);
                        GlobalUnlock (hText);

                        /* Clear the current contents of the clipboard, and set
                         * the data handle to the new string.
                         */

                        if (OpenClipboard(hWnd)) {
                            EmptyClipboard();
                            SetClipboardData(CF_TEXT, hData);
                            CloseClipboard();
                        }
                        hData = NULL;

                                    if (LOWORD(wParam) == IDM_CUT) {
                            GlobalFree (hText);
                            hText = NULL;
                            EnableMenuItem(GetMenu (hWnd), IDM_CUT, MF_GRAYED);
                            EnableMenuItem(GetMenu(hWnd), IDM_COPY, MF_GRAYED);
                            InvalidateRect (hWnd, NULL, TRUE);
                            UpdateWindow (hWnd);
                        }
                    }

                    return (TRUE);

                case IDM_PASTE:
                    if (OpenClipboard(hWnd)) {

                        /* get text from the clipboard */

                        if (!(hClipData = GetClipboardData(CF_TEXT))) {
                            CloseClipboard();
                            break;
                        }
                        if (hText != NULL) {
                            GlobalFree(hText);
                        }
                        if (!(hText = GlobalAlloc(GMEM_MOVEABLE
                                                    , GlobalSize(hClipData)))) {
                            OutOfMemory();
                            CloseClipboard();
                            break;
                        }
                        if (!(lpClipData = GlobalLock(hClipData))) {
                            OutOfMemory();
                            CloseClipboard();
                            break;
                        }
                        if (!(lpszText = GlobalLock(hText))) {
                            OutOfMemory();
                            CloseClipboard();
                            break;
                        }
                        strcpy(lpszText, lpClipData);
                        GlobalUnlock(hClipData);
                        CloseClipboard();
                        GlobalUnlock(hText);
                        EnableMenuItem(GetMenu(hWnd), IDM_CUT, MF_ENABLED);
                        EnableMenuItem(GetMenu(hWnd), IDM_COPY, MF_ENABLED);

                        /* copy text to the application window */

                        InvalidateRect(hWnd, NULL, TRUE);
                        UpdateWindow(hWnd);
                        return (TRUE);
                    }
                    else
                        return (FALSE);
            }
            break;

            case WM_SIZE:
                InvalidateRect(hWnd, NULL, TRUE);
                break;

            case WM_PAINT:
            hDC = BeginPaint (hWnd, &ps);
            if (hText != NULL) {
                if (!(lpszText = GlobalLock (hText))) {
                    OutOfMemory();
                } else {
                            GetClientRect (hWnd, &rectClient);
                    DrawText (hDC, lpszText, -1, &rectClient
                                , DT_EXTERNALLEADING | DT_NOPREFIX | DT_WORDBREAK);
                    GlobalUnlock (hText);
                }
            }
                EndPaint (hWnd, &ps);
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

BOOL APIENTRY About( HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    switch (message) {
            case WM_INITDIALOG:
                return (TRUE);

            case WM_COMMAND:
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

    FUNCTION: OutOfMemory(void)

    PURPOSE:  Displays warning message

****************************************************************************/
VOID OutOfMemory()
{
    MessageBox(
        GetFocus(),
        "Out of Memory",
        NULL,
        MB_ICONHAND | MB_SYSTEMMODAL);
    return;
}
