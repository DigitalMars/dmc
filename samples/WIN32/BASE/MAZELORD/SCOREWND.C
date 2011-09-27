/***********************************************************************
File:   ScoreWnd.c
Author: JohnMil
Date:   7-22-92


Abstract:

    This module contains functions to maintain the Score window.


Contents:

    DelScoreType() -- Delete a record of type ScoreType
    NewScoreType() -- Create & initialize a record of type ScoreType
    DrawScore() -- Draws a score entry with parametric update selection
    ScoreWndProc() -- Main windows entrypoint for window with scores


Revision History:

10-30-92 (JohnMil):
    Modified to new personal comment format
************************************************************************/

#include "winmaze.h"
#include "mazproto.h"
#include "net.h"
#include <mmsystem.h>
#include <string.h>


TEXTMETRIC tmScoreInfo;
RECT rDirRect[4],rBitmapRect; // These will need ix and iy added to be right.

int iTextYPos;

#define SC_PIC    0x1
#define SC_NAME   0x2
#define SC_SCORE  0x4
#define SC_DIRECT 0x8
#define SC_REDRAW 0x10




/*=====================================================================
Function:   DelScoreType()

Inputs:     Pointer to record of ScoreType to delete

Outputs:    returns ->next field of deleted record

Abstract:
    Returns ->next field to facilitate linked list maintenance
======================================================================*/

ScoreType FAR *DelScoreType(
    ScoreType FAR *stTrav
    )
{
    ScoreType FAR *stHold;
    HGLOBAL hMem;

    if (stTrav == (ScoreType FAR *) NULL) {
        stHold = (ScoreType FAR *) NULL;
        }
    else {
        stHold = stTrav->next;
        hMem = (HGLOBAL) GlobalHandle(SELECTOROF(stTrav));
        GlobalUnlock(hMem);
        GlobalFree(hMem);
        }

    return(stHold);
}



/*=====================================================================
Function:   NewScoreType()

Inputs:     Values for ScoreType member elements

Outputs:    Returns pointer to a newly allocated & Init'd record

Abstract:
    Needs no explanation
======================================================================*/

ScoreType FAR *NewScoreType(
    unsigned long ulID,
    int ix,
    int iy,
    int iScore,
    BYTE bDir,
    ScoreType FAR *next
    )
{
    ScoreType FAR *stTrav;
    HGLOBAL hMem;

    hMem = GlobalAlloc(GHND,sizeof(ScoreType));
    stTrav = (ScoreType FAR *) GlobalLock(hMem);
    if (stTrav == (ScoreType FAR *)NULL) {
        MessageBox((HWND)NULL,"ERROR: Unable to allocate memory","NewScoreType",
               MB_ICONEXCLAMATION|MB_APPLMODAL);
        }
    else {
        stTrav->ulID = ulID;
        stTrav->ix = ix;
        stTrav->iy = iy;
        stTrav->iScore = iScore;
        stTrav->bDir = bDir;
        stTrav->next = next;
        }

    return(stTrav);
}






/*=====================================================================
Function:   DrawScore()

Inputs:     Pointer to score structure for to draw, flags for what to update

Outputs:    none

Abstract:
    DrawScore is the workhorse routine for drawing score entries.
======================================================================*/

