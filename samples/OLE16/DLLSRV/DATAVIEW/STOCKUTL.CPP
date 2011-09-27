/*
	stockutl.cpp

	Miscellaneous utility functions for the Simulated Stock Watcher OLE2 sample
	program.

	(c) Copyright Microsoft Corp. 1992 - 1993 All Rights Reserved
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stockutl.h"

/*
 * Str2Stock-- convert a string in the form of int [int/[248]] into an integer
 * representing the number of 1/8's in stock price.  Return -1 on error.
 * This function does not modify s
 */
/*
 * BUGS:  strings of the form "/[248]xxxx" are allowed although they shouldn't be.
		  a forward scan may be faster/more intuitive
 */
long Str2Stock(LPSTR s)
{
	LPSTR a, b;
	char szBuf[128];
	long r, mult = 0;
	if( !s )
		return -1;

	_fstrcpy((LPSTR)szBuf, s);

	//only valid characters are [0-9] and /

	while ( *s )
		if( !(isdigit(*s) || isspace(*s) || *s == '/') )
			return -1;
		else
			s++;

	s = (LPSTR)szBuf;

	if( !(a = _fstrchr(s, '/')) )
		return atoi(s)*8;
	if( *(a+1) == '8' || *(a+1) == '4' || *(a+1) == '2')
		if( (b = _fstrrchr(s, ' ')) ){
			*a = '\0';
			r = atoi(b) * ((*(a+1)=='8')? 1 : (*(a+1)=='4') ? 2 : 4);
			*b = '\0';
			r += atoi(s)*8; //assuming atoi returns 0 with non-integral string
			return r;
		}
	return -1;
}

/*
 * Stock2Str -- convert an integer (representing 1/8ths of dollars) into a string
 * representation.  The string is *not* allocated.
 */

LPSTR Stock2Str( long n )
{
	long a = n%8l;
	static char szBuf[32];

	if( a )
		wsprintf((LPSTR)szBuf, (LPSTR)"%ld %ld/%ld",n/8l, (a == 4l)? 1l : (a%2l) ? a : a/2l,
			 (a == 4l)? 2l : (a%2l) ? 8l : 4l );
	else
		wsprintf((LPSTR)szBuf, (LPSTR)"%ld", n/8l);

	return (LPSTR)szBuf;
}
