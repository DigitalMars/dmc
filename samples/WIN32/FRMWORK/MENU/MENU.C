// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: menu.c
//
//  PURPOSE: Handles messages and commands for simple menus
//
//
//  FUNCTIONS:
//    WndProc       - Processes messages for the main window.
//    MsgCommand    - Handle the WM_COMMAND messages for the main window.
//    MsgCreate     - Insert the states menu into the windows menu bar
//    MsgPaint      - Places instructions for popup menu in main window.
//    MsgDestroy    - Frees resources allocated by MsgCreate
//    CmdStates     - Handle the States menu
//    ShrinkBitmap  - Shrinks a 64x64 bitmap to checkmark size
//    CreateStateMI - Sets up the MENUITEMINFO stucture and inserts the item
//
//  COMMENTS:
//
//
//
//   SPECIAL INSTRUCTIONS: N/A
//

#include <windows.h>            // required for all Windows applications
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

// Main window message table definition.
MSD rgmsd[] =
{
    {WM_COMMAND,        MsgCommand},
    {WM_DESTROY,        MsgDestroy},
    {WM_CREATE,         MsgCreate},
    {WM_MEASUREITEM,    MsgMeasureItem},
    {WM_DRAWITEM,       MsgDrawItem},
    {WM_RBUTTONUP,      MsgButtonUp},
    {WM_PAINT,          MsgPaint}
};

MSDI msdiMain =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpWindow
};


// Main window command table definition.
CMD rgcmd[] =
{
    {IDM_STATE1,        CmdStates},
    {IDM_STATE2,        CmdStates},
    {IDM_STATE3,        CmdStates},
    {IDM_STATE4,        CmdStates},
    {IDM_BLACK,         CmdColors},
    {IDM_RED,           CmdColors},
    {IDM_BLUE,          CmdColors},
    {IDM_GREEN,         CmdColors},
    {IDM_COLOROWNERDR,  CmdColorOwnerDr},
    {IDM_FONT,          CmdFont},
    {IDM_SIZE,          CmdSize},
    {IDM_STYLE,         CmdStyle},
    {IDM_EXIT,          CmdExit}
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpWindow
};


HBITMAP ShrinkBitmap(HWND, HBITMAP);
BOOL CreateStateMI(HWND, int, int, int, LPSTR, int);

HBITMAP  hbmpCheckOn;
HBITMAP  hbmpCheckOff;
HBITMAP  hbmpDot;

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispMessage() function with the main window's message dispatch
//    information (msdiMain) and the message specific information.
//

