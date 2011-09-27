/* Copyright (C) 1994-1995 by Digital Mars
 * All Rights Reserved
 * March 10, 1994, Sep 23, 1995
 *
 * spawn.c
 * Entries:
 *   __spawn
 *   __seek_envp
 * Notes:
 *   These entries are intended for use by the execX and spawnX routines.
 *   They are not part of the published RTL interface.
 */

#include <tchar.h>
#include <sctchar.h>
#include <process.h>
#include <stdio.h>
#include <scrtl.h>
#include <errno.h>
#include <string.h>
#include <dos.h>
#include <stdlib.h>
#include <scio.h>

#include "dbgint.h"

#if !__NT__
# define CMDLEN 128   /* max length of command line */
#endif

#if __INTSIZE == 4
# define FSTRCPY _tcscpy
#else
# define FSTRCPY _fstrcpy
#endif

/* Utility routine to get file extension from File Name. */
static _TCHAR __ss * __near get_file_extension(_TCHAR __ss *);

#ifdef _UNICODE

#include <setlocal.h>
#include <lcapi32.h>

static void __make_wide_envptr (void) {
 UINT		cPage = __locale_codepage;
 int		lsz;
 int		tsz;
 int		rsz;
 char *		envptr;
 char *		cenvstr;
 wchar_t *	wenvstr;
 wchar_t *	twp;
  envptr = _envptr;
  if (envptr == NULL) {
    return;
  }
/*
 * ??? if starting character is '=' then skip until single null.
 * "=C:=C:\test\0var=XXX" points to 'v' character. see to BUILDENV.C, SPAWN.C
 */
  while (*envptr == '=') {
    envptr += strlen (envptr) + 1;
  }
  cenvstr = envptr;
  tsz = 0;
  while (*cenvstr != '\0') {
    lsz = MtoWcmSZ (cenvstr, -1);
    if (lsz == 0) {
      return;
    }
    cenvstr += strlen (cenvstr) + 1;
    tsz += lsz;
  }
  tsz++;
//  twp = GETWMAIN (tsz);
  twp = (wchar_t *) _malloc_crt((tsz) * sizeof (wchar_t));
  if (twp == NULL) {
    return;
  }
  rsz = tsz;
  cenvstr = envptr;
  wenvstr = twp;
  while (*cenvstr != '\0') {
    if (mbstowcs (wenvstr, cenvstr, rsz) == CPYERR) {
      _free_crt (twp);
      return;
    }
    lsz = wcslen (wenvstr) + 1;
    wenvstr += lsz;
    cenvstr += strlen (cenvstr) + 1;
    rsz -= lsz;
  }
  *wenvstr = L'\0';
  _wenvptr = twp;
}

void _STD_make_wide_envptr (void) {
  if (_wenvptr != NULL) {
    free (_wenvptr);
    _wenvptr = NULL;
  }
}

#endif

