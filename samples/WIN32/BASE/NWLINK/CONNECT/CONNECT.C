/****************************************************************************\
*  dgrecv.c -- sample program demonstrating NWLink.
*
*       Microsoft Developer Support
*       Copyright (c) 1992, 1993 Microsoft Corporation
*
*  This program is a simple example of using SPX connect.
* 
****************************************************************************/
#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <wsipx.h>
#include "../testlib/testlib.h"

SOCKADDR_IPX addr;
SOCKADDR_IPX baddr;
SOCKADDR_IPX caddr;

UCHAR ch = 0;

/*
*   Function Prototypes 
*/

extern int main(int, char **);
extern int net_init(SOCKET *);
extern int make_connection(SOCKET);
extern int do_send_receive(SOCKET);

/****************************************************************************
*
*    FUNCTION:  main( int argc, char **argv )
*
*    PURPOSE:   This is the main entry for the program
*   		    
*
*    ARGUMENTS:	argc = Number of arguments
*               argv = Array of ptrs to cmd line args
*                
*
*	 RETURNS:   Exit code for the program
*				
*\***************************************************************************/
int main(int argc, char **argv)
{
    SOCKET s;

    /*
    *   Set up our default values before checking command line 
    */

    memcpy(Remote_Node_Number, "\x08\x00\x2B\x2E\x98\xA8", 6);
    memcpy(Remote_Socket_Number, "\x05\x00", 2);
    Socket_Type = SOCK_STREAM;
    Protocol = NSPROTO_SPX;
    Remote_Address_Family = AF_NS;
    Sleep_Time = 250;

    /*
    *   Get any command line options 
    */

    parse_cmd_line(argc, argv);

    /*
    *   Initialize the network and set up the socket 
    */

    if (net_init(&s))
        return 1;

    /*
    *   Try to connect to our server 
    */

    if (make_connection(s))
        return 1;

    /*
    *   Send/receive data to/from server 
    */

    do_send_receive(s);

    /*
    *   All done - close up 
    */

    if (verbose)
        printf("calling closesocket(socket = %d)\n", s);

    closesocket(s);
    return 0;
}

/****************************************************************************
*
*    FUNCTION:  net_init( SOCKET *skt )
*
*    PURPOSE:   Initializes the WinSock stuff and sets up our socket.
*   		    
*
*    ARGUMENTS:	SOCKET * => struct to fill in with opened socket.
*
*	 RETURNS:   0 if ok
*				1 if error
*
*\***************************************************************************/
int net_init(SOCKET *skt)
{
    SOCKET s;
    int rc, addrlen;
    WSADATA wsdata;
    WORD    wVersionRequested;

    wVersionRequested = MAKEWORD(1,1);


    /*
    *   Register with the socket library 
    */

    rc = WSAStartup(wVersionRequested, &wsdata);

    if (verbose)
        printf("WSAStartup returned 0x%X\n", rc);

    if (rc) {
        printf("WSAStartup failed: error = %d\n", rc);
        return 1;
    }

    if (verbose) {
        printf("contents of wsdata structure: \n");
        print_wsa(&wsdata);
        printf("calling socket(address family = %d, socket type = %d, protocol = %d)\n", Local_Address_Family, Socket_Type, Protocol);
    }

    /*
    *   Open a STREAMING socket 
    */

    s = socket(Local_Address_Family, Socket_Type, Protocol);

    if (verbose)
        printf("socket() returned 0x%X (%d)\n", s, s);

    if (s == INVALID_SOCKET) {
        dos_net_perror("socket call failed");
        return 1;
    }

    /*
    *   Bind to any address 
    */

    addr.sa_family = Local_Address_Family;
    memcpy(&addr.sa_netnum, Local_Network_Number, 4);
    memcpy(&addr.sa_nodenum, Local_Node_Number, 6);
    memcpy(&addr.sa_socket, Local_Socket_Number, 2);

    rc = bind(s, (const struct sockaddr FAR *)&addr, 16);

    if (verbose)
        printf("bind() returned 0x%X\n", rc);

    if (rc == SOCKET_ERROR) {
        dos_net_perror("bind call failed");
        return 1;
    }

    if (verbose)
        printf("calling getsockname(socket = %d)\n", s);

    addrlen = 16;
    rc = getsockname(s, (struct sockaddr *) &addr, &addrlen);

    if (verbose)
        printf("getsockname() returned 0x%X\n", rc);

    if (rc == SOCKET_ERROR) {
        dos_net_perror("Error binding to socket");
        closesocket(s);
        return 1;
    }

    if (verbose) {
        printf("addrlen = %d\n", addrlen);
        print_netaddr(addr.sa_netnum, "Bound address = ", "\n");
    }


    /*
    *   Build the address of the node to connect to 
    */

    memcpy(&caddr.sa_netnum, Remote_Network_Number, 4);
    memcpy(&caddr.sa_nodenum, Remote_Node_Number, 6);
    memcpy(&caddr.sa_socket, Remote_Socket_Number, 2);
    caddr.sa_family = AF_NS;

    /*
    *   Set up socket for return 
    */

    *skt = s;

    return 0;
}

