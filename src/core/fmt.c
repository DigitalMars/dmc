//_ fmt.c
// Copyright (C) 1986-2009 by Digital Mars
// All Rights Reserved
// http://www.digitalmars.com

#include	<tchar.h>
#include	<stdlib.h>
#include	<string.h>
#include	<float.h>
#include	<stdio.h>
#include	<fp.h>
#include	<ctype.h>
#ifdef _WIN32
#include	<setlocal.h>
#endif
#include	<sctchar.h>

#if _MT
#include	"mt.h"
#endif

#define ECVT	0
#define FCVT	1

#ifdef __NT__
# ifndef _UNICODE
#define TDECPOINT	*__locale_decpoint
# else
#define TDECPOINT	__wdecpoint
static wchar_t		__wdecpoint = L'.';
static void __pascal __getwdecpoint (void) {
 wchar_t	wdecpoint;
  if (mbtowc (&wdecpoint, __locale_decpoint, 1) > 0 ) {
    __wdecpoint = wdecpoint;
  }
}
# endif
#else
#define TDECPOINT	_T('.')
#endif

char * __pascal __floatcvt(int cnvflag,char *digstr,long double val,int ndig,int *pdecpt,int *psign);

/********************** printf interface ********************/

/* Flags for flag word (keep in sync with ecvt.c)	*/
#define FLleft		1
#define	FLplus		2
#define	FLspc		4
#define FLhash		8
#define	FLlong		0x10
#define FLlngdbl	0x20
#define FL0pad		0x40
#define FLprec		0x80
#define FLuc		0x100
#define FLuns		0x200
#define FLlptr		0x400		/* far pointer format (with :)	*/
#define FLshort		0x800		/* 'h' modifier			*/
#define FLgformat	0x1000		/* g floating point format	*/

#ifndef _UNICODE
char __near * __pascal __dosign (int, int);
#define TDOSIGN		__dosign
#else
wchar_t __near * __pascal __wdosign (int, int);
#define TDOSIGN		__wdosign
#endif

static void __pascal __near __doexponent(_TCHAR __ss * __ss *ps,int exp,int flag);
static _TCHAR __ss * __pascal __near __trim0(_TCHAR __ss *s);

/***********************************
 * Do floating point formatting.
 */

