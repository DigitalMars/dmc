// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   browse.c
//
//  PURPOSE:  Contains functions to browse windows on the system. 
//
//  FUNCTIONS:
//    CmdBrowseWnd     - Handles the WM_COMMAND, IDM_WINDOW message. 
//    CmdAllWnds       - Handles the WM_COMMAND, IDM_ALLWNDS message. 
//    DisplayBrowseDlg - Displays the Browse Dialog
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <commctrl.h>           // prototypes and defs for common controls
#include "globals.h"            // prototypes specific to this application
#include "hooks.h"
#include "syshook.h"


typedef struct tagHSAVE
{
    HWND      hDlg;
    HWND      hWndBrowse;
    HCURSOR   ghCursor;
} HSAVE, *PHSAVE;

HANDLE  hMemMapped, hLibrary;
PHSAVE  phSave;
extern PGLOBOPTS pS;


extern HWND hWndHeaderCtl, hWndLBoxCtl, hWndToolbar, ghWndTreeView,
       hWndStatusbar;

//
// Global Variables for this module
//


//
// Function prototypes specific to this module.
//

void DisplayBrowseDlg(HWND);
LRESULT CALLBACK Browse(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgBrowseInit(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgBrowseCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgBrowseUpdateText(HWND, UINT, WPARAM, LPARAM);
LRESULT CmdBrowseDone(HWND, WORD, WORD, HWND);



// Browse dialog message table definition.
MSD rgmsdBrowse[] =
{
    {WM_COMMAND,    MsgBrowseCommand},
    {WM_INITDIALOG, MsgBrowseInit},
    {WM_UPDATETEXT, MsgBrowseUpdateText}
};

MSDI msdiBrowse =
{
    sizeof(rgmsdBrowse) / sizeof(MSD),
    rgmsdBrowse,
    edwpNone
};

// Browse dialog command table definition.
CMD rgcmdBrowse[] =
{
    {IDCANCEL, CmdBrowseDone}
};

CMDI cmdiBrowse =
{
    sizeof(rgcmdBrowse) / sizeof(CMD),
    rgcmdBrowse,
    edwpNone
};


//
//  FUNCTION: LimitStringLength (LPSTR, DWORD, DWORD)
//
//  PURPOSE: Limits the length of string to dwLimit characters.
//
//  PARAMETERS:
//    lpszString     - String buffer
//    dwStrLen       - Current string length (number of chars copied)
//    dwLimit        - Limit string length this much
//
//  RETURN VALUE:
//    void
//

void LimitStringLength (LPSTR lpszString, DWORD dwStrLen, DWORD dwLimit)
{       
    // Put "..." at the end of the string if
    // a. strlen is over the limit; or
    // b. strlen is EXACTLY the limit AND
    //    the last char is NOT the terminating NULL character.
    if ((dwStrLen > dwLimit) ||
        ((dwStrLen == dwLimit) && (lpszString[dwLimit] != '\0')))
        lstrcpy (lpszString+(dwLimit-3), "...\0");
}

//
//  FUNCTION: CmdBrowseWnd(HWND, WORD, WORD, HWND)
//
//  PURPOSE:  Browse Windows as the user moves the mouse.
//
//  PARAMETERS:
//    hwnd      - The window          (unused)
//    wCommand  - 
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL                (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//

LRESULT CmdBrowseWnd(HWND hwnd, 
                     WORD wCommand,
                     WORD wNotify, 
                     HWND hwndCtrl)
{
    DWORD dwState;

    dwState = SendMessage (hWndToolbar, TB_GETSTATE, IDM_CAPOFF, 0);


    // Check if the CAPOFF Button is enabled.
    // This means that CAPture is on.
    // We want to make sure we set CAPTURE off before we browse
    // on another window.
    if (TBSTATE_ENABLED & dwState)
        SendMessage (hwnd, WM_COMMAND, IDM_CAPOFF, 0);

    DisplayBrowseDlg(hwnd) ;
    return 0;

}


//
//  FUNCTION: CmdAllWnds(HWND, WORD, WORD, HWND)
//
//  PURPOSE:  Browse All Windows was chosen.
//
//  PARAMETERS:
//    hwnd      - The window          (unused)
//    wCommand  - 
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL                (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//

LRESULT CmdAllWnds(HWND hwnd, 
                     WORD wCommand,
                     WORD wNotify, 
                     HWND hwndCtrl)
{
    DWORD dwState;

    dwState = SendMessage (hWndToolbar, TB_GETSTATE, IDM_CAPOFF, 0);


    // Check if the CAPOFF Button is enabled.
    // This means that CAPture is on.
    // We want to make sure we set CAPTURE off before we browse
    // on another window.
    if (TBSTATE_ENABLED & dwState)
        SendMessage (hwnd, WM_COMMAND, IDM_CAPOFF, 0);

    pS->hWndMonitored = (HWND) 0x0000FFFF;
    return 0;

}



//
//  FUNCTION: DisplayBrowseDlg(HWND)
//
//  PURPOSE:  Brings up the browse dialog
//
//  PARAMETERS:
//    hwnd      - The window          (unused)
//    wCommand  - 
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL                (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//

void DisplayBrowseDlg(HWND hwnd) 
{
    DialogBox(hInst, "BrowseDlg", hwnd, (DLGPROC)Browse);  
    
}




//
//  FUNCTION: Browse(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Browse" dialog box.
//
//  PARAMETERS:
//    hdlg     - window handle of the dialog box
//    wMessage - type of message
//    wparam   - message-specific information
//    lparam   - message-specific information
//
//  RETURN VALUE:
//    TRUE  - message handled
//    FALSE - message not handled
//
//  COMMENTS:
//
//

LRESULT CALLBACK Browse(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiBrowse, hdlg, uMessage, wparam, lparam);
}


//
//  FUNCTION: CmdBrowseDone(HWND, WORD, HWND)
//
//  PURPOSE: Free the about box and related data.
//
//  PARAMETERS:
//    hwnd     - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Calls EndDialog to finish the dialog session.
//

LRESULT CmdBrowseDone(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    char  szBuf1 [30], szBuf2[30], szWindowText [60];
    DWORD dwResult;
    HWND  hWndToBrowse = (HWND)GetWindowLong (hdlg, DWL_USER);
    FARPROC fpRemoveMouseHook;
 
    fpRemoveMouseHook = GetProcAddress (hLibrary, "RemoveMouseHook");
    (*fpRemoveMouseHook)(hdlg);

    FreeLibrary(hLibrary);
    UnmapViewOfFile (phSave);
    CloseHandle (hMemMapped);



    EndDialog(hdlg, TRUE);          // Exit the dialog

    if (hWndToBrowse)
    {
        dwResult = GetWindowText (hWndToBrowse, szBuf1, sizeof (szBuf1));
        LimitStringLength (szBuf1, dwResult, 20);
        wsprintf(szBuf2, "0x%04x [%s]", hWndToBrowse, szBuf1);

        lstrcpy (szWindowText, SZDESCRIPTION);
        lstrcat (szWindowText,": ");
        lstrcat (szWindowText, szBuf2);

        SetWindowText (FindWindow (szAppName, NULL), szWindowText);
    }

    return TRUE;
}


//
//  FUNCTION: MsgBrowseCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND message sent to the Browse box.
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
//    Uses this DipsCommand function defined in wndproc.c combined
//    with the cmdiBrowse structure defined in this file to handle
//    the command messages for the Browse dialog box.
//

LRESULT MsgBrowseCommand(HWND   hwnd,
                        UINT   uMessage,
                        WPARAM wparam,
                        LPARAM lparam)
{
    return DispCommand(&cmdiBrowse, hwnd, wparam, lparam);
}




//
//  FUNCTION: MsgBrowseInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the Browse box.
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

LRESULT MsgBrowseInit(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    
    FARPROC fpInstallMouseHook;


    CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));
    SetCursor(LoadCursor(NULL, IDC_ARROW));

    hMemMapped = CreateFileMapping((HANDLE)0xFFFFFFFF, 
                                    NULL, 
                                    PAGE_READWRITE,
                                    0, 
                                    sizeof (HSAVE), 
                                    "BrowseMapping");
    if (!hMemMapped)
    {
        MessageBox (NULL, 
                    "CreateFileMapping() failed.",
                    "ERROR",
                    MB_OK);
        PostMessage (hdlg, WM_COMMAND, (WPARAM)IDCANCEL, (LPARAM)0);
     }
     phSave = (PHSAVE)MapViewOfFile (hMemMapped,
                                        FILE_MAP_WRITE,
                                        0,
                                        0,
                                        sizeof (HSAVE));
     if (!phSave)
     {
        MessageBox (NULL, 
                    "MapViewOfFile() failed.",
                    "ERROR",
                    MB_OK);
        PostMessage (hdlg, WM_COMMAND, (WPARAM)IDCANCEL, (LPARAM)0);
     }
     phSave->hDlg = hdlg;
     phSave->hWndBrowse = NULL;

    hLibrary = LoadLibrary ("DLLSKEL.DLL"); 
    if (!hLibrary)
    {
        MessageBox(NULL, "LOAD LIBRARY FAILED", "ERROR", MB_OK);
        return 0;
    }
        
    fpInstallMouseHook = GetProcAddress (hLibrary, "InstallMouseHook");

    (*fpInstallMouseHook)(hdlg);



    return 0;
}


//
//  FUNCTION: MsgBrowseUpdateText(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To update the BrowseDlg that displays information
//           about the window being browsed.
//
//  PARAMETERS:
//    hwnd     - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam   - Window to browse on.
//    lparam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//

LRESULT MsgBrowseUpdateText (HWND hdlg, 
                             UINT uMessage, 
                             WPARAM wparam, 
                             LPARAM lparam)
{

    typedef struct tagINFO {
       char szWndInfo   [30];
       char szClassInfo [30];
       char szModuleInfo[30];
       char szOwner     [30];
       char szParent    [30];
       char szStyle     [30];
       char szExtended  [30];
    } INFO;

    INFO myInfo;
    HWND hWndToBrowse = (HWND)wparam;
    int  i;

    lstrcpy (myInfo.szWndInfo   , " ");
    lstrcpy (myInfo.szClassInfo , " ");
    lstrcpy (myInfo.szModuleInfo, " ");
    lstrcpy (myInfo.szOwner     , " ");
    lstrcpy (myInfo.szParent    , " ");
    lstrcpy (myInfo.szStyle     , " ");
    lstrcpy (myInfo.szExtended  , " ");

    if (hWndToBrowse)
    {
        char  szBuf  [30] = " ";
        char  szBuf2 [80] = " ";
        HWND  tmphWnd;
        DWORD dwResult;

        dwResult = GetWindowText (hWndToBrowse, szBuf, sizeof (szBuf));
        LimitStringLength (szBuf, dwResult, 20);
        wsprintf(myInfo.szWndInfo, "0x%04x \"%s\"", hWndToBrowse, szBuf);


        dwResult = GetClassName (hWndToBrowse, 
                                 myInfo.szClassInfo, 
                                 sizeof (myInfo.szClassInfo));
        if (dwResult > 30)
            myInfo.szClassInfo [30] = '\0';
    
        dwResult = GetModuleFileName ((HMODULE)GetWindowLong (hWndToBrowse, 
                                               GWL_HINSTANCE),
                                      szBuf2, 
                                      sizeof (szBuf2));
        if (dwResult)
        {
            for (i=(lstrlen (szBuf2)+1); i>= 0; i--)
            {
                if (szBuf2[i] == '\\')
                    break;
            }

            lstrcpy (myInfo.szModuleInfo, szBuf2+i+1);
            LimitStringLength (myInfo.szModuleInfo, 
                               lstrlen (myInfo.szModuleInfo), 
                               30);
        }

        tmphWnd = (HWND)GetWindowLong (hWndToBrowse, GWL_HWNDPARENT);
        if (!tmphWnd)
            lstrcpy (myInfo.szOwner, "0x0000 [UNDEFINED]");
        else
        {
            dwResult = GetClassName (tmphWnd, szBuf, sizeof (szBuf));
            LimitStringLength (szBuf, dwResult, 20);
            wsprintf (myInfo.szOwner, "0x%04x %s", tmphWnd, szBuf);
        }

        tmphWnd = (HWND)GetWindowLong (hWndToBrowse, GWL_HWNDPARENT);
        if (!tmphWnd)
            lstrcpy (myInfo.szParent, "0x0000 [UNDEFINED]");
        else
        {
            dwResult = GetClassName (tmphWnd, szBuf, sizeof (szBuf));
            LimitStringLength (szBuf, dwResult, 20);

            wsprintf (myInfo.szParent, "0x%04x %s", tmphWnd, szBuf);
        }

        wsprintf (myInfo.szStyle, 
                  "0x%08x", 
                  GetWindowLong (hWndToBrowse, GWL_STYLE));
        wsprintf (myInfo.szExtended, 
                  "0x%08x", 
                  GetWindowLong (hWndToBrowse, GWL_EXSTYLE));
    }

    SetDlgItemText (hdlg, IDM_WNDINFO,   myInfo.szWndInfo   );
    SetDlgItemText (hdlg, IDM_CLASSINFO, myInfo.szClassInfo );
    SetDlgItemText (hdlg, IDM_MODULEINFO,myInfo.szModuleInfo);
    SetDlgItemText (hdlg, IDM_OWNER,     myInfo.szOwner     );
    SetDlgItemText (hdlg, IDM_PARENT,    myInfo.szParent    );
    SetDlgItemText (hdlg, IDM_STYLE,     myInfo.szStyle     );
    SetDlgItemText (hdlg, IDM_EXTENDED,  myInfo.szExtended  );
    return 0;
}
