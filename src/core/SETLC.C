/*_ setlc.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>
#include <limits.h>
#include <locale.h>
#include <setlocal.h>
#include <lcapi32.h>

#include "dbgint.h"

#define NWORKSZ		16
#define CTYPESZ		257
#define CTYLNGS		3
#define LNG(x,y)	MAKELANGID(x,y)

static char	_CLC[] = "C";

typedef struct {
  char *	locale;
  char *	name;
  short		nlen;
} CatTab;

static int	_geterror;		/* keep getxxxinfo error status */

static void cleargeterror (void) {
  _geterror = 0;
}

static int geterror (void) {
  return _geterror;
}

static BOOL getstrinfo (LCID lId, LCTYPE lType, char **ppData) {
 BOOL		ret = TRUE;
 int		len;
 char *		sp;
  if (_geterror == 0) {
    len = __aGetLocaleInfo (0, lId, lType, NULL, 0);
    if (len == 0) {
      goto error;
    }
//    sp = GETCMAIN (len);
    sp = (char *) _malloc_crt ((len) * sizeof (char));
    if (sp == NULL) {
      goto error;
    }
    if (__aGetLocaleInfo (0, lId, lType, sp, len) == 0) {
      goto error;
    }
    *ppData = sp;
  }
done:
  return ret;
error:
  ret = FALSE;
  _geterror = 1;
  goto done;
}

static BOOL getchrinfo (LCID lId, LCTYPE lType, char *pData) {
 BOOL		ret = TRUE;
 int		i;
 int		val;
 BYTE		ch;
 char		buf[NWORKSZ];
  if (_geterror == 0) {
    if (__aGetLocaleInfo (0, lId, lType, buf, NWORKSZ) == 0) {
      goto error;
    }
    val = 0;
    for (i = 0; i < NWORKSZ; i++) {
      if (isdigit ((ch = (BYTE) buf[i])) == FALSE) {
        break;
      }
      val = val * 10 + (ch - '0');
    }
    *pData = (char) val;
  }
done:
  return ret;
error:
  ret = FALSE;
  _geterror = 1;
  goto done;
}

static BOOL getuintinfo (LCID lId, LCTYPE lType, UINT *pData) {
 BOOL		ret = TRUE;
 int		i;
 UINT		val;
 BYTE		ch;
 char		buf[NWORKSZ];
  if (_geterror == 0) {
    if (__aGetLocaleInfo (0, lId, lType, buf, NWORKSZ) == 0) {
      goto error;
    }
    val = 0;
    for (i = 0; i < NWORKSZ; i++) {
      if (isdigit ((ch = (BYTE) buf[i])) == FALSE) {
        break;
      }
      val = val * 10 + (ch - '0');
    }
    *pData = val;
  }
done:
  return ret;
error:
  ret = FALSE;
  _geterror = 1;
  goto done;
}

static void ansigrp (char *grp) {	/* Convert MS to ANSI format */
  if (grp != NULL) {
   char *	gp;
   char *	bp;
   int		val;
    bp = gp = grp;
    val = 0;
    while (*gp != '\0') {
      if (isdigit ((BYTE) *gp)) {
        val = val * 10 + (*gp - '0');
      } else if (*gp == ';') {
        *bp = (char) val;
        bp++;
        val = 0;
      }
      gp++;
    }
    *bp = (char) val;
    bp++;
    *bp = '\0';
  }
}

static BOOL _sti_all (void) {
  return TRUE;
}

static BOOL _sti_collate (void) {
  return TRUE;
}

static BOOL _sti_ctype (void) {
 BOOL		ret = TRUE;
 WORD *		bctype = _pctype - 1;
 wctype_t *	bwtype = _pwctype - 1;
 WORD *		nctype = NULL;
 wctype_t *	nwtype = NULL;
 WORD *		wbuf = NULL;
  if (IsCLocale (LC_CTYPE)) {
    _pctype = _ctype + 1;
    _pwctype = _ctype + 1;
  } else {
   LCID		lid;
   CPINFO	info;
   int		i;
   char *	cbuf;
   BYTE *	cp;
    cleargeterror ();
    lid = __locale_ident[LC_CTYPE].lgid;
    if (__locale_codepage == 0) {
      getuintinfo (lid, LOCALE_IDEFAULTCODEPAGE, &__locale_codepage);
    }
    if (geterror ()) {
      goto error;
    }
    nctype = (WORD *) _malloc_crt (CTYPESZ * sizeof(WORD));
    nwtype = (wctype_t *) _malloc_crt (CTYPESZ * sizeof(wctype_t));
    if (nctype == NULL || nwtype == NULL) {
      goto error;
    }
    if (GetCPInfo(__locale_codepage, &info) == FALSE) {
      goto error;
    }
    __locale_mbsize = (WORD) info.MaxCharSize;
//    wbuf = GETWMAIN (CTYPESZ);
    wbuf = (wchar_t *) _malloc_crt((CTYPESZ) * sizeof (wchar_t));
    if (wbuf == NULL) {
      goto error;
    }
    cbuf = (char *) wbuf;
    for (i = 0; i < CTYPESZ - 1; i++) {
      cbuf[i] = (char) i;
    }
    if (__locale_mbsize > 1) {
      for (cp = info.LeadByte; cp[0] != 0 && cp[1] != 0; cp += 2) {
        for (i = cp[0]; i <= cp[1]; i++) {
          cbuf[i] = 0;
        }
      }
    }
    if (__aGetStringType (0, __locale_ident[LC_CTYPE].lgid,
	CT_CTYPE1, cbuf, CTYPESZ - 1, nctype + 1) == FALSE) {
      goto error;
    }
    if (__locale_mbsize > 1) {
      for (cp = info.LeadByte; cp[0] != 0 && cp[1] != 0; cp += 2) {
        for (i = cp[0]; i <= cp[1]; i++) {
          nctype[i + 1] = _LEADBYTE;
        }
      }
    }
    nctype[0] = 0;
    _pctype = nctype + 1;
    for (i = 0; i < CTYPESZ - 1; i++) {
      wbuf[i] = (wchar_t) i;
    }
    if (__wGetStringType (0, __locale_ident[LC_CTYPE].lgid,
	CT_CTYPE1, wbuf, CTYPESZ - 1, nwtype + 1) == FALSE) {
      goto error;
    }
    nwtype[0] = 0;
    _pwctype = nwtype + 1;
  }
  if (bctype != _ctype) {
    free (bctype);
  }
  if (bwtype != _ctype) {
    free (bwtype);
  }
done:
  _free_crt (wbuf);
  return ret;
error:
  free (nctype);
  free (nwtype);
  ret = FALSE;
  goto done;
}

