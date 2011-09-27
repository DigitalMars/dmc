/*_ winstart.c
 * Copyright (C) 1995-2001 by Digital Mars
 * All rights reserved.
 *
 * Win32 GUI App startup code
 */

#if defined(_WIN32)

#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"user32.lib")

#include <tchar.h>
#include <sctchar.h>
#include <windows.h>
#include <excpt.h>
#include <stdlib.h>
#include <lcapi32.h>


void _allocinit(void);
void _thread_init(void);
void _cinit(void);
void _dodtors(void);
void _clearbss(void);           // Routine to clear BSS Segment.
void __initmbctype(void);       // Initializes mbctype array
void _InitSemaphoreSys(void);   // Initialize semaphores for malloc
void CheckWin32sVer(void);      // Check that we are on proper Win32s
void _tsetargv();


#ifndef _UNICODE
int _acrtused = 0x9876;         /* cause this to get pulled in */
#else
int _wacrtused = 0x9876;        /* cause this to get pulled in for wide */
#endif

BOOL WINAPI _tWinMain(HINSTANCE, HINSTANCE, _TCHAR *, int);

/* Exception Filter */
LONG __xcfilter(LPEXCEPTION_POINTERS);

#ifdef _DLL
#define _argc	(*__p___argc())
#define __argv	(*__p___argv())
#define __wargv	(*__p___wargv())
#else
extern int _argc;
extern _TCHAR **__targv;
#endif

_TCHAR *_tcmdln;
unsigned _atopsp;		// "top" of stack

HINSTANCE __module_handle;


#ifndef _UNICODE
void WinMainCRTStartup()
#else
void wWinMainCRTStartup()
#endif
{
    __try
    {
        _clearbss();
	_atopsp = _ESP;
        _InitSemaphoreSys();
        __module_handle = GetModuleHandle(NULL);
#if !defined(_DLL)
        _allocinit();
#endif
        CheckWin32sVer();
        _tcmdln = __tGetCommandLine();
#if defined(_MT) && !defined(_DLL)
        _thread_init();
#endif
        __initmbctype();
        /*
         * NOTE: setargv will setup argc and argv, but also point _acmdln past
         * program name
         */
        _tsetargv();
        _cinit();
        exit(_tWinMain(__module_handle, NULL, _tcmdln, SW_SHOWDEFAULT));
    }
    __except(__xcfilter(GetExceptionInformation()))
    {
        _Exit(EXIT_FAILURE);
    }
}

#ifndef _UNICODE
#pragma startaddress(WinMainCRTStartup)  //Mark as starting point
#else
#pragma startaddress(wWinMainCRTStartup) //Mark as starting point
#endif

#endif
