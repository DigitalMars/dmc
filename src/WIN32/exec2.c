/*_ exec2.c   Modified by: Walter Bright
 * Copyright (C) 1991 by Digital Mars
 * All rights reserved.
 * Written by Walter Bright
 */

#include <tchar.h>
#include <sctchar.h>
#include <stdio.h>
#include <dos.h>
#include <errno.h>
#include <scrtl.h>

#ifdef __OS2__
#include "os2lib.h"
#endif
#ifdef __NT__
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <lcapi32.h>
#endif


/************************
 * Execute a command.
 * Input:
 *     chain
 *     0 = Run synchronously - return exit code
 *     1 = Run asynchronously - return process ID - ignore exit code
 *     2 = Run asynchronously - return Process ID - save exit code for wait()
 *     3 = Run under trace mode (for debuggers)
 *     4 = Run asynchronous detached in background
 *     5 = Load but don't execute
 * Returns:
 *     -1      error (_errno has the OS/2 error code)
 *     n       success (n is the exit status or PID of the executed command)
 */

int _texec(_TCHAR *command,_TCHAR *commandline,_TCHAR *envptr,int chain)
{
#if __OS2__
    RESULTCODES resultcodes;
    int result;

#if (__SMALL__ || __MEDIUM__) && __I86__ <= 2
    result = DosExecPgm(0,0,chain,commandline,(envptr ? envptr : (char _far *)NULL),&resultcodes,command);
#else
    result = DosExecPgm(0,0,chain,commandline,envptr,&resultcodes,command);
#endif
    if (result)
    {
        __set_errno (result);
        result = -1;
    }
    else
    {
        if (chain)                      /* running asynch - return PID  */
            result = resultcodes.TermCode;
        else                            /* running synchronously        */
            result = resultcodes.ResultCode | (resultcodes.TermCode << 8);
    }
    return result;
#else                          /* __NT__ */
    DWORD crflag;
    STARTUPINFO startup;
    PROCESS_INFORMATION pinfo;
    DWORD result;
    _TCHAR *p;

    memset (&startup, 0, sizeof (startup));
    startup.cb = sizeof (startup);             /* no real start up info */

    switch (chain)
    {
        default:
        case 0:
        case 1:
        case 2:
            crflag = 0;
            break;
        case 3:
            crflag = DEBUG_PROCESS;
            break;
        case 4:
            crflag = DETACHED_PROCESS;
            break;
        case 5:
            crflag = CREATE_SUSPENDED;
            break;
    }
   /* take into account 2 quotes */
    size_t n;
    n = _tcslen(command) + _tcslen(commandline) + 2 + 2;

    p = (_TCHAR *) malloc(n * sizeof (_TCHAR));
    if (p == NULL)
        goto enomem;
    /*
       If command contains a space and is not already quoted,
       then quote it
    */
    if (_tcschr(command, _T(' ')) && !_tcschr(command,_T('"')))
    {
        _stprintf(p, _T("\"%s\""),command);
    }
    else
    {
        _tcscpy(p,command);
    }
    _tcscat(p,_T(" "));
    _tcscat(p,commandline);
#ifdef _UNICODE
    crflag |= CREATE_UNICODE_ENVIRONMENT;
#endif
    if (!__tCreateProcess(NULL, p, NULL, NULL, TRUE, crflag, envptr, NULL, &startup, &pinfo))
    {
        __set_nt_errno ();
        free(p);
        return -1;
    }
    free(p);
    CloseHandle (pinfo.hThread);
    if (chain)
    {
        CloseHandle (pinfo.hProcess);
        exit (0);
    }
    if (pinfo.hProcess == 0) /* Must be win32s */
        return 0;
    WaitForSingleObject (pinfo.hProcess, -1);
    GetExitCodeProcess (pinfo.hProcess, &result);
    CloseHandle (pinfo.hProcess);
    return (int)result;
enomem:
    __set_errno (ENOMEM);
    return -1;
#endif
}
