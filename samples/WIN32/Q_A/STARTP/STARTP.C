
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
* This program demonstrates various CreateProcess parameters,       *
* including starting processes in a given priority class. This is   *
* very similar to the "start" command but with added functionality. *
*                                                                   *
* This program demonstrates the use of the following Win32 APIs:    *
*   CreateProcess, TerminateProcess.                                *
*                                                                   *
* This program also uses the following Win32 APIs:                  *
*   WaitForSingleObject GetLastError.                               *
*                                                                   *
* Execution instructions:                                           *
*   see the help() function or run the program without any          *
*   parameters to see detailed execution info.                      *
*                                                                   *
* Possible future enhancements:                                     *
*   Handle cmd.exe internal commands                                *
*                                                                   *
* Version 1.0: NT 3.1 SDK release                                   *
* Version 1.1: NT 3.5 SDK release                                   *
*   Changes: added support for CREATE_SEPARATE_WOW_VDM,             *
*   CREATE_UNICODE_ENVIRONMENT, and CREATE_NEW_PROCESS_GROUP        *
********************************************************************/

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Standard error macro for reporting API errors */
#define PERR(bSuccess, api) {if (!(bSuccess)) printf("%s: Error %d from %s \
    on line %d\n", __FILE__, GetLastError(), api, __LINE__);}

void help()
{
  puts("Starts a specified program, batch, or command file.");
  puts("STARTP [options] [program] [parameters]\n");
  puts("  /Ttitle     Title to display in window title bar. Quote the");
  puts("              entire paramter to include spaces in the title");
  puts("  /Dpath      Starting directory");
  puts("  /l|/h|/r    Set priority to low/high/realtime");
  puts("  /min|/max   Start window minimized/maximized");
  puts("  /w          Wait for started process to end before returning");
  puts("              control to the command processor. This option starts");
  puts("              the process synchronously");
  puts("  /c          Use current console instead of creating a new console");
  puts("  /b          Start detached with no console at all");
  puts("  /p          Create new process group for ^c, ^break events");
  puts("  /s          Run Win16 app in separate VDM");
  puts("  program     A batch file or program to run as either a GUI");
  puts("              application or a console application");
  puts("  parameters  These are the parameters passed to the program");
  puts("\nNote that the priority parameters may have no effect if the child");
  puts("process changes its own priority.");
  exit(1);
}

