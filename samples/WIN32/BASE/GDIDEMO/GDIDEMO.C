
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*---------------------------------------------------------------------------*\
| GDIDEMO MODULE
|   This is the main entry-point module for the GDIDEMO application.  It is
|   intended to provide simple demonstrations of graphical functionality of
|   WIN32.  This module is only concerned with the FRAME-WINDOW object.
\*---------------------------------------------------------------------------*/

#include <windows.h>
#include "gdidemo.h"
#include "poly.h"
#include "xform.h"
#include "maze.h"
#include "draw.h"
#include "bounce.h"

/*---------------------------------------------------------------------------*\
| WINDOWS MAIN PROCEDURE
|   This is the process entry-point routine.  This is the basis for all
|   application events.
\*---------------------------------------------------------------------------*/
int PASCAL WinMain(HANDLE hInst, HANDLE hPrevInst, LPSTR lpszLine, int nShow)
{
    HWND hWndFrame;
    MSG  msg;

    lpszLine = lpszLine;

    /*
    ** If there's a previous instance of this application, then we do not need
    ** to register it again.
    */
    if(!hPrevInst)
        if(!RegisterAppClass(hInst))
            return(1);


    /*
    ** Enter the application message-polling loop.  This is the anchor for
    ** the application.
    */
    msg.wParam = 1;
    if(hWndFrame = CreateAppWindow(hInst))
    {
        ShowWindow(hWndFrame,nShow);
        UpdateWindow(hWndFrame);

        while(GetMessage(&msg,NULL,0,0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    UnregisterAppClass(hInst);

    return(msg.wParam);
}


/*---------------------------------------------------------------------------*\
| CLIENT WINDOW PROCEDURE
|   This is the main window function for the client-window created.  This is
|   the frame window which encompasses the MDI control window.
\*---------------------------------------------------------------------------*/
LONG APIENTRY WndProc(HWND hWndFrame, UINT wMsg, WPARAM wParam, LONG lParam)
{
    HWND hWndClient;


    switch(wMsg)
    {
        /*
        ** Set up any pre-display stuff.  This is where we create the MDI
        ** control window.
        */
        case WM_CREATE:
            CreateProc(hWndFrame);

/*
** DEMO MODE - These PostMessages Are for Demonstration Only
*/
            PostMessage(hWndFrame,WM_COMMAND,IDM_DEMO_DRAW,0);
            PostMessage(hWndFrame,WM_COMMAND,IDM_DEMO_POLYBEZIER,0);
            PostMessage(hWndFrame,WM_COMMAND,IDM_DEMO_BOUNCE,0);
            PostMessage(hWndFrame,WM_COMMAND,IDM_WINDOW_TILE,0);

            break;


        /*
        ** Paint the background of the frame.  This really is a NOP since
        ** the MDI control is displayed over the frame's client window.
        */
        case WM_PAINT:
            PaintProc(hWndFrame);
            break;


        /*
        ** Time to die.
        */
        case WM_DESTROY:
            DestroyProc(hWndFrame);
            break;


        /*
        ** Process the frame-commands.  We need to let the default handler
        ** have this event, since the MDI control handles the
        ** commands as well.
        */
        case WM_COMMAND:
            CommandProc(hWndFrame,wParam,lParam);


        /*
        ** Since this is the frame-window, we need to grab the MDI client
        ** control window to pass to the MDI control.  We store this as
        ** part of the extra-object information for the frame-window.
        */
        default:
            hWndClient = (HWND)GetWindowLong(hWndFrame,CLIENTWND);
            return(DefFrameProc(hWndFrame,hWndClient,wMsg,wParam,lParam));
    }
    return(0l);
}


/*---------------------------------------------------------------------------*\
| CLIENT CREATE PROCEDURE
|   This is the main event-handler for the WM_CREATE message.  It is here
|   we create the MDI control window for the application.  We store this
|   information as part of the frame-window extra object information.
\*---------------------------------------------------------------------------*/
BOOL CreateProc(HWND hWndFrame)
{
    HMENU hMenu;
    HWND  hWnd;


    /*
    ** Set the window information to contain the child window.
    */
    if(hWnd = CreateMDIClientWindow(hWndFrame))
        SetWindowLong(hWndFrame,CLIENTWND,(LONG)hWnd);

    if(hMenu = GetSubMenu(GetMenu(hWndFrame),0))
    {
        ModifyMenu(hMenu,IDM_DEMO_XFORM ,MF_BYCOMMAND | MF_GRAYED,IDM_DEMO_XFORM ,"&XForm");
        ModifyMenu(hMenu,IDM_DEMO_MAZE  ,MF_BYCOMMAND | MF_GRAYED,IDM_DEMO_MAZE  ,"&Maze");
        DrawMenuBar(hWndFrame);
    }

    return(TRUE);
}


/*---------------------------------------------------------------------------*\
| COMMAND PROCEDURE
|   This is the main event-handler for the WM_COMMAND event for the window
|   application.  All we really do is process the MENU commands for creating
|   the DEMO windows.
\*---------------------------------------------------------------------------*/
BOOL CommandProc(HWND hWndFrame, WPARAM wParam, LONG lParam)
{
    HWND hWndClient;

    lParam = lParam;

    switch(wParam)
    {
        /*
        ** Demo the poly-bezier window.
        */
        case IDM_DEMO_POLYBEZIER:
            if(hWndClient = (HWND)GetWindowLong(hWndFrame,CLIENTWND))
                CreatePolyWindow(hWndClient,0);
            break;


        /*
        ** Demo the xform's.
        */
        case IDM_DEMO_XFORM:
            if(hWndClient = (HWND)GetWindowLong(hWndFrame,CLIENTWND))
                CreateXFormWindow(hWndClient,0);
            break;


        /*
        ** Demo the maze.
        */
        case IDM_DEMO_MAZE:
            if(hWndClient = (HWND)GetWindowLong(hWndFrame,CLIENTWND))
                CreateMazeWindow(hWndClient,0);
            break;


        /*
        ** Demo random drawing objects.
        */
        case IDM_DEMO_DRAW:
            if(hWndClient = (HWND)GetWindowLong(hWndFrame,CLIENTWND))
                CreateDrawWindow(hWndClient,0);
            break;


        /*
        ** Demo bouncing region balls.
        */
        case IDM_DEMO_BOUNCE:
            if(hWndClient = (HWND)GetWindowLong(hWndFrame,CLIENTWND))
                CreateBounceWindow(hWndClient,0);
            break;


        /*
        ** MDI cascade the demo windows.
        */
        case IDM_WINDOW_CASCADE:
            if(hWndClient = (HWND)GetWindowLong(hWndFrame,CLIENTWND))
                SendMessage(hWndClient,WM_MDICASCADE,0,0l);
            break;


        /*
        ** MDI tile the demo windows.
        */
        case IDM_WINDOW_TILE:
            if(hWndClient = (HWND)GetWindowLong(hWndFrame,CLIENTWND))
                SendMessage(hWndClient,WM_MDITILE,0,0l);
            break;


        /*
        ** MDI arrange the MDI icons.
        */
        case IDM_WINDOW_ICON:
            if(hWndClient = (HWND)GetWindowLong(hWndFrame,CLIENTWND))
                SendMessage(hWndClient,WM_MDIICONARRANGE,0,0l);
            break;


        /*
        ** Display the about box.
        */
        case IDM_HELP_ABOUT:
            DisplayDialogBox(hWndFrame,MAKEINTRESOURCE(ABOUTBOX),(WNDPROC)AboutDlgProc,0l);
            break;


        /*
        ** Command not recognized.
        */
        default:
            return(FALSE);
    }
    return(TRUE);
}


/*---------------------------------------------------------------------------*\
| CLIENT PAINT PROCEDURE
|   This is the main event-handler for the WM_PAINT message.
\*---------------------------------------------------------------------------*/
VOID PaintProc(HWND hWndFrame)
{
    HDC         hDC;
    PAINTSTRUCT ps;


    if(hDC = BeginPaint(hWndFrame,&ps))
        EndPaint(hWndFrame,&ps);

    return;
}


/*---------------------------------------------------------------------------*\
| CLIENT DESTROY PROCEDURE
|   This routine is called to cleanup global resources upon exit of the
|   application.
\*---------------------------------------------------------------------------*/
VOID DestroyProc(HWND hWndFrame)
{
    hWndFrame = hWndFrame;

    PostQuitMessage(0);

    return;
}



DWORD FAR lRandom(VOID)
{
    static DWORD glSeed = (DWORD)-365387184;

    glSeed *= 69069;
    return(++glSeed);
}
