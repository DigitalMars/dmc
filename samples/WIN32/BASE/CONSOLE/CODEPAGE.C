
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
#include <stdlib.h>
#include <stdio.h>
#include "console.h"

/* Note: demos for Get/SetConsoleCP not implemented */

/*********************************************************************
* FUNCTION: showCP(HANDLE hConOut, UINT uiCP)                        *
*                                                                    *
* PURPOSE: dump the 256 character set in the given codepage          *
*                                                                    *
* INPUT: the console output handle to use and the desired code page  *
*        to display the characters in                                *
*********************************************************************/

void showCP(HANDLE hConOut, UINT uiCP)
{
  BOOL bSuccess;
  DWORD dwSaveMode; /* store the current console mode to restore later */
  UINT uiSaveCP; /* store the current code page to restore later */
  int i;
  DWORD dwWritten;
  
  /* save the current output mode and code page */
  uiSaveCP = GetConsoleOutputCP();
  bSuccess = GetConsoleMode(hConOut, &dwSaveMode);
  PERR(bSuccess, "GetconsoleMode");
  /* turn off processed output so that all chars will display */
  bSuccess = SetConsoleMode(hConOut, dwSaveMode & ~ENABLE_PROCESSED_OUTPUT);
  PERR(bSuccess, "SetConsoleMode");
  bSuccess = SetConsoleOutputCP(uiCP);
  PERR(bSuccess, "SetConsoleOutputCP");
  for (i = 0; i < 256; i++)
    WriteConsole(hConOut, &i, 1, &dwWritten, NULL);
  /* now reset to original code page and output mode */
  SetConsoleOutputCP(uiSaveCP);
  bSuccess = SetConsoleMode(hConOut, dwSaveMode);
  PERR(bSuccess, "SetConsoleMode");
  return;
}

/*********************************************************************
* FUNCTION: demoOutputCodePage(HANDLE hConOut)                       *
*                                                                    *
* PURPOSE: demonstrate SetConsoleOutputCP and GetConsoleOutputCP.    *
*          Dump the 256 character set for each code paged installed  *
*          in the system.                                            *
*                                                                    *
* INPUT: the console output handle to manipulate the codepage for    *
*********************************************************************/

void demoOutputCodePage(HANDLE hConOut)
{
  DWORD iValue = 0; /* index value for RegEnumValue */
  LONG lSuccess; /* registry API return code */
  HKEY hKey; /* handle to registry key containing codepage info */
  char szValue[16], szData[16]; /* value and data info from registry */
  DWORD dwType, cchValue, cbData;
  UINT uiCP; /* the code page number that we extract from the registry */
  char szTemp[128];
  BOOL bSuccess;

  setConTitle(__FILE__);
  /* set the color for future text output */
  bSuccess = SetConsoleTextAttribute(hConOut, FOREGROUND_YELLOW |
      FOREGROUND_INTENSITY | BACKGROUND_BLUE);
  PERR(bSuccess, "SetConsoleTextAttribute");
  cls(hConOut); /* clear screen to new attributes */
  myPuts(hConOut, "Warning: code page info will not display properly unless you are\n"
         "using a TrueType font in the console. From the system menu, select\n"
         "Fonts, then select a TrueType font from the list available.");
  /* open the key containing all the code page info */
  lSuccess = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
    "SYSTEM\\CurrentControlSet\\Control\\Nls\\CodePage", 0, KEY_QUERY_VALUE,
    &hKey);
  PERR(lSuccess == ERROR_SUCCESS, "RegOpenKeyEx");
  while(TRUE)
  {
    cchValue = sizeof(szValue);
    cbData = sizeof(szData);
    /* get the next code page entry */
    lSuccess = RegEnumValue(hKey, iValue++, szValue, &cchValue, NULL,
      &dwType, szData, &cbData);
    if (lSuccess == ERROR_NO_MORE_ITEMS)
      break;
    if (!szData[0]) /* if value data is NULL, it's an invalid code page; skip */
      continue;
    if (!(uiCP = atoi(szValue))) /* if value name is not a number, skip */
      continue;
    sprintf(szTemp, "\n\nCodepage: %s", szValue);
    myPuts(hConOut, szTemp);
    /* set code page and display all 256 chars on the console */
    showCP(hConOut, uiCP);
  }
  myPuts(hConOut, "\n\nHit enter to continue...");
  myGetchar();
  return;
}


/*********************************************************************
* FUNCTION: demoInputCodePage(HANDLE hConOut)                        *
*                                                                    *
* PURPOSE: demonstrate SetConsoleInputCP and GetConsoleInputCP.      *
*          Read and display keyboard input, including special chars. *
*                                                                    *
* INPUT: the console output handle to manipulate the codepage for    *
*********************************************************************/

void demoInputCodePage(HANDLE hConOut)
{
  myPuts(hConOut, "SetConsoleCP and GetConsoleCP demos");
  myPuts(hConOut, "not implemented yet.");
  myPuts(hConOut, "\n\nHit enter to continue...");
  myGetchar();
  return;
}

