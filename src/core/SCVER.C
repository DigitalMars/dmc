/*_ scver.c   */
/* Copyright (C) 1995   */
/* All rights reserved. */
/* $Revision: 1.1.1.3 $ */

#define RTL_VERSION 0x750  /* Rtl Version number */
#define RTL_BUILD 35    /* Rtl build number */
static char versionKey[] = "(SCVersion)@rtl 7.50b35"; /* scver string */

#if !defined(_DLL)
unsigned __rtl_version()
{
    return RTL_VERSION;
}

unsigned __rtl_build()
{
    return RTL_BUILD;
}

#endif

