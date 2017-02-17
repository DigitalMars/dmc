/* scanf.c
 * Copyright (C) 1985-2003 by Digital Mars, www.digitalmars.com
 * All Rights Reserved
 * Written by Walter Bright
 */

#ifndef _UNICODE

#include <tchar.h>
#include <scio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sctchar.h>

/**********************
 */

int fscanf(FILE * fp, const char *format,...)
{
    return _sfmt((fp_t) ungetc, (fp2_t) fgetc, fp, format, (va_list) & (format) + sizeof(format));
}

int vfscanf(FILE * restrict fp, const char * restrict format, va_list args)
{
    return _sfmt((fp_t) ungetc, (fp2_t) fgetc, fp, format, args);
}

int scanf(const char *format,...)
{
    return _sfmt((fp_t) ungetc, (fp2_t) fgetc, stdin, format, (va_list) & (format) + sizeof(format));
}

int vscanf(const char * restrict format, va_list args)
{
    return _sfmt((fp_t) ungetc, (fp2_t) fgetc, stdin, format, args);
}

/**************************
 * Input from a string.
 * Need a couple functions to make the string look like fgetc, ungetc.
 */

struct Sscanf
{
    const char *s;
    char c;		// the ungetc character
};

static int sgetc(void *ps)
{
    struct Sscanf *psc = (struct Sscanf *)ps;
    if (psc->c)
    {
	char c = psc->c;
	psc->c = 0;
	return c;
    }
    return *psc->s ? *psc->s++ : EOF;
}

static int sungetc(int c, void *ps)
{
    if (c != EOF)
    {
	struct Sscanf *psc = (struct Sscanf *)ps;
	psc->c = c;
    }
    return c;
}

int sscanf(const char *s, const char *format,...)
{
    struct Sscanf sc;
    sc.s = s;
    sc.c = 0;
    return _sfmt((fp_t) sungetc, (fp2_t) sgetc, &sc, format, (va_list) & (format) + sizeof(format));
}

int vsscanf(const char * restrict s, const char * restrict format, va_list args)
{
    struct Sscanf sc;
    sc.s = s;
    sc.c = 0;
    return _sfmt((fp_t) sungetc, (fp2_t) sgetc, &sc, format, args);
}

#endif

/*************************
 * Input formatting routine.
 * Input:
 *      func -> function where we get the input from. -1 if no more input.
 *      uget -> unget function
 * Returns number of assigned input items, which can be 0.
 * Returns EOF if end of input before first conflict or conversion.
 */

/* This must match up with ecvt.c       */

#define FLneg   1
#define FLstar  2
#define FLh     4
#define FLl     8
#define FLL     0x10
#define FLhh	0x40

#define MAXINT  32767

#define SIGN    {switch (x) {   case _T('-'): ss.flag |= FLneg; \
				case _T('+'): x = GETCHAR; ss.width--; }}

#define GETCHAR         (ss.nread++, (*ss.func)(fp))
# ifdef _MBCS
#define UNGET(c)        (ss.nread--, (*ss.ufunc)(c,fp))
# else
#define UNGET(c)        (ss.nread--, (*uget)(c,fp))
# endif

#define assign          (!(ss.flag & FLstar))
#define LONGLONG        (__INTSIZE == 4)    // support long long's

#if LONGLONG
#define FLll            0x20
#define llong           long long
#else
#define llong           long
#endif

/****************************
 * Variables to save on parameter passing.
 * Collect all state variables into a struct.
 */

struct scanf_state
{
    int nread;                  // number of characters read from input
    int nass;                   // number of assigned input items
    int width;                  // field width
    int flag;                   // mask of FLxxxx
    int x;                      // last character read
    _TCHAR *p;                  // argument pointer
    const _TCHAR *format;       // pointer to format string
    void *fp;                   // argument to function pointers
#ifndef _UNICODE
    int (*func) (void *);       // where to get characters
# ifdef _MBCS
    int (*ufunc) (int, void *); // where to unget characters
# endif
#else
    wint_t (*func) (void *);    // where to get characters
#endif
};

#define x ss.x
#define p ss.p
#define format ss.format
#define fp ss.fp

