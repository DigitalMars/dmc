
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

/********************************************************************
* FUNCTION: invertRectangle(HANDLE hConOut, COORD coordStart,       *
*                           COORD coordCur)                         *
*                                                                   *
* PURPOSE: invert the attributes of the rectangle defined as the    *
*          area between two input COORD parameters                  *
*                                                                   *
* INPUT: the output handle of the console to make the changes to,   *
*        and two sets of COORD structures defining the rectangle to *
*        invert                                                     *
********************************************************************/

void invertRectangle(HANDLE hConOut, COORD coordStart, COORD coordCur)
{
  WORD *awLineAttr;
  unsigned i;
  DWORD dwBoxWidth;
  WORD top, left, right, bottom;
  BOOL bSuccess;
  DWORD dwNumAttrsRead;

  /* convert coordSart to upper left & coordCur to lower right */
  top = min(coordStart.Y, coordCur.Y);
  bottom = max(coordStart.Y, coordCur.Y);
  left = min(coordStart.X, coordCur.X);
  right = max(coordStart.X, coordCur.X);
  coordStart.X = left;
  coordStart.Y = top;
  coordCur.X = right;
  coordCur.Y = bottom;
  /* get the width of the selected area */
  dwBoxWidth = right - left + 1;
  /* allocate a buffer for the attributes read from the console */
  awLineAttr = (PWORD) malloc(getConX(hConOut) * sizeof(DWORD));
  PERR(awLineAttr, "malloc");
  /* for each line in the selected area */
  while (coordStart.Y <= coordCur.Y)
    {
    /* read the attributes on that line */
    bSuccess = ReadConsoleOutputAttribute(hConOut, awLineAttr,
        dwBoxWidth, coordStart, &dwNumAttrsRead);
    PERR(bSuccess, "ReadConsoleOutputAttribute");
    /* for each attribute in the line, invert the color */
    for (i = 0; i < dwBoxWidth; i++)
      awLineAttr[i] = ~awLineAttr[i];
    /* put the inverted color back on the line */
    bSuccess = WriteConsoleOutputAttribute(hConOut, awLineAttr, dwBoxWidth,
        coordStart, &dwNumAttrsRead);
    PERR(bSuccess, "WriteConsoleOutputAttribute");
    coordStart.Y++; /* go to the next line */
    }
  free(awLineAttr); /* free allocated attribute buffer */
  return;
}


/********************************************************************
* FUNCTION: demoReadConOut(HANDLE hConOut)                          *
*                                                                   *
* PURPOSE: demonstrate ReadConsoleOutput, WriteConsoleOutput,       *
*          ReadConsoleOutputAttribute, and                          *
*          WriteConsoleOutputAttribute. Implement a cut and past    *
*          function; as the user clicks and drags the mouse, invert *
*          the selection rectangle to show the area selected. When  *
*          the mouse button is released, copy the selected area and *
*          paste it to the console                                  *
*                                                                   *
* INPUT: the output handle to read from and write to                *
********************************************************************/

