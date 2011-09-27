/*
 *  fstream.cpp -- class filebuf and fstream member functions
 *
 * $Id: fstream.cpp,v 1.1.1.1 1997/01/02 19:18:03 smarx Exp $
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
 * $Log: fstream.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:03  smarx
 * cafe12
 *
   
      Rev 1.9   16 May 1995 12:36:00   Andrew
   Fixed bug in filebuf::open where default sharing mode was not _SH_DENYNONE for Win32 builds
   
      Rev 1.8   20 Jan 1995 11:39:20   Andy
   Fixed defaults for ifstream and ofstream to always be sure ios::in and ios::out are set in mode
   
      Rev 1.7   14 Jan 1995 17:12:00   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
      Rev 1.6   15 Dec 1994 13:44:54   Welden
   o Changed to use sopen, and to interpret the third filebuf (& therefore
     i/o fstream opens) as a sharing parameter, not the write protect-after-
     close bits from UNIX).
   
      Rev 1.5   09 Dec 1994 16:10:26   Welden
   o Bug #12465 -- tellg was changing the file pointer inappropriately.
   
      Rev 1.4   12 Oct 1994 16:40:16   BUSHNELL
   Put in include of typeinfo.h for -ER
   
      Rev 1.3   05 Oct 1994 13:34:06   anderson
   Added support for ios::binary
   
      Rev 1.2   05 Oct 1994 11:53:06   anderson
   Added setmode memberst to filebuf and fstreambase.
   
      Rev 1.1   06 Jun 1994  8:52:48   bushnell
   Changed LF to CR/LF to view files in ARUBA/Brief
   

      Rev 1.0   20 Apr 1994 17:38:34   thompson                   

   Initial revision.

// Revision 1.3  1994/04/14  17:38:39  vriezen
// Eliminated extraneous warning due to if ( (count = in_avail() ) construct
//
// Revision 1.2  1994/04/14  00:48:52  vriezen
// Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
//
 *
 * 
 */

#include "filesys.h"
#include <fcntl.h>
#include <fstream.h>
#include <typeinfo.h>  /* for RTTI */
#include <share.h>
#include <sys\stat.h>

static Iostream_init iostream_init;  // Causes CIN, CERR, etc. to be initialized

extern "C" int setmode (int fd, int mode);

const int filebuf::openprot = P_default;
const int filebuf::binary = _O_BINARY;
const int filebuf::text = _O_TEXT;

#define SHARE_BIT 0x80
#define WRITE_BIT 0x40
#define READ_BIT  0x20

const int filebuf::sh_compat = READ_BIT | WRITE_BIT;
const int filebuf::sh_none =   SHARE_BIT;
const int filebuf::sh_read =   SHARE_BIT | READ_BIT;
const int filebuf::sh_write =  SHARE_BIT | WRITE_BIT;

// make a closed filebuf
filebuf::filebuf()
{
    xfd = EOF;
    mode = 0;
    opened = 0;
    last_seek = 0;
    char* p = new char[B_size];
    if( p ) {
	setb(p, p+B_size, 1);	// ~streambuf() will delete buffer
	setp(p+4, p+4);
	setg(p, p+4, p+4);
    }
}


// make a filebuf attached to an open fd -- assume fd is actually open
filebuf::filebuf(int f)
{
    xfd = f;		// assumed to be valid
    opened = 1;		// unless we can find out otherwise
    mode = 0;		// unless we can find out otherwise
    last_seek = 0;
    char* p = new char[B_size];
    if( p ) {
	setb(p, p+B_size, 1);	// ~streambuf() will delete buffer
	setp(p+4, p+4);
	setg(p, p+4, p+4);
    }
}


// make a filebuf attached to an open fd -- assume fd is actually open
// use buf as the buffer area
filebuf::filebuf(int f, char* buf, int len)
{
    xfd = f;		// assumed to be valid
    opened = 1;		// unless we can find out otherwise
    mode = 0;		// unless we can find out otherwise
    last_seek = 0;
    setbuf(buf, len);
}


