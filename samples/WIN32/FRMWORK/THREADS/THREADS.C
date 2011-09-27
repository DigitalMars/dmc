// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   threads.c
//
//  PURPOSE:  Window and thread procedures for the 4 threads. 
//
//  FUNCTIONS:
//    WndProc            - Processes messages for the main window.
//    MsgCommand         - Handle the WM_COMMAND messages for the main window.
//    MsgCreate          - Handle the WM_CREATE message
//    MsgSize            - Handle the WM_SIZE message for the main window
//    MsgDestroy         - Handle the WM_DESTROY message by calling 
//                         PostQuitMessage().
//    CmdExit            - Handles the file exit command by calling destory 
//                         window on the main window.
//    ThreadChildWndProc - Window procedure for the child thread windows
//    MsgSizeThread      - Process the WM_SIZE message for the child windows.
//    CountThread        - Thread for counting 
//    GCDThread          - Thread for computing the Greatest Common 
//                         Demoniator for two numbers
//    PrimeThread        - Thread for computing prime numbers.
//    RectThread         - Thread for displaying random rectangles.
//
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

#include <limits.h>             // for the max value stuff
#include <math.h>               // for sqrt function
#include <stdlib.h>             // for rand function


// Main window message table definition.
MSD rgmsd[] =
{
    {WM_COMMAND, MsgCommand},
    {WM_DESTROY, MsgDestroy},
    {WM_CREATE, MsgCreate},
    {WM_CLOSE, MsgClose},
    {WM_SIZE, MsgSize}
};

MSDI msdiMain =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpWindow
};


// Main window command table definition.
CMD rgcmd[] =
{
    //
    // We'll go to the same function for all the thread
    // state functions.  Inside CmdThreadState, we'll use 
    // IDM_ to figure out which thread state we're actually
    // going to set.
    //
    {IDM_COUNT,     CmdThreadState},
    {IDM_GCD,       CmdThreadState},
    {IDM_PRIME,     CmdThreadState},
    {IDM_RECT,      CmdThreadState},
    {IDM_EXIT,      CmdExit},
    {IDM_ABOUT,     CmdAbout}
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpWindow
};


// Child window message table definition.
MSD rgmsdThreadChild[] =
{
   {WM_SIZE, MsgSizeThread}
};

MSDI msdiThreadChild =
{
    sizeof(rgmsdThreadChild) / sizeof(MSD),
    rgmsdThreadChild,
    edwpWindow
};

// Array of structures for the 4 threads.  See globals.h for more 
// information on this structure.
// 
THREADINFO ThreadInfo[4];
LONG       cyChar;                    // Character height
LONG       cxChar;                    // Character height

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispMessage() function with the main window's message dispatch
//    information (msdiMain) and the message specific information.
//

LRESULT CALLBACK WndProc(HWND   hwnd, 
                         UINT   uMessage, 
                         WPARAM wparam, 
                         LPARAM lparam)
{
    return DispMessage(&msdiMain, hwnd, uMessage, wparam, lparam);
}

//
//  FUNCTION: MsgCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_COMMAND messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - WM_COMMAND (Unused)
//    GET_WM_COMMAND_ID(wparam, lparam)   - Command identifier
//    GET_WM_COMMAND_HWND(wparam, lparam) - Control handle
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispCommand() function with the main window's command dispatch
//    information (cmdiMain) and the command specific information.
//

LRESULT MsgCommand(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&cmdiMain, hwnd, wparam, lparam);
}


//
//  FUNCTION: MsgCreate
//
//  PURPOSE: Handle the WM_CREATE message
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - WM_COMMAND (Unused)
//    GET_WM_COMMAND_ID(wparam, lparam)   - Command identifier
//    GET_WM_COMMAND_HWND(wparam, lparam) - Control handle
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispCommand() function with the main window's command dispatch
//    information (cmdiMain) and the command specific information.
//

LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    DWORD      dwThreadID;
    WNDCLASS   wc;
    int        i;
    char       szChildClass[9];
    HDC        hdc;
    TEXTMETRIC tm;
    LPTHREAD_START_ROUTINE lpfnThread[4] = 
                   {CountThread, GCDThread, PrimeThread, RectThread};
    
    // Get and store information as globals which will be used later.
    hdc = GetDC(hwnd);
    GetTextMetrics(hdc, &tm);
    cyChar = tm.tmHeight;
    cxChar = tm.tmAveCharWidth;
    ReleaseDC(hwnd, hdc);


    // Build the template for the child window class
    // Get the child window class from the resource string table
    LoadString(hInst, IDS_THREADCHILD, szChildClass, sizeof(szChildClass));

    // Set the class name and the window proc
    wc.lpszClassName = szChildClass;
    wc.style         = CS_HREDRAW | CS_VREDRAW; // Class style(s).
    wc.cbClsExtra    = 0;                       // No per-class extra data.
    wc.cbWndExtra    = 0;                       // No per-window extra data.
    wc.hInstance     = hInst;                   // Global instance
    wc.hIcon         = NULL;                    // No icon for child windows
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW); // Cursor
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Default color
    wc.lpszMenuName  = NULL;                    // No menu
    wc.lpfnWndProc   = ThreadChildWndProc;

    // Register the child windows
    RegisterClass(&wc);

    // Create the child windows
    for (i = IDC_COUNT; i <= IDC_RECT ; i++)
    {
        // Create the window and keep track of the window
        // handle in our global ThreadInfo structure.
        ThreadInfo[i].hwnd = 
              CreateWindow(szChildClass, 
                           NULL,
                           WS_CHILDWINDOW | WS_BORDER | WS_VISIBLE,
                           0, 0, 0, 0,
                           hwnd,
                           (HMENU) i,
                           hInst,
                           NULL);

        ThreadInfo[i].bThreadState  = TRUE;
        ThreadInfo[i].hThread = CreateThread (NULL,
                                              0,
                                              lpfnThread[i],
                                              NULL,
                                              CREATE_SUSPENDED,
                                              &dwThreadID);
        SetThreadPriority(ThreadInfo[i].hThread, THREAD_PRIORITY_BELOW_NORMAL);
    }
    
    return 0;
}


//  FUNCTION: MsgSize
//
//  PURPOSE: Handle the WM_SIZE message for the main window
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - WM_SIZE
//    wparam - not used
//    lparam - loword is the width, hiword the height
//
//  RETURN VALUE:
//
//  COMMENTS:
//    Call the DispCommand() function with the main window's command dispatch
//    information (cmdiMain) and the command specific information.
//
LRESULT MsgSize(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    // Count window at upper left of main window's client area
    MoveWindow(ThreadInfo[IDC_COUNT].hwnd,
               0, 
               0, 
               LOWORD(lparam) / 2, 
               HIWORD(lparam) / 2, 
               TRUE);

    // GCD window at upper right quadrant
    MoveWindow(ThreadInfo[IDC_GCD].hwnd,
               LOWORD(lparam) / 2, 
               0,
               LOWORD(lparam) / 2, 
               HIWORD(lparam) / 2,
               TRUE);

    // Prime window in lower left
    MoveWindow(ThreadInfo[IDC_PRIME].hwnd,
               0,
               HIWORD(lparam) / 2,
               LOWORD(lparam) / 2, 
               HIWORD(lparam) / 2,
               TRUE);

    // Put Rect window in lower left quandrant
    MoveWindow(ThreadInfo[IDC_RECT].hwnd,
               LOWORD(lparam) / 2, 
               HIWORD(lparam) / 2,
               LOWORD(lparam) / 2, 
               HIWORD(lparam) / 2,
               TRUE);

    return 0;
}


