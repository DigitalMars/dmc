
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
#include "console.h"

/*******************************************************************
* FUNCTION: demoFillChar(HANDLE hConOut)                           *
*                                                                  *
* PURPOSE: demonstrate FillConsoleOutputCharacter. Fill the entire *
*          console with the character that the user hits           *
*                                                                  *
* INPUT: the output console to fill with characters                *
********************************************************************/

void demoFillChar(HANDLE hConOut)
{
  HANDLE hStdIn;
  INPUT_RECORD inputBuf;
  CHAR c; /* ascii character read from the console */
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  COORD coordScreen = {0, 1}; /* location to start the attribute fill */
  DWORD cCharsWritten;
  BOOL bSuccess; 
  DWORD cInputEvents;

  setConTitle(__FILE__);
  myPuts(hConOut, "Let's fill the console buffer with a given character by\n"
                  "using the FillConsoleOutputCharacter API. Hit a key to \n"
                  "fill the buffer with (hit ESC to return):");
  hStdIn = GetStdHandle(STD_INPUT_HANDLE);
  PERR(hStdIn != INVALID_HANDLE_VALUE, "GetStdHandle");
  for(;;)
    {
    do
      {
      /* get input events until you get a key-down event */
      bSuccess = ReadConsoleInput(hStdIn, &inputBuf, 1, &cInputEvents);
      PERR(bSuccess, "ReadConsoleInput");
      } while (inputBuf.EventType != KEY_EVENT ||
            !inputBuf.Event.KeyEvent.bKeyDown);
    c = (char) inputBuf.Event.KeyEvent.uChar.AsciiChar;
    if (inputBuf.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
      break;
    /* we need to get the console buffer size */
    bSuccess = GetConsoleScreenBufferInfo(hConOut, &csbi);
    PERR(bSuccess, "GetConsoleScreenBufferInfo");
    bSuccess = FillConsoleOutputCharacter(hConOut, /* screen buffer handle */
        c, /* character to write */
        (csbi.dwSize.X * csbi.dwSize.Y) - csbi.dwSize.X, /* number of chars */
        coordScreen, /* x and y coordinates of first cell */
        &cCharsWritten); /* receives number of cells written to */
    PERR(bSuccess, "FillConsoleOutputCharacter");
  }
  return;
}
