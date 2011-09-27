// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   simple.c
//
//  PURPOSE:   Implement the windows procedure for the main application
//    windows.
//
//  FUNCTIONS:
//    WndProc - Processes messages for the main window.
//
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant of message number)
//    lparam   - additional information (dependant of message number)
//
//  MESSAGES:
//
//    WM_COMMAND    - exit command
//    WM_DESTROY    - destroy window
//
//  RETURN VALUE:
//
//    Depends on the message number.
//
//  COMMENTS:
//
//

LRESULT CALLBACK WndProc(HWND hwnd,
                         UINT uMessage,
                         WPARAM wparam,
                         LPARAM lparam)
{
    switch (uMessage)
    {

        //
        // **TODO** Add cases here for application messages
        //

        case WM_COMMAND: // message: command from application menu

            // Message packing of wparam and lparam have changed for Win32,
            // so use the GET_WM_COMMAND macro to unpack the commnad

            switch (GET_WM_COMMAND_ID(wparam,lparam))
            {

                //
                // **TODO** Add cases here for application specific commands
                //

                case IDM_EXIT:
                    DestroyWindow(hwnd);
                    break;


                default:
                    return DefWindowProc(hwnd, uMessage, wparam, lparam);
            }
            break;

        case WM_DESTROY:  // message: window being destroyed
            PostQuitMessage(0);
            break;

        default:          // Passes it on if unproccessed
            return DefWindowProc(hwnd, uMessage, wparam, lparam);
    }
    return 0;
}
