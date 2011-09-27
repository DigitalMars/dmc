
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef _IOSTREAM_H_
#define _IOSTREAM_H_

#ifndef __cplusplus
#error  Use C++ compiler for iostream.h - invoke with -cpp switch
#endif

/*
 *  iostream.h -- basic stream I/O declarations
 *
 * $Id: iostream.h,v 1.1.1.1 1997/01/02 19:16:41 smarx Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 * Voice: (503) 754-3010        FAX: (503) 757-6650
 *
 * Copyright (C) 1991,  1993, 1994.
 * This software is subject to copyright protection under the laws of 
 * the United States and other countries.
 * ALL RIGHTS RESERVED
 *
 ***************************************************************************
 *
 * $Log: iostream.h,v $
 * Revision 1.1.1.1  1997/01/02 19:16:41  smarx
 * cafe12
 *
// * 
// * 13    5/01/96 10:26a Smarx
// * added long double support
// * 
// * 12    2/26/96 8:04a Smarx
 * 
 *    Rev 1.10   24 May 1995 15:42:34   Andrew
 * Fixed problem where structs/classes were not in pragma pack(__DEFALIGN)
 * 
 *    Rev 1.9   08 May 1995 15:20:40   Andrew
 * Fixed debug info problem
 * 
 *    Rev 1.8   03 May 1995  9:40:08   Andrew
 * Fixed problem with -A compiles
 * 
 *    Rev 1.7   14 Jan 1995 17:12:28   Andy
 * FIxed iostream.h so that users can use it in a precompiled header
 * 
 *    Rev 1.6   04 Jan 1995 14:20:50   Andy
 * added operator= (ostream_withassign&) member to class ostream_withassign
 * 
 *    Rev 1.5   02 Nov 1994 12:09:54   BUSHNELL
 * Ifdefed in long long support for intsize==4
 * 
 *    Rev 1.4   08 Oct 1994 14:11:54   BUSHNELL
 * Added pragma once for faster compilations
 * 
 *    Rev 1.3   05 Oct 1994 13:34:30   anderson
 * Added support for ios::binary and ios::translated
 * 
 *    Rev 1.2   04 Oct 1994 17:43:50   DYNIN
 * 10181 : added 2 funcs for MS compatibility.
 * 
 *    Rev 1.1   02 Jun 1994 21:35:42   bushnell
 * added ifdef so that MS RC will not include header twice
 * 
 *    Rev 1.0   28 Apr 1994 19:17:38   thompson                   
 * Initial revision.
 * Revision 1.4  1994/04/14  16:48:51  vriezen
 * Add conditional compilation for long long and long double
 *
 * Revision 1.3  1994/04/14  00:50:17  vriezen
 * Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
 *
 *
 * 
 */



#include <string.h>     // to get memcpy and NULL

// Definition of EOF must match the one in <stdio.h>
#define EOF (-1)

// extract a char from int i, ensuring that zapeof(EOF) != EOF
#define zapeof(i) ((unsigned char)(i))

typedef long streampos;         // should be int or long
typedef long streamoff;         // should be int or long

class streambuf;
class ostream;

#pragma pack(__DEFALIGN)
class ios {
public:
	// stream status bits
	enum io_state   {
		goodbit  = 0x00,        // no bit set: all is ok
		eofbit   = 0x01,        // at end of file
		failbit  = 0x02,        // last I/O operation failed
		badbit   = 0x04,        // invalid operation attempted
		hardfail = 0x80         // unrecoverable error
		};

	// stream operation mode
	enum open_mode  {
		in         = 0x01,        // open for reading
		out        = 0x02,        // open for writing
		ate        = 0x04,        // seek to eof upon original open
		app        = 0x08,        // append mode: all additions at eof
		trunc      = 0x10,        // truncate file if already exists
		nocreate   = 0x20,        // open fails if file doesn't exist
		noreplace  = 0x40,        // open fails if file already exists
		binary     = 0x80,        // file is not translated
		translated = 0x00         // for compatability
		};

	// stream seek direction
	enum seek_dir { beg=0, cur=1, end=2 };

