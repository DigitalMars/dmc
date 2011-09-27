/***********************************************************************
File:   MazeDlg.c
Author: JohnMil
Date:   7-23-92


Abstract:

    This module contains dialog boxes for Maze Lords.


Contents:

    DroneDlg() -- Drone Dialog Box winproc
    AboutDlg() -- About Box
    IntoneDlg() -- Dialog box for spoken messages
    PlayerDlg() -- Dialog box for selecting player picture/maze
    cwCenter() -- Center a given dialog box in the window


Revision History:

10-30-92 (JohnMil):
    Modified to new personal comment format

************************************************************************/

#include "winmaze.h"
#include "mazproto.h"
#include "mazedlg.h"



int iCurPic,iCurMaze;


/*=====================================================================
Function:   DroneDlg()

Inputs:     Standard Dialog inputs

Outputs:    Returns close status

Abstract:
    This dialog box allows the user to input # of drones and their movement
    speed.
======================================================================*/

BOOL FAR PASCAL DroneDlg(
    HWND hWndDlg,
    UINT Message,
    WPARAM wParam,
    LPARAM lParam
    )
{
    BOOL bRet;  // holding variable only.
    UINT uCmdId,uCmdCmd;
    HWND hCmd;
    static INT iOldNumDrones,iOldDroneSpeed;
    int i;
    char c[132];


    switch(Message) {
        case WM_INITDIALOG:
            cwCenter(hWndDlg, 0);
            iOldNumDrones = iNumDrones;
            iOldDroneSpeed = iDroneSpeed;
            SetDlgItemInt(hWndDlg,DLG_NUMDRONES,iNumDrones,TRUE);
            SetDlgItemInt(hWndDlg,DLG_MOVESPEED,iDroneSpeed,TRUE);
            break;

        case WM_CLOSE:
            PostMessage(hWndDlg, WM_COMMAND, IDCANCEL, 0L);
            break;

        case WM_COMMAND:
            uCmdId = GET_WM_COMMAND_ID(wParam,lParam);
            uCmdCmd = GET_WM_COMMAND_CMD(wParam,lParam);
            hCmd = GET_WM_COMMAND_HWND(wParam,lParam);
            switch(uCmdId) {
                case DLG_NUMDRONES:
                    if (uCmdCmd == EN_CHANGE) {
                        i = GetDlgItemInt(hWndDlg,DLG_NUMDRONES,&bRet,TRUE);
                        if ((i < 0)||(i > MAX_DRONES)) {
                            MessageBeep(MB_ICONHAND);
                            sprintf(c,"Number of Drones must be between 0 and %d.",
                                    MAX_DRONES);
                            MessageBox(hWndDlg,c,"Input Error",MB_APPLMODAL|MB_ICONHAND);
                            SetDlgItemInt(hWndDlg,DLG_NUMDRONES,iOldNumDrones,TRUE);
                        }

                    }
                    break;

                case DLG_MOVESPEED:
                    if (uCmdCmd == EN_CHANGE) {
                        i = GetDlgItemInt(hWndDlg,DLG_MOVESPEED,&bRet,TRUE);
                        if ((i < 0)||(i > MAX_DRONE_SPEED)) {
                            MessageBeep(MB_ICONHAND);
                            sprintf(c,"Moves/Second must be between 0 and %d.",
                                    MAX_DRONE_SPEED);
                            MessageBox(hWndDlg,c,"Input Error",MB_APPLMODAL|MB_ICONHAND);
                            SetDlgItemInt(hWndDlg,DLG_MOVESPEED,iOldDroneSpeed,TRUE);
                        }

                    }
                    break;

                case IDOK:
                    iNumDrones = GetDlgItemInt(hWndDlg,DLG_NUMDRONES,&bRet,TRUE);
                    iDroneSpeed = GetDlgItemInt(hWndDlg,DLG_MOVESPEED,&bRet,TRUE);
                    if ((iOldNumDrones != iNumDrones)&&GameStarted) {
                        PostMessage(hWndMaze,WM_COMMAND,IDM_REDRAW,(DWORD)NULL);
                        }

                    EndDialog(hWndDlg, TRUE);
                    break;

                case IDCANCEL:
                    EndDialog(hWndDlg, FALSE);
                    break;
                }
            break;

        default:
            return FALSE;
        }

    return(TRUE);
}



