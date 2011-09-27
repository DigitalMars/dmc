/****************************************************************************\
*  dgrecv.c -- sample program demonstrating NWLink.
*
*       Microsoft Developer Support
*       Copyright (c) 1992, 1993 Microsoft Corporation
*
*  This program is a simple example of opening a socket,
*  binding to the socket, and waiting for a datagram packet.
* 
****************************************************************************/
#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <wsipx.h>
#include <wsnwlink.h>
#include "../testlib/testlib.h"

/*
*   Sockaddr structures 
*/

SOCKADDR_IPX addr;
SOCKADDR_IPX baddr;
SOCKADDR_IPX raddr;

/*
*   Function Prototypes 
*/

extern int main(int, char **);
extern int net_init(SOCKET *);
extern int dg_recv(SOCKET);

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
    *   Initialize our default values before checking the command line 
    */

    *Local_Socket_Number = 0x06;
    *(Local_Socket_Number+1) = 0x00;

    /*
    *   Parse the command line to set up any command line options 
    */

    parse_cmd_line(argc, argv);

    /*
    *   Initialize the network and set up our socket 
    */

    if (net_init(&s))
        return 1;

    /*
    *   Do the receive thing 
    */

    if (dg_recv(s))
        return 1;

    /*
    *   All Done - Close up the socket and exit 
    */

    if (verbose)
	printf("Calling closesocket()\n");

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
*    ARGUMENTS:	SOCKET * => struct to receive our socket info	
*
*	 RETURNS:   0 if ok
*				1 if error
*
*\***************************************************************************/
int net_init(SOCKET *skt)
{
    SOCKET s;
    int rc, addrlen = 16;
    WSADATA wsdata;
    WORD    wVersionRequested;

    if (verbose)
        printf("Calling WSAStartup(), ");

    /*
    *   Initialize with the WINSOCK library 
    */

    wVersionRequested = MAKEWORD(1,1);
    rc = WSAStartup(wVersionRequested, &wsdata);

    if (verbose)
        printf("return = 0x%X (%d)\n", rc, rc);

    if (rc) {
        printf("WSAStartup failed: error code = %d\n", rc);
        return 1;
    }

    if (verbose) {
        printf("Contents of wsadata struct:\n");
        print_wsa(&wsdata);
    }

    /*
    *   Open a DATAGRAM socket with IPX 
    */

    if (verbose)
        printf("Calling socket(address family = %d, socket type = %d, protocol = %d)\n", Local_Address_Family, Socket_Type, Protocol);

    s = socket(Local_Address_Family, Socket_Type, Protocol);

    if (verbose)
	printf("socket() returned 0x%X (%d)\n", s, s);

    if (s == INVALID_SOCKET) {
        dos_net_perror("Socket call failed");
        exit(1);
    }

    /*
    *    Bind to a socket.  We dont care what socket we bind to,
    *    so we will send down all 0's
    */

    addr.sa_family = Local_Address_Family;

    memcpy(&addr.sa_netnum, Local_Network_Number, 4);
    memcpy(&addr.sa_nodenum, Local_Node_Number, 6);
    memcpy(&addr.sa_socket, Local_Socket_Number, 2);

    if (verbose) {
        printf("calling bind(), local address =\n  ");
        print_saddr(&addr);
    }

    rc = bind(s, (struct sockaddr *) &addr, 16);

    if (verbose)
        printf("\nbind() returned 0x%X (%d)\n", rc, rc);

    if (rc == SOCKET_ERROR) {
        dos_net_perror("Error binding to socket");
        closesocket(s);
        return 1;
    }

    /*
    *   Get the address we bound to and print it out 
    */

    if (verbose)
        printf("calling getsockname(socket = %d), ", s);

    rc = getsockname(s, (struct sockaddr *) &baddr, &addrlen);

    if (verbose)
        printf("return = 0x%lX (%d)\n", rc, rc);

    if (rc == SOCKET_ERROR) {
        dos_net_perror("Error getting socket name");
        closesocket(s);
        return 1;
    }

    /*
    *   Set the packet type to send for this socket 
    */

    if (verbose)
        printf("Calling setsockopt for packet type %d\n", Local_Packet_Type);

    rc = setsockopt(s, NSPROTO_IPX, IPX_PTYPE, (const char *) &Local_Packet_Type, 4);

    if (rc == SOCKET_ERROR)
        dos_net_perror("setsockopt() call failed");


    if (Filter_Packet_Type) {

        /*
        *   Set the packet type for this socket 
        */

        if (verbose)
            printf("Calling setsockopt to filter packet type %d\n", Filter_Packet_Type);

	rc = setsockopt(s, NSPROTO_IPX, IPX_FILTERPTYPE, (const char *) &Filter_Packet_Type, sizeof(int));
	printf("RC from FILTER SOCKOPT = %d\n", rc);

        if (rc == SOCKET_ERROR)
            dos_net_perror("setsockopt() call failed setting filter packet type");
    }
    /*
    *   Print out the network address 
    */

    if (verbose) {
        printf("addrlen = %d\n", addrlen);
        print_netaddr(baddr.sa_netnum, "  Bound to address ", "\n");
    }

    *skt = s;

    return 0;
}

/****************************************************************************
*
*    FUNCTION:  dg_recv( SOCKET s )
*
*    PURPOSE:   Receives datagrams.
*
*    ARGUMENTS:	SOCKET   socket to receive on
*
*	 RETURNS:   0 if ok
*               1 if error
*
*\***************************************************************************/
int dg_recv(SOCKET s)
{
    int rc, errflag = 0;
    int addrlen = 16;
    UINT dgrams = 0;
    LPSTR recvbuf;


    if (verbose)
        printf("allocating %d bytes for receive buffer\n", Receive_Length);

    /*
    *   Set up the data buffer to send 
    */

    recvbuf = (LPSTR)malloc(Send_Length);

    if (!recvbuf) {
        printf("Error allocating %d bytes for receive buffer\n", Receive_Length);
        return 1;
    }

    if (verbose) {
        printf("calling recvfrom(socket = %d, length = %d),\n", s, Receive_Length);
    }
    else
        printf("Waiting for call...\n");

    while (1) {
        /*
        *   Receive a packet from anyone 
        */

        rc = recvfrom(s, recvbuf, Receive_Length, 0, (struct sockaddr *)&raddr, &addrlen);

        if (rc == SOCKET_ERROR) {
            dos_net_perror("recvfrom() failed");
            errflag++;
            break;
        }

        if (verbose) {
            printf("Received %d bytes from \n  ", rc);
            print_saddr(&raddr);
            printf("\n");
        }
        else
            printf("\rReceived packet %d, length = %d bytes", ++dgrams, rc);


        if (No_Loop)
            break;
    }

    if (verbose)
        printf("Freeing receive buffer\n");

    free(recvbuf);

    return errflag;
}
