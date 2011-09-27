
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

/* The following is the sample text that we will place on the lower
half of the screen to scroll: */

static PCHAR szSampTxt =
  "BOOLÿScrollConsoleScreenBuffer(ÿhConsoleOutput, lpScrollRectangle,\n"
  "                               lpClipRectangle, dwDestinationOrigin,\n"
  "                               lpFill)\n"
  "HANDLE hConsoleOutput;\n"
  "PSMALL_RECT lpScrollRectangle;\n"
  "PSMALL_RECT lpClipRectangle;\n"
  "COORD dwDestinationOrigin;\n"
  "PCHAR_INFO lpFill;\n"
  "\n"
  "This function may be used to scroll the data in the screen buffer.\n"
  "\n"
  "Parameter            Description\n"
  "ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n"
  "hConsoleOutput       Supplies an open handle to console output.\n"
  "lpScrollRectangle    Pointer to region within screen buffer to move.\n"
  "lpClipRectangle      Pointer to region within screen buffer that may be\n"
  "                     affected by this scroll. This pointer may be NULL.\n"
  "dwDestinationOrigin  Upper left corner of new location of ScrollRectangle\n"
  "                     contents.\n"
  "lpFill               Pointer to structure containing new contents of\n"
  "                     ScrollRectangle region.\n"
  "\n"
  "Return Value\n"
  "The return value is TRUE if the function was successful, otherwise it is\n"
  "FALSE in which case extended error information can be retrieved by calling\n"
  "the GetLastError function.\n"
  "\n"
  "Comments\n"
  "This function copies the contents of a rectangular region of the screen\n"
  "buffer, the scroll region, to another area of the screen buffer, the target\n"
  "region. The target region is defined as a rectangle the same dimensions as\n"
  "the scroll region with the upper left corner at dwDestinationOrigin. Each\n"
  "cell in the scroll region is then filled with the contents of Fill. Any\n"
  "overlap between the scroll region and the target region is not filled. The\n"
  "clip rectangle applies to changes made in both the ScrollRectangle and the\n"
  "destination rectangle, i.e. if the clip rectangle does not include the\n"
  "scroll rectangle, the scroll rectangle will not be updated with the contents";


/*******************************************************************
* FUNCTION: demoScrollCon(HANDLE hConOut)                          *
*                                                                  *
* PURPOSE: demonstrate ScrollConsoleScreenBuffer. Scroll the lower *
*          half of the console with each mouse click               *
*                                                                  *
* INPUT: the console output handle to scroll                       *
********************************************************************/

void demoScrollCon(HANDLE hConOut)
{
  BOOL bSuccess;
  INPUT_RECORD inputBuffer;
  DWORD dwStdInMode;
  HANDLE hStdIn;
  DWORD dwInputEvents;
  COORD coordDest; /* destination of scroll movement */
  BOOL bDragMode = FALSE;
  CHAR_INFO chiFill; /* char and attributes to fill empty space with */
  CONSOLE_SCREEN_BUFFER_INFO csbi; /* used to get current attribute */
  SMALL_RECT srctScrollRect; /* area of the screen to scroll */

  setConTitle(__FILE__);
  myPuts(hConOut, "Let's use ScrollConsoleScreenBuffer to scroll the lower");
  myPuts(hConOut, "half of the screen up with each mouse click. I'll fill the");
  myPuts(hConOut, "screen with some sample text so you can see the effect.");
  myPuts(hConOut, "Hit return to continue, and hit ESC at any time to return.\n");
  myGetchar();
  myPuts(hConOut, szSampTxt);
  hStdIn = GetStdHandle(STD_INPUT_HANDLE);
  PERR(hStdIn != INVALID_HANDLE_VALUE, "GetStdHandle");
  bSuccess = GetConsoleMode(hStdIn, &dwStdInMode);
  PERR(bSuccess, "GetConsoleMode");
  bSuccess = SetConsoleMode(hStdIn, dwStdInMode | ENABLE_MOUSE_INPUT);
  PERR(bSuccess, "SetConsoleMode");
  /* define region we want to move */
  srctScrollRect.Top = getConY(hConOut) / 2;
  srctScrollRect.Bottom = getConY(hConOut) - 1;
  srctScrollRect.Left = 0;
  srctScrollRect.Right = getConX(hConOut) - 1;
  /* define origin where we want to move the scrolled region */
  coordDest.X = 0;
  coordDest.Y = (getConY(hConOut) / 2) - 1;
  /* get current attributes and fill out CHAR_INFO structure for fill char */
  bSuccess = GetConsoleScreenBufferInfo(hConOut, &csbi);
  PERR(bSuccess, "GetConsoleScreenBufferInfo");
  chiFill.Char.AsciiChar = ' ';
  chiFill.Attributes = csbi.wAttributes;
  for(;;)
    {
    bSuccess = ReadConsoleInput(hStdIn, &inputBuffer, 1, &dwInputEvents);
    PERR(bSuccess, "ReadConsoleInput");
    switch (inputBuffer.EventType)
      {
      case KEY_EVENT:
        if (inputBuffer.Event.KeyEvent.bKeyDown &&
            inputBuffer.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
          {
          /* set input mode back to what it was originally */
          bSuccess = SetConsoleMode(hStdIn, dwStdInMode);
          PERR(bSuccess, "SetConsoleMode");
          return;
          }
        break;
      case MOUSE_EVENT:
        /* was this was a click or double click event? Is any button down? */
        if (inputBuffer.Event.MouseEvent.dwEventFlags != MOUSE_MOVED &&
            inputBuffer.Event.MouseEvent.dwButtonState)
          {
          bSuccess = ScrollConsoleScreenBuffer(hConOut,
              &srctScrollRect,
              NULL, /* no clipping rectangle */
              coordDest, /* coordinates of destination */
              &chiFill); /* attribute to fill empty space with */
          PERR(bSuccess, "ScrollConsoleScreenBuffer");
          }
        break;
      } /* switch */
    } /* while */
}
