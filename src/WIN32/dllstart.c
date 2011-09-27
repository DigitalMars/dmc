/*_ dllentry.c   Modified by: Bill Andrews
 * Copyright (C) 1991-1994 by Digital Mars
 * All rights reserved.
 * www.digitalmars.com
 */

#if defined(_WIN32)

#include <windows.h>
#include <stdlib.h>
#include <process.h>
#include "ehsup.h"
#include "semlock.h"
#include "mt.h"

typedef BOOL(WINAPI * DLLMAINPTR) (HINSTANCE, DWORD, LPVOID);

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID);
void _allocinit(void);
void _cinit(void);
void _dodtors(void);
void _clearbss(void);           // Routine to clear BSS Segment.
void __initmbctype(void);       // Initializes mbctype array
void _InitSemaphoreSys(void);   // Initialize semaphores for malloc

extern int __terminate_done;    // Set when exit has been run
DLLMAINPTR _pRawDllMain;        // Function ptr, supplied by user

int _acrtused_dll = 0x09876;    // cause this module to get linked in.

HINSTANCE __module_handle;


/* Declarations for Default Exception Handling */
EXCEPTION_DISPOSITION __cdecl __xcnotify(EXCEPTION_RECORD *, ESTABLISHER_FRAME *, CONTEXT *);



/**************************************************
 *  If this is a DLL this routine must be the entry point
 *  NOTES:  DLLS are treated differently under Win32s than under NT/Windows 95.
 *    Under Win32s, each exe/process that attaches to this DLL share the same
 *    code and data, unlike under NT, where each gets its own address space.
 *    this means that under Win32s, care needs to be taken not to call
 *    static constructors or destructors only once. Accomplish this
 *    by keeping a counter for the number of processes that are attached,
 *    at 0 call cinit and when it gets back to 0, call destructors.
 */
BOOL WINAPI _DllMainCRTStartup(HINSTANCE hdll, DWORD reason, LPVOID reserved)
{
    DWORD version = GetVersion();
    BOOL fWin32s = ((version & 0x80000000) && (LOBYTE(LOWORD(version)) < 0x04)) ? 1 : 0;
    BOOL retval = TRUE;
    static int __processes_attached = 0;

    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
            if (reason == DLL_PROCESS_ATTACH)
            {
                if (!__processes_attached || !fWin32s)
                {
                    _clearbss();// Clear uninitialized Data
                    __module_handle = hdll;
                    _InitSemaphoreSys();
                #if !defined(_DLL)
                    _allocinit();
                    _thread_init(); //*after* __allocinit
                #endif
                    __initmbctype();
                }
            }
            // First call user supplied function and check return
            retval = (_pRawDllMain) ? _pRawDllMain(hdll, reason, reserved) : TRUE;
            if (retval)
            {
                if (reason == DLL_PROCESS_ATTACH)
                {
                    if (!__processes_attached || !fWin32s)
                    {
                        _cinit();
                    }
                }
                retval = DllMain(hdll, reason, reserved); // Call DllMain

                /*
                 * Deal with user failure, since we called static constructors
                 * we should call static destructors
                 */
                if (!retval && (reason == DLL_PROCESS_ATTACH))
                {
                    if (!__processes_attached || !fWin32s)
                    {
                        _dodtors(); /* call static destructors      */
                    }
                }
                /* Things were ok from DllMain, so increment process attached. */
                else if (reason == DLL_PROCESS_ATTACH)
                {
                    __processes_attached++;
                }
            }
            break;
        case DLL_PROCESS_DETACH:
        case DLL_THREAD_DETACH:
            if (reason == DLL_PROCESS_DETACH)
                __processes_attached--; // Decrement Process count
        #if !defined(_DLL)
            else
                _removethreadtableentry(GetCurrentThreadId());
        #endif

            retval = DllMain(hdll, reason, reserved);

            // If termination has not already occured, then call _cexit.
            // to call dtors etc.
            if (!__terminate_done && (reason == DLL_PROCESS_DETACH))
            {
                // Under Win32s, there could still be processes attached, do
                // not destruct
                if (!__processes_attached || !fWin32s)
                {
                    _cexit();
                }
            }

            if (_pRawDllMain)
                retval = _pRawDllMain(hdll, reason, reserved);
            break;
        default:
            retval = DllMain(hdll, reason, reserved);
            break;
    }
    return retval;
}

#pragma startaddress(_DllMainCRTStartup) // Mark as starting point in DLL.

#endif
