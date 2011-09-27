/*_ dprintf.c   Fri Aug 18 1989   Modified by: Walter Bright */
/* Copyright (C) 1986-1994 by Digital Mars  */
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* Fast screen display package			*/

#include	<stdio.h>
#include	<disp.h>
#include	<stdarg.h>
#include	<io.h>

/****************************
 * A printf() that uses the disp package.
 */

static int disp2_putc(int c,void *fp)
{
    return disp_putc(c);
}

int disp_printf(char *format,...)
{   extern int _pformat(int (__cdecl *)(int,void *),void *,const char *,va_list);

    return _pformat((int (__cdecl *)(int,void *))disp2_putc,NULL,format,(va_list)&(format) + sizeof(format));
}

