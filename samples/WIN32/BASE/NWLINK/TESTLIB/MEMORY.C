/****************************************************************************\
*  memory.c -- sample program demonstrating NWLink.
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
*    FUNCTION:  mem_check( LPSTR p, UCHAR ch, int len)
*
*    PURPOSE:   Makes sure that a buffer is filled with only the character
*   		    specified.
*
*    ARGUMENTS:	LPSTR	=> buffer to scan
*               char    character to check for
*               int     length of buffer to check 
*
*	 RETURNS:   0 if buffer has only the specified character
*				offset to non-matching character if found 
*
*\***************************************************************************/
int mem_check(LPSTR p, UCHAR ch, int len)
{
    int buflen;

    buflen = len;

    while (len--) {
        if ((UCHAR)*p++ != ch) {
            return (buflen - len);
        }
    }
  
    return 0;
}
