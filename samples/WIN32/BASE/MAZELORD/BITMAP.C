/***********************************************************************
File:   BitMap.c
Author: JohnMil
Date:   5-23-92


Abstract:

    This file contains functions pertaining to drawing a bitmap on the screen.

Contents:

    FadePic() -- Slowly fade a picture on the screen to the background
    DrawClippedPic() -- Draw a picture, clipped to a specified rectangle
    NewFullPic() -- Create a record of type FullPicType
    DeleteFullPic() -- Delete a record of type FullPicType
    AddPic() -- Load a new picture bitmap set
    DelPic() -- Remove a picture bitmap set from memory.


Revision History:

10-29-92 (JohnMil):
    Modified to new personal comment format

************************************************************************/
#include "winmaze.h"
#include "mazproto.h"

#define MASKROP(fore,back) (DWORD)(((back << 8)&0xFF000000) | fore)
#define MASKROP2(black,white) (DWORD)(((black << 8)&0xFF000000) | white) & 0xFFFF0000




//
// PicSpec goes:
//      Base BM filename, iPicNum for the picture, offsets for each facing:
//              {width, depth, left offset, top offset}
//
PicSpecType PicSpec[] = {
   {"FISH", PIC_FISH,
        {
            {35,41,30,42},
            {62,35,15,49},
            {21,31,36,51},
            {62,31,21,53}
        }},
   {"ROBO", PIC_ROBOT,
        {
            {40,107,19,43},
            {50,107,13,43},
            {43,106,23,44},
            {48,107,30,43}
        }},
   {"SMIL", PIC_SMILEY,
        {
            {62,62,17,40},
            {68,62,17,40},
            {62,62,17,40},
            {70,62,9,40}
        }},
   {"REAP", PIC_REAPER,
        {
            {90,114,5,26},
            {85,114,8,26},
            {90,114,5,26},
            {85,114,8,26}
        }}
   };

int iNumPicSpecs = sizeof(PicSpec) / sizeof(PicSpecType);





/*=====================================================================
Function: FadePic()

Inputs: Picture #, facing, and clipping rectangle to fade.

Outputs:none

Abstract:
    Fade will fade out a given bitmap by XORing its bitmap with a 50% grey tone.
======================================================================*/

void FadePic(
    int iPicNum,
    BYTE bFacing,
    HDC hDC,
    LPRECT lprFrom,
    LPRECT lprDRect
    )
{
    FullPicType FAR *fptTrav;
    BOOL bFound;
    HDC hFadeDC,hMaskDC,hbmFadeDC;
    HBITMAP hbmFade;
    int DestWidth,DestHeight,SrcWidth,SrcHeight;
    int iRelDirIndex,i,j;


    fptTrav = &fptPic;
    bFound = FALSE;

    iRelDirIndex=2;
    while(bFacing != ptSelf.Pos.Facing) {
        iRelDirIndex = (iRelDirIndex+1)%4;
        bFacing = LEFT_TO_ABS(bFacing);
        }

    while(fptTrav->next != NULL) {
        fptTrav = fptTrav->next;
        if (fptTrav->iPicNum == iPicNum) {
            bFound = TRUE;
            break;
            }
        }

    if (bFound) {
        hMaskDC = CreateCompatibleDC(hDC);
        hFadeDC = CreateCompatibleDC(hDC);
        hbmFadeDC = CreateCompatibleDC(hFadeDC);
        SrcWidth = lprFrom->right-lprFrom->left+1;
        SrcHeight = lprFrom->bottom-lprFrom->top+1;
        DestWidth = lprDRect->right-lprDRect->left+1;
        DestHeight = lprDRect->bottom-lprDRect->top+1;
        hbmFade = CreateCompatibleBitmap(hFadeDC,DestWidth,DestHeight);
        SelectObject(hbmFadeDC,hbmFade);
        SelectObject(hFadeDC,hFadeBM[0]);
        SelectObject(hMaskDC,fptTrav->M[iRelDirIndex].hBitmap);

        //
        // Copy the Fade pattern into the Fade Bitmap
        //
        for(i=0;i<DestWidth;i+=PIC_X) {
            for(j=0;j<DestHeight;j+=PIC_Y) {
                BitBlt(hbmFadeDC,i,j,
                       ((i+PIC_X)<=DestWidth) ? PIC_X : DestWidth,
                       ((j+PIC_Y)<=DestHeight) ? PIC_Y : DestHeight,
                       hFadeDC,0,0,
                       SRCCOPY);
                }
            }

        //
        // StretchBlt the mask onto the fade
        //
        StretchBlt(hbmFadeDC,0,0,DestWidth,DestHeight,
                   hMaskDC,0,0,SrcWidth,SrcHeight,
                   NOTSRCERASE);

        //
        // BitBlt the result onto the screen.
        //
        BitBlt(hDC,lprDRect->left,lprDRect->top,DestWidth,DestHeight,
               hbmFadeDC,0,0,
               SRCPAINT);

        DeleteDC(hFadeDC);
        DeleteDC(hMaskDC);
        DeleteDC(hbmFadeDC);
        DeleteObject(hbmFade);
        DeleteObject(hFadeBM[0]);
        hFadeBM[0] = LoadBitmap(hInst,(LPCTSTR)((0)?"FADE2":"FADE1"));

        }

    return;

}