// We assume that mode==0 means that we attached to an already-open file,
// and should not now close it.  We do flush it in any case.
filebuf::~filebuf()
{
    if( mode )
	close();
    else
	overflow(EOF);
}


/*
 * Open named file with mode and protection, attach to this filebuf.
 * We make no assumptions about the mode bits and what ::open() wants to
 * see, except we assume that some mapping is possible.
 * "prot" is supplied by user, assumed consistent with operating system.
 * Note checks against OS_err, assumed to be operating system error return.
 */
filebuf* filebuf::open(const char* name, int m, int prot)
{
    if( opened  || ! m )
	return 0;

    // set up "how" parameter to system ::sopen()
    // we let the OS decide whether the combination is legal
    int how;
    if( m & ios::out ) {
	if( m & ios::in ) how = O_rdwr;
	else how = O_wronly;
	if( ! (m & ios::nocreate) ) {
	    how |= O_create;
	    if( m & ios::noreplace ) how |= O_excl;
	}
	if( m & ios::trunc ) how |= O_trunc;
    }
    else if( m & ios::in )
	how = O_rdonly;
    else
	return 0;	// must specfify in, out, or in/out
    if( m & ios::app ) how |= O_append;
	if (m & ios::binary) how |= O_binary;
    else how |= O_text;
    
    // set up "share" parameter to system ::sopen()
    int share;
    switch (prot & (SHARE_BIT | READ_BIT | WRITE_BIT))
    {
    case SHARE_BIT:
       share = _SH_DENYRW;
       break;
    case SHARE_BIT | READ_BIT:
       share = _SH_DENYWR;
       break;
    case SHARE_BIT | WRITE_BIT:
       share = _SH_DENYRD;
       break;
    case SHARE_BIT | READ_BIT | WRITE_BIT:
       share = _SH_DENYNO;
       break;
#if defined(_WIN32)
    case READ_BIT | WRITE_BIT:
       share = _SH_COMPAT;
       break;
    default:
       share = _SH_DENYNO;  
#else 
   default:
       share = _SH_COMPAT;
#endif
       break;
    }

    // now try to open
    int f = ::sopen(name, how, share, S_IREAD|S_IWRITE);
    if( f == OS_err )
	return 0;

    // finish up
    xfd = f;
    opened = 1;
    mode = m;
    last_seek = ::lseek(f, 0L, (m & ios::ate) ? L_end : L_set);
    if( last_seek == long(OS_err) )
	return 0;

    char *b = base();		// buffer address
    int pb = b ? ((blen() > 8) ? 4 : 1) : 0;	// putback area size
    setp(b+pb, b+pb);
    setg(b, b+pb, b+pb);

    return this;
}


// flush and close file
filebuf* filebuf::close()
{
    if( xfd == EOF )
	opened = 0;		// consistency check

    if( ! opened )
	return 0;		// nothing to do

    int ores = 0;		// result of overflow()
    if( out_waiting() )
	ores = (overflow(EOF) == EOF) ? 1 : 0;
    int cres = mode ? ::close(xfd)	// result of system ::close()
		    : 0;		// don't close it
    
    xfd = EOF;
    opened = 0;
    return (ores | cres) ? 0 : this;
}

int filebuf::setmode(int iMode)
{
	int iRes = -1;
	if (is_open())
	{
		if (iMode == filebuf::text)
			iRes = ::setmode(xfd, _O_TEXT);
		else if (iMode == filebuf::binary)
			iRes = ::setmode(xfd, _O_BINARY);
	}
	return iRes;
}

streambuf* filebuf::setbuf(char* b, int len)
{
    if( opened  &&  base() )
	return 0;		// already open with a buffer -- no change
    
    int pb;			// putback area size
    if( b  &&  len > 0 )	// use b as the new buffer
	pb = (len > 8) ? 4 : 1;	// guard against tiny buffers
    else  {			// unbuffered
	len = pb = 0;
	b = 0;
    }
    setb(b, b+len, 0);		// will delete old buffer if needed
    setp(b+pb, b+pb);
    setg(b, b+pb, b+pb);
    return this;
}


