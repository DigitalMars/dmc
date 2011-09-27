// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   mdichild.c
//
//  PURPOSE:
//    To implement the basic mdi child commands.
//
//  FUNCTIONS:
//    InitMDIChild     - To register the MDI child window class.
//    MDIChildWndProc  - Processes messages for MDI child windows.
//    MsgMCCommand     - Handle the WM_COMMAND messages for mdi children.
//    MsgMCMenuSelect  - Handle the WM_MENUSELECT message for the mdi child
//    MsgMCCreate      - Handle the WM_CREATE message for the MDI child
//    MsgMCSize        - Handle the WM_SIZE message for the MDI child
//    MsgMCSetFocus    - Handle the WM_SETFOCUS message for the MDI child
//    MsgMCClose       - Handle the WM_CLOSE message for the MDI child
//    MsgMCDestroy     - Handle the WM_DESTROY message for the MDI child
//    CmdEdit          - Handle the notification messages for the edit control
//                       contained within the MDI child
//
//  COMMENTS:
//
//
//  SPECIAL INSTRUCTIONS: N/A
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include "globals.h"            // prototypes specific to this application
#include "mdichild.h"
#include "toolbar.h"
#include "statbar.h"
#include "resource.h"

static MSD rgmsd[] =
{
    {WM_SETFOCUS,    MsgMCSetFocus},
    {WM_CREATE,      MsgMCCreate  },
    {WM_COMMAND,     MsgMCCommand },
    {WM_MENUSELECT,  MsgMenuSelect},
    {WM_SIZE,        MsgMCSize    },
    {WM_CLOSE,       MsgMCClose   },
    {WM_DESTROY,     MsgMCDestroy },
};

MSDI msdiChild =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpMDIChild
};

static CMD rgcmd[] =
{
    {IDC_EDIT,       CmdEdit},
};

CMDI cmdiChild =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpMDIChild
};

//Module specific globals

char   szChildName[] = "MdiChild";
static HANDLE hszEditBuffer = NULL;


//
//  FUNCTION: InitMDIChild(HINSTANCE)
//
//  PURPOSE: To register the MDI child window class.
//
//  PARAMETERS:
//    hinst - The instance of the application used to register the class.
//
//  RETURN VALUE:
//    TRUE - Succeeded in registering the class.
//    FALSE - Failed to register the class.
//
//  COMMENTS:
//
//

BOOL InitMDIChild(HINSTANCE hinst)
{
    WNDCLASSEX wc;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC)MDIChildWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hinst;                      // Owner of this class
    wc.hIcon         = LoadIcon(hinst, MAKEINTRESOURCE(IDI_CHILDICON));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Default color
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = szChildName;
    wc.hIconSm       = LoadImage(hInst,            // Load small icon image
	                             MAKEINTRESOURCE(IDI_CHILDICON),
	                             IMAGE_ICON,
	                             16, 16,
	                             0);

    if (!RegisterClassEx(&wc))
    {
        //Assume we are running on NT where RegisterClassEx() is
        //not implemented, so let's try calling RegisterClass().

        if (!RegisterClass((LPWNDCLASS)&wc.style))
        	return FALSE;
    }

    return TRUE;
}


//
//  FUNCTION: MDIChildWndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for MDI child windows.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant of message number)
//    lparam   - additional information (dependant of message number)
//
//  RETURN VALUE:
//    Depends on the message number.
//
//  COMMENTS:
//    Uses the combination of the message structure defined in mditable.c
//    and the DispMessage function defined in WndProc.c to handle all
//    messages directed to an MDI child window.
//

LRESULT CALLBACK MDIChildWndProc(HWND hwnd,
                                 UINT uMessage,
                                 WPARAM wparam,
                                 LPARAM lparam)
{
    return DispMessage( &msdiChild, hwnd, uMessage, wparam, lparam );
}


//
//  FUNCTION: MsgMCCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_COMMAND messages for mdi children.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number (Unused)
//    GET_WM_COMMAND_ID(wparam,lparam)   - Command identifier
//    GET_WM_COMMAND_HWND(wparam,lparam) - Control handle
//
//  RETURN VALUE:
//
//  COMMENTS:
//    Uses the combination of the command structure defined in mditable.c
//    and the DispCommand function defined in WndProc.c to handle all
//    commands directed to an MDI child window.
//
//