/*=====================================================================
Function: DrawClippedPic()

Inputs: Picture #, Relative facing, clip/dest rectangles,

Outputs:none

Abstract:
    DrawClippedPic will draw the portions of picture # iPicNum visible through
    the clipping window.  Player bitmaps smaller than a full panel in size
    Will need to be scaled.
======================================================================*/

void DrawClippedPic(
    int iPicNum,
    BYTE bFacingIndex,
    HDC hDC,
    LPRECT lprDRect,
    LPRECT lprClip,
    LPRECT lprFrom,
    int iRelx,
    int iRely
    )

{
    RECT rSrc;
    FullPicType FAR *fptTrav;
    BOOL bFound;
    HDC hPicDC,hMaskDC;
    int DestWidth,DestHeight;
    POINT pBlt[3];
    //
    // the floats are offset ratio's, ie percentage of the total
    // width/depth to add to each side of the source rect.
    //
    float sWidth,sDepth,dWidth,dDepth;
    float fRelScaling;

    if (lprClip->right < lprClip->left+2) {
        return;
        }

    fptTrav = &fptPic;
    bFound = FALSE;

    //
    // Check to see if the Picture we're supposed to draw has been loaded.
    //
    AddPic(iPicNum);

    while(fptTrav->next != NULL) {
        fptTrav = fptTrav->next;
        if (fptTrav->iPicNum == iPicNum) {
            bFound = TRUE;
            break;
            }
        }

    if (bFound) {

        //
        // bFound will now indicates whether the picture has been pre-stretched
        //

        bFound = (PreStretch[iPicNum][iRely].P[bFacingIndex].hBitmap != (HANDLE) NULL);
        bFound = bFound && (PreStretch[iPicNum][iRely].M[bFacingIndex].hBitmap != (HANDLE) NULL);

        //
        // We'll need the source dimensions whether it's prestretched or not.
        //
        if (bFound) {
            sWidth = (float) PreStretch[iPicNum][iRely].P[bFacingIndex].xSize;
            sDepth = (float) PreStretch[iPicNum][iRely].P[bFacingIndex].ySize;
            }
        else {
            sWidth = (float) fptTrav->P[bFacingIndex].xSize;
            sDepth = (float) fptTrav->P[bFacingIndex].ySize;
            }

        //
        // Assume that we want the ENTIRE source bitmap.
        //
        rSrc.left = 0;
        rSrc.top = 0;
        rSrc.right = (INT) sWidth-1;
        rSrc.bottom = (INT) sDepth-1;

        //
        // fRelScaling*an offset gives # of pixels of REAL offset.
        //
        fRelScaling = ((float) (lprDRect->bottom - lprDRect->top))/(float)PIC_Y;

        if (bFound) {
            lprDRect->left += PreStretch[iPicNum][iRely].P[bFacingIndex].xOrg;
            lprDRect->top += PreStretch[iPicNum][iRely].P[bFacingIndex].yOrg;
            lprDRect->right = lprDRect->left + (int) sWidth;
            lprDRect->bottom = lprDRect->top + (int) sDepth;
            }
        else {
            lprDRect->left += (int) (fRelScaling*fptTrav->P[bFacingIndex].xOrg);
            lprDRect->top += (int) (fRelScaling*fptTrav->P[bFacingIndex].yOrg);
            lprDRect->right = lprDRect->left + (int) (fRelScaling*sWidth);
            lprDRect->bottom = lprDRect->top + (int) (fRelScaling*sDepth);
            }

        dWidth = (float) lprDRect->right - lprDRect->left;
        dDepth = (float) lprDRect->bottom - lprDRect->top;

        //
        // Rather than doing a clipped bitblt, we just modify the source
        // rectangle to be the parts which are visible.
        //
        if (lprDRect->left < lprClip->left) {
            rSrc.left += (LONG) (sWidth*((float) (lprClip->left - lprDRect->left))/dWidth);
            lprDRect->left = lprClip->left;
            }
        if (lprDRect->top < lprClip->top) {
            rSrc.top += (LONG) (sDepth*((float) lprClip->top - lprDRect->top)/dDepth);
            lprDRect->top = lprClip->top;
            }
        if (lprDRect->right > lprClip->right) {
            rSrc.right += (LONG) (sWidth*((float) lprClip->right - lprDRect->right)/dWidth);
            lprDRect->right = lprClip->right;
            }
        if (lprDRect->bottom > lprClip->bottom) {
            rSrc.bottom += (LONG) (sDepth*((float) lprClip->bottom - lprDRect->bottom)/dDepth);
            lprDRect->bottom = lprClip->bottom;
            }
        //
        // Set the Dest variables to the TRUE physical dest width/depth
        //
        DestWidth = lprDRect->right - lprDRect->left;
        DestHeight = lprDRect->bottom - lprDRect->top+1;

        if ((DestWidth < 1)||(DestHeight < 1)) {
            return;
            }

        hPicDC = CreateCompatibleDC(hDC);
        hMaskDC = CreateCompatibleDC(hPicDC);

        if (bFound) {
            //
            // If the bitmaps are pre-stretched...
            //

            SelectObject(hPicDC,PreStretch[iPicNum][iRely].P[bFacingIndex].hBitmap);
            SelectObject(hMaskDC,PreStretch[iPicNum][iRely].M[bFacingIndex].hBitmap);

            BitBlt(hDC,lprDRect->left,lprDRect->top,DestWidth,DestHeight,
                   hMaskDC,rSrc.left,rSrc.top,
                   SRCAND);
            BitBlt(hDC,lprDRect->left,lprDRect->top,DestWidth,DestHeight,
                   hPicDC,rSrc.left,rSrc.top,
                   SRCPAINT);
            }
        else {
            //
            // If the bitmaps are NOT pre-stretched.
            //
            SelectObject(hPicDC,fptTrav->P[bFacingIndex].hBitmap);
            SelectObject(hMaskDC,fptTrav->M[bFacingIndex].hBitmap);
            //
            // stretch the mask onto the screen
            //
            pBlt[0].x = lprDRect->left;
            pBlt[0].y = lprDRect->top;
            pBlt[1].x = lprDRect->right;
            pBlt[1].y = lprDRect->top;
            pBlt[2].x = lprDRect->left;
            pBlt[2].y = lprDRect->bottom;

            StretchBlt(hDC,lprDRect->left,lprDRect->top,DestWidth,DestHeight,
                       hMaskDC,rSrc.left,rSrc.top,rSrc.right-rSrc.left+1,rSrc.bottom-rSrc.top+1,
                       SRCAND);
            //
            // then the picture as well
            //
            StretchBlt(hDC,lprDRect->left,lprDRect->top,DestWidth,DestHeight,
                       hPicDC,rSrc.left,rSrc.top,rSrc.right-rSrc.left+1,rSrc.bottom-rSrc.top+1,
                       SRCPAINT);
            }

        DeleteDC(hPicDC);
        DeleteDC(hMaskDC);
        }
    else {
        MessageBox(NULL,"Unknown Picture Draw Requested","DrawClippedPic",
                   MB_ICONEXCLAMATION|MB_APPLMODAL);
        }

    *lprFrom = rSrc;

    return;
}




