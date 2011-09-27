/***********************************************************************
 *  lowpass.h - Header file for LOWPASS sample application.
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
 *  Copyright (C) 1993 - 1995 Microsoft Corporation. All Rights Reserved.
 ***********************************************************************/

#define LOADSTRBUFSIZE 256	// 255 + nul is biggest stringtable string.

/* Constants for dialogs.
 */
#define IDM_ABOUT       11          // menu items

#define ID_INPUTFILEEDIT    101     // input file name edit box
#define ID_OUTPUTFILEEDIT   102     // output file name edit box


/* Function Prototypes
 */
// WinMain is prototyped in windows.h or its children
void DoLowPass(HWND hwnd);
BOOL FAR PASCAL AboutDlgProc(HWND hwnd, UINT wMsg,
	WPARAM wParam, LPARAM lParam);
BOOL WINAPI LowPassDlgProc(HWND hwnd, UINT wMsg,
	WPARAM wParam, LPARAM lParam);
