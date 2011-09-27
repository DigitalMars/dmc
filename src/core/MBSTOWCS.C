/*_ mbstowcs.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>
#include <errno.h>
#include <scrtl.h>

size_t __cdecl mbstowcs (wchar_t  *wcs, const char *s, size_t n) {
 size_t	len = 0;
 BYTE	uch;
  if (wcs != NULL) {
    if (n != 0) {
      if (IsCLocale (LC_CTYPE)) {
        while (len < n) {
          uch = (BYTE) s[len];
          *wcs = (wchar_t) uch;
          if (uch == '\0') {
            break;
          }
          len++;
          wcs++;
        }
      } else {
        len = MultiByteToWideChar (__locale_codepage, __locale_chars,
		s, -1, wcs, n);
        if (len == 0) {
         size_t		bc;
         size_t		i;
          if (GetLastError() == ERROR_NO_UNICODE_TRANSLATION) {
            goto error;
          }
          bc = 0;
          for (i = 0; i < n; i++) {
            uch = (BYTE) s[bc];
            if (uch == '\0') {
              break;
            }
            if (isleadbyte (uch)) {
              bc++;
            }
            bc++;
          }
          len = MultiByteToWideChar (__locale_codepage, MB_PRECOMPOSED,
		s, bc, wcs, n);
          if (len == 0) {
            goto error;
          }
        } else {
          len--;
        }
      }
    }
  } else {				/* take storage size */
    if (IsCLocale (LC_CTYPE)) {
      len = strlen (s);
    } else {
      len = MultiByteToWideChar (__locale_codepage, __locale_chars,
		s, -1, NULL, 0);
      if (len == 0) {
        goto error;
      }
      len--;
    }
  }
done:
  return len;
error:
  __set_errno (EILSEQ);
  len = (size_t) -1;
  goto done;
}

#else

/* Copyright (C) 1994 by Digital Mars		*/
/* All Rights Reserved				*/

#include <stdlib.h>

/*******************************
 */

size_t mbstowcs(wchar_t *pwcs, const char *s, size_t n)
{   size_t i;

    for (i = 0; i < n; i++)
    {
   // Make sure not to sign extend *s.
	if ((*pwcs = (unsigned char)*s) == 0)
	    break;
	pwcs++;
	s++;
    }
    return i;
}

#endif

/**/
