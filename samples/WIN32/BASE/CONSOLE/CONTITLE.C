
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

/********************************************************************
* FUNCTION: demoGetTitle(HANDLE hConOut)                            *
*                                                                   *
* PURPOSE: demonstrate GetConsoleTitle and SetConsoleTitle. Read and*
*          display the console title, then set the console title.   *
*                                                                   *
* INPUT: handle to write to                                         *
********************************************************************/

void demoGetTitle(HANDLE hConOut)
{
  BOOL bSuccess;
  CHAR szTitleBuf[256]; /* buffer for the current console title */
  CHAR szTemp[256];
  HANDLE hStdIn; /* standard input handle */
  DWORD dwStdInMode; /* standard input handle mode */
  DWORD dwBytesRead;

  setConTitle(__FILE__);
  hStdIn = GetStdHandle(STD_INPUT_HANDLE);
  PERR(hStdIn != INVALID_HANDLE_VALUE, "GetStdHandle");
  /* save the console mode */
  bSuccess = GetConsoleMode(hStdIn, &dwStdInMode);
  PERR(bSuccess, "GetConsoleMode");
  /* turn on line input mode so we can enter a string. */
  /* when turning on ENABLE_LINE_INPUT, you MUST also turn on */
  /* ENABLE_ECHO_INPUT. */
  bSuccess = SetConsoleMode(hStdIn, dwStdInMode | ENABLE_LINE_INPUT |
      ENABLE_ECHO_INPUT);
  PERR(bSuccess, "SetConsoleMode");
  myPuts(hConOut, "Let's get the console title with GetConsoleTitle.");
  /* get the console title */
  dwBytesRead = GetConsoleTitle(szTitleBuf, sizeof(szTitleBuf));
  PERR(dwBytesRead, "GetConsoleTitle");
  sprintf(szTemp, "The console title is: %s", szTitleBuf);
  myPuts(hConOut, szTemp);
  myPuts(hConOut, "\nNow let's set a new console title with SetConsoleTitle.");
  myPuts(hConOut, "\nEnter a new console title:");
  bSuccess = ReadFile(hStdIn, szTitleBuf, 256, &dwBytesRead, NULL);
  PERR(bSuccess, "ReadFile");
  if (dwBytesRead > 2) /* did the user type any chars before hitting return? */
    {
    /* null terminate the string - less two for cr/lf */
    szTitleBuf[dwBytesRead - 2] = 0;
    bSuccess = SetConsoleTitle(szTitleBuf);
    PERR(bSuccess, "SetConsoleTitle");
    }
  /* restore the console title to the original mode */
  bSuccess = SetConsoleMode(hStdIn, dwStdInMode);
  PERR(bSuccess, "SetConsoleMode");
  myPuts(hConOut, "\nHit enter to return...");
  myGetchar();
  return;
}
