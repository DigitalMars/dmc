/*_ setchars.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <windows.h>
#include <setlocal.h>

/* MultiByteToWideChar converting options.
*/
int __locale_chars = MB_PRECOMPOSED | MB_ERR_INVALID_CHARS;

#endif