_TCHAR __near *
#ifndef _UNICODE
__floatfmt
#else
__wfloatfmt
#endif
(int c,int flags,int precision,double *pdval,_TCHAR __ss *buf,int __ss *psl,int width)
{
    char *p;
    int fpd;			/* classification of double value	*/
    int decpt;			/* exponent (base 10) for floats	*/
    int sign;
    _TCHAR __near *prefix;
    int sl;			/* length of formatted string s		*/
    char digstr[LDBL_DIG*2 + 2];
    _TCHAR __ss *sbuf;
    long double dval;
    int bufmax = *psl;

#ifdef __NT__
# ifdef _UNICODE
    __getwdecpoint ();
# endif
#endif

    if (!(flags & FLprec))	/* if no precision	*/
	precision = 6;		/* default precision	*/

    if (flags & FLlngdbl)
	dval = *(long double *)pdval;
    else
	dval = *pdval;
    fpd = fpclassify(dval);
    if (fpd <= FP_INFINITE)	/* if nan or infinity	*/
    {
	static _TCHAR fptab[][5] =
	{ _T("nans"),_T("nan"),_T("inf") };
#ifdef _UNICODE
	static _TCHAR fptabuc[][5] =
	{ _T("NANS"),_T("NAN"),_T("INF") };
#endif
	static char fptablen[] = { 4,3,3 };

	/* '#' and '0' flags have no effect	*/
	flags &= ~(FL0pad | FLhash);
	prefix = TDOSIGN(signbit(dval),flags);
#ifdef _UNICODE
	wcscpy(buf,(c & 0x20) ? fptab[fpd] : fptabuc[fpd]);
#else
#if 1 /* warty but small */
	*(unsigned long *)buf = *(unsigned long *)fptab[fpd];
	if (!(c & 0x20))	/* if c is upper case	*/
	    /* Convert to upper case */
	    *(unsigned long *)buf &= ~0x20202020;
#else /* portable */
	strcpy(buf,fptab[fpd]);
	if (isupper(c))
	    strupr(buf);
#endif
#endif
	sl = fptablen[fpd];	/* length of string	*/
    }
    else
    {
	switch (c)
	{
	    case _T('f'):
	    case _T('F'):
	    fformat:
		p = __floatcvt(FCVT,digstr,dval,precision,&decpt,&sign);
		prefix = TDOSIGN(sign,flags);
		sbuf = &buf[0];
		if (decpt <= 0)
		    *sbuf++ = _T('0');	/* 1 digit before dec point */
		while (decpt > 0)
		{
		    if (sbuf >= &buf[bufmax - 1])
			goto Lerror;
		    *sbuf++ = *p ? *p++ : _T('0');
		    decpt--;
		}
		if (precision > 0 || flags & FLhash)
		{   int n;

		    *sbuf++ = TDECPOINT;	/* . */
		    while (decpt < 0 && precision > 0)
		    {
			if (sbuf >= &buf[bufmax])
			    goto Lerror;
			*sbuf++ = _T('0');
			decpt++;
			precision--;
		    }

		    if (precision > &buf[bufmax] - sbuf)
			goto Lerror;
		    for (; precision; precision--)
		    {
			*sbuf++ = *p ? *p++ : _T('0');
		    }

		    /* remove trailing 0s	*/
		    if ((flags & (FLgformat | FLhash)) == FLgformat)
			sbuf = __trim0(sbuf);
		}
		sl = sbuf - &buf[0];	/* length of string	*/
		break;
	    case _T('e'):
	    case _T('E'):
		p = __floatcvt(ECVT,digstr,dval,precision + 1,&decpt,&sign);
	    eformat:
		prefix = TDOSIGN(sign,flags);
		buf[0] = *p;
		sbuf = &buf[1];
		if (flags & FLhash ||
		    (precision > 0 &&
		    (!(flags & FLgformat) || precision != 1 || width))
		   )
		{	int n;

			buf[1] = TDECPOINT;	/* . */
			n = bufmax - 5 - 2;
			if (n < precision)
			    goto Lerror;

			p++;
			sbuf++;
			for (n = 0; n < precision; n++)
			{
			    *sbuf++ = *p ? *p++ : _T('0');
			}

			/* remove trailing 0s	*/
			if ((flags & (FLgformat | FLhash)) == FLgformat)
			    sbuf = __trim0(sbuf);
		}
		if (precision || (flags & FLprec && !(flags & FLgformat)))
		{   *sbuf++ = c;
		    if (dval)		/* avoid 0.00e-01	*/
			decpt--;
		    __doexponent(&sbuf,decpt,1);
		}
		sl = sbuf - &buf[0];	/* length of string	*/
		break;
	    case _T('G'):
	    case _T('g'):
		flags |= FLgformat;
		if (precision == 0)
		    precision = 1;
		p = __floatcvt(ECVT,digstr,dval,precision,&decpt,&sign);
		/* decpt-1 is the exponent	*/
		if (decpt < -3 || decpt > precision)
		{			/* use e format		*/
		    if (precision > 1)
			precision--;
		    c -= _T('g') - _T('e');
		    goto eformat;
		}
		else
		{   /* Convert precision to digits *after* dot	*/
		    precision -= decpt;	/* precision is >= 0	*/
		    goto fformat;
		}

	    case _T('a'):
	    case _T('A'):
#if _WIN32
		decpt = (((unsigned short *)&dval)[4] & 0x7FFF) - 0x3FFF;
		{   unsigned long long val = *(unsigned long long *)&dval;
		    unsigned long long val2;
		    int i;

		    /* deal with denormals
		     * (do not include implicit bit in shift count)
		     * Fix for http://d.puremagic.com/issues/show_bug.cgi?id=675
		     *
			void main()
			{
			    printf("%La %La\n", 0x1p-16382L, 0x1p-16383L);
			}

			prints:
			-----------
			0x1p-16382 0x1p-16384 <= wrong!
			------------
		     */
		    if ((long long)val > 0)
			decpt++;	// compensate for implicit bit

		    // Left justify
		    while ((long long)val > 0)
		    {	val <<= 1;
			decpt--;
		    }

		Lagain:
		    val2 = val;
		    p = digstr;
		    if ((long long)val < 0)
			*p = '1';
		    else
			*p = '0';
		    p++;
		    val <<= 1;

		    if (!(flags & FLprec))
			precision = 1000;	// number arbitrarilly larger
						// than nibbles in mantissa

		    for (i = 0; i < precision; i++)
		    {
			unsigned nibble;

			if (!(flags & FLprec) && !val)	// don't leave trailing 0's
			{   precision = i;
			    break;
			}
			nibble = (val >> 60) & 0x0F;
			if (nibble < 10)
			    *p = '0' + nibble;
			else if (c == _T('a'))
			    *p = 'a' + nibble - 10;
			else
			    *p = 'A' + nibble - 10;
			val <<= 4;
			p++;
			if (p - digstr == sizeof(digstr)/sizeof(digstr[0]) - 1)
			    break;
		    }
		    *p = 0;

		    // Round up if (guard && (odd || sticky))
		    if ((long long)val < 0 && (p[-1] & 1 || (val << 1)))
		    {
			val2 += 0x8000000000000000LL >> (i * 4);
			if ((long long)val2 >= 0)
			{
			    val2 >>= 1;
			    val2 |= 0x8000000000000000LL;
			    decpt++;
			}
			val = val2;
			goto Lagain;
		    }
		}
#else
		decpt = ((((unsigned short *)&dval)[3] & 0x7FF0) >> 4) - 0x3FF;
		{   unsigned long val[2];
		    unsigned long val2[2];
		    int i;

		    val[0] = ((unsigned long *)&dval)[0];
		    val[1] = ((unsigned long *)&dval)[1];

		    // Left justify
		    if (dval)
		    {	val[1] |= 0x100000L;
			val[1] <<= 11;
			val[1] |= val[0] >> 5;
			val[0] <<= 11;
		    }

		Lagain:
		    val2[0] = val[0];
		    val2[1] = val[1];
		    p = digstr;
		    if ((long)val[1] < 0)
			*p = '1';
		    else
			*p = '0';
		    p++;
		    val[1] <<= 1;
		    if ((long)val[0] < 0)
			val[1] |= 1;
		    val[0] <<= 1;

		    if (!(flags & FLprec))
			precision = 1000;	// number arbitrarilly larger
						// than nibbles in mantissa

		    for (i = 0; i < precision; i++)
		    {
			unsigned nibble;

			if (!(flags & FLprec) && !(val[1] | val[0]))	// don't leave trailing 0's
			{   precision = i;
			    break;
			}
			nibble = (val[1] >> 28) & 0x0F;
			if (nibble < 10)
			    *p = '0' + nibble;
			else if (c == _T('a'))
			    *p = 'a' + nibble - 10;
			else
			    *p = 'A' + nibble - 10;
			p++;
			val[1] <<= 4;
			val[1] |= val[0] >> 28;
			val[0] <<= 4;
			if (p - digstr == sizeof(digstr)/sizeof(digstr[0]) - 1)
			    break;
		    }
		    *p = 0;

		    // Round up if (guard && (odd || sticky))
		    if ((long)val[1] < 0 && (p[-1] & 1 || (val[1] << 1) || val[0]))
		    {
			if (i >= 8)
			{   if ((long)val2[0] < 0)
			    {	val2[0] += 0x80000000L >> ((i - 8) * 4);
				if ((long)val2[0] >= 0)
				    val2[1]++;
			    }
			    else
				val2[0] += 0x80000000L >> ((i - 8) * 4);
			}
			else
			    val2[1] += 0x80000000L >> (i * 4);
			if ((long)val2[1] >= 0)
			{
			    val2[0] >>= 1;
			    if (val2[1] & 1)
				val2[0] |= 0x80000000L;
			    val2[1] >>= 1;
			    val2[1] |= 0x80000000L;
			    decpt++;
			}
			val[0] = val2[0];
			val[1] = val2[1];
			goto Lagain;
		    }
		}
#endif
		p = digstr;
		sign = 0;
		//if (dval < 0)
		if (signbit(dval))
		    sign = 1;

		prefix = TDOSIGN(sign,flags);
		buf[0] = _T('0');
		buf[1] = 'X' | (c & 0x20);	// 'X' or 'x'
		buf[2] = *p;
		sbuf = &buf[3];
		if (flags & FLhash || precision > 0 || !dval)
		{	int n;

			buf[3] = TDECPOINT;	/* . */
			n = bufmax - 7 - 4;	// 4 for 0xh., 7 for p+ddddd
			if (n < precision)
			    goto Lerror;

			p++;
			sbuf++;
			for (n = 0; n < precision; n++)
			{
			    *sbuf++ = *p ? *p++ : _T('0');
			}
		}
		//if (precision || flags & FLprec)
		{   *sbuf++ = 'P' | (c & 0x20);		// 'P' or 'p'
		    if (!dval)
			decpt = 0;	// exponent for 0 is 0
		    __doexponent(&sbuf,decpt,0);
		}
		sl = sbuf - &buf[0];	/* length of string	*/
		break;
	}
    }
    *psl = sl;
    return prefix;

Lerror:
    *psl = -1;
    return prefix;
}

