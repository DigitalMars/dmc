//_ printf.c
// Copyright (C) 1985-2001 by Digital Mars
// All Rights Reserved
// www.digitalmars.com
// Written by Walter Bright

#ifndef _UNICODE

#include <tchar.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <scio.h>
#include <dos.h>
#include <sctchar.h>


#if __SMALL__ || __MEDIUM__
#if __INTSIZE == 4
#if M_UNIX || M_XENIX || __NT__ || __OS2__ || DOS386
#define ALWAYS_NEAR_DATA 1
#define FAR
#define FSTRLEN _tcslen
#else
#define DFT_NEAR_DATA 1
#define FAR __far
#define FSTRLEN _inline_fstrlen
size_t __CLIB __far _inline_fstrlen(const char __far *);
#endif
#else
#define DFT_NEAR_DATA 1
#define FAR __far
#define FSTRLEN _fstrlen
#endif
#else
#define DFT_FAR_DATA 1
#define FAR __far
#define FSTRLEN _tcslen
#endif

/***********************
 */

int fprintf(FILE *fp,const char *format,...)
{
	return vfprintf(fp,format,(va_list)&(format) + sizeof(format));
}

/***********************
 */

int vfprintf(FILE *fp,const char *format,va_list args)
{   int status;

    __fp_lock(fp);
    status = _pformat((fp_t)_fputc_nlock,fp,format,args);
    __fp_unlock(fp);
    return status;
}


/*************************
 * Return number of chars written in array, excluding trailing 0.
 */

static int __cdecl _sprintf(int c,void *s)
{
	*(*(char **)s)++ = c;
	return c & 0xFF;
}

int sprintf(char *s,const char *format,...)
{
	return vsprintf(s,format,(va_list)&(format) + sizeof(format));
}

int vsprintf(char *s,const char *format,va_list args)
{	int i;
	char *sstart = s;

	if ((i = _pformat((fp_t)_sprintf,&s,format,args)) < 0)
		i = 0;		/* an error occurred		*/
	sstart[i] = 0;
	return i;
}

/***********************
 * Same as vsprintf(), but have a max count.
 */

struct VSN { char *s; size_t count; };

static int __cdecl __snprintf(int c,void *pvsn)
{
    if (((struct VSN *)pvsn)->count)
    {	((struct VSN *)pvsn)->count--;
	*(((struct VSN *)pvsn)->s)++ = c;
	return c & 0xFF;
    }
    return EOF;
}

int __CLIB _snprintf(char *s,size_t count,const char *format,...)
{
	return vsnprintf(s,count,format,(va_list)&(format) + sizeof(format));
}

int __CLIB vsnprintf(char *s,size_t count,const char *format,va_list args)
{	int i;
	struct VSN vsn;

	vsn.s = s;
	vsn.count = count;
	if ((i = _pformat((fp_t)__snprintf,&vsn,format,args)) < 0)
	    i = 0;		/* an error occurred			*/
	if (!vsn.count) {	/* if no space for terminating 0	*/
#if 0
	    if (count)
		s[count - 1] = 0;
	    i = EOF;
#else				/* like strncpy */
            if (count == 0 || i != count) {
                i = EOF;
            }
#endif
	}
	else
	    s[i] = 0;
	return i;
}

__declspec(naked) int __CLIB _vsnprintf(char *s,size_t count,const char *format,va_list args)
{
    _asm
    {
#if __SMALL__ || __COMPACT__
	jmp vsnprintf
#else
	jmp far ptr vsnprintf
#endif
    }
}

#endif

/* Flags for flag word (keep in sync with ecvt.c)	*/

