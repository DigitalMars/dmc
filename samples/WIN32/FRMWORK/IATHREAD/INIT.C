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
//    InitFrameWindow()       - Initializes window data and registers window.
//    RegisterMDIChildClass() - Registers the MDI child window class.
//
//  COMMENTS:
//

#include <windows.h>            // Required for all Windows applications
#include "globals.h"            // Prototypes specific to this application
#include "resource.h"

//
//  FUNCTION: InitFrameWindow(HINSTANCE, int, HMENU)
//
//  PURPOSE: Initializes frame window data and registers window class.
//
//  PARAMETERS:
//    hInstance - The handle to the instance of this application that
//                is currently being executed.
//    nCmdShow  - Specifies how the main window is to be displayed.
//    hMenu     - The initial menu to be attached to the frame window
//
//  RETURN VALUE:
//    If successful, returns the frame window's handle.
//    Returns NULL if initialization failed.
//
//  COMMENTS:
//
//    This function is called at application initialization time.  It
//    performs initialization tasks for the current application instance.
//    Unlike Win16, in Win32, each instance of an application must register
//    window classes.
//
//    In this function, we initialize a window class by filling out a data
//    structure of type WNDCLASS and calling the Windows RegisterClass()
//    function.  Then we create the main window and show it.
//
//

HWND InitFrameWindow(HINSTANCE hInstance, int nCmdShow, HMENU hMenu)
{
    HWND       hWndFrame = NULL;
    WNDCLASSEX wc        = {0};
    char       szAppName[DEFAULT_STR_LEN];   // The name of this application
    char       szTitle[DEFAULT_STR_LEN];     // The title bar text


    // Load the application name and description strings.
    LoadString(hInstance, IDS_APPNAME, szAppName, sizeof(szAppName));
    LoadString(hInstance, IDS_DESCRIPTION, szTitle, sizeof(szTitle));

    // Fill in window class structure with parameters that describe the
    // frame window.
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW; // Class style(s).
    wc.lpfnWndProc   = (WNDPROC)FrameWndProc;   // Window Procedure
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

    // Register the window class and return NULL if unsuccesful.
    if (!RegisterClassEx(&wc))
    {
        //Assume we are running on NT where RegisterClassEx() is
        //not implemented, so let's try calling RegisterClass().

        if (!RegisterClass((LPWNDCLASS)&wc.style))
        	return FALSE;
    }

    // Create a frame window for this application instance.
    hWndFrame = CreateWindow(szAppName,     // See RegisterClass() call
                       szTitle,             // Text for window title bar
                       WS_CLIPCHILDREN|WS_OVERLAPPEDWINDOW, // Window style
                       CW_USEDEFAULT, 0,    // Use default positioning
                       CW_USEDEFAULT, 0,    // Use default size
                       NULL,                // Overlapped has no parent
                       hMenu,               // Use the window class menu
                       hInstance,           // This instance owns this window
                       NULL                 // Don't need data in WM_CREATE
                       );

    // If window was created, show and update it.
    if (hWndFrame != NULL)
    {
        ShowWindow(hWndFrame, nCmdShow);   // Show the window
        UpdateWindow(hWndFrame);           // Sends WM_PAINT message
    }

    return hWndFrame;                      // Return NULL if failed, or
                                           // frame window's handle if success
}


//
//  FUNCTION: RegisterMDIChildClass(HINSTANCE)
//
//  PURPOSE: To register the MDI child window class.
//
//  PARAMETERS:
//    hinst - The instance of the application used to register the class.
//
//  RETURN VALUE:
//    TRUE  - Succeeded in registering the class.
//    FALSE - Failed to register the class.
//
//  COMMENTS:
//
//

BOOL RegisterMDIChildClass(HINSTANCE hinst)
{
    WNDCLASSEX  wc = {0};
    char szChildName[DEFAULT_STR_LEN];

    LoadString (hinst, IDS_CHILDNAME, szChildName, sizeof(szChildName));

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = (WNDPROC)MDIChildWndProc;
    wc.hIcon         = LoadIcon(hinst, MAKEINTRESOURCE(IDI_CHILDICON));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.lpszMenuName  = NULL;
    wc.hInstance     = hinst;                      // Owner of this class
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = sizeof (PTHREAD_DATA);      // Will hold a pointer
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Default color
    wc.lpszClassName = szChildName;
	wc.hIconSm       = LoadImage(hInst,            // Load small icon image
	                             MAKEINTRESOURCE(IDI_CHILDICON),
	                             IMAGE_ICON,
	                             16, 16,
	                             0);

    if (!RegisterClassEx(&wc))
    {
        //Assume we are running on NT where RegisterClassEx() is
        //not implemented, so let's try calling RegisterClass().

        if (!RegisterClass((LPWNDCLASS)&wc.style))
        	return FALSE;
    }

    return TRUE;
}

