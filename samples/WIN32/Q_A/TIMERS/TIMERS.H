
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/******************************************************************************\
*
*                                  TIMERS.H
*
\******************************************************************************/



/******************************************************************************\
*                              SYMBOLIC CONSTANTS
\******************************************************************************/

#define ID_TIMER1      1
#define ID_TIMER2      2
#define ID_TIMER3      3
#define ID_TIMER4      4

#define BUFSIZE        20

#define BUTTONBORDER   5
#define BUTTONHEIGHT   25
#define TEXTHEIGHT     8

#define TIMERINTERVAL  250



/******************************************************************************\
*                               GLOBAL VARIABLES
\******************************************************************************/

HANDLE ghInst;
HWND   gahwndButtons[5];  /*  array of HWNDs for the start/stop timer
                           *  buttons. Note: the first element
                           *  (hwndButtons[0]) is unused (since it doesn't
                           *  correspond with a timer ID).
                           */
RECT   gaFlashRects[5];   /*  array of RECTs for the flashing rectandgles.
                           *  Note: the first element (flashRects[0]) is
                           *  unused (since it doesn't correspond with a
                           *  timer ID).
                           */


/******************************************************************************\
*                              FUNCTION PROTOTYPES
\******************************************************************************/

LRESULT CALLBACK MainWndProc  (HWND, UINT, WPARAM, LPARAM);
VOID    CALLBACK TimerFunc    (HWND, UINT, UINT, DWORD);
VOID SetButtonPositions    (int,  int);
VOID SetFlashRectPositions (LONG, LONG);
VOID DrawStuff             (HWND, HDC);