#ifndef _UNICODE
#define TPSCANFLOAT	__pscanfloat
int (__pascal * __pscanfloat) (struct scanf_state __ss *);
#define TFATERR		__faterr
void __cdecl __faterr(char *);
#else
#define TPSCANFLOAT	__wpscanfloat
int (__pascal * __wpscanfloat) (struct scanf_state __ss *);
#define TFATERR		__faterr
void __cdecl __faterr(char *);
#endif
static int __near __pascal _scanre(struct scanf_state __ss * pss);
static void __near __pascal _mbstowcs (struct scanf_state __ss *pss);
static int __near __pascal _mbtowc (struct scanf_state __ss *pss, int *go);
static void __near __pascal _wcstombs (struct scanf_state __ss *pss);
static int __near __pascal _wctomb (struct scanf_state __ss *pss, int *go);

int
#ifndef _UNICODE
_sfmt
#else
_wsfmt
#endif
(fp_t uget, fp2_t fgetc, void *fparg, const _TCHAR *fmt, va_list pargs)
{
    int i;
    int base;
    llong val;
    unsigned llong ul;
    int gotone;
    struct scanf_state ss;
#ifndef _UNICODE
    unsigned char c;            /* last char read from format string, Remember
                                 * treat it as unsigned for special chars.   */
#else
    _TCHAR c;                   /* last char read from format string, Remember
                                 * treat it as unsigned for special chars.   */
#endif
    int firstdigit;

    fp = fparg;
    format = fmt;               /* copy to a global                     */
    ss.func = fgetc;
# ifdef _MBCS
    ss.ufunc = uget;
# endif
    ss.nass = 0;
    ss.nread = 0;               /* # of chars read from func()          */
    x = _T(' ');                /* 32 */
    while ((c = *format++) != 0)/* while not end of format string       */
    {
        if (x != _TEOF)
        {
            x = GETCHAR;        /* get next char of input               */
    loop:
            if (x == _TEOF)
                goto eof;
            if (c == _T(' ') || c == _T('\t') || c == _T('\n'))
            {
                do
                    c = *format++;
                while (c == _T(' ') || c == _T('\t') || c == _T('\n'));

                while (_istspace(x))
                    x = GETCHAR;
                if (c == 0)
                    goto err;
                goto loop;
            }
        }
        if (c == _T('%'))
        {
            ss.flag = 0;
            c = *format++;
            if (c == _T('*'))       /* assignment suppression       */
            {
                ss.flag |= FLstar;
                c = *format++;
            }

            if (_istdigit(c))
            {
                ss.width = 0;
                while (_istdigit(c))
                {
                    ss.width = ss.width * 10 + c - _T('0');
                    c = *format++;
                }
                /* Treat width of 0 as an error. */
                if (ss.width == 0)
                    goto err;
            }
            else
                ss.width = MAXINT;

            switch (c)
            {
                case _T('h'):
                    ss.flag |= FLh;
                    c = *format++;
                    if (c == _T('h'))
                    {
                        ss.flag = (ss.flag & ~FLh) | FLhh;
                        c = *format++;
                    }
                    break;
                case _T('l'):
                    ss.flag |= FLl;
                    c = *format++;
#if LONGLONG
                    if (c == _T('l'))
                    {
                        ss.flag = (ss.flag & ~FLl) | FLll;
                        c = *format++;
                    }
#endif
                    break;
                case _T('L'):
                    ss.flag |= FLL;
		    c = *format++;
                    break;
                case _T('j'):		// intmax_t or uintmax_t
#if LONGLONG
                    ss.flag |= FLll;
#else
                    ss.flag |= FLl;
#endif
                case _T('z'):		// size_t
                case _T('t'):		// ptrdiff_t
		    c = *format++;
                    break;

            }
            if (c != _T('%') && assign)
                p = va_arg(pargs, _TCHAR *);
#ifdef __NT__
            if (c != _T('[') && c != _T('c') && c != _T('C') && c != _T('n'))
#else
            if (c != _T('[') && c != _T('c') && c != _T('n'))
#endif
            {
                while (_istspace(x))
                    x = GETCHAR;
            }

            switch (c)
            {
                case _T('i'):
                    SIGN;
                    if (x != _T('0'))
                        goto decimal;
                    x = GETCHAR;
                    if (x == _T('b') || x == _T('B'))
                    {
                        base = 2;
                        goto L2;
                    }
                    if (x == _T('x') || x == _T('X'))
                    {
                        base = 16;
                L2:     x = GETCHAR;
                        goto integer;
                    }
                    if (x <= _T('0') || x >= _T('9'))
                    {
                        UNGET(x); /* restore non numeric character  */
                        x = _T('0');/* treat single '0' as octal zero */
                    }
                    goto octal; /* else octal           */

                case _T('b'):
                    SIGN;
                    base = 2;
                    goto integer;

                case _T('o'):
                    SIGN;
            octal:
                    base = 8;
                    goto integer;

                case _T('d'):
                case _T('u'):
                    SIGN;
            decimal:
                    base = 10;
                    goto integer;

                case _T('x'):
                case _T('p'):
                case _T('X'):
                    SIGN;
                    base = 16;
                    goto integer;

#ifdef __NT__
                case _T('S'):
# ifndef _UNICODE
                    ss.flag = (ss.flag & ~FLh) | FLl;
# else
                    ss.flag = (ss.flag & ~FLl) | FLh;
# endif
#endif
                case _T('s'):
                    while (_istspace(x))
                        x = GETCHAR;
                    if (x == _TEOF)
                        goto eof;
                    if (assign)
                        ss.nass++;
#ifdef __NT__
                    if (assign) {
# ifndef _UNICODE
                        if (ss.flag & FLl) {
                            _mbstowcs (&ss);
                        } else {
                            while (ss.width-- && x != _T(' ') && x != _T('\n') && x != _T('\t')) {
                                *p++ = (_TCHAR) x;
                                x = GETCHAR;
                                if (x == _TEOF) {
                                    break;
                                }
                            }
                            *p = '\0';	/* terminate the string */
                        }
# else
                        if (ss.flag & FLh) {
                            _wcstombs (&ss);
                        } else {
                            while (ss.width-- && x != _T(' ') && x != _T('\n') && x != _T('\t')) {
                                *p++ = (_TCHAR) x;
                                x = GETCHAR;
                                if (x == _TEOF) {
                                    break;
                                }
                            }
                            *p = L'\0';	/* terminate the string */
                        }
# endif
                    } else {		/* scan only */
                        while (ss.width-- && x != _T(' ') && x != _T('\n') && x != _T('\t')) {
                            x = GETCHAR;
                            if (x == _TEOF) {
                                break;
                            }
                        }
                    }
#else
                    while (ss.width-- && x != _T(' ') && x != _T('\n') && x != _T('\t'))
                    {
                        if (assign)
                            *p++ = x;
                        x = GETCHAR;
                        if (x == _TEOF)
                            break;
                    }
                    if (assign)
                        *p = 0; /* terminate the string */
#endif
                    goto L1;

#ifdef __NT__
                case _T('C'):
# ifndef _UNICODE
                    ss.flag = (ss.flag & ~FLh) | FLl;
# else
                    ss.flag = (ss.flag & ~FLl) | FLh;
# endif
#endif
                case _T('c'):
                    if (ss.width == MAXINT)
                        ss.width = 1; /* read just 1 char     */
                    gotone = 0;
#ifdef __NT__
                    if (assign) {
# ifndef _UNICODE
                        if (ss.flag & FLl) {
                            if (_mbtowc (&ss, &gotone) == FALSE) goto err;
                        } else {
                            while (ss.width--) {
                                if (x == _TEOF) {
                                    goto done;
                                }
                                gotone = 1;
                                *p++ = x;
                                /* only get character if you need one */
                                if (ss.width != 0)
                                    x = GETCHAR;
                            }
                        }
# else
                        if (ss.flag & FLh) {
                            if (_wctomb (&ss, &gotone) == FALSE) goto err;
                        } else {
                            while (ss.width--) {
                                if (x == _TEOF) {
                                    goto done;
                                }
                                gotone = 1;
                                *p++ = x;
                                /* only get character if you need one */
                                if (ss.width != 0)
                                    x = GETCHAR;
                            }
                        }
# endif
                    } else {		/* scan only */
                        while (ss.width--) {
                            if (x == _TEOF) {
                                goto done;
                            }
                            gotone = 1;
                            /* only get character if you need one */
                            if (ss.width != 0)
                                x = GETCHAR;
                        }
                    }
#else
                    while (ss.width--)
                    {
                        if (x == _TEOF)
                            goto done;
                        gotone = 1;
                        if (assign)
                            *p++ = x;

                        /* only get character if you need one */
                        if (ss.width != 0)
                            x = GETCHAR;

                    }
#endif
                    if (gotone)
                    {
                        if (assign)
                            ss.nass++;
                    }
                    else if (x == _TEOF)
                        goto eof;
                    continue;
            L1:
                    UNGET(x);
                    continue;

                case _T('a'):
                case _T('A'):
                case _T('e'):
                case _T('E'):
                case _T('f'):
                case _T('g'):
                case _T('G'):
                    if (!TPSCANFLOAT)
                        TFATERR ("Floating point not loaded");
                case _T('['):
                    if ((c == _T('[')) ? _scanre(&ss) : (*TPSCANFLOAT) (&ss))
                        goto L1;
                    else if (x == _TEOF)
                        goto eof;
                    else
                        goto err;

                case _T('n'):
                    UNGET(x);
                    if (x == _TEOF)
                        ss.nread++;
                    if (assign)
                        *((int *) p) = ss.nread;
                    continue;

                case _T('%'):
                    goto match;
                case 0:
                    goto done;
                default:
                    goto err;
            }
    integer:
            val = 0;
            gotone = 0;         /* don't have one just yet      */
            firstdigit = 1;
            while (ss.width--)
            {
                if (!_istxdigit(x))
                {
                    if (firstdigit)
                    {
                        if (x == _TEOF)
                            goto eof;
                        else
                            goto err;
                    }
                    else
                        break;
                }
                firstdigit = 0;
                i = (x >= _T('a')) ? x - (_T('a') - 10) :
                    (x >= _T('A')) ? x - (_T('A') - 10) : x - _T('0');
                if (i < 0 || i >= base)
                    break;
                else
                {
                    gotone = 1; /* got at least one digit */
                    val = val * base - i;
                }
                x = GETCHAR;
            }                   /* while */
            UNGET(x);
            if (gotone)
            {
                if (assign)     /* if not suppressed    */
                {
                    if (!(ss.flag & FLneg))
                        val = -val;
                    if (ss.flag & FLhh)
                        *((char *) p) = val;
                    else if (ss.flag & FLh)
                        *((short *) p) = val;
                    else if (ss.flag & FLl)
                        *((long *) p) = val;
#if LONGLONG
                    else if (ss.flag & FLll)
                        *((long long *) p) = val;
#endif
                    else
                        *((int *) p) = val;
                    ss.nass++;
                }
            }
            else if (x == _TEOF)
                goto eof;
        }
        else
        {
    match:                      /* c must match input           */
            if (x != c)
                goto err;
#ifdef _MBCS
            if (isleadbyte ((unsigned char) x)) {
                if (*format == 0) {
                    goto err;
                }
                c = x;               /* save 1st char */
                x = GETCHAR;         /* get next char of input */
                if (x == _TEOF) {
                    goto eof;
                }
                if (x != *format) {
                    UNGET (x);       /* push back 2nd char */
                    x = c;           /* restore 1st char */
                    goto err;
                }
                format++;
            }
#endif
        }
    }                           /* while */

done:                           /* end of format                        */
    return ss.nass;

err:
    (*uget) (x, fp);            /* push back offending input char       */
    goto done;

eof:                            /* end of file found on input           */
    return (ss.nass) ? ss.nass : _TEOF;
}


