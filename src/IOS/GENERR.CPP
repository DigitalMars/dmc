/*
 *   generr.cpp
 *
 * $Id: generr.cpp,v 1.1.1.1 1997/01/02 19:18:03 smarx Exp $
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
 * $Log: generr.cpp,v $
 * Revision 1.1.1.1  1997/01/02 19:18:03  smarx
 * cafe12
 *
   
      Rev 1.2   08 May 1995 18:16:26   Andrew
   Fixed CR/LF problems that were giving Aruba Fits
   
      Rev 1.1   06 Jun 1994  8:52:50   bushnell
   Changed LF to CR/LF to view files in ARUBA/Brief
   
      Rev 1.0   20 Apr 1994 17:38:34   thompson                   
   Initial revision.
// Revision 1.2  1994/04/14  00:48:52  vriezen
// Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
//
 *
 * 
 */

#include <stdlib.h>
#include <stdio.h>

void genericerror(int i, char* c)
{
    fprintf(stderr, "%s %d\n", c ? c : "error in generic library function", i);
    abort();
}
