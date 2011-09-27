/*_ crtinit.c
 * Copyright (C) 1995 by Digital Mars
 * All rights reserved.
 * $Revision:
 */

#if defined(_WIN32)

#include <windows.h>
#include <process.h>
#include "mt.h"

void _thread_init(void);
void _allocinit(void);
void _cinit(void);
void _dodtors(void);
void _clearbss(void);           // Routine to clear BSS Segment.
void __initmbctype(void);       // Initializes mbctype array
void _InitSemaphoreSys(void);   // Initialize semaphores for malloc

//void init_dll_iob(void);

extern int __terminate_done;    // Set when exit has been run

HINSTANCE __module_handle;

BOOL WINAPI _CRT_INIT(HINSTANCE hdll, DWORD reason, LPVOID reserved)
{
    BOOL retval = TRUE;
    DWORD version = GetVersion();
    BOOL fWin32s = ((version & 0x80000000) && (LOBYTE(LOWORD(version)) < 0x04)) ? 1 : 0;
    static int __processes_attached = 0;

    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            if (!__processes_attached || !fWin32s)
            {
                //init_dll_iob();
                _clearbss();    // Clear uninitialized Data
                _InitSemaphoreSys();
                __module_handle = hdll;
            #if !defined(_DLL)
                _allocinit();
            #endif
            #if defined(_MT) && !defined(_DLL)
                _thread_init();
            #endif
                __initmbctype();
                _cinit();
            }
            __processes_attached++;
            break;
        case DLL_PROCESS_DETACH:
            __processes_attached--;
            if ((!__processes_attached || !fWin32s) && !__terminate_done)
                _cexit();
            break;
        default:
        #if !defined(_DLL)
            if (reason == DLL_THREAD_DETACH)
                _removethreadtableentry(GetCurrentThreadId());
        #endif
            break;
    }
    return retval;
}

#if defined(_WINDLL)
FILE * _p_iob(void)
{
    return &_iob[0];
}
#endif

/*
void init_dll_iob(void)
{
  int index;
    FILE *pIob = _p_iob();
    memset( pIob, 0, sizeof( FILE ) );
    pIob->_flag = _IOREAD;   // stdin
    pIob += 1;
    memset( pIob, 0, sizeof( FILE ) );
    pIob->_flag = _IOWRT;   // stdout
    pIob->_file = 1;
    pIob += 1;
    memset( pIob, 0, sizeof( FILE ) );
    pIob->_flag = _IOWRT | _IONBF;   // stderr
    pIob->_file = 2;
    pIob += 1;
    for(index=3; index < _NFILE; index++)
    {  // the rest have all zero entries
        memset( pIob, 0, sizeof( FILE ) );
        pIob += 1;
    }
}*/

#if defined(_WINDLL)
#pragma startaddress(_CRT_INIT)
#endif

#endif