//  FUNCTION: MsgClose
//
//  PURPOSE: Handle the WM_CLOSE
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - WM_CLOSE
//    wparam - not used
//    lparam - not used
//
//  RETURN VALUE:
//
//  COMMENTS:
//    This is our chance to do some clean-up before we exit.
//
LRESULT MsgClose(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HANDLE lpThreadHandles[4];
    // 
    // On the close of the window we need to let the threads
    // stop processing. We can do that by setting the booleans
    // in the ThreadInfo structures to FALSE indicating that they
    // can be exited.
    // 
        
    ThreadInfo[IDC_COUNT].bThreadState = FALSE;
    ThreadInfo[IDC_GCD].bThreadState   = FALSE;
    ThreadInfo[IDC_PRIME].bThreadState = FALSE;
    ThreadInfo[IDC_RECT].bThreadState  = FALSE;

    // Make sure all threads are active before waiting on them!
    ResumeThread(ThreadInfo[IDC_COUNT].hThread);
    ResumeThread(ThreadInfo[IDC_GCD].hThread);
    ResumeThread(ThreadInfo[IDC_PRIME].hThread);
    ResumeThread(ThreadInfo[IDC_RECT].hThread);

    // Store all thread handles in an array that will be passed to
    // WaitForMultipleObjects so that we can wait until all threads
    // exit before destroying the window.
    lpThreadHandles[0] = ThreadInfo[IDC_COUNT].hThread;
    lpThreadHandles[1] = ThreadInfo[IDC_GCD].hThread;
    lpThreadHandles[2] = ThreadInfo[IDC_PRIME].hThread;
    lpThreadHandles[3] = ThreadInfo[IDC_RECT].hThread;

    // Wait until all the threads die before destroying the window.
    // This is important because these threads write to children of
    // this window.
    WaitForMultipleObjects (4, lpThreadHandles, TRUE, INFINITE);
    
    DestroyWindow(hwnd);  // All children are destroyed too.

    // Don't forget to destroy all thread handles!
    CloseHandle(ThreadInfo[IDC_COUNT].hThread);
    CloseHandle(ThreadInfo[IDC_GCD].hThread);
    CloseHandle(ThreadInfo[IDC_PRIME].hThread);
    CloseHandle(ThreadInfo[IDC_RECT].hThread);
   
    return 0;
}


//
//  FUNCTION: MsgDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handles the WM_DESTROY message
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - WM_DESTROY
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    PostQuitMessage(0);
    return 0;
}


//
//  FUNCTION: CmdThreadState(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Start or end the count thread
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - One of IDM_COUNT, IDM_GCD, IDM_PRIME, IDM_RECT.
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//
LRESULT CmdThreadState(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    UINT mfMenuCheck;
    int  nThread;
    
    //
    // Get the correct array index by subtracting the base menu command 
    // (IDM_COUNT) from wCommand.  Set mfMenuCheck to the old state of 
    // the thread state indicator.  That is, if bThread state is active
    // we're going to make it inactive so we need to uncheck the menu
    // item.  If the thread state is inactive, we're going to change it
    // to active so check to menu item.
    //

    nThread = wCommand - IDM_COUNT;

    // If the menu is already checked, then the thread should be running.  We
    // should then stop the thread and uncheck the menu.  However, if the menu
    // is unchecked, then the thread is suspended.  We should then resume the
    // thread and check the menu.
    if (GetMenuState(GetMenu(hwnd), wCommand, MF_BYCOMMAND) & MF_CHECKED)
    {   
        SuspendThread (ThreadInfo[nThread].hThread);
        mfMenuCheck = MF_UNCHECKED;
    }
    else
    {
        mfMenuCheck = MF_CHECKED;
        ResumeThread (ThreadInfo[nThread].hThread);
    }
    
    CheckMenuItem(GetMenu(hwnd), wCommand, MF_BYCOMMAND|mfMenuCheck);
    
    return 0;
}


//
//  FUNCTION: CmdExit(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_EXIT (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdExit(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    // Close the main window, and let it clean up.
    SendMessage(hwnd, WM_CLOSE, 0, 0);
    return 0;
}


