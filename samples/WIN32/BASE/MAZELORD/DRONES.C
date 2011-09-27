/***********************************************************************
File:   Drones.c
Author: JohnMil
Date:   5-15-92


Abstract:

    This file contains the functions which are associated with moving and
    drawing drones.


Contents:

    DeletePlayer() -- Free a record of type PlayerType
    NewPlayer() -- Create & initialize a record of type PlayerType
    RandRange() -- return a random number in a given range
    InitDrones() -- Add/remove drone entries from the list of drones
    MoveDrone() -- Timer callback to change drone positions in data struct


Revision History:

10-30-92 (JohnMil):
    Modified to new personal comment format

************************************************************************/

#include <stdlib.h>
#include "winmaze.h"
#include "mazproto.h"
#include <mmsystem.h>



/*=====================================================================
Function: DeletePlayer()

Inputs: Pointer to record to delete

Outputs: Returns pointer to next record in list

Abstract:
    Returns pointer to next element to facilitate maintenance of
    linked lists
======================================================================*/

PlayerType FAR *DeletePlayer(
    PlayerType FAR *ptTrav
    )
{
    PlayerType FAR *ptHold;
    HGLOBAL hMem;

    ptHold = NULL;
    if (ptTrav != NULL) {
        ptHold = ptTrav->next;
        hMem = (HGLOBAL) GlobalHandle(SELECTOROF( ptTrav));
        GlobalUnlock(hMem);
        GlobalFree(hMem);
        }

    return(ptHold);
}


/*=====================================================================
Function: NewPlayer()

Inputs: Data elements of PlayerType element to be created

Outputs: Pointer to newly created & initialize PlayerType record

Abstract:
    Needs no explanation
======================================================================*/

PlayerType FAR *NewPlayer(
    unsigned long ulID,     // Unique identifier for this player struct
    DWORD dwPID,            // process id of window
    LPSTR cUserName,        // Name of the player
    LPSTR cComputerName,    // Name of the computer player is on
    int ix,                 // Maze X- coordinate of player
    int iy,                 // Maze Y- coordinate of player
    BYTE Facing,            // Facing of player in maze
    int iScore,             // Points player is reporting
    int iPicNum,            // Picture number to use for player
    int iGridNum,           // Home grid #
    int iGridxPos,          // Home grid x-position (in grid coordinates
    int iGridyPos,          // Home grid y-position (in grid coordinates
    PlayerType FAR *next    // for linked list maintenance
    )
{
    PlayerType FAR *ptRet;
    HGLOBAL hMem;

    hMem = GlobalAlloc(GHND,sizeof(PlayerType));
    ptRet = (PlayerType FAR *) GlobalLock(hMem);

    if (ptRet == NULL) {
        MessageBox((HWND) NULL,"Insufficient memory. Terminating program.","NewPlayer",
               MB_APPLMODAL|MB_ICONEXCLAMATION);
        PostMessage(hWndMain,WM_CLOSE,0,0);
        }
    else {
        ptRet->ulID = ulID;
        ptRet->dwPID = dwPID;
        lstrcpy(ptRet->cUserName,cUserName);
        lstrcpy(ptRet->cComputerName,cComputerName);
        ptRet->Pos.ix = ix;
        ptRet->Pos.iy = iy;
        ptRet->Pos.Facing = Facing;
        ptRet->Drawn = FALSE;
        ptRet->iScore = iScore;
        ptRet->iPicNum = iPicNum;
        ptRet->iGridNum = iGridNum;
        ptRet->pGridLoc.x = iGridxPos;
        ptRet->pGridLoc.y = iGridyPos;

        ptRet->next = next;
        }

    return(ptRet);
}



/*=====================================================================
Function: RandRange()

Inputs: Low, high end of range return value should be in

Outputs: returns number between iLow and iHigh inclusive

Abstract:
    Since this is using rand() it is not so random for a new process.
======================================================================*/

int RandRange(
    int iLow,
    int iHigh
    )
{
    float f;

    f = ((float) rand())/((float) RAND_MAX);

    return((int) (iLow+f*(iHigh - iLow + 1)));
}



/*=====================================================================
Function: InitDrones()

Inputs: Globals only

Outputs: none

Abstract:
    Performs PART of drone initialization whenever # of drones or their
    speed is changed. This will add or delete drones from the drone list,
    as required.
======================================================================*/