static int takelocaleconv (struct lconv *pCv) {
 LCID		cid;
  cid = __locale_ident[LC_MONETARY].cyid;
  cleargeterror ();
  getstrinfo (cid, LOCALE_SINTLSYMBOL, &pCv->int_curr_symbol);
  getstrinfo (cid, LOCALE_SCURRENCY, &pCv->currency_symbol);
  getstrinfo (cid, LOCALE_SMONDECIMALSEP, &pCv->mon_decimal_point);
  getstrinfo (cid, LOCALE_SMONTHOUSANDSEP, &pCv->mon_thousands_sep);
  getstrinfo (cid, LOCALE_SMONGROUPING, &pCv->mon_grouping);
  getstrinfo (cid, LOCALE_SPOSITIVESIGN, &pCv->positive_sign);
  getstrinfo (cid, LOCALE_SNEGATIVESIGN, &pCv->negative_sign);
  getchrinfo (cid, LOCALE_IINTLCURRDIGITS, &pCv->int_frac_digits);
  getchrinfo (cid, LOCALE_ICURRDIGITS, &pCv->frac_digits);
  getchrinfo (cid, LOCALE_IPOSSYMPRECEDES, &pCv->p_cs_precedes);
  getchrinfo (cid, LOCALE_IPOSSEPBYSPACE, &pCv->p_sep_by_space);
  getchrinfo (cid, LOCALE_INEGSYMPRECEDES, &pCv->n_cs_precedes);
  getchrinfo (cid, LOCALE_INEGSEPBYSPACE, &pCv->n_sep_by_space);
  getchrinfo (cid, LOCALE_IPOSSIGNPOSN, &pCv->p_sign_posn);
  getchrinfo (cid, LOCALE_INEGSIGNPOSN, &pCv->n_sign_posn);
  ansigrp (pCv->mon_grouping);
  return geterror ();
}

static void freelocaleconv (struct lconv *pCv) {
  if (pCv != NULL) {
    if (pCv->int_curr_symbol != __locale_null_const) {
      _free_crt (pCv->int_curr_symbol);
    }
    if (pCv->currency_symbol != __locale_null_const) {
      _free_crt (pCv->currency_symbol);
    }
    if (pCv->mon_decimal_point != __locale_null_const) {
      _free_crt (pCv->mon_decimal_point);
    }
    if (pCv->mon_thousands_sep != __locale_null_const) {
      _free_crt (pCv->mon_thousands_sep);
    }
    if (pCv->mon_grouping != __locale_null_const) {
      _free_crt (pCv->mon_grouping);
    }
    if (pCv->positive_sign != __locale_null_const) {
      _free_crt (pCv->positive_sign);
    }
    if (pCv->negative_sign != __locale_null_const) {
      _free_crt (pCv->negative_sign);
    }
    _free_crt (pCv);			/* Release self */
  }
}

static BOOL _sti_monetary (void) {
 BOOL		ret = TRUE;
 struct lconv *	bp;
 struct lconv *	cp;
  bp = __locale_conv;
  if (IsCLocale (LC_MONETARY)) {
    __locale_conv = &__locale_conv_const;
  } else {
    cp = (struct lconv *) _malloc_crt (sizeof (struct lconv));
    if (cp == NULL) {
      goto error;
    }
    memset (cp, 0x00, sizeof (struct lconv));
    if (takelocaleconv (cp)) {
      freelocaleconv (cp);
      goto error;
    }
    __locale_conv = cp;
  }
  __locale_conv->decimal_point = bp->decimal_point;
  __locale_conv->thousands_sep = bp->thousands_sep;
  __locale_conv->grouping      = bp->grouping;
  if (bp != &__locale_conv_const) {
    freelocaleconv (bp);
  }
done:
  return ret;
error:
  ret = FALSE;
  goto done;
}

static BOOL _sti_numeric (void) {
 BOOL		ret = TRUE;
 char *		cdec;
 char *		csep;
 char *		cgrp;
 LCID		cid;
  if (IsCLocale (LC_NUMERIC)) {
    cdec = __locale_decimal_const;
    csep = __locale_null_const;
    cgrp = __locale_null_const;
  } else {
    cid = __locale_ident[LC_NUMERIC].cyid;
    cleargeterror ();
    getstrinfo (cid, LOCALE_SDECIMAL, &cdec);
    getstrinfo (cid, LOCALE_STHOUSAND, &csep);
    getstrinfo (cid, LOCALE_SGROUPING, &cgrp);
    if (geterror ()) {
      _free_crt (cdec);
      _free_crt (csep);
      _free_crt (cgrp);
      goto error;
    }
    ansigrp (cgrp);
  }
  if (__locale_conv->decimal_point != __locale_decimal_const) {
    free (__locale_conv->decimal_point);
  }
  if (__locale_conv->thousands_sep != __locale_null_const) {
    free (__locale_conv->thousands_sep);
  }
  if (__locale_conv->grouping != __locale_null_const) {
    free (__locale_conv->grouping);
  }
  __locale_conv->decimal_point = cdec;
  __locale_conv->thousands_sep = csep;
  __locale_conv->grouping = cgrp;
  __locale_decpoint = __locale_conv->decimal_point;
done:
  return ret;
error:
  ret = FALSE;
  goto done;
}

static int takelocaletime (LCTIME *pTm) {
 int		ret = 0;
 LCID		lid;
 LCID		cid;
 int		i;
 char *		sp = NULL;
 char *		fp;
 char		itime;
 char		itlzr;
 size_t		sz;
  lid = __locale_ident[LC_TIME].lgid;
  cid = __locale_ident[LC_TIME].cyid;
  cleargeterror ();
  for (i = 0; i < 7; i++) {
    getstrinfo (lid, LOCALE_SDAYNAME1 + i, &pTm->lgweek[(i + 1) % 7]);
    getstrinfo (lid, LOCALE_SABBREVDAYNAME1 + i, &pTm->shweek[(i + 1) % 7]);
  }
  for (i = 0; i < 12; i++) {
    getstrinfo (lid, LOCALE_SMONTHNAME1 + i, &pTm->lgmont[i]);
    getstrinfo (lid, LOCALE_SABBREVMONTHNAME1 + i, &pTm->shmont[i]);
  }
  getstrinfo (cid, LOCALE_SSHORTDATE, &pTm->shdate);
  getstrinfo (cid, LOCALE_SLONGDATE, &pTm->lgdate);
  for (i = 0; i < 2; i++) {
    getstrinfo (lid, LOCALE_S1159 + i, &pTm->lyampm[i]);
  }
  getchrinfo (cid, LOCALE_ITIME, &itime);
  getchrinfo (cid, LOCALE_ITLZERO, &itlzr);
  getstrinfo (cid, LOCALE_STIME, &sp);
  if (geterror ()) {
    goto error;
  }
  sz = strlen (sp);
  fp = (char *) _malloc_crt (2 + sz + 2+ sz + 2 + 1);
  if (fp == NULL) {
    goto error;
  }
  pTm->lytime = fp;			/* format "hh:mm:ss" */
  if (itime) {
    *fp++ = 'H';
    if (itlzr) {
      *fp++ = 'H';
    }
  } else {
    *fp++ = 'h';
    if (itlzr) {
      *fp++ = 'h';
    }
  }
  memcpy (fp, sp, sz);
  fp += sz;
  *fp++ = 'm';
  if (itlzr) {
    *fp++ = 'm';
  }
  memcpy (fp, sp, sz);
  fp += sz;
  *fp++ = 's';
  *fp++ = 's';
  *fp = '\0';
  ret = 0;
done:
  _free_crt (sp);
  return ret;
error:
  ret = 1;
  goto done;
}

