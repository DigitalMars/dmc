
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#define MAX_PIPE_INSTANCES 100

#define LINE_LEN   80
#define NAME_SIZE  25

// Bitmaps and coordinates.
#define END_LIVE   1
#define END_DEAD   2
#define MID_LIVE   3
#define MID_DEAD   4
#define BITMAP_X   20
#define BITMAP_Y   32
#define NAME_X     60
#define NAME_Y     10

// Buffers and pipe symbolic constants.
#define IN_BUF_SIZE        1000
#define OUT_BUF_SIZE       1000
#define TIME_OUT           0
#define PLEASE_READ        1000
#define PLEASE_WRITE       1000

// Structure used to hold information about a pipe client.
// They're used as array elements.
typedef struct { HANDLE     hPipe;
                 HANDLE     hEvent;
                 OVERLAPPED overLap;
                 BOOL       live;
                 CHAR Name[NAME_SIZE];
               } WRTHANDLE;


LONG CALLBACK MainWndProc (HWND, UINT, WPARAM, LPARAM);
VOID          ServerProc (HWND *);
VOID          TellAll    (CHAR *);
VOID          DrawBranch(HDC);
