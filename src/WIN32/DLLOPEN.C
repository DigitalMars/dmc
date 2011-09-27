/*_ dllopen.c   Modified by: Bill Andrews
 * Copyright (C) 1991-1994 by Digital Mars
 * All rights reserved.
 * Written by Bill Andrews
 *
 */

#if __NT__
#include <windows.h>

/**************************************************
*	Dllmain that does nothing in case user does not implement one.
*/

BOOL WINAPI	DllMain (HINSTANCE hInst, DWORD reason, LPVOID reserved)
{
  return TRUE;

}

#endif

