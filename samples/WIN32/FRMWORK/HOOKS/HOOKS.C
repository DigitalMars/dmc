// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   hooks.c
//
//  PURPOSE:  Contains all helper functions "global" to the application.
//
//  FUNCTIONS:
//    MsgDestroy          - Handles the WM_DESTROY message by calling
//                          PostQuitMessage().
//    CmdDLLSetMouseHook  - Calls DLLFunction1() from DLLSKEL.DLL
//    CmdExit             - Handles the file exit command by calling destory
//                          window on the main window.
//    CenterWindow -        Center one window over another.
//
//  COMMENTS:
//
//
#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include "dllskel.h"            // prototypes for DLL functions
#include "globals.h"            // prototypes specific to this application
#include "header.h"
#include "toolbar.h"
#include "statbar.h"
#include "listbx.h"
#include "treeview.h"
#include <commctrl.h>
#include "hooks.h"
#include "split.h"
#include "propsht.h"
#include "syshook.h"


//
// Global Variables
//

extern HWND hWndHeaderCtl, hWndLBoxCtl, hWndToolbar, ghWndTreeView,
       hWndStatusbar;
extern BOOL isWindowSplit;
extern int  listTabs[];
extern COLORREF crLBoxCtl;

int     gToolbarHeight;
int     gStatusbarHeight, gHeaderCtlHeight;
HCURSOR hSplitCur, hOldCursor;
BOOL    bDragging;
POINT   gPoint;
int     nSplitXCoord;

HANDLE      hSharedDataMemFile  = NULL;
HANDLE      hMsgDataMemFile     = NULL;
HANDLE      hCom                = NULL;
HANDLE      hLogFile            = NULL;
PGLOBOPTS   pS                  = NULL;
PHOOKSDATA  pM                  = NULL;
char        szComPort[]         = "COM1";
char        szLogFile[]         = "Log.TXT";

