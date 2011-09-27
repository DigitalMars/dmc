/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>
#include <string.h>
#include <winbase.h>

extern HWND ghwndMain;

HANDLE OpenMap(char MapName[30]);
void ErrorOut(char errstring[30]);

/************************************************************************
 * void ErrorOut(char errstring[30])
 *
 * Purpose: Print out an meainful error code by means of
 *        GetLastError and printf
 *
 * Inputs:  errstring - the action that failed, passed by the
 *                    calling proc.
 *
 * Returns: none
 *
 * Calls:   GetLastError
 *
\************************************************************************/


void ErrorOut(char errstring[30])
{
DWORD Error;
CHAR  str[80];

Error= GetLastError();
wsprintf((LPSTR) str, "Error on %s = %d\n", errstring, Error);
MessageBox(ghwndMain, (LPSTR)str, "Error", MB_OK);
}

/*************************************************************************
 * HANDLE OpenMap(char *MapName[30])
 *
 * Purpose: Open the mapping object pointed to by MapName
 *
 * Inputs: none
 *
 * Returns: handle to mapped object or NULL if failure
 *
 * Calls: OpenFileMapping, ErrorOut
 *
 * History:
 * 09-13-91 Pete Gray   Created.
 *
\*************************************************************************/

HANDLE OpenMap(char MapName[30])
{
HANDLE hAMap;
//char MapNameH[30]="MapName1";

hAMap= OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE,
                       TRUE,
                       MapName);

if (hAMap == NULL)
  {
  ErrorOut("OpenFileMapping");
  return(NULL);
  }
else
  return(hAMap);

}
