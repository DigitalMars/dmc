/*_ putenv.c    */
/* Copyright (C) 1990-1995 by Digital Mars		*/
/* All Rights Reserved                         */
/* Written by Walter Bright                    */
/* DOS386 support added by G. Eric Engstrom     */

#include <tchar.h>
#include <scrtl.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <dos.h>
#include "semlock.h"
#include <sctchar.h>

#if defined(_WIN32)

#include "dbgint.h"

#ifndef _DEBUG
#define _realloc_crt        realloc
#define _free_crt           free
#define _malloc_crt         malloc
#define _strdup_crt         strdup
#define _wcsdup_crt         _wcsdup
#else
#include <crtdbg.h>
#define _realloc_crt(p, s)  _realloc_dbg(p, s, _CRT_BLOCK, __FILE__, __LINE__)
#define _malloc_crt(s)      _malloc_dbg(s, _CRT_BLOCK, __FILE__, __LINE__)
#define _free_crt(p)        _free_dbg(p, _CRT_BLOCK)
#define _strdup_crt(s)      __strdup_dbg(s,_CRT_BLOCK,__FILE__, __LINE__)
#define _wcsdup_crt(s)      __wcsdup_dbg(s,_CRT_BLOCK,__FILE__,__LINE__)
#endif

#endif

#if (__OS2__ && (MSDOS || _WINDOWS || DOS16RM || DOS386)) || (DOS16RM && DOS386)
#error "invalid operating system #defines"
#endif

#if __OS2__
#include       <os2lib.h>
#endif
#ifdef __NT__
#include       <windows.h>
#endif


#if _WINDOWS
#include <dpmi.h>
#endif

#if DOS16RM
void __far *D16MemAlloc(unsigned);
int D16MemFree(void __far *);

#endif

#ifndef LPTR
//#define LPTR (sizeof(void *) > sizeof(int))
#define LPTR   (__COMPACT__ || __LARGE__ || __VCM__)
#endif

#ifndef __NT__
extern char *_envptr;

#endif

#if __OS2__ && __INTSIZE == 4
extern char *_aenvptr;

#endif

#if __INTSIZE == 2
extern char *_envbuf;
extern unsigned aenvseg;

#endif

#ifdef _WIN32
extern void _setenvp(void);

static int __get_wenvlength(void)
{
    wchar_t **wenvptr;
    int count;

    if (_wenviron == NULL)
    {
        return 0;
    }
    count = 1;
    for (wenvptr = _wenviron; *wenvptr != NULL; wenvptr++)
    {
        count++;
    }
    return count;
}
static int __find_wenvptr(const wchar_t * varp, int size)
{
    wchar_t **wenvptr;
    int count;

    count = 1;
    for (wenvptr = _wenviron; *wenvptr != NULL; wenvptr++)
    {
        if (wcsnicmp(*wenvptr, varp, size) == 0)
        {
            break;
        }
        count++;
    }
    return *wenvptr != L'\0' ? count : -count;
}

