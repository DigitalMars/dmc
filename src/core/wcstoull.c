//_ strtol.c
// Copyright (C) 1986-2001 by Digital Mars
// All Rights Reserved
// Written by Walter Bright
// www.digitalmars.com

#include	<ctype.h>
#include	<errno.h>
#include	<stdlib.h>
#include 	<limits.h>
#include	<tchar.h>

#include <stdio.h>

#include        <scrtl.h>

#ifndef __STDC__
#define BINARY	1		/* support non-standard 0b conversions	*/
#endif

/****************************
 * Convert ascii string to an unsigned long.
 * The ascii string consists of:
 *	{ isspace chars } ['0' | '0x' | '0X'] { digit | letter }
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
 *	value ULONG_MAX is returned. If no conversion is done, 0 is returned.
 *	Otherwise, the converted number is returned.
 */

#if _UNICODE
unsigned long long wcstoull(const wchar_t *p, wchar_t **pend, int base)
#else
unsigned long long strtoull(const char *p,char **pend,int base)
#endif
{
	unsigned long long number = 0;
	int c;
	int error;

	while (_istspace(*p))		// skip leading white space
	    p++;
	if (*p == '+')
	    p++;
	switch (base)
	{   case 0:
		base = 10;		// assume decimal base
		if (*p == '0')
		{	base = 8;	// could be octal
			p++;
			switch (*p)
			{   case 'x':
			    case 'X':
				base = 16;	// hex
				p++;
				break;
#if BINARY
			    case 'b':
			    case 'B':
				base = 2;	// binary
				p++;
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
	{	c = *p;
		if ('0' <= c && c <= '9')
			c -= '0';
		else if ('a' <= c && c <= 'z')
			c -= 'a' - 10;
		else if ('A' <= c && c <= 'Z')
			c -= 'A' - 10;
		else			// unrecognized character
			break;
		if (c >= base)		// not in number base
			break;
		if ((ULLONG_MAX - c) / base < number)
			error = 1;
		number = number * base + c;
		p++;
	}
	if (pend)
		*pend = (void *)p;
	if (error)
	{	number = ULLONG_MAX;
		__set_errno (ERANGE);
	}
	return number;
}
