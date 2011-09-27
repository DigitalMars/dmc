/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */
#include <stdlib.h>
#include <windows.h>
#include <dos.h>

#if __SMALL__ || __MEDIUM__
unsigned __cdecl coreleft()
{
   return LocalCompact(0);
}
#else
unsigned long __cdecl coreleft()
{
   return GlobalCompact(0);
}
#endif