static void freelocaletime (LCTIME *pTm) {
 int		i;
  if (pTm != NULL) {
    for (i = 0; i < 7; i++) {
      _free_crt (pTm->lgweek[i]);
      _free_crt (pTm->shweek[i]);
    }
    for (i = 0; i < 12; i++) {
      _free_crt (pTm->lgmont[i]);
      _free_crt (pTm->shmont[i]);
    }
    _free_crt (pTm->lgdate);
    _free_crt (pTm->shdate);
    for (i = 0; i < 2; i++) {
      _free_crt (pTm->lyampm[i]);
    }
    _free_crt (pTm->lytime);
    _free_crt (pTm);			/* Release self */
  }
}

static BOOL _sti_time (void) {
 int		ret = TRUE;
 LCTIME *	bp;
 LCTIME *	cp;
  bp = __locale_time;
  if (IsCLocale (LC_TIME)) {
    __locale_time = &__locale_time_const;
  } else {
    cp = (LCTIME *) _malloc_crt (sizeof (LCTIME));
    if (cp == NULL) {
      goto error;
    }
    memset (cp, 0x00, sizeof (LCTIME));
    if (takelocaletime (cp)) {
      freelocaletime (cp);
      goto error;
    }
    __locale_time = cp;
  }
  if (bp != &__locale_time_const) {
    freelocaletime (bp);
  }
done:
  return ret;
error:
  ret = FALSE;
  goto done;
}

static CatTab	_cattab[] = {
  { _CLC, "LC_ALL",       6 },
  { _CLC, "LC_COLLATE",  10 },
  { _CLC, "LC_CTYPE",     8 },
  { _CLC, "LC_MONETARY", 11 },
  { _CLC, "LC_NUMERIC",  10 },
  { _CLC, "LC_TIME",      7 }
};

typedef struct {
  WORD		code;
  char *	name;
} KeyTab;

static KeyTab _langtab[] = {
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_US), "american english"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_US), "american-english"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_AUS), "australian"},
  {LNG(LANG_DUTCH, SUBLANG_DUTCH_BELGIAN), "belgian"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_CAN), "canadian"},
  {LNG(LANG_CHINESE, SUBLANG_CHINESE_HONGKONG), "chh"},
  {LNG(LANG_CHINESE, SUBLANG_CHINESE_SINGAPORE), "chi"},
  {LNG(LANG_CHINESE, SUBLANG_DEFAULT), "chinese"},
  {LNG(LANG_CHINESE, SUBLANG_CHINESE_HONGKONG), "chinese-hongkong"},
  {LNG(LANG_CHINESE, SUBLANG_CHINESE_HONGKONG), "chinese(hong kong)"},
  {LNG(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), "chinese-simplified"},
  {LNG(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), "chinese(prc)"},
  {LNG(LANG_CHINESE, SUBLANG_CHINESE_SINGAPORE), "chinese-singapore"},
  {LNG(LANG_CHINESE, SUBLANG_CHINESE_SINGAPORE), "chinese(singapore)"},
  {LNG(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL), "chinese-traditional"},
  {LNG(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL), "chinese(taiwan)"},
  {LNG(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), "chs"},
  {LNG(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL), "cht"},
  {LNG(LANG_CZECH, SUBLANG_DEFAULT), "csy"},
  {LNG(LANG_CZECH, SUBLANG_DEFAULT), "czech"},
  {LNG(LANG_DANISH, SUBLANG_DEFAULT), "danish"},
  {LNG(LANG_GERMAN, SUBLANG_GERMAN_AUSTRIAN), "dea"},
  {LNG(LANG_GERMAN, SUBLANG_GERMAN_SWISS), "des"},
  {LNG(LANG_GERMAN, SUBLANG_GERMAN), "deu"},
  {LNG(LANG_DUTCH, SUBLANG_DUTCH), "dutch"},
  {LNG(LANG_DUTCH, SUBLANG_DUTCH_BELGIAN), "dutch-belgian"},
  {LNG(LANG_GREEK, SUBLANG_DEFAULT), "ell"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_AUS), "ena"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_CAN), "enc"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_UK), "eng"},
  {LNG(LANG_ENGLISH, SUBLANG_DEFAULT), "english"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_US), "english-american"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_AUS), "english-aus"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_CAN), "english-can"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_EIRE), "english-ire"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_NZ), "english-nz"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_UK), "english-uk"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_US), "english-usa"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_EIRE), "eni"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_US), "enu"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_NZ), "enz"},
  {LNG(LANG_SPANISH, SUBLANG_SPANISH_MEXICAN), "esm"},
  {LNG(LANG_SPANISH, SUBLANG_SPANISH_MODERN), "esn"},
  {LNG(LANG_SPANISH, SUBLANG_SPANISH), "esp"},
  {LNG(LANG_FINNISH, SUBLANG_DEFAULT), "finnish"},
  {LNG(LANG_FRENCH, SUBLANG_FRENCH), "fra"},
  {LNG(LANG_FRENCH, SUBLANG_FRENCH_BELGIAN), "frb"},
  {LNG(LANG_FRENCH, SUBLANG_FRENCH_CANADIAN), "frc"},
  {LNG(LANG_FRENCH, SUBLANG_FRENCH), "french"},
  {LNG(LANG_FRENCH, SUBLANG_FRENCH_BELGIAN), "french-belgian"},
  {LNG(LANG_FRENCH, SUBLANG_FRENCH_CANADIAN), "french-canadian"},
  {LNG(LANG_FRENCH, SUBLANG_FRENCH_SWISS), "french-swiss"},
  {LNG(LANG_FRENCH, SUBLANG_FRENCH_SWISS), "frs"},
  {LNG(LANG_GERMAN, SUBLANG_GERMAN), "german"},
  {LNG(LANG_GERMAN, SUBLANG_GERMAN_AUSTRIAN), "german-austrian"},
  {LNG(LANG_GERMAN, SUBLANG_GERMAN_SWISS), "german-swiss"},
  {LNG(LANG_GREEK, SUBLANG_DEFAULT), "greek"},
  {LNG(LANG_HUNGARIAN, SUBLANG_DEFAULT), "hungarian"},
  {LNG(LANG_ICELANDIC, SUBLANG_DEFAULT), "icelandic"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_EIRE), "irish-english"},
  {LNG(LANG_ICELANDIC, SUBLANG_DEFAULT), "isl"},
  {LNG(LANG_ITALIAN, SUBLANG_ITALIAN), "italian"},
  {LNG(LANG_ITALIAN, SUBLANG_ITALIAN_SWISS), "italian-swiss"},
  {LNG(LANG_ITALIAN, SUBLANG_ITALIAN_SWISS), "its"},
  {LNG(LANG_JAPANESE, SUBLANG_DEFAULT), "japanese"},
  {LNG(LANG_JAPANESE, SUBLANG_DEFAULT), "jpn"},
  {LNG(LANG_KOREAN, SUBLANG_DEFAULT), "korean"},
  {LNG(LANG_DUTCH, SUBLANG_DUTCH_BELGIAN), "nlb"},
  {LNG(LANG_DUTCH, SUBLANG_DUTCH), "nld"},
  {LNG(LANG_NORWEGIAN, SUBLANG_NORWEGIAN_NYNORSK), "non"},
  {LNG(LANG_NORWEGIAN, SUBLANG_NORWEGIAN_BOKMAL), "nor"},
  {LNG(LANG_NORWEGIAN, SUBLANG_DEFAULT), "norwegian"},
  {LNG(LANG_NORWEGIAN, SUBLANG_NORWEGIAN_BOKMAL), "norwegian (bokm\xE5l)"},
  {LNG(LANG_NORWEGIAN, SUBLANG_NORWEGIAN_BOKMAL), "norwegian-bokmal"},
  {LNG(LANG_NORWEGIAN, SUBLANG_NORWEGIAN_NYNORSK), "norwegian (nynorsk)"},
  {LNG(LANG_NORWEGIAN, SUBLANG_NORWEGIAN_NYNORSK), "norwegian-nynorsk"},
  {LNG(LANG_POLISH, SUBLANG_DEFAULT), "plk"},
  {LNG(LANG_POLISH, SUBLANG_DEFAULT), "polish"},
  {LNG(LANG_PORTUGUESE, SUBLANG_PORTUGUESE), "portuguese"},
  {LNG(LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN), "portuguese-brazilian"},
  {LNG(LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN), "ptb"},
  {LNG(LANG_PORTUGUESE, SUBLANG_PORTUGUESE), "ptg"},
  {LNG(LANG_RUSSIAN, SUBLANG_DEFAULT), "russian"},
  {LNG(LANG_SLOVAK, SUBLANG_DEFAULT), "sky"},
  {LNG(LANG_SLOVAK, SUBLANG_DEFAULT), "slovak"},
  {LNG(LANG_SPANISH, SUBLANG_NEUTRAL), "spanish"},
  {LNG(LANG_SPANISH, SUBLANG_SPANISH_MODERN), "spanish - modern Sort"},
  {LNG(LANG_SPANISH, SUBLANG_SPANISH), "spanish - traditional Sort"},
  {LNG(LANG_SPANISH, SUBLANG_SPANISH_MEXICAN), "spanish-mexican"},
  {LNG(LANG_SPANISH, SUBLANG_SPANISH_MODERN), "spanish-modern"},
  {LNG(LANG_SWEDISH, SUBLANG_DEFAULT), "sve"},
  {LNG(LANG_SWEDISH, SUBLANG_DEFAULT), "swedish"},
  {LNG(LANG_GERMAN, SUBLANG_GERMAN_SWISS), "swiss"},
  {LNG(LANG_TURKISH, SUBLANG_DEFAULT), "trk"},
  {LNG(LANG_TURKISH, SUBLANG_DEFAULT), "turkish"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_UK), "uk"},
  {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_US), "usa"}
};

