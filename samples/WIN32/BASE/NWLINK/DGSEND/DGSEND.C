/****************************************************************************\
*  dgsend.c -- sample program demonstrating NWLink.
*
*       Microsoft Developer Support
*       Copyright (c) 1992, 1993 Microsoft Corporation
*
*  	This program is a simple example of opening a socket,
*  	binding to the socket, and sending a packet repeatedly.
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
#include "..\testlib\testlib.h"

/*
*   Sockaddr structures 
*/

SOCKADDR_IPX addr;
SOCKADDR_IPX baddr;
SOCKADDR_IPX saddr;

/*
*   Data for sending 
*/

char send_data[] = "This is an IPX packet from NT";

/*
*   Function Prototypes 
*/
extern int main(int, char **);
extern int net_init(SOCKET *);
extern int enable_broadcasts(SOCKET);
extern void build_dest_addr(SOCKET, PSOCKADDR_IPX);
extern int dg_send(SOCKET);

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
    *   Fill in any default values before checking the command line 
    */

    *Remote_Socket_Number = 0x06;
    *(Remote_Socket_Number+1) = 0x00;
    Sleep_Time = 500;

    /*
    *   Parse the command line to set up any command line options 
    */

    parse_cmd_line(argc, argv);

    if (net_init(&s))
        return 1;

    if (!No_Broadcast) {
        if (enable_broadcasts(s))
            return 1;
    }

    build_dest_addr(s, &saddr);

    /*
    *   Send some datagrams 
    */

    dg_send(s);

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
    WSADATA wsdata;
    WORD    wVersionRequested;


    int rc, addrlen = 16;

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
    *   Bind to a socket.  We dont care what socket we bind to,
    *   so we will send down all 0's
    */

    addr.sa_family = Local_Address_Family;

    memcpy(&addr.sa_netnum, Local_Network_Number, 4);
    memcpy(&addr.sa_nodenum, Local_Node_Number, 6);
    memcpy(&addr.sa_socket, Local_Socket_Number, 2);

    if (verbose) {
        printf("calling bind(), local address =\n  ");
        print_saddr(&addr);
    }

    rc = bind(s, (const struct sockaddr *) &addr, 16);

    if (verbose)
        printf("\nbind() returned 0x%X (%d)\n", rc, rc);

    if (rc == SOCKET_ERROR) {
        dos_net_perror("Error binding to socket");
        printf("Socket = 0x%lx\n", s);
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
    *   Set the packet type for this socket 
    */

    if (verbose)
        printf("Calling setsockopt for packet type %d\n", Local_Packet_Type);

    rc = setsockopt(s, NSPROTO_IPX, IPX_PTYPE, (const char *) &Local_Packet_Type, 4);

    if (rc == SOCKET_ERROR)
        dos_net_perror("setsockopt() call failed");

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
*    FUNCTION:  enable_broadcasts( SOCKET s )
*
*    PURPOSE:   Sets the socket option to enable broadcast sends on it.
*   		    
*
*    ARGUMENTS:	SOCKET   socket to enable	
*
*	 RETURNS:   0 if ok
*				1 if error
*
*\***************************************************************************/
int enable_broadcasts(SOCKET s)
{
    int rc;
    BOOL optval = TRUE;

    /*
    *   Enable sending of broadcasts 
    */

    /*
    *    NOTE:  This only needs to be done if you want to SEND
    *           broadcast packets.  Reception of broadcast
    *           packets will happen automatically.
    */

    if (verbose)
        printf("Setting socket option to broadcast, ");
	rc = setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char *)&optval, sizeof(int));
    if (verbose)
        printf("return = 0x%X (%d)\n", rc, rc);

    if (rc == SOCKET_ERROR) {
        dos_net_perror("Error enabling broadcast address");
        closesocket(s);
        return 1;
    }

    return 0;
}

/****************************************************************************
*
*    FUNCTION:  build_dest_addr( SOCKET s )
*
*    PURPOSE:   Fills in the destination address fields and sets the packet type.
*   		    
*
*    ARGUMENTS:	SOCKET       socket to set packet type for
*				PSOCKADDR_NS => address struct to fill in	
*
*	 RETURNS:   0 if ok
*				1 if error
*
*\***************************************************************************/
void build_dest_addr(SOCKET s, PSOCKADDR_IPX psaddr)
{
    int rc;

    /*
    *   Build the dest. address 
    */

    psaddr->sa_family = Remote_Address_Family;

    /*
    *   Set dest. network number 
    */

    memcpy(&psaddr->sa_netnum, Remote_Network_Number, 4);

    /*
    *   Set dest. node address 
    */

    memcpy(&psaddr->sa_nodenum, Remote_Node_Number, 6);

    /*
    *   Set the dest. socket number 
    */

    memcpy(&psaddr->sa_socket, Remote_Socket_Number, 2);


    /*
    *   Set the packet type for this socket 
    */

    if (verbose)
        printf("Calling setsockopt for packet type %d\n", Send_Packet_Type);

    rc = setsockopt(s, NSPROTO_IPX, IPX_PTYPE, (const char *) &Send_Packet_Type, sizeof(int));

    if (rc == SOCKET_ERROR)
        dos_net_perror("setsockopt() call failed");

    return;
}

/****************************************************************************
*
*    FUNCTION:  dg_send( SOCKET s )
*
*    PURPOSE:   Receives datagrams.
*
*    ARGUMENTS:	SOCKET socket to transmit on
*
*	 RETURNS:   0 if ok
*               1 if error
*
*\***************************************************************************/
int dg_send(SOCKET s)
{
    LPSTR sendbuf;
    int rc, errflag = 0;
    UINT dgrms = 0;

    if (verbose)
        printf("allocating %d bytes for send buffer\n", Send_Length);

    /*
    *   Set up the data buffer to send 
    */

    sendbuf = (LPSTR)malloc(Send_Length);

    if (!sendbuf) {
        printf("Error allocating %d bytes for send buffer\n", Send_Length);
        return 1;
    }

    /*
    *   Zero the buffer and copy as much of our data to it as possible 
    */

    memset(sendbuf, 0, Send_Length);
    strncpy(sendbuf, send_data, Send_Length);

    while (1) {
        if (verbose) {
            printf("calling sendto(socket = %d, length = %d),\n", s, Send_Length);
            printf("destination address:\n  ");
            print_saddr(&saddr);
        }

        /*
        *   Send a packet to everybody 
        */

        rc = sendto(s, sendbuf, Send_Length, 0, (const struct sockaddr *) &saddr, 16);

        if (verbose)
	    printf("\nsendto() returned %d\n", rc);

        if (rc == SOCKET_ERROR) {
            dos_net_perror("Sendto() failed");
            errflag++;
            break;
        }
        else {
            printf("\rSent datagram %d, length = %d bytes", ++dgrms, rc);
            if (verbose)
                printf("\n");
        }

        /*
        *   If we are to send only one, break out 
        */

        if (No_Loop)
            break;

        /*
        *   Sleep for a little while so we don't bombard the network 
        */

        Sleep(Sleep_Time);
    }

    if (verbose)
        printf("Freeing send buffer\n");

    free(sendbuf);

    return errflag;
}
