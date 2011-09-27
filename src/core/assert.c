/*_ assert.c   Fri May 12 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars  */
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* Modified for Windows 3.0 by G. Eric Engstrom		*/

#include	<stdio.h>
#include	<errno.h>
#include	<assert.h>
#include	<string.h>
#include	<stdlib.h>
#ifdef _WINDOWS
#include	<stdarg.h>
#include	<windows.h>
#endif

/**************************
 * Assertion failure.
 */

#ifdef _WINDOWS

/***********************
 * Use this helper function to get the arg list set up for _vsnprintf()
 */

static void __near _msgbox_printf(const char __near *format,...)
{
    char buf[81];

    _vsnprintf(buf,sizeof(buf) - 1,format,(va_list)&(format) + sizeof(format));
    buf[sizeof(buf) - 1] = 0;
    MessageBox(0,buf,"ASSERTION FAILURE",MB_OK|MB_ICONSTOP|MB_TASKMODAL);
}

#endif

void _assert(const void *e,const void *file,unsigned line)
{
#ifdef _WINDOWS
    MessageBeep(0);
    _msgbox_printf("'%s' on line %u in file '%s'",e,line,file);
    FatalAppExit(0,"Warning: Memory may be corrupted.");
#else
    fprintf(stderr,"Assertion failure: '%s' on line %u in file '%s'\n",e,line,file);
    abort();
#endif
}