static WORD getlangcode (char *name) {
 int	i;
 int	n = sizeof (_langtab) / sizeof (KeyTab);
 int	ilen = strlen (name);
  for (i = 0; i < n; i++) {
    if (strnicmp (name, _langtab[i].name, ilen) == 0) {
      return _langtab[i].code;
    }
  }
  for (i = n - 1; i >= 0; i--) {
    ilen = strlen (_langtab[i].name);
    if (strnicmp (name, _langtab[i].name, ilen) == 0) {
      return _langtab[i].code;
    }
  }
  return 0;
}

static KeyTab _countab[] = {
  {CTRY_UNITED_STATES, "america"},
  {CTRY_AUSTRALIA, "australia"},
  {CTRY_AUSTRIA, "austria"},
  {CTRY_AUSTRIA, "aut"},
  {CTRY_BELGIUM, "belgium"},
  {CTRY_BRAZIL, "brazil"},
  {CTRY_UNITED_KINGDOM, "britain"},
  {CTRY_BULGARIA, "bulgaria"},
  {CTRY_CANADA, "canada"},
  {CTRY_SWITZERLAND, "che"},
  {CTRY_PRCHINA, "china"},
  {CTRY_PRCHINA, "chn"},
  {CTRY_CROATIA, "croatia"},
  {CTRY_CZECH, "czech"},
  {CTRY_CZECH, "czech republic"},
  {CTRY_DENMARK, "denmark"},
  {CTRY_GERMANY, "deu"},
  {CTRY_DENMARK, "dnk"},
  {CTRY_UNITED_KINGDOM, "england"},
  {CTRY_SPAIN, "esp"},
  {CTRY_FINLAND, "finland"},
  {CTRY_FRANCE, "france"},
  {CTRY_UNITED_KINGDOM, "gbr"},
  {CTRY_GERMANY, "germany"},
  {CTRY_GREECE, "grc"},
  {CTRY_UNITED_KINGDOM, "great britain"},
  {CTRY_GREECE, "greece"},
  {CTRY_HONG_KONG, "hkg"},
  {CTRY_NETHERLANDS, "holland"},
  {CTRY_HONG_KONG, "hong kong"},
  {CTRY_HONG_KONG, "hong-kong"},
  {CTRY_HUNGARY, "hungary"},
  {CTRY_ICELAND, "iceland"},
  {CTRY_IRELAND, "ireland"},
  {CTRY_IRELAND, "irl"},
  {CTRY_ICELAND, "isl"},
  {CTRY_ITALY, "italy"},
  {CTRY_JAPAN, "japan"},
  {CTRY_JAPAN, "jpn"},
  {CTRY_SOUTH_KOREA, "korea"},
  {CTRY_MEXICO, "mexico"},
  {CTRY_NETHERLANDS, "netherlands"},
  {CTRY_NEW_ZEALAND, "new zealand"},
  {CTRY_NEW_ZEALAND, "new-zealand"},
  {CTRY_NETHERLANDS, "nld"},
  {CTRY_NORWAY, "norway"},
  {CTRY_NEW_ZEALAND, "nzl"},
  {CTRY_PRCHINA, "peoples' republic of china"},
  {CTRY_POLAND, "poland"},
  {CTRY_PORTUGAL, "portugal"},
  {CTRY_PRCHINA, "pr china"},
  {CTRY_PRCHINA, "pr-china"},
  {CTRY_PORTUGAL, "prt"},
  {CTRY_ROMANIA, "romania"},
  {CTRY_RUSSIA, "russia"},
  {CTRY_SINGAPORE, "singapore"},
  {CTRY_SINGAPORE, "sgp"},
  {CTRY_SLOVAK, "slovak"},
  {CTRY_SLOVAK, "slovak republic"},
  {CTRY_SLOVENIA, "slovenia"},
  {CTRY_SOUTH_KOREA, "south korea"},
  {CTRY_SOUTH_KOREA, "south-korea"},
  {CTRY_SPAIN, "spain"},
  {CTRY_SLOVAK, "svk"},
  {CTRY_SWEDEN, "sweden"},
  {CTRY_SWITZERLAND, "switzerland"},
  {CTRY_TAIWAN, "taiwan"},
  {CTRY_TURKEY, "turkey"},
  {CTRY_TAIWAN, "twn"},
  {CTRY_UNITED_KINGDOM, "uk"},
  {CTRY_UNITED_KINGDOM, "united kingdom"},
  {CTRY_UNITED_STATES, "united states"},
  {CTRY_UNITED_KINGDOM, "united-kingdom"},
  {CTRY_UNITED_STATES, "united-states"},
  {CTRY_UNITED_STATES, "usa"}
};