#define ss (*pss)
#ifdef __NT__
typedef unsigned long	_CHRTAB;
#else
typedef unsigned short	_CHRTAB;
#endif
typedef unsigned short  _MBCH;
typedef unsigned char   _SBCH;
#define _CTSZ		(sizeof (_CHRTAB) * 8)
#define gettab(i) (chartab[(i) / _CTSZ + 1] &   (1 << ((i) & (_CTSZ - 1))))
#define settab(i)  chartab[(i) / _CTSZ + 1] |=  (1 << ((i) & (_CTSZ - 1)))
#define clrtab(i)  chartab[(i) / _CTSZ + 1] &= ~(1 << ((i) & (_CTSZ - 1)))

/************************
 * Read in regular expression.
 * This is in a separate routine because of the large stack usage.
 * Input:
 *      x = first char of string
 * Returns:
 *      x = char after the string, or EOF
 *      *p = string
 *      non-zero: successful conversion
 *      0: failed conversion
 */

static int near __pascal _scanre(struct scanf_state __ss * pss)
{
    int j;
    int anymatch;
    int dash_first;
#ifdef _MBCS
    _CHRTAB chartab[(256 / _CTSZ) * 256 + 1];  /* +1 for EOF           */
    _MBCH   c;             /* last char read from format string    */
    _MBCH   prev;          /* character previous to c */
    _MBCH   min_range = 0;
    _MBCH   max_range = 0; /* to support inclusive scanning */
    _MBCH   cmax;
    _SBCH   c2;
    _SBCH   x2;
    int     xch;
#else
# ifdef __NT__
#  ifndef _UNICODE
    _CHRTAB chartab[256 / _CTSZ + 1];          /* +1 for EOF           */
#  else
    _CHRTAB chartab[(256 / _CTSZ) * 256 + 1];  /* +1 for EOF           */
#  endif
# else
    char    chartab[257];                      /* +1 for EOF           */
# endif
    _TCHAR  c;                   /* last char read from format string    */
    _TCHAR  prev;                /* character previous to c */
    _TCHAR  min_range = 0;
    _TCHAR  max_range = 0;       /* to support inclusive scanning */
    _TCHAR  cmax;
#endif

    //printf("scanre()\n");
#if __ZTC__
    /* Can't use _chkstack() if this routine is used in a DLL       */
    /* _chkstack(); *//* chartab[] uses a lot of stack */
#endif
    prev = c = *format++;
#ifdef _MBCS
    if (c != EOF) {
        prev = (c &= 0xFF);
    }
    if (isleadbyte ((unsigned char) c)) {
        c2 = *format++;
        prev = c = (c << 8) | c2;
    }
    if (c == _T('^')) {
        prev = c = *format++;
        if (c != EOF) {
            prev = (c &= 0xFF);
        }
        if (isleadbyte ((unsigned char) c)) {
            c2 = *format++;
            prev = c = (c << 8) | c2;
        }
        j = 0;
    } else {
        j = 1;
    }
#else
    j = (c == _T('^')) ? ((prev = c = *format++), 0) : 1; /* invert everything */
#endif
    dash_first = (c == _T('-')) ? 1 : 0;
    chartab[EOF + 1] = 0;       /* no match for EOF     */

    //printf("j = %d, c = '%c'\n", j, c);
#ifdef _MBCS
    memset (chartab + 1, j == 0 ? -1 : 0, 256 / _CTSZ * 256 * sizeof(_CHRTAB));
#else
# ifdef __NT__
#  ifndef _UNICODE
    memset (chartab + 1, j == 0 ? -1 : 0, 256 / _CTSZ * sizeof(_CHRTAB));
#  else
    memset (chartab + 1, j == 0 ? -1 : 0, 256 / _CTSZ * 256 * sizeof(_CHRTAB));
#  endif
# else
    memset(chartab + 1, j ^ 1, 256);
# endif
#endif
    do
    {

        if (c == _T('-') && !dash_first)
        {
            min_range = prev; /* beginning of range is previous */
            max_range = c = *format++; /* next char is max of range */
#ifdef _MBCS
            if (c != EOF) {
                max_range = (c &= 0xFF);
            }
            if (isleadbyte ((unsigned char) c)) {
                c2 = *format++;
                max_range = c = (c << 8) | c2;
            }
#endif
            if (c == _T(']'))
                break;          /* user did not supply a closing range, so
                                 * break loop! */
        }

        prev = c;

#ifdef _MBCS
        if (j == 0) {
            clrtab (c);
        } else {
            settab (c);
        }
#else
# ifdef __NT__
        if (j == 0) {
            clrtab (c);
        } else {
            settab (c);
        }
# else
        chartab[(unsigned char) c + 1] = j; /* DJB */
# endif
#endif

        /* use min and max macros just in case user reversed order. */
        cmax = max (min_range, max_range);
        for (c = min (min_range, max_range); c <= cmax; c++)
#ifdef _MBCS
            if (j == 0) {
                clrtab (c);
            } else {
                settab (c);
            }
#else
# ifdef __NT__
            if (j == 0) {
                clrtab (c);
            } else {
                settab (c);
            }
# else
            chartab[(unsigned char) c + 1] = j;
# endif
#endif

        min_range = max_range = c = *format++;
#ifdef _MBCS
        if (c != EOF) {
            min_range = max_range = (c &= 0xFF);
        }
        if (isleadbyte ((unsigned char) c)) {
            c2 = *format++;
            min_range = max_range = c = (c << 8) | c2;
        }
#endif
        dash_first = 0;         /* Ok, read past first, cannot be dash */
    } while (c != _T(']'));
    anymatch = 0;

#ifdef _MBCS
    xch = x;
    x2 = 0;
    if (isleadbyte ((unsigned char) x)) {
        x2 = GETCHAR;           /* get next char of input */
        xch = (xch << 8) | x2;
    }
    //printf("x = '%c', ss.width = %d, gettab = %x\n", x, ss.width, gettab(xch));
    while (ss.width-- && xch != EOF && gettab (xch))
#else
# ifdef __NT__
    while (ss.width-- && x != EOF && gettab (x))
# else
    while (ss.width-- && chartab[x + 1])
# endif
#endif
    {
	//printf("xch = '%c' x%0x, ss.width = %d, gettab = %x\n", xch, xch, ss.width, gettab(xch));
        if (assign)
        {
            *p = x;
#ifdef _MBCS
            if (x2 != 0) {
                *(p + 1) = x2;
                p++;
            }
#endif
            *(p + 1) = 0;
            p++;
        }
        anymatch = 1;
        x = GETCHAR;
#ifdef _MBCS
        xch = x;
        x2 = 0;
        if (isleadbyte ((unsigned char) x)) {
            x2 = GETCHAR;       /* get next char of input */
            xch = (xch << 8) | x2;
        }
#endif
    }
#ifdef _MBCS
    if (x2 != 0) {
        UNGET (x2);
    }
#endif
    if (assign)
        ss.nass += anymatch;
    return anymatch;            /* 1 if we matched any chars    */
}

