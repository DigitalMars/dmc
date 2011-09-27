/****************************************************************************\
*  wperror.c -- sample program library demonstrating NWLink.
*
*       Microsoft Developer Support
*       Copyright (c) 1992, 1993 Microsoft Corporation
*
*  Demonstrates basic sockets programming with the Windows Sockets API
*  using the NWLink transport.
* 
****************************************************************************/
#include <stdio.h>
#include <windows.h>
#include <winsock.h>
#include <wsipx.h>

/****************************************************************************
*
*    FUNCTION:  print_network_num( char *addrp)
*
*    PURPOSE:   Print out a given network number
* 				
*    ARGUMENTS:	addrp   Pointer to the network address
*               
*	 RETURNS:   nothing.
*
*\***************************************************************************/
void print_network_num(char *addrp)
{

    printf("%02X%02X%02X%02X",
        (unsigned char)*addrp,
        (unsigned char)*(addrp + 1),
        (unsigned char)*(addrp + 2),
        (unsigned char)*(addrp + 3));

    return;
}


/****************************************************************************
*
*    FUNCTION:  print_node_num( char *addrp)
*
*    PURPOSE:   Print out a given node number
* 				
*    ARGUMENTS:	addrp   Pointer to the network address
*               
*	 RETURNS:   nothing.
*
*\***************************************************************************/
void print_node_num(char *addrp)
{

    printf("%02X%02X%02X%02X%02X%02X",
        (unsigned char)*addrp,
        (unsigned char)*(addrp + 1),
        (unsigned char)*(addrp + 2),
        (unsigned char)*(addrp + 3),
        (unsigned char)*(addrp + 4),
        (unsigned char)*(addrp + 5));

    return;
}


/****************************************************************************
*
*    FUNCTION:  print_socket_num( char *addrp)
*
*    PURPOSE:   Print out a given socket number
* 				
*    ARGUMENTS:	addrp   Pointer to the network address
*               
*	 RETURNS:   nothing.
*
*\***************************************************************************/
void print_socket_num(char *addrp)
{

    printf("%02X%02X",
        (unsigned char)*addrp,
        (unsigned char)*(addrp + 1));

    return;
}

/****************************************************************************
*
*    FUNCTION:  print_netaddr( char *addrp, char *prestr, char *poststr)
*
*    PURPOSE:   Print out a given network address
* 				
*    ARGUMENTS:	addrp   = Ptr to the network address
*				prestr  = String to print before addr
*				poststr = String to print after addr
*               
*	 RETURNS:   nothing.
*
*\***************************************************************************/
void print_netaddr(char *addrp, char *prestr, char *poststr)
{

    if (prestr)
        printf(prestr);


    print_network_num(addrp);
    printf(" - ");
    print_node_num(addrp+4);
    printf(" - ");
    print_socket_num(addrp+10);


    if (poststr)
        printf(poststr);

    return;
}

/****************************************************************************
*
*    FUNCTION:  print_saddr( PSOCKADDR_IPX p )
*
*    PURPOSE:   Prints out contents of the SOCKADDR_IPX struct passed in.
* 				
*    ARGUMENTS:	PSOCKADDR_IPX => SOCKADDR_IPX struct to dump.
*               
*	 RETURNS:   nothing.
*
*\***************************************************************************/
void print_saddr(PSOCKADDR_IPX p)
{
    printf("address family = %d  address = ", p->sa_family);
    print_netaddr(p->sa_netnum, NULL, NULL);
}
 
/****************************************************************************
*
*    FUNCTION:  print_wsa( LPWSADATA w )
*
*    PURPOSE:   Dumps out the contents of a WSA structure.
* 				
*    ARGUMENTS:	umps out the contents of a WSA structure.
*               
*	 RETURNS:   nothing.
*
*\***************************************************************************/
void print_wsa(LPWSADATA w)
{
    printf("  Version =         %ld\n", w->wVersion);
    printf("  High Version =    %ld\n", w->wHighVersion);
    printf("  Description =     %s\n", w->szDescription);
    printf("  System Status =   %s\n", w->szSystemStatus);
    printf("  Max sockets =     %d\n", w->iMaxSockets);
    printf("  Max UPD dg size = %d\n", w->iMaxUdpDg);
}
