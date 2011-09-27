// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   dispatch.c
//
//  PURPOSE:  Implement the generic message dispatcher.
//    
//
//  FUNCTIONS:
//    DispMessage - Call the function associated with a message.
//    DispDefault - Call the appropriate default window procedure.
//
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application

LRESULT DispDefault(EDWP, HWND, UINT, WPARAM, LPARAM);

//
//  FUNCTION: DispMessage(LPMSDI, HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Call the function associated with a message.
//
//  PARAMETERS:
//    lpmsdi - Structure containing the message dispatch information.
//    hwnd - The window handle
//    uMessage - The message number
//    wparam - Message specific data
//    lparam - Message specific data
//
//  RETURN VALUE:
//    The value returned by the message function that was called.
//
//  COMMENTS:
//    Runs the table of messages stored in lpmsdi->rgmsd searching
//    for a message number that matches uMessage.  If a match is found,
//    call the associated function.  Otherwise, call DispDefault to
//    call the default function, if any, associated with the message
//    structure.  In either case, return the value recieved from the
//    message or default function.
//

LRESULT DispMessage(LPMSDI lpmsdi, 
                    HWND   hwnd, 
                    UINT   uMessage, 
                    WPARAM wparam, 
                    LPARAM lparam)
{
    int  imsd = 0;

    MSD *rgmsd = lpmsdi->rgmsd;
    int  cmsd  = lpmsdi->cmsd;

    for (imsd = 0; imsd < cmsd; imsd++)
    {
        if (rgmsd[imsd].uMessage == uMessage)
            return rgmsd[imsd].pfnmsg(hwnd, uMessage, wparam, lparam);
    }

    return DispDefault(lpmsdi->edwp, hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: DispDefault(EDWP, HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Call the appropriate default window procedure.
//
//  PARAMETERS:
//    edwp - Enumerate specifying the appropriate default winow procedure.
//    hwnd - The window handle
//    uMessage - The message number
//    wparam - Message specific data
//    lparam - Message specific data
//
//  RETURN VALUE:
//    If there is a default proc, return the value returned by the
//    default proc.  Otherwise, return 0.
//
//  COMMENTS:
//    Calls the default procedure associated with edwp using the specified
//    parameters.
//

LRESULT DispDefault(EDWP   edwp, 
                    HWND   hwnd, 
                    UINT   uMessage, 
                    WPARAM wparam, 
                    LPARAM lparam)
{
    switch (edwp)
    {
        case edwpNone:
            return 0;
        case edwpWindow:
            return DefWindowProc(hwnd, uMessage, wparam, lparam);
        case edwpDialog:
            return DefDlgProc(hwnd, uMessage, wparam, lparam);
        case edwpMDIFrame:
            return DefFrameProc(hwnd, hwndMDIClient, uMessage, wparam, lparam);
        case edwpMDIChild:
            return DefMDIChildProc(hwnd, uMessage, wparam, lparam);
    }
    return 0;
}
