// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: header.c
//
//  PURPOSE: Handles general routines for the header control
//
//  FUNCTIONS:
//    CreateTBar    - Creates the header control for the sample.
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <commctrl.h>           // prototypes and defs for common controls
#include "globals.h"            // prototypes specific to this application
#include "header.h"


// Global Variables for the status and toolbar controls.

HWND        hWndHeaderCtl;
extern int  listTabs[];
extern HWND hWndLBoxCtl;
extern int  gToolbarHeight, gStatusbarHeight, gHeaderCtlHeight;


//
//  FUNCTION: CreateHeaderCtl(HWND)
//
//  PURPOSE:  Calls CreateWindow() to create the header
//
//
//  PARAMETERS:
//
//  hwnd - Window handle : Used for the hWndParent parameter of the control.
//
//  RETURN VALUE:
//
//  If header control was created successfully Return TRUE,
//  else returns FALSE.
//
//  COMMENTS:
//
//

BOOL CreateHeaderCtl(HWND hwndParent)
{
    // Ensure the common control DLL is loaded and then cre
    // header control.
    InitCommonControls();
                                                                              
    hWndHeaderCtl = CreateWindow(WC_HEADER, 
                                (LPSTR)NULL, 
                                WS_CHILD | WS_BORDER | 
                                HDS_HORZ | // HDS_DIVIDERTRACK |
                                HDS_BUTTONS,
                                0, 
                                0, 
                                0, 
                                0,
                                hwndParent,
                                (HMENU)IDM_HEADERCTL, 
                                hInst,
                               (LPVOID)NULL);

                
    if (!hWndHeaderCtl)
        return FALSE;
    else
        return TRUE;
}



//
//  FUNCTION: InitializeHeaderCtl(HWND)
//
//  PURPOSE:  Initialize header control.
//
//
//  PARAMETERS:
//
//  hwnd - Window handle : Used for the hWndParent parameter of the controls.
//
//
//  RETURN VALUE:  NONE
//
//
//  COMMENTS:
//
//

BOOL InitializeHeaderCtl(HWND hwndParent)
{
    RECT rcParent;
    HD_LAYOUT hdl;
    WINDOWPOS wp;


// Retrieve the bounding rectangle of the parent window's
// client area and then request size and position values
// from the header control.

    GetClientRect(hwndParent, &rcParent);

    hdl.prc = &rcParent;
    hdl.pwpos = &wp;
    if (!SendMessage(hWndHeaderCtl, 
                     HDM_LAYOUT, 
                     0,
                    (LPARAM) &hdl))

         return FALSE;

        // Set the size, position, and visibility of the header control.

        SetWindowPos(hWndHeaderCtl, 
                     wp.hwndInsertAfter, 
                     wp.x, 
                     gToolbarHeight,
                     wp.cx, 
                     37,
                     wp.flags | SWP_SHOWWINDOW);

    return TRUE;
}



//
//  FUNCTION: InsertHeaderItem(HWND)
//
//  PURPOSE:  Insert  header control items.
//
//
//  PARAMETERS:
//
//  hwnd - Window handle : Used for the hWndParent parameter of the controls.
//
//
//  RETURN VALUE:  NONE
//
//
//  COMMENTS:
//
//

BOOL InsertHeaderItem(HWND hwndHeader, 
                      int insertAfter, 
                      int nWidth, 
                      LPSTR lpsz)
{
    HD_ITEM hdi;
    int index;

    hdi.mask = HDI_TEXT | HDI_FORMAT | HDI_WIDTH | HDI_LPARAM;
    hdi.pszText = lpsz;
    hdi.cxy = nWidth;
    hdi.cchTextMax = lstrlen(hdi.pszText);
    hdi.fmt = HDF_CENTER | HDF_STRING;
    hdi.lParam = (LPARAM)(LPSTR)"TEXT";

    index = SendMessage(hwndHeader, 
                        HDM_INSERTITEM, 
                       (WPARAM)insertAfter,
                       (LPARAM) &hdi);

    return TRUE;
}




//
//  FUNCTION: MsgNotify(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//


LRESULT MsgHeaderNotify(HWND hwnd, 
                        UINT uMessage, 
                        WPARAM wparam, 
                        LPARAM lparam)
{
    HD_NOTIFY FAR *phdr;

    phdr = (HD_NOTIFY FAR *)lparam;
    switch(phdr->hdr.code)
    {
        case HDN_ITEMCLICK:
            return 0;
        break;

        case HDN_TRACK:
            // Call the tracking function with the appropriate values
            TrackHeader(phdr->iButton, phdr->iItem, phdr->pitem->cxy);
            return FALSE;
        break;

        default:
        return FALSE;

    }
 
}