void InitDrones(
    )
{
    int i,iCurNumDrones;
    BYTE b;
    PlayerType FAR *ptTrav;
    char buff[20];


    b = NORTH;

    //
    // If the number of drones has changed, but drones were
    // already active, we need to take a different course of
    // action, ie add or delete drones from active drones,
    // rather than creating completely new ones.
    //

    //
    // Find number of drones currently active.
    //
    iCurNumDrones = 0;
    ptTrav = &ptDrones;
    while (ptTrav->next != NULL) {
        iCurNumDrones++;
        ptTrav = ptTrav->next;
        }

    //
    // read in/ remove the drone bitmaps, as necessary
    //
    if (iNumDrones > 0) {
        AddPic(PIC_DRONE);
        }
    else {
        DelPic(PIC_DRONE);
        }

    //
    // If more drones have been added, we need to create them
    //
    for(i=iCurNumDrones;i<iNumDrones;i++) {
        sprintf(buff,"Drone#%d",i+1);
        ptTrav->next = NewPlayer(i,i,buff,ptSelf.cComputerName,
                    RandRange(5*X_CELLS_PER_SUBGRID,8*X_CELLS_PER_SUBGRID-1),
                    RandRange(7*Y_CELLS_PER_SUBGRID,8*Y_CELLS_PER_SUBGRID-1),
                    b,0,PIC_DRONE,0,0,0,ptTrav->next);
        b = (BYTE) ((b*2 > 0x8) ? 0x1 : b*2);
        }

    //
    // Or, delete drones as needed
    //
    for(i=iNumDrones; i < iCurNumDrones;i++) {
        ptDrones.next = DeletePlayer(ptDrones.next);
        }

    return;
}



/*=====================================================================
Function: MoveDrone()

Inputs: Ignored except for hWnd, handle to current window

Outputs:returns success/failure

Abstract:
    This is a timer callback function which modifies the position of the
    drones in their data structures. After their positions have been changed
    in their record, a message is sent to the 3-d window telling it that
    drones need to be redrawn.
======================================================================*/

