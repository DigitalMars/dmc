/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 *
 * The following definitions are used internally to Digital Mars RTL I/O routines.
 * They are not part of the published interface.
 * Names in this file are defined in one source file and referenced in one or more 
 *   other source files.
 * This file is included in both the defining and referencing files, insuring that
 *   the definition and references will remain in sync.
 *
 * This file contains I/O related definitions only. Note that this file has some
 * includes in it, while SCRTL.H does not.
 */
 
#pragma once

#if defined (_WINDOWS) || defined (__NT__)
#  include <windows.h>
#endif
#include <stdio.h>
#include <stdarg.h>
#include "mt.h"
#include "semlock.h"

#define FHND_APPEND 0x04
#define FHND_DEVICE 0x08
#define FHND_TEXT   0x10
#define FHND_BYTE   0x20	// byte stream
#define FHND_WCHAR  0x40	// wide oriented

struct __FILE2        /* must match definition in tmpnam.c	*/
{
  int tmpnum;
};

extern struct __FILE2 _iob2[];
extern unsigned char __fhnd_info[_NFILE];
extern int __cdecl _fmode;

#if (__OS2__ && __INTSIZE == 4) || __NT__
#  define _tmpnum(fp) ((fp)->__tmpnum)
#else
#  define _tmpnum(fp) (_iob2[(fp) - _iob].tmpnum)
#endif

FILE *_pascal __fopen (const char *name, const char *mode, FILE *fp);
FILE *_pascal __sopenf (const char *name, const char *mode, FILE *fp, int shflag);
void _pascal _freebuf (FILE *fp);

#ifdef __NT__
  void *_osfhnd[_NFILE];
  int __nt_creat (char *, int, int);
#endif

/* printf stuff */
#ifndef _UNICODE
typedef int (__cdecl *fp_t)(int,void *);
int _pformat(fp_t,void *,const char *,va_list);
#else
typedef wint_t (__cdecl *fp_t) (wint_t, void *);
int _wpformat(fp_t,void *,const wchar_t *,va_list);
#endif

/* scanf stuff */
#ifndef _UNICODE
typedef int (*fp_t)(int,void *);
typedef int (*fp2_t)(void *);
int _sfmt(fp_t,fp2_t,void *,const char *,va_list);
#else
typedef wint_t (*fp_t) (wint_t, void *);
typedef wint_t (*fp2_t) (void *);
int _wsfmt(fp_t,fp2_t,void *,const wchar_t *,va_list);
#endif

#ifdef __NT__
long __CLIB _wfilesize(const wchar_t *);
#endif

/* console I/O for NT only */
#ifdef __NT__
  extern HANDLE __conin, __conout;
#endif

