
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
| MAZE MODULE
\*---------------------------------------------------------------------------*/

#include <windows.h>
#include "gdidemo.h"
#include  "maze.h"

/*---------------------------------------------------------------------------*\
| CREATE MAZE WINDOW PROCEDURE
\*---------------------------------------------------------------------------*/
HWND FAR CreateMazeWindow(HWND hWndClient, int nItem)
{
    HANDLE          hInstance;
    MDICREATESTRUCT mcs;


    hInstance = GETINSTANCE(hWndClient);

    /*
    ** Initialize the MDI create struct for creation of the
    ** test window.
    */
    mcs.szClass = MAZECLASS;
    mcs.szTitle = MAZETITLE;
    mcs.hOwner  = hInstance;
    mcs.x       = CW_USEDEFAULT;
    mcs.y       = CW_USEDEFAULT;
    mcs.cx      = CW_USEDEFAULT;
    mcs.cy      = CW_USEDEFAULT;
    mcs.style   = 0l;
    mcs.lParam  = (LONG)nItem;

    return((HWND)SendMessage(hWndClient,WM_MDICREATE,0,(LONG)(LPMDICREATESTRUCT)&mcs));
}


/*---------------------------------------------------------------------------*\
| MAZE WINDOW PROCEDURE
\*---------------------------------------------------------------------------*/
LONG APIENTRY MazeProc(HWND hWnd, UINT wMsg, WPARAM wParam, LONG lParam)
{
    switch(wMsg)
    {
        case WM_CREATE:
            MazeCreateProc(hWnd);
            break;


        case WM_COMMAND:
            MazeCommandProc(hWnd,wParam,lParam);
            break;

        case WM_TIMER:
            DrawMaze(hWnd);
            break;


        case WM_PAINT:
            MazePaintProc(hWnd);
            break;


        case WM_DESTROY:
            MazeDestroyProc(hWnd);
            break;


        default:
            return(DefMDIChildProc(hWnd,wMsg,wParam,lParam));
    }
    return(0l);
}


/*---------------------------------------------------------------------------*\
| MAZE CREATE PROCEDURE
\*---------------------------------------------------------------------------*/
BOOL MazeCreateProc(HWND hWnd)
{
    PMAZEDATA ppd;


    if(AllocWindowInfo(hWnd,sizeof(MAZEDATA)))
    {
        if(ppd = (PMAZEDATA)LockWindowInfo(hWnd))
        {
            UnlockWindowInfo(hWnd);
            return(TRUE);
        }
        FreeWindowInfo(hWnd);
    }
    return(FALSE);
}


/*---------------------------------------------------------------------------*\
| MAZE COMMAND PROCEDURE
\*---------------------------------------------------------------------------*/
BOOL MazeCommandProc(HWND hWnd, WPARAM wParam, LONG lParam)
{
    hWnd   = hWnd;
    wParam = wParam;
    lParam = lParam;

    return(TRUE);
}


/*---------------------------------------------------------------------------*\
| MAZE PAINT PROCEDURE
\*---------------------------------------------------------------------------*/
VOID MazePaintProc(HWND hWnd)
{
    HDC         hDC;
    PAINTSTRUCT ps;


    if(hDC = BeginPaint(hWnd,&ps))
        EndPaint(hWnd,&ps);

    return;
}


/*---------------------------------------------------------------------------*\
| MAZE DESTROY PROCEDURE
\*---------------------------------------------------------------------------*/
VOID MazeDestroyProc(HWND hWnd)
{
    KillTimer(hWnd,1);
    FreeWindowInfo(hWnd);
    return;
}


VOID DrawMaze(HWND hWnd)
{
    hWnd = hWnd;

    return;
}
