/*	winsleep.c
 *	contains the message loop routine used to release control
 *	to other windows apps. It is invoked from the application
 *	with calls to sleep(0). It is invoked automatically anytime
 *	a fillbuf is called or a flush is performed. It is also called
 *	when input is requested.
 *
 *	(C) Copyright 1991-1993 Digital Mars
 *	All Rights Reserved
 *	Written by: G. Eric Engstrom
 */

#ifdef _WINDOWS
#include	<stdio.h>
#include	<stdlib.h>
#include <winc.h>
#include	<windows.h>

void __pascal MessageLoop(MSG __far *msg)
{
    while (GetMessage(msg, 0, 0, 0) != 0)
      {
      TranslateMessage(msg);
      DispatchMessage(msg);
      }
}
#endif
