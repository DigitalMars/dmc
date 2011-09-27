// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993, 1994  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   misc.c
//
//  PURPOSE:  Contains all helper functions "global" to the application.
//
//  FUNCTIONS:
//    CenterWindow - Center one window over another.
//    AtoH - Converts ascii to network order hex
//    BtoH - Converts ascii byte to numeric
//    deregistername - Notifies "Available" peers of clients non-availability
//    senddatamessage - Sends a message to a client node
//    recvdatamessage - reads an entire message
//    UpdateClientList - Updates Client Status Listbox
//    MakeServSock - Attempts to initialize socket for listening
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <winsock.h>
#include <nspapi.h>
#include <svcguid.h>
#include <wsipx.h>
#include <wsnetbs.h>
#include "globals.h"            // prototypes specific to this application



//
//  FUNCTION: CenterWindow(HWND, HWND)
//
//  PURPOSE:  Center one window over another.
//
//  PARAMETERS:
//    hwndChild - The handle of the window to be centered.
//    hwndParent- The handle of the window to center on.
//
//  RETURN VALUE:
//
//    TRUE  - Success
//    FALSE - Failure
//
//  COMMENTS:
//
//    Dialog boxes take on the screen position that they were designed
//    at, which is not always appropriate. Centering the dialog over a
//    particular window usually results in a better position.
//

BOOL CenterWindow(HWND hwndChild, HWND hwndParent)
{
    RECT    rcChild, rcParent;
    int     cxChild, cyChild, cxParent, cyParent;
    int     cxScreen, cyScreen, xNew, yNew;
    HDC     hdc;

    // Get the Height and Width of the child window
    GetWindowRect(hwndChild, &rcChild);
    cxChild = rcChild.right - rcChild.left;
    cyChild = rcChild.bottom - rcChild.top;

    // Get the Height and Width of the parent window
    GetWindowRect(hwndParent, &rcParent);
    cxParent = rcParent.right - rcParent.left;
    cyParent = rcParent.bottom - rcParent.top;

    // Get the display limits
    hdc = GetDC(hwndChild);
    cxScreen = GetDeviceCaps(hdc, HORZRES);
    cyScreen = GetDeviceCaps(hdc, VERTRES);
    ReleaseDC(hwndChild, hdc);

    // Calculate new X position, then adjust for screen
    xNew = rcParent.left + ((cxParent - cxChild) / 2);
    if (xNew < 0)
    {
        xNew = 0;
    }
    else if ((xNew + cxChild) > cxScreen)
    {
        xNew = cxScreen - cxChild;
    }

    // Calculate new Y position, then adjust for screen
    yNew = rcParent.top  + ((cyParent - cyChild) / 2);
    if (yNew < 0)
    {
        yNew = 0;
    }
    else if ((yNew + cyChild) > cyScreen)
    {
        yNew = cyScreen - cyChild;
    }

    // Set it, and return
    return SetWindowPos(hwndChild,
                        NULL,
                        xNew, yNew,
                        0, 0,
                        SWP_NOSIZE | SWP_NOZORDER);
}

//
//  FUNCTION: AtoH(char *, char *, int)
//
//  PURPOSE:  Converts ascii string to network order hex
//
//  PARAMETERS:
//    src    - pointer to input ascii string
//    dest   - pointer to output hex
//    destlen - size of dest
//
//  COMMENTS:
//
//    2 ascii bytes make a hex byte so must put 1st ascii byte of pair
//    into upper nibble and 2nd ascii byte of pair into lower nibble.
//

void AtoH(char * src, char * dest, int destlen)
{
    char * srcptr;

    srcptr = src;

    while(destlen--)
    {
    *dest = BtoH(*srcptr++) << 4;    // Put 1st ascii byte in upper nibble.
    *dest++ += BtoH(*srcptr++);      // Add 2nd ascii byte to above.
    }
}

//
//  FUNCTION: BtoH(char *, char *, int)
//
//  PURPOSE:  Converts ascii byte to numeric
//
//  PARAMETERS:
//    ch - ascii byte to convert
//
//  RETURNS:
//    associated numeric value
//
//  COMMENTS:
//
//    Will convert any hex ascii digit to its numeric counterpart.
//    Puts in 0xff if not a valid hex digit.
//