static int __sync_wenviron(const wchar_t * newp)
{
    wchar_t *varp;
    wchar_t *valp;
    wchar_t **newenvp;
    wchar_t **envp;
    int deleting;
    int envi;
    int ret;

    if (newp == NULL)
    {
        goto error;
    }
    ret = 0;
    valp = wcschr(newp, L'=');
    if (valp == NULL)
    {
        goto error;
    }
    if (_wenviron == NULL)
    {
        _wenviron = _malloc_crt(sizeof(wchar_t **));
        if (_wenviron == NULL)
        {
            goto error;
        }
        *_wenviron = NULL;
    }
    deleting = (valp[1] == L'\0');
    envi = __find_wenvptr(newp, valp - newp);
    if (envi > 0)
    {
        wchar_t *oldptr;
        wchar_t *newptr;

        if (deleting == FALSE)
        {                       /* Replace VAR=NewVal	 */
            oldptr = _wenviron[envi - 1];
            newptr = _wcsdup_crt(newp);
            if (newptr == NULL)
            {
                goto error;
            }
            _wenviron[envi - 1] = newptr;
            _free_crt(oldptr);
        }
        else
        {                       /* Delete VAR=		 */
            int len;

            len = __get_wenvlength();
            oldptr = _wenviron[envi - 1];
            _free_crt(oldptr);
            for (envp = _wenviron + (envi - 1); *envp != NULL; envp++)
            {
                *envp = *(envp + 1);
            }
            newenvp = _realloc_crt(_wenviron, (len - 1) * sizeof(wchar_t *));
            if (newenvp == NULL)
            {
                goto error;
            }
            _wenviron = newenvp;
        }
    }
    else
    {                           /* Add VAR=NewVal	 */
        if (deleting == FALSE)
        {
            wchar_t *newptr;

            envi = 0 - envi;
            newenvp = _realloc_crt(_wenviron, (envi + 1) * sizeof(wchar_t *));
            if (newenvp == NULL)
            {
                goto error;
            }
            _wenviron = newenvp;
            newptr = _wcsdup_crt(newp);
            if (newptr == NULL)
            {
                goto error;
            }
            _wenviron[envi - 1] = newptr;
            _wenviron[envi] = NULL;
        }
    }
done:
    return ret;
error:
    ret = -1;
    goto done;
}
static int __wputenv(const char *newp)
{
    wchar_t *wnewp;
    size_t wlen;
    int ret = -1;

    wlen = mbstowcs(NULL, newp, 0);
    if (wlen != (size_t) - 1)
    {
        wlen++;                 /* Include NUL stopper */
        wnewp = (wchar_t *) _malloc_crt(wlen * sizeof(wchar_t));
        if (wnewp != NULL)
        {
            if (mbstowcs(wnewp, newp, wlen) != (size_t) - 1)
            {
                ret = __sync_wenviron(wnewp);
            }
            _free_crt(wnewp);
        }
    }
    return ret;
}

static int __find_envptr(char *, int);
static int __get_envlength();

/*
 * __get_envlength returns the number of environ strings contained in _environ
 *
 */
static int __get_envlength()
{
    int count;
    char **envptr;

    if (!_environ)
        return 0;

    for (count = 1, envptr = _environ; *envptr; count++, envptr++)
        ;

    return count;
}


/*
 * __find_envptr returns the index into _environ where var is located
 * If it fails it returns a negative of the size of _environ so that
 * the caller can use the info to realloc _environ to fit in the new var.
 * NOTE: The index returned is 1 based.
 * Used by putenv.
 */

static int __find_envptr(char *var, int size)
{
    char **envptr;
    int count = 1;

    for (envptr = _environ; (*envptr) != NULL; envptr++, count++)
    {
#ifdef _MBCS
        if (_tcsnicmp((*envptr), var, size) == 0)
#else
        if (strncmpi((*envptr), var, size) == 0)
#endif
            break;
    }
    return (*envptr) ? count : -count;
}

#endif

/****************************************
 * Returns:
 *     0       success
 *     -1      failure
 */

