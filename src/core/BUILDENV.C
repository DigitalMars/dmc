/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 *
 * buildenv.c
 * Entry:
 *   __build_environ
 * Notes:
 *   This entry is intended for use by the C initialization and _putenv routines.
 *   They are not part of the published RTL interface.
 */
#include <scrtl.h>
#include <stdlib.h>
#include <string.h>

#include "dbgint.h"

char ** _environ = NULL;

#ifdef __NT__
#include <stdio.h>
#include <malloc.h>
#include <setlocal.h>
#include <lcapi32.h>

wchar_t ** _wenviron = NULL;

static BOOL __build_wenviron (void) {
 UINT		cPage = __locale_codepage;
 char *		envp;
 char *		cenvp;
 wchar_t * * 	newp;
 int		count;
 size_t		lsz;
 int            i;
/*
 * ???: if starting character is '=' then skip until single null.
 * "=C:=C:\test\0var=XXXXX" points to 'v' character. see to SPAWN.C
 */
  envp = _envptr;
  while (*envp == '=') {
    envp += strlen (envp) + 1;
  }
  count = 0;
  for (cenvp = envp; *cenvp != '\0'; cenvp += strlen (cenvp) + 1) {
    count += 1;
  }
  newp = _realloc_crt (_wenviron, (count + 1) * sizeof (wchar_t *));
  if (newp == NULL) {
    goto error;
  }
  for (i = 0; i < count; i++) {
    newp[i] = NULL;
  }
  cenvp = envp;
  for (i = 0; i < count; i++) {
    lsz = MtoWcmSZ (cenvp, -1);
    if (lsz == 0) {
      goto error;
    }
//    newp[i] = GETWMAIN (lsz);
    newp[i] = (wchar_t *) _malloc_crt((lsz) * sizeof (wchar_t));
    if (newp[i] == NULL) {
      goto error;
    }
//    if (mbstowcs (newp[i], cenvp, lsz) == CPYERR) {
    if (MtoWcm (cenvp, -1, newp[i], lsz) == 0) {
      goto error;
    }
    cenvp += strlen (cenvp) + 1;
  }
  newp[i] = NULL;
  _wenviron = newp;
  return TRUE;
error:
  if (newp != NULL) {
    for (i = 0; i < count; i++) {
      _free_crt (newp[i]);
    }
    _free_crt (newp);
  }
  return FALSE;
}
#endif

/*
 * __build_environ
 * Return value:
 *   0 if successful, -1 if any error.
 * Notes:
 *   __build_environ synchronizes the _environ structure with the _envptr structure.
 *   _environ structure:
 *     _environ points to an array of pointers to environment strings.
 *     Each environment string defines an environment variable.
 *     Environment strings have the form "VAR=VALUE".
 *     The last pointer in the array is NULL.
 *   _envptr structure:
 *     _envptr points to an environment block.
 *     The environment block contains one environment string after another, separated by
 *       nul characters ('\0').
 *     The end of the block is marked by a null string, that is, there is an additonal nul
 *     at the end of the block.
 *   __build_environ:
 *     If _environ is NULL
 *       allocates an array of pointers sufficient for all strings in the block pointed to
 *         by _envptr, plus the NULL pointer at the end.
 *     If _environ is not NULL
 *       reallocates _environ to the new size necessary.
 *     Sets each pointer to point to the beginning of each string in the _envptr block.
 *     Sets the NULL at the end.
 */
int __build_environ (void)
{
  register char *envstr;
  char *newptr;
  register int count;
  char **new_environ;

#ifdef __NT__
  char *envptr;
/*
 * ???: if starting character is '=' then skip until single null.
 * "=C:=C:\test\0var=XXXXX" points to 'v' character. see to SPAWN.C
 */
  envptr = _envptr;
  while (*envptr == '=') {
    envptr += strlen (envptr) + 1;
  }
  count = 1;
  for (envstr = envptr; *envstr != '\0'; envstr += strlen (envstr) + 1)
#else
  count = 1;
  for (envstr = _envptr; *envstr != '\0'; envstr += strlen (envstr) + 1)
#endif
    count += 1;
  count *= sizeof (char *);
  if (_environ == NULL)
    new_environ = _malloc_crt (count);
  else
    new_environ = _realloc_crt (_environ, count);
  if (new_environ == NULL)
    return -1;
  _environ = new_environ;
  count = 0;
#ifdef __NT__
  for (envstr = envptr; *envstr != '\0'; envstr += strlen (envstr) + 1)
#else
  for (envstr = _envptr; *envstr != '\0'; envstr += strlen (envstr) + 1)
#endif
  {
#if defined(_WIN32)
    newptr = _strdup_crt(envstr);
    if (!newptr)
       return -1;
    _environ[count] = newptr;
#else
    _environ[count] = envstr;
#endif

    count += 1;
  }
  _environ[count] = NULL;
#ifdef __NT__
  __build_wenviron ();
#endif
  return 0;
}

int (*__init_environ_ptr)(void) = __build_environ;


#ifdef _WINDLL

char ***_p_environ()
{
    return &_environ;
}


wchar_t ***_p_wenviron()
{
    return &_wenviron;
}


#endif
