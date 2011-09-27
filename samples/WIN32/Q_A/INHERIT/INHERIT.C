
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/********************************************************************
* This program is to demonstrate the use of anonymous pipes used as *
* stdout and stderr replacements. One of two techniques can be      *
* chose to do this: the SetStdHandle technique or the               *
* STARTF_USESTDHANDLES technique. The SetStdHandle technique sets   *
* the standard output handles to the pipe that we will read from,   *
* which the child will inherit. The STARTF_USESTDHANDLES technique  *
* passes the pipe handles to the child as standard handles via the  *
* STARTUPINFO structure. The STARTF_USESTDHANDLES technique *must*  *
* be used for "console-less" processes such as GUI applications or  *
* detached processes.                                               *
*                                                                   *
* This program demonstrates the use of the following Win32 APIs:    *
*   CreatePipe, CreateProcess.                                      *
*                                                                   *
* This program also uses the following Win32 APIs:                  *
*   GetLastError, CreateFile, CloseHandle, CreateProcess, ReadFile, *
*   WriteFile.                                                      *
*                                                                   *
*                                                                   *
* Execution instructions:                                           *
*                                                                   *
*   inherit <trace file> <command to execute>                       *
*   trace file is the name of the file where the stdout             *
*     and stderr of command will be redirected                      *
*                                                                   *
*   command to execute can be either an external executable or an   *
*     internal cmd.exe command.                                     *
*                                                                   *
*   Examples:                                                       *
*                                                                   *
*   inherit chkdsk.dat chkdsk d:                                    *
*   inherit nmake.txt nmake /f foo.mak                              *
*                                                                   *
********************************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* define USESTDHANDLES to use the new technique of passing the
standard handles to the child process via the STARTUPINFO structure.
This technique must be used for "console-less" parents such as GUI
applications or detached applications. */

#define USESTDHANDLES

/* Standard error macro for reporting API errors */
#define PERR(bSuccess, api) {if (!(bSuccess)) printf("%s: Error %d from %s \
    on line %d\n", __FILE__, GetLastError(), api, __LINE__);}