#ifdef __NT__

#define iSBCS		0
#define iMBCS		1
#define jMBCS		2

# ifndef _UNICODE

static void __near __pascal _mbstowcs (struct scanf_state __ss *pss)
{
  wchar_t *	wp;
  char		mbc[4];
    wp = (wchar_t *) p;
    while (x != _T(' ') && x != _T('\n') && x != _T('\t')) {
        if(ss.width-- == 0) {
            break;
        }
        mbc[0] = x;
        if (isleadbyte ((unsigned char) x)) {
            x = GETCHAR;
            if (x == EOF) {
                break;
            }
            mbc[1] = x;
            mbc[2] = '\0';
        } else {
            mbc[1] = '\0';
        }
        if (mbtowc (wp, mbc, MB_CUR_MAX) < 1) goto error;
        wp++;
        x = GETCHAR;
        if (x == EOF) {
            break;
        }
    }
error:
    *wp = L'\0';			/* terminate the string */
}

static int __near __pascal _mbtowc (struct scanf_state __ss *pss, int *go)
{
  wchar_t *	wp;
  char		mbc[4];
  int		ret;
    wp = (wchar_t *) p;
    while (ss.width--) {
        if (x == EOF) {
            break;
        }
        mbc[0] = x;
        if (isleadbyte ((unsigned char) x)) {
            x = GETCHAR;
            if (x == EOF) {
                break;
            }
            mbc[1] = x;
            mbc[2] = '\0';
        } else {
            mbc[1] = '\0';
        }
        if (mbtowc (wp, mbc, MB_CUR_MAX) < 1) {
            ret = FALSE;	/* Error at convert */
            goto error;
        }
        wp++;
        *go = 1;
        /* only get character if you need one */
        if (ss.width != 0) {
            x = GETCHAR;
        }
    }
    ret = TRUE;
error:
    return ret;
}

