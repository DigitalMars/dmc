/**********************************************************************
File:   InitMaze.c
Author: JohnMil
Date:   5-13-92


Abstract:

    This module contains drawing routines for MazeLords.

Contents:

    DelHole() --
    NewHole() --
    DrawRect() --
    NewPlayerDraw() --
    DelPlayerDraw() --
    CheckForPlayers() --
    DrawFoundPlayers() --
    DrawMaze() --
    DrawTopView() --
    DrawPlayers() --


Revision History:

10-30-92 (JohnMil):
    Modified to new personal comment format

************************************************************************/

#include "winmaze.h"
#include "mazproto.h"

LPPOINT pDraw;
LPBYTE pType;
int PointCount;


//
// convert an offset to an increment to add.
//
#define ABS2OFFX(x,y,Facing) (((Facing)&NORTH) ? (x) : \
                              ((Facing)&SOUTH) ? -(x) : \
                              ((Facing)&EAST) ? (y) : -(y))
#define ABS2OFFY(x,y,Facing) (((Facing)&NORTH) ? -(y) : \
                              ((Facing)&SOUTH) ? (y) : \
                              ((Facing)&EAST) ? (x) : -(x))


/*=====================================================================
Function: DelHole()

Inputs: Pointer to HoleType record to delete

Outputs: Returns ->next field of deleted record

Abstract:
    Deletes a record of type HoleType, returns ->next pointer to
    facilitate list maintenance
======================================================================*/

HoleType FAR *DelHole(
    HoleType FAR *h
    )
{
    HoleType FAR *t;
    HANDLE hMem;

    if (h == NULL) {
        return((HoleType FAR *) NULL);
        }
    else {
        hMem = (HGLOBAL) GlobalHandle(SELECTOROF( h));
        t = h->next;
        GlobalUnlock(hMem);
        GlobalFree(hMem);
        return(t);
        }
}




/*=====================================================================
Function: NewHole()

Inputs: HoleType element values

Outputs:Returns a pointer to a new initialized HoleType record

Abstract:
    No explanation needed
======================================================================*/

HoleType FAR *NewHole(
    int x1,
    int x2,
    HoleType FAR *next
    )

{
    HoleType FAR *h;
    HANDLE hMem;

    hMem =  GlobalAlloc(GHND,sizeof(HoleType));
    h = (HoleType FAR *) GlobalLock(hMem);
    if (h == (HoleType FAR *) NULL) {
        MessageBox((HANDLE) NULL,"Out of memory -- Terminating.","ERROR",MB_ICONEXCLAMATION);
        exit(0);
        }
    h->x[0] = x1;
    h->x[1] = x2;
    h->next = next;

    return(h);
}



/*=====================================================================
Function: DrawRect()

Inputs: Hole List (determining what is visible),
    relative x and y coordinates of wall to draw,
    relative facing of wall to draw

Outputs: Modifies hole list, returns a pointer to where hole processing
    should resume from

Abstract:
    DrawRect expects to draw a wall for the coordinate (iRelX,iRelY) relative
    facing bRelDir.

    For clipping, suppose we have

          xr
          |
      p1|---
        |   ---    xs
        |      --- |
        |         --- p4
        |            |
        |            |
        |            |
        |         --- p3
        |      ---
        |   ---
      p2|---

    where (p1 to p4) are known endpoints for a panel, and xr and xs are
    vertical clipping lines (fortunately we're restricted to these.
    The new dimensions for p1-p4 are easy to calculate, since, it is a matter
    of similar triangles:

        |         ---. p3
        |      --- ; .
        |   ---    ; .
      p2|---..........
                   ;
                   xs

    We need to recalculate p3, since it is past the clipping boundary. By
    similar triangles, we know  (p3.y-p2.y)/(p3.x-p2.x) = (xs.y-p2.y)/(xs.x-p2.x).
    This in turn gives us xs.y = (p3.y-p2.y)*(xs.x-p2.x)/(p3.x-p2.x) + p2.y. This
    can be applied to all four endpoints.
======================================================================*/

