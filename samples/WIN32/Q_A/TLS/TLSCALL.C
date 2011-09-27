
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/****************************************************************************\
*
* MODULE:       tlscall.c
*
/****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>
#include <string.h>
#include <winbase.h>
#include "tlsdll.h"

#define NUMTHREADS 3    /* The number of threads to start */

/*-----------------------------------------------------------------*/

void ErrorOut(char errstring[30])
/*
Purpose: Print out an meainful error code by means of
         GetLastError and printf

Inputs:  errstring - the action that failed, passed by the
                     calling proc.

Returns: none

Calls:   GetLastError
*/

{
DWORD Error;

Error= GetLastError();
printf("Error on %s = %d\n", errstring, Error);
}

/*-----------------------------------------------------------------*/

void Another_Thread()
/*
Purpose: Make a call into a DLL and then terminate, allowing
         DLL init. and termination to be called.

Inputs:  none

Returns: none

Calls:   TlsInit: Dummy proc. to allow DLL initialization.
*/

{
Sleep(300);      /* Sleep while other threads start */
TlsInit();       /* Call dummy init. procedure */
ExitThread(0);   /* Exit, calling DLL termination */
}

/*-----------------------------------------------------------------*/

void main(void)
/*
Purpose: Make a call into a DLL and then terminate, allowing
         DLL init. and termination to be called.

Inputs:  none

Returns: none

Calls:   CreateThread: to create threads for DLL init.
         ErrorOut: To output any errors
         WaitForMultipleObjects: To wait on thread(s) termination
*/

{
int  threadid;
HANDLE hThreads[NUMTHREADS];
int count;

  // check if Win32s, if so, display notice and terminate
     if( GetVersion() & 0x80000000 && (GetVersion() & 0xFF) ==3)
     {
       MessageBox( NULL,
          "This application cannot run on Windows 3.1.\n"
          "This application will now terminate.",
          "Tls",
          MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
       return;
     }

 /* start NUMTHREADS threads */
  for (count = 0 ; count < NUMTHREADS ; count++)
  {
  hThreads[count]= CreateThread(NULL,
                                0,
                                (LPTHREAD_START_ROUTINE)Another_Thread,
                                NULL,
                                0,
                                (LPDWORD)&threadid);
  if( hThreads[count] == 0)
    ErrorOut("CreateThread");
  else
    printf("\nStarting thread# %d\n\n", count +1);
  }

      /* now wait for threads to terminate */
printf("\n\n***** Waiting for %d Threads to Exit*****\n\n", NUMTHREADS);
WaitForMultipleObjects( NUMTHREADS,
                        hThreads,
                        FALSE,
                        0xFFFFFFFF);
Sleep(1000);
printf("\n\n*****   All %d Threads Terminated   *****\n\n", NUMTHREADS);

}
