// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: ownrdraw.c
//
//  PURPOSE: Example of a menu with simple owner draw items.
//
//
//  FUNCTIONS:
//    MsgDrawItem - Handles updating of owner draw menu item.
//    CmdColors - Toggle owner draw state of color menu.
//    CdmColorOwnerDr - Handle checking/unchecking of colors.
//
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

static UINT idmCur = IDM_BLACK;

#define MEASUREITEMWIDTH  40
#define MEASUREITEMHEIGHT 40

LRESULT MsgMeasureItem(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    LPMEASUREITEMSTRUCT lpmi = (LPMEASUREITEMSTRUCT)lparam;

    // Use the same width for all items. We could examine the item id
    // and use different widths/heights for each item.

    lpmi->itemWidth  = MEASUREITEMWIDTH;
    lpmi->itemHeight = MEASUREITEMHEIGHT;

    return TRUE;
}

//
//  FUNCTION: MsgDrawItem(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handles updating of owner draw menu item.
//
//  PARAMETERS:
//    hwnd      - Window handle  (Unused)
//    uMessage  - WM_DRAWITEM    (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - LPDRAWITEMSTRUCT
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    Called in response to a WM_DRAWITEM message, i.e. when the
//    colors menu is being modified to an owner-draw menu, or
//    one of the items is selected. It sizes the checkmark bitmap
//    to fit next to a color band and draws the color bands and
//    the checkmark on the popup menu.
//
//

LRESULT MsgDrawItem(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT) lparam;
    HBRUSH  hbr;
    RECT    rc;
    LONG    lSize;
    int     hSize;
    int     vSize;

    // Get the size of the checkmark so we can leave room for it since we
    // want to be able to check the selected color.

    lSize = GetMenuCheckMarkDimensions();
    vSize = HIWORD(lSize);
    hSize = LOWORD(lSize);

    if (lpdis->itemAction == ODA_SELECT ||
        lpdis->itemAction == ODA_DRAWENTIRE)
    {
        CopyRect(&rc, &lpdis->rcItem);
        InflateRect(&rc, -(hSize+2), -2);

        if (lpdis->itemState & ODS_SELECTED)
        {
            // Item has been selected -- hilite with a gray frame
            hbr = GetStockObject(GRAY_BRUSH);
            FrameRect(lpdis->hDC, &rc, hbr);
        }
        else if (lpdis->itemAction == ODA_SELECT)
        {
            // Item has been de-selected -- remove gray frame
            hbr = CreateSolidBrush(GetSysColor(COLOR_MENU));
            FrameRect(lpdis->hDC, &rc, hbr);
            DeleteObject(hbr);
        }
    }

    if (lpdis->itemAction == ODA_DRAWENTIRE)
    {
        // Paint the color item in the color requested.
        hbr = CreateSolidBrush(lpdis->itemData);
        CopyRect(&rc, &lpdis->rcItem);
        InflateRect(&rc, -(hSize+10), -10);
        FillRect(lpdis->hDC, (LPRECT)&rc, hbr);
        DeleteObject(hbr);

        if (lpdis->itemState & ODS_CHECKED)
        {
            HDC     hdcBitmap;
            HBITMAP hbmpSave;
            DWORD   textColorSave;
            DWORD   bkColorSave;

            // Draw the check mark if the item is checked.
            hdcBitmap = CreateCompatibleDC(lpdis->hDC);
            hbmpSave  = SelectObject(hdcBitmap, hbmpDot);
            textColorSave = SetTextColor(lpdis->hDC, 0x00000000L);
            bkColorSave   = SetBkColor(lpdis->hDC, 0x00FFFFFFL);

            BitBlt(lpdis->hDC,
                   lpdis->rcItem.left,
                   lpdis->rcItem.top + (MEASUREITEMHEIGHT - vSize) / 2,
                   hSize,
                   vSize,
                   hdcBitmap,
                   0, 0,
                   SRCAND);

            // Restore colors and bitmap and clean up
            SetTextColor(lpdis->hDC, textColorSave);
            SetBkColor(lpdis->hDC, bkColorSave);
            SelectObject(hdcBitmap, hbmpSave);
            DeleteDC(hdcBitmap);
        }
    }

    return TRUE;
}