HoleType FAR *DrawRect(
    HoleType FAR *htTrav,
    int iRelX,
    int iRelY,
    BYTE bRelDir
    )
{
    POINT p[4];
    int i,rb;
    int ihLeft,ihRight;
    BOOL bIsLeftSide;


    bIsLeftSide = (iRelX < 0);


    if ((htTrav == NULL)||(htTrav->next == NULL)) {
        return(NULL);
        }

    ihLeft = htTrav->next->x[0];
    ihRight= htTrav->next->x[1];

    if (ihLeft >= ihRight) {
        return(htTrav->next);
        }

    //
    //p1=p[0],p2=p[1],p4=p[2],p3=p[3] from diagram above.
    //
    switch(bRelDir) {

        //
        // Wall furthest away from us
        //
        case NORTH:
            for(i=0;i<2;i++) {
                p[i] = pPost[iRelX+MAX_DRAW_DIST+1][iRelY+1][i];
                p[i+2] = pPost[iRelX+MAX_DRAW_DIST+2][iRelY+1][i];
                }
            break;

        //
        // Wall closest to us
        //
        case SOUTH:
            for(i=0;i<2;i++) {
                p[i] = pPost[iRelX+MAX_DRAW_DIST+1][iRelY][i];
                p[i+2] = pPost[iRelX+MAX_DRAW_DIST+2][iRelY][i];
                }
            break;

        //
        // West ==> we're working on the left side of the screen
        //
        case WEST:
            for(i=0;i<2;i++) {
                p[i] = pPost[iRelX+MAX_DRAW_DIST+1][iRelY][i];
                p[i+2] = pPost[iRelX+MAX_DRAW_DIST+1][iRelY+1][i];
                }
            break;

        //
        // East ==> we're working on the right side of the screen
        //
        case EAST:
            for(i=0;i<2;i++) {
                p[i] = pPost[iRelX+MAX_DRAW_DIST+2][iRelY+1][i];
                p[i+2] = pPost[iRelX+MAX_DRAW_DIST+2][iRelY][i];
                }
            break;
        }

    //
    // If we can see any of the panel through the hole,
    // go ahead and draw it!
    //
    if ((p[0].x < ihRight)&&(p[2].x > ihLeft)) {
        for(i=0;i<2;i++) {
            //
            // Clip the left side, if needed.
            //
            if (p[i].x < ihLeft ) {
                p[i].y = ((p[i].y-p[i+2].y)*(ihLeft-p[i+2].x)/
                            (p[i].x-p[i+2].x)) + p[i+2].y;
                p[i].x = ihLeft;
                }

            //
            // and the right side...
            //
            if (p[3-i].x > ihRight) {
                p[3-i].y = ((p[3-i].y-p[1-i].y)*(ihRight-p[1-i].x)/
                            (p[3-i].x-p[1-i].x)) + p[1-i].y;
                p[3-i].x = ihRight;
                }
            }

        //
        // Now we need to add these points into the draw list.
        //
        pType[PointCount]=PT_MOVETO;
        pDraw[PointCount++]=p[0];
        pType[PointCount]=PT_LINETO;
        pDraw[PointCount++]=p[1];
        pType[PointCount]=PT_LINETO;
        pDraw[PointCount++]=p[3];
        pType[PointCount]=PT_LINETO;
        pDraw[PointCount++]=p[2];
        pType[PointCount]=PT_LINETO;
        pDraw[PointCount++]=p[0];

        if (p[0].x == ihLeft) {
            if (p[2].x == ihRight) {
                //
                // If left and right match exactly, delete hole.
                //
                htTrav->next = DelHole(htTrav->next);
                }
            else {
                //
                // if left matches, but right doesn't, set hole to rt,rt
                //
                htTrav->next->x[0] = p[2].x;
                }
            }
        else {
            //
            // if the left doesn't match, at the very least we need
            // to keep the left to the current spot.
            //
            rb = htTrav->next->x[1];
            htTrav->next->x[1] = p[0].x;

            if (p[2].x != rb) {
                //
                // Also, if the right side didn't match up, we need to
                // create a new hole.

                //
                // need to make sure the hole won't get skipped as we
                // continue processing inside to outside...
                //
                if (bIsLeftSide) {
                    htTrav->next = NewHole(p[2].x,rb,htTrav->next);
                    }
                else {
                    htTrav->next->next = NewHole(p[2].x,rb,htTrav->next->next);
                    }
                }
            }
        }

    return(htTrav);
}



/*=====================================================================
Function: NewPlayerDraw()

Inputs: PlayerDrawType record element values

Outputs: Pointer to a new, initialized record of type PlayerDrawType

Abstract:
    No explanation needed.
======================================================================*/

PlayerDrawType FAR *NewPlayerDraw(
    PlayerType FAR *p,
    LPRECT rDraw,
    LPRECT rClip,
    int iRelx,
    int iRely,
    PlayerDrawType FAR *next
    )
{
    PlayerDrawType FAR *pdtRet;
    HGLOBAL hMem;

    hMem = GlobalAlloc(GHND,(sizeof(PlayerDrawType)));
    pdtRet = (PlayerDrawType FAR *) GlobalLock(hMem);
    if (pdtRet == NULL) {
        MessageBox((HWND) NULL,"Unable to allocate memory.","NewPlayerDraw",
                   MB_APPLMODAL|MB_ICONEXCLAMATION);
        }
    else {
        pdtRet->p = p;
        pdtRet->rDraw = *rDraw;
        pdtRet->rClip = *rClip;
        pdtRet->iRelx = iRelx;
        pdtRet->iRely = iRely;
        pdtRet->next = next;
        }

    return(pdtRet);
}


/*=====================================================================
Function: DelPlayerDraw()

Inputs: Pointer to PlayerDrawType record to delete

Outputs: Pointer to ->next field value of deleted record

Abstract:
    returns the ->next field for linked list maintenance
======================================================================*/

PlayerDrawType FAR *DelPlayerDraw(
    PlayerDrawType FAR *pdtP
    )
{
    PlayerDrawType FAR *pdtRet;
    HGLOBAL hMem;

    pdtRet = NULL;
    if (pdtP != NULL) {
        pdtRet = pdtP->next;
        hMem = (HGLOBAL) GlobalHandle(SELECTOROF(pdtP));
        GlobalFree(hMem);
        }

    return(pdtRet);
}


