// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   connect.c
//
//  PURPOSE:   Displays the "ConnectDlg" dialog box
//
//  FUNCTIONS:
//    ConnectDlg           - Processes messages for "ConnectDlg" dialog box.
//    MsgConnectDlgInit    - To initialize the connectdlg box.
//    MsgConnectDlgCommand - Process WM_COMMAND message.
//    CmdConnectDlgOK      - Free the ConnectDlg box and return Server Name.
//    CmdConnectDlgCancel  - Free the ConnectDlg box.
//
//  COMMENTS:
//    Dialog Box to get server to connect to
//
//  AUTHOR:  Craig Link - Microsoft Developer Support
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "connect.h"

LRESULT MsgConnectDlgInit(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgConnectDlgCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT CmdConnectDlgOK(HWND, WORD, WORD, HWND);
LRESULT CmdConnectDlgCancel(HWND, WORD, WORD, HWND);


// ConnectDlg dialog message table definition.
MSD rgmsdConnectDlg[] =
{
    {WM_COMMAND,      MsgConnectDlgCommand},
    {WM_INITDIALOG,   MsgConnectDlgInit}
};

MSDI msdiConnectDlg =
{
    sizeof(rgmsdConnectDlg) / sizeof(MSD),
    rgmsdConnectDlg,
    edwpNone
};

// ConnectDlg dialog command table definition.
CMD rgcmdConnectDlg[] =
{
    {IDOK,     CmdConnectDlgOK},
    {IDCANCEL, CmdConnectDlgCancel}
};

CMDI cmdiConnectDlg =
{
    sizeof(rgcmdConnectDlg) / sizeof(CMD),
    rgcmdConnectDlg,
    edwpNone
};

// Module specific "globals"  Used when a variable needs to be
// accessed in more than on handler function.

//
//  FUNCTION: ConnectDlg(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "ConnectDlg" dialog box.
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
//    Dispatches message to appropriate handler
//

LRESULT CALLBACK ConnectDlg(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiConnectDlg, hdlg, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgConnectDlgInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the authdlg box
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. WM_INITDIALOG.
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns TRUE - message handled.
//
//  COMMENTS:
//    Registers window message for RAS event.
//    Makes call to RasDial with appropriate params.
//

LRESULT MsgConnectDlgInit(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    // Set text limits on edit controls
    SendDlgItemMessage(hdlg, IDE_SERVERNAME, EM_LIMITTEXT, (WPARAM) MAX_COMPUTERNAME_LENGTH+2, 0L );

    // Center the dialog over the application window
    CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));

    return TRUE;
}


//
//  FUNCTION: MsgConnectDlgCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND message sent to the authdlg box.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. WM_COMMAND
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns TRUE - message handled.
//
//  COMMENTS:
//    Uses this DispCommand function defined in wndproc.c combined
//    with the cmdiConnectDlg structure defined in this file to handle
//    the command messages for the authdlg dialog box.
//

LRESULT MsgConnectDlgCommand(HWND   hwnd,
                        UINT   uMessage,
                        WPARAM wparam,
                        LPARAM lparam)
{
    return DispCommand(&cmdiConnectDlg, hwnd, wparam, lparam);
}

//
//  FUNCTION: CmdConnectDlgOK(HWND, WORD, WORD,  HWND)
//
//  PURPOSE: Get the server name and free the dialog box.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handle. IDOK.
//    wNotify - The notification to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Calls EndDialog to finish the dialog session.
//

LRESULT CmdConnectDlgOK(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    char szServer[MAX_COMPUTERNAME_LENGTH+5] = "\\\\";
    LPSTR psz = (LPSTR)szServer+ 2L;

    GetDlgItemText( hdlg, IDE_SERVERNAME, psz, MAX_COMPUTERNAME_LENGTH+3 );

    if ( *psz == '\0' ) // no server name, so same as cancelled
    {
        EndDialog(hdlg, FALSE);
    }
    else
    {
        if ( *psz != '\\' ) // if the name does not begin with a '\', we make 
        {                   // it begin with '\\' by ajusting the pointer
            psz = (LPSTR) szServer;
            psz[MAX_COMPUTERNAME_LENGTH+2] = '\0';
        }

        lstrcpy( (LPSTR)g_szServer, psz );
        EndDialog(hdlg, TRUE);
    }
    return TRUE;

}

//
//  FUNCTION: CmdConnectDlgCancel(HWND, WORD, WORD,  HWND)
//
//  PURPOSE: Free the dialog box.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled. IDCANCEL.
//    wNotify - The notification to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Calls EndDialog to finish the dialog session.
//

LRESULT CmdConnectDlgCancel(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    EndDialog(hdlg, FALSE);          // Exit the dialog
    return TRUE;
}
