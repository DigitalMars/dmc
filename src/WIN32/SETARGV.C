/*_ setargv.c   Modified by: Walter Bright
 * Copyright (C) 1991 by Digital Mars
 * All rights reserved.
 * Written by Walter Bright
 */

#include <tchar.h>
#include <sctchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>

extern _TCHAR *_tcmdln;
extern _TCHAR *_tpgmptr;
#ifdef _DLL
#define _argc	(*__p___argc())
#define __argv	(*__p___argv())
#define __wargv	(*__p___wargv())
#else
extern int _argc;
extern _TCHAR **__targv;
#endif

extern void __cdecl __faterr(char *);

#if defined(_WIN32)
static _TCHAR *parse_program_name(_TCHAR *);
static _TCHAR *Add_Full_Path(_TCHAR *);
#include <windows.h>
#include <lcapi32.h>
#include "dbgint.h"
#endif

#ifdef _WIN32
int __cdecl __wildcard;		/* 0:Not used. 1:Used */
				/* See macro WILDCARDS in dos.h */
static int setwildcard (void) {
 _TCHAR *      s;
 _TCHAR *      cp;
 _TCHAR *      p;
 _TCHAR        path[FILENAME_MAX+1];
 struct FIND * fp;
 size_t        psz;
 size_t        i;
  if (__wildcard != 0) {
    s = __targv[_argc];
    if ( s[0] != '"' && (_tcschr (s, '*') || _tcschr (s, '?'))) {
      cp = s;
      psz = 0;
      for (i = 0; cp[i] != '\0'; i++) {
        if (cp[i] == ':' || cp[i] == '\\' || cp[i] == '/') {
          psz = i + 1;
        }
#ifdef _MBCS
        if (_istlead (cp[i])) {
          i++;
          if (cp[i] == '\0') {
            break;
          }
        }
#endif
      }
      if (psz > 0) {
        memcpy (path, cp, psz * sizeof (_TCHAR));
      }
      path[psz] = 0;
      _argc--;
      fp = __tfindfirst (cp, 0);
      while (fp != NULL) {
        p = _malloc_crt ((psz + _tcslen (fp->name) + 1) * sizeof (_TCHAR));
        if (p == NULL) {
          return 0;
        }
        _tcscpy (p, path);
        _tcscpy (p + psz, fp->name);
        _argc++;
        __targv = (_TCHAR **) _realloc_crt (__targv, (_argc + 1) * sizeof (_TCHAR *));
        if (__targv == NULL) {
          return 0;
        }
        __targv[_argc] = p;
        fp = __tfindnext ();
      }
    }
  }
  return 1;
}
#endif

/**********************************
 * Command line processing.
 * Input:
 *   _tcmdln      raw command line
 * Output:
 *   _argc
 *   __targv
 *   _tpgmptr
 */

void _tsetargv()
{
    _TCHAR *cmd;
    int argc;
    _TCHAR *s,*d;
    int len;
    int len0;
#ifdef _WIN32
    char nowild;
#endif

#if __OS2__
    /* Find the qualified program name   */
    s = _tcmdln;
    while (--s && s[-1])
       ;
    _tpgmptr = s;

    /* The first string in _tcmdln is the name with which the program
      was invoked. If it is 0, then use _tpgmptr for the program name.
     */
    if (*_tcmdln)
        s = _tcmdln;

    /* Make our own copy of the command line   */
    len0 = _tcslen(s) + 1;      /* argv[0] is 0-terminated   */
    len = len0 + _tcslen(s + len0) + 1;
    cmd = (_TCHAR *) malloc(len * sizeof (_TCHAR));
    if (!cmd)
        goto err;
    memcpy(cmd,s,len * sizeof (_TCHAR));
    s = cmd + len0;      /* first argument already 0 terminated   */
    _argc = 1;
#endif

#ifdef _WIN32
    /* point past program string for MSVCNT compatability...  */
    _tcmdln = parse_program_name(_tcmdln);
    /* but for argv, argc we want program name and want it fully qualified */
    cmd = Add_Full_Path(_tcmdln);
    if (!cmd)
        goto err;
    _argc = 0;
    __targv = (_TCHAR **) _malloc_crt ((_argc + 1) * sizeof(_TCHAR *));
    if (__targv == NULL) {
      goto err;
    }
    s = cmd;
#endif

    /* Loop through command line, splitting it up into 0-terminated
      argument strings, and counting the number of arguments.
     */
    d = s;         /* d (destination) chases s (source)   */
    while (1)
    {
        _TCHAR c;
        _TCHAR lastc;
        char   instring;
        int num_slashes,non_slashes;

        nextarg:
#ifdef _WIN32
        __targv[_argc] = d;
        nowild = 0;
#endif
        switch (*s)
        {
            case 0:
                goto done;
            case _T(' '):
            case _T('\t'):
                s++;
                continue;   /* scan to start of argument   */
            default:
                /* Start of argument   */
#ifndef _WIN32
                _argc++;
#endif
                instring = 0;
                c = 0;
                num_slashes = 0;
#ifdef _WIN32
                if (*s == _T('"'))
                {
                    nowild = 1;
                }
#endif
                while (1)
                {
                    lastc = c;
                    c = *s++;
                    switch (c)
                    {
                        case _T('"'):
                            /*
                               Yes this looks strange,but this is so that we are
                               MS Compatible, tests have shown that:
                               \\\\"foo bar"  gets passed as \\foo bar
                               \\\\foo  gets passed as \\\\foo
                               \\\"foo gets passed as \"foo
                               and \"foo gets passed as "foo in VC!
                            */
                            non_slashes = num_slashes % 2;
                            num_slashes = num_slashes / 2;
                            for (; num_slashes > 0; num_slashes--)
                            {
                                d--;
                                *d = _T('\0');
                            }

                            if (non_slashes)
                            {
                                *(d-1) = c;
                            }
                            else
                            {
                                instring ^= 1;
                            }
                            break;
                        case _T(' '):
                        case _T('\t'):
                            if (!instring)
                            {
                        case 0: /* end of command line   */
                                *d++ = 0;   /* terminate argument   */
#ifdef _WIN32
                                if (nowild == 0) {
                                    if (setwildcard () == 0) {
                                        goto err;
                                    }
                                }
                                _argc++;
                                __targv = (_TCHAR **) _realloc_crt
                                    (__targv, (_argc + 1) * sizeof (_TCHAR *));
                                if (__targv == NULL) {
                                    goto err;
                                }
#endif
                                if (c == 0)
                                    goto done;
                                goto nextarg;
                            }
                            /* FALL-THROUGH */
                        default:
                            if (c == _T('\\'))
                                num_slashes++;
                            else
                                num_slashes = 0;
                            *d++ = c;
                            break;
                    }
#ifdef _MBCS
                    if (_istlead (c)) {
                            *d++ = *s++;
                            if (*(d - 1) == '\0') {
                                    goto done;
                            }
                    }
#endif
                }
        }
    }
done:
#ifndef _WIN32
    /* Create argv[] array */
    __targv = (_TCHAR **) malloc(sizeof(_TCHAR *) * (_argc + 1));
    if (!__targv)
        goto err;
    /* Initialize the argv[] array   */
    s = cmd;
    for (argc = 0; argc < _argc; argc++)
    {
        __targv[argc] = s;
        s += _tcslen(s) + 1;
    }
#endif
    __targv[_argc] = NULL;
#ifdef _WIN32
    _tpgmptr = __targv[0];		/* point _tpgmptr at program */
#endif
    return;

err:
    __faterr("Out of memory");
}