/*=====================================================================
Function: CheckForPlayers()

Inputs: List of players, holes in view, positions to check

Outputs:

Abstract:
    This routine checks for players on a single square, given the relative
    and absolute coordinates of that square. If a player IS on that square,
    then we check the hole we're looking through to see if any of the
    destination rectangle is visible. If it is, we make a note of the player
    by adding them to the pdtTD = 'Players To Draw' list.
======================================================================*/

void CheckForPlayers(
    PlayerDrawType FAR *pdtTD,
    PlayerType FAR *ptObj,
    HoleType FAR *htHole,
    int iRelx,
    int iRely,
    int iAbsx,
    int iAbsy
    )
{
    PlayerType FAR *ptTrav;
    RECT rDraw,rClip;
    float x,y,z;
    int xSize,ySize;
    POINT pCenter;

    if (htHole == NULL) {
        return;
        }

    GetClientRect(hWndMaze,&rMaze);

    pCenter.x = rMaze.left + (rMaze.right-rMaze.left)/2;
    pCenter.y = rMaze.top + (rMaze.bottom-rMaze.top)/2;

    //
    // Set x/y/z to screen coordinate upper left corner of panel
    //
    x = (float) ((iRelx*PANEL_WIDTH) - PANEL_WIDTH/2);
    y = (float) (-PANEL_HEIGHT/2);
#if ( _ALPHA_ == 1 )
    z = (float) iRely*PANEL_WIDTH - (PANEL_WIDTH/2);
#else
    z = (float) (iRely-1)*PANEL_WIDTH;
#endif

    //
    // Calculate the physical width and depth of a panel placed at the bitmap
    // location, since this is the maximum possible size for a player bitmap.
    // PIC_X and PIC_Y are the pixel values for this.
    //
    xSize = (int) MC_TO_SC(PIC_X,z);
    ySize = (int) MC_TO_SC(PIC_Y,z);

    //
    // Calculate the physical rectangle to contain the player bitmap
    //
    rDraw.left = pCenter.x + (int) MC_TO_SC(x,z);
    rDraw.top  = pCenter.y + (int) MC_TO_SC(y,z);
    rDraw.right = rDraw.left + xSize-1;
    rDraw.bottom = rDraw.top + ySize-1;

    //
    // Set the clipping rectangle to the visible screen
    //
    rClip.left = GREATEROF(rDraw.left,0);
    rClip.top = GREATEROF(rDraw.top,0);
    rClip.right = LESSEROF(rDraw.right,rMaze.right);
    rClip.bottom = LESSEROF(rDraw.bottom,rMaze.bottom);

    //
    // Adjust the clipping rectangle's left and right by the hole
    // which we're constrained by (Holes are horizontal coordinates only)
    //
    if (htHole != (HoleType FAR *) -1) {
        rClip.left = GREATEROF(rClip.left,htHole->x[0]);
        rClip.right = LESSEROF(rClip.right,htHole->x[1]);
        }

    //
    // If The whole picture is clipped, don't bother drawing it.
    //
    if (rClip.left >= rClip.right) {
        return;
        }

    ptTrav=ptObj;
    while (ptTrav->next != NULL) {
        ptTrav = ptTrav->next;
        if ((ptTrav->Pos.ix == iAbsx)&&(ptTrav->Pos.iy == iAbsy)) {
            //
            // We need to adjust the drawing coordinates according to this
            // player's picture.
            //
            pdtTD->next = NewPlayerDraw(ptTrav,&rDraw,&rClip,iRelx,iRely,pdtTD->next);
            }
        }
}



/*=====================================================================
Function: DrawFoundPlayers()

Inputs: List of players

Outputs: modifies rBounds to contain rectangle saying where player is.

Abstract:
    BUGBUG -- needs abstract
======================================================================*/

void DrawFoundPlayers(
    HDC hDC,
    PlayerDrawType FAR *pdtTrav,
    LPRECT rBounds
    )
{
    BYTE dBackward,b1,b2;

    dBackward = BACK_TO_ABS(ptSelf.Pos.Facing);

    while (pdtTrav->next != NULL) {
        b1 = BACK;
        b2 = ptSelf.Pos.Facing;
        while (b2 != pdtTrav->next->p->Pos.Facing) {
            b1 = (BYTE) ((b1+1)%4);
            b2 = RIGHT_TO_ABS(b2);
            }

        pdtTrav->next->p->rDrawn = pdtTrav->next->rDraw;
        if (rBounds != NULL) {
            rBounds->left = LESSEROF(rBounds->left,
                               GREATEROF(pdtTrav->next->rDraw.left,
                                         pdtTrav->next->rClip.left));
            rBounds->top = LESSEROF(rBounds->top,
                               GREATEROF(pdtTrav->next->rDraw.top,
                                         pdtTrav->next->rClip.top));
            rBounds->right = GREATEROF(rBounds->right,
                               LESSEROF(pdtTrav->next->rDraw.right,
                                         pdtTrav->next->rClip.right));
            rBounds->bottom = GREATEROF(rBounds->bottom,
                               LESSEROF(pdtTrav->next->rDraw.bottom,
                                         pdtTrav->next->rClip.bottom));
            }

        DrawClippedPic(pdtTrav->next->p->iPicNum,b1,hDC,
                       &pdtTrav->next->p->rDrawn,
                       &pdtTrav->next->rClip,
                       &pdtTrav->next->p->rFrom,
                       pdtTrav->next->iRelx,
                       pdtTrav->next->iRely);
        pdtTrav->next->p->Drawn = TRUE;
        pdtTrav->next->p->rDrawn.right++;
        pdtTrav->next->p->rDrawn.bottom++;

        pdtTrav->next = DelPlayerDraw(pdtTrav->next);
        }

    return;
}




