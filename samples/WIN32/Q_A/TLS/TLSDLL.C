
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
* MODULE:       tlsdll.c
/****************************************************************************/


#include "windows.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "tlsdll.h"

#define TLS_MINIMUM_AVAILABLE 64

DWORD TlsIndex;        /* Global TLS index */
int TlsCount=0;        /* a counter for data in TLS storage */
char chTlsCount[3];    /* string for counter values */

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

BOOL WINAPI DllMain(HINSTANCE hInst, ULONG ul_reason_being_called, LPVOID lpReserved)

/*
Purpose: Is called by LibEntry, upon entering a dll or detaching
         from a DLL.  This function does most of the TLS manipulation.

Inputs:  hInst - not used
         ul_reason_being_called - reason DllMain is called


Returns: N/A

Calls:   TlsAlloc - allocate a TLS index
         malloc - allocate space for tls storage
         TlsSetValue - Store TLS values
         TlsFree - free TLS index
         TlsRetreive - retreives TLS values, based on index
*/

{

LPVOID TlsString;  /* TLS string for storage */


UNREFERENCED_PARAMETER(hInst);
UNREFERENCED_PARAMETER(lpReserved);

switch (ul_reason_being_called)
  {
  case DLL_PROCESS_ATTACH:           /* process attaches         */
    printf("Process Attaching\n");
    TlsIndex= TlsAlloc();            /* create TLS index         */
    if (TlsIndex == 0xFFFFFFFF)
      ErrorOut("TlsAlloc");
    break;

  case DLL_THREAD_ATTACH:            /* thread attaches          */
    if (!(DLL_PROCESS_ATTACH & ul_reason_being_called))
    {
    printf("Thread Attaching\n");
    TlsCount++;
    TlsString= malloc(100);          /* allocate storage         */
    strcat(TlsString, "Tls Thread#");/* build -                  */
    strcat(TlsString,
           itoa(TlsCount, chTlsCount,
           10) );                    /* - string                 */
    if (TlsSetValue(TlsIndex,        /* set TLS value            */
                    TlsString))
      printf("TlsSetValue success\n\n");
    else
      ErrorOut("TlsSetValue");
    }
    else
      printf("Process & Thread attaching\n\n");
    break;

  case DLL_THREAD_DETACH:            /* thread detaches          */
    printf("Thread Detaching\n");
    TlsRetreive(TlsIndex);           /* retreive TLS data        */
    break;

  case DLL_PROCESS_DETACH:           /* process detaches         */
    printf("Process Detaching\n");
    if (!TlsFree(TlsIndex))          /* free TLS index           */
      ErrorOut("TlsFree");
    else
      printf("TlsIndex Freed\n");
    break;


  default:
    break;
  }

return 1;
}

/****************************************************************************/

INT WINAPI TlsInit()

/*
Purpose: Allow DllMain to be executed for each thread

Inputs:  none

Returns: none

Calls:   none
*/

{
return 1;
}

/****************************************************************************/

INT WINAPI TlsRetreive(DWORD TlsIndex)

/*
Purpose: Retreive TLS values and display (printf) them

Inputs:  TlsIndex - TLS index value

Returns: none

Calls:   TlsGetValue - Gets the TLS value from the index
                       based on the current thread.
*/

{
LPVOID tlsvals;

(LPVOID)tlsvals= TlsGetValue(TlsIndex);   /* get the TLS value */
if (tlsvals == 0)
  ErrorOut("TlsGetValue");
else
  printf("tls value=  %s\n\n", tlsvals ); /* output it         */
return 1;
}
