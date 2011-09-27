
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*  wxform.h -- header file for World transform sample.  */


LRESULT CALLBACK MainWndProc     (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK TransformDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MouseDlgProc    (HWND, UINT, WPARAM, LPARAM);



/* dialog id's for the Transform dialog */
#define IDD_EM11        101
#define IDD_EM12        102
#define IDD_EDX         103
#define IDD_EM21        104
#define IDD_EM22        105
#define IDD_EDY         106
#define IDD_13          107
#define IDD_23          108
#define IDD_33          109
#define IDD_SETXFORM    110
#define IDD_IDENTITY    111


/* dialog id's for the Mouse dialog */
#define IDD_SCREENX     201
#define IDD_SCREENY     202
#define IDD_DEVICEX     203
#define IDD_DEVICEY     204
#define IDD_WORLDX      205
#define IDD_WORLDY      206


/* special messages for doTrackobject(). */
#define TROB_NEW         WM_USER+1
#define TROB_DELETE      WM_USER+2
#define TROB_PAINT       WM_USER+3
#define TROB_HITTEST     WM_USER+5
#define TROB_CENTER      WM_USER+6
#define TROB_SETXFORM    WM_USER+7

/* special user message for two of the dialogs */
#define WM_PUTUPFLOATS  WM_USER+1


/* Different "Track modes." */
#define TMNONE    0x0000
#define TMMOVE    0x0001
#define TMSIZEX   0x0002
#define TMSIZEY   0x0004
#define TMSIZEXY  0x0006
#define TMSHEARX  0x0010
#define TMSHEARY  0x0020
#define TMROTATE  0x0008


/* misc. defines for drawing and formating */
#define TICKSPACE 20
#define MAXCHARS  32
#define INC   5
#define FORMATFLOAT    "%1.2f"
#define GRIDCOLOR (COLORREF) 0x01000006



/* structure for the track object.  c.f. ptoRect */
typedef struct tagTrackObject{
    RECT   rect;
    XFORM  xfmChange;   /* World coordinate transform      */
    XFORM  xfmDown;     /* transform when the mouse down happens. */
    HDC    hdc;
    int    Mode;        /* TMROTATE, TMSIZEXY, TMMOVE, ... */
    struct tagTrackObject* Next;
} TrackObject, *PTrackObject;


/* function prototypes. */
PTrackObject doTrackObject(PTrackObject, int, HWND, LONG);
VOID MouseMove(PTrackObject, int, HWND, LONG);
VOID CenterOrigin (HWND, HDC);


/* Global variables. */
BOOL showTransform, showMouse, showHelp;
HWND hwndTransform, hwndMouse, hwndHelp;
PTrackObject ptoRect = NULL;
HANDLE hInst;
HWND   hwndMain;