/*=====================================================================
Function: DrawMaze()

Inputs: DC, rectangle that needs to be redrawn

Outputs: none

Abstract:
    This entrypoint takes care of drawing the 3-d maze. It loops from
    front to back, inside to outside, Checking for panels. It keeps a list
    of all the 'holes' in panels in terms of physical coordinates. For
    instance, if I've found the wall in front of me, but no others, and
    I can see to the left and to the right of the wall, I would have two
    holes in my hole-list. The first would be from the left of the view
    window to the left side of the wall, and the second would be from the
    right side of the wall to the right side of the view window.
======================================================================*/

void DrawMaze(HDC hDC,LPRECT rUpd)
{
    int i,j,tx,ty,iEndl,iEndr;
    BYTE dLeft,dRight,dForward,dBackward,b1,b2,b3;
    HoleType htLeft,htRight,FAR *htTrav, FAR *htHold;
    BOOL bFound;
    PlayerDrawType pdtToDraw;
    HBRUSH hOldBrush,hWhiteBrush;
    HPEN hOldPen,hWhitePen,hBlackPen;
    HGLOBAL hMem;

    if ((rUpd->left >= rUpd->right)||(rUpd->top >= rUpd->bottom)) {
        return;
        }

    hMem = GlobalAlloc(GHND,10000*sizeof(POINT));
    pDraw = (LPPOINT) GlobalLock(hMem);
    hMem = GlobalAlloc(GHND,10000*sizeof(BYTE));
    pType = (LPBYTE) GlobalLock(hMem);

    GetClientRect(hWndMaze,&rMaze);
    pdtToDraw.next = NULL;

    //
    // Clear out the space to be drawn into
    //
    hWhiteBrush = GetStockObject(WHITE_BRUSH);
    hWhitePen = GetStockObject(WHITE_PEN);
    hBlackPen = GetStockObject(BLACK_PEN);
    hOldBrush = SelectObject(hDC,hWhiteBrush);
    hOldPen = SelectObject(hDC,hWhitePen);

    PointCount = 0;

    dForward = ptSelf.Pos.Facing;
    dBackward = BACK_TO_ABS(ptSelf.Pos.Facing);
    dLeft = LEFT_TO_ABS(ptSelf.Pos.Facing);
    dRight = RIGHT_TO_ABS(ptSelf.Pos.Facing);

    tx = ptSelf.Pos.ix;
    ty = ptSelf.Pos.iy;

    Rectangle(hDC,rUpd->left,rUpd->top,rUpd->right,rUpd->bottom);

    SelectObject(hDC,hOldBrush);
    SelectObject(hDC,hBlackPen);

    //
    // Find the End which lies straight before us, else default
    // it to MAX_DRAW_DIST.
    //
    b2 = bMaze[tx][ty];
    bFound = FALSE;
    for(j=0,ViewEnd=MAX_DRAW_DIST-1;j<MAX_DRAW_DIST;j++) {
        b1 = b2;
        tx = ADJ_X(tx,ptSelf.Pos.Facing);
        ty = ADJ_Y(ty,ptSelf.Pos.Facing);
        b2 = bMaze[tx][ty];
        if ((b1&dForward)||(b2&dBackward)) {
            ViewEnd = j;
            bFound = TRUE;
            break;
            }
        }

    PointCount = 0;
    iEndl = iEndr = (rMaze.right - rMaze.left) / 2;

    //
    // Draw the end rectangle.
    //
    if (bFound) {
        //
        // Only bother with the draw if the end is to be updated
        //
        iEndl = pPost[MAX_DRAW_DIST+1][ViewEnd+1][0].x;
        iEndr = pPost[MAX_DRAW_DIST+2][ViewEnd+1][0].x;

        //
        // Left Post
        //
        pDraw[0].x = pPost[MAX_DRAW_DIST+1][ViewEnd+1][0].x;
        pDraw[0].y = pPost[MAX_DRAW_DIST+1][ViewEnd+1][0].y;
        pType[0] = PT_MOVETO;
        pDraw[1].x = pPost[MAX_DRAW_DIST+1][ViewEnd+1][1].x;
        pDraw[1].y = pPost[MAX_DRAW_DIST+1][ViewEnd+1][1].y;
        pType[1] = PT_LINETO;

        //
        // Right Post
        //
        pDraw[2].x = pPost[MAX_DRAW_DIST+2][ViewEnd+1][1].x;
        pDraw[2].y = pPost[MAX_DRAW_DIST+2][ViewEnd+1][1].y;
        pType[2] = PT_LINETO;
        pDraw[3].x = pPost[MAX_DRAW_DIST+2][ViewEnd+1][0].x;
        pDraw[3].y = pPost[MAX_DRAW_DIST+2][ViewEnd+1][0].y;
        pType[3] = PT_LINETO;
        pDraw[4].x = pPost[MAX_DRAW_DIST+1][ViewEnd+1][0].x;
        pDraw[4].y = pPost[MAX_DRAW_DIST+1][ViewEnd+1][0].y;
        pType[4] = PT_LINETO;
        PointCount = 5;
        }

    //
    // Make LeftHoles and RightHoles accordingly
    //
    htLeft.next = NewHole(0,iEndl,NULL);
    htRight.next = NewHole(iEndr,rMaze.right-rMaze.left,NULL);

    //
    // For the left side, loop from front to back, right to
    // left, checking the rEAST and rNORTH walls, in that
    // order. Eventually we'll need to put the check for players
    // in between the two. As a wall is found, draw it.
    //

    //
    // j is the relative y-offset into the maze.
    //
    for(j=0;j<MAX_DRAW_DIST;j++) {
        //
        // Check for players/drones directly in front of us,
        // make note of them to draw them later.
        //
        if (j < ViewEnd+1) {
            CheckForPlayers(&pdtToDraw,&ptPlayers,(HoleType FAR *) -1,0,j,
                      ptSelf.Pos.ix + ABS2OFFX(0,j,dForward),
                      ptSelf.Pos.iy + ABS2OFFY(0,j,dForward));
            CheckForPlayers(&pdtToDraw,&ptDrones,(HoleType FAR *) -1,0,j,
                      ptSelf.Pos.ix + ABS2OFFX(0,j,dForward),
                      ptSelf.Pos.iy + ABS2OFFY(0,j,dForward));
            }

        //
        // b1 is the square we're in, b2 the square we want to
        // see if there's a wall EAST/WEST between. b3 is the
        // square we want to see if there is a NORTH/SOUTH wall
        // from b2 to.  Then we move to b2 and try it again.
        //
        htTrav=&htLeft;

        while((htTrav != (HoleType FAR *) NULL)&&(htTrav->next != (HoleType FAR *)NULL)) {
            // i will loop from the 0th relative column to the far left.
            // This will be done for each and every hole, or even some
            // holes more than once, depending upon how they are filled.
            // set (tx,ty) to spot in our x-pos with relative y j.
            tx = ptSelf.Pos.ix + ABS2OFFX(0,j,dForward);
            ty = ptSelf.Pos.iy + ABS2OFFY(0,j,dForward);
            b2 = bMaze[tx][ty];

            htHold = htTrav->next;
            for (i=0;(i>= -MAX_DRAW_DIST)&&(htTrav != (HoleType FAR *)NULL)&&(htTrav->next != (HoleType FAR *)NULL);i--) {
                //
                // if we're not in the hole yet, continue
                //
                if (htTrav->next->x[1] < pPost[MAX_DRAW_DIST+i][j+1][0].x) {
                   continue;
                   }

                //
                // if we're past the outside of the hole, go to next hole.
                //
                if (htTrav->next->x[0] > pPost[MAX_DRAW_DIST+1+i][j+1][0].x) {
                    break;
                    }

                //
                // b1, b2, and b3 correspond to the square to the inside of the one
                // we're checking, the square we're checking, and the square
                // forward past the square we're checking.
                //
                b1 = b2;

                //
                // set (tx,ty) one square to the 'left'.
                //
                tx = ADJ_X(tx,dLeft);
                ty = ADJ_Y(ty,dLeft);

                b2 = bMaze[tx][ty];

                b3 = bMaze[ADJ_X(tx,dForward)][ADJ_Y(ty,dForward)];

                if ((b1&dLeft)||(b2&dRight)) {
                    htTrav = DrawRect(htTrav,i,j,WEST);
                    }

                CheckForPlayers(&pdtToDraw,&ptPlayers,htTrav->next,i-1,j,tx,ty);
                CheckForPlayers(&pdtToDraw,&ptDrones,htTrav->next,i-1,j,tx,ty);

                if ((b2&dForward)||(b3&dBackward)) {
                    htTrav = DrawRect(htTrav,i-1,j,NORTH);
                    }
                }

            if ((htTrav!= (HoleType FAR *)NULL)&&(htHold == htTrav->next)) {
                htTrav = htTrav->next;
                }
            }
         }

    //
    // Same as above, except this time for the RIGHT side.
    //
    for(j=0;j<MAX_DRAW_DIST;j++) {
        htTrav=&htRight;

        while((htTrav != (HoleType FAR *)NULL)&&(htTrav->next != (HoleType FAR *)NULL)) {
            tx = ptSelf.Pos.ix + ABS2OFFX(0,j,dForward);
            ty = ptSelf.Pos.iy + ABS2OFFY(0,j,dForward);

            b2 = bMaze[tx][ty];

            htHold = htTrav->next;
            for (i=0;(i<MAX_DRAW_DIST)&&(htTrav != (HoleType FAR *)NULL)&&(htTrav->next != (HoleType FAR *)NULL);i++) {
                //
                // if we're before the hole, continue
                //
                if (htTrav->next->x[0] > pPost[MAX_DRAW_DIST+i+3][j+1][0].x) {
                    continue;
                    }

                //
                // if we're outside the hole, go to next hole.
                //
                if (htTrav->next->x[1] < pPost[MAX_DRAW_DIST+i+2][j+1][0].x) {
                    break;
                    }

                b1 = b2;

                //
                // set (tx,ty) one square to the 'right'.
                //
                tx = ADJ_X(tx,dRight);
                ty = ADJ_Y(ty,dRight);

                b2 = bMaze[tx][ty];

                b3 = bMaze[ADJ_X(tx,dForward)][ADJ_Y(ty,dForward)];

                if ((b1&dRight)||(b2&dLeft)) {
                    htTrav = DrawRect(htTrav,i,j,EAST);
                    }

                CheckForPlayers(&pdtToDraw,&ptPlayers,htTrav->next,i+1,j,tx,ty);
                CheckForPlayers(&pdtToDraw,&ptDrones,htTrav->next,i+1,j,tx,ty);


                if ((b2&dForward)||(b3&dBackward)) {
                    htTrav = DrawRect(htTrav,i+1,j,NORTH);
                    }
                }

            if ((htTrav != (HoleType FAR *)NULL)&&(htHold == htTrav->next)) {
                htTrav = htTrav->next;
                }
            }
        }


    //
    // Draw the maze itself
    //

    PolyDraw(hDC,pDraw,pType,PointCount);

    //
    // We were looking for players as we went along. Draw all the ones we
    // found
    //
    DrawFoundPlayers(hDC,&pdtToDraw,rUpd);

    //
    // Clean up.
    //

    htTrav = &htLeft;
    while (htTrav->next != NULL) {
        htTrav->next = DelHole(htTrav->next);
    }

    htTrav = &htRight;
    while (htTrav->next != NULL) {
        htTrav->next = DelHole(htTrav->next);
    }



    DeleteObject(hWhiteBrush);
    DeleteObject(hWhitePen);
    DeleteObject(hBlackPen);
    hMem = (HGLOBAL) GlobalHandle(SELECTOROF(pType));
    GlobalUnlock(hMem);
    GlobalFree(hMem);
    hMem = (HGLOBAL) GlobalHandle(SELECTOROF(pDraw));
    GlobalUnlock(hMem);
    GlobalFree(hMem);
    SelectObject(hDC,hOldPen);

    return;
}



