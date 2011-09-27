/* Copyright (C) 1994-2005 by Digital Mars
 * All Rights Reserved
 * www.digitalmars.com
 */

#include <windows.h>
#include <stdlib.h>

extern void __cdecl __faterr(char *);
CRITICAL_SECTION __rtl_criticalsection = { 0 };   /* Critical Section to be used by RTL */


void _rtl_critical_enter()
{
    EnterCriticalSection(&__rtl_criticalsection);
}

void _rtl_critical_exit()
{
    LeaveCriticalSection(&__rtl_criticalsection);
}

/*****
 *  Initialize the rtl_criticalsection
 */

void _STI_critical()
{
  InitializeCriticalSection(&__rtl_criticalsection);
}


void _STD_critical()
{
  DeleteCriticalSection(&__rtl_criticalsection);
}
