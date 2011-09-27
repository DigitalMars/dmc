/*
 *  istreamx.cpp -- obsolete class istream constructors
 *
 * $Id: istreamx.cpp,v 1.1.1.1 1997/01/02 19:18:04 smarx Exp $
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
 * $Log: istreamx.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:04  smarx
 * cafe12
 *
   
      Rev 1.4   14 Jan 1995 17:12:00   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
      Rev 1.3   12 Oct 1994 16:40:14   BUSHNELL
   Put in include of typeinfo.h for -ER
   
      Rev 1.2   23 Jun 1994 15:56:36   BUSHNELL
   changed includes to only have 8 characters so that RTL will build under NT
   
      Rev 1.1   06 Jun 1994  8:52:52   bushnell
   Changed LF to CR/LF to view files in ARUBA/Brief
   

      Rev 1.0   20 Apr 1994 17:38:40   thompson                   

   Initial revision.

// Revision 1.2  1994/04/14  00:48:52  vriezen
// Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
//
 *
 * 
 */

#include <strstrea.h>
#include <fstream.h>
#include <typeinfo.h>  /* For RTTI */

static Iostream_init iostream_init;  // Causes CIN, CERR, etc. to be initialized

// Obsolete constructor, for streams 1.2 compatibility
istream::istream(int, char* b, int sk)
{
    ios::init(new strstreambuf(b, 0, 0));
    skip(sk);
}


// Obsolete constructor, for streams 1.2 compatibility
istream::istream(int fd, int sk, ostream* t)
{
    ios::init(new filebuf(fd));
    skip(sk);
    tie(t);
}