#else

static void __near __pascal _wcstombs (struct scanf_state __ss *pss)
{
  int		len;
  char		mbc[4];
  char *	cp;
    cp = (char *) p;
    while (x != _T(' ') && x != _T('\n') && x != _T('\t')) {
        if (ss.width-- == 0) {
            break;
        }
        len = wctomb (mbc, x);
        if (len < 1) goto error;
        if (len == 2) {
            if (ss.width-- == 0) {
                break;
            }
        }
        *cp = mbc[0];
        cp++;
        if (len == 2) {
            *cp = mbc[1];
            cp++;
        }
        x = GETCHAR;
        if (x == WEOF) {
            break;
        }
    }
error:
    *cp = '\0';				/* terminate the string */
}

static int __near __pascal _wctomb (struct scanf_state __ss *pss, int *go)
{
  int		len;
  char		mbc[4];
  char *	cp;
  int		ret;
    cp = (char *) p;
    while (ss.width--) {
        if (x == WEOF) {
            break;
        }
        len = wctomb (mbc, x);
        if (len < 1) {
            ret = FALSE;	/* Error at convert */
            goto error;
        }
        if (len == 2) {
            ret = FALSE;	/* Error at 1[unicode] -> 2[char] */
            goto error;
        }
        *cp = mbc[0];
        cp++;
        *go = 1;
        /* only get character if you need one */
        if (ss.width != 0) {
            x = GETCHAR;
        }
    }
    ret = TRUE;
error:
    return ret;
}

#endif

#endif

