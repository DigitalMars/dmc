// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: listbx.c
//
//  PURPOSE: Handles general routines for the list box control;
//
//  FUNCTIONS:
//    CreateLBox    - Creates the Listbox control for the sample.
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include "globals.h"            // prototypes specific to this application
#include "listbx.h"

// Global Variables for the list box  control module

HWND    hWndLBoxCtl;
int     listTabs[4];  // Tabstops for the list box.


extern int gToolbarHeight, gStatusbarHeight, gHeaderCtlHeight;


//
//  FUNCTION: CreateLBox(HWND)
//
//  PURPOSE:  Calls CreateWindow() to create the Listbox
//
//
//  PARAMETERS:
//
//  hwnd - Window handle : Used for the hWndParent parameter of the control.
//
//  RETURN VALUE:
//
//  If ListBox control was created successfully Return TRUE,
//  else returns FALSE.
//
//  COMMENTS:
//
//

BOOL CreateLBox(HWND hWndParent)
{               
    RECT rect;
    
    
    GetClientRect(hWndParent, &rect);
    listTabs[0] = rect.right/8;
    listTabs[1] = listTabs[0] * 2;
    listTabs[2] = listTabs[0] * 3;
    hWndLBoxCtl = CreateWindowEx (WS_EX_CLIENTEDGE,
                              "listbox", 
                              (LPSTR)NULL, 
                               
                               WS_CHILD | 
                               WS_VISIBLE | 
                               WS_VSCROLL |
                               WS_BORDER |
                               LBS_USETABSTOPS | 
                               LBS_NOINTEGRALHEIGHT,
                               0, 
                               gHeaderCtlHeight + gToolbarHeight, 
                               rect.right,
                               rect.bottom - 
                               gToolbarHeight - 
                               gStatusbarHeight,
                               hWndParent,
                               (HMENU)IDM_LISTBOX, 
                               hInst,
                              (LPVOID)NULL);
    
    SendMessage(hWndLBoxCtl, 
                LB_SETTABSTOPS, 
                TOTAL_TABS,
                (LPARAM)(LPINT)listTabs);
                
    return TRUE;
}

