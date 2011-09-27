/*
 *   siosync.cpp -- sync_with_stdio function
 *
 * $Id: siosync.cpp,v 1.1.1.1 1997/01/02 19:18:05 smarx Exp $
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
 * $Log: siosync.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:05  smarx
 * cafe12
 *
   
      Rev 1.4   08 May 1995 18:16:24   Andrew
   Fixed CR/LF problems that were giving Aruba Fits
   
      Rev 1.3   14 Jan 1995 17:12:00   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
      Rev 1.2   23 Jun 1994 15:56:34   BUSHNELL
   changed includes to only have 8 characters so that RTL will build under NT
   
      Rev 1.1   06 Jun 1994  8:52:56   bushnell
   Changed LF to CR/LF to view files in ARUBA/Brief
   
      Rev 1.0   20 Apr 1994 17:38:46   thompson                   
   Initial revision.
// Revision 1.2  1994/04/14  00:48:52  vriezen
// Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
//
 *
 * 
 */

#include <stdiostr.h>

static Iostream_init iostream_init;  // Causes CIN, CERR, etc. to be initialized

static short already_done = 0;
static stdiobuf *stdin_siob;
static stdiobuf *stdout_siob;
static stdiobuf *stderr_siob;

void ios::sync_with_stdio()
{
    if( already_done )
	return;		// we only do this once!
    already_done = 1;

    cin.sync();
    cout.flush();
    clog.flush();

    // recover the streambuf space
    cin.rdbuf()->streambuf::~streambuf();
    cout.rdbuf()->streambuf::~streambuf();
    clog.rdbuf()->streambuf::~streambuf();

    // make stdiobufs for the stdio files
    stdin_siob = new stdiobuf(stdin);
    stdout_siob = new stdiobuf(stdout);
    stderr_siob = new stdiobuf(stderr);

    cin = stdin_siob;
    cout = stdout_siob;
    clog = stderr_siob;
    cerr = stderr_siob;

    cout.setf(ios::unitbuf);
    clog.setf(ios::unitbuf);
}
