/*
 *  istream.cpp -- class istream member functions
 *
 * $Id: istream.cpp,v 1.1.1.1 1997/01/02 19:18:03 smarx Exp $
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
 * $Log: istream.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:03  smarx
 * cafe12
 *
   
      Rev 1.3   14 Jan 1995 17:11:58   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
      Rev 1.2   12 Oct 1994 16:40:14   BUSHNELL
   Put in include of typeinfo.h for -ER
   
      Rev 1.1   06 Jun 1994  8:52:52   bushnell
   Changed LF to CR/LF to view files in ARUBA/Brief
   

      Rev 1.0   20 Apr 1994 17:38:38   thompson                   

   Initial revision.

// Revision 1.2  1994/04/14  00:48:52  vriezen
// Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
//
 *
 * 
 */

#include <iostream.h>
#include <ctype.h>
#include <stdlib.h>
#include <typeinfo.h> /* for RTTI */

static Iostream_init iostream_init;  // Causes CIN, CERR, etc. to be initialized

istream::istream()
{
    gcount_ = 0;
}


istream::istream(streambuf* s)
{
    ios::init(s);
    gcount_ = 0;
}


istream::~istream()
{
}


// obsolete: set skip via format, tie via tie() function
istream::istream(streambuf* s, int sk, ostream* t)
{
    ios::init(s);
    gcount_ = 0;
    skip(sk);
    tie(t);
}


// set/read the get pointer's position
istream& istream::seekg(streampos pos)
{
    if( bad()  ||  bp->seekpos(pos, ios::in) == EOF )
	setstate(ios::failbit);
    return *this;
}


istream& istream::seekg(streamoff off, ios::seek_dir dir)
{
    if( bad()  ||  bp->seekoff(off, dir, ios::in) == EOF )
	setstate(ios::failbit);
    else	// turn of eof and fail
	clear(rdstate() & ~(ios::eofbit | ios::failbit));
    return *this;
}


streampos istream::tellg()
{
    streampos p = EOF;
    if( bad()  ||  (p = bp->seekoff(0, ios::cur, ios::in)) == EOF )
	setstate(ios::failbit);
    return p;
}


/*
* Unformatted extraction operations
*/

// extract consecutive whitespace
void istream::eatwhite()
{
    int c;
    gcount_ = 0;
    while( (c = bp->sgetc()), isspace(c) ) {
	bp->stossc();
	++gcount_;
    }

    if( c == EOF )
	setstate(ios::eofbit);
}


// implementation of ipfx
// flush tied stream if needed, skip whitespace if needed
int istream::do_ipfx(int need)
{
    gcount_ = 0;

    if( ! good() ) {
	setstate(ios::failbit);
	return 0;
    }

    // flush tied stream if needed
    if ( tie()  &&  (! need  ||  need > bp->in_avail()) )
	tie()->flush();

    if( ! need  &&  (ispecial & skipping) )
	eatwhite();

    if( ! good() ) {
	setstate(ios::failbit);
	return 0;
    }
    return 1;
}


// get next char, not immediately available
char istream::do_get()
{
    int c = bp->sbumpc();
    if( c == EOF )
	setstate(ios::eofbit | ios::failbit);
    return c;
}


// extract characters into an array, up to len chars or delimiter d
istream& istream::get(char* p, int len, char d)
{
    if( ipfx1() ) {
	char* op = p;
	int c = 0;
	gcount_ = 0;
	while( --len > 0  &&  (c = bp->sgetc()) != d  &&  c != EOF ) {
	    *p++ = c;
	    ++gcount_;
	    bp->stossc();
	}
	if( c == EOF )
	    setstate((p == op) ? (ios::eofbit | ios::failbit) : ios::eofbit);
    }
    *p = 0;	// always terminate with a null
    return *this;
}


istream& istream::read(char* p, int len)
{
    if( ipfx1() ) {
	int c = 0;
	gcount_ = 0;
	while( --len >= 0  &&  (c = bp->sgetc()) != EOF ) {
	    *p++ = c;
	    ++gcount_;
	    bp->stossc();
	}
	if( c == EOF )
	    setstate((len >= 0) ? (ios::eofbit | ios::failbit) : ios::eofbit);
    }
    return *this;
}


