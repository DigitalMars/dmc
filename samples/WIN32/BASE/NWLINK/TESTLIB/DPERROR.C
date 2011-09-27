/****************************************************************************\
*  dperror.c -- sample program demonstrating NWLink.
*
*       Microsoft Developer Support
*       Copyright (c) 1992, 1993 Microsoft Corporation
*
*  Demonstrates basic sockets programming with the Windows Sockets API
*  using the NWLink transport.
* 
****************************************************************************/
#include <stdio.h>
#include <winsock.h>
#include "externs.h"

/****************************************************************************
*
*    FUNCTION:  dos_net_perror( LPSTR p )
*
*    PURPOSE:   This will printf an error message to stdout showing the 
*               last error for the calling process.  This should be 
*               called IMMEDIATELY after a call has returned an error 
*               condition.
*   		    
*    ARGUMENTS:	LPSTR	=> user string to print first
*
*	 RETURNS:   nothing
*
*\***************************************************************************/
void dos_net_perror(LPSTR p)
{
    printf(dos_net_strerror(p));
    printf("\n");

    return;
}
