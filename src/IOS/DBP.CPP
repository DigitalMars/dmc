/*
 *   dbp.cpp -- debugging output for streambuf
 *
 * $Id: dbp.cpp,v 1.1.1.1 1997/01/02 19:18:03 smarx Exp $
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
 * $Log: dbp.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:03  smarx
 * cafe12
 *
   
      Rev 1.3   08 May 1995 18:16:24   Andrew
   Fixed CR/LF problems that were giving Aruba Fits
   
      Rev 1.2   14 Jan 1995 17:12:04   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
      Rev 1.1   06 Jun 1994  8:52:48   bushnell
   Changed LF to CR/LF to view files in ARUBA/Brief
   
      Rev 1.0   20 Apr 1994 17:38:32   thompson                   
   Initial revision.
// Revision 1.2  1994/04/14  00:48:52  vriezen
// Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
//
 *
 * 
 */

#include "filesys.h"
#include <iostream.h>
#include <stdio.h>	/* for sprintf */
#include <string.h>

static Iostream_init iostream_init;  // Causes CIN, CERR, etc. to be initialized

// debugging output for streambuf
void streambuf::dbp()
{
    char s[80];
    sprintf(s, "alloc_ = %d, unbuf_ = %d\n", alloc_, unbuf_);
    write(F_stdout, s, strlen(s));
    sprintf(s, "base_ = %p, ebuf_ = %p\n", base_, ebuf_);
    write(F_stdout, s, strlen(s));
    sprintf(s, "pbase_ = %p, pptr_ = %p, epptr_ = %p\n", pbase_, pptr_, epptr_);
    write(F_stdout, s, strlen(s));
    sprintf(s, "gptr_ = %p, egptr_ = %p, eback_ = %p\n", gptr_, egptr_, eback_);
    write(F_stdout, s, strlen(s));
}
