/*
 *  strstream.cpp -- class strstrea member functions
 *
 * $Id: strstrea.cpp,v 1.1.1.1 1997/01/02 19:18:05 smarx Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 * Voice: (503) 754-3010        FAX: (503) 757-6650
 *
 * Copyright (C) 1991,  1993, 1994.
 * This software is subject to copyright protection under the laws of 
 * the United States and other countries.
 * ALL RIGHTS RESERVED
 *
 ***************************************************************************
 *
 * $Log: strstrea.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:05  smarx
 * cafe12
 *
   
      Rev 1.5   14 Jan 1995 17:11:58   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
      Rev 1.4   12 Oct 1994 16:40:14   BUSHNELL
   Put in include of typeinfo.h for -ER
   
      Rev 1.3   29 Sep 1994 23:04:12   DYNIN
   Enforced MS compatibility by non-discarding initial buffer contents.
   
      Rev 1.2   23 Jun 1994 15:56:32   BUSHNELL
   changed includes to only have 8 characters so that RTL will build under NT
   
      Rev 1.1   06 Jun 1994  8:52:58   bushnell
   Changed LF to CR/LF to view files in ARUBA/Brief
   
      Rev 1.0   20 Apr 1994 17:38:50   thompson                   
   Initial revision.
// Revision 1.1  1994/04/14  16:59:01  vriezen
// Initial revision
//
// Revision 1.2  1994/04/14  00:48:52  vriezen
// Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
//
 *
 * 
 */

#include <strstrea.h>
#include <limits.h>
#include <typeinfo.h>  /* for RTTI */

static Iostream_init iostream_init;  // Causes CIN, CERR, etc. to be initialized


// minimum string size
const int min_str_alloc = 16;

// minimum string increment when extending
const int min_str_incr = 8;

// to improve legibility
#define expandable ((ssbflags&(dynamic|frozen))==dynamic)


strstreambuf::strstreambuf()
{
    allocf = 0;
    freef = 0;
    next_alloc = 0;
    real_end = ebuf();
    ssbflags = dynamic;
}


strstreambuf::strstreambuf(void* (*a)(long), void (*f)(void*))
{
    allocf = a;
    freef = f;
    next_alloc = 0;
    real_end = ebuf();
    ssbflags = dynamic;
}


strstreambuf::strstreambuf(int n)
{
    if( n < min_str_alloc )
	n = min_str_alloc;
    real_end = 0;
    char *p = new char[n];
    if( p ) {
	setb(p, p, 0);
	real_end = p + n;
	*p = 0;
    }
    allocf = 0;
    freef = 0;
    next_alloc = 0;
    ssbflags = dynamic;
}


/*
    If n is negative, the provided buffer is supposed to be unlimited.
    As a practical way to handle this, the buffer is arbitrarily defined
    to be size INT_MAX.  (It really should be the max value supported by
    type size_t, but AT&T defined all sizes to be int.)
*/
void strstreambuf::init(char* ptr, int n, char* pstart)
{
    ssbflags = 0;
    if( n == 0 ) n = strlen(ptr);
    else if( n < 0 ) {
	n = INT_MAX;
	ssbflags |= unlimited;  // distinguish it from merely large
    }
    setb(ptr, ptr+n, 0);
    if( pstart ) {
	setp(pstart, ebuf());
	int pstart_sz = strlen(pstart);
	int ninit = (pstart_sz <= n) ? pstart_sz : n;
	setg(ptr, ptr, pstart+ninit);
    }
    else
	setg(ptr, ptr, ebuf());

    real_end = ebuf();
    allocf = 0;
    freef = 0;
}


strstreambuf::strstreambuf(char* ptr, int n, char* pstart)
{
    init(ptr, n, pstart);
}


strstreambuf::~strstreambuf()
{
    if( expandable )
	if( freef )
	    (*freef)((void*)base());
	else
	    delete base();
}


void strstreambuf::freeze(int n)
{
    if( ssbflags & dynamic )
	if( n )
	    ssbflags |= frozen;
	else
	    ssbflags &= ~frozen;
}


char* strstreambuf::str()
{
    freeze(1);
    return base();
}


int strstreambuf::doallocate()
{
    int size = next_alloc ? next_alloc : min_str_alloc;
    char *p;
    if( allocf )
	p = (char*)(*allocf)((long)size);
    else
	p = new char[size];
    if( ! p )
	return EOF;

    next_alloc = 0;     // assume next_alloc increment is one time only
    setb(p, p + blen(), 0);
    real_end = p + size;
    *p = 0;
    return 1;
}


