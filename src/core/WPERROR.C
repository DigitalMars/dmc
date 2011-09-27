/*_ wperror.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

void __CLIB _wperror (const wchar_t *ws) {
#if 0
    fputws (ws, stderr);
    fputs (": ", stderr);
    fputs (strerror (errno), stderr);
    fputc ('\n', stderr);
#else
    if (ws != NULL && wcslen (ws) > 0) {
      fprintf (stderr, "%S: ", ws);
    }
    fprintf (stderr, "%s\n", strerror (errno));
#endif
}

#endif

/**/