void DrawScore(
    ScoreType FAR *stTrav,
    DWORD dwSCFlags
    )
{
    char Buff[132];
    UINT uiAlign;
    int x,y,px,py;
    PlayerType FAR *ptTrav;
    FullPicType FAR *fptTrav;
    HBRUSH hWhiteBrush,hBlackBrush,hOld;
    HPEN hWhitePen, hOldPen;
    HDC hDC,hPicDC;
    RECT rScratch;
    BOOL bFound,bIsSelf,bFound2;
    BYTE bDir,bNewDir,bB;
    float fAspect;


    bFound = FALSE;
    ptTrav = &ptPlayers;

    hWhiteBrush = GetStockObject(WHITE_BRUSH);
    hWhitePen = GetStockObject(WHITE_PEN);

    //
    // Find the player structure associated with the score we're updating
    //
    while (ptTrav->next != NULL) {
        ptTrav = ptTrav->next;
        if (ptTrav->ulID == stTrav->ulID) {
            bFound = TRUE;
            break;
            }
        }

    if (!bFound) {
        bIsSelf = (stTrav->ulID == ptSelf.ulID);
        }
    else {
        bIsSelf = FALSE;
        }


    hDC = GetDC(hWndScore);

    //
    // Play the dog-being-kicked sound if the player is doing bad.
    //
    if (stTrav->ulID == ptSelf.ulID) {
        if (iKilledByDrones&&(!(iKilledByDrones%5))&&(stTrav->iScore > ptSelf.iScore)) {
            sndPlaySound("dog.wav",SND_ASYNC);
            }
        }

    //
    // Draw a profile of the player if appropriate
    //
    if ((dwSCFlags&SC_PIC)&&(dwSCFlags&SC_REDRAW)) {
        rScratch.left = rBitmapRect.left + stTrav->ix;
        rScratch.right = rBitmapRect.right + stTrav->ix;
        rScratch.top = rBitmapRect.top + stTrav->iy;
        rScratch.bottom = rBitmapRect.bottom + stTrav->iy;

        hPicDC = CreateCompatibleDC(hDC);
        fptTrav = &fptPic;
        bFound2 = FALSE;
        while (fptTrav->next != (FullPicType FAR *)NULL) {
            fptTrav = fptTrav->next;
            if (fptTrav->iPicNum == ((bIsSelf) ? ptSelf.iPicNum : ptTrav->iPicNum)) {
                bFound2 = TRUE;
                break;
                }
            }

        if (bFound2) {
            SelectObject(hPicDC,fptTrav->P[1].hBitmap);
            x = rScratch.right - rScratch.left;
            y = rScratch.bottom - rScratch.top;
            fAspect=((float) fptTrav->P[1].ySize)/
                     ((float) fptTrav->P[1].xSize);

            StretchBlt(hDC,rScratch.left,
                       rScratch.top+fptTrav->P[1].yOrg*y/PIC_Y,
                       (int) ((x < (y/fAspect))? x : y/fAspect),
                       (int) ((y < (x*fAspect))? y : (int) x*fAspect),
                       hPicDC,0,0,
                       fptTrav->P[1].xSize,
                       fptTrav->P[1].ySize,
                       SRCCOPY);
            }

        DeleteDC(hPicDC);
        }

    //
    // Print the User & computer name of the player if appropriate
    //
    if ((dwSCFlags&SC_NAME)&&(dwSCFlags&SC_REDRAW)) {
        if (bIsSelf) {
            sprintf(Buff,"%s@%s",ptSelf.cUserName,ptSelf.cComputerName);
            }
        else {
           if (bFound) {
               sprintf(Buff,"%s@%s",ptTrav->cUserName,ptTrav->cComputerName);
               }
           }
        TextOut(hDC,80,iTextYPos+stTrav->iy,Buff,lstrlen(Buff));
        }

    //
    // Print the current score of the player if appropriate
    //
    if ((dwSCFlags&SC_SCORE)&&
        ((dwSCFlags&SC_REDRAW)||
        (bFound&&(ptTrav->iScore != stTrav->iScore)) ||
        (bIsSelf&&(ptSelf.iScore != stTrav->iScore)))
       ) {

        hOldPen = SelectObject(hDC,hWhitePen);
        hOld = SelectObject(hDC,hWhiteBrush);
        sprintf(Buff,"%ld",stTrav->iScore);

        Rectangle(hDC,rScore.right-5-((strlen(Buff)+1)* tmScoreInfo.tmAveCharWidth),
                  iTextYPos+stTrav->iy,rScore.right-5,
                  iTextYPos+stTrav->iy + tmScoreInfo.tmHeight);
        SelectObject(hDC,hOldPen);
        SelectObject(hDC,hOld);

        if (bIsSelf) {
            sprintf(Buff,"%ld",ptSelf.iScore);
            stTrav->iScore = ptSelf.iScore;
            }
        else {
            if (bFound) {
                sprintf(Buff,"%ld",ptTrav->iScore);
                stTrav->iScore = ptTrav->iScore;
                }
            else {
                MessageBox((HWND)NULL,"Modify score for unknown person","DrawScore",
                           MB_ICONEXCLAMATION|MB_APPLMODAL);
                }
            }

        uiAlign = SetTextAlign(hDC,TA_TOP | TA_RIGHT);
        TextOut(hDC,rScore.right-5,iTextYPos+stTrav->iy,Buff,lstrlen(Buff));
        SetTextAlign(hDC,uiAlign);
        }

    //
    // Modify the squares which say which direction to go to find this
    // particular player, if appropriate.
    //
    if (bFound&&(dwSCFlags&SC_DIRECT)) {
        x = ptSelf.Pos.ix;
        y = ptSelf.Pos.iy;

        hBlackBrush = GetStockObject(BLACK_BRUSH);

        px = ptTrav->Pos.ix;
        py = ptTrav->Pos.iy;
        bDir = stTrav->bDir;

        bNewDir = (py <= y) ? NORTH : (BYTE)0;
        bNewDir |= (py >= y) ? SOUTH : (BYTE)0;
        bNewDir |= (px >= x) ? EAST : (BYTE)0;
        bNewDir |= (px <= x) ? WEST : (BYTE)0;

        //
        // Now that we have the ABSOLUTE directions, we need to rotate it for
        // our offset from true north.
        //

        bB = ptSelf.Pos.Facing;

        while (bB /=2 ) {
            if (bNewDir & 1) {
                bNewDir = bNewDir / 2 + 8;
                }
            else {
                bNewDir /= 2;
                }
/****
            if (bNewDir & 16) {
                bNewDir = (bNewDir & 15) + 1;
                }
****/
            }

        //
        // Draw square #0
        //
        if ((dwSCFlags&SC_REDRAW)||
            ((bNewDir&(NORTH|WEST)) != (bDir&(NORTH|WEST)))
           ) {
            if ((bNewDir&NORTH)&&(bNewDir&WEST)) {
                hOld = SelectObject(hDC,hBlackBrush);
                }
            else {
                hOld = SelectObject(hDC,hWhiteBrush);
                }

            Rectangle(hDC,rDirRect[0].left+stTrav->ix,rDirRect[0].top+stTrav->iy,
                      rDirRect[0].right+stTrav->ix,rDirRect[0].bottom+stTrav->iy);
            SelectObject(hDC,hOld);
            }

        //
        // Draw square #1
        //
        if ((dwSCFlags&SC_REDRAW)||
            ((bNewDir&(NORTH|EAST)) != (bDir&(NORTH|EAST)))
           ) {
            if ((bNewDir&NORTH)&&(bNewDir&EAST)) {
                hOld = SelectObject(hDC,hBlackBrush);
                }
            else {
                hOld = SelectObject(hDC,hWhiteBrush);
                }

            Rectangle(hDC,rDirRect[1].left+stTrav->ix,rDirRect[1].top+stTrav->iy,
                      rDirRect[1].right+stTrav->ix,rDirRect[1].bottom+stTrav->iy);
            SelectObject(hDC,hOld);
            }

        //
        // Draw square #2
        //
        if ((dwSCFlags&SC_REDRAW)||
            ((bNewDir&(SOUTH|WEST)) != (bDir&(SOUTH|WEST)))
           ) {
            if ((bNewDir&SOUTH)&&(bNewDir&WEST)) {
                hOld = SelectObject(hDC,hBlackBrush);
                }
            else {
                hOld = SelectObject(hDC,hWhiteBrush);
                }

            Rectangle(hDC,rDirRect[2].left+stTrav->ix,rDirRect[2].top+stTrav->iy,
                      rDirRect[2].right+stTrav->ix,rDirRect[2].bottom+stTrav->iy);
            SelectObject(hDC,hOld);
            }

        //
        // Draw square #1
        //
        if ((dwSCFlags&SC_REDRAW)||
            ((bNewDir&(SOUTH|EAST)) != (bDir&(SOUTH|EAST)))
           ) {
            if ((bNewDir&SOUTH)&&(bNewDir&EAST)) {
                hOld = SelectObject(hDC,hBlackBrush);
                }
            else {
                hOld = SelectObject(hDC,hWhiteBrush);
                }

            Rectangle(hDC,rDirRect[3].left+stTrav->ix,rDirRect[3].top+stTrav->iy,
                      rDirRect[3].right+stTrav->ix,rDirRect[3].bottom+stTrav->iy);
            SelectObject(hDC,hOld);
            }

        stTrav->bDir = bNewDir;

        DeleteObject(hBlackBrush);
        }

    DeleteObject(hWhiteBrush);
    DeleteObject(hWhitePen);
    ReleaseDC(hWndScore,hDC);

    return;
}