// Main window message table definition.
MSD rgmsd[] =
{
    {WM_CREATE,        MsgCreate},    
    {WM_SIZE,          MsgSize},
    {WM_COMMAND,       MsgCommand},
    {WM_COPYDATA,      MsgCopyData},
    {WM_NOTIFY,        MsgNotify},
    {WM_TIMER,         MsgTimer},
    {WM_MENUSELECT,    MsgMenuSelect},
    {WM_PASSHWNDBROWSE,MsgPassHwndBrowse},
    {WM_SETCURSOR,     MsgSetCursor},
    {WM_LBUTTONDOWN,   MsgLButtonDown},
    {WM_LBUTTONUP,     MsgLButtonUp},
    {WM_MOUSEMOVE,     MsgMouseMove},
    {WM_DESTROY,       MsgDestroy},
    {WM_CTLCOLORLISTBOX, MsgCtlColorListBox}
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
    {IDM_EXIT,      CmdExit},
    {IDM_ABOUT,     CmdAbout},
    {IDM_WINDOW,    CmdBrowseWnd},
    {IDM_ALLWNDS,   CmdAllWnds},
    {IDM_CLEARWND,  CmdClearWnd},
    {IDM_CAPON,     CmdCapOn},
    {IDM_CAPOFF,    CmdCapOff},
    {IDM_MESSAGES,  CmdMessages},
    {IDM_WNDINF,    CmdWindowInformation},
    {IDM_PPTIES,    CmdWindowProperties},
    {IDM_FONT,      CmdFont},
    {IDM_WNDMSG,    CmdWndMsgInformation}

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
//    hwnd       - Window handle
//    uMessage   - WM_COMMAND (Unused)
//    wparam     - Contains command identifier
//    lparam     - Contains control handle
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
//  PURPOSE: Calls CreateHeaderCtl() and InitializeHeaderCtl().
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

LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{

    static  char * hdrStrings[] = {"HWND (TEXT)",
                                   "MESSAGE (TEXT)", 
                                   "WPARAM (TEXT)", 
                                   "LPARAM (TEXT)"};
    int     i;
    int     nWidths;
    RECT    rect;

    // Create the split cursor for drag operations
    hSplitCur = LoadCursor(hInst, "splitcur");

    // Create the tool bar for the application
    if(!CreateTBar(hwnd))
        MessageBox(hwnd, "TOOLBAR NOT CREATED", "ERROR", MB_OK);

    // Disable the CAPOFF button.
    SendMessage (hWndToolbar,
                 TB_ENABLEBUTTON,
                 (WPARAM)IDM_CAPOFF,
                 (LPARAM)MAKELONG (FALSE,0));

    GetClientRect (hWndToolbar, &rect);
    gToolbarHeight = rect.bottom - rect.top + 3; // +3 is for asthetics

    // Create the status bar for the application
    if(!CreateSBar(hwnd))
        MessageBox(hwnd, "STATUSBAR NOT CREATED", "ERROR", MB_OK);

    GetClientRect (hWndStatusbar, &rect);
    gStatusbarHeight = rect.bottom - rect.top;

    // Create the Header bar for the application
    if(!CreateHeaderCtl(hwnd))
        MessageBox(hwnd, "HEADER NOT CREATED", "ERROR", MB_OK);   

    // Initialize the header control
    InitializeHeaderCtl(hwnd);
    GetClientRect(hwnd, &rect);
    nWidths = rect.right/4;
    for(i = 0; i < 4; i++)
        InsertHeaderItem (hWndHeaderCtl, i, nWidths, hdrStrings[i]);

    GetClientRect (hWndHeaderCtl, &rect);
    gHeaderCtlHeight = rect.bottom - rect.top;

    // Create the header control.
    if(!CreateLBox(hwnd))
        MessageBox(hwnd, "LISTBOX NOT CREATED", NULL, MB_OK);   


    if (!InitAppHandles())
        return -1;

    InitSharedData (MSG_ALL_MSGS, hwnd, hWndLBoxCtl);
  
    // Create the Treeview  for the application
    if(!CreateTreeView(hwnd))
        MessageBox(hwnd, "TREEVIEW NOT CREATED", NULL, MB_OK);   

    isWindowSplit = FALSE;      


    // Initialize the property Sheets here..
    OptionsPropertySheets(INITIALIZE, hwnd);


    return 0;
}


//
//  FUNCTION: MsgSize(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Calls SendMessage() to send the size message.
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

LRESULT MsgSize(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    RECT rect;
    
    GetClientRect(hwnd, &rect);

    // pass the WM_SIZE message on to the Toolbar/Statusbar
    SendMessage (hWndToolbar, uMessage, wparam, lparam);
    SendMessage (hWndStatusbar, uMessage, wparam, lparam);

    if (IsWindowVisible (hWndHeaderCtl))
        InitializeHeaderCtl(hwnd);

    if(isWindowSplit)
    {
        MoveWindow(hWndHeaderCtl, 
                   (rect.right/2) + 2,
                   gToolbarHeight,
                   rect.right,
                   gHeaderCtlHeight,
                   TRUE);
        MoveWindow(hWndLBoxCtl, 
                   (rect.right/2) + 2,
                   gHeaderCtlHeight +1 + gToolbarHeight,
                   rect.right/2,
                   rect.bottom - 
                   (gHeaderCtlHeight +1) - 
                   gToolbarHeight -
                   gStatusbarHeight,
                   TRUE);
        MoveWindow(ghWndTreeView,
                   0,
                   gToolbarHeight,
                   (rect.right/2) - 2,
                   rect.bottom - gToolbarHeight - gStatusbarHeight,
                   TRUE);                                       

    }
    else
    {
        if(IsWindowVisible(hWndLBoxCtl))
        {
            MoveWindow(hWndLBoxCtl, 
                       0, 
                       gHeaderCtlHeight + 1 + gToolbarHeight, 
                       rect.right, 
                       rect.bottom - 
                       (gHeaderCtlHeight +1) - 
                       gToolbarHeight -
                       gStatusbarHeight, 
                       TRUE);

        }
        if(IsWindowVisible(hWndHeaderCtl))
            MoveWindow(hWndHeaderCtl, 
                       0, 
                       gToolbarHeight, 
                       rect.right, 
                       gHeaderCtlHeight, 
                       TRUE);

        if(IsWindowVisible(ghWndTreeView))
            MoveWindow(ghWndTreeView, 
                       0, 
                       gToolbarHeight, 
                       rect.right, 
                       rect.bottom - gToolbarHeight - gStatusbarHeight, 
                       TRUE);
    }

    // Re-position the panes in the status bar
    InitializeStatusBar(hwnd);

    return 0;
}


//
//  FUNCTION: MsgDestroy(HWND, UINT, WPARAM, LPARAM)

//
//  PURPOSE: Calls PostQuitMessage().
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

LRESULT MsgDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    RemoveHooks ();
    CloseAppHandles ();
    PostQuitMessage(0);
    return 0;
}