// attach this filebuf to open file descriptor -- assume fd is actually open
filebuf* filebuf::attach(int f)
{
    if( opened )
	return 0;

    xfd = f;		// assumed to be valid
    opened = 1;		// unless we can find out otherwise
    mode = 0;		// unless we can find out otherwise
    char *b = base();		// buffer address
    if( ! b ) {
	b = new char[B_size];
	if( b )
	    setb(b, b+B_size, 1);	// ~streambuf() will delete buffer
    }
    int pb = b ? ((blen() > 8) ? 4 : 1) : 0;	// putback area size
    setp(b+pb, b+pb);
    setg(b, b+pb, b+pb);
    return this;
}


// detach this filebuf from its file descriptor, return descriptor
int filebuf::detach()
{
    int result = EOF;
    if( opened ) {
	if( out_waiting() )
	    overflow(EOF);
	result = xfd;
	xfd = EOF;
	opened = 0;
    }
    return result;
}


// always flush
int filebuf::overflow(int c)
{
    if( ! opened  ||  (mode & (ios::in | ios::out)) == ios::in )
	return EOF;

    if( unbuffered()  ||  ! base() ) {
	if( c != EOF ) {
	    char b = c;
	    if( ::write(xfd, &b, 1) != 1 )
		return EOF;
	}
    }
    else {
	// now we know this is buffered and state is not bad
	int count = out_waiting();
	if( count )
	    if( ::write(xfd, pbase(), count) != count )
		return EOF;

	// reset get and put areas
	int pb = (blen() > 8) ? 4 : 1;	// putback area size
	char *b = base();
	setp(b+pb, b+blen());
	setg(b, b+pb, b+pb);
	if( c != EOF ) {
	    sputc(c);
	    gbump(1);		// pptr and gptr must be the same
	}
    }
    return 1;
}


// return the next available character, reading in a buffer if needed
int filebuf::underflow()
{
    if( ! opened  ||  (mode & (ios::in | ios::out)) == ios::out )
	return EOF;

    if( in_avail() )	// no action needed
	return (unsigned char) *gptr();

    int c;	// the return value
    int count;	// input character count

    if ( ! unbuffered()  &&  base() ) {		// this is buffered
	// flush any output buffer
	count = out_waiting();
	if( count )
	    if( ::write(xfd, pbase(), count) != count )
		return EOF;

	// find buffer data
	int pb = (blen() > 8) ? 4 : 1;	// putback area size
	char *b = base();		// base of buffer

	// read in a new buffer
	count = ::read(xfd, b+pb, blen()-pb);
	if( count == OS_err )
	    return EOF;

	// set up get and put areas
	setg(b, b+pb, b+pb+count);
	setp(b+pb, b+pb);

	if( count )
	    c = (unsigned char) *gptr();
    }
    else {		// this is not buffered
	count = ::read(xfd, lahead, 1);
	if( count == OS_err) {
	    c = EOF;
	    setg(0, 0, 0);
	}
	else {
	    c = (unsigned char)lahead[0];
	    setg(lahead, lahead, lahead+1);
	}
    }

    if( ! count )
	c = EOF;	// end of file

    return c;
}


// Seek file to position.
// We take a simple approach, and don't check for small position changes
// within the current buffer.
streampos filebuf::seekoff(streamoff off, ios::seek_dir dir, int /* mode ignored */)
{
    long loff = off;
    int count = out_waiting();
    if( count ) {		// flush the output
	if( ::write(xfd, pbase(), count) != count )
	    return EOF;
    }
    else if( dir == ios::cur ) {
        // back up the OS file pointer to match our current buffer location
        count = in_avail();
        if( count )
           loff -= count;
        // for translated files we must also back up a few characters to compensate for the CRs which
        // appear in the file but not in memory
        if ((mode & ios::binary) == 0)
        {
           for (char *p = gptr(); p < egptr(); p++)
           {
               if (*p == '\n')
                  loff --;
           }
        }   
      }
    int w = (dir == ios::beg) ? L_set : ((dir == ios::cur) ? L_cur : L_end);
    last_seek = ::lseek(xfd, loff, w);
    if( ! unbuffered()  &&  base() ) {		// set up get and put areas
	int pb = (blen() > 8) ? 4 : 1;	// putback area size
	char *b = base();
	setp(b+pb, b+pb);
	setg(b, b+pb, b+pb);
    }
    return (last_seek == long(OS_err)) ? EOF : last_seek;
}


