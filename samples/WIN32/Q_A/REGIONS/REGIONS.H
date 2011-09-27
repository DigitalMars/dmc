
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
*                                    REGIONS.H
*
\******************************************************************************/



/******************************************************************************\
*                                SYMBOLIC CONSTANTS
\******************************************************************************/

#define IDM_ERASE           101    // menuitems
#define IDM_OFFSETRGN       102
#define IDM_INVERTRGN       103
#define IDM_FRAMERGN      104
#define IDM_PTINRGN         105
#define IDM_RECTINRGN       106
#define IDM_SETRECTRGN      107
#define IDM_GETRGNBOX       108
#define IDM_ABOUT           109

#define IDM_ELLIPSE         201
#define IDM_POLYPOLYGON     202
#define IDM_RECT            203

#define IDM_AND             301
#define IDM_COPY            302
#define IDM_DIFF            303
#define IDM_OR              304
#define IDM_XOR             305

#define ELLIPTIC_RGN        0X0001 // flags used in RGNSTRUCT.flags
#define POLYPOLYGON_RGN     0X0002
#define RECT_RGN            0X0004

#define MAXRGNSTRUCTS       3      // max # of rgns existing at 1 time

#define POLYPOLYGONCOUNT    3      // # of polygons in call to
                                   //   CreatePolyPolygonRgn

#define TRECT_NEW           1      // TrackRect() actions
#define TRECT_DELETE        2
#define TRECT_PAINT         3

#define  RESET_ALL          0      // Reset() actions
#define  ENABLE_OPTIONS     1
#define  ENABLE_COMBINERGN  2
#define  DISABLE_COMBINERGN 3



/******************************************************************************\
*                                    TYPEDEFS
\******************************************************************************/

typedef struct
{
  HRGN hrgn;
  WORD type;
} RGNSTRUCT;


typedef struct
{
    WORD   xOrigin, yOrigin;
    RECT   trackRect, clientRect;
    HDC    hdc;
} TRACKRECTSTRUCT, *PTRACKRECTSTRUCT;



/******************************************************************************\
*                                GLOBAL VARIABLES
\******************************************************************************/

HANDLE    hInst;
RGNSTRUCT rgnArray[MAXRGNSTRUCTS];
int       currRgn;
HMENU     hMenu;

RECT      ellipseRect = {40,  40, 180, 160};
POINT     polyPolygonPoints[] = {{120, 20}, {200, 20}, {200, 80},
                                                       { 80,140},
                                 { 20, 20}, { 80, 20}, { 20, 80},
                                 {200,120}, {200,200}, {160,200}};

int       polyPolygonCounts[] = {4,3,3};
RECT      rectRect    = {20, 100, 120, 200};



/******************************************************************************\
*                               FUNCTION PROTOTYPES
\******************************************************************************/

LRESULT CALLBACK MainWndProc   (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc  (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK RgnBoxDlgProc (HWND, UINT, WPARAM, LPARAM);

VOID             MyCreateRgn   (WORD);
VOID             MyCombineRgn  (int);
PTRACKRECTSTRUCT TrackRect     (PTRACKRECTSTRUCT, int, HWND, LONG);
VOID             Reset         (WORD);
