
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/


//
// Forward declarations in FILER.C
//
BOOL    InitializeApp           (void);
int     GetDriveBitmap          (LPBINFO);
BOOL    ChangeDrive             (LPTSTR, DWORD);
BOOL    RunCommandItem          (LPCINFO);
BOOL    UpdateDrivesMenu        (HMENU, HANDLE);
void    ErrorMsg                (LPTSTR);

LRESULT WINAPI MainWndProc      (HWND, UINT, WPARAM, LPARAM);
LRESULT WINAPI AboutProc        (HWND, UINT, WPARAM, LPARAM);
LRESULT WINAPI DriveBarProc     (HWND, UINT, WPARAM, LPARAM);
LRESULT WINAPI FunctionBarProc  (HWND, UINT, WPARAM, LPARAM);
