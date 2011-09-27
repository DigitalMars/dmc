// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: commdlg.c
//
//  PURPOSE: Implement stub and paint functions for the commdlg sample.
//
//
//  FUNCTIONS:
//    WndProc         - Processes messages for the main window.
//    MsgCommand      - Handle the WM_COMMAND messages for the main window.
//    MsgPaint        - raw text inside a rectange using attributes set by
//                      the common font and color dialogs.
//    MsgDestroy      - Handles the WM_DESTROY message by calling 
//                      PostQuitMessage().
//    CmdExit         - Handles the file exit command by calling destory 
//                      window on the main window.
//    Open            - (STUB) Open a file.
//    SaveAs          - (STUB) Save a file.
//    GetPageRange    - (STUB) Return the valid range of pages for the current
//                      document.
//    Print           - (STUB) Print a document given the settings from the 
//                      print dialog.
//    FindReplace     - (STUB) Find and/or replace text in the current
//                      document.
//    SetCurrentFont  - Set the current font based on the fonts dialog.
//    SetCurrentColor - Set the current color based on the color dialog.
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
#include "commdlg.h"           
#endif
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

// Main window message table definition.
MSD rgmsd[] =
{
    {0,             MsgFindReplace},
    {WM_PAINT,      MsgPaint},
    {WM_COMMAND,    MsgCommand},
    {WM_DESTROY,    MsgDestroy}
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
    {IDM_OPEN,      CmdOpen},
    {IDM_SAVEAS,    CmdSaveAs},
    {IDM_PRINT,     CmdPrint},
    {IDM_PRINTSU,   CmdPrintSU},
    {IDM_FIND,      CmdFindReplace},
    {IDM_REPLACE,   CmdFindReplace},
    {IDM_FONTS,     CmdFonts},
    {IDM_COLORS,    CmdColors},
    {IDM_EXIT,      CmdExit},
    {IDM_ABOUT,     CmdAbout}
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpWindow
};


static HFONT hfont = NULL;          // The current font
static DWORD rgbFont = RGB(0,0,0);  // The current font color
static DWORD rgbRect = RGB(0,0,0);  // The current rectangle color

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
//  FUNCTION: MsgPaint(HWND,UINT,WPARAM,LPARAM)
//
//  PURPOSE: Draw text inside a rectange using attributes set by the
//      common font and color dialogs.
//
//  PARAMETERS:
//    hwnd      - Window handle  (Unused)
//    uMessage  - WM_PAINT       (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
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
    HDC hdc;
    RECT rc;
    HFONT hfontT;
    DWORD rgbT;
    HBRUSH hbrush;

    static char * szText =
        "This sample explores the basic functionality of all of the "
        "windows common dialog boxes.  Most of the common dialogs "
        "provide a user interface to request information from "
        "the user.  Then this information is used to perform a task "
        "such as opening a file or setting a font.  This sample "
        "presents the dialog, but does not implement the code to "
        "perform most of the related functions.  Instead, a modal "
        "dialog is displayed when an action would normally occur, "
        "describing the action that would take place in a fully "
        "implemented program.";

    hdc = BeginPaint(hwnd, &ps);

    if (hfont != NULL)
        hfontT = SelectObject(hdc, hfont);
    rgbT = SetTextColor(hdc, rgbFont);
    hbrush = CreateSolidBrush(rgbRect);

    GetClientRect(hwnd, &rc);
    InflateRect(&rc, -5, -5);
    FrameRect(hdc, &rc, hbrush);
    DeleteObject(hbrush);

    InflateRect(&rc, -5, -5);
    DrawText(hdc,
             szText,
             -1,
             &rc,
             DT_EXTERNALLEADING | DT_NOPREFIX | DT_WORDBREAK);

    SetTextColor(hdc, rgbT);
    if (hfont != NULL)
        SelectObject(hdc, hfontT);

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
	if (hfont != NULL)
		DeleteObject(hfont);
    PostQuitMessage(0);
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
//  FUNCTION: Open(char FAR *, HWND)
//
//  PURPOSE: (STUB) Open a file.
//
//  PARAMETERS:
//    szFName - The file to open.
//    hwnd    - The window.
//
//  RETURN VALUE:
//
//  COMMENTS:
//
//

VOID Open(char FAR *szFName, HWND hwnd)
{
    MessageBox(hwnd, szFName, "File Open", MB_ICONINFORMATION | MB_OK);
}

//
//  FUNCTION: SaveAs(char FAR *, HWND)
//
//  PURPOSE: (STUB) Save a file.
//
//  PARAMETERS:
//    szFName - The file to save.
//    hwnd    - The window.
//
//  RETURN VALUE:
//
//  COMMENTS:
//
//

VOID SaveAs(char FAR *szFName, HWND hwnd)
{
    MessageBox(hwnd, szFName, "File Save As",MB_ICONINFORMATION | MB_OK);
}


