
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#define IDE_API                     101
#define IDE_RETSTAT                 102
#define IDB_STEP                    103
#define IDE_TIME                    104

#define DIALOGHEIGHT                80

#define WM_REPSTAT                  WM_USER+1


#define IDE_FILENAME                101
#define IDB_OPENFILE                102
#define IDE_MONTH                   103
#define IDE_DAY                     104
#define IDE_YEAR                    105
#define IDE_HOURS                   201
#define IDE_MINUTES                 202
#define IDE_SECONDS                 203
#define IDB_SET                     204
#define IDE_SIZE                    205
#define IDC_HIDE                    206
#define IDC_SYSTEM                  207
#define IDC_ARC                     208
#define IDC_READ                    209
#define IDC_NORMAL                  210
#define IDB_ATTR                    222


#define YRMASK        0xFE00
#define YRSHIFT       9

#define MONMASK       0x01E0
#define MONSHIFT      5

#define DAYMASK       0x001F
#define DAYSHIFT      0

#define HRMASK        0xF800
#define HRSHIFT       11
#define MINMASK       0x07E0
#define MINSHIFT      5
#define SECMASK       0x001F
#define SECSHIFT      0

LONG APIENTRY MainWndProc (HWND, UINT, UINT, LONG);
LONG APIENTRY FileDlgProc (HWND, UINT, UINT, LONG);
LONG APIENTRY StatusDlgProc (HWND, UINT, UINT, LONG);
BOOL RepStat (CHAR *, DWORD);
