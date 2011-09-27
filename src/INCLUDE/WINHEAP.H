/***
*winheap.h - Private include file for winheap directory.
*
*       Copyright (c) 1988-1995, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains information needed by the C library heap code.
*
*       [Internal]
*
*******************************************************************************/

#ifndef _INC_WINHEAP
#define _INC_WINHEAP

#ifndef _CRTBLD
/*
 * This is an internal C runtime header file. It is used when building
 * the C runtimes only. It is not to be used as a public header file.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <windows.h>

#ifdef _M_ALPHA
#define _PAGESIZE_      0x2000      /* one page */
#else  /* _M_ALPHA */
#define _PAGESIZE_      0x1000      /* one page */
#endif  /* _M_ALPHA */

#ifndef DLL_FOR_WIN32S
extern  HANDLE _crtheap;
#endif  /* DLL_FOR_WIN32S */

void * __cdecl _nh_malloc( size_t, int);
void * __cdecl _heap_alloc(size_t);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#ifdef DLL_FOR_WIN32S
#include <win32s.h>
#endif  /* DLL_FOR_WIN32S */

#endif  /* _INC_WINHEAP */