unsigned char BtoH(char ch)
{
    if (ch >= '0' && ch <= '9') return (ch - '0');        // Handle numerals
    if (ch >= 'A' && ch <= 'F') return (ch - 'A' + 0xA);  // Handle capitol hex digits
    if (ch >= 'a' && ch <= 'f') return (ch - 'a' + 0xA);  // Handle small hex digits
    return(255);
}

//
//  FUNCTION: deregistername(char *)
//
//  PURPOSE:  Removes client's availability from other "AVAILABLE" clients
//
//  PARAMETERS:
//    name - name to deregister
//
//  RETURNS:
//    nothing
//
//  COMMENTS:
//
//    Between the time when a client connects to the server and when it starts
//    a chat, the client is in "AVAILABLE" state.  It keeps of list of other
//    available peers.  deregistername removes a previously available client
//    from the lists of all the other available clients
//

void deregistername(char * name)
{
    int j;

    // Set up message header
    xferbuf.hdr.signature = MYSIGNATURE;
    xferbuf.hdr.length = REALLEN(name) + HDRSIZE;
    xferbuf.hdr.command = DEREGISTER_NAME;
    // Message data is simply the name
    lstrcpy(xferbuf.data, name);

    // Send to every connected client whose status is SOCKET_AVAILABLE
    for (j = 0; j < NextFree; j++)
    {
        if (ConnectedSockets[j].status == SOCKSTAT_AVAILABLE)
        {
            senddatamessage(ConnectedSockets[j].sock, &xferbuf);
        }
    }
    return;
}

//
//  FUNCTION: senddatamessage(SOCKET, LPDATAMSG)
//
//  PURPOSE:  Sends a message to a client
//
//  PARAMETERS:
//    sock - Client socket to send data on
//    sendbuf - message to send
//
//  RETURNS:
//    TRUE - message sent
//
//  COMMENTS:
//
//    Performs send() until entire message is sent
//

BOOL senddatamessage (SOCKET sock, LPDATAMSG sendbuf)
{
    int totalbytes, bytessent;
	int thissendsize;

    bytessent = 0;                    // start count of bytessent
    totalbytes = sendbuf->hdr.length; // read total bytes from message header
    while(bytessent < totalbytes)
    {
        // We may not be able to send the entire message with a single
		// send.  Therefore continue sending until the whole thing
		// is gone.
        if ((thissendsize = send(sock,
                                (char *)sendbuf + bytessent,
                                totalbytes - bytessent,
                                0)) == SOCKET_ERROR)
		{
		     // Error
			 return FALSE;
	    }
        bytessent += thissendsize;
    }

    return TRUE;
}

//
//  FUNCTION: recvdatamessage(LPSOCKDATA, LPDATAMSG)
//
//  PURPOSE:  Receives a message from a client
//
//  PARAMETERS:
//    lpSockdat - pointer to sockdata struct of socket we are receiving data on.
//    recvbuf - buffer to receive data into.
//
//  RETURNS:
//    TRUE - message received
//    FALSE - message not received
//
//  COMMENTS:
//    Performs recv() until entire message is received
//

BOOL recvdatamessage (LPSOCKDATA lpSockdat, LPDATAMSG recvbuf)
{
    int readsize, totalbytesread, msgsize, bytesread;

    if (lpSockdat->type == SOCK_SEQPACKET)
    {
        // Message mode protocol!!  Just post one big read.
        readsize = sizeof(*recvbuf);
    }
    else
    {
        // Stream mode protocol!!  Just read header...then read data (data size determined
        // from header)
        readsize = HDRSIZE;
    }
    if((totalbytesread = recv(lpSockdat->sock, (char *)recvbuf, readsize, 0)) == SOCKET_ERROR)
    {
        // ERROR  -- just return dropping message
        return FALSE;
    }

    // Check for my signature at the beginning of the message
    if(recvbuf->hdr.signature != MYSIGNATURE)
    {
        // I've received some data that's in mid message--drop it
        return FALSE;
    }

    // Read size of message
    msgsize = recvbuf->hdr.length;
    readsize = msgsize - totalbytesread;

    while(totalbytesread < msgsize)
    {
        // we should only get hear for stream sockets
        if((bytesread = recv(lpSockdat->sock,
                             (char *)recvbuf + totalbytesread,
                             readsize,
                             0)) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                // ERROR -- throw out message
                return FALSE;
            }
            // If you got a WSAWOULDBLOCK error, just keep trying...it shouldn't take
            // too much longer for the rest of the message to get here.  Let's hope
            // we don't hog the CPU so the data doesn't get to us.
        }
        totalbytesread += bytesread;
        readsize -= bytesread;
    }

    return TRUE;
}

