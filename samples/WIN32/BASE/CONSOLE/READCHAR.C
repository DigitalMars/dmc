
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
#include <string.h>
#include <malloc.h>
#include "console.h"

/*********************************************************************
* FUNCTION: demoReadConChar(HANDLE hConOut)                          *
*                                                                    *
* PURPOSE: demonstrate ReadConsoleOutputCharacter. Read the text on  *
*          line that the user clicks on and output it to the console *
*                                                                    *
* INPUT: the console output handle to write to                       *
*********************************************************************/

void demoReadConChar(HANDLE hConOut)
{
  BOOL bSuccess;
  INPUT_RECORD inputBuffer;
  DWORD dwStdInMode;
  HANDLE hStdIn;
  DWORD dwInputEvents;
  COORD coordLine; /* coordinates of where to read characters from */
  CHAR *szLine;  /* buffer to hold the line read from the console */
  DWORD dwCharsRead;
  int i;

  setConTitle(__FILE__);
  myPuts(hConOut, "Click on any line containing characters. I will use\n"
                  "ReadConsoleOutputCharacter to read that line of text into\n"
                  "a buffer, then print that buffer to the console at the\n"
                  "current cursor position. Hit ESC to return.\n\n");
  hStdIn = GetStdHandle(STD_INPUT_HANDLE);
  PERR(hStdIn != INVALID_HANDLE_VALUE, "GetStdHandle");
  /* save the console mode */
  bSuccess = GetConsoleMode(hStdIn, &dwStdInMode);
  PERR(bSuccess, "GetConsoleMode");
  /* enable mouse input */
  bSuccess = SetConsoleMode(hStdIn, dwStdInMode | ENABLE_MOUSE_INPUT);
  PERR(bSuccess, "SetConsoleMode");
  /* allocate space for one line */
  szLine = (char *) malloc(getConX(hConOut));
  PERR(szLine, "malloc");
  for(;;)
    {
    /* get a single input event */
    bSuccess = ReadConsoleInput(hStdIn, &inputBuffer, 1, &dwInputEvents);
    PERR(bSuccess, "ReadConsoleInput");
    switch (inputBuffer.EventType)
      {
      case KEY_EVENT:
        /* is it an ESC key? */
        if (inputBuffer.Event.KeyEvent.bKeyDown &&
            inputBuffer.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
          {
          /* set input mode back to what it was originally and return */
          bSuccess = SetConsoleMode(hStdIn, dwStdInMode);
          PERR(bSuccess, "SetConsoleMode");
          free(szLine); /* free allocated space for a text line */
          return;
          }
        break;
      case MOUSE_EVENT:
        /* was this was a click event? Is any button down or not? */
        if (inputBuffer.Event.MouseEvent.dwEventFlags != MOUSE_MOVED &&
            inputBuffer.Event.MouseEvent.dwButtonState)
          {
          /* read the line where the mouse is, starting at column 0 */
          coordLine.X = 0;
          coordLine.Y = inputBuffer.Event.MouseEvent.dwMousePosition.Y;
          bSuccess = ReadConsoleOutputCharacter(hConOut, szLine,
              getConX(hConOut), coordLine, &dwCharsRead);
          PERR(bSuccess, "ReadConsoleOutputCharacter");
          /* strip trailing spaces */
          i = getConX(hConOut) - 1;
          szLine[i--] = 0; /* null terminate */
          while (szLine[i] == ' ')
            szLine[i--] = 0;
          myPuts(hConOut, szLine);
          }
      } /* switch */
    } /* while */
}
