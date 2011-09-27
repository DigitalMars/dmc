/*
 *  ostream.cpp -- class ostream member functions
 *
 * $Id: ostream.cpp,v 1.1.1.1 1997/01/02 19:18:04 smarx Exp $
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
 * $Log: ostream.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:04  smarx
 * cafe12
 *
   
      Rev 1.4   14 Jan 1995 17:11:58   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
      Rev 1.3   04 Jan 1995 14:21:00   Andy
   added operator= (ostream_withassign&) member to class ostream_withassign
   
      Rev 1.2   12 Oct 1994 16:40:14   BUSHNELL
   Put in include of typeinfo.h for -ER
   
      Rev 1.1   06 Jun 1994  8:52:54   bushnell
   Changed LF to CR/LF to view files in ARUBA/Brief
   

      Rev 1.0   20 Apr 1994 17:38:42   thompson                   

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

ostream::ostream()
{
}


ostream::ostream(streambuf* s)
{
    ios::init(s);
}


ostream::~ostream()
{
    flush();	// in case ostream::ostream(int fd) was called
}


// implementation of opfx -- output prefix operations
int ostream::do_opfx()
{
    if( ! fail() ) {
	if( x_tie )
	    x_tie->flush();
	return 1;
    }

    return 0;
}


// implementation of osfx -- output suffix operations
void ostream::do_osfx()
{
    if( ! fail()  && (x_flags & ios::unitbuf) )
	flush();
    
    if( x_flags & ios::stdio ) {
	cout.flush();
	clog.flush();
    }
}


// set the put pointer's position
ostream& ostream::seekp(streampos pos)
{
    if( bad()  ||  bp->seekpos(pos, ios::out) == EOF )
	setstate(ios::failbit);
    return *this;
}


// set the put pointer's position
ostream& ostream::seekp(streamoff off, ios::seek_dir dir)
{
    if( bad()  ||  bp->seekoff(off, dir, ios::out) == EOF )
	setstate(ios::failbit);
    else	// turn of eof and fail
	clear(rdstate() & ~(ios::eofbit | ios::failbit));
    return *this;
}


// read the put pointer's position
streampos ostream::tellp()
{
    streampos p = EOF;
    if( bad()  ||  (p = bp->seekoff(0, ios::cur, ios::out)) == EOF )
	setstate(ios::failbit);
    return p;
}


ostream& ostream::flush()
{
    if( bp->sync() == EOF )
	setstate(ios::badbit);
    return *this;
}


/*
 * Formatted insertion operations
 */

// Perform the prefix routine, output the string with any needed padding,
// and perform the suffix routine.
// 'd' is the data portion, 'p' is the prefix portion.
void ostream::outstr(const char *d, const char *p)
{
    if( opfx() ) {
	int plen = p ? strlen(p) : 0;
	int dlen = d ? strlen(d) : 0;
	int pad = width(0) - plen - dlen;

	// pad on left (right-adjust) if needed -- the default case
	if( ! (x_flags & (ios::left | ios::internal)) ) {
	    while( --pad >= 0 )
		if( bp->sputc(x_fill) == EOF ) {
		    setstate(ios::badbit);
		    break;
		}
	}

	// output the prefix
	if( ! fail()  &&  plen )
	    if( bp->sputn(p, plen) != plen )
		setstate(ios::badbit);

	// internal padding if needed
	if( ! fail()  &&  (x_flags & ios::internal) ) {
	    while( --pad >= 0 )
		if( bp->sputc(x_fill) == EOF ) {
		    setstate(ios::badbit);
		    break;
		}
	}

	// output the data
	if( ! fail()  &&  dlen )
	    if( bp->sputn(d, dlen) != dlen )
		setstate(ios::badbit);

	// pad on right (left-adjust) if needed
	if( ! fail()  &&  (x_flags & ios::left) ) {
	    while( --pad >= 0 )
		if( bp->sputc(x_fill) == EOF ) {
		    setstate(ios::badbit);
		    break;
		}
	}
    }
    osfx();
}


// extract from streambuf, insert into this ostream
ostream& ostream::operator<< (streambuf* s)
{
    if( opfx() ) {
	int c;
	while( (c = s->sbumpc()) != EOF )
	    if( bp->sputc(c) == EOF ) {
		setstate(ios::badbit);
		break;
	    }
    }
    osfx();
    return *this;
}


// manipulators
ostream& ostream::operator<< (ios& (*f)(ios&))
{
    (*f)(*((ios*)this));
    return *this;
}


// insert newline and flush
ostream& endl(ostream& os)
{
    os << '\n';
    os.flush();
    return os;
}


// insert null to terminate string
ostream& ends(ostream& os)
{
    os << char(0);
    return os;
}


// flush the ostream
ostream& flush(ostream& os)
{
    os.flush();
    return os;
}


/*
 * ostream with assign
 */

// does no initialization
ostream_withassign::ostream_withassign() :
		ostream()
{
}


ostream_withassign::~ostream_withassign()
{
}


// gets buffer from ostream and does entire initialization
ostream_withassign& ostream_withassign::operator= (ostream& os)
{
    ios::init(os.rdbuf());
    return *this;
}

// gets buffer from ostream and does entire initialization
ostream_withassign& ostream_withassign::operator= (ostream_withassign& os)
{
    return ostream_withassign::operator= ((ostream&)os);
}


// associates streambuf with stream and does entire initialization
ostream_withassign& ostream_withassign::operator= (streambuf* s)
{
    ios::init(s);
    return *this;
}
