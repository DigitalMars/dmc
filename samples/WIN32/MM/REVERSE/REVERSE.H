/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
 * 
 **************************************************************************/

/* reverse.h - Header file for REVERSE sample application.
 *

 */


/*
 *  Constants
 */
#define LOADSTRBUFSIZE 256 // 255 + nul is biggest stringtable string.

#ifndef GET_WM_COMMAND_ID	// not in WIN16 windowsx.h
#define GET_WM_COMMAND_ID(wp, lp)               LOWORD(wp)
#endif

// Child window identifiers
#define IDE_NAME        200
#define IDB_PLAY        201
#define IDB_QUIT        202

// Window Position and size definitions
#define WMAIN_DX        207
#define WMAIN_DY        120
#define NAME_DX         180
#define NAME_DY         30
#define NAME_X          10
#define NAME_Y          10
#define PLAY_DX         85
#define PLAY_DY         35
#define PLAY_X          10
#define PLAY_Y          50
#define QUIT_DX         85
#define QUIT_DY         35
#define QUIT_X          105
#define QUIT_Y          50

#define IDM_ABOUT           0x101

/*
 *  Function prototypes
 */
BOOL FAR PASCAL AppAbout(HWND, UINT ,WPARAM, LPARAM);
LONG FAR PASCAL WndProc(HWND, UINT ,WPARAM, LPARAM);
void Interchange(HPSTR, HPSTR, WORD);
void ReversePlay(void); 
VOID cleanup(void);

// message cracker things
#define MyDefProc    DefWindowProc
void ReverseOnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void ReverseOnDestroy(HWND hwnd);
void ReverseOnSysCommand(HWND hwnd, UINT cmd, int x, int y);