//
//  FUNCTION: MsgCtlColorListBox (HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes the WM_CTLCOLORLISTBOX message to change the text
//           color in hWndLBoxCtl.
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - HDC of hWndLBoxCtl 
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    Right now this function is a hack.  It automatically sets the list box
//    background to white.  It needs to get the system metrics and use that
//    color to update the background of the list box.  crLBoxCtl is global 
//    from misc.c and contains a COLORREF returned from the font dialog box.
//

LRESULT MsgCtlColorListBox(HWND hwnd, 
                           UINT uMessage, 
                           WPARAM wparam, 
                           LPARAM lparam)
{
    SetTextColor((HDC)wparam, crLBoxCtl);
    UpdateWindow(hWndLBoxCtl);
    return (LRESULT) GetStockObject( WHITE_BRUSH );
}


//
//  FUNCTION: MsgPassHwndBrowse (HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Sets the pS->hWndMonitored to appropriate hWnd
//           passed in wParam.
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - hWnd selected to browse; 
//                passed from Exports.C
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//

LRESULT MsgPassHwndBrowse (HWND hwnd, 
                           UINT uMessage, 
                           WPARAM wparam, 
                           LPARAM lparam)
{

    pS->hWndMonitored = (HWND)wparam;
    InitTreeViewItems (ghWndTreeView, pS->hWndMonitored);
    return 0;
}

//
//  FUNCTION: MsgNotify(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handles WM_NOTIFY messages for the header and
//           TreeView controls.
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

LRESULT MsgNotify(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    LRESULT lResult;

// LPNMHDR  lpnmhdr = (LPNMHDR)lparam;
// char szBuf [50];
//wsprintf (szBuf, "Notify code = %d %d", wparam, lpnmhdr->code);
//SetWindowText (FindWindow (SZAPPNAME, NULL), szBuf);

    switch(wparam)
    {
        case IDM_TREEVIEW:
            lResult = MsgNotifyTreeView(hwnd, uMessage, wparam, lparam);
            break;
        
        case IDM_HEADERCTL:
            lResult = MsgHeaderNotify(hwnd, uMessage, wparam, lparam);
            break;
        
        case IDM_TOOLBAR:
            lResult = MsgNotifyToolbar(hwnd, uMessage, wparam, lparam);
            break;

        default:

// Bug in Toolbar code does not set the WM_NOTIFY's wParam to the 
// ctl ID. Always set to 0.
// To workaround, calling it here as default case.
// When fixed, don't forget to change back to calling DefWindowProc().

            lResult = MsgNotifyToolbar(hwnd, uMessage, wparam, lparam);
//            lResult = DefWindowProc(hwnd, uMessage, wparam, lparam);
            break;
    }


    return lResult;
}

//
//  FUNCTION: MsgSetCursor(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Calls LoadCursor if the window is split.
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

LRESULT MsgSetCursor(HWND hwnd, 
                     UINT uMessage, 
                     WPARAM wparam, 
                     LPARAM lparam)
{
    if(isWindowSplit && 
       LOWORD(lparam) == HTCLIENT &&
       hwnd == (HWND)wparam)

    {
        // Window is split, so load the cursor..
        hOldCursor = SetCursor(hSplitCur);
        return 0;
    }    
    DefWindowProc(hwnd, uMessage, wparam, lparam);
}



//
//  FUNCTION: MsgLButtonDown(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Calls SetCapture if the window is split.
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

LRESULT MsgLButtonDown(HWND hwnd, 
                       UINT uMessage, 
                       WPARAM wparam, 
                       LPARAM lparam)
{
    if(isWindowSplit)
    {
        RECT    rect;
        HDC     hdc;

        bDragging = TRUE;
        GetWindowRect(hwnd, &rect);
        ClipCursor(&rect);
        SetCapture(hwnd);
        GetClientRect(hwnd, &rect);

        gPoint.x = LOWORD (lparam);
        gPoint.y = HIWORD (lparam);

        hdc = GetDC(hwnd);
        PatBlt(hdc, 
               LOWORD(lparam) - 2, 
               rect.top + gToolbarHeight, 
               4, 
               rect.bottom - gStatusbarHeight - gToolbarHeight, 
               DSTINVERT);
        ReleaseDC(hwnd, hdc);
        return 0;
    }    
    return 0;
}