// extract characters into an array up to and including delimeter d
istream& istream::getline(char* p, int len, char d)
{
    if( ipfx1() ) {
	char* op = p;
	int c = 0;
	gcount_ = 0;
	while( --len > 0  &&  (c = bp->sgetc()) != EOF ) {
	    ++gcount_;		// do count the delimiter
	    bp->stossc();	// do discard the delimiter
	    if( c == d )
		break;		// do not store the delimiter
	    *p++ = c;
	}
	if( c == EOF )
	    setstate((p == op) ? (ios::eofbit | ios::failbit) : ios::eofbit);
    }
    *p = 0;	// always terminate with a null
    return *this;
}


// extract characters into a streambuf up to delimiter
istream& istream::get(streambuf& s, char d)
{
    if( ipfx1() ) {
	int c;
	int err = 0;
	gcount_ = 0;
	while( (c = bp->sgetc()) != d  &&  c != EOF ) {
	    if( s.sputc(c) == EOF ) {
		err = 1;
		break;
	    }
	    ++gcount_;
	    bp->stossc();
	}
	if( c == EOF )
	    setstate(ios::eofbit);
	if( err )
	    setstate(ios::failbit);
    }
    return *this;
}


// push back char into input
istream& istream::putback(char c)
{
    if( ! fail() )
	if( bp->sputbackc(c) == EOF )
	    setstate(ios::failbit);
	else
	    clear(rdstate() & ~ios::eofbit);
    return *this;
}


// extract and discard n chars, or through delimiter, whichever is first
istream& istream::ignore(int n, int d)
{
    if( ipfx1() ) {
	int c = 0;
	gcount_ = 0;
	while( --n >= 0  &&  (c = bp->sgetc()) != EOF ) {
	    ++gcount_;
	    bp->stossc();
	    if( c == d )
		break;	// we DO consume the delimiter
	}
	if( c == EOF )
	    setstate(ios::eofbit);
    }
    return *this;
}


// manipulators
istream& istream::operator>> (ios& (*f)(ios&) )
{
    (*f)(*((ios*)this));
    return *this;
}


// extract whitespace characters
istream& ws(istream& is) 
{
    long f = is.flags();
    is.setf(ios::skipws, ios::skipws);
    is.ipfx0();
    is.flags(f);
    return is;
}


/*
* Formatted extraction operations
*/

istream& istream::operator>> (char* p)
{
    if( ipfx0() ) {
	char* op = p;
	int c = 0;
	int len = width(0);	// <=0 means no limit
	while( --len  &&  (c = bp->sgetc(), ! isspace(c))  &&  c != EOF ) {
	    *p++ = c;
	    bp->stossc();
	}
	if( c == EOF )
	    setstate((p == op) ? (ios::eofbit | ios::failbit) : ios::eofbit);
	*p = 0;	// always terminate with a null
    }
    return *this;
}


// extract from this istream, insert into streambuf
istream& istream::operator>> (streambuf* s)
{
    enum errkind { noerr, nodata, putfail } err;

    if( ipfx0() ) {
	int c;
	err = nodata;
	while( (c = bp->sgetc()) != EOF ) {
	    if( s->sputc(c) == EOF ) {
		err = putfail;
		break;
	    }
	    err = noerr;
	    bp->stossc();
	}
	int errstate = ( err == noerr ) ? 0 : ios::failbit;
	if( c == EOF ) {
	    errstate |= ios::eofbit;
	    if( err == nodata )
		errstate |= ios::badbit;
	}
	if( errstate )
	    setstate(errstate);
    }
    return *this;
}


/*
 * istream with assign
 */

// does no initialization
istream_withassign::istream_withassign() :
		istream()
{
}


istream_withassign::~istream_withassign()
{
}


// gets buffer from istream and does entire initialization
istream_withassign& istream_withassign::operator= (istream& is)
{
    ios::init(is.rdbuf());
    return *this;
}


// associates streambuf with stream and does entire initialization
istream_withassign& istream_withassign::operator= (streambuf* s)
{
    ios::init(s);
    return *this;
}
