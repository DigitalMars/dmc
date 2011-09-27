#if __SC__ || __RCC__
#pragma once
#endif

#ifndef _STDSTREAM_H_
#define _STDSTREAM_H_

#ifndef __cplusplus
#error  Use C++ compiler for stdiostr.h
#endif

/*
 *    stdiostream.h -- class stdiobuf and stdiostream declarations
 *
 * $Id: stdiostr.h,v 1.1.1.1 1997/01/02 19:16:44 smarx Exp $
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
 * $Log: stdiostr.h,v $
 * Revision 1.1.1.1  1997/01/02 19:16:44  smarx
 * cafe12
 *
// * 
// * 5     2/26/96 8:05a Smarx
 * 
 *    Rev 1.3   24 May 1995 15:42:16   Andrew
 * Fixed problem where structs/classes were not in pragma pack(__DEFALIGN)
 * 
 *    Rev 1.2   08 Oct 1994 14:11:56   BUSHNELL
 * Added pragma once for faster compilations
 * 
 *    Rev 1.1   02 Jun 1994 21:35:44   bushnell
 * added ifdef so that MS RC will not include header twice
 * 

 *    Rev 1.0   28 Apr 1994 19:17:46   thompson                   

 * Initial revision.

 * 

 *    Rev 1.0   20 Apr 1994 17:46:14   thompson                   

 * Initial revision.

 * Revision 1.1  1994/04/14  16:58:12  vriezen
 * Initial revision
 *
 * Revision 1.2  1994/04/14  00:50:17  vriezen
 * Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
 *
 *
 * 
 */

/* 
    NOTE: These are inefficient and obsolete.  Use the standard classes
	  and functions in <fstream.h> instead.
*/


#include <iostream.h>
#include <stdio.h>

#pragma pack(__DEFALIGN)

class stdiobuf : public streambuf {
public:
		stdiobuf(FILE*);
	FILE*	stdiofile();
		~stdiobuf();

virtual	int	overflow(int=EOF);
virtual	int	pbackfail(int);
virtual	int	sync();
virtual	streampos seekoff(streamoff, ios::seek_dir, int);
virtual	int	underflow();

private:
	FILE*	sio;			
	char	lahead[2];
};
inline	FILE*	stdiobuf::stdiofile() { return sio; }


class stdiostream : public ios {
public:
		stdiostream(FILE*);
		~stdiostream();
	stdiobuf* rdbuf();

private:
	stdiobuf buf;
};
inline	stdiobuf* stdiostream::rdbuf() { return &buf; }

#pragma pack()

#endif /* _STDSTREAM_H_ */
