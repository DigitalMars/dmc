// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   initree.c
//
//  PURPOSE:  Implement the windows procedure for the main application
//    windows.  Also implement the generic message and command dispatchers.
//
//  FUNCTIONS:
//    WndProc         - Processes messages for the main window.
//    MsgCommand      - Handle the WM_COMMAND messages for the main window.
//    MsgDestroy      - Handles the WM_DESTROY message by calling 
//                      PostQuitMessage().
//    CmdExit         - Handles the file exit command by calling destory 
//                      window on the main window.
//    CreateImageList - Create an Image List.
//    CreateTreeView  - Create a TreeViewControl.
//
//  COMMENTS:
//

#include <windows.h>                // required for all Windows applications.
#include <windowsx.h>
#include <commctrl.h>               // TreeView declarations live here.
#include "globals.h"                // prototypes specific to this application.
#include "resource.h"

HWND        hwndTreeView;           // hwnd of TreeView Control.
HIMAGELIST  hImageList;             // Handle to ImageList.

int         iINIFileImage;          // Index of "INI" Image in ImageList.
int         iSectionKeyImage;       // Index of "Folder" Image in ImageList.
int         iSelectedEntryImage;    // Index of Selected Entry Image 
                                    // in ImageList.
int         iNonSelectedEntryImage; // Index of Non-Selected Entry Image 
                                    // in ImageList.

// FUNCTION PROTOTYPES

HWND CreateTreeView(HWND hwndParent, int iID, DWORD dwStyle);

HIMAGELIST CreateImageList(PINT piINIFileImage,
    PINT piSectionKeyImage,
    PINT piSelectedEntryImage,
    PINT piNonSelectedEntryImage);

extern void INI_FillTreeView(HWND hwndTreeView, 
    int iINIFileImage,
    int iSectionKeyImage,
    int iSelectedEntryImage,
    int iNonSelectedEntryImage);

// Main window message table definition.
MSD rgmsd[] =
{
    {WM_COMMAND,    MsgCommand},
    {WM_DESTROY,    MsgDestroy},
    {WM_CREATE,     MsgCreate},
    {WM_SIZE,       MsgSize}
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
    {IDM_EXIT,  CmdExit},
    {IDM_ABOUT, CmdAbout},
    {IDM_FILL,  CmdFill}
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpWindow
};


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

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
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
//  PURPOSE: Creates TreeView Control, and posts "Fill" message.
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - not used
//    lparam    - not used
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    // Create the TreeView control, with lines and buttons, even at
    // the root level.

    hwndTreeView = CreateTreeView(hwnd, 
                                  0, 
                                  TVS_HASLINES | TVS_HASBUTTONS | 
                                  TVS_LINESATROOT | TVS_HASLINES |
                                  WS_VISIBLE | WS_CHILD | WS_BORDER);

    // Post a message to fill the control, or nix app if
    // the creation failed.

    if (hwndTreeView)
        PostMessage(hwndTreeView, WM_COMMAND, IDM_FILL, 0L );
    else
        PostMessage(hwndTreeView, WM_COMMAND, IDM_EXIT, 0L );

    return 0;
}

//
//  FUNCTION: MsgDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Calls PostQuitMessage(), cleans up image list.
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
//

LRESULT MsgDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    // Clean up the image list.

    if (hImageList)
        ImageList_Destroy(hImageList);

    PostQuitMessage(0);
    return 0;
}

//
//  FUNCTION: MsgSize(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Resizes TreeView Control
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Not used.
//    lparam    - Not used.
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgSize(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    RECT rc;    // Client area rect

    // Figure the new client area

    GetClientRect(hwnd, &rc);

    // Resize the TreeView control

    if (hwndTreeView)
        SetWindowPos(hwndTreeView,
                     NULL,
                     0, 0,
                     rc.right, rc.bottom,
                     SWP_NOZORDER);

    return 0;
}

//
//  FUNCTION: CmdFill
//
//  PURPOSE: Fill TreeView Control will the INI files.
//
//  PARAMETERS:
//    hwnd     - window handle
//    wCommand - IDM_FILL
//    wNotify  - Not Used.
//    hwndCtrl - Not Used.
//
//  RETURN VALUE:
//    Always 0.
//
//  COMMENTS:
//    This calls the INI_FillTreeView function in INI.C to fill the
//    TreeView control. The message to invoke this function is posted
//    at startup in MsgCreate.

LRESULT CmdFill(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    INI_FillTreeView(hwndTreeView, 
                     iINIFileImage,
                     iSectionKeyImage,
                     iSelectedEntryImage,
                     iNonSelectedEntryImage);
                     
    return 0;
}

