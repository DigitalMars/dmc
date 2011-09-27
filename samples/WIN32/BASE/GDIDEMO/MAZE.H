
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/


#ifdef WIN16
#define APIENTRY FAR PASCAL
typedef WORD WPARAM;
//#else
//typedef DWORD WPARAM;
#endif


#define MAZECLASS  "MAZEDEMO"
#define MAZETITLE  "Maze Demo"


typedef struct _MAZEDATA
{
    int nMaze;
} MAZEDATA;
typedef MAZEDATA      *PMAZEDATA;
typedef MAZEDATA NEAR *NPMAZEDATA;
typedef MAZEDATA FAR  *LPMAZEDATA;


/*
** MAZE WINDOW ROUTINES (maze.c)
*/
HWND  FAR      CreateMazeWindow(HWND,int);
LONG  APIENTRY MazeProc(HWND,UINT,WPARAM,LONG);
BOOL           MazeCreateProc(HWND);
VOID           MazeDestroyProc(HWND);
BOOL           MazeCommandProc(HWND,WPARAM,LONG);
VOID           MazePaintProc(HWND);


VOID DrawMaze(HWND);
