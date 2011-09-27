// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: edit.c
//
//  PURPOSE: Handle basic functionality of the edit control.
//
//  FUNCTIONS:
//    WndProc           - Processes messages for the main window.
//    MsgCommand        - Handle the WM_COMMAND messages for the main window.
//    MsgDestroy        - Handle the WM_DESTROY message by calling 
//                        PostQuitMessage().
//    MsgSetFocus       - Sets the focus to the edit control when main window
//                        gets it.
//    MsgQueryEndSession- Handles the case where user attempts to quit with
//                        unsaved changes.
//    MsgClose          - Close the editor.
//    MsgSize           - Resize the editor window when the main window has
//                        been resized.
//    MsgInitMenu       - Enable/Disable the paste menu command.
//    CmdEdit           - Hand edit control notifications.
//    CmdClip           - Handle clipboard commands.
//    CmdExit           - Handles the file exit command by calling destory 
//                        window on the main window.
//    InitEdit          - Do editor specific initialization.
//    SetEditText       - Set the text of the edit control hwnd.
//    LockEditText      - Return a handle to the text associated with the
//                        edit control.
//    UnlockEditText    - Unlock the handle to the text associated with the
//                        edit control.
//
//  COMMENTS:
//


#include <windows.h>            // required for all Windows applications
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "resource.h"
                 
// Main window message table definition.
static MSD rgmsd[] =
{
    {0,                 MsgFindReplace},
    {WM_COMMAND,        MsgCommand},
    {WM_DESTROY,        MsgDestroy},
    {WM_INITMENU,       MsgInitMenu},
    {WM_SETFOCUS,       MsgSetFocus},
    {WM_SIZE,           MsgSize},
    {WM_QUERYENDSESSION,MsgQueryEndSession},
    {WM_CLOSE,          MsgClose}
};

MSDI msdiMain =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpWindow
};

// Main window command table definition.
static CMD rgcmd[] =
{
    {IDM_FILENEW,    CmdFileNew},
    {IDM_FILEOPEN,   CmdFileOpen},
    {IDM_FILESAVE,   CmdFileSave},
    {IDM_FILESAVEAS, CmdFileSaveAs},
    {IDM_FILEPRINT,  CmdPrint},
    {IDM_FILEPRINTSU,CmdPrintSU},
    {IDC_EDIT,       CmdEdit},
    {IDM_EXIT,       CmdExit},
    {IDM_ABOUT,      CmdAbout},
    {IDM_CUT,        CmdClip},
    {IDM_COPY,       CmdClip},
    {IDM_CLEAR,      CmdClip},
    {IDM_PASTE,      CmdClip},
    {IDM_UNDO,       CmdClip},
    {IDM_FIND,       CmdFindReplace},
    {IDM_REPLACE,    CmdFindReplace},
    {IDM_FINDNEXT,   CmdFindNext},
    {IDM_FINDPREV,   CmdFindNext}
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpWindow
};

//Module specific "globals"

HWND hwndEdit;                               // Edit control handle
HCURSOR hcursHourGlass;

#ifndef WIN16
static HANDLE hszEditBuffer = NULL;
#endif

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

