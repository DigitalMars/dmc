
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

/* number of colors in the color bar */
#define MAXCOLORS 16
/* horizontal size in characters of the sample text area */
#define SAMPTEXTX 33
/* veritcal size in characters of the sample text area */
#define SAMPTEXTY 5

/********************************************************************
* FUNCTION: demoFillAtt(HANDLE hConOut)                             *
*                                                                   *
* PURPOSE: demonstrate FillConsoleOutputAttribute and               *
*          WriteConsoleOutputCharacter. Create a console version of *
*          the "ScreenSize..." menu item from the console system    *
*          menu. Allow the user to select a foreground and          *
*          background color, which will be immediately displayed in *
*          the sample text box                                      *
*                                                                   *
* INPUT: the console output handle to write to                      *
********************************************************************/

void demoFillAtt(HANDLE hConOut)
{
  COORD foregLoc = {5, 5}; /* foreground 'button' location */
  COORD backgLoc = {5, 7}; /* background 'button' location */
  COORD okLoc = {20, 5}; /* ok 'button' location */
  COORD colorbarLoc = {6, 10};  /* loc of first color in colorbar */
  COORD sampleLoc = {5, 13}; /* sample text location */
  COORD dwBufCoord; /* temp COORD structure */
  BOOL bSuccess;
  PCHAR szForeg = "þ Screen Text"; /* foreground 'button' and text */
  PCHAR szBackg = "þ Screen Background"; /* background 'button' */
  PCHAR szOk = "þ OK"; /* OK 'button' */
  /* these strings are the box around the color bar */
  PCHAR szColors1 = "ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿";
  PCHAR szColors2 = "³                                ³";
  PCHAR szColors3 = "ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ";
  WORD wButtonColor; /* holds the button 'color' */
  DWORD dwCharsWritten;
  WORD i;
  INPUT_RECORD inputBuf;
  DWORD cInputEvents;
  BOOL bForeground; /* state flag: foreground button active? */
  COORD wCurPos;
  HANDLE hStdIn;
  /* arraw of attributes to put in the colorbar, each attribute in 2 spaces */
  WORD szAttr[MAXCOLORS * 2];
  PCHAR szSampText = "A console consists of a keyboard and mouse input buffer"
                  " and one or more screen buffers.  \"CONIN$\" refers to the"
                  " input buffer. Stdin is a handle to \"CONIN$\".  \"CONOUT$\""
                  " refers to a screen buffer.";
  PCHAR p;
  WORD wCurSampAttr; /* current attribute of the sample text area */

  setConTitle(__FILE__);
  /* place our "buttons" and text on the screen. First the 'foreground' */
  /* button... */
  bSuccess = WriteConsoleOutputCharacter(hConOut, szForeg, strlen(szForeg),
      foregLoc, &dwCharsWritten);
  /* now the 'background' button */
  PERR(bSuccess, "WriteConsoleOutputCharacter");
  bSuccess = WriteConsoleOutputCharacter(hConOut, szBackg, strlen(szBackg),
      backgLoc, &dwCharsWritten);
  /* place the 'OK' button */
  PERR(bSuccess, "WriteConsoleOutputCharacter");
  bSuccess = WriteConsoleOutputCharacter(hConOut, szOk, strlen(szOk),
      okLoc, &dwCharsWritten);
  PERR(bSuccess, "WriteConsoleOutputCharacter");
  /* now let's draw a boxed colorbar */
  dwBufCoord = colorbarLoc;
  /* move up and left one to draw a box around the colorbar */
  dwBufCoord.X--;
  dwBufCoord.Y--;
  /* the top of the box */
  bSuccess = WriteConsoleOutputCharacter(hConOut, szColors1, strlen(szColors1),
      dwBufCoord, &dwCharsWritten);
  PERR(bSuccess, "WriteConsoleOutputCharacter");
  dwBufCoord.Y++;
  /* the middle of the box */
  bSuccess = WriteConsoleOutputCharacter(hConOut, szColors2, strlen(szColors2),
      dwBufCoord, &dwCharsWritten);
  PERR(bSuccess, "WriteConsoleOutputCharacter");
  dwBufCoord.Y++;
  /* the bottom of the box */
  bSuccess = WriteConsoleOutputCharacter(hConOut, szColors3, strlen(szColors3),
      dwBufCoord, &dwCharsWritten);
  PERR(bSuccess, "WriteConsoleOutputCharacter");

  /* color the "buttons" a different color */
  wButtonColor = FOREGROUND_WHITE | BACKGROUND_WHITE;
  /* color the 'backgroud' button */
  bSuccess = WriteConsoleOutputAttribute(hConOut, &wButtonColor,
      1, backgLoc, &dwCharsWritten);
  PERR(bSuccess, "WriteConsoleOutputAttribute");
  /* color the 'OK' button */
  bSuccess = WriteConsoleOutputAttribute(hConOut, &wButtonColor,
      1, okLoc, &dwCharsWritten);
  PERR(bSuccess, "WriteConsoleOutputAttribute");
  /* color the 'foreground' button, but with a different color */
  wButtonColor = BACKGROUND_WHITE;
  bSuccess = WriteConsoleOutputAttribute(hConOut, &wButtonColor,
      1, foregLoc, &dwCharsWritten);
  PERR(bSuccess, "WriteConsoleOutputAttribute");
  bForeground = TRUE; /* the 'foreground' button is 'highlighted' */

  /* construct the attribute string */
  for (i = 0; i < MAXCOLORS; i++)
    /* for each set of two, the color attributes are in the high byte */
    /* of the word - shift them into the high byte */
    szAttr[i * 2] = szAttr[(i * 2) + 1] = i << 4;
  /* write out the attributes at the colorbar location */
  dwBufCoord = colorbarLoc;
  bSuccess = WriteConsoleOutputAttribute(hConOut, szAttr, MAXCOLORS * 2,
      dwBufCoord, &dwCharsWritten);
  PERR(bSuccess, "WriteConsoleOutputAttribute");

  /* put up a sample text area */
  p = szSampText;
  wCurPos = sampleLoc;
  wCurSampAttr = BACKGROUND_WHITE;
  for (i = 0; i < SAMPTEXTY; i++)
    {
    bSuccess = WriteConsoleOutputCharacter(hConOut, p,
        min(strlen(p), SAMPTEXTX), wCurPos, &dwCharsWritten);
    PERR(bSuccess, "WriteConsoleOutputCharacter");
    /* color the text with the current sample attribute */
    bSuccess = FillConsoleOutputAttribute(hConOut, wCurSampAttr, SAMPTEXTX,
        wCurPos, &dwCharsWritten);
    PERR(bSuccess, "FillConsoleOutputAttribute");
    wCurPos.Y++;
    /* advance pointer to the next row of characters in the sample string */
    p += min(strlen(p), SAMPTEXTX);
    }

  hStdIn = GetStdHandle(STD_INPUT_HANDLE);
  PERR(hStdIn != INVALID_HANDLE_VALUE, "GetStdHandle");
  for(;;)
    {
    /* get an input event */
    bSuccess = ReadConsoleInput(hStdIn, &inputBuf, 1, &cInputEvents);
    /* if it's a mouse event but not a mouse move, it's a click */
    PERR(bSuccess, "ReadConsoleInput");
    if (inputBuf.EventType == MOUSE_EVENT &&
        inputBuf.Event.MouseEvent.dwEventFlags != MOUSE_MOVED)
      {
      wCurPos = inputBuf.Event.MouseEvent.dwMousePosition;
      /* is the mouse on the 'foreground' button? */
      if (wCurPos.X == foregLoc.X && wCurPos.Y == foregLoc.Y)
        {
        bForeground = TRUE;
        /* recolor the background color and foreground color */
        /* of the foreground to show the foreground button active */
        wButtonColor = BACKGROUND_WHITE;
        bSuccess = WriteConsoleOutputAttribute(hConOut, &wButtonColor,
            1, foregLoc, &dwCharsWritten);
        PERR(bSuccess, "WriteConsoleOutputAttribute");
        /* recolor the background color to show as inactive */
        wButtonColor = FOREGROUND_WHITE | BACKGROUND_WHITE;
        bSuccess = WriteConsoleOutputAttribute(hConOut, &wButtonColor,
            1, backgLoc, &dwCharsWritten);
        PERR(bSuccess, "WriteConsoleOutputAttribute");
        }
      /* is the mouse on the 'background' button? */
      if (wCurPos.X == backgLoc.X && wCurPos.Y == backgLoc.Y)
        {
        bForeground = FALSE;
        /* recolor the background button to show as active */
        wButtonColor = BACKGROUND_WHITE;
        bSuccess = WriteConsoleOutputAttribute(hConOut, &wButtonColor,
            1, backgLoc, &dwCharsWritten);
        PERR(bSuccess, "WriteConsoleOutputAttribute");
        /* recolor the foreground button to show as inactive */
        wButtonColor = FOREGROUND_WHITE | BACKGROUND_WHITE;
        bSuccess = WriteConsoleOutputAttribute(hConOut, &wButtonColor,
            1, foregLoc, &dwCharsWritten);
        PERR(bSuccess, "WriteConsoleOutputAttribute");
        }
      /* is the mouse on the 'OK' button? */
      if (wCurPos.X == okLoc.X && wCurPos.Y == okLoc.Y)
        break;
      /* is the mouse in the colorbar? */
      if (wCurPos.Y == colorbarLoc.Y && wCurPos.X >= colorbarLoc.X &&
          wCurPos.X < colorbarLoc.X + (SHORT) (MAXCOLORS * 2))
        {
        /* get the screen attribute at the mouse position */
        bSuccess = ReadConsoleOutputAttribute(hConOut, &wButtonColor,
            sizeof(wButtonColor), wCurPos, &dwCharsWritten);
        PERR(bSuccess, "ReadConsoleOutputAttribute");
        /* if the foreground button is active, set the foreground */
        /* attribute for the sample text */
        if (bForeground)
          /* mask off the foreground color and 'or' it with the color */
          /* we just got from the screen, shifted up to make it a */
          /* foreground color (it's actually a background attribute when */
          /* read from the console buffer). */
          wCurSampAttr = (wCurSampAttr & (WORD) 0xF0) | (wButtonColor >> 4);
        /* otherwise set the background attribute for the sample text */
        else
          /* mask off the background color and 'or'  it with the background */
          /* color we just read from the console. */
          wCurSampAttr = (wCurSampAttr & (WORD) 0x0F) | wButtonColor;
        /* now set the attribute of the sample text to the new attribute */
        wCurPos = sampleLoc;
        for (i = 0; i < SAMPTEXTY; i++)
          {
          bSuccess = FillConsoleOutputAttribute(hConOut, wCurSampAttr,
              SAMPTEXTX, wCurPos, &dwCharsWritten);
          PERR(bSuccess, "FillConsoleOutputAttribute");
          wCurPos.Y++;
          }
        }  /* if */
      }  /* if */
    }  /* while */
  return;
}