#define LONGLONG    (__INTSIZE == 4)    // support long long's
#define FLleft		1		// '-' flag character
#define	FLplus		2		// '+' flag character
#define	FLspc		4		// ' ' flag character
#define FLhash		8		// '#' flag character (alternate form)
#define	FLlong		0x10		// 'l' length modifier
#define FLlngdbl	0x20		// 'L' length modifier
#define FL0pad		0x40		// '0' flag character
#define FLprec		0x80		// .precision
#define FLuc		0x100		// uppercase 'X' format
#define FLuns		0x200
#define FLlptr		0x400		// far pointer format (seg:offset)
#define FLshort		0x800		// 'h' length modifier
#define FLhh		0x1000		// 'hh' length modifier
#define FLnear		0x2000		// 'N' length modifier
#define FLfar		0x4000		// 'F' length modifier
#define FLwide		0x8000		// for printf()
#define FLchar		0x8000		// for wprintf()

#if LONGLONG
#define FLlonglong  0x10000		// 'll' length modifier
typedef long long llong;
typedef unsigned long long ullong;
#else
typedef long llong;
typedef unsigned long ullong;
#endif

#if _WIN32
#define BUFMAX	48	/* temporary buffer size for _pformat()		*/
#else
#define BUFMAX	32	/* temporary buffer size for _pformat()		*/
#endif

#ifdef __NT__
static wchar_t *	__WNULLCONST = L"(null)";
#endif
static char *		__CNULLCONST = "(null)";

#ifndef _UNICODE
#define TCHARMASK	0xFF
#define TPFLOATFMT	__pfloatfmt
char __near *(*__pfloatfmt)(int c,int flags,int precision,double *pdval,char __ss *buf,int __ss *psl,int width);
#define FATERR		__faterr
void __cdecl __faterr(char *);
#else
#define TCHARMASK	0xFFFF
#define TPFLOATFMT	__wpfloatfmt
wchar_t __near *(*__wpfloatfmt)(int c,int flags,int precision,double *pdval,wchar_t __ss *buf,int __ss *psl,int width);
#define FATERR		__faterr
void __cdecl __faterr(char *);
#endif

/***************************
 * Get an int or a long out of the varargs, and return it.
 */

static llong __near __pascal getlong(va_list __SS *ppargs,int flags)
{	llong l;

#if LONGLONG
    if (flags & FLlonglong)
        return va_arg(*ppargs,llong);
#endif

#if __INTSIZE == 4	/* sizeof(int) == sizeof(long) */
	if (flags & FLuns)
		l = (llong)va_arg(*ppargs,unsigned long);
	else
		l = (llong)va_arg(*ppargs,long);
	if (flags & FLshort)
	{	l = (short) l;
		if (flags & FLuns)
		    l &= 0xFFFFL;
	}
	else
#else
	if (flags & FLlong)
		l = va_arg(*ppargs,long);
	else
	{	l = va_arg(*ppargs,int);
		if (flags & FLuns)
			l &= 0xFFFFL;
	}
#endif
	if (flags & FLhh)
	{	l = (signed char) l;
		if (flags & FLuns)
		    l &= 0xFFL;
	}
	return l;
}

/***************************
 * Get a near or far pointer out of the varargs, and return it.
 */

static FAR * __near __pascal getpointer(va_list __SS *ppargs,int flags)
{
	void FAR *p;
	
#if DFT_NEAR_DATA
	if (flags & FLfar)
	    p = va_arg(*ppargs, void FAR *);
	else
	    p = (void FAR *) va_arg(*ppargs, void *);
#elif DFT_FAR_DATA
	if (flags & FLnear)
	    p = (void *) va_arg(*ppargs, void __near *);
	else
	    p = va_arg(*ppargs, void *);
#elif ALWAYS_NEAR_DATA
	p = va_arg(*ppargs,void *);
#endif
	return p;
}

/***********************
 * Add sign to prefix.
 */

