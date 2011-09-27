/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.3 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __STDLIB_H
#define __STDLIB_H 1

#if __cplusplus
extern "C" {
#endif

/* Define _CRTAPI1 (for compatibility with the NT SDK) */
#ifndef _CRTAPI1
#define _CRTAPI1 __cdecl
#endif

/* Define _CRTAPI2 (for compatibility with the NT SDK) */
#ifndef _CRTAPI2
#define _CRTAPI2 __cdecl
#endif

/* Define CRTIMP */
#ifndef _CRTIMP
#if defined(_WIN32) && defined(_DLL)
#define _CRTIMP  __declspec(dllimport)
#else
#define _CRTIMP
#endif
#endif

#if __OS2__ && __INTSIZE == 4
#define __CLIB __stdcall
#else
#define __CLIB __cdecl
#endif

#define EXIT_SUCCESS   0
#define EXIT_FAILURE   1

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#if _M_AMD64
typedef unsigned long long size_t;
#else
typedef unsigned size_t;
#endif

//#ifndef __STDC__
#if !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED 1
#endif
//#endif

#pragma pack(__DEFALIGN)
typedef struct _DIV { int  quot,rem; } div_t;
typedef struct _LDIV { long quot,rem; } ldiv_t;
#if __INTSIZE == 4
typedef struct _LLDIV { long long quot,rem; } lldiv_t;
#endif
#pragma pack()

#if !__STDC__

#if !defined(ERANGE)
#define ERANGE 34
#endif

#define HUGE_VAL       __inf

#define RAND_MAX       32767

#define _div_t _DIV
#define _ldiv_t _LDIV

#ifdef __NT__
extern int __CLIB	__locale_mbsize;
#define MB_CUR_MAX	__locale_mbsize
#else
#define MB_CUR_MAX	1
#endif

#endif

#define _MAX_PATH   260
#define _MAX_DRIVE  3
#define _MAX_DIR    256
#define _MAX_FNAME  256
#define _MAX_EXT    256

#if defined(_MT) || __cplusplus

#if !__STDC__
extern int *__CLIB _errno (void);
#define errno (*_errno())
#endif
extern int *__CLIB __doserrno (void);
#define _doserrno (*__doserrno ())

#else

#if !__STDC__
extern int __CLIB errno;
#endif
extern int __CLIB _doserrno;

#endif

double __CLIB atof(const char *);
#define _atold atof
int    __CLIB atoi(const char *);
long   __CLIB atol(const char *);
double __CLIB strtod(const char *,char **);
long double __CLIB strtold(const char *,char **);
long   __CLIB strtol(const char *,char **,int);
unsigned long  __CLIB strtoul(const char *,char **,int);
#if __INTSIZE == 4
long long   __CLIB atoll(const char *);
long long   __CLIB strtoll(const char *,char **,int);
unsigned long long __CLIB strtoull(const char *,char **,int);
#endif
int    __CLIB rand(void);
void   __CLIB srand(unsigned);
int    __CLIB random(int num);
void    __CLIB randomize(void);
_CRTIMP void * __CLIB calloc(size_t,size_t);
_CRTIMP void   __CLIB free(void *);
_CRTIMP void * __CLIB malloc(size_t);
_CRTIMP void * __CLIB realloc(void *,size_t);
void   __CLIB abort(void);
int    __CLIB atexit(void (*)(void));
void   __CLIB exit(int);
void   __CLIB _Exit(int);
void   __CLIB _exit(int);

#pragma noreturn (abort)
#pragma noreturn (_exit)
#pragma noreturn (_Exit)

_CRTIMP char * __CLIB getenv(const char *);
int    __CLIB system(const char *);
void * __CLIB bsearch(const void *,const void *,size_t,size_t,
       int (__CLIB *)(const void *,const void *));
void   __CLIB qsort(void *,size_t,size_t,
       int (__CLIB *)(const void *,const void *));
int    __CLIB abs(int);
long   __CLIB labs(long);
div_t  __CLIB div(int,int);
ldiv_t __CLIB ldiv(long,long);
#if __INTSIZE == 4
long long   __CLIB llabs(long long);
lldiv_t __CLIB lldiv(long long, long long);
#endif
int    __CLIB mblen(const char *, size_t);
size_t __CLIB _mbstrlen(const char *);
int    __CLIB mbtowc(wchar_t *, const char *, size_t);
int    __CLIB wctomb(char *, wchar_t);
size_t __CLIB mbstowcs(wchar_t *, const char *, size_t);
size_t __CLIB wcstombs(char *, const wchar_t *, size_t);
void   __CLIB perror(const char *);
#ifndef toupper
int    __CLIB toupper(int);
#endif
#ifndef tolower
int    __CLIB tolower(int);
#endif

unsigned       __CLIB _rotl(unsigned,int);
unsigned       __CLIB _rotr(unsigned,int);
unsigned long  __CLIB _lrotl(unsigned long,int);
unsigned long  __CLIB _lrotr(unsigned long,int);
char * __CLIB _fullpath(char *buf,const char *path,size_t buflen);

#if !__STDC__
#define fullpath       _fullpath
#endif

extern int __CLIB sys_nerr;
extern char * __CLIB sys_errlist[];
#define _sys_nerr sys_nerr
#define _sys_errlist sys_errlist

#ifndef __STDC__

#if __INTSIZE == 2
int    __CLIB __far _fmblen(const char __far *,size_t);
int    __CLIB __far _fmbtowc(wchar_t __far *,const char __far *,size_t);
int    __CLIB __far _fwctomb(char __far *,wchar_t);
size_t __CLIB __far _fmbstowcs(wchar_t __far *, const char __far *, size_t);
size_t __CLIB __far _fwcstombs(char __far *, const wchar_t __far *, size_t);
#else
int    __CLIB _fmblen(const char __far *,size_t);
int    __CLIB _fmbtowc(wchar_t __far *,const char __far *,size_t);
int    __CLIB _fwctomb(char __far *,wchar_t);
size_t __CLIB _fmbstowcs(wchar_t __far *, const char __far *, size_t);
size_t __CLIB _fwcstombs(char __far *, const wchar_t __far *, size_t);
unsigned long long __CLIB strtoull(const char *,char **,int);
#endif

#if __INTSIZE == 2
extern char __far *__cdecl _pgmptr;
#else
extern char *__cdecl _pgmptr;
extern wchar_t *__cdecl _wpgmptr;
#endif

extern unsigned int __cdecl _osver;
#ifndef __NT__
extern unsigned short __cdecl _psp;
extern unsigned char __cdecl _osmajor;
extern unsigned char __cdecl _osminor;
#endif

#ifdef _DLL
extern int * __CLIB __p___argc(void);
extern char *** __CLIB __p___argv(void);
extern wchar_t *** __CLIB __p___wargv(void);
#define __argc	(*__p___argc())
#define __argv	(*__p___argv())
#define __wargv	(*__p___wargv())
#else
extern int __cdecl __argc;
extern char __cdecl ** __argv;
extern wchar_t __cdecl ** __wargv;
#endif

#if defined (_WIN32)
wchar_t ** __CLIB __wparsecmdline(const wchar_t *, int *);
char ** __CLIB __parsecmdline(const char *, int *);
#endif

#if defined(_WINDOWS) || defined(__NT__)
extern unsigned char __cdecl _winmajor;
extern unsigned char __cdecl _winminor;
extern unsigned int __cdecl _winver;
#endif

#define _DOS_MODE 0
#define _OS2_MODE 1
#define _WIN_MODE 2

#if !__STDC__
#define DOS_MODE _DOS_MODE
#define OS2_MODE _OS2_MODE
#define WIN_MODE _WIN_MODE
#endif

extern unsigned char __cdecl _osmode;

#define _REAL_MODE 0
#define _PROT_MODE 1

extern unsigned char __cdecl _cpumode;

extern int __cdecl _fmode;

void __cdecl _splitpath(const char *, char *,char *, char *, char *);
void __cdecl _makepath(char *, const char *,const char *, const char *, const char *);
void __cdecl _searchenv(const char *filename, const char *varname, char *pathname);

#include <heapstat.h>

_CRTIMP int __cdecl _heapchk(void);
_CRTIMP int __cdecl _heapset(unsigned);
#if __INTSIZE == 2
int __cdecl _heapadd(void __far *,size_t);
#else
int __cdecl _heapadd(void *,size_t);
#endif
int __cdecl _heapmin(void);

#ifdef __NT__
#define _expand realloc
#else
void * __cdecl _expand(void *,size_t);
#endif

_CRTIMP size_t __cdecl _msize(void *);

#if __LARGE__ || __COMPACT__
void * __cdecl _fmalloc(size_t);
void * __cdecl _fcalloc(size_t,size_t);
void * __cdecl _frealloc(void *,size_t);
void __cdecl _ffree(void *);
size_t __cdecl _fmsize(void *);
unsigned long __cdecl coreleft(void);
unsigned long __cdecl farcoreleft(void);
#if _WINDOWS
void __near * __cdecl _nmalloc(size_t);
void __near * __cdecl _ncalloc(size_t,size_t);
void __near * __cdecl _nrealloc(void __near *,size_t);
void __cdecl _nfree(void __near *);
size_t __cdecl _nmsize(void __near *);
size_t __cdecl _memmax(void);
size_t __cdecl _memavl(void);
unsigned int __cdecl _freect(size_t);
#endif
#else
void * __cdecl _nmalloc(size_t);
void * __cdecl _ncalloc(size_t,size_t);
void * __cdecl _nrealloc(void *,size_t);
void __cdecl _nfree(void *);
size_t __cdecl _nmsize(void *);
size_t __cdecl _memmax(void);
size_t __cdecl _memavl(void);
unsigned int __cdecl _freect(size_t);
unsigned __cdecl coreleft(void);
unsigned long __cdecl farcoreleft(void);
#if __INTSIZE == 2 || (!__NT__ && !__OS2__)
void __far * __cdecl _fmalloc(size_t);
void __far * __cdecl _fcalloc(size_t,size_t);
void __far * __cdecl _frealloc(void __far *,size_t);
void __cdecl _ffree(void __far *);
size_t __cdecl _fmsize(void __far *);
#endif
#endif

#if __INTSIZE == 2
void __huge *_halloc (long, size_t);
void _hfree (void __huge *);
#endif

char * __CLIB itoa(int,char *,int);
char * __CLIB _itoa(int, char *,int);
char * __CLIB ltoa(long, char *, int);
char * __CLIB _ltoa(long, char *,int);
char *  __CLIB ultoa(unsigned long, char *, int);
char * __CLIB ecvt(double,int,int *,int *);
char * __CLIB fcvt(double,int,int *,int *);
char * __CLIB _fcvt(double,int,int *,int *);
char * __CLIB gcvt(double,int,char *);
void * __cdecl sbrk(size_t);
void   __CLIB swab(char *,char *,size_t);
float  __CLIB strtof(const char *,char **);
#define _swab swab
#define _ecvt ecvt
#define _gcvt gcvt
#if !_WIN32
#define strtold        strtod
#endif
#define _strtold       strtold
#define _ultoa ultoa
#if _MSDOS
void __ss *    __CLIB alloca(size_t);
#else
void *    __CLIB alloca(size_t);
#endif
#define _alloca alloca
size_t __CLIB stackavail(void);
size_t __CLIB _chkstack(void);
#define _stackavail stackavail

#if __INTSIZE == 4
int __cdecl _fatexit (void (__cdecl *func)(void));
typedef void (__cdecl * _fonexit_t)(void);
_fonexit_t __cdecl _fonexit (_fonexit_t func);
#else
int __cdecl __far _fatexit (void (__cdecl __far *func)(void));
typedef void (__cdecl __far * _fonexit_t)(void);
_fonexit_t __cdecl __far _fonexit (_fonexit_t func);
#endif

typedef void (__cdecl * _onexit_t)(void);
_onexit_t __cdecl _onexit (_onexit_t func);

#ifndef __STDC__
#define onexit _onexit
#endif

#if __INTSIZE == 2
void __far * __far __CLIB __mallocf(size_t);
void        __far __CLIB __freef(void __far *);
#endif
_CRTIMP int            __CLIB putenv(const char *);

#define _putenv putenv

/* Min and Max macros useful in both C and CPP */
#define __max(a,b)            (((a) > (b)) ? (a) : (b))
#define __min(a,b)            (((a) < (b)) ? (a) : (b))


#ifndef __cplusplus

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#endif

extern int (__far __cdecl *_malloc_handler)(void);
extern char * __cdecl _envptr;
#ifdef _DLL
_CRTIMP char ***__CLIB _p_environ (void);
#define _environ (*_p_environ())
#else
extern char ** __cdecl _environ;
#endif

#define environ _environ

#ifdef __NT__
#define _aenvptr _envptr
extern wchar_t * __cdecl _wenvptr;
#ifdef _DLL
_CRTIMP wchar_t ***__CLIB _p_wenviron (void);
#define _wenviron (*_p_wenviron())
#else
extern wchar_t ** __cdecl _wenviron;
#endif

#define wenviron _wenviron
#endif

#if !(defined (_WINDOWS) || defined (__NT__) || defined (DOS386))
extern int __CLIB _fileinfo;
#endif

#if M_UNIX || M_XENIX
extern char * __cdecl optarg;
extern int __cdecl optind;
extern int __cdecl opterr;
extern int __cdecl optopt;
#endif

#endif

#ifdef __NT__
#ifndef __STDC__
int __CLIB _wtoi(const wchar_t *);
long __CLIB _wtol(const wchar_t *);
//double __CLIB wcstod(const wchar_t *, wchar_t **);
//long double __CLIB wcstold(const wchar_t *, wchar_t **);
//long __CLIB wcstol(const wchar_t *, wchar_t **, int);
//unsigned long __CLIB wcstoul(const wchar_t *, wchar_t **, int);
//long long __CLIB wcstoll(const wchar_t *, wchar_t **, int);
//unsigned long long __CLIB wcstoull(const wchar_t *, wchar_t **, int);
_CRTIMP wchar_t * __CLIB _wgetenv(const wchar_t *);
int __CLIB _wsystem(const wchar_t *);
wchar_t * __CLIB _itow(int, wchar_t *, int);
wchar_t * __CLIB _ltow(long, wchar_t *, int);
wchar_t * __CLIB _ultow(unsigned long, wchar_t *, int);
wchar_t * __CLIB _wfullpath(wchar_t *, const wchar_t *, size_t);
void __CLIB _wmakepath(wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *);
void __CLIB _wperror(const wchar_t *);
_CRTIMP int __CLIB _wputenv(const wchar_t *);
void __CLIB _wsearchenv(const wchar_t *, const wchar_t *, wchar_t *);
void __CLIB _wsplitpath(const wchar_t *, wchar_t *, wchar_t *, wchar_t *, wchar_t *);
#define itow _itow
#define ltow _ltow
#define ultow _ultow
#endif
#endif

#if __cplusplus
}
#endif

#endif
