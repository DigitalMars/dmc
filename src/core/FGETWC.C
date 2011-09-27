/*_ fgetwc.c						*/
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

int __cdecl _fillbuf (FILE *fp);
void _flushterm (void);

static int __cdecl _1fillbuf (FILE *fp) {
  if (fp->_flag & _IORW) {
    fp->_flag = (fp->_flag & ~_IOWRT) | _IOREAD;
  }
  if ((fp->_flag & (_IOREAD | _IOERR | _IOEOF)) != _IOREAD) {
    goto L2;
  }
  if (fp->_base == NULL) {
   int mode;
    mode = fp->_flag & (_IOFBF | _IOLBF | _IONBF);
    if (mode == 0) {
      mode = _IOFBF;
    }
    if (mode & _IONBF) {
L3:
      setvbuf (fp, NULL, _IONBF, sizeof (wchar_t));
    } else {
      if (isatty (fp->_file)) {
        mode = _IOLBF;
      }
    }
    if (setvbuf (fp, NULL, mode, BUFSIZ) != 0) {
      goto L3;
    }
  }
  if (fp->_flag & (_IOLBF | _IONBF)) {
    _flushterm();
  }
  fp->_ptr = fp->_base;
  fp->_cnt = read (fileno(fp), fp->_ptr, sizeof (char));
  if (fp->_cnt <= 0) {
    if (fp->_cnt == 0) {
      fp->_flag |= _IOEOF;
      if (fp->_flag & _IORW) {
        fp->_flag &= ~_IOERR;
      }
    } else {
      fp->_flag |= _IOERR;
    }
L2:
    fp->_cnt = 0;
    return EOF;
  }
  return 0;
}

wint_t __cdecl _fgetwc_nlock (FILE *fp) {
 wint_t		ret;
 int		ch;
 union {
  char		bch[2];
  wchar_t	wch;
 }		dch;
#if 1
  if ((fp->_flag & _IOTRAN) != 0 ||
	(__fhnd_info[_fileno(fp)] & FHND_TEXT) != 0 ) {	/* Translate mode */
#else
  if ((fp->_flag & _IOTRAN) != 0) {			/* Translate mode */
#endif
    ch = _fgetc_nlock (fp);
    if (ch == EOF) {
      ret = WEOF;
    } else {
      dch.bch[0] = (char) ch;
      if (isleadbyte ((unsigned char) dch.bch[0])) {
        ch = _fgetc_nlock (fp);
        if (ch == EOF) {
          ungetc (dch.bch[0], fp);
          ret = WEOF;
        } else {
          dch.bch[1] = (char) ch;
          if (mbtowc (&ret, dch.bch, MB_CUR_MAX) < 1) {
            __set_errno (EILSEQ);
            ret = WEOF;
          }
        }
      } else {
        if (mbtowc (&ret, dch.bch, MB_CUR_MAX) < 1) {
          __set_errno (EILSEQ);
          ret = WEOF;
        }
      }
    }
  } else {						/* Binary mode */
    if (fp->_cnt < sizeof (wchar_t)) {
      if (fp->_cnt > 0 || (fp->_flag & _IONBF)) {
        ch = _fgetc_nlock (fp);
        if (ch == EOF) {
          ret = WEOF;
        } else {
          dch.bch[0] = (char) ch;
          if (_1fillbuf (fp) == EOF) {
            ungetc (dch.bch[0], fp);
            ret = WEOF;
          } else {
            ch = _fgetc_nlock (fp);
            if (ch == EOF) {
              ungetc (dch.bch[0], fp);
              ret = WEOF;
            } else {
              dch.bch[1] = (char) ch;
              ret = dch.wch;
            }
          }
        }
      } else {
        if (_fillbuf (fp) == EOF) {
          ret = WEOF;
        } else {
          ret = *((wchar_t *) (fp->_ptr))++;
          fp->_cnt -= sizeof (wchar_t);
        }
      }
    } else {
      ret = *((wchar_t *) (fp->_ptr))++;
      fp->_cnt -= sizeof (wchar_t);
    }
  }
  return ret;
}

wint_t __CLIB fgetwc (FILE *fp) {
 wint_t		ret;
  __fp_lock(fp);
  ret = _fgetwc_nlock (fp);
  __fp_unlock (fp);
  return ret;
}

#undef getwc

wint_t __CLIB getwc (FILE *fp) {
  return fgetwc (fp);
}

wint_t __CLIB fgetwchar (void) {
  return getwc (stdin);
}

wint_t __CLIB _fgetwchar (void) {
  return getwc (stdin);
}

#endif

/**/