/*=====================================================================
Function:   ScoreWndProc()

Inputs:     Standard Windows Proc parms

Outputs:    returns success/failure

Abstract:
    Handles updating/controlling the score child window
======================================================================*/

LONG FAR PASCAL ScoreWndProc(
    HWND hWnd,
    UINT Message,
    WPARAM wParam,
    LPARAM lParam
    )
{
    PAINTSTRUCT ps;
    HDC hDC;
    ScoreType FAR *stTrav,FAR *stTrav2;
    BOOL bFound,bFoundpt,bIsSelf;
    PlayerType FAR *ptTrav;
    RECT rScratch;
    HPEN hWhitePen,hOldPen;


    bIsSelf = (lParam == (LPARAM) ptSelf.ulID);
    bFoundpt = FALSE;
    ptTrav=&ptPlayers;
    while(ptTrav->next != NULL) {
        ptTrav = ptTrav->next;
        if (ptTrav->ulID == (UINT) lParam) {
            bFoundpt = TRUE;
            break;
            }
        }

    switch (Message) {
        case WM_CREATE:
            GetClientRect(hWnd,&rScore);
            hDC = GetDC(hWnd);

            //
            // Kludge: fill out scores with our own entry
            //
            Scores.ulID = ptSelf.ulID;
            Scores.ix = 0;
            Scores.iy = 0;
            Scores.iScore = ptSelf.iScore;
            Scores.bDir = 0xFF;
            Scores.next=NULL;

            if(!GetTextMetrics(hDC,&tmScoreInfo)) {
                MessageBox((HWND)NULL,"Unable to get text metrics. Scores won't work right.",
                           "ScoreWndProc",MB_ICONEXCLAMATION|MB_APPLMODAL);
                }

            rBitmapRect.left = 40;
            rBitmapRect.right = rBitmapRect.left + 19;
            rBitmapRect.top = 5;
            rBitmapRect.bottom = rBitmapRect.top+29;

            //
            // 01
            // 23
            //
            rDirRect[0].left = rDirRect[2].left = 5;
            rDirRect[1].left = rDirRect[3].left = rDirRect[0].right = rDirRect[2].right = 20;
            rDirRect[1].right = rDirRect[3].right = 35;
            rDirRect[0].top     = rDirRect[1].top = 5;
            rDirRect[2].top     = rDirRect[3].top = rDirRect[0].bottom = rDirRect[1].bottom = 20;
            rDirRect[2].bottom = rDirRect[3].bottom = 35;

            iTextYPos = rBitmapRect.top + (rBitmapRect.bottom - tmScoreInfo.tmHeight - rBitmapRect.top)/2;

            ReleaseDC(hWnd,hDC);
            break;

        case WM_COMMAND:
            //
            // Note -- we use lParam because we pass WM_COMMAND msg's ourselves.
            //
            bFound = FALSE;
            stTrav = &Scores;
            while (stTrav->next != NULL) {
                if (stTrav->next->ulID == (unsigned long)lParam) {
                    bFound = TRUE;
                    break;
                    }
                stTrav = stTrav->next;
                }

            switch (wParam) {

                case WC_ADDSCORE:
                    if (!bFound) {
                        if ((lParam != 0)&&bFoundpt) {
                            stTrav->next = NewScoreType(ptTrav->ulID,stTrav->ix,
                                                        stTrav->iy+35,ptTrav->iScore,0,stTrav->next);
                            DrawScore(stTrav->next,SC_PIC|SC_NAME|SC_SCORE|SC_DIRECT|SC_REDRAW);
                            }
                        else {
                            MessageBox((HWND)NULL,"Attempted to ADD 0 for SCORE ulID","ScoreDlg",
                                       MB_ICONEXCLAMATION|MB_APPLMODAL);
                            }
                        }
                    break;

                case WC_UPDATESCORE:
                    if (bFound&&bFoundpt) {
                        DrawScore(stTrav->next,SC_SCORE);
                        }
                    else {
                        if (bIsSelf) {
                            DrawScore(&Scores,SC_SCORE);
                            }
                        else {
                            MessageBox((HWND)NULL,"Attempted to update nonexistant score.","ScoreDlg",
                                       MB_ICONEXCLAMATION|MB_APPLMODAL);
                            }
                        }
                    break;

                case WC_UPDATEDIRECTION:
                    //
                    // if it's a global update
                    //
                    if (lParam == 0) {
                        stTrav = &Scores;
                        while (stTrav->next != NULL) {
                            stTrav = stTrav->next;
                            DrawScore(stTrav,SC_DIRECT);
                            }
                        }
                    else {
                        if (bFound&&bFoundpt) {
                            //
                            // must be updated in routine
                            //
                            DrawScore(stTrav->next,SC_DIRECT);
                            }
                        else {
                            MessageBox((HWND)NULL,"Attempted to update Direction.","ScoreDlg",
                                       MB_ICONEXCLAMATION|MB_APPLMODAL);
                            }
                        }
                    break;

                case WC_DELETESCORE:
                    if (bFound) {
                        stTrav2 = stTrav;
                        stTrav->next = DelScoreType(stTrav->next);
                        while (stTrav2->next != NULL) {
                            DrawScore(stTrav2->next,SC_PIC|SC_NAME|SC_SCORE|SC_DIRECT|SC_REDRAW);
                            stTrav2 = stTrav2->next;
                            }
                        rScratch.top = rDirRect[0].top + stTrav->iy + 35;
                        rScratch.bottom = rDirRect[2].bottom + stTrav->iy + 35;
                        rScratch.left = 5;
                        rScratch.right = rScore.right - 5;
                        hDC = GetDC(hWndScore);
                        hWhitePen = GetStockObject(WHITE_PEN);
                        hOldPen = SelectObject(hDC,hWhitePen);
                        Rectangle(hDC,rScratch.left,rScratch.top,
                                  rScratch.right,rScratch.bottom);
                        SelectObject(hDC,hOldPen);
                        DeleteObject(hWhitePen);
                        ReleaseDC(hWndScore,hDC);
                        }
                    else {
                        MessageBox((HWND)NULL,"Attempted to delete nonexistant score.","ScoreDlg",
                                   MB_ICONEXCLAMATION|MB_APPLMODAL);
                        }
                    break;

                default:
                    MessageBox((HWND)NULL,"Unknown WM_COMMAND message.","ScoreDlg",
                               MB_ICONEXCLAMATION|MB_APPLMODAL);
                    break;
                }
            break;

        case WM_KEYDOWN:
            SendMessage(hWndMaze,WM_KEYDOWN,wParam,lParam);
            break;

        case WM_MOVE:
            break;

        case WM_SIZE:
//
// BUGBUG -- eventually we'll want to soft-code the now hard-coded rects.
//
            break;

        case WM_PAINT:
            GetClientRect(hWnd,&rScore);
            hDC = BeginPaint(hWnd, &ps);
//
// OPTIMIZE -- this can be set to loop through only visible portions
//
            stTrav = &Scores;
            SetBkMode(hDC, TRANSPARENT);
            do {
                DrawScore(stTrav,SC_PIC|SC_NAME|SC_SCORE|SC_DIRECT|SC_REDRAW);
                stTrav = stTrav->next;
                } while (stTrav != NULL);
            EndPaint(hWnd, &ps);
            break;

        case WM_CLOSE:
            stTrav = &Scores;
            while (stTrav->next != NULL) {
                stTrav->next = DelScoreType(stTrav->next);
            }
            DestroyWindow(hWnd);
            break;

        default:
            return DefWindowProc(hWnd, Message, wParam, lParam);
        }

    return(0);
}
