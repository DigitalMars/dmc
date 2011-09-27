// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: cliptext.c
//
//  PURPOSE: To show cut/copy/paste of text with the clipboard
//
//
//  FUNCTIONS:
//    WndProc        - Processes messages for the main window.
//    InitClipText   - Creates the initial text for the main window.
//    UninitClipText - Frees resources allocated by InitClipText.
//    MsgCommand     - Handle the WM_COMMAND messages for the main window.
//    MsgDestroy     - Handles the WM_DESTROY message by calling 
//                     PostQuitMessage().
//    CmdCut         - Place the text in the window into the clipboard and 
//                     delete it.
//    CmdCopy        - Place the text in the window into the clipboard.
//    CmdPaste       - Pastes the text from the clipboard in the main window.
//    CmdClear       - Delete's the text in the main window.
//    CmdExit        - Handles the file exit command by calling destory 
//                     window on the main window.
//    OutOfMemory    - Display and "Out of Memory" dialog box.
//    Copy           - Copies the text from hText to the clipboard.
//    Clear          - Deletes the text contained in hText.
//
//
//  COMMENTS:
//
//
//
//  SPECIAL INSTRUCTIONS: N/A
//


#include <windows.h>            // required for all Windows applications
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include <assert.h>
#include "resource.h"

// Main window message table definition.
MSD rgmsd[] =
{
    {WM_COMMAND,    MsgCommand},
    {WM_DESTROY,    MsgDestroy},
    {WM_INITMENU,   MsgInitMenu},
    {WM_PAINT,      MsgPaint}
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
    {IDM_CUT,   CmdCut},
    {IDM_COPY,  CmdCopy},
    {IDM_CLEAR, CmdClear},
    {IDM_PASTE, CmdPaste}
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpWindow
};

//Module specific "globals"

char szInitialClientAreaText[] =
    "This program demonstrates the use of the Edit menu to copy and "
    "paste text to and from the clipboard.  Try using the Copy command "
    "to move this text to the clipboard, and the Paste command to replace "
    "this text with data from another application.  \r\n\r\n"
    "You might want to try running Notepad and Clipbrd alongside this "
    "application so that you can watch the data exchanges take place.  ";

static HANDLE hText = NULL;

VOID OutOfMemory(VOID);
BOOL Copy(HWND);
VOID Clear(HWND);

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
//  FUNCTION: InitClipText()
//
//  PURPOSE: Creates the initial text for the main window.
//
//  PARAMETERS:
//    NONE.
//
//  RETURN VALUE:
//    TRUE - Success.
//    FALSE - Failure, due to memory allocation.
//
//  COMMENTS:
//
//

BOOL InitClipText(VOID)
{
    LPSTR lpszText;

    hText = GlobalAlloc(GMEM_MOVEABLE, (DWORD)sizeof(szInitialClientAreaText));
    if (hText == NULL)
    {
        OutOfMemory();
        return FALSE;
    }

    if (!(lpszText = GlobalLock(hText)))
    {
        OutOfMemory();
        return FALSE;
    }

    lstrcpy(lpszText, szInitialClientAreaText);
    GlobalUnlock(hText);

    return TRUE;
}

//
//  FUNCTION: UninitClipText()
//
//  PURPOSE: Frees resources allocated by InitClipText.
//
//  PARAMETERS:
//    NONE.
//
//  RETURN VALUE:
//    NONE.
//
//  COMMENTS:
//
//

VOID UninitClipText(VOID)
{
    if (hText != NULL)
        GlobalFree(hText);
}


//
//  FUNCTION: MsgInitMenu(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Enable/Disable the paste menu command.
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number (Unused)
//    wparam    - HMENU - The menu about to be activated
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    0 - The message was handled.
//    1 - The message was not handled - wrong menu.
//
//  COMMENTS:
//
//

LRESULT MsgInitMenu(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    if (GetMenu(hwnd) == (HMENU)wparam)
    {
        UINT mf = MF_GRAYED;

        if (OpenClipboard(hwnd))
        {
            if (IsClipboardFormatAvailable(CF_TEXT) ||
                IsClipboardFormatAvailable(CF_OEMTEXT))
            {
                mf = MF_ENABLED;
            }
        }
        CloseClipboard();

        EnableMenuItem((HMENU)wparam, IDM_PASTE, mf);

        return 0;
    }
    else
    {
        return 1;
    }
}


//
//  FUNCTION: MsgPaint(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Paint the main window's current text, if any.
//
//  PARAMETERS:
//
//    hwnd      - Window handle
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

LRESULT MsgPaint(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    hdc = BeginPaint(hwnd, &ps);
    if (hText != NULL)
    {
        LPSTR lpszText;

        if (!(lpszText = GlobalLock (hText)))
        {
            OutOfMemory();
        }
        else
        {
            RECT rc;
            GetClientRect (hwnd, &rc);
            DrawText(hdc,
                     lpszText,
                     -1,
                     &rc,
                     DT_EXTERNALLEADING | DT_NOPREFIX | DT_WORDBREAK);
            GlobalUnlock(hText);
        }
    }
    EndPaint(hwnd, &ps);

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
//

LRESULT MsgDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    PostQuitMessage(0);
    return 0;
}



