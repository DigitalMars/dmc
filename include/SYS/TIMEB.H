/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif


#ifndef __SYS_TIMEB_H
#define __SYS_TIMEB_H 1

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

typedef long time_t;
   
#pragma pack (__DEFALIGN)
  struct _timeb
  {
    time_t time;
    unsigned short millitm;
    short timezone, dstflag;
  };
#pragma pack ()

void __cdecl _ftime (struct _timeb *);
 
#if !__STDC__
# define ftime _ftime
# define timeb _timeb
#endif

#if __cplusplus
  }
#endif

#endif