LRESULT CALLBACK WndProc
    (HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
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
//    GET_WM_COMMAND_ID(wparam,lparam)   - Command identifier
//    GET_WM_COMMAND_HWND(wparam,lparam) - Control handle
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
//  FUNCTION: MsgSetFocus(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Sets the focus to the edit control when main window gets it.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//
//

LRESULT MsgSetFocus(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    SetFocus (GetEditWindow());
    return 0;
}


//
//  FUNCTION: MsgQueryEndSession(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handles the case where user attempts to quit with unsaved changes.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    TRUE - Quiting is now safe.
//    FALSE - Don't quit.
//
//  COMMENTS:
//    Let the function QuerySaveFile handle the real work.
//

LRESULT MsgQueryEndSession(HWND hwnd, 
                           UINT uMessage, 
                           WPARAM wparam,
                           LPARAM lparam)
{
    return QuerySaveFile(hwnd);
}


//
//  FUNCTION: MsgClose(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Close the editor.
//
//  PARAMETERS:
//    hwnd     - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam   - Message specific data (unused).
//    lparam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//
//

LRESULT MsgClose(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    if (QuerySaveFile(hwnd))
        DestroyWindow(hwnd);
    return 0;
}


//
//  FUNCTION: MsgSize(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Resize the editor window when the main window has been resized.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//
//

LRESULT MsgSize(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    MoveWindow(GetEditWindow(), 0, 0, LOWORD(lparam), HIWORD(lparam), TRUE);
    return 0;
}


//
//  FUNCTION: MsgInitMenu(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Enable/Disable the paste menu command.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_INITMENU (Unused)
//    wparam    - HMENU - The menu about to be activated
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
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
        UINT ichStart;
        UINT ichEnd;

        // Enable/Disable Paste menu item
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

        // Enable/Disable Undu menu item
        mf = SendMessage(GetEditWindow(), EM_CANUNDO, 0, 0L) ?
            MF_ENABLED : MF_GRAYED;
        EnableMenuItem((HMENU)wparam, IDM_UNDO, mf);

        // Enable/Disable Cut/Copy/Delete menu items
        GETSEL(ichStart,ichEnd);
        mf = (ichEnd != ichStart) ? MF_ENABLED : MF_GRAYED;
        EnableMenuItem((HMENU)wparam, IDM_CUT, mf);
        EnableMenuItem((HMENU)wparam, IDM_COPY, mf);
        EnableMenuItem((HMENU)wparam, IDM_CLEAR, mf);

        // Enable/Disable Save menu item
        mf = SendMessage(GetEditWindow(), EM_GETMODIFY, 0, 0L) ? MF_ENABLED : MF_GRAYED;
        EnableMenuItem((HMENU)wparam, IDM_FILESAVE, mf);

        // Enable/Disable FindNext/FindPrev menu intes
        mf = CanFind() ? MF_ENABLED : MF_GRAYED;
        EnableMenuItem((HMENU)wparam, IDM_FINDNEXT, mf);
        EnableMenuItem((HMENU)wparam, IDM_FINDPREV, mf);

        return 0;
    }
    else
        return 1;
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
//  FUNCTION: CmdEdit(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Hand edit control notifications.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDC_EXIT (unused)
//    wNotify  - EN_*
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//    Handle the edit control's out of space error by putting up an
//    "Out of Memory" warning dialog.
//

LRESULT CmdEdit(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    switch (wNotify)
    {
        case EN_ERRSPACE:
            MessageBox(hwnd, 
                       "Out of memory.", 
                       SZDESCRIPTION, 
                       MB_ICONHAND | MB_OK);
            break;
    }
    return 0;
}


//
//  FUNCTION: CmdClip(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Handle clipboard commands.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_CUT, IDM_COPY, IDM_CLEAR IDM_PASTE, IDM_UNDO
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//    Translate the commands into messages and send them to the edit control.
//

LRESULT CmdClip(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    WORD wMessage;

    switch (wCommand)
    {
        case IDM_CUT:   wMessage = WM_CUT;   break;
        case IDM_COPY:  wMessage = WM_COPY;  break;
        case IDM_PASTE: wMessage = WM_PASTE; break;
        case IDM_CLEAR: wMessage = WM_CLEAR; break;
        case IDM_UNDO:  wMessage = EM_UNDO;  break;
    }

    SendMessage(GetEditWindow(), wMessage, 0, 0L);
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
    if (QuerySaveFile(hwnd))
       DestroyWindow(hwnd);
    return 0;
}


//
//  FUNCTION: InitEdit(HWND)
//
//  PURPOSE: Do editor specific initialization.
//
//  PARAMETERS:
//    hwnd - The main application window.
//
//  RETURN VALUE:
//    TRUE - If initialization was successful.
//    FALSE - If initialization failed.
//
//  COMMENTS:
//    Create the multi-line edit control as a child of the main application
//    window.  Also load the hour-glass cursor for display when saving
//    and printing.