#ifndef _UNICODE
char __near * __pascal __dosign (int sign, int flags) {
#define TDOSIGN		__dosign
#else
wchar_t __near * __pascal __wdosign (int sign, int flags) {
#define TDOSIGN		__wdosign
#endif
  return	(sign)		 ? (_TCHAR __near *) _T("-") :
		(flags & FLplus) ? (_TCHAR __near *) _T("+") :
		(flags & FLspc)	 ? (_TCHAR __near *) _T(" ") :
				   (_TCHAR __near *) _T("");
}

/************************
 * strlen that finds min of strlen or maxlen.
 */

static int __near __pascal strlen2(const char *s, int maxlen)
{
    int len = 0;

    while (len < maxlen && s[len])
	len++;
    return len;
}

static int __near __pascal wcslen2(const wchar_t *s, int maxlen)
{
    int len = 0;

    while (len < maxlen && s[len])
	len++;
    return len;
}


#if 0

/************************************
 * strlen for SBCS or MBCS version.
 */

#define iSBCS		0
#define iMBCS		1
#define jMBCS		2

static size_t __near __pascal _mbssize (char *s /*, size_t n*/)
{
  size_t	len;
  size_t	i;
  int		mode;
  char		ch;
    mode = iSBCS;
    len = 0;
    for (i = 0; (ch = s[i]) != '\0'; i++) {
#if 0
	if (len >= n) {
	    break;
	}
#endif
	if (mode != iMBCS) {
	    if (isleadbyte ((unsigned char) ch)) {
		mode = iMBCS;
	    } else {
		len++;
		mode = iSBCS;
	    }
	} else {
	    len++;
	    mode = jMBCS;
	}
    }
    return len;
}

static size_t __near __pascal _strnsize (char *s, size_t n)
{
  size_t	len;
  size_t	i;
  int		mode;
  char		ch;
    mode = iSBCS;
    len = 0;
    for (i = 0; (ch = s[i]) != '\0'; i++) {
	if (len >= n) {
	    break;
	}
	if (mode != iMBCS) {
	    if (isleadbyte ((unsigned char) ch)) {
		mode = iMBCS;
	    } else {
		len++;
		mode = iSBCS;
	    }
	} else {
	    len++;
	    mode = jMBCS;
	}
    }
    return i;
}

#endif

/**********************************
 * Print formatting program.
 * Input:
 *	func	pointer to function to send the chars to. Returns char
 *		if successful, -1 if not.
 * Returns:
 *	# of chars sent to func else -1 if error
 */

int
#ifndef _UNICODE
_pformat
#else
_wpformat
#endif
	(fp_t func, void *fp, const _TCHAR *format, va_list pargs)
{	int nout;		/* # of chars sent to func()		*/
	int i,c;
	llong l;
	ullong ul;
#if __INTSIZE == 4
	unsigned long uls;
#else
#define	uls	0
#endif
	int pl;			/* length of prefix string		*/
	int width;		/* field width				*/
	int padding;		/* # of chars to pad (on left or right)	*/
	int precision;
	_TCHAR buf[BUFMAX];
	_TCHAR __ss *fltbuf;
	int fltbufmax;
	int sign;
	_TCHAR __near *prefix;
	_TCHAR *p;
	_TCHAR FAR *s;
#ifdef __NT__
# ifndef _UNICODE
	wchar_t *ws;
	wchar_t wbuf[2];
# else
	char *cs;
	char cbuf[2];
# endif
#endif
	int sl;			/* length of formatted string s		*/
	int flags;		/* FLxxx				*/
	int base;		/* number base (decimal, octal, etc.)	*/

	fltbuf = buf;
	fltbufmax = BUFMAX;

	nout = 0;
	while ((c = *format++) != 0)	/* while not end of format string */
	{	if (c != _T('%'))	/* not a format control character */
		{
#ifdef _MBCS
			if (isleadbyte ((unsigned char) c)) {
				if (*format == 0)
					break;
				if ((*func)(c & TCHARMASK,fp) == _TEOF)
					goto err;
				nout++;
				c = *format++;
			}
#endif
			if ((*func)(c & TCHARMASK,fp) == _TEOF)
				goto err;
			nout++;
			continue;
		}

		prefix = (_TCHAR __near *)_T(""); /* assume no prefix	*/
		flags = 0;			/* reset		*/
		while (1)
		{	c = *format++;
			switch (c)
			{
			    case _T('-'):
				flags |= FLleft; /* left-justify	*/
				break;
			    case _T('+'):
				flags |= FLplus; /* do + or - sign	*/
				break;
			    case _T(' '):
				flags |= FLspc;	/* space flag		*/
				break;
			    case _T('#'):
				flags |= FLhash; /* alternate form	*/
				break;
			    case _T('0'):
				flags |= FL0pad; /* pad with 0s		*/
				break;
			    case _T('*'):
				width = va_arg(pargs,int);
				if (width < 0)	/* negative field width	*/
				{   flags |= FLleft;
				    width = -width;
				}
				c = *format++;
				goto getprecision;
			    case 0:		/* end of format string	*/
				goto err;
			    default:
				goto getwidth;
			}
		}

	    getwidth:
		width = 0;
		while (_istdigit(c))
		{	width = width * 10 + c - _T('0');
			c = *format++;
		}

	    getprecision:
		precision = 0;
		if (c == _T('.'))	/* if precision follows		*/
		{	flags |= FLprec;
			c = *format++;
			if (c == _T('*'))
			{	precision = va_arg(pargs,int);
				if (precision < 0)
				{	flags &= ~FLprec;
					precision = 0;
				}
				c = *format++;
			}
			else
			{
				while (_istdigit(c))
				{	precision = precision * 10 + c - _T('0');
					c = *format++;
				}
			}
		}

		while (1)
		{
			switch (c)
			{   case _T('l'):
				flags |= FLlong;
				c = *format++;
#if LONGLONG
				if (c == _T('l'))
				{
				    flags = (flags & ~FLlong) | FLlonglong;
				    c = *format++;
				}
#endif
				break;
			    case _T('h'):
				flags |= FLshort;
				c = *format++;
				if (c == _T('h'))
				{
				    flags = (flags & ~FLshort) | FLhh;
				    c = *format++;
				}
				break;
			    case _T('L'):
				flags |= FLlngdbl;
				c = *format++;
				break;
			    case _T('N'):
			        flags |= FLnear;
			        c = *format++;
			        break;
			    case _T('F'):
				flags |= FLfar;
				c = *format++;
				break;

			    case _T('j'):	// intmax_t or uintmax_t
#if LONGLONG
				flags |= FLlonglong;
#else
				flags |= FLlong;
#endif
				c = *format++;
				break;
			    case _T('z'):	// size_t
			    case _T('t'):	// ptrdiff_t
				c = *format++;
				break;

			    default:
			        goto gettype;
			}
		}
	   gettype:
#if __INTSIZE == 4
		uls = 0;
#endif
		switch (c)
		{
		    case _T('s'):
#ifdef __NT__
# ifndef _UNICODE
			if (flags & FLlong) {
			    ws = (wchar_t *) getpointer (&pargs,flags);
			    if (ws == NULL)
				ws = __WNULLCONST;
			    if (flags & FLprec)
				sl = wcslen2(ws, precision);
			    else
				sl = wcslen(ws);
			} else {
			    flags |= FLshort;
			    s = (char *) getpointer (&pargs,flags);
			    if (s == NULL)
				s = __CNULLCONST;
			    if (flags & FLprec)
				sl = strlen2(s, precision);
			    else
				sl = strlen(s);	/* _mbssize (s) */
			}
			if (flags & FLlong) {
			    flags |= FLwide;
			}
# else
			if (flags & FLshort) {
			    cs = (char *) getpointer (&pargs,flags);
			    if (cs == NULL)
				cs = __CNULLCONST;
			    if (flags & FLprec)
				sl = strlen2(cs, precision);
			    else
				sl = strlen (cs);	/* _mbssize (cs) */
			} else {
			    flags |= FLlong;
			    s = (wchar_t *) getpointer (&pargs,flags);
			    if (s == NULL)
				s = __WNULLCONST;
			    if (flags & FLprec)
				sl = wcslen2(s, precision);
			    else
				sl = wcslen(s);
			}
			if(flags & FLshort) {
			    flags |= FLchar;
			}
# endif
#else
		        s = (_TCHAR FAR *)getpointer(&pargs,flags);
# ifdef DFT_NEAR_DATA
			if ((void *)FP_OFF (s) == NULL)
# else
			if (s == NULL)
# endif
			    s = _T("(null)");
			sl = FSTRLEN (s);
			if (flags & FLprec)	/* if there is a precision */
			{	if (precision < sl)
					sl = precision;
					if (sl < 0)
						sl = 0;
			}
#endif
			break;

#ifdef __NT__
		    case _T('S'):
# ifndef _UNICODE
			if (flags & FLshort) {
			    s = (char *) getpointer (&pargs,flags);
			    if (s == NULL)
				s = __CNULLCONST;
			    if (flags & FLprec)
				sl = strlen2(s, precision);
			    else
				sl = strlen (s);	/* _mbssize (s) */
			} else {
			    flags |= FLlong;
			    ws = (wchar_t *) getpointer (&pargs,flags);
			    if (ws == NULL)
				ws = __WNULLCONST;
			    if (flags & FLprec)
				sl = wcslen2(ws, precision);
			    else
				sl = wcslen(ws);
			}
			if(flags & FLlong) {
			    flags |= FLwide;
			}
# else
			if (flags & FLlong) {
			    s = (wchar_t *) getpointer (&pargs,flags);
			    if (s == NULL)
				s = __WNULLCONST;
			    if (flags & FLprec)
				sl = wcslen2(s, precision);
			    else
				sl = wcslen(s);
			} else {
			    flags |= FLshort;
			    cs = (char *) getpointer (&pargs,flags);
			    if (cs == NULL)
				cs = __CNULLCONST;
			    if (flags & FLprec)
				sl = strlen2(cs, precision);
			    else
				sl = strlen (cs);	/* _mbssize (cs) */
			}
			if(flags & FLshort) {
			    flags |= FLchar;
			}
# endif
			break;
#endif

		    case _T('%'):
			buf[0] = _T('%');
		    L1:
			s = &buf[0];
			sl = 1;
			break;

		    case _T('c'):
#ifdef __NT__
# ifndef _UNICODE
			if (flags & FLlong) {
			    wbuf[0] = va_arg (pargs, int);
			    ws = wbuf;
			} else if (flags & FLshort) {
			    buf[0] = va_arg (pargs, int);
			    s = buf;
			} else {
			    flags |= FLshort;
			    buf[0] = va_arg (pargs, int);
			    s = buf;
			}
			if(flags & FLlong) {
			    flags |= FLwide;
			}
# else
			if (flags & FLlong) {
			    buf[0] = va_arg (pargs, int);
			    s = buf;
			} else if (flags & FLshort) {
			    cbuf[0] = va_arg (pargs, int);
			    cs = cbuf;
			} else {
			    flags |= FLlong;
			    buf[0] = va_arg (pargs, int);
			    s = buf;
			}
			if(flags & FLshort) {
			    flags |= FLchar;
			}
# endif
			sl = 1;
			break;

		    case _T('C'):
# ifndef _UNICODE
			if (flags & FLlong) {
			    wbuf[0] = va_arg (pargs, int);
			    ws = wbuf;
			} else if (flags & FLshort) {
			    buf[0] = va_arg (pargs, int);
			    s = buf;
			} else {
			    flags |= FLlong;
			    wbuf[0] = va_arg (pargs, int);
			    ws = wbuf;
			}
			if(flags & FLlong) {
			    flags |= FLwide;
			}
# else
			if (flags & FLlong) {
			    buf[0] = va_arg (pargs, int);
			    s = buf;
			} else if (flags & FLshort) {
			    cbuf[0] = va_arg (pargs, int);
			    cs = cbuf;
			} else {
			    flags |= FLshort;
			    cbuf[0] = va_arg (pargs, int);
			    cs = cbuf;
			}
			if(flags & FLshort) {
			    flags |= FLchar;
			}
# endif
			sl = 1;
			break;
#else
			buf[0] = va_arg(pargs,int);
			goto L1;
#endif

		    case _T('d'):
		    case _T('i'):
			base = 10;
			ul = l = getlong(&pargs,flags);
			if (l < 0)
			{	sign = 1;
				ul = -l;
			}
			else
				sign = 0;
			prefix = TDOSIGN(sign,flags);
			goto ulfmt;

		    case _T('b'):
			base = 2;
			goto getuns;

		    case _T('o'):
			base = 8;
			if (flags & FLhash)
			    prefix = (_TCHAR __near *) _T("0");
			goto getuns;

		    case _T('u'):
			base = 10;
		    getuns:
			ul = getlong(&pargs,flags | FLuns);
			goto ulfmt;

		    case _T('p'):
#if DFT_NEAR_DATA
			if (flags & (FLlong | FLfar))
			   flags |= (FLlong | FLlptr);
#elif DFT_FAR_DATA
			if (!(flags & (FLshort | FLnear)))
			   flags |= (FLlong | FLlptr);
#endif

#if __INTSIZE == 4
 			flags &= ~FLshort;
#endif
			if (!(flags & FLprec))
			{   precision = sizeof(int) * 2;
			    if (flags & FLlptr)
				precision += 4 + 1;	/* seg:offset	*/
			}
			flags |= FL0pad;
		    case _T('X'):
			flags |= FLuc;
		    case _T('x'):
			base = 16;
			ul = getlong(&pargs,flags | FLuns);
#if __INTSIZE == 4
			if (flags & FLlptr)		/* if far pointer */
			    /* Pull 16 bit segment off of arguments	*/
			    uls = getlong(&pargs,flags) & 0xFFFF;
#endif
			if ((flags & FLhash) && (ul | uls))
			    prefix = (_TCHAR __near *)
				((flags & FLuc) ? _T("0X") : _T("0x"));
			goto ulfmt;

		    ulfmt:
			{
			_TCHAR __ss *sbuf;

			if (flags & FLprec && c != _T('p'))
				flags &= ~FL0pad;
			sbuf = &buf[BUFMAX - 1];
			if (ul | uls)
			{   do
			    {	_TCHAR c;

				if (flags & FLlptr && sbuf == &buf[BUFMAX-1-(sizeof(int)*2)])
				{   c = _T(':');
#if __INTSIZE == 4
				    ul = uls;		/* now work on seg */
				    uls = 0;
#endif
				}
				else
				{
				    c = (ul % base) + _T('0');
				    if (c > _T('9'))
					c += (flags & FLuc) ? _T('A')-_T('0')-10
							    : _T('a')-_T('0')-10;
				    ul /= base;
				}
				*sbuf = c;
				sbuf--;
			    } while (ul | uls);
			    sbuf++;
			}
			else
			{   /* 0 and 0 precision yields 0 digits	*/
			    if (precision == 0 && flags & FLprec)
				sbuf++;
			    else
				*sbuf = _T('0');
			}
			sl = &buf[BUFMAX] - sbuf;
			if (precision > BUFMAX)
			    precision = BUFMAX;
			if (sl < precision)
			{
			    for (i = precision - sl; i--;)
			    {	--sbuf;
				*sbuf = (flags & FLlptr && sbuf == &buf[BUFMAX-1-(sizeof(int)*2)])
				    ? _T(':') : _T('0');
			    }
			    sl = precision;
			}
			s = (_TCHAR *) sbuf;
			}
			break;

		    case _T('f'):
		    case _T('F'):
		    case _T('e'):
		    case _T('E'):
		    case _T('g'):
		    case _T('G'):
		    case _T('a'):
		    case _T('A'):
			if (!TPFLOATFMT)
			    FATERR ("Floating point not loaded");
		    Lagain:
			sl = fltbufmax;
			prefix = (*TPFLOATFMT)(c,flags,precision,(double *)pargs,fltbuf,&sl,width);
			if (sl == -1)
			{   // Buffer overflow. Make a bigger one
			    fltbufmax *= 2;
			    fltbuf = (_TCHAR __ss *)alloca(fltbufmax * sizeof(_TCHAR));
			    goto Lagain;
			}
			if (flags & FLlngdbl)
			    (void) va_arg(pargs,long double);
			else
			    (void) va_arg(pargs,double);
			s = fltbuf;
			break;

		    case _T('n'):
			{
				int FAR *pi;
				
				pi = (int FAR *)getpointer (&pargs, flags);
				if (flags & FLhh)
				    *(char FAR *)pi = nout;
#if __INTSIZE == 4
				else if (flags & FLshort)
				    *(short FAR *)pi = nout;
#else
				else if (flags & FLlong)
				    *(long FAR *)pi = nout;
#endif
				else
				    *pi = nout;
			}
			continue;
		    default:
			goto err;
		}

		/* Send out the data. Consists of padding, prefix,	*/
		/* more padding, the string, and trailing padding	*/

		pl = FSTRLEN (prefix);		/* length of prefix string */
		nout += pl + sl;
		padding = width - (pl + sl);	/* # of chars to pad	*/

		/* if 0 padding, send out prefix (if any)	*/
		if (flags & FL0pad)
		    for (; *prefix; prefix++)
			if ((*func)(*prefix & TCHARMASK,fp) == _TEOF) goto err;

		/* if right-justified and chars to pad			*/
		/*	send out prefix string				*/
		if (padding > 0)
		{   nout += padding;
		    if (!(flags & FLleft))
		    {	while (--padding >= 0)
			    if ((*func)((flags & FL0pad) ? _T('0')
					 : _T(' '),fp) == _TEOF) goto err;
		    }
		}

		/* send out prefix (if any)	*/
		for (; *prefix; prefix++)
			if ((*func)(*prefix & TCHARMASK,fp) == _TEOF) goto err;
#ifdef __NT__
# ifndef _UNICODE
		/* send out to sbc or mbc string for printf	*/
		if (flags & FLwide) {
		    /* convert wide to sbc or mbc string	*/
		    for (i = 0; i < sl; i++) {
		      char	mbc[4];
		      int	len;
                        len = wctomb (mbc, ws[i]);
			if (len < 1) goto err;
			if ((*func)(mbc[0] & TCHARMASK,fp) == _TEOF) goto err;
			if (len == 2) {
			    if ((*func)(mbc[1] & TCHARMASK,fp) == _TEOF) goto err;
			    nout++;
			}
		    }
		} else {
		    /* non convert				*/
		    /* sl = _strnsize (s, sl); */
		    for (i = 0; i < sl; i++)
			if ((*func)(s[i] & TCHARMASK,fp) == _TEOF) goto err;
		}
# else
		/* send out to wide string for wprintf		*/
		if (flags & FLchar) {
		    /* convert sbc or mbc to wide string	*/
		    /* sl = _strnsize (cs, sl); */
		    for (i = 0; i < sl; i++) {
		      wchar_t	wch;
                        if (mbtowc (&wch, cs + i, MB_CUR_MAX) < 1) goto err;
			if ((*func)(wch & TCHARMASK,fp) == _TEOF) goto err;
                        if (isleadbyte ((unsigned char) cs[i])) {
			    i++;
			    nout--;
			}
		    }
		} else {
		    /* non convert				*/
		    for (i = 0; i < sl; i++)
			if ((*func)(s[i] & TCHARMASK,fp) == _TEOF) goto err;
		}
# endif
#else
		/* send out string	*/
		for (i = 0; i < sl; i++)
			if ((*func)(s[i] & TCHARMASK,fp) == _TEOF) goto err;
#endif
		/* send out right padding	*/
		if (flags & FLleft)
		{	while (--padding >= 0)
			    if ((*func)(_T(' '),fp) == _TEOF) goto err;
		}

	} /* while */
	return nout;

    err:
	return -1;
}

/**/