int main(int argc, char *argv[])
{
  char chReadBuffer[64];  /* pipe read buffer */
  BOOL bSuccess;  /* BOOL return code for APIs */
  int j;
  HANDLE hOutFile;  /* handle to log file */
  /* handles to the anonymous pipe */
  HANDLE hReadPipe, hWritePipe, hWritePipe2;
  char szArgs[256];  /* child process argument buffer */
  char *p;  /* temporary pointer into szArgs */
  DWORD cchReadBuffer;  /* number of bytes read or to be written */
  STARTUPINFO si;  /* for CreateProcess call */
  PROCESS_INFORMATION pi;  /* for CreateProcess call */
  SECURITY_ATTRIBUTES saPipe;  /* security for anonymous pipe */

  // check if running on Windows NT, if not, display notice and terminate
  if( GetVersion() & 0x80000000 )
  {
       MessageBox( NULL,
       "This sample application can only be run on Windows NT.\n"
       "This application will now terminate.",
       "Inherit",
       MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
    return( 1 );
  }

  if (argc < 3)
    {
    puts("format: inherit <trace file> <command to execute>");
    puts("trace file is the name of the file where the stdout");
    puts("and stderr of command will be redirected\n");
    puts("command to execute is command line of the function");
    puts("you wish to perform.\n");
    puts("Examples:\n");
    puts("  inherit trace.txt chkdsk d:");
    puts("  inherit trace.txt nmake /f foo.mak");
    return(1);
    }

  /* create the log file where we will save all output from child */
  hOutFile = CreateFile(argv[1],  /* file to open */
      GENERIC_WRITE,  /* access mode */
      FILE_SHARE_READ,  /* share mode */
      NULL,  /* security attributes */
      CREATE_ALWAYS,  /* creation flags - trash existing file */
      FILE_ATTRIBUTE_NORMAL,  /* file attributes */
      NULL);
  PERR(hOutFile != INVALID_HANDLE_VALUE, "CreateFile");

  /* set up the security attributes for the anonymous pipe */
  saPipe.nLength = sizeof(SECURITY_ATTRIBUTES);
  saPipe.lpSecurityDescriptor = NULL;
  /* In order for the child to be able to write to the pipe, the handle */
  /* must be marked as inheritable by setting this flag: */
  saPipe.bInheritHandle = TRUE;

  /* create the anonymous pipe */
  bSuccess = CreatePipe(&hReadPipe,  /* read handle */
      &hWritePipe,  /* write handle, used as stdout by child */
      &saPipe,  /* security descriptor */
      0);  /* pipe buffer size */
  PERR(bSuccess, "CreatePipe");

  /* Now we need to change the inheritable property for the readable
  end of the pipe so that the child will not inherit that handle as
  a "garbage" handle. This will keep us from having extra,
  unclosable handles to the pipe. Alternatively, we could have
  opened the pipe with saPipe.bInheritHandle = FALSE and changed the
  inherit property on the *write* handle of the pipe to TRUE. */

  bSuccess = DuplicateHandle(GetCurrentProcess(), /* source process */
      hReadPipe, /* handle to duplicate */
      GetCurrentProcess(), /* destination process */
      NULL, /* new handle - don't want one, change original handle */
      0, /* new access flags - ignored since DUPLICATE_SAME_ACCESS */
      FALSE, /* make it *not* inheritable */
      DUPLICATE_SAME_ACCESS);
  PERR(bSuccess, "DuplicateHandle");

  /* I most cases you can get away with using the same anonymous
  pipe write handle for both the child's standard output and
  standard error, but this may cause problems if the child app
  explicitly closes one of its standard output or error handles. If
  that happens, the anonymous pipe will close, since the child's
  standard output and error handles are really the same handle. The
  child won't be able to write to the other write handle since the
  pipe is now gone, and parent reads from the pipe will return
  ERROR_BROKEN_PIPE and child output will be lost. To solve this
  problem, simply duplicate the write end of the pipe to create
  another distinct, separate handle to the write end of the pipe.
  One pipe write handle will serve as standard out, the other as
  standard error. Now *both* write handles must be closed before the
  write end of the pipe actually closes. */

  bSuccess = DuplicateHandle(GetCurrentProcess(), /* source process */
      hWritePipe, /* handle to duplicate */
      GetCurrentProcess(), /* destination process */
      &hWritePipe2, /* new handle, used as stderr by child */
      0, /* new access flags - ignored since DUPLICATE_SAME_ACCESS */
      TRUE, /* it's inheritable */
      DUPLICATE_SAME_ACCESS);
  PERR(bSuccess, "DuplicateHandle");

  /* Set up the STARTUPINFO structure for the CreateProcess() call */
  memset(&si, 0, sizeof(si));
  si.cb = sizeof(si);

  /* Set up the command-line buffer for the child for CreateProcess() */
  memset(szArgs, 0, sizeof(szArgs));
  strcpy(szArgs, argv[2]);
  if (strchr(szArgs, '.') == NULL)  /* does it have a '.'? */
    strcat(szArgs, ".exe");  /* if not, assume it's an .exe */
  strcat(szArgs, " ");
  p = strchr(szArgs, 0);  /* point to the terminating null */
  for (j = 3; j < argc; j++)
    {
    strcat(p, argv[j]);
    /* the program and parameters are delimited by spaces */
    strcat(p, " ");
    }

#ifdef USESTDHANDLES
  /* If using the STARTUPINFO STARTF_USESTDHANDLES flag, be sure to
  set the CreateProcess fInheritHandles parameter too TRUE so that
  the file handles specified in the STARTUPINFO structure will be
  inheritied by the child. Note that we don't specify a standard
  input handle; the child will not inherit a valid input handle, so
  if it reads from stdin, it will encounter errors. */

  si.hStdOutput = hWritePipe; /* write end of the pipe */
  si.hStdError = hWritePipe2; /* duplicate of write end of the pipe */
  si.dwFlags = STARTF_USESTDHANDLES;
#else
  /* If we're not using the STARTF_USESTDHANDLES flag, set the
  standard output and error handles to the end of the pipe we want
  the child to inherit with SetStdHandle(). For this program, we
  don't want standard input inherited so we'll also change the
  handle inheritance property of standard input so that it is not
  inherited */

  bSuccess = SetStdHandle(STD_OUTPUT_HANDLE, hWritePipe);
  PERR(bSuccess, "SetStdHandle");
  bSuccess = SetStdHandle(STD_ERROR_HANDLE, hWritePipe2);
  PERR(bSuccess, "SetStdHandle");
  bSuccess = DuplicateHandle(GetCurrentProcess(), /* source process */
      GetStdHandle(STD_INPUT_HANDLE), /* handle to duplicate */
      GetCurrentProcess(), /* destination process */
      NULL, /* new handle - don't want one, change original handle */
      0, /* new access flags - ignored since DUPLICATE_SAME_ACCESS */
      FALSE, /* it's *not* inheritable */
      DUPLICATE_SAME_ACCESS);
  PERR(bSuccess, "DuplicateHandle");
#endif

  /* Now create the child process, inheriting handles */

  bSuccess = CreateProcess(NULL,  /* filename */
      szArgs,  /* full command line for child */
      NULL,  /* process security descriptor */
      NULL,  /* thread security descriptor */
      TRUE,  /* inherit handles? Also use if STARTF_USESTDHANDLES */
      0,  /* creation flags */
      NULL,  /* inherited environment address */
      NULL,  /* startup dir; NULL = start in current */
      &si,  /* pointer to startup info (input) */
      &pi);  /* pointer to process info (output) */
  PERR(bSuccess, "CreateProcess");

  /* We can close the returned child process handle and thread
  handle as we won't be needing them; you could, however, wait on
  the process handle to wait until the child process terminates. */

  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);

  /* We need to close our instances of the inheritable pipe write
  handle now that it's been inherited so that all open handles to
  the pipe are closed when the child process ends and closes its
  handles to the pipe. */

  bSuccess = CloseHandle(hWritePipe);
  PERR(bSuccess, "CloseHandle");
  bSuccess = CloseHandle(hWritePipe2);
  PERR(bSuccess, "CloseHandle");

  /* read from the pipe until we get an ERROR_BROKEN_PIPE */
  for (;;)
    {
    bSuccess = ReadFile(hReadPipe,  /* read handle */
        chReadBuffer,  /* buffer for incoming data */
        sizeof(chReadBuffer),  /* number of bytes to read */
        &cchReadBuffer,  /* number of bytes actually read */
        NULL);  /* no overlapped reading */
    if (!bSuccess && (GetLastError() == ERROR_BROKEN_PIPE))
      break;  /* child has died */
    PERR(bSuccess, "ReadFile");
    if (bSuccess && cchReadBuffer)
      {
      /* write the data from the child to the file */
      bSuccess = WriteFile(hOutFile,  /* write handle */
          chReadBuffer,  /* buffer to write */
          cchReadBuffer,  /* number of bytes to write */
          &cchReadBuffer,  /* number of bytes actually written */
          NULL);  /* no overlapped writing */
      PERR(bSuccess, "WriteFile");

      /* write buffer (of specified length) to console */
      printf("%.*s", cchReadBuffer, chReadBuffer);
      }
    }
  /* close the trace file, pipe handles */
  CloseHandle(hOutFile);
  CloseHandle(hReadPipe);
  return(0);
}