	// formatting flags
	enum    {
		skipws    = 0x0001,     // skip whitespace on input
		left      = 0x0002,     // left-adjust output
		right     = 0x0004,     // right-adjust output
		internal  = 0x0008,     // padding after sign or base indicator
		dec       = 0x0010,     // decimal conversion
		oct       = 0x0020,     // octal conversion
		hex       = 0x0040,     // hexidecimal conversion
		showbase  = 0x0080,     // use base indicator on output
		showpoint = 0x0100,     // force decimal point (floating output)
		uppercase = 0x0200,     // upper-case hex output
		showpos   = 0x0400,     // add '+' to positive integers
		scientific= 0x0800,     // use 1.2345E2 floating notation
		fixed     = 0x1000,     // use 123.45 floating notation
		unitbuf   = 0x2000,     // flush all streams after insertion
		stdio     = 0x4000      // flush stdout, stderr after insertion
		};

	// constants for second parameter of seft()
static  const long basefield;           // dec | oct | hex
static  const long adjustfield;         // left | right | internal
static  const long floatfield;          // scientific | fixed

	// constructor, destructor
		ios(streambuf*);
virtual         ~ios();

	// for reading/setting/clearing format flags
	long    flags();
	long    flags(long);
	long    setf(long _setbits, long _field);
	long    setf(long);
	long    unsetf(long);

	// reading/setting field width
	int     width();
	int     width(int);

	// reading/setting padding character
	char    fill();
	char    fill(char);

	// reading/setting digits of floating precision
	int     precision(int);
	int     precision();

	// reading/setting ostream tied to this stream
	ostream* tie(ostream*);
	ostream* tie();

	// find out about current stream state
	int     rdstate();              // return the stream state
	int     eof();                  // non-zero on end of file
	int     fail();                 // non-zero if an operation failed
	int     bad();                  // non-zero if error occurred
	int     good();                 // non-zero if no state bits set
	void    clear(int = 0);         // set the stream state
		operator void* ();      // zero if state failed
	int     operator! ();           // non-zero if state failed

	streambuf* rdbuf();             // get the assigned streambuf

	// for declaring additional flag bits and user words
static long     bitalloc();     // acquire a new flag bit, value returned
static int      xalloc();       // acquire a new user word, index returned
	long  & iword(int);     // return the nth user word as an int
	void* & pword(int);     // return the nth user word as a pointer

static void     sync_with_stdio();

	// obsolete, for streams 1.2 compatibility
	int     skip(int);

protected:
	// additional state flags for ispecial and ospecial
	enum { skipping = 0x100, tied = 0x200 };

	streambuf* bp;          // the associated streambuf
	ostream* x_tie;         // the tied ostream, if any
	int     state;          // status bits
	int     ispecial;       // istream status bits  ***
	int     ospecial;       // ostream status bits  ***
	long    x_flags;        // formatting flag bits
	int     x_precision;    // floating-point precision on output
	int     x_width;        // field width on output
	int     x_fill;         // padding character on output
	int     isfx_special;   // unused               ***
	int     osfx_special;   // unused               ***
	int     delbuf;         // unused               ***
	int     assign_private; // unused               ***
/*
 * The data members marked with *** above are not documented in the AT&T
 * release of streams, so we cannot guarantee compatibility with any
 * other streams release in the use or values of these data members.
 * If you can document any expected behavior of these data members, we
 * will try to adjust our implementation accordingly.
 */

		ios();          // null constructor, does not initialize

	void    init(streambuf*);       // the actual initialization

	void    setstate(int);  // set all status bits

static  void    (*stdioflush)();

private:
	// for extra flag bits and user words
static  long    nextbit;
static  int     usercount;
	void *userwords;
	int     nwords;
	void    usersize(int);

	// these declarations prevent automatic copying of an ios
		ios(ios&);              // declared but not defined
	void    operator= (ios&);       // declared but not defined

};
inline streambuf* ios::rdbuf() { return bp; }
inline ostream* ios::tie() { return x_tie; }    
inline char     ios::fill() { return x_fill; }
inline int      ios::precision() { return x_precision; }
inline int      ios::rdstate() { return state; }
inline int      ios::eof() { return state & eofbit; }
inline int      ios::fail() { return state & (failbit | badbit | hardfail); }
inline int      ios::bad() { return state & (badbit | hardfail); }
inline int      ios::good() { return state == 0; }
inline long     ios::flags() { return x_flags; }
inline int      ios::width() { return x_width; }
inline int      ios::width(int _w) { int _i = x_width; x_width = _w; return _i; }
inline char     ios::fill(char _c) { char _x = x_fill; x_fill = _c; return _x; }
inline int      ios::precision(int _p) {
			int _x = x_precision; x_precision = _p; return _x;
		}