//
//  FUNCTION: UpdateClientList(char *, int, char *)
//
//  PURPOSE:  Updates Client Status List box
//
//  PARAMETERS:
//    name - name of client to update
//    status - status of client to display
//    peername - connected peer for status = IN_SESSION
//
//  RETURNS:
//    nothing
//
//  COMMENTS:
//    Finds and updates client status in listbox, or adds
//    it if client is new
//


void UpdateClientList(char * name, int status, char * peername)
{
    int index;
    char outtext[80];

    // Find list box entry with name in it
    index = SendMessage(hwndClientList,
                        LB_FINDSTRING,
                        (WPARAM)-1,
                        (LPARAM)name);

    // Delete the entry--we'll add a new entry below
    SendMessage(hwndClientList, LB_DELETESTRING, index, 0);

    switch (status)
    {
        case SOCKSTAT_CLOSED:  // No new entry required.  Just return.
            return;

        case SOCKSTAT_AVAILABLE:  // build available entry
	    wsprintf(outtext, "%s\tAVAILABLE", name);
	    break;

        case SOCKSTAT_INSESSION:  // build in_session entry
            wsprintf(outtext, "%s\tIN SESSION\t%s", name, peername);
            break;

        case SOCKSTAT_REQSESSION: // build reqsession entry
            wsprintf(outtext, "%s\tSESSION SETUP\t%s", name, peername);
    }
	// Add the new list box entry to the client status list box
    SendMessage(hwndClientList, LB_ADDSTRING, 0, (LPARAM)&outtext);
}

//
//  FUNCTION: MakeServSock(HWND, LPSOCKDATA, LPPROTOCOL_INFO)
//
//  PURPOSE:  Attempts to initialize server side socket for listening
//
//  PARAMETERS:
//    hwnd - handle to main window
//    ServSocks - SOCKDATA struct to hold socket specific information
//    lpProtBuf - PROTOCOL_INFO struct describing protocol to 
//                attempt listen on
//
//  RETURNS:
//    TRUE - Listen successfully posted
//    FALSE - Initialization failed
//
//  COMMENTS:
//    Given the protocol triple in the PROTOCOL_INFO struct, we call
//    socket(), then GetAddressByName(), then bind(), listen(), and 
//    finally WSAAsyncSelect() to get socket it appropriate state
//    for listening.  If we fail at any point, then cleanup and do
//    not use protocol
//

