//_ strtod.c
// Copyright (C) 1985-2001 by Digital Mars
// All Rights Reserved

#include	<stdlib.h>
#include	<ctype.h>
#include	<float.h>
#include	<string.h>
#include	<math.h>
#include	<fltpnt.h>
#include	<fenv.h>
#include        <scrtl.h>
#if _WIN32
#include        <setlocal.h>
#endif

#if _WIN32

double strtod(const char *p,char **endp)
{   int errnosave = errno;
    long double ld;
    double d;

    errno = 0;
    ld = strtold(p,endp);
    d = ld;
    if (ld != 0 && d == 0)		// if underflow converting to double
	__set_errno(ERANGE);
    else if (ld != HUGE_VAL && d == HUGE_VAL)	// if infinity converting to double
	__set_errno(ERANGE);
    else if (errno != ERANGE)
    {
	errno = errnosave;
    }
    return d;
}

#else

#if 0
static double negtab[] =
	{1e-256,1e-128,1e-64,1e-32,1e-16,1e-8,1e-4,1e-1,1e-1,1.0};

static double postab[] =
	{1e+256,1e+128,1e+64,1e+32,1e+16,1e+8,1e+4,1e+2,1e+1};
#else

// Use exact values, computed separately, to bootstrap.
// The digits here past 17 are just for amusement value, they
// only contribute to the 'sticky' bit.

static double negtab[] =
{
    1 / 0x5.53f75fdcefcef46eeddc80dcc7f755bc28f265f9ef17cc5573c063ff540e3c42d35a1d153624adc666b026b2716ed595d80fcf4a6e706bde50c612152f87d8d99f72bed3875b982e7c01p+848,
    1 / 0x2.4ee91f2603a6337f19bccdb0dac404dc08d3cff5ec2374e42f0f1538fd03df99092e953e01p+424,
    1 / 0x18.4f03e93ff9f4daa797ed6e38ed64bf6a1f01p+208,
    1 / 0x4.ee2d6d415b85acef81p+104,
    1 / 0x23.86f26fc1p+48,
    1 / 0x5.f5e1p+24,
    1 / 0x27.10p+8,
    1 / 0x64.p+0,
    1 / 0xa.p+0,
};

static double postab[] =
{
    0x5.53f75fdcefcef46eeddc80dcc7f755bc28f265f9ef17cc5573c063ff540e3c42d35a1d153624adc666b026b2716ed595d80fcf4a6e706bde50c612152f87d8d99f72bed3875b982e7c01p+848,
    0x2.4ee91f2603a6337f19bccdb0dac404dc08d3cff5ec2374e42f0f1538fd03df99092e953e01p+424,
    0x18.4f03e93ff9f4daa797ed6e38ed64bf6a1f01p+208,
    0x4.ee2d6d415b85acef81p+104,
    0x23.86f26fc1p+48,
    0x5.f5e1p+24,
    0x27.10p+8,
    0x64.p+0,
    0xa.p+0,
};

#endif

/*************************
 * Convert string to double.
 * Terminates on first unrecognized character.
 */