//
//  FUNCTION: CmdExit(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_EXIT (unused)
//    wNotify  - Notification number (unused)
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
//  FUNCTION: CreateImageList(PINT piINIFileImage,
//                            PINT piSectionKeyImage,
//                            PINT piSelectedEntryImage,
//                            PINT piNonSelectedEntryImage)
//
//  PURPOSE: Create an ImageList for use with the TreeView.
//
//  PARAMETERS:
//    piINIFileImage           - Ptr to Index of "INI" Image in ImageList.
//    piSectionKeyImage        - Ptr to Index of "Folder" Image in ImageList.
//    piSelectedEntryImage     - Ptr to Index of Selected Entry Image 
//                               in ImageList.
//    piNonSelectedEntryImage  - Ptr to Index of Non-Selected Entry Image 
//                               in ImageList.
//  RETURN VALUE:
//    On Success: Handle to the Image List
//    On Failure: NULL
//
//  COMMENTS:
//    The bitmaps for the image list are stored in the resources
//    of this application. For this example, each bitmap is a
//    separate bitmap. The background (mask) color of the bitmaps
//    is white - RGB (255,255,255), which is what the image list
//    will automagically use as a mask as explained below.
//
//    Even though the bitmaps in the resources are 32x32 pixels,
//    we are only using the upper left 20x12 portion, as this is the
//    minimum bounding rectangle to fit the little teensy pictures.
//
//    To change the size of the images, edit both the bitmaps,
//    and change the parameters to ImageList_Create.
//

HIMAGELIST CreateImageList(PINT piINIFileImage,
                           PINT piSectionKeyImage,
                           PINT piSelectedEntryImage,
                           PINT piNonSelectedEntryImage)
{
    HIMAGELIST hIL;       // Handle to ImageList
    HBITMAP    hBitmap;   // Handle to a bitmap

    // Create the Image List

    hIL = ImageList_Create(20,     // X size of one image
                           12,     // Y size of one image
                           TRUE,   // Masked images
                           4,      // Four images in this list
                           0);     // No "growing" bitmaps in this list.

    if (!hIL) return NULL;

    // Add Each bitmap to the ImageList.
    //
    // ImageList_AddMasked will add the bitmap, and treat every
    // pixel that is (in this example) white as a "transparent" pixel,
    // since we specified TRUE for fMask in the above call to
    // ImageList_Create.

    hBitmap = LoadBitmap(hInst, "INI");
    if (hBitmap)
    {
        *piINIFileImage = ImageList_AddMasked(hIL, hBitmap, RGB(255,255,255));
        DeleteObject (hBitmap);
    }

    hBitmap = LoadBitmap(hInst, "KEY");
    if (hBitmap)
    {
        *piSectionKeyImage = ImageList_AddMasked(hIL, hBitmap, RGB(255,255,255));
        DeleteObject (hBitmap);
    }

    hBitmap = LoadBitmap(hInst, "ENTRYS");
    if (hBitmap)
    {
        *piSelectedEntryImage = ImageList_AddMasked(hIL, hBitmap, RGB(255,255,255));
        DeleteObject (hBitmap);
    }

    hBitmap = LoadBitmap(hInst, "ENTRYN");
    if (hBitmap)
    {
        *piNonSelectedEntryImage = ImageList_AddMasked(hIL, hBitmap, RGB(255,255,255));
        DeleteObject (hBitmap);
    }

    // Paranoid checking to make sure everything was added successfully.

    if (ImageList_GetImageCount(hIL) < 3)
    {
        ImageList_Destroy(hIL);
        hIL = NULL;
        *piINIFileImage          = 0;
        *piSectionKeyImage       = 0;
        *piSelectedEntryImage    = 0;
        *piNonSelectedEntryImage = 0;
    }

    // Success! 

    return hIL;
}

//
//  FUNCTION: CreateTreeView(HWND hwndParent, int iID, DWORD dwStyle)
//
//  PURPOSE: Create a TreeView control
//
//  PARAMETERS:
// 
//    hwndParent - Parent of the TreeView.
//    iID        - ID of the TreeView.
//    dwStyle    - Style bits of the TreeView.
//
//  RETURN VALUE:
//    On Success: Handle to the TreeView
//    On Failure: NULL
//
//  COMMENTS:
//
//    The call to InitCommonControls is required once per application
//    lifetime. It is OK to move this to init.c, but for the
//    cut-and-pastibility of this sample, the call is placed here.
//

HWND CreateTreeView(HWND hwndParent, int iID, DWORD dwStyle)
{
    RECT rc;      // Client rect of parent
    HWND hwnd;    // Handle to TreeView

    // This registers the TreeView class.

    InitCommonControls();

    // Get the client area of the parent.

    GetClientRect(hwndParent, &rc);

    // Create the TreeView control.

    hwnd = CreateWindowEx(0,                        // Ex style
                          WC_TREEVIEW,              // Class
                          "",                       // Dummy Text
                          dwStyle,                  // Style
                          0, 0,                     // Use all of
                          rc.right, rc.bottom,      // client area.
                          hwndParent,               // Parent
                          (HMENU)iID,               // ID
                          hInst,                    // Instance
                          NULL);                    // No extra

    // Create the ImageList for this TreeView

    hImageList = CreateImageList(&iINIFileImage,
                                 &iSectionKeyImage,
                                 &iSelectedEntryImage,
                                 &iNonSelectedEntryImage);

    // Attach ImageList to TreeView

    if (hImageList && hwnd)
        TreeView_SetImageList(hwnd, hImageList, 0);

    // Return the window handle

    return hwnd;
}
