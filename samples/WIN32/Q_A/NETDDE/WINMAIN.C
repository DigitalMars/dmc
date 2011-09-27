// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
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
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application

HINSTANCE g_hInstNetDde;

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
//          In WindowsNT, this parameter is always NULL.
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
    DWORD dwVersion;
    
    // First check to see if we're running on Win95
    dwVersion = GetVersion();
    if (!(dwVersion < 0x8000000))
    {
        MessageBox(NULL, 
                   TEXT("This application cannot run on Windows 95 or Win32s"),
                   TEXT("NDdeChat"), MB_OK | MB_ICONHAND);
        return (FALSE);
    }


    // Other instances of app running?
    if (!hPrevInstance)
    {
        // Initialize shared things
        if (!InitApplication(hInstance))
        {
            return FALSE;               // Exits if unable to initialize
        }
    }

    // Attempt to load the NDDEAPI.DLL.  If this is Win95 it will fail.
    g_hInstNetDde = LoadLibrary(TEXT("NDDEAPI.DLL"));
    if (!g_hInstNetDde)
    {
        MessageBox(NULL, TEXT("Could not load NDDEAPI.DLL."), TEXT("NDDECHAT"), MB_OK);
        return (FALSE);                                                              
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
            TranslateMessage(&msg);
            DispatchMessage(&msg); 
        }
    }

    //
    // **TODO** Call module specific instance free/delete functions here.
    //
    if (g_hInstNetDde)
        FreeLibrary(g_hInstNetDde);

    // Returns the value from PostQuitMessage
    return msg.wParam;
}
