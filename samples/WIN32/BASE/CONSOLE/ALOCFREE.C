
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#include <windows.h>
#include <stdio.h>
#include "console.h"

/*********************************************************************
* FUNCTION: demoAllocFree(HANDLE hConOld, HANDLE *hConOut)           *
*                                                                    *
* PURPOSE: demonstrate FreeConsole & AllocConsole. Free the console  *
*          and allocate a new one                                    *
*                                                                    *
* INPUT: the current console output handle and a temporary 'scratch' *
*        console handle                                              *
*********************************************************************/

void demoAllocFree(HANDLE hConOld, HANDLE *hConOut)
{
  BOOL bSuccess;

  setConTitle(__FILE__);
  myPuts(hConOld, "Let's free the current console with FreeConsole. It will\n"
                  "disappear for 3 seconds, then we'll allocate a new one.\n"
                  "with AllocConsole. Hit return to continue...");
  myGetchar();
  bSuccess = FreeConsole();
  PERR(bSuccess, "FreeConsole");
  Sleep(3000);
  bSuccess = AllocConsole();
  PERR(bSuccess, "AllocConsole");
  *hConOut = GetStdHandle(STD_OUTPUT_HANDLE);
  /* set our console text attribute */
  bSuccess = SetConsoleTextAttribute(*hConOut, FOREGROUND_CYAN);
  PERR(bSuccess, "SetConsoleTextAttribute");
  myPuts(*hConOut, "This is a new console that we've allocated with AllocConsole.");
  Sleep(2000);
  /* must refresh the screen since we've replaced the console */
  showConAPIs(*hConOut);
  return;
}