static WORD getcountrycode (char *name) {
 int	i;
 int	n = sizeof (_countab) / sizeof (KeyTab);
 int	ilen = strlen (name);
  for (i = 0; i < n; i++) {
    if (strnicmp (name, _countab[i].name, ilen) == 0) {
      return _countab[i].code;
    }
  }
  return 0;
}

typedef struct {
  short		key;
  WORD		code[CTYLNGS];
} CodeTab;

static CodeTab	_counlangs[] = {
  {  0, {0, 0, 0}},
  {  1, {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_US), 0, 0}},
  {  2, {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_CAN),
         LNG(LANG_FRENCH, SUBLANG_FRENCH_CANADIAN), 0}},
  {  7, {LNG(LANG_RUSSIAN, SUBLANG_DEFAULT), 0, 0}},
  { 30, {LNG(LANG_GREEK, SUBLANG_DEFAULT), 0, 0}},
  { 31, {LNG(LANG_DUTCH, SUBLANG_DUTCH), 0, 0}},
  { 32, {LNG(LANG_DUTCH, SUBLANG_DUTCH_BELGIAN),
         LNG(LANG_FRENCH, SUBLANG_FRENCH_BELGIAN), 0}},
  { 33, {LNG(LANG_FRENCH, SUBLANG_FRENCH), 0, 0}},
  { 34, {LNG(LANG_SPANISH, SUBLANG_SPANISH), 0, 0}},
  {351, {LNG(LANG_PORTUGUESE, SUBLANG_PORTUGUESE), 0, 0}},
  {353, {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_EIRE), 0, 0}},
  {354, {LNG(LANG_ICELANDIC, SUBLANG_DEFAULT), 0, 0}},
  {358, {LNG(LANG_FINNISH, SUBLANG_DEFAULT), 0, 0}},
  {359, {0, 0, 0}},
  { 36, {LNG(LANG_HUNGARIAN, SUBLANG_DEFAULT), 0, 0}},
  {385, {0, 0, 0}},
  {386, {0, 0, 0}},
  { 39, {LNG(LANG_ITALIAN, SUBLANG_ITALIAN), 0, 0}},
  { 41, {LNG(LANG_GERMAN, SUBLANG_GERMAN_SWISS),
         LNG(LANG_FRENCH, SUBLANG_FRENCH_SWISS),
         LNG(LANG_ITALIAN, SUBLANG_ITALIAN_SWISS)}},
  { 42, {LNG(LANG_CZECH, SUBLANG_DEFAULT),
         LNG(LANG_SLOVAK, SUBLANG_DEFAULT), 0}},
  { 43, {LNG(LANG_GERMAN, SUBLANG_GERMAN_AUSTRIAN), 0, 0}},
  { 44, {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_UK), 0, 0}},
  { 45, {LNG(LANG_DANISH, SUBLANG_DEFAULT), 0, 0}},
  { 46, {LNG(LANG_SWEDISH, SUBLANG_DEFAULT), 0, 0}},
  { 47, {LNG(LANG_NORWEGIAN, SUBLANG_DEFAULT), 0, 0}},
  { 48, {LNG(LANG_POLISH, SUBLANG_DEFAULT), 0, 0}},
  { 49, {LNG(LANG_GERMAN, SUBLANG_GERMAN), 0, 0}},
  { 51, {LNG(LANG_PORTUGUESE, SUBLANG_PORTUGUESE), 0, 0}},
  { 52, {LNG(LANG_SPANISH, SUBLANG_SPANISH_MEXICAN), 0, 0}},
  { 53, {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_EIRE), 0, 0}},
  { 54, {LNG(LANG_ICELANDIC, SUBLANG_DEFAULT), 0, 0}},
  { 55, {LNG(LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN), 0, 0}},
  { 58, {LNG(LANG_FINNISH, SUBLANG_DEFAULT), 0, 0}},
  { 61, {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_AUS), 0, 0}},
  { 64, {LNG(LANG_ENGLISH, SUBLANG_ENGLISH_NZ), 0, 0}},
  { 65, {LNG(LANG_CHINESE, SUBLANG_CHINESE_SINGAPORE), 0, 0}},
  { 81, {LNG(LANG_JAPANESE, SUBLANG_DEFAULT), 0, 0}},
  { 82, {LNG(LANG_KOREAN, SUBLANG_DEFAULT), 0, 0}},
  {852, {LNG(LANG_CHINESE, SUBLANG_CHINESE_HONGKONG), 0, 0}},
  { 86, {LNG(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), 0, 0}},
  {886, {LNG(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL), 0, 0}},
  { 90, {LNG(LANG_TURKISH, SUBLANG_DEFAULT), 0, 0}}
};

static int getcountryindex (int key) {
 int	i;
 int	n = sizeof (_counlangs) / sizeof (CodeTab);
  for (i = 0; i < n; i++) {
    if (_counlangs[i].key == key) {
      return i;
    }
  }
  return 0;
}

