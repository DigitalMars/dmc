/*
 *  streambf.cpp -- class streambuf member functions
 *
 * $Id: streambf.cpp,v 1.1.1.1 1997/01/02 19:18:05 smarx Exp $
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
 * $Log: streambf.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:05  smarx
 * cafe12
 *
   
      Rev 1.3   14 Jan 1995 17:12:00   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
      Rev 1.2   12 Oct 1994 16:40:14   BUSHNELL
   Put in include of typeinfo.h for -ER
   
      Rev 1.1   06 Jun 1994  8:52:58   bushnell
   Changed LF to CR/LF to view files in ARUBA/Brief
   

      Rev 1.0   20 Apr 1994 17:38:48   thompson                   

   Initial revision.

// Revision 1.2  1994/04/14  00:48:52  vriezen
// Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
//
 *
 * 
 */

#include <iostream.h>
#include <typeinfo.h> /* For RTTI */

static Iostream_init iostream_init;  // Causes CIN, CERR, etc. to be initialized

const int streambuf_size = 1024;

streambuf::streambuf()
{
    alloc_ = 0;
    unbuf_ = 0;
    base_ = 0;
    pbase_ = 0;
    pptr_ = 0;
    epptr_ = 0;
    gptr_ = 0;
    egptr_ = 0;
    eback_ = 0;
    ebuf_ = 0;
}


streambuf::streambuf(char* b, int len)
{
    alloc_ = 0;
    unbuf_ = 0;
    base_ = (char*)b;
    ebuf_ = (char*)((len > 0) ? b + len : b);
    pbase_ = 0;
    pptr_ = 0;
    epptr_ = 0;
    gptr_ = 0;
    egptr_ = 0;
    eback_ = 0;
}


streambuf::~streambuf()
{
    if( alloc_ )
	delete base_;
}


int streambuf::doallocate()
{
    char *p = new char[streambuf_size];
    if( ! p )
	return EOF;
    base_ = p;
    ebuf_ = p + streambuf_size;
    alloc_ = 1;
    return 1;
}


void streambuf::setb(char* b, char* eb, int a)
{
    if( alloc_  &&  base_  &&  base_ != b )
	delete base_;
    base_ = b;
    ebuf_ =  (( b && eb && (eb >= b) ) ? eb : b);
    unbuf_ = ! b;
    alloc_ = a && b;
}


void streambuf::setg(char* eb, char* g, char* eg)
{
    eback_ = ((g && eb) ? eb : g);
    gptr_ = g;
    egptr_ = ((g && eg && (eg >= g)) ? eg : g);
 }


void streambuf::setp(char* p, char* ep)
{
    pbase_ = pptr_ = p;
    epptr_ = ((p && ep && (ep >= p)) ? ep : p);
}


streambuf* streambuf::setbuf(char* p, int len)
{
    setb(p, p + len, 0);
    return this;
}


// called when sgetn finds not enough chars in the get area
int streambuf::xsgetn(char* s, int n)
{
    int c, i;

    i = in_avail();
    if( i > 0 ) {
	memcpy(s, gptr_, i);
	s += i;
	gbump(i);
    }
    while( i < n  &&  (c = sbumpc()) != EOF ) {
	*s++ = c;
	++i;
    }
    return i;
}


// called by sputn when more chars are needed than are in the buffer
int streambuf::xsputn(const char* s, int n)
{
    int i;

    i = epptr_ - pptr_;
    if( i > 0 ) {
	memcpy(pptr_, s, i);
	s += i;
	pbump(i);
    }
    while( i < n  &&  sputc(*s++) != EOF )
	++i;
    return i;
}


// called when snextc doesn't have an available character
int streambuf::do_snextc()
{
    return (underflow() == EOF) ? EOF : (unsigned char) *gptr_;
}


// streambuf can't do anything about underflow -- no place to get chars
int streambuf::underflow()
{
    if( egptr_ > gptr_ )
	return (unsigned char) *gptr_;

    gptr_ = egptr_;
    return EOF;
}


// streambuf can't do anything about overflow -- no place to put chars
int streambuf::overflow(int)
{
    return EOF;
}


int streambuf::pbackfail(int)
{
    return EOF;
}


streampos streambuf::seekoff(streamoff, ios::seek_dir, int)
{
    return EOF;
}


streampos streambuf::seekpos(streampos pos, int mode)
{
    return seekoff(streamoff(pos), ios::beg, mode);
}


int streambuf::sync() 
{
     return 0;	// OK, nothing to do
}