BOOL InitEdit(HWND hwnd)
{
    RECT rc;

    GetClientRect(hwnd, &rc);

    hwndEdit = CreateWindow("Edit",
                            NULL,
                            WS_CHILD   | WS_VISIBLE | ES_MULTILINE   |
                            WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL |
                            ES_AUTOVSCROLL | ES_NOHIDESEL,
                            0, 0,
                            (rc.right-rc.left), (rc.bottom-rc.top),
                            hwnd,
                            (HMENU)IDC_EDIT,           // Child control i.d.
                            hInst,
                            NULL);

    if (!hwndEdit)
    {
        DestroyWindow(hwnd);
        return FALSE;
    }
    else
    {
        hcursHourGlass = LoadCursor(NULL, IDC_WAIT);
        return TRUE;
    }
}


//
//  FUNCTION: SetEditText(HWND, hsz)
//
//  PURPOSE: Set the text of the edit control hwnd.
//
//  PARAMETERS:
//    hwnd - The edit control to set.
//    hsz  - A local handle to the text to set.
//
//  RETURN VALUE:
//    NONE
//
//  COMMENTS:
//    Some implementation of the WIN32 api do not correctly handle
//    EM_SETHANDLE and EM_GETHANDLE.  SetEditText, LockEditText, and
//    UnlockEditText encapsulate the workaround for this limitation.
//

VOID SetEditText(HWND hwnd, HANDLE hsz)
{
#ifdef WIN16
    HANDLE hszOld;

    hszOld = (HANDLE)SendMessage(hwnd, EM_GETHANDLE, 0, 0L);
    LocalFree(hszOld);
    SendMessage(hwnd, EM_SETHANDLE, (WPARAM) hsz, 0L);
#else
    //
    // For Win32s, use WM_SETTEXT.
    //
    // Win32s has 32-bit local memory handles which it cannot pass to
    // Win3.1.  This means that the EM_SETHANDLE and EM_GETHANDLE messages
    // are not supported.  A private 16-bit heap (unknown to the app)
    // provides space for the multiline edit control.
    //

    char *sz = LocalLock(hsz);
    SendMessage(GetEditWindow(), WM_SETTEXT, 0, (LPARAM)(LPSTR)sz);
    LocalUnlock(hsz);
    LocalFree(hsz);
#endif
}


//
//  FUNCTION: LockEditText(HWND)
//
//  PURPOSE: Return a handle to the text associated with the edit control.
//
//  PARAMETERS:
//    hwnd - The edit control whose text is to be locked.
//
//  RETURN VALUE:
//    A local buffer containing the text associated with the editor control.
//
//  COMMENTS:
//
//

char *LockEditText(HWND hwnd)
{
#ifdef WIN16
    HANDLE hsz = (HANDLE)SendMessage(hwnd, EM_GETHANDLE, 0, 0L);
    return LocalLock(hsz);
#else
     // For Win32s, allocate hText and read edit control text into it.
     // Lock hText and fall through to existing code.

    INT cbText;
    char *sz = NULL;

    cbText = SendMessage(hwnd, WM_GETTEXTLENGTH, 0, 0L) + 1;
    hszEditBuffer = LocalAlloc(LMEM_MOVEABLE | LMEM_ZEROINIT, cbText);
    if (hszEditBuffer == NULL) return NULL;
    sz = LocalLock(hszEditBuffer);
    if (sz == NULL)
    {
        LocalFree(hszEditBuffer);
        hszEditBuffer = NULL;
        return NULL;
    }
    SendMessage(hwnd, WM_GETTEXT, cbText, (LPARAM)(LPSTR)sz);
    return sz;
#endif
}


//
//  FUNCTION: UnlockEditText(HWND)
//
//  PURPOSE: Return a handle to the text associated with the edit control.
//
//  PARAMETERS:
//    hwnd - The edit control whose text is to be unlocked.
//
//  RETURN VALUE:
//    NONE
//
//  COMMENTS:
//
//

VOID UnlockEditText(HWND hwnd)
{
#ifdef WIN16
    HANDLE hsz;
    hsz = (HANDLE)SendMessage(hwnd, EM_GETHANDLE, 0, 0L);
    LocalUnlock(hsz);
#else
    SetEditText(hwnd, hszEditBuffer);
    hszEditBuffer = NULL;
#endif
}
