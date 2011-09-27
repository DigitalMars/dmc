
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*-------------------------------------------------------------------------*\
| XFORM MODULE
\*---------------------------------------------------------------------------*/

#include <windows.h>
#include "gdidemo.h"
#include "xform.h"

VOID ResetXForm(HWND);
VOID DrawXForm(HWND);


/*---------------------------------------------------------------------------*\
| CREATE XFORM WINDOW PROCEDURE
\*---------------------------------------------------------------------------*/
HWND FAR CreateXFormWindow(HWND hWndClient, int nItem)
{
    HANDLE          hInstance;
    MDICREATESTRUCT mcs;


    hInstance = GETINSTANCE(hWndClient);

    /*
    ** Initialize the MDI create struct for creation of the
    ** test window.
    */
    mcs.szClass = XFORMCLASS;
    mcs.szTitle = XFORMTITLE;
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
| XFORM WINDOW PROCEDURE
\*---------------------------------------------------------------------------*/
LONG APIENTRY XFormProc(HWND hWnd, UINT wMsg, WPARAM wParam, LONG lParam)
{
    switch(wMsg)
    {
        case WM_CREATE:
            XFormCreateProc(hWnd);
            break;


        case WM_MOVE:
            ResetXForm(hWnd);
            break;


        case WM_TIMER:
            DrawXForm(hWnd);
            break;


        case WM_COMMAND:
            XFormCommandProc(hWnd,wParam,lParam);
            break;


        case WM_PAINT:
            XFormPaintProc(hWnd);
            break;


        case WM_DESTROY:
            XFormDestroyProc(hWnd);
            break;


        default:
            return(DefMDIChildProc(hWnd,wMsg,wParam,lParam));
    }
    return(0l);
}


/*---------------------------------------------------------------------------*\
| XFORM CREATE PROCEDURE
\*---------------------------------------------------------------------------*/
BOOL XFormCreateProc(HWND hWnd)
{
    PXFORMDATA pxd;

#ifndef NOT_IMPLEMENTED

    RECT       rect;
    int        dx,dy;

#endif

    if(AllocWindowInfo(hWnd,sizeof(XFORMDATA)))
    {
        if(pxd = (PXFORMDATA)LockWindowInfo(hWnd))
        {

#ifndef NOT_IMPLEMENTED

            GetClientRect(hWnd,&rect);

            dx = (rect.right + rect.left) >> 4;
            dy = (rect.bottom + rect.top) >> 4;

            pxd->pObject[0].x = dx << 1;
            pxd->pObject[0].y = 0;
            pxd->pObject[1].x = dx;
            pxd->pObject[1].y = dy;
            pxd->pObject[2].x = 0;
            pxd->pObject[2].y = dy;
            pxd->pObject[3].x = dx << 1;
            pxd->pObject[3].y = 0;
            pxd->nObject      = 4;

            pxd->xForm.eM11 = (float) 0.866;
            pxd->xForm.eM12 = (float) 0.5;
            pxd->xForm.eM21 = (float)-0.5;
            pxd->xForm.eM22 = (float) 0.866;
            pxd->xForm.eDx  = (float) 0;
            pxd->xForm.eDy  = (float) 0;

#endif

            UnlockWindowInfo(hWnd);

            SetTimer(hWnd,1,500,NULL);

            return(TRUE);
        }
        FreeWindowInfo(hWnd);
    }
    return(FALSE);
}


/*---------------------------------------------------------------------------*\
| XFORM COMMAND PROCEDURE
\*---------------------------------------------------------------------------*/
BOOL XFormCommandProc(HWND hWnd, WPARAM wParam, LONG lParam)
{
    hWnd   = hWnd;
    wParam = wParam;
    lParam = lParam;

    return(TRUE);
}


/*---------------------------------------------------------------------------*\
| XFORM PAINT PROCEDURE
\*---------------------------------------------------------------------------*/
VOID XFormPaintProc(HWND hWnd)
{
    HDC         hDC;
    PAINTSTRUCT ps;


    if(hDC = BeginPaint(hWnd,&ps))
        EndPaint(hWnd,&ps);

    ResetXForm(hWnd);

    return;
}


/*---------------------------------------------------------------------------*\
| XFORM DESTROY PROCEDURE
\*---------------------------------------------------------------------------*/
VOID XFormDestroyProc(HWND hWnd)
{
    KillTimer(hWnd,1);
    FreeWindowInfo(hWnd);
    return;
}



VOID ResetXForm(HWND hWnd)
{
    PXFORMDATA pxd;
    HDC        hDC;

#ifndef NOT_IMPLEMENTED

    RECT       rect;

#endif


    if(pxd = (PXFORMDATA)LockWindowInfo(hWnd))
    {
        if(hDC = GetDC(hWnd))
        {

#ifndef NOT_IMPLEMENTED

            GetClientRect(hWnd,&rect);
            BitBlt(hDC,0,0,rect.right,rect.bottom,(HDC)0,0,0,0);

            SetViewportOrgEx(hDC,rect.right >> 1,rect.bottom >> 1,NULL);
            SetWorldTransform(hDC,(LPXFORM)&pxd->xForm);
            Polyline(hDC,pxd->pObject,pxd->nObject);
            ModifyWorldTransform(hDC,(LPXFORM)&pxd->xForm,MWT_IDENTITY);


            // I have to put this in since GDI currently doesn't
            // reset the drawing attributes -- 5/24/91
            //
            SetViewportOrgEx(hDC,0,0,NULL);

#endif

            ReleaseDC(hWnd,hDC);
        }
        UnlockWindowInfo(hWnd);
    }
    return;
}


VOID DrawXForm(HWND hWnd)
{
    PXFORMDATA pxd;
    HDC        hDC;
#ifndef NOT_IMPLEMENTED
    XFORM      xForm;
    RECT       rect;
#endif


    if(pxd = (PXFORMDATA)LockWindowInfo(hWnd))
    {
        if(hDC = GetDC(hWnd))
        {

#ifndef NOT_IMPLEMENTED

            GetClientRect(hWnd,&rect);
            SetViewportOrgEx(hDC,rect.right >> 1, rect.bottom >> 1,NULL);


            SetWorldTransform(hDC,(LPXFORM)&pxd->xForm);
            Polyline(hDC,pxd->pObject,pxd->nObject);

            ModifyWorldTransform(hDC,(LPXFORM)&pxd->xForm,MWT_RIGHTMULTIPLY);
            Polyline(hDC,pxd->pObject,pxd->nObject);

            GetWorldTransform(hDC,(LPXFORM)&xForm);
            pxd->xForm.eM11 = xForm.eM11;
            pxd->xForm.eM12 = xForm.eM12;
            pxd->xForm.eM21 = xForm.eM21;
            pxd->xForm.eM22 = xForm.eM22;
            pxd->xForm.eDx  = xForm.eDx;
            pxd->xForm.eDy  = xForm.eDy;

            ModifyWorldTransform(hDC,(LPXFORM)&pxd->xForm,MWT_IDENTITY);


            // I have to put this in since GDI currently doesn't
            // reset the drawing attributes -- 5/24/91
            //
            SetViewportOrgEx(hDC,0,0,NULL);

#endif
            ReleaseDC(hWnd,hDC);
        }
        UnlockWindowInfo(hWnd);
    }
    return;
}
