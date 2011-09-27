/*_ coreleft.c   Modified by: Walter Bright
 * Copyright (C) 1991 by Digital Mars
 * All rights reserved.
 * Written by Walter Bright
 */

#include	<stdlib.h>

#ifdef __OS2__
#include "os2lib.h"
#endif
#ifdef __NT__
#include <windows.h>
#endif

unsigned _cdecl coreleft()
{
#if __NT__
   return GlobalCompact(0);
#else
   return 0;
#endif      
}