/*=====================================================================
Function: DrawTopView()

Inputs: DC, whether or not it's a total redraw

Outputs: none

Abstract:
    This routine takes care of the top maze view window. It draws white
    lines over black and vice versa to get the window drawn in the shortest
    possible time. A triangle is drawn in the middle of the window to
    represent the player and his current facing.
======================================================================*/
void DrawTopView(
    HDC hDC,
    BOOL bRedraw
    )
{
    int i,j,Step;
//
//BUGBUG -- these can be fixed after we have square pens
//
    // POINT p[5*5*4*2+5];
    // BYTE bType[5*5*4*2+5];
    LPPOINT p;
    //[3*5*5*4*2+5];
    LPBYTE bType;
    //[3*5*5*4*2+5];
    int cPoint,iPenWidth;
    int x,y;
    HPEN hBlackPen,hWhitePen,hPenOld;
    HGLOBAL hPMem,hBMem;

    hPMem = GlobalAlloc(GHND,(3*5*5*4*2 + 5)*sizeof(POINT));
    p = (LPPOINT) GlobalLock(hPMem);
    hBMem = GlobalAlloc(GHND,(3*5*5*4*2 + 5)*sizeof(BYTE));
    bType = (LPBYTE) GlobalLock(hBMem);

    cPoint = 0;

    Step = (rTopView.right - rTopView.left)/5;

    iPenWidth = Step/10+1;
    hBlackPen = CreatePen(PS_SOLID,iPenWidth,0);

    hWhitePen = CreatePen(PS_SOLID,iPenWidth,0x00FFFFFF);

    //
    // First, we need to draw black lines for all the missing lines in
    // the current map
    //
    hPenOld = SelectObject(hDC,hBlackPen);

    for (i =-3;i<=2; i++) {
        for (j= -3; j<=2; j++) {
            if (bRedraw ||
                (!((bMaze[ptLastPos.ix + i][ptLastPos.iy + j]&SOUTH)||
                (bMaze[ptLastPos.ix + i][ptLastPos.iy + j+1]&NORTH)))
               ) {

                if ((bMaze[ptSelf.Pos.ix + i][ptSelf.Pos.iy + j]&SOUTH)||
                    (bMaze[ptSelf.Pos.ix + i][ptSelf.Pos.iy + j+1]&NORTH)
                   ) {

                    p[cPoint].x = (i+2)*Step;
                    p[cPoint].y = (j+3)*Step;
                    bType[cPoint++] = PT_MOVETO;
                    p[cPoint].x = (i+3)*Step;
                    p[cPoint].y = (j+3)*Step;
                    bType[cPoint++] = PT_LINETO;
                    }
                }
            if (bRedraw ||
                (!((bMaze[ptLastPos.ix + i][ptLastPos.iy + j]&EAST)||
                 (bMaze[ptLastPos.ix + i+1][ptLastPos.iy + j]&WEST)))
               ) {

                if ((bMaze[ptSelf.Pos.ix + i][ptSelf.Pos.iy + j]&EAST)||
                    (bMaze[ptSelf.Pos.ix + i+1][ptSelf.Pos.iy + j]&WEST)
                   ) {

                    p[cPoint].x = (i+3)*Step;
                    p[cPoint].y = (j+2)*Step;
                    bType[cPoint++] = PT_MOVETO;
                    p[cPoint].x = (i+3)*Step;
                    p[cPoint].y = (j+3)*Step;
                    bType[cPoint++] = PT_LINETO;
                    }
                }
            }
        }

    PolyDraw(hDC,p,bType,cPoint);

    //
    // next, we draw white lines over BLACK lines drawn but no longer
    // needed.
    //
    SelectObject(hDC,hWhitePen);
    cPoint = 0;

    for (i =-3;i<=2; i++) {
        for (j= -3; j<=2; j++) {
            if ((!bRedraw) &&
               ((bMaze[ptLastPos.ix + i][ptLastPos.iy + j]&SOUTH)||
               (bMaze[ptLastPos.ix + i][ptLastPos.iy + j+1]&NORTH))
              ) {

                if (!((bMaze[ptSelf.Pos.ix + i][ptSelf.Pos.iy + j]&SOUTH)||
                   (bMaze[ptSelf.Pos.ix + i][ptSelf.Pos.iy + j+1]&NORTH))
                  ) {

                    p[cPoint].x = (i+2)*Step+iPenWidth;
                    p[cPoint].y = (j+3)*Step;
                    bType[cPoint++] = PT_MOVETO;
                    p[cPoint].x = (i+3)*Step-iPenWidth;
                    p[cPoint].y = (j+3)*Step;
                    bType[cPoint++] = PT_LINETO;
                    //
                    // We also need to kludge to clean up until we get a square pen.
                    //
                    p[cPoint].x = (i+2)*Step+iPenWidth;
                    p[cPoint].y = (j+3)*Step-iPenWidth;
                    bType[cPoint++] = PT_MOVETO;
                    p[cPoint].x = (i+2)*Step+iPenWidth;
                    p[cPoint].y = (j+3)*Step+iPenWidth;
                    bType[cPoint++] = PT_LINETO;
                    p[cPoint].x = (i+3)*Step-iPenWidth;
                    p[cPoint].y = (j+3)*Step-iPenWidth;
                    bType[cPoint++] = PT_MOVETO;
                    p[cPoint].x = (i+3)*Step-iPenWidth;
                    p[cPoint].y = (j+3)*Step+iPenWidth;
                    bType[cPoint++] = PT_LINETO;
                    //
                    // End kludge BUGBUG
                    //
                    }
                }
            if ((!bRedraw) &&
                ((bMaze[ptLastPos.ix + i][ptLastPos.iy + j]&EAST)||
                (bMaze[ptLastPos.ix + i+1][ptLastPos.iy + j]&WEST))
               ) {

                if (!((bMaze[ptSelf.Pos.ix + i][ptSelf.Pos.iy + j]&EAST)||
                    (bMaze[ptSelf.Pos.ix + i+1][ptSelf.Pos.iy + j]&WEST))
                   ) {

                    p[cPoint].x = (i+3)*Step;
                    p[cPoint].y = (j+2)*Step+iPenWidth;
                    bType[cPoint++] = PT_MOVETO;
                    p[cPoint].x = (i+3)*Step;
                    p[cPoint].y = (j+3)*Step-iPenWidth;
                    bType[cPoint++] = PT_LINETO;
                    //
                    // We also need to kludge to clean up until we get a square pen.
                    //
                    p[cPoint].x = (i+3)*Step-iPenWidth;
                    p[cPoint].y = (j+2)*Step+iPenWidth;
                    bType[cPoint++] = PT_MOVETO;
                    p[cPoint].x = (i+3)*Step+iPenWidth;
                    p[cPoint].y = (j+2)*Step+iPenWidth;
                    bType[cPoint++] = PT_LINETO;
                    p[cPoint].x = (i+3)*Step-iPenWidth;
                    p[cPoint].y = (j+3)*Step-iPenWidth;
                    bType[cPoint++] = PT_MOVETO;
                    p[cPoint].x = (i+3)*Step+iPenWidth;
                    p[cPoint].y = (j+3)*Step-iPenWidth;
                    bType[cPoint++] = PT_LINETO;
                    //
                    // End kludge BUGBUG
                    //
                    }
                }
            };
        }

    PolyDraw(hDC,p,bType,cPoint);



    SelectObject(hDC,hPenOld);
    DeleteObject(hBlackPen);
    DeleteObject(hWhitePen);

    //
    // And drawn an arrow for our ptSelf.Pos.Facing
    //
    x = y = (Step * 10) / 4;    // the center
    Step /= 4;

    if (ptLastPos.Facing != ptSelf.Pos.Facing) {
        if (hWhitePen != NULL) {
            DeleteObject(hWhitePen);
        }
        hWhitePen = CreatePen(PS_SOLID,1,0x00FFFFFF);
        hPenOld = SelectObject(hDC,hWhitePen);
        cPoint = 0;

        p[cPoint].x = p[cPoint+4].x = x;
        p[cPoint].y = p[cPoint+4].y = y;
        bType[cPoint+4] = PT_LINETO;
        bType[cPoint++] = PT_MOVETO;

        switch (ptLastPos.Facing) {
            case NORTH:
                p[cPoint].x = x+Step;
                p[cPoint].y = y;
                bType[cPoint++] = PT_LINETO;
                p[cPoint].x = x;
                p[cPoint].y = y-Step;
                bType[cPoint++] = PT_LINETO;
                p[cPoint].x = x-Step;
                p[cPoint].y = y;
                bType[cPoint++] = PT_LINETO;
                break;
            case SOUTH:
                p[cPoint].x = x+Step;
                p[cPoint].y = y;
                bType[cPoint++] = PT_LINETO;
                p[cPoint].x = x;
                p[cPoint].y = y+Step;
                bType[cPoint++] = PT_LINETO;
                p[cPoint].x = x-Step;
                p[cPoint].y = y;
                bType[cPoint++] = PT_LINETO;
                break;
            case EAST:
                p[cPoint].x = x;
                p[cPoint].y = y+Step;
                bType[cPoint++] = PT_LINETO;
                p[cPoint].x = x+Step;
                p[cPoint].y = y;
                bType[cPoint++] = PT_LINETO;
                p[cPoint].x = x;
                p[cPoint].y = y-Step;
                bType[cPoint++] = PT_LINETO;
                break;
            case WEST:
                p[cPoint].x = x;
                p[cPoint].y = y+Step;
                bType[cPoint++] = PT_LINETO;
                p[cPoint].x = x-Step;
                p[cPoint].y = y;
                bType[cPoint++] = PT_LINETO;
                p[cPoint].x = x;
                p[cPoint].y = y-Step;
                bType[cPoint++] = PT_LINETO;
                break;
            }

        cPoint++;
        PolyDraw(hDC,p,bType,cPoint);
        SelectObject(hDC,hPenOld);
        DeleteObject(hWhitePen);
        }

    cPoint = 0;

    p[cPoint].x = p[cPoint+4].x = x;
    p[cPoint].y = p[cPoint+4].y = y;
    bType[cPoint+4] = PT_LINETO;
    bType[cPoint++] = PT_MOVETO;


    switch (ptSelf.Pos.Facing) {
        case NORTH:
            p[cPoint].x = x+Step;
            p[cPoint].y = y;
            bType[cPoint++] = PT_LINETO;
            p[cPoint].x = x;
            p[cPoint].y = y-Step;
            bType[cPoint++] = PT_LINETO;
            p[cPoint].x = x-Step;
            p[cPoint].y = y;
            bType[cPoint++] = PT_LINETO;
            break;
        case SOUTH:
            p[cPoint].x = x+Step;
            p[cPoint].y = y;
            bType[cPoint++] = PT_LINETO;
            p[cPoint].x = x;
            p[cPoint].y = y+Step;
            bType[cPoint++] = PT_LINETO;
            p[cPoint].x = x-Step;
            p[cPoint].y = y;
            bType[cPoint++] = PT_LINETO;
            break;
        case EAST:
            p[cPoint].x = x;
            p[cPoint].y = y+Step;
            bType[cPoint++] = PT_LINETO;
            p[cPoint].x = x+Step;
            p[cPoint].y = y;
            bType[cPoint++] = PT_LINETO;
            p[cPoint].x = x;
            p[cPoint].y = y-Step;
            bType[cPoint++] = PT_LINETO;
            break;
        case WEST:
            p[cPoint].x = x;
            p[cPoint].y = y+Step;
            bType[cPoint++] = PT_LINETO;
            p[cPoint].x = x-Step;
            p[cPoint].y = y;
            bType[cPoint++] = PT_LINETO;
            p[cPoint].x = x;
            p[cPoint].y = y-Step;
            bType[cPoint++] = PT_LINETO;
            break;
        }

    cPoint++;
    PolyDraw(hDC,p,bType,cPoint);

    GlobalUnlock(hBMem);
    GlobalUnlock(hPMem);
    GlobalFree(hBMem);
    GlobalFree(hPMem);
}



