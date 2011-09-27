/*_ system.c   Sun Apr 16 1989   Modified by: Walter Bright */
/* OS2 support added by Nikki Locke May 1989 */
/* Copyright (C) 1985-1995 by Walter Bright */
/* All Rights Reserved                  */
/* Written by Walter Bright             */

#include <tchar.h>
#include <io.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <scrtl.h>
#include <dos.h>

#if _WIN32
#include <windows.h>
static _TCHAR *GetWin32CmdInterp();
#endif

/**************************
 * Execute command line using COMMAND.COM.
 * Note that since COMMAND.COM doesn't return the exit status of the
 * program it ran, we can't either. All we can return is the exit
 * status of COMMAND.COM itself.
 * Returns:
 *  0   success
 *  -1  error
 */

int _tsystem(const _TCHAR *cmdline)
{   _TCHAR *comspec;
    int retval;

    comspec = _tgetenv(_T("COMSPEC"));

   /* If nothing passed in, simply return access of comspec */
   if (!cmdline) {
        if (!_taccess(comspec,F_OK)) {
        /* Command Interpreter found, return ok */
                retval = 1;
        }
        else {
      /* Command Interpreter not found, seterrno and return 0; */
                __set_errno (ENOENT);
                retval = 0;
        }
    }
    else {

     /* if comspec not set, use default */
     if (!comspec)
     {
#if __OS2__
       comspec = _T("cmd.exe");
#elif _WIN32 
       comspec = GetWin32CmdInterp();
#else
       comspec = _T("command.com");
#endif
     }

     retval = _tspawnlpe(0,comspec,comspec,_T("/c"),cmdline,(_TCHAR *) NULL,(_TCHAR **)NULL);
    }
    return retval;
}


#if _WIN32
static _TCHAR *GetWin32CmdInterp()
{
    _TCHAR *retv = _T("command.com");

    /* high bit of high word contains platform info */
    int osBit = GetVersion() & 0x80000000;
    
    /* osBit of 0 means nt */
    if (!osBit)
    {
        retv = _T("cmd.exe");
    }
    return retv;
}
#endif