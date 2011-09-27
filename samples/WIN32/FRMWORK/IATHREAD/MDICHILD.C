// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   mdichild.c
//
//  PURPOSE:
//    To implement the basic mdi child commands.
//
//  FUNCTIONS:
//    ChildThreadProc    - Performs work for each MDI child window.
//    MDIChildWndProc    - Processes messages for MDI child windows.
//    MsgMCCommand       - Handle the WM_COMMAND messages for MDI children.
//    MsgMCCreate        - Initializes new MDI child window.
//    MsgMCDestroy       - Destroys MDI child window and frees its resources.
//    MsgMCMdiActiveate  - Changes menus depending on which child is active.
//    CmdMCThreadSuspend - Suspends active MDI child's worker thread.
//    CmdMCThreadResume  - Resumes active MDI child's worker thread.
//
//  COMMENTS:
//


#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include "globals.h"            // prototypes specific to this application
#include <stdlib.h>
#include "resource.h"

// Child window message table definition.
static MSD rgmsd[] =
{
    {WM_COMMAND,     MsgMCCommand},
    {WM_CREATE,      MsgMCCreate},
    {WM_MDIACTIVATE, MsgMCMdiActivate},
    {WM_DESTROY,     MsgMCDestroy}
};

static MSDI msdiChild =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpMDIChild
};


// Child window command table definition.
static CMD rgcmd[] =
{
    {IDM_THREAD_PRIORITY,   CmdMCThreadPriority},
    {IDM_THREAD_SUSPEND,    CmdMCThreadSuspend},
    {IDM_THREAD_RESUME,     CmdMCThreadResume}
};

static CMDI cmdiChild =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpMDIChild
};


//
//  FUNCTION: ChildThreadProc(LPDWORD)
//
//  PURPOSE:  To perform work for each MDI child window.
//
//  PARAMETERS:
//
//
//  RETURN VALUE:
//    Always returns 0.
//
//  COMMENTS:
//    This function executes in one thread for each MDI child window.
//
//

DWORD WINAPI ChildThreadProc (LPDWORD lpData)
{
    PTHREAD_DATA pThreadData = (PTHREAD_DATA)lpData;
    HWND         hWnd        = NULL;
    HDC          hDrawDC     = NULL;
    RECT         rClientRect = {0};
    HBRUSH       hRectBrush  = NULL;
    HBRUSH       hOldBrush   = NULL;
    COLORREF     crRectColor = 0;
    int          nRed, nGreen, nBlue;
    int          nLeft, nTop, nRight, nBottom;


    nRed = nGreen = nBlue = 0;
    nLeft = nTop = nRight = nBottom = 0;

    while (TRUE)
    {
        EnterCriticalSection(&(pThreadData -> csCritSec));

        if (pThreadData -> bKill == TRUE)
        {
            // Be sure to eave critical section before exiting!
            LeaveCriticalSection (&(pThreadData -> csCritSec));
            ExitThread(0);
        }

        // Check timeout counter and reduce this thread's priority and
        // the process's priority class if needed.  This prevents this
        // thread/process from hogging the system.
        ReducePriority (pThreadData);


        // Draw rectangles in MDI child window only if it's client
        // area is at least 1 pixel wide and 1 pixel tall.
        hWnd = pThreadData -> hWindow;
        hDrawDC = GetDC(hWnd);
        GetClientRect(hWnd, &rClientRect);

        if ((rClientRect.right != 0) && (rClientRect.bottom != 0))
        {
            // Pick a random pure color for the rectangle.
            // Below is a more-optimized version of this:
            //    nRed   = rand() % 0xFF;
            //    nGreen = rand() % 0xFF;
            //    nBlue  = rand() % 0xFF;

            nRed   = nGreen;
            nGreen = nBlue;
            nBlue  = rand() % 0xFF;

            crRectColor = RGB(nRed, nGreen, nBlue);

            hRectBrush = CreateSolidBrush(
                                          GetNearestColor(hDrawDC,
                                                          crRectColor)
                                         );
            hOldBrush = SelectObject(hDrawDC, hRectBrush);

            // Draw the rectangle
            nLeft   = rand() % rClientRect.right;
            nTop    = rand() % rClientRect.bottom;
            nRight  = nLeft + 20;
            nBottom = nTop + 20;
            Rectangle(hDrawDC, nLeft, nTop, nRight, nBottom);

            // Clean up all of the resources used to draw rectangle.
            SelectObject(hDrawDC, hOldBrush);
            ReleaseDC(hWnd, hDrawDC);
            DeleteObject(hRectBrush);

            // Be sure to leave the critical section before exiting
            // the while loop!
            LeaveCriticalSection(&(pThreadData -> csCritSec));
        }
        else
        {
            // Go to sleep temporarily while MDI child is minimized or has no
            // client area.

            // Leave the critical section before sleeping.  No need to
            // hold it while sleeping.  Note that we also have to leave the
            // critical section at the end of the if() clause above.
            LeaveCriticalSection(&(pThreadData -> csCritSec));

            Sleep(1000);
        }
    }
    return 0;
}