static int __near pascal do_exec (int mode, _TCHAR *cmdbuf, const _TCHAR *const *argv,
 _TCHAR _32BIT *envblock, int batch)
{
# if __NT__
    _TCHAR *argbuf;
    const _TCHAR * const *at;
    size_t len;
    _TCHAR *a;
# else
    _TCHAR argbuf[1 + CMDLEN + 2];
    _TCHAR __ss *a;
    unsigned env;
# endif
  const _TCHAR __ss *p;
  int i, chain;
  _TCHAR *batch_file;

  if (batch)
  {
    batch_file = cmdbuf;
    cmdbuf = _tgetenv (_T("COMSPEC"));
    if (cmdbuf == NULL)
      goto enomem;
  }
  /* Build argbuf[] */
# if __OS2__
    /* Copy 0th argument (command) to argbuf, move argv to next */
    a = argbuf;
    if(*argv)
    {
      i = _tcslen (*argv);
      if (i > CMDLEN - 1)   /* leave room for \0 at end */
        goto e2big;
      _tcscpy (a, *argv++);
      a += _tcslen (a) + 1;
    }
# elif __NT__
    /* If there is a first argument, move argv to next. Allocate argbuf big enough for
     * remaining args.
     */
    if (*argv)
      argv++;
    len = 1;
    for (at = argv; *at; at++)
      len += _tcslen (*at) + 1; /* +1 for separator   */
    if (batch)
      len += _tcslen (batch_file) + 4 + 1;
    argbuf = (_TCHAR *) alloca(len * sizeof (_TCHAR));
    if (argbuf == NULL)
      goto enomem;
    a = argbuf;
# else
    a = argbuf + 1; /* For DOS, leave a space for the number of bytes in the line */
    /* If there is a first argument, move argv to next. */
    if (*argv)
      argv++;
# endif
  *a = 0;
  i = 0;
  if (batch)
  {
    i += _tcslen (batch_file) + 4;
#   if !__NT__
      if (i > CMDLEN - 1) /* leave room for \0 or \r at end */
        goto e2big;
#   endif
    _tcscat (a, _T(" /c "));
    _tcscat (a, batch_file);
    if (*argv)    /* if more arguments    */
    {
      i++;
#     if !__NT__
        if (i > CMDLEN - 1)
          goto e2big;
#     endif
      _tcscat(a,_T(" "));  /* separate with a space  */
    }
  }
  while (*argv)
  {
    i += _tcslen (*argv);
#   if !__NT__
      if (i > CMDLEN - 1) /* leave room for \0 or \r at end */
        goto e2big;
#   endif
    _tcscat (a, *argv++);
    if (*argv)    /* if more arguments    */
    {
      i++;
#     if !__NT__
        if (i > CMDLEN - 1)
          goto e2big;
#     endif
      _tcscat (a,_T(" "));  /* separate with a space  */
    }
  }
# if !__OS2__ && !__NT__
    argbuf[0] = i;            /* For DOS, # of bytes in command line */
    if (argbuf[i] != _T('\r'))    /* if not already ending in \r  */
      _tcscat (a, _T("\r"));       /* terminate command with \r  */
# endif
  chain = mode == _P_OVERLAY;
# if __INTSIZE == 4
    if (envblock == NULL)
    {
#  ifndef _UNICODE
     char *envptr;
      envptr = _envptr;
#   ifdef __NT__
/*
 * ??? if starting character is '=' then skip until single null.
 * "=C:=C:\test\0var=XXX" points to 'v' character. see BUILDENV.C, WGETENV.C
 */
      while (*envptr == '=') {
        envptr += strlen (envptr) + 1;
      }
#   endif
      envblock = envptr;
#  else
      if (_wenvptr == NULL) {
        __make_wide_envptr ();
      }
      envblock = _wenvptr;
#  endif
    }
    return _texec (cmdbuf, argbuf, envblock, chain);
# else
    if (envblock != NULL)
      env = _FP_SEG (envblock);
    else
      env = aenvseg;
    return _texec (cmdbuf, argbuf, env, chain);
# endif
e2big:
  __set_errno (E2BIG);
  return -1;
enomem:
  __set_errno (ENOMEM);
  return -1;
}

/*
 * Given a File Name, return a pointer to the extension part.
 *  If there is no extension, return NULL.
 */
