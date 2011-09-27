
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/* track.h - header file for doTrackObject support */


/* special messages for doTrackobject(). */
#define TROB_NEW         WM_USER+1
#define TROB_DELETE      WM_USER+2
#define TROB_PAINT       WM_USER+3
#define TROB_HITTEST     WM_USER+5


/* Different "Track modes." */
#define TMNONE    0x0000
#define TMMOVE    0x0001
#define TMSIZEX   0x0002
#define TMSIZEY   0x0004
#define TMSIZEXY  0x0006
#define TMSHEARX  0x0010
#define TMSHEARY  0x0020
#define TMROTATE  0x0008
#define TMALL     0x00ff



/* structure for the track object.  */
typedef struct tagTrackObject{
    RECT   rect;
    XFORM  xfmChange;   /* World coordinate transform      */
    XFORM  xfmDown;     /* transform when the mouse down happens. */
    HDC    hdc;
    int    Mode;        /* TMROTATE, TMSIZEXY, TMMOVE, ... */
    int    allowedModes;
    RECT   rectClip;
    struct tagTrackObject* Next;
} TrackObject, *PTrackObject;


/* function prototypes. */
PTrackObject doTrackObject(PTrackObject, int, HWND, LONG);
VOID MouseMove(PTrackObject, int, HWND, LONG);