double strtod(const char *p,char **endp)
{
	double dval;
	int exp;
	unsigned long msdec,lsdec;
	unsigned long msscale;
	char hex,dot,sign,subject;
	int pow;
	const char *pinit = p;
	static char infinity[] = "infinity";
	static char nans[] = "nans";
	fenv_t flagp;
	unsigned int old_cw;
	unsigned int old_status;
	
	fegetenv(&flagp);  /* Store all exceptions, and current status word */
	if (_8087)
	{
	    old_cw = _control87(_MCW_EM,_MCW_EM);  /* disable exceptions from occurring */
	}
	
	while (isspace(*p))
	    p++;
	sign = 0;			/* indicating +			*/
	switch (*p)
	{	case '-':
			sign++;
			/* FALL-THROUGH */
		case '+':
			p++;
	}
	subject = 0;
	dval = 0.0;
	dot = 0;			/* if decimal point has been seen */
	exp = 0;
	hex = 0;
	msdec = lsdec = 0;
	msscale = 1;

	switch (*p)
	{   case 'i':
	    case 'I':
		if (memicmp(p,infinity,8) == 0)
		{   p += 8;
		    goto L4;
		}
		if (memicmp(p,infinity,3) == 0)		/* is it "inf"?	*/
		{   p += 3;
		L4:
		    dval = HUGE_VAL;
		    subject = 1;
		    goto L3;
		}
		break;
	    case 'n':
	    case 'N':
		if (memicmp(p,nans,4) == 0)		/* "nans"?	*/
		{   p += 4;
		    dval = NANS;
		    goto L5;
		}
		if (memicmp(p,nans,3) == 0)		/* "nan"?	*/
		{   p += 3;
		    dval = NAN;
		L5:
		    if (*p == '(')		/* if (n-char-sequence)	*/
			goto L1;		/* invalid input	*/
		    subject = 1;
		    goto L3;
		}
	}

	if (*p == '0' && (p[1] == 'x' || p[1] == 'X'))
	{   p += 2;
	    hex++;
	    while (1)
	    {	int i = *p;

		while (isxdigit(i))
		{
		    subject = 1;	/* must have at least 1 digit	*/
		    i = isalpha(i) ? ((i & ~0x20) - ('A' - 10)) : i - '0';
		    if (msdec < (0xFFFFFFFF-16)/16)
			msdec = msdec * 16 + i;
		    else if (msscale < (0xFFFFFFFF-16)/16)
		    {	lsdec = lsdec * 16 + i;
			msscale *= 16;
		    }
		    else
			exp++;
		    exp -= dot;
		    i = *++p;
		}
#if _WIN32
		if (i == *__locale_decpoint && !dot)
#else
		if (i == '.' && !dot)
#endif
		{	p++;
			dot++;
		}
		else
			break;
	    }
	    exp *= 4;
	    if (!subject)		/* if error (no digits seen)	*/
		goto L1;
	    if (*p == 'p' || *p == 'P')
		goto L2;
	    else
	    {	subject = 0;
		goto L1;		/* error, exponent is req'd	*/
	    }
	}
	else
	{
	    while (1)
	    {	int i = *p;

		while (isdigit(i))
		{
		    subject = 1;	/* must have at least 1 digit	*/
		    if (msdec < (0x7FFFFFFF-10)/10)
			msdec = msdec * 10 + (i - '0');
		    else if (msscale < (0x7FFFFFFF-10)/10)
		    {	lsdec = lsdec * 10 + (i - '0');
			msscale *= 10;
		    }
		    else
			exp++;
		    exp -= dot;
		    i = *++p;
		}
#if _WIN32
		if (i == *__locale_decpoint && !dot)
#else
		if (i == '.' && !dot)
#endif
		{	p++;
			dot++;
		}
		else
			break;
	    }
	}
	if (!subject)			/* if error (no digits seen)	*/
	    goto L1;			/* return 0.0			*/
	if (*p == 'e' || *p == 'E')
	{
	    char sexp;
	    int e;
	L2:
	    sexp = 0;
	    switch (*++p)
	    {	case '-':
			sexp++;
			/* FALL-THROUGH */
		case '+':
			p++;
	    }
	    subject = 0;
	    e = 0;
	    while (isdigit(*p))
	    {
		if (e < DBL_MAX_EXP*2)		/* prevent integer overflow */
		    e = e * 10 + *p - '0';
		p++;
		subject = 1;
	    }
	    exp += (sexp) ? -e : e;
	    if (!subject)		/* if no digits in exponent	*/
		goto L1;		/* return 0.0			*/
	}

	dval = msdec;
	if (msscale != 1)		/* if stuff was accumulated in lsdec */
	    dval = dval * msscale + lsdec;

	if (dval)
	{   unsigned u;

	    if (hex)
	    {
		/* Exponent is power of 2, not power of 10	*/
		dval = ldexp(dval,exp);
		exp = 0;
	    }

	    u = 0;
	    pow = 256;

	    while (exp > 0)
	    {	while (exp >= pow)
		{	dval *= postab[u];
			exp -= pow;
		}
		pow >>= 1;
		u++;
	    }
	    while (exp < 0)
	    {	while (exp <= -pow)
		{	dval *= negtab[u];
			if (dval == 0)
				__set_errno (ERANGE);
			exp += pow;
		}
		pow >>= 1;
		u++;
	    }
	        
	    /* if overflow occurred		*/
	    if (dval == HUGE_VAL)
		__set_errno (ERANGE);		/* range error			*/
	}

    L1:
	if (endp)
	{   if (subject == 0)		/* if subject string was empty	*/
		p = pinit;		/* indicate no conversion done	*/
	    *endp = (void *) p;
	}
    L3:
    fesetenv(&flagp);  /* reset floating point environment */
    if (_8087)
    {
        /* clear status */
        _clear87();
        /* reset control word */
        _control87(old_cw,old_cw);
    }
    
    return (sign) ? -dval : dval;
}

#endif
