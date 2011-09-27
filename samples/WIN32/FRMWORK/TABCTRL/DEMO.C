// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:    Demo.c
//
//  PURPOSE:   Displays 
//
//  FUNCTIONS:
//    DemoInit         - Calls initialization routines
//    CreateTabControl - Creates the tab control window. 
//    InitTabControl   - Initializes the tab control with tabs
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <commctrl.h>
#include "globals.h"            // prototypes specific to this application
#include "demo.h"
 

HWND hwndTabControl;            // Global Window handle for tab control.
HWND hwndDisplayChild;
BOOL    CreateTabControl    (HWND hwndParent);
void    InitializeTabControl(HWND hwndTabControl);


//
//  FUNCTION: DemoInit (HWND)
//
//  PURPOSE: Initializes the tab control
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//
//  RETURN VALUE:
//    BOOL - to indicate success/failure
//
//  COMMENTS:
//    
//

BOOL DemoInit(HWND hwnd)
{
    
    // initialize common controls..
    InitCommonControls();

    // Call CreateTabControl() to create the tab control window. 
    if(!CreateTabControl(hwnd))
    {
        MessageBox(hwnd, 
                  (LPSTR)"ERROR: TAB CONTROL WAS NOT CREATED: CLOSING WINDOW)", 
                  (LPSTR)"ERROR", 
                  MB_OK);
        return 0;

    }
    // Call InitializeTabControl() to initialize the control and to create 
    // the property page dialogs. 

    InitializeTabControl(hwndTabControl);
    return TRUE;
}


  

//
//  FUNCTION: CreateTabControl(HWND)
//
//  PURPOSE:  Create the Tab Control.
//
//  PARAMETERS:
//    hwnd     - Parent Window Handle.
//
//  RETURN VALUE:
//    returns TRUE if the tab control was created, else false.
//
//  COMMENTS:
//    This function calls CreateWindow to create the tab control. The hwnd
//    that is passed to this function is the parent window handle.
//

BOOL CreateTabControl(HWND hwndParent)
{
    RECT dispRect;

    
    GetClientRect(hwndParent, &dispRect);
    hwndTabControl = CreateWindow(WC_TABCONTROL,
                                  "",
                                  WS_CHILD | WS_VISIBLE | WS_TABSTOP |
                                  WS_CLIPSIBLINGS |
                                  TCS_TOOLTIPS,  
                                  dispRect.left  + 20,
                                  dispRect.top   + 20,
                                  dispRect.right - 50,
                                  dispRect.bottom- 50,
                                  hwndParent,
                                  (HMENU)IDM_TABCONTROL,
                                  hInst,
                                  (LPVOID) NULL);

    if(hwndTabControl)
        return TRUE;
    else
        return FALSE;
}


//
//  FUNCTION: InitializeTabControl(HWND)
//
//  PURPOSE:  Initialize the Tab Control.
//
//  PARAMETERS:
//    hwnd     - TabControl  Window Handle.
//
//  RETURN VALUE:
//    None.
//
//  COMMENTS:
//    This function calls TabCtrl_InsertItem to insert items into
//    the tab control. The hwnd that is passed to this function is
//    is the  window handle of the tab control created earlier.
//    It also creates the child modeless dialogs that are displayed
//    as property sheets.
//

void InitializeTabControl(HWND hwndTabControl)
{
    int         i;                    // variable used in the FOR loop.
    RECT        tabRect, tabItemRect; // Rectangles to position Page Dialogs.
    char        szBuffer[64];
    TC_ITEM     myTCItem;             // Array of TC_ITEM structures
    HIMAGELIST  hWndImageList;        // Image list window handle.
    HBITMAP     hBmp;


    TabCtrl_GetItemRect (hwndTabControl, 0, &tabItemRect);
    GetClientRect (hwndTabControl, &tabRect);

    // Initialize the image list by creating it first. 
    // Our Tab Control uses IMAGELISTS. 
    
    hWndImageList = ImageList_Create (32, 32, FALSE, 4, 4);
    if (!hWndImageList)
        MessageBox(NULL, "Image List not initialized", "ERROR", MB_OK);

    hBmp = LoadBitmap (hInst, "IMAGES");
    ImageList_Add (hWndImageList, hBmp, NULL);
    DeleteObject (hBmp);


    // Fail if not all images were added.
    if (ImageList_GetImageCount (hWndImageList) < 4)
        MessageBox(NULL, "Bitmaps not added to Image List", "ERROR", MB_OK);
    
    // Initialize the Tab control with text and images from the 
    // resource and image
    for(i = 0; i < 4; i++)
    {

        // Load the string for the TABS from the RC file.
        LoadString(hInst, IDS_START+i, szBuffer, sizeof(szBuffer));

        myTCItem.mask      = (UINT)TCIF_TEXT | TCIF_IMAGE;
        myTCItem.pszText   = (LPSTR)szBuffer;
        myTCItem.cchTextMax= sizeof(szBuffer);
        myTCItem.iImage    = i;

        TabCtrl_InsertItem (hwndTabControl,
                           (int) i,
                           (TC_ITEM FAR*)&myTCItem);

    }


    TabCtrl_SetImageList(hwndTabControl, hWndImageList);
    TabCtrl_SetCurSel (hwndTabControl, 0);
    SetFocus (hwndTabControl);

    // Create child window inside tab control
    // to display bitmap.
    hwndDisplayChild = CreateWindowEx(WS_EX_CLIENTEDGE,
                                    szChildName,
                                    "",
                                    WS_CHILD | WS_VISIBLE | WS_BORDER,  
                                    tabRect.left + 10,
                                    tabItemRect.bottom + 30,
                                    tabRect.right - 20,
                                    tabRect.bottom - tabItemRect.bottom
                                    - 40,
                                    hwndTabControl,
                                    (HMENU)IDM_DISPLAYCHILD,
                                    hInst,
                                    (LPVOID) NULL);

        
}