inline          ios::operator void* () { return fail() ? 0 : this; }
inline int      ios::operator! () { return fail(); }


class streambuf {
public:
	// constructors and destructors
	streambuf();            // make empty streambuf
	streambuf(char*, int);  // make streambuf with given char array
virtual ~streambuf();

	// use the provided char array for the buffer if possible
virtual streambuf* setbuf(char*, int);

	// getting (extracting) characters
	int     sgetc();                // peek at next char
	int     snextc();               // advance to and return next char
	int     sbumpc();               // return current char and advance
	void    stossc();               // advance to next character
	int     sgetn(char*, int);      // get next n chars
virtual int     xsgetn(char*, int);     // implementation of sgetn
virtual int     underflow();            // fill empty buffer
	int     sputbackc(char);        // return char to input
virtual int     pbackfail(int);         // implementation of sputbackc
	int     in_avail();             // number of avail chars in buffer

	// putting (inserting) characters
	int     sputc(int);             // put one char
	int     sputn(const char*, int); // put n chars from string
virtual int     xsputn(const char* s, int n); // implementation of sputn
virtual int     overflow(int = EOF);    // flush buffer and make more room
	int     out_waiting();          // number of unflushed chars

	// moving around in stream
virtual streampos seekoff(streamoff, ios::seek_dir, int = (ios::in | ios::out));
virtual streampos seekpos(streampos, int = (ios::in | ios::out));
virtual int     sync();

	void    dbp();          // for debugging streambuf implementations

protected:
	char*   base();         // return start of buffer area
	char*   ebuf();         // return end+1 of buffer area
	int     blen();         // return length of buffer area
	char*   pbase();        // return start of put area
	char*   pptr();         // return next location in put area
	char*   epptr();        // return end+1 of put area
	char*   eback();        // return base of putback section of get area
	char*   gptr();         // return next location in get area
	char*   egptr();        // return end+1 of get area
	void    setp(char*, char*); // initialize the put pointers
	void    setg(char*, char*, char*); // initialize the get pointers
	void    pbump(int);     // advance the put pointer
	void    gbump(int);     // advance the get pointer
	void    setb(char*, char*, int = 0 ); // set the buffer area
	void    unbuffered(int);// set the buffering state
	int     unbuffered();   // non-zero if not buffered
	int     allocate();     // set up a buffer area
virtual int     doallocate();   // implementation of allocate

private:
	short   alloc_;         // non-zero if buffer should be deleted
	short   unbuf_;         // non-zero if unbuffered
	char*   base_;          // start of buffer area
	char*   ebuf_;          // end+1 of buffer area
	char*   pbase_;         // start of put area
	char*   pptr_;          // next put location
	char*   epptr_;         // end+1 of put area
	char*   eback_;         // base of putback section of get area
	char*   gptr_;          // next get location
	char*   egptr_;         // end+1 of get area

	int     do_snextc();    // implementation of snextc

	// these declarations prevent copying of a streambuf
		streambuf(streambuf&);  // declared but not defined
	void    operator= (streambuf&); // declared but not defined
};
inline char*    streambuf::base() { return base_; }
inline char*    streambuf::pbase() { return pbase_; }
inline char*    streambuf::pptr() { return pptr_; }
inline char*    streambuf::epptr() { return epptr_; }
inline char*    streambuf::gptr() { return gptr_; }
inline char*    streambuf::egptr()      { return egptr_; }
inline char*    streambuf::eback()      { return eback_; }
inline char*    streambuf::ebuf()       { return ebuf_; }
inline int      streambuf::unbuffered() { return unbuf_; }
inline int      streambuf::blen() { return ebuf_ - base_; }
inline void     streambuf::pbump(int _n) { pptr_ += _n; }
inline void     streambuf::gbump(int _n) { gptr_ += _n; }
inline void     streambuf::unbuffered(int _unb) { unbuf_ = (_unb != 0); }
inline int      streambuf::in_avail() {
			return (egptr_ > gptr_) ? egptr_ - gptr_ : 0;
		}