//
//  FUNCTION: MsgLButtonUp(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Calls ReleaseCapture if the window is split.
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

LRESULT MsgLButtonUp(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    if(isWindowSplit)
    {
        RECT rect;
        HDC  hdc;

        bDragging = FALSE;
        ClipCursor(NULL);
        ReleaseCapture();

        // Call PatBlt() again to erase the previous highlight
        // in LBUTTONDOWN/MOUSEMOVE

        GetClientRect(hwnd, &rect);
        hdc = GetDC(hwnd);

        PatBlt(hdc, 
               gPoint.x - 2, 
               rect.top + gToolbarHeight, 
               4, 
               rect.bottom - gStatusbarHeight - gToolbarHeight, 
               DSTINVERT);

        ReleaseDC(hwnd, hdc);

        MoveWindow(ghWndTreeView,
                   0,
                   gToolbarHeight,
                   (gPoint.x) - 2,
                   rect.bottom - gToolbarHeight - gStatusbarHeight,
                   TRUE);                                       

        MoveWindow(hWndHeaderCtl, 
                   gPoint.x + 2,
                   gToolbarHeight,
                   rect.right,
                   gHeaderCtlHeight,
                   TRUE);
        MoveWindow(hWndLBoxCtl, 
                   gPoint.x + 2,
                   gHeaderCtlHeight +1 + gToolbarHeight,
                   rect.right - (gPoint.x + 2),
                   rect.bottom - 
                   (gHeaderCtlHeight +1) - 
                   gToolbarHeight -
                   gStatusbarHeight,
                   TRUE);



        return 0;
    }    
    return 0;
}

//
//  FUNCTION: MsgCopyData(HWND, UINT, WPARAM, LPARAM)
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

LRESULT MsgCopyData(HWND hwnd, 
                     UINT uMessage, 
                     WPARAM wParam, 
                     LPARAM lParam)
{
    HandleHookMessage ((PCWPSTRUCT)((PCOPYDATASTRUCT)lParam)->lpData);
    return TRUE;
}
//
//  FUNCTION: MsgMouseMove(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Calls Patblt if the window is split.
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

LRESULT MsgMouseMove(HWND hwnd, 
                     UINT uMessage, 
                     WPARAM wparam, 
                     LPARAM lparam)
{
    if(bDragging)
    {
        RECT rect;
        HDC  hdc;

        // Call PatBlt() again to erase the previous highlight

        GetClientRect(hwnd, &rect);
        hdc = GetDC(hwnd);

        PatBlt(hdc, 
               gPoint.x - 2, 
               rect.top + gToolbarHeight, 
               4, 
               rect.bottom - gStatusbarHeight - gToolbarHeight, 
               DSTINVERT);

        // Save the new mouse position in our global gPoint variable
        // so we can un-highlight later.

        gPoint.x = LOWORD (lparam);
        gPoint.y = HIWORD (lparam);

        // Now we want to highlight the new mouse position

        PatBlt(hdc, 
               gPoint.x - 2, 
               rect.top + gToolbarHeight, 
               4, 
               rect.bottom - gStatusbarHeight - gToolbarHeight, 
               DSTINVERT);

        ReleaseDC(hwnd, hdc);
        return 0;
    }    
    return 0;
}




//
//  FUNCTION: CmdWindowProperties(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_PPTIES            (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL                (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//

LRESULT CmdWindowProperties(HWND hwnd, 
                            WORD wCommand, 
                            WORD wNotify, 
                            HWND hwndCtrl)
{
    // Call the options property sheet api to show the ppt sheets

    OptionsPropertySheets(SHOW, hwnd);
    return 0;

}

//
//  FUNCTION: CmdCapOn(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_PPTIES            (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL                (unused)
//
//  RETURN VALUE:    Always returns 0 - command handled.
//
//  COMMENTS:
//

