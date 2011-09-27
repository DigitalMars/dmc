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
//  PLATFORMS: Windows 95, Windows NT, Win3.1
//
//  FUNCTIONS:
//    WinMain() - calls initialization functions, processes message loop
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application

//
//  FUNCTION: WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//
//  PURPOSE: calls initialization function, processes message loop
//
//  PARAMETERS:
//
//    hInstance - The handle to the instance of this application that
//          is currently being executed.
//
//    hPrevInstance - The handle to the instance of this application
//          that was last executed.  If this is the only instance
//          of this application executing, hPrevInstance is NULL.
//          In Win32 applications, this parameter is always NULL.
//
//    lpCmdLine - A pointer to a null terminated string specifying the
//          command line of the application.
//
//    nCmdShow - Specifies how the main window is to be diplayed.
//
//  RETURN VALUE:
//    If the function terminates before entering the message loop,
//      return FALSE.
//    Otherwise, return the WPARAM value sent by the WM_QUIT message.
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
//    returns the conventional value NULL.
//

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance, 
                     LPSTR     lpCmdLine, 
                     int       nCmdShow)
{
    MSG msg;
    HANDLE hAccelTable;

    // Other instances of app running?
    // Note that this is really only meaningful under Win16.
    // In Win32, hPrevInstance will always be 0.
    if (!hPrevInstance)
    {
        // Initialize shared things
        if (!InitApplication(hInstance))
        {
            return FALSE;               // Exits if unable to initialize
        }
    }

    // Perform initializations that apply to a specific instance
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, szAppName);

    // Acquire and dispatch messages until a WM_QUIT message is received.
    while (GetMessage(&msg, NULL, 0, 0))
    {
        //
        // **TODO** Add other Translation functions (for modeless dialogs
        //  and/or MDI windows) here.
        //

        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);// Translates virtual key codes
            DispatchMessage(&msg); // Dispatches message to window
        }
    }

    //
    // **TODO** Call module specific instance free/delete functions here.
    //

    // Returns the value from PostQuitMessage
    return msg.wParam;
}
