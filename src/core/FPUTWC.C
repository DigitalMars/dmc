/*_ fputwc.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <scrtl.h>
#include <io.h>
#if MSDOS || __OS2__ || _WINDOWS || __NT__
#include <dos.h>
#endif
#include <scio.h>
#include "mt.h"

wint_t __cdecl _flushwbu (wint_t c, FILE *fp);

wint_t __cdecl _fputwc_nlock (wint_t wch, FILE *fp) {
 wint_t		ret;
#if 1
  if ((fp->_flag & _IOTRAN) != 0 ||
	(__fhnd_info[_fileno(fp)] & FHND_TEXT) != 0 ) {	/* Translate mode */
#else
  if ((fp->_flag & _IOTRAN) != 0) {			/* Translate mode */
#endif
   int		size;
   char		mbc[3];
    size = wctomb (mbc, wch);
    if (size < 1) {
      __set_errno (EILSEQ);
      ret = WEOF;
    } else {
      if (_fputc_nlock (mbc[0], fp) == EOF) {
        ret = WEOF;
      } else {
        if (size == 2) {
          if (_fputc_nlock (mbc[1], fp) == EOF) {
            ret = WEOF;
          } else {
            ret = (wint_t) wch;
          }
        } else {
          ret = (wint_t) wch;
        }
      }
    }
  } else {						/* Binary mode */
    if( fp->_cnt < sizeof (wchar_t)) {
      ret = _flushwbu (wch, fp);
    } else {
      fp->_cnt -= sizeof (wchar_t);
      ret = (wint_t) (*((wchar_t *) (fp->_ptr))++ = (wchar_t) wch);
    }
  }
  return ret;
}

wint_t __CLIB fputwc (wint_t wch, FILE *fp) {
 wint_t		ret;
  __fp_lock(fp);
  ret = _fputwc_nlock (wch, fp);
  __fp_unlock (fp);
  return ret;
}

#undef putwc

wint_t __CLIB putwc (wint_t ch, FILE *fp) {
  return fputwc (ch, fp);
}

wint_t __CLIB fputwchar (wint_t ch) {
  return putwc (ch, stdout);
}

wint_t __CLIB _fputwchar (wint_t ch) {
  return putwc (ch, stdout);
}

#endif

/**/