inline int      streambuf::out_waiting() { return pptr_ ? pptr_ - pbase_ : 0; }
inline int      streambuf::allocate() {
			return (base_ || unbuf_) ? 0 : doallocate();
		}
inline int      streambuf::sgetc() {
			return (gptr_ >= egptr_) ? underflow() :
						   (unsigned char)(*gptr_);
		}
inline int      streambuf::snextc() {
			return (! gptr_ || (++gptr_ >= egptr_)) ?
				do_snextc() :
				(unsigned char)(*gptr_);
		}
inline int      streambuf::sbumpc() {
			return (gptr_ >= egptr_ && underflow() == EOF) ?
				EOF :
				(unsigned char)(*gptr_++);
		}
inline void     streambuf::stossc() {
			if( gptr_ >= egptr_ ) underflow();
			else ++gptr_;
		}
inline int      streambuf::sputbackc(char _c) {
			return (gptr_ > eback_) ?
				(unsigned char)(*--gptr_ = _c) :
				pbackfail(_c);
		}
inline int      streambuf::sputc(int _c) {
			return (pptr_ >= epptr_) ?
				overflow((unsigned char)_c) :
				(unsigned char)(*pptr_++ = _c);
		}
inline int      streambuf::sputn(const char* _s, int _n) {
			if( _n <= (epptr_ - pptr_) ) {
				memcpy(pptr_, _s, _n);
				pbump(_n);
				return _n;
			}
			return xsputn(_s, _n);
		}
inline int      streambuf::sgetn(char* _s, int _n) {
			if( _n <= (egptr_ - gptr_) ) {
				memcpy(_s, gptr_, _n);
				gbump(_n);
				return _n;
			}
			return xsgetn(_s, _n);
		}


class istream : virtual public ios {
public:
	// constructor and destructor
		istream(streambuf*);
virtual         ~istream();

	// Obsolete constructors, for streams 1.2 compatibility
		// obsolete: set skip via format, tie via tie() function
		istream(streambuf*, int _sk, ostream* _t=0);
		// obsolete: use strstream
		istream(int _sz, char*, int _sk=1);
		// obsolete: use fstream
		istream(int _fd, int _sk=1, ostream* _t=0);

	int     ipfx(int = 0);          // input prefix function
	int     ipfx0();                // same as ipfx(0)
	int     ipfx1();                // same as ipfx(1)
	void    isfx()          { }     // unused input suffix function

	// set/read the get pointer's position
	istream& seekg(streampos);
	istream& seekg(streamoff, ios::seek_dir);
	streampos tellg();

	int     sync();

	/*
	 * Unformatted extraction operations
	 */
	// extract characters into an array
	istream& get(char*, int, char = '\n');
	istream& read(char*, int);

	// extract characters into an array up to termination char
	istream& getline(char*, int, char = '\n');
	istream& getline(unsigned char *buf, int n, char d = '\n')
	  { return getline((char*) buf, n, d); }
	istream& getline(signed char* buf, int n, char d = '\n')
	  { return getline((char*) buf, n, d); }

	// extract characters into a streambuf up to termination char
	istream& get(streambuf&, char = '\n');

	// extract a single character
	istream& get(char&);
	istream& get(unsigned char&);
	int     get();

	int     peek();         // return next char without extraction
	int     gcount();       // number of unformatted chars last extracted
	istream& putback(char); // push back char into input

	// extract and discard chars but stop at delim
	istream& ignore(int = 1, int = EOF);

