
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
* FUNCTION: demoGetLargest(HANDLE hConOut)                           *
*                                                                    *
* PURPOSE: demonstrate GetLargestConsoleWindowSize. Reports the size *
*          of the largest possible console window, given the current *
*          font.                                                     *
*                                                                    *
* INPUT: console input handle to query the information from and to   *
*        output to                                                   *
*********************************************************************/

void demoGetLargest(HANDLE hConOut)
{
  COORD coordLargest; /* hold the largest window size */
  CHAR szTemp[128];

  setConTitle(__FILE__);
  coordLargest = GetLargestConsoleWindowSize(hConOut);
  PERR(coordLargest.X | coordLargest.Y, "GetLargestConsoleWindowSize");
  myPuts(hConOut, "The largest console window size for this console, as\n"
                  "reported by GetLargestConsoleWindowSize, is:");
  sprintf(szTemp, "%d wide by %d high.", coordLargest.X, coordLargest.Y);
  myPuts(hConOut, szTemp);
  myPuts(hConOut, "\nHit enter to return...");
  myGetchar();
  return;
}