//
//  FUNCTION: GetPageRange(VOID)
//
//  PURPOSE: (STUB) Return the valid range of pages for the current document.
//
//  PARAMETERS:
//    NONE.
//
//  RETURN VALUE:
//    LOWORD() - The first printable page.
//    HIWORD() - The last printable page.
//
//  COMMENTS:
//
//

DWORD GetPageRange(VOID)
{
    return (DWORD) MAKELONG(1,0xFFFF);
}

//
//  FUNCTION: Print(HDC,BOOL,BOOL,BOOL,BOOL,UINT,UINT,UINT)
//
//  PURPOSE: (STUB) Print a document given the settings from the print dialog.
//
//  PARAMETERS:
//    hdc - The printer display context.
//    fRange - Print a page range.
//    fSelection - Print the current selection.
//    fCollate - Collate the copies.
//    fFile - Print to a file.
//    nFromPage - Print from this page (valid only when fRange==TRUE).
//    nToPage - Print to this page (valid only when fRange==TRUE).
//    nCopies - Number of copied to print.
//
//  RETURN VALUE:
//    NONE.
//
//  COMMENTS:
//
//

VOID Print(HDC     hdc,
           BOOL    fRange, 
           BOOL    fSelection, 
           BOOL    fCollate, 
           BOOL    fFile,
           UINT    nFromPage, 
           UINT    nToPage, 
           UINT    nCopies,
           HGLOBAL hdns)
{
    char rgchTitle[80];
    char rgchText[100];
    char rgchRange[40];
    LPDEVNAMES lpdns;
    char FAR *lpbDevNames;

    lpdns = (LPDEVNAMES)GlobalLock(hdns);
    lpbDevNames = (char FAR *)lpdns;
    wsprintf(rgchTitle,
             "%s;%s;%s",
             lpbDevNames + lpdns->wDriverOffset,
             lpbDevNames + lpdns->wDeviceOffset,
             lpbDevNames + lpdns->wOutputOffset);
    GlobalUnlock(hdns);

    if (!fRange)
    {
        if (fSelection)
            lstrcpy(rgchRange, "Selection");
        else
            lstrcpy(rgchRange, "All");
    }
    else
    {
        wsprintf(rgchRange,"From %d to %d", nFromPage, nToPage);
    }

    wsprintf(rgchText,
             "Print %s %s %s (Copies = %d)",
             rgchRange,
             fCollate ? "Collate" : "",
             fFile ? "File" : "",
             nCopies);
    
    MessageBox(NULL, rgchText, rgchTitle, MB_ICONINFORMATION | MB_OK);
}

//
//  FUNCTION: FindReplace(char*,char*,BOOL,BOOL,BOOL,BOOL,BOOL,BOOL)
//
//  PURPOSE: (STUB) Find and/or replace text in the current document.
//
//  PARAMETERS:
//    szFind - The string to find.
//    szReplace - The string used to replace.
//    fFind - Do a find.
//    fReplace - Do a replace.
//    fRepAll - Do a replace all.
//    fDown - Search down if true, up if false.
//    fMatchCase - Match case on a search.
//    fWholeWord - Match whole words only.
//
//  RETURN VALUE:
//    NONE.
//
//  COMMENTS:
//
//

VOID FindReplace(char *szFind, 
                 char *szReplace,
                 BOOL fFind, 
                 BOOL fReplace, 
                 BOOL fRepAll,
                 BOOL fDown, 
                 BOOL fMatchCase, 
                 BOOL fWholeWord)
{
    char szTitle[80];
    char szText[80];

    wsprintf(szTitle,
             "%s%s%s%s",
             fFind ? "Find " : "",
             fFind ? szFind : "",
             fReplace || fRepAll ? " Replace with " : "",
             fReplace || fRepAll ? szReplace : "");

    wsprintf(szText,
             "%s%s%s%s",
             fRepAll ? "Replace All " : "",
             fDown ?  "Down " : "Up ",
             fMatchCase ?  "Match Case " : "",
             fWholeWord ?  "Whole Word " : "");
 
    MessageBox(NULL,
               szText,
               szTitle,
               MB_APPLMODAL | MB_ICONINFORMATION | MB_OK);
}


//
//  FUNCTION: SetCurrentFont(LPLOGFONT,WORD)
//
//  PURPOSE: Set the current font based on the fonts dialog.
//
//  PARAMETERS:
//    lplf - The font to set
//    rgb - The color to set
//
//  RETURN VALUE:
//    NONE.
//
//  COMMENTS:
//
//

VOID SetCurrentFont(LPLOGFONT lplf, DWORD rgb)
{
    if (hfont != NULL)
        DeleteObject(hfont);
    hfont = CreateFontIndirect(lplf);
    rgbFont = rgb;
}


//
//  FUNCTION: SetCurrentColor(rgb)
//
//  PURPOSE: Set the current color based on the color dialog.
//
//  PARAMETERS:
//    rgb - the color to set
//
//  RETURN VALUE:
//    NONE.
//
//  COMMENTS:
//
//

VOID SetCurrentColor(DWORD rgb)
{
    rgbRect = rgb;
}


