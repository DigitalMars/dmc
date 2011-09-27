// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993, 1994  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:    select.c
//
//  PURPOSE:   Displays the "Select" dialog box
//
//  FUNCTIONS:
//    CmdSelect          - Displays the "Select" dialog box
//    Select             - Processes messages for "Select" dialog box.
//    MsgSelectInit      - Sets up event message notification for socket
//    MsgSelectDataReady - Process socket event message notifications
//    MsgSelectCommand   - Process WM_COMMAND message sent to the select box.
//    CmdSelectDone      - Free the select box and related data.
//    CmdSelectOK        - Request Peer session
//    CmdSelectList		 - Enables/Disables OK button
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <wsipx.h>              // IPX Sockets defs
#include <wsnetbs.h>
#include <nspapi.h>
#include "globals.h"            // prototypes specific to this application

LRESULT CALLBACK Select(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgSelectInit(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgSelectCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgSelectDataReady(HWND, UINT, WPARAM, LPARAM);
LRESULT CmdSelectDone(HWND, WORD, WORD, HWND);
LRESULT CmdSelectOK(HWND, WORD, WORD, HWND);
LRESULT CmdSelectList(HWND, WORD, WORD, HWND);

// Select dialog message table definition.
MSD rgmsdSelect[] =
{
    {WM_COMMAND,    MsgSelectCommand},
    {WM_INITDIALOG, MsgSelectInit},
    {MW_DATAREADY,  MsgSelectDataReady}
};

MSDI msdiSelect =
{
    sizeof(rgmsdSelect) / sizeof(MSD),
    rgmsdSelect,
    edwpNone
};

// Select dialog command table definition.
CMD rgcmdSelect[] =
{
    {IDOK,     CmdSelectOK},
    {IDCANCEL, CmdSelectDone},
    {SD_LIST,  CmdSelectList}
};

CMDI cmdiSelect =
{
    sizeof(rgcmdSelect) / sizeof(CMD),
    rgcmdSelect,
    edwpNone
};

// Module specific "globals"  Used when a variable needs to be
// accessed in more than on handler function.

HFONT hFontCopyright;

//
//  FUNCTION: CmdSelect(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays the "Select" dialog box
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_Select (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    To process the IDM_Select message, call DialogBox() to display the
//    Select dialog box.

LRESULT CmdSelect(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    char outtext[80];

    if (DialogBox(hInst, "SelectBox", hwnd, (DLGPROC)Select))
    {
        // reset socket event messaging to point at the main window
        if (WSAAsyncSelect(MySock.sock, hwnd, MW_DATAREADY, FD_READ | FD_CLOSE) == SOCKET_ERROR)
        {
            // I hate it when this happens!
            wsprintf(outtext, "WSAAsyncSelect() failed w/ error %u", WSAGetLastError());
            MessageBox(hwnd, outtext, "Oops!", MB_OK);
            return 0;
        }

        // We've got a connection...set up menu and title bar
        EnableMenuItem(GetMenu(hwnd), IDM_END_CHAT, MF_ENABLED);
        EnableMenuItem(GetMenu(hwnd), IDM_DISCONNECT_SERVER, MF_ENABLED);
        EnableMenuItem(GetMenu(hwnd), IDM_CONNECT, MF_GRAYED);
        wsprintf(outtext, "%s: chatting with remote %s", MySock.name, peername);
        SetWindowText(hwnd, outtext);
        // we're now ready to rumble
    }
    else
    {
        // clean up
        closesocket(MySock.sock);
        EnableMenuItem(GetMenu(hwnd), IDM_END_CHAT, MF_GRAYED);
        EnableMenuItem(GetMenu(hwnd), IDM_DISCONNECT_SERVER, MF_GRAYED);
        EnableMenuItem(GetMenu(hwnd), IDM_CONNECT, MF_ENABLED);
        SetWindowText(hwnd, szTitle);
    }
    return 0;
}


//
//  FUNCTION: Select(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Select" dialog box.
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

LRESULT CALLBACK Select(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiSelect, hdlg, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgSelectInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Sets up socket message notifications
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
//    Uses WSAAsyncSelect() to have socket event notifications sent
//    to the Select Dialog Window
//

LRESULT MsgSelectInit(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{

    // Center the dialog over the application window
    CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));

    if(WSAAsyncSelect(MySock.sock, hdlg, MW_DATAREADY, FD_READ | FD_CLOSE) != 0)
    {
        // ERROR  do something
    }
    return TRUE;
}

//
//  FUNCTION: MsgSelectDataReady(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes incoming messages from server
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
//    Receive message and then process command in message header
//

LRESULT MsgSelectDataReady(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{

    int iIndex;
    char displaytext[128];

    // First check to see if connection was closed
    if(LOWORD(lparam) == FD_CLOSE)
    {
        // Connection closed kill our socket
        closesocket(MySock.sock);

        MessageBox(hdlg, "Server Connection Dropped", "Oops!", MB_OK);

        // Fix menus
        EnableMenuItem(GetMenu(GetParent(hdlg)), IDM_CONNECT, MF_ENABLED);
        EnableMenuItem(GetMenu(GetParent(hdlg)), IDM_DISCONNECT_SERVER, MF_GRAYED);
        EnableMenuItem(GetMenu(GetParent(hdlg)), IDM_END_CHAT, MF_GRAYED);

        EndDialog(hdlg, FALSE);
        return 0;
    }
    
    // receive the message
    if(!recvdatamessage(&MySock, &xferbuf))
    {
        return 0;
    }

    // We've got our whole message!  Now switch on the command flag
    switch(xferbuf.hdr.command)
    {
        case REGISTER_NAME:
            // Add name to list box
            SendMessage(GetDlgItem(hdlg, SD_LIST), LB_ADDSTRING, 0, (LPARAM)&xferbuf.data);
            break;

        case DEREGISTER_NAME:
            // Remove name from list box
            if((iIndex = SendMessage(GetDlgItem(hdlg, SD_LIST),
                                     LB_FINDSTRING,
                                     (UINT)-1, (LPARAM)&xferbuf.data)) != LB_ERR)
            {
                // found the index to the item...delete it!
                SendMessage(GetDlgItem(hdlg, SD_LIST), LB_DELETESTRING, iIndex, 0);
            }
            break;

        case REQUEST_SESSION:
		    // Someone is asking us for a chat session
            lstrcpy(displaytext, xferbuf.data);
            lstrcat(displaytext, " requests a chat.");
            if(MessageBox(hdlg, displaytext, "Session Request", MB_OKCANCEL) == IDOK)
            {
                // save the name of the peer
                lstrcpy(peername, xferbuf.data);
                // We've got ourselves a session!  Send response
                xferbuf.hdr.command = SESSION_REQUEST_RESPONSE;
                *(xferbuf.data) = 1;    // Flag to say we accept this session
                senddatamessage(MySock.sock, &xferbuf);
                EndDialog(hdlg, TRUE);  // kill the dialog
                return 0;
            }
            else
            {
                // We're obviously to stuck up to accept a session
				// from this lowlife...send the denial message
                xferbuf.hdr.command = SESSION_REQUEST_RESPONSE;
                *(xferbuf.data) = 0;    // Flag to deny session
                xferbuf.hdr.length = HDRSIZE;
                senddatamessage(MySock.sock, &xferbuf);
                return 0;
            }
            break;  // should never get here

        case SESSION_REQUEST_RESPONSE:
		    // Someone responded to our session request!

            if (MySock.status != SOCKSTAT_REQSESSION)
            {
                return 0;  // we didn't expect this...drop packet
            }
            if (*xferbuf.data == 1)
            {
                // Oh JOY!  Acceptance by our peer!
                EndDialog(hdlg, TRUE);
                return 0;
            }
            else
            {
                // Depression...nobody loves us: reenable listbox so we 
                // can just get rejected again.
                EnableWindow(GetDlgItem(hdlg, SD_LIST), TRUE);
                EnableWindow(GetDlgItem(hdlg, IDOK), TRUE);
                return 0;
            }


        default:
            return 0; // This is unexpected... drop packet
    }

    return TRUE;
}

//
//  FUNCTION: MsgSelectCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND message sent to the Select box.
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
//    with the cmdiSelect structure defined in this file to handle
//    the command messages for the Select dialog box.
//

LRESULT MsgSelectCommand(HWND   hwnd,
                         UINT   uMessage,
                         WPARAM wparam,
                         LPARAM lparam)
{
    return DispCommand(&cmdiSelect, hwnd, wparam, lparam);
}

//
//  FUNCTION: CmdSelectDone(HWND, WORD, HWND)
//
//  PURPOSE: Free the Select box and related data.
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

LRESULT CmdSelectDone(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    if (hFontCopyright)
       DeleteObject(hFontCopyright);

    EndDialog(hdlg, FALSE);          // Exit the dialog
    return TRUE;
}

//
//  FUNCTION: CmdSelectOK(HWND, WORD, HWND)
//
//  PURPOSE: Requests session with peer
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
//    Sends session request message to selected peer
//

LRESULT CmdSelectOK(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    int iIndex;

    // Get selected peer name
    iIndex = SendMessage(GetDlgItem(hdlg, SD_LIST), LB_GETCURSEL, 0, 0);
    SendMessage(GetDlgItem(hdlg, SD_LIST), LB_GETTEXT, iIndex, (LPARAM)&peername);

    // Build session request message
    MySock.status = SOCKSTAT_REQSESSION;
    xferbuf.hdr.signature = MYSIGNATURE;
    xferbuf.hdr.length = REALLEN(peername) + HDRSIZE;
    xferbuf.hdr.command = REQUEST_SESSION;
    lstrcpy(xferbuf.data, peername);
    senddatamessage(MySock.sock, &xferbuf);       // Send it!

    EnableWindow(GetDlgItem(hdlg, SD_LIST), FALSE);
    EnableWindow(GetDlgItem(hdlg, IDOK), FALSE);

    return TRUE;
}

//
//  FUNCTION: CmdSelectList(HWND, WORD, HWND)
//
//  PURPOSE: Enables OK button
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
//    When a peer is selected, enable OK button
//

LRESULT CmdSelectList(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    if (wNotify == LBN_SELCHANGE)  // A peer has been selected!
    {
        EnableWindow(GetDlgItem(hdlg, IDOK), TRUE);	  // Enable that button!
    }
    return TRUE;
}