/**************************
 * Add exponent to string s in form +-nn.
 * At least 2 digits.
 */

static void __pascal __near __doexponent(_TCHAR __ss * __ss *ps,int exp,int flag)
{	register _TCHAR __ss *s = *ps;

	*s++ = (exp < 0) ? ((exp = -exp),_T('-')) : _T('+');

	if (exp >= 10000)
	{
	    *s++ = exp / 10000 + _T('0');
	    exp %= 10000;
	    goto L2;
	}
	else if (exp >= 1000)
	{
	 L2:
	    *s++ = exp / 1000 + _T('0');
	    exp %= 1000;
	    goto L1;
	}
	else if (exp >= 100)
	{
	 L1:
	    *s++ = exp / 100 + _T('0');
	    exp %= 100;
	    goto L0;
	}
	// Microsoft uses a minimum of 3 digits.
	// We do 2 for ANSI C99 compatibility.
	else if (flag || exp >= 10)
	{
	 L0:
	    *s++ = exp / 10 + _T('0');
	    exp %= 10;
	}
	*s++ = exp + _T('0');
	*ps = s;
}

/**************************
 * Trim trailing 0s and decimal point from string.
 */

static _TCHAR __ss * __pascal __near __trim0(_TCHAR __ss *s)
{   while (*(s-1) == _T('0'))
	s--;
    if (*(s-1) == TDECPOINT)		/* . */
	s--;
    return s;
}



