
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
#include <memory.h>
#include <stdio.h>
#include <ctype.h>
#include "console.h"

#define MAX_MACRO_EVENTS 256
#define ALT_PRESSED (RIGHT_ALT_PRESSED | LEFT_ALT_PRESSED)
#define CONTROL_KEY (RIGHT_ALT_PRESSED | LEFT_ALT_PRESSED | \
                     RIGHT_CTRL_PRESSED | LEFT_CTRL_PRESSED | ENHANCED_KEY)

/************************************************************************
* FUNCTION: demoWriteIn(HANDLE hConOut)                                 *
*                                                                       *
* PURPOSE: demonstrate WriteConsoleInput by implementing a macro        *
*          recording facility that writes the recorded keystrokes to    *
*          the input queue for playback.                                *
*                                                                       *
* INPUT: the console buffer to record from and playback to              *
*                                                                       *
* RETURNS: none                                                         *
*************************************************************************/


void demoWriteIn(HANDLE hConOut)
{
  BOOL bSuccess;
  HANDLE hStdIn; /* standard input handle */
  INPUT_RECORD inputBuffer; /* buffer to hold a single console input record */
  INPUT_RECORD irMacroBuf[MAX_MACRO_EVENTS]; /* array of input events */
  DWORD dwInputEvents;
  DWORD dwBytesWritten, dwRecordsWritten;
  CHAR bOutBuf[256]; /* buffer to format event information into */
  BOOL bRecording = FALSE; /* TRUE if recording a macro */
  int iir = 0; /* index into macro input buffer */
  CHAR c;

  setConTitle(__FILE__);
  myPuts(hConOut, "\n\nLet's implement a simple macro record/playback facility.\n"
                  "Hit Alt+R to record, and Alt+P to playback. After turning\n"
                  "on Record mode, enter some keystrokes or mouse clicks.\n"
                  "Hit Alt+R to turn Record mode off, and Alt+P to playback\n"
                  "your keystrokes. I'll use the WriteConsoleInput API to\n"
                  "insert the recorded events into the input buffer.\n"
                  "Hit ESC at any time to return.");
  hStdIn = GetStdHandle(STD_INPUT_HANDLE);
  PERR(hStdIn != INVALID_HANDLE_VALUE, "GetStdHandle");
  for(;;)
    {
    /* if our macro event buffer is full, leave Record mode */
    if (iir == MAX_MACRO_EVENTS && bRecording)
      {
      bRecording = FALSE;
      putStatusLine(hConOut, "");
      }
    /* read an input event from the input event queue */
    bSuccess = ReadConsoleInput(hStdIn, &inputBuffer, 1, &dwInputEvents);
    PERR(bSuccess, "ReadConsoleInput");
    switch (inputBuffer.EventType)
      {
      case KEY_EVENT:
        if (inputBuffer.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
          return;
        /* get the character that we read */
        c = inputBuffer.Event.KeyEvent.uChar.AsciiChar;
        if (!bRecording)
          {
          /* ignore key releases */
          if (!inputBuffer.Event.KeyEvent.bKeyDown)
            break;
          /* display the key event info on the status line */
          sprintf(bOutBuf, "key: virtual=%d ascii=%c",
              inputBuffer.Event.KeyEvent.wVirtualKeyCode, c);
          putStatusLine(hConOut, bOutBuf);
          /* if it's printable, and no control keys are down, show it */
          if (isprint(c) && !(inputBuffer.Event.KeyEvent.dwControlKeyState &
                CONTROL_KEY))
            {
            bSuccess = WriteFile(hConOut, &c, sizeof(c), &dwBytesWritten,
                NULL);
            PERR(bSuccess, "WriteFile");
            }
          /* is it an 'r' key? Is either the left or right ALT key down? */
          if (c == 'r' && (inputBuffer.Event.KeyEvent.dwControlKeyState &
              (ALT_PRESSED)))
            {
            bRecording = TRUE;
            iir = 0; /* reset macro buffer pointer to beginning */
            putStatusLine(hConOut, "RECORDING...");
            }
          /* is it a 'p' key? Is either the left of right ALT key down? */
          if (c == 'p' && (inputBuffer.Event.KeyEvent.dwControlKeyState &
              (ALT_PRESSED)))
            {
            /* stuff all the input events into the input buffer */
            bSuccess = WriteConsoleInput(hStdIn, /* console input buffer */
                irMacroBuf, /* address of the buffer for write data */
                iir, /* number of records to write */
                &dwRecordsWritten); /* returns number of records written */
            PERR(bSuccess, "WriteConsoleInput");
            }
          }
        else /* we're recording */
          {
          if (inputBuffer.Event.KeyEvent.bKeyDown)
            {
            /* is it an 'r' key? Is either the left or right ALT key down? */
            if (c == 'r' && (inputBuffer.Event.KeyEvent.dwControlKeyState &
                (ALT_PRESSED)))
              {
              bRecording = FALSE;
              putStatusLine(hConOut, "");
              break;
              }
            /* we need to ignore the 'playback' keystroke or we'll */
            /* automatically start playing back during playback! */
            if (c == 'p' && (inputBuffer.Event.KeyEvent.dwControlKeyState &
                (ALT_PRESSED)))
              break;
            /* if it's printable, and no control keys are down, show it */
            if (isprint(c) && !(inputBuffer.Event.KeyEvent.dwControlKeyState &
                CONTROL_KEY))
              {
              bSuccess = WriteFile(hConOut, &c, sizeof(c), &dwBytesWritten,
                  NULL);
              PERR(bSuccess, "WriteFile");
              }
            }
          /* store the key event in the macro buffer */
          memcpy(&irMacroBuf[iir++], &inputBuffer, sizeof(inputBuffer));
          } /* else */
        break;
      case MOUSE_EVENT:
        if (!bRecording)
          {
          sprintf(bOutBuf, "mouse: %s at %d, %d",
              (inputBuffer.Event.MouseEvent.dwEventFlags == MOUSE_MOVED ?
              "moved" : "clicked"), inputBuffer.Event.MouseEvent.dwMousePosition.X,
              inputBuffer.Event.MouseEvent.dwMousePosition.Y);
          putStatusLine(hConOut, bOutBuf);
          }
        else /* copy the mouse event into the macro buffer */
          memcpy(&irMacroBuf[iir++], &inputBuffer, sizeof(inputBuffer));
        break;
      case WINDOW_BUFFER_SIZE_EVENT:
        if (!bRecording)
          {
          sprintf(bOutBuf, "window: %d, %d",
              inputBuffer.Event.WindowBufferSizeEvent.dwSize.X,
              inputBuffer.Event.WindowBufferSizeEvent.dwSize.Y);
          putStatusLine(hConOut, bOutBuf);
          Sleep(1000);
          }
        break;
      } /* switch */
    } /* while */
  return;
}