LRESULT CmdCapOn(HWND hwnd, 
                 WORD wCommand, 
                 WORD wNotify, 
                 HWND hwndCtrl)
{

    if (pS->fOutputToCom || pS->fOutputToLogFile)
        OpenOutputHandles ();

    if (!pS->hWndMonitored)
    {
        char szBuf [80];

        lstrcpy (szBuf, "No window to browse on. \r\n");
        lstrcat (szBuf, "Select a window to Browse first.");
        MessageBox (hwnd, szBuf, NULL, MB_OK | MB_ICONEXCLAMATION);

        SendMessage (hWndToolbar, 
                 TB_CHECKBUTTON, 
                 (WPARAM)IDM_CAPON, 
                 (LPARAM)MAKELONG (FALSE, 0));
    }
    else 
    {
        // Check first if hWndMonitored still exists
        // or if we're browsing all windows (0x0000FFFF)
        // before installing the hook for it.
        // May have been destroyed since selected.
        if ((!IsWindow (pS->hWndMonitored)) &&
            (pS->hWndMonitored != (HWND)0x0000FFFF))
        {
            char szBuf [80];
            lstrcpy (szBuf, "Selected window no longer exists. \r\n");
            lstrcat (szBuf, "Select a window to monitor.");
            MessageBox (hwnd, szBuf, NULL, MB_OK | MB_ICONEXCLAMATION);
            SendMessage (hWndToolbar, 
                         TB_CHECKBUTTON, 
                         (WPARAM)IDM_CAPON, 
                         (LPARAM)MAKELONG (FALSE, 0));
        }
        else
        {
            if (!InstallHooks (pS->hWndMonitored))
            {
                MessageBox (hwnd, 
                            "HOOK NOT INSTALLED - InstallHooks() failed",
                            NULL,
                            MB_OK | MB_ICONEXCLAMATION);
            }
            else
            {
                // Set the CAPON button to unpressed state first.
                // Then, disable it.
                SendMessage (hWndToolbar, 
                             TB_CHECKBUTTON, 
                             (WPARAM)IDM_CAPON, 
                             (LPARAM)MAKELONG (FALSE, 0));
                SendMessage (hWndToolbar,
                             TB_ENABLEBUTTON,
                             (WPARAM)IDM_CAPON,
                             (LPARAM)MAKELONG (FALSE,0));
    
                // Enable the CAPOFF button.
                SendMessage (hWndToolbar,
                             TB_ENABLEBUTTON,
                             (WPARAM)IDM_CAPOFF,
                             (LPARAM)MAKELONG (TRUE,0));
            }
        }
    }
    return 0;
}

//
//  FUNCTION: CmdCapOff(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_PPTIES            (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL                (unused)
//
//  RETURN VALUE:    Always returns 0 - command handled.
//
//  COMMENTS:
//

LRESULT CmdCapOff(HWND hwnd, 
                  WORD wCommand, 
                  WORD wNotify, 
                  HWND hwndCtrl)
{
    // Called with 1 if window receives a WM_DESTROY.
//    if (hwndCtrl == (HWND)1)
//       pS->hWndMonitored = (HWND)0;

    RemoveHooks ();
    CloseOutputHandles ();

    // Set the CAPOFF button to unpressed state.
    // And then, disable it.
    SendMessage (hWndToolbar, 
                 TB_CHECKBUTTON, 
                 (WPARAM)IDM_CAPOFF, 
                 (LPARAM)MAKELONG (FALSE, 0));
    SendMessage (hWndToolbar,
                 TB_ENABLEBUTTON,
                 (WPARAM)IDM_CAPOFF,
                 (LPARAM)MAKELONG (FALSE,0));
    // Enable the CAPON button.
    SendMessage (hWndToolbar,
                 TB_ENABLEBUTTON,
                 (WPARAM)IDM_CAPON,
                 (LPARAM)MAKELONG (TRUE,0));
    return 0;
}

//
//  FUNCTION: CmdClearWnd (HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_PPTIES            (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL                (unused)
//
//  RETURN VALUE:    Always returns 0 - command handled.
//
//  COMMENTS:
//

LRESULT CmdClearWnd(HWND hwnd, 
                    WORD wCommand, 
                    WORD wNotify, 
                    HWND hwndCtrl)
{
    ClearMsgList ();
    return 0;
}


//
//  FUNCTION: CmdFont(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Choose a new font.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_FONT            (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL                (unused)
//
//  RETURN VALUE:    Always returns 0 - command handled.
//
//  COMMENTS:
//

