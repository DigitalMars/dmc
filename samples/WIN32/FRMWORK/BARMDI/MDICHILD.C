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
//    InitMDIChild - To register the MDI child window class.
//    MDIChildWndProc - Processes messages for MDI child windows.
//    MsgMCCommand - Handle the WM_COMMAND messages for mdi children.
//    MsgMCDestroy - Handle the WM_DESTROY messages for mdi children.
//
//  COMMENTS:
//
//
//  SPECIAL INSTRUCTIONS: N/A
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include "globals.h"            // prototypes specific to this application
#include "resource.h"
#include "toolbar.h"
#include "statbar.h"


static MSD rgmsd[] =
{
    {WM_COMMAND,    MsgMCCommand},
    {WM_MENUSELECT, MsgMenuSelect},
    {WM_DESTROY,    MsgMCDestroy}
};

MSDI msdiChild =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpMDIChild
};

static CMD rgcmd[] =
{
    {0, NULL}
};

CMDI cmdiChild =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpMDIChild
};


// Module specific globals

char szChildName[] = SZCHILDNAME;

//
//  FUNCTION: InitMDIChild(HINSTANCE)
//
//  PURPOSE: To register the MDI child window class.
//
//  PARAMETERS:
//    hinst - The instance of the application used to register the class.
//
//  RETURN VALUE:
//    TRUE - Succeeded in registering the class.
//    FALSE - Failed to register the class.
//
//  COMMENTS:
//
//

BOOL InitMDIChild(HINSTANCE hinst)
{
    WNDCLASSEX wc;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC)MDIChildWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hinst;                      // Owner of this class
    wc.hIcon         = LoadIcon(hinst, MAKEINTRESOURCE(IDI_CHILDICON));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Default color
    wc.lpszMenuName  = NULL;
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

//
//  FUNCTION: MDIChildWndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for MDI child windows.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant of message number)
//    lparam   - additional information (dependant of message number)
//
//  RETURN VALUE:
//    Depends on the message number.
//
//  COMMENTS:
//    Uses the combination of the message structure defined in mditable.c
//    and the DispMessage function defined in WndProc.c to handle all
//    messages directed to an MDI child window.
//

LRESULT CALLBACK MDIChildWndProc(HWND hwnd,
                                 UINT uMessage,
                                 WPARAM wparam,
                                 LPARAM lparam)
{
    return DispMessage( &msdiChild, hwnd, uMessage, wparam, lparam );
}


//
//  FUNCTION: MsgMCCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_COMMAND messages for mdi children.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number (Unused)
//    GET_WM_COMMAND_ID(wparam,lparam)   - Command identifier
//    GET_WM_COMMAND_HWND(wparam,lparam) - Control handle
//
//  RETURN VALUE:
//
//  COMMENTS:
//    Uses the combination of the command structure defined in mditable.c
//    and the DispCommand function defined in WndProc.c to handle all
//    commands directed to an MDI child window.
//
//

LRESULT MsgMCCommand(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&cmdiChild, hwnd, wparam, lparam);
}


//
//  FUNCTION: MsgMCDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//
//

LRESULT MsgMCDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    cOpen -= 1;

    return 0;
}


