/*
 *   istreami.cpp -- class istream member functions for extracting integral types
 *
 * $Id: istreami.cpp,v 1.1.1.1 1997/01/02 19:18:04 smarx Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 * Voice: (503) 754-3010	FAX: (503) 757-6650
 *
 * Copyright (C) 1991,  1993, 1994.
 * This software is subject to copyright protection under the laws of 
 * the United States and other countries.
 * ALL RIGHTS RESERVED
 *
 ***************************************************************************
 *
 * $Log: istreami.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:04  smarx
 * cafe12
 *
   
      Rev 1.4   08 May 1995 18:16:30   Andrew
   Fixed CR/LF problems that were giving Aruba Fits
   
      Rev 1.3   14 Jan 1995 17:12:00   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
      Rev 1.2   02 Nov 1994 12:08:38   BUSHNELL
   Ifdefed in Long long support for __INTSIZE==4
   
      Rev 1.1   06 Jun 1994  8:52:52   bushnell
   Changed LF to CR/LF to view files in ARUBA/Brief
   
      Rev 1.0   20 Apr 1994 17:38:40   thompson                   
   Initial revision.
// Revision 1.3  1994/04/14  16:49:43  vriezen
// Added conditional compilation for long long
//
// Revision 1.2  1994/04/14  00:48:52  vriezen
// Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
//
 *
 * 
 */

#include <stdio.h>
#include <ctype.h>
#include <iostream.h>

static Iostream_init iostream_init;  // Causes CIN, CERR, etc. to be initialized

// scan off chars into buffer, set stream state
// return scanf formatting character
char istream::intstr(char* buf, int isunsigned)
{
    char *p = buf;		// where to write next output char
    char formchar = 'i';	// scanf format specifier

    if( ipfx0() ) {		// verify file state and possibly skip white
	int formok = 0;		// number format, assumed invalid
	int c = 0;		// current input character
	int ishex = 0;		// hex number only
	int isoct = 0;		// octal number only
	int isdec = 0;		// decimal number only

	switch( flags() & ios::basefield ) {
	    case 0:		// determined by number format
		break;
	    case ios::oct:	// octal only
		isoct = 1;
		formchar = 'o';
		break;
	    case ios::hex:	// hex only
		ishex = 1;
		formchar = 'x';
		break;
	    default:	// just dec, or more than one bit set
		isdec = 1;
		formchar = isunsigned ? 'u' : 'd';
		break;
	}

	c = bp->sgetc();	// first character

	// get leading sign
	if( c == '+'  ||  c == '-' ) {
	    *p++ = c;
	    c = bp->snextc();
	}
	else if( c == EOF )
	    setstate(ios::badbit);	// no characters extracted

	// check for leading "0x"
	if( ! isdec && ! isoct && c == '0' ) {
	    formok = 1;		// we know we have a zero
	    *p++ = c;
	    c = bp->snextc();
	    if( c == 'x' || c == 'X' ) {
		if( ! ishex ) {	// bug in scanf: won't allow 0x for "%x" input
		    *p++ = c;
		    ishex = 1;
		}
		c = bp->snextc();
	    }
	}

	// scan off remaining digits
	if( isoct ) {
	    if( isdigit(c) && c < '8' ) {
		formok = 1;
		do {
		    *p++ = c;
		    c = bp->snextc();
		} while( isdigit(c) && c < '8' );
	    }
	}
	else if( ishex ) {
	    if( isxdigit(c) ) {
		formok = 1;
		do {
		    *p++ = c;
		    c = bp->snextc();
		} while( isxdigit(c) );
	    }
	}
	else {
	    if( isdigit(c) ) {
		formok = 1;
		do {
		    *p++ = c;
		    c = bp->snextc();
		} while( isdigit(c) );
	    }
	}

	if( ! formok )
	    setstate(ios::failbit);
	if( c == EOF )
	    setstate(ios::eofbit);
    }
    *p = 0;	// terminate string
    return formchar;
}


istream& istream::operator>> (short& i)
{
    char buf[512];
    char spec[] = "%hi";

    spec[2] = intstr(buf, 0);

    if( !fail() ) {
	if( sscanf(buf, spec, &i) != 1 )
	    setstate(ios::failbit);
    }
    return *this;
}


istream& istream::operator>> (unsigned short& u)
{
    char buf[512];
    char spec[] = "%hi";

    spec[2] = intstr(buf, 1);

    if( !fail() ) {
	if( sscanf(buf, spec, &u) != 1 )
	    setstate(ios::failbit);
    }
    return *this;
}


istream& istream::operator>> (int& i)
{
    char buf[512];
    char spec[] = "%i";

    spec[1] = intstr(buf, 0);

    if( !fail() ) {
	if( sscanf(buf, spec, &i) != 1 )
	    setstate(ios::failbit);
    }
    return *this;
}


istream& istream::operator>> (unsigned int& u)
{
    char buf[512];
    char spec[] = "%i";

    spec[1] = intstr(buf, 1);

    if( !fail() ) {
	if( sscanf(buf, spec, &u) != 1 )
	    setstate(ios::failbit);
    }
    return *this;
}


istream& istream::operator>> (long& l)
{
    char buf[512];
    char spec[] = "%li";

    spec[2] = intstr(buf, 0);

    if( !fail() ) {
	if( sscanf(buf, spec, &l) != 1 )
	    setstate(ios::failbit);
    }
    return *this;
}


istream& istream::operator>> (unsigned long& ul)
{
    char buf[512];
    char spec[] = "%li";

    spec[2] = intstr(buf, 1);

    if( !fail() ) {
	if( sscanf(buf, spec, &ul) != 1 )
	    setstate(ios::failbit);
    }
    return *this;
}

#if defined(RW_LONG_LONG) || __INTSIZE==4
istream& istream::operator>> (long long& ll)
{
    char buf[512];
    char spec[] = "%lli";

    spec[3] = intstr(buf, 0);

    if( !fail() ) {
	if( sscanf(buf, spec, &ll) != 1 )
	    setstate(ios::failbit);
    }
    return *this;
}


istream& istream::operator>> (unsigned long long& ull)
{
    char buf[512];
    char spec[] = "%lli";

    spec[3] = intstr(buf, 1);

    if( !fail() ) {
	if( sscanf(buf, spec, &ull) != 1 )
	    setstate(ios::failbit);
    }
    return *this;
}

#endif