//
//  FUNCTION: TrackHeader()
//
//  PURPOSE:  Track  header control items and adjust the list box
//
//
//  PARAMETERS:
//
//
//
//  RETURN VALUE:  NONE
//
//
//  COMMENTS:
//
//
void TrackHeader(int iButton, int iItem, int nWidth)

{
    HDC         hdc;
    HFONT       hFont, hOldObj;
    TEXTMETRIC  tm;
    char *allchar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    SIZE        size;
    LONG        xAveChar;
    HD_ITEM     testItem;


    // HD_ITEM's mask has to be set to request for information
    // from the header control.
    testItem.mask = HDI_WIDTH;

    // Get Dialog base units and dialog units to convert
    // pixel positions from header control to dlg base units.

    hFont = (HFONT)SendMessage(hWndLBoxCtl, 
                               WM_GETFONT, 
                               0,
                               0);
    hdc = GetDC(hWndLBoxCtl);
    hOldObj = SelectObject(hdc, hFont);
    
    GetTextMetrics(hdc, &tm);
    GetTextExtentPoint(hdc, 
                      (LPSTR)allchar, 
                      52, 
                      &size);
    xAveChar = size.cx / 26;
    xAveChar = (xAveChar+1) / 2; //to avoid rounding errors.
    SelectObject(hdc, hOldObj);
    ReleaseDC(hWndLBoxCtl, hdc);

    if(iButton == 0)
    {
        // Left Mouse Button...
        switch(iItem)
        {

            case 0:

                Header_GetItem(hWndHeaderCtl, 1, &testItem); 
                listTabs[0] = MulDiv(4, nWidth, xAveChar);
                listTabs[1] = listTabs[0] + 
                              MulDiv(4, testItem.cxy, xAveChar);
                Header_GetItem(hWndHeaderCtl, 2, &testItem); 
                listTabs[2] = listTabs[1] + 
                              MulDiv(4, testItem.cxy, xAveChar);

  
//                wsprintf((LPSTR)szBuffer, 
//                         "Width of First TABSTOP is: %d::",
//                         listTabs[0]);
//                         
//                SetWindowText(GetParent(hWndHeaderCtl),
//                             (LPSTR)szBuffer);

                SendMessage(hWndLBoxCtl, 
                            LB_SETTABSTOPS,
                            TOTAL_TABS, 
                            (LPARAM)(LPINT)listTabs);
                InvalidateRect(hWndLBoxCtl, NULL, TRUE);
            break;


            case 1:
//                wsprintf((LPSTR)szBuffer, 
//                         "ID of Item: %d:: Width of Item is: %d::",
//                         iItem,
//                         nWidth);
//                SetWindowText(GetParent(hWndHeaderCtl),
//                             (LPSTR)szBuffer);

                listTabs[1] = MulDiv(4, nWidth, xAveChar) +
                              listTabs[0];
//              SetWindowText(GetParent(hWndHeaderCtl),
//                           (LPSTR)szBuffer);

                Header_GetItem(hWndHeaderCtl, 2, &testItem); 
                listTabs[2] = listTabs[1] + 
                              MulDiv(4, testItem.cxy, xAveChar);


                SendMessage(hWndLBoxCtl, 
                            LB_SETTABSTOPS,
                            TOTAL_TABS, 
                            (LPARAM)(LPINT)listTabs);
                InvalidateRect(hWndLBoxCtl, NULL, TRUE);


            break;

            case 2:
//                wsprintf((LPSTR)szBuffer, 
//                         "ID of Item: %d:: Width of Item is: %d::",
//                         iItem,
//                         nWidth);
//                SetWindowText(GetParent(hWndHeaderCtl),
//                             (LPSTR)szBuffer);

                listTabs[2] = MulDiv(4, nWidth, xAveChar) +
                              listTabs[1];
//                SetWindowText(GetParent(hWndHeaderCtl),
//                             (LPSTR)szBuffer);

                SendMessage(hWndLBoxCtl, 
                            LB_SETTABSTOPS,
                            TOTAL_TABS, 
                            (LPARAM)(LPINT)listTabs);
                InvalidateRect(hWndLBoxCtl, NULL, TRUE);
                  
            break;

        } //end switch

    } // end of if...

    else 
        return;  // if iButton != 0

 }




