
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
* FUNCTION: demoCreate(HANDLE hConOld)                               *
*                                                                    *
* PURPOSE: demonstrate CreateConsoleScreenBuffer,                    *
*          SetConsoleTextAttribute, and                              *
*          SetConsoleActiveScreenBuffer. Create a 'help' screen on a *
*          new buffer and quickly switch between the main buffer and *
*          the help buffer on command without redrawing the entire   *
*          screens. SetConsoleTextAttribute will be used to make the *
*          help screen a different color.                            *
*                                                                    *
* INPUT: the output handle to write to                               *
**********************************************************************/

void demoCreate(HANDLE hConOld)
{
  BOOL bSuccess;
  HANDLE hConHelp; /* console for the help screen */
  COORD dwWriteCoord = {0, 0}; /* where to write the screen attributes */
  DWORD cCharsWritten;
  HANDLE hStdIn; /* standard input handle */
  CHAR c = 0; /* virtual key code that we will read */
  HANDLE hConCurrent; /* keep track of the current visible console buffer */
  INPUT_RECORD inputBuf; /* console input event record */
  DWORD cInputEvents;

  setConTitle(__FILE__);
  myPuts(hConOld, "Let's create a help screen in another buffer. Then\n"
                  "when the user hits F1, we can easily flip to the help\n"
                  "screen and back without recreating the text on the\n"
                  "screens by simply changing the active buffer.\n"
                  "We'll change the default text attribute with\n"
                  "SetConsoleTextAttribute before creating the help screen\n"
                  "so that the sample text is in a different color.\n"
                  "Hit F1 now for help, and esc to return here.\n"
                  "Hit esc from this screen to return to API list.");
  /* create a separate console buffer for the help screen */
  hConHelp = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
      FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER,
      NULL);
  PERR(hConHelp != INVALID_HANDLE_VALUE, "CreateConsoleScreenBuffer");
  /* change the color of the help screen */
  bSuccess = FillConsoleOutputAttribute(hConHelp, BACKGROUND_BLUE,
      getConX(hConHelp) * getConY(hConHelp), dwWriteCoord, &cCharsWritten);
  PERR(bSuccess, "FillConsoleOutputAttribute");
  /* set the color for future text output */
  bSuccess = SetConsoleTextAttribute(hConHelp, FOREGROUND_YELLOW |
      FOREGROUND_INTENSITY | BACKGROUND_BLUE);
  PERR(bSuccess, "SetConsoleTextAttribute");
  myPuts(hConHelp, "                S u p e r D u p e r B a s e   H e l p\n"
                   "\n\n                  F1: search        F6: window\n"
                   "                  F2: extract       F7: initialize\n"
                   "                  F3: sort          F8: exit\n"
                   "                  F4: query         F9: save\n"
                   "                  F5: execute       F10: explode\n"
                   "                  ESC: exit help");
  hStdIn = GetStdHandle(STD_INPUT_HANDLE);
  PERR(hStdIn != INVALID_HANDLE_VALUE, "GetStdHandle");
  /* keep track of the currently visible console */
  hConCurrent = hConOld;
  /* switch between the help and previous buffer when user hits F1 or ESC */
  for(;;)
    {
    do
      {
      /* throw away any non-keystroke events or any key-up events */
      bSuccess = ReadConsoleInput(hStdIn, &inputBuf, 1, &cInputEvents);
      PERR(bSuccess, "ReadConsoleInput");
      } while (inputBuf.EventType != KEY_EVENT ||
          !inputBuf.Event.KeyEvent.bKeyDown);
    /* get the virtual scan code of the key-down event */
    c = (char) inputBuf.Event.KeyEvent.wVirtualKeyCode;
    switch(c)
      {
      case VK_F1:
        /* if the current buffer is the original buffer, switch to the */
        /* help buffer */
        if (hConCurrent == hConOld)
          {
          bSuccess = SetConsoleActiveScreenBuffer(hConHelp);
          PERR(bSuccess, "SetConsoleActiveScreenBuffer");
          hConCurrent = hConHelp;
          }
        break;
      case VK_ESCAPE:
        /* if the current buffer is the help buffer, switch to the */
        /* original buffer. Otherwise, clean up and return */
        if (hConCurrent == hConHelp)
          {
          bSuccess = SetConsoleActiveScreenBuffer(hConOld);
          PERR(bSuccess, "SetConsoleActiveScreenBuffer");
          hConCurrent = hConOld;
          }
        else
          {
          CloseHandle(hConHelp);
          return;
          }
        break;
      case VK_F10:
        if (hConCurrent == hConHelp)
          myPuts(hConHelp, "BOOM!");
        break;
      default:
        break;
      }  /* switch */
    }  /* while */
  return;
}
