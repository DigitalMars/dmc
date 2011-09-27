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

extern LRESULT OnIdle(VOID);
extern HWND hwndMain;

HANDLE hRTFLib;

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
//    hPrevInstance - This parameter is always NULL in Win32
//          applications.
//
//    lpCmdLine - A pointer to a null terminated string specifying the
//          command line of the application.
//
//    nCmdShow - Specifies how the main window is to be diplayed.
//
//  RETURN VALUE:
//    If the function terminates before entering the message loop,
//    return FALSE.
//    Otherwise, return the WPARAM value sent by the WM_QUIT message.
//
//
//  COMMENTS:
//
//    Windows recognizes this function by name as the initial entry point
//    for the program.  This function calls the initialization routine
//    It then executes a message retrieval and dispatch loop that is the
//    top-level control structure for the remainder of execution.  The
//    loop is terminated when a WM_QUIT  message is received, at which
//    time this function exits the application instance by returning the
//    value passed by PostQuitMessage().
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
    BOOL bRet;

    // Initialize application by setting up the main window.
    if (!InitApplication(hInstance, nCmdShow))
    {
        return FALSE;           // Exits if unable to initialize
    }

	hRTFLib = LoadLibrary("RICHED32.DLL");
	if (!hRTFLib)
	{
	    return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, szAppName);

    // In the true Microsoft Foundation Classes spirit, we implement
    // OnIdle() processing.  The OnIdle() routine is used to update
    // the toolbar so we can accurately reflect the active/inactive
    // state of the Save, Cut, Copy and Paste buttons.
    for (; ;)
    {
        while (!PeekMessage(&msg, 
                            NULL, 
                            (UINT)NULL, 
                            (UINT)NULL, 
                            PM_NOREMOVE) &&
               OnIdle())
        {
            ;  // Do nothing; we either have a message to process, or
               // have already called OnIdle() and can now call
               // GetMessage() again.  GetMessage() will either retrieve
               // the next message, or yield to another application
        }

        // Acquire and dispatch the next message
        // GetMessage() will return 0 when WM_QUIT is retreived
        if (bRet=GetMessage(&msg, NULL, 0, 0))
        {
            if (!TranslateMDISysAccel(hwndMDIClient, &msg))
                if (!IsFindReplaceMsg(&msg))
                    if (!TranslateAccelerator(hwndMain, hAccelTable, &msg))
                    {
                        TranslateMessage(&msg);  // Translates virtual key codes
                        DispatchMessage(&msg);   // Dispatches message to window
                    }
        }

        if (!bRet)
           break;
    }

    if (hRTFLib) FreeLibrary (hRTFLib);

    return msg.wParam;  // Returns the value from PostQuitMessage
}
