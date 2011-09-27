
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/* Standard output handle/standard error handle redirection test
program */

#include <stdio.h>
#include <io.h>
#include <string.h>
#include <windows.h>

/* This function opens up a handle to CONOUT$ so that we can be
guaranteed to get error messages to the console even if the standard
handles are redirected. */

void myError(HANDLE h, char *api, int line)
{
  char buf[512];
  BOOL fSuccess;
  DWORD cBytesWritten;
  HANDLE hConout;

  sprintf(buf, "%s: Error %d from %s on line %d\n", __FILE__, GetLastError(),
      api, line);
  fSuccess = WriteFile(h, buf, strlen(buf), &cBytesWritten, NULL);
  if (!fSuccess)
    {
    hConout = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL);
    sprintf(buf, "%s: Error %d from %s on line %d\n", __FILE__,
      GetLastError(), api, line);
    WriteFile(hConout, buf, strlen(buf), &cBytesWritten, NULL);
    CloseHandle(hConout);
    }
  return;
}

/* get the standard output handle and standard error handle and
write a message to each */

int main()
{
  DWORD cBytesWritten;
  HANDLE hStdErr, hStdOut, hConout;
  BOOL fSuccess;
  char *stdErrMsg = "Hello from child to standard error!\n";
  char *stdOutMsg = "Hello from child to standard output!\n";

  hConout = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE,
      FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL, NULL);
  if ((int) hConout == -1)
    Beep(5000,500); /* no reliable way of reporting errors to the console */
                    /* if we can't open a console handle */
  hStdErr = GetStdHandle(STD_ERROR_HANDLE);
  if (hStdErr == INVALID_HANDLE_VALUE)
    myError(hConout, "GetStdHandle", __LINE__);
  hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hStdOut == INVALID_HANDLE_VALUE)
    myError(hConout, "GetStdHandle", __LINE__);
  fSuccess = WriteFile(hStdErr, stdErrMsg, strlen(stdErrMsg),
      &cBytesWritten, NULL);
  if (!fSuccess)
    myError(hConout, "WriteFile", __LINE__);
  fSuccess = WriteFile(hStdOut, stdOutMsg, strlen(stdOutMsg),
      &cBytesWritten, NULL);
  if (!fSuccess)
    myError(hConout, "WriteFile", __LINE__);
  CloseHandle(hConout);
  return(0);
}