LRESULT CALLBACK WndProc(HWND   hwnd, 
                         UINT   uMessage, 
                         WPARAM wparam, 
                         LPARAM lparam)
{
    return DispMessage(&msdiMain, hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_COMMAND messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - WM_COMMAND (Unused)
//    GET_WM_COMMAND_ID(wparam, lparam)   - Command identifier
//    GET_WM_COMMAND_HWND(wparam, lparam) - Control handle
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispCommand() function with the main window's command dispatch
//    information (cmdiMain) and the command specific information.
//

LRESULT MsgCommand(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&cmdiMain, hwnd, wparam, lparam);
}


//
//  FUNCTION: MsgCreate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Insert the states menu into the windows menu bar.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_CREATE     (Unused)
//    wparam    - Extra data    (Unused)
//    lparam    - Extra data    (Unused)
//
//  RETURN VALUE:
//     0 - Message handled - success
//    -1 - Could not handle message - failure
//
//  COMMENTS: Creates a new (empty) menu and appends to it the "State"
//    menu items. It sets up the user-defined checkmarks for the
//    menu. It then inserts this menu into the main menu bar.
//

LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    MENUITEMINFO mii;
    char    miStr[20];
    HMENU   hmenu;
    HMENU   hmenuWnd;

    // Create a new menu into the menubar on the fly.
    hmenu = CreatePopupMenu();
    if (hmenu == NULL)
    {
        // Return failure if the menu could not be created
        return -1;
    }

    // Set up the bitmaps for menu check states
    hbmpCheckOn  = ShrinkBitmap(hwnd, LoadBitmap(hInst, "checkon"));
    hbmpCheckOff = ShrinkBitmap(hwnd, LoadBitmap(hInst, "checkoff"));

    // Load the Menu item information structure and insert the menu item in 
    // to the sub menu via CreateStateMI() helper function below.
    if (!(CreateStateMI(hmenu, IDM_STATE1, 0, MFS_CHECKED, "South Dakota", 12)
        &&
          CreateStateMI(hmenu, IDM_STATE2, 1, MFS_UNCHECKED, "Washington", 10)
        &&
          CreateStateMI(hmenu, IDM_STATE3, 2, MFS_UNCHECKED, "California", 10)
        &&
          CreateStateMI(hmenu, IDM_STATE4, 3, MFS_UNCHECKED, "Oregon", 6)))
    {
        // If any of the insersions fail, destroy the menu and return failure
       DestroyMenu(hmenu);
       return -1;
    }


    // Set up the MENUITEMINFO for the "State" Menu
    lstrcpy(miStr,"States");
    mii.fMask         = MIIM_TYPE |           // The Mask determines which of
                        MIIM_SUBMENU |        // he items will be set, Type
                        MIIM_ID;              // (typedata,cch),ID & submenu
    mii.fType         = MFT_STRING;           // MIIM_TYPE-> Type is set
    mii.wID           = IDM_STATE;            // MIIM_ID-> ID is set
    mii.hSubMenu      = hmenu;                // MIIM_SUBMENU-> hSubMenu is set
    mii.dwTypeData    = (LPSTR)&miStr;        // MIIM_TYPE is MFT_STRING this 
    mii.cch           = sizeof(miStr);        //    string and its size 
    mii.cbSize        = sizeof(MENUITEMINFO); // Size of menu item structure

    // Now insert the menu into the main menu bar.
    hmenuWnd = GetMenu (hwnd);
    InsertMenuItem(hmenuWnd, 2, TRUE, &mii);

    // Load bitmap for custom menu
    hbmpDot = LoadBitmap(hInst, "dot");

    return 0;
}

//
//  FUNCTION: MsgPaint(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Places instructions for popup menu in main window.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_PAINT      (Unused)
//    wparam    - Extra data    (Unused)
//    lparam    - Extra data    (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgPaint(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    RECT rc;

    // Center the text in the client area
    GetClientRect (hwnd, (LPRECT)&rc);
    DrawText(hdc,
             "Right click in this window for a popup menu",
             -1,
             (LPRECT)&rc,
             DT_CENTER | DT_WORDBREAK);

    EndPaint(hwnd, &ps);

    return 0;
}

//
//  FUNCTION: MsgDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Frees resources allocated by MsgCreate
//
//  PARAMETERS:
//    hwnd      - Window handle (Unused)
//    uMessage  - WM_DESTROY    (Unused)
//    wparam    - Extra data    (Unused)
//    lparam    - Extra data    (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    DeleteObject(hbmpCheckOn);
    DeleteObject(hbmpCheckOff);
    DeleteObject(hbmpDot);

    PostQuitMessage(0);

    return 0;
}

//
//  FUNCTION: CmdStates(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Handle the States menu
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_STATEx
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL.
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT CmdStates(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    static UINT idmCur = IDM_STATE1;

    if ((UINT) wCommand != idmCur)
    {
        // Get the State Menu from the window
        HMENU hmenu = GetSubMenu(GetMenu(hwnd), IDSTATES_POS);

        MENUITEMINFO mii;

        // Uncheck the currently selected item
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask = MIIM_STATE;
        mii.fState = MFS_UNCHECKED;
        SetMenuItemInfo(hmenu,idmCur,FALSE,&mii);

        // Check the newly checked item
        mii.fState = MFS_CHECKED;
        SetMenuItemInfo(hmenu,wCommand,FALSE,&mii);

        idmCur = (UINT)wCommand;
    }

    return 0;
}


//
//  FUNCTION: CmdExit(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd - The window.
//    wCommand - IDM_EXIT (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdExit(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    DestroyWindow(hwnd);
    return 0;
}


