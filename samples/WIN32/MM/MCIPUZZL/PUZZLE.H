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

#define PSIZE   4       // Puzzle is 4x4

typedef struct {
    int a[PSIZE][PSIZE];        // square # at position [x][y]
    int hx;                     // coordinates of "hole"
    int hy;
} PUZZLE;

typedef PUZZLE * LPPUZZLE;

void InitPuzzle(LPPUZZLE, BOOL);
void ClickPuzzle(LPPUZZLE, int, int);
void MixPicture(LPPUZZLE, LPBITMAPINFOHEADER, LPBYTE, LPBYTE);