/*=====================================================================
Function: NewFullPic()

Inputs: Picture # to load, pointer to next

Outputs:Returns a pointer to an initialized FullPicType data structure

Abstract:
======================================================================*/

FullPicType FAR *NewFullPic(
    int iPicNum,
    FullPicType FAR *next
    )
{
    FullPicType FAR *fptTemp;
    int i;
    HANDLE hMem;

    hMem = GlobalAlloc(GHND,sizeof(FullPicType));
    fptTemp = (FullPicType FAR *) GlobalLock(hMem);
    if (fptTemp == NULL) {
        MessageBox((HANDLE)NULL,"Out of memory: unable to create NewFullPic.","NewFullPic",
               MB_ICONEXCLAMATION|MB_APPLMODAL);
        }
    else {
        for(i=0;i<4;i++) {
            fptTemp->P[i].hBitmap = (HANDLE) NULL;
            fptTemp->P[i].xSize = PicSpec[iPicNum].PicPos[i].xSize;
            fptTemp->P[i].ySize = PicSpec[iPicNum].PicPos[i].ySize;
            fptTemp->P[i].xOrg = PicSpec[iPicNum].PicPos[i].xOrg;
            fptTemp->P[i].yOrg = PicSpec[iPicNum].PicPos[i].yOrg;
            fptTemp->M[i].hBitmap = (HANDLE) NULL;
            fptTemp->M[i].xSize = PicSpec[iPicNum].PicPos[i].xSize;
            fptTemp->M[i].ySize = PicSpec[iPicNum].PicPos[i].ySize;
            fptTemp->M[i].xOrg = PicSpec[iPicNum].PicPos[i].xOrg;
            fptTemp->M[i].yOrg = PicSpec[iPicNum].PicPos[i].yOrg;
            }
        fptTemp->iPicNum = iPicNum;
        fptTemp->next = next;
        }

    return(fptTemp);

}



