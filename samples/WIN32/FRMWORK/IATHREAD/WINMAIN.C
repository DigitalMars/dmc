// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   winmain.c
//
//  PURPOSE:   Calls initialization functions and processes the message loop
//
//
//  PLATFORMS: Windows 95, Windows NT
//
//  FUNCTIONS:
//    WinMain() - calls initialization functions, processes message loop
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

//-------------------------------------------------------------------------
// Global variables

HINSTANCE hInst          = NULL;    // Current instance's handle
HWND      ghwndFrame     = NULL;    // Application's main frame window
HWND      ghwndMDIClient = NULL;    // The MDI client window handle.
HMENU     ghInitMenu     = NULL;    // Initial menu for frame window
HMENU     ghDocMenu      = NULL;    // Document menu for frame window


//
//  FUNCTION: WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//
//  PURPOSE: calls initialization function, processes message loop
//
//  PARAMETERS:
//
//    hInstance     - The handle to the instance of this application that
//                    is currently being executed.
//    hPrevInstance - This parameter is always NULL.
//    lpCmdLine     - A pointer to a null terminated string specifying the
//                    command line of the application.
//    nCmdShow      - Specifies how the main window is to be diplayed.
//
//  RETURN VALUE:
//    If the function terminates before entering the message loop,
//    return FALSE. Otherwise, return the WPARAM value sent by the WM_QUIT 
//    message.
//
//
//  COMMENTS:
//
//    Windows recognizes this function by name as the initial entry point
//    for the program.  This function calls the application initialization
//    routine, if no other instance of the program is running, and always
//    calls the instance initialization routine.  It then executes a
//    message retrieval and dispatch loop that is the top-level control
//    structure for the remainder of execution.  The loop is terminated
//    when a WM_QUIT  message is received, at which time this function
//    exits the application instance by returning the value passed by
//    PostQuitMessage().
//
//    If this function must abort before entering the message loop, it
//    returns the conventional value FALSE.
//

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance, 
                   LPSTR     lpCmdLine, 
                   int       nCmdShow)
{
    MSG    msg;

    // Save the instance handle in a global variable, which may be handy
    // in the application later.
    hInst = hInstance;


    // Load resources that will be used later        
    ghInitMenu  = LoadMenu (hInstance, MAKEINTRESOURCE(IDM_INITMENU));
    ghDocMenu   = LoadMenu (hInstance, MAKEINTRESOURCE(IDM_DOCMENU));


    // Initialize application by setting up the main window and 
    // registering the MDI child window class.
    ghwndFrame = InitFrameWindow(hInstance, nCmdShow, ghInitMenu);
    if (ghwndFrame == NULL)
        return FALSE;

    if (!RegisterMDIChildClass(hInstance))
        return FALSE;


    // Acquire and dispatch messages until a WM_QUIT message is received.
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateMDISysAccel(ghwndMDIClient, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg); 
        }
    }

    // Returns the value from PostQuitMessage
    return msg.wParam;
}
