/*
 *  stdiostream.cpp -- class stdiobuf and stdiostream implementation
 *
 * $Id: stdiostr.cpp,v 1.1.1.1 1997/01/02 19:18:05 smarx Exp $
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
 * $Log: stdiostr.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:05  smarx
 * cafe12
 *
   
      Rev 1.4   14 Jan 1995 17:11:58   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
      Rev 1.3   12 Oct 1994 16:40:16   BUSHNELL
   Put in include of typeinfo.h for -ER
   
      Rev 1.2   23 Jun 1994 15:56:34   BUSHNELL
   changed includes to only have 8 characters so that RTL will build under NT
   
      Rev 1.1   06 Jun 1994  8:52:56   bushnell
   Changed LF to CR/LF to view files in ARUBA/Brief
   

      Rev 1.0   20 Apr 1994 17:38:48   thompson                   

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

#include "filesys.h"
#include <stdiostr.h>
#include <typeinfo.h> /* For RTTI */

static Iostream_init iostream_init;  // Causes CIN, CERR, etc. to be initialized

stdiobuf::stdiobuf(FILE* f)
{
    sio = f;
    char  *p = new char [B_size];
    if( p ) {
	setb(p, p+B_size+4, 1);	// ~streambuf() will delete it
	setp(p+4, p+4);
	setg(p, p+4, p+4);
    }
}


stdiobuf::~stdiobuf()
{
    overflow(EOF);
}


int	stdiobuf::overflow(int c)
{
    if ( ferror(sio) )
	return EOF;

    char *p;
    int count = out_waiting();
    if( count > 0 ) {
	p = pbase();
	do {
	    putc(*p, sio);
	    ++p;
	} while( --count );
    }
    if( c != EOF )
	putc(c, sio);

    char *b = base();
    setp(b+4, b+blen());
    setg(b, b+4, b+4);

    return ferror(sio) ? EOF : 1;
}


int	stdiobuf::pbackfail(int c)
{
    return ungetc(c, sio);
}


int	stdiobuf::sync()
{
    int count;
    if( out_waiting() ) {
	if( overflow(EOF) == EOF )
	    return EOF;
    }
    else if( (count = in_avail()) > 0 ) {
	char *p = egptr();
	do {
	    --p;
	    ungetc(*p, sio);
	    --count;
	} while( count );
	p = egptr();
	setp(p, p);
	setg(eback(), p, p);
    }
    /*
     * We assume that fseek() does any needed synchronization.
     */
    fseek(sio, 0L, SEEK_CUR);	// should flush stdio buffer
    return ferror(sio) ? EOF : 0;
}


streampos stdiobuf::seekoff(streamoff off,
			    ios::seek_dir dir,
			    int /*mode ignored*/)
{
    if( out_waiting()  ||  in_avail() ) {
	if( sync() == EOF )
	    return EOF;
    }

    int w;
    if( dir == ios::beg ) w = SEEK_SET;
    else if( dir == ios::end ) w = SEEK_END;
    else w = SEEK_CUR;
    w = fseek(sio, off, w);
    char *b = base();
    if( ! unbuffered()  &&  b ) {	// reset get and put areas
	setp(b+4, b+4);
	setg(b, b+4, b+4);
    }
    return w ? EOF : ftell(sio);
}


int	stdiobuf::underflow()
{
    if( in_avail() )	// no action needed
	return (unsigned char) *gptr();

    // read a character, the return value
    int c = getc(sio);

    if ( ! unbuffered()  &&  base() ) {		// this is buffered
	// flush any output buffer
	if( out_waiting() )
	    if( overflow(EOF) == EOF )
		return EOF;

	// set up get and put areas
	char *b = base();
	setg(b, b+4, b+5);	// room for the char we read
	setp(b+4, b+4);

	// put the character in the buffer
	*(gptr()) = c;
    }
    else {		// this is not buffered
	if( c == EOF )
	    setg(0, 0, 0);
	else {
	    lahead[0] = c;
	    setg(lahead, lahead, lahead+1);
	}
    }

    return c;
}


stdiostream::stdiostream(FILE* f) : buf(f)
{
    ios::init(&buf);
}


stdiostream::~stdiostream()
{
}
