/*
 *   ostreamf.cpp -- class ostream member functions for floating-point insertion
 *
 * $Id: ostreamf.cpp,v 1.1.1.1 1997/01/02 19:18:04 smarx Exp $
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
 * $Log: ostreamf.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:04  smarx
 * cafe12
 *
// * 
// * 6     5/01/96 10:26a Smarx
// * added long double support
   
      Rev 1.4   08 May 1995 18:16:24   Andrew
   Fixed CR/LF problems that were giving Aruba Fits
   
      Rev 1.3   14 Jan 1995 17:12:00   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
      Rev 1.2   02 Nov 1994 12:08:38   BUSHNELL
   Ifdefed in Long long support for __INTSIZE==4
   
      Rev 1.1   06 Jun 1994  8:52:54   bushnell
   Changed LF to CR/LF to view files in ARUBA/Brief
   
      Rev 1.0   20 Apr 1994 17:38:44   thompson                   
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

static Iostream_init iostream_init;  // Causes CIN, CERR, etc. to be initialized


static void fmtdbl(ostream& os, char *spec, int is_long_double)
{
    spec[0] = '%';	// initialize to "%"
    int end = 0;

    // qualifiers
    if( os.flags() & ios::showpoint )
	spec[++end] = '#';
    if( os.flags() & ios::showpos )
	spec[++end] = '+';

    int fixed = (os.flags() & ios::floatfield) == ios::fixed;
    int upcase = (os.flags() & ios::uppercase) != 0;

    // precision spec
    if( fixed || (os.precision() > 0) ) {
	sprintf(spec+end+1, ".%d", os.precision());
	end = strlen(spec) - 1;
    }

    // main conversion spec
    if( is_long_double ) spec[++end] = 'L';
    if( fixed )
	spec[++end] = 'f';
    else if( (os.flags() & ios::floatfield) == ios::scientific )
	spec[++end] = (upcase ? 'E' : 'e');
    else
	spec[++end] = (upcase ? 'G' : 'g');
    spec[end+1] = 0;
}


ostream& ostream::operator<< (double d)
{
    char buf[512];	// big enough for any floating result
    char spec[32];	// printf format string

    fmtdbl(*this, spec, 0);

    // convert the value
    sprintf(buf, spec, d);

    // internal padding goes between leading sign and rest of number
    char *prefix = 0;
    char *rest = buf;
    if( buf[0] == '-' ) {
	prefix = "-";
	rest++;
    }
    else if( buf[0] == '+' ) {
	prefix = "+";
	rest++;
    }
    outstr(rest, prefix);
    return *this;
}

ostream& ostream::operator<< (long double d)
{
    char buf[4096];	// big enough for any floating result
    char spec[32];	// printf format string

    fmtdbl(*this, spec, 1);

    // convert the value
    sprintf(buf, spec, d);

    // internal padding goes between leading sign and rest of number
    char *prefix = 0;
    char *rest = buf;
    if( buf[0] == '-' ) {
	prefix = "-";
	rest++;
    }
    else if( buf[0] == '+' ) {
	prefix = "+";
	rest++;
    }
    outstr(rest, prefix);
    return *this;
}

