
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


#define POLYCLASS  "POLYDEMO"
#define POLYTITLE  "PolyBezier Demo"

#define MAX_BEZIER 80
#define VELMAX     10
#define VELMIN      2
#define BEZ_CURVES  1              //Number of Curves in the Bezier
#define BEZ_PTS    3*BEZ_CURVES+1  //Number of Points is the #Curves * 3 + 1 


/*
** This structue defines a basic bezier curve.  It is intended to be used in
** an array of beziers.
*/
typedef struct _BEZBUFFER
{
    POINT pPts[BEZ_PTS];
} BEZBUFFER;
typedef BEZBUFFER      *PBEZBUFFER;
typedef BEZBUFFER NEAR *NPBEZBUFFER;
typedef BEZBUFFER FAR  *LPBEZBUFFER;


/*
** This is the main object for the polybezier window.  This structure defines
** a series of bezier curves and the distance between each curve.
*/
typedef struct _POLYDATA
{
    int      nBezTotal;
    int      nBezCurr;
    int      nColor;
    POINT    pVel[BEZ_PTS];
    HANDLE   hBezBuffer;
} POLYDATA;
typedef POLYDATA      *PPOLYDATA;
typedef POLYDATA NEAR *NPPOLYDATA;
typedef POLYDATA FAR  *LPPOLYDATA;


/*
** POLYBEZIER WINDOW ROUTINES (poly.c)
*/
HWND  FAR      CreatePolyWindow(HWND,int);
LONG  APIENTRY PolyProc(HWND,UINT,WPARAM,LONG);
BOOL           PolyCreateProc(HWND);
VOID           PolyDestroyProc(HWND);
BOOL           PolyCommandProc(HWND,WPARAM,LONG);
VOID           PolyPaintProc(HWND);


VOID           PolyRedraw(HWND);
int            PolyNewVel(int);
VOID           PolyInitPoints(HWND);
VOID           PolyDrawBez(HWND);
