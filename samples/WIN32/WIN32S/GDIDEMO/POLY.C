
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
| POLYBEZIER MODULE
|   This module contains the routines for demo
\*---------------------------------------------------------------------------*/

#include <windows.h>
#include "gdidemo.h"
#include  "poly.h"

/*---------------------------------------------------------------------------*\
| CREATE BEZIER WINDOW PROCEDURE
|   Create the bezier MDI-child window.
\*---------------------------------------------------------------------------*/
HWND FAR CreatePolyWindow(HWND hWndClient, int nItem)
{
    HANDLE          hInstance;
    MDICREATESTRUCT mcs;


    hInstance = GETINSTANCE(hWndClient);

    /*
    ** Initialize the MDI create struct for creation of the
    ** test window.
    */
    mcs.szClass = POLYCLASS;
    mcs.szTitle = POLYTITLE;
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
| POLYBEZIER WINDOW PROCEDURE
|   This is the main window function for the polybezier demo window.
\*---------------------------------------------------------------------------*/
LONG APIENTRY PolyProc(HWND hWnd, UINT wMsg, WPARAM wParam, LONG lParam)
{
    switch(wMsg)
    {
        case WM_CREATE:
            PolyCreateProc(hWnd);
            break;


        case WM_COMMAND:
            PolyCommandProc(hWnd,wParam,lParam);
            break;

        case WM_MOVE:
            PolyRedraw(hWnd);
            break;


        case WM_TIMER:
            PolyDrawBez(hWnd);
            break;


        case WM_PAINT:
            PolyPaintProc(hWnd);
            break;


        case WM_DESTROY:
            PolyDestroyProc(hWnd);
            break;


        default:
            return(DefMDIChildProc(hWnd,wMsg,wParam,lParam));
    }
    return(0l);
}


/*---------------------------------------------------------------------------*\
| POLYBEZIER CREATE PROCEDURE
|   Create the polybezier window for the demo application.  This is a child
|   of the MDI client window.  Allocate the extra object information for
|   handling of the polybezier demo.
\*---------------------------------------------------------------------------*/
BOOL PolyCreateProc(HWND hWnd)
{
    PPOLYDATA ppd;


    if(AllocWindowInfo(hWnd,sizeof(POLYDATA)))
    {
        if(ppd = (PPOLYDATA)LockWindowInfo(hWnd))
        {
            ppd->nBezTotal  = 20;
            ppd->nBezCurr   = 0;
            ppd->nColor     = 0;
            ppd->hBezBuffer = GlobalAlloc(GHND,(DWORD)(sizeof(BEZBUFFER) * MAX_BEZIER));

            UnlockWindowInfo(hWnd);

            PolyInitPoints(hWnd);

            SetTimer(hWnd,1,50,NULL);
            return(TRUE);
        }
        FreeWindowInfo(hWnd);
    }
    return(FALSE);
}


/*---------------------------------------------------------------------------*\
| POLYBEZIER COMMAND PROCEDURE
|   Process polybezier commands.  This is a NOP for now.  But who knows what
|   tomorrow may bring.
\*---------------------------------------------------------------------------*/
BOOL PolyCommandProc(HWND hWnd, WPARAM wParam, LONG lParam)
{
    hWnd   = hWnd;
    wParam = wParam;
    lParam = lParam;

    return(TRUE);
}


/*---------------------------------------------------------------------------*\
| POLYBEZIER PAINT PROCEDURE
|   Repaint the bezier window.  All we really do here is validate our window,
|   and reset the array of bezier objects.
\*---------------------------------------------------------------------------*/
VOID PolyPaintProc(HWND hWnd)
{
    HDC         hDC;
    PAINTSTRUCT ps;


    if(hDC = BeginPaint(hWnd,&ps))
        EndPaint(hWnd,&ps);

    PolyRedraw(hWnd);

    return;
}


/*---------------------------------------------------------------------------*\
| POLYBEZIER DESTROY PROCEDURE
|   Kill the polybezier demo.  Free up the resources allocated on behalf of
|   this object.
\*---------------------------------------------------------------------------*/
VOID PolyDestroyProc(HWND hWnd)
{
    PPOLYDATA ppd;


    KillTimer(hWnd,1);
    if(ppd = (PPOLYDATA)LockWindowInfo(hWnd))
    {
        GlobalFree(ppd->hBezBuffer);
        UnlockWindowInfo(hWnd);
    }
    FreeWindowInfo(hWnd);

    return;
}


/*---------------------------------------------------------------------------*\
| GET NEW VELOCITY
|   This routine creates a new velocity for the bezier points.  Each bezier
|   point is randomly chosen.  The two inside points should have a speed
|   less then the endpoints (most of the time-better effect).
\*---------------------------------------------------------------------------*/
int PolyNewVel(int i)
{
    int nRet;


    if ((i == 1) || (i == 2))
        nRet = (int)((lRandom() % VELMAX) / 3) + VELMIN;
    else
        nRet = (int)(lRandom() % VELMAX) + VELMIN;

    return((nRet < 0) ? -nRet : nRet);
}


/*---------------------------------------------------------------------------*\
| INITIALIZE POLYBEZIER POINTS
|   This routine initializes the polybezier points for the first object.  This
|   is performed on startup of the window.
\*---------------------------------------------------------------------------*/
VOID PolyInitPoints(HWND hWnd)
{
    PPOLYDATA   ppd;
    LPBEZBUFFER lpBez;
    int         idx;
    RECT        rect;


    if(ppd = (PPOLYDATA)LockWindowInfo(hWnd))
    {
        if(lpBez = (LPBEZBUFFER)GlobalLock(ppd->hBezBuffer))
        {
            GetClientRect(hWnd,&rect);

            for(idx=0; idx < BEZ_PTS-1; idx++)
            {
                lpBez->pPts[idx].x = lRandom() % rect.right;
                lpBez->pPts[idx].y = lRandom() % rect.bottom;

                ppd->pVel[idx].x = PolyNewVel(idx);
                ppd->pVel[idx].y = PolyNewVel(idx);
            }
            GlobalUnlock(ppd->hBezBuffer);
        }
        UnlockWindowInfo(hWnd);
    }
    return;
}


/*---------------------------------------------------------------------------*\
| POLYBEZIER REDRAW
|   This routine resets the bezier curves and redraws the poly-bezier client
|   area.
\*---------------------------------------------------------------------------*/
VOID PolyRedraw(HWND hWnd)
{
    PPOLYDATA   ppd;
    LPBEZBUFFER lpBez,lpCurr;
    HDC         hDC;
    int         i,j;
    RECT        rect;


    if(ppd = (PPOLYDATA)LockWindowInfo(hWnd))
    {
        if(lpBez = (LPBEZBUFFER)GlobalLock(ppd->hBezBuffer))
        {
            if(hDC = GetDC(hWnd))
            {
                /*
                ** Save the current bezier.  Set the first bezier in the
                ** array to that curve, and use it as a basis for the next
                ** series.
                */
                lpCurr        = lpBez+ppd->nBezCurr;
                *lpBez        = *lpCurr;
                ppd->nBezCurr = 0;


                /*
                ** Clean the curves (all but the first curve).
                */
                for(j=1; j < ppd->nBezTotal; j++)
                {
                    for(i=0; i < BEZ_PTS; i++)
                    {
                        (lpBez+j)->pPts[i].x = -1;
                        (lpBez+j)->pPts[i].y = 0;
                    }
                }



                /*
                ** Clear the display.
                */
                GetClientRect(hWnd,&rect);
                BitBlt(hDC,0,0,rect.right, rect.bottom,(HDC)0,0,0,0);


                /*
                ** Draw the first curve in the bezier array.
                */
#if defined(_WIN32) && defined(WIN32)
                PolyBezier(hDC,lpBez->pPts,BEZ_PTS);
#else
                Polyline(hDC,lpBez->pPts,BEZ_PTS);
#endif
                ReleaseDC(hWnd,hDC);
            }
            GlobalUnlock(ppd->hBezBuffer);
        }
        UnlockWindowInfo(hWnd);
    }
    return;
}




VOID PolyDrawBez(HWND hWnd)
{
    PPOLYDATA   ppd;
    LPBEZBUFFER lpBez,lpCurr,lpPrev;
    int         idx,x,y;
    RECT        rect;
    HDC         hDC;
    HPEN        hPen;

static COLORREF crColor[] = {0x000000FF,0x0000FF00,0x00FF0000,0x0000FFFF,
                             0x00FF00FF,0x00FFFF00,0x00FFFFFF,0x00000080,
                             0x00008000,0x00800000,0x00008080,0x00800080,
                             0x00808000,0x00808080,0x000000FF,0x0000FF00,
                             0x00FF0000,0x0000FFFF,0x00FF00FF,0x00FFFF00};


    if(ppd = (PPOLYDATA)LockWindowInfo(hWnd))
    {
        if(lpBez = (LPBEZBUFFER)GlobalLock(ppd->hBezBuffer))
        {
            if(hDC = GetDC(hWnd))
            {
                GetClientRect(hWnd,&rect);

                lpPrev = lpBez+ppd->nBezCurr;

                ppd->nBezCurr += 1;

                if(ppd->nBezCurr >= ppd->nBezTotal)
                {
                    ppd->nBezCurr = 0;
                    ppd->nColor  = (++ppd->nColor % 20);
                }
                lpCurr = lpBez+ppd->nBezCurr;


                if(lpCurr->pPts[0].x != -1)
                {
                    hPen = SelectObject(hDC,GetStockObject(BLACK_PEN));
#if defined(_WIN32) && defined(WIN32)
                    PolyBezier(hDC,lpCurr->pPts,BEZ_PTS);
#else
                    Polyline(hDC,lpCurr->pPts,BEZ_PTS);
#endif
                    SelectObject(hDC,hPen);
                }

                for(idx=0; idx < BEZ_PTS; idx++)
                {
                    x  = lpPrev->pPts[idx].x;
                    y  = lpPrev->pPts[idx].y;
                    x += ppd->pVel[idx].x;
                    y += ppd->pVel[idx].y;

                    if(x >= rect.right)
                    {
                        x = rect.right - ((x - rect.right)+1);
                        ppd->pVel[idx].x = -PolyNewVel(idx);
                    }

                    if(x <= rect.left)
                    {
                        x = rect.left + ((rect.left - x)+1);
                        ppd->pVel[idx].x = PolyNewVel(idx);
                    }

                    if(y >= rect.bottom)
                    {
                        y = rect.bottom - ((y - rect.bottom)+1);
                        ppd->pVel[idx].y = -PolyNewVel(idx);
                    }

                    if(y <= rect.top)
                    {
                        y = rect.top + ((rect.top - y)+1);
                        ppd->pVel[idx].y = PolyNewVel(idx);
                    }

                    lpCurr->pPts[idx].x = x;
                    lpCurr->pPts[idx].y = y;

                }

                hPen = SelectObject(hDC,CreatePen(PS_SOLID,1,crColor[ppd->nColor]));
#if defined(_WIN32) && defined(WIN32)
                PolyBezier(hDC,lpCurr->pPts,BEZ_PTS);
#else
                Polyline(hDC,lpCurr->pPts,BEZ_PTS);
#endif
                DeleteObject(SelectObject(hDC,hPen));

#if defined(_WIN32) && defined(WIN32)
                SetROP2(hDC,R2_COPYPEN);
#endif
                ReleaseDC(hWnd,hDC);
            }
            GlobalUnlock(ppd->hBezBuffer);
        }
        UnlockWindowInfo(hWnd);
    }
}