/********************** scanf interface ********************/

/* This must match up with scanf.c	*/

#define FLneg	1
#define FLstar	2
#define	FLh	4
#define FLl	8
#define FLL	0x10
#define FLhh	0x40

#define GETCHAR		(ss.nread++, (*ss.func)(ss.fp))
#define assign		(!(ss.flag & FLstar))

/****************************
 * Variables to save on parameter passing.
 * Collect all state variables into a struct.
 */

struct scanf_state
{
	int nread;	/* number of characters read from input		*/
	int nass;	/* number of assigned input items		*/
	int width;	/* field width					*/
	int flag;	/* mask of FLxxxx				*/
	int x;		/* last character read				*/
	_TCHAR *p;	/* argument pointer				*/
	const _TCHAR *format;	/* pointer to format string		*/
	void *fp;		/* argument to function pointers	*/
	int (*func)(void *);	/* where to get characters		*/
};

#define x ss.x

#define ss (*pss)

/******************************
 * Do a floating point number.
 * Use atof() so we only have to get one conversion routine right. Just
 * load chars into a buffer and pass to atof().
 * Input:
 *	x = first char of number
 * Returns:
 *	x = char after the float, or EOF
 *	*p = double result
 *	non-zero: successful conversion
 *	0: failed conversion
 */

