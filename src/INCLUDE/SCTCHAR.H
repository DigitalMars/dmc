/*
 * Copyright (C) 1995 by Digital Mars.
 * SCTCHAR.H  Controls mapping of string routines to either unicode or
 * MBCS or sbcs variants for internal.
 * $Revision: 1.1.1.1 $
 */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SCTCHAR_H
#define __SCTCHAR_H

# if __cplusplus
extern "C" {
# endif

# ifdef	_UNICODE	/* Current UNICODE Support */

/* ProgramEntry/Environment
*/
#define _tenvptr	_wenvptr	/* Internal */
#define _tsetargv	_wsetargv

/* Execute functions
*/
#define _texec		_wexec		/* Internal */
#define __tspawn	__wspawn	/* Internal */
#define __tseek_envp	__wseek_envp	/* Internal */

/* Environment/Path functions
*/
#define __tfnmerge	wfnmerge	/* Internal */
#define __tfnsplit	wfnsplit	/* Internal */

/* Stdio functions
*/
#define ___tfopen	__wfopen	/* Internal */
#define ___tsopenf	__wsopenf	/* Internal */

/* Io functions
*/
#define _tfinddata_t	_wfinddata_t
#define __tfilesize	_wfilesize	/* Internal */
#define __tfindfirst	_wfindfirst	/* Internal */
#define __tfindnext	_wfindnext	/* Internal */

# else			/* SingleByte and MultiByte Character Support */

#  ifdef _MBCS
#   ifndef _WIN32
#    ifdef isleadbyte
#undef  isleadbyte
#    endif
#define isleadbyte	_ismbblead
#   endif
#  endif

/* ProgramEntry/Environment
*/
#define _tenvptr	_envptr		/* Internal */
#define _tsetargv	_setargv

/* Execute functions
*/
#define _texec		_exec		/* Internal */
#define __tspawn	__spawn		/* Internal */
#define __tseek_envp	__seek_envp	/* Internal */

/* Environment/Path functions
*/
#define __tfnmerge	fnmerge
#define __tfnsplit	fnsplit

/* Stdio functions
*/
#define ___tfopen	__fopen
#define ___tsopenf	__sopenf

/* Io functions
*/
#define _tfinddata_t	_finddata_t
#define __tfilesize	filesize
#define __tfindfirst	findfirst
#define __tfindnext	findnext

# endif

#  if 0 /* DIRECT.H No support routine */
int __CLIB _wgetcurdir(int drive, wchar_t *dir);
#  endif
#if 0 /* IO.H No support routine */
int __CLIB _wremove(const wchar_t *);	/* STDIO.H */
int __CLIB _wdos_open(const wchar_t *,int);
int __CLIB _wcreatnew(const wchar_t *, int);
int __CLIB _wcreattemp(wchar_t *, int);
#endif
#  if 0 /* STDIO.H No support routine */
int __CLIB _wrename(const wchar_t *, const wchar_t *);	/* IO.H */
long __CLIB _wfilesize(const wchar_t *);
#  endif
#  if 0 /* STRING.H No Support routines */
char * __CLIB _wstpcpy(wchar_t *,const wchar_t *);
void __CLIB _wswab(wchar_t *, wchar_t *, size_t);
wchar_t * __CLIB _wcserror(const wchar_t *);
#  endif

# if __cplusplus
}
# endif

#endif