static size_t strpad (char *s, size_t off, char *p) {
 size_t		n;
  n = strlen (p);
  if (s != NULL) {
    memcpy (s + off, p, n + 1);
  }
  return off + n;
}

static char *scanlocale (const char *s, char *pLc, LCIDENT *pId) {
  if (stricmp (s, _CLC) == 0) {
    if (pLc == NULL) {
      pLc = _CLC;
    } else {
      strcpy (pLc, _CLC);
    }
    if (pId) {
      pId->lang = 0;
      pId->lsrt = SORT_DEFAULT;
      pId->cnty = 0;
      pId->csrt = SORT_DEFAULT;
      pId->lgid = MAKELCID(0, SORT_DEFAULT);			/* XXX */
      pId->cyid = MAKELCID(0, SORT_DEFAULT);			/* XXX */
      pId->cpge = _LOCALE_C_CP;		/* Ansi Code Page */
    }
  } else {
   LCIDENT	id;
   LCSTRING	ls;
   char		ch;
   size_t	n;
   int		i;
   int		ci;
   BOOL		oem;
   WORD		clng;
   WORD		plng;
    memset (&ls, 0x00, sizeof (LCSTRING));
    ch = '/';
    n = strcspn (s, "_.");
    if (n == 0) {
      ch = s[n];
      s++;
    }
    while (ch != '\0') {
      n = strcspn (s, "_.");
      switch (ch) {
      case '/':
        strncpy (ls.lang, s, n < LC_LGSZ ? n : LC_LGSZ - 1);
        break;
      case '_':
        strncpy (ls.cnty, s, n < LC_CYSZ ? n : LC_CYSZ - 1);
        break;
      case '.':
        strncpy (ls.cpge, s, n < LC_CPSZ ? n : LC_CPSZ - 1);
        break;
      default:
        return NULL;
      }
      ch = s[n];
      s += (n + 1);
    }
    id.lang = 0;
    id.lsrt = SORT_DEFAULT;
    if (*ls.lang != '\0') {
      id.lang = getlangcode (ls.lang);
      if (id.lang == 0) {
        return NULL;
      }
    }
    id.cnty = 0;
    id.csrt = SORT_DEFAULT;
    if (*ls.cnty != '\0') {
      id.cnty = getcountrycode (ls.cnty);
      if (id.cnty == 0) {
        return NULL;
      }
    }
    oem = FALSE;
    id.cpge = 0;
    if (*ls.cpge != '\0') {
      if (stricmp (ls.cpge, "OCP") == 0) {
        oem = TRUE;			/* Oem Code Page */
      } else if (stricmp (ls.cpge, "ACP") != 0) {
        id.cpge = atoi (ls.cpge);
        if (id.cpge == 0) {
          return NULL;
        }
      }
    }
    switch (id.lang) {
    case 0:
      switch (id.cnty) {
      case 0:
        id.lang =
        id.cnty = LANGIDFROMLCID (GetUserDefaultLCID ());
        break;
      default:
        id.lang =
        id.cnty = _counlangs[getcountryindex (id.cnty)].code[0];
        break;
      }
      break;
    default:
      switch (id.cnty) {
      case 0:
        id.cnty = id.lang;
        break;
      default:
        plng = PRIMARYLANGID (id.lang);
        ci = getcountryindex (id.cnty);
        for (i = 0; i < CTYLNGS; i++) {
          clng = _counlangs[ci].code[i];
          if (clng != 0) {
            if (PRIMARYLANGID (clng) == plng) {
              if (SUBLANGID (id.lang) == 0) {
                id.lang = clng;
              }
              id.cnty = clng;
              break;
            }
          }
        }
        if (i >= CTYLNGS) {
          id.cnty = _counlangs[ci].code[0];
          if (id.cnty == 0) {
            return NULL;
          }
        }
        break;
      }
      break;
    }
    id.lgid = MAKELCID (id.lang, id.lsrt);	/* Language Locale ID XXX */
    id.cyid = MAKELCID (id.cnty, id.csrt);	/* Country Locale ID  XXX */
    if (id.cpge == 0) {
     char	buf[NWORKSZ];
     int	cpmode;
      if (oem) {
        cpmode = LOCALE_IDEFAULTCODEPAGE;
      } else {
        cpmode = LOCALE_IDEFAULTANSICODEPAGE;
      }
      if (__aGetLocaleInfo (0, id.cyid, cpmode, buf, NWORKSZ) == 0) {
        return NULL;
      }
      id.cpge = (WORD) strtol (buf, NULL, 10);
    }
    if (IsValidCodePage (id.cpge)) {
      if (__aGetLocaleInfo (0, id.lgid,
		LOCALE_SENGLANGUAGE, ls.lang, LC_LGSZ) == 0) {
        return NULL;
      }
      if (__aGetLocaleInfo (0, id.cyid,
		LOCALE_SENGCOUNTRY, ls.cnty, LC_CYSZ) == 0) {
        return NULL;
      }
      _itoa (id.cpge, ls.cpge, 10);
    } else {
      return NULL;
    }
retry:
    n = strpad (pLc, 0, ls.lang);
    if (ls.cnty != 0) {
      n = strpad (pLc, strpad (pLc, n, "_"), ls.cnty);
    }
    if (ls.cpge != 0) {
      n = strpad (pLc, strpad (pLc, n, "."), ls.cpge);
    }
    if (pLc == NULL) {
//      pLc = GETCMAIN (n + 1);
      pLc = (char *) _malloc_crt ((n+1) * sizeof (char));
      if (pLc != NULL) {
        goto retry;
      }
    }
    if (pId) {
      *pId = id;
    }
  }
  return pLc;
}

static char *getalllocale (void) {
 BOOL		equ;
 size_t		off;
 size_t		sz;
 int		i;
 char *		cp;
  equ = TRUE;
  for (i = LC_ALL + 1; i < LC_MAX; i++) {
    if (strcmp (_cattab[i].locale, _cattab[i + 1].locale) != 0) {
      equ = FALSE;
    }
  }
  if (equ == FALSE) {
    sz = 0;
    for (i = LC_ALL + 1; i < LC_MAX + 1; i++) {
      sz += _cattab[i].nlen + 1 + strlen (_cattab[i].locale) + 1;
    }
  } else {
    sz = strlen (_cattab[LC_COLLATE].locale);
  }
  sz++;
//  cp = GETCMAIN (sz);
  cp = (char *) _malloc_crt ((sz) * sizeof (char));
  if (cp != NULL) {
    if (equ == FALSE) {
      off = 0;
      for (i = LC_ALL + 1; i < LC_MAX + 1; i++) {
        off = strpad (cp, off, _cattab[i].name);
        off = strpad (cp, off, "=");
        off = strpad (cp, off, _cattab[i].locale);
        if (i < LC_MAX) {
          off = strpad (cp, off, ";");
        }
      }
    } else {
      if (_cattab[LC_COLLATE].locale != _CLC) {
        memcpy (cp, _cattab[LC_COLLATE].locale, sz);
      } else {
        _free_crt (cp);
        cp = _CLC;
      }
    }
    if (_cattab[LC_ALL].locale != _CLC) {
      _free_crt (_cattab[LC_ALL].locale);
    }
    _cattab[LC_ALL].locale = cp;
  }
  return cp;
}