#if defined(_WIN32)

/*
 *
 *    Given a command line, return a pointer that has the fully
 *  qualified full path name to the executable prepended to the command line
 */
static _TCHAR *Add_Full_Path(_TCHAR *cmd)
{
    _TCHAR tempBuff[_MAX_PATH];
    _TCHAR *new_cmd = NULL;

    /* Size is maxpath for filename, + space, + rest of cmdline + 2 quotes */
    unsigned size = _MAX_PATH + 1 + (_tcslen(cmd)+1) + 2;  // Size of buffer
    unsigned ret_size;   // Size returned from OS

    new_cmd = (_TCHAR *)_malloc_crt(size * sizeof (_TCHAR));

    if (!new_cmd)
        goto memerr;

    new_cmd[0] = _T('\0');  /* initialize to empy */

    ret_size = __tGetModuleFileName(NULL,tempBuff,_MAX_PATH);

    if (!ret_size)
        goto memerr;

    if (!_tcschr(tempBuff,_T('"')))
    {
        _tcscat(new_cmd,_T("\""));
        _tcscat(new_cmd,tempBuff);
        _tcscat(new_cmd,_T("\""));
    }
    else
    {
         _tcscpy(new_cmd,tempBuff);
    }

    _tcscat(new_cmd,_T(" "));
    _tcscat(new_cmd,cmd);

    return new_cmd;

    memerr:
        return NULL;
}


/*
 *  Given a value from get commandline, parse out program name
 *  and return pointer to new location.
 */
static _TCHAR *parse_program_name(_TCHAR *s)
{
    int instring = 0;  // Non-zero for " strings.
    int done = 0;

    while (!done)
    {
        switch (*s)
        {
            case _T('"') :
                instring ^= 1;
                s++;
                break;
            case _T('\0') :
                done = 1;
                break;
            case _T(' '):
            case _T('\t'):
                if (!instring)
                    done = 1;
                s++;
                break;
            default:
                s++;
                break;
        }
    }

    /* Ok, now pointing at first thing following program name,
       so read away spaces until first _TCHAR of first arg. */
    while ((*s <= _T(' ')) && (*s))
        s++;

    return s;

}


/*
 * Exposed "wrapper" around _setargv. 
 * Given lpszCmdLine, fill in argcount with appropriate 
 * argc value and return argv.
 * NOTE: The returned value is an allocated pointer 
 * that the caller is responsible for freeing.
 */
_TCHAR **__tparsecmdline(const _TCHAR *lpszCmdLine, int *argcount)
{
    int save_argc = _argc;
    _TCHAR **save_argv = __targv;
    _TCHAR *save_cmdln = _tcmdln;
    _TCHAR *save_pgmptr = _tpgmptr;
    _TCHAR **ret_argv;
    
    _tcmdln = (_TCHAR *)lpszCmdLine;
    
    _tsetargv();
    
    *argcount = _argc;
    ret_argv = __targv;
    
    _tpgmptr = save_pgmptr;
    _argc = save_argc;
    __targv = save_argv;
    _tcmdln = save_cmdln;
    
    return ret_argv;
    
}

#endif