	/*
	 * Formatted extraction operations
	 */
	istream& operator>> (istream& (*_f)(istream&));
	istream& operator>> (ios& (*_f)(ios&) );
	istream& operator>> (char*);
	istream& operator>> (unsigned char*);
	istream& operator>> (char&);
	istream& operator>> (unsigned char&);
	istream& operator>> (short&);
	istream& operator>> (int&);
	istream& operator>> (long&);
#if   (defined(RW_LONG_LONG) || __INTSIZE==4) && !__STDC__
	istream& operator>> (long long&);
#endif
	istream& operator>> (unsigned short&);
	istream& operator>> (unsigned int&);
	istream& operator>> (unsigned long&);
#if (defined(RW_LONG_LONG) || __INTSIZE==4) && !__STDC__
	istream& operator>> (unsigned long long&);
#endif
	istream& operator>> (float&);
	istream& operator>> (double&);
#if !__STDC__
	istream& operator>> (long double&);
#endif

	// extract from this istream, insert into streambuf
	istream& operator>> (streambuf*);

protected:
		istream();
	int     do_ipfx(int);   // implementation of ipfx
	void    eatwhite();     // extract consecutive whitespace

private:
	int     gcount_;        // chars extracted by last unformatted operation
	char    do_get();       // implementation of get
	void    floatstr(char*);
	char    intstr(char*, int);
};
inline int      istream::gcount() { return gcount_; }
inline int      istream::ipfx(int _need) {
			if( _need ? (ispecial & ~skipping) : ispecial )
				return do_ipfx(_need);
			return 1;
		}
inline int      istream::ipfx0() { return ispecial ? do_ipfx(0) : 1; }
inline int      istream::ipfx1() {
			return (ispecial & ~skipping) ? do_ipfx(1) : 1;
		}
inline istream& istream::operator>> (unsigned char& _c) {
			if( ipfx0() )
				_c = bp->in_avail() ? bp->sbumpc() : do_get();
			return *this;
		}
inline istream& istream::operator>> (char& _c) {
			if( ipfx0() )
				_c = bp->in_avail() ? bp->sbumpc() : do_get();
			return *this;
		}
inline istream& istream::operator>> (unsigned char* _p) {
			return *this >> (char *)_p;
		}
inline int      istream::sync() { return bp->sync(); }
inline istream& istream::operator>> (istream& (*_f) (istream&)) {
			return (*_f)(*this);
		}
inline istream& istream::get(unsigned char& _c) {
			if( ipfx1() ) {
				if( bp->in_avail() ) _c = bp->sbumpc();
				else _c = do_get();
				gcount_ = 1;
			}
			return *this;
		}
inline istream& istream::get(char& _c) {
			if( ipfx1() ) {
				if( bp->in_avail()) _c = bp->sbumpc();
				else _c = do_get();
				gcount_ = 1;
			}
			return *this;
		}
inline int      istream::get() {
			if( ipfx1() ) {
				int _c = bp->sbumpc();
				if( _c == EOF ) setstate(eofbit);
				else gcount_ = 1;
				return _c;
			}
			else return EOF;
		}
inline int      istream::peek() { return ipfx1() ? bp->sgetc() : EOF; }


class ostream : virtual public ios {
public:
	// constructors and destructor
		ostream(streambuf*);
virtual         ~ostream();
	// Obsolete constructors, for streams 1.2 compatibility
		ostream(int _fd); // obsolete, use fstream
		ostream(int _sz, char*); // obsolete, use strstream

	int     opfx();         // output prefix function
	void    osfx();         // output suffix function
	ostream& flush();

	// set/read the put pointer's position
	ostream& seekp(streampos);
	ostream& seekp(streamoff, ios::seek_dir);
	streampos tellp();

	/*
	 * Unformatted insertion operations
	 */
	ostream& put(char);     // insert the character
	ostream& write(const char*, int); // insert the string

	/*
	 * Formatted insertion operations
	 */
	// insert the character
	ostream& operator<< (char);
	ostream& operator<< (unsigned char);

	// for the following, insert character representation of numeric value
	ostream& operator<< (short);
	ostream& operator<< (unsigned short);
	ostream& operator<< (int);
	ostream& operator<< (unsigned int);
	ostream& operator<< (long);
	ostream& operator<< (unsigned long);
#if (defined(RW_LONG_LONG) || __INTSIZE==4) && !__STDC__
	ostream& operator<< (long long);
	ostream& operator<< (unsigned long long);
#endif
	ostream& operator<< (float);
	ostream& operator<< (double);
#if !__STDC__
	ostream& operator<< (long double);
#endif
	// insert the null-terminated string
	ostream& operator<< (const char*);

