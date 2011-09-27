// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   init.c
//
//  PURPOSE:   Performs application and instance specific initialization.
//
//  FUNCTIONS:
//    InitApplication() - Initializes window data and registers window.
//    InitInstance() - Saves instance handle and creates main window.
//
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

HINSTANCE hInst;                // current instance
HWND      ghwnd;                // Main window handle.

char szAppName[9];              // The name of this application
char szTitle[40];               // The title bar text

//
//  FUNCTION: InitApplication(HINSTANCE)
//
//  PURPOSE: Initializes window data and registers window class.
//
//  PARAMETERS:
//    hInstance - The handle to the instance of this application that
//          is currently being executed.
//
//  RETURN VALUE:
//    TRUE - Success
//    FALSE - Initialization failed
//
//  COMMENTS:
//
//    This function is called at initialization time only if no other
//    instances of the application are running.  This function performs
//    initialization tasks that can be done once for any number of running
//    instances.
//
//    In this case, we initialize a window class by filling out a data
//    structure of type WNDCLASS and calling the Windows RegisterClass()
//    function.  Since all instances of this application use the same
//    window class, we only need to do this when the first instance is
//    initialized.
//

BOOL InitApplication(HINSTANCE hInstance)
{
    WNDCLASSEX wc;

    // Load the application name and description strings.

    LoadString(hInstance, IDS_APPNAME, szAppName, sizeof(szAppName));
    LoadString(hInstance, IDS_DESCRIPTION, szTitle, sizeof(szTitle));

    // Fill in window class structure with parameters that describe the
    // main window.

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW; // Class style(s).
    wc.lpfnWndProc   = (WNDPROC)WndProc;        // Window Procedure
    wc.cbClsExtra    = 0;                       // No per-class extra data.
    wc.cbWndExtra    = 0;                       // No per-window extra data.
    wc.hInstance     = hInstance;               // Owner of this class
    wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON)); // Icon name from .RC
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW); // Cursor
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Default color
    wc.lpszMenuName  = szAppName;               // Menu name from .RC
    wc.lpszClassName = szAppName;               // Name to register as
	wc.hIconSm       = LoadImage(hInstance,		// Load small icon image
	                             MAKEINTRESOURCE(IDI_APPICON),
	                             IMAGE_ICON,
	                             16, 16,
	                             0);

    // Register the window class and return FALSE if unsuccesful.

    if (!RegisterClassEx(&wc))
    {
        //Assume we are running on NT where RegisterClassEx() is
        //not implemented, so let's try calling RegisterClass().

        if (!RegisterClass((LPWNDCLASS)&wc.style))
        	return FALSE;
    }
    //
    // **TODO** Call module specific application initialization functions here.
    //

    return TRUE;
}


//
//  FUNCTION:  InitInstance(HINSTANCE, int)
//
//  PURPOSE:  Saves instance handle and creates main window.
//
//  PARAMTERS:
//    hInstance - The handle to the instance of this application that
//          is currently being executed.
//    nCmdShow - Specifies how the main window is to be diplayed.
//
//  RETURN VALUE:
//    TRUE - Success
//    FALSE - Initialization failed
//
//  COMMENTS:
//    This function is called at initialization time for every instance of
//    this application.  This function performs initialization tasks that
//    cannot be shared by multiple instances.
//
//    In this case, we save the instance handle in a static variable and
//    create and display the main program window.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    // Save the instance handle in static variable, which will be used in
    // many subsequence calls from this application to Windows.

    hInst = hInstance; // Store instance handle in our global variable

    // Create a main window for this application instance.
    ghwnd = CreateWindow(szAppName,           // See RegisterClass() call.
                         szTitle,             // Text for window title bar.
                         WS_OVERLAPPEDWINDOW, // Window style.
                         CW_USEDEFAULT, 0,    // Use default positioning
                         CW_USEDEFAULT, 0,    // Use default size
                         NULL,                // Overlapped has no parent.
                         NULL,                // Use the window class menu.
                         hInstance,           
                         NULL);               
    
    // If window could not be created, return "failure"
    if (!ghwnd)
        return FALSE;

    //
    // **TODO** Call module specific instance initialization functions here.
    //

    // Make the window visible; update its client area; and return "success"
    ShowWindow(ghwnd, nCmdShow); // Show the window
    UpdateWindow(ghwnd);         // Sends WM_PAINT message

    return TRUE;                 // We succeeded...
}
