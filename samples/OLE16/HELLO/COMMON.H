/***
*
*  Copyright (C) 1993, Microsoft Corporation.  All Rights Reserved.
*  Information Contained Herein Is Proprietary and Confidential.
*
*  File:
*    common.h
*
*  Purpose:
*
*    Common definitions across Win16/Win32
*
*****************************************************************************/

#ifndef __Common_h_
#define __Common_h_

#include <windows.h>
#include <ole2.h>
#ifndef WIN32
#include <olenls.h>
#endif
#include <dispatch.h>


#ifdef WIN32
# define STRLEN     strlen
# define STRICMP    _stricmp
# define MEMCPY     memcpy
# define MEMCMP     memcmp
# define MEMSET     memset
# define STRSTR     strstr
#else
# define STRLEN     _fstrlen
# define STRICMP    _fstricmp
# define MEMCPY     _fmemcpy
# define MEMCMP     _fmemcmp
# define MEMSET     _fmemset
# define STRSTR     _fstrstr
#endif

#ifndef CLASS
# ifdef __TURBOC__
#  define CLASS class huge
# else
#  define CLASS class FAR
# endif
#endif


#endif // __Common_h_
