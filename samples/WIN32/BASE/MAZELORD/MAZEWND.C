/***********************************************************************
File:   MazeWnd.c
Author: JohnMil
Date:   7-21-92


Abstract:

    This module contains the windows procedure responsible for drawing the 3-d
    maze.


Contents:

    StretchBitmaps() --Prestretches loaded pictures to bitmap size.
    KillSelf() -- Takes care of hassles associated with dying
    MazeWndProc() -- Main entrypoint for drawing 3-d maze

Revision History:

10-30-92 (JohnMil):
    Modified to new personal comment format
************************************************************************/

#include "winmaze.h"
#include "mazproto.h"
#include "net.h"
#include <mmsystem.h>


/*=====================================================================
Function:   StretchBitmaps()

Inputs:     none

Outputs:    none

Abstract:
    StretchBitmaps() will take care of stretching the appropriate bitmaps
    to their correct dimensions.
======================================================================*/

void StretchBitmaps(
    void
    )
{
    float xSize,ySize,xOrg,yOrg,x,y,z;
    int iPicNum,iRely,iFacing;
    HDC hDC,hSrcDC,hDestDC,hMemDC;
    FullPicType FAR *fptTrav;
    HBITMAP hBM;
    HCURSOR hCursor;

    //
    // Set the cursor to an hourglass since this takes time...
    //
    hCursor = SetCursor(LoadCursor((HINSTANCE)NULL,IDC_WAIT));

    hDC = GetDC(hWndMaze);
    hSrcDC = CreateCompatibleDC(hDC);
    hDestDC = CreateCompatibleDC(hDC);
    hMemDC = CreateCompatibleDC(hDestDC);

    GetClientRect(hWndMaze,(LPRECT) &rMaze);

    //
    // Set x and y to logical panel origin.
    //
    y = (float) (-PANEL_HEIGHT/2);
    x = (float) (-PANEL_WIDTH/2);

    //
    // Loop through all of the pictures we know about.
    //
    for(iPicNum=0;iPicNum < NUM_PICS;iPicNum++) {

        AddPic(iPicNum);

        //
        // Find the fptPic entry for Picture # iPicNum
        //
        fptTrav = &fptPic;
        while (fptTrav->next != NULL) {
            fptTrav = fptTrav->next;
            if (fptTrav->iPicNum == iPicNum) {
                break;
                }
            }
        //
        // Loop from closest to us to furthest away from us for stretching
        //
        for (iRely = 0;iRely < MAX_DRAW_DIST;iRely++) {
            PreStretch[iPicNum][iRely].next = NULL;
            PreStretch[iPicNum][iRely].iPicNum = iPicNum;

            //
            // We need to stretch once per facing
            //
            for (iFacing=0;iFacing<4;iFacing++) {

#if ( _ALPHA_ == 1 ) 
                z = (float) (iRely ? iRely*PANEL_WIDTH:PANEL_WIDTH/4);
#else
                z = (float) (iRely-1)*PANEL_WIDTH; //- PANEL_WIDTH/2;
#endif

                xSize = MC_TO_SC(fptTrav->P[iFacing].xSize,z);
                ySize = MC_TO_SC(fptTrav->P[iFacing].ySize,z);
                xOrg = MC_TO_SC(fptTrav->P[iFacing].xOrg,z);
                yOrg = MC_TO_SC(fptTrav->P[iFacing].yOrg,z);

                PreStretch[iPicNum][iRely].P[iFacing].xSize = (int) xSize;
                PreStretch[iPicNum][iRely].P[iFacing].ySize = (int) ySize;
                PreStretch[iPicNum][iRely].P[iFacing].xOrg = (int) xOrg;
                PreStretch[iPicNum][iRely].P[iFacing].yOrg = (int) yOrg;
                PreStretch[iPicNum][iRely].M[iFacing].xSize = (int) xSize;
                PreStretch[iPicNum][iRely].M[iFacing].ySize = (int) ySize;
                PreStretch[iPicNum][iRely].M[iFacing].xOrg = (int) xOrg;
                PreStretch[iPicNum][iRely].M[iFacing].yOrg = (int) yOrg;

                //
                // If we're supposed to prestretch this bitmap, do so
                //
                if (((iPicNum == PIC_DRONE)&&bDronePrestretch)||
                     (bPlayerPrestretch)
                   ) {
                    //
                    // If there were already stretched bitmaps, delete them.
                    //
                    if (PreStretch[iPicNum][iRely].P[iFacing].hBitmap !=
                        (HBITMAP)NULL) {
                        DeleteObject(PreStretch[iPicNum][iRely].P[iFacing].hBitmap);
                        }
                    if (PreStretch[iPicNum][iRely].M[iFacing].hBitmap !=
                        (HBITMAP)NULL) {
                        DeleteObject(PreStretch[iPicNum][iRely].M[iFacing].hBitmap);
                        }

                    //
                    // Stretch the bitmap
                    //
                    hBM = CreateCompatibleBitmap(hDC,(int)xSize,(int)ySize);

                    SelectObject(hSrcDC,fptTrav->P[iFacing].hBitmap);
                    SelectObject(hDestDC,hBM);
                    StretchBlt(hDestDC,0,0,(int)xSize,(int)ySize,
                               hSrcDC,0,0,
                               fptTrav->P[iFacing].xSize,
                               fptTrav->P[iFacing].ySize,
                               SRCCOPY);
                    PreStretch[iPicNum][iRely].P[iFacing].hBitmap = hBM;

                    //
                    // Now stretch the mask
                    //
                    hBM = CreateCompatibleBitmap(hMemDC,(int)xSize,(int)ySize);

                    SelectObject(hSrcDC,fptTrav->M[iFacing].hBitmap);
                    SelectObject(hMemDC,hBM);
                    StretchBlt(hMemDC,0,0,(int)xSize,(int)ySize,
                               hSrcDC,0,0,
                               fptTrav->M[iFacing].xSize,
                               fptTrav->M[iFacing].ySize,
                               SRCCOPY);
                    PreStretch[iPicNum][iRely].M[iFacing].hBitmap = hBM;

                    }
                else {
                    //
                    // If we're not stretching bitmaps, we should delete
                    // any we don't need anymore to free memory.
                    //
                    if (PreStretch[iPicNum][iRely].P[iFacing].hBitmap !=
                        (HBITMAP)NULL) {
                        DeleteObject(PreStretch[iPicNum][iRely].P[iFacing].hBitmap);
                        }
                    if (PreStretch[iPicNum][iRely].M[iFacing].hBitmap !=
                        (HBITMAP)NULL) {
                        DeleteObject(PreStretch[iPicNum][iRely].M[iFacing].hBitmap);
                        }
                    PreStretch[iPicNum][iRely].P[iFacing].hBitmap = (HBITMAP)NULL;
                    PreStretch[iPicNum][iRely].M[iFacing].hBitmap = (HBITMAP)NULL;
                    }
                }
            }
        }

    DeleteDC(hMemDC);
    DeleteDC(hSrcDC);
    DeleteDC(hDestDC);
    ReleaseDC(hWndMaze,hDC);

    //
    // Return the cursor.
    //
    SetCursor(hCursor);
    return;
}



