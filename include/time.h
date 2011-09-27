/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.2 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __TIME_H
#define __TIME_H 1

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

#if _M_AMD64
typedef unsigned long long size_t;
#else
typedef unsigned size_t;
#endif

#ifndef __STDC__
#if !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED 1
#endif
#endif

#define CLOCKS_PER_SEC ((clock_t)1000) /* (clock_t / CLOCKS_PER_SEC) == seconds */

#if !__STDC__
#define CLK_TCK ((clock_t)1000) /* (clock_t / CLK_TCK) == seconds      */
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

typedef long clock_t;
#ifndef __TIME_T_DEFINED
typedef long time_t;
#define __TIME_T_DEFINED
#endif

#if __OS2__ && __INTSIZE == 4
#define __CLIB __stdcall
#else
#define __CLIB __cdecl
#endif

_CRTIMP extern int daylight;
_CRTIMP extern long timezone;
_CRTIMP extern long altzone;
_CRTIMP extern char * const tzname[2];

#define _daylight daylight
#define _timezone timezone
#define _tzname tzname

#ifndef _STRUCTTM_DEFINED
#define _STRUCTTM_DEFINED 1
struct tm
{      int     tm_sec,
               tm_min,		// 0..59
               tm_hour,		// 0..23
               tm_mday,		// 1..31
               tm_mon,		// 0..11
               tm_year,
               tm_wday,		// 0..6, Sunday=0
               tm_yday,		// day of year as 0..365
               tm_isdst;
};
#endif


#define _TIMEOFFSET     315558000

#if !__STDC__
#define TIMEOFFSET  _TIMEOFFSET
#endif

clock_t __CLIB clock(void);
time_t __CLIB time(time_t *);
time_t __CLIB mktime(struct tm *);
char * __CLIB asctime(const struct tm *);
char * __CLIB ctime(const time_t *);
struct tm * __CLIB localtime(const time_t *);
struct tm * __CLIB gmtime(const time_t *);
size_t __CLIB strftime(char *,size_t,const char *,const struct tm *);
char * __CLIB _strdate(char *dstring);
char * __CLIB _strtime(char *timestr);
double __CLIB difftime(time_t t1,time_t t2);
void __CLIB _tzset(void);


#if !__STDC__
#if M_UNIX || M_XENIX

#if !defined(_POSIX_SOURCE)
struct timeval {
       long    tv_sec;
       long    tv_usec;
};

struct timezone {
       int     tz_minuteswest;
       int     tz_dsttime;
};

#define        DST_NONE        0
#define        DST_USA         1
#define        DST_AUST        2
#define        DST_WET         3
#define        DST_MET         4
#define        DST_EET         5
#define        DST_CAN         6
#define        DST_GB          7
#define        DST_RUM         8
#define        DST_TUR         9
#define        DST_AUSTALT     10

#define        timerisset(tvp) ((tvp)->tv_sec || (tvp)->tv_usec)
#define        timercmp(tvp, uvp, cmp) ((tvp)->tv_sec cmp (uvp)->tv_sec || \
        (tvp)->tv_sec == (uvp)->tv_sec && (tvp)->tv_usec cmp (uvp)->tv_usec)
#define        timerclear(tvp) (tvp)->tv_sec = (tvp)->tv_usec = 0

#define        ITIMER_REAL     0
#define        ITIMER_VIRTUAL  1
#define        ITIMER_PROF     2

struct itimerval {
       struct timeval  it_interval;
       struct timeval  it_value;
};

#if !defined(__STDC__)
int __cdecl adjtime(struct timeval *, struct timeval *);
int __cdecl getitimer(int, struct itimerval *);
int __cdecl setitimer(int, struct itimerval *, struct itimerval *);
#endif /* __STDC__ */
#endif /* !defined(_POSIX_SOURCE) */

struct tms
{      time_t  tms_utime;      /* user                 */
       time_t  tms_stime;      /* system               */
       time_t  tms_cutime;     /* user children        */
       time_t  tms_cstime;     /* system children      */
};

typedef struct
        {
        time_t  tv_sec;
        long    tv_nsec;
        } timestruc_t;

clock_t __CLIB times(struct tms *);
unsigned int __CLIB sleep(unsigned int);
long int __CLIB nap(long int mS);
#define usleep(s) (void)nap((s)/1000)
#define msleep(s) (void)nap(s)

#else /* M_UNIX || M_XENIX */

void __CLIB sleep(time_t);
void __CLIB usleep(unsigned long);
void __CLIB msleep(unsigned long);
#endif /* M_UNIX || M_XENIX */

void __cdecl tzset(void);

#endif /* !__STDC__ */

#ifdef __NT__
#ifndef __STDC__
wchar_t * __cdecl _wasctime(const struct tm *);
wchar_t * __cdecl _wctime(const time_t *);
size_t __cdecl wcsftime(wchar_t *, size_t, const wchar_t *, const struct tm *);
wchar_t * __cdecl _wstrdate(wchar_t *);
wchar_t * __cdecl _wstrtime(wchar_t *);
#endif
#endif

#if __cplusplus
}
#endif

#endif