/*=====================================================================
Function:   AboutDlg()

Inputs:     Standard Dialog box parms

Outputs:    Returns close status

Abstract:
    This procedure displays the About dialog box.
======================================================================*/

BOOL FAR PASCAL AboutDlg(
    HWND hWndDlg,
    UINT Message,
    WPARAM wParam,
    LPARAM lParam
    )
{
    UINT uCmdId,uCmdCmd;
    HWND hCmd;

    switch(Message) {

        case WM_INITDIALOG:
            cwCenter(hWndDlg, 0);
            break;

        case WM_CLOSE:
            PostMessage(hWndDlg, WM_COMMAND, IDCANCEL, 0L);
            break;

        case WM_COMMAND:
            uCmdId = GET_WM_COMMAND_ID(wParam,lParam);
            uCmdCmd = GET_WM_COMMAND_CMD(wParam,lParam);
            hCmd = GET_WM_COMMAND_HWND(wParam,lParam);
            switch(uCmdId) {
                case IDOK:
                    EndDialog(hWndDlg, TRUE);
                    break;
                }
            break;

        default:
            return FALSE;
        }

    return(TRUE);
}


/*=====================================================================
Function:   IntoneDlg()

Inputs:     Standard Dialog Box Parms

Outputs:    Returns dialog close conditions

Abstract:
    This dialog allows the user to type a message to whisper or shout.
    It is called by both commands, with the loudness having been set
    before the dialog was invoked by the method it was invoked, either
    whispering or shouting.
======================================================================*/

BOOL FAR PASCAL IntoneDlg(
    HWND hWndDlg,
    UINT Message,
    WPARAM wParam,
    LPARAM lParam
    )
{
    IntoneType itTemp;
    BOOL bScratch;
    UINT uCmdId;

    switch(Message) {

        case WM_INITDIALOG:
            cwCenter(hWndDlg, 0);
            SetDlgItemInt(hWndDlg,DLG_INT_LOUDNESS,iLoudness,TRUE);
            SetDlgItemText(hWndDlg,DLG_INT_SAY,"");
            break;

        case WM_CLOSE:
            PostMessage(hWndDlg, WM_COMMAND, IDCANCEL, 0L);
            break;

        case WM_COMMAND:
            uCmdId = GET_WM_COMMAND_ID(wParam,lParam);
            switch(uCmdId) {

                case DLG_INT_LOUDNESS:
                    break;

                case DLG_INT_SAY:
                    break;

                case IDOK:
                    GetDlgItemText(hWndDlg,DLG_INT_SAY,itTemp.cBuff,131);
                    itTemp.dwLen=lstrlen(itTemp.cBuff);
                    iLoudness = GetDlgItemInt(hWndDlg,DLG_INT_LOUDNESS,&bScratch,FALSE);
                    itTemp.iLoudness= iLoudness;
                    itTemp.Pos = ptSelf.Pos;
                    SendNetMessage(0,0,&itTemp,NP_INTONE);
                    EndDialog(hWndDlg, TRUE);
                    break;

                case IDCANCEL:
                    EndDialog(hWndDlg, FALSE);
                    break;
                }
            break;

        default:
            return FALSE;
        }


    return(TRUE);
}



/*=====================================================================
Function:   PlayerDlg()

Inputs:     Standard Dialog box parms

Outputs:    returns dialog termination condition

Abstract:
    This dialog box uses two custom list-boxes to allow the player to
    select options. The first contains all possible player bitmaps. The
    second contains all possible player sub-grids to choose from.
======================================================================*/

