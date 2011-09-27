/*
 *   ostreami.cpp -- class ostream member functions for integer insertion
 *
 * $Id: ostreami.cpp,v 1.1.1.1 1997/01/02 19:18:04 smarx Exp $
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
 * $Log: ostreami.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:04  smarx
 * cafe12
 *
   
      Rev 1.4   08 May 1995 18:16:24   Andrew
   Fixed CR/LF problems that were giving Aruba Fits
   
      Rev 1.3   14 Jan 1995 17:11:58   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
      Rev 1.2   02 Nov 1994 12:08:38   BUSHNELL
   Ifdefed in Long long support for __INTSIZE==4
   
      Rev 1.1   06 Jun 1994  8:52:56   bushnell
   Changed LF to CR/LF to view files in ARUBA/Brief
   
      Rev 1.0   20 Apr 1994 17:38:44   thompson                   
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
#include <iostream.h>

static Iostream_init iostream_init;  // Causes CIN, CERR, etc. to be initialized

// generate format string for some kind of integer
static void intformat(ostream& os, char *fmt, int isunsigned, int islonglong)
{
    fmt[0] = '%';
    int end = 0;	// end of fmt string

    if( os.flags() & ios::showbase )
	fmt[++end] = '#';
    if( os.flags() & ios::showpos )
	fmt[++end] = '+';

    fmt[++end] = 'l';
    if( islonglong ) fmt[++end] = 'l';

    switch( os.flags() & ios::basefield ) {
    case ios::hex:
	fmt[++end] = (os.flags() & ios::uppercase) ? 'X' : 'x';
	break;
    case ios::oct:
	fmt[++end] = 'o';
	break;
    default:	// no bits, more than one bit, or dec alone set
	fmt[++end] = isunsigned ? 'u' : 'd';
	break;
    }
    fmt[++end] = 0;
}

// final formatting and output of string
void ostream::printint(const char* buf)
{
    const char *prefix = 0;	// displayed numeric prefix string
    const char *rest = buf;	// remainder after prefix

    // pick off prefix, if any
    switch( buf[0] ) {
    case '+':
	prefix = "+";
	rest++;
	break;
    case '-':
	prefix = "-";
	rest++;
	break;
    case '0':
	switch( buf[1] ) {
	case '0':
	    rest++;	// no extra leading zero if octal zero
	case 'x':
	    prefix = "0x";
	    rest += 2;
	    break;
	case 'X':
	    prefix = "0X";
	    rest += 2;
	    break;
	}
	break;
    }

    // now we have a formatted string for output, to be possibly padded
    outstr(rest, prefix);
}


// format and insert a signed long
ostream& ostream::operator<< (long l)
{
    char fmt[16];	// big enough for format string
    char buf[32];	// big enough for long

    intformat(*this, fmt, 0, 0);
    sprintf(buf, fmt, l);
    printint(buf);
    return *this;
}


// format and insert an unsigned long
ostream& ostream::operator<< (unsigned long ul)
{
    char fmt[16];	// big enough for format string
    char buf[32];	// big enough for long

    intformat(*this, fmt, 1, 0);
    sprintf(buf, fmt, ul);
    printint(buf);
    return *this;
}

#if defined(RW_LONG_LONG) || __INTSIZE==4
// format and insert a signed long long
ostream& ostream::operator<< (long long ll)
{
    char fmt[16];	// big enough for format string
    char buf[64];	// big enough for long long

    intformat(*this, fmt, 0, 1);
    sprintf(buf, fmt, ll);
    printint(buf);
    return *this;
}


// format and insert an unsigned long long
ostream& ostream::operator<< (unsigned long long ull)
{
    char fmt[16];	// big enough for format string
    char buf[64];	// big enough for long long

    intformat(*this, fmt, 1, 1);
    sprintf(buf, fmt, ull);
    printint(buf);
    return *this;
}

#endif

// insert character representation of the value of the pointer
ostream& ostream::operator<< (void* p)
{
    long f = flags();
    setf((ios::hex | ios::showbase), (ios::basefield | ios::showbase));
    unsetf(ios::internal);
    *this << (unsigned long)p;
    flags(f);
    return *this;
}