int strstreambuf::overflow(int c)
{
    if( expandable ) {
	// allocate put area as needed
	if( ! base() )
	    doallocate();
	if( ! pbase() )
	    setp(base(), ebuf());
    }
    if( pbase() ) {
	if( (pptr() >= epptr())  &&  expandable ) {
	    // expand the reserve area
	    char *p = base();                   // original buf
	    char *oldp = p;
	    char *freeptr = oldp;
	    int len = blen();                   // length of old buf
	    int get_area = (gptr() != 0);       // do we have a get area
	    int ebloc, gloc, egloc;             // get area pointer data

	    // compute expansion increment
	    int incr = next_alloc ? next_alloc : min_str_alloc;
	    next_alloc = 0;     // assume next_alloc increment is one time only

	    if( pptr() >= real_end ) {
		// expand the real space - make a new buf
		if( allocf )
		   p = (char*)(*allocf)((long)len + incr);
		else
		   p = new char[len + incr];
		if( ! p )
		    return EOF;                 // out of memory

		real_end = p + len + incr;
		memcpy(p, oldp, len);           // copy data to new buf
		// get relative pointer data 
		if( get_area ) {
		    ebloc = eback() - oldp;     // eback location
		    gloc = gptr() - oldp;       // gptr location
		    egloc = egptr() - oldp;     // egptr location
		}
		if( freef )
		    (*freef)((void*)freeptr);
		else
		    delete [] freeptr;
	    }
	    // set up new pointers
	    int ploc = pptr() - oldp;   // original offset of pptr
	    setb(p, p + len + 1, 0);
	    setp(p, ebuf());            // the put area goes to end of buf
	    pbump(ploc);                // set pptr
	    if( p != oldp && get_area ) // adjust get area
		setg(p + ebloc, p + gloc, p + egloc);
	}

	if( pptr() < epptr() )
	    return sputc(c);            // now there is room in the put area
    }

    return EOF;         // could not make more space
}


int strstreambuf::underflow()
{
    if( egptr() > gptr() )
	return (unsigned char) *gptr();

    if( base()  &&  ! unbuffered()  &&  pptr() > pbase() ) {
	if( ! gptr()  ||  gptr() < pbase() ) {
	    // initialize get area to current put area
	    setg(base(), pbase(), pptr());
	    return (unsigned char) *gptr();
	}
	if( gptr() < pptr() ) {
	    // extend get area to match put area
	    setg(eback(), gptr(), pptr());
	    return (unsigned char) *gptr();
	}
	// else no more chars
    }

    return EOF;
}


streambuf* strstreambuf::setbuf(char* p, int n)
{
    if( p )
	return 0;       // can't honor this kind of request

    if( n > min_str_incr )
	next_alloc = n; // next alloc will be this many
    return this;
}


// we assume streampos and streamoff are type int or long
streampos strstreambuf::seekoff(streamoff off, ios::seek_dir dir, int omode)
{
    streamoff pos = EOF;        // result

    // find position if from beginning or end
    if( dir == ios::end ) {
	if( (ssbflags & unlimited) || off > 0 )
	    pos = -1;   // can't do it
	else
	    pos = blen() + off;
    }
    else if( dir == ios::beg )
	pos = off;

    if( omode & ios::in ) {
	// set up get area
	if( dir == ios::cur ) {
	    // find a reasonable value for current position
	    if( gptr() ) pos = streamoff(gptr() - base()) + off;
	    else if( pptr() ) pos = streamoff(pptr() - base()) + off;
	    else pos = off;
	}
	if( pos < 0  ||  blen() < pos )
	    pos = EOF;  // illegal request
	else {
	    char* g = base() + pos;
	    char* eb = (eback() && eback() < g) ? eback() : g;
	    char* eg = (egptr() && g < egptr()) ? egptr() : g;
	    setg(eb, g, eg);
	}
    }
    if( omode & ios::out ) {
	// set up put area
	if( dir == ios::cur ) {
	    // find a reasonable value for current position
	    if( pptr() ) pos = streamoff(pptr() - base()) + off;
	    else if( gptr() ) pos = streamoff(gptr() - base()) + off;
	    else pos = off;
	}
	if( pos < 0  ||  blen() < pos )
	    pos = EOF;  // illegal request
	else {
	    char* p = base() + pos;
	    char* ep = (epptr() && p < epptr()) ? epptr() : ebuf();
	    setp(p, ep);
	}
    }

    return streampos(pos);
}



strstreambase::strstreambase(char* ptr, int n, char* pstart) :
		buf(ptr, n, pstart)
{
    ios::init(&this->buf);
}


strstreambase::strstreambase() :
		buf()
{
    ios::init(&this->buf);
}


strstreambase::~strstreambase()
{
}



istrstream::istrstream(char* str) :
		strstreambase(str, 0, 0),
		istream(strstreambase::rdbuf())
{
}


istrstream::istrstream(char* str, int len) :
		strstreambase(str, len, 0),
		istream(strstreambase::rdbuf())
{
}


istrstream::~istrstream()
{
}



ostrstream::ostrstream(char* str, int len, int omode) :
		strstreambase(str, len, str),
		ostream(strstreambase::rdbuf())
{
    if( omode & (ios::ate | ios::app) )
	strstreambase::rdbuf()->seekoff(strlen(str), ios::beg, ios::out);
}


ostrstream::ostrstream() :
		strstreambase(),
		ostream(strstreambase::rdbuf())
{
}


ostrstream::~ostrstream()
{
}



strstream::strstream() :
		strstreambase(),
		iostream(strstreambase::rdbuf())
{
}


strstream::strstream(char* str, int size, int omode) :
		strstreambase(str, size, str),
		iostream(strstreambase::rdbuf())
{
    if( omode & (ios::ate | ios::app) )
	strstreambase::rdbuf()->seekoff(strlen(str), ios::beg, ios::out);
}


strstream::~strstream()
{
}
