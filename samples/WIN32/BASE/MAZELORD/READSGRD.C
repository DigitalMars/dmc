/***********************************************************************
File:   ReadSGrd.c
Author: JohnMil
Date:   11-16-92


Abstract:

    This module contains the entrypoint for ReadSubGrids(). This is a file
    to read in the text versions of the various subgrids which users can
    choose from. The subgrids are read into SubGrids[], and translated into
    their various cells with flags of NORTH SOUTH EAST WEST saying what
    walls are present for that cell.

    COMMENTS:   This file needs more error checking put into it. In particular,
                an error routine with popups for missing or invalid files.


Contents:

    ReadSubGrids() -- Reads subgrids in from the resources.

Revision History:

10-30-92 (JohnMil):
    Modified to new personal comment format
************************************************************************/

#include "winmaze.h"
#include "mazproto.h"



/*=====================================================================
Function:   ReadSubGrids()

Inputs:     none

Outputs:    returns success/failure

Abstract:
    This will read in subgrids from the linked resources, convert them
    from text to bytes with walls, filling the subgrid structre.
======================================================================*/

int ReadSubGrids(
    VOID
    )
{
    char GridName[]="SUBGRID00";
    HRSRC hGrid;
    LPSTR lpBuff;
    HGLOBAL hMem;

    int i,j,k,m;

    //
    // Initialize all cells of each subgrid to 0.
    //
    for (k=0;k<NUM_SUBGRIDS;k++) {
        for (i=0;i<X_CELLS_PER_SUBGRID;i++) {
            for (j=0;j<Y_CELLS_PER_SUBGRID;j++) {
                SubGrids[k].Cell[j][i] = 0;
                }
            }
        }

    //
    // Fill Subgrids with info from SubGrixx.TXT files
    //
    for(i=0;i<NUM_SUBGRIDS;i++) {       // Loop through all subgrids
        GridName[7]=(char) ('0'+(i/10));
        GridName[8]=(char) ('0'+(i%10));
        hGrid = FindResource(hInst,(LPCTSTR)GridName,(LPCTSTR)RT_RCDATA);
        if (hGrid == (HRSRC)NULL) {
            MessageBox((HWND)NULL,"Unable to load Subgrid Resource","ReadSubGrids",
                       MB_ICONEXCLAMATION|MB_APPLMODAL);
            }


        hMem = (HGLOBAL) LoadResource(hInst,hGrid);
        lpBuff = LockResource(hMem);
        if (lpBuff == (LPSTR)NULL) {
            MessageBox((HWND)NULL,"Unable to load Subgrid Resource","ReadSubGrids",
                         MB_ICONEXCLAMATION|MB_APPLMODAL);
            }



        j=0;

    //
    // For each file, loop through all lines. The top line
    // is ONLY the North wall for the first row.
    // The sixth row in the file will be the fifth row of the subgrid.
    //
    for(m=0;m<6;m++) {
        //
        // Get the NORTH/SOUTH walls.
        //
        for (k=1;k<2*X_CELLS_PER_SUBGRID;k+=2) {
            if (lpBuff[k] == '_') {
                if (j<Y_CELLS_PER_SUBGRID) {
                    SubGrids[i].Cell[k/2][j] |= NORTH;
                    }
                if (j>0) {
                    SubGrids[i].Cell[k/2][j-1] |= SOUTH;
                    }
                }
            }

        //
        // Get the EAST/WEST walls.
        //
        if (j > 0){
            for (k=0;k<2*X_CELLS_PER_SUBGRID+1;k+=2) {
                if (lpBuff[k] == '|') {
                    if ((k/2) < X_CELLS_PER_SUBGRID){
                        SubGrids[i].Cell[k/2][j-1] |= WEST;
                        }
                    if (((int) (k/2))>0) {
                        SubGrids[i].Cell[(k/2)-1][j-1] |= EAST;
                        }
                    }
                }
            }

        while (*lpBuff != '\0') {
            lpBuff++;
            }
        lpBuff++;

        j++;
        }
    }

    return(1);
}
