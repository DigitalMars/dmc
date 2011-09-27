
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
#endif

#define BOUNCECLASS  "BOUNCEDEMO"
#define BOUNCETITLE  "Bouncing Ball Demo (xforms)"


typedef struct _MYXFORM
{
    int xForm[3][3];
} MYXFORM;
typedef MYXFORM      *PMYXFORM;
typedef MYXFORM NEAR *NPMYXFORM;
typedef MYXFORM FAR  *LPMYXFORM;


/*
** BALL OBJECT
*/
typedef struct _BALLDATA
{
    HWND     hWnd;
    int      nWidth;
    int      nHeight;
    int      xDirection;
    int      yDirection;
    BOOL     bNewPosition;
    int      xPosition;
    int      yPosition;
    COLORREF crColor;
    MYXFORM  xForm;
} BALLDATA;
typedef BALLDATA      *PBALLDATA;
typedef BALLDATA NEAR *NPBALLDATA;
typedef BALLDATA FAR  *LPBALLDATA;

HANDLE ballCreate(HWND,int,int,COLORREF);
BOOL   ballDestroy(HANDLE);
VOID   ballBounce(HANDLE);
BOOL   ballSetDimensions(HANDLE,int,int);
BOOL   ballGetDimensions(HANDLE,LPPOINT);
BOOL   ballSetDirection(HANDLE,int,int);
BOOL   ballGetDirection(HANDLE,LPPOINT);
BOOL   ballGetPosition(HANDLE,LPPOINT);
BOOL   ballSetPosition(HANDLE,int,int);




typedef struct _BOUNCEDATA
{
    HANDLE hBall1;
    HANDLE hBall2;
    HANDLE hBall3;
    HANDLE hBall4;
} BOUNCEDATA;
typedef BOUNCEDATA      *PBOUNCEDATA;
typedef BOUNCEDATA NEAR *NPBOUNCEDATA;
typedef BOUNCEDATA FAR  *LPBOUNCEDATA;


/*
** DRAW WINDOW ROUTINES (draw.c)
*/
HWND  FAR      CreateBounceWindow(HWND,int);
LONG  APIENTRY BounceProc(HWND,UINT,WPARAM,LONG);
BOOL           BounceCreateProc(HWND);
VOID           BounceDestroyProc(HWND);
BOOL           BounceCommandProc(HWND,WPARAM,LONG);
VOID           BouncePaintProc(HWND);

VOID           BounceObjects(HWND);
VOID           BounceRefresh(HWND);


VOID           CheckEdgePosition(HWND,HANDLE);