//
//  FUNCTION: CmdColors(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Toggle owner draw state of color menu.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - WM_BLACK, WM_RED, WM_BLUE, or WM_GREEN
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL.
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT CmdColors(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    if ((UINT)wCommand != idmCur)
    {
        // Get the Color Menu from the window
        HMENU hmenu = GetSubMenu(GetMenu(hwnd), IDCOLORS_POS);

        MENUITEMINFO mii;

        // Uncheck the currently selected item
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask  = MIIM_STATE;
        mii.fState = MFS_UNCHECKED;
        SetMenuItemInfo(hmenu, idmCur, FALSE, &mii);

        // Check the newly checked item
        mii.fState = MFS_CHECKED;
        SetMenuItemInfo(hmenu, wCommand, FALSE, &mii);

        idmCur = (UINT)wCommand;
    }

    return 0;
}


//
//  FUNCTION: CmdColorOwnerDr(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Handle checking/unchecking of colors.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_COLOROWNERDR    (Unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL.               (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    Toggles the state of the Owner Draw item in the Colors menu. If it
//    is on, the "Black", "Blue", "Red", and "Green" individual menu text
//    items are modified so that they will contain bands of color.
//    Otherwise, the colors are replaced by the text.
//
//

typedef struct _CLRM {
    WORD  wCommand;     // Command number for menu item
    DWORD dwColor;      // RGB color associated with this entry
    UINT  cch;          // Size of the string
    CHAR *szColor;      // Descriptive string for this entry
} CLRM; // Color menu info

CLRM rgclrm[] = {
    {IDM_BLACK, RGB(  0,   0,   0), 6, "&Black"},
    {IDM_BLUE,  RGB(  0,   0, 255), 5, "B&lue"},
    {IDM_RED,   RGB(255,   0,   0), 4, "&Red"},
    {IDM_GREEN, RGB(  0, 255,   0), 6, "&Green"}
};

LRESULT CmdColorOwnerDr(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    MENUITEMINFO mii;
    HMENU hmenu;
    BOOL  fOwnerDraw;
    int   iclrm;

    // Get a handle to the Colors menu. This is at position 1.
    hmenu = GetSubMenu(GetMenu(hwnd), IDCOLORS_POS);

    // Get the current state of the item
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STATE;
    GetMenuItemInfo(hmenu, IDM_COLOROWNERDR, FALSE, &mii);
    fOwnerDraw = !(mii.fState & MFS_CHECKED);

    // Toggle the state of the item.
    mii.fState = (fOwnerDraw ? MFS_CHECKED : MFS_UNCHECKED);
    SetMenuItemInfo(hmenu, IDM_COLOROWNERDR, FALSE, &mii);

    mii.fType = (fOwnerDraw ? MFT_OWNERDRAW : MFT_STRING);
    mii.fMask = MIIM_STATE | MIIM_TYPE;

    for ( iclrm = 0; iclrm < sizeof(rgclrm) / sizeof(CLRM); iclrm++)
    {
        CLRM *pclrm = &rgclrm[iclrm];

        // Get the state of the menu item
        mii.fState = (pclrm->wCommand == idmCur) ? MFS_CHECKED : MFS_UNCHECKED;

        // if it is an owner draw get its data otherwise get its string
        if (fOwnerDraw)
        {
            // In case of Owner draw the MIIM_DATA should also be set since
            // WM_DRAWITEM will be sent with the value of dwItemData and not
            // dwTypeDate.  The dwTypeData is set also because MIIM_TYPE is
            // passed to update the data type(ie MFT_OWNERDRAW vs MFT_STRING)
            mii.fMask |= MIIM_DATA;
            mii.dwItemData = pclrm->dwColor;
            mii.dwTypeData = (LPSTR)pclrm->szColor;
            mii.cch = pclrm->cch;
        }
        else
        {
            // update the string values
            mii.dwTypeData = (LPSTR)pclrm->szColor;
            mii.cch = pclrm->cch;
        }

        // Update the menu item information structure.
        SetMenuItemInfo(hmenu, pclrm->wCommand, FALSE, &mii);
    }

    return TRUE;
}