int filebuf::sync()
{
    if( out_waiting() ) {
	if( overflow(EOF) == EOF )
	    return EOF;
    }
    else if( in_avail() ) {
	last_seek = ::lseek(xfd, long(-in_avail()), L_cur);
	setg(eback(), gptr(), gptr());
	setp(gptr(), gptr());
	if( last_seek == long(OS_err) )
	    return EOF;
    }
    return 0;
}


// we have no data on what this is supposed to return
// it is not used in this implementation
// int filebuf::last_op() { }	???


fstreambase::fstreambase()
		: buf()
{
    ios::init(&buf);
}


fstreambase::fstreambase(const char* name, int omode, int prot)
		: buf()
{
    ios::init(&buf);
    open(name, omode, prot);
}


fstreambase::fstreambase(int f)
		: buf(f)
{
    ios::init(&buf);
}


fstreambase::fstreambase(int f, char* b, int len)
		: buf(f, b, len)
{
    ios::init(&buf);
}


fstreambase::~fstreambase()
{
}


void fstreambase::open(const char* b, int m, int prot)
{
    if( m & ios::app )
	m |= ios::out;	// append implies output
    else if( (m & (ios::out | ios::ate | ios::app | ios::in)) == ios::out )
	m |= ios::trunc; // output implies truncate unless in, app, or ate

    if( buf.is_open() )
	clear(ios::failbit);		// fail - already open
    else if( buf.open(b, m, prot) )
	clear(ios::goodbit);		// successful open
    else
	clear(ios::badbit);		// open failed
}


void fstreambase::attach(int f)
{
    if( buf.is_open() )
	setstate(ios::failbit);
    else if( buf.attach(f) )
	clear(ios::goodbit);
    else
	clear(ios::badbit);
}


void fstreambase::close()
{
    if( buf.close() )
	clear(ios::goodbit);
    else
	setstate(ios::failbit);
}


void fstreambase::setbuf(char* b, int len)
{
    if( buf.setbuf(b, len) )
	clear(ios::goodbit);
    else
	setstate(ios::failbit);
}


// we have no data on what this is supposed to do
// it is not used in this implementation
//	void fstreambase::verify(int) { }	???


ifstream::ifstream() :
		fstreambase(),
		istream()
{
}


ifstream::ifstream(const char* name, int omode, int prot) :
		fstreambase(name, omode|ios::in, prot),
		istream()
{
}


ifstream::ifstream(int f) :
		fstreambase(f),
		istream()
{
}


ifstream::ifstream(int f, char* b, int len) :
		fstreambase(f, b, len),
		istream()
{
}


ifstream::~ifstream()
{
}


ofstream::ofstream() :
		fstreambase(),
		ostream()
{
}


ofstream::ofstream(const char* name, int omode, int prot) :
		fstreambase(name, omode|ios::out, prot),
		ostream()
{
}


ofstream::ofstream(int f) :
		fstreambase(f),
		ostream()
{
}


ofstream::ofstream(int f, char* b, int len) :
		fstreambase(f, b, len),
		ostream()
{
}


ofstream::~ofstream()
{
}


fstream::fstream() :
		fstreambase(),
		iostream()
{
}


fstream::fstream(const char* name, int omode, int prot) :
		fstreambase(name, omode, prot),
		iostream()
{
}


fstream::fstream(int f) :
		fstreambase(f),
		iostream()
{
}


fstream::fstream(int f, char* b, int len) :
		fstreambase(f, b, len),
		iostream()
{
}


fstream::~fstream()
{
}