/****************************************************************************
*
*    FUNCTION:  make_connection( SOCKET s )
*
*    PURPOSE:   Establishes a connection with our server.
*
*    ARGUMENTS:	SOCKET socket to use for connection
*
*	 RETURNS:   0 if ok
*				1 if error
*
*\***************************************************************************/
int make_connection(SOCKET s)
{
    int rc, addrlen;

    /*
    *   Connect 
    */

    if (verbose)
        printf("calling connect(socket = %d)\n", s);

    rc = connect(s, (const struct sockaddr FAR *)&caddr, 16);

    if (verbose)
        printf("connect() returned 0x%X\n", rc);

    if (rc == SOCKET_ERROR) {
        dos_net_perror("connect call failed");
        return 1;
    }

    printf("Connect OK\n");

    /*
    *   Print out address we connected to 
    */

    if (verbose)
        printf("calling getpeername(socket = %d)\n", s);

    addrlen = 16;
    rc = getpeername(s, (struct sockaddr *) &caddr, &addrlen);

    if (verbose)
        printf("getpeername() returned 0x%X\n", rc);

    if (verbose) {
        printf("addrlen = %d\n", addrlen);
        print_netaddr(caddr.sa_netnum, "Remote Address = ", "\n");
    }

    return 0;
}
/****************************************************************************
*
*    FUNCTION:  do_send_receive( SOCKET *s )
*
*    PURPOSE:   Alternately sends/receives data to/from the server.
*
*    ARGUMENTS:	SOCKET socket to transmit on
*
*	 RETURNS:   0 if ok
*				1 if error
*
*\***************************************************************************/
int do_send_receive(SOCKET s)
{
    int rc, errflag = 0;
    int sndpkts = 0, rcvpkts = 0;
    LPSTR sendbuf;

    /*
    *   Allocate a send buffer 
    */

    if (verbose)
        printf("Allocating %d bytes for send buffer \n");

    sendbuf = malloc(Send_Length);

    if (!sendbuf) {
        printf("Error allocating %d bytes for send buffer\n");
        return 1;
    }

    /*
    *   Send data and recv it back 
    */

    while (1) {

        /*
        *   Fill the buffer with our current character 
        */

        memset(sendbuf, ch, Send_Length);

        /*
        *   Send data 
        */

        if (verbose)
	    printf("calling send(socket = %d, length = %d)\n", s, Send_Length);

        rc = send(s, sendbuf, Send_Length, 0);

        if (verbose)
            printf("send() returned 0x%X\n", rc);

        if (rc == SOCKET_ERROR) {
            dos_net_perror("\nsend() call failed");
            errflag++;
            break;
        }

        if (verbose)
            printf("Sent packet %d: sent %d bytes\n", sndpkts++, rc);
        else
            printf("\rSent packet %d: sent %d bytes... ", sndpkts++, rc);

        /*
        *   Receive the data back 
        */

        if (verbose)
            printf("calling recv(socket = %d, length = %d)\n", s, Send_Length);

        rc = recv(s, sendbuf, Send_Length, 0);

        if (verbose)
            printf("recv() returned %d\n", rc);

        if (rc == SOCKET_ERROR) {
            dos_net_perror("\nrecv() call failed");
            errflag++;
            break;
        }

        if (!rc) {
            printf("connection has been lost\n");
            break;
        }

        printf("Received %d bytes", rc);

        if (verbose)
            printf("\n");

        /*
        *   See if our buffer has the same data in it 
        */

        rc = mem_check(sendbuf, ch++, Send_Length);

        if (rc)
            printf("Data compare error: packet = %d, offset = %d\n", (sndpkts-1), rc);
        else if (verbose)
            printf("Data compares okay\n");

        /*
        *   If we are to send just 1, break out 
        */

        if (No_Loop)
            break;

        /*
        *   Pause a little while (don't trash the network) 
        */

        Sleep(Sleep_Time);
    }

    if (verbose)
        printf("\nFreeing send buffer\n");

    free(sendbuf);

    return errflag;
}
