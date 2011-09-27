/****************************************************************************\
*  ping.c -- sample program demonstrating NWLink.
*
*       Microsoft Developer Support
*       Copyright (c) 1992, 1993 Microsoft Corporation
*
*  This program is a simple example of opening a socket,
*   binding to the socket, receiving a packet and sending
*  that packet back to the original sender.
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
int addrlen;

/*
*   Function Prototypes 
*/

extern int main(int, char **);
extern int net_init(SOCKET *);
extern int do_ping(SOCKET);

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
    *   Set our default values before calling parse_cmd_line 
    */

    *Local_Socket_Number = 0x30;
    *(Local_Socket_Number+1) = 0x00;
    Receive_Length = 2048;

    /*
    *   Get any command line options 
    */

    parse_cmd_line(argc, argv);

    /*
    *   Initialize the network and set up our socket 
    */

    if (net_init(&s))
        return 1;

    do_ping(s);

    /*
    *   All done (We only get here on error) 
    */

    if (verbose)
        printf("calling closesocket()");

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
    int rc, addrlen;
    WSADATA wsdata;
    SOCKET s;
    WORD    wVersionRequested;

    /*
    *   Initialize with the WINSOCK library 
    */

    if (verbose)
        printf("calling WSAStartup(), ");

    wVersionRequested = MAKEWORD(1,1);
    rc = WSAStartup(wVersionRequested, &wsdata);

    if (verbose)
        printf("return = 0x%X (%d)\n", rc, rc);

    if (rc) {
        printf("WSAStartup failed: error code = %d\n", rc);
        return 1;
    }

    if (verbose) {
        printf("contents of wsdata struct: \n");
        print_wsa(&wsdata);
    }

    if (verbose)
        printf("calling socket(address family = %d, socket type = %d, protocol = %d)\n", Local_Address_Family, Socket_Type, Protocol);

    /*
    *   Open a DATAGRAM socket with IPX 
    */

    s = socket(AF_NS, SOCK_DGRAM, NSPROTO_IPX);

    if (verbose)
        printf("socket() returned 0x%lX\n", s);

    if (s == INVALID_SOCKET) {
        dos_net_perror("Socket call failed");
        exit(1);
    }

    /*
    *   Bind to a socket.  We want to bind to a well known
    *   socket so that the app. that sends us a packet will
    *   know where to send it.
    */

    addr.sa_family = Local_Address_Family;

    memcpy(&addr.sa_netnum, Local_Network_Number, 4);
    memcpy(&addr.sa_nodenum, Local_Node_Number, 6);
    memcpy(&addr.sa_socket, Local_Socket_Number, 2);

    if (verbose) {
        printf("calling bind():\n  ");
        print_saddr(&addr);
    }

    rc = bind(s, (const struct sockaddr *) &addr, 16);

    if (verbose)
        printf("bind() returned 0x%X\n", rc);

    if (rc == SOCKET_ERROR) {
        dos_net_perror("Error binding to socket");
        closesocket(s);
        return 1;
    }
    /*
    *   Set the packet type for this socket 
    */

    if (verbose)
        printf("Calling setsockopt for packet type %d\n", Local_Packet_Type);

    rc = setsockopt(s, SOL_SOCKET, IPX_PTYPE, (const char *) &Local_Packet_Type, 4);

    if (rc == SOCKET_ERROR)
        dos_net_perror("setsockopt() call failed");


    /*
    *   Get the address we bound to and print it out 
    */

    if (verbose)
        printf("Calling getsockname(socket = %d), ");

    addrlen = 16;
    rc = getsockname(s, (struct sockaddr *) &baddr, &addrlen);

    if (verbose)
        printf("return = 0x%X (%d)\n", rc, rc);

    if (rc == SOCKET_ERROR) {
        dos_net_perror("Error getting socket name");
        closesocket(s);
        return 1;
    }

    /*
    *   Print out the network address 
    */

    if (verbose) {
        printf("addrlen = %d\n", addrlen);
        print_netaddr(baddr.sa_netnum, "Bound address = ", "\n");
    }

    if (verbose)
        printf("Allocating %d byte receive buffer\n", Receive_Length);

    /*
    *   Set up socket to send back 
    */

    *skt = s;

    return 0;
}

/****************************************************************************
*
*    FUNCTION:  do_ping( SOCKET s )
*
*    PURPOSE:   This will receive a packet then send it back.
*   		    
*    ARGUMENTS:	SOCKET socket we are transmitting on.
*
*	 RETURNS:   0 if ok
*				1 if error
*
*\***************************************************************************/
int do_ping(SOCKET s)
{
    LPSTR recvbuf;
    int nbytes, rc, errflag = 0;
    int rcvpkts = 0, sndpkts = 0;

    /*
    *   Allocate a buffer for receives 
    */

    recvbuf = malloc(Receive_Length);

    if (!recvbuf) {
        printf("Error allocating %d bytes for receive buffer\n", Receive_Length);
        return 1;
    }

    /*
    *   This loop will receive a packet and then send
    *   it back to whoever sent it to us.
	*
	*      To exit the loop - hit CTRL-C.
    */

    while (1) {

        /*
        *   Recv a packet 
        */

        /*
        *    NOTE:  If you want to know the packet type field of the
        *           packet you just received, look at the byte at
		*           raddr.sa_ptype.  The addrlen returned will be
		*           15.
		*
        *           By using the addrlen field unchanged when sending
        *           the packet back, you will send the packet back with
        *           the same packet type that is was sent with originally
        */

        addrlen = 16;

        if (verbose)
            printf("calling recvfrom(socket = %d, len = %d)\n", s, Receive_Length);

        nbytes = recvfrom(s, recvbuf, Receive_Length, 0, (struct sockaddr *) &raddr, &addrlen);

        /*
        *   If error - print it and exit 
        */

        if (nbytes == SOCKET_ERROR) {
            dos_net_perror("recvfrom() failed");
            errflag++;
            break;
        }

        if (verbose) {
            printf("received %d bytes, raddr = \n  ", nbytes);
            print_saddr(&raddr);
        }

        if (!verbose)
            printf("\rRecv packet number %d", ++rcvpkts);

        /*
        *   Send the data back 
        */

        if (verbose)
            printf("calling sendto(socket = %d, len = %d\n", s, nbytes);

	addrlen = 16;
        rc = sendto(s, recvbuf, nbytes, 0, (const struct sockaddr *) &raddr, addrlen);

        if (verbose)
            printf("sendto() returned 0x%X\n", rc);

        if (rc == SOCKET_ERROR) {
            dos_net_perror("sendto() failed");
            errflag++;
            break;
        }

        if (!verbose)
            printf(" : Send packet number %d", ++sndpkts);

    }

    if (verbose)
        printf("Freeing receive buffer\n");

    free(recvbuf);

    return errflag;
}
