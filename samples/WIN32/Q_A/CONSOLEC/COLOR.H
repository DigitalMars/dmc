
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/


/************************************************************************\
*                         SYMBOLIC CONSTANTS
\************************************************************************/

#define MAXCOLORS        8
#define BACKGROUND_COLOR 0x00F0

/************************************************************************\
*                          GLOBAL VARIABLES
\************************************************************************/

char* Color[MAXCOLORS] = {
  "BLACK",   /* 0                                                       */
  "BLUE",    /* 1, FOREGROUND_BLUE                                      */
  "GREEN",   /* 2, FOREGROUND_GREEN                                     */
  "CYAN",    /* 3, FOREGROUND_BLUE  | FOREGROUND_GREEN                  */
  "RED",     /* 4, FOREGROUND_RED                                       */
  "MAGENTA", /* 5, FOREGROUND_BLUE  | FOREGROUND_RED                    */
  "YELLOW",  /* 6, FOREGROUND_GREEN | FOREGROUND_RED                    */
  "WHITE",   /* 7, FOREGROUND_BLUE  | FOREGROUND_GREEN | FOREGROUND_RED */
};
/************************************************************************\
 * FOREGROUND_INTENSITY & BACKGROUND_INTENSITY control support
 * could be easily added to this sample
\************************************************************************/


/************************************************************************\
*                         FUNCTION PROTOTYPES
\************************************************************************/

BOOL Syntax( HANDLE hConsole );
WORD GetConsoleTextAttribute( HANDLE hConsole );