BOOL MakeServSock(HWND hwnd, LPSOCKDATA ServSocks, LPPROTOCOL_INFO lpProtBuf)
{
	CSADDR_INFO CSABuf[10];
	DWORD dwCSABufsize = sizeof(CSABuf);
	GUID guidNW =  SVCID_NETWARE(NWCHATID);  // Macro generates GUID using hard coded NetWare Service Type
	GUID guidDNS = SVCID_TCP(DNSCHATID);	 // Macro generates GUID using hard coded port
	LPSOCKADDR_NB lpNBaddr;

	// open socket using protocol defined by PROTOCOL_INFO structure
	if ((ServSocks->sock = socket(lpProtBuf->iAddressFamily,
	                                    lpProtBuf->iSocketType,
	                                    lpProtBuf->iProtocol)) == SOCKET_ERROR)
	{
	    // ERROR
		return FALSE;  // This will reuse the current ServSocks structure
	}

	// Populate SOCKDATA structure
	ServSocks->status = SOCKSTAT_INIT;
	ServSocks->type = lpProtBuf->iSocketType;
	ServSocks->protocol = lpProtBuf->iProtocol;
	ServSocks->currconnects = 0;
	ServSocks->lpProtocolName = lpProtBuf->lpProtocol;
	
	// ***********************************************************************
	//          
	//    Below we will be calling GetAddressByName with the RES_SERVICE
	//    option in order to find the necessary sockaddr structure to use 
	//    for the following bind() call.  At this point, GetAddressByName()
	//    only supports the DNS and the SAP/Bindery name spaces.  Ultimately
	//    a single call to GetAddressByName will return information on
	//    all available protocols, but this requires a central database
	//    which is currently not available.  In the mean time we will make
	//    name space specific calls to GetAddressByName.
	//
	// ***********************************************************************
	
	switch (lpProtBuf->iAddressFamily)
	{
	    case AF_IPX:
	        // Use SAP/Bindery Name Space
	        if (GetAddressByName(0,    // Since GUID is name space specific, we don't need to specify distinct name space.
	                             &guidNW,  // GUID defined by NetWare service type.
	                             "GLOBAL CHAT SERVER",  // This parameter is actually not used for RES_SERVICE calls.
	                             NULL,    // Since GUID implies the protocol, we don't need to specify a distinct one.
	                             RES_SERVICE,  // Specifies that we are just looking for local address to bind to.
	                             NULL,   // Currently not supported
	                             CSABuf, // Structure which will hold information for bind() call
	                             &dwCSABufsize,  // Size of CSABuf
	                             NULL,   //  Currently not supported
	                             NULL)   // Currently not supported
	                             < 1) 	 // We need at least one address returned in order to bind()
	        {
	            // Error--cleanup
				closesocket(ServSocks->sock);
				return FALSE;
	        }

			// Copy sockaddr info and addresslength
			memcpy(&ServSocks->addr, CSABuf[0].LocalAddr.lpSockaddr, sizeof(struct sockaddr));
			ServSocks->addrlen = CSABuf[0].LocalAddr.iSockaddrLength;
	        break;

		case AF_INET:
	        // Use DNS Name Space
	        if (GetAddressByName(0,   // Since GUID is name space specific, we don't need to specify
	                             &guidDNS,  // GUID defined by TCP port number
	                             "globchat",  // This parameter is actually not used for RES_SERVICE calls
	                             NULL,	  // GUID implies protocol so no need to specify
	                             RES_SERVICE,  // Specifies that we are trying to find local address to bind to
	                             NULL,  // Currently not supported
	                             CSABuf, // Results buffer
	                             &dwCSABufsize,	 // Size of results buffer
	                             NULL,	  // Not supported
	                             NULL)    // Not supported
	                             < 1)   // Need at least one address returned in order to bind()
	        {
	            // Error -- cleanup
				closesocket(ServSocks->sock);
				return FALSE;
	        }
			// Copy sockaddr info and address length
			memcpy(&ServSocks->addr, CSABuf[0].LocalAddr.lpSockaddr, sizeof(struct sockaddr));
			ServSocks->addrlen = CSABuf[0].LocalAddr.iSockaddrLength;
	        break;

	    case AF_NETBIOS:
	        // no netbios name space provider so we will simulate GetAddressByName() results
			lpNBaddr = (LPSOCKADDR_NB)&ServSocks->addr;
	        SET_NETBIOS_SOCKADDR(lpNBaddr,
	                             NETBIOS_GROUP_NAME,
	                             "GLOBSERV",
	                             0);
			ServSocks->addrlen = lpProtBuf->iMaxSockAddr;
	        break;
		
	}

	// We have an address to bind() to, so do it!
	if (bind(ServSocks->sock,
	         &ServSocks->addr,
		     ServSocks->addrlen) == SOCKET_ERROR)
	{
	    // Error -- cleanup
		closesocket(ServSocks->sock);
		return FALSE;
	}

	// Find out the specific local address
	if (getsockname(ServSocks->sock, 
	                &ServSocks->addr, 
	                &ServSocks->addrlen) == SOCKET_ERROR)
	{
	    // Error -- better cleanup
	    closesocket(ServSocks->sock);
		return FALSE;
	}

	// Listen on the socket
	if (listen(ServSocks->sock, 5) == SOCKET_ERROR)
	{
	    // Error -- cleanup
	    closesocket(ServSocks->sock);
		return FALSE;
	}

	// Set up window message to indicate connection
	if (WSAAsyncSelect(ServSocks->sock,
	                   hwnd,
		               MW_CONNECTED,
					   FD_ACCEPT) == SOCKET_ERROR)
	{
	    // Error -- cleanup
		closesocket(ServSocks->sock);
		return FALSE;
	}

	// Success!!!
	return TRUE;
}


