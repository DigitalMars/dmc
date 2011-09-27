/*
 *  manip.cpp -- predefined manipulators
 *
 * $Id: manip.cpp,v 1.1.1.1 1997/01/02 19:18:04 smarx Exp $
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
 * $Log: manip.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:04  smarx
 * cafe12
 *
   
      Rev 1.3   08 May 1995 18:16:24   Andrew
   Fixed CR/LF problems that were giving Aruba Fits
   
      Rev 1.2   14 Jan 1995 17:12:00   Andy
   Moved static declaration of iostream_init from iostream.h into .cpp files
   
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
#include <iomanip.h>

static Iostream_init iostream_init;  // Causes CIN, CERR, etc. to be initialized

// set the conversion base to 0, 8, 10, or 16
static ios& sbase(ios& i, int b)
{
    long l = 0;
    if( b == 16 ) l = ios::hex;
    else if (b == 10) l = ios::dec;
    else if( b == 8 ) l = ios::oct;
    i.setf(l, ios::basefield);
    return i;
}
smanip_int setbase(int b)
{
    return smanip_int(sbase, b);
}


// clear the flags bitvector according to the bits set in b
static ios& rsios(ios& i, long b)
{
    i.unsetf(b);
    return i;
}
smanip_long resetiosflags(long b)
{
    return smanip_long(rsios, b);
}


// set the flags bitvector according to the bits set in b
static ios& sios(ios& i, long b)
{
    i.setf(b);
    return i;
}
smanip_long setiosflags(long b)
{
    return smanip_long(sios, b);
}


// set fill character for padding a field
static ios& sfill(ios& i, int f)
{
    i.fill((char)f);
    return i;
}
smanip_int setfill(int f)
{
    return smanip_int(sfill, f);
}


// set the floating-point precision to n digits
static ios& sprec(ios& i, int n)
{
    i.precision(n);
    return i;
}
smanip_int setprecision(int n)
{
    return smanip_int(sprec, n);
}


// set the field width to n
static ios& swidth(ios& i, int n)
{
    i.width(n);
    return i;
}
smanip_int setw(int n)
{
    return smanip_int(swidth, n);
}
