// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993, 1994  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   generic.c
//
//  PURPOSE:   Implement the windows procedure for the main application
//    windows.  Also implement the generic message and command dispatchers.
//
//  FUNCTIONS:
//    WndProc      - Processes messages for the main window.
//    MsgCreate    - Handles the WM_CREATE message.  Sets up Display and
//                   initializes listening sockets.  Also advertises the 
//                   service
//    MsgSize      - Resizes the Listboxes to fit nicely in main window.
//    MsgTimer     - Handles the WM_TIMER messages which alert us to the
//                   fact that its time to send a SAP packet
//    MsgConnected - Handles new connection notifications
//    MsgDataready - Handles recv data notifications and close socket
//                   notifications
//    MsgCommand   - Handle the WM_COMMAND messages for the main window.
//    MsgDestroy   - Handles the WM_DESTROY message by calling
//                   PostQuitMessage().
//    CmdExit      - Handles the file exit command by calling destory
//                   window on the main window.
//
//  COMMENTS:
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <winsock.h>
#include <wsipx.h>
#include <wsnetbs.h>
#include <svcguid.h>
#include "globals.h"            // prototypes specific to this application


// Main window message table definition.
MSD rgmsd[] =
{
    {WM_CREATE,    MsgCreate},
    {WM_SIZE,      MsgSize},
    {WM_TIMER,     MsgTimer},
    {WM_COMMAND,   MsgCommand},
    {WM_DESTROY,   MsgDestroy},
    {MW_CONNECTED, MsgConnected},
    {MW_DATAREADY, MsgDataready}
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
    {IDM_ABOUT, CmdAbout}
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
//  FUNCTION: MsgCreate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Sets up child windows, initializes listening sockets, and advertises
//            the existence of the service
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

LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int sizeProtBuf = 0; 
    int cNumProt;
    int j, k;
    int GoodProts[40];
    WORD wVersionRequested;
    WSADATA wsaData;
    char outtext[80];
    int tabwidth[1] = {100};
    BOOL bcaststat;
    int addrlen;

    // Create Protocol listbox
    hwndProtocolList = CreateWindow("LISTBOX",
                                    NULL,
                                    WS_BORDER | WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_USETABSTOPS,
                                    0, 0, 0, 0,
                                    hwnd,
                                    (HMENU)ID_PROTOCOLBOX,
                                    (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                                    0);
    // Set tabs
    SendMessage(hwndProtocolList, LB_SETTABSTOPS, 1, (LPARAM)tabwidth);

    // Create Client List listbox
    hwndClientList = CreateWindow("LISTBOX",
                                  NULL,
                                  WS_BORDER | WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_USETABSTOPS | LBS_HASSTRINGS,
                                  0, 0, 0, 0,
                                  hwnd,
                                  (HMENU)ID_CLIENTBOX,
                                  (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                                  0);
    // Set tab stops
    SendMessage(hwndClientList, LB_SETTABSTOPS, 1, (LPARAM)tabwidth);

    // First allocate Connected Sockets Heap
    if((ConnectHeap = HeapCreate(0, sizeof(SOCKDATA) * 5, sizeof(SOCKDATA) * 100)) == NULL)
    {
        // ERROR - abort
		return 0;
    }
    if(((LPVOID)ConnectedSockets = HeapAlloc(ConnectHeap,0, sizeof(SOCKDATA) * 5)) == NULL)
    {
        // ERROR - abort
        return 0;
    }
    NextFree = 0;  // First available SOCKDATA struct
    MaxConnects = 5; // Allocate only 5 structures at a time

    // Load winsock dll
    wVersionRequested = MAKEWORD(1, 1);
    if (WSAStartup(wVersionRequested, &wsaData) != 0)
    {
        // ERROR -abort
		return 0;
    }

    // Determine size of buffer needed
    if (EnumProtocols(NULL, NULL, &sizeProtBuf) != SOCKET_ERROR)
    {
        // ERROR
        return 0;
    }

    // Allocate buffer of appropriate size
    if (((LPVOID)lpProtBuf = VirtualAlloc(NULL,
                                          sizeProtBuf,
                                          MEM_COMMIT,
                                          PAGE_READWRITE)) == NULL)
    {
        // ERROR
        return 0;
    }

    // Do the real EnumProtocols call--note that there is no GETNEXT mechanism here.
	// We have to allocate a single buffer big enough to hold everything.
    if ((cNumProt = EnumProtocols(NULL, lpProtBuf, &sizeProtBuf)) == SOCKET_ERROR)
    {
        // ERROR
        return 0;
    }

    // Now that we know how many protocols we have, allocate array of globchat
	// SOCKDATA structures to hold info on each.
    if (((LPVOID)ServerSockets = VirtualAlloc(NULL,
                                              cNumProt * sizeof(SOCKDATA),
                                              MEM_COMMIT,
                                              PAGE_READWRITE)) == NULL)
    {
        // ERROR
        return 0;
    }

    // Walk through available protocols, j is index to list of all protocols, k is index to list of
	// protocols we can use.
    for (j = 0, k = 0; j < cNumProt; j++)
    {
        // Only want connection oriented protocols
        if((lpProtBuf[j].dwServiceFlags & XP_CONNECTIONLESS) == 0)  
        {
			// Attempt to setup listen and fill in SOCKDATA struct
			if (MakeServSock(hwnd, &ServerSockets[k], &lpProtBuf[j]))
			{
	            // Successfully using protocol...add it to our list box
	            wsprintf(outtext, "%s\t%d", ServerSockets[k].lpProtocolName, ServerSockets[k].currconnects);
	            SendMessage(hwndProtocolList, LB_ADDSTRING, 0, (LPARAM)&outtext);

	            // Found a good protocol!
	            GoodProts[k++] = j;
			}
        }
    }

    cNumProt = k;  // Number of good protocols




    // Register with Name Spaces

    // In the future this will be done transparently, but for now must do it
    // manually for specific protocols.  The only NSPs we have are DNS and SAP.
    // Since DNS is static, that just leaves us with SAP to do.

    
    if((SAPSocket = socket(AF_IPX,                // IPX family
                           SOCK_DGRAM, 			  // SAPs are broadcasted
                           NSPROTO_IPX + 4)		  // Uses IPX packet type 4
                           ) != INVALID_SOCKET)
    {
        // Find IPX address which we are bound to
        for(k = 0;k < cNumProt; k++)
        {
            if(ServerSockets[k].protocol == NSPROTO_SPXII)
            {
		        // Initialize SAP data
		        SAPData.operation = 0x200;         // 2 = SAP response(network order)
		        SAPData.servicetype = 0x5607;  // 0756 Assigned by Novell specifically for GLOBCHAT only!!!!
		        strcpy(SAPData.name, "GLOBCHAT");  // My service name
		        memcpy((void *)SAPData.network,    // Copy address into SAP body
		               (void *)(ServerSockets[k].addr.sa_data), 
		               12);
		        SAPData.hops = 0;                 // Hop count starts at zero

		        SAPSockAddr.sa_family = AF_IPX;   // SAP packets sent over IPX
		        AtoH((char *)&SAPSockAddr.sa_socket, "0452", 2);  // SAP destination socket address is 0452.
	        
				// Bind to SAP socket
		        if (bind(SAPSocket, (PSOCKADDR) &SAPSockAddr, sizeof(SOCKADDR_IPX)) == SOCKET_ERROR)
		        {
		            // Error binding to SAP socket
					return 0;
		        }
		        else
		        {
		            // build destination address for SAP's IPX header (broadcast)
		            SAPDestSockAddr.sa_family = AF_IPX;
		            AtoH("00000000",SAPDestSockAddr.sa_netnum, 4);
		            AtoH("FFFFFFFFFFFF", SAPDestSockAddr.sa_nodenum, 6);
		            AtoH("0452", (char *)&SAPDestSockAddr.sa_socket, 2);

	                // Setup socket to allow broadcasts
	                bcaststat = TRUE;
	                addrlen = sizeof(bcaststat);
	                setsockopt(SAPSocket, SOL_SOCKET, SO_BROADCAST, (char *)&bcaststat, sizeof(bcaststat));
		            sendto(SAPSocket, (char *)&SAPData, sizeof(SAPData), MSG_DONTROUTE, (struct sockaddr *)&SAPDestSockAddr, sizeof(SAPDestSockAddr));

		            // Set timer to remind us to send SAP packet every 60 seconds.
		            SetTimer(hwnd, SAPTIMER, 60000, NULL);
		        }
                break;
            }
        }
    }
    return 0;
}

//
//  FUNCTION: MsgSize(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Resizes child listboxes to fit nicely within parent
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number (Unused)
//    wparam    - Extra data	 (Unused)
//    lparam    - Window dimensions
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgSize(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    MoveWindow(hwndProtocolList,
               1, (HIWORD(lparam) / 4),
               (LOWORD(lparam) / 2),
               (HIWORD(lparam) * 3) / 4,
               TRUE);

    MoveWindow(hwndClientList,
               LOWORD(lparam) /2,
               HIWORD(lparam) / 4,
               LOWORD(lparam) / 2,
               (HIWORD(lparam) * 3) / 4,
               TRUE);
    return 0;
}

//
//  FUNCTION: MsgTimer(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Sends out SAP packet every 60 seconds
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number (Unused)
//    wparam    - Should be = SAPTIMER
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgTimer(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    sendto(SAPSocket, (char *) &SAPData, sizeof(SAPData), MSG_DONTROUTE, (struct sockaddr *)&SAPDestSockAddr, sizeof(SAPDestSockAddr));
    return 0;
}

//
//  FUNCTION: MsgConnected(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handles new connection notifications
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number (Unused)
//    wparam    - socket which has the connection
//    lparam    - loword = event, hiword = error code
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgConnected(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int k, addrlen;
    int index;
    char outtext[80];

    // May have to increase our heap size
    if(NextFree >= MaxConnects)
    {
        if(HeapAlloc(ConnectHeap, 0, sizeof(SOCKDATA) * 5) == NULL)
        {
           // ERROR on heap allocation...don't accept new connections
		   return 0;
        }
        MaxConnects += 5;  // We've got five more connect entries available
    }

    // find socket that connect is refering to
    k = 0;
    while(ServerSockets[k].sock != wparam) k++;

    // Lets keep track of our progress
    ServerSockets[k].status = SOCKSTAT_ACCEPTING;

    // Some address our bigger than a sockaddr struct.  I threw in some reserved bytes to cover it.
    addrlen = sizeof(ConnectedSockets[NextFree].addr) + sizeof(ConnectedSockets[NextFree].reserved);

    // Accept the connect request
    if ((ConnectedSockets[NextFree].sock = accept(ServerSockets[k].sock,
                                                 &ConnectedSockets[NextFree].addr,
                                                 &addrlen)) == INVALID_SOCKET)
    {
        // ERROR clean up ConnectedSockets (that is don't increment NextFree) and go on our way
        return 0;
    }
    if(WSAAsyncSelect(ConnectedSockets[NextFree].sock, hwnd, MW_DATAREADY, FD_READ | FD_CLOSE) == SOCKET_ERROR)
    {
        // ERROR clean up connection
		closesocket(ConnectedSockets[NextFree].sock);
        return 0;
    }
    // Fill in SOCKDATA structure for client socket
    ConnectedSockets[NextFree].protocol = ServerSockets[k].protocol;
    ConnectedSockets[NextFree].type = ServerSockets[k].type;
    ConnectedSockets[NextFree].servsockindex = k;
    ConnectedSockets[NextFree++].status = SOCKSTAT_CONNECTED;
	// Increment protocol connection count and display
    ServerSockets[k].currconnects ++;
    index = SendMessage(hwndProtocolList, LB_FINDSTRING, (WPARAM)-1, (LPARAM)ServerSockets[k].lpProtocolName);
    SendMessage(hwndProtocolList, LB_DELETESTRING, index, 0);
    wsprintf(outtext, "%s\t%d", ServerSockets[k].lpProtocolName, ServerSockets[k].currconnects);
    SendMessage(hwndProtocolList, LB_ADDSTRING, 0, (LPARAM)&outtext);

    // Don't forget to set server socket status back to listening
    ServerSockets[k].status = SOCKSTAT_LISTENING;

    return 0;
}

//
//  FUNCTION: MsgDataready(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handles incoming socket data notifications and close socket
//           notifications
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number
//    wparam    - socket which has the data
//    lparam    - loword = event, hiword = error code
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgDataready(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int k, j, l;
    int totalbytesread;
    int index;
    char outtext[80];

    // Find the appropriate socket...allow for the reuse of a closed socket handle
    for (k = 0; k < NextFree; k++)
    {
        if((ConnectedSockets[k].sock == wparam) && (ConnectedSockets[k].status != SOCKSTAT_CLOSED))
            break;
    }

    if(LOWORD(lparam) == FD_CLOSE)
    {
        // Socket closed notification--cleanup!
        closesocket(ConnectedSockets[k].sock);

        // Delete name from client list display
        UpdateClientList(ConnectedSockets[k].name, SOCKSTAT_CLOSED, NULL);

        // Take name off of other clients' available list
        if (ConnectedSockets[k].status == SOCKSTAT_AVAILABLE)
        {
            deregistername(ConnectedSockets[k].name);
        }

        // if this connection was in session with another peer...notify peer of disconnect

        // First, find peer
        for (j = 0; j < NextFree; j++)
        {
            if((ConnectedSockets[j].sock == ConnectedSockets[k].peer) &&
               (ConnectedSockets[j].status == SOCKSTAT_INSESSION))
            {
                break;
            }
        }
        if (j < NextFree)  // Did we find a peer?
        {
            // Yes, build message header
            xferbuf.hdr.signature = MYSIGNATURE;
            xferbuf.hdr.length = totalbytesread = HDRSIZE;
            xferbuf.hdr.command = SESSION_CLOSE;
            // Send session close message
            senddatamessage(ConnectedSockets[j].sock, &xferbuf);
			// Update Peer's displayed status
            ConnectedSockets[j].status = SOCKSTAT_AVAILABLE;
            UpdateClientList(ConnectedSockets[j].name, SOCKSTAT_AVAILABLE, NULL);
			// Propagate the fact that the peer is now available for other chats
			// and give peer list of other available clients
            for (l = 0; l < NextFree; l++)
            {
                if ((l != j) && (ConnectedSockets[l].status == SOCKSTAT_AVAILABLE))
                {
                    xferbuf.hdr.command = REGISTER_NAME;
                    lstrcpy(xferbuf.data, ConnectedSockets[l].name);
                    xferbuf.hdr.length = REALLEN(ConnectedSockets[l].name) + HDRSIZE;
                    senddatamessage(ConnectedSockets[j].sock, &xferbuf);

                    lstrcpy(xferbuf.data, ConnectedSockets[j].name);
                    xferbuf.hdr.length = REALLEN(ConnectedSockets[j].name) + HDRSIZE;
                    senddatamessage(ConnectedSockets[l].sock, &xferbuf);
                }
            }
        }
        // Cleanup ConnectedSockets array
        ConnectedSockets[k].status = SOCKSTAT_CLOSED;
        j = ConnectedSockets[k].servsockindex;
	    // Fix protocol connection count display
	    ServerSockets[j].currconnects --;
	    index = SendMessage(hwndProtocolList, LB_FINDSTRING, (WPARAM)-1, (LPARAM)ServerSockets[j].lpProtocolName);
	    SendMessage(hwndProtocolList, LB_DELETESTRING, index, 0);
	    wsprintf(outtext, "%s\t%d", ServerSockets[j].lpProtocolName, ServerSockets[j].currconnects);
	    SendMessage(hwndProtocolList, LB_ADDSTRING, 0, (LPARAM)&outtext);
        return 0;
    }

    // There's data to read...read it!

    if(!recvdatamessage(&ConnectedSockets[k], &xferbuf))
    {
        return 0;
    }

    // We've got our whole message!  Now switch() on the command flag

    switch(xferbuf.hdr.command)
    {
        case REGISTER_NAME:  // First message we should receive on a connection
            if(ConnectedSockets[k].status != SOCKSTAT_CONNECTED)
            {
                // ERROR -- we weren't expecting this...drop it
                return 0;
            }
            // Get name and add to internal structs and display
            lstrcpy(ConnectedSockets[k].name, xferbuf.data);
            ConnectedSockets[k].status = SOCKSTAT_AVAILABLE;
            UpdateClientList(ConnectedSockets[k].name, SOCKSTAT_AVAILABLE, NULL);

            // Send notification to other "AVAILABLE" sockets that we have a new peer available
            for (j = 0; j < NextFree; j++)
            {
                if((j != k) && (ConnectedSockets[j].status == SOCKSTAT_AVAILABLE))
                {
                    // message in xferbuf should be able to be sent just like it is
                    senddatamessage(ConnectedSockets[j].sock, &xferbuf);
                }
            }

            // Send notifications back to registering peer of all the currently available peers

            for (j = 0; j < NextFree; j++)
            {
                if((j != k) && (ConnectedSockets[j].status == SOCKSTAT_AVAILABLE))
                {
                    // found one...build message and send it
                    xferbuf.hdr.signature = MYSIGNATURE;
                    xferbuf.hdr.length = REALLEN(ConnectedSockets[j].name) + HDRSIZE;
                    xferbuf.hdr.command = REGISTER_NAME;
                    lstrcpy(xferbuf.data, ConnectedSockets[j].name);
                    senddatamessage(ConnectedSockets[k].sock, &xferbuf);
                }
            }

            return 0;

        case XFER_DATA:   // For passing data between two insession peers
            if(ConnectedSockets[k].status != SOCKSTAT_INSESSION)
            {
                // ERROR -- we weren't expecting this...drop data
                return 0;
            }

            // forward the message to peer...should be able transfer
            // message without modification
            senddatamessage(ConnectedSockets[k].peer, &xferbuf);
            return 0;

        case REQUEST_SESSION:  // Client is asking another peer for a chat
            if(ConnectedSockets[k].status != SOCKSTAT_AVAILABLE)
            {
                // ERROR -- we weren't expecting this...drop data
                return 0;
            }

            ConnectedSockets[k].status = SOCKSTAT_REQSESSION;
            UpdateClientList(ConnectedSockets[k].name, SOCKSTAT_REQSESSION, xferbuf.data);

            // Find the socket which corresponds to the name

            for(j = 0; j < NextFree; j++)
            {
                if(lstrcmp(ConnectedSockets[j].name, xferbuf.data) == 0)
                {
                    if(ConnectedSockets[j].status == SOCKSTAT_AVAILABLE)
                    {
                        // Found It!
                        break;
                    }
                }
            }

            if (j == NextFree) return 0;

            // Copy requester's name into send data buffer
            lstrcpy(xferbuf.data, ConnectedSockets[k].name);
            xferbuf.hdr.length = HDRSIZE + REALLEN(ConnectedSockets[k].name);
            senddatamessage(ConnectedSockets[j].sock, &xferbuf);

            // Update connected sockets structures
            ConnectedSockets[j].status = SOCKSTAT_REQSESSION;
            UpdateClientList(ConnectedSockets[j].name, SOCKSTAT_REQSESSION, ConnectedSockets[k].name);
            ConnectedSockets[j].peer = ConnectedSockets[k].sock;
            ConnectedSockets[k].peer = ConnectedSockets[j].sock;
            return 0;

        case SESSION_REQUEST_RESPONSE:  // Response to session request

            if(ConnectedSockets[k].status != SOCKSTAT_REQSESSION)
            {
                // ERROR -- not expecting this...drop packet
                return 0;
            }

            // find peer entry
            for (j = 0; j < NextFree; j++)
            {
                if ((ConnectedSockets[j].sock == ConnectedSockets[k].peer) &&
                    (ConnectedSockets[j].status == SOCKSTAT_REQSESSION))
                {
                    // Found it!
                    break;
                }
            }

            if (j == NextFree)
            {
                // ERROR -- couldn't find peer...drop packet
                return 0;
            }

            // forward response to requester
            senddatamessage(ConnectedSockets[k].peer, &xferbuf);

            if(*(xferbuf.data) == 1)
            {
                // Session accepted, change status of sockets
                ConnectedSockets[k].status = SOCKSTAT_INSESSION;
                ConnectedSockets[j].status = SOCKSTAT_INSESSION;
                UpdateClientList(ConnectedSockets[k].name, SOCKSTAT_INSESSION, ConnectedSockets[j].name);
                UpdateClientList(ConnectedSockets[j].name, SOCKSTAT_INSESSION, ConnectedSockets[k].name);
                deregistername(ConnectedSockets[k].name);
                deregistername(ConnectedSockets[j].name);
            }
            else
            {
                // Session not accepted, make sockets available
                ConnectedSockets[k].status = SOCKSTAT_AVAILABLE;
                ConnectedSockets[j].status = SOCKSTAT_AVAILABLE;
                UpdateClientList(ConnectedSockets[k].name, SOCKSTAT_AVAILABLE, NULL);
                UpdateClientList(ConnectedSockets[j].name, SOCKSTAT_AVAILABLE, NULL);
            }

            return 0;

        case SESSION_CLOSE:  // Insession client chose "End Chat" option
            if (ConnectedSockets[k].status != SOCKSTAT_INSESSION)
            {
                // We weren't expecting this...drop packet
                return 0;
            }

            // Find Peer
            for (j = 0; j < NextFree; j++)
            {
                if((ConnectedSockets[j].sock == ConnectedSockets[k].peer) &&
                   (ConnectedSockets[j].status == SOCKSTAT_INSESSION))
                {
                    // Found it
                    break;
                }
            }

            if(j == NextFree)
            {
                // ERROR - couldn't find peer...drop message
                return 0;
            }

            // forward message
            senddatamessage(ConnectedSockets[k].peer,&xferbuf);

            // Change Status
            ConnectedSockets[k].status = SOCKSTAT_AVAILABLE;
            ConnectedSockets[j].status = SOCKSTAT_AVAILABLE;
            UpdateClientList(ConnectedSockets[k].name, SOCKSTAT_AVAILABLE, NULL);
            UpdateClientList(ConnectedSockets[j].name, SOCKSTAT_AVAILABLE, NULL);

            // register names of both peers with other available clients.  Also
			// provide current available client names to both peers

            for (l = 0; l < NextFree; l++)  // l = index to all connected sockets
			                                // k = index of current peer
											// j = index of socket who generated message
            {
                if (ConnectedSockets[l].status == SOCKSTAT_AVAILABLE)
                {
                    if ( l != k)
                    {
                        xferbuf.hdr.signature = MYSIGNATURE;
                        xferbuf.hdr.length = REALLEN(ConnectedSockets[k].name) + HDRSIZE;
                        xferbuf.hdr.command = REGISTER_NAME;
                        lstrcpy(xferbuf.data, ConnectedSockets[k].name);
                        senddatamessage(ConnectedSockets[l].sock, &xferbuf);

                        if (l != j)
                        {
                            xferbuf.hdr.length = REALLEN(ConnectedSockets[l].name) + HDRSIZE;
                            lstrcpy(xferbuf.data, ConnectedSockets[l].name);
                            senddatamessage(ConnectedSockets[k].sock, &xferbuf);
                        }
                    }
                    if (l != j)
                    {
                        xferbuf.hdr.signature = MYSIGNATURE;
                        xferbuf.hdr.length = REALLEN(ConnectedSockets[j].name) + HDRSIZE;
                        xferbuf.hdr.command = REGISTER_NAME;
                        lstrcpy(xferbuf.data, ConnectedSockets[j].name);
                        senddatamessage(ConnectedSockets[l].sock, &xferbuf);

                        if (l != k)
                        {
                            xferbuf.hdr.length = REALLEN(ConnectedSockets[l].name) + HDRSIZE;
                            lstrcpy(xferbuf.data, ConnectedSockets[l].name);
                            senddatamessage(ConnectedSockets[j].sock, &xferbuf);
                        }
                    }
                }
            }
            return 0;

    } // End message command switch

    // ERROR -- If we get here, we had an invalid message... drop it
    return 0;
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