//
//  FUNCTION: MDIChildWndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for MDI child windows.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (depends on message)
//    lparam   - additional information (depends on message)
//
//  RETURN VALUE:
//    Depends on the message number.
//
//  COMMENTS:
//    Dispatch all messages sent to MDI child windows.
//

LRESULT CALLBACK MDIChildWndProc(HWND   hwnd,
                                 UINT   uMessage,
                                 WPARAM wparam,
                                 LPARAM lparam)
{
    return DispMessage( &msdiChild, hwnd, uMessage, wparam, lparam );
}


//
//  FUNCTION: MsgMCCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_COMMAND messages for MDI children.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - WM_COMMAND (Unused)
//    wparam   - Contains command-specific information
//    lparam   - Contains command-specific information
//
//  RETURN VALUE:
//
//  COMMENTS:
//    Dispatch all commands sent to MDI child windows.
//

LRESULT MsgMCCommand(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&cmdiChild, hwnd, wparam, lparam);
}


//
//  FUNCTION: MsgMCCreate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Initializes new child window and starts its worker thread.
//
//  PARAMETERS:
//    hwnd     - The window being created.
//    uMessage - WM_CREATE             (unused).
//    wparam   - Message specific data (unused).
//    lparam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//

LRESULT MsgMCCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    PTHREAD_DATA  pThreadData = NULL;
    HANDLE        hThread     = NULL;
    DWORD         dwThreadID  = 0;


    // Allocate thread data block.  If allocation fails, force window
    // creation failure.
    pThreadData = AllocThreadData();
    if (pThreadData == NULL)
        return -1;


    // Create the child window thread suspended.  This will ensure that
    // the new thread doesn't try to access any of the members of the
    // thread data before all members have been initialized.
    // If thread couldn't be created, clean up critical section, allocated
    // memory, and force window creation failure.
    hThread = CreateThread(NULL,
                           0,
                           ChildThreadProc,
                           (LPVOID)pThreadData,
                           CREATE_SUSPENDED,
                           &dwThreadID);
    if (hThread == NULL)
    {
        DeleteCriticalSection(&(pThreadData -> csCritSec));
        FreeThreadData(pThreadData);
        return -1;
    }

    // Initialize all thread block members
    InitializeCriticalSection(&(pThreadData -> csCritSec));
    pThreadData -> hWindow    = hwnd;
    pThreadData -> bKill      = FALSE;
    pThreadData -> hThread    = hThread;
    pThreadData -> dwThreadID = dwThreadID;
    pThreadData -> nPriorityTimer = -1;     // Timer initially deactivated

    // Store a pointer to the thread data block in the extra bytes for
    // convenient access.
    SetWindowLong(hwnd, GWL_USERDATA, (long)pThreadData);

    ResumeThread(hThread);

    gcOpenChildWnds++;

    return 0;
}


//
//  FUNCTION: MsgMCDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To handle the WM_DESTROY message sent to MDI child windows.
//
//  PARAMETERS:
//    hwnd     - The window being destroyed.
//    uMessage - WM_DESTROY            (unused).
//    wparam   - Message specific data (unused).
//    lparam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//