LRESULT MsgMCCommand(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&cmdiChild, hwnd, wparam, lparam);
}


//
//  FUNCTION: MsgMCCreate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:
//
//  PARAMETERS:
//    hwnd     - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam   - Message specific data (unused).
//    lparam   - Message specific data (unused).
//
//  RETURN VALUE:
//    
//
//  COMMENTS:
//
//

LRESULT MsgMCCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{

    if (!CreateEditControl(hwnd))
        return -1;
   
    return 0;
}


//
//  FUNCTION: MsgMCSize(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Resize the edit control window within the MDI Child
//
//  PARAMETERS:
//    hwnd     - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam   - Message specific data (unused).
//    lparam   - Message specific data (unused).
//
//  RETURN VALUE:
//    
//
//  COMMENTS:
//    We must call DefMDIChildProc so that if the child is being maximized
//    we'll get the system menu, min, max and close buttons painted properly.
//

LRESULT MsgMCSize(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    MoveWindow(GetEditWindow(hwnd), 
               0, 0, 
               LOWORD(lparam), HIWORD(lparam), 
               TRUE);

    return DefMDIChildProc(hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgMCMenuSelect(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  
//
//  PARAMETERS:
//    hwnd     - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam   - Message specific data (unused).
//    lparam   - Message specific data (unused).
//
//  RETURN VALUE:
//    
//
//  COMMENTS:
//

LRESULT MsgMCMenuSelect(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{

    return DefMDIChildProc(hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgMCSetFocus(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:
//    Sets the focus to the edit control window contained within this MDI
//    child window.
//
//  PARAMETERS:
//    hwnd     - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam   - Message specific data (unused).
//    lparam   - Message specific data (unused).
//
//  RETURN VALUE:
//    
//
//  COMMENTS:
//

LRESULT MsgMCSetFocus(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    SetFocus(GetEditWindow(NULL));

//    return DefMDIChildProc(hwnd, uMessage, wparam, lparam);
    return 0;
}


//
//  FUNCTION: MsgMCClose(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Query user to save file before closing window
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Return 0 if user hits "Cancel" from the query save dialog.  This
//    prevents the MDI child from closing.
//
//  COMMENTS:
//    Call DefMDIChildProc so that the window gets destroyed.
//

LRESULT MsgMCClose(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    if (QuerySaveFile(hwnd))
        return DefMDIChildProc(hwnd, uMessage, wparam, lparam);
    else
        return 0;
}


//
//  FUNCTION: MsgMCDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:
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

LRESULT MsgMCDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    cOpen -= 1;
    RemoveProp(hwnd, "FilenameSet");

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
//  FUNCTION: CreateMDIChild(LPSTR)
//
//  PURPOSE: Create an MDI child window, setting caption to szFileName
//
//  PARAMETERS:
//    hwnd - The main application window.
//
//  RETURN VALUE:
//    TRUE  - If creation was successful.
//    FALSE - If creation failed.
//
//  COMMENTS:
//

BOOL CreateMDIChild(LPSTR szFileName)
{
    HWND hwndChild;
    DWORD dwVersion;

    // Create the MDI child window

    // Windows NT and Windows 95 present different options for creating
    // an MDI child window.  While using the WM_MDICREATE message will
    // work on both Windows versions, Windows 95 presents a new window
    // style which simplifies the process.  Here the function uses the
    // method apropriate for the system it's running on.

    dwVersion = GetVersion();
    if ((dwVersion < 0x80000000) || (LOBYTE(LOWORD(dwVersion)) < 4))
    {
        // This is Windows NT or Win32s, so use the WM_MDICREATE message
        MDICREATESTRUCT mcs;

        mcs.szClass = szChildName;      // window class name
        mcs.szTitle = szFileName;       // window title
        mcs.hOwner  = hInst;            // owner
        mcs.x       = CW_USEDEFAULT;    // x position
        mcs.y       = CW_USEDEFAULT;    // y position
        mcs.cx      = CW_USEDEFAULT;    // width
        mcs.cy      = CW_USEDEFAULT;    // height
        mcs.style   = 0;                // window style
        mcs.lParam  = 0;                // lparam

        hwndChild = (HWND) SendMessage(hwndMDIClient,
                                       WM_MDICREATE,
                                       0,
                                       (LPARAM)(LPMDICREATESTRUCT) &mcs);
    }
    else
    {
        // This method will only work with Windows 95, not Windows NT or Win32s
        hwndChild = CreateWindowEx(WS_EX_MDICHILD,  // EX window style
                                   szChildName,     // window class name
                                   szFileName,      // window title
                                   0,               // window style
                                   CW_USEDEFAULT,   // x position
                                   CW_USEDEFAULT,   // y position
                                   CW_USEDEFAULT,   // width
                                   CW_USEDEFAULT,   // height
                                   hwndMDIClient,   // parent
                                   NULL,            // menu (child ID)
                                   hInst,           // owner
                                   0);              // lparam
    }

    if (hwndChild != NULL)
    {
        ShowWindow(hwndChild, SW_SHOW);
        SetProp(hwndChild, "FilenameSet", 0);

        cUntitled += 1;
        cOpen += 1;
        return TRUE;
    }
    else
        return FALSE;
}


//
//  FUNCTION: CreateEditControl(HWND)
//
//  PURPOSE: Create the edit control with the MDI child as the parent
//
//  PARAMETERS:
//    hwnd - The MDI child window.
//
//  RETURN VALUE:
//    TRUE - If initialization was successful.
//    FALSE - If initialization failed.
//
//  COMMENTS:
//

BOOL CreateEditControl(HWND hwndMDIChild)
{
    RECT rc;
    HWND hwndEdit;

    GetClientRect(hwndMDIChild, &rc);

    hwndEdit = CreateWindow("Edit",
                            NULL,
                            WS_CHILD   | WS_VISIBLE | ES_MULTILINE   |
                            WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL |
                            ES_AUTOVSCROLL | ES_NOHIDESEL,
                            0, 0,
                            (rc.right-rc.left), (rc.bottom-rc.top),
                            hwndMDIChild,
                            (HMENU)IDC_EDIT,           // Child control i.d.
                            hInst,
                            NULL);

    if (!hwndEdit)
    {
        DestroyWindow(hwndMDIChild);
        return FALSE;
    }

    return TRUE;
}


//
//  FUNCTION: GetEditWindow(HWND)
//
//  PURPOSE: Return a handle to the current edit control.  The "current"
//           edit control is contained within the active MDI child
//
//  PARAMETERS:
//    hwdnMDIChild - handle to the MDI Child window
//
//  RETURN VALUE:
//    A handle to the current edit control.
//
//  COMMENTS:
//    If hwndMDIChild is NULL on entry, we'll return the handle to the
//    edit control in the active MDI child
//

HWND GetEditWindow(HWND hwndMDIChild)
{
    HWND hwndEdit;
    
    if (!hwndMDIChild)
        hwndMDIChild = GetActiveMDIChild();

    hwndEdit = GetDlgItem(hwndMDIChild, IDC_EDIT);

    return hwndEdit;
}


//
//  FUNCTION: GetActiveMDIChild(void)
//
//  PURPOSE: Return a handle to the current active MDI child
//
//  PARAMETERS:
//    void
//
//  RETURN VALUE:
//    A handle to the current active MDI child
//
//  COMMENTS:
//    

HWND GetActiveMDIChild(void)
{
    HWND hwndMDIChild;
    
    hwndMDIChild = (HWND)SendMessage(hwndMDIClient, 
                                     WM_MDIGETACTIVE, 
                                     0, 0L);

    return hwndMDIChild;
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
//    
//

VOID SetEditText(HWND hwnd, HANDLE hsz)
{
    char *sz = LocalLock(hsz);
    SendMessage(GetEditWindow(NULL), WM_SETTEXT, 0, (LPARAM)(LPSTR)sz);
    LocalUnlock(hsz);
    LocalFree(hsz);
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
    SetEditText(hwnd, hszEditBuffer);
    hszEditBuffer = NULL;
}
