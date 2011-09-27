/*
 *   istreamf.cpp -- class istream member functions for extracting floating types
 *
 * $Id: istreamf.cpp,v 1.1.1.1 1997/01/02 19:18:04 smarx Exp $
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
 * $Log: istreamf.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:04  smarx
 * cafe12
 *
// * 
// * 6     5/01/96 10:26a Smarx
// * added long double support
   
      Rev 1.4   08 May 1995 18:16:28   Andrew
   Fixed CR/LF problems that were giving Aruba Fits
   
      Rev 1.3   14 Jan 1995 17:12:00   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
      Rev 1.2   02 Nov 1994 12:08:38   BUSHNELL
   Ifdefed in Long long support for __INTSIZE==4
   
      Rev 1.1   06 Jun 1994  8:52:52   bushnell
   Changed LF to CR/LF to view files in ARUBA/Brief
   
      Rev 1.0   20 Apr 1994 17:38:38   thompson                   
   Initial revision.
// Revision 1.3  1994/04/14  16:49:43  vriezen
// Added conditional compilation for long double
//
// Revision 1.2  1994/04/14  00:48:52  vriezen
// Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
//
 *
 * 
 */

#include <stdio.h>
#include <iostream.h>
#include <ctype.h>

static Iostream_init iostream_init;  // Causes CIN, CERR, etc. to be initialized

// scan off floating-point chars into buffer, set stream state
void istream::floatstr(char* buf)
{
    char *p = buf;		// where to write next output char

    if( ipfx0() ) {		// verify file state and possibly skip white
	int formok = 0;		// floating-point number format, assumed invalid
	int c = 0;		// current input character

	c = bp->sgetc();	// first character

	// get leading sign
	if( c == '+'  ||  c == '-' ) {
	    *p++ = c;
	    c = bp->snextc();
	}
	else if( c == EOF )
	    setstate(ios::badbit);	// no characters extracted

	// scan off digits up to decimal point
	if( isdigit(c) ) {
	    formok = 1;		// possible sign followed by digits
	    do {
		*p++ = c;
		c = bp->snextc();
	    } while( isdigit(c) );
	}

	// scan off fractional digits
	if( c == '.' ) {
	    *p++ = c;
	    c = bp->snextc();
	    if( isdigit(c) ) {
		formok = 1;	// leading dot OK if followed by digits
		do {
		    *p++ = c;
		    c = bp->snextc();
		} while( isdigit(c) );
	    }
	}

	// look for possibly-signed exponent
	if( formok  &&  (c == 'e'  ||  c == 'E') ) {
	    *p++ = c;
	    c = bp->snextc();
	    formok = isdigit(c)  ||  c == '+'  ||  c == '-';

	    if( formok ) {
		// convert possibly-signed exponent
		if( c == '+'  ||  c == '-' ) {
		    *p++ = c;
		    c = bp->snextc();
		}
		formok = isdigit(c);
		if( formok ) {
		    do {
			*p++ = c;
			c = bp->snextc();
		    } while( isdigit(c) );
		}
	    }
	}

	if( ! formok )
	    setstate(ios::failbit);
	if( c == EOF )
	    setstate(ios::eofbit);
    }
    *p = 0;	// terminate string
}


istream& istream::operator>> (float& f)
{
    char buf[2048];

    floatstr(buf);

    if( !fail() ) {
	if( sscanf(buf, "%f", &f) != 1 )
	    setstate(ios::failbit);
    }
    return *this;
}


istream& istream::operator>> (double& d)
{
    char buf[2048];

    floatstr(buf);

    if( !fail() ) {
	if( sscanf(buf, "%lf", &d) != 1 )
	    setstate(ios::failbit);
    }
    return *this;
}

istream& istream::operator>> (long double& ld)
{
    char buf[4096];

    floatstr(buf);

    if( !fail() ) {
	if( sscanf(buf, "%Lf", &ld) != 1 )
	    setstate(ios::failbit);
    }
    return *this;
}