LRESULT CmdFont(HWND hwnd, 
                WORD wCommand, 
                WORD wNotify, 
                HWND hwndCtrl)
{
    ChooseNewFont(hwnd);
    return 0;
}

//
//  FUNCTION: CmdDLLSetMouseHook(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd      - The window          (unused)
//    wCommand  - IDM_DLLFUNC1        (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL                (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//

LRESULT CmdDLLSetMouseHook(HWND hwnd, 
                           WORD wCommand,
                           WORD wNotify, 
                           HWND hwndCtrl)

{
    return 0;
}

//
//  FUNCTION: CmdExit(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_EXIT            (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL                (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//

LRESULT CmdExit(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    DestroyWindow(hwnd);
    return 0;
}

BOOL WINAPI InitAppHandles ()
{
    hSharedDataMemFile = CreateFileMapping ((HANDLE)0xFFFFFFFF, 
                                            NULL, 
                                            PAGE_READWRITE,
                                            0, 
                                            sizeof (GLOBOPTS), 
                                            HOOK_SHARED_DATA_MEM_FILE);
    if (!hSharedDataMemFile)
        goto OnError;
    pS = (PGLOBOPTS)MapViewOfFile (hSharedDataMemFile, 
                                   FILE_MAP_WRITE, 
                                   0, 
                                   0, 
                                   0);
    if (!pS)
        goto OnError;
    
    hMsgDataMemFile = CreateFileMapping ((HANDLE)0xFFFFFFFF, 
                                         NULL, 
                                         PAGE_READWRITE,
                                         0, 
                                         sizeof (HOOKSDATA), 
                                         HOOK_MSG_DATA_MEM_FILE);
    if (!hMsgDataMemFile)
        return FALSE;
    pM = (PHOOKSDATA)MapViewOfFile (hMsgDataMemFile, 
                                    FILE_MAP_WRITE, 
                                    0, 
                                    0, 
                                    0);
    if (!pM)
        goto OnError;

    return TRUE;
OnError:
    CloseAppHandles ();
    return FALSE;
}

void WINAPI CloseAppHandles ()
{
    CloseOutputHandles ();
    UnmapViewOfFile ((LPVOID)pS);
    UnmapViewOfFile ((LPVOID)pS);
    CloseHandle (hMsgDataMemFile);
    CloseHandle (hSharedDataMemFile);
}

void WINAPI InitSharedData (DWORD options, HWND hWnd, HWND hWndLBoxCtl)
{
    memset (pS, 0, sizeof (GLOBOPTS));
    pS->dwMsgFilterOptions      = options;
    pS->nIndex                  = 0;
    pS->fMonitorChild           = FALSE;
    pS->fOutputToWindow         = TRUE;
    pS->fOutputToCom            = FALSE;
    pS->fOutputToLogFile        = FALSE;
    pS->fHwndInText             = TRUE;
    pS->fMessageInText          = TRUE;
    pS->fWParamInText           = TRUE;
    pS->fLParamInText           = TRUE;
    pS->hMsgWindow              = hWnd;
    pS->hListBox                = hWndLBoxCtl;

    lstrcpy (pS->szOutputFile, szLogFile);

}

BOOL WINAPI OpenOutputHandles ()
{
    DCB dcb;
    if (!hCom)
    {    
        hCom = CreateFile (szComPort,
                           GENERIC_WRITE | GENERIC_READ,
                           0,
                           NULL,
                           OPEN_EXISTING,
                           0,
                           NULL); 
        if (INVALID_HANDLE_VALUE == hCom || NULL == hCom)
        {
            hCom = NULL;
            MessageBox (NULL, "Error Opening and Locking COM1:", NULL, MB_OK);
            pS->fOutputToCom = FALSE;
            return FALSE;
        }
        GetCommState (hCom, &dcb);
        dcb.BaudRate    = 9600;
        dcb.ByteSize    = 8;
        dcb.Parity      = NOPARITY;
        dcb.StopBits    = ONESTOPBIT;
        SetCommState (hCom, &dcb);
    }
    if (!hLogFile)
    {    
        hLogFile = CreateFile (pS->szOutputFile,
                               GENERIC_WRITE | GENERIC_READ,
                               FILE_SHARE_WRITE | FILE_SHARE_READ,
                               NULL,
                               CREATE_ALWAYS,
                               0,
                               NULL);
        if (INVALID_HANDLE_VALUE == hLogFile || NULL == hLogFile)
        {
            hLogFile = NULL;
            MessageBox (NULL, "Error Opening Log File", NULL, MB_OK);
            pS->fOutputToLogFile = FALSE;
            return FALSE;
        }
    }
    return TRUE;
}

