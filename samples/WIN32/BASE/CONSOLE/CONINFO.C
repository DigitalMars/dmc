
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
#include <conio.h>
#include "console.h"

/*******************************************************************
* FUNCTION: demoConInfo(HANDLE hConOut)                            *
*                                                                  *
* PURPOSE: demonstrate GetConsoleScreenBufferInfo. Get the current *
*          console screen buffer information and display it on the *
*          console.                                                *
*                                                                  *
* INPUT: console handle to output to                               *
*******************************************************************/

void demoConInfo(HANDLE hConOut)
{
  BOOL bSuccess;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  CHAR szTemp[128];

  setConTitle(__FILE__);
  myPuts(hConOut, "For information on the console buffer, we need to call\n"
                  "GetConsoleScreenBufferInfo. Make any adjustments to the\n"
                  "screen buffer/console window now, then hit return...\n");
  myGetchar();
  bSuccess = GetConsoleScreenBufferInfo(hConOut, &csbi);
  PERR(bSuccess, "GetConsoleScreenBufferInfo");
  myPuts(hConOut, "\nHere is the information on the buffer returned in the\n"
                  "CONSOLE_SCREEN_BUFFER_INFO structure:");
  sprintf(szTemp, "Size: X = %d, Y = %d", csbi.dwSize.X, csbi.dwSize.Y);
  myPuts(hConOut, szTemp);
  sprintf(szTemp, "Cursor pos: X = %d, Y = %d", csbi.dwCursorPosition.X,
      csbi.dwCursorPosition.Y);
  myPuts(hConOut, szTemp);
  sprintf(szTemp, "Character display attribute: 0x%04x", csbi.wAttributes);
  myPuts(hConOut, szTemp);
  sprintf(szTemp, "Window position: Left = %d, Right = %d, Top = %d, "
      "Bottom = %d", csbi.srWindow.Left, csbi.srWindow.Top,
      csbi.srWindow.Right, csbi.srWindow.Bottom);
  myPuts(hConOut, szTemp);
  sprintf(szTemp, "Maximum window size: X = %d, Y = %d",
      csbi.dwMaximumWindowSize.X, csbi.dwMaximumWindowSize.Y);
  myPuts(hConOut, szTemp);
  myPuts(hConOut, "\nHit any key to return...");
  myGetchar();
  return;
}
