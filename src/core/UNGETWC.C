/*_ ungetwc.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <dos.h>
#include <errno.h>
#include "mt.h"
#include <scio.h>
#include <scrtl.h>

wint_t __CLIB ungetwc (wint_t wch, FILE *fp) {
  __fp_lock (fp);
  if (fp->_flag & (_IOWRT | _IOEOF | _IOERR) ||
	wch == WEOF || fp->_base == NULL) {
    wch = WEOF;
  } else {
   int		off;
   int		size;
   union {
    char	bch[4];
    wchar_t	wch;
   }		dch;
  if ((fp->_flag & _IOTRAN) != 0 ||
	(__fhnd_info[_fileno(fp)] & FHND_TEXT) != 0 ) {	/* Translate mode */
      size = wctomb (dch.bch, wch);
    } else {
      dch.wch = wch;
      size = 2;
    }
    if (size < 1) {
      __set_errno (EILSEQ);
      wch = WEOF;
    } else {
      off = fp->_ptr - fp->_base;
      if (off < size) {
        fp->_ptr = fp->_base;
        fp->_cnt = size;
      } else {
        fp->_ptr -= size;
        fp->_cnt += size;
      }
      fp->_ptr[0] = dch.bch[0];
      if (size == 2) {
        fp->_ptr[1] = dch.bch[1];
      }
    }
  }
  __fp_unlock (fp);
  return wch;
}

#endif

/**/
