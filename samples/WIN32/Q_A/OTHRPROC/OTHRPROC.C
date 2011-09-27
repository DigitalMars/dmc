
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*************************************************************************\
*  PROGRAM: othrproc.c
*
*  PURPOSE:
*
*    To assist in showing how to processes can share named shared memory.
*
*  GLOBAL VARIABLES:
*
*    HANDLE hInst:  - Instance handle.
*
*    HWND   hWnd:   - Client window handle.
*
*    HWND   hWndDlg:- Window handle to upper dialog box.
*
*    HANDLE hMapObject:
*                   - Handle to the file mapping object.
*
*    LONG *MapView: - Base address of the share memory.
*
*    CHAR Buf[80], Buf2[80]:
*                   - Error buffers.
*
*
*  FUNCTIONS:
*
*    WinMain()      - Initializes the window, and process the message loop.
*    MainWndProc()  - To handle the messages to the main window.
*    ReadProc()     - A thread which loops; reading the shared memory
*                     and drawing a bitmap in a location indicated by
*                     the values in the share memory.
*
*  COMMENTS:
*
*    Overview:
*      This sample is meant to be run with the Sharemem sample.  This
*      code basically reads the shared memory, and based on the values
*      found there (an X and Y coordinate representing the mouse
*      position in the window of the Sharemem sample), this code draws
*      a bitmap in the same relative position.
*
*    To Use:
*      Start the Sharemem sample first, and then this one.  Set the
*      focus to this application, and then move the mouse in the window
*      of the Sharemem application.  A bitmap will be drawn to represent
*      the relative mouse position in the other application, and the
*      mouse's X and Y coordinates will appear in the edit fields in
*      the dialog box imposed on the top of the client space.
*
\*************************************************************************/

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "othrproc.h"

#define WERR(who,where)    {sprintf(Buf,"ERROR: %s returned %u, line: %u", who, GetLastError(), __LINE__);\
                            sprintf(Buf2,"From within %s", where);\
                            MessageBox(hwnd, Buf, Buf2, MB_OK);}

CHAR Buf[80];
CHAR Buf2[80];

HANDLE hInst;
HANDLE hMapObject;
HWND   hWnd, hWndDlg;
HANDLE hThread;
LONG   *MapView;



/*************************************************************************\
*
*  FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)
*
*  PURPOSE:  To initialize the window, and process the message loop.
*
*  COMMENTS: A fairly standard WinMain except that it creates a dialog
*            box which is imposed on the upper part of the window.  This
*            dialog box is used to show the mouse coordinates.
*
\*************************************************************************/

int PASCAL WinMain ( HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR  lpCmdLine,
                     int    nCmdShow)


{

    MSG  msg;
    WNDCLASS wc;
    RECT rect;
    DWORD dwVersion = GetVersion();

    UNREFERENCED_PARAMETER( lpCmdLine );
    UNREFERENCED_PARAMETER( hPrevInstance );

    // check if running on Windows NT, if not, display notice and terminate
    if( !(dwVersion < 0x80000000) && (LOBYTE(LOWORD(dwVersion)) < 4) )
    {
        MessageBoxA(NULL,
                    "This sample application cannot run on Win32s.\n"
                    "This application will now terminate.",
                    "OthrProc",
                    MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
        return( 1 );
    }

    hInst = hInstance;

    wc.style = 0;                          // Replaces CS_SIZEREDRAW.
    wc.lpfnWndProc = (WNDPROC)MainWndProc; // The client window procedure.
    wc.cbClsExtra = 0;                     // No room reserved for extra data.
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject (WHITE_BRUSH);
    wc.lpszMenuName = "";
    wc.lpszClassName = "OthrProcWClass";

    RegisterClass(&wc);

    hWnd = CreateWindow ("OthrProcWClass",
                         "Other Procedure",
                         WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT,
                         CW_USEDEFAULT,
                         CW_USEDEFAULT,
                         CW_USEDEFAULT,
                         NULL,
                         NULL,
                         hInstance,
                         NULL);

    hWndDlg     = CreateDialog (hInst, "StatusDlg", hWnd, NULL);

    GetClientRect (hWnd, &rect);
    SendMessage (hWnd, WM_SIZE, 0, (rect.right - rect.left));
    ShowWindow  (hWndDlg,  SW_SHOW);
    ShowWindow  (hWnd, nCmdShow);

    while (GetMessage (&msg, NULL, 0, 0))
    if (!IsDialogMessage (hWndDlg, &msg))
    {
        DispatchMessage (&msg);    // Dispatch message to window.
    }

    return (msg.wParam);           // Returns value from PostQuitMessage.
}

/*************************************************************************\
*
*  FUNCTION:  MainWndProc (HWND, UINT, WPARAM, LPARAM)
*
*  PURPOSE:   To process messages.  To open and gain access to the shared
*             memory.
*
*  VARIABLES USED:
*
*    - ThreadID:
*             DWORD, used in the CreateThread call.
*
*  MESSAGES:
*
*    WM_DESTROY      - Cleans up handles and destroys the window.
*    WM_SIZE         - Sizes the dialog box to the client window.
*    WM_CREATE       - First it opens the shared memory using
*                      OpenFileMapping().  It then gets an address to
*                      the shared memory by using MapViewOfFile().
*                      If both calls were successful, this code then
*                      creates a thread which continually reads the
*                      shared memory and draws the bitmap.
*
*  CALLED BY:
*
*    WinMain();
*
*  CALLS TO:
*
*    ReadProc();
*
\*************************************************************************/

LRESULT CALLBACK MainWndProc (HWND hwnd,
                              UINT message,
                              WPARAM wParam,
                              LPARAM lParam)
{
    DWORD ThreadID;

    switch (message)
    {
        case WM_CREATE:
            hMapObject = OpenFileMapping (FILE_MAP_READ,
                                          FALSE,
                                          "shared_memory");
            if (!hMapObject)
            {
                WERR("OpenFileMapping","WM_CREATE");
                PostQuitMessage (0);
                return (0);
            }

            (LPVOID)MapView = MapViewOfFile (hMapObject,
                                             FILE_MAP_READ,
                                             0, 0, 0);
            if (!MapView)
            {
                MessageBox (hwnd, "ERROR: MapViewOfFile was not successful.",
                            "From within WM_CREATE", MB_OK);
                PostQuitMessage (0);
                return (0);
            }

            hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReadProc,
                                   NULL, 0, &ThreadID);
            if (!hThread)
            {
                WERR("CreateThread", "WM_CREATE");
                PostQuitMessage(0);
            }

            return (0);

        case WM_SIZE :
            SetWindowPos (hWndDlg, NULL, 0,0, LOWORD(lParam), DIALOGHEIGHT, 0);
            return (0);

        case WM_DESTROY :
            CloseHandle (hMapObject);
            CloseHandle (hThread);
            PostQuitMessage (0);
            return (0);

    }
    return DefWindowProc (hwnd, message, wParam, lParam);
}

