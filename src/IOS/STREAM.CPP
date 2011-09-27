/*
 *   stream.cpp -- implement old stream whitespace functions with new streams
 *
 * $Id: stream.cpp,v 1.1.1.1 1997/01/02 19:18:05 smarx Exp $
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
 * $Log: stream.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:05  smarx
 * cafe12
 *
   
      Rev 1.3   08 May 1995 18:16:24   Andrew
   Fixed CR/LF problems that were giving Aruba Fits
   
      Rev 1.2   14 Jan 1995 17:12:00   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
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

#include <ctype.h>
#include <stream.h>

static Iostream_init iostream_init;  // Causes CIN, CERR, etc. to be initialized

void eatwhite(istream& is)
{
    while( isspace(is.peek()) )
	is.get();
}


/* manipulator which eats white space */
istream& WS(istream& is)
{
    eatwhite(is);
    return is;
}
