#if __SC__ || __RCC__
#pragma once
#endif

#ifndef _STRSTREAM_H_
#define _STRSTREAM_H_

#ifndef __cplusplus
#error  Use C++ compiler for strstrea.h
#endif

/*
 *    strstream.h -- class strstream declarations
 *
 * $Id: strstrea.h,v 1.1.1.1 1997/01/02 19:16:44 smarx Exp $
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
 * $Log: strstrea.h,v $
 * Revision 1.1.1.1  1997/01/02 19:16:44  smarx
 * cafe12
 *
// * 
// * 5     2/26/96 8:05a Smarx
 * 
 *    Rev 1.3   24 May 1995 15:42:34   Andrew
 * Fixed problem where structs/classes were not in pragma pack(__DEFALIGN)
 * 
 *    Rev 1.2   08 Oct 1994 14:11:54   BUSHNELL
 * Added pragma once for faster compilations
 * 
 *    Rev 1.1   02 Jun 1994 21:35:44   bushnell
 * added ifdef so that MS RC will not include header twice
 * 

 *    Rev 1.0   28 Apr 1994 19:19:44   thompson                   

 * Initial revision.

 * 

 *    Rev 1.0   20 Apr 1994 17:46:36   thompson                   

 * Initial revision.

 * Revision 1.1  1994/04/14  16:57:40  vriezen
 * Initial revision
 *
 * Revision 1.2  1994/04/14  00:50:17  vriezen
 * Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
 *
 *
 * 
 */


#include <iostream.h>

#pragma pack(__DEFALIGN)

class strstreambuf : public streambuf {
public:
		strstreambuf();
		strstreambuf(int n);
		strstreambuf(void* (*a)(long), void (*f)(void*));
		strstreambuf(char* _s, int, char* _strt=0);
		~strstreambuf();

	void	freeze(int = 1);
	int	isfrozen();
	char*	str();
virtual int	doallocate();
virtual int	overflow(int);
virtual int	underflow();
virtual streambuf* setbuf(char*, int);
virtual streampos seekoff(streamoff, ios::seek_dir, int);

private:
	void*	(*allocf)(long);
	void	(*freef)(void*);
	short	ssbflags;
	enum	{ dynamic = 1, frozen = 2, unlimited = 4 };
	int	next_alloc;
	char*	real_end;

	void	init(char*, int, char*);
};
inline int strstreambuf::isfrozen() { return (ssbflags&frozen) != 0; }


class strstreambase : public virtual ios {
public:
	strstreambuf* rdbuf();

protected:
		strstreambase(char*, int, char*);
		strstreambase();
		~strstreambase();
private:
	strstreambuf buf;
};
inline strstreambuf* strstreambase::rdbuf() { return &this->buf; }


class istrstream : public strstreambase, public istream {
public:
		istrstream(char*);
		istrstream(char*, int);
		~istrstream();
};


class ostrstream : public strstreambase, public ostream {
public:
		ostrstream(char*, int, int = ios::out);
		ostrstream();
		~ostrstream();

	char*	str();
	int	pcount();
};
inline char*	ostrstream::str() { return strstreambase::rdbuf()->str(); }
inline int	ostrstream::pcount() {
			return strstreambase::rdbuf()->out_waiting();
		}


class strstream : public strstreambase, public iostream {
public:
		strstream();
		strstream(char*, int _sz, int _m);
		~strstream();

	char*	str();
};
inline char* strstream::str() { return strstreambase::rdbuf()->str(); }

#pragma pack()

#endif /* _STRSTREAM_H_ */