void demoReadConOut(HANDLE hConOut)
{
  BOOL bSuccess;
  INPUT_RECORD inputBuffer;
  DWORD dwStdInMode;
  HANDLE hStdIn;
  DWORD dwInputEvents;
  COORD coordStart, coordEnd, coordDest = {0, 0};
  BOOL bDragMode = FALSE; /* mode flag - selecting (dragging) or not? */
  COORD coordPrev; /* coordinates of the start of the selected area */
  CHAR_INFO *pCharInfo; /* buffer to store info from ReadConsoleOutput */
  COORD coordBufSize; /* the size of the buffer to read into */
  SMALL_RECT srReadRegion; /* source rectangle to read from */
  WORD top, left, right, bottom; /* sides of the source rectangle */
  CONSOLE_SCREEN_BUFFER_INFO csbi; /* used to get cursor position */

  setConTitle(__FILE__);
  myPuts(hConOut, "Let's implement a simple cut & paste with the left mouse\n"
                  "button. Click and drag to select an area of the screen\n"
                  "to copy. We will read, invert, then write the screen attributes\n"
                  "of the area you are dragging over to show the current\n"
                  "selection. Release the mouse button and I will read the\n"
                  "screen contained in the output area and print the contents\n"
                  "of the buffer to the screen at the current cursor location.\n"
                  "Hit ESC to return.\n\n");
  hStdIn = GetStdHandle(STD_INPUT_HANDLE);
  PERR(hStdIn != INVALID_HANDLE_VALUE, "GetStdHandle");
  /* save the console mode */
  bSuccess = GetConsoleMode(hStdIn, &dwStdInMode);
  PERR(bSuccess, "GetConsoleMode");
  /* enable mouse input */
  bSuccess = SetConsoleMode(hStdIn, dwStdInMode | ENABLE_MOUSE_INPUT);
  PERR(bSuccess, "SetConsoleMode");
  for(;;)
    {
    /* get an input event */
    bSuccess = ReadConsoleInput(hStdIn, &inputBuffer, 1, &dwInputEvents);
    PERR(bSuccess, "ReadConsoleInput");
    switch (inputBuffer.EventType)
      {
      case KEY_EVENT:
        /* is it an ESC character? */
        if (inputBuffer.Event.KeyEvent.bKeyDown &&
            inputBuffer.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
          {
          /* set input mode back to what it was originally and return */
          bSuccess = SetConsoleMode(hStdIn, dwStdInMode);
          PERR(bSuccess, "SetConsoleMode");
          return;
          }
        break;
      case MOUSE_EVENT:
        /* was this was a click event? */
        if (inputBuffer.Event.MouseEvent.dwEventFlags != MOUSE_MOVED)
          /* is only leftmost button down? */
          {
          if(inputBuffer.Event.MouseEvent.dwButtonState ==
              FROM_LEFT_1ST_BUTTON_PRESSED)
            {
            /* save the starting coordinates of the selected area */
            coordStart = coordPrev =
                inputBuffer.Event.MouseEvent.dwMousePosition;
            /* invert the single char attribute the cursor is on */
            invertRectangle(hConOut, coordStart, coordStart);
            bDragMode = TRUE; /* we are now in 'drag' mode */
            }
          else /* button #1 has just been released */
            {
            /* if we're not in drag mode, break; we might get a mouse */
            /* button release when gaining the focus */
            if (!bDragMode)
              break;
            /* save the coordinates of the mouse release */
            coordEnd = inputBuffer.Event.MouseEvent.dwMousePosition;
            bDragMode = FALSE; /* we're not in 'drag' mode anymore */
            /* invert the rectangle back to normal */
            invertRectangle(hConOut, coordStart, coordEnd);
            /* copy the buffer in the rectangle and output it. First convert */
            /* coordSart to upper left & coordCur to lower right */
            top = min(coordStart.Y, coordEnd.Y);
            left = min(coordStart.X, coordEnd.X);
            bottom = max(coordStart.Y, coordEnd.Y);
            right = max(coordStart.X, coordEnd.X);
            coordStart.X = srReadRegion.Left = left;
            coordStart.Y = srReadRegion.Top = top;
            coordEnd.X = srReadRegion.Right = right;
            coordEnd.Y = srReadRegion.Bottom = bottom;
            /* get the size of the selected rectangle */
            coordBufSize.X = coordEnd.X - coordStart.X + (SHORT) 1;
            coordBufSize.Y = coordEnd.Y - coordStart.Y + (SHORT) 1;
            /* allocate a buffer for the screen rectangle */
            pCharInfo = malloc(coordBufSize.X * coordBufSize.Y *
                sizeof(CHAR_INFO));
            PERR(pCharInfo, "malloc");
            /* read the screen rectangle into the buffer */
            bSuccess = ReadConsoleOutput(hConOut, /* output buffer */
                pCharInfo, /* buffer that receives data */
                coordBufSize, /* col/row size of destination buffer */
                coordDest, /* upper-left cell to write data to in dest */
                &srReadRegion); /* screen buffer rectangle to read from */
            PERR(bSuccess, "ReadConsoleOutput");
            /* get current cursor position */
            bSuccess = GetConsoleScreenBufferInfo(hConOut, &csbi);
            PERR(bSuccess, "GetConsoleScreenBufferInfo");
            /* set up an output destination rectangle for the buffer. */
            /* Put it just below the current cursor position. */
            srReadRegion.Top = csbi.dwCursorPosition.Y;
            srReadRegion.Bottom = csbi.dwCursorPosition.Y + (bottom - top);
            srReadRegion.Left = 0;
            srReadRegion.Right = right - left;
            bSuccess = WriteConsoleOutput(hConOut, /* output buffer */
                pCharInfo, /* buffer with data to write */
                coordBufSize, /* col/row size of source buffer */
                coordDest, /* upper-left cell to write data from in src */
                &srReadRegion); /* screen buffer rect to write data to */
            PERR(bSuccess, "WriteConsoleOutput");
            free(pCharInfo);
            }
          }
        /* is it a mouse movement while we are in drag mode? */
        if (inputBuffer.Event.MouseEvent.dwEventFlags == MOUSE_MOVED &&
            bDragMode)
          {
          /* if the cursor moves from the rectangle it was previously in, */
          /* invert the old rectangle then invert the new rectangle. */
          /* Compare the current coordinates with the previous coordinates */
          if (memcmp(&inputBuffer.Event.MouseEvent.dwMousePosition,
              &coordPrev, sizeof(COORD)))
            {
            /* You could be clever and only invert what needs to be */
            /* inverted rather than the entire two rectangles... */
            /* Invert the old rectangle */
            invertRectangle(hConOut, coordStart, coordPrev);
            /* and invert the new rectangle */
            invertRectangle(hConOut, coordStart,
                inputBuffer.Event.MouseEvent.dwMousePosition);
            /* save the current mouse position to the previous position */
            memcpy(&coordPrev,
                &inputBuffer.Event.MouseEvent.dwMousePosition,
                sizeof(COORD));
            }
          }
        break;
      } /* switch */
    } /* while */
}
