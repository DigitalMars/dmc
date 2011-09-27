
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
| INITIALIZATION MODULE
|   This module contains startup-routines for the creation of windows for this
|   application.
|
|   RegisterAppClass
|   UnregisterAppClass
|   CreateAppWindow
|   CreateMDIClientWindow
|
\*---------------------------------------------------------------------------*/

#include <windows.h>
#include "gdidemo.h"
#include "poly.h"
#include "xform.h"
#include "maze.h"
#include "draw.h"
#include "bounce.h"

/*---------------------------------------------------------------------------*\
| REGISTER APPLICATION CLASS
|   This routine registers all classes necessary for the application.
\*---------------------------------------------------------------------------*/
BOOL FAR RegisterAppClass(HANDLE hInstance)
{
    WNDCLASS wndClass;


    /*
    ** Set the common wndClass information.  This is common to all windows
    ** of this application.
    */
    wndClass.style         = CS_HREDRAW | CS_VREDRAW;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = sizeof(LONG);
    wndClass.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wndClass.hInstance     = hInstance;


    /*
    ** Register the main top-level window (frame).
    */
    wndClass.lpfnWndProc   = WndProc;
    wndClass.hIcon         = LoadIcon(hInstance,MAKEINTRESOURCE(APPICON));
    wndClass.hbrBackground = GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName  = MAKEINTRESOURCE(APPMENU);
    wndClass.lpszClassName = APPCLASS;

    if(!RegisterClass(&wndClass))
        return(FALSE);


    /*
    ** Register the polybezier demo window.
    */
    wndClass.lpfnWndProc   = PolyProc;
    wndClass.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wndClass.hbrBackground = GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName  = NULL;
    wndClass.lpszClassName = POLYCLASS;

    if(!RegisterClass(&wndClass))
    {
        UnregisterClass(APPCLASS,hInstance);
        return(FALSE);
    }


    /*
    ** Register the xform demo window.
    */
    wndClass.lpfnWndProc   = XFormProc;
    wndClass.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wndClass.hbrBackground = GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName  = NULL;
    wndClass.lpszClassName = XFORMCLASS;

    if(!RegisterClass(&wndClass))
    {
        UnregisterClass(APPCLASS ,hInstance);
        UnregisterClass(POLYCLASS,hInstance);
        return(FALSE);
    }


    /*
    ** Register the maze demo window.
    */
    wndClass.lpfnWndProc   = MazeProc;
    wndClass.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wndClass.hbrBackground = GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName  = NULL;
    wndClass.lpszClassName = MAZECLASS;

    if(!RegisterClass(&wndClass))
    {
        UnregisterClass(APPCLASS  ,hInstance);
        UnregisterClass(POLYCLASS ,hInstance);
        UnregisterClass(XFORMCLASS,hInstance);
        return(FALSE);
    }


    /*
    ** Register the random draw demo window.
    */
    wndClass.lpfnWndProc   = DrawProc;
    wndClass.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wndClass.hbrBackground = GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName  = NULL;
    wndClass.lpszClassName = DRAWCLASS;

    if(!RegisterClass(&wndClass))
    {
        UnregisterClass(APPCLASS  ,hInstance);
        UnregisterClass(POLYCLASS ,hInstance);
        UnregisterClass(XFORMCLASS,hInstance);
        UnregisterClass(MAZECLASS ,hInstance);
        return(FALSE);
    }


    /*
    ** Register the bouncing ball demo window.
    */
    wndClass.lpfnWndProc   = BounceProc;
    wndClass.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wndClass.hbrBackground = GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName  = NULL;
    wndClass.lpszClassName = BOUNCECLASS;

    if(!RegisterClass(&wndClass))
    {
        UnregisterClass(APPCLASS  ,hInstance);
        UnregisterClass(POLYCLASS ,hInstance);
        UnregisterClass(XFORMCLASS,hInstance);
        UnregisterClass(MAZECLASS ,hInstance);
        UnregisterClass(DRAWCLASS ,hInstance);
        return(FALSE);
    }

    return(TRUE);
}


/*---------------------------------------------------------------------------*\
| UNREGISTER APPLICATION CLASS
|   This routine unregisters all classes registered for the application.
|   It is typically called upon termination of the application.
\*---------------------------------------------------------------------------*/
VOID FAR UnregisterAppClass(HANDLE hInstance)
{
    UnregisterClass(APPCLASS   ,hInstance);
    UnregisterClass(POLYCLASS  ,hInstance);
    UnregisterClass(XFORMCLASS ,hInstance);
    UnregisterClass(MAZECLASS  ,hInstance);
    UnregisterClass(DRAWCLASS  ,hInstance);
    UnregisterClass(BOUNCECLASS,hInstance);

    return;
}


/*---------------------------------------------------------------------------*\
| CREATE APPLICATION WINDOW
|   This routine creates the main top-level window.
\*---------------------------------------------------------------------------*/
HWND FAR CreateAppWindow(HANDLE hInstance)
{
    /*
    ** Upon creation of the window, initialize the extra object information.
    */
    return(CreateWindow(APPCLASS,APPTITLE,WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
            CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
            NULL,NULL,hInstance,NULL));
}


/*---------------------------------------------------------------------------*\
| CREATE MDI CLIENT WINDOW
|   This routine creates the client window which handles the MDI windows.
|   The window will eventually be size to fit into the frame-window's client
|   area.
\*---------------------------------------------------------------------------*/
HWND FAR CreateMDIClientWindow(HWND hWndFrame)
{
    HWND               hWndClient;
    HANDLE             hInstance;
    CLIENTCREATESTRUCT ccs;


    /*
    ** Initialize the client struct to point to define the menu and child
    ** identifiers.  The menu item specifies the parent menu in which the
    ** the list of MDI childs will be appended to this menu.
    */
    ccs.hWindowMenu  = NULL;
    ccs.idFirstChild = 0;


    hInstance = GETINSTANCE(hWndFrame);

    hWndClient = CreateWindow("MDICLIENT",NULL,WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
            0,0,1,1,hWndFrame,NULL,hInstance,(LPVOID)&ccs);

    return(hWndClient);
}