int __pascal
#ifndef _UNICODE
__scanfloat
#else
__wscanfloat
#endif
(struct scanf_state __ss *pss)
{   int dot,exp,digs;
    char nbuf[31];	// temp storage for float string
    char __ss *buf;
    int bufi;
    int bufmax;
    static _TCHAR snan[] = _T("ans");
    static _TCHAR sinf[] = _T("nfinity");
    _TCHAR __near *fs;
    int dblstate;
    int hex;			/* is this a hexadecimal-floating-constant? */

#ifdef _WIN32
# ifdef _UNICODE
    __getwdecpoint ();
# endif
#endif

    dot = 0;			/* if decimal point has been seen */
    exp = 0;			/* if we're reading in the exponent */
    digs = 0;

    buf = nbuf;
    bufmax = sizeof(nbuf) / sizeof(nbuf[0]);
    bufi = 0;

    dblstate = 10;
    hex = 0;
    for (;; x = GETCHAR)
    {
	if (ss.width-- <= 0)
	    goto done;

	while (1)
	{   switch (dblstate)
	    {
		case 10:		/* initial state		*/
		    switch (x)
		    {	case _T('+'):
			case _T('-'):
			    dblstate = 0;
			    goto store;
		    }
		    /* FALL-THROUGH */
		case 0:			/* start of number		*/
		    switch (x)
		    {	case _T('0'):
			    dblstate = 9;
			    digs = 1;	/* saw some digits		*/
			    break;
			case _T('n'):
			case _T('N'):	/* nan or nans			*/
			    fs = snan;
			    dblstate = 11;
			    break;
			case _T('i'):
			case _T('I'):	/* inf or infinity		*/
			    fs = sinf;
			    dblstate = 11;
			    break;
			default:
			    dblstate = 1;
			    continue;
		    }
		    break;

		case 11:		/* parsing strings		*/
		    if (isalpha(x) && tolower(x) == *fs)
			fs++;
		    else
		    {	/* Determine if we did a proper match	*/
			if (*fs == 0 || *fs == _T('s') ||
			    (fs - sinf == 2))
			    digs = 1;
			goto done;
		    }
		    break;

		case 9:
		    dblstate = 1;
		    if (x == _T('X'))
		    {	hex++;
			digs = 0;
			break;
		    }
		    /* FALL-THROUGH */

		case 1:			/* digits to left of .		*/
		case 3:			/* digits to right of .		*/
		case 7:			/* continuing exponent digits	*/
		    if (!isdigit(x) && !(hex && isxdigit(x)))
		    {   dblstate++;
			continue;
		    }
		    digs = 1;		/* saw some digits		*/
		    break;

		case 2:			/* no more digits to left of .	*/
		    if (x == TDECPOINT)	/* . */
		    {   dblstate++;
			break;
		    }
		    /* FALL-THROUGH */

		case 4:			/* no more digits to right of .	*/
		    if (x == _T('E') || x == _T('e') ||
			hex && (x == _T('P') || x == _T('p')))
		    {   dblstate = 5;
			hex = 0;	/* exponent is always decimal	*/
			break;
		    }
		    if (hex)
			goto error;	/* binary-exponent-part required */
		    goto done;

		case 5:			/* looking immediately to right of E */
		    dblstate++;
		    if (x == _T('-') || x == _T('+'))
			break;
		    /* FALL-THROUGH */

		case 6:			/* 1st exponent digit expected	*/
		    if (!isdigit(x))
			goto error;	/* exponent expected		*/
		    dblstate++;
		    break;

		case 8:			/* past end of exponent digits	*/
		    goto done;
	    }
	    break;
	}
    store:
#if _UNICODE
	if (!isascii(x))
	    goto error;		// unicode digits can't be part of floating literal
#endif
	buf[bufi] = (char)x;
	bufi++;

	// Reallocate buf[] if it overflows
	if (bufi == bufmax)
	{   char __ss *b;

#if __INTSIZE == 2
	    bufmax += 34;	// use smaller increments to save on stack
#else
	    bufmax *= 2;
#endif
	    b = (char __ss *)alloca(bufmax);
	    if (!b)
		goto error;
	    buf = (char __ss *)memcpy(b, buf, bufi * sizeof(char));
	}
    }
done:
    buf[bufi] = 0;		// terminate float string
    if (digs)			// if we actually saw some digits
    {	if (assign)		// if not suppressed
	{   long double ld;

	    ss.nass++;
	    ld = strtold(buf, NULL);
	    if (ss.flag & FLL)
		*(long double *)ss.p = ld;
	    else if (ss.flag & FLl)
		*(double *)ss.p = (double)ld;
	    else
		*(float *)ss.p = (float)ld;
	}
	return 1;
    }
error:
    return 0;
}