/*=====================================================================
Function: DeleteFullPic()

Inputs: pointer to FullPicType to free

Outputs:Returns pointer to ->next element of deleted pic.

Abstract:
======================================================================*/

FullPicType FAR *DeleteFullPic(
    FullPicType FAR *fptTrav
    )
{
    int i;
    FullPicType FAR *fptTemp;
    HANDLE hMem;

    if (fptTrav == NULL) {
        fptTemp = fptTrav;
        }
    else {
        fptTemp = fptTrav->next;

        for (i=0;i<4;i++) {
            if (fptTrav->P[i].hBitmap != (HANDLE) NULL) {
                DeleteObject(fptTrav->P[i].hBitmap);
                }
            if (fptTrav->M[i].hBitmap != (HANDLE) NULL) {
                DeleteObject(fptTrav->M[i].hBitmap);
                }
            }

        hMem = (HGLOBAL) GlobalHandle(SELECTOROF(fptTrav));
        GlobalUnlock(hMem);
        GlobalFree(hMem);
        }

    return(fptTemp);
}




/*=====================================================================
Function: AddPic()

Inputs: Picture # to load

Outputs:Returns success/failure.

Abstract:
    AddPic will take care of loading in a given picture's bitmap, if it isn't
    already in the fptPic list. If the specified type of picture can't be found,
    FALSE is returned.  Note that when we load a picture, we also make sure
    that it is restricted to the confines of its mask by turning everything
    outside the mask BLACK. This is necessary for drawing as well.
======================================================================*/