BOOL FAR PASCAL PlayerDlg(
    HWND hWndDlg,
    UINT Message,
    WPARAM wParam,
    LPARAM lParam
    )
{
    int i,j,k,x,y;
    MEASUREITEMSTRUCT FAR *miInfo;
    DRAWITEMSTRUCT FAR *diInfo;
    HDC hDC,hPicDC;
    FullPicType FAR *fptTrav;
    BOOL bFound;
    LPPOINT p;
//[5*5*5*2+2];
    LPBYTE bType;
//[5*5*5*2+2],
    BYTE b;
    int iNumPts,Step;
    HGLOBAL hPMem,hBMem;
    float fAspect;


    switch(Message) {

        case WM_INITDIALOG:
            cwCenter(hWndDlg, 0);

            iCurPic = iCurMaze = 0;
            if (PIC_DRONE == 0) {
                iCurPic = 1;
                }
            for (i=0;i<NUM_PICS;i++) {
                if (i != PIC_DRONE) {
                    AddPic(i);
                    SendDlgItemMessage(hWndDlg,PC_DLG_PIC,LB_ADDSTRING,0,i);
                    }
                }
            for (i=1;i<NUM_SUBGRIDS-1;i++) {
                SendDlgItemMessage(hWndDlg,PC_DLG_MAZE,LB_ADDSTRING,0,i);
                }
            break;

        case WM_MEASUREITEM:
            miInfo = (MEASUREITEMSTRUCT FAR *) lParam;
            miInfo->CtlType = ODT_LISTBOX;
            miInfo->itemID = 0;     // Not used

            if (wParam == PC_DLG_PIC) {
                miInfo->CtlID = PC_DLG_PIC;
                miInfo->itemWidth = 20;
                miInfo->itemHeight = 45;
                miInfo->itemData = 0;
                }
            else {
                miInfo->CtlID = PC_DLG_MAZE;
                miInfo->itemWidth = 30;
                miInfo->itemHeight = 45;
                miInfo->itemData = 0;
                }
            break;

        case WM_DRAWITEM:

            diInfo = (DRAWITEMSTRUCT FAR *) lParam;
            hDC = diInfo->hDC;
            switch(diInfo->CtlID) {

                case PC_DLG_PIC:
                    if (diInfo->itemAction == ODA_DRAWENTIRE) {
                        hPicDC = CreateCompatibleDC(hDC);
                        fptTrav = &fptPic;
                        bFound = FALSE;
                        while (fptTrav->next != (FullPicType FAR *)NULL) {
                            fptTrav = fptTrav->next;
                            if (fptTrav->iPicNum == (int)diInfo->itemData) {
                                bFound = TRUE;
                                break;
                                }
                            }

                        if (bFound) {
                            iCurPic = (int)diInfo->itemData;
                            SelectObject(hPicDC,fptTrav->P[1].hBitmap);
                            x = diInfo->rcItem.right - diInfo->rcItem.left;
                            y = diInfo->rcItem.bottom - diInfo->rcItem.top;
                            fAspect=((float) fptTrav->P[1].ySize)/
                                    ((float) fptTrav->P[1].xSize);

                            StretchBlt(hDC,diInfo->rcItem.left,diInfo->rcItem.top,
                                       (int) ((x < (y/fAspect))? x : y/fAspect),
                                       (int) ((y < (x*fAspect))? y : x*fAspect),
                                       hPicDC,0,0,
                                       fptTrav->P[1].xSize,
                                       fptTrav->P[1].ySize,
                                       SRCCOPY);
                            }

                        DeleteDC(hPicDC);
                        }

                    break;

                case PC_DLG_MAZE:
                    hPMem = GlobalAlloc(GHND,(3*5*5*4*2 + 5)*sizeof(POINT));
                    p = (LPPOINT) GlobalLock(hPMem);
                    hBMem = GlobalAlloc(GHND,(3*5*5*4*2 + 5)*sizeof(BYTE));
                    bType = (LPBYTE) GlobalLock(hBMem);

                    if (diInfo->itemAction == ODA_DRAWENTIRE) {
                        x = diInfo->rcItem.left;
                        y = diInfo->rcItem.top;
                        k = (int)diInfo->itemData;
                        iNumPts = 0;
                        Step = 9;
                        iCurMaze = (int)diInfo->itemData;

                        for (i = 0;i<5; i++) {
                            for (j= 0; j<5; j++) {
                                b = SubGrids[k].Cell[i][j];
                                if (b&NORTH) {
                                    p[iNumPts].x = i*Step;
                                    p[iNumPts].y = j*Step;
                                    bType[iNumPts++] = PT_MOVETO;
                                    p[iNumPts].x = (i+1)*Step;
                                    p[iNumPts].y = j*Step;
                                    bType[iNumPts++] = PT_LINETO;
                                    }
                                if (b&SOUTH) {
                                    p[iNumPts].x = i*Step;
                                    p[iNumPts].y = (j+1)*Step;
                                    bType[iNumPts++] = PT_MOVETO;
                                    p[iNumPts].x = (i+1)*Step;
                                    p[iNumPts].y = (j+1)*Step;
                                    bType[iNumPts++] = PT_LINETO;
                                    }
                                if (b&WEST) {
                                    p[iNumPts].x = i*Step;
                                    p[iNumPts].y = j*Step;
                                    bType[iNumPts++] = PT_MOVETO;
                                    p[iNumPts].x = i*Step;
                                    p[iNumPts].y = (j+1)*Step;
                                    bType[iNumPts++] = PT_LINETO;
                                    }
                                if (b&EAST) {
                                    p[iNumPts].x = (i+1)*Step;
                                    p[iNumPts].y = j*Step;
                                    bType[iNumPts++] = PT_MOVETO;
                                    p[iNumPts].x = (i+1)*Step;
                                    p[iNumPts].y = (j+1)*Step;
                                    bType[iNumPts++] = PT_LINETO;
                                    }
                                }
                            }
                        PolyDraw(hDC,p,bType,iNumPts);
                        }

                    GlobalUnlock(hBMem);
                    GlobalUnlock(hPMem);
                    GlobalFree(hBMem);
                    GlobalFree(hPMem);
                    break;

                default:
                    break;
                }
//BUGBUG -- shouldn't it be hWindow,hDC???
            ReleaseDC(diInfo->hDC,hDC);
            break;

        case WM_CLOSE:
            for(i=0;i<NUM_PICS;i++) {
                if (ptSelf.iPicNum != i) {
                    DelPic(i);
                    }
                }
            PostMessage(hWndDlg, WM_COMMAND, IDCANCEL, 0L);
            break;

        case WM_COMMAND:
            switch(wParam) {
                case IDOK:
                    ptSelf.iGridNum = iCurMaze;
                    ptSelf.iPicNum = iCurPic;
                    EndDialog(hWndDlg, TRUE);
                    break;
                case IDCANCEL:
                    EndDialog(hWndDlg, FALSE);
                    break;
                }
            break;

        default:
            return FALSE;
        }

    return(TRUE);
}



/*=====================================================================
Function:   cwCenter()

Inputs:     Handle to window to center, top of window

Outputs:    none

Abstract:
    Does a MoveWindow to center the window in question.
======================================================================*/

void cwCenter(
    HWND hWnd,
    int top
    )
{
    POINT    pt;
    RECT     swp;
    RECT     rParent;
    int      iwidth;
    int      iheight;

    GetWindowRect(hWnd, &swp);
    GetClientRect(hWndMain, &rParent);

    iwidth = swp.right - swp.left;
    iheight = swp.bottom - swp.top;

    pt.x = (rParent.right - rParent.left) / 2;
    pt.y = (rParent.bottom - rParent.top) / 2;
    ClientToScreen(hWndMain, &pt);

    pt.x -= (iwidth / 2);
    pt.y -= (iheight / 2);

    if (top) {
        pt.y += top;
        }

    MoveWindow(hWnd, pt.x, pt.y, iwidth, iheight, FALSE);
}