LRESULT CALLBACK MoveDrone(
    HWND hWnd,
    UINT wMsg,
    UINT idTimer,
    DWORD dwTime
    )
{
    int x,y;
    BYTE b1,b2,dForward,dBack,dLeft,dRight;
    BOOL blocked;
    PlayerType FAR *ptTrav;
    PositionType ptPos;
    char cBuff[132];
    LPRECT rUpd;
    HANDLE hMem;
    static DWORD dwCallCount=0;

    dwCallCount++;

    //
    // If we're in demo mode, use this callback to move US!
    //
    if (bDemoMode) {

        x=ptSelf.Pos.ix;
        y=ptSelf.Pos.iy;
        dForward = ptSelf.Pos.Facing;
        dBack = BACK_TO_ABS(dForward);
        dLeft = LEFT_TO_ABS(dForward);
        dRight = RIGHT_TO_ABS(dForward);
        b1 = bMaze[x][y];
        b2 = bMaze[ADJ_X(x,dForward)][ADJ_Y(y,dForward)];
        blocked = FALSE;
        if ((b1&dForward)||(b2&dBack)) {
            blocked = TRUE;
            b2 = bMaze[ADJ_X(x,dRight)][ADJ_Y(y,dRight)];
            if ((b1&dLeft)||(b2&dRight)) {
                b2 = bMaze[ADJ_X(x,dLeft)][ADJ_Y(y,dLeft)];
                if ((b1&dLeft)||(b2&dRight)) {
                    x = RIGHT;
                    }
                else {
                    x = LEFT;
                    }
                }
            else {
                x = RIGHT;
                }
            }
        else {
            x = FORWARD;
            }

        y = RandRange(1,11);

        switch (y) {
            case 8:
                x = LEFT;
                break;
            case 9:
                x = RIGHT;
                break;
            case 10:
                x = FORWARD;
                break;
            case 11:
                x = BACK;
                break;
            }

        switch (x) {
            case LEFT:
                PostMessage(hWndMaze,WM_KEYDOWN,VK_LEFT,0);
                break;
            case RIGHT:
                PostMessage(hWndMaze,WM_KEYDOWN,VK_RIGHT,0);
                break;
            case FORWARD:
                PostMessage(hWndMaze,WM_KEYDOWN,VK_UP,0);
                break;
            }

        if ((RandRange(1,3) == 3)&&(!blocked)) {
            PostMessage(hWndMaze,WM_KEYDOWN,VK_SPACE,0);
            }
        }

    hMem = GlobalAlloc(GHND,sizeof(RECT));
    rUpd = (LPRECT) GlobalLock(hMem);
    if (rUpd == NULL) {
        MessageBox((HWND)NULL,"Unable to allocate rectangle. Exiting.","MoveDrone",
               MB_APPLMODAL);
        PostMessage(hWndMain,WM_CLOSE,0,0);
        }

    rUpd->right = rMaze.left;
    rUpd->left = rMaze.right;
    rUpd->top = rMaze.bottom;
    rUpd->bottom = rMaze.top;

    ptTrav = &ptDrones;
    while ((ptTrav->next != NULL)&&(!GamePaused)) {
        //
        // Only use every 2nd drone move in demo mode
        //
        if (bDemoMode && (dwCallCount % 2)) {
            break;
            }

        ptTrav = ptTrav->next;

        if (ptTrav->Drawn) {
            rUpd->right = (rUpd->right > ptTrav->rDrawn.right) ? rUpd->right : ptTrav->rDrawn.right;
            rUpd->left = (rUpd->left < ptTrav->rDrawn.left) ? rUpd->left : ptTrav->rDrawn.left;
            rUpd->top = (rUpd->top < ptTrav->rDrawn.top) ? rUpd->top : ptTrav->rDrawn.top;
            rUpd->bottom = (rUpd->bottom > ptTrav->rDrawn.bottom) ? rUpd->bottom : ptTrav->rDrawn.bottom;
            ptTrav->Drawn=FALSE;
            }
        x=ptTrav->Pos.ix;
        y=ptTrav->Pos.iy;
        dForward = ptTrav->Pos.Facing;
        dBack = BACK_TO_ABS(dForward);
        dLeft = LEFT_TO_ABS(dForward);
        dRight = RIGHT_TO_ABS(dForward);
        ptPos.ix = x;
        ptPos.iy = y;
        ptPos.Facing = dForward;
        b1 = bMaze[x][y];
        b2 = bMaze[ADJ_X(x,dForward)][ADJ_Y(y,dForward)];
        if ((b1&dForward)||(b2&dBack)) {
            b2 = bMaze[ADJ_X(x,dLeft)][ADJ_Y(y,dLeft)];
            if ((b1&dLeft)||(b2&dRight)) {
                b2 = bMaze[ADJ_X(x,dRight)][ADJ_Y(y,dRight)];
                if ((b1&dRight)||(b2&dLeft)) {
                    ptPos.Facing = LEFT_TO_ABS(ptTrav->Pos.Facing);
                    }
                else {
                    ptPos.Facing = RIGHT_TO_ABS(ptTrav->Pos.Facing);
                    }
                }
            else {
                ptPos.Facing = LEFT_TO_ABS(ptTrav->Pos.Facing);
                }
            }
        else {
            if (RandRange(1,10) == 1) {
                ptPos.Facing = LEFT_TO_ABS(ptTrav->Pos.Facing);
                }
            else {
                if (RandRange(1,10) == 1) {
                    ptPos.Facing = RIGHT_TO_ABS(ptTrav->Pos.Facing);
                    }
                else {
                    ptPos.ix = ADJ_X(ptTrav->Pos.ix,dForward);
                    ptPos.iy = ADJ_Y(ptTrav->Pos.iy,dForward);
                    }
                }
            }

        ptTrav->Pos = ptPos;

        //
        // If the drone has homed in on us, we are dead!
        //
        if ((!InSanctuary(&ptSelf.Pos))&&
            (ptSelf.Pos.ix == ptPos.ix)&&(ptSelf.Pos.iy == ptPos.iy)
           ) {
            sprintf(cBuff,"You were run down by %s!",ptTrav->cUserName);
            PrintTextLine(cBuff);
            ptSelf.iScore -=(iDroneSpeed) ? 36/iDroneSpeed/iDroneSpeed : 72;
            KillSelf();
            SendNetMessage(0,0,NULL,NP_SCORE);
            iKilledByDrones++;
            break;
            }
        }

    PostMessage(hWndMaze,WM_COMMAND,IDM_DRAWDRONES,(DWORD) rUpd);

    return(0);
}