static char * setpartlocale (int category, const char *locale) {
 UINT		bcp;
 LCIDENT	bid, nid;
 char		nlo[LC_LCSZ];
 char *		lp = NULL;
 size_t		sz;
 BOOL		ret;
  lp = scanlocale (locale, NULL, &nid);
  if (lp != NULL) {
    bid = __locale_ident[category];
    bcp = __locale_codepage;
    __locale_ident[category] = nid;
    if (category == LC_CTYPE) {
      __locale_codepage = nid.cpge;
    }
    switch (category) {
    case LC_ALL:
      ret = _sti_all();
      break;
    case LC_COLLATE:
      ret = _sti_collate();
      break;
    case LC_CTYPE:
      ret = _sti_ctype();
      break;
    case LC_MONETARY:
      ret = _sti_monetary();
      break;
    case LC_NUMERIC:
      ret = _sti_numeric();
      break;
    case LC_TIME:
      ret = _sti_time();
      break;
    }
    if (ret) {
      if (_cattab[category].locale != _CLC) {
        _free_crt (_cattab[category].locale);
      }
      _cattab[category].locale = lp;
    } else {
      __locale_ident[category] = bid;
      __locale_codepage = bcp;
    }
  }
  return lp;
}

char * __CLIB setlocale (int category, const char *locale) {
 char *		ret = NULL;
 char		buf[LC_LCSZ];
 char *		bp;
 const char *	lp;
 int		i;
 size_t		lsz;
  if (category >= LC_MIN && category <= LC_MAX) {
    if (category == LC_ALL) {
      if (locale == NULL) {		/* LC_ALL,NULL */
        ret = getalllocale ();
      } else {				/* LC_ALL,"XXX" */
        lp = locale;
        if (strnicmp (lp, "LC_", 3) == 0) {
          do {
            bp = strchr (lp, '=');
            if(bp == NULL) {
              return NULL;
            }
            bp++;			/* skip '=' */
            lsz = strcspn (bp, ";");
            if(lsz == 0) {
              return NULL;
            }
            for (i = LC_ALL + 1; i <= LC_MAX; i++) {
              if (strnicmp (lp, _cattab[i].name, _cattab[i].nlen) == 0) {
                strncpy (buf, bp, lsz);
                buf[lsz] = '\0';
                if (setpartlocale (i, buf) == NULL) {
                  return NULL;
                }
                break;
              }
            }
            if (i > LC_MAX) {
              return NULL;		/* Not found keyword */
            }
            lp = bp + lsz;
            if (*lp == ';') {
              lp++;			/* skip ';' */
            }
          } while (*lp != '\0');
          ret = getalllocale();
        } else {			/* LC_ALL,"Lang_Ctry.Page" */
          ret = scanlocale (locale, buf, NULL);
          if (ret != NULL) {
            for (i = LC_ALL + 1; i <= LC_MAX; i++) {
              if (strcmp (buf, _cattab[i].locale) != 0) {
                if (setpartlocale (i, buf) == NULL) {
                  return NULL;
                }
              }
            }
            ret = getalllocale();
          }
        }
      }
    } else {
      if (locale == NULL) {		/* LC_XXX,NULL */
        ret = _cattab[category].locale;
      } else {				/* LC_XXX,"Lang_Ctry.Page" */
        ret = setpartlocale (category, locale);
      }
    }
  }
  return ret;
}

#else

/*_ setlc.c   Thu Sep 28 1989   Modified by: Walter Bright */

#include	"locale.h"
#include	<limits.h>
#include	<string.h>

extern struct lconv __current_lconv;
extern int __charmax;  /* used for checking if user compiled with -J */


/* Some strings to reduce redundancy	*/
static char NA[] = "";		/* not available			*/
static char DOT[] = ".";

/* __charmax_locales is a list of locales that are initialized whose char
   values are initialized to CHAR_MAX. This table is used in the STI_setlc
   to fix up those values to UCHAR_MAX if the user has compiled with -J.
   Any additional locales added to locale_data that are to be initalized
  to CHAR_MAX, should also be added to this array so that they will
  work properly with -J.
*/
#if 1
static char __charmax_locales[] = {
   0,	/* C      */
   6,	/* UK     */
   7,	/* Japan  */
   8,	/* Korea  */
   9,	/* China  */
  10	/* Taiwan */
};
#else
static char __charmax_locales[][7] = {
  "C",
  "UK",
  "Japan",
  "Korea",
  "China",
  "Taiwan"
};
#endif

