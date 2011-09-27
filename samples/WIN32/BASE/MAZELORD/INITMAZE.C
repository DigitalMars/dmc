/***********************************************************************
File:   InitMaze.c
Author: JohnMil
Date:   5-10-92


Abstract:

    This module contains code to initialize the full maze, as well as the
    data structures associated with it.

Contents:

    InitMaze() -- Inserts subgrids into the Maze array, sets player position
    Calc3DMaze() -- Calculates posts used for drawing 3-d view
    InsertSubGrid() -- copies a sub-grid into the maze


Revision History:

10-30-92 (JohnMil):
    Modified to new personal comment format

************************************************************************/

#include "winmaze.h"
#include "mazproto.h"
#include "math.h"


/*=====================================================================
Function: InitMaze()

Inputs: none

Outputs: none

Abstract:
    InitMaze is responsible for initializing the full maze to blocked
    characters, and setting up the temple space.
======================================================================*/

void InitMaze(
    void
    )
{
    int i,j;
    BYTE b;

    //
    // Initialize the entire maze to being filled already.
    // we'll leave the outer subgrid ring as a buffer.
    //
    b = NORTH | WEST | SOUTH | EAST;

    for(i=0;i<X_SIZE;i++) {
        for (j=0;j<Y_SIZE;j++) {
            bMaze[i][j] = b;
            }
        }

    InsertSubGrid(NUM_PLAYER_SUBGRIDS,6,6);
    InsertSubGrid(NUM_PLAYER_SUBGRIDS+1,7,6);
    InsertSubGrid(NUM_PLAYER_SUBGRIDS+2,5,7);
    InsertSubGrid(NUM_PLAYER_SUBGRIDS+3,6,7);
    InsertSubGrid(NUM_PLAYER_SUBGRIDS+4,7,7);
    InsertSubGrid(NUM_PLAYER_SUBGRIDS+5,6,8);
    InsertSubGrid(NUM_PLAYER_SUBGRIDS+6,7,8);

    //
    // Kludge to start new players out in the sanctuary
    //
    ptSelf.Pos.ix = 6*X_CELLS_PER_SUBGRID+1;
    ptSelf.Pos.iy = 7*Y_CELLS_PER_SUBGRID+1;
    ptSelf.Pos.Facing = NORTH;
    ptLastPos = ptSelf.Pos;

    return;
}



/*=====================================================================
Function: Calc3DMaze()

Inputs: none

Outputs: none

Abstract:
    Calc maze is responsible for recalculating the maze drawing info.
    3-d transformations are done only when the window is resized. The
    results are stored in a table (giving the polygons for each panel, from
    left to right in 2-d coordinates.
======================================================================*/

void Calc3DMaze(
     )
{
    int i,j,k,width,depth;
    float pw, ph,x[3],y[3], dx, dyh, dyl, dz, dist;
    POINT pCenter;
    float m,b;


    SetCursor(LoadCursor((HINSTANCE)NULL,IDC_WAIT));

    pw = (float) PANEL_WIDTH;
    ph = (float) PANEL_HEIGHT;

    //
    // establish the scale. Should be sufficient to allow the end of the panel
    // we're in to show.  A full panel displayed 1/2 panel away should be 2/3
    // of the screen wide.
    //
    width = (rMaze.right - rMaze.left);
    depth = (rMaze.bottom - rMaze.top);

    //
    // Since scale is used in MC_TO_SC, we need to nullify its effect to set it
    //
    scale = (float) 1.0;
    scale = (float) (.9 * depth/((float)MC_TO_SC(ph,-pw/2)));


    pCenter.x = rMaze.left + width/2;
    pCenter.y = rMaze.top + depth/2;

    //
    // x and y center
    //
    dyl = ph /2;
    dyh = - dyl;

    for(i=0;i<(MAX_DRAW_DIST*2+2);i++) {
        for (j=1;j<(MAX_DRAW_DIST+2);j++) {

            //
            // Calculate post in left position
            //

            dx = (i - (MAX_DRAW_DIST+1)) * pw - pw /2;
            dz = (j-1)*pw - pw/2;

            //
            // left top
            //
            dist = dz;

            pPost[i][j][0].x = pCenter.x + (int) MC_TO_SC(dx,dist);
            pPost[i][j][0].y = pCenter.y + (int) MC_TO_SC(dyh,dist);

            //
            // left bottom
            //
            dist = dz;
            pPost[i][j][1].x = pCenter.x + (int) MC_TO_SC(dx,dist);
            pPost[i][j][1].y = pCenter.y + (int) MC_TO_SC(dyl,dist);

            }
        }

    //
    // Clip as necessary, using line equation: y=mx + b.
    //
    for (i=0,j=0;i<(MAX_DRAW_DIST*2+2);i++) {
        for (k=0;k<2;k++) {
            x[1] = (float) pPost[i][1][k].x;
            x[2] = (float) pPost[i][2][k].x;
            y[1] = (float) pPost[i][1][k].y;
            y[2] = (float) pPost[i][2][k].y;
            m = (y[2] - y[1])/(x[2]-x[1]);
            b = y[1] - m*x[1];
            if (i < MAX_DRAW_DIST+2) {
                pPost[i][0][k].x = rMaze.left;
                }
            else {
                pPost[i][0][k].x = rMaze.right;
                }
            pPost[i][0][k].y = (int)(m*((float)pPost[i][0][k].x) + b);
            }
        }

    SetCursor(LoadCursor((HINSTANCE)NULL,IDC_ARROW));

    return;
}



/*=====================================================================
Function: InsertSubGrid()

Inputs: # of subgrid, x and y position in maze to insert the subgrid

Outputs: none

Abstract:
    Insert Subgrid will copy the specified subgrid into bMaze, with its upper
    left corner at iXPos*X_CELLS_PER_SUBGRID, iYPos*Y_CELLS_PER_SUBGRID.
======================================================================*/

void InsertSubGrid(
    int SubGridNo,
    int iXPos,
    int iYPos
    )
{
    int i,j;

    iXPos = iXPos*X_CELLS_PER_SUBGRID;
    iYPos = iYPos*Y_CELLS_PER_SUBGRID;


    for (i=0;i<X_CELLS_PER_SUBGRID;i++) {
        for (j=0;j<Y_CELLS_PER_SUBGRID;j++) {
            bMaze[i+iXPos][j+iYPos] = SubGrids[SubGridNo].Cell[i][j];
            }
        }

    return;
}
