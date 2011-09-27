#if __SC__ || __RCC__
#pragma once
#endif

#ifndef _STREAM_H_
#define _STREAM_H_

/*
 *    stream.h -- provide a migration path from old streams to iostreams
 *
 * $Id: stream.h,v 1.1.1.1 1997/01/02 19:16:44 smarx Exp $
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
 * $Log: stream.h,v $
 * Revision 1.1.1.1  1997/01/02 19:16:44  smarx
 * cafe12
 *
 * 
 *    Rev 1.3   08 Oct 1994 14:11:56   BUSHNELL
 * Added pragma once for faster compilations
 * 
 *    Rev 1.2   23 Jun 1994 15:57:52   BUSHNELL
 * Changed include to have only 8 characters for NT builds of the RTL
 * 
 *    Rev 1.1   02 Jun 1994 21:35:44   bushnell
 * added ifdef so that MS RC will not include header twice
 * 

 *    Rev 1.0   20 Apr 1994 17:46:20   thompson                   

 * Initial revision.

 * Revision 1.2  1994/04/14  00:50:17  vriezen
 * Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
 *
 *
 * 
 */



#include <iostream.h>
#include <iomanip.h>
#include <stdiostr.h>
#include <fstream.h>

/*
 * Simulate the whitespace object in old streams.
 * WS is an istream manipulator which eats white space, having the
 * same purpose as struct WS in old streams.
 */
istream& WS(istream&);
void eatwhite(istream&);

const int input  = ios::in;
const int output = ios::out;
const int append = ios::app;
const int atend  = ios::ate;
const int _good  = ios::goodbit;
const int _bad   = ios::badbit;
const int _fail  = ios::failbit;
const int _eof   = ios::eofbit;

typedef ios::io_state state_value;

#endif /* _STREAM_H_ */