//
//  FUNCTION: ThreadChildWndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for child thread proc
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//
//  COMMENTS:
//    Call the DispMessage() function with the main window's message dispatch
//    information (msdiMain) and the message specific information.
//
LRESULT CALLBACK ThreadChildWndProc(HWND   hwnd, 
                                    UINT   uMessage, 
                                    WPARAM wparam, 
                                    LPARAM lparam)
{
    return DispMessage(&msdiThreadChild, hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgSizeThread(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Process the WM_SIZE message for the child windows.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - WM_SIZE
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//
//  COMMENTS:
//    Call the DispMessage() function with the main window's message dispatch
//    information (msdiMain) and the message specific information.
//
LRESULT MsgSizeThread(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int i; 

    // Find the ThreadInfo structure that corresponds to this window and
    // then update its size.
    for (i = IDC_COUNT; i <= IDC_RECT ; i++)
    {
        if (ThreadInfo[i].hwnd == hwnd)
        {
            ThreadInfo[i].cy = HIWORD(lparam);
            ThreadInfo[i].cx = LOWORD(lparam);
        }
    }
    
    return 0;
}


//
//  FUNCTION: CountThread()
//
//  PURPOSE:  Thread for counting 
//
//  PARAMETERS:
//      void
//
//  RETURN VALUE:
//
//  COMMENTS:
//      Nothing real fancy, just incrementing through some numbers.
//
//
DWORD WINAPI CountThread(LPVOID arg)
{
    int nLine = 0;
    int nCount = 0;
    char szCount[12];
    HDC hdc;

    while (ThreadInfo[IDC_COUNT].bThreadState)
    {
        wsprintf(szCount, "%ld", nCount++);
        hdc = GetDC(ThreadInfo[IDC_COUNT].hwnd);
        TextOut(hdc, cxChar, nLine * cyChar, szCount, strlen(szCount));
        ReleaseDC(ThreadInfo[IDC_COUNT].hwnd, hdc);

        if (++nLine * cyChar > (ThreadInfo[IDC_COUNT].cy))
        {
            InvalidateRect(ThreadInfo[IDC_COUNT].hwnd, NULL, TRUE);
            UpdateWindow(ThreadInfo[IDC_COUNT].hwnd);
            nLine = 0;
        }
        if (nCount < 0)
            nCount = 0;
    }
    return 0;
}

//
//  FUNCTION: GCDThread()
//
//  PURPOSE:  Thread for computing the Greatest Common Demoniator
//            for two numbers
//
//  PARAMETERS:
//      void
//
//  RETURN VALUE:
//
//  COMMENTS:
//      Nothing real fancy
//
//
DWORD WINAPI GCDThread(LPVOID arg)
{
    int x = INT_MAX;
    int y = 1;
    int nMod;
    int nTempx;
    int nTempy;
    int nLine = 0;
    char szGCD[31];
    HDC hdc;

    while (ThreadInfo[IDC_GCD].bThreadState)
    {
        // Remember what the start values are
        nTempx = x;
        nTempy = y;
        
        //
        // Iterative loop to find GCD based on Euclid's
        // algorithm
        //
        while (nTempy)
        {
            nMod = nTempx % nTempy;
            nTempx = nTempy;
            nTempy = nMod;
        }
        
        // Show the results
        wsprintf(szGCD, "%10ld %10ld %6ld", x, y, nTempx);
        hdc = GetDC(ThreadInfo[IDC_GCD].hwnd);
        TextOut(hdc, cxChar, nLine * cyChar, szGCD, strlen(szGCD));
        ReleaseDC(ThreadInfo[IDC_GCD].hwnd, hdc);

        // If we've reached the bottom of the display start over at the top.
        if (++nLine * cyChar> (ThreadInfo[IDC_GCD].cy))
        {
            InvalidateRect(ThreadInfo[IDC_GCD].hwnd, NULL, TRUE);
            UpdateWindow(ThreadInfo[IDC_GCD].hwnd);
            nLine = 0;
        }

        // Decrement x and reset if we go below 1
        if (--x < 1)
        {
            x = 1;
        }

        // Increment y and reset if we go past INT_MAX
        if (++y > INT_MAX)
        {
            y = 1;
        }
    }
    return 0;
}


//
//  FUNCTION: PrimeThread(void *arg)
//
//  PURPOSE:  Thread for computing prime numbers.
//
//  PARAMETERS:
//      void
//
//  RETURN VALUE:
//
//  COMMENTS:
//      Nothing real fancy
//
//
DWORD WINAPI PrimeThread(LPVOID arg)
{
    int nLine = 0;          // Line number
    int nPrime = 1;         // Prime number
    int nSqrtPrime;         // The square root of the prime number
    int nDiv;
    BOOL bPrime;            // Prime flag
    char szPrime[20];       // String to hold prime number
    HDC hdc;                // HDC for TextOut

    while (ThreadInfo[IDC_PRIME].bThreadState)
    {
        // Set the sqrt root of the number as the max div we care about. 
        nSqrtPrime = (int) sqrt(nPrime);

        // Now assume that it the number is a prime number
        bPrime = TRUE;

        // Now look for possible divisors
        for (nDiv = 2; nDiv <= nSqrtPrime; nDiv++)
        {
            // If nPrime is evenly divisible then it's not a prime
            if (!(nPrime % nDiv))
            {
                bPrime = FALSE;
                break;
            }
        }

        if (bPrime)
        {
            wsprintf(szPrime, "%ld", nPrime);
            hdc = GetDC(ThreadInfo[IDC_PRIME].hwnd);
            TextOut(hdc, cxChar, nLine * cyChar, szPrime, strlen(szPrime));
            ReleaseDC(ThreadInfo[IDC_PRIME].hwnd, hdc);

            if (++nLine * cyChar> (ThreadInfo[IDC_PRIME].cy))
            {
                InvalidateRect(ThreadInfo[IDC_PRIME].hwnd, NULL, TRUE);
                UpdateWindow(ThreadInfo[IDC_PRIME].hwnd);
                nLine = 0;
            }
        }
        
        // Increment the prime number. Reset to 1 if it's negative
        if (++nPrime < 0)
        {
            nPrime = 1;
        }
    }
    return 0;
}


//
//  FUNCTION: RectThread(void *arg)
//
//  PURPOSE:  Thread for displaying random rectangles.
//
//  PARAMETERS:
//      void
//
//  RETURN VALUE:
//
//  COMMENTS:
//      Nothing real fancy
//
//
DWORD WINAPI RectThread(LPVOID arg)
{
    HDC hdc;
    HBRUSH hBrush, hOldBrush;
    int nLeft;
    int nRight;
    int nTop;
    int nBottom;
    BYTE nRed   = 0;
    BYTE nGreen = 0;
    BYTE nBlue  = 0;
    

    hdc = GetDC(ThreadInfo[IDC_RECT].hwnd);

    while (ThreadInfo[IDC_RECT].bThreadState)
    {
        //
        // Make sure the coordinates of the child
        // window are valid - we could be minimized.
        // and we don't want to do a divide if the 
        // coordinates are 0.
        //
        if (ThreadInfo[IDC_RECT].cx > 0 && ThreadInfo[IDC_RECT].cy > 0)
        {
            // Seed the coordinates for the rectangle
            nLeft   = rand() % ThreadInfo[IDC_RECT].cx;
            nRight  = rand() % ThreadInfo[IDC_RECT].cx;
            nTop    = rand() % ThreadInfo[IDC_RECT].cy;
            nBottom = rand() % ThreadInfo[IDC_RECT].cy;

            // Seed the colors for the rectangle
            nRed    = nBlue;
            nBlue   = nGreen;
            nGreen  = rand() % 255;

            // Set the brush color, draw the rectangle

            hBrush = CreateSolidBrush(RGB(nRed, nGreen, nBlue));
            hOldBrush = SelectObject(hdc, hBrush);
            Rectangle(hdc, 
                      min(nLeft, nRight),
                      min(nTop, nBottom),
                      max(nLeft, nRight),
                      max(nTop, nBottom));

            SelectObject(hdc, hOldBrush);
            DeleteObject(hBrush);
        }
    }

    ReleaseDC(ThreadInfo[IDC_RECT].hwnd, hdc);
    return 0;
}