/*=====================================================================
Function:   KillSelf()

Inputs:     none

Outputs:    none

Abstract:
    KillSelf() takes care of playing the teleport sound, teleporting us back to
    the sanctuary, and notifying the network that we've died...
======================================================================*/

void KillSelf(void)
{
    RECT rScratch;

    if (!sndPlaySound("tele.wav",SND_ASYNC)) {
        MessageBeep((UINT) -1);
        MessageBeep((UINT) -1);
        MessageBeep((UINT) -1);
        }
    //
    // these are from initmaze.c
    //

    if (bDemoMode) {
        ptSelf.Pos.ix = RandRange(5*X_CELLS_PER_SUBGRID,8*X_CELLS_PER_SUBGRID-1);
        ptSelf.Pos.iy = RandRange(7*Y_CELLS_PER_SUBGRID,8*Y_CELLS_PER_SUBGRID-1);
        PrintTextLine("You have been randomly teleported.");
        }
    else {
        ptSelf.Pos.ix = 6*X_CELLS_PER_SUBGRID+1;
        ptSelf.Pos.iy = 7*Y_CELLS_PER_SUBGRID+1;
        }
    ptSelf.Pos.Facing = NORTH;
    ptLastPos = ptSelf.Pos;

    bSelfInSanct=TRUE;
    PrintTextLine("You materialize in the Sanctuary.");

    if (!SendNetMessage(0,0,&ptSelf.Pos,NP_MOVETO)) {
        MessageBox((HWND)NULL,"Unable to send Packet","MazeWndProc",
               MB_ICONEXCLAMATION|MB_APPLMODAL);
        }
    if (bBitmapDraw) {
        PostMessage(hWndMaze,WM_COMMAND,IDM_REDRAW,(DWORD)NULL);
        }
    else {
        InvalidateRect(hWndMaze,&rMaze,TRUE);
        }

    GetClientRect(hWndTopView,&rScratch);
    InvalidateRect(hWndTopView,&rScratch,TRUE);

    PostMessage(hWndScore,WM_COMMAND,WC_UPDATESCORE,ptSelf.ulID);
    PostMessage(hWndScore,WM_COMMAND,WC_UPDATEDIRECTION,0);
    iTimesKilled++;


    return;
}




