/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1992 - 1995  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/

#define  STRICT
#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include "puzzle.h"

#define MEM_COPY( hpd, hps, size ) memcpy( hpd, hps, size )
#define DATA_PTR BYTE *

#define WIDTHBYTES(i)     ((unsigned)((i+31)&(~31))/8)  /* ULONG aligned ! */
#define DIBWIDTHBYTES(bi) (DWORD)WIDTHBYTES((int)(bi).biWidth * (int)(bi).biBitCount)

//
// Initialize the puzzle, and optionally simulate 1000 clicks on the puzzle
//
void InitPuzzle(
LPPUZZLE p,
BOOL fScramble)
{
        int i,j;

        // Set the puzzle to a "solved" state
        for (i = 0; i < PSIZE; i++)
                for (j = 0; j < PSIZE; j++)
                        p->a[i][j] = i + j * PSIZE;

        // Put the "hole" in the lower right corner.
        p->a[PSIZE-1][PSIZE-1] = -1;
        p->hx = PSIZE - 1;
        p->hy = PSIZE - 1;

        if(fScramble) {
                // Make things really be random
                srand((unsigned int) timeGetTime());

                for (i = 0; i < 1000; i++) {
                        // Click on a random square
                        int r = rand() % PSIZE;
                        int s = rand() % PSIZE;
                        ClickPuzzle(p, r, s);
                }
        }
}

//
// Given a puzzle, and x & y in puzzle coordinates, move squares around
// or not as appropriate, given how such puzzles work.
//
void ClickPuzzle(
LPPUZZLE p,
int x,
int y)
{
        int i;

        if (x < 0 || x >= PSIZE)
                return;

        if (y < 0 || y >= PSIZE)
                return;

        if (x == p->hx) {
                if (y < p->hy) {
                        for (i = p->hy; i > y; i--)
                                p->a[x][i] = p->a[x][i-1];
                } else if (y > p->hy) {
                        for (i = p->hy; i < y; i++)
                                p->a[x][i] = p->a[x][i+1];
                }
                p->hy = y;
                p->a[x][y] = -1;
        } else if (y == p->hy) {
                if (x < p->hx) {
                        for (i = p->hx; i > x; i--)
                                p->a[i][y] = p->a[i-1][y];
                } else if (x > p->hx) {
                        for (i = p->hx; i < x; i++)
                                p->a[i][y] = p->a[i+1][y];
                }
                p->hx = x;
                p->a[x][y] = -1;
        }

        // We could potentially see if the puzzle was solved here.
        // If we do that, the prototype should change to
        // BOOL ClickPuzzle(LPPUZZLE p, int x, int y, BOOL fCheckSolved)
        // where we would pass TRUE for fCheckSolved if the call was
        // a result of the user really clicking, and not a call from
        // InitPuzzle() or something....
}


//
// Given a puzzle, map the input picture to the output picture with squares
// rearranged.
//
// Works on any RGB DIB.  Doesn't work on bitmaps, probably, so could be a
// problem with Todd's new DrawDib.
//
void MixPicture(
LPPUZZLE p,
LPBITMAPINFOHEADER lpbi,
LPBYTE lpIn,
LPBYTE lpOut)
{
        int i,j;
        int y;
        char achHack[1024] = {0};
        int dx = ((int) lpbi->biWidth / PSIZE) * ((int) lpbi->biBitCount / 8);
        int dy = (int) lpbi->biHeight / PSIZE;
        LONG lRowBytes = DIBWIDTHBYTES(*lpbi);
        DATA_PTR lpI;
        DATA_PTR lpO;

        for (i = 0; i < PSIZE; i++) {
                for (j = 0; j < PSIZE; j++) {
                        // Get pointer to square we're copying into
                        lpO = (DATA_PTR) lpOut + (PSIZE - 1 - j) * dy * lRowBytes + dx * i;

                        if (p->a[i][j] >= 0) {
                                // Get pointer to square we're copying from
                                lpI = (DATA_PTR) lpIn +
                                                (PSIZE - 1 - (p->a[i][j] / PSIZE)) * dy * lRowBytes +
                                                dx * (p->a[i][j] % PSIZE);

                                // do the copy
                                for (y = 0; y < dy; y++) {
                                        MEM_COPY(lpO, lpI, dx);
                                        lpO += lRowBytes;
                                        lpI += lRowBytes;
                                }
                        } else {
                                // clear the square to zeroes
                                for (y = 0; y < dy; y++) {
                                        MEM_COPY(lpO, achHack, dx);
                                        lpO += lRowBytes;
                                }
                        }
                }
        }
}
