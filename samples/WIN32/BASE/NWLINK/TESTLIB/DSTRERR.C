/****************************************************************************\
*  dstrerr.c -- sample program demonstrating NWLink.
*
*       Microsoft Developer Support
*       Copyright (c) 1992, 1993 Microsoft Corporation
*
*  Demonstrates basic sockets programming with the Windows Sockets API
*  using the NWLink transport.
* 
****************************************************************************/
#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include "externs.h"

/****************************************************************************
*
*    FUNCTION:  dos_net_strerror( LPSTR p )
*
*    PURPOSE:   This will return an far char * to an error message 
*               string preceded by the user string passed in and 
*               seperated by a :.  The user string cannot be more 
*               than xx bytes in length. 
*
*    ARGUMENTS:	LPSTR	=> user string to print first
*
*	 RETURNS:   LPCSTR  => user string + error string
*
*\***************************************************************************/
LPCSTR dos_net_strerror(LPSTR p)
{
    int error = 0;
    static char return_string[256];

    /*
    *   Get the error number from the system 
    */
    
    error = h_errno;

    /*
    *   Build the string to return 
    */

    sprintf(return_string, "%s :(%d)%s", p, error, get_error_text(error));

    return (LPSTR)return_string;
}
