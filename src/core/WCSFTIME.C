/*_ wcsftime.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <setlocal.h>
#include <lcapi32.h>

size_t __CLIB wcsftime
(wchar_t *ws, size_t lsz, const wchar_t *wf, const struct tm *t ) {
 size_t	ret = 0;
 char *	fp = NULL;
 char *	sp = NULL;
 size_t	len;
  len = wcstombs (NULL, wf, 0);
  if (len != CPYERR) {
    len++;				/* Include NUL stopper */
    fp = GETCMAIN (len);
    if (fp != NULL) {
      if (wcstombs (fp, wf, len) != CPYERR) {
        sp = GETCMAIN (lsz * MB_CUR_MAX + 1);
        if (sp != NULL) {
          if (strftime (sp, lsz * MB_LEN_MAX, fp, t) > 0) {
            ret = mbstowcs (ws, sp, lsz + 1);
            if (len == CPYERR) {
              ret = 0;
            }
          }
        }
      }
    }
  }
  free (fp);
  free (sp);
  return ret;
}

#endif

/**/
