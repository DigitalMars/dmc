/***********************************************************************
File:   TopWnd.c
Author: JohnMil
Date:   7-21-92


Abstract:

    This contains the windows procudure responsible for drawing the
    top-view map.


Contents:

    TopViewWndProc() -- windows message processor for Top view window.


Revision History:

10-30-92 (JohnMil):
    Modified to new personal comment format

************************************************************************/

#include "winmaze.h"
#include "mazproto.h"
#include "net.h"
#include <mmsystem.h>




/*=====================================================================
Function: TopViewWndProc()

Inputs: Standard windows entrypoint parms

Outputs: returns success

Abstract:
    Takes care of maintaining the top view map.
======================================================================*/

LONG FAR PASCAL TopViewWndProc(
    HWND hWnd,
    UINT Message,
    UINT wParam,
    LONG lParam
    )
{
    PAINTSTRUCT ps;
    HDC hDC;

    switch (Message) {

        case WM_KEYDOWN:
           SendMessage(hWndMaze,WM_KEYDOWN,wParam,lParam);
           break;

        case WM_MOVE:
            break;

        case WM_SIZE:
            break;

        case WM_PAINT:
            hDC = BeginPaint(hWnd, &ps);
            SetBkMode(hDC, TRANSPARENT);
            DrawTopView(hDC,TRUE);
            EndPaint(hWnd, &ps);
            break;

        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;

        default:
            return DefWindowProc(hWnd, Message, wParam, lParam);
        }

    return(0);
}
