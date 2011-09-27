/*	winerr.c
 *	contains the fatal error message dialog box and window instance
 *	termination code for Windows 3.0
 *
 *	(C) Copyright 1993 Digital Mars
 *	All Rights Reserved
 *	Written by: G. Eric Engstrom
 */

#ifdef _WINDOWS
#include	<stdio.h>
#include	<stdlib.h>
#include <winc.h>
#include	<windows.h>

void __pascal MessageFatalError(char __far *wFatalErrorMessage)
{
// wipe out the stack
   asm
   {
// Save the error string
      mov   dx,bp[8] // segment
      mov   ax,bp[6] // offset
// Save the return address
      mov   cx,bp[4] // segment
      mov   bx,bp[2] // offset

// Wipe the stack
      mov   sp,ss:0Eh
      sub   sp,800h

// Push the error string address back
      push  dx
      push  ax

// Push the return adress string back
      push  cx
      push  bx
   }

   MessageBeep(0);
   FatalAppExit(0, wFatalErrorMessage);
}
#endif
