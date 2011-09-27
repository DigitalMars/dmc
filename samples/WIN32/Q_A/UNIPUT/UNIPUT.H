
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/* uniput.h - header file */

/* function prototypes, window procedures first. */
LRESULT CALLBACK MainWndProc    (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DisplayWndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK StatusWndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK About (HWND, UINT, WPARAM, LPARAM);

int  openDisplay(VOID);
int  closeDisplay(VOID);

/* Name of the display window window class */
#define DISPLAYCLASS TEXT("DisplayClass")


/* Define a value for the LOGFONT.lfCharSet
 *  This should be included in wingdi.h, but it
 *  was removed because the font mapper is not
 *  using it anyway in version 1.0.  Currently
 *  scheduled to be included in NT ver 1.1.
 */
#define UNICODE_CHARSET  1


/* menu IDs */
#define  IDM_MDICASCADE     1001
#define  IDM_MDITILE        1002

#define  IDM_SHOWHEX        2000
#define  IDM_SHOWNAMES      2001

#define    IDM_CHARBASE     2400
#define    IDM_CHAR10       2410
#define    IDM_CHAR16       2416
#define    IDM_CHAR20       2420
#define    IDM_CHAR32       2432
#define    IDM_CHAR40       2440
#define    IDM_CHAR64       2464

#define    IDM_FONTBASE     2500
#define    IDM_FONT18       2518
#define    IDM_FONT36       2536
#define    IDM_FONT72       2572


#define  IDM_NEWWINDOW      3001
#define  IDM_NEWFILE        3002
#define  IDM_CLOSEFILE      3003

#define  IDM_HELP           4001
#define  IDM_ABOUT          4002


/* USER messages sent between windows. */
#define  WMU_NEWRANGE       WM_USER+0x1001
#define  WMU_NEWFONT        WM_USER+0x1002
#define  WMU_SETTARGETNAME    WM_USER+0x1003
#define  WMU_SETNCHAR       WM_USER+0x1004
#define  WMU_CHARACTER      WM_USER+0x1005


/* define strings and flags for use in warning message boxes */
#define MBGETFONTDATAERR    TEXT("Not a scalable font.")
#define MBERROR             TEXT("Application Error.")
#define MBERRORFLAGS        MB_OK | MB_ICONHAND


/* properties of the "Lucida Sans Unicode" font */
#define UCFONTHEIGHT       36
#define UCFONTWIDTH        16

/* spacing for the fields in the status bar... */
#define SBORDER 6
#define SFIELD0 160
#define SFIELD1  80



/* global variables */
extern HANDLE  hInst;
extern HWND    hwndMain;
extern HWND    hwndClient;
extern HWND    hwndStatus;
extern HWND hwndStatic0, hwndStatic1;

extern int gShowhex, gShowNames;
