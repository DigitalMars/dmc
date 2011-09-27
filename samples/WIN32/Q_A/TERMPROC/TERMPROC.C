
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include <winbase.h>

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


void main(int argc, char *argv[])
/*
Purpose: terminate a process based on the PID passed in

Inputs:  argc - number of arguments passed in
         argv - arguments, including PID of process to terminate

Returns: none

Calls:   GetLastError
         OpenProcess - to get handle to process
         TerminateProcess - to terminate process
*/

{
HANDLE hProcess;
double ProcId;
BOOL TermSucc;
int chrintval, chrpos, downcount;
int count=0;
DWORD dwVersion;


    // check if running on Windows NT, if not, display notice and terminate
    dwVersion = GetVersion();
    if( !(dwVersion < 0x80000000) && (LOBYTE(LOWORD(dwVersion)) < 4) )
    {
         MessageBoxA( NULL,
         "This sample application cannot be run on Windows 3.1.\n"
         "This application will now terminate.",
         "TermProc",
         MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
      return;
    }
  
if (argc == 2)  /* if a PID is passed in */
  {
    chrpos= strcspn( argv[1], "\0");
    count=0;
    ProcId=0;

         /* convert decimal value to hex */
    for (downcount = chrpos -1 ; downcount >= 0; downcount--)
      {
      if (argv[1][downcount] >= '0' && argv[1][downcount] <= '9')
        chrintval= argv[1][downcount]-48;
      else
        if (argv[1][downcount] >= 'a' && argv[1][downcount] <= 'f')
          chrintval= argv[1][downcount]-87;
        else
          chrintval= argv[1][downcount]-55;
      ProcId+= chrintval * pow( (double)16.0, (double)count );
      count++;
      }
       /* open the process to terminate */
  hProcess= OpenProcess(PROCESS_ALL_ACCESS, TRUE, (DWORD)ProcId);
  if (hProcess == NULL)
    ErrorOut("OpenProcess");
  TermSucc= TerminateProcess(hProcess, 0); /* terminate it */
  if (TermSucc == FALSE)
    ErrorOut("TerminateProcess");
  else
    printf("Process# %s terminated successfully!\n", argv[1]);
  }
else  /* no PID, inform user of operation */
  {
  printf("\nTerminates an active Process\n");
  printf("Usage: termproc ProcessID\n");
  }

}
