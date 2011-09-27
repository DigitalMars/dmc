
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
| BOUNCING BALL MODULE
\*---------------------------------------------------------------------------*/

#include <windows.h>
#include "gdidemo.h"
#include "bounce.h"


VOID XFormClear(PMYXFORM);
VOID XFormScale(PMYXFORM,int,int);
VOID XFormTrans(PMYXFORM,int,int);
VOID XFormPoint(PMYXFORM,LPPOINT);


/*---------------------------------------------------------------------------*\
| CREATE BOUNCE WINDOW PROCEDURE
|
\*---------------------------------------------------------------------------*/
HWND FAR CreateBounceWindow(HWND hWndClient, int nItem)
{
    HANDLE          hInstance;
    MDICREATESTRUCT mcs;


    hInstance = GETINSTANCE(hWndClient);

    /*
    ** Initialize the MDI create struct for creation of the
    ** test window.
    */
    mcs.szClass = BOUNCECLASS;
    mcs.szTitle = BOUNCETITLE;
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
| BOUNCE WINDOW PROCEDURE
|
\*---------------------------------------------------------------------------*/
LONG APIENTRY BounceProc(HWND hWnd, UINT wMsg, WPARAM wParam, LONG lParam)
{
    switch(wMsg)
    {
        case WM_CREATE:
            BounceCreateProc(hWnd);
            break;

        case WM_MOVE:
            BounceRefresh(hWnd);
            break;

        case WM_COMMAND:
            BounceCommandProc(hWnd,wParam,lParam);
            break;

        case WM_TIMER:
            BounceObjects(hWnd);
            break;

        case WM_PAINT:
            BouncePaintProc(hWnd);
            break;

        case WM_DESTROY:
            BounceDestroyProc(hWnd);
            break;


        default:
            return(DefMDIChildProc(hWnd,wMsg,wParam,lParam));
    }
    return(0l);
}


/*---------------------------------------------------------------------------*\
| BOUNCE CREATE PROCEDURE
|
\*---------------------------------------------------------------------------*/
BOOL BounceCreateProc(HWND hWnd)
{
    PBOUNCEDATA pbd;


    if(AllocWindowInfo(hWnd,sizeof(BOUNCEDATA)))
    {
        if(pbd = (PBOUNCEDATA)LockWindowInfo(hWnd))
        {
            pbd->hBall1 = ballCreate(hWnd,20,20,RGB(255,0,0));
            pbd->hBall2 = ballCreate(hWnd,20,20,RGB(0,0,255));
            pbd->hBall3 = ballCreate(hWnd,20,20,RGB(0,255,0));
            pbd->hBall4 = ballCreate(hWnd,20,20,RGB(255,0,255));

            UnlockWindowInfo(hWnd);

            SetTimer(hWnd,1,50,NULL);
            return(TRUE);
        }
        FreeWindowInfo(hWnd);
    }
    return(FALSE);
}


/*---------------------------------------------------------------------------*\
| BOUNCE COMMAND PROCEDURE
|
\*---------------------------------------------------------------------------*/
BOOL BounceCommandProc(HWND hWnd, WPARAM wParam, LONG lParam)
{
    hWnd   = hWnd;
    wParam = wParam;
    lParam = lParam;

    return(TRUE);
}


/*---------------------------------------------------------------------------*\
| BOUNCE PAINT PROCEDURE
|
\*---------------------------------------------------------------------------*/
VOID BouncePaintProc(HWND hWnd)
{
    HDC         hDC;
    PAINTSTRUCT ps;


    if(hDC = BeginPaint(hWnd,&ps))
        EndPaint(hWnd,&ps);

    BounceRefresh(hWnd);

    return;
}


/*---------------------------------------------------------------------------*\
| BOUNCE DESTROY PROCEDURE
|
\*---------------------------------------------------------------------------*/
VOID BounceDestroyProc(HWND hWnd)
{
    PBOUNCEDATA pbd;


    KillTimer(hWnd,1);
    if(pbd = (PBOUNCEDATA)LockWindowInfo(hWnd))
    {
        ballDestroy(pbd->hBall1);
        ballDestroy(pbd->hBall2);
        ballDestroy(pbd->hBall3);
        ballDestroy(pbd->hBall4);

        UnlockWindowInfo(hWnd);
    }
    FreeWindowInfo(hWnd);
    return;
}


/*---------------------------------------------------------------------------*\
| BOUNCE REFRESH
|
\*---------------------------------------------------------------------------*/
VOID BounceRefresh(HWND hWnd)
{
    PBOUNCEDATA pbd;
    RECT        rect;
    HDC         hDC;
    POINT       pDimensions;
    int         xDirection,yDirection;


    if(pbd = (PBOUNCEDATA)LockWindowInfo(hWnd))
    {
        if(hDC = GetDC(hWnd))
        {
            GetClientRect(hWnd,&rect);
            FillRect(hDC,&rect,GETCLASSBRUSH(hWnd));

            xDirection = ((lRandom() % 5)+5);
            yDirection = ((lRandom() % 5)+5);
            ballSetPosition(pbd->hBall1,0,0);
            ballSetDirection(pbd->hBall1,xDirection,yDirection);


            ballGetDimensions(pbd->hBall2,&pDimensions);
            ballSetPosition(pbd->hBall2,rect.right-pDimensions.x,0);
            xDirection = -(((int)lRandom() % 5)+5);
            yDirection =  ((lRandom() % 5)+5);
            ballSetDirection(pbd->hBall2,xDirection,yDirection);


            ballGetDimensions(pbd->hBall3,&pDimensions);
            ballSetPosition(pbd->hBall3,0,rect.bottom-pDimensions.y);
            xDirection =  ((lRandom() % 5)+5);
            yDirection = -((int)(lRandom() % 5)+5);
            ballSetDirection(pbd->hBall3,xDirection,yDirection);


            ballGetDimensions(pbd->hBall4,&pDimensions);
            ballSetPosition(pbd->hBall4,rect.right-pDimensions.x,rect.bottom-pDimensions.y);
            xDirection = -((int)(lRandom() % 5)+5);
            yDirection = -((int)(lRandom() % 5)+5);
            ballSetDirection(pbd->hBall4,xDirection,yDirection);


            ballBounce(pbd->hBall1);
            ballBounce(pbd->hBall2);
            ballBounce(pbd->hBall3);
            ballBounce(pbd->hBall4);

            ReleaseDC(hWnd,hDC);
        }
        UnlockWindowInfo(hWnd);
    }
    return;
}



VOID BounceObjects(HWND hWnd)
{
    PBOUNCEDATA pbd;


    if(pbd = (PBOUNCEDATA)LockWindowInfo(hWnd))
    {
        /*
        ** Determine if the balls hit any of the edges of the display.  If
        ** they do, then these functions reset their position to something
        ** more desireable and re-do their direction.
        */
        CheckEdgePosition(hWnd,pbd->hBall1);
        CheckEdgePosition(hWnd,pbd->hBall2);
        CheckEdgePosition(hWnd,pbd->hBall3);
        CheckEdgePosition(hWnd,pbd->hBall4);

        ballBounce(pbd->hBall1);
        ballBounce(pbd->hBall2);
        ballBounce(pbd->hBall3);
        ballBounce(pbd->hBall4);


        UnlockWindowInfo(hWnd);
    }
    return;
}


VOID CheckEdgePosition(HWND hWnd, HANDLE hBall)
{
    POINT pPos,pDir,pDim;
    int   xNewPos,yNewPos,xNewDir,yNewDir;
    RECT  rect;



    ballGetPosition(hBall,&pPos);
    ballGetDirection(hBall,&pDir);
    ballGetDimensions(hBall,&pDim);

    GetClientRect(hWnd,&rect);


    /*
    ** Check each edge of the client rectagle.  If the ball goes past the
    ** boundries, reset the position and give it a new direction.
    */
    xNewDir = pDir.x;
    yNewDir = pDir.y;
    xNewPos = pPos.x+pDir.x;
    yNewPos = pPos.y+pDir.y;

    if(xNewPos < rect.left)
    {
        xNewDir = ((lRandom() % 5)+5);
        ballSetPosition(hBall,rect.left,pPos.y);
    }
    if((xNewPos+pDim.x) > rect.right)
    {
        xNewDir = -(((int)lRandom() % 5)+5);
        ballSetPosition(hBall,rect.right-pDim.x,pPos.y);
    }
    if(yNewPos < rect.top)
    {
        yNewDir = ((lRandom() % 5)+5);
        ballSetPosition(hBall,pPos.x,rect.top);
    }
    if((yNewPos+pDim.y) > rect.bottom)
    {
        yNewDir = -(((int)lRandom() % 5)+5);
        ballSetPosition(hBall,pPos.x,rect.bottom-pDim.y);
    }
    ballSetDirection(hBall,xNewDir,yNewDir);

    return;
}


/*---------------------------------------------------------------------------*\
| CLEAR XFORM
|
\*---------------------------------------------------------------------------*/
VOID XFormClear(PMYXFORM pXForm)
{
    int Row,Col;


    for(Row=0; Row < 3; Row++)
        for(Col=0; Col < 3; Col++)
            if(Row == Col)
                pXForm->xForm[Row][Col] = 1;
            else
                pXForm->xForm[Row][Col] = 0;
    return;
}


/*---------------------------------------------------------------------------*\
| XFORM SCALE
|
\*---------------------------------------------------------------------------*/
VOID XFormScale(PMYXFORM pXForm, int xScale, int yScale)
{
    int idx;


    for(idx=0; idx < 3; idx++)
    {
        pXForm->xForm[idx][0] = pXForm->xForm[idx][0] * xScale;
        pXForm->xForm[idx][1] = pXForm->xForm[idx][1] * yScale;
    }
    return;
}



VOID XFormTrans(PMYXFORM pXForm, int xTrans, int yTrans)
{
    pXForm->xForm[2][0] = pXForm->xForm[2][0] + xTrans;
    pXForm->xForm[2][1] = pXForm->xForm[2][1] + yTrans;

    return;
}



VOID XFormPoint(PMYXFORM pXForm, LPPOINT pPoint)
{
    int x,y;


    x = (pXForm->xForm[0][0] * pPoint->x) + (pXForm->xForm[1][0] * pPoint->y) + pXForm->xForm[2][0];
    y = (pXForm->xForm[0][1] * pPoint->x) + (pXForm->xForm[1][1] * pPoint->y) + pXForm->xForm[2][1];

    pPoint->x = x;
    pPoint->y = y;

    return;
}


/*
** -------------------------------
** -------------------------------
** -------------------------------
*/

HANDLE ballCreate(HWND hWnd, int nWidth, int nHeight, COLORREF crColor)
{
    HANDLE    hBall;
    PBALLDATA pbd;

    if(hBall = LocalAlloc(LHND,sizeof(BALLDATA)))
    {
        if(pbd = (PBALLDATA)LocalLock(hBall))
        {
            pbd->hWnd         = hWnd;
            pbd->nWidth       = nWidth;
            pbd->nHeight      = nHeight;
            pbd->xDirection   = 0;
            pbd->yDirection   = 0;
            pbd->bNewPosition = FALSE;
            pbd->xPosition    = 0;
            pbd->yPosition    = 0;
            pbd->crColor      = crColor;
            XFormClear(&pbd->xForm);

            LocalUnlock(hBall);
            return(hBall);
        }
        LocalFree(hBall);
    }
    return(NULL);
}


BOOL ballDestroy(HANDLE hBall)
{
    if(LocalFree(hBall))
        return(FALSE);
    return(TRUE);
}


VOID ballBounce(HANDLE hBall)
{
    PBALLDATA pbd;
    HDC       hDC;
    HBRUSH    hBrush;
    POINT     pOrg,pExt;
    HRGN      hTmp,hOld,hNew;


    if(pbd = (PBALLDATA)LocalLock(hBall))
    {
        /*
        ** Create old ball object.
        */
        pOrg.x = 0;
        pOrg.y = 0;
        pExt.x = pbd->nWidth;
        pExt.y = pbd->nHeight;
        XFormPoint(&pbd->xForm,&pOrg);
        XFormPoint(&pbd->xForm,&pExt);
        hOld = CreateEllipticRgn(pOrg.x,pOrg.y,pExt.x,pExt.y);


        /*
        ** Create new ball object.
        */
        if(pbd->bNewPosition)
        {
            pbd->bNewPosition = FALSE;
            XFormClear(&pbd->xForm);
            XFormTrans(&pbd->xForm,pbd->xPosition,pbd->yPosition);
        }
        else
            XFormTrans(&pbd->xForm,pbd->xDirection,pbd->yDirection);
        pOrg.x = 0;
        pOrg.y = 0;
        pExt.x = pbd->nWidth;
        pExt.y = pbd->nHeight;
        XFormPoint(&pbd->xForm,&pOrg);
        XFormPoint(&pbd->xForm,&pExt);
        hNew = CreateEllipticRgn(pOrg.x,pOrg.y,pExt.x,pExt.y);



        if(hDC = GetDC(pbd->hWnd))
        {
            hTmp = CreateRectRgn(1,1,2,2);
            CombineRgn(hTmp,hOld,hNew,RGN_DIFF);
            FillRgn(hDC,hTmp,GETCLASSBRUSH(pbd->hWnd));

            hBrush = CreateSolidBrush(pbd->crColor);
            FillRgn(hDC,hNew,hBrush);
            DeleteObject(hBrush);

            ReleaseDC(pbd->hWnd,hDC);

            DeleteObject(hTmp);
        }

        DeleteObject(hOld);
        DeleteObject(hNew);

        LocalUnlock(hBall);
    }
    return;
}


BOOL ballGetDimensions(HANDLE hBall, LPPOINT pDimension)
{
    PBALLDATA pbd;
    BOOL      bOK;


    bOK = FALSE;
    if(pbd = (PBALLDATA)LocalLock(hBall))
    {
        pDimension->x = pbd->nWidth;
        pDimension->y = pbd->nHeight;

        bOK = TRUE;
        LocalUnlock(hBall);
    }
    return(bOK);
}


BOOL ballSetDimensions(HANDLE hBall, int nWidth, int nHeight)
{
    PBALLDATA pbd;
    BOOL      bOK;


    bOK = FALSE;
    if(pbd = (PBALLDATA)LocalLock(hBall))
    {
        pbd->nWidth  = nWidth;
        pbd->nHeight = nHeight;

        bOK = TRUE;
        LocalUnlock(hBall);
    }
    return(bOK);
}


BOOL ballGetDirection(HANDLE hBall, LPPOINT pDirection)
{
    PBALLDATA pbd;
    BOOL      bOK;


    bOK = FALSE;
    if(pbd = (PBALLDATA)LocalLock(hBall))
    {
        pDirection->x = pbd->xDirection;
        pDirection->y = pbd->yDirection;

        bOK = TRUE;
        LocalUnlock(hBall);
    }
    return(bOK);
}


BOOL ballSetDirection(HANDLE hBall, int xDirection, int yDirection)
{
    PBALLDATA pbd;
    BOOL      bOK;


    bOK = FALSE;
    if(pbd = (PBALLDATA)LocalLock(hBall))
    {
        pbd->xDirection = xDirection;
        pbd->yDirection = yDirection;

        bOK = TRUE;
        LocalUnlock(hBall);
    }
    return(bOK);
}


BOOL ballGetPosition(HANDLE hBall, LPPOINT pPosition)
{
    PBALLDATA pbd;
    BOOL      bOK;
    POINT     pOrg;


    bOK = FALSE;
    if(pbd = (PBALLDATA)LocalLock(hBall))
    {
        pOrg.x = 0;
        pOrg.y = 0;
        XFormPoint(&pbd->xForm,&pOrg);

        pPosition->x = pOrg.x;
        pPosition->y = pOrg.y;

        bOK = TRUE;
        LocalUnlock(hBall);
    }
    return(bOK);
}


BOOL ballSetPosition(HANDLE hBall, int x, int y)
{
    PBALLDATA pbd;
    BOOL      bOK;


    bOK = FALSE;
    if(pbd = (PBALLDATA)LocalLock(hBall))
    {
        pbd->bNewPosition = TRUE;
        pbd->xPosition    = x;
        pbd->yPosition    = y;

        bOK = TRUE;
        LocalUnlock(hBall);
    }
    return(bOK);
}
