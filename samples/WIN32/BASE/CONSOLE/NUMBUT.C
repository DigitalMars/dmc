
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

/*************************************************************
* FUNCTION: demoGetNumBut(HANDLE hConOut)                    *
*                                                            *
* PURPOSE: simply report the number of buttons on your mouse *
*                                                            *
* INPUT: the console output handle to write to               *
*************************************************************/

void demoGetNumBut(HANDLE hConOut)
{
  DWORD dwNumMouseButtons;
  BOOL bSuccess;
  CHAR szTemp[256];

  setConTitle(__FILE__);
  bSuccess = GetNumberOfConsoleMouseButtons(&dwNumMouseButtons);
  PERR(bSuccess, "GetNumberOfConsoleMouseButtons");
  myPuts(hConOut, "Using GetNumberOfConsoleMouseButtons to obtain the\n"
                  "number of buttons on your mouse...");
  sprintf(szTemp, "Your mouse has %d buttons.", dwNumMouseButtons);
  myPuts(hConOut, szTemp);
  myPuts(hConOut, "\nHit enter to return...");
  myGetchar();
  return;
}