//
//  FUNCTION: CmdCut(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Place the text in the window into the clipboard and delete it.
//
//  PARAMETERS:
//    hwnd - The window handle
//    wCommand - IDM_CUT (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    The bulk of the work is done by the Copy and Clear functions.
//    If copy to the clipboard does not succeed, Clear is not called.
//
//

LRESULT CmdCut(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    if (Copy(hwnd))
        Clear(hwnd);

    InvalidateRect(hwnd, NULL, TRUE);
    return 0;
}


//
//  FUNCTION: CmdCopy(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Place the text in the window into the clipboard.
//
//  PARAMETERS:
//    hwnd - The window handle
//    wCommand - IDM_COPY (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT CmdCopy(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    (void) Copy(hwnd);

    return 0;
}


//
//  FUNCTION: CmdClear(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Delete's the text in the main window.
//
//  PARAMETERS:
//    hwnd - The window handle
//    wCommand - IDM_CLEAR (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT CmdClear(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    Clear(hwnd);
    InvalidateRect(hwnd, NULL, TRUE);

    return 0;
}


//
//  FUNCTION: CmdPaste(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Pastes the text from the clipboard in the main window.
//
//  PARAMETERS:
//    hwnd - The window handle
//    wCommand - IDM_PASTE (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT CmdPaste(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    HANDLE hData;
    LPSTR  lpszText,lpszClip;
    BOOL   fOpen;

    fOpen = OpenClipboard(hwnd);

    // If the clipboard can't be opened, this function should
    //  never have been called.
    assert(fOpen);

    // Get the text from the clipboard.

    if (!(hData = GetClipboardData(CF_TEXT)))
    {
        CloseClipboard();
        return 0;
    }

    // Remove the old window text.
    if (hText != NULL)
    {
        GlobalFree(hText);
    }

    hText = GlobalAlloc(GMEM_MOVEABLE, GlobalSize(hData));
    lpszText = GlobalLock(hText);
    lpszClip = GlobalLock(hData);

    if (hText == NULL || lpszText == NULL || lpszClip == NULL)
    {
        OutOfMemory();
        CloseClipboard();
        return 0;
    }

    lstrcpy(lpszText, lpszClip);
    GlobalUnlock(hData);
    CloseClipboard();
    GlobalUnlock(hText);
    EnableMenuItem(GetMenu(hwnd), IDM_CUT, MF_ENABLED);
    EnableMenuItem(GetMenu(hwnd), IDM_COPY, MF_ENABLED);
    EnableMenuItem(GetMenu(hwnd), IDM_CLEAR, MF_ENABLED);

    InvalidateRect(hwnd, NULL, TRUE);

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
//  FUNCTION: OutOfMemory
//
//  PURPOSE: Display and "Out of Memory" dialog box.
//
//  PARAMETERS:
//    NONE.
//
//  RETURN VALUE:
//    NONE.
//
//  COMMENTS:
//
//

VOID OutOfMemory(VOID)
{
    MessageBox(GetFocus(),
               "Out of Memory",
               NULL,
               MB_ICONHAND | MB_SYSTEMMODAL);
}

//
//  FUNCTION: Copy(HWND)
//
//  PURPOSE: Copies the text from hText to the clipboard.
//
//  PARAMETERS:
//    hwnd - The main application window.
//
//  RETURN VALUE:
//    TRUE - Sucessful copy.
//    FALSE - Copy failed.
//
//  COMMENTS:
//
//

BOOL Copy(HWND hwnd)
{
    HANDLE hData;
    LPSTR  lpszText,lpszClip;

    // If hText is NULL, this point should never be reached, since
    //  the menu item is disabled.

    assert(hText != NULL);

    // Allocate memory and copy the string to it

    hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, GlobalSize(hText));
    if (hData == NULL)
    {
        OutOfMemory();
        return FALSE;
    }

    if (!(lpszClip = GlobalLock(hData)))
    {
        GlobalFree(hData);
        OutOfMemory();
        return FALSE;
    }

    if (!(lpszText = GlobalLock(hText)))
    {
        GlobalUnlock(hData);
        GlobalFree(hData);
        OutOfMemory();
        return FALSE;
    }

    lstrcpy(lpszClip, lpszText);
    GlobalUnlock(hData);
    GlobalUnlock(hText);

    // Clear the current contents of the clipboard, and set
    // the data handle to the new string.

    if (OpenClipboard(hwnd))
    {
        EmptyClipboard();
        SetClipboardData(CF_TEXT, hData);
        CloseClipboard();
        return TRUE;
    }

    return FALSE;
}

//
//  FUNCTION: Clear(HWND)
//
//  PURPOSE: Deletes the text contained in hText.
//
//  PARAMETERS:
//    hwnd - The main application window.
//
//  RETURN VALUE:
//    NONE.
//
//  COMMENTS:
//
//

VOID Clear(HWND hwnd)
{
    // If hText is Null, this point should never be reached, since
    //  the menu item is disabled.

    assert(hText != NULL);

    GlobalFree (hText);
    hText = NULL;
    EnableMenuItem(GetMenu(hwnd), IDM_CUT, MF_GRAYED);
    EnableMenuItem(GetMenu(hwnd), IDM_COPY, MF_GRAYED);
    EnableMenuItem(GetMenu(hwnd), IDM_CLEAR, MF_GRAYED);
}

