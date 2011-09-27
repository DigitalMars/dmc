/****************************************************************************\
*  geterror.c -- sample program demonstrating NWLink.
*
*       Original code from Micro Computer Systems, Inc.
*       Copyright(c) 1992  All Rights Reserved.
*       Microsoft Developer Support
*       Copyright (c) 1992, 1993 Microsoft Corporation
*
*  Demonstrates basic sockets programming with the Windows Sockets API
*  using the NWLink transport.
* 
****************************************************************************/
#include <winsock.h>
#include <stdio.h>
#include "externs.h"

#define WSAERRGAP1 28
#define WSAERRGAP2 19
 
extern ERROR_STRUCT errlist[];

/****************************************************************************
*
*    FUNCTION:  get_error_text( int error )
*
*    PURPOSE:   Gets the text string explaining the error number passed in.
*   		    If the error number is 0, or is not found, a respective
*				message is returned.
*
*    ARGUMENTS:	int	error number to look up
*                
*	 RETURNS:   LPCSTR	=> char string having error message
*				
*\***************************************************************************/
LPCSTR get_error_text(int error)
{
    int search = 0;


    /*
    *   No error
    */

    if (!error)
        return (LPCSTR)msg_no_error;

    /* 
    *   Search through our array of error number / string pairs
    *   until we find a matching error number or get to the end
    *   of our list.  If we found a matching error number, 
    *   return a LPSTR to the corresponding string.
    */

    while (errlist[search].errstr) {
        if (error == errlist[search].errnum)
            return errlist[search].errstr;
        search++;
    }

    /*
    * If we didn't have the error in our list, return unkown 
    */

    return (LPCSTR)msg_unknown_error;
}

    