LRESULT MsgMCDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    PTHREAD_DATA pThreadData = NULL;
    HANDLE       hThread     = NULL;

    // Get thread data block associated with this child window
    pThreadData = (PTHREAD_DATA)GetWindowLong(hwnd, GWL_USERDATA);

    if (pThreadData != NULL)  // Should never be NULL. ASSERT better!
    {
        EnterCriticalSection (&(pThreadData -> csCritSec));

        // Tell thread that it is time to exit
        pThreadData -> bKill = TRUE;
        hThread = pThreadData -> hThread;

        LeaveCriticalSection(&(pThreadData -> csCritSec));

        // Wait for thread to exit before closing its handle and deleting
        // the thread data block.
        WaitForSingleObject(hThread, INFINITE);

        CloseHandle(hThread);
        DeleteCriticalSection(&(pThreadData -> csCritSec));
        FreeThreadData(pThreadData);
    }

    // Decrement number of open MDI child windows.
    gcOpenChildWnds--;

    return 0;
}


//
//  FUNCTION: MsgMCMdiActivate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  To change the menu on the frame window so that it is the right
//            one for the MDI child window.
//
//  PARAMETERS:
//    hwnd     - The window handing the message.
//    uMessage - WM_MDIACTIVATE        (unused).
//    wparam   - The MDI child window becoming inactive (unused).
//    lparam   - The MDI child window becoming active.
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//    If the window becoming active is an MDI chil window, set the menu to
//    document menu; otherwise, set it to the initial frame menu.
//

LRESULT MsgMCMdiActivate(HWND   hwnd,
                         UINT   uMessage,
                         WPARAM wparam,
                         LPARAM lparam)
{
    HWND  hActiveWnd     = (HWND)lparam;     // Window becoming active
    HMENU hNewMenu       = NULL;
    HMENU hNewWindowMenu = NULL;

    // If this window is becoming active, set the menu to document window;
    // otherwise set it to the initial frame window.
    if (hwnd == hActiveWnd)
    {
        hNewMenu       = ghDocMenu;
        hNewWindowMenu = GetSubMenu (ghDocMenu, DOCWINDOWMENU);
    }
    else
    {
        hNewMenu       = ghInitMenu;
        hNewWindowMenu = GetSubMenu (ghInitMenu, INITWINDOWMENU);
    }
    SendMessage (ghwndMDIClient,
                 WM_MDISETMENU,
                 (WPARAM)hNewMenu,
                 (LPARAM)hNewWindowMenu);
    DrawMenuBar (ghwndFrame);

    return 0;
}


//
//  FUNCTION: CmdMCThreadSuspend(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To suspend the thread associated with a given child window.
//
//  PARAMETERS:
//    hwnd      - The window handling the command.
//    wCommand  - IDM_THREAD_SUSPEND  (unused).
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL                (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//

LRESULT CmdMCThreadSuspend(HWND hwnd,
                           WORD wCommand,
                           WORD wNotify,
                           HWND hwndCtrl)
{
    PTHREAD_DATA pThreadData = NULL;


    pThreadData = (PTHREAD_DATA)GetWindowLong(hwnd, GWL_USERDATA);

    if (pThreadData != NULL)      // Should never be NULL  ASSERT better
    {
        EnterCriticalSection(&(pThreadData -> csCritSec));

        // Call SuspendThread inside critical section to make sure thread
        // is not in the critical section.  Otherwise, a deadlock will occur
        // if you suspend the thread while it is in the critical section and
        // try to claim the critical section before the thread is resumed.
        SuspendThread(pThreadData -> hThread);

        LeaveCriticalSection(&(pThreadData -> csCritSec));
    }
    return 0;
}


//
//  FUNCTION: CmdMCThreadResume(HWND, WORD, WORD, HWND)
//
//  PURPOSE: To resume the thread associated with a given child window.
//
//  PARAMETERS:
//    hwnd      - The window handling the command.
//    wCommand  - IDM_THREAD_RESUME   (unused).
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL                (unused).
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdMCThreadResume(HWND hwnd,
                          WORD wCommand,
                          WORD wNotify,
                          HWND hwndCtrl)
{
    PTHREAD_DATA pThreadData   = NULL;
    HANDLE       hThread       = NULL;


    pThreadData = (PTHREAD_DATA)GetWindowLong(hwnd, GWL_USERDATA);

    if (pThreadData != NULL)      // Should never be NULL  ASSERT better
    {
        // Get thread's handle.  Use critical section in case thread
        // is not suspended.  Suspended thread won't be in critical section
        // because we suspended it while we were in the critical section.
        EnterCriticalSection(&(pThreadData -> csCritSec));
        hThread = pThreadData -> hThread;
        LeaveCriticalSection(&(pThreadData -> csCritSec));

        ResumeThread (hThread);
    }
    return 0;
}

