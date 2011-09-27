/*
 *   iostream.cpp -- class iostream member functions
 *
 * $Id: iostream.cpp,v 1.1.1.1 1997/01/02 19:18:03 smarx Exp $
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
 * $Log: iostream.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:03  smarx
 * cafe12
 *Rev 1.0   20 Apr 1994 17:38:38   thompson                   
   
      Rev 1.4   20 Apr 1995 18:33:00   Andrew
   Fixed bug with octal and hex input to cin
   
      Rev 1.3   14 Jan 1995 17:11:58   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
      Rev 1.2   12 Oct 1994 16:40:14   BUSHNELL
   Put in include of typeinfo.h for -ER
   
      Rev 1.1   06 Jun 1994  8:52:48   bushnell
   Changed LF to CR/LF to view files in ARUBA/Brief
 *  		Initial revision.

// Revision 1.2  1994/04/14  00:48:52  vriezen
// Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
//
 *
 * 
 */

#include "filesys.h"
#include <fstream.h>
#include <typeinfo.h>

iostream::iostream(streambuf* s) :
		istream(),
		ostream()
{
    ios::init(s);
}


iostream::iostream() :
		istream(),
		ostream()
{
}


iostream::~iostream()
{
}


/*
 * iostream with assign
 */

// does no initialization
iostream_withassign::iostream_withassign() :
		iostream()
{
}


iostream_withassign::~iostream_withassign() { }	// nothing to do


// gets buffer from iostream and does entire initialization
iostream_withassign& iostream_withassign::operator= (ios& s)
{
    ios::init(s.rdbuf());
    return *this;
}


// associates streambuf with stream and does entire initialization
iostream_withassign& iostream_withassign::operator= (streambuf* s)
{
    ios::init(s);
    return *this;
}


/*
 * one-time initializer for standard files
 */

// this allows calling a constructor for an existing object
inline void * operator new(size_t, void *p)
{
    return p;
}


static Iostream_init iostream_init;  // Causes CIN, CERR, etc. to be initialized

istream_withassign cin;
ostream_withassign cout;
ostream_withassign cerr;
ostream_withassign clog;

static filebuf *stdin_filebuf;
static filebuf *stdout_filebuf;
static filebuf *stderr_filebuf;

int Iostream_init::stdstatus = 0;
int Iostream_init::initcount = 0;

Iostream_init::Iostream_init()
{
    // do all this exactly once
    if( ++initcount != 1 )
	return;

    stdin_filebuf = new filebuf(F_stdin);
    stdout_filebuf = new filebuf(F_stdout);
    stderr_filebuf = new filebuf(F_stderr);

    // call constructors for standard streams
    new (&cin) istream_withassign;
    new (&cout) ostream_withassign;
    new (&cerr) ostream_withassign;
    new (&clog) ostream_withassign;
    // attach the standard files to the standard streams
    cin = stdin_filebuf;
    cout = stdout_filebuf;
    clog = stderr_filebuf;
    cerr = stderr_filebuf;

    // tie cin, cerr, and clog to cout
    cin.tie(&cout);
    clog.tie(&cout);
    cerr.tie(&cout);

    // unit-buffer cerr
    cerr.setf(ios::unitbuf);
    
    // cin should only skipws and not have ios::dec set.
    cin.flags(ios::skipws);
}


Iostream_init::~Iostream_init()
{
    // do all this exactly once
    if( --initcount != 0 )
	return;

    delete stdin_filebuf;
    delete stdout_filebuf;
    delete stderr_filebuf;
}