/*************************************************************************\
*
*  FUNCTION:  VOID ReadProc (VOID)
*
*  PURPOSE:   A thread which continually reads the shared memory for new
*             X and Y coordinates, and then draws the bitmap where indicated.
*
*  VARIABLES USED:
*
*    - hCrossHair:
*             A handle to the bitmap.
*
*    - lParam, oldParam:
*             LONGs used to hold the new and the old mouse coordinates.
*
*    - hDC, hDCMem:
*             The DC of the window, and a memory DC used as the source
*             of the bitmap.
*
*    - X, Y:  Integers representing the X and Y coordinates taken from
*             the shared memory and used to locate the bitmap.
*
*    - hBrush:
*             Handle to a brush, used to erase the old bitmap before
*             redrawing it in the new location.
*
*    - rect:  A RECT, used to blot out the old bitmap (erasing it);
*
*    - bm:    a BITMAP structure for the bitmap.
*
*    - ptSize, ptOrg:
*             POINTS used in transforming the coordinates for the bitmap.
*
*  COMMENTS:
*
*    This function first uses structured exception handling to try to
*    read the shared memory a first time.  If successful, it's assumed
*    that the rest of the read will not cause an exception.  The code then
*    sets a brush for erasing the bitmap, and loads the bitmap.  It then
*    begins to loop.  Each loop reads the memory.  If the value has
*    changed since the last read the old bitmap is erased by drawing a
*    rectangle over the old coordinates, and then the bitmap is moved
*    and redrawn using the new coordinates.
*
\*************************************************************************/
VOID ReadProc(VOID)
{
    HBITMAP hCrossHair;
    LONG lParam, oldParam = 0;
    HDC hDC, hDCMem;
    int X, Y;
    HANDLE hBrush;
    RECT Rect;
    BITMAP bm;
    POINT ptSize, ptOrg;

    _try
    {
        lParam = MapView[0];
    }
    _except(EXCEPTION_EXECUTE_HANDLER)
    {
        MessageBox(hWnd, "Trying to read shared memory caused an exception.",
                   "From within WM_MOUSEMOVE", MB_OK);
        PostMessage(hWnd, WM_DESTROY, 0, 0);
    }

    hBrush = GetStockObject(WHITE_BRUSH);
    X = LOWORD(lParam);
    Y = HIWORD(lParam);

    hCrossHair = LoadBitmap(hInst, "CrossHair");

    while (1)
    {
        lParam = MapView[0];

        if (lParam != oldParam)
        {
            hDC = GetDC(hWnd);

            Rect.top = Y; Rect.left = X; Rect.bottom = Y+16; Rect.right = X+16;
            FillRect(hDC, &Rect, hBrush);

            X = LOWORD(lParam);
            Y = HIWORD(lParam);

            hDCMem = CreateCompatibleDC(hDC);
            SelectObject(hDCMem, hCrossHair);
            SetMapMode(hDCMem, GetMapMode(hDC));

            GetObject(hCrossHair, sizeof(BITMAP), &bm);

            ptSize.x = bm.bmWidth;
            ptSize.y = bm.bmHeight;
            DPtoLP (hDC, &ptSize, 1);

            ptOrg.x = 0;
            ptOrg.y = 0;
            DPtoLP (hDCMem, &ptOrg, 1);

            BitBlt(hDC, X, Y, ptSize.x, ptSize.y,
                   hDCMem, ptOrg.x, ptOrg.y, SRCCOPY);

            oldParam = lParam;
            SetDlgItemInt (hWndDlg, IDE_XCOORD, X, FALSE);
            SetDlgItemInt (hWndDlg, IDE_YCOORD, Y, FALSE);
            ReleaseDC(hWnd, hDC);
            DeleteDC(hDCMem);

            Sleep(100);
        }
    }
}
