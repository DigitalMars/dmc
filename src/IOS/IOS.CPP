/*
 *  ios.cpp -- class ios member functions
 *
 * $Id: ios.cpp,v 1.1.1.1 1997/01/02 19:18:03 smarx Exp $
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
 * $Log: ios.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:03  smarx
 * cafe12
 *
   
      Rev 1.4   08 May 1995 15:21:16   Andrew
   Fixed debug info problem with ios_user_union
   
      Rev 1.3   14 Jan 1995 17:11:58   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
      Rev 1.2   12 Oct 1994 16:40:16   BUSHNELL
   Put in include of typeinfo.h for -ER
   
      Rev 1.1   06 Jun 1994  8:52:50   bushnell
   Changed LF to CR/LF to view files in ARUBA/Brief
   

      Rev 1.0   20 Apr 1994 17:38:36   thompson                   

   Initial revision.

// Revision 1.2  1994/04/14  00:48:52  vriezen
// Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
//
 *
 * 
 */

#include <iostream.h>
#include <typeinfo.h>  /* For RTTI */

static Iostream_init iostream_init;  // Causes CIN, CERR, etc. to be initialized


union ios_user_union {
    long lword;
    void *pword;
};

// static members of ios
const long ios::basefield = ios::dec | ios::oct | ios::hex;
const long ios::adjustfield = ios::left | ios::right | ios::internal;
const long ios::floatfield = ios::scientific | ios::fixed;
long ios::nextbit = 16;
int ios::usercount = 0;


ios::ios()
{
    userwords = 0;
}


ios::ios(streambuf* sb)
{
    userwords = 0;
    init(sb);
}


void ios::init(streambuf* sbp)
{
    nwords = 0;
    state = ospecial = 0;
    ispecial = skipping;
    x_flags = skipws|dec;
    x_precision = 6;
    x_width = 0;
    x_fill = ' ';
    x_tie = 0;
    bp = sbp;
}


ios::~ios()
{
    if( userwords )
	delete userwords;
}


ostream* ios::tie(ostream* s)
{
    ostream *x = x_tie;
    // documentation doesn't state whether to flush old tied stream
    if( x ) x->flush();
    x_tie = s;
    if( s ) {
	ispecial |= tied;
	ospecial |= tied;
    }
    else {
	ispecial &= ~tied;
	ospecial &= ~tied;
    }
    return x;
}


long ios::flags(long l)
{
    long x = x_flags;
    x_flags = l;
    if( l & ios::skipws )
	ispecial |= skipping;
    else
	ispecial &= ~skipping;
    return x;
}


long ios::setf(long _setbits, long _field)
{
    long x = x_flags;
    x_flags &= ~_field;
    x_flags |= (_setbits & _field);
    if( x_flags & ios::skipws )
	ispecial |= skipping;
    else
	ispecial &= ~skipping;
    return x;
}


long ios::setf(long l)
{
    long x = x_flags;
    x_flags |= l;
    if( x_flags & ios::skipws )
	ispecial |= skipping;
    else
	ispecial &= ~skipping;
    return x;
}


long ios::unsetf(long l)
{
    long x = x_flags;
    x_flags &= ~l;
    if( x_flags & ios::skipws )
	ispecial |= skipping;
    else
	ispecial &= ~skipping;
    return x;
}


ios& dec(ios& s)
{
    s.setf(ios::dec, ios::basefield);
    return s;
}


ios& hex(ios& s)
{
    s.setf(ios::hex, ios::basefield);
    return s;
}


ios& oct(ios& s)
{
    s.setf(ios::oct, ios::basefield);
    return s;
}


void ios::clear(int i)
{
    state = (i & 0xFF) | (state & hardfail);
    ispecial = (ispecial & ~0xFF) | state;
    ospecial = (ospecial & ~0xFF) | state;
}


void ios::setstate(int b)
{
    state |= (b & 0xFF);
    ispecial |= (b & ~(skipping | tied));
    ospecial |= (b & ~tied);
}


int ios::skip(int i)
{
    int x = (ispecial & skipping);
    if( i ) {
	ispecial |= skipping;
	x_flags |= skipws;
    }
    else {
	ispecial &= ~skipping;
	x_flags &= ~skipws;
    }
    return x != 0;
}


// supply a new flags bit
long ios::bitalloc()
{
    if( nextbit >= 31 )
	return 0;
    return 1 << ++nextbit;
}


// supply a new word index for derived class use
int ios::xalloc()
{
    return ++usercount;
}


// allocate or resize user array -- we assume i > nwords
void ios::usersize(int i)
{
    // allocate space for new array
    ios_user_union *p = new ios_user_union[i];

    if( ! p )
	return;		// cannot enlarge array

    if( nwords ) {	// copy old array to new one
	memcpy(p, userwords, nwords * sizeof(ios_user_union));
	delete userwords;
    }
    userwords = p;
    do {		// set new words to zero
	p[nwords].lword = 0;
    } while( ++nwords < i );
}


// return user word as an integer
long& ios::iword(int i)
{
    static long l = 0;		// for error return
    if( i < 1  ||  usercount < i )
	return l;		// no such entry
    if( i > nwords )
	usersize(i);
    return (i > nwords) ? l : ((union ios_user_union *)userwords)[i-1].lword;
}


// return user word as a void pointer
void*& ios::pword(int i)
{
    static void* p = 0;		// for error return
    if( i < 1  ||  usercount < i )
	return p;		// no such entry
    if( i > nwords )
	usersize(i);
    return (i > nwords) ? p : ((union ios_user_union *)userwords)[i-1].pword;
}