/*=====================================================================
Function: DrawPlayers()

Inputs: DC, pointer to list of players, rectangle being updated on scrn

Outputs: none

Abstract:
    BUGBUG -- make an abstract for this.
======================================================================*/

void DrawPlayers(
    HDC hDC,
    PlayerType FAR *ptPlyr,
    LPRECT rUpd
    )
{
    LPRECT rHld;
    PlayerType FAR *ptTrav;
    HGLOBAL hMem;

    if (rUpd == NULL) {
        hMem = GlobalAlloc(GHND,sizeof(RECT));
        rHld = (LPRECT) GlobalLock(hMem);
        if (rHld == NULL) {
            MessageBox((HWND) NULL,"Unable to allocate a rectangle","DrawPlayers",MB_APPLMODAL);
            }
/******
        rHld->right = rMaze.left;
        rHld->left = rMaze.right;
        rHld->top = rMaze.bottom;
        rHld->bottom = rMaze.top;
******/
*rHld = rMaze;
        }
    else {
        rHld = rUpd;
        }


    ptTrav = ptPlyr;
    while (ptTrav->next != NULL) {
        ptTrav = ptTrav->next;
        if (ptTrav->Drawn) {
            rHld->left = LESSEROF(rHld->left,ptTrav->rDrawn.left);
            rHld->right = GREATEROF(rHld->right,ptTrav->rDrawn.right);
            rHld->top = LESSEROF(rHld->top,ptTrav->rDrawn.top);
            rHld->bottom = GREATEROF(rHld->bottom,ptTrav->rDrawn.bottom);
            }
        }

    PostMessage(hWndMaze,WM_COMMAND,IDM_REDRAW,(DWORD) rHld);
}