static _TCHAR __ss * __near get_file_extension(_TCHAR __ss *cmdbuf)
{
    _TCHAR __ss *end = cmdbuf + _tcslen(cmdbuf);
#ifdef _MBCS
    for (; end > cmdbuf; end = (_TCHAR _ss *) _tcsdec (cmdbuf, end))
    {
#else
    for (; end > cmdbuf; end--)
    {
#endif
	switch (*end)
	{   case _T('.'):
		return end + 1;
	    case _T('\\'):
	    case _T('/'):
	    case _T(':'):
		return 0;
	}
    }
    return 0;
}

static int __near pascal execute (int mode, const _TCHAR *cmd, const _TCHAR *const *argv,
  _TCHAR _32BIT *envblock)
{
  _TCHAR __ss *cmdbuf;
  _TCHAR __ss *ext;
  int i;
  size_t len;
  int errnoval;
  static _TCHAR extensions[][4] = { _T("com"),_T("exe"),_T("bat") };


  len = _tcslen(cmd);
  cmdbuf = alloca ((len + 4 + 1) * sizeof (_TCHAR)); /* leave room for . extension */
  if (!cmdbuf)
  { errnoval = ENOMEM;
    goto enomem;
  }
  memcpy (cmdbuf, cmd, (len + 1) * sizeof (_TCHAR));
  ext = get_file_extension(cmdbuf);
  if (ext) {
    if (__tfilesize (cmdbuf) != -1)
	goto Lexec;
  }
  else
  {
    ext = cmdbuf + len;
    *ext++ = _T('.');
    for (i = 0; i < sizeof(extensions) / sizeof(extensions[0]); i++)
    {
      memcpy(ext, extensions[i], sizeof(extensions[0]));
      if (__tfilesize (cmdbuf) != -1)
      {
       Lexec:
         return do_exec(mode, cmdbuf,argv,envblock,_tcsnicmp(ext,_T("bat"),4) == 0);
      }
    }
  }
  errnoval = ENOENT;
enomem:
  __set_errno (errnoval);
  return -1;
}

/*
 * execute_via_path
 * Notes:
 *   1. Tries to exec the cmd as given.
 *   2. If that fails and the cmd doesn't contain an absolute path, it tries exec'ing
 *      with every path in the PATH variable until it works or an error occurs.
 */

static int __near pascal execute_via_path (int mode, const _TCHAR *cmd, const _TCHAR *const *argv,
  _TCHAR _32BIT *envblock)
{
  _TCHAR *p, c, lastc, __ss *cmdbuf, __ss *pc;
  int status;

  status = execute (mode, cmd, argv, envblock);
  if (status >= 0 || errno != ENOENT || cmd[0] == _T('\\') || cmd[0] == _T('/') || cmd[0] == _T('.') || (cmd[0] && cmd[1] == _T(':')))
    return status;
  p = _tgetenv (_T("PATH"));
  if (!p)
    return status;
  /* Allocate a buffer that will always be big enough, so we
    never need to worry about FILENAME_MAX or any overflow checks.
   */
  cmdbuf = alloca ((_tcslen (p) + _tcslen (cmd) + 1 + 1) * sizeof (_TCHAR));
  if (!cmdbuf)
    return status;
  while ((c = *p) != 0)
  {
    pc = cmdbuf;
    lastc = _T('\\');
    do
    {
      if (c == _T(';'))   /* path separator */
      {
        p++;
        break;
      }
      *pc++ = c;
      lastc = c;
#ifdef _MBCS
      if (_istlead (c)) {
        c = *++p;
        if (c == '\0') {
          break;
        }
        *pc++ = c;
      }
#endif
      c = *++p;
    } while (c);
    if (lastc != _T('\\') && lastc != _T('/') && lastc != _T(':'))
      *pc++ = _T('\\');
    _tcscpy (pc, cmd);
    status = execute (mode, cmdbuf, argv, envblock);
    if (status >= 0 || errno != ENOENT)
      break;
  }
  return status;
}

#if !(defined (_WINDOWS) || defined (__NT__) || defined (DOS386))
static _TCHAR * __near __pascal build_fileinfo_string ()
{
  unsigned char *infoptr;
  _TCHAR *string, *stringptr;
  int size;
  _TCHAR envvar[] = _T("C_FILE_INFO=");

  if (!_fileinfo)
    return NULL;
  for (infoptr = __fhnd_info + _NFILE - 1; infoptr >= __fhnd_info; infoptr -= 1)
    if (*infoptr != 0)
      break;
  size = infoptr - __fhnd_info + 1;
  if (size <= 0)
    return NULL;
  string = malloc ((sizeof (envvar) + (size * 2 + 1)) * sizeof (_TCHAR));
  if (string == NULL)
    return (_TCHAR *)-1;
  _tcscpy (string, envvar);
  stringptr = string + (sizeof (envvar) - 1) / sizeof (_TCHAR);
  for (infoptr = __fhnd_info; infoptr < __fhnd_info + size; infoptr += 1)
  {
    *stringptr = (*infoptr & 0xf) + _T('A');
    stringptr += 1;
    *stringptr = ((*infoptr >> 4) & 0xf) + _T('A');
    stringptr += 1;
  }
  *stringptr = _T('\0');
  return string;
}
#endif

/* __spawn
 * Notes:
 *   All exec* and spawn* functions call this function.
 *   If envp is not NULL, spawn allocates memory to create an environment block for the
 *     new program. If mode is _P_WAIT, when the new program returns the block is freed.
 *     If mode is _P_OVERLAY, all of the memory associated with the current program will
 *     be freed by the OS when the current program exits.
 *   An environment block is a contiguous block of memory. The block contains one string
 *     after another with no gaps. Each string is terminated by a nul character. At the
 *     end of all of the strings is a null string, that is, there is an additional nul
 *     character at the end of the block.
 *     This is the kind of structure that DOS uses to store environment variables.
 *   For DOS16 and WIN16, envblocks have two special properties.
 *     1. The pointer is aligned to point to a paragraph boundary.
 *     2. The offset has been normalized to zero.
 *   DOS needs this kind of pointer, because it only stores the segment of the
 *     environment in the PSP of an EXEC'ed program.
 *   For DOS16, spawn conditions the pointer appropriately. For Windows, GlobalAlloc
 *     always returns this kind of pointer.
 * Parameters:
 *   mode
 *     If _P_WAIT, __spawn executes the new program and returns when it completes.
 *     If _P_NOWAIT, __spawn executes the new program and returns immediately. This applies
 *       to the European OEM MSDOS 4.0 only.
 *     If _P_OVERLAY, the current program is replaced by the new program and  __spawn
 *       never returns.
 *     If any other value, __spawn sets errno to EINVAL and returns -1.
 *   cmd
 *     The new program.
 *   argv
 *     A pointer to an array of strings which will be arguments to the new program.
 *   envp
 *     A pointer to an array of strings, which will become the new program's environment.
 *     If NULL, the current environment will be passed to the new program.
 *   use_path
 *     If true, the paths specified in the PATH environment variable will be searched to
 *       find the program if it cannot be found in the path as given.
 * Returns:
 *   -1 if there was an error executing the new program.
 *   Otherwise, the return value of the new program.
 */


int __tspawn (int mode, const _TCHAR *cmd, const _TCHAR *const *argv, const _TCHAR *const *envp,
  int use_path)
{
  int retcode;
  unsigned size;
  _TCHAR _32BIT *envblock, _32BIT *blockpos;
  const _TCHAR *const *envstr;
# if _WINDOWS
    HGLOBAL handle;
# elif !(defined (__NT__) || defined (DOS386))
    _TCHAR __far *start, *fileinfo_string;
# endif

  if (mode != _P_WAIT && mode != _P_NOWAIT && mode != _P_OVERLAY)
    goto einval;
  if (cmd == NULL)
    goto einval;
# if !(defined (_WINDOWS) || defined (__NT__) || defined (DOS386))
    fileinfo_string = build_fileinfo_string ();
    if (fileinfo_string == (_TCHAR *)-1)
      goto enomem;
# endif
  if (envp == NULL)
  {
#   if !(defined (_WINDOWS) || defined (__NT__) || defined (DOS386))
      if (fileinfo_string != NULL)
        _tputenv (fileinfo_string);
#   endif
    envblock = NULL;
  }
  else
  {
    size = 1;
    for (envstr = envp; *envstr != NULL; envstr += 1)
      size += _tcslen (*envstr) + 1;
#   if !(defined (_WINDOWS) || defined (__NT__) || defined (DOS386))
      if (fileinfo_string != NULL)
        size += _tcslen (fileinfo_string) + 1;
#   endif
#   if __NT__ || DOS386
      envblock = malloc (size * sizeof (_TCHAR));
      if (envblock == NULL)
        goto enomem;
#   elif _WINDOWS
      handle = GlobalAlloc (size * sizeof (_TCHAR), GMEM_MOVEABLE);
      if (handle == 0)
        goto enomem;
      envblock = GlobalLock (handle);
#   else
      start = (_TCHAR far *)_fmalloc ((size + 0xf) * sizeof (_TCHAR));
      if (start == NULL)
        goto enomem;
      envblock = (_TCHAR __far *)_MK_FP (_FP_SEG (start) + ((_FP_OFF (start) + 0xf & ~0xf) >> 4), 0);
#   endif
    blockpos = envblock;
    for (envstr = envp; *envstr != NULL; envstr += 1)
    {
      FSTRCPY (blockpos, *envstr);
      blockpos += _tcslen (*envstr) + 1;
    }
#   if !(defined (_WINDOWS) || defined (__NT__) || defined (DOS386))
      if (fileinfo_string != NULL)
      {
        FSTRCPY (blockpos, fileinfo_string);
        blockpos += _tcslen (fileinfo_string) + 1;
      }
#   endif
    *blockpos = _T('\0');
  }
# if !(defined (_WINDOWS) || defined (__NT__) || defined (DOS386))
    if (fileinfo_string != NULL)
      free (fileinfo_string);
# endif
  if (use_path)
    retcode = execute_via_path (mode, cmd, argv, envblock);
  else
    retcode = execute (mode, cmd, argv, envblock);
  if (envblock == NULL)
    _tputenv (_T("C_FILE_INFO="));
  else
  {
#   if __NT__ || DOS386
      free (envblock);
#   elif _WINDOWS
      GlobalUnlock (handle);
      GlobalFree (handle);
#   else
      _ffree (start);
#   endif
  }
  return retcode;
einval:
  __set_errno (EINVAL);
  return -1;
enomem:
  __set_errno (ENOMEM);
  return -1;
}

/*
 * __seek_envp
 * Notes:
 *   A handy routine for the four LE routines: spanwnle, spawnlpe, execle, and execlpe.
 *   It finds the envp argument that comes after a bunch of args.
 */

const _TCHAR *const *__tseek_envp (register const _TCHAR *const *arg)
{
  for (; *arg != NULL; arg += 1)
    ;
  return (const _TCHAR *const *)*(arg + 1);
}

