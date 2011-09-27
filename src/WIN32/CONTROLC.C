/*_ control.c   Modified by: Bill Andrews
 * Copyright (C) 1991 by Digital Mars
 * All rights reserved.
 * Written by Bill Andrews
 */

#include <controlc.h>

#ifdef __NT__

#include <windows.h>

BOOL WINAPI _BreakHandler (ULONG type)
{
    if (_controlc_handler) {
	(*_controlc_handler)();
	return TRUE;
	}
    return FALSE;
}

void controlc_open (void)
{
    SetConsoleCtrlHandler (_BreakHandler, TRUE);
}


void controlc_close (void)
{
    SetConsoleCtrlHandler (_BreakHandler, FALSE);
}

#endif