void WINAPI CloseOutputHandles ()
{
    CloseHandle (hCom);
    CloseHandle (hLogFile);
    hCom        = NULL;
    hLogFile    = NULL;
}

void WINAPI HandleHookMessage (PCWPSTRUCT pCwp)
{
    DWORD  dwIndex, dwBytes;
    char string[256];
    char buffer[PARAM_STRING_SIZE];
    if (pS->nIndex == MAX_MSG_BUFFER)
    {
        pS->nIndex = 0;
        pS->fWrapped = TRUE;
    }
    pM->Msgs[pS->nIndex].hWnd    = pCwp->hwnd;
    pM->Msgs[pS->nIndex].message = pCwp->message;
    pM->Msgs[pS->nIndex].wParam  = pCwp->wParam;
    pM->Msgs[pS->nIndex].lParam  = pCwp->lParam;
    DecodeMessageParams (pM->Msgs[pS->nIndex].hWnd, 
                         pM->Msgs[pS->nIndex].message,
                         pM->Msgs[pS->nIndex].wParam,
                         pM->Msgs[pS->nIndex].lParam,
                         pM->Msgs[pS->nIndex].szhWnd,
                         pM->Msgs[pS->nIndex].szMessage,
                         pM->Msgs[pS->nIndex].szwParam,
                         pM->Msgs[pS->nIndex].szlParam);
    if (pS->fHwndInText)
    {
        lstrcpy (string, pM->Msgs[pS->nIndex].szhWnd);
        lstrcat (string, "\t");
    }
    else
        wsprintf (string, "HWND: %#08x\t", pM->Msgs[pS->nIndex].hWnd);
            
    if (pS->fMessageInText)
        lstrcat (string, pM->Msgs[pS->nIndex].szMessage);
    else 
    {
        wsprintf (buffer, "Message Number: %#08x\t", pM->Msgs[pS->nIndex].message);
        lstrcat (string, buffer);
    }
            
    if (pS->fWParamInText)
        lstrcat (string, pM->Msgs[pS->nIndex].szwParam);
    else
    {
        wsprintf (buffer, "WPARAM: %#08x\t", pM->Msgs[pS->nIndex].wParam);
        lstrcat (string, buffer);
    }
    
    if (pS->fLParamInText)
        lstrcat (string, pM->Msgs[pS->nIndex].szlParam);
    else
    {
        wsprintf (buffer, "LPARAM: %#08x", pM->Msgs[pS->nIndex].lParam);
        lstrcat (string, buffer);
    }
            
    if (pS->fOutputToWindow)
    {
        SendMessage (pS->hListBox, WM_SETREDRAW, 0, 0);
        if (MAX_MSG_BUFFER == pS->nListBoxSize)
        {   
            int i;
            for (i=0; i<5; i++)
                SendMessage (pS->hListBox, LB_DELETESTRING, (WPARAM)i, 0);
            pS->nListBoxSize -= i;
        }
        else
        {
            pS->nListBoxSize++;
        }
        dwIndex = SendMessage (pS->hListBox, LB_ADDSTRING, 0, (LPARAM)string);
        SendMessage (pS->hListBox, LB_SETCURSEL, (WPARAM)dwIndex,  0);
        SendMessage (pS->hListBox, WM_SETREDRAW, 1, 0);
    }
    if (pS->fOutputToCom || pS->fOutputToLogFile)
    {
        dwIndex = wsprintf (string,
                            "%-16s %-24s %-32s %-32s\r\n",
                            pM->Msgs[pS->nIndex].szhWnd,
                            pM->Msgs[pS->nIndex].szMessage,
                            pM->Msgs[pS->nIndex].szwParam,
                            pM->Msgs[pS->nIndex].szlParam);
        if (pS->fOutputToCom)
            WriteFile (hCom, string, dwIndex, &dwBytes, NULL);
        if (pS->fOutputToLogFile)
            WriteFile (hLogFile, string, dwIndex, &dwBytes, NULL);
    }
    pS->nIndex++;
}

