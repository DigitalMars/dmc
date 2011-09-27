
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

/*********************************************************************
* FUNCTION: rawOut(HANDLE hConsole, PCHAR s)                         *
*                                                                    *
* PURPOSE: output the contents of the input buffer to the console    *
*          handle in 'raw' mode                                      *
*                                                                    *
* INPUT: the console handle to write to, and the string to output in *
*        'raw' mode                                                  *
*********************************************************************/

void rawOut(HANDLE hConsole, PCHAR s)
{
  BOOL bSuccess;
  DWORD cCharsWritten;
  DWORD dwOutputMode;

  bSuccess = GetConsoleMode(hConsole, &dwOutputMode);
  PERR(bSuccess, "GetconsoleMode");
  /* output the string in raw mode. */
  /* turn off processed output, output the string, and reset output mode */
  bSuccess = SetConsoleMode(hConsole, dwOutputMode & ~ENABLE_PROCESSED_OUTPUT);
  PERR(bSuccess, "SetConsoleMode");
  bSuccess = WriteFile(hConsole, s, strlen(s), &cCharsWritten, NULL);
  PERR(bSuccess, "WriteFile");
  /* now reset output mode */
  bSuccess = SetConsoleMode(hConsole, dwOutputMode);
  PERR(bSuccess, "SetConsoleMode");
  return;
}

/******************************************************************
* FUNCTION: demoConMode(HANDLE hConOut)                           *
*                                                                 *
* PURPOSE: demonstrate GetConsoleMode and SetConsoleMode. Display *
*          the current console input and output modes, then       *
*          demonstrate each console input and output mode         *
*                                                                 *
*                                                                 *
* INPUT: the console output handle to write to                    *
******************************************************************/