//
//  FUNCTION: ShrinkBitmap(HWND, HBITMAP)
//
//  PURPOSE: Shrinks a 64x64 bitmap to checkmark size
//
//  PARAMETERS:
//    hwnd - The window used to get the DC
//    hbmp - The 64x64 bitmap to shrink
//
//  RETURN VALUE:
//
//    A handle to 16x16 bitmap
//
//  COMMENTS:
//    This function shrinks a 64x64 bitmap into a bitmap useable
//    for the user-defined checkmark for menu items. This can be
//    easily generalized to shrink bitmaps of any size.
//
//    This function deletes the original bitmap.
//

HBITMAP ShrinkBitmap(HWND hwnd, HBITMAP hbmp)
{
    HDC     hdc;
    HDC     hdcNew;
    HDC     hdcOld;
    LONG    checkMarkSize;
    HBITMAP hbmpRet;
    HBITMAP hbmpOld;
    HBITMAP hbmpNew;

    hdc = GetDC(hwnd);

    // Create DCs for the source (old) and target (new) bitmaps
    hdcNew = CreateCompatibleDC(hdc);
    hdcOld = CreateCompatibleDC(hdc);

    // Determine the dimensions of the default menu checkmark and
    // create a target bitmap of the same dimensions
    checkMarkSize = GetMenuCheckMarkDimensions();
    hbmpRet = CreateCompatibleBitmap(hdc,
                                     LOWORD(checkMarkSize),
                                     HIWORD(checkMarkSize));

    // Select the source bitmap and the target bitmap into their
    // respective DCs.
    hbmpOld = SelectObject(hdcNew, hbmpRet);
    hbmpNew = SelectObject(hdcOld, hbmp);

    // Shrink the source bitmap into the target DC
    if (!StretchBlt(hdcNew,
                    0, 0,
                    LOWORD(checkMarkSize),
                    HIWORD(checkMarkSize),
                    hdcOld,
                    0, 0,
                    64, 64,
                    SRCCOPY))
        hbmpRet = NULL;

    // De-select the bitmaps and clean up ..
    SelectObject(hdcNew, hbmpOld);
    SelectObject(hdcOld, hbmpNew);
    DeleteDC(hdcNew);
    DeleteDC(hdcOld);
    DeleteObject(hbmp);
    ReleaseDC(hwnd, hdc);

    // Return a handle to the target bitmap
    return hbmpRet;
}

//
//  FUNCTION: CreateStateMI(HWND, int, int, int, LPSTR, int)
//
//  PURPOSE: Create a Submenu item for the State Menu by setting up the 
//    MENYITEMINFO structure and calling InsertMenuItem().  
//
//  PARAMETERS:
//    hMenu - Hanle of the submenu.
//    miID  - Menu Item ID.
//    Pos   - Position of the menu item.
//    State - Menu Item State Checked(MFS_CHECKED) or Unchecked(MFS_UNCHECKED)
//    lpmiStr - Menu item description string.
//    SizeOfStr - Size of the menu item description string.
//
//  RETURN VALUE:
//    BOOL - TRUE if InsertMenuItem was successful otherwise FALSE
//
//  COMMENTS:
//

BOOL CreateStateMI(HWND hmenu,
                   int miID,
                   int pos,
                   int state,
                   LPSTR lpmiStr,
                   int SizeOfStr)
{
    MENUITEMINFO mii;

    // Set up the MENUITEMINFO;
    mii.fMask         = MIIM_TYPE | MIIM_STATE |
                        MIIM_CHECKMARKS | MIIM_ID;
    mii.fType         = MFT_STRING;
    mii.fState        = state;
    mii.wID           = miID;
    mii.hSubMenu      = NULL;
    mii.hbmpChecked   = hbmpCheckOn;
    mii.hbmpUnchecked = hbmpCheckOff;
    mii.dwTypeData    = lpmiStr;
    mii.cch           = SizeOfStr;
    mii.cbSize        = sizeof(MENUITEMINFO);

    return InsertMenuItem(hmenu,pos,TRUE,&mii);
}
