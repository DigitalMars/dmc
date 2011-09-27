// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   misc.c
//
//  PURPOSE:  Contains all helper functions "global" to the application.
//
//  FUNCTIONS:
//    CenterWindow - Center one window over another.
//    CmdStub      - Handles unimplemented commands. 
//                   Demonstrates the statusbar updating. 
//
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include "globals.h"            // prototypes specific to this application
#include "statbar.h"


//
//  FUNCTION: CenterWindow(HWND, HWND)
//
//  PURPOSE:  Center one window over another.
//
//  PARAMETERS:
//    hwndChild - The handle of the window to be centered.
//    hwndParent- The handle of the window to center on.
//
//  RETURN VALUE:
//
//    TRUE  - Success
//    FALSE - Failure
//
//  COMMENTS:
//
//    Dialog boxes take on the screen position that they were designed
//    at, which is not always appropriate. Centering the dialog over a
//    particular window usually results in a better position.
//

BOOL CenterWindow(HWND hwndChild, HWND hwndParent)
{
    RECT    rcChild, rcParent;
    int     cxChild, cyChild, cxParent, cyParent;
    int     cxScreen, cyScreen, xNew, yNew;
    HDC     hdc;

    // Get the Height and Width of the child window
    GetWindowRect(hwndChild, &rcChild);
    cxChild = rcChild.right - rcChild.left;
    cyChild = rcChild.bottom - rcChild.top;

    // Get the Height and Width of the parent window
    GetWindowRect(hwndParent, &rcParent);
    cxParent = rcParent.right - rcParent.left;
    cyParent = rcParent.bottom - rcParent.top;

    // Get the display limits
    hdc = GetDC(hwndChild);
    cxScreen = GetDeviceCaps(hdc, HORZRES);
    cyScreen = GetDeviceCaps(hdc, VERTRES);
    ReleaseDC(hwndChild, hdc);

    // Calculate new X position, then adjust for screen
    xNew = rcParent.left + ((cxParent - cxChild) / 2);
    if (xNew < 0)
    {
        xNew = 0;
    }
    else if ((xNew + cxChild) > cxScreen)
    {
        xNew = cxScreen - cxChild;
    }

    // Calculate new Y position, then adjust for screen
    yNew = rcParent.top  + ((cyParent - cyChild) / 2);
    if (yNew < 0)
    {
        yNew = 0;
    }
    else if ((yNew + cyChild) > cyScreen)
    {
        yNew = cyScreen - cyChild;
    }

    // Set it, and return
    return SetWindowPos(hwndChild,
                        NULL,
                        xNew, yNew,
                        0, 0,
                        SWP_NOSIZE | SWP_NOZORDER);
}


//
//  FUNCTION: CmdStub(HWND, WORD, WORD, HWND)
//
//  PURPOSE:  Display statusbar updates by calling UpdateStatusBar
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - Menu command ID
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//    Assumes there is a resource string describing this command with the
//    same ID as the command ID.  Loads the string and calls UpdateStatusBar
//    to put the string into main pane of the status bar.
//

LRESULT CmdStub(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl) 
{
    char szBuffer[50];   
    int  cbWritten = 0;

    cbWritten = LoadString(hInst, wCommand, szBuffer, sizeof(szBuffer)); 
    if(cbWritten == 0) 
    {
        lstrcpy(szBuffer, "Unknown Command");
        UpdateStatusBar(szBuffer, 0, 0);
    }
    else
    { 
        UpdateStatusBar(szBuffer, 0, 0);
        MessageBox (hwnd, 
                    "Command Not Implemented:\r\n Demonstration Purposes Only", 
                    "BarSDI",
                    MB_OK | MB_ICONEXCLAMATION);
    }
     /*
      * Once the command is executed, set the statusbar text to 
      * original text.
      */
    UpdateStatusBar(SZDESCRIPTION, 0, 0);

    return 0;
}