void demoConMode(HANDLE hConOut)
{
  BOOL bSuccess;
  DWORD dwOutputMode, dwInputMode; /* save input & output console modes */
  CHAR szTemp[128];
  HANDLE hStdIn; /* standard input handle */
  /* a string to test 'raw' vs. 'processed' output with */
  PCHAR szModeTst = "tab:\t backspace:\b linefeed:\n bell:\a cr:\r";
  DWORD dwRead;
  CHAR chBuf[256]; /* buffer to read a user string from the console */
  PCHAR szLong = "This is a line of text that is greater than 80 characters wide. This is a test of ENABLE_WRAP_AT_EOL_OUTPUT.";

  setConTitle(__FILE__);
  hStdIn = GetStdHandle(STD_INPUT_HANDLE);
  PERR(hStdIn != INVALID_HANDLE_VALUE, "GetStdHandle");
  /* get the input mode and save it so that we can restore it later */
  bSuccess = GetConsoleMode(hStdIn, &dwInputMode);
  PERR(bSuccess, "GetconsoleMode");
  myPuts(hConOut, "Current console input modes, as reported by GetconsoleMode:");
  sprintf(szTemp, "ENABLE_LINE_INPUT: %s", dwInputMode & ENABLE_LINE_INPUT ?
      "on" : "off");
  myPuts(hConOut, szTemp);
  sprintf(szTemp, "ENABLE_ECHO_INPUT: %s", dwInputMode & ENABLE_ECHO_INPUT ?
      "on" : "off");
  myPuts(hConOut, szTemp);
  sprintf(szTemp, "ENABLE_WINDOW_INPUT: %s", dwInputMode &
      ENABLE_WINDOW_INPUT ? "on" : "off");
  myPuts(hConOut, szTemp);
  sprintf(szTemp, "ENABLE_PROCESSED_INPUT: %s", dwInputMode &
      ENABLE_PROCESSED_INPUT ? "on" : "off");
  myPuts(hConOut, szTemp);
  bSuccess = GetConsoleMode(hConOut, &dwOutputMode);
  PERR(bSuccess, "GetconsoleMode");
  myPuts(hConOut, "\nCurrent console output modes:");
  sprintf(szTemp, "ENABLE_PROCESSED_OUTPUT: %s", dwOutputMode &
      ENABLE_PROCESSED_OUTPUT ? "on" : "off");
  myPuts(hConOut, szTemp);
  sprintf(szTemp, "ENABLE_WRAP_AT_EOL_OUTPUT: %s", dwOutputMode &
      ENABLE_WRAP_AT_EOL_OUTPUT ? "on" : "off");
  myPuts(hConOut, szTemp);
  bSuccess = SetConsoleMode(hConOut, dwOutputMode);  /* back to normal */
  PERR(bSuccess, "SetConsoleMode");
  myPuts(hConOut, "\nHit enter to continue...");
  myGetchar();
  cls(hConOut);
  myPuts(hConOut, "\nLet's test the console output modes with SetConsoleMode.\n"
                  "First we'll output a long line of text with\n"
                  "ENABLE_WRAP_AT_EOL_OUTPUT enabled, then with it disabled:\n");
  myPuts(hConOut, szLong);
  myPuts(hConOut, "\nNow we'll disable ENABLE_WRAP_AT_EOL_OUTPUT and display the same\n"
                  "string. Note how the wrapped line does NOT get displayed\n"
                  "on the next line but on the same line:\n");
  /* turn off EOL wrap */
  bSuccess = SetConsoleMode(hConOut, dwOutputMode &
      ~ENABLE_WRAP_AT_EOL_OUTPUT);
  PERR(bSuccess, "SetConsoleMode");
  myPuts(hConOut, szLong);
  /* turn on EOL wrap */
  bSuccess = SetConsoleMode(hConOut, dwOutputMode |
      ENABLE_WRAP_AT_EOL_OUTPUT);
  PERR(bSuccess, "SetConsoleMode");
  myPuts(hConOut, "\n\nNow let's test processed output. We'll output a string\n"
                  "with ENABLE_PROCESSED_OUTPUT disabled, then with it enabled.\n"
                  "Note how the backspace, tab, bell, carriage return, and\n"
                  "linefeed characters are processed differently:\n");
  /* turn off processed ("cooked") output - now in "raw" mode */
  bSuccess = SetConsoleMode(hConOut, dwOutputMode & ~ENABLE_PROCESSED_OUTPUT);
  PERR(bSuccess, "SetConsoleMode");
  /* myPuts() appends a \n, so can't use it for this case */
  bSuccess = WriteFile(hConOut, szModeTst, strlen(szModeTst), &dwRead, NULL);
  PERR(bSuccess, "WriteFile");
  /* turn processed ("cooked") mode back on */
  bSuccess = SetConsoleMode(hConOut, dwOutputMode | ENABLE_PROCESSED_OUTPUT);
  PERR(bSuccess, "SetConsoleMode");
  myPuts(hConOut, "\nNow we'll set ENABLE_PROCESSED_OUTPUT and output the same\n"
                  "string:\n");
  myPuts(hConOut, szModeTst);
  myPuts(hConOut, "\nHit enter for the next test...");
  myGetchar();
  cls(hConOut);
  myPuts(hConOut, "Now let's test the input modes. First you'll input\n"
                  "an input string with ENABLE_PROCESSED_INPUT enabled.\n"
                  "We'll output exactly what was read in this mode.\n"
                  "Note how characters such as backspace, tab, ctrl+g,\n"
                  "carriage return and line feed are interpreted.\n"
                  "Enter a line of text, end by hitting enter:");
  /* turn on processed, line, and echo modes. */
  /* MUST turn on echo mode when turning on line mode */
  bSuccess = SetConsoleMode(hStdIn, dwInputMode | ENABLE_PROCESSED_INPUT |
      ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
  PERR(bSuccess, "SetConsoleMode");
  memset(chBuf, 0, sizeof(chBuf));
  bSuccess = ReadFile(hStdIn, chBuf, sizeof(chBuf), &dwRead, NULL);
  PERR(bSuccess, "ReadFile");
  /* output the contents of chBuf in raw mode */
  rawOut(hConOut, chBuf);
  myPuts(hConOut, "\nNow let's disable ENABLE_PROCESSED_INPUT. Enter another\n"
                  "input string:");
  /* turn off processed, line, and echo input. */
  /* MUST turn off echo input when turning off line input */
  bSuccess = SetConsoleMode(hStdIn, dwInputMode & ~ENABLE_PROCESSED_INPUT |
      ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
  PERR(bSuccess, "SetConsoleMode");
  memset(chBuf, 0, sizeof(chBuf));
  bSuccess = ReadFile(hStdIn, chBuf, sizeof(chBuf), &dwRead, NULL);
  PERR(bSuccess, "ReadFile");    
  rawOut(hConOut, chBuf);
  myPuts(hConOut, "\nHit enter to continue...");
  myGetchar();
  cls(hConOut);
  myPuts(hConOut, "\n\nLet's turn off ENABLE_LINE_INPUT - all of our input\n"
                  "previously has been line-input. We must also disable\n"
                  "ENABLE_ECHO_INPUT when disabling this flag. Let's read\n"
                  "a single character from the console and we will return\n"
                  "immediately with the charater read. Note that the character\n"
                  "does not echo. Hit any key:");
  bSuccess = SetConsoleMode(hStdIn, dwInputMode & ~ENABLE_LINE_INPUT &
      ~ENABLE_ECHO_INPUT);
  PERR(bSuccess, "SetConsoleMode");
  memset(chBuf, 0, sizeof(chBuf));
    bSuccess = ReadFile(hStdIn, chBuf, sizeof(chBuf), &dwRead, NULL);
  PERR(bSuccess, "ReadFile");    
  myPuts(hConOut, "\nHere is the character read:");
  myPuts(hConOut, chBuf);
  myPuts(hConOut, "\n\nHit enter to continue...");
  bSuccess = SetConsoleMode(hStdIn, dwInputMode | ENABLE_WINDOW_INPUT);
  PERR(bSuccess, "SetConsoleMode");
  myGetchar();
  return;
}
