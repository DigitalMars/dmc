
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
#include "console.h"

HANDLE hConsole; /* current console output handle */

/*******************************************************************
* FUNCTION: handler_routine(DWORD dwCtrlType)                      *
*                                                                  *
* PURPOSE: this is the control handler routine activated by ctrl+c *
*          or ctrl+break keys                                      *
*                                                                  *
* INPUT: the type of control event                                 *
*******************************************************************/

BOOL WINAPI handler_routine(DWORD dwCtrlType)
{
  CHAR szTemp[64];

  /* print out what control event was received to the current console */
  switch(dwCtrlType)
    {
    case CTRL_C_EVENT:
      strcpy(szTemp, "CTRL_C_EVENT");
      break;
    case CTRL_BREAK_EVENT:
      strcpy(szTemp, "CTRL_BREAK_EVENT");
      break;
    case CTRL_CLOSE_EVENT:
      strcpy(szTemp, "CTRL_CLOSE_EVENT");
      break;
    case CTRL_LOGOFF_EVENT:
      strcpy(szTemp, "CTRL_LOGOFF_EVENT");
      break;
    case CTRL_SHUTDOWN_EVENT:
      strcpy(szTemp, "CTRL_SHUTDOWN_EVENT");
      break;
    default:
      strcpy(szTemp, "unknown event");
      break;
    }
  strcat(szTemp, " detected");
  myPuts(hConsole, szTemp);
  return(TRUE);
}


/*********************************************************************
* FUNCTION: demoSetCtrlHandler(HANDLE hConOut)                       *
*                                                                    *
* PURPOSE: demonstrate SetConsoleCtrlHandler by setting a ctrl+break *
*          and ctrl+c handler. When the user hits either one of      *
*          these keys, a message is printed to the console           *
*          indicating the event.                                     *
*                                                                    *
* INPUT: the output handle to write to                               *
*********************************************************************/

void demoSetCtrlHandler(HANDLE hConOut)
{
  BOOL bSuccess;

  setConTitle(__FILE__);
  hConsole = hConOut; /* set global console output handle for handler */
  myPuts(hConOut, "Let's install a ctrl+c and ctrl+break handler for this\n"
                  "process. Hit ctrl+c and ctrl+break a few times to test\n"
                  "the handler. Hit enter to return...");
  /* set handler for this process */
  bSuccess = SetConsoleCtrlHandler(handler_routine, TRUE);
  PERR(bSuccess, "SetConsoleCtrlHandler");
  /* wait for user to hit enter */
  while (myGetchar() != 0xd)
    ;
  /* now let's generate some control events */
  myPuts(hConOut, "Now we'll use GenerateConsoleCtrlEvent to generate a\n"
                  "ctrl+c and a ctrl+break event...\n");
  bSuccess = GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);
  PERR(bSuccess, "GenerateConsoleCtrlEvent");
  bSuccess = GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, 0);
  PERR(bSuccess, "GenerateConsoleCtrlEvent");
  Sleep(1000); /* give ctrl handle time to output messages */
  myPuts(hConOut, "\nNow choose 'Close' then 'Cancel' from the system\n"
                  "menu of this console and note that we receive a\n"
                  "CTRL_CLOSE_EVENT...\n");
  myPuts(hConOut, "\nHit enter to continue...");
  myGetchar();
  /* remove our handler from the list of handlers */
  bSuccess = SetConsoleCtrlHandler(handler_routine, FALSE);
  PERR(bSuccess, "SetConsoleCtrlHandler");
  return;
}