	// insert character representation of the value of the pointer
	ostream& operator<< (void*);

	// extract from streambuf, insert into this ostream
	ostream& operator<< (streambuf*);

	// manipulators
	ostream& operator<< (ostream& (*_f)(ostream&));
	ostream& operator<< (ios& (*_f)(ios&));

protected:
	int     do_opfx();      // implementation of opfx
	void    do_osfx();      // implementation of osfx
		ostream();

private:
	void    outstr(const char*, const char*);
	void    printint(const char*);
};
inline int      ostream::opfx() { return ospecial ? do_opfx() : 1; }
inline void     ostream::osfx() { if( x_flags & (stdio | unitbuf) ) do_osfx(); }
inline ostream& ostream::operator<< (char _c) {
			if( opfx() )
				if( bp->sputc(_c) == EOF ) setstate(badbit);
			osfx();
			return *this;
		}
inline ostream& ostream::operator<< (unsigned char _c) {
			return *this << (char)_c;
		}
inline ostream& ostream::operator<< (const char* _s) {
			outstr(_s, 0);
			return *this;
		}
inline ostream& ostream::operator<< (short _i) { return *this << (long) _i; }
inline ostream& ostream::operator<< (unsigned short _i) {
			return *this << (unsigned long) _i;
		}
inline ostream& ostream::operator<< (int _i) { return *this << (long) _i; }
inline ostream& ostream::operator<< (unsigned int _i) {
			return *this << (unsigned long) _i;
		}
inline ostream& ostream::operator<< (float _f) { return *this << (double) _f; }
inline ostream& ostream::operator<< (ostream& (*_f)(ostream&)) {
			return (*_f)(*this);
		}
inline ostream& ostream::put(char _c) {
			if( bp->sputc(_c) == EOF ) setstate(badbit);
			return *this;
		}
inline ostream& ostream::write(const char* _s, int _n) {
			if( ! fail() )
				if( bp->sputn(_s, _n) != _n )
					setstate(badbit);
			return *this;
		}


class iostream : public istream, public ostream {
public:
		iostream(streambuf*);
virtual         ~iostream();

protected:
		iostream();
};


class istream_withassign : public istream {
public:
		// does no initialization
		istream_withassign();

virtual         ~istream_withassign();

	// gets buffer from istream and does entire initialization
	istream_withassign& operator= (istream&);

	// associates streambuf with stream and does entire initialization
	istream_withassign& operator= (streambuf*);
};


class ostream_withassign : public ostream {
public:
		// does no initialization
		ostream_withassign();

virtual         ~ostream_withassign();

	// gets buffer from istream and does entire initialization
	ostream_withassign& operator= (ostream&);
   ostream_withassign& operator= (ostream_withassign&);

	// associates streambuf with stream and does entire initialization
	ostream_withassign& operator= (streambuf*);
};


class iostream_withassign : public iostream {
public:
		// does no initialization
		iostream_withassign();

virtual         ~iostream_withassign();

	// gets buffer from stream and does entire initialization
	iostream_withassign& operator= (ios&);

	// associates streambuf with stream and does entire initialization
	iostream_withassign& operator= (streambuf*);
};


/*
 * The predefined streams
 */
extern istream_withassign cin;
extern ostream_withassign cout;
extern ostream_withassign cerr;
extern ostream_withassign clog;

/*
 * Manipulators
 */
ostream& endl(ostream&);        // insert newline and flush
ostream& ends(ostream&);        // insert null to terminate string
ostream& flush(ostream&);       // flush the ostream
ios&     dec(ios&);             // set conversion base to decimal
ios&     hex(ios&);             // set conversion base to hexidecimal
ios&     oct(ios&);             // set conversion base to octal
istream& ws(istream&);          // extract whitespace characters

/*
 * The following allows proper initialization of cin, cout, cerr, clog
 */
class Iostream_init {
public:
	Iostream_init();
	~Iostream_init();

private:
    static int  stdstatus;
    static int  initcount;
    friend      ios;
};

#pragma pack()

#endif /* _IOSTREAM_H_ */
