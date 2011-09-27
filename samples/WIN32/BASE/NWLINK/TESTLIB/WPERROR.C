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
#include "externs.h"

/****************************************************************************
*
*    FUNCTION:  win_net_perror( HWND parent, LPSTR p, int tm_flag)
*
*    PURPOSE:   Similar to the perror() function. This will display
*   		a dialog box witht an error message showing the last error for 
*		the calling process.  This should be called IMMEDIATELY after 
* 		a call has returned an error condition.
*
*    ARGUMENTS:	HWND    to the parent window	
*               LPSTR	=> user string to print first
*               int	flag set if message box should be task modal
*
*	 RETURNS:   nothing.
*
*\***************************************************************************/
void win_net_perror(HWND parent, LPSTR p, int tm_flag)
{
    MessageBox(parent, dos_net_strerror(p), NULL, 
        MB_OK | MB_ICONEXCLAMATION | (tm_flag ? MB_TASKMODAL : 0));

    return;
}
