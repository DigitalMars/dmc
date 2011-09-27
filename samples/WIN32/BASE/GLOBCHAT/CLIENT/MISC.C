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
//    CenterWindow    - Center one window over another.
//    ReceiveInBox    - Reads incoming socket data.
//    SendOutBox      - Writes outgoing socket data.
//    AtoH            - Converts ascii string to network order hex
//    BtoH            - Converts ascii byte to hex
//    senddatamessage - Sends a message to the server
//    recvdatamessage - Receives a message from the server
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <wsipx.h>
#include <wsnetbs.h>
#include <nspapi.h>
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
//  FUNCTION: ReceiveInBox(HWND, WPARAM, LPARAM, char *, int)
//
//  PURPOSE:  Reads incoming data from socket and process message
//
//  PARAMETERS:
//    hWnd      - Handle to current window
//    uParam    - WPARAM (unused)
//    lParam    - LPARAM contains event (FD_READ or FD_CLOSE).
//    szRBuf    - Receive Buffer
//    cRBufLen  - size of Receive Buffer
//
//  RETURN VALUE:
//
//    TRUE  - Data Read
//    FALSE - If FD_CLOSE message or recv failed
//
//  COMMENTS:
//
//    Called if socket has data OR if it is closed.  If closed post
//    WM_DISCONNECTED message.  Else read data and make sure it is
//    NULL terminated.
//

BOOL ReceiveInBox(HWND hWnd, WPARAM uParam, LPARAM lParam, char * szRBuf, int cRBufLen)
{
    char outtext[80];


    if (LOWORD(lParam) == FD_CLOSE)                   // Is this a FD_CLOSE event?
    {
        SendMessage(hWnd, MW_DISCONNECTED, 0, 0);     // Yes, post message
        return(FALSE);
    }

    if (!recvdatamessage(&MySock, &xferbuf))         // Receive data
    {
        return(TRUE);
    }

    // Switch on message command
    switch (xferbuf.hdr.command)
    {
        case XFER_DATA:	  // Chat data...put it in buffer
            lstrcpy(szRBuf, xferbuf.data);
            cRBufLen = xferbuf.hdr.length - HDRSIZE;
            break;

        case SESSION_CLOSE:  // Peer killed us...cleanup
            MySock.status = SOCKSTAT_AVAILABLE;
            
            // Clear event indicator since we need to transfer message
			// indications to the dialog we are about to create
            if (WSAAsyncSelect(MySock.sock, hWnd, 0, 0) == SOCKET_ERROR)
            {
                // oops
                return FALSE;
            }
            PostMessage(hWnd, WM_COMMAND, MAKELONG(IDM_SELECT, 0), 0);
            wsprintf(outtext, "%s: available for chatting", MySock.name);
            SetWindowText(hWnd, outtext);
            return FALSE;

        default:
            break;
            // unexpected meesage...drop it
    }
    return TRUE;
}

//
//  FUNCTION: SendOutBox(char *, int)
//
//  PURPOSE:  Sends outbox text via data message
//
//  PARAMETERS:
//    szSBuf    - Send Buffer
//    cSBufLen  - size of Send Buffer
//
//  COMMENTS:
//
//    Writes send buffer to socket
//

void SendOutBox(char * szSBuf, int cSBufLen)
{
    xferbuf.hdr.signature = MYSIGNATURE;
    xferbuf.hdr.length = cSBufLen + HDRSIZE;
    xferbuf.hdr.command = XFER_DATA;
    lstrcpy(xferbuf.data, szSBuf);
    senddatamessage(MySock.sock, &xferbuf);
    return;
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
//  FUNCTION: senddatamessage(SOCKET, LPDATAMSG)
//
//  PURPOSE:  sends a chat message to the server
//
//  PARAMETERS:
//    sock    - our socket handle
//    sendbuf - the message to send
//
//  RETURNS:
//    ascii byte
//
//  COMMENTS:
//    Send() may not be able to send entire message continue to send until
//    entire message is sent
//

BOOL senddatamessage (SOCKET sock, LPDATAMSG sendbuf)
{
    int totalbytes, bytessent;

    bytessent = 0;                          // Set initial count to zero
    totalbytes = sendbuf->hdr.length;		// Set total bytes to send
    
	// loop until entire message is sent
    while(bytessent < totalbytes)
    {
        bytessent += send(sock,
                          (char *)sendbuf + bytessent,
                          totalbytes - bytessent,
                          0);
    }

    return TRUE;
}

//
//  FUNCTION: recvdatamessage(LPSOCKDATA, LPDATAMSG)
//
//  PURPOSE:  receives chat message from server
//
//  PARAMETERS:
//    lpSockdat - SOCKDATA structure
//    recvbuf   - buffer for incoming data
//
//  RETURNS:
//    TRUE  - Message received
//    FALSE - Message not received
//
//  COMMENTS:
//	  For stream sockets we will need to make sure that we read
//    an entire message--no more, no less.  Find message size from
//    message header.
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
        // ERROR  -- just return
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