BOOL AddPic(
    int iPicNum
    )
{
    char cDirs[4] = {'F','L','B','R'};
    FullPicType FAR *fptTrav;
    BOOL bFound;
    int i,j;
    char cBitmapName[32];
    HDC hDC,hPicDC,hMaskDC;

    fptTrav = &fptPic;
    bFound = FALSE;

    //
    // Look to see if the picture's already been loaded into memory. If
    // it has, we won't have to load it.
    //
    while (fptTrav->next != NULL) {
        fptTrav = fptTrav->next;

        if (fptTrav->iPicNum == iPicNum) {
            bFound = TRUE;
            break;
            }
        }

    //
    // If we didn't find the bitmap, we really DO need to load it.
    //
    if (!bFound) {

        //
        // We can only load the requested picture if it's in our list of
        // known pics. Check to make sure it is.
        //
        for(i=0;i<iNumPicSpecs;i++) {
            if (PicSpec[i].iPicNum == iPicNum) {
                bFound = TRUE;
                break;
                }
            }

        if (bFound) {
            hDC = GetDC(hWndMaze);
            hPicDC = CreateCompatibleDC(hDC);
            hMaskDC = CreateCompatibleDC(hPicDC);
            ReleaseDC(hWndMaze,hDC);

            fptTrav->next = NewFullPic(iPicNum,fptTrav->next);
            fptTrav = fptTrav->next;
            strcpy(cBitmapName,PicSpec[i].cBase);
            cBitmapName[5]='\0';

            //
            // Load in the bitmaps
            //
            for(j=0;j<4;j++) {
                cBitmapName[4]=cDirs[j];
                if (fptTrav->P[j].hBitmap != NULL) {
                    DeleteObject(fptTrav->P[j].hBitmap);
                }
                fptTrav->P[j].hBitmap = LoadBitmap(hInst,(LPCTSTR)cBitmapName);
                if (fptTrav->P[j].hBitmap == (HANDLE) NULL) {
                    MessageBox((HANDLE) NULL,"Unable to load bitmap","AddPic",
                               MB_ICONEXCLAMATION|MB_APPLMODAL);
                    bFound = FALSE;
                    }
                //
                // Copy stats for the Bitmap's size/origin into the struct
                //
                fptTrav->P[j].xSize = PicSpec[i].PicPos[j].xSize;
                fptTrav->P[j].ySize = PicSpec[i].PicPos[j].ySize;
                fptTrav->P[j].xOrg = PicSpec[i].PicPos[j].xOrg;
                fptTrav->P[j].yOrg = PicSpec[i].PicPos[j].yOrg;
                }

            cBitmapName[5]='M';
            cBitmapName[6]='\0';

            //
            // Load in the masks for the bitmaps
            //
            for(j=0;j<4;j++) {
                cBitmapName[4]=cDirs[j];
                if (fptTrav->M[j].hBitmap != NULL) {
                    DeleteObject(fptTrav->M[j].hBitmap);
                }
                fptTrav->M[j].hBitmap = LoadBitmap(hInst,(LPCTSTR)cBitmapName);
                if (fptTrav->M[j].hBitmap == (HANDLE) NULL) {
                    MessageBox((HANDLE) NULL,"Unable to load bitmap mask","AddPic",
                                MB_ICONEXCLAMATION|MB_APPLMODAL);
                    bFound = FALSE;
                    }
                //
                // Copy stats for the Mask's size/origin into the struct
                //
                fptTrav->M[j].xSize = PicSpec[i].PicPos[j].xSize;
                fptTrav->M[j].ySize = PicSpec[i].PicPos[j].ySize;
                fptTrav->M[j].xOrg = PicSpec[i].PicPos[j].xOrg;
                fptTrav->M[j].yOrg = PicSpec[i].PicPos[j].yOrg;
                }
/***********BUGBUG
            //
            // Invert the background of the bitmaps from white to black
            //
            for(j=0;j<4;j++) {
                SelectObject(hPicDC,fptTrav->P[j].hBitmap);
                SelectObject(hMaskDC,fptTrav->M[j].hBitmap);
                BitBlt(hPicDC,0,0,
                                 PicSpec[iPicNum].PicPos[i].xSize,
                                 PicSpec[iPicNum].PicPos[i].ySize,
                       hMaskDC,0,0,SRCINVERT);
                }
*************/

            DeleteDC(hPicDC);
            DeleteDC(hMaskDC);
            }
        }

    return(bFound);
}




/*=====================================================================
Function: DelPic()

Inputs: Picture # to Delete from pic list

Outputs:Returns success/failure.

Abstract:
    DelPic will remove a picture, assuming that it is not being used by anyone
    in ptDrones and ptPlayers, and it isn't of type PIC_DEFAULT.
======================================================================*/

BOOL DelPic(
    int iPicNum
    )
{
    FullPicType FAR *fptTrav;
    BOOL bFound,bStillUsed;
    PlayerType FAR *ptTrav;


    if (iPicNum == PIC_DEFAULT) {
        return(TRUE);
        }

    fptTrav = &fptPic;
    bFound = FALSE;

    while (fptTrav->next != NULL) {
        if (fptTrav->next->iPicNum == iPicNum) {
            bFound = TRUE;
            bStillUsed = FALSE;

            ptTrav = &ptPlayers;
            while (ptTrav->next != NULL) {
            ptTrav = ptTrav->next;
            if (ptTrav->iPicNum == iPicNum) {
                bStillUsed = TRUE;
                break;
                }
            }

            ptTrav = &ptDrones;
            while (ptTrav->next != NULL) {
            ptTrav = ptTrav->next;
            if (ptTrav->iPicNum == iPicNum) {
                bStillUsed = TRUE;
                break;
                }
            }

            if (!bStillUsed) {
                fptTrav->next = DeleteFullPic(fptTrav->next);
                }

            break;
            }

        fptTrav = fptTrav->next;
        }

    return(bFound);
}
