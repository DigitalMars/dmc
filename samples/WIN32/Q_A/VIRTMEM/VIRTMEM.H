
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#define IDM_FREE           501
#define IDM_RESERVE        502
#define IDM_COMMIT         503

#define IDM_NOACCESS       504
#define IDM_READONLY       505
#define IDM_READWRITE      506

#define IDM_LOCK           901
#define IDM_UNLOCK         902

#define IDM_WRITE          701

#define IDM_SHOW           801

#define PAGESIZE           0x1000

LRESULT CALLBACK MainWndProc (HWND, UINT, UINT, LONG);
BOOL CALLBACK ShowDlgProc (HWND, UINT, UINT, LONG);
VOID ResetMenu (VOID);


#define IDE_BASEADDR                101
#define IDE_ALLOCBASE               102
#define IDE_INITPROT                103
#define IDE_SIZE                    104
#define IDE_STATE                   105
#define IDE_PROTECT                 106
