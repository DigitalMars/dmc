// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993, 1994  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   connect.c
//
//  PURPOSE:   Displays the "Connect" dialog box
//
//  FUNCTIONS:
//    CmdConnect              - Displays the "Connect" dialog box
//    Connect                 - Processes messages for "Connect" dialog box.
//    MsgConnectInit          - Initializes edit controls
//    MsgConnectReadyForWrite - Sends name registration message when connection 
//                              established with server
//    MsgConnectCommand       - Process WM_COMMAND message sent to the connect box.
//    CmdConnectDone          - Free the connect box and related data.
//    CmdConnectNow           - Establishes connection to server.
//    CmdConnectEnableOK      - Enables/Disables OK button when input data is 
//                              valid/invalid 
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <wsipx.h>              // IPX sockets
#include <wsnetbs.h>            // NetBIOS sockets
#include <stdlib.h>
#include <stdio.h>
#include <nspapi.h>
#include <svcguid.h>
#include "globals.h"            // prototypes specific to this application

#define WSN_READYFORWRITE   700 // dlg window message indicating the connection is ready to send on

//   Function Definitions

LRESULT CALLBACK Connect(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgConnectInit(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgConnectReadyForWrite(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgConnectCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT CmdConnectNow(HWND, WORD, WORD, HWND);
LRESULT CmdConnectDone(HWND, WORD, WORD, HWND);
LRESULT CmdConnectEnableOK(HWND, WORD, WORD, HWND);

// Connect dialog message table definition.
MSD rgmsdConnect[] =
{
    {WM_COMMAND,        MsgConnectCommand},
    {WM_INITDIALOG,     MsgConnectInit},
    {WSN_READYFORWRITE,  MsgConnectReadyForWrite}
};

MSDI msdiConnect =
{
    sizeof(rgmsdConnect) / sizeof(MSD),
    rgmsdConnect,
    edwpNone
};

// Connect dialog command table definition.
CMD rgcmdConnect[] =
{
    {IDOK,        CmdConnectNow},
    {IDCANCEL,    CmdConnectDone},
    {CD_NAME,     CmdConnectEnableOK},
    {CD_PROTOCOL, CmdConnectEnableOK},
	{CD_SERVER,   CmdConnectEnableOK}
};

CMDI cmdiConnect =
{
    sizeof(rgcmdConnect) / sizeof(CMD),
    rgcmdConnect,
    edwpNone
};

// Module specific "globals"  Used when a variable needs to be
// accessed in more than one handler function.

HFONT hfontDlg;

//
//  FUNCTION: CmdConnect(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays the "Connect" dialog box
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_CONNECT (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    To process the IDM_CONNECT message, call DialogBox() to display the
//    Connect dialog box.

LRESULT CmdConnect(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{

    // Start dialog box
    if(DialogBox(hInst, "ConnectBox", hwnd, (DLGPROC)Connect))
    {
        // We have a connection and have registered our name.  Set up Select dialog.
        PostMessage(hwnd, WM_COMMAND, MAKELONG(IDM_SELECT,0), 0);
        return 0;
    }

    // Connection failed - reset window title
    SetWindowText(hwnd, szTitle);
    return 0;
}


//
//  FUNCTION: Connect(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Connect" dialog box.
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
//     Gets connection information from user and then establishes a connection.
//
//     Connect when user clicks on the OK button.  Kill Dialog when connection
//     established.
//

LRESULT CALLBACK Connect(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiConnect, hdlg, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgConnectInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To center dialog, limit size of edit controls, and set up available protocols
//           to connect with
//
//  PARAMETERS:
//    hdlg - The window handling the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//    Set size of edit controls for the following
//           Network  8  chars (4 2-digit hex numbers)
//           Node     12 chars (6 2-digit hex numbers)
//           Socket   4  chars (2 2-digit hex numbers)
//

LRESULT MsgConnectInit(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int numStrucs;
    int j, k;
    int iProtBufSize = 0;
	char outtext[80];

    // Create a font to use
    hfontDlg = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          VARIABLE_PITCH | FF_SWISS, "");

    // Center the dialog over the application window
    CenterWindow (hdlg, GetWindow (hdlg, GW_OWNER));

	// Call EnumProtocols with NULL buffer in order to determine size of buffer required
	EnumProtocols(NULL, NULL, &iProtBufSize);

    if (((LPVOID)lpProtBuf = VirtualAlloc(NULL,
                                          iProtBufSize,
                                          MEM_COMMIT,
                                          PAGE_READWRITE)) == NULL)
    {
        // ERROR -- abort
        return 0;
    }

    // Calling EnumProtocols with large enough buffer
    if ((numStrucs = EnumProtocols(NULL, lpProtBuf, &iProtBufSize)) == SOCKET_ERROR)
    {
        // Error -- abort
        return 0;
    }
    
	// Add every connection oriented protocol to protocol combo box
    for (j = 0, k = 0; j < numStrucs; j++)
    {
        if ((lpProtBuf[j].dwServiceFlags & XP_CONNECTIONLESS) == 0)
        {
			wsprintf(outtext, "%2d: %s", k, lpProtBuf[j].lpProtocol);
            SendMessage(GetDlgItem(hdlg, CD_PROTOCOL), CB_ADDSTRING, 0, (LPARAM)outtext);
            goodprots[k++] = j; // Keep track of good protocols
        }
    }

    // Limit size of edit controls
    SendDlgItemMessage(hdlg, CD_NAME, EM_LIMITTEXT, 15, 0);

    // Initialize Edit Controls
    SetDlgItemText(hdlg, CD_NAME, szConnectName);

    return (TRUE);
}

//
//  FUNCTION: MsgConnectReadyForWrite(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Receive notification of connected socket and register name
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
//    Uses this DispCommand function defined in wndproc.c combined
//    with the cmdiConnect structure defined in this file to handle
//    the command messages for the Connect dialog box.
//

LRESULT MsgConnectReadyForWrite(HWND   hwnd,
                                UINT   uMessage,
                                WPARAM wparam,
                                LPARAM lparam)
{
    char outtext[80];

    // Build our name
    GetDlgItemText(hwnd, CD_NAME, MySock.name, sizeof(MySock.name));

    // Build name registration packet
    xferbuf.hdr.signature = MYSIGNATURE;                 // signature
    xferbuf.hdr.length = REALLEN(MySock.name) + HDRSIZE;
    lstrcpy(xferbuf.data, MySock.name);                  // copy name
    xferbuf.hdr.command = REGISTER_NAME;                 // REGISTER_NAME command

    // Lets send it
    senddatamessage(MySock.sock, &xferbuf);

    wsprintf(outtext, "%s: available for chatting", MySock.name);
    SetWindowText(GetParent(hwnd), outtext);

    EndDialog(hwnd, TRUE);          // Exit the dialog
    DeleteObject (hfontDlg);
    return (TRUE);
}

//
//  FUNCTION: MsgConnectCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND message sent to the Connect box.
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
//    Uses this DispCommand function defined in wndproc.c combined
//    with the cmdiConnect structure defined in this file to handle
//    the command messages for the Connect dialog box.
//

LRESULT MsgConnectCommand(HWND   hwnd,
                          UINT   uMessage,
                          WPARAM wparam,
                          LPARAM lparam)
{
    return DispCommand(&cmdiConnect, hwnd, wparam, lparam);
}

//
//  FUNCTION: CmdConnectDone(HWND, WORD, HWND)
//
//  PURPOSE: Free the Connect box and related data.
//
//  PARAMETERS:
//    hdlg - The window handling the command.
//    wCommand - The command to be handled (unused).
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Cleans up sockets then calls EndDialog to finish the dialog session.
//

LRESULT CmdConnectDone(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{

    closesocket(MySock.sock);      // Free any aborted socket resources
    EndDialog(hdlg, FALSE);        // Exit Dialog -- rtrn false since no connection
    return TRUE;
}

//
//  FUNCTION: CmdConnectNow(HWND, WORD, HWND)
//
//  PURPOSE: Establish the connection
//
//  PARAMETERS:
//    hdlg - The window handling the command.
//    wCommand - The command to be handled (unused).
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Makes Connection calls
//

LRESULT CmdConnectNow(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    int iIndex;
    char outtext[80];
    int numStrucs;
	GUID guidNW = SVCID_NETWARE(NWCHATID);
	GUID guidDNS = SVCID_TCP(DNSCHATID);

    // Get protocol selected
    iIndex = SendMessage(GetDlgItem(hdlg, CD_PROTOCOL), CB_GETCURSEL, 0, 0);

	// ************************************************************************
	//          
	//    Below we will be calling GetAddressByName with the RES_FIND_MULTIPLE
	//    option in order to find a remote address to connect() to.  At this 
	//    point, GetAddressByName() only supports the DNS and the SAP/Bindery 
	//    name spaces.  Ultimately a single call to GetAddressByName will 
	//    query all available protocols and name spaces, but this requires a 
	//    central database which is currently not available.  In the mean time 
	//    we will make name space specific calls to GetAddressByName.
	//
	// ***********************************************************************

    switch (lpProtBuf[goodprots[iIndex]].iAddressFamily)
    {
        case AF_IPX:
		    //  SAP/Bindery Name Space
		    dwCSABufsize = sizeof(CSABuf);
            if (((numStrucs = GetAddressByName(0,  // GUID indicates name space so we don't need to specify
                                               &guidNW, // GUID generated from NetWare service type
                                               "GLOBCHAT", // Name of service to look for
                                               NULL, // GUID implies IPX/SPX protocols
                                               RES_FIND_MULTIPLE, // We might get multiple responses
                                               NULL, // Not currently supported
                                               CSABuf, // results buffer
                                               &dwCSABufsize,  // size of results buffer
                                               NULL,  // not supported
                                               NULL)  // not supported
                                               ) == SOCKET_ERROR) || (numStrucs == 0))
            {
                // Error -- try another protocol.  We've got lots!
                MessageBox(hdlg, "Server not found.  Try a different protocol", NULL, MB_OK);
                EnableWindow(GetDlgItem(hdlg, IDOK), FALSE);
                SetFocus(GetDlgItem(hdlg, CD_PROTOCOL));
                return 0;
            }
            break;

        case AF_INET:
            //  DNS Name Space
            
            // Static Name Space requires us to specify the host name
            GetDlgItemText(hdlg, CD_SERVER, lpServName, sizeof(lpServName));  
			dwCSABufsize = sizeof(CSABuf);
            if (((numStrucs = GetAddressByName(0,	  // GUID indicates name space so we don't need to specify
                                             &guidDNS, // GUID generated from TCP port number
                                             lpServName, // Name of host to look for
                                             NULL, // GUID implies UDP/TCP protocols
                                             RES_FIND_MULTIPLE, // We might get multiple responses
                                             NULL, // Not currently supported
                                             CSABuf, // results buffer
                                             &dwCSABufsize, // size of results buffer
                                             NULL, // not supported
                                             NULL) // not supported
                                             ) == SOCKET_ERROR) || (numStrucs == 0))
            {
                // Error -- try another protocol.  We've got lots!
                MessageBox(hdlg, "Server not found.  Try a different protocol", NULL, MB_OK);
                EnableWindow(GetDlgItem(hdlg, IDOK), FALSE);
                SetFocus(GetDlgItem(hdlg, CD_PROTOCOL));
                return 0;
            }
            break;

        case AF_NETBIOS:
			// NetBIOS name space???
		    
            // no netbios name space provider so fill in lpCSABuf ourselves
            numStrucs = 1;
            CSABuf[0].iSocketType = lpProtBuf[goodprots[iIndex]].iSocketType;
            CSABuf[0].iProtocol = lpProtBuf[goodprots[iIndex]].iProtocol;

            SET_NETBIOS_SOCKADDR(&NBAddr,
                                 NETBIOS_GROUP_NAME,
                                 "GLOBSERV",
                                 0);
	        CSABuf[0].RemoteAddr.lpSockaddr = (LPSOCKADDR)&NBAddr;
			CSABuf[0].RemoteAddr.iSockaddrLength = sizeof(NBAddr);
            break;

        default:
		    // We don't support anything else
            MessageBox(hdlg, "Name Space not supported over this protocol family.  Try a different protocol", NULL, MB_OK);
            EnableWindow(GetDlgItem(hdlg, IDOK), FALSE);
            SetFocus(GetDlgItem(hdlg, CD_PROTOCOL));
            return 0;
    }

	// Populate SOCKDATA struct
    MySock.type = lpProtBuf[goodprots[iIndex]].iSocketType;
    MySock.protocol = lpProtBuf[goodprots[iIndex]].iProtocol;

    SetDlgItemText(hdlg, CD_HELP, "Calling socket()");

	// Call socket() using triple provided by EnumProtocols()
    if((MySock.sock = socket(lpProtBuf[goodprots[iIndex]].iAddressFamily,
                             lpProtBuf[goodprots[iIndex]].iSocketType,
                             lpProtBuf[goodprots[iIndex]].iProtocol)) == INVALID_SOCKET)
    {
        // ERROR
        wsprintf(outtext, "Failure: socket() returned %u", WSAGetLastError());
        SetDlgItemText(hdlg, CD_HELP, outtext);
        return 0;
    }

    SetDlgItemText(hdlg, CD_HELP, "Calling connect()");

	// Call connect() using info from GetAddressByName()
    if (connect(MySock.sock,
                CSABuf[0].RemoteAddr.lpSockaddr,
                CSABuf[0].RemoteAddr.iSockaddrLength) == SOCKET_ERROR)
    {
        // ERROR
        wsprintf(outtext, "Failure: connect() returned %u", WSAGetLastError());
        SetDlgItemText(hdlg, CD_HELP, outtext);
        return 0;
    }

    // Signal for when we are ready for writing
    SetDlgItemText(hdlg, CD_HELP, "Waiting for the server to accept() us");

	// Specify message to signal accepted socket
    if (WSAAsyncSelect(MySock.sock, hdlg, WSN_READYFORWRITE, FD_WRITE) == SOCKET_ERROR)
    {
        wsprintf(outtext, "Failure: WSAAsyncSelect() returned %u", WSAGetLastError());
        SetDlgItemText(hdlg, CD_HELP, outtext);
        return 0;
    }
    return 0;

}

//
//  FUNCTION: CmdConnectEnableOK(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Enable/Disable OK button when input data is valid/invalid
//
//  PARAMETERS:
//    hdlg - The window handling the command.
//    wCommand - The command to be handled (unused).
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Checks for acceptable name and protocol.  We also need an acceptable
//    machine name for TCP/IP protocol since DNS name space is static
//

LRESULT CmdConnectEnableOK(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    char buf[128];
	int iIndex;

    // Is message a change notification?
    if ((wNotify == CBN_SELCHANGE) || (wNotify == EN_CHANGE))
    {
        // Yes!, get protocol
        iIndex = SendMessage(GetDlgItem(hdlg, CD_PROTOCOL), CB_GETCURSEL, 0, 0);
		if (iIndex == LB_ERR)
		{
		    // No protocol selected...OK disabled
		    EnableWindow(GetDlgItem(hdlg, IDOK), FALSE);
		    return TRUE;
		}
        // Protocol selected!  Check which one...
        switch(lpProtBuf[goodprots[iIndex]].iAddressFamily)
        {
            case AF_IPX:
            case AF_NETBIOS:
                // For IPX and NetBIOS, we don't need the machine name
				// so hide that edit control
                ShowWindow(GetDlgItem(hdlg, CD_SERVER), SW_HIDE);
                ShowWindow(GetDlgItem(hdlg, CD_SERVER_TEXT), SW_HIDE);
				
				// We already have a protocol selected.  Do we have a name?
                if(GetDlgItemText(hdlg, CD_NAME, buf, sizeof(buf)) >= 1)
                {
                    // Yes, we have a name and a protocol.  Enable OK.
                    EnableWindow(GetDlgItem(hdlg, IDOK), TRUE);
                }
				else
				{
				    // We don't have a name.  Disable OK.
				    EnableWindow(GetDlgItem(hdlg, IDOK), FALSE);
				}
                return TRUE;

            case AF_INET:
			    // For TCP/IP we need a protocol, name, and machine name
				// in order for data to be valid

				// Enable Machine name edit control and text
                ShowWindow(GetDlgItem(hdlg, CD_SERVER), SW_SHOW);
				ShowWindow(GetDlgItem(hdlg, CD_SERVER_TEXT), SW_SHOW);
				EnableWindow(GetDlgItem(hdlg, CD_SERVER), TRUE);
				EnableWindow(GetDlgItem(hdlg, CD_SERVER_TEXT), TRUE);
                
                // We have a protocol selected.  Do we have a name and a server name
                if((GetDlgItemText(hdlg, CD_NAME, buf, sizeof(buf)) >= 1) &&
                   (GetDlgItemText(hdlg, CD_SERVER, buf, sizeof(buf)) >= 1))
                {
                    // YES!  Enable OK.
                    EnableWindow(GetDlgItem(hdlg, IDOK), TRUE);
					SendMessage(GetDlgItem(hdlg, IDOK), BM_SETSTYLE, (WPARAM) LOWORD(BS_DEFPUSHBUTTON), MAKELPARAM(TRUE, 0));
                }
				else
				{
				    // No name or server name.  Disable OK.
				    EnableWindow(GetDlgItem(hdlg, IDOK), FALSE);
				}
                return TRUE;

            default:
			    // We only support DNS, SAP/Bindery, and we simulate NetBIOS name spaces.
                SetDlgItemText(hdlg, CD_HELP, "This protocol uses an unsupported name space");
                EnableWindow(GetDlgItem(hdlg, IDOK), FALSE);
                return TRUE;
        }
    }

    // When protocol combo box gets the focus, set help text at bottom of dialog
    if (wNotify == CBN_SETFOCUS)
    {
        SetDlgItemText(hdlg, CD_HELP, "Select Protocol to use");
    }
    
    if (wNotify == EN_SETFOCUS)
    {
        if (hwndCtrl == GetDlgItem(hdlg, CD_NAME))
        {
            // When name edit control gets the focus, set help text at bottom of dialog.
            SetDlgItemText(hdlg, CD_HELP, "Enter name you will be known by");
        }
        else
        {
            // When server edit control gets the focus, set help text at bottom of dialog.
            SetDlgItemText(hdlg, CD_HELP, "Enter machine name Global Chat Server is running on");
        }
    }
    return TRUE;
}
