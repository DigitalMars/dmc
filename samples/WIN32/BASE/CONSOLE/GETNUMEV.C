
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
#include <string.h>
#include "console.h"

/* maximum number of input queue events to peek at */
#define INPUT_RECS 256

/* array of records to store peeked events from the input queue */
INPUT_RECORD aInputBuffer[INPUT_RECS];

/*********************************************************************
* FUNCTION: demoGetNumEvents(HANDLE hConOut)                         *
*                                                                    *
* PURPOSE: demonstrate GetNumberOfConsoleInputEvents,                *
*          PeekConsoleInput, and ReadConsoleInput. Delay the         *
*          processing of console input to start filling the input    *
*          queue. The number of console events in the input queue    *
*          will be updated in the status line at the top of the      *
*          console. Peek the unread characters for an ESC and return *
*          when one is found                                         *
*                                                                    *
* INPUT: console output handle to write to                           *
*********************************************************************/

void demoGetNumEvents(HANDLE hConOut)
{
  BOOL bSuccess;
  DWORD dwNumEvents; /* number of events in the input queue */
  DWORD dwStdInMode; /* save the input mode here */
  HANDLE hStdIn;
  DWORD dwInputEvents; /* number of events read from the queue */
  CHAR bOutBuf[256], szTemp[256];
  /* indexes to latest unread event checked for ESC char */
  DWORD iEvent, iPrevEvent;
  DWORD dwEventsPeeked; /* number of events peeked at */
  unsigned i;
  DWORD dwCharsWritten;

  setConTitle(__FILE__);
  hStdIn = GetStdHandle(STD_INPUT_HANDLE);
  PERR(hStdIn != INVALID_HANDLE_VALUE, "GetStdHandle");
  myPuts(hConOut, "\nLet's display a running count of events waiting in the\n"
                  "console input queue by using the GetNumberOfConsoleInputEvents\n"
                  "API. I will enable mouse input in this console; try\n"
                  "generating a lot of mouse and keyboard events. I will\n"
                  "read them from the input queue using ReadConsoleInput\n"
                  "with .5 second delays between reads. Note the event\n"
                  "count and event information on the top line.\n\n"
                  "Hit ESC at any time to return. We will use\n"
                  "PeekConsoleInput to monitor the unread input queue\n"
                  "contents for an ESC character. When this character\n"
                  "is detected, we will flush the input queue and return\n"
                  "immediately. Note that for simplification of this\n"
                  "demo only the first 256 unread events will be scanned\n"
                  "to find an ESC keystroke.\n\n");

  bSuccess = GetConsoleMode(hStdIn, &dwStdInMode);
  PERR(bSuccess, "GetConsoleMode");
  /* when turning off ENABLE_LINE_INPUT, you MUST also turn off */
  /* ENABLE_ECHO_INPUT. */
  bSuccess = SetConsoleMode(hStdIn, (dwStdInMode & ~(ENABLE_LINE_INPUT |
      ENABLE_ECHO_INPUT)) | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
  PERR(bSuccess, "SetConsoleMode");
  iEvent = 0; /* index to last event peeked in the input queue */
  for(;;)
    {
    Sleep(500);
    bSuccess = GetNumberOfConsoleInputEvents(hStdIn, &dwNumEvents);
    PERR(bSuccess, "GetNumberOfConsoleInputEvents");
    sprintf(bOutBuf, "input queue events: %d", dwNumEvents);
    if (!dwNumEvents)
      /* put a status line on the first line */
      putStatusLine(hConOut, bOutBuf);
    else
      {
      /* save the previous index we've peeked at */
      iPrevEvent = iEvent;
      /* peek at the console input queue. Don't peek more than what will */
      /* fit in the buffer */
      bSuccess = PeekConsoleInput(hStdIn, aInputBuffer, min(dwNumEvents,
          INPUT_RECS), &dwEventsPeeked);
      PERR(bSuccess, "PeekConsoleInput");
      /* set current index to the highest number event peeked at */
      iEvent = dwEventsPeeked;
      /* scan unread events for an ESC key */
      for (i = iPrevEvent; i < iEvent; i++)
        {
        if (aInputBuffer[i].EventType == KEY_EVENT &&
            aInputBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
          {
          /* set input mode back to what it was originally */
          bSuccess = SetConsoleMode(hStdIn, dwStdInMode);
          PERR(bSuccess, "SetConsoleMode");
          /* flush the input buffer and return */
          bSuccess = FlushConsoleInputBuffer(hStdIn);
          PERR(bSuccess, "FlushConsoleInputBuffer");
          return;
          }
        } /* for */
      } /* else */
    bSuccess = ReadConsoleInput(hStdIn, &aInputBuffer[0], 1, &dwInputEvents);
    PERR(bSuccess, "ReadConsoleInput");
    /* decrement "last peeked at" index by number of records we just read */
    iEvent -= dwInputEvents;
    switch (aInputBuffer[0].EventType)
      {
      case KEY_EVENT:
        if (aInputBuffer[0].Event.KeyEvent.bKeyDown)
          {
          if (aInputBuffer[0].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
            {
            /* set input mode back to what it was originally */
            bSuccess = SetConsoleMode(hStdIn, dwStdInMode);
            PERR(bSuccess, "SetConsoleMode");
            return;
            }
          /* put the keystroke information on the status line */
          sprintf(szTemp, " keystroke: %c",
              aInputBuffer[0].Event.KeyEvent.uChar.AsciiChar);
          strcat(bOutBuf, szTemp);
          /* put the status line on the screen */
          putStatusLine(hConOut, bOutBuf);
          /* output the character read from the input queue */
          bSuccess = WriteFile(hConOut,
              &aInputBuffer[0].Event.KeyEvent.uChar.AsciiChar, 1,
              &dwCharsWritten, NULL);
          PERR(bSuccess, "WriteFile");
          }
        break;
      case MOUSE_EVENT:
        sprintf(szTemp, " mouse: %s at %d, %d",
            (aInputBuffer[0].Event.MouseEvent.dwEventFlags == MOUSE_MOVED ?
            "moved" : "clicked"), aInputBuffer[0].Event.MouseEvent.dwMousePosition.X,
            aInputBuffer[0].Event.MouseEvent.dwMousePosition.Y);
        strcat(bOutBuf, szTemp);
        putStatusLine(hConOut, bOutBuf);
        break;
      case WINDOW_BUFFER_SIZE_EVENT:
        sprintf(szTemp, " window: %d, %d",
            aInputBuffer[0].Event.WindowBufferSizeEvent.dwSize.X,
            aInputBuffer[0].Event.WindowBufferSizeEvent.dwSize.Y);
        strcat(bOutBuf, szTemp);
        putStatusLine(hConOut, bOutBuf);
        break;
      } /* switch */
    } /* while */
}