int main(int argc, char *argv[])
{
  char szArgs[512], *p;  /* new process arguments buffer & temp pointer */
  char szPgmName[MAX_PATH];  /* name of the program */
  BOOL fSuccess;  /* API return code */
  STARTUPINFO si;  /* used for CreateProcess */
  PROCESS_INFORMATION pi;  /* used for CreateProcess */
  LPSTR lpszCurDir = NULL;  /* current directory for new process */
  BOOL bMoreParams;  /* flag end of new process parameter processing */
  BOOL bWait = FALSE;  /* wait/no wait for new process to end */
  DWORD dwResult;  /* API return code */
  DWORD dwCreate = CREATE_NEW_CONSOLE;  /* new process creation flags */
  BOOL bExtension;  /* input filename have an explicit extension? */
  int i;
  /* the following filename extensions must all be 4 chars long */
  char *aExt[] = { ".exe", ".com", ".bat", ".cmd" };
  DWORD dwExitCode;
  DWORD dwVersion = GetVersion();

  /* Check to make sure we are running on Windows NT or Windows 95 */
  if( !(dwVersion < 0x80000000) && (LOBYTE(LOWORD(dwVersion)) < 4) )
    {
    MessageBox( NULL,
       "This application cannot run on Windows 3.1.\n"
       "This application will now terminate.",
       "StartP",
       MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
    return(1);
    } 
  /* process all command-line parameters */
  if (argc < 2)
    help();
  memset(&si, 0, sizeof(STARTUPINFO));
  bMoreParams = TRUE;
  while(bMoreParams)
    {
    argv++;  /* point to the first parameter */
    if (!*argv) /* if *argv is NULL we're missing the program name! */
      {
      puts("Error: missing program name");
      exit(1);
      }
    if ((*argv)[0] == '/' || (*argv)[0] == '-')
      {
      strlwr(*argv);
      switch ((*argv)[1])  /* letter after the '/' or '-' */
        {
        case 't':
          si.lpTitle = &(*argv)[2];  /*  /Ttitle */
          break;
        case 'd':
          lpszCurDir = &(*argv)[2];  /*  /Dpath */
          break;
        case 'h':
          dwCreate |= HIGH_PRIORITY_CLASS;
          break;
        case 'l':
          dwCreate |= IDLE_PRIORITY_CLASS;
          break;
        case 'r':
          dwCreate |= REALTIME_PRIORITY_CLASS;
          break;
        case 'm':
          switch ((*argv)[2])
            {
            case 'i':
              si.wShowWindow = SW_SHOWMINNOACTIVE;
              si.dwFlags |= STARTF_USESHOWWINDOW;
              break;
            case 'a':
              si.wShowWindow = SW_SHOWMAXIMIZED;
              si.dwFlags |= STARTF_USESHOWWINDOW;
              break;
            default:
              printf("Error: invalid switch - \"%s\"", *argv);
              help();
            } /* switch */
          break;
        case '?':
          help();  /* help() will terminate app */
        case 'w':
          bWait = TRUE;  /* don't end until new process ends as well */
          break;
        case 'b':
          dwCreate |= DETACHED_PROCESS;  /* start detached */
          /* detached implies no CREATE_NEW_CONSOLE so fall through */
        case 'c':
          dwCreate &= ~CREATE_NEW_CONSOLE;  /* turn off this bit */
          break;
        case 'p':
          dwCreate |= CREATE_NEW_PROCESS_GROUP;
          break;
        case 's':
          dwCreate |= CREATE_SEPARATE_WOW_VDM;
          break;
        case 'u':
          dwCreate |= CREATE_UNICODE_ENVIRONMENT;
          break;
        default:
          printf("Error: invalid switch - \"%s\"", *argv);
          help();
        } /* switch */
      } /* if */
    else  /* not a '-' or '/', must be the program name */
      bMoreParams = FALSE;
    } /* while */
  strcpy(szPgmName, *argv++);  /* copy program name as first param */
  bExtension = (BOOL) strchr(szPgmName, '.');  /* has an extension? */
  if (!bExtension)
    strcat(szPgmName, aExt[0]);  /* first extension to try */
  memset(szArgs, 0, sizeof(szArgs));
  strcpy(szArgs, szPgmName);  /* first arg: program name */
  p = strchr(szArgs, 0);  /* point past program name */
  while (*argv)  /* copy remaining arguments to szArgs separated by spaces */
    {
    strcat(p, " ");
    strcat(p, *argv++);
    }
  si.cb = sizeof(STARTUPINFO);
  i = 1;
  do
    {
    fSuccess = CreateProcess(NULL,  /* image file name */
        szArgs,  /* command line (including program name) */
        NULL,  /* security for process */
        NULL,  /* security for main thread */
        FALSE,  /* new process inherits handles? */
        dwCreate,  /* creation flags */
        NULL,  /* environment */
        lpszCurDir,  /* new current directory */
        &si,  /* STARTUPINFO structure */
        &pi);  /* PROCESS_INFORMATION structure */
    if (!fSuccess)
      switch (GetLastError())  /* process common errors */
        {
        case ERROR_FILE_NOT_FOUND:
          if (bExtension || i > 3)
            {
            puts("Error: program or batch file not found");
            exit(1);
            }
          else /* replace the filename extension with the next one */
            {
            strcpy(strchr(szPgmName, '.'), aExt[i++]);
            strncpy(szArgs, szPgmName, strlen(szPgmName));
            }
          break;
        case ERROR_DIRECTORY:
          puts("Error: bad starting directory");
          exit(1);
        case ERROR_PATH_NOT_FOUND:
          puts("Error: bad path");
          exit(1);
        default:
          PERR(0, "CreateProcess");
          exit(1);
        }  /* switch */
    } while (!fSuccess);
  /* pi.dwProcessId for win16 apps is actually an event handle, not */
  /* a process handle, so it can only be used with synchronization APIs */
  printf("Process %d (0x%x) created\n", pi.dwProcessId, pi.dwProcessId);
  /* close thread handle if not needed anymore */
  CloseHandle(pi.hThread);
  if (bWait)  /* /w flag specified? */
    {
    dwResult = WaitForSingleObject(pi.hProcess,  /* object to wait for */
        (DWORD) -1);  /* timeout time */
    PERR(dwResult != -1, "WaitForSingleObject");
    /* get and report the child's exit code */
    fSuccess = GetExitCodeProcess(pi.hProcess, &dwExitCode);
    PERR(fSuccess, "GetExitCodeProcess");
    printf("Child exit code: %d\n", dwExitCode);
    }
  CloseHandle(pi.hProcess);  /* close process handle or it won't die */
  return(0);
}
