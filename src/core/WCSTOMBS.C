/*_ wcstombs.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdlib.h>
#include <locale.h>
#include <setlocal.h>
#include <limits.h>
#include <errno.h>
#include <scrtl.h>

size_t __cdecl wcstombs (char *s, const wchar_t *wcs, size_t n) {
 char		ch;
 wchar_t	wch;
 size_t		i;
 size_t		slen;
 size_t		len = 0;
 BOOL		stus;
 char		buf[MB_LEN_MAX];
  if (s != NULL) {
    if (n != 0) {
      if (IsCLocale (LC_CTYPE)) {
        while (len < n) {
          wch = *wcs;
          if (wch > 0x00FF) {
            goto error;
          }
          s[len] = (char) wch;
          if (wch == L'\0') {
            break;
          }
          wcs++;
          len++;
        }
      } else {
        if (MB_CUR_MAX == 1) {
          if (n > 0) {
            for (i = 0; i < n; i++) {
              if (wcs[i] == L'\0') {
                n = i + 1;
                break;
              }
            }
          }
          len = WideCharToMultiByte (__locale_codepage,
		WC_COMPOSITECHECK | WC_SEPCHARS,
		wcs, n, s, n, NULL, &stus);
          if (len == 0 || stus != 0) {
            goto error;
          } else {
            if (s[len - 1] == '\0') {
              len--;
            }
          }
        } else {
          len = WideCharToMultiByte (__locale_codepage,
		WC_COMPOSITECHECK | WC_SEPCHARS,
		wcs, -1, s, n, NULL, &stus);
          if (len == 0) {
            if (stus != 0) {
              goto error;
            } else {
              while (len < n) {
                slen = WideCharToMultiByte (__locale_codepage,
			0, wcs, 1, buf, MB_CUR_MAX, NULL, &stus);
                if (slen == 0 || stus != 0) {
                  goto error;
                } else {
                  if (len + slen > n) {
                    break;
                  } else {
                    for (i = 0; i < slen; i++) {
                      ch = s[len] = buf[i];
                      if(ch == '\0') {
                        goto done;
                      }
                      len++;
                    }
                  }
                  wcs++;
                }
              }
            }
          } else {
            if (stus != 0) {
              goto error;
            }
            len--;
          }
        }
      }
    }
  } else {				/* take storage size */
    if (IsCLocale (LC_CTYPE)) {
      len = wcslen (wcs);
    } else {
      len = WideCharToMultiByte (__locale_codepage,
		WC_COMPOSITECHECK | WC_SEPCHARS,
		wcs, -1, NULL, 0, NULL, &stus);
      if (len == 0 || stus != 0) {
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

/* see MBYTE.C */

#endif