int putenv(const char *newstr)
{
#ifdef _WIN32
    char *var;
    char *value;
    char **newenviron;
    char **envptr;
    char deleting;
    char *oldptr,
    *newptr;
    int len;
    int envindex;
    int envlength;
    int retval;

    LockSemaphore(_semEnvironment);

    if (!newstr)
        goto failure;

    retval = 0;

    /* if invalid format, return failure */
    value = _tcschr(newstr, '=');
    if (value == NULL)
        goto failure;

    /* if _environ is NULL, then allocate one and initialize it */
    if (!_environ)
    {

        _environ = _malloc_crt(sizeof(char **));
        if (!_environ)
            goto failure;

        *_environ = NULL;
    }


    deleting = (value[1] == '\0');

    len = value - (char *) newstr;

    /* Search for Var in _environ, returning index  */
    envindex = __find_envptr((char *) newstr, len);

    /* It was found */
    if (envindex > 0)
    {
        if (!deleting)
        {
            /* Replace with string passed in */
            oldptr = _environ[envindex - 1];
            newptr = _strdup_crt(newstr);

            if (!newptr)
                goto failure;

            _environ[envindex - 1] = newptr;
            _free_crt(oldptr);
        }
        else
        {
            /* Get length of _environ for realloc purposes */
            envlength = __get_envlength();
            /* free pointer that we are deleting */
            oldptr = *(_environ + (envindex - 1));
            _free_crt(oldptr);
            /* adjust environ array */
            for (envptr = (_environ + (envindex - 1)); *envptr; envptr++)
            {
                *envptr = *(envptr + 1);
            }
            /* shrink block */
            newenviron = _realloc_crt(_environ, (envlength - 1) * sizeof(char *));
            if (!newenviron)
                goto failure;

            _environ = newenviron;
        }

    }
    /* not found, new var */
    else if (!deleting)
    {
        /* Realloc _environ  and add ptr in */
        newenviron = _realloc_crt(_environ, ((-envindex) + 1) * sizeof(char *));
        if (!newenviron)
            goto failure;

        _environ = newenviron;
        newptr = _strdup_crt(newstr);
        if (!newptr)
            goto failure;

        _environ[(-envindex) - 1] = newptr;
        _environ[(-envindex)] = NULL;

    }

    /* inform Win32 of new var */
    var = _malloc_crt(len + 1);
    if (var == NULL)
        goto failure;

    memcpy(var, newstr, len);
    var[len] = 0;

    value = &value[1];          // point at value for variable.
    SetEnvironmentVariable(var, deleting ? NULL : value);
    _free_crt(var);

    /* update _envptr with new variable */
    if (_envptr)
        FreeEnvironmentStrings(_envptr);
    _envptr = GetEnvironmentStrings();

    if (__wputenv(newstr) != 0)
    {
        goto failure;
    }
    if (_wenvptr != NULL)
    {
        _free_crt(_wenvptr);
        _wenvptr = NULL;        /* reset */
    }

Lret:
    UnlockSemaphore(_semEnvironment);
    return retval;

failure:
    retval = -1;
    goto Lret;

#else

    const char *p;
    char deleting;
    char match;
    size_t len;
    size_t newlen;
    size_t namelen;
    size_t matchi,
     matchj;
    size_t newsize,
     oldsize;
    char *newbuf;
    char *newenvp;
    char *oldenvp;
    char *op;
    int result;

# if LPTR
# else
    unsigned newenvs;

#endif

    LockSemaphore(_semEnvironment)

        if (!newstr)
        goto failure;

    /* Determine if we are deleting or modifying/adding        */
    for (p = newstr; *p != '='; p++)
#ifdef _MBCS
    {
        if (*p == '\0')
        {                       /* name followed by nothing     */
            goto failure;       /* is a failure                 */
        }
        if (_istlead((unsigned char) *p))
        {
            p++;
            if (*p == '\0')
            {
                goto failure;   /* is a failure                 */
            }
        }
    }
#else
        if (!*p)                /* name followed by nothing     */
            goto failure;       /* is a failure                 */
#endif
    deleting = (p[1] == 0);     /* if ABC=                      */
    namelen = (p + 1) - newstr; /* include the '=' in the length */
    newlen = (deleting) ? 0 : strlen(newstr) + 1;

    /* Get pointer to existing environment, oldenvp    */
    oldenvp = _envptr;

    /* Determine new environment size, newsize         */
    match = 0;
    newsize = 0;
    oldsize = 0;
    op = oldenvp;
    do
    {
        len = strlen(op) + 1;
#ifdef _MBCS
        if (_tcsnicmp(op, newstr, namelen) == 0)  /* if match             */
#else
        if (memcmp(op, newstr, namelen) == 0) /* if match             */
#endif
        {
            if (match)
                goto failure;   /* duplicate def?       */
            matchi = oldsize;
            matchj = oldsize + len;
            match = 1;
            newsize += newlen;
        }
        else
            newsize += len;
        oldsize += len;
        op += len;
    } while (*op);
    if (!match)
    {
        if (deleting)
            goto success;
        /* We're adding a new environment variable      */
        newsize += newlen;
        matchi = oldsize;
        matchj = oldsize;
    }
    oldsize++;
    newsize++;                  /* allow for terminating 0 at end of env */

    /* Allocate new environment block, newenvp    */
#if DOS16RM
    newenvp = (char *) D16MemAlloc(newsize);
    if (!newenvp)
        goto failure;
#elif DOS386
    newenvp = (char *) malloc(newsize + 15);
    if (!newenvp)
        goto failure;
#elif _WINDOWS
    newbuf = (char *) malloc(newsize + 15);
    if (!newbuf)
        goto failure;
#if LPTR
    {
        unsigned seg = FP_SEG(newbuf);
        unsigned off = FP_OFF(newbuf);

        off = (off + 15) & ~15; /* round to next paragraph */
        if (_cpumode == 1)      /* if running in protected mode windows */
        {
            short a = 0;
            char far *b;
            unsigned c;

            b = ((char *) dpmi_GetBaseAddress(seg)) + off;
            if ((a = dpmi_AllocLDTDescriptors(1)) != -1
                && dpmi_SetBaseAddress(a, b) != -1
                && dpmi_SetSegLimit(a, newsize) != -1)
                newenvp = (char *) MK_FP(a, 0);
            else
            {
                free(newbuf);
                if (a != -1)
                    dpmi_FreeDescriptor(a);
                newbuf = NULL;
            }
        }
        else
            newenvp = (char *) MK_FP(seg + (off >> 4), 0);
    }
#else
# if 0
    newenvp = (char *) (((unsigned) newbuf + 15) & ~15);
# else
    {
        unsigned seg = getDS();
        unsigned off = ((unsigned) newbuf + 15) & ~15;

        newenvp = (char *) off;
        if (_cpumode == 1)
        {                       /* if running in protected mode windows */
            short a;
            char far *b;

            newenvs = 0;
            a = dpmi_AllocLDTDescriptors(1);
            if (a != -1)
            {
                b = (char far *) dpmi_GetBaseAddress(seg) + off;
                if (dpmi_SetBaseAddress(a, b) != -1)
                {
                    if (dpmi_SetSegLimit(a, newsize) != -1)
                    {
                        newenvs = a;
                    }
                }
            }
            if (newenvs == 0)
            {
                if (a != -1)
                {
                    dpmi_FreeDescriptor(a);
                }
                free(newbuf);
                goto failure;
            }
        }
        else
        {
            newenvs = getDS() + ((unsigned) newenvp >> 4);
        }
    }
# endif
#endif
#elif MSDOS
    newbuf = (char *) malloc(newsize + 15);
    if (!newbuf)
        goto failure;
#if LPTR
    {
        unsigned seg = FP_SEG(newbuf);
        unsigned off = FP_OFF(newbuf);

        off = (off + 15) & ~15; /* round to next paragraph */
        newenvp = (char *) MK_FP(seg + (off >> 4), 0);
    }
#else
    newenvp = (char *) (((unsigned) newbuf + 15) & ~15);
#endif
#elif __OS2__
    newenvp = (char *) malloc(newsize);
    if (!newenvp)
        goto failure;
#else
#error "OS undefined"
#endif

    /* Copy over new environment       */
    memcpy(newenvp, oldenvp, matchi);
    memcpy(newenvp + matchi, newstr, newlen);
    memcpy(newenvp + matchi + newlen, oldenvp + matchj, oldsize - matchj);

    /* Dump old environment block      */
#if DOS16RM
    if (_envbuf)
        D16MemFree(_envbuf);
    _envbuf = newenvp;
    _envptr = newenvp;
    aenvseg = FP_SEG(newenvp);
    *(unsigned __far *) MK_FP(_psp, 0x2C) = aenvseg;
#elif DOS386
    free(_envptr);
    _envptr = newenvp;
#elif _WINDOWS
    if (_envbuf)
    {
        free(_envbuf);
        if (_cpumode == 1)      /* if running in Protected mode */
            dpmi_FreeDescriptor(aenvseg);
    }
    _envbuf = newbuf;
    _envptr = newenvp;
#if LPTR
    aenvseg = FP_SEG(newenvp);
#else
# if 0
    aenvseg = getDS() + ((unsigned) newenvp >> 4);
# else
    aenvseg = newenvs;
# endif
#endif
    *(unsigned __far *) MK_FP(_psp, 0x2C) = aenvseg;
#elif MSDOS
    free(_envbuf);
    _envbuf = newbuf;
    _envptr = newenvp;
#if LPTR
    aenvseg = FP_SEG(newenvp);
#else
    aenvseg = getDS() + ((unsigned) newenvp >> 4);
#endif
    *(unsigned __far *) MK_FP(_psp, 0x2C) = aenvseg;
#elif __OS2__ && __INTSIZE == 2
    if (FP_SEG(oldenvp) != aenvseg) /* if not original              */
        free(_envptr);
    _envptr = newenvp;
#elif __OS2__
    if (oldenvp != _aenvptr)    /* if not original              */
        free(_envptr);
    _envptr = newenvp;
#else
#error "OS undefined"
#endif

success:
    result = __build_environ(); /* success                      */

ret:
    UnlockSemaphore(_semEnvironent)
        return result;

failure:
    result = -1;
    goto ret;
#endif
}
