//_ strtoll.c
// Copyright (C) 1986-2003 by Digital Mars
// All Rights Reserved
// Written by Walter Bright
// www.digitalmars.com

#include	<ctype.h>
#include	<errno.h>
#include	<stdlib.h>
#include 	<limits.h>
#include 	<tchar.h>
#include        <scrtl.h>

#ifndef __STDC__
#define BINARY	1		/* support non-standard 0b conversions	*/
#endif

#if !_UNICODE
long long atoll(const char *p)
{
    return strtoll(p, (char **)NULL, 0);
}
#endif

/****************************
 * Convert ascii string to a long long.
 * The ascii string consists of:
 *	{ isspace chars } [ '+' | '-'] ['0' | '0x' | '0X'] { digit | letter }
 * Input:
 *	p	pointer to ascii string to convert
 *	base	base of conversion (2..36). If base is 0, the base is
 *		determined from whether the sequence starts with 0: octal,
 *		0x: hex, 0X: hex, otherwise decimal.
 *		If a base of 11..36 is used, the letters a..z are used
 *		to represent the digits.
 * Output:
 *	**pend = the first unrecognized character (the one that doesn't
 *		match the above grammar) if pend is not NULL.
 * Returns:
 *	If an overflow or underflow occurs, errno is set to ERANGE and the
 *	value LLONG_MAX or LLONG_MIN, respectively, is returned.
 *	If no conversion is done, 0 is returned.
 *	Otherwise, the converted number is returned.
 */

#if _UNICODE
long long wcstoll(const wchar_t *p, wchar_t **pend, int base)
#else
long long strtoll(const char *p,char **pend,int base)
#endif
{
    long long number = 0;
    int positive,c;
    int error;
    const TCHAR *pstart;
    int anytrans = 0;

    pstart = p;

    while (_istspace(*p))		// skip leading white space
	p++;

    switch (*p)
    {   case '-':
	    positive = 0;
	    p++;
	    break;
	case '+':
	    p++;
	    // FALL-THROUGH
	default:
	    positive = 1;
	    break;
    }
    switch (base)
    {   case 0:
	    base = 10;		// assume decimal base
	    if (*p == '0')
	    {   base = 8;	// could be octal
		p++;
		anytrans++;	// count 0 as a translated char
		switch (*p)
		{   case 'x':
		    case 'X':
			base = 16;	// hex
			p++;
			anytrans--;	// oops, the previous 0 was part of a 0x, now look for valid chars
			break;
#if BINARY
		    case 'b':
		    case 'B':
			base = 2;	// binary
			p++;
			anytrans--;
			break;
#endif
		}
	    }
	    break;
	case 16:			// skip over '0x' and '0X'
	    if (*p == '0' && (p[1] == 'x' || p[1] == 'X'))
		p += 2;
	    break;
#if BINARY
	case 2:			// skip over '0b' and '0B'
	    if (*p == '0' && (p[1] == 'b' || p[1] == 'B'))
		p += 2;
	    break;
#endif
    }
    error = 0;
    while (1)
    {   c = *p;
	if (c >= '0' && c <= '9')
	    c -= '0';
	else if (c >= 'a' && c <= 'z')
	    c = c - ('a' - 10);
	else if (c >= 'A' && c <= 'Z')
	    c = c - ('A' - 10);
	else
	{   // unrecognized character
	    // Go back to beginning of string if nothing
	    // was dealt with properly
	    if (anytrans == 0)
		p = pstart;
	    break;
	}
	if (c >= base)		// not in number base
	{
	    // Go back to beginning of string if no characters
	    // were successfully dealt with
	    if (anytrans == 0)
		p = pstart;
	    break;
	}
	if ((LLONG_MIN + c) / base > number)
	    error = 1;
	number = number * base - c;
	p++;
	anytrans++;
    }
    if (pend)
	*pend = (void *) p;
    if (positive && number == LLONG_MIN || error)
    {
	/*  a range error has occurred, set errno and return
	    LLONG_MAX or LLONG_MIN dependant upon positive.
	    I.e. errange on a negative, means it was under
	    LLONG_MIN, on a positive, was above LLONG_MAX
	 */
	number = (positive) ? LLONG_MAX : LLONG_MIN;
	__set_errno (ERANGE);
	return number;
    }
    return (positive) ? -number : number;
}
