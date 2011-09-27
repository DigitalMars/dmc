// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   about.c
//
//  PURPOSE:   Displays the "About" dialog box
//
//  FUNCTIONS:
//    CmdAbout        - Displays the "About" dialog box
//    About           - Processes messages for "About" dialog box.
//    MsgAboutInit    - To initialize the about box with version info
//                      from resources.
//    MsgAboutCommand - Process WM_COMMAND message sent to the about box.
//    CmdAboutDone    - Free the about box and related data.
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>

#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

#ifdef WIN16
// Dialog box procedures must be exported in 16-bit applications for Windows.
LRESULT CALLBACK __export About(HWND, UINT, WPARAM, LPARAM);
#else
LRESULT CALLBACK About(HWND, UINT, WPARAM, LPARAM);
#endif

LRESULT MsgAboutInit(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgAboutCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT CmdAboutDone(HWND, WORD, WORD, HWND);

// About dialog message table definition.
MSD rgmsdAbout[] =
{
    {WM_COMMAND,    MsgAboutCommand},
    {WM_INITDIALOG, MsgAboutInit}
};

MSDI msdiAbout =
{
    sizeof(rgmsdAbout) / sizeof(MSD),
    rgmsdAbout,
    edwpNone
};

// About dialog command table definition.
CMD rgcmdAbout[] =
{
    {IDOK,     CmdAboutDone},
    {IDCANCEL, CmdAboutDone}
};

CMDI cmdiAbout =
{
    sizeof(rgcmdAbout) / sizeof(CMD),
    rgcmdAbout,
    edwpNone
};

// Module specific "globals"  Used when a variable needs to be
// accessed in more than on handler function.

HFONT hFontCopyright;

//
//  FUNCTION: CmdAbout(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays the "About" dialog box
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_ABOUT (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    To process the IDM_ABOUT message, call DialogBox() to display the
//    about dialog box.

LRESULT CmdAbout(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    DialogBox(hInst, "AboutBox", hwnd, (DLGPROC)About);
    return 0;
}


//
//  FUNCTION: About(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "About" dialog box.
//
//  PARAMETERS:
//    hdlg - window handle of the dialog box
//    wMessage - type of message
//    wparam - message-specific information
//    lparam - message-specific information
//
//  RETURN VALUE:
//    TRUE - message handled
//    FALSE - message not handled
//
//  COMMENTS:
//
//     Display version information from the version section of the
//     application resource.
//
//     Wait for user to click on "Ok" button, then close the dialog box.
//

LRESULT CALLBACK About(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiAbout, hdlg, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgAboutInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the about box with version info from resources.
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
//    Uses the version apis to retrieve version information for
//    each of the static text boxes in the about box.
//

LRESULT MsgAboutInit(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    #define POINTSIZE 8

    char  szFullPath[256];
    DWORD dwVerHnd;
    DWORD dwVerInfoSize;
    HDC   hDC;
    int   iLogPixelsY, iPointSize;

    // Center the dialog over the application window
    CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));

    // Set the copyright font to something smaller than default
    hDC = GetDC(hdlg);
    iLogPixelsY = GetDeviceCaps(hDC, LOGPIXELSY);
    ReleaseDC(hdlg, hDC);
    iPointSize = MulDiv(iLogPixelsY, POINTSIZE, 72);
    iPointSize *= -1;

    hFontCopyright = CreateFont(iPointSize,
                                0, 0, 0,
                                FW_BOLD,
                                0, 0, 0, 0,
                                0, 0, 0, 0,
                                "Arial");

    SendDlgItemMessage(hdlg, 
                       IDD_VERLAST, 
                       WM_SETFONT, 
                       (WPARAM)hFontCopyright,
                       0L);

    // Get version information from the application
    GetModuleFileName (hInst, szFullPath, sizeof(szFullPath));
    dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);
    if (dwVerInfoSize)
    {
        // If we were able to get the information, process it:
        HANDLE  hMem;
        LPVOID  lpvMem;
        char    szGetName[256];
        int     cchRoot;
        int     i;

        hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
        lpvMem  = GlobalLock(hMem);
        GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpvMem);
        lstrcpy(szGetName, "\\StringFileInfo\\040904E4\\");
        cchRoot = lstrlen(szGetName);

        // Walk through the dialog items that we want to replace:
        for (i = IDD_VERFIRST; i <= IDD_VERLAST; i++)
        {
            BOOL  fRet;
            UINT  cchVer = 0;
            LPSTR lszVer = NULL;
            char  szResult[256];

            GetDlgItemText(hdlg, i, szResult, sizeof(szResult));
            lstrcpy(&szGetName[cchRoot], szResult);
            fRet =  VerQueryValue(lpvMem, szGetName, &lszVer, &cchVer);

            if (fRet && cchVer && lszVer)
            {
                // Replace dialog item text with version info
                lstrcpy(szResult, lszVer);
                SetDlgItemText(hdlg, i, szResult);
            }
        }
        GlobalUnlock(hMem);
        GlobalFree(hMem);
    }
    return TRUE;
}

//
//  FUNCTION: MsgAboutCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND message sent to the about box.
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
//    Uses this DipsCommand function defined in wndproc.c combined
//    with the cmdiAbout structure defined in this file to handle
//    the command messages for the about dialog box.
//

LRESULT MsgAboutCommand(HWND   hwnd, 
                        UINT   uMessage, 
                        WPARAM wparam, 
                        LPARAM lparam)
{
    return DispCommand(&cmdiAbout, hwnd, wparam, lparam);
}

//
//  FUNCTION: CmdAboutDone(HWND, WORD, HWND)
//
//  PURPOSE: Free the about box and related data.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled (unused).
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Calls EndDialog to finish the dialog session.
//

LRESULT CmdAboutDone(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    if (hFontCopyright)
       DeleteObject(hFontCopyright);

    EndDialog(hdlg, TRUE);          // Exit the dialog
    return TRUE;
}