static struct
{   char *locale;		/* locale string			*/
    struct lconv locdata;	/* values for this locale		*/
} locale_data[_LOCALE_MAX] =
{
    {	"C",			/* 0 */
	{ DOT,NA,NA,NA,NA,NA,NA,NA,NA,NA,
	  CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,
	  { _LOCALE_C,_LOCALE_C,_LOCALE_C,
	    _LOCALE_C,_LOCALE_C,_LOCALE_C
	  }
	}
    },
    {	"USA",			/* 1 */
	{ DOT, ",","\3", "USD ", "$", DOT, ",","\3" ,"+", "-",
	  2,2,1,0,1,0,0,0,
	  { _LOCALE_NATIVE,_LOCALE_NATIVE,_LOCALE_NATIVE,
	    _LOCALE_NATIVE,_LOCALE_NATIVE,_LOCALE_NATIVE
	  }
	}
    },
    {	"Italy",		/* 2 */
	{ DOT,",","\3","ITL.","L.",NA,DOT,"\3",NA,"-",
	  0,0,1,0,1,0,1,1,
	  { _LOCALE_ITALY,_LOCALE_ITALY,_LOCALE_ITALY,
	    _LOCALE_ITALY,_LOCALE_ITALY,_LOCALE_ITALY
	  }
	}
    },
    {	"Netherlands",		/* 3 */
	{ DOT,",","\3","NLG ","F",",",DOT,"\3",NA,"-",
	  2,2,1,1,1,1,1,4,
	  { _LOCALE_NETHERLANDS,_LOCALE_NETHERLANDS,_LOCALE_NETHERLANDS,
	    _LOCALE_NETHERLANDS,_LOCALE_NETHERLANDS,_LOCALE_NETHERLANDS
	  }
	}
    },
    {	"Norway",		/* 4 */
	{ DOT,",","\3","NOK ","kr",",",DOT,"\3",NA,"-",
	  2,2,1,0,1,0,1,2,
	  { _LOCALE_NORWAY,_LOCALE_NORWAY,_LOCALE_NORWAY,
	    _LOCALE_NORWAY,_LOCALE_NORWAY,_LOCALE_NORWAY
	  }
	}
    },
    {	"Switzerland",		/* 5 */
	{ DOT,",","\3","CHF ","SFrs.",DOT,",","\3",NA,"C",
	  2,2,1,0,1,0,1,2,
	  { _LOCALE_SWITZERLAND,_LOCALE_SWITZERLAND,_LOCALE_SWITZERLAND,
	    _LOCALE_SWITZERLAND,_LOCALE_SWITZERLAND,_LOCALE_SWITZERLAND
	  }
	}
    },
    {	"UK",			/* 6 */
	{ DOT,NA,NA,"GBP ",NA,NA,NA,NA,NA,NA,
	  CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,
	  { _LOCALE_UK,_LOCALE_UK,_LOCALE_UK,
	    _LOCALE_UK,_LOCALE_UK,_LOCALE_UK
	  }
	}
    },
    {	"Japan",		/* 7 */
	{ DOT, ",","\3", "JPY ","\\", DOT, ",","\3", NA, "-",
	  CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,
	  { _LOCALE_JAPAN,_LOCALE_JAPAN,_LOCALE_JAPAN,
	    _LOCALE_JAPAN,_LOCALE_JAPAN,_LOCALE_JAPAN
	  }
	}
    },
    {	"Korea",		/* 8 */
	{ DOT,  NA,  NA, "WON ", "\\",DOT,  NA,  NA, NA, NA,
	  CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,
	  { _LOCALE_KOREA,_LOCALE_KOREA,_LOCALE_KOREA,
	    _LOCALE_KOREA,_LOCALE_KOREA,_LOCALE_KOREA
	  }
	}
    },
    {	"China",		/* 9 */
	{ DOT,NA,NA,NA,NA,NA,NA,NA,NA,NA,
	  CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,
	  { _LOCALE_CHINA,_LOCALE_CHINA,_LOCALE_CHINA,
	    _LOCALE_CHINA,_LOCALE_CHINA,_LOCALE_CHINA
	  }
	}
    },
    {	"Taiwan",		/* 10 */
	{ DOT,NA,NA,NA,NA,NA,NA,NA,NA,NA,
	  CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,
	  { _LOCALE_CHINA,_LOCALE_CHINA,_LOCALE_CHINA,
	    _LOCALE_CHINA,_LOCALE_CHINA,_LOCALE_CHINA
	  }
	}
    }
};

#ifdef _MBCS
#include <mbctype.h>
typedef struct {
 char *		cp; /* code page         */
 short		ix; /* locale_data index */
} CPCONV;
static CPCONV	_cpcnv[] = {
 { ".932",  7 },    /* Japan          */
 { ".936",  8 },    /* Korea          */
 { ".949",  9 },    /* China          */
 { ".950", 10 },    /* Taiwan         */
 { NULL,    0 }     /* Stopper        */
};
static short	_cpnum[] = {
  _MB_CP_SBCS,      /*  0: C           */
  _MB_CP_SBCS,      /*  1: USA         */
  _MB_CP_SBCS,      /*  2: Italy       */
  _MB_CP_SBCS,      /*  3: Netherlands */
  _MB_CP_SBCS,      /*  4: Norway      */
  _MB_CP_SBCS,      /*  5: Switzerland */
  _MB_CP_SBCS,      /*  6: UK          */
  932,              /*  7: Japan       */
  936,              /*  8: Korea       */
  949,              /*  9: China       */
  950               /* 10: China       */
};
#endif

/**********************************
 */

char *setlocale(int category, const char *locale)
{   enum _LOCALE n;
    struct lconv near *pconv;

    if (locale)
    {
	if (*locale == 0)		/* if native environment	*/
	    n = _LOCALE_NATIVE;
	else
	{
#ifdef _MBCS
	    for (n = 0; _cpcnv[n].cp != NULL; n++)
	    {   if (strcmp (locale, _cpcnv[n].cp) == 0) {
		    break;
		}
	    }
	    if (_cpcnv[n].cp != NULL) {
		n = _cpcnv[n].ix;
	    }
	    else
#endif
	    for (n = 0; 1; n++)
	    {   if (n == _LOCALE_MAX)	/* if locale is not found	*/
		    return 0;		/* selection cannot be honored	*/
		if (strcmpl(locale,locale_data[n].locale) == 0)
		    break;		/* found it			*/
	    }
	}
#ifdef _MBCS
	switch (category)
	{   case LC_ALL:
	    case LC_CTYPE:
		_setmbcp (_cpnum[n]);
		break;
	}
#endif
	pconv = &(locale_data[n].locdata);
	__current_lconv.lc[category] = n;
	switch (category)
	{   case LC_ALL:
		__current_lconv = *pconv;
		break;
#if 0
	    case LC_COLLATE:
	    case LC_CTYPE:
	    case LC_TIME:
		break;
#endif
	    case LC_MONETARY:
		memcpy(&__current_lconv.int_curr_symbol,
			&(pconv->int_curr_symbol),
			sizeof(struct lconv) - 2 * sizeof(char *) -
			sizeof(__current_lconv.lc));
		break;
	    case LC_NUMERIC:
		memcpy(&__current_lconv,pconv,2 * sizeof(char *));
		break;
	}
    }
    else
	n = __current_lconv.lc[category];
    return locale_data[n].locale;
}

void _STI_setlc()
{
   int i,j;

   /* if __charmax is 0, then user compiled with -J, fixup locale_data table */
   if (!__charmax)
   {

      for(i = 0; i < sizeof(__charmax_locales) / sizeof(__charmax_locales[0]); i++)
      {
#if 1
          j = __charmax_locales[i];
#else
          /* search local_data_table */
          for (j = 0; j < _LOCALE_MAX; j++)
	       {
		      if (strcmpl(__charmax_locales[i],locale_data[j].locale) == 0)
            {
#endif
               /* found it, fix up structure and break loop */
               locale_data[j].locdata.int_frac_digits = UCHAR_MAX;
               locale_data[j].locdata.frac_digits = UCHAR_MAX;
               locale_data[j].locdata.p_cs_precedes = UCHAR_MAX;
	            locale_data[j].locdata.p_sep_by_space = UCHAR_MAX;
	            locale_data[j].locdata.n_cs_precedes  = UCHAR_MAX;
	            locale_data[j].locdata.n_sep_by_space = UCHAR_MAX;
	            locale_data[j].locdata.p_sign_posn   =  UCHAR_MAX;
	            locale_data[j].locdata.n_sign_posn   = UCHAR_MAX;
#if 0
               break;
            }
          }
#endif
      }
   }
}

#endif

/**/