/*=====================================================================
Function:   MazeWndProc()

Inputs:     Standard windows procedure inputs

Outputs:    returns success/failure

Abstract:
    MazeWndProc is the process responsible for drawing the maze 3-d view
    perspective window. It has the corresponding controls.
======================================================================*/

LONG FAR PASCAL MazeWndProc(
    HWND hWnd,
    UINT Message,
    WPARAM wParam,
    LPARAM lParam
    )
{
    HDC     hDC,hBMDC;
    int     nRc=0,i,ix,iy;
    BYTE bSquare1,bSquare2,Facing;
    PlayerType FAR *ptTrav;
    BOOL bKilled,bNewSelfInSanct,bHitSomething;
    PositionType ptPos;
    char cBuff[132];
    LPRECT rTemp,rPtr;
    HGLOBAL hMem;


    switch (Message) {

        case WM_CREATE:
            if (uiTimer != (UINT) NULL) {
                KillTimer((HWND)NULL,uiTimer);
                }

            if ((iNumDrones)&&(iDroneSpeed != 0)) {
                if (! (uiTimer = SetTimer((HWND) NULL,0,ONE_SECOND/iDroneSpeed,MoveDrone))) {
                    MessageBox(NULL,"Unable to create timer. Killing Game.","FATAL ERROR",
                               MB_ICONEXCLAMATION|MB_APPLMODAL);
                    PostMessage(hWndMain,WM_CLOSE,0,0);
                    }
                }

            InitDrones();

            if ((bBitmapDraw)&&(((HWND)hMaze3DBM) == ((HWND)NULL))) {
                hDC = GetDC(hWnd);
                GetClientRect(hWnd,&rMaze);
                hMazeDC = CreateCompatibleDC(hDC);
                hMaze3DBM = CreateCompatibleBitmap(hDC,rMaze.right-rMaze.left,
                                                  rMaze.bottom-rMaze.top);
                SelectObject(hMazeDC,hMaze3DBM);
                ReleaseDC(hWnd,hDC);
                }
            else {
                hMazeDC = GetDC(hWnd);
                }

            break;

        case WM_COMMAND:
            switch(wParam) {

                case IDM_DRAWPLAYERS:
                    DrawPlayers(hMazeDC,&ptPlayers,NULL);
                    break;

                case IDM_DRAWDRONES:
                    DrawPlayers(hMazeDC,&ptDrones,(LPRECT) lParam);
//                    if (lParam != (LPARAM) NULL) {
//                        hMem = (HGLOBAL) GlobalHandle(SELECTOROF( (LPRECT) lParam));
//                        GlobalUnlock(hMem);
//                        GlobalFree(hMem);
//                    }
                    break;

                case IDM_REDRAW:
                    hDC = GetDC(hWnd);
                    if (lParam != (LPARAM)NULL) {
                        rTemp = (LPRECT)lParam;
                        }
                    else {
                        hMem = GlobalAlloc(GHND,sizeof(RECT));
                        rTemp = (LPRECT) GlobalLock(hMem);
                        if (rTemp == NULL) {
                            MessageBox((HWND)NULL,"Unable to allocate memory. Exiting.","MazeWnd",
                                       MB_ICONEXCLAMATION|MB_APPLMODAL);
                            PostMessage(hWndMain,WM_CLOSE,0,0);
                            }
                        GetClientRect(hWnd,rTemp);
                        }

                    DrawMaze(hMazeDC,rTemp);

                    if (bBitmapDraw) {
                        BitBlt(hDC,rTemp->left, rTemp->top,
                               RECTWIDTH(*rTemp),
                               RECTDEPTH(*rTemp),
                               hMazeDC,rTemp->left,rTemp->top,SRCCOPY);
                        }

                    ReleaseDC(hWnd,hDC);
                    hMem = (HGLOBAL) GlobalHandle(SELECTOROF( rTemp));
                    GlobalUnlock(hMem);
                    GlobalFree(hMem);

                default:
                    break;
                }

            break;

        case WM_KEYDOWN:
            bSquare1 = bMaze[ptSelf.Pos.ix][ptSelf.Pos.iy];

            if ((! GamePaused)&&(GameStarted)) {

                switch(wParam) {
                    //
                    // Space = Fire straight ahead
                    //
                    case VK_SPACE:
                        if (InSanctuary(&ptSelf.Pos)) {
                            //
                            //If we shoot while in the Sanctuary, we get chastized.
                            //
                            sprintf(cBuff,"~<No violence allowed in the Sanctuary>~");
                            PrintTextLine(cBuff);
                            sndPlaySound("sanctu.wav",SND_SYNC);
                            }
                        else {
                            //
                            // Otherwise, fine, shoot.
                            //
                            hDC=GetDC(hWndMaze);
                            hBMDC = CreateCompatibleDC(hDC);
                            SelectObject(hBMDC,hShotBM[1]);
                            BitBlt(hDC,(rMaze.left+rMaze.right)/2 - 12,
                                    (rMaze.top+rMaze.bottom)/2 - 12,
                                    24,24,
                                    hBMDC,0,0,SRCINVERT);

                            if (!sndPlaySound("laser.wav",SND_ASYNC)) {
                                MessageBeep((UINT)-1);
                                }

                            //
                            // Tell everyone else we're firing */
                            //
                            if (!SendNetMessage(0,0,&ptSelf.Pos,NP_SHOTFIRED)) {
                                MessageBox((HWND)NULL,"Unable to send Packet","WndProc",
                                           MB_ICONEXCLAMATION|MB_APPLMODAL);
                                }

                            bHitSomething = FALSE;

                            //
                            // Check to see if we hit any drones
                            //
                            ix = ptSelf.Pos.ix;
                            iy = ptSelf.Pos.iy;
                            Facing = ptSelf.Pos.Facing;

                            bSquare2 = bMaze[ix][iy];

                            //
                            // for each square until the next wall, */
                            //
                            do {
                                ptTrav = &ptDrones;
                                        /* Loop through all drones to see if it's there */
                                while (ptTrav->next != NULL) {
                                    ptTrav = ptTrav->next;

                                    if ((ix == ptTrav->Pos.ix)&&(iy == ptTrav->Pos.iy)) {
                                        if (!sndPlaySound("squish.wav",SND_ASYNC)) {
                                            MessageBeep((UINT)-1);
                                            }

                                        FadePic(ptTrav->iPicNum,
                                                ptTrav->Pos.Facing,
                                                hMazeDC,
                                                &ptTrav->rFrom,
                                                &ptTrav->rDrawn);
                                        ptTrav->Pos.ix = RandRange(5*X_CELLS_PER_SUBGRID,8*X_CELLS_PER_SUBGRID-1);
                                        ptTrav->Pos.iy = RandRange(7*Y_CELLS_PER_SUBGRID,8*Y_CELLS_PER_SUBGRID-1);
                                        i = RandRange(1,4);
                                        ptTrav->Pos.Facing = ((i == 1) ? NORTH :
                                                      ((i == 2) ? WEST :
                                                      ((i == 3) ? SOUTH :
                                                      EAST)));
                                        if (bBitmapDraw) {
                                            hMem = GlobalAlloc(GHND,sizeof(RECT));
                                            rPtr = (LPRECT)GlobalLock(hMem);
                                            *rPtr = ptTrav->rDrawn;
                                            PostMessage(hWndMaze,WM_COMMAND,IDM_REDRAW,(DWORD)rPtr);
                                            }
                                        ptSelf.iScore += iDronesKilled*iDroneSpeed*iDroneSpeed*iNumDrones/20;
                                        SendNetMessage(0,0,NULL,NP_SCORE);
                                        iDronesKilled++;
                                        PostMessage(hWndScore,WM_COMMAND,WC_UPDATESCORE,ptSelf.ulID);
                                        bHitSomething = TRUE;
                                        sprintf(cBuff,"You zapped %s!",ptTrav->cUserName);
                                        PrintTextLine(cBuff);
                                        }
                                    }

                                bSquare1 = bSquare2;
                                bSquare2 = bMaze[ix = ADJ_X(ix,Facing)][iy = ADJ_Y(iy,Facing)];

                                } while (!(bHitSomething||
                                         ((bSquare1 & Facing)||(bSquare2 & BACK_TO_ABS(Facing)))));

                            if (!bHitSomething) {
                                Sleep(100);
                                BitBlt(hDC,(rMaze.left+rMaze.right)/2 - 12,
                                       (rMaze.top+rMaze.bottom)/2 - 12,
                                       24,24,
                                       hBMDC,0,0,SRCINVERT);
                                SelectObject(hBMDC,(HGDIOBJ)NULL);
                                }


                            DeleteDC(hBMDC);
                            ReleaseDC(hWndMaze,hDC);
                            }

                        break;


                    case VK_UP:
                        bSquare2 = bMaze[ADJ_X(ptSelf.Pos.ix,ptSelf.Pos.Facing)]
                                    [ADJ_Y(ptSelf.Pos.iy,ptSelf.Pos.Facing)];
                        if ((bSquare1 & ptSelf.Pos.Facing)||(bSquare2&BACK_TO_ABS(ptSelf.Pos.Facing))) {
                            if (!sndPlaySound("blocks.wav",SND_ASYNC)) {
                                MessageBeep((UINT) -1);
                                }
                            }
                        else {
                            ptLastPos = ptSelf.Pos;
                            ptSelf.Pos.ix = ADJ_X(ptSelf.Pos.ix,ptSelf.Pos.Facing);
                            ptSelf.Pos.iy = ADJ_Y(ptSelf.Pos.iy,ptSelf.Pos.Facing);
                            ptTrav = &ptDrones;
                            bKilled = FALSE;
                            while (ptTrav->next != NULL) {
                                ptTrav = ptTrav->next;
                                ptPos = ptTrav->Pos;
                                if ((!InSanctuary(&ptSelf.Pos))&&
                                    (ptSelf.Pos.ix == ptPos.ix)&&
                                    (ptSelf.Pos.iy == ptPos.iy)
                                   ) {
                                    sprintf(cBuff,"You ran into %s and it slew you!",ptTrav->cUserName);
                                    PrintTextLine(cBuff);
                                    iKilledByDrones++;
                                    ptSelf.iScore -= (iDroneSpeed) ?    36/iDroneSpeed/iDroneSpeed : 72;
                                    KillSelf();
                                    SendNetMessage(0,0,NULL,NP_SCORE);

                                    bKilled = TRUE;
                                    break;
                                    }
                                }

                            if (!bKilled) {
                                if (!SendNetMessage(0,0,&ptSelf.Pos,NP_MOVETO)) {
                                    MessageBox((HWND)NULL,"Unable to send Packet","MazeWndProc",
                                               MB_ICONEXCLAMATION|MB_APPLMODAL);
                                    }
                                bNewSelfInSanct = InSanctuary(&ptSelf.Pos);
                                if (bSelfInSanct != bNewSelfInSanct) {
                                    sprintf(cBuff,"You have %s the Sanctuary.",
                                            bNewSelfInSanct ? "entered" : "left");
                                    PrintTextLine(cBuff);
                                    bSelfInSanct = bNewSelfInSanct;
                                    }

                                PostMessage(hWnd,WM_COMMAND,IDM_REDRAW,(LPARAM)NULL);
                                hDC = GetDC(hWndTopView);
                                DrawTopView(hDC,FALSE);
                                ReleaseDC(hWndTopView,hDC);
                                PostMessage(hWndScore,WM_COMMAND,WC_UPDATEDIRECTION,0);
                                }
                            }
                        break;

                    case VK_LEFT:
                        ptLastPos = ptSelf.Pos;
                        ptSelf.Pos.Facing = LEFT_TO_ABS(ptSelf.Pos.Facing);
                        PostMessage(hWnd,WM_COMMAND,IDM_REDRAW,(LPARAM)NULL);
                        if (!SendNetMessage(0,0,&ptSelf.Pos,NP_MOVETO)) {
                            MessageBox((HWND)NULL,"Unable to send Packet","MazeWndProc",
                                        MB_ICONEXCLAMATION|MB_APPLMODAL);
                            }
                        hDC = GetDC(hWndTopView);
                        DrawTopView(hDC,FALSE);
                        ReleaseDC(hWndTopView,hDC);
                        PostMessage(hWndScore,WM_COMMAND,WC_UPDATEDIRECTION,0);
                        break;

                    case VK_RIGHT:
                        ptLastPos = ptSelf.Pos;
                        ptSelf.Pos.Facing = RIGHT_TO_ABS(ptSelf.Pos.Facing);
                        PostMessage(hWnd,WM_COMMAND,IDM_REDRAW,(LPARAM)NULL);
                        if (!SendNetMessage(0,0,&ptSelf.Pos,NP_MOVETO)) {
                            MessageBox((HWND)NULL,"Unable to send Packet","MazeWndProc",
                                             MB_ICONEXCLAMATION|MB_APPLMODAL);
                            }
                        hDC = GetDC(hWndTopView);
                        DrawTopView(hDC,FALSE);
                        ReleaseDC(hWndTopView,hDC);
                        PostMessage(hWndScore,WM_COMMAND,WC_UPDATEDIRECTION,0);
                        break;

                    case VK_DOWN:
                        bSquare2 = bMaze[ADJ_X(ptSelf.Pos.ix,BACK_TO_ABS(ptSelf.Pos.Facing))]
                                    [ADJ_Y(ptSelf.Pos.iy,BACK_TO_ABS(ptSelf.Pos.Facing))];
                        if ((bSquare2 & ptSelf.Pos.Facing)||(bSquare1&BACK_TO_ABS(ptSelf.Pos.Facing))) {
                            if (!sndPlaySound("blocks.wav",SND_ASYNC)) {
                                MessageBeep((UINT) -1);
                                }
                            }
                        else {
                            ptLastPos = ptSelf.Pos;

                            ptSelf.Pos.ix = ADJ_X(ptSelf.Pos.ix,BACK_TO_ABS(ptSelf.Pos.Facing));
                            ptSelf.Pos.iy = ADJ_Y(ptSelf.Pos.iy,BACK_TO_ABS(ptSelf.Pos.Facing));
                            ptTrav = &ptDrones;
                            bKilled = FALSE;
                            while (ptTrav->next != NULL) {
                                ptTrav = ptTrav->next;
                                ptPos = ptTrav->Pos;
                                if ((!InSanctuary(&ptSelf.Pos))&&
                                    (ptSelf.Pos.ix == ptPos.ix)&&
                                    (ptSelf.Pos.iy == ptPos.iy)
                                   ) {
                                    sprintf(cBuff,"You ran into %s and it slew you!",ptTrav->cUserName);
                                    PrintTextLine(cBuff);
                                    ptSelf.iScore -= (iDroneSpeed) ? 36/iDroneSpeed/iDroneSpeed : 72;
                                    SendNetMessage(0,0,NULL,NP_SCORE);

                                    iKilledByDrones++;
                                    KillSelf();
                                    bKilled = TRUE;
                                    break;
                                    }
                                }
                            if (!bKilled) {
                                if (!SendNetMessage(0,0,&ptSelf.Pos,NP_MOVETO)) {
                                    MessageBox((HWND)NULL,"Unable to send Packet","MazeWndProc",
                                               MB_ICONEXCLAMATION|MB_APPLMODAL);
                                    }
                                bNewSelfInSanct = InSanctuary(&ptSelf.Pos);
                                if (bSelfInSanct != bNewSelfInSanct) {
                                    sprintf(cBuff,"You have %s the Sanctuary.",
                                            bNewSelfInSanct ? "entered" : "left");
                                    PrintTextLine(cBuff);
                                    bSelfInSanct = bNewSelfInSanct;
                                    }
                                PostMessage(hWnd,WM_COMMAND,IDM_REDRAW,(LPARAM)NULL);
                                hDC = GetDC(hWndTopView);
                                DrawTopView(hDC,FALSE);
                                ReleaseDC(hWndTopView,hDC);
                                PostMessage(hWndScore,WM_COMMAND,WC_UPDATEDIRECTION,0);
                                }
                            }
                        break;

                    //
                    // Whisper something
                    //
                    case 'W':
                    case 'w':
                        iLoudness = iWhisperDist;
                        DialogBox(hInst,"INTONE_DLG", hWnd, IntoneDlg);
                        iWhisperDist = iLoudness;
                        break;
                    //
                    // Shout something
                    //
                    case 'S':
                    case 's':
                        iLoudness = iShoutDist;
                        DialogBox(hInst,"INTONE_DLG", hWnd, IntoneDlg);
                        iShoutDist = iLoudness;
                        break;
                    default:
                        SendMessage(hWnd,WM_KEYDOWN,wParam,lParam);
                        break;
                    }
                }
            break;

        case WM_MOVE:
            break;

        case WM_SIZE:
            GetClientRect(hWnd,&rMaze);
            Calc3DMaze();
            StretchBitmaps();
            if (bBitmapDraw) {
                if (((HWND)hMaze3DBM) != (HWND)NULL) {
                    DeleteObject(hMaze3DBM);
                    }
                hDC = GetDC(hWnd);
                if (hMaze3DBM != NULL) {
                    DeleteObject(hMaze3DBM);
                }
                hMaze3DBM = CreateCompatibleBitmap(hDC,rMaze.right-rMaze.left,
                                                   rMaze.bottom-rMaze.top);
                SelectObject(hMazeDC,hMaze3DBM);
                ReleaseDC(hWnd,hDC);
                }
            break;

        case WM_PAINT:
            hMem = GlobalAlloc(GHND,sizeof(RECT));
            rTemp = (LPRECT) GlobalLock(hMem);
            if (rTemp == NULL) {
                MessageBox((HWND)NULL,"Unable to allocate memory. Exiting.","MazeWnd",
                           MB_ICONEXCLAMATION|MB_APPLMODAL);
                PostMessage(hWndMain,WM_CLOSE,0,0);
                }
            GetUpdateRect(hWnd,rTemp,FALSE);
            ValidateRect(hWnd,rTemp);
            PostMessage(hWnd,WM_COMMAND,IDM_REDRAW,(LPARAM)rTemp);
            break;

        case WM_CLOSE:
            KillTimer((HWND)NULL,uiTimer);
#ifdef WIN32
            //
            // Destroy the mailslot.
            //
            if (hMailSlot != (HANDLE)NULL) {
                CloseHandle(hMailSlot);
                }
#endif
            DestroyWindow(hWnd);
            if (((HWND)hMaze3DBM) != (HWND)NULL) {
                DeleteObject(hMaze3DBM);
                DeleteDC(hMazeDC);
                }
            if (hWnd == hWndMain) {
                PostQuitMessage(0);
                }
            break;

        default:
            return DefWindowProc(hWnd, Message, wParam, lParam);
            break;
        }

    return(0);
}